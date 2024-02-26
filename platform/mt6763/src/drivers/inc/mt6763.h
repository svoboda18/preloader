/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2011. All rights reserved.
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
 fix* The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#ifndef MT6763_H
#define MT6763_H

/*=======================================================================*/
/* Constant Definitions                                                  */
/*=======================================================================*/

#define IO_PHYS             (0x10000000)
#define IO_SIZE             (0x02000000)

#define VER_BASE            (0x08000000)

/*=======================================================================*/
/* Register Bases                                                        */
/*=======================================================================*/
/* FIXME, these registers are defined for mt6589, not sure the corresponding register in mt6582*/
#define TOPCKGEN_BASE       (IO_PHYS + 0x0000)
#define CONFIG_BASE         (IO_PHYS + 0x1000)		/* FIXME do not list in mt6763 memory map*/
#define INFRACFG_AO_BASE    (IO_PHYS + 0x1000)
#define GPIO_BASE           (0x10005000)
#define IOCFG_LT_BASE       (0x11F20000)
#define IOCFG_LM_BASE       (0x11E80000)
#define IOCFG_LB_BASE       (0x11E70000)
#define IOCFG_BL_BASE       (0x11E90000)
#define IOCFG_RB_BASE       (0x11D30000)
#define IOCFG_RM_BASE       (0x11D20000)
#define IOCFG_RT_BASE       (0x11C50000)
#define IOCFG_TL_BASE       (0x11F30000)
#define DEVINFO_BASE        (0x11F10000)
#define PERICFG_BASE        (IO_PHYS + 0x3000)
//#define PERI_CON_BASE       (IO_PHYS + 0x00003000)  /* CHECKME & FIXME */

#define DRAMC0_BASE         (IO_PHYS + 0x4000)
#define SPM_BASE            (IO_PHYS + 0x6000)
#define RGU_BASE            (IO_PHYS + 0x7000)
#define GPT_BASE            (IO_PHYS + 0x8000)
#define PWRAP_BASE          (IO_PHYS + 0xD000)
#define DDRPHY_BASE         (IO_PHYS + 0xF000)
#define KPD_BASE            (IO_PHYS + 0x00010000)

#define DEM_BASE            (0x0D0A0000)

#define MCUSYS_CFGREG_BASE  (0x0C530000)
//#define CA7MCUCFG_BASE      (IO_PHYS + 0x00200100)	/* FIXME do not list in mt6763 memory map*/
//#define CA15L_CONFIG_BASE   (IO_PHYS + 0x00200200)	/* FIXME do not list in mt6763 memory map*/
#define SRAMROM_BASE        (IO_PHYS + 0x00214000)
#define EMI_BASE            (IO_PHYS + 0x00219000)
#define EMI_MPU_BASE        (IO_PHYS + 0x00226000)
#define CHN0_EMI_BASE       (IO_PHYS + 0x0022D000)
#define CHN1_EMI_BASE       (IO_PHYS + 0x00235000)
#define DRAMC_NAO_BASE      (IO_PHYS + 0x00228000)
#define MIPI_TX0_BASE         (IO_PHYS + 0x00215000)    /* FIXME do not list in mt6763 memory map*/
#define MIPI_RX_ANA_CSI0_BASE (IO_PHYS + 0x00217000)    /* FIXME do not list in mt6763 memory map*/
#define MIPI_RX_ANA_CSI1_BASE (IO_PHYS + 0x00218000)    /* FIXME do not list in mt6763 memory map*/
#define GICD_BASE           (0x0c000000)
#define GICR_BASE           (0x0c100000)

//#define RTC_BASE            (IO_PHYS + 0x01003000)  /* CHECKME & FIXME (new design) */
#define RTC_BASE            0x0980	//used in rtc.c

#define UART0_BASE          (IO_PHYS + 0x01002000)
#define UART1_BASE          (IO_PHYS + 0x01003000)
#define UART2_BASE          (IO_PHYS + 0x01004000)
#define UART3_BASE          (IO_PHYS + 0x01005000)      /* FIXME do not list in mt6763 memory map*/
#define I2C0_BASE           (IO_PHYS + 0x01007000)
#define I2C1_BASE           (IO_PHYS + 0x01008000)
#define I2C2_BASE           (IO_PHYS + 0x01009000)
#define I2C3_BASE           (IO_PHYS + 0x0100F000)
#define I2C4_BASE           (IO_PHYS + 0x01011000)
#define SPI_BASE            (IO_PHYS + 0x0100A000)
//#define NFI_BASE            (IO_PHYS + 0x0100D000)    //Yi-Lung: Do not list it MT6763 memory map
//#define NFIECC_BASE         (IO_PHYS + 0x0100E000)    //Yi-Lung: Do not list it MT6763 memory map
#define USB_BASE            (IO_PHYS + 0x01200000)
#define USBSIF_BASE         (IO_PHYS + 0x01F40000)

#define MSDC0_TOP_BASE      (IO_PHYS + 0x01F50000)
#define MSDC0_BASE          (IO_PHYS + 0x01230000)
#define MSDC1_TOP_BASE      (IO_PHYS + 0x01E10000)
#define MSDC1_BASE          (IO_PHYS + 0x01240000)
#define MSDC2_BASE          (IO_PHYS + 0x01250000)
#define MSDC3_BASE          (IO_PHYS + 0x01260000)      //MingTsang: remove from the mt6763 memory map
#define U3D_BASE			(IO_PHYS + 0x01270000)      /* FIXME do not list in mt6763 memory map*/



/*=======================================================================*/
/* AP HW code offset                                                     */
/*=======================================================================*/
#define APHW_CODE           (VER_BASE)
#define APHW_SUBCODE        (VER_BASE + 0x04)
#define APHW_VER            (VER_BASE + 0x08)
#define APSW_VER            (VER_BASE + 0x0C)

#define AMCONFG_BASE        (0xFFFFFFFF)            /* CHECKME & FIXME */

/*=======================================================================*/
/* USB register offset                                                   */
/*=======================================================================*/
#define SSUSB_DEV_BASE					(U3D_BASE + 0x1000)
#define SSUSB_EPCTL_CSR_BASE			(U3D_BASE + 0x1800)
#define SSUSB_USB3_MAC_CSR_BASE			(U3D_BASE + 0x2400)
#define SSUSB_USB3_SYS_CSR_BASE			(U3D_BASE + 0x2400)
#define SSUSB_USB2_CSR_BASE				(U3D_BASE + 0x3400)

#define SSUSB_SIFSLV_SPLLC_BASE			(USB_SIF2_BASE + 0x000)	/* FIXME do not list in mt6763 memory map*/
#define SSUSB_SIFSLV_IPPC_BASE			(USB_SIF_BASE + 0x700)	/* FIXME do not list in mt6763 memory map*/
#define SSUSB_SIFSLV_U2PHY_COM_BASE		(USB_SIF2_BASE + 0x800)	/* FIXME do not list in mt6763 memory map*/
#define SSUSB_SIFSLV_U3PHYD_BASE		(USB_SIF2_BASE + 0x900)	/* FIXME do not list in mt6763 memory map*/
#define SSUSB_SIFSLV_U2PHY_COM_SIV_B_BASE	(USB_SIF2_BASE + 0x800)	/* FIXME do not list in mt6763 memory map*/
#define SSUSB_USB30_PHYA_SIV_B_BASE				(USB_SIF2_BASE + 0xB00) /* FIXME do not list in mt6763 memory map*/
#define SSUSB_SIFSLV_U3PHYA_DA_BASE				(USB_SIF2_BASE + 0xC00) /* FIXME do not list in mt6763 memory map*/


/*=======================================================================*/
/* USB download control                                                  */
/*=======================================================================*/
#define SECURITY_AO                 (0x1001A000)
#define BOOT_MISC0                  (SECURITY_AO + 0x0080)
#define MISC_LOCK_KEY               (SECURITY_AO + 0x0100)
#define RST_CON                     (SECURITY_AO + 0x0108)

#define MISC_LOCK_KEY_MAGIC    0xAD98
#define USBDL_FLAG    BOOT_MISC0


#define USBDL_BIT_EN        (0x00000001) /* 1: download bit enabled */
#define USBDL_BROM          (0x00000002) /* 0: usbdl by brom; 1: usbdl by bootloader */
#define USBDL_TIMEOUT_MASK  (0x0000FFFC) /* 14-bit timeout: 0x0000~0x3FFE: second; 0x3FFFF: no timeout */
#define USBDL_TIMEOUT_MAX   (USBDL_TIMEOUT_MASK >> 2) /* maximum timeout indicates no timeout */
#define USBDL_MAGIC         (0x444C0000) /* Brom will check this magic number */


#define SRAMROM_USBDL_TO_DIS (SRAMROM_BASE + 0x0054)
#define USBDL_TO_DIS         (0x00000001)

/*=======================================================================*/
/* NAND Control                                                          */
/*=======================================================================*/
#define NAND_PAGE_SIZE                  (2048)  // (Bytes)
#define NAND_BLOCK_BLKS                 (64)    // 64 nand pages = 128KB
#define NAND_PAGE_SHIFT                 (9)
#define NAND_LARGE_PAGE                 (11)    // large page
#define NAND_SMALL_PAGE                 (9)     // small page
#define NAND_BUS_WIDTH_8                (8)
#define NAND_BUS_WIDTH_16               (16)
#define NAND_FDM_SIZE                   (8)
#define NAND_ECC_SW                     (0)
#define NAND_ECC_HW                     (1)

#define NFI_MAX_FDM_SIZE                (8)
#define NFI_MAX_FDM_SEC_NUM             (8)
#define NFI_MAX_LOCK_CHANNEL            (16)

#define ECC_MAX_CORRECTABLE_BITS        (12)
#define ECC_MAX_PARITY_SIZE             (20)    /* in bytes */

#define ECC_ERR_LOCATION_MASK           (0x1FFF)
#define ECC_ERR_LOCATION_SHIFT          (16)

#define NAND_FFBUF_SIZE                 (2048+64)

/*=======================================================================*/
/* SW Reset Vector 
/*=======================================================================*/
/* setup the reset vector base address after warm reset to Aarch64 */
#define RVBADDRESS_CPU0     (MCUSYS_CFGREG_BASE + 0x38)

#endif //#ifndef MT6763_H
