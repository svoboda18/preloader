/**
* @file    mt_clk_buf_ctl.c
* @brief   Driver for RF clock buffer control
*
*/
#ifndef __MT_CLK_BUF_CTL_H__
#define __MT_CLK_BUF_CTL_H__

#include <gpio.h>

enum clk_buf_id{
    CLK_BUF_BB_MD		= 0,
    CLK_BUF_CONN		= 1,
    CLK_BUF_NFC			= 2,
    CLK_BUF_AUDIO		= 3,
    CLK_BUF_INVALID		= 4,
};
typedef enum
{
   CLK_BUF_SW_DISABLE = 0,
   CLK_BUF_SW_ENABLE  = 1,
}CLK_BUF_SWCTRL_STATUS_T;

#define CLKBUF_NUM         4
#define STA_CLK_ON      1
#define STA_CLK_OFF     0

void clk_buf_all_on(void);
void clk_buf_enable_clkbuf4(void);
void clk_buf_disable_clkbuf4(void);
bool is_clk_buf_from_pmic(void);
void clk_buf_dump_log(void);
#endif

