/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2010
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

#ifndef _MSDC_H_
#define _MSDC_H_

#include "cust_msdc.h"
#include "msdc_cfg.h"
#include "msdc_utils.h"
#include "mmc_core.h"
#include "msdc_irq.h"
#include "mmc_test.h"

#if defined(MMC_MSDC_DRV_PRELOADER)
#include "platform.h"
#include "pll.h"
#include "sync_write.h"
#endif

#if defined(MMC_MSDC_DRV_LK)
#include "sync_write.h"
#include "mt_reg_base.h"
#endif

#if defined(MMC_MSDC_DRV_CTP)
#include "reg_base.H"
#include "intrCtrl.h"

#endif

#if defined(MSDC_ENABLE_DMA_MODE)
#include "msdc_dma.h"
#endif

//#define MTK_MSDC_DUMP_FIFO

/*--------------------------------------------------------------------------*/
/* Common Macro                                                             */
/*--------------------------------------------------------------------------*/
#define REG_ADDR(x)             ((volatile uint32*)(base + OFFSET_##x))

/*--------------------------------------------------------------------------*/
/* Common Definition                                                        */
/*--------------------------------------------------------------------------*/
#define MSDC_FIFO_SZ            (128)
#define MSDC_FIFO_THD           (64)

#define MSDC_MS                 (0)
#define MSDC_SDMMC              (1)

#define MSDC_MODE_UNKNOWN       (0)
#define MSDC_MODE_PIO           (1)
#define MSDC_MODE_DMA_BASIC     (2)
#define MSDC_MODE_DMA_DESC      (3)
#define MSDC_MODE_DMA_ENHANCED  (4)

#if defined(MSDC_MODE_DEFAULT_PIO)
#define MSDC_MODE_DEFAULT   MSDC_MODE_PIO
#elif defined(MSDC_MODE_DEFAULT_DMA_BASIC)
#define MSDC_MODE_DEFAULT   MSDC_MODE_DMA_BASIC
#elif defined(MSDC_MODE_DEFAULT_DMA_DESC)
#define MSDC_MODE_DEFAULT   MSDC_MODE_DMA_DESC
#elif defined(MSDC_MODE_DEFAULT_DMA_ENHANCED)
#define MSDC_MODE_DEFAULT   MSDC_MODE_DMA_ENHANCED
#else
#define MSDC_MODE_DEFAULT   MSDC_MODE_UNKNOWN
#endif

#define MSDC_BUS_1BITS          (0)
#define MSDC_BUS_4BITS          (1)
#define MSDC_BUS_8BITS          (2)

#define MSDC_BRUST_8B           (3)
#define MSDC_BRUST_16B          (4)
#define MSDC_BRUST_32B          (5)
#define MSDC_BRUST_64B          (6)

#define MSDC_PIN_PULL_NONE      (0)
#define MSDC_PIN_PULL_DOWN      (1)
#define MSDC_PIN_PULL_UP        (2)
#define MSDC_PIN_KEEP           (3)

#if defined(FPGA_PLATFORM)
#define MSDC_OP_SCLK            (12000000)
#define MSDC_SRC_CLK            (12000000)
#define MSDC_MAX_SCLK           (MSDC_SRC_CLK>>1)
#else
#define MSDC_OP_SCLK            (200000000)
#define MSDC_MAX_SCLK           (800000000)
#endif

#define MSDC_MIN_SCLK           (260000)

#define MSDC_350K_SCLK          (350000)
#define MSDC_400K_SCLK          (400000)
#define MSDC_25M_SCLK           (25000000)
#define MSDC_26M_SCLK           (26000000)
#define MSDC_50M_SCLK           (50000000)
#define MSDC_52M_SCLK           (52000000)
#define MSDC_100M_SCLK          (100000000)
#define MSDC_179M_SCLK          (179000000)
#define MSDC_200M_SCLK          (200000000)
#define MSDC_208M_SCLK          (208000000)
#define MSDC_400M_SCLK          (400000000)
#define MSDC_800M_SCLK          (800000000)

#define MSDC_CLK_CFG_3_OFFSET	(0x070 )
#define MSDC_CLK_CFG_3_MSDC30_MASK	(0xf << 0 )
#define MSDC_CLK_CFG_3_MSDC31_MASK	(0xf << 8 )


#define MSDC_AUTOCMD12          (0x0001)
#define MSDC_AUTOCMD23          (0x0002)
#define MSDC_AUTOCMD19          (0x0003)

#define TYPE_CMD_RESP_EDGE      (0)
#define TYPE_WRITE_CRC_EDGE     (1)
#define TYPE_READ_DATA_EDGE     (2)
#define TYPE_WRITE_DATA_EDGE    (3)

#define START_AT_RISING                 (0x0)
#define START_AT_FALLING                (0x1)
#define START_AT_RISING_AND_FALLING     (0x2)
#define START_AT_RISING_OR_FALLING      (0x3)

/* Tuning Parameter */
#define DEFAULT_DEBOUNCE   (8)    /* 8 cycles */
#define DEFAULT_DTOC       (40)   /* data timeout counter. 65536x40 sclk. */
#define DEFAULT_WDOD       (0)    /* write data output delay. no delay. */
#define DEFAULT_BSYDLY     (8)    /* card busy delay. 8 extend sclk */

/* Timeout Parameter */
#define TMO_IN_CLK_2POWER   20              /* 2^20=1048576 */
#define EMMC_BOOT_TMO_IN_CLK_2POWER    16   /* 2^16=65536   */

/*--------------------------------------------------------------------------*/
/* Register Offset                                                          */
/*--------------------------------------------------------------------------*/
#define OFFSET_MSDC_CFG                  (0x0)
#define OFFSET_MSDC_IOCON                (0x04)
#define OFFSET_MSDC_PS                   (0x08)
#define OFFSET_MSDC_INT                  (0x0c)
#define OFFSET_MSDC_INTEN                (0x10)
#define OFFSET_MSDC_FIFOCS               (0x14)
#define OFFSET_MSDC_TXDATA               (0x18)
#define OFFSET_MSDC_RXDATA               (0x1c)
#define OFFSET_SDC_CFG                   (0x30)
#define OFFSET_SDC_CMD                   (0x34)
#define OFFSET_SDC_ARG                   (0x38)
#define OFFSET_SDC_STS                   (0x3c)
#define OFFSET_SDC_RESP0                 (0x40)
#define OFFSET_SDC_RESP1                 (0x44)
#define OFFSET_SDC_RESP2                 (0x48)
#define OFFSET_SDC_RESP3                 (0x4c)
#define OFFSET_SDC_BLK_NUM               (0x50)
#define OFFSET_SDC_VOL_CHG               (0x54)
#define OFFSET_SDC_CSTS                  (0x58)
#define OFFSET_SDC_CSTS_EN               (0x5c)
#define OFFSET_SDC_DCRC_STS              (0x60)
#define OFFSET_EMMC_CFG0                 (0x70)
#define OFFSET_EMMC_CFG1                 (0x74)
#define OFFSET_EMMC_STS                  (0x78)
#define OFFSET_EMMC_IOCON                (0x7c)
#define OFFSET_SDC_ACMD_RESP             (0x80)
#define OFFSET_SDC_ACMD19_TRG            (0x84)
#define OFFSET_SDC_ACMD19_STS            (0x88)
#define OFFSET_MSDC_DMA_SA_HIGH4BIT      (0x8C)
#define OFFSET_MSDC_DMA_SA               (0x90)
#define OFFSET_MSDC_DMA_CA               (0x94)
#define OFFSET_MSDC_DMA_CTRL             (0x98)
#define OFFSET_MSDC_DMA_CFG              (0x9c)
#define OFFSET_MSDC_DBG_SEL              (0xa0)
#define OFFSET_MSDC_DBG_OUT              (0xa4)
#define OFFSET_MSDC_DMA_LEN              (0xa8)
#define OFFSET_MSDC_PATCH_BIT0           (0xb0)
#define OFFSET_MSDC_PATCH_BIT1           (0xb4)
#define OFFSET_MSDC_PATCH_BIT2           (0xb8)
#define OFFSET_DAT0_TUNE_CRC             (0xc0)
#define OFFSET_DAT1_TUNE_CRC             (0xc4)
#define OFFSET_DAT2_TUNE_CRC             (0xc8)
#define OFFSET_DAT3_TUNE_CRC             (0xcc)
#define OFFSET_CMD_TUNE_CRC              (0xd0)
#define OFFSET_SDIO_TUNE_WIND            (0xd4)
#define OFFSET_MSDC_PAD_TUNE0            (0xf0)
#define OFFSET_MSDC_DAT_RDDLY0           (0xf8)
#define OFFSET_MSDC_DAT_RDDLY1           (0xfc)
#define OFFSET_MSDC_HW_DBG               (0x110)
#define OFFSET_MSDC_VERSION              (0x114)
#define OFFSET_MSDC_ECO_VER              (0x118)
#define OFFSET_EMMC50_PAD_CTL0           (0x180)
#define OFFSET_EMMC50_PAD_DS_CTL0        (0x184)
#define OFFSET_EMMC50_PAD_DS_TUNE        (0x188)
#define OFFSET_EMMC50_PAD_CMD_TUNE       (0x18c)
#define OFFSET_EMMC50_PAD_DAT01_TUNE     (0x190)
#define OFFSET_EMMC50_PAD_DAT23_TUNE     (0x194)
#define OFFSET_EMMC50_PAD_DAT45_TUNE     (0x198)
#define OFFSET_EMMC50_PAD_DAT67_TUNE     (0x19c)
#define OFFSET_EMMC51_CFG0               (0x204)
#define OFFSET_EMMC50_CFG0               (0x208)
#define OFFSET_EMMC50_CFG1               (0x20c)
#define OFFSET_EMMC50_CFG2               (0x21c)
#define OFFSET_EMMC50_CFG3               (0x220)
#define OFFSET_EMMC50_CFG4               (0x224)

/*--------------------------------------------------------------------------*/
/* Register Address                                                         */
/*--------------------------------------------------------------------------*/
/* common register */
#define MSDC_CFG                    REG_ADDR(MSDC_CFG)
#define MSDC_IOCON                  REG_ADDR(MSDC_IOCON)
#define MSDC_PS                     REG_ADDR(MSDC_PS)
#define MSDC_INT                    REG_ADDR(MSDC_INT)
#define MSDC_INTEN                  REG_ADDR(MSDC_INTEN)
#define MSDC_FIFOCS                 REG_ADDR(MSDC_FIFOCS)
#define MSDC_TXDATA                 REG_ADDR(MSDC_TXDATA)
#define MSDC_RXDATA                 REG_ADDR(MSDC_RXDATA)

/* sdmmc register */
#define SDC_CFG                     REG_ADDR(SDC_CFG)
#define SDC_CMD                     REG_ADDR(SDC_CMD)
#define SDC_ARG                     REG_ADDR(SDC_ARG)
#define SDC_STS                     REG_ADDR(SDC_STS)
#define SDC_RESP0                   REG_ADDR(SDC_RESP0)
#define SDC_RESP1                   REG_ADDR(SDC_RESP1)
#define SDC_RESP2                   REG_ADDR(SDC_RESP2)
#define SDC_RESP3                   REG_ADDR(SDC_RESP3)
#define SDC_BLK_NUM                 REG_ADDR(SDC_BLK_NUM)
#define SDC_VOL_CHG                 REG_ADDR(SDC_VOL_CHG)
#define SDC_CSTS                    REG_ADDR(SDC_CSTS)
#define SDC_CSTS_EN                 REG_ADDR(SDC_CSTS_EN)
#define SDC_DCRC_STS                REG_ADDR(SDC_DCRC_STS)

/* emmc register*/
#define EMMC_CFG0                   REG_ADDR(EMMC_CFG0)
#define EMMC_CFG1                   REG_ADDR(EMMC_CFG1)
#define EMMC_STS                    REG_ADDR(EMMC_STS)
#define EMMC_IOCON                  REG_ADDR(EMMC_IOCON)

/* auto command register */
#define SDC_ACMD_RESP               REG_ADDR(SDC_ACMD_RESP)
#define SDC_ACMD19_TRG              REG_ADDR(SDC_ACMD19_TRG)
#define SDC_ACMD19_STS              REG_ADDR(SDC_ACMD19_STS)

/* dma register */
#define MSDC_DMA_SA_HIGH4BIT        REG_ADDR(MSDC_DMA_SA_HIGH4BIT)
#define MSDC_DMA_SA                 REG_ADDR(MSDC_DMA_SA)
#define MSDC_DMA_CA                 REG_ADDR(MSDC_DMA_CA)
#define MSDC_DMA_CTRL               REG_ADDR(MSDC_DMA_CTRL)
#define MSDC_DMA_CFG                REG_ADDR(MSDC_DMA_CFG)
#define MSDC_DMA_LEN                REG_ADDR(MSDC_DMA_LEN)

/* data read delay */
#define MSDC_DAT_RDDLY0             REG_ADDR(MSDC_DAT_RDDLY0)
#define MSDC_DAT_RDDLY1             REG_ADDR(MSDC_DAT_RDDLY1)

/* debug register */
#define MSDC_DBG_SEL                REG_ADDR(MSDC_DBG_SEL)
#define MSDC_DBG_OUT                REG_ADDR(MSDC_DBG_OUT)

/* misc register */
#define MSDC_PATCH_BIT0             REG_ADDR(MSDC_PATCH_BIT0)
#define MSDC_PATCH_BIT1             REG_ADDR(MSDC_PATCH_BIT1)
#define MSDC_PATCH_BIT2             REG_ADDR(MSDC_PATCH_BIT2)
#define DAT0_TUNE_CRC               REG_ADDR(DAT0_TUNE_CRC)
#define DAT1_TUNE_CRC               REG_ADDR(DAT1_TUNE_CRC)
#define DAT2_TUNE_CRC               REG_ADDR(DAT2_TUNE_CRC)
#define DAT3_TUNE_CRC               REG_ADDR(DAT3_TUNE_CRC)
#define CMD_TUNE_CRC                REG_ADDR(CMD_TUNE_CRC)
#define SDIO_TUNE_WIND              REG_ADDR(SDIO_TUNE_WIND)
#define MSDC_PAD_TUNE0              REG_ADDR(MSDC_PAD_TUNE0)
#define MSDC_HW_DBG                 REG_ADDR(MSDC_HW_DBG)
#define MSDC_VERSION                REG_ADDR(MSDC_VERSION)
#define MSDC_ECO_VER                REG_ADDR(MSDC_ECO_VER)

/* eMMC 5.0 register */
#define EMMC50_PAD_CTL0             REG_ADDR(EMMC50_PAD_CTL0)
#define EMMC50_PAD_DS_CTL0          REG_ADDR(EMMC50_PAD_DS_CTL0)
#define EMMC50_PAD_DS_TUNE          REG_ADDR(EMMC50_PAD_DS_TUNE)
#define EMMC50_PAD_CMD_TUNE         REG_ADDR(EMMC50_PAD_CMD_TUNE)
#define EMMC50_PAD_DAT01_TUNE       REG_ADDR(EMMC50_PAD_DAT01_TUNE)
#define EMMC50_PAD_DAT23_TUNE       REG_ADDR(EMMC50_PAD_DAT23_TUNE)
#define EMMC50_PAD_DAT45_TUNE       REG_ADDR(EMMC50_PAD_DAT45_TUNE)
#define EMMC50_PAD_DAT67_TUNE       REG_ADDR(EMMC50_PAD_DAT67_TUNE)
#define EMMC51_CFG0                 REG_ADDR(EMMC51_CFG0)
#define EMMC50_CFG0                 REG_ADDR(EMMC50_CFG0)
#define EMMC50_CFG1                 REG_ADDR(EMMC50_CFG1)
#define EMMC50_CFG2                 REG_ADDR(EMMC50_CFG2)
#define EMMC50_CFG3                 REG_ADDR(EMMC50_CFG3)
#define EMMC50_CFG4                 REG_ADDR(EMMC50_CFG4)

/*--------------------------------------------------------------------------*/
/* Register Mask                                                            */
/*--------------------------------------------------------------------------*/

/* MSDC_CFG mask */
#define MSDC_CFG_MODE           (0x1  << 0)     /* RW */
#define MSDC_CFG_CKPDN          (0x1  << 1)     /* RW */
#define MSDC_CFG_RST            (0x1  << 2)     /* A0 */
#define MSDC_CFG_PIO            (0x1  << 3)     /* RW */
#define MSDC_CFG_CKDRVEN        (0x1  << 4)     /* RW */
#define MSDC_CFG_BV18SDT        (0x1  << 5)     /* RW */
#define MSDC_CFG_BV18PSS        (0x1  << 6)     /* R  */
#define MSDC_CFG_CKSTB          (0x1  << 7)     /* R  */
#define MSDC_CFG_CKDIV          (0xfff<< 8)     /* RW */
#define MSDC_CFG_CKMOD          (0x3  << 20)    /* W1C */
#define MSDC_CFG_CKMOD_HS400    (0x1  << 22)    /* RW */
#define MSDC_CFG_START_BIT      (0x3  << 23)    /* RW */
#define MSDC_CFG_SCLK_STOP_DDR  (0x1  << 25)    /* RW */

/* MSDC_IOCON mask */
#define MSDC_IOCON_SDR104CKS    (0x1  << 0)     /* RW */
#define MSDC_IOCON_RSPL         (0x1  << 1)     /* RW */
#define MSDC_IOCON_R_D_SMPL     (0x1  << 2)     /* RW */
#define MSDC_IOCON_DDLSEL       (0x1  << 3)     /* RW */
#define MSDC_IOCON_DDR50CKD     (0x1  << 4)     /* RW */
#define MSDC_IOCON_R_D_SMPL_SEL (0x1  << 5)     /* RW */
#define MSDC_IOCON_W_D_SMPL     (0x1  << 8)     /* RW */
#define MSDC_IOCON_W_D_SMPL_SEL (0x1  << 9)     /* RW */
#define MSDC_IOCON_W_D0SPL      (0x1  << 10)    /* RW */
#define MSDC_IOCON_W_D1SPL      (0x1  << 11)    /* RW */
#define MSDC_IOCON_W_D2SPL      (0x1  << 12)    /* RW */
#define MSDC_IOCON_W_D3SPL      (0x1  << 13)    /* RW */
#define MSDC_IOCON_R_D0SPL      (0x1  << 16)    /* RW */
#define MSDC_IOCON_R_D1SPL      (0x1  << 17)    /* RW */
#define MSDC_IOCON_R_D2SPL      (0x1  << 18)    /* RW */
#define MSDC_IOCON_R_D3SPL      (0x1  << 19)    /* RW */
#define MSDC_IOCON_R_D4SPL      (0x1  << 20)    /* RW */
#define MSDC_IOCON_R_D5SPL      (0x1  << 21)    /* RW */
#define MSDC_IOCON_R_D6SPL      (0x1  << 22)    /* RW */
#define MSDC_IOCON_R_D7SPL      (0x1  << 23)    /* RW */

/* MSDC_PS mask */
#define MSDC_PS_CDEN            (0x1  << 0)     /* RW */
#define MSDC_PS_CDSTS           (0x1  << 1)     /* R  */
#define MSDC_PS_CDDEBOUNCE      (0xf  << 12)    /* RW */
#define MSDC_PS_DAT             (0xff << 16)    /* R  */
#define MSDC_PS_CMD             (0x1  << 24)    /* R  */
#define MSDC_PS_WP              (0x1UL<< 31)    /* R  */

/* MSDC_INT mask */
#define MSDC_INT_MMCIRQ         (0x1  << 0)     /* W1C */
#define MSDC_INT_CDSC           (0x1  << 1)     /* W1C */
#define MSDC_INT_ACMDRDY        (0x1  << 3)     /* W1C */
#define MSDC_INT_ACMDTMO        (0x1  << 4)     /* W1C */
#define MSDC_INT_ACMDCRCERR     (0x1  << 5)     /* W1C */
#define MSDC_INT_DMAQ_EMPTY     (0x1  << 6)     /* W1C */
#define MSDC_INT_SDIOIRQ        (0x1  << 7)     /* W1C */
#define MSDC_INT_CMDRDY         (0x1  << 8)     /* W1C */
#define MSDC_INT_CMDTMO         (0x1  << 9)     /* W1C */
#define MSDC_INT_RSPCRCERR      (0x1  << 10)    /* W1C */
#define MSDC_INT_CSTA           (0x1  << 11)    /* R   */
#define MSDC_INT_XFER_COMPL     (0x1  << 12)    /* W1C */
#define MSDC_INT_DXFER_DONE     (0x1  << 13)    /* W1C */
#define MSDC_INT_DATTMO         (0x1  << 14)    /* W1C */
#define MSDC_INT_DATCRCERR      (0x1  << 15)    /* W1C */
#define MSDC_INT_ACMD19_DONE    (0x1  << 16)    /* W1C */
#define MSDC_INT_BDCSERR        (0x1  << 17)    /* W1C */
#define MSDC_INT_GPDCSERR       (0x1  << 18)    /* W1C */
#define MSDC_INT_DMAPRO         (0x1  << 19)    /* W1C */
#define MSDC_INT_AXI_RESP_ERR   (0x1  << 23)    /* W1C */
#ifdef MTK_SDIO30_TEST_MODE_SUPPORT
#define MSDC_INT_GEAR_OUT_BOUND (0x1  << 20)    /* W1C */
#define MSDC_INT_ACMD53_DONE    (0x1  << 21)    /* W1C */
#define MSDC_INT_ACMD53_FAIL    (0x1  << 22)    /* W1C */
#endif

/* MSDC_INTEN mask */
#define MSDC_INTEN_MMCIRQ       (0x1  << 0)     /* RW */
#define MSDC_INTEN_CDSC         (0x1  << 1)     /* RW */
#define MSDC_INTEN_ACMDRDY      (0x1  << 3)     /* RW */
#define MSDC_INTEN_ACMDTMO      (0x1  << 4)     /* RW */
#define MSDC_INTEN_ACMDCRCERR   (0x1  << 5)     /* RW */
#define MSDC_INTEN_DMAQ_EMPTY   (0x1  << 6)     /* RW */
#define MSDC_INTEN_SDIOIRQ      (0x1  << 7)     /* RW */
#define MSDC_INTEN_CMDRDY       (0x1  << 8)     /* RW */
#define MSDC_INTEN_CMDTMO       (0x1  << 9)     /* RW */
#define MSDC_INTEN_RSPCRCERR    (0x1  << 10)    /* RW */
#define MSDC_INTEN_CSTA         (0x1  << 11)    /* RW */
#define MSDC_INTEN_XFER_COMPL   (0x1  << 12)    /* RW */
#define MSDC_INTEN_DXFER_DONE   (0x1  << 13)    /* RW */
#define MSDC_INTEN_DATTMO       (0x1  << 14)    /* RW */
#define MSDC_INTEN_DATCRCERR    (0x1  << 15)    /* RW */
#define MSDC_INTEN_ACMD19_DONE  (0x1  << 16)    /* RW */
#define MSDC_INTEN_BDCSERR      (0x1  << 17)    /* RW */
#define MSDC_INTEN_GPDCSERR     (0x1  << 18)    /* RW */
#define MSDC_INTEN_DMAPRO       (0x1  << 19)    /* RW */
#ifdef MTK_SDIO30_TEST_MODE_SUPPORT
#define MSDC_INTEN_GOBOUND      (0x1  << 20)    /* RW */
#define MSDC_INTEN_ACMD53_DONE  (0x1  << 21)    /* RW */
#define MSDC_INTEN_ACMD53_FAIL  (0x1  << 22)    /* RW */
#endif
#define MSDC_INTEN_AXI_RESP_ERR (0x1  << 23)    /* RW */


/* MSDC_FIFOCS mask */
#define MSDC_FIFOCS_RXCNT       (0xff << 0)     /* R  */
#define MSDC_FIFOCS_TXCNT       (0xff << 16)    /* R  */
#define MSDC_FIFOCS_CLR         (0x1UL<< 31)    /* RW */

/* SDC_CFG mask */
#define SDC_CFG_SDIOINTWKUP     (0x1  << 0)     /* RW */
#define SDC_CFG_INSWKUP         (0x1  << 1)     /* RW */
#define SDC_CFG_BUSWIDTH        (0x3  << 16)    /* RW */
#define SDC_CFG_SDIO            (0x1  << 19)    /* RW */
#define SDC_CFG_SDIOIDE         (0x1  << 20)    /* RW */
#define SDC_CFG_INTATGAP        (0x1  << 21)    /* RW */
#define SDC_CFG_DTOC            (0xffUL << 24)  /* RW */

/* SDC_CMD mask */
#define SDC_CMD_OPC             (0x3f << 0)     /* RW */
#define SDC_CMD_BRK             (0x1  << 6)     /* RW */
#define SDC_CMD_RSPTYP          (0x7  << 7)     /* RW */
#define SDC_CMD_DTYP            (0x3  << 11)    /* RW */
#define SDC_CMD_RW              (0x1  << 13)    /* RW */
#define SDC_CMD_STOP            (0x1  << 14)    /* RW */
#define SDC_CMD_GOIRQ           (0x1  << 15)    /* RW */
#define SDC_CMD_BLKLEN          (0xfff<< 16)    /* RW */
#define SDC_CMD_AUTOCMD         (0x3  << 28)    /* RW */
#define SDC_CMD_VOLSWTH         (0x1  << 30)    /* RW */
#ifdef MTK_SDIO30_TEST_MODE_SUPPORT
#define SDC_CMD_ACMD53          (0x1UL  << 31)    /* RW */
#endif
/* SDC_VOL_CHG mask */
#define SDC_VOL_CHG_CNT         (0xffff  << 0)     /* RW  */

/* SDC_STS mask */
#define SDC_STS_SDCBUSY         (0x1  << 0)     /* RW  */
#define SDC_STS_CMDBUSY         (0x1  << 1)     /* RW  */
#define SDC_STS_CMD_WR_BUSY     (0x1  << 16)    /* W1C */
#define SDC_STS_SWR_COMPL       (0x1  << 31)    /* RO  */

/* SDC_DCRC_STS mask */
#define SDC_DCRC_STS_POS        (0xff << 0)     /* RO */
#define SDC_DCRC_STS_NEG        (0xff << 8)     /* RO */

/* EMMC_CFG0 mask */
#define EMMC_CFG0_BOOTSTART     (0x1  << 0)     /* W  */
#define EMMC_CFG0_BOOTSTOP      (0x1  << 1)     /* W  */
#define EMMC_CFG0_BOOTMODE      (0x1  << 2)     /* RW */
#define EMMC_CFG0_BOOTACKDIS    (0x1  << 3)     /* RW */
#define EMMC_CFG0_BOOTWDLY      (0x7  << 12)    /* RW */
#define EMMC_CFG0_BOOTSUPP      (0x1  << 15)    /* RW */

/* EMMC_CFG1 mask */
#define EMMC_CFG1_BOOTDATTMC    (0xfffff << 0)  /* RW */
#define EMMC_CFG1_BOOTACKTMC    (0xfffUL << 20) /* RW */

/* EMMC_STS mask */
#define EMMC_STS_BOOTCRCERR     (0x1  << 0)     /* W1C */
#define EMMC_STS_BOOTACKERR     (0x1  << 1)     /* W1C */
#define EMMC_STS_BOOTDATTMO     (0x1  << 2)     /* W1C */
#define EMMC_STS_BOOTACKTMO     (0x1  << 3)     /* W1C */
#define EMMC_STS_BOOTUPSTATE    (0x1  << 4)     /* R   */
#define EMMC_STS_BOOTACKRCV     (0x1  << 5)     /* W1C */
#define EMMC_STS_BOOTDATRCV     (0x1  << 6)     /* R   */

/* EMMC_IOCON mask */
#define EMMC_IOCON_BOOTRST      (0x1  << 0)     /* RW */

/* SDC_ACMD19_TRG mask */
#define SDC_ACMD19_TRG_TUNESEL  (0xf  << 0)     /* RW */

/* MSDC_DMA_SA_HIGH4BIT */ 
#define MSDC_DMA_SURR_ADDR_HIGH4BIT (0xf  << 0)     /* RW */
/* MSDC_DMA_CTRL mask */
#define MSDC_DMA_CTRL_START     (0x1  << 0)     /* W  */
#define MSDC_DMA_CTRL_STOP      (0x1  << 1)     /* W  */
#define MSDC_DMA_CTRL_RESUME    (0x1  << 2)     /* W  */
#define MSDC_DMA_CTRL_REDAYM    (0x1  << 3)     /* RO */
#define MSDC_DMA_CTRL_MODE      (0x1  << 8)     /* RW */
#define MSDC_DMA_CTRL_ALIGN     (0x1  << 9)     /* RW */
#define MSDC_DMA_CTRL_LASTBUF   (0x1  << 10)    /* RW */
#define MSDC_DMA_CTRL_SPLI1K    (0x1  << 11)    /* RW */
#define MSDC_DMA_CTRL_BRUSTSZ   (0x7  << 12)    /* RW */

/* MSDC_DMA_CFG mask */
#define MSDC_DMA_CFG_STS        (0x1  << 0)     /* R */
#define MSDC_DMA_CFG_DECSEN     (0x1  << 1)     /* RW */
#define MSDC_DMA_CFG_LOCKDISABLE     (0x1  << 2)     /* RW */
#define MSDC_DMA_CFG_AHBEN      (0x3  << 8)     /* RW */
#define MSDC_DMA_CFG_ACTEN      (0x3  << 12)    /* RW */
#define MSDC_DMA_CFG_CS12B      (0x1  << 16)    /* RW */

/* MSDC_PATCH_BIT0 mask */
#define MSDC_PB0_RESV1           (0x1 << 0)
#define MSDC_PB0_EN_8BITSUP      (0x1 << 1)
#define MSDC_PB0_DIS_RECMDWR     (0x1 << 2)
#define MSDC_PB0_RESV2           (0x7 << 3)
#define MSDC_PB0_DESCUP          (0x1 << 6)
#define MSDC_PB0_INT_DAT_LATCH_CK_SEL (0x7 << 7)
#define MSDC_PB0_CKGEN_MSDC_DLY_SEL   (0x1F<<10)
#define MSDC_PB0_FIFORD_DIS      (0x1 << 15)
#define MSDC_PB0_BLKNUM_SEL      (0x1 << 16)
#define MSDC_PB0_SDIO_INTCSEL    (0x1 << 17)
#define MSDC_PB0_SDC_BSYDLY      (0xf << 18)
#define MSDC_PB0_SDC_WDOD        (0xf << 22)
#define MSDC_PB0_CMDIDRTSEL      (0x1 << 26)
#define MSDC_PB0_CMDFAILSEL      (0x1 << 27)
#define MSDC_PB0_SDIO_INTDLYSEL  (0x1 << 28)
#define MSDC_PB0_SPCPUSH         (0x1 << 29)
#define MSDC_PB0_DETWR_CRCTMO    (0x1 << 30)
#define MSDC_PB0_EN_DRVRSP       (0x1UL << 31)

/* MSDC_PATCH_BIT1 mask */
#define MSDC_PB1_WRDAT_CRCS_TA_CNTR   (0x7 << 0)
#define MSDC_PB1_CMD_RSP_TA_CNTR      (0x7 << 3)
#define MSDC_PB1_GET_BUSY_MA     (0x1 << 6)
#define MSDC_PB1_GET_CRC_MA      (0x1 << 7)
#define MSDC_PB1_BIAS_TUNE_28NM  (0xf << 8)
#define MSDC_PB1_BIAS_EN18IO_28NM (0x1 << 12)
#define MSDC_PB1_BIAS_EXT_28NM   (0x1 << 13)
#define MSDC_PB1_RESV2           (0x1 << 14)
#define MSDC_PB1_RESET_GDMA      (0x1 << 15)
#define MSDC_PB1_SINGLEBURST     (0x1 << 16)
#define MSDC_PB1_FROCE_STOP      (0x1 << 17)
#define MSDC_PB1_DCM_DEV_SEL2    (0x3 << 18)
#define MSDC_PB1_DCM_DEV_SEL1    (0x1 << 20)
#define MSDC_PB1_DCM_EN          (0x1 << 21)
#define MSDC_PB1_AXI_WRAP_CKEN   (0x1 << 22)
#define MSDC_PB1_AHBCKEN         (0x1 << 23)
#define MSDC_PB1_CKSPCEN         (0x1 << 24)
#define MSDC_PB1_CKPSCEN         (0x1 << 25)
#define MSDC_PB1_CKVOLDETEN      (0x1 << 26)
#define MSDC_PB1_CKACMDEN        (0x1 << 27)
#define MSDC_PB1_CKSDEN          (0x1 << 28)
#define MSDC_PB1_CKWCTLEN        (0x1 << 29)
#define MSDC_PB1_CKRCTLEN        (0x1 << 30)
#define MSDC_PB1_CKSHBFFEN       (0x1UL << 31)

/* MSDC_PATCH_BIT2 mask */
#define MSDC_PB2_ENHANCEGPD      (0x1 << 0)
#define MSDC_PB2_SUPPORT64G      (0x1 << 1)
#define MSDC_PB2_RESPWAITCNT     (0x3 << 2)
#define MSDC_PB2_CFGRDATCNT      (0x1f << 4)
#define MSDC_PB2_CFGRDAT         (0x1 << 9)
#define MSDC_PB2_INTCRESPSEL     (0x1 << 11)
#define MSDC_PB2_CFGRESPCNT      (0x7 << 12)
#define MSDC_PB2_CFGRESP         (0x1 << 15)
#define MSDC_PB2_RESPSTENSEL     (0x7 << 16)
#define MSDC_PB2_POPENCNT        (0xf << 20)
#define MSDC_PB2_CFG_CRCSTS_SEL  (0x1 << 24)
#define MSDC_PB2_CFGCRCSTSEDGE   (0x1 << 25)
#define MSDC_PB2_CFGCRCSTSCNT    (0x3 << 26)
#define MSDC_PB2_CFGCRCSTS       (0x1 << 28)
#define MSDC_PB2_CRCSTSENSEL     (0x7UL << 29)

#ifdef MTK_SDIO30_TEST_MODE_SUPPORT
#define MSDC_MASK_ACMD53_CRC_ERR_INTR   (0x1<<4)
#define MSDC_ACMD53_FAIL_ONE_SHOT       (0X1<<5)
#endif

/* MSDC_PAD_TUNE0 mask */
#define MSDC_PAD_TUNE0_DATWRDLY  (0x1F << 0)     /* RW */
#define MSDC_PAD_TUNE0_DELAYEN   (0x1 << 7)      /* RW */
#define MSDC_PAD_TUNE0_DATRRDLY  (0x1F << 8)     /* RW */
#define MSDC_PAD_TUNE0_CMDRDLY   (0x1F << 16)    /* RW */
#define MSDC_PAD_TUNE0_CMDRRDLY  (0x1FUL << 22)  /* RW */
#define MSDC_PAD_TUNE0_CLKTXDLY  (0x1FUL << 27)  /* RW */

/* MSDC_DAT_RDDLY0/1 mask */
#define MSDC_DAT_RDDLY0_D3      (0x1F << 0)     /* RW */
#define MSDC_DAT_RDDLY0_D2      (0x1F << 8)     /* RW */
#define MSDC_DAT_RDDLY0_D1      (0x1F << 16)    /* RW */
#define MSDC_DAT_RDDLY0_D0      (0x1FUL << 24)  /* RW */

#define MSDC_DAT_RDDLY1_D7      (0x1F << 0)     /* RW */
#define MSDC_DAT_RDDLY1_D6      (0x1F << 8)     /* RW */
#define MSDC_DAT_RDDLY1_D5      (0x1F << 16)    /* RW */
#define MSDC_DAT_RDDLY1_D4      (0x1FUL << 24)  /* RW */

/* MSDC_HW_DBG_SEL mask */
#define MSDC_HW_DBG0_SEL         (0xff << 0)
#define MSDC_HW_DBG1_SEL         (0x3f << 8)
#define MSDC_HW_DBG2_SEL         (0xff << 16)
#define MSDC_HW_DBG3_SEL         (0x3f << 24)
#define MSDC_HW_DBG_WRAPTYPE_SEL (0x1  << 30)

/* EMMC50_PAD_DS_TUNE mask */
#define MSDC_EMMC50_PAD_DS_TUNE_DLYSEL  (0x1 << 0)
#define MSDC_EMMC50_PAD_DS_TUNE_DLY2SEL (0x1 << 1)
#define MSDC_EMMC50_PAD_DS_TUNE_DLY1    (0x1f << 2)
#define MSDC_EMMC50_PAD_DS_TUNE_DLY2    (0x1f << 7)
#define MSDC_EMMC50_PAD_DS_TUNE_DLY3    (0x1F << 12)

/* EMMC50_PAD_CMD_TUNE mask */
#define MSDC_EMMC50_PAD_CMD_TUNE_DLY3SEL (0x1 << 0)
#define MSDC_EMMC50_PAD_CMD_TUNE_RXDLY3  (0x1f << 1)
#define MSDC_EMMC50_PAD_CMD_TUNE_TXDLY   (0x1f << 6)

/* EMMC50_PAD_DAT01_TUNE mask */
#define MSDC_EMMC50_PAD_DAT0_RXDLY3SEL   (0x1 << 0)
#define MSDC_EMMC50_PAD_DAT0_RXDLY3      (0x1f << 1)
#define MSDC_EMMC50_PAD_DAT0_TXDLY       (0x1f << 6)
#define MSDC_EMMC50_PAD_DAT1_RXDLY3SEL   (0x1 << 16)
#define MSDC_EMMC50_PAD_DAT1_RXDLY3      (0x1f << 17)
#define MSDC_EMMC50_PAD_DAT1_TXDLY       (0x1f << 22)

/* EMMC50_PAD_DAT23_TUNE mask */
#define MSDC_EMMC50_PAD_DAT2_RXDLY3SEL   (0x1 << 0)
#define MSDC_EMMC50_PAD_DAT2_RXDLY3      (0x1f << 1)
#define MSDC_EMMC50_PAD_DAT2_TXDLY       (0x1f << 6)
#define MSDC_EMMC50_PAD_DAT3_RXDLY3SEL   (0x1 << 16)
#define MSDC_EMMC50_PAD_DAT3_RXDLY3      (0x1f << 17)
#define MSDC_EMMC50_PAD_DAT3_TXDLY       (0x1f << 22)

/* EMMC50_PAD_DAT45_TUNE mask */
#define MSDC_EMMC50_PAD_DAT4_RXDLY3SEL   (0x1 << 0)
#define MSDC_EMMC50_PAD_DAT4_RXDLY3      (0x1f << 1)
#define MSDC_EMMC50_PAD_DAT4_TXDLY       (0x1f << 6)
#define MSDC_EMMC50_PAD_DAT5_RXDLY3SEL   (0x1 << 16)
#define MSDC_EMMC50_PAD_DAT5_RXDLY3      (0x1f << 17)
#define MSDC_EMMC50_PAD_DAT5_TXDLY       (0x1f << 22)

/* EMMC50_PAD_DAT67_TUNE mask */
#define MSDC_EMMC50_PAD_DAT6_RXDLY3SEL   (0x1 << 0)
#define MSDC_EMMC50_PAD_DAT6_RXDLY3      (0x1f << 1)
#define MSDC_EMMC50_PAD_DAT6_TXDLY       (0x1f << 6)
#define MSDC_EMMC50_PAD_DAT7_RXDLY3SEL   (0x1 << 16)
#define MSDC_EMMC50_PAD_DAT7_RXDLY3      (0x1f << 17)
#define MSDC_EMMC50_PAD_DAT7_TXDLY       (0x1f << 22)

/* EMMC51_CFG0 mask */
#define MSDC_EMMC51_CFG0_CMDQ_EN         (0x1 << 0)
#define MSDC_EMMC51_CFG0_WDAT_CNT        (0x3ff << 1)
#define MSDC_EMMC51_CFG0_RDAT_CNT        (0x3ff << 11)
#define MSDC_EMMC51_CFG0_CMDQ_CMD_EN     (0x1 << 21)
/* EMMC50_CFG0 mask */
#define MSDC_EMMC50_CFG_PADCMD_LATCHCK      (0x1 << 0)
#define MSDC_EMMC50_CFG_CRC_STS_CNT         (0x3 << 1)
#define MSDC_EMMC50_CFG_CRC_STS_EDGE        (0x1 << 3)
#define MSDC_EMMC50_CFG_CRC_STS_SEL         (0x1 << 4)
#define MSDC_EMMC50_CFG_END_BIT_CHK_CNT     (0xf << 5)
#define MSDC_EMMC50_CFG_CMD_RESP_SEL        (0x1 << 9)
#define MSDC_EMMC50_CFG_CMD_EDGE_SEL        (0x1 << 10)
#define MSDC_EMMC50_CFG_ENDBIT_CNT          (0x3ff << 11)
#define MSDC_EMMC50_CFG_READ_DAT_CNT        (0x7 << 21)
#define MSDC_EMMC50_CFG_EMMC50_MON_SEL      (0x1 << 24)
#define MSDC_EMMC50_CFG_MSDC_WR_VALID       (0x1 << 25)
#define MSDC_EMMC50_CFG_MSDC_RD_VALID       (0x1 << 26)
#define MSDC_EMMC50_CFG_MSDC_WR_VALID_SEL   (0x1 << 27)
#define MSDC_EMMC50_CFG_EMMC50_MON_SE       (0x1 << 28)
#define MSDC_EMMC50_CFG_EMMC50_TXSKEWSEL    (0x1 << 29)

/* EMMC50_CFG1 mask */
#define MSDC_EMMC50_CFG1_WRPTR_MARGIN    (0xff << 0)
#define MSDC_EMMC50_CFG1_CKSWITCH_CNT    (0x7  << 8)
#define MSDC_EMMC50_CFG1_RDDAT_STOP      (0x1  << 11)
#define MSDC_EMMC50_CFG1_WAITCLK_CNT     (0xf  << 12)
#define MSDC_EMMC50_CFG1_DBG_SEL         (0xff << 16)
#define MSDC_EMMC50_CFG1_PSHCNT          (0x7  << 24)
#define MSDC_EMMC50_CFG1_PSHPSSEL        (0x1  << 27)
#define MSDC_EMMC50_CFG1_DSCFG           (0x1  << 28)
#define MSDC_EMMC50_CFG1_RESV0           (0x7UL  << 29)

/* EMMC50_CFG2_mask */
//#define MSDC_EMMC50_CFG2_AXI_GPD_UP            (0x1 << 0)
#define MSDC_EMMC50_CFG2_AXI_IOMMU_WR_EMI      (0x1 << 1)
#define MSDC_EMMC50_CFG2_AXI_SHARE_EN_WR_EMI   (0x1 << 2)
#define MSDC_EMMC50_CFG2_AXI_IOMMU_RD_EMI      (0x1 << 7)
#define MSDC_EMMC50_CFG2_AXI_SHARE_EN_RD_EMI   (0x1 << 8)
#define MSDC_EMMC50_CFG2_AXI_BOUND_128B        (0x1 << 13)
#define MSDC_EMMC50_CFG2_AXI_BOUND_256B        (0x1 << 14)
#define MSDC_EMMC50_CFG2_AXI_BOUND_512B        (0x1 << 15)
#define MSDC_EMMC50_CFG2_AXI_BOUND_1K          (0x1 << 16)
#define MSDC_EMMC50_CFG2_AXI_BOUND_2K          (0x1 << 17)
#define MSDC_EMMC50_CFG2_AXI_BOUND_4K          (0x1 << 18)
#define MSDC_EMMC50_CFG2_AXI_RD_OUTS_NUM       (0x1f<< 19)
#define MSDC_EMMC50_CFG2_AXI_SET_LEN           (0xf << 24)
#define MSDC_EMMC50_CFG2_AXI_RESP_ERR_TYPE     (0x3 << 28)
#define MSDC_EMMC50_CFG2_AXI_BUSY              (0x1 << 30)

/* EMMC50_CFG3_mask */
#define MSDC_EMMC50_CFG3_OUTS_WR               (0x1f << 0)
#define MSDC_EMMC50_CFG3_ULTRA_SET_WR          (0x3f << 5)
#define MSDC_EMMC50_CFG3_PREULTRA_SET_WR       (0x3f << 11)
#define MSDC_EMMC50_CFG3_ULTRA_SET_RD          (0x3f << 17)
#define MSDC_EMMC50_CFG3_PREULTRA_SET_RD       (0x3f <<23)

/* EMMC50_CFG4_mask */
#define MSDC_EMMC50_CFG4_IMPR_ULTRA_SET_WR     (0xff << 0)
#define MSDC_EMMC50_CFG4_IMPR_ULTRA_SET_RD     (0xff << 8)
#define MSDC_EMMC50_CFG4_ULTRA_EN              (0x3  << 16)
#define MSDC_EMMC50_CFG4_AXI_WRAP_DBG_SEL      (0x1f << 18)

/* SDIO_TUNE_WIND mask*/
#define MSDC_SDIO_TUNE_WIND      (0x1f << 0)





/*  
 * MSDC0~3 IO Configuration Base.
 */

#define GPIO_REG_BASE               	(0x10211000)


/*--------------------------------------------------------------------------*/
/* MSDC0 GPIO Related Register                                              */
/*--------------------------------------------------------------------------*/

/* MSDC0 related register base*/
#define MSDC0_GPIO_MODE18_ADDR			(GPIO_REG_BASE + 0x410)
#define MSDC0_GPIO_MODE19_ADDR			(GPIO_REG_BASE + 0x420)

#define MSDC0_GPIO_IES_G5_ADDR			(GPIO_REG_BASE + 0xD00)
#define MSDC0_GPIO_SMT_G5_ADDR			(GPIO_REG_BASE + 0xD10)
#define MSDC0_GPIO_TDSEL0_G5_ADDR		(GPIO_REG_BASE + 0xD20)
#define MSDC0_GPIO_RDSEL0_G5_ADDR		(GPIO_REG_BASE + 0xD28)
#define MSDC0_GPIO_DRV0_G5_ADDR			(GPIO_REG_BASE + 0xD70)

#define MSDC0_GPIO_PUPD0_G5_ADDR		(GPIO_REG_BASE + 0xD80)
#define MSDC0_GPIO_PUPD1_G5_ADDR		(GPIO_REG_BASE + 0xD90)


/* MSDC0 mode mask*/
#define MSDC0_MODE_CMD_MASK				(0x7  << 6 )
#define MSDC0_MODE_DSL_MASK				(0x7  << 9 )
#define MSDC0_MODE_CLK_MASK				(0x7  << 12)
#define MSDC0_MODE_DAT0_MASK			(0x7  << 16)
#define MSDC0_MODE_DAT1_MASK			(0x7  << 19)
#define MSDC0_MODE_DAT2_MASK			(0x7  << 22)
#define MSDC0_MODE_DAT3_MASK			(0x7  << 25)
#define MSDC0_MODE_DAT4_MASK			(0x7  << 28)

#define MSDC0_MODE_DAT5_MASK			(0x7  << 0 )
#define MSDC0_MODE_DAT6_MASK			(0x7  << 3 )
#define MSDC0_MODE_DAT7_MASK			(0x7  << 6 )
#define MSDC0_MODE_RSTB_MASK			(0x7  << 9 )

/* MSDC0 IES mask*/
#define MSDC0_IES_CMD_MASK				(0x1  << 0 )
#define MSDC0_IES_DSL_MASK				(0x1  << 1 )
#define MSDC0_IES_CLK_MASK				(0x1  << 2 )
#define MSDC0_IES_DAT_MASK				(0x1  << 3 )
#define MSDC0_IES_RSTB_MASK				(0x1  << 4 )
#define MSDC0_IES_ALL_MASK				(0x1f << 0 )

/* MSDC0 SMT mask*/
#define MSDC0_SMT_CMD_MASK				(0x1  << 0 )
#define MSDC0_SMT_DSL_MASK				(0x1  << 1 )
#define MSDC0_SMT_CLK_MASK				(0x1  << 2 )
#define MSDC0_SMT_DAT_MASK				(0x1  << 3 )
#define MSDC0_SMT_RSTB_MASK				(0x1  << 4 )
#define MSDC0_SMT_ALL_MASK				(0x1f << 0 )

/* MSDC0 TDSEL mask*/
#define MSDC0_TDSEL_CMD_MASK			(0xF  << 0 )
#define MSDC0_TDSEL_DSL_MASK			(0xF  << 4 )
#define MSDC0_TDSEL_CLK_MASK			(0xF  << 8 )
#define MSDC0_TDSEL_DAT_MASK			(0xF  << 12)
#define MSDC0_TDSEL_RSTB_MASK			(0xF  << 16)
#define MSDC0_TDSEL_ALL_MASK			(0xFFFFF << 0)

/* MSDC0 RDSEL mask*/
#define MSDC0_RDSEL_CMD_MASK			(0x1F  << 0 )
#define MSDC0_RDSEL_DSL_MASK			(0x1F  << 6 )
#define MSDC0_RDSEL_CLK_MASK			(0x1F  << 12)
#define MSDC0_RDSEL_DAT_MASK			(0x1F  << 18)
#define MSDC0_RDSEL_RSTB_MASK			(0x1F  << 24)
#define MSDC0_RDSEL_ALL_MASK			(0x3FFFFFFF << 0)

/* MSDC0 SR mask*/
#define MSDC0_SR_CMD_MASK				(0x1  << 3 )
#define MSDC0_SR_DSL_MASK				(0x1  << 7 )
#define MSDC0_SR_CLK_MASK				(0x1  << 11)
#define MSDC0_SR_DAT_MASK				(0x1  << 15)
#define MSDC0_SR_RSTB_MASK				(0x1  << 19)
#define MSDC0_SR_ALL_MASK				(0x11111 << 3)

/* MSDC0 DRV mask*/
#define MSDC0_DRV_CMD_MASK				(0x7  << 0 )
#define MSDC0_DRV_DSL_MASK				(0x7  << 4 )
#define MSDC0_DRV_CLK_MASK				(0x7  << 8 )
#define MSDC0_DRV_DAT_MASK				(0x7  << 12)
#define MSDC0_DRV_RSTB_MASK				(0x7  << 16)
#define MSDC0_DRV_ALL_MASK				(0x77777 << 0)

/* MSDC0 PUPD mask*/
#define MSDC0_PUPD_CMD_MASK				(0x7  << 0 )
#define MSDC0_PUPD_DSL_MASK				(0x7  << 4 )
#define MSDC0_PUPD_CLK_MASK				(0x7  << 8 )
#define MSDC0_PUPD_DAT0_MASK			(0x7  << 12)
#define MSDC0_PUPD_DAT1_MASK			(0x7  << 16)
#define MSDC0_PUPD_DAT2_MASK			(0x7  << 20)
#define MSDC0_PUPD_DAT3_MASK			(0x7  << 24)
#define MSDC0_PUPD_DAT4_MASK			(0x7  << 28)
#define MSDC0_PUPD_CMD_DSL_CLK_DAT04_MASK (0x77777777 << 0)

#define MSDC0_PUPD_DAT5_MASK			(0x7  << 0 )
#define MSDC0_PUPD_DAT6_MASK			(0x7  << 4 )
#define MSDC0_PUPD_DAT7_MASK			(0x7  << 8 )
#define MSDC0_PUPD_RSTB_MASK			(0x7  << 12)
#define MSDC0_PUPD_DAT57_RSTB_MASK		(0x7777 << 0)




/*--------------------------------------------------------------------------*/
/* MSDC1 GPIO Related Register                                              */
/*--------------------------------------------------------------------------*/
/* MSDC1 related register base*/
#define MSDC1_GPIO_MODE17_ADDR			(GPIO_REG_BASE + 0x400)
#define MSDC1_GPIO_MODE18_ADDR			(GPIO_REG_BASE + 0x410)

#define MSDC1_GPIO_IES_G4_ADDR			(GPIO_REG_BASE + 0xC00)
#define MSDC1_GPIO_SMT_G4_ADDR			(GPIO_REG_BASE + 0xC10)
#define MSDC1_GPIO_TDSEL0_G4_ADDR		(GPIO_REG_BASE + 0xC20)
#define MSDC1_GPIO_RDSEL0_G4_ADDR		(GPIO_REG_BASE + 0xC28)
#define MSDC1_GPIO_DRV0_G4_ADDR			(GPIO_REG_BASE + 0xC70)
#define MSDC1_GPIO_PUPD0_G4_ADDR		(GPIO_REG_BASE + 0xC80)


/* MSDC1 mode mask*/
#define MSDC1_MODE_CMD_MASK				(0x7  << 19)
#define MSDC1_MODE_CLK_MASK				(0x7  << 22)
#define MSDC1_MODE_DAT0_MASK			(0x7  << 25)
#define MSDC1_MODE_DAT1_MASK			(0x7  << 28)

#define MSDC1_MODE_DAT2_MASK			(0x7  << 0 )
#define MSDC1_MODE_DAT3_MASK			(0x7  << 3 )


/* MSDC1 IES mask*/
#define MSDC1_IES_CMD_MASK				(0x1  << 2 )
#define MSDC1_IES_CLK_MASK				(0x1  << 3 )
#define MSDC1_IES_DAT_MASK				(0x1  << 4 )
#define MSDC1_IES_ALL_MASK				(0x7  << 2 )

/* MSDC1 SMT mask*/
#define MSDC1_SMT_CMD_MASK				(0x1  << 2 )
#define MSDC1_SMT_CLK_MASK				(0x1  << 3 )
#define MSDC1_SMT_DAT_MASK				(0x1  << 4 )
#define MSDC1_SMT_ALL_MASK				(0x7  << 2 )

/* MSDC1 TDSEL mask*/
#define MSDC1_TDSEL_CMD_MASK			(0xF  << 8 )
#define MSDC1_TDSEL_CLK_MASK			(0xF  << 12)
#define MSDC1_TDSEL_DAT_MASK			(0xF  << 16)
#define MSDC1_TDSEL_ALL_MASK			(0xFFF << 8)

/* MSDC1 RDSEL mask*/
#define MSDC1_RDSEL_CMD_MASK			(0x1F << 12)
#define MSDC1_RDSEL_CLK_MASK			(0x1F << 18)
#define MSDC1_RDSEL_DAT_MASK			(0x1F << 24)
#define MSDC1_RDSEL_ALL_MASK			(0x7FFF <<12)

/* MSDC1 SR mask*/
#define MSDC1_SR_CMD_MASK				(0x1  << 11)
#define MSDC1_SR_CLK_MASK				(0x1  << 15)
#define MSDC1_SR_DAT_MASK				(0x1  << 19)
#define MSDC1_SR_ALL_MASK				(0x111 <<11)

/* MSDC1 DRV mask*/
#define MSDC1_DRV_CMD_MASK				(0x7  << 8 )
#define MSDC1_DRV_CLK_MASK				(0x7  << 12)
#define MSDC1_DRV_DAT_MASK				(0x7  << 16)
#define MSDC1_DRV_ALL_MASK				(0x777 << 8)

/* MSDC1 PUPD mask*/
#define MSDC1_PUPD_CMD_MASK				(0x7  << 0 )
#define MSDC1_PUPD_CLK_MASK				(0x7  << 4 )
#define MSDC1_PUPD_DAT0_MASK			(0x7  << 8 )
#define MSDC1_PUPD_DAT1_MASK			(0x7  << 12)
#define MSDC1_PUPD_DAT2_MASK			(0x7  << 16)
#define MSDC1_PUPD_DAT3_MASK			(0x7  << 20)
#define MSDC1_BIAS_TUNE_MASK			(0xF  << 24)
#define MSDC1_PUPD_CMD_CLK_DAT_MASK		(0x777777 << 0)



/*--------------------------------------------------------------------------*/
/* MSDC2 GPIO Related Register                                              */
/*--------------------------------------------------------------------------*/
/* msdc2 related register base*/
#define MSDC2_GPIO_MODE20_ADDR			(GPIO_REG_BASE + 0x430)
#define MSDC2_GPIO_MODE21_ADDR			(GPIO_REG_BASE + 0x440)

#define MSDC2_GPIO_IES_G0_ADDR			(GPIO_REG_BASE + 0x800)
#define MSDC2_GPIO_SMT_G0_ADDR			(GPIO_REG_BASE + 0x810)
#define MSDC2_GPIO_TDSEL0_G0_ADDR		(GPIO_REG_BASE + 0x820)
#define MSDC2_GPIO_RDSEL0_G0_ADDR		(GPIO_REG_BASE + 0x828)
#define MSDC2_GPIO_DRV0_G0_ADDR			(GPIO_REG_BASE + 0x870)
#define MSDC2_GPIO_PUPD0_G0_ADDR		(GPIO_REG_BASE + 0x880)

/* MSDC2 mode mask*/
#define MSDC2_MODE_CMD_MASK				(0x7  << 25)
#define MSDC2_MODE_CLK_MASK				(0x7  << 28)

#define MSDC2_MODE_DAT0_MASK			(0x7  << 0 )
#define MSDC2_MODE_DAT1_MASK			(0x7  << 3 )
#define MSDC2_MODE_DAT2_MASK			(0x7  << 6 )
#define MSDC2_MODE_DAT3_MASK			(0x7  << 9 )

/* MSDC2 IES mask*/
#define MSDC2_IES_CMD_MASK				(0x1  << 2 )
#define MSDC2_IES_CLK_MASK				(0x1  << 3 )
#define MSDC2_IES_DAT_MASK				(0x1  << 4 )
#define MSDC2_IES_ALL_MASK				(0x7  << 2 )

/* MSDC2 SMT mask*/
#define MSDC2_SMT_CMD_MASK				(0x1  << 2 )
#define MSDC2_SMT_CLK_MASK				(0x1  << 3 )
#define MSDC2_SMT_DAT_MASK				(0x1  << 4 )
#define MSDC2_SMT_ALL_MASK				(0x7  << 2 )

/* MSDC2 TDSEL mask*/
#define MSDC2_TDSEL_CMD_MASK			(0xF  << 8 )
#define MSDC2_TDSEL_CLK_MASK			(0xF  << 12)
#define MSDC2_TDSEL_DAT_MASK			(0xF  << 16)
#define MSDC2_TDSEL_ALL_MASK			(0xFFF << 8)


/* MSDC2 RDSEL mask*/
#define MSDC2_RDSEL_CMD_MASK			(0x1F << 4 )
#define MSDC2_RDSEL_CLK_MASK			(0x1F << 10)
#define MSDC2_RDSEL_DAT_MASK			(0x1F << 16)
#define MSDC2_RDSEL_ALL_MASK			(0x7FFF <<4)

/* MSDC2 SR mask*/
#define MSDC2_SR_CMD_MASK				(0x1  << 11)
#define MSDC2_SR_CLK_MASK				(0x1  << 15)
#define MSDC2_SR_DAT_MASK				(0x1  << 19)
#define MSDC2_SR_ALL_MASK				(0x111 <<11)

/* MSDC2 DRV mask*/
#define MSDC2_DRV_CMD_MASK				(0x7  << 8 )
#define MSDC2_DRV_CLK_MASK				(0x7  << 12)
#define MSDC2_DRV_DAT_MASK				(0x7  << 16)
#define MSDC2_DRV_ALL_MASK				(0x777 << 8)

/* MSDC2 PUPD mask*/
#define MSDC2_PUPD_CMD_MASK				(0x7  << 0 )
#define MSDC2_PUPD_CLK_MASK				(0x7  << 4 )
#define MSDC2_PUPD_DAT0_MASK			(0x7  << 8 )
#define MSDC2_PUPD_DAT1_MASK			(0x7  << 12)
#define MSDC2_PUPD_DAT2_MASK			(0x7  << 16)
#define MSDC2_PUPD_DAT3_MASK			(0x7  << 20)
#define MSDC2_BIAS_TUNE_MASK			(0xF  << 24)
#define MSDC2_PUPD_CMD_CLK_DAT_MASK		(0x777777 << 0)


/*
   MSDC Driving Strength Definition: specify as gear instead of driving
*/
#define MSDC_DRVN_GEAR0                 0x0
#define MSDC_DRVN_GEAR1                 0x1
#define MSDC_DRVN_GEAR2                 0x2
#define MSDC_DRVN_GEAR3                 0x3
#define MSDC_DRVN_GEAR4                 0x4
#define MSDC_DRVN_GEAR5                 0x5
#define MSDC_DRVN_GEAR6                 0x6
#define MSDC_DRVN_GEAR7                 0x7
#define MSDC_DRVN_DONT_CARE             0x0


typedef struct {
    int    pio_bits;                   /* PIO width: 32, 16, or 8bits */
    int    autocmd;
    int    cmd23_flags;
    struct dma_config  cfg;
    struct scatterlist sg[MAX_SG_POOL_SZ];
    int    alloc_gpd;
    int    alloc_bd;
    int    rdsmpl;
    int    wdsmpl;
    int    rsmpl;
    int    start_bit;
    gpd_t *active_head;
    gpd_t *active_tail;
    gpd_t *gpd_pool;
    bd_t  *bd_pool;
} msdc_priv_t;

#define DMA_FLAG_NONE       (0x00000000)
#define DMA_FLAG_EN_CHKSUM  (0x00000001)
#define DMA_FLAG_PAD_BLOCK  (0x00000002)
#define DMA_FLAG_PAD_DWORD  (0x00000004)

#define MSDC_WRITE32(addr, data)    mt_reg_sync_writel(data,(unsigned int)addr)
#define MSDC_READ32(addr)           (*(volatile uint32*)(addr))
#define MSDC_WRITE16(addr, data)    mt_reg_sync_writew(data,(unsigned int)addr)
#define MSDC_READ16(addr)           (*(volatile uint16*)(addr))
#define MSDC_WRITE8(addr, data)     mt_reg_sync_writeb(data,(unsigned int)addr)
#define MSDC_READ8(addr)            (*(volatile uint8*)(addr))

#define MSDC_SET_BIT32(addr,mask) \
	do {	\
	unsigned int tv = MSDC_READ32(addr); \
	tv |=((u32)(mask)); \
	MSDC_WRITE32(addr,tv); \
} while(0)
#define MSDC_CLR_BIT32(addr,mask) \
	do {	\
	unsigned int tv = MSDC_READ32(addr); \
	tv &= ~((u32)(mask)); \
	MSDC_WRITE32(addr,tv); \
} while(0)
#define MSDC_SET_BIT16(addr,mask) \
	do {	\
	unsigned short tv = MSDC_READ16(addr); \
	tv |= ((u16)(mask)); \
	MSDC_WRITE16(addr,tv); \
} while(0)
#define MSDC_CLR_BIT16(addr,mask) \
	do {	\
	unsigned short tv = MSDC_READ16(addr); \
	tv &= ~((u16)(mask)); \
	MSDC_WRITE16(addr,tv); \
} while(0)


#define MSDC_SET_FIELD(reg,field,val) \
    do {    \
        volatile uint32 tv = MSDC_READ32(reg); \
        tv &= ~(field); \
        tv |= ((val) << (uffs(field) - 1)); \
        MSDC_WRITE32(reg,tv); \
    } while(0)

#define MSDC_GET_FIELD(reg,field,val) \
    do {    \
        volatile uint32 tv = MSDC_READ32(reg); \
        val = ((tv & (field)) >> (uffs(field) - 1)); \
    } while(0)

#define MSDC_SET_FIELD_16(reg,field,val) \
    do {    \
        volatile uint16 tv = MSDC_READ16(reg); \
        tv &= ~(field); \
        tv |= ((val) << (uffs(field) - 1)); \
        MSDC_WRITE16(reg,tv); \
    } while(0)

#define MSDC_GET_FIELD_16(reg,field,val) \
    do {    \
        volatile uint16 tv = MSDC_READ16(reg); \
        val = ((tv & (field)) >> (uffs(field) - 1)); \
    } while(0)

#define MSDC_RETRY(expr,retry,cnt) \
    do { \
        uint32 t = cnt; \
        uint32 r = retry; \
        uint32 c = cnt; \
        while (r) { \
            if (!(expr)) break; \
            if (c-- == 0) { \
                r--; udelay(200); c = t; \
            } \
        } \
        BUG_ON(r == 0); \
    } while(0)
/*sd card change voltage wait time= (1/freq) * SDC_VOL_CHG_CNT(default 0x145)*/
#define msdc_set_vol_change_wait_count(count) \
    do { \
            MSDC_SET_FIELD(SDC_VOL_CHG,SDC_VOL_CHG_CNT, (count)); \
    } while(0)

#define MSDC_RESET() \
    do { \
        MSDC_SET_BIT32(MSDC_CFG, MSDC_CFG_RST); \
        MSDC_RETRY(MSDC_READ32(MSDC_CFG) & MSDC_CFG_RST, 5, 1000); \
    } while(0)

#define MSDC_CLR_INT() \
    do { \
        volatile uint32 val = MSDC_READ32(MSDC_INT); \
        MSDC_WRITE32(MSDC_INT, val); \
        if (MSDC_READ32(MSDC_INT)) { \
            MSG(ERR, "[ASSERT] MSDC_INT is NOT clear\n"); \
        } \
    } while(0)

#define MSDC_CLR_FIFO() \
    do { \
        MSDC_SET_BIT32(MSDC_FIFOCS, MSDC_FIFOCS_CLR); \
        MSDC_RETRY(MSDC_READ32(MSDC_FIFOCS) & MSDC_FIFOCS_CLR, 5, 1000); \
    } while(0)

#define MSDC_FIFO_WRITE32(val)  MSDC_WRITE32(MSDC_TXDATA, val)
#define MSDC_FIFO_READ32()      MSDC_READ32(MSDC_RXDATA)
#define MSDC_FIFO_WRITE16(val)  MSDC_WRITE16(MSDC_TXDATA, val)
#define MSDC_FIFO_READ16()      MSDC_READ16(MSDC_RXDATA)
#define MSDC_FIFO_WRITE8(val)   MSDC_WRITE8(MSDC_TXDATA, val)
#define MSDC_FIFO_READ8()       MSDC_READ8(MSDC_RXDATA)

#define MSDC_FIFO_WRITE(val)    MSDC_FIFO_WRITE32(val)
#define MSDC_FIFO_READ()        MSDC_FIFO_READ32()

#define MSDC_TXFIFOCNT() \
    ((MSDC_READ32(MSDC_FIFOCS) & MSDC_FIFOCS_TXCNT) >> 16)
#define MSDC_RXFIFOCNT() \
    ((MSDC_READ32(MSDC_FIFOCS) & MSDC_FIFOCS_RXCNT) >> 0)

#define MSDC_CARD_DETECTION_ON()  MSDC_SET_BIT32(MSDC_PS, MSDC_PS_CDEN)
#define MSDC_CARD_DETECTION_OFF() MSDC_CLR_BIT32(MSDC_PS, MSDC_PS_CDEN)

#define MSDC_DMA_ON()   MSDC_CLR_BIT32(MSDC_CFG, MSDC_CFG_PIO)
#define MSDC_DMA_OFF()  MSDC_SET_BIT32(MSDC_CFG, MSDC_CFG_PIO)

#define SDC_IS_BUSY()       (MSDC_READ32(SDC_STS) & SDC_STS_SDCBUSY)
#define SDC_IS_CMD_BUSY()   (MSDC_READ32(SDC_STS) & SDC_STS_CMDBUSY)

#define SDC_SEND_CMD(cmd,arg) \
    do { \
        MSDC_WRITE32(SDC_ARG, (arg)); \
        MSDC_WRITE32(SDC_CMD, (cmd)); \
    } while(0)

#define MSDC_INIT_GPD_EX(gpd,extlen,cmd,arg,blknum) \
    do { \
        ((gpd_t*)gpd)->extlen = extlen; \
        ((gpd_t*)gpd)->cmd    = cmd; \
        ((gpd_t*)gpd)->arg    = arg; \
        ((gpd_t*)gpd)->blknum = blknum; \
    }while(0)

#define MSDC_INIT_BD(bd, blkpad, dwpad, dptr, dlen) \
    do { \
        BUG_ON(dlen > 0xFFFFFFUL); \
        ((bd_t*)bd)->blkpad = blkpad; \
        ((bd_t*)bd)->dwpad  = dwpad; \
        ((bd_t*)bd)->ptr    = (void*)dptr; \
        ((bd_t*)bd)->buflen = dlen; \
    }while(0)

typedef struct msdc_reg_control {
    u32 addr;
    u32 mask;
    u32 value;
    u32 default_value;
    int (*restore_func)(int restore);
}MSDC_REG_CONTROL_T;

typedef enum MT65XX_POWER_VOL_TAG
{
    VOL_DEFAULT,
    VOL_0900 = 900,
    VOL_1000 = 1000,
    VOL_1100 = 1100,
    VOL_1200 = 1200,
    VOL_1300 = 1300,
    VOL_1350 = 1350,
    VOL_1500 = 1500,
    VOL_1800 = 1800,
    VOL_2000 = 2000,
    VOL_2100 = 2100,
    VOL_2500 = 2500,
    VOL_2800 = 2800,
    VOL_3000 = 3000,
    VOL_3300 = 3300,
    VOL_3400 = 3400,
    VOL_3500 = 3500,
    VOL_3600 = 3600
} MT65XX_POWER_VOLTAGE;

#ifdef MMC_PROFILING
static inline void msdc_timer_init(void)
{
    /* clear. CLR[1]=1, EN[0]=0 */
    MSDC_WRITE32(GPT_BASE + 0x30, 0x0);
    MSDC_WRITE32(GPT_BASE + 0x30, 0x2);

    MSDC_WRITE32(GPT_BASE + 0x38, 0);
    MSDC_WRITE32(GPT_BASE + 0x3C, 32768);

    /* 32678HZ RTC free run */
    MSDC_WRITE32(GPT_BASE + 0x34, 0x30);
    MSDC_WRITE32(GPT_BASE + 0x30, 0x32);
}
static inline void msdc_timer_start(void)
{
    *(volatile unsigned int*)(GPT_BASE + 0x30) |= (1 << 0);
}
static inline void msdc_timer_stop(void)
{
    *(volatile unsigned int*)(GPT_BASE + 0x30) &= ~(1 << 0);
}
static inline void msdc_timer_stop_clear(void)
{
    *(volatile unsigned int*)(GPT_BASE + 0x30) &= ~(1 << 0); /* stop  */
    *(volatile unsigned int*)(GPT_BASE + 0x30) |= (1 << 1);  /* clear */
}
static inline unsigned int msdc_timer_get_count(void)
{
    return MSDC_READ32(GPT_BASE + 0x38);
}
#else
#define msdc_timer_init()       do{}while(0)
#define msdc_timer_start()      do{}while(0)
#define msdc_timer_stop()       do{}while(0)
#define msdc_timer_stop_clear() do{}while(0)
#define msdc_timer_get_count()  0
#endif

#if defined(FPGA_PLATFORM)
#define msdc_set_sr(host, clk, cmd, dat, rst, ds)
#define msdc_set_rdtdsel(host, sd_18)
#define msdc_pin_pud(host, pin, mode, val)
#define msdc_set_smt(host, set_smt)
#define msdc_set_driving(host, msdc_cap, sd_1)
#endif

#if !defined(FPGA_PLATFORM)
typedef enum MT65XX_POWER_TAG {    

    MT6328_POWER_LDO_VAUX18=0,     
    MT6328_POWER_LDO_VTCXO_0,     
    MT6328_POWER_LDO_VTCXO_1,     
    MT6328_POWER_LDO_VAUD28,    
    MT6328_POWER_LDO_VCN28,     
    MT6328_POWER_LDO_VCAMA,      
    MT6328_POWER_LDO_VCN33,      
    MT6328_POWER_LDO_VUSB33,      
    MT6328_POWER_LDO_VEFUSE,    
    MT6328_POWER_LDO_VSIM1,     
    MT6328_POWER_LDO_VSIM2,       
    MT6328_POWER_LDO_VEMC33,        
    MT6328_POWER_LDO_VMCH,     
    MT6328_POWER_LDO_VTREF,      
    MT6328_POWER_LDO_VMC,    
    MT6328_POWER_LDO_VCAM_AF,       
    MT6328_POWER_LDO_VIO28,     
    MT6328_POWER_LDO_VGP1,      
    MT6328_POWER_LDO_VIBR,      
    MT6328_POWER_LDO_VCAMD,      
    MT6328_POWER_LDO_VRF18_0,      
    MT6328_POWER_LDO_VRF18_1,       
    MT6328_POWER_LDO_VIO18,      
    MT6328_POWER_LDO_VCN18,    
    MT6328_POWER_LDO_VCAM_IO,
    MT6328_POWER_LDO_VSRAM,       
    MT6328_POWER_LDO_VM,     
    
    MT65XX_POWER_LDO_DEFAULT,
    MT65XX_POWER_COUNT_END,
    MT65XX_POWER_NONE = -1
} MT65XX_POWER;

typedef struct pwr_desc {
    u32 pwr_io_id;
    u32 pwr_io_val;
    u32 *pwr_io_status;
    u32 pwr_flash_id;
    u32 pwr_flash_val;
    u32 *pwr_flash_status;

}pwr_desc_t;
#endif

/* each PLL have different gears for select
 * software can used mux interface from clock management module to select */
enum {
    MSDC50_CLKSRC_26MHZ  = 0,
    MSDC50_CLKSRC_800MHZ,
    MSDC50_CLKSRC_400MHZ,
    MSDC50_CLKSRC_200MHZ,
    MSDC50_CLKSRC_182MHZ,
    MSDC50_CLKSRC_136MHZ,
    MSDC50_CLKSRC_156MHZ,
    MSDC50_CLKSRC_416MHZ,
    MSDC50_CLKSRC_48MHZ,
    MSDC50_CLKSRC_91MHZ,
    MSDC50_CLKSRC_624MHZ
};

enum {
    MSDC30_CLKSRC_26MHZ   = 0,
    MSDC30_CLKSRC_208MHZ,
    MSDC30_CLKSRC_200MHZ,
    MSDC30_CLKSRC_182MHZ,
    MSDC30_CLKSRC_136MHZ,
    MSDC30_CLKSRC_156MHZ,
    MSDC30_CLKSRC_48MHZ,
    MSDC30_CLKSRC_91MHZ
};

#define MSDC50_CLKSRC_DEFAULT     MSDC50_CLKSRC_400MHZ
#define MSDC30_CLKSRC_DEFAULT     MSDC30_CLKSRC_200MHZ

#define MSDC_RELIABLE_WRITE     (0x1 << 0)
#define MSDC_PACKED             (0x1 << 1)
#define MSDC_TAG_REQUEST        (0x1 << 2)
#define MSDC_CONTEXT_ID         (0x1 << 3)
#define MSDC_FORCED_PROG        (0x1 << 4)

extern void msdc_set_reliable_write(struct mmc_host *host, int on);
extern void msdc_set_autocmd23_feature(struct mmc_host *host, int on);
extern void msdc_set_autocmd(struct mmc_host *host, int cmd, int on);
extern void msdc_set_pio_bits(struct mmc_host *host, int bits);
extern void msdc_clr_fifo(struct mmc_host *host);
extern void msdc_intr_unmask(struct mmc_host *host, u32 bits);
extern void msdc_intr_mask(struct mmc_host *host, u32 bits);
extern int msdc_send_cmd(struct mmc_host *host, struct mmc_command *cmd);
extern int msdc_wait_rsp(struct mmc_host *host, struct mmc_command *cmd);
extern void msdc_abort(struct mmc_host *host);
extern void msdc_irq_deinit(struct mmc_host *host);

extern int msdc_init(int id, struct mmc_host *host, int clksrc, int mode);
extern int msdc_pio_bread(struct mmc_host *host, uchar *dst, ulong src, ulong nblks);
extern int msdc_pio_bwrite(struct mmc_host *host, ulong dst, uchar *src, ulong nblks);
extern int msdc_dma_bread(struct mmc_host *host, uchar *dst, ulong src, ulong nblks);
extern int msdc_dma_bwrite(struct mmc_host *host, ulong dst, uchar *src, ulong nblks);
extern int msdc_tune_bwrite(struct mmc_host *host, ulong dst, uchar *src, ulong nblks);
extern int msdc_tune_bread(struct mmc_host *host, uchar *dst, ulong src, ulong nblks);
extern int msdc_tune_cmdrsp(struct mmc_host *host, struct mmc_command *cmd);
extern void msdc_reset_tune_counter(struct mmc_host *host);
extern void msdc_abort_handler(struct mmc_host *host, int abort_card);
extern int msdc_tune_read(struct mmc_host *host);
extern void msdc_intr_sdio(struct mmc_host *host, int enable);
extern void msdc_intr_sdio_gap(struct mmc_host * host, int enable);
extern void msdc_config_clock(struct mmc_host *host, int ddr, u32 hz, u32 hs_timing);
extern int msdc_cmd_stop(struct mmc_host *host, struct mmc_command *cmd);
extern int msdc_pio_send_sandisk_fwid(struct mmc_host *host,uchar *src);
extern int msdc_pio_get_sandisk_fwid(struct mmc_host * host,uchar * src);
extern int msdc_dma_send_sandisk_fwid(struct mmc_host *host, uchar *buf,u32 opcode, ulong nblks);
extern void msdc_set_startbit(struct mmc_host *host, u8 start_bit);
extern void msdc_set_axi_burst_len(struct mmc_host *host, u8 len);
extern void msdc_set_axi_outstanding(struct mmc_host *host, u8 rw, u8 num);
extern void msdc_set_blklen(struct mmc_host *host, u32 blklen);
extern void msdc_set_timeout(struct mmc_host *host, u64 ns, u32 clks);
extern int msdc_pio_read(struct mmc_host *host, u32 *ptr, u32 size);
extern int msdc_tune_rw_hs400(struct mmc_host *host, uchar *dst, ulong src, ulong nblks, unsigned int rw);
extern int msdc_cmd(struct mmc_host *host, struct mmc_command *cmd);
extern void msdc_set_blknum(struct mmc_host *host, u32 blknum);
extern void msdc_config_bus(struct mmc_host *host, u32 width);
#if defined(MMC_MSDC_DRV_CTP) || defined(MMC_MSDC_DRV_LK)
extern void msdc_hard_reset(struct mmc_host *host);
#endif
#if defined(MMC_MSDC_DRV_CTP)
extern int msdc_register_partial_backup_and_reset(struct mmc_host* host);
extern int msdc_register_partial_restore(struct mmc_host* host);
#endif

extern void msdc_set_autocmd(struct mmc_host *host, int cmd, int on);
extern void msdc_set_reliable_write(struct mmc_host *host, int on);

#endif /* end of _MSDC_H_ */
