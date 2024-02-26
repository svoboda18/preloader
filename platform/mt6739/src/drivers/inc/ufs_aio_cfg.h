/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2015. All rights reserved.
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
 */

#ifndef _UFS_AIO_CFG_H
#define _UFS_AIO_CFG_H

/*--------------------------------------------------------------------------
 * AIO Driver Platform Configuration
 *--------------------------------------------------------------------------*/

#define MTK_UFS_PLATFORM_MT6739

/*--------------------------------------------------------------------------
 * AIO Driver Environtment Configuration
 *--------------------------------------------------------------------------*/

#define MTK_UFS_DRV_PRELOADER
//#define MTK_UFS_DRV_LK
//#define MTK_UFS_DRV_DA

#if defined(MTK_UFS_DRV_PRELOADER)
#if defined(MTK_UFS_DRV_LK) || defined(MTK_UFS_DRV_DA)
#error "Only one environment definition is allowed!"
#endif
#elif defined(MTK_UFS_DRV_LK)
#if defined(MTK_UFS_DRV_PRELOADER) || defined(MTK_UFS_DRV_DA)
#error "Only one environment definition is allowed!"
#endif
#elif defined(MTK_UFS_DRV_DA)
#if defined(MTK_UFS_DRV_PRELOADER) || defined(MTK_UFS_DRV_LK)
#error "Only one environment definition is allowed!"
#endif
#else
#error "One of following definition shall be defined: MTK_UFS_DRV_PRELOADER, MTK_UFS_DRV_LK, MTK_UFS_DRV_DA"
#endif

/*--------------------------------------------------------------------------
 * FPGA Definition
 *--------------------------------------------------------------------------*/

#if defined(MTK_UFS_DRV_PRELOADER)

#ifdef CFG_FPGA_PLATFORM
#define UFS_CFG_FPGA_PLATFORM
#endif

#elif defined(MTK_UFS_DRV_LK)

#ifdef MACH_FPGA
#define UFS_CFG_FPGA_PLATFORM
#endif

#elif defined(MTK_UFS_DRV_DA)

#if (DA_MODE == DA_MODE_FPGA)
#define UFS_CFG_FPGA_PLATFORM
#endif

#endif

/*--------------------------------------------------------------------------
 * Transfer Mode Configuration
 *--------------------------------------------------------------------------*/

#if defined(MTK_UFS_DRV_PRELOADER)

#define UFS_CFG_ENABLE_DMA
#define UFS_CFG_DEFAULT_DMA

#elif defined(MTK_UFS_DRV_LK)

#define UFS_CFG_ENABLE_DMA
#define UFS_CFG_DEFAULT_DMA

#elif defined(MTK_UFS_DRV_DA)

#define UFS_CFG_ENABLE_DMA
#define UFS_CFG_DEFAULT_DMA

#endif

#ifdef UFS_CFG_DEFAULT_DMA
#ifndef UFS_CFG_ENABLE_DMA
#error "DMA mode shall be enabled since DMA is selected as default mode"
#endif
#endif

#ifdef UFS_CFG_DEFAULT_PIO
#ifndef UFS_CFG_ENABLE_PIO
#error "PIO mode shall be enabled since PIO is selected as default mode"
#endif
#endif

#if !defined(UFS_CFG_DEFAULT_DMA) && !defined(UFS_CFG_DEFAULT_PIO)
#error "Must define DMA or PIO as default mode"
#endif

#if defined(UFS_CFG_DEFAULT_DMA) && defined(UFS_CFG_DEFAULT_PIO)
#error "Must define only UFS_CFG_DEFAULT_DMA or only UFS_CFG_DEFAULT_PIO"
#endif

/*--------------------------------------------------------------------------*/
/* AIO Driver Features                                                      */
/*--------------------------------------------------------------------------*/

/*
 * Support single command only
 * Must define this currently because AIO driver supports single command only now.
 */

#define UFS_CFG_SINGLE_COMMAND

/*
 * Defined:     Enable UFS raw performance profiling
 * Non-defined: Disable UFS raw performance profiling
 */

/* #define UFS_CFG_PERFORMANCE_PROFILING */

/*
 * Defined:     Use slow UFS device speed for safe bring-up. Used during development stage only.
 * Non-defined: Use high UFS device speed for performance. Shall be default configuration.
 */

/* #define UFS_CFG_SAFE_BRING_UP */

/*
 * Defined:     Enable HQA mode. Preloader will configure GPIO for MPHY I2C mux and halt to wait for HQA process.
 * Non-defined: Disable HQA mode.
 */

/* #define UFS_CFG_HQA_MODE */

#ifdef MTK_UFS_DRV_DA

/*
 * Defined:     Device reset can be triggerred by SW driver (default)
 * Non-defined: Device reset can be triggerred only by watchdog or system reset (for security-concerned customers)
 */

#define UFS_CFG_DEVICE_RESET_NONPROTECTED

#endif

/*--------------------------------------------------------------------------
 * Debugging Configuration
 *--------------------------------------------------------------------------*/

#define UFS_DBG_LVL_VERBOSE     0
#define UFS_DBG_LVL_DEBUG       1
#define UFS_DBG_LVL_INFO        2
#define UFS_DBG_LVL_WARNING     3
#define UFS_DBG_LVL_CRITICAL    4
#define UFS_DBG_LVL_FATAL       5

#if defined(UFS_CFG_SAFE_BRING_UP) || defined(UFS_CFG_DEBUG)
#define UFS_DBG_LVL             (UFS_DBG_LVL_DEBUG)
#else
#define UFS_DBG_LVL             (UFS_DBG_LVL_INFO)
#endif

/*--------------------------------------------------------------------------
 * Common Definition
 *--------------------------------------------------------------------------*/

/* UFS PA power modes */
enum {
	FAST_MODE   = 1,
	SLOW_MODE   = 2,
	FASTAUTO_MODE   = 4,
	SLOWAUTO_MODE   = 5,
	UNCHANGED   = 7,
};

/* UFS PA TX/RX Frequency Series */
enum {
	PA_HS_MODE_A    = 1,
	PA_HS_MODE_B    = 2,
};

/* UFS device default power mode, gear, lane configurations */
#ifndef UFS_CFG_SAFE_BRING_UP

#if defined(MTK_UFS_DRV_PRELOADER)
#define UFS_DEV_MAX_GEAR_RX     (1)
#define UFS_DEV_MAX_GEAR_TX     (1)
#define UFS_DEV_MAX_LANE_RX     (1)
#define UFS_DEV_MAX_LANE_TX     (1)
#define UFS_DEV_DEFAULT_HS_RATE (PA_HS_MODE_B)
#define UFS_DEV_DEFAULT_PWR_RX  (FAST_MODE)
#define UFS_DEV_DEFAULT_PWR_TX  (FAST_MODE)
#elif defined(MTK_UFS_DRV_LK)
#define UFS_DEV_MAX_GEAR_RX     (1)
#define UFS_DEV_MAX_GEAR_TX     (1)
#define UFS_DEV_MAX_LANE_RX     (1)
#define UFS_DEV_MAX_LANE_TX     (1)
#define UFS_DEV_DEFAULT_HS_RATE (PA_HS_MODE_B)
#define UFS_DEV_DEFAULT_PWR_RX  (FAST_MODE)
#define UFS_DEV_DEFAULT_PWR_TX  (FAST_MODE)
#elif defined(MTK_UFS_DRV_DA)
#define UFS_DEV_MAX_GEAR_RX     (1)
#define UFS_DEV_MAX_GEAR_TX     (1)
#define UFS_DEV_MAX_LANE_RX     (1)
#define UFS_DEV_MAX_LANE_TX     (1)
#define UFS_DEV_DEFAULT_HS_RATE (PA_HS_MODE_B)
#define UFS_DEV_DEFAULT_PWR_RX  (FAST_MODE)
#define UFS_DEV_DEFAULT_PWR_TX  (FAST_MODE)
#endif

#else

/* Use PWM-G4 for safe bring-up case */
#define UFS_DEV_MAX_GEAR_RX     (4)
#define UFS_DEV_MAX_GEAR_TX     (4)
#define UFS_DEV_MAX_LANE_RX     (1)
#define UFS_DEV_MAX_LANE_TX     (1)
#define UFS_DEV_DEFAULT_HS_RATE (PA_HS_MODE_A)
#define UFS_DEV_DEFAULT_PWR_RX  (SLOW_MODE)
#define UFS_DEV_DEFAULT_PWR_TX  (SLOW_MODE)

#endif
#endif /* _UFS_AIO_CFG_H */

