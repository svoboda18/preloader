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

#ifndef _MSDC_CFG_H_
#define _MSDC_CFG_H_

//#define MMC_MSDC_DRV_CTP
#define MMC_MSDC_DRV_PRELOADER
//#define MMC_MSDC_DRV_LK

#if !defined(MMC_MSDC_DRV_CTP) && !defined(MMC_MSDC_DRV_PRELOADER) && !defined(MMC_MSDC_DRV_LK)
#error Please define of MMC_MSDC_DRV_CTP, MMC_MSDC_DRV_PRELOADER, MMC_MSDC_DRV_LK
#elif defined(MMC_MSDC_DRV_CTP) && defined(MMC_MSDC_DRV_PRELOADER)
#error Please define only one of MMC_MSDC_DRV_CTP, MMC_MSDC_DRV_PRELOADER, MMC_MSDC_DRV_LK
#elif defined(MMC_MSDC_DRV_CTP) && defined(MMC_MSDC_DRV_LK)
#error Please define only one of MMC_MSDC_DRV_CTP, MMC_MSDC_DRV_PRELOADER, MMC_MSDC_DRV_LK
#elif defined(MMC_MSDC_DRV_PRELOADER) && defined(MMC_MSDC_DRV_LK)
#error Please define only one of MMC_MSDC_DRV_CTP, MMC_MSDC_DRV_PRELOADER, MMC_MSDC_DRV_LK
#endif

/*--------------------------------------------------------------------------*/
/* FPGA Definition                                                          */
/*--------------------------------------------------------------------------*/
#if defined(MMC_MSDC_DRV_LK)
#ifdef MACH_FPGA
#define FPGA_PLATFORM
#else
#undef FPGA_PLATFORM
#endif
#endif

#if defined(MMC_MSDC_DRV_CTP)
// FPGA_PLATFORM is determined in drivers/msdc/makefile
#endif

#if defined(MMC_MSDC_DRV_PRELOADER)
#if CFG_FPGA_PLATFORM
#define FPGA_PLATFORM
#else
#undef FPGA_PLATFORM
#endif
#endif

#ifndef FPGA_PLATFORM
//#define MTK_MSDC_BRINGUP_DEBUG
#endif
/*--------------------------------------------------------------------------*/
/* MSDC Host Number                                                         */
/*--------------------------------------------------------------------------*/

#if (defined(MMC_MSDC_DRV_CTP) && !defined(FPGA_PLATFORM))
#define MSDC_MAX_NUM            (2)
#else
#define MSDC_MAX_NUM            (1)
#endif

/*--------------------------------------------------------------------------*/
/* Transfer Mode Definition                                                 */
/*--------------------------------------------------------------------------*/
#define MSDC_ENABLE_DMA_MODE

#if defined(MMC_MSDC_DRV_CTP)
#define MSDC_ENABLE_ENH_DMA_MODE
#endif

#if !defined(MSDC_ENABLE_DMA_MODE) && defined(MSDC_ENABLE_ENH_DMA_MODE)
#error Please define MSDC_ENABLE_DMA_MODE first
#endif

//#define MSDC_MODE_DEFAULT_PIO
//#define MSDC_MODE_DEFAULT_DMA_BASIC
//#define MSDC_MODE_DEFAULT_DMA_DESC
//#define MSDC_MODE_DEFAULT_DMA_ENHANCED

#if defined(MMC_MSDC_DRV_CTP)
//#define MSDC_MODE_DEFAULT_PIO
#define MSDC_MODE_DEFAULT_DMA_BASIC
#endif


#if defined(MMC_MSDC_DRV_PRELOADER) && !defined(SLT)
/* Force preloader of android load to use PIO mode when DMA does not work on 4GB DRAM */
#undef MSDC_ENABLE_DMA_MODE
#endif

#if defined(MMC_MSDC_DRV_PRELOADER)
#if defined(MSDC_ENABLE_DMA_MODE)
#define MSDC_MODE_DEFAULT_DMA_BASIC
#else
#define MSDC_MODE_DEFAULT_PIO
#endif
#endif

#if defined(MMC_MSDC_DRV_LK)
#if defined(MSDC_ENABLE_DMA_MODE)
#define MSDC_MODE_DEFAULT_DMA_DESC
#else
#define MSDC_MODE_DEFAULT_PIO
#endif
#endif


/*--------------------------------------------------------------------------*/
/* Speed Mode Definition                                                    */
/*--------------------------------------------------------------------------*/
#if defined(MMC_MSDC_DRV_CTP)
#define FEATURE_MMC_UHS1
#endif


/*--------------------------------------------------------------------------*/
/* Tuning Definition                                                        */
/*--------------------------------------------------------------------------*/
#define FEATURE_MMC_WR_TUNING
#define FEATURE_MMC_RD_TUNING
#define FEATURE_MMC_CM_TUNING
/* DE comment:it's better to modify EMMC_TOP_CONTROL & EMMC_TOP_CMD
 * to tune than edge tune.However, edge tune can also meet the demand.
 */
/* #define FEATURE_MMC_TUNING_EDGE_ONLY_WHEN_HIGH_SPEED */


/*--------------------------------------------------------------------------*/
/* Common Definition                                                        */
/*--------------------------------------------------------------------------*/
#if defined(MMC_MSDC_DRV_PRELOADER)
#define FEATURE_MMC_SLIM
#endif

#if defined(MMC_MSDC_DRV_LK)
#define MSDC_WITH_DEINIT
#define FEATURE_MMC_BOOT_ACCESS
#define FEATURE_ERASE
#ifdef MTK_POWER_ON_WP
#define FEATURE_MMC_POWER_ON_WP
#endif
#endif

#define MTK_HS400_USED_800M       (0)

#if defined(FPGA_PLATFORM)
#define MSDC_USE_EMMC45_POWER     (1)
#endif

#define FEATURE_EMCP

#if defined(MMC_MSDC_DRV_CTP)
#define FEATURE_MMC_CARD_DETECT

/* CTP use autocmd23 */
#define MSDC_USE_DATA_TAG         (0) /* autocmd23 with data tag */
#define MSDC_USE_RELIABLE_WRITE   (1) /* autocmd23 with reliable write */
#define MSDC_USE_FORCE_FLUSH      (0) /* autocmd23 with force flush cache */
#define MSDC_USE_PACKED_CMD       (0) /* autocmd23 with packed cmd, this feature is conflict with data tag, reliable write, and force flush cache */

#define FEATURE_MMC_BOOT_ACCESS
#define FEATURE_ERASE

#define FEATURE_MMC_BOOT_MODE

/* For CTP Only, but not necessary for CTP */
//#define FEATURE_MMC_SDCARD
//#define FEATURE_MMC_SDIO
//#define MSDC_USE_DCM
//#define MSDC_USE_IRQ

//#define FEATURE_MMC_SW_CMDQ
//#define FEATURE_MMC_SW_CMDQ_CMD44and45_WHILE_DATA_XFR //unused, to be removed
//#define FEATURE_MMC_HW_CMDQ
//#define FEATURE_MMC_USE_EMMC51_CFG0_FOR_CMD44_45

#if defined(FEATURE_MMC_SW_CMDQ) || defined(FEATURE_MMC_SW_CMDQ_CMD44and45_WHILE_DATA_XFR) || defined(FEATURE_MMC_HW_CMDQ) || defined(FEATURE_MMC_USE_EMMC51_CFG0_FOR_CMD44_45)
#define FEATURE_MMC_CMDQ
#endif

//#define FEATURE_NONBLOCKING_RW

#define FEATURE_MULTI_HOST

#endif


/*--------------------------------------------------------------------------*/
/* Test Definition                                                         */
/*--------------------------------------------------------------------------*/
//#define MMC_TEST

/*--------------------------------------------------------------------------*/
/* Debug Definition                                                         */
/*--------------------------------------------------------------------------*/
#define MMC_DEBUG               (0)
#define MSDC_DEBUG              (0)

#if MSDC_DEBUG
#define MSG_DEBUG
#endif

#define MSDC_TUNE_LOG           (1)

#define MSDC_SLT                (0)
#define MSDC_SLT_MASK_LOG       (1)
#define MSDC_SLT_ANALYSIS       (0)
#if MSDC_SLT_ANALYSIS
#undef FEATURE_MMC_WR_TUNING
#undef FEATURE_MMC_RD_TUNING
#undef FEATURE_MMC_CM_TUNING
#endif

//#define USE_SDIO_1V8


#endif /* end of _MSDC_CFG_H_ */

