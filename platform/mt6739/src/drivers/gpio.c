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
 * MediaTek Inc. (C) 2017. All rights reserved.
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
 * gpio.c - MTK Linux GPIO Device Driver
 *
 * Copyright 2008-2009 MediaTek Co.,Ltd.
 *
 * DESCRIPTION:
 *     This file provid the other drivers GPIO relative functions
 *
 ******************************************************************************/

#include <typedefs.h>
#include <gpio.h>
#include <platform.h>
#include <gpio_cfg.h>
/******************************************************************************
 MACRO Definition
******************************************************************************/
//#define  GIO_SLFTEST
#define GPIO_BRINGUP
#define GPIO_DEVICE "mt-gpio"
#define VERSION     "$Revision$"
/*---------------------------------------------------------------------------*/
#define SUPPORT_MODE_MWR
#define REGSET (4)
#define REGCLR (8)
#define REGMWR (0xC)
/*---------------------------------------------------------------------------*/
#define GPIO_WR32(addr, data)           DRV_WriteReg32(addr, data)
#define GPIO_RD32(addr)                 DRV_Reg32(addr)
#define GPIO_SET_BITS(REG, BIT)         GPIO_WR32(REG + REGSET, BIT)
#define GPIO_CLR_BITS(REG, BIT)         GPIO_WR32(REG + REGCLR, BIT)
/*---------------------------------------------------------------------------*/
#define TRUE                            1
#define FALSE                           0
/*---------------------------------------------------------------------------*/
#define GPIOTAG                         "[GPIO] "
#define GPIOLOG(fmt, arg...)            //printf(GPIOTAG fmt, ##arg)
#define GPIOMSG(fmt, arg...)            //printf(fmt, ##arg)
#define GPIOERR(fmt, arg...)            //printf(GPIOTAG "%5d: "fmt, __LINE__, ##arg)
#define GPIOFUC(fmt, arg...)            //printf(GPIOTAG "%s\n", __FUNCTION__)
#define GIO_INVALID_OBJ(ptr)            ((ptr) != gpio_obj)
/******************************************************************************
Enumeration/Structure
******************************************************************************/
#ifdef FPGA_PLATFORM
S32 mt_set_gpio_dir(u32 pin, u32 dir)           {return RSUCCESS;}
S32 mt_get_gpio_dir(u32 pin)                    {return GPIO_DIR_UNSUPPORTED;}
S32 mt_set_gpio_pull_enable(u32 pin, u32 en)    {return RSUCCESS;}
S32 mt_get_gpio_pull_enable(u32 pin)            {return GPIO_PULL_EN_UNSUPPORTED;}
S32 mt_set_gpio_pull_select(u32 pin, u32 sel)   {return RSUCCESS;}
S32 mt_get_gpio_pull_select(u32 pin)            {return GPIO_PULL_UNSUPPORTED;}
S32 mt_set_gpio_smt(u32 pin, u32 enable)        {return RSUCCESS;}
S32 mt_get_gpio_smt(u32 pin)                    {return GPIO_SMT_UNSUPPORTED;}
S32 mt_set_gpio_ies(u32 pin, u32 enable)        {return RSUCCESS;}
S32 mt_get_gpio_ies(u32 pin)                    {return GPIO_IES_UNSUPPORTED;}
S32 mt_set_gpio_out(u32 pin, u32 output)        {return RSUCCESS;}
S32 mt_get_gpio_out(u32 pin)                    {return GPIO_OUT_UNSUPPORTED;}
S32 mt_get_gpio_in(u32 pin)                     {return GPIO_IN_UNSUPPORTED;}
S32 mt_set_gpio_mode(u32 pin, u32 mode)         {return RSUCCESS;}
S32 mt_get_gpio_mode(u32 pin)                   {return GPIO_MODE_UNSUPPORTED;}
#else
struct mt_gpio_obj {
	GPIO_REGS       *reg;
};
static struct mt_gpio_obj gpio_dat = {
	.reg  = (GPIO_REGS*)(GPIO_BASE),
};
static struct mt_gpio_obj *gpio_obj = &gpio_dat;

/*---------------------------------------------------------------------------*/
S32 mt_set_gpio_dir_chip(u32 pin, u32 dir)
{
	u32 pos;
	u32 bit;
	struct mt_gpio_obj *obj = gpio_obj;

	if (!obj)
		return -ERACCESS;

	if (pin >= MAX_GPIO_PIN)
		return -ERINVAL;

	if (dir >= GPIO_DIR_MAX)
		return -ERINVAL;

	pos = pin / MAX_GPIO_REG_BITS;
	bit = pin % MAX_GPIO_REG_BITS;

	/* To reduce memory usage, we don't use DIR_addr[] array*/
	if (dir == GPIO_DIR_IN)
		GPIO_WR32(&obj->reg->dir[pos].rst, 1L << bit);
	else
		GPIO_WR32(&obj->reg->dir[pos].set, 1L << bit);

	return RSUCCESS;
}
/*---------------------------------------------------------------------------*/
S32 mt_get_gpio_dir_chip(u32 pin)
{
	u32 pos;
	u32 bit;
	u32 reg;
	struct mt_gpio_obj *obj = gpio_obj;

	if (!obj)
		return -ERACCESS;

	if (pin >= MAX_GPIO_PIN)
		return -ERINVAL;

	pos = pin / MAX_GPIO_REG_BITS;
	bit = pin % MAX_GPIO_REG_BITS;

	/* To reduce memory usage, we don't use DIR_addr[] array*/
	reg = GPIO_RD32(&obj->reg->dir[pos].val);
	return (((reg & (1L << bit)) != 0)? 1: 0);
}
/*---------------------------------------------------------------------------*/
S32 mt_set_gpio_out_chip(u32 pin, u32 output)
{
	u32 pos;
	u32 bit;
	struct mt_gpio_obj *obj = gpio_obj;

	if (!obj)
		return -ERACCESS;

	if (pin >= MAX_GPIO_PIN)
		return -ERINVAL;

	if (output >= GPIO_OUT_MAX)
		return -ERINVAL;

	pos = pin / MAX_GPIO_REG_BITS;
	bit = pin % MAX_GPIO_REG_BITS;

	/* To reduce memory usage, we don't use DATAOUT_addr[] array*/
	if (output == GPIO_OUT_ZERO)
		GPIO_WR32(&obj->reg->dout[pos].rst, 1L << bit);
	else
		GPIO_WR32(&obj->reg->dout[pos].set, 1L << bit);

	return RSUCCESS;
}
/*---------------------------------------------------------------------------*/
S32 mt_get_gpio_out_chip(u32 pin)
{
	u32 pos;
	u32 bit;
	u32 reg;
	struct mt_gpio_obj *obj = gpio_obj;

	if (!obj)
		return -ERACCESS;

	if (pin >= MAX_GPIO_PIN)
		return -ERINVAL;

	pos = pin / MAX_GPIO_REG_BITS;
	bit = pin % MAX_GPIO_REG_BITS;

	/* To reduce memory usage, we don't use DATAOUT_addr[] array*/
	reg = GPIO_RD32(&obj->reg->dout[pos].val);
	return (((reg & (1L << bit)) != 0)? 1: 0);
}
/*---------------------------------------------------------------------------*/
S32 mt_get_gpio_in_chip(u32 pin)
{
	u32 pos;
	u32 bit;
	u32 reg;
	struct mt_gpio_obj *obj = gpio_obj;

	if (!obj)
		return -ERACCESS;

	if (pin >= MAX_GPIO_PIN)
		return -ERINVAL;

	pos = pin / MAX_GPIO_REG_BITS;
	bit = pin % MAX_GPIO_REG_BITS;

	/* To reduce memory usage, we don't use DIN_addr[] array*/
	reg = GPIO_RD32(&obj->reg->din[pos].val);
	return (((reg & (1L << bit)) != 0)? 1: 0);
}
/*---------------------------------------------------------------------------*/
S32 mt_set_gpio_mode_chip(u32 pin, u32 mode)
{
	unsigned long pos;
	unsigned long bit;
	unsigned long data;
	struct mt_gpio_obj *obj = gpio_obj;
	u32 mask;

	if (!obj)
		return -ERACCESS;

	if (pin >= MAX_GPIO_PIN)
		return -ERINVAL;

	if (mode >= GPIO_MODE_MAX)
		return -ERINVAL;

#ifdef SUPPORT_MODE_MWR
	mask = (1L << (GPIO_MODE_BITS - 1)) - 1;
#else
	mask = (1L << GPIO_MODE_BITS) - 1;
#endif

	mode = mode & mask;
	pos = pin / MAX_GPIO_MODE_PER_REG;
	bit = (pin % MAX_GPIO_MODE_PER_REG) * GPIO_MODE_BITS;

#ifdef SUPPORT_MODE_MWR
	data = (1L << (bit + GPIO_MODE_BITS - 1)) | (mode << bit);
	GPIO_WR32(&obj->reg->mode[pos]._align1, data);
#else
	data = GPIO_RD32(&reg->mode[pos].val);
	data &= (~(mask << bit));
	data |= (mode << bit);
	GPIO_WR32(&obj->reg->mode[pos].val, data);
#endif

	return RSUCCESS;
}
/*---------------------------------------------------------------------------*/
S32 mt_get_gpio_mode_chip(u32 pin)
{
	unsigned long pos;
	unsigned long bit;
	unsigned long data;
	u32 mask;
	struct mt_gpio_obj *obj = gpio_obj;

	if (!obj)
		return -ERACCESS;

	if (pin >= MAX_GPIO_PIN)
		return -ERINVAL;

#ifdef SUPPORT_MODE_MWR
	mask = (1L << (GPIO_MODE_BITS - 1)) - 1;
#else
	mask = (1L << GPIO_MODE_BITS) - 1;
#endif

	pos = pin / MAX_GPIO_MODE_PER_REG;
	bit = (pin % MAX_GPIO_MODE_PER_REG) * GPIO_MODE_BITS;

	data = GPIO_RD32(&obj->reg->mode[pos].val);
	return (data >> bit) & mask;
}
/*---------------------------------------------------------------------------*/
S32 mt_set_gpio_smt_chip(u32 pin, u32 enable)
{
	if (pin >= MAX_GPIO_PIN)
		return -ERINVAL;

	if (SMT_offset[pin].offset == (S8)-1)
		return GPIO_SMT_UNSUPPORTED;

	if (enable == GPIO_SMT_DISABLE)
		GPIO_CLR_BITS(SMT_addr[pin].addr, 1L << SMT_offset[pin].offset);
	else
		GPIO_SET_BITS(SMT_addr[pin].addr, 1L << SMT_offset[pin].offset);

	return RSUCCESS;
}
/*---------------------------------------------------------------------------*/
S32 mt_get_gpio_smt_chip(u32 pin)
{
	u32 reg;

	if (pin >= MAX_GPIO_PIN)
		return -ERINVAL;

	if (SMT_offset[pin].offset == (S8)-1)
		return GPIO_SMT_UNSUPPORTED;

	reg = GPIO_RD32(SMT_addr[pin].addr);
	return ((reg & (1L << SMT_offset[pin].offset)) ? 1 : 0);
}
/*---------------------------------------------------------------------------*/
S32 mt_set_gpio_ies_chip(u32 pin, u32 enable)
{
	if (pin >= MAX_GPIO_PIN)
		return -ERINVAL;

	if (IES_offset[pin].offset == (S8)-1)
		return GPIO_IES_UNSUPPORTED;

	if (enable == GPIO_IES_DISABLE)
		GPIO_CLR_BITS(IES_addr[pin].addr, 1L << IES_offset[pin].offset);
	else
		GPIO_SET_BITS(IES_addr[pin].addr, 1L << IES_offset[pin].offset);

	return RSUCCESS;
}
/*---------------------------------------------------------------------------*/
S32 mt_get_gpio_ies_chip(u32 pin)
{
	u32 reg;

	if (pin >= MAX_GPIO_PIN)
		return -ERINVAL;

	if (IES_offset[pin].offset == (S8)-1)
		return GPIO_IES_UNSUPPORTED;

	reg = GPIO_RD32(IES_addr[pin].addr);
	return ((reg & (1L << IES_offset[pin].offset)) ? 1 : 0);
}
/*---------------------------------------------------------------------------*/
S32 mt_set_gpio_pull_enable_chip(u32 pin, u32 en)
{
	if (pin >= MAX_GPIO_PIN)
		return -ERINVAL;

	if ((PU_offset[pin].offset == (S8)-1) && (PUPD_offset[pin].offset == (S8)-1))
		return GPIO_PULL_EN_UNSUPPORTED;

	if (en == GPIO_PULL_DISABLE) {
		if (PU_offset[pin].offset == (S8)-1) {
			GPIO_CLR_BITS(R0_addr[pin].addr, 1L << R0_offset[pin].offset);
			GPIO_CLR_BITS(R1_addr[pin].addr, 1L << R1_offset[pin].offset);
		} else {
			GPIO_CLR_BITS(PU_addr[pin].addr, 1L << PU_offset[pin].offset);
			GPIO_CLR_BITS(PD_addr[pin].addr, 1L << PD_offset[pin].offset);
		}
	} else if (en == GPIO_PULL_ENABLE) {
		if (PU_offset[pin].offset == (S8)-1) {
			/* For PUPD+R0+R1 Type, mt_set_gpio_pull_enable does not know
			 * which one between PU and PD shall be enabled.
			 * Use R0 to guarantee at one resistor is set when lk
			 * apply default setting
			 */
			GPIO_SET_BITS(R0_addr[pin].addr, 1L << R0_offset[pin].offset);
			GPIO_CLR_BITS(R1_addr[pin].addr, 1L << R1_offset[pin].offset);
		} else {
			/* For PU + PD Ttype, mt_set_gpio_pull_enable does not know
			 * which one between PU and PD shall be enabled.
			 * Use mt_set_gpio_pull_select() or GPIO_PULL_ENABLE_Rx instead.
			 */
			 return GPIO_PULL_EN_UNSUPPORTED;
		}
	} else if (en == GPIO_PULL_ENABLE_R0) {
		if (PUPD_offset[pin].offset == (S8)-1)
			return GPIO_PULL_EN_UNSUPPORTED;
		/* IOConfig does not support MWR operation, so use CLR + SET */
		GPIO_SET_BITS(R0_addr[pin].addr, 1L << R0_offset[pin].offset);
		GPIO_CLR_BITS(R1_addr[pin].addr, 1L << R1_offset[pin].offset);
	} else if (en == GPIO_PULL_ENABLE_R1) {
		if (PUPD_offset[pin].offset == (S8)-1)
			return GPIO_PULL_EN_UNSUPPORTED;
		/* IOConfig does not support MWR operation, so use CLR + SET */
		GPIO_CLR_BITS(R0_addr[pin].addr, 1L << R0_offset[pin].offset);
		GPIO_SET_BITS(R1_addr[pin].addr, 1L << R1_offset[pin].offset);
	} else if (en == GPIO_PULL_ENABLE_R0R1) {
		if (PUPD_offset[pin].offset == (S8)-1)
			return GPIO_PULL_EN_UNSUPPORTED;
		GPIO_SET_BITS(R0_addr[pin].addr, 1L << R0_offset[pin].offset);
		GPIO_SET_BITS(R1_addr[pin].addr, 1L << R1_offset[pin].offset);
	} else {
		return -ERINVAL;
	}

	return RSUCCESS;
}
/*---------------------------------------------------------------------------*/
S32 mt_get_gpio_pull_enable_chip(u32 pin)
{
	u32 reg, reg2;

	if (pin >= MAX_GPIO_PIN)
		return -ERINVAL;

	if ((PU_offset[pin].offset == (S8)-1) && (PUPD_offset[pin].offset == (S8)-1))
		return GPIO_PULL_EN_UNSUPPORTED;

	if (PU_offset[pin].offset == (S8)-1) {
		reg = (GPIO_RD32(R0_addr[pin].addr) >> R0_offset[pin].offset) & 1;
		reg2 = (GPIO_RD32(R1_addr[pin].addr) >> R1_offset[pin].offset) & 1;
		return ((reg | reg2) ? 1 : 0);
	} else if (PUPD_offset[pin].offset == (S8)-1) {
		reg = (GPIO_RD32(PU_addr[pin].addr) >> PU_offset[pin].offset) & 1;
		reg2 = (GPIO_RD32(PD_addr[pin].addr) >> PD_offset[pin].offset) & 1;
		return ((reg | reg2) ? 1 : 0);
	}

	return -ERINVAL;
}
/*---------------------------------------------------------------------------*/
S32 mt_set_gpio_pull_select_chip(u32 pin, u32 sel)
{
	if (pin >= MAX_GPIO_PIN)
		return -ERINVAL;

	if ((PU_offset[pin].offset == (S8)-1) && (PUPD_offset[pin].offset == (S8)-1))
		return GPIO_PULL_UNSUPPORTED;

	if (sel == GPIO_NO_PULL) {
		mt_set_gpio_pull_enable_chip(pin, GPIO_PULL_DISABLE);
		if (PU_offset[pin].offset == (S8)-1) {
			GPIO_SET_BITS(PUPD_addr[pin].addr, 1L << PUPD_offset[pin].offset);
		} else {
			/* do nothting since already modified in mt_set_gpio_pull_enable_chip */
		}
	} else if (sel == GPIO_PULL_UP) {
		if (PU_offset[pin].offset == (S8)-1)
			GPIO_CLR_BITS(PUPD_addr[pin].addr, 1L << PUPD_offset[pin].offset);
		else {
			GPIO_SET_BITS(PU_addr[pin].addr, 1L << PU_offset[pin].offset);
			GPIO_CLR_BITS(PD_addr[pin].addr, 1L << PD_offset[pin].offset);
		}
	} else if (sel == GPIO_PULL_DOWN) {
		if (PU_offset[pin].offset == (S8)-1)
			GPIO_SET_BITS(PUPD_addr[pin].addr, 1L << PUPD_offset[pin].offset);
		else {
			GPIO_CLR_BITS(PU_addr[pin].addr, 1L << PU_offset[pin].offset);
			GPIO_SET_BITS(PD_addr[pin].addr, 1L << PD_offset[pin].offset);
		}
	} else {
		return -ERINVAL;
	}

	return RSUCCESS;
}
/*---------------------------------------------------------------------------*/
/* get pull-up or pull-down, regardless of resistor value */
S32 mt_get_gpio_pull_select_chip(u32 pin)
{
	u32 reg, reg2;

	if (pin >= MAX_GPIO_PIN)
		return -ERINVAL;

	if ((PU_offset[pin].offset == (S8)-1) && (PUPD_offset[pin].offset == (S8)-1))
		return GPIO_PULL_UNSUPPORTED;

	if (PU_offset[pin].offset == (S8)-1) {
		reg = GPIO_RD32(PUPD_addr[pin].addr);
		/* Reg value: 0 for PU, 1 for PD --> reverse return value */
		return (S32)((reg & (1L << PUPD_offset[pin].offset)) ? GPIO_PULL_DOWN : GPIO_PULL_UP);
	} else if (PUPD_offset[pin].offset == (S8)-1) {
		reg = (GPIO_RD32(PU_addr[pin].addr) >> PU_offset[pin].offset) & 1;
		reg2 = (GPIO_RD32(PD_addr[pin].addr) >> PD_offset[pin].offset) & 1;
		if (reg)
			return GPIO_PULL_UP;
		else if (reg2)
			return GPIO_PULL_DOWN;
		else
			return GPIO_NO_PULL;
	}

	return -ERINVAL;
}
/*---------------------------------------------------------------------------*/
void mt_gpio_pin_decrypt(u32 *cipher)
{
	//just for debug, find out who used pin number directly
	if ((*cipher & (0x80000000)) == 0) {
		GPIOERR("Pin %d decrypt warning! \n",*cipher);
		//dump_stack();
		//return;
	}

	//GPIOERR("Pin magic number is %x\n",*cipher);
	*cipher &= ~(0x80000000);
	return;
}
/*---------------------------------------------------------------------------*/
S32 mt_set_gpio_dir(u32 pin, u32 dir)
{
	mt_gpio_pin_decrypt(&pin);
	return mt_set_gpio_dir_chip(pin,dir);
}
/*---------------------------------------------------------------------------*/
S32 mt_get_gpio_dir(u32 pin)
{
	mt_gpio_pin_decrypt(&pin);
	return mt_get_gpio_dir_chip(pin);
}
/*---------------------------------------------------------------------------*/
S32 mt_set_gpio_pull_enable(u32 pin, u32 enable)
{
	mt_gpio_pin_decrypt(&pin);
	return mt_set_gpio_pull_enable_chip(pin,enable);
}
/*---------------------------------------------------------------------------*/
S32 mt_get_gpio_pull_enable(u32 pin)
{
	mt_gpio_pin_decrypt(&pin);
	return mt_get_gpio_pull_enable_chip(pin);
}
/*---------------------------------------------------------------------------*/
S32 mt_set_gpio_pull_select(u32 pin, u32 select)
{
	mt_gpio_pin_decrypt(&pin);
	return mt_set_gpio_pull_select_chip(pin,select);
}
/*---------------------------------------------------------------------------*/
S32 mt_get_gpio_pull_select(u32 pin)
{
	mt_gpio_pin_decrypt(&pin);
	return mt_get_gpio_pull_select_chip(pin);
}
/*---------------------------------------------------------------------------*/
S32 mt_set_gpio_smt(u32 pin, u32 enable)
{
	mt_gpio_pin_decrypt(&pin);
	return mt_set_gpio_smt_chip(pin,enable);
}
/*---------------------------------------------------------------------------*/
S32 mt_get_gpio_smt(u32 pin)
{
	mt_gpio_pin_decrypt(&pin);
	return mt_get_gpio_smt_chip(pin);
}
/*---------------------------------------------------------------------------*/
S32 mt_set_gpio_ies(u32 pin, u32 enable)
{
	mt_gpio_pin_decrypt(&pin);
	return mt_set_gpio_ies_chip(pin,enable);
}
/*---------------------------------------------------------------------------*/
S32 mt_get_gpio_ies(u32 pin)
{
	mt_gpio_pin_decrypt(&pin);
	return mt_get_gpio_ies_chip(pin);
}
/*---------------------------------------------------------------------------*/
S32 mt_set_gpio_out(u32 pin, u32 output)
{
	mt_gpio_pin_decrypt(&pin);
	return mt_set_gpio_out_chip(pin,output);
}
/*---------------------------------------------------------------------------*/
S32 mt_get_gpio_out(u32 pin)
{
	mt_gpio_pin_decrypt(&pin);
	return mt_get_gpio_out_chip(pin);
}
/*---------------------------------------------------------------------------*/
S32 mt_get_gpio_in(u32 pin)
{
	mt_gpio_pin_decrypt(&pin);
	return mt_get_gpio_in_chip(pin);
}
/*---------------------------------------------------------------------------*/
S32 mt_set_gpio_mode(u32 pin, u32 mode)
{
	mt_gpio_pin_decrypt(&pin);
	return mt_set_gpio_mode_chip(pin,mode);
}
/*---------------------------------------------------------------------------*/
S32 mt_get_gpio_mode(u32 pin)
{
	mt_gpio_pin_decrypt(&pin);
	return mt_get_gpio_mode_chip(pin);
}
#endif

void mt_gpio_init(void)
{
#ifdef TINY
	mt_gpio_set_default();
#endif
}
