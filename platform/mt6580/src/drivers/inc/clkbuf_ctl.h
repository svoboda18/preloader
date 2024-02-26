/**
 * @file    mt_clk_buf_ctl.c
 * @brief   Driver for RF clock buffer control
 *
 */
#ifndef __MT_CLK_BUF_CTL_H__
#define __MT_CLK_BUF_CTL_H__

#ifdef __KERNEL__
#include <linux/kernel.h>
#include <linux/mutex.h>
#include <cust_clk_buf.h>
#else
#ifdef MTKDRV_CLKBUF_CTL // for CTP
#else
#include <gpio.h>
#endif
#endif

#if 0
#define GPIO53 53
#define GPIO54 54
#define GPIO55 55
#define GPIO56 56
#define GPIO57 57
#endif

#if 1//TODO, need add Pad name@DCT tool
#ifndef GPIO_RFIC0_BSI_CS
#define GPIO_RFIC0_BSI_CS         (GPIO53|0x80000000)    /* RFIC0_BSI_CS = GPIO53 */
#endif
#ifndef GPIO_RFIC0_BSI_CK
#define GPIO_RFIC0_BSI_CK         (GPIO54|0x80000000)    /* RFIC0_BSI_CK = GPIO54 */
#endif
#ifndef GPIO_RFIC0_BSI_D0
#define GPIO_RFIC0_BSI_D0         (GPIO55|0x80000000)    /* RFIC0_BSI_D0 = GPIO55 */
#endif
#ifndef GPIO_RFIC0_BSI_D1
#define GPIO_RFIC0_BSI_D1         (GPIO56|0x80000000)    /* RFIC0_BSI_D1 = GPIO56 */
#endif
#ifndef GPIO_RFIC0_BSI_D2
#define GPIO_RFIC0_BSI_D2         (GPIO57|0x80000000)    /* RFIC0_BSI_D2 = GPIO57 */
#endif
#endif

#ifdef __KERNEL__
#define RF_CK_BUF_REG            (spm_base + 0x620)
#define RF_CK_BUF_REG_SET        (spm_base + 0x624)
#define RF_CK_BUF_REG_CLR        (spm_base + 0x628)
#else
#define RF_CK_BUF_REG            0x10006620
#define RF_CK_BUF_REG_SET        0x10006624
#define RF_CK_BUF_REG_CLR        0x10006628
#endif

#define CLK_BUF_BB_GRP                  (0x1<<0)
#define CLK_BUF_BB_GRP_MASK             (0x1<<0)
#define CLK_BUF_BB_GRP_GET(x)           (((x)&CLK_BUF_BB_GRP_MASK)>>0)
#define CLK_BUF_BB_GRP_SET(x)           (((x)&0x1)<<0)
#define CLK_BUF_CONN_GRP                (0x1<<8)
#define CLK_BUF_CONN_GRP_MASK           (0x3<<8)
#define CLK_BUF_CONN_GRP_GET(x)         (((x)&CLK_BUF_CONN_GRP_MASK)>>8)
#define CLK_BUF_CONN_GRP_SET(x)         (((x)&0x3)<<8)
#define CLK_BUF_CONN_SEL                (0x80<<8)
#define CLK_BUF_NFC_GRP                 (0x1<<16)
#define CLK_BUF_NFC_GRP_MASK            (0x3<<16)
#define CLK_BUF_NFC_GRP_GET(x)          (((x)&CLK_BUF_NFC_GRP_MASK)>>16)
#define CLK_BUF_NFC_GRP_SET(x)          (((x)&0x3)<<16)
#define CLK_BUF_NFC_SEL                 (0x80<<16)
#define CLK_BUF_AUDIO_GRP               (0x1<<24)
#define CLK_BUF_AUDIO_GRP_MASK          (0x1<<24)
#define CLK_BUF_AUDIO_GRP_GET(x)        (((x)&CLK_BUF_AUDIO_GRP_MASK)>>24)
#define CLK_BUF_AUDIO_GRP_SET(x)        (((x)&0x1)<<24)
#define CLK_BUF_AUDIO_SEL               (0x80<<24)


enum clk_buf_id
{
    CLK_BUF_BB              = 0,
    CLK_BUF_CONN            = CLK_BUF_CONN_GRP_SET(1),
    CLK_BUF_CONNSRC         = CLK_BUF_CONN_GRP_SET(2),
    CLK_BUF_NFC             = CLK_BUF_NFC_GRP_SET(1),
    CLK_BUF_NFCSRC          = CLK_BUF_NFC_GRP_SET(2),
    CLK_BUF_NFC_NFCSRC      = CLK_BUF_NFC_GRP_SET(3),
    CLK_BUF_AUDIO           = CLK_BUF_AUDIO_GRP_SET(1),
};
typedef enum
{
    CLK_BUF_HW_SPM          = -1,
    CLK_BUF_SW_DISABLE      = 0,
    CLK_BUF_SW_ENABLE       = 1,
    CLK_BUF_SW_CONNSRC      = 2, // for RF_CLK_CFG bit 9
    CLK_BUF_SW_NFCSRC       = 2, // for RF_CLK_CFG bit 17
    CLK_BUF_SW_NFC_NFCSRC   = 3,
} CLK_BUF_SWCTRL_STATUS_T;
#define CLKBUF_NUM      4

#define STA_CLK_ON      1
#define STA_CLK_OFF     0

#ifdef __KERNEL__
bool clk_buf_ctrl(enum clk_buf_id id, bool onoff);
void clk_buf_get_swctrl_status(CLK_BUF_SWCTRL_STATUS_T *status);
bool clk_buf_init(void);

extern struct mutex clk_buf_ctrl_lock;
#else
void clk_buf_all_on(void);
void clk_buf_all_default(void);
#endif
#endif

