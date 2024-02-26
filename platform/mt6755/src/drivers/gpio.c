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
//#include <pmic_wrap_init.h>
//autogen
#include <gpio_cfg.h>

//#include <cust_power.h>
/******************************************************************************
 MACRO Definition
******************************************************************************/
//#define  GIO_SLFTEST
#define GPIO_BRINGUP
#define GPIO_DEVICE "mt-gpio"
#define VERSION     "$Revision$"
/*---------------------------------------------------------------------------*/
#define GPIO_WR32(addr, data)   __raw_writel(data, addr)
#define GPIO_RD32(addr)         __raw_readl(addr)
#define GPIO_SW_SET_BITS(BIT,REG)   GPIO_WR32(REG,GPIO_RD32(REG) | ((unsigned long)(BIT)))
#define GPIO_SET_BITS(BIT,REG)   ((*(volatile u32*)(REG)) = (u32)(BIT))
#define GPIO_CLR_BITS(BIT,REG)   ((*(volatile u32*)(REG)) &= ~((u32)(BIT)))
//S32 pwrap_read( U32  adr, U32 *rdata ){return 0;}
//S32 pwrap_write( U32  adr, U32  wdata ){return 0;}

/*---------------------------------------------------------------------------*/
#define TRUE                   1
#define FALSE                  0
/*---------------------------------------------------------------------------*/
//#define MAX_GPIO_REG_BITS      16
//#define MAX_GPIO_MODE_PER_REG  5
//#define GPIO_MODE_BITS         3
/*---------------------------------------------------------------------------*/
#define GPIOTAG                "[GPIO] "
#define GPIOLOG(fmt, arg...)   //printf(GPIOTAG fmt, ##arg)
#define GPIOMSG(fmt, arg...)   //printf(fmt, ##arg)
#define GPIOERR(fmt, arg...)   //printf(GPIOTAG "%5d: "fmt, __LINE__, ##arg)
#define GPIOFUC(fmt, arg...)   //printf(GPIOTAG "%s\n", __FUNCTION__)
#define GIO_INVALID_OBJ(ptr)   ((ptr) != gpio_obj)
/******************************************************************************
Enumeration/Structure
******************************************************************************/
#if (CFG_FPGA_PLATFORM)
		s32 mt_set_gpio_dir(u32 pin, u32 dir)			{return RSUCCESS;}
		s32 mt_get_gpio_dir(u32 pin)				{return GPIO_DIR_UNSUPPORTED;}
		s32 mt_set_gpio_pull_enable(u32 pin, u32 enable)	{return RSUCCESS;}
		s32 mt_get_gpio_pull_enable(u32 pin)			{return GPIO_PULL_EN_UNSUPPORTED;}
		s32 mt_set_gpio_pull_select(u32 pin, u32 select)	{return RSUCCESS;}
		s32 mt_get_gpio_pull_select(u32 pin)			{return GPIO_PULL_UNSUPPORTED;}
		s32 mt_set_gpio_smt(u32 pin, u32 enable)		{return RSUCCESS;}
		s32 mt_get_gpio_smt(u32 pin)				{return GPIO_SMT_UNSUPPORTED;}
		s32 mt_set_gpio_ies(u32 pin, u32 enable)		{return RSUCCESS;}
		s32 mt_get_gpio_ies(u32 pin)				{return GPIO_IES_UNSUPPORTED;}
		s32 mt_set_gpio_out(u32 pin, u32 output)		{return RSUCCESS;}
		s32 mt_get_gpio_out(u32 pin)				{return GPIO_OUT_UNSUPPORTED;}
		s32 mt_get_gpio_in(u32 pin) 				{return GPIO_IN_UNSUPPORTED;}
		s32 mt_set_gpio_mode(u32 pin, u32 mode) 		{return RSUCCESS;}
		s32 mt_get_gpio_mode(u32 pin)				{return GPIO_MODE_UNSUPPORTED;}
#else
struct mt_gpio_obj {
    GPIO_REGS       *reg;
};
static struct mt_gpio_obj gpio_dat = {
    .reg  = (GPIO_REGS*)(GPIO_BASE),
};
static struct mt_gpio_obj *gpio_obj = &gpio_dat;
#define REGSET (4)
#define REGCLR (8)

/*---------------------------------------------------------------------------*/
#ifdef GPIO_BRINGUP
s32 mt_set_gpio_dir_chip(u32 pin, u32 dir)
{
    u32 bit;
    u32 reg;

    if (pin >= MAX_GPIO_PIN)
        return -ERINVAL;

    if (dir >= GPIO_DIR_MAX)
        return -ERINVAL;

    bit = DIR_offset[pin].offset;
    reg = GPIO_RD32(DIR_addr[pin].addr);
	
    if (dir == GPIO_DIR_IN)
		   reg &= (~(1<<bit));
    else
		   reg |= (1 << bit);
	
    GPIO_WR32(DIR_addr[pin].addr,reg);

    return RSUCCESS;
}
#else
s32 mt_set_gpio_dir_chip(u32 pin, u32 dir)
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

    if (dir == GPIO_DIR_IN)
        GPIO_SET_BITS((1L << bit), &obj->reg->dir[pos].rst);
    else
        GPIO_SET_BITS((1L << bit), &obj->reg->dir[pos].set);
    return RSUCCESS;

}
#endif
/*---------------------------------------------------------------------------*/
#ifdef GPIO_BRINGUP
s32 mt_get_gpio_dir_chip(u32 pin)
{
    u32 bit;
    u32 reg;


    if (pin >= MAX_GPIO_PIN)
        return -ERINVAL;

    bit = DIR_offset[pin].offset;
	
    reg = GPIO_RD32(DIR_addr[pin].addr);
	
    return (((reg & (1L << bit)) != 0)? 1: 0);
}
#else
s32 mt_get_gpio_dir_chip(u32 pin)
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

    reg = GPIO_RD32(&obj->reg->dir[pos].val);
    return (((reg & (1L << bit)) != 0)? 1: 0);
}
#endif
/*---------------------------------------------------------------------------*/
s32 mt_set_gpio_pull_enable_chip(u32 pin, u32 enable)
{
    if (pin >= MAX_GPIO_PIN)
        return -ERINVAL;
    /*PUPD type and PULLEN type IO abstraction*/
    if(PULLEN_offset[pin].offset == -1){
	  return GPIO_PULL_EN_UNSUPPORTED;
    }
    else{
      if (enable == GPIO_PULL_DISABLE)
		GPIO_SET_BITS((1L << (PULLEN_offset[pin].offset)), PULLEN_addr[pin].addr + REGCLR);
	  else
		GPIO_SET_BITS((1L << (PULLEN_offset[pin].offset)), PULLEN_addr[pin].addr + REGSET);
    }

    return RSUCCESS;
}
/*---------------------------------------------------------------------------*/
s32 mt_get_gpio_pull_enable_chip(u32 pin)
{
    unsigned long data;

    if (pin >= MAX_GPIO_PIN)
        return -ERINVAL;

    if(PULLEN_offset[pin].offset == -1){
	  return GPIO_PULL_EN_UNSUPPORTED;
    }
    else{
	  data = GPIO_RD32(PULLEN_addr[pin].addr);

          return (((data & (1L << (PULLEN_offset[pin].offset))) != 0)? 1: 0);
    }
}
/*---------------------------------------------------------------------------*/
s32 mt_set_gpio_smt_chip(u32 pin, u32 enable)
{
    u32 reg=0;
    u32 bit=0;
	
    if (pin >= MAX_GPIO_PIN)
        return -ERINVAL;
        
	if(SMT_offset[pin].offset == -1){
		return GPIO_SMT_UNSUPPORTED;
	
	}
	else{
		bit = SMT_offset[pin].offset;
		reg = GPIO_RD32(SMT_addr[pin].addr);
	    if (enable == GPIO_SMT_DISABLE)
		    //reg &= (~(1<<bit));
		    GPIO_SET_BITS((1L << bit), SMT_addr[pin].addr + REGCLR);
	    else
		    //reg |= (1 << bit);
		    GPIO_SET_BITS((1L << bit), SMT_addr[pin].addr + REGSET);
	}
		
	//GPIO_WR32(SMT_addr[pin].addr, reg);

    return RSUCCESS;
}
/*---------------------------------------------------------------------------*/
s32 mt_get_gpio_smt_chip(u32 pin)
{
    unsigned long data;
    u32 bit =0;
    bit = SMT_offset[pin].offset;
    if (pin >= MAX_GPIO_PIN)
        return -ERINVAL;

    if(SMT_offset[pin].offset == -1){
	  return GPIO_SMT_UNSUPPORTED;
    }
    else{
	  data = GPIO_RD32(SMT_addr[pin].addr);
      return (((data & (1L << bit)) != 0)? 1: 0);
    }
}
/*---------------------------------------------------------------------------*/
s32 mt_set_gpio_ies_chip(u32 pin, u32 enable)
{
    //unsigned long flags;

    if (pin >= MAX_GPIO_PIN)
        return -ERINVAL;

    if(IES_offset[pin].offset == -1){
	  return GPIO_IES_UNSUPPORTED;
    }
    else{
    	  if (enable == GPIO_IES_DISABLE)
		GPIO_SET_BITS((1L << (IES_offset[pin].offset)), IES_addr[pin].addr + REGCLR);
	  else
		GPIO_SET_BITS((1L << (IES_offset[pin].offset)), IES_addr[pin].addr + REGSET);
    }

    return RSUCCESS;
}
/*---------------------------------------------------------------------------*/
s32 mt_get_gpio_ies_chip(u32 pin)
{
    unsigned long data;

    if (pin >= MAX_GPIO_PIN)
        return -ERINVAL;

    if(IES_offset[pin].offset == -1){
	  return GPIO_IES_UNSUPPORTED;
    }
    else{
	  data = GPIO_RD32(IES_addr[pin].addr);

          return (((data & (1L << (IES_offset[pin].offset))) != 0)? 1: 0);
    }
}
/*---------------------------------------------------------------------------*/
s32 mt_set_gpio_pull_select_chip(u32 pin, u32 select)
{
    //unsigned long flags;
	
    if (pin >= MAX_GPIO_PIN)
        return -ERINVAL;


    if((PULLSEL_offset[pin].offset == -1)&& (PUPD_offset[pin].offset == -1))
    {
	  return GPIO_PULL_UNSUPPORTED;
    }
	else if(PULLSEL_offset[pin].offset == -1){
	//spin_lock_irqsave(&mtk_gpio_lock, flags);
	/*  
     *  SIM1, SIM2 may need special care for resistance selection
     */
	  if (select == GPIO_PULL_DOWN){   
		GPIO_SET_BITS((1L << (PUPD_offset[pin].offset+2)), PUPD_addr[pin].addr + REGSET);
	  }else {
        GPIO_SET_BITS((1L << (PUPD_offset[pin].offset+2)), PUPD_addr[pin].addr + REGCLR);
	  }
	//spin_unlock_irqrestore(&mtk_gpio_lock, flags);
    }
    else{
	  if (select == GPIO_PULL_DOWN)
		GPIO_SET_BITS((1L << (PULLSEL_offset[pin].offset)), PULLSEL_addr[pin].addr + REGCLR);
	  else
		GPIO_SET_BITS((1L << (PULLSEL_offset[pin].offset)), PULLSEL_addr[pin].addr + REGSET);
    }

    return RSUCCESS;
}
/*---------------------------------------------------------------------------*/
s32 mt_get_gpio_pull_select_chip(u32 pin)
{
    unsigned long data;

    if (pin >= MAX_GPIO_PIN)
        return -ERINVAL;

    if((PULLSEL_offset[pin].offset == -1) && (PUPD_offset[pin].offset == -1))
    {
	  return GPIO_PULL_UNSUPPORTED;
    }else if(PULLSEL_offset[pin].offset == -1){
	  data = GPIO_RD32(PUPD_addr[pin].addr);

    	  return (((data & (1L << (PUPD_offset[pin].offset+2))) != 0)? 0: 1);
    }
    else{
	  data = GPIO_RD32(PULLSEL_addr[pin].addr);

    	  return (((data & (1L << (PULLSEL_offset[pin].offset))) != 0)? 1: 0);
    }
}
/*---------------------------------------------------------------------------*/
#ifdef GPIO_BRINGUP
s32 mt_set_gpio_out_chip(u32 pin, u32 output)
{
u32 pos;
    u32 bit;
	u32 reg=0;
    //struct mt_gpio_obj *obj = gpio_obj;

    if (pin >= MAX_GPIO_PIN)
        return -ERINVAL;

    if (output >= GPIO_OUT_MAX)
        return -ERINVAL;

    bit = DATAOUT_offset[pin].offset;

    reg = GPIO_RD32(DATAOUT_addr[pin].addr);
    if (output == GPIO_OUT_ZERO)
	reg &= (~(1<<bit));
    else
 	reg |= (1 << bit);
    
    GPIO_WR32(DATAOUT_addr[pin].addr,reg);

    return RSUCCESS;
} 
#else
s32 mt_set_gpio_out_chip(u32 pin, u32 output)
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

    if (output == GPIO_OUT_ZERO)
        GPIO_SET_BITS((1L << bit), &obj->reg->dout[pos].rst);
    else
        GPIO_SET_BITS((1L << bit), &obj->reg->dout[pos].set);
    return RSUCCESS;
}
#endif
/*---------------------------------------------------------------------------*/
#ifdef GPIO_BRINGUP
s32 mt_get_gpio_out_chip(u32 pin)
{
    
    u32 bit;
    u32 reg;


    if (pin >= MAX_GPIO_PIN)
        return -ERINVAL;

	bit = DATAOUT_offset[pin].offset;

    reg = GPIO_RD32(DATAOUT_addr[pin].addr);

    return (((reg & (1L << bit)) != 0)? 1: 0);
}
#else
s32 mt_get_gpio_out_chip(u32 pin)
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

    reg = GPIO_RD32(&obj->reg->dout[pos].val);
    return (((reg & (1L << bit)) != 0)? 1: 0);
}
#endif
/*---------------------------------------------------------------------------*/
s32 mt_get_gpio_in_chip(u32 pin)
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

    reg = GPIO_RD32(&obj->reg->din[pos].val);
    return (((reg & (1L << bit)) != 0)? 1: 0);
}
/*---------------------------------------------------------------------------*/
s32 mt_set_gpio_mode_chip(u32 pin, u32 mode)
{
    u32 pos;
    u32 bit;
    u32 reg;
    
    struct mt_gpio_obj *obj = gpio_obj;

    if (!obj)
        return -ERACCESS;

    if (pin >= MAX_GPIO_PIN)
        return -ERINVAL;

    if (mode >= GPIO_MODE_MAX)
        return -ERINVAL;
	
        
#ifdef MODE_MWR
    /*
     *  if MWR is supported, no need to read before write.
     *  MWR[3:0] has 4 bits not 3 bits ; additional 1 bit,MWR[3], is for update enable
     */
    reg = 0;
    pos = pin / MAX_GPIO_MODE_PER_REG;
    bit = pin % MAX_GPIO_MODE_PER_REG;

    reg = ((1L << (GPIO_MODE_BITS*bit)) << 3) | (mode << (GPIO_MODE_BITS*bit));

    GPIO_WR32(&obj->reg->mode[pos]._align1, reg);
    
#else
/* like Denali, there is no mwr register for simple register setting in Jade 
     * Need 1R+1W to set MODE registers
     */
    u32 mask = (1L << GPIO_MODE_BITS) - 1;
    bit = MODE_offset[pin].offset;
    mode = mode & mask;

    reg = GPIO_RD32(MODE_addr[pin].addr);

    reg &= (~(mask << bit));
    reg |= (mode << bit);
    GPIO_WR32( MODE_addr[pin].addr,reg);
    
    
#endif
    return RSUCCESS;
}
/*---------------------------------------------------------------------------*/
#ifndef MODE_MWR
s32 mt_get_gpio_mode_chip(u32 pin)
{

    u32 bit;
    u32 reg;
    u32 mask = (1L << GPIO_MODE_BITS) - 1;

    if (pin >= MAX_GPIO_PIN)
        return -ERINVAL;

	
	bit = MODE_offset[pin].offset;
	reg = GPIO_RD32(MODE_addr[pin].addr);
   	return ((reg >> bit) & mask);
}
#else
s32 mt_get_gpio_mode_chip(u32 pin)
{
    u32 pos;
    u32 bit;
    u32 reg;
    u32 mask = (1L << GPIO_MODE_BITS) - 1;
    struct mt_gpio_obj *obj = gpio_obj;

    if (!obj)
        return -ERACCESS;

    if (pin >= MAX_GPIO_PIN)
        return -ERINVAL;

	pos = pin / MAX_GPIO_MODE_PER_REG;
	bit = pin % MAX_GPIO_MODE_PER_REG;

	reg = GPIO_RD32(&obj->reg->mode[pos].val);

	return ((reg >> (GPIO_MODE_BITS*bit)) & mask);
}
#endif
/*---------------------------------------------------------------------------*/
void mt_gpio_pin_decrypt(u32 *cipher)
{
	//just for debug, find out who used pin number directly
	if((*cipher & (0x80000000)) == 0){
		GPIOERR("Pin %d decrypt warning! \n",*cipher);	
		//dump_stack();
		//return;
	}

	//GPIOERR("Pin magic number is %x\n",*cipher);
	*cipher &= ~(0x80000000);
	return;
}
//set GPIO function in fact
/*---------------------------------------------------------------------------*/
s32 mt_set_gpio_dir(u32 pin, u32 dir)
{
	mt_gpio_pin_decrypt(&pin);
    return mt_set_gpio_dir_chip(pin,dir);
}
/*---------------------------------------------------------------------------*/
s32 mt_get_gpio_dir(u32 pin)
{    
	mt_gpio_pin_decrypt(&pin);
    return mt_get_gpio_dir_chip(pin);
}
/*---------------------------------------------------------------------------*/
s32 mt_set_gpio_pull_enable(u32 pin, u32 enable)
{
	mt_gpio_pin_decrypt(&pin);
    return mt_set_gpio_pull_enable_chip(pin,enable);
}
/*---------------------------------------------------------------------------*/
s32 mt_get_gpio_pull_enable(u32 pin)
{
	mt_gpio_pin_decrypt(&pin);
    return mt_get_gpio_pull_enable_chip(pin);
}
/*---------------------------------------------------------------------------*/
s32 mt_set_gpio_pull_select(u32 pin, u32 select)
{
	mt_gpio_pin_decrypt(&pin);
    return mt_set_gpio_pull_select_chip(pin,select);
}
/*---------------------------------------------------------------------------*/
s32 mt_get_gpio_pull_select(u32 pin)
{
	mt_gpio_pin_decrypt(&pin);
    return mt_get_gpio_pull_select_chip(pin);
}
/*---------------------------------------------------------------------------*/
s32 mt_set_gpio_smt(u32 pin, u32 enable)
{
	mt_gpio_pin_decrypt(&pin);
    return mt_set_gpio_smt_chip(pin,enable);
}
/*---------------------------------------------------------------------------*/
s32 mt_get_gpio_smt(u32 pin)
{
	mt_gpio_pin_decrypt(&pin);
    return mt_get_gpio_smt_chip(pin);
}
/*---------------------------------------------------------------------------*/
s32 mt_set_gpio_ies(u32 pin, u32 enable)
{
	mt_gpio_pin_decrypt(&pin);
    return mt_set_gpio_ies_chip(pin,enable);
}
/*---------------------------------------------------------------------------*/
s32 mt_get_gpio_ies(u32 pin)
{
	mt_gpio_pin_decrypt(&pin);
    return mt_get_gpio_ies_chip(pin);
}
/*---------------------------------------------------------------------------*/
s32 mt_set_gpio_out(u32 pin, u32 output)
{
	mt_gpio_pin_decrypt(&pin);
    return mt_set_gpio_out_chip(pin,output);
}
/*---------------------------------------------------------------------------*/
s32 mt_get_gpio_out(u32 pin)
{
	mt_gpio_pin_decrypt(&pin);
    return mt_get_gpio_out_chip(pin);
}
/*---------------------------------------------------------------------------*/
s32 mt_get_gpio_in(u32 pin)
{
	mt_gpio_pin_decrypt(&pin);
    return mt_get_gpio_in_chip(pin);
}
/*---------------------------------------------------------------------------*/
s32 mt_set_gpio_mode(u32 pin, u32 mode)
{
	mt_gpio_pin_decrypt(&pin);
    return mt_set_gpio_mode_chip(pin,mode);
}
/*---------------------------------------------------------------------------*/
s32 mt_get_gpio_mode(u32 pin)
{
	mt_gpio_pin_decrypt(&pin);
    return mt_get_gpio_mode_chip(pin);
}
#endif

void mt_gpio_init(void)
{
#ifdef DUMMY_AP
	mt_gpio_set_default();
#endif

#ifdef TINY
	mt_gpio_set_default();
#endif
}
