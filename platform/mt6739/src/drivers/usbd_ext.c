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

#if (MTK_UART_USB_SWITCH)
#include "usbtty.h"
#include "usbphy.h"

/* Get LineState of DP/DM */
bool get_linestate(void)
{
    u8 tmpReg8;

    /* linestate */
    tmpReg8 = __raw_readb (DBG_PRB0);
    print("\n[USBD] USB PRB0 LineState: %x\n", tmpReg8);
    tmpReg8 = __raw_readb (DBG_PRB4);
    print("\n[USBD] USB PRB4 LineState: %x\n", tmpReg8);

    return 0;
}

bool is_uart_cable_inserted(void)
{
    u32 tmpReg32;

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

	/* RG_USB20_PHY_REV[7:0], 8'b01000000 */
	USBPHY_SET32(0x18, (0x1000000 << 24));

	/* wait for 800 usec. */
	udelay(800);

    /* linestate */
    //tmpReg32 = USBPHY_READ32(DBG_PRB0);
    tmpReg32 = __raw_readl (DBG_PRB0);
    print("\n[USBD] USB PRB0 LineState: %x\n", tmpReg32);

	if (tmpReg32 == 0xC0 ||
		tmpReg32 == 0x80) {
		/* Prolific/FTDI UART cable inserted */
        print("\n[USBD] Prolific/FTDI UART cable inserted!\n");
		return true;
    }
	else {
        /* Step 1. Power down mode OTG setting */
		USBPHY_CLR32(0x6c, (0x3E << 0));
		USBPHY_SET32(0x6c, (0x12 << 0));
        /* Step 2. Do not apply USB IP reset, Wait MAC off ready */
        mdelay(10);
        /* Step 3. Clear OTG Force mode */
		USBPHY_CLR32(0x6c, (0x1F << 9));
        /* Step 4. RG_SUSPENDM 1'b0 */
	    USBPHY_CLR32(0x68, (0x1 << 3));
		/* Step 5. force_suspendm, 1'b1 */
		USBPHY_SET32(0x68, (0x1 << 18));
        /* Step 6. Wait 2ms */
	    mdelay(2);
		/* USB cable/ No Cable inserted */
        print("\n[USBD] USB cable/ No Cable inserted!\n");
		return false;
    }
}
#endif
