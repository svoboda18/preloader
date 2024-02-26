/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
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
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#include "platform.h"
#include "usbphy.h"
#include "usbd.h"


#if CFG_FPGA_PLATFORM
#include "mtk-phy-a60810.h"
#include "i2c.h"

static struct mt_i2c_t usb_i2c;
#define U3_PHY_PAGE		0xff

u32 usb_i2c_read8(u8 addr, u8 *databuffer)
{
	u32 ret_code = I2C_OK;
	u16 len;
	*databuffer = addr;

	usb_i2c.id = USB_I2C_ID;
	/* Since i2c will left shift 1 bit, we need to set USB I2C address to 0x60 (0xC0>>1) */
	usb_i2c.addr = 0x60;
	usb_i2c.mode = ST_MODE;
	usb_i2c.speed = 100;
	len = 1;

	ret_code = i2c_write_read(&usb_i2c, databuffer, len, len);

	return ret_code;
}

u32 usb_i2c_write8(u8 addr, u8 value)
{
	u32 ret_code = I2C_OK;
	u8 write_data[2];
	u16 len;

	write_data[0]= addr;
	write_data[1] = value;

	usb_i2c.id = USB_I2C_ID;
	/* Since i2c will left shift 1 bit, we need to set USB I2C address to 0x60 (0xC0>>1) */
	usb_i2c.addr = 0x60;
	usb_i2c.mode = ST_MODE;
	usb_i2c.speed = 100;
	len = 2;

	ret_code = i2c_write(&usb_i2c, write_data, len);

	return ret_code;
}

void _u3_write_bank(u32 value)
{
	usb_i2c_write8((u8)U3_PHY_PAGE, (u8)value);
}

u32 _u3_read_reg(u32 address)
{
	u8 databuffer = 0;
	usb_i2c_read8((u8)address, &databuffer);
	return databuffer;
}

void _u3_write_reg(u32 address, u32 value)
{
	usb_i2c_write8((u8 )address, (u8 )value);
}

u32 u3_phy_read_reg32(u32 addr)
{
	u32 bank;
	u32 addr8;
	u32 data;

	bank = (addr >> 16) & 0xff;
	addr8 = addr & 0xff;

	_u3_write_bank(bank);
	data = _u3_read_reg(addr8);
	data |= (_u3_read_reg(addr8 + 1) << 8);
	data |= (_u3_read_reg(addr8 + 2) << 16);
	data |= (_u3_read_reg(addr8 + 3) << 24);
	return data;
}

u32 u3_phy_write_reg32(u32 addr, u32 data) {
	u32 bank;
	u32 addr8;
	u32 data_0, data_1, data_2, data_3;

	bank = (addr >> 16) & 0xff;
	addr8 = addr & 0xff;
	data_0 = data & 0xff;
	data_1 = (data >> 8) & 0xff;
	data_2 = (data >> 16) & 0xff;
	data_3 = (data >> 24) & 0xff;

	_u3_write_bank(bank);
	_u3_write_reg(addr8, data_0);
	_u3_write_reg(addr8 + 1, data_1);
	_u3_write_reg(addr8 + 2, data_2);
	_u3_write_reg(addr8 + 3, data_3);

	return 0;
}

void u3_phy_write_field32(int addr, int offset, int mask, int value)
{
	u32 cur_value;
	u32 new_value;
	cur_value = u3_phy_read_reg32(addr);
	new_value = (cur_value & (~mask)) | ((value << offset) & mask);
	//udelay(i2cdelayus);
	u3_phy_write_reg32(addr, new_value);
}

u32 u3_phy_write_reg8(u32 addr, u8 data)
{
	u32 bank;
	u32 addr8;

	bank = (addr >> 16) & 0xff;
	addr8 = addr & 0xff;
	_u3_write_bank(bank);
	_u3_write_reg(addr8, data);

	return 0;
}

void mt_usb_phy_poweron (void)
{
    static struct u3phy_info info;
	volatile u32 u3phy_version;

	info.phyd_version_addr = 0x2000e4;
	u3phy_version = u3_phy_read_reg32(info.phyd_version_addr);
	print("[UPHY] Phy version is %x\n", u3phy_version);

	info.u2phy_regs_a = (struct u2phy_reg_a *)0x0;
	info.u3phyd_regs_a = (struct u3phyd_reg_a *)0x100000;
	info.u3phyd_bank2_regs_a = (struct u3phyd_bank2_reg_a *)0x200000;
	info.u3phya_regs_a = (struct u3phya_reg_e *)0x300000;
	info.u3phya_da_regs_a = (struct u3phya_da_reg_a *)0x400000;
	info.sifslv_chip_regs_a = (struct sifslv_chip_reg_a *)0x500000;
	info.spllc_regs_a = (struct spllc_reg_a *)0x600000;
	info.sifslv_fm_regs_a = (struct sifslv_fm_feg_a *)0xf00000;

    /* force_suspendm =  1'b0 */
    USBPHY_CLR32(0x68, (0x01 << 18));

    /* force vbus valid */
    /* Need to to this if externl phy doesn't connect to MAC layer correctly */
    USBPHY_SET32(0x6c, (0x3f2e << 0));

    if (u3phy_version == 0xA60810A) {
        /* BANK 0x00 */
        /* for U2 hS eye diagram */
        u3_phy_write_field32(((u32)&info.u2phy_regs_a->usbphyacr1)
        , A60810_RG_USB20_TERM_VREF_SEL_OFST, A60810_RG_USB20_TERM_VREF_SEL, 0x05);
        /* for U2 hS eye diagram */
        u3_phy_write_field32(((u32)&info.u2phy_regs_a->usbphyacr1)
        , A60810_RG_USB20_VRT_VREF_SEL_OFST, A60810_RG_USB20_VRT_VREF_SEL, 0x05);
        /* for U2 sensititvity */
        u3_phy_write_field32(((u32)&info.u2phy_regs_a->usbphyacr6)
        , A60810_RG_USB20_SQTH_OFST, A60810_RG_USB20_SQTH, 0x04);

        /* BANK 0x10 */
        /* disable ssusb_p3_entry to work around resume from P3 bug */
        u3_phy_write_field32(((u32)&info.u3phyd_regs_a->phyd_lfps0)
        , A60810_RG_SSUSB_P3_ENTRY_OFST, A60810_RG_SSUSB_P3_ENTRY, 0x00);
        /* force disable ssusb_p3_entry to work around resume from P3 bug */
        u3_phy_write_field32(((u32)&info.u3phyd_regs_a->phyd_lfps0)
        , A60810_RG_SSUSB_P3_ENTRY_SEL_OFST, A60810_RG_SSUSB_P3_ENTRY_SEL, 0x01);

        /* BANK 0x40 */
        /* fine tune SSC delta1 to let SSC min average ~0ppm */
        u3_phy_write_field32(((u32)&info.u3phya_da_regs_a->reg19)
        , A60810_RG_SSUSB_PLL_SSC_DELTA1_U3_OFST, A60810_RG_SSUSB_PLL_SSC_DELTA1_U3, 0x46);
        /* U3PhyWriteField32(((u32)&info.u3phya_da_regs_a->reg19) */
        u3_phy_write_field32(((u32)&info.u3phya_da_regs_a->reg21)
        , A60810_RG_SSUSB_PLL_SSC_DELTA1_PE1H_OFST, (U32)A60810_RG_SSUSB_PLL_SSC_DELTA1_PE1H, 0x40);

        /* fine tune SSC delta to let SSC min average ~0ppm */

        /* Fine tune SYSPLL to improve phase noise */
        /* I2C  60    0x08[01:00]	0x03   RW  RG_SSUSB_PLL_BC_U3 */
        u3_phy_write_field32(((u32)&info.u3phya_da_regs_a->reg4)
        , A60810_RG_SSUSB_PLL_BC_U3_OFST, A60810_RG_SSUSB_PLL_BC_U3, 0x3);
        /* I2C  60    0x08[12:10]	0x03   RW  RG_SSUSB_PLL_DIVEN_U3 */
        u3_phy_write_field32(((u32)&info.u3phya_da_regs_a->reg4)
        , A60810_RG_SSUSB_PLL_DIVEN_U3_OFST, A60810_RG_SSUSB_PLL_DIVEN_U3, 0x3);
        /* I2C  60    0x0C[03:00]	0x01   RW  RG_SSUSB_PLL_IC_U3 */
        u3_phy_write_field32(((u32)&info.u3phya_da_regs_a->reg5)
        , A60810_RG_SSUSB_PLL_IC_U3_OFST, A60810_RG_SSUSB_PLL_IC_U3, 0x1);
        /* I2C  60    0x0C[23:22]	0x01   RW  RG_SSUSB_PLL_BR_U3 */
        u3_phy_write_field32(((u32)&info.u3phya_da_regs_a->reg5)
        , A60810_RG_SSUSB_PLL_BR_U3_OFST, A60810_RG_SSUSB_PLL_BR_U3, 0x1);
        /* I2C  60    0x10[03:00]	0x01   RW  RG_SSUSB_PLL_IR_U3 */
        u3_phy_write_field32(((u32)&info.u3phya_da_regs_a->reg6)
        , A60810_RG_SSUSB_PLL_IR_U3_OFST, A60810_RG_SSUSB_PLL_IR_U3, 0x1);
        /* I2C  60    0x14[03:00]	0x0F   RW  RG_SSUSB_PLL_BP_U3 */
        u3_phy_write_field32(((u32)&info.u3phya_da_regs_a->reg7)
            , A60810_RG_SSUSB_PLL_BP_U3_OFST, A60810_RG_SSUSB_PLL_BP_U3, 0x0f);

        /* BANK 0x60 */
        /* force xtal pwd mode enable */
        u3_phy_write_field32(((u32)&info.spllc_regs_a->u3d_xtalctl_2)
            , A60810_RG_SSUSB_FORCE_XTAL_PWD_OFST, A60810_RG_SSUSB_FORCE_XTAL_PWD, 0x1);
        /* force bias pwd mode enable */
        u3_phy_write_field32(((u32)&info.spllc_regs_a->u3d_xtalctl_2)
            , A60810_RG_SSUSB_FORCE_BIAS_PWD_OFST, A60810_RG_SSUSB_FORCE_BIAS_PWD, 0x1);
        /* force xtal pwd mode off to work around xtal drv de */
        u3_phy_write_field32(((u32)&info.spllc_regs_a->u3d_xtalctl_2)
            , A60810_RG_SSUSB_XTAL_PWD_OFST, A60810_RG_SSUSB_XTAL_PWD, 0x0);
        /* force bias pwd mode off to work around xtal drv de */
        u3_phy_write_field32(((u32)&info.spllc_regs_a->u3d_xtalctl_2)
            , A60810_RG_SSUSB_BIAS_PWD_OFST, A60810_RG_SSUSB_BIAS_PWD, 0x0);

        /********* test chip settings ***********/
        /* BANK 0x00 */
        /* slew rate setting */
        u3_phy_write_field32(((u32)&info.u2phy_regs_a->usbphyacr5)
            , A60810_RG_USB20_HSTX_SRCTRL_OFST, A60810_RG_USB20_HSTX_SRCTRL, 0x4);

        /* BANK 0x50 */

        /* PIPE setting  BANK5 */
        /* PIPE drv = 2 */
        u3_phy_write_reg8(((u32)&info.sifslv_chip_regs_a->gpio_ctla)+2, 0x10);
        /* PIPE phase */
        /* U3PhyWriteReg8(((u32)&info.sifslv_chip_regs_a->gpio_ctla)+3, 0xdc); */
        u3_phy_write_reg8(((u32)&info.sifslv_chip_regs_a->gpio_ctla)+3, 0x24);

    } else {
        #define PHY_DRIVING   0x3

        UINT8 usbreg8;
        unsigned int i;

        USBPHY_I2C_WRITE8(0xff, 0x00);
        USBPHY_I2C_WRITE8(0x61, 0x04);
        USBPHY_I2C_WRITE8(0x68, 0x00);
        USBPHY_I2C_WRITE8(0x6a, 0x00);
        USBPHY_I2C_WRITE8(0x00, 0x6e);
        USBPHY_I2C_WRITE8(0x1b, 0x0c);
        USBPHY_I2C_WRITE8(0x08, 0x44);
        USBPHY_I2C_WRITE8(0x11, 0x55);
        USBPHY_I2C_WRITE8(0x1a, 0x68);

        #if defined(USB_PHY_DRIVING_TUNING)
        /* driving tuning */
        USBPHY_I2C_READ8(0xab, &usbreg8);
        usbreg8 &= ~0x3;
        usbreg8 |= PHY_DRIVING;
        USBPHY_I2C_WRITE8(0xab, usbreg8);

        for(i = 0; i < 16; i++)
        {
            USBPHY_I2C_READ8((0x92+i), &usbreg8);
            usbreg8 &= ~0x3;
            usbreg8 |= PHY_DRIVING;
            USBPHY_I2C_WRITE8((0x92+i), usbreg8);
        }

        USBPHY_I2C_READ8(0xbc, &usbreg8);
        usbreg8 &= ~0x3;
        usbreg8 |= PHY_DRIVING;
        USBPHY_I2C_WRITE8(0xbc, usbreg8);

        USBPHY_I2C_READ8(0xbe, &usbreg8);
        usbreg8 &= ~0x3;
        usbreg8 |= PHY_DRIVING;
        USBPHY_I2C_WRITE8(0xbe, usbreg8);

        USBPHY_I2C_READ8(0xbf, &usbreg8);
        usbreg8 &= ~0x3;
        usbreg8 |= PHY_DRIVING;
        USBPHY_I2C_WRITE8(0xbf, usbreg8);

        USBPHY_I2C_READ8(0xcd, &usbreg8);
        usbreg8 &= ~0x3;
        usbreg8 |= PHY_DRIVING;
        USBPHY_I2C_WRITE8(0xcd, usbreg8);

        USBPHY_I2C_READ8(0xf1, &usbreg8);
        usbreg8 &= ~0x3;
        usbreg8 |= PHY_DRIVING;
        USBPHY_I2C_WRITE8(0xf1, usbreg8);

        USBPHY_I2C_READ8(0xa7, &usbreg8);
        usbreg8 &= ~0x3;
        usbreg8 |= PHY_DRIVING;
        USBPHY_I2C_WRITE8(0xa7, usbreg8);

        USBPHY_I2C_READ8(0xa8, &usbreg8);
        usbreg8 &= ~0x3;
        usbreg8 |= PHY_DRIVING;
        USBPHY_I2C_WRITE8(0xa8, usbreg8);
        #endif
    }
    udelay(800);
}
void mt_usb_phy_savecurrent (void)
{
	/* no need */
}
void mt_usb_phy_recover (void)
{
	/* no need */
}
void mt_usb11_phy_savecurrent(void)
{
	/* no need */
}
void mt_usb_calibraion ()
{
	/* no need */
}

void Charger_Detect_Init(void)
{
	/* no need */
}

void Charger_Detect_Release(void)
{
	/* no need */
}

int mt_usb_phy_is_on(void)
{
	/* no need */
	return 0;
}

#else
void mt_usb_phy_poweron (void)
{
    /*
	 * force_uart_en	1'b0		0x68 26
	 * RG_UART_EN		1'b0		0x6c 16
	 * rg_usb20_gpio_ctl	1'b0		0x20 09
	 * usb20_gpio_mode	1'b0		0x20 08
	 * RG_USB20_BC11_SW_EN	1'b0		0x18 23
	 * rg_usb20_dp_100k_mode 1'b1		0x20 18
	 * USB20_DP_100K_EN	1'b0		0x20 16
	 * RG_USB20_DM_100K_EN	1'b0		0x20 17
	 * RG_USB20_OTG_VBUSCMP_EN	1'b1	0x18 20
	 * force_suspendm		1'b0	0x68 18
	*/

	/* force_uart_en, 1'b0 */
	USBPHY_CLR32(0x68, (0x1 << 26));
	/* RG_UART_EN, 1'b0 */
	USBPHY_CLR32(0x6c, (0x1 << 16));
	/* rg_usb20_gpio_ctl, 1'b0, usb20_gpio_mode, 1'b0 */
	USBPHY_CLR32(0x20, ((0x1 << 9) | (0x1 << 8)));

	/* RG_USB20_BC11_SW_EN, 1'b0 */
	USBPHY_CLR32(0x18, (0x1 << 23));

	/* rg_usb20_dp_100k_mode, 1'b1 */
	USBPHY_SET32(0x20, (0x1 << 18));
	/* USB20_DP_100K_EN 1'b0, RG_USB20_DM_100K_EN, 1'b0 */
	USBPHY_CLR32(0x20, ((0x1 << 16) | (0x1 << 17)));

	/* RG_USB20_OTG_VBUSCMP_EN, 1'b1 */
	USBPHY_SET32(0x18, (0x1 << 20));

	/* force_suspendm, 1'b0 */
	USBPHY_CLR32(0x68, (0x1 << 18));

	/* wait for 800 usec. */
	udelay(800);
}

void mt_usb_phy_savecurrent (void)
{
	/*
	 * force_uart_en	1'b0		0x68 26
	 * RG_UART_EN		1'b0		0x6c 16
	 * rg_usb20_gpio_ctl	1'b0		0x20 09
	 * usb20_gpio_mode	1'b0		0x20 08

	 * RG_USB20_BC11_SW_EN	1'b0		0x18 23
	 * RG_USB20_OTG_VBUSCMP_EN	1'b0	0x18 20
	 * RG_SUSPENDM		1'b1		0x68 03
	 * force_suspendm	1'b1		0x68 18

	 * RG_DPPULLDOWN	1'b1		0x68 06
	 * RG_DMPULLDOWN	1'b1		0x68 07
	 * RG_XCVRSEL[1:0]	2'b01		0x68 [04-05]
	 * RG_TERMSEL		1'b1		0x68 02
	 * RG_DATAIN[3:0]	4'b0000		0x68 [10-13]
	 * force_dp_pulldown	1'b1		0x68 20
	 * force_dm_pulldown	1'b1		0x68 21
	 * force_xcversel	1'b1		0x68 19
	 * force_termsel	1'b1		0x68 17
	 * force_datain		1'b1		0x68 23

	 * RG_SUSPENDM		1'b0		0x68 03
	*/

	/* force_uart_en, 1'b0 */
	USBPHY_CLR32(0x68, (0x1 << 26));
	/* RG_UART_EN, 1'b0 */
	USBPHY_CLR32(0x6c, (0x1 << 16));
	/* rg_usb20_gpio_ctl, 1'b0, usb20_gpio_mode, 1'b0 */
	USBPHY_CLR32(0x20, (0x1 << 9));
	USBPHY_CLR32(0x20, (0x1 << 8));

	/* RG_USB20_BC11_SW_EN, 1'b0 */
	USBPHY_CLR32(0x18, (0x1 << 23));
	/* RG_USB20_OTG_VBUSCMP_EN, 1'b0 */
	USBPHY_CLR32(0x18, (0x1 << 20));

	/* RG_SUSPENDM, 1'b1 */
	USBPHY_SET32(0x68, (0x1 << 3));
	/* force_suspendm, 1'b1 */
	USBPHY_SET32(0x68, (0x1 << 18));

	/* RG_DPPULLDOWN, 1'b1, RG_DMPULLDOWN, 1'b1 */
	USBPHY_SET32(0x68, ((0x1 << 6) | (0x1 << 7)));

	/* RG_XCVRSEL[1:0], 2'b01. */
	USBPHY_CLR32(0x68, (0x3 << 4));
	USBPHY_SET32(0x68, (0x1 << 4));
	/* RG_TERMSEL, 1'b1 */
	USBPHY_SET32(0x68, (0x1 << 2));
	/* RG_DATAIN[3:0], 4'b0000 */
	USBPHY_CLR32(0x68, (0xF << 10));

	/* force_dp_pulldown, 1'b1, force_dm_pulldown, 1'b1,
	 * force_xcversel, 1'b1, force_termsel, 1'b1, force_datain, 1'b1
	 */
	USBPHY_SET32(0x68, ((0x1 << 20) | (0x1 << 21) | (0x1 << 19) | (0x1 << 17) | (0x1 << 23)));

	udelay(800);

	/* RG_SUSPENDM, 1'b0 */
	USBPHY_CLR32(0x68, (0x1 << 3));

	udelay(1);
}

void mt_usb_phy_recover (void)
{
    /*
	 * 04.force_uart_en	1'b0 0x68 26
	 * 04.RG_UART_EN		1'b0 0x6C 16
	 * 04.rg_usb20_gpio_ctl	1'b0 0x20 09
	 * 04.usb20_gpio_mode	1'b0 0x20 08

	 * 05.force_suspendm	1'b0 0x68 18

	 * 06.RG_DPPULLDOWN	1'b0 0x68 06
	 * 07.RG_DMPULLDOWN	1'b0 0x68 07
	 * 08.RG_XCVRSEL[1:0]	2'b00 0x68 [04:05]
	 * 09.RG_TERMSEL		1'b0 0x68 02
	 * 10.RG_DATAIN[3:0]	4'b0000 0x68 [10:13]
	 * 11.force_dp_pulldown	1'b0 0x68 20
	 * 12.force_dm_pulldown	1'b0 0x68 21
	 * 13.force_xcversel	1'b0 0x68 19
	 * 14.force_termsel	1'b0 0x68 17
	 * 15.force_datain	1'b0 0x68 23
	 * 16.RG_USB20_BC11_SW_EN	1'b0 0x18 23
	 * 17.RG_USB20_OTG_VBUSCMP_EN	1'b1 0x18 20
	*/

	/* clean PUPD_BIST_EN */
	/* PUPD_BIST_EN = 1'b0 */
	/* PMIC will use it to detect charger type */
	/* NEED?? USBPHY_CLR8(0x1d, 0x10);*/
	USBPHY_CLR32(0x1c, (0x1 << 12));

	/* force_uart_en, 1'b0 */
	USBPHY_CLR32(0x68, (0x1 << 26));
	/* RG_UART_EN, 1'b0 */
	USBPHY_CLR32(0x6C, (0x1 << 16));
	/* rg_usb20_gpio_ctl, 1'b0, usb20_gpio_mode, 1'b0 */
	USBPHY_CLR32(0x20, (0x1 << 9));
	USBPHY_CLR32(0x20, (0x1 << 8));

	/* force_suspendm, 1'b0 */
	USBPHY_CLR32(0x68, (0x1 << 18));

	/* RG_DPPULLDOWN, 1'b0, RG_DMPULLDOWN, 1'b0 */
	USBPHY_CLR32(0x68, ((0x1 << 6) | (0x1 << 7)));

	/* RG_XCVRSEL[1:0], 2'b00. */
	USBPHY_CLR32(0x68, (0x3 << 4));

	/* RG_TERMSEL, 1'b0 */
	USBPHY_CLR32(0x68, (0x1 << 2));
	/* RG_DATAIN[3:0], 4'b0000 */
	USBPHY_CLR32(0x68, (0xF << 10));

	/* force_dp_pulldown, 1'b0, force_dm_pulldown, 1'b0,
	 * force_xcversel, 1'b0, force_termsel, 1'b0, force_datain, 1'b0
	 */
	USBPHY_CLR32(0x68, ((0x1 << 20) | (0x1 << 21) | (0x1 << 19) | (0x1 << 17) | (0x1 << 23)));

	/* RG_USB20_BC11_SW_EN, 1'b0 */
	USBPHY_CLR32(0x18, (0x1 << 23));
	/* RG_USB20_OTG_VBUSCMP_EN, 1'b1 */
	USBPHY_SET32(0x18, (0x1 << 20));

	/* wait 800 usec. */
	udelay(800);

	/* force enter device mode */
	USBPHY_CLR32(0x6C, (0x10<<0));
	USBPHY_SET32(0x6C, (0x2F<<0));
	USBPHY_SET32(0x6C, (0x3F<<8));
}

void mt_usb_calibraion (void)
{
}

void mt_usb_phy_calibraion (int case_set, int input_reg)
{
}

void mt_usb11_phy_savecurrent(void)
{
}

void Charger_Detect_Init(void)
{
	/* RG_USB20_BC11_SW_EN = 1'b1 */
	USBPHY_SET32(0x18, (0x1 << 23));
}

void Charger_Detect_Release(void)
{
	/* RG_USB20_BC11_SW_EN = 1'b0 */
	USBPHY_CLR32(0x18, (0x1 << 23));
}

int mt_usb_phy_is_on(void)
{
	/* check RG_SUSPENDM */
	return (USBPHY_READ32(0x68) & (0x1 << 3));
}

#endif

