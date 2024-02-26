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
#ifndef _GPIO_H_
#define _GPIO_H_

#if (CFG_FPGA_PLATFORM)
#define FPGA_PLATFORM
#endif

#include <typedefs.h>
#include <cust_gpio_usage.h>
#include <gpio_const.h>

/*----------------------------------------------------------------------------*/
//  Error Code No.
#define RSUCCESS        0
#define ERACCESS        1
#define ERINVAL         2
#define ERWRAPPER       3
/*----------------------------------------------------------------------------*/
#define MAX_GPIO_PIN    (MT_GPIO_BASE_MAX)

//#define FPGA_SIMULATION
//#define SELF_TEST

#ifdef FPGA_SIMULATION
#define GPIO_BASE                      0x452D0000
#define IO_CFG_RB_BASE                 0x11C10000
#define IO_CFG_BR_BASE                 0x11D30000
#define IO_CFG_BL_BASE                 0x11D40000
#define IO_CFG_BM_BASE                 0x11D50000
#define IO_CFG_LB_BASE                 0x11E60000
#define IO_CFG_LT_BASE                 0x11E10000
#define IO_CFG_TR_BASE                 0x11F00000
#define MIPI_TX0_BASE                  0x45005000
#define MIPI_TX1_BASE                  0x4500E000
#define MIPI_RX_ANA_BASE               0x45217000
#endif
/******************************************************************************
* Enumeration for GPIO pin
******************************************************************************/
/* GPIO MODE CONTROL VALUE*/
typedef enum {
	GPIO_MODE_UNSUPPORTED = -1,
	GPIO_MODE_GPIO  = 0,
	GPIO_MODE_00    = 0,
	GPIO_MODE_01    = 1,
	GPIO_MODE_02    = 2,
	GPIO_MODE_03    = 3,
	GPIO_MODE_04    = 4,
	GPIO_MODE_05    = 5,
	GPIO_MODE_06    = 6,
	GPIO_MODE_07    = 7,

	GPIO_MODE_MAX,
	GPIO_MODE_DEFAULT = GPIO_MODE_01,
} GPIO_MODE;
/*----------------------------------------------------------------------------*/
/* GPIO DIRECTION */
typedef enum {
	GPIO_DIR_UNSUPPORTED = -1,
	GPIO_DIR_IN     = 0,
	GPIO_DIR_OUT    = 1,

	GPIO_DIR_MAX,
	GPIO_DIR_DEFAULT = GPIO_DIR_IN,
} GPIO_DIR;
/*----------------------------------------------------------------------------*/
/* GPIO PULL ENABLE*/
typedef enum {
	GPIO_PULL_EN_UNSUPPORTED = -1,
	GPIO_PULL_DISABLE   = 0,
	GPIO_PULL_ENABLE    = 1,
	GPIO_PULL_ENABLE_R0 = 2,
	GPIO_PULL_ENABLE_R1 = 3,
	GPIO_PULL_ENABLE_R0R1 = 4,

	GPIO_PULL_EN_MAX,
	GPIO_PULL_EN_DEFAULT = GPIO_PULL_ENABLE,
} GPIO_PULL_EN;
/*----------------------------------------------------------------------------*/
/* GPIO SMT*/
typedef enum {
	GPIO_SMT_UNSUPPORTED = -1,
	GPIO_SMT_DISABLE = 0,
	GPIO_SMT_ENABLE  = 1,

	GPIO_SMT_MAX,
	GPIO_SMT_DEFAULT = GPIO_SMT_ENABLE,
} GPIO_SMT;
/*----------------------------------------------------------------------------*/
/* GPIO IES*/
typedef enum {
	GPIO_IES_UNSUPPORTED = -1,
	GPIO_IES_DISABLE = 0,
	GPIO_IES_ENABLE  = 1,

	GPIO_IES_MAX,
	GPIO_IES_DEFAULT = GPIO_IES_ENABLE,
} GPIO_IES;
/*----------------------------------------------------------------------------*/
/* GPIO DRIVING*/
typedef enum {
	GPIO_DRV_UNSUPPORTED = -1,
	GPIO_DRV0   = 0x00,
	GPIO_DRV1   = 0x01,
	GPIO_DRV2   = 0x10,
	GPIO_DRV3   = 0x11,
	GPIO_DRV4   = 0x100,
	GPIO_DRV5   = 0x101,
	GPIO_DRV6   = 0x110,
	GPIO_DRV7   = 0x111,
} GPIO_DRV;
/*----------------------------------------------------------------------------*/
/* GPIO PULL-UP/PULL-DOWN*/
typedef enum {
	GPIO_PULL_UNSUPPORTED = -1,
	GPIO_PULL_DOWN  = 0,
	GPIO_PULL_UP    = 1,
	GPIO_NO_PULL    = 2,

	GPIO_PULL_MAX,
	GPIO_PULL_DEFAULT = GPIO_PULL_DOWN
} GPIO_PULL;
/*----------------------------------------------------------------------------*/
/* GPIO OUTPUT */
typedef enum {
	GPIO_OUT_UNSUPPORTED = -1,
	GPIO_OUT_ZERO = 0,
	GPIO_OUT_ONE  = 1,

	GPIO_OUT_MAX,
	GPIO_OUT_DEFAULT = GPIO_OUT_ZERO,
	GPIO_DATA_OUT_DEFAULT = GPIO_OUT_ZERO,  /*compatible with DCT*/
} GPIO_OUT;
/*----------------------------------------------------------------------------*/
/* GPIO INPUT */
typedef enum {
	GPIO_IN_UNSUPPORTED = -1,
	GPIO_IN_ZERO = 0,
	GPIO_IN_ONE  = 1,

	GPIO_IN_MAX,
} GPIO_IN;
/*----------------------------------------------------------------------------*/
/* GPIO POWER*/
typedef enum {
	GPIO_VIO28 = 0,
	GPIO_VIO18 = 1,
	GPIO_GPIO_VIO18 = 2,
	GPIO_MSDC_VMC = 3,
	GPIO_VIO_MAX,
} GPIO_POWER;
/*----------------------------------------------------------------------------*/
typedef struct {
	unsigned int val;
	unsigned int set;
	unsigned int rst;
	unsigned int _align1;
} VAL_REGS;
/*----------------------------------------------------------------------------*/
typedef struct {
	VAL_REGS dir[6];        /*0x0000 ~ 0x005F:  96 bytes */
	u8 rsv00[160];          /*0x0060 ~ 0x00FF: 160 bytes */
	VAL_REGS dout[6];       /*0x0100 ~ 0x015F:  96 bytes */
	u8 rsv01[160];          /*0x0160 ~ 0x01FF: 160 bytes */
	VAL_REGS din[6];        /*0x0200 ~ 0x025F:  96 bytes */
	u8 rsv02[160];          /*0x0260 ~ 0x02FF: 160 bytes */
	VAL_REGS mode[23];      /*0x0300 ~ 0x046F: 368 bytes */
} GPIO_REGS;
/*----------------------------------------------------------------------------*/
typedef struct {
	unsigned int no     : 16;
	unsigned int mode   : 3;
	unsigned int pullsel: 1;
	unsigned int din    : 1;
	unsigned int dout   : 1;
	unsigned int pullen : 1;
	unsigned int dir    : 1;
	unsigned int dinv   : 1;
	unsigned int _align : 7;
} GPIO_CFG;
/******************************************************************************
* GPIO Driver interface
******************************************************************************/
/*direction*/
S32 mt_set_gpio_dir(u32 pin, u32 dir);
S32 mt_get_gpio_dir(u32 pin);

/*pull enable*/
S32 mt_set_gpio_pull_enable(u32 pin, u32 en);
S32 mt_get_gpio_pull_enable(u32 pin);
/*pull select*/
S32 mt_set_gpio_pull_select(u32 pin, u32 sel);
S32 mt_get_gpio_pull_select(u32 pin);

/*schmitt trigger*/
S32 mt_set_gpio_smt(u32 pin, u32 enable);
S32 mt_get_gpio_smt(u32 pin);

/*IES*/
S32 mt_set_gpio_ies(u32 pin, u32 enable);
S32 mt_get_gpio_ies(u32 pin);

/*input/output*/
S32 mt_set_gpio_out(u32 pin, u32 output);
S32 mt_get_gpio_out(u32 pin);
S32 mt_get_gpio_in(u32 pin);

/*mode control*/
S32 mt_set_gpio_mode(u32 pin, u32 mode);
S32 mt_get_gpio_mode(u32 pin);

/*misc functions for protect GPIO*/
void mt_gpio_init(void);
void mt_gpio_set_default(void);
#endif //_GPIO_H_
