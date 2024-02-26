
#ifndef _DRAMC_REGISTER_H_
#define _DRAMC_REGISTER_H_

#include "dramc_pi_api.h"

#define OLYMPUS_TO_BE_PORTING
#define SYLVIA_TO_BE_PORTING

#define POS_BANK_NUM 16  /* SW Virtual base address position */

#if ENABLE_DDRPHY_FREQ_METER
//need check base addr
// APB Module dramc_ch0_top0
#define DRAMC_CH0_TOP0_BASE (0x10228000)
// APB Module dramc_ch0_top1
#define DRAMC_CH0_TOP1_BASE (0x1022A000)
// APB Module dramc_ch0_top2
#define DRAMC_CH0_TOP2_BASE (0x1022C000)
// APB Module dramc_ch0_top3
#define DRAMC_CH0_TOP3_BASE (0x1022D000)
// APB Module chn_emi
#define CHN0_EMI_BASE (0x1022D000)
// APB Module dramc_ch1_top0
#define DRAMC_CH1_TOP0_BASE (0x10230000)
// APB Module dramc_ch1_top1
#define DRAMC_CH1_TOP1_BASE (0x10232000)
// APB Module dramc_ch1_top2
#define DRAMC_CH1_TOP2_BASE (0x10234000)
// APB Module dramc_ch1_top3
#define DRAMC_CH1_TOP3_BASE (0x10235000)
// APB Module chn_emi
#define CHN1_EMI_BASE (0x10235000)
#endif

/* SW Virtual base address */
#define Channel_A_DRAMC_NAO_BASE_VIRTUAL    0x40000
#define Channel_B_DRAMC_NAO_BASE_VIRTUAL    0x50000
#define Channel_A_DRAMC_AO_BASE_VIRTUAL	    0x60000
#define Channel_B_DRAMC_AO_BASE_VIRTUAL     0x70000
#define Channel_A_PHY_NAO_BASE_VIRTUAL      0x80000
#define Channel_B_PHY_NAO_BASE_VIRTUAL      0x90000
#define Channel_A_PHY_AO_BASE_VIRTUAL       0xa0000
#define Channel_B_PHY_AO_BASE_VIRTUAL       0xb0000
#define MAX_BASE_VIRTUAL                    0xc0000

/*
 * HW Physical base address
 * #if (fcFOR_CHIP_ID == fcCervino)
 */
#define Channel_A_DRAMC_NAO_BASE_ADDRESS    0x1022C000
#define Channel_B_DRAMC_NAO_BASE_ADDRESS    0x10234000
#define Channel_A_DRAMC_AO_BASE_ADDRESS     0x1022A000
#define Channel_B_DRAMC_AO_BASE_ADDRESS     0x10232000
#define Channel_A_PHY_NAO_BASE_ADDRESS      0x1022E000
#define Channel_B_PHY_NAO_BASE_ADDRESS      0x10236000
#define Channel_A_PHY_AO_BASE_ADDRESS       0x10228000
#define Channel_B_PHY_AO_BASE_ADDRESS       0x10230000
/* #endif */




#define SPM_BASE_ADDRESS 0x10006000

#define SPM_POWERON_CONFIG_EN (SPM_BASE_ADDRESS + 0x0000)
#define POWERON_CONFIG_EN_BCLK_CG_EN Fld(1, 0)/* [0:0] */
#define POWERON_CONFIG_EN_PROJECT_CODE Fld(16, 16)/* [31:16] */
#define SPM_POWER_ON_VAL0 (SPM_BASE_ADDRESS + 0x0004)
#define SPM_POWER_ON_VAL0_INI_PWRON_REG0_RF Fld(32, 0)/* [31:0] */
#define SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL_PCM Fld(2, 30)/* [31:30] */
#define SPM_POWER_ON_VAL0_SC_PHYPLL2_MODE_SW_PCM Fld(1, 29)/* [29:29] */
#define SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW_PCM Fld(1, 28)/* [28:28] */
#define SPM_POWER_ON_VAL0_SC_PHYPLL2_SHU_EN_PCM Fld(1, 27)/* [27:27] */
#define SPM_POWER_ON_VAL0_SC_PHYPLL_SHU_EN_PCM Fld(1, 26)/* [26:26] */
#define SPM_POWER_ON_VAL0_SC_DPHY_RXDLY_TRACK_EN Fld(1, 25)/* [25:25] */
#define SPM_POWER_ON_VAL0_SC_MPLL_S_OFF Fld(1, 24)/* [24:24] */
#define SPM_POWER_ON_VAL0_SC_DPHY_PRECAL_UP Fld(1, 23)/* [23:23] */
#define SPM_POWER_ON_VAL0_SC_DR_SHU_EN_PCM Fld(1, 22)/* [22:22] */
#define SPM_POWER_ON_VAL0_SC_CKSQ1_OFF Fld(1, 21)/* [21:21] */
#define SPM_POWER_ON_VAL0_SC_CKSQ0_OFF Fld(1, 20)/* [20:20] */
#define SPM_POWER_ON_VAL0_SC_SHU_RESTORE Fld(1, 19)/* [19:19] */
#define SPM_POWER_ON_VAL0_SC_MPLL_OFF Fld(1, 18)/* [18:18] / /BIANCO_TO_BE_PORTING */
#define SPM_POWER_ON_VAL0_SC_DPY_BCLK_ENABLE Fld(1, 17)/* [17:17] */
#define SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN_PCM Fld(1, 16)/* [16:16] */
#define SPM_POWER_ON_VAL0_SC_PHYPLL_EN_PCM Fld(1, 15)/* [15:15] */
#define SPM_POWER_ON_VAL0_SC_DPY_VREF_EN_PCM Fld(1, 14)/* [14:14] */
#define SPM_POWER_ON_VAL0_SC_DPY_DLL_CK_EN_PCM Fld(1, 13)/* [13:13] */
#define SPM_POWER_ON_VAL0_SC_DPY_DLL_EN_PCM Fld(1, 12)/* [12:12] */
#define SPM_POWER_ON_VAL0_SC_TX_TRACKING_DIS Fld(1, 11)/* [11:11] */
#define SPM_POWER_ON_VAL0_SC_MPLLOUT_OFF Fld(1, 10)/* [10:10] */
/* Empty [9:9] */
#define SPM_POWER_ON_VAL0_SC_DPY_2ND_DLL_EN_PCM Fld(1, 8)/* [8:8] */
#define SPM_POWER_ON_VAL0_SC_DMSUS_OFF_PCM Fld(1, 7)/* [7:7] */
#define SPM_POWER_ON_VAL0_SC_DPY_MODE_SW_PCM Fld(1, 6)/* [6:6] */
#define SPM_POWER_ON_VAL0_SC_MD26M_CK_OFF Fld(1, 5)/* [5:5] */
#define SPM_POWER_ON_VAL0_SC_DR_GATE_RETRY_EN_PCM Fld(1, 4)/* [4:4] */
#define SPM_POWER_ON_VAL0_SC_AXI_CK_OFF Fld(1, 3)/* [3:3] */
#define SPM_POWER_ON_VAL0_SC_MEM_CK_OFF Fld(1, 2)/* [2:2] */
#define SPM_POWER_ON_VAL0_SC_TIMER_RST Fld(1, 1)/* [1:1] */
#define SPM_POWER_ON_VAL0_SC_26M_CK_OFF Fld(1, 0)/* [0:0] */
#define SPM_POWER_ON_VAL1 (SPM_BASE_ADDRESS + 0x0008)
#define SPM_POWER_ON_VAL1_SC_DR_SHORT_QUEUE_PCM Fld(1, 31)/* [31:31] */
#define SPM_POWER_ON_VAL1_EMI_CLK_OFF_REQ_PCM Fld(1, 1)/* [1:1] */
#define SPM_CLK_CON (SPM_BASE_ADDRESS + 0x000C)
#define SPM_CLK_CON_REG_SYSCLK0_EN_CTRL Fld(2, 0)/* [1:0] */
#define SPM_CLK_CON_REG_SYSCLK1_EN_CTRL Fld(2, 2)/* [3:2] */
#define SPM_CLK_CON_REG_SYSSETTLE_SEL Fld(1, 4)/* [4:4] */
#define SPM_CLK_CON_REG_SPM_LOCK_INFRA_DCM Fld(1, 5)/* [5:5] */
#define SPM_CLK_CON_REG_EXT_SRCCLKEN_MASK Fld(3, 6)/* [8:6] */
#define SPM_CLK_CON_REG_CXO32K_REMOVE_EN_MD1 Fld(1, 9)/* [9:9] */
#define SPM_CLK_CON_REG_CXO32K_REMOVE_EN_MD2 Fld(1, 10)/* [10:10] */
#define SPM_CLK_CON_REG_CLKSQ0_SEL_CTRL Fld(1, 11)/* [11:11] */
#define SPM_CLK_CON_REG_CLKSQ1_SEL_CTRL Fld(1, 12)/* [12:12] */
#define SPM_CLK_CON_REG_SRCCLKEN0_EN Fld(1, 13)/* [13:13] */
#define SPM_CLK_CON_REG_SRCCLKEN1_EN Fld(1, 14)/* [14:14] */
#define SPM_CLK_CON_REG_SCP_DCM_EN Fld(1, 15)/* [15:15] */
#define SPM_CLK_CON_REG_SYSCLK0_SRC_MASK_B Fld(7, 16)/* [22:16] */
#define SPM_CLK_CON_REG_SYSCLK1_SRC_MASK_B Fld(7, 23)/* [29:23] */
#define SPM_PCM_PWR_IO_EN (SPM_BASE_ADDRESS + 0x002C)
#define PCM_PWR_IO_EN_RG_PCM_PWR_IO_EN Fld(8, 0)/* [7:0] */
#define PCM_PWR_IO_EN_RG_RF_SYNC_EN Fld(8, 16)/* [23:16] */

#define SPM_DRAMC_DPY_CLK_SW_CON_SEL (SPM_BASE_ADDRESS + 0x0460)
#define DRAMC_DPY_CLK_SW_CON_SEL_FULL Fld(32, 0)/* [31:0] */
#define DRAMC_DPY_CLK_SW_CON_SEL_SW_DR_GATE_RETRY_EN_SEL Fld(2, 0)/* [1:0] */
#define DRAMC_DPY_CLK_SW_CON_SEL_SW_EMI_CLK_OFF_SEL Fld(2, 2)/* [3:2] */
#define DRAMC_DPY_CLK_SW_CON_SEL_SW_DPY_MODE_SW_SEL Fld(2, 4)/* [5:4] */
#define DRAMC_DPY_CLK_SW_CON_SEL_SW_DMSUS_OFF_SEL Fld(2, 6)/* [7:6] */
#define DRAMC_DPY_CLK_SW_CON_SEL_SW_MEM_CK_OFF_SEL Fld(2, 8)/* [9:8] */
#define DRAMC_DPY_CLK_SW_CON_SEL_SW_DPY_2ND_DLL_EN_SEL Fld(2, 10)/* [11:10] */
#define DRAMC_DPY_CLK_SW_CON_SEL_SW_DPY_DLL_EN_SEL Fld(2, 12)/* [13:12] */
#define DRAMC_DPY_CLK_SW_CON_SEL_SW_DPY_DLL_CK_EN_SEL Fld(2, 14)/* [15:14] */
#define DRAMC_DPY_CLK_SW_CON_SEL_SW_DPY_VREF_EN_SEL Fld(2, 16)/* [17:16] */
#define DRAMC_DPY_CLK_SW_CON_SEL_SW_PHYPLL_EN_SEL Fld(2, 18)/* [19:18] */
#define DRAMC_DPY_CLK_SW_CON_SEL_SW_DDRPHY_FB_CK_EN_SEL Fld(2, 20)/* [21:20] */
#define DRAMC_DPY_CLK_SW_CON_SEL_SEPERATE_PHY_PWR_SEL Fld(1, 23)/* [23:23] */
#define DRAMC_DPY_CLK_SW_CON_SEL_SW_SW_DMDRAMCSHU_ACK_SEL Fld(2, 24)/* [25:24] */
#define DRAMC_DPY_CLK_SW_CON_SEL_SW_EMI_CLK_OFF_ACK_SEL Fld(2, 26)/* [27:26] */
#define DRAMC_DPY_CLK_SW_CON_SEL_SW_DR_SHORT_QUEUE_ACK_SEL Fld(2, 28)/* [29:28] */
#define DRAMC_DPY_CLK_SW_CON_SEL_SW_DRAMC_DFS_STA_SEL Fld(2, 30)/* [31:30] */
#define SPM_DRAMC_DPY_CLK_SW_CON (SPM_BASE_ADDRESS + 0x0464)
#define DRAMC_DPY_CLK_SW_CON_SW_DR_GATE_RETRY_EN Fld(2, 0)/* [1:0] */
#define DRAMC_DPY_CLK_SW_CON_SW_EMI_CLK_OFF Fld(2, 2)/* [3:2] */
#define DRAMC_DPY_CLK_SW_CON_SW_DPY_MODE_SW Fld(2, 4)/* [5:4] */
#define DRAMC_DPY_CLK_SW_CON_SW_DMSUS_OFF Fld(2, 6)/* [7:6] */
#define DRAMC_DPY_CLK_SW_CON_SW_MEM_CK_OFF Fld(2, 8)/* [9:8] */
#define DRAMC_DPY_CLK_SW_CON_SW_DPY_2ND_DLL_EN Fld(2, 10)/* [11:10] */
#define DRAMC_DPY_CLK_SW_CON_SW_DPY_DLL_EN Fld(2, 12)/* [13:12] */
#define DRAMC_DPY_CLK_SW_CON_SW_DPY_DLL_CK_EN Fld(2, 14)/* [15:14] */
#define DRAMC_DPY_CLK_SW_CON_SW_DPY_VREF_EN Fld(2, 16)/* [17:16] */
#define DRAMC_DPY_CLK_SW_CON_SW_PHYPLL_EN Fld(2, 18)/* [19:18] */
#define DRAMC_DPY_CLK_SW_CON_SW_DDRPHY_FB_CK_EN Fld(2, 20)/* [21:20] */
#define DRAMC_DPY_CLK_SW_CON_SC_DMDRAMCSHU_ACK Fld(2, 24)/* [25:24] */
#define DRAMC_DPY_CLK_SW_CON_SC_EMI_CLK_OFF_ACK Fld(2, 26)/* [27:26] */
#define DRAMC_DPY_CLK_SW_CON_SC_DR_SHORT_QUEUE_ACK Fld(2, 28)/* [29:28] */
#define DRAMC_DPY_CLK_SW_CON_SC_DRAMC_DFS_STA Fld(2, 30)/* [31:30] */
#define SPM_S1_MODE_CH (SPM_BASE_ADDRESS + 0x0468)
#define S1_MODE_CH_SPM_S1_MODE_CH Fld(2, 0)/* [1:0] */
#define S1_MODE_CH_S1_EMI_CK_SWITCH Fld(2, 8)/* [9:8] */
#define SPM_DRAMC_DPY_CLK_SW_CON_SEL2 (SPM_BASE_ADDRESS + 0x0470)
#define DRAMC_DPY_CLK_SW_CON_SEL2_FULL Fld(32, 0)/* [31:0] */
#define DRAMC_DPY_CLK_SW_CON_SEL2_SW_PHYPLL_SHU_EN_SEL Fld(1, 0)/* [0:0] */
#define DRAMC_DPY_CLK_SW_CON_SEL2_SW_PHYPLL2_SHU_EN_SEL Fld(1, 1)/* [1:1] */
#define DRAMC_DPY_CLK_SW_CON_SEL2_SW_PHYPLL_MODE_SW_SEL Fld(1, 2)/* [2:2] */
#define DRAMC_DPY_CLK_SW_CON_SEL2_SW_PHYPLL2_MODE_SW_SEL Fld(1, 3)/* [3:3] */
#define DRAMC_DPY_CLK_SW_CON_SEL2_SW_DR_SHORT_QUEUE_SEL Fld(1, 4)/* [4:4] */
#define DRAMC_DPY_CLK_SW_CON_SEL2_SW_DR_SHU_EN_SEL Fld(1, 5)/* [5:5] */
#define DRAMC_DPY_CLK_SW_CON_SEL2_SW_DR_SHU_LEVEL_SEL Fld(1, 6)/* [6:6] */
#define DRAMC_DPY_CLK_SW_CON_SEL2_SW_DPY_BCLK_ENABLE_SEL Fld(2, 8)/* [9:8] */
#define DRAMC_DPY_CLK_SW_CON_SEL2_SW_SHU_RESTORE_SEL Fld(2, 10)/* [11:10] */
#define DRAMC_DPY_CLK_SW_CON_SEL2_SW_DPHY_PRECAL_UP_SEL Fld(2, 12)/* [13:12] */
#define DRAMC_DPY_CLK_SW_CON_SEL2_SW_DPHY_RXDLY_TRACK_EN_SEL Fld(2, 6)/* [15:14] */
#define DRAMC_DPY_CLK_SW_CON_SEL2_SW_TX_TRACKING_DIS_SEL Fld(2, 16)/* [17:16] */
#define SPM_DRAMC_DPY_CLK_SW_CON2 (SPM_BASE_ADDRESS + 0x0474)
#define DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL_SHU_EN Fld(1, 0)/* [0:0] */
#define DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL2_SHU_EN Fld(1, 1)/* [1:1] */
#define DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL_MODE_SW Fld(1, 2)/* [2:2] */
#define DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL2_MODE_SW Fld(1, 3)/* [3:3] */
#define DRAMC_DPY_CLK_SW_CON2_SW_DR_SHORT_QUEUE Fld(1, 4)/* [4:4] */
#define DRAMC_DPY_CLK_SW_CON2_SW_DR_SHU_EN Fld(1, 5)/* [5:5] */
#define DRAMC_DPY_CLK_SW_CON2_SW_DR_SHU_LEVEL Fld(2, 6)/* [7:6] */
#define DRAMC_DPY_CLK_SW_CON2_SW_DPY_BCLK_ENABLE Fld(2, 8)/* [9:8] */
#define DRAMC_DPY_CLK_SW_CON2_SW_SHU_RESTORE Fld(2, 10)/* [11:10] */
#define DRAMC_DPY_CLK_SW_CON2_SW_DPHY_PRECAL_UP Fld(2, 12)/* [13:12] */
#define DRAMC_DPY_CLK_SW_CON2_SW_DPHY_RXDLY_TRACK_EN Fld(2, 14)/* [15:14] */
#define DRAMC_DPY_CLK_SW_CON2_SW_TX_TRACKING_DIS Fld(2, 16)/* [17:16] */
#define SPM_DRAMC_DMYRD_CON (SPM_BASE_ADDRESS + 0x0478)
#define SPM_DRAMC_DMYRD_EN_MOD_SEL_CH1 Fld(1, 10)/* [10:10] */
#define SPM_DRAMC_DMYRD_INTV_SEL_CH1 Fld(1, 9)/* [9:9] */
#define SPM_DRAMC_DMYRD_EN_CH1 Fld(1, 8)/* [8:8] */
#define SPM_DRAMC_DMYRD_EN_MOD_SEL_CH0 Fld(1, 2)/* [2:2] */
#define SPM_DRAMC_DMYRD_INTV_SEL_CH0 Fld(1, 1)/* [1:1] */
#define SPM_DRAMC_DMYRD_EN_CH0 Fld(1, 0)/* [0:0] */
/*
 * #define SPM_SW_RSV_8 (SPM_BASE_ADDRESS + 0x0654)
 * #define SW_RSV_8_RX_TRACKING_EN Fld(2,6,AC_MSKB0)//[7:6]
 */




/*
 * HW Phyical base address for mem.c use
 * #define DRAMC0_NAO_BASE Channel_A_DRAMC_NAO_BASE_ADDRESS
 * #define DRAMC1_NAO_BASE Channel_B_DRAMC_NAO_BASE_ADDRESS
 * #define DRAMC0_BASE     Channel_A_DRAMC_AO_BASE_ADDRESS
 * #define DRAMC1_BASE     Channel_B_DRAMC_AO_BASE_ADDRESS
 * #define DDRPHY_BASE     Channel_A_PHY_BASE_VIRTUAL
 * #define DDRPHY1_BASE    Channel_B_PHY_BASE_VIRTUAL
 */

#define DRAMC_WBR 0x100010B4
#define DRAMC_BROADCAST_ON 0x1f
#define DRAMC_BROADCAST_OFF 0x0

#define PHY_FLD_FULL Fld(32, 0)/* [31:0] */

/*--------------------------------------------
 * Page DRAMC_NAO
 *--------------------------------------------
 */

#define DRAMC_NAO_BASE_ADDRESS Channel_A_DRAMC_NAO_BASE_VIRTUAL

#define DRAMC_REG_TESTMODE                             (DRAMC_NAO_BASE_ADDRESS + 0x00000)
    #define TESTMODE_TESTM_PAT0                                Fld(8, 24) //[31:24]

#define DRAMC_REG_LBWDAT0                              (DRAMC_NAO_BASE_ADDRESS + 0x00004)
    #define LBWDAT0_LBWDATA0                                   Fld(32, 0) //[31:0]

#define DRAMC_REG_LBWDAT1                              (DRAMC_NAO_BASE_ADDRESS + 0x00008)
    #define LBWDAT1_LBWDATA1                                   Fld(32, 0) //[31:0]

#define DRAMC_REG_LBWDAT2                              (DRAMC_NAO_BASE_ADDRESS + 0x0000C)
    #define LBWDAT2_LBWDATA2                                   Fld(32, 0) //[31:0]

#define DRAMC_REG_LBWDAT3                              (DRAMC_NAO_BASE_ADDRESS + 0x00010)
    #define LBWDAT3_LBWDATA3                                   Fld(32, 0) //[31:0]

#define DRAMC_REG_CKPHCHK                              (DRAMC_NAO_BASE_ADDRESS + 0x00020)
    #define CKPHCHK_CKPHCHKCYC                                 Fld(16, 0) //[15:0]
    #define CKPHCHK_CKPHCNTEN                                  Fld(1, 31) //[31:31]

#define DRAMC_REG_DMMONITOR                            (DRAMC_NAO_BASE_ADDRESS + 0x00024)
    #define DMMONITOR_JMTR_EN                                  Fld(1, 0) //[0:0]
    #define DMMONITOR_MONPAUSE_SW                              Fld(1, 2) //[2:2]
    #define DMMONITOR_BUSMONEN_SW                              Fld(1, 3) //[3:3]
    #define DMMONITOR_WDQ_MON_OPT                              Fld(1, 4) //[4:4]
    #define DMMONITOR_REQQUE_MON_SREF_DIS                      Fld(1, 8) //[8:8]
    #define DMMONITOR_REQQUE_MON_SREF_REOR                     Fld(1, 9) //[9:9]
    #define DMMONITOR_REQQUE_MON_SREF_LLAT                     Fld(1, 10) //[10:10]
    #define DMMONITOR_REQQUE_MON_SREF_HPRI                     Fld(1, 11) //[11:11]
    #define DMMONITOR_REQQUE_MON_SREF_RW                       Fld(1, 12) //[12:12]
    #define DMMONITOR_JMTRCNT                                  Fld(16, 16) //[31:16]

#define DRAMC_REG_TESTCHIP_DMA1                        (DRAMC_NAO_BASE_ADDRESS + 0x00030)
    #define TESTCHIP_DMA1_DMAEN                                Fld(1, 0) //[0:0]
    #define TESTCHIP_DMA1_DMAPUREWREN                          Fld(1, 1) //[1:1]
    #define TESTCHIP_DMA1_DMAPURERDEN                          Fld(1, 2) //[2:2]
    #define TESTCHIP_DMA1_DMA_MWR                              Fld(1, 3) //[3:3]
    #define TESTCHIP_DMA1_DMABURSTLEN                          Fld(3, 4) //[6:4]
    #define TESTCHIP_DMA1_DMAEN_LOOP                           Fld(1, 8) //[8:8]
    #define TESTCHIP_DMA1_DMAFIXPAT                            Fld(1, 9) //[9:9]
    #define TESTCHIP_DMA1_DMA_LP4MATAB_OPT                     Fld(1, 12) //[12:12]

#define DRAMC_REG_MISC_STATUSA                         (DRAMC_NAO_BASE_ADDRESS + 0x00080)
    #define MISC_STATUSA_WAIT_DLE                              Fld(1, 0) //[0:0]
    #define MISC_STATUSA_WRITE_DATA_BUFFER_EMPTY               Fld(1, 1) //[1:1]
    #define MISC_STATUSA_REQQ_EMPTY                            Fld(1, 2) //[2:2]
    #define MISC_STATUSA_PG_VLD                                Fld(1, 3) //[3:3]
    #define MISC_STATUSA_REQQUE_DEPTH                          Fld(4, 4) //[7:4]
    #define MISC_STATUSA_REFRESH_RATE                          Fld(3, 8) //[10:8]
    #define MISC_STATUSA_DRAM_HWCFG                            Fld(1, 12) //[12:12]
    #define MISC_STATUSA_CKEO_PRE                              Fld(1, 13) //[13:13]
    #define MISC_STATUSA_CKE1O_PRE                             Fld(1, 14) //[14:14]
    #define MISC_STATUSA_SREF_STATE                            Fld(1, 16) //[16:16]
    #define MISC_STATUSA_SELFREF_SM                            Fld(3, 17) //[19:17]
    #define MISC_STATUSA_REFRESH_OVER_CNT                      Fld(4, 20) //[23:20]
    #define MISC_STATUSA_REFRESH_QUEUE_CNT                     Fld(4, 24) //[27:24]
    #define MISC_STATUSA_REQDEPTH_UPD_DONE                     Fld(1, 28) //[28:28]
    #define MISC_STATUSA_MANUTXUPD_DONE                        Fld(1, 29) //[29:29]
    #define MISC_STATUSA_DRAMC_IDLE_STATUS                     Fld(1, 30) //[30:30]
    #define MISC_STATUSA_DRAMC_IDLE_DCM                        Fld(1, 31) //[31:31]

#define DRAMC_REG_SPECIAL_STATUS                       (DRAMC_NAO_BASE_ADDRESS + 0x00084)
    #define SPECIAL_STATUS_SPECIAL_COMMAND_ENABLE              Fld(1, 0) //[0:0]
    #define SPECIAL_STATUS_H_ZQLAT_REQ                         Fld(1, 1) //[1:1]
    #define SPECIAL_STATUS_H_ZQLCAL_REQ                        Fld(1, 2) //[2:2]
    #define SPECIAL_STATUS_H_DQSOSCEN_REQ                      Fld(1, 4) //[4:4]
    #define SPECIAL_STATUS_DQSOSCEN_PERIOD                     Fld(1, 5) //[5:5]
    #define SPECIAL_STATUS_H_ZQCS_REQ                          Fld(1, 6) //[6:6]
    #define SPECIAL_STATUS_H_REFR_REQ                          Fld(1, 7) //[7:7]
    #define SPECIAL_STATUS_STBUPD_STOP                         Fld(1, 8) //[8:8]
    #define SPECIAL_STATUS_HW_ZQLAT_REQ                        Fld(1, 9) //[9:9]
    #define SPECIAL_STATUS_HW_ZQCAL_REQ                        Fld(1, 10) //[10:10]
    #define SPECIAL_STATUS_SPECIAL_STATUS                      Fld(1, 11) //[11:11]
    #define SPECIAL_STATUS_SCSM                                Fld(5, 12) //[16:12]
    #define SPECIAL_STATUS_SCARB_SM                            Fld(5, 20) //[24:20]
    #define SPECIAL_STATUS_SC_DRAMC_QUEUE_ACK                  Fld(1, 28) //[28:28]
    #define SPECIAL_STATUS_SREF_REQ_2Q                         Fld(1, 30) //[30:30]
    #define SPECIAL_STATUS_SREF_REQ                            Fld(1, 31) //[31:31]

#define DRAMC_REG_SPCMDRESP                            (DRAMC_NAO_BASE_ADDRESS + 0x00088)
    #define SPCMDRESP_MRW_RESPONSE                             Fld(1, 0) //[0:0]
    #define SPCMDRESP_MRR_RESPONSE                             Fld(1, 1) //[1:1]
    #define SPCMDRESP_PREA_RESPONSE                            Fld(1, 2) //[2:2]
    #define SPCMDRESP_AREF_RESPONSE                            Fld(1, 3) //[3:3]
    #define SPCMDRESP_ZQC_RESPONSE                             Fld(1, 4) //[4:4]
    #define SPCMDRESP_TCMD_RESPONSE                            Fld(1, 5) //[5:5]
    #define SPCMDRESP_ZQLAT_RESPONSE                           Fld(1, 6) //[6:6]
    #define SPCMDRESP_RDDQC_RESPONSE                           Fld(1, 7) //[7:7]
    #define SPCMDRESP_STEST_RESPONSE                           Fld(1, 8) //[8:8]
    #define SPCMDRESP_MPCMAN_RESPONSE                          Fld(1, 9) //[9:9]
    #define SPCMDRESP_DQSOSCEN_RESPONSE                        Fld(1, 10) //[10:10]
    #define SPCMDRESP_DQSOSCDIS_RESPONSE                       Fld(1, 11) //[11:11]
    #define SPCMDRESP_ACT_RESPONSE                             Fld(1, 12) //[12:12]
    #define SPCMDRESP_MPRW_RESPONSE                            Fld(1, 13) //[13:13]
    #define SPCMDRESP_DVFS_RESPONSE                            Fld(1, 16) //[16:16]
    #define SPCMDRESP_HW_ZQLAT_POP                             Fld(1, 17) //[17:17]
    #define SPCMDRESP_HW_ZQCAL_POP                             Fld(1, 18) //[18:18]
    #define SPCMDRESP_RDFIFO_RESPONSE                          Fld(1, 30) //[30:30]
    #define SPCMDRESP_WRFIFO_RESPONSE                          Fld(1, 31) //[31:31]

#define DRAMC_REG_MRR_STATUS                           (DRAMC_NAO_BASE_ADDRESS + 0x0008C)
    #define MRR_STATUS_MRR_REG                                 Fld(16, 0) //[15:0]
    #define MRR_STATUS_MRR_SW_REG                              Fld(16, 16) //[31:16]

#define DRAMC_REG_MRR_STATUS2                          (DRAMC_NAO_BASE_ADDRESS + 0x00090)
    #define MRR_STATUS2_MR4_REG                                Fld(16, 0) //[15:0]
    #define MRR_STATUS2_SHUFFLE_MRW_VRCG_NORMAL_OK             Fld(1, 16) //[16:16]
    #define MRR_STATUS2_TFC_OK                                 Fld(1, 17) //[17:17]
    #define MRR_STATUS2_TCKFSPX_OK                             Fld(1, 18) //[18:18]
    #define MRR_STATUS2_TVRCG_EN_OK                            Fld(1, 19) //[19:19]
    #define MRR_STATUS2_TCKFSPE_OK                             Fld(1, 20) //[20:20]
    #define MRR_STATUS2_TVRCG_DIS_OK                           Fld(1, 21) //[21:21]
    #define MRR_STATUS2_PHY_SHUFFLE_PERIOD_GO_ZERO_OK          Fld(1, 22) //[22:22]
    #define MRR_STATUS2_DVFS_STATE                             Fld(8, 24) //[31:24]

#define DRAMC_REG_MRRDATA0                             (DRAMC_NAO_BASE_ADDRESS + 0x00094)
    #define MRRDATA0_MRR_DATA0                                 Fld(32, 0) //[31:0]

#define DRAMC_REG_MRRDATA1                             (DRAMC_NAO_BASE_ADDRESS + 0x00098)
    #define MRRDATA1_MRR_DATA1                                 Fld(32, 0) //[31:0]

#define DRAMC_REG_MRRDATA2                             (DRAMC_NAO_BASE_ADDRESS + 0x0009C)
    #define MRRDATA2_MRR_DATA2                                 Fld(32, 0) //[31:0]

#define DRAMC_REG_MRRDATA3                             (DRAMC_NAO_BASE_ADDRESS + 0x000A0)
    #define MRRDATA3_MRR_DATA3                                 Fld(32, 0) //[31:0]

#define DRAMC_REG_DRS_STATUS                           (DRAMC_NAO_BASE_ADDRESS + 0x000A8)
    #define DRS_STATUS_DRS_MONERR_ACK                          Fld(1, 8) //[8:8]
    #define DRS_STATUS_DRS_MONERR_REQ                          Fld(1, 9) //[9:9]
    #define DRS_STATUS_RK1_DRS_REQ                             Fld(1, 16) //[16:16]
    #define DRS_STATUS_RK1_DRS_2Q                              Fld(1, 17) //[17:17]
    #define DRS_STATUS_RK1_DRSLY_REQ                           Fld(1, 18) //[18:18]
    #define DRS_STATUS_RK1_DRS_RDY                             Fld(1, 19) //[19:19]
    #define DRS_STATUS_RK1_DRS_ACK                             Fld(1, 20) //[20:20]
    #define DRS_STATUS_RK1_DRS_STATUS                          Fld(1, 21) //[21:21]
    #define DRS_STATUS_SELFREF_SM_RK1                          Fld(3, 24) //[26:24]

#define DRAMC_REG_JMETER_ST                            (DRAMC_NAO_BASE_ADDRESS + 0x000BC)
    #define JMETER_ST_ZEROS_CNT                                Fld(15, 0) //[14:0]
    #define JMETER_ST_ONES_CNT                                 Fld(15, 16) //[30:16]
    #define JMETER_ST_JMTR_DONE                                Fld(1, 31) //[31:31]

#define DRAMC_REG_TCMDO1LAT                            (DRAMC_NAO_BASE_ADDRESS + 0x000C0)
    #define TCMDO1LAT_TCMD_O1_LATCH_DATA0                      Fld(6, 0) //[5:0]
    #define TCMDO1LAT_TCMD_O1_LATCH_DATA1                      Fld(6, 8) //[13:8]
    #define TCMDO1LAT_CATRAIN_CMP_ERR0                         Fld(6, 16) //[21:16]
    #define TCMDO1LAT_CATRAIN_CMP_ERR                          Fld(6, 24) //[29:24]
    #define TCMDO1LAT_CATRAIN_CMP_CPT                          Fld(1, 31) //[31:31]

#define DRAMC_REG_RDQC_CMP                             (DRAMC_NAO_BASE_ADDRESS + 0x000C4)
    #define RDQC_CMP_RDDQC_CMP0_ERR                            Fld(16, 0) //[15:0]
    #define RDQC_CMP_RDDQC_CMP1_ERR                            Fld(16, 16) //[31:16]

#define DRAMC_REG_CKPHCHK_STATUS                       (DRAMC_NAO_BASE_ADDRESS + 0x000C8)
    #define CKPHCHK_STATUS_CKPHCHK_STATUS                      Fld(16, 0) //[15:0]

#define DRAMC_REG_HWMRR_PUSH2POP_CNT                   (DRAMC_NAO_BASE_ADDRESS + 0x0010C)
    #define HWMRR_PUSH2POP_CNT_HWMRR_PUSH2POP_CNT              Fld(32, 0) //[31:0]

#define DRAMC_REG_HWMRR_STATUS                         (DRAMC_NAO_BASE_ADDRESS + 0x00110)
    #define HWMRR_STATUS_OV_P2P_CNT                            Fld(8, 0) //[7:0]
    #define HWMRR_STATUS_MRR_CNT_UNDER_FULL                    Fld(1, 30) //[30:30]
    #define HWMRR_STATUS_MRR_CNT_OVER_FULL                     Fld(1, 31) //[31:31]

#define DRAMC_REG_HW_REFRATE_MON                       (DRAMC_NAO_BASE_ADDRESS + 0x00114)
    #define HW_REFRATE_MON_REFRESH_RATE_MIN_MON                Fld(3, 0) //[2:0]
    #define HW_REFRATE_MON_REFRESH_RATE_MAX_MON                Fld(3, 8) //[10:8]

#define DRAMC_REG_TESTRPT                              (DRAMC_NAO_BASE_ADDRESS + 0x00120)
    #define TESTRPT_DM_CMP_CPT_RK0                             Fld(1, 0) //[0:0]
    #define TESTRPT_DM_CMP_CPT_RK1                             Fld(1, 1) //[1:1]
    #define TESTRPT_DM_CMP_ERR_RK0                             Fld(1, 4) //[4:4]
    #define TESTRPT_DM_CMP_ERR_RK1                             Fld(1, 5) //[5:5]
    #define TESTRPT_DLE_CNT_OK_RK0                             Fld(1, 8) //[8:8]
    #define TESTRPT_DLE_CNT_OK_RK1                             Fld(1, 9) //[9:9]
    #define TESTRPT_TESTSTAT                                   Fld(3, 20) //[22:20]
    #define TESTRPT_LB_CMP_FAIL                                Fld(1, 24) //[24:24]
    #define TESTRPT_CALI_DONE_MON                              Fld(1, 28) //[28:28]

#define DRAMC_REG_CMP_ERR                              (DRAMC_NAO_BASE_ADDRESS + 0x00124)
    #define CMP_ERR_CMP_ERR                                    Fld(32, 0) //[31:0]

#define DRAMC_REG_TEST_ABIT_STATUS1                    (DRAMC_NAO_BASE_ADDRESS + 0x00128)
    #define TEST_ABIT_STATUS1_TEST_ABIT_ERR1                   Fld(32, 0) //[31:0]

#define DRAMC_REG_TEST_ABIT_STATUS2                    (DRAMC_NAO_BASE_ADDRESS + 0x0012C)
    #define TEST_ABIT_STATUS2_TEST_ABIT_ERR2                   Fld(32, 0) //[31:0]

#define DRAMC_REG_TEST_ABIT_STATUS3                    (DRAMC_NAO_BASE_ADDRESS + 0x00130)
    #define TEST_ABIT_STATUS3_TEST_ABIT_ERR3                   Fld(32, 0) //[31:0]

#define DRAMC_REG_TEST_ABIT_STATUS4                    (DRAMC_NAO_BASE_ADDRESS + 0x00134)
    #define TEST_ABIT_STATUS4_TEST_ABIT_ERR4                   Fld(32, 0) //[31:0]

#define DRAMC_REG_DQSDLY0                              (DRAMC_NAO_BASE_ADDRESS + 0x00150)
    #define DQSDLY0_DEL0DLY                                    Fld(7, 0) //[6:0]
    #define DQSDLY0_DEL1DLY                                    Fld(7, 8) //[14:8]
    #define DQSDLY0_DEL2DLY                                    Fld(7, 16) //[22:16]
    #define DQSDLY0_DEL3DLY                                    Fld(7, 24) //[30:24]

#define DRAMC_REG_DQ_CAL_MAX_0                         (DRAMC_NAO_BASE_ADDRESS + 0x00154)
    #define DQ_CAL_MAX_0_DQ0_0_DLY_MAX                         Fld(8, 0) //[7:0]
    #define DQ_CAL_MAX_0_DQ0_1_DLY_MAX                         Fld(8, 8) //[15:8]
    #define DQ_CAL_MAX_0_DQ0_2_DLY_MAX                         Fld(8, 16) //[23:16]
    #define DQ_CAL_MAX_0_DQ0_3_DLY_MAX                         Fld(8, 24) //[31:24]

#define DRAMC_REG_DQ_CAL_MAX_1                         (DRAMC_NAO_BASE_ADDRESS + 0x00158)
    #define DQ_CAL_MAX_1_DQ0_4_DLY_MAX                         Fld(8, 0) //[7:0]
    #define DQ_CAL_MAX_1_DQ0_5_DLY_MAX                         Fld(8, 8) //[15:8]
    #define DQ_CAL_MAX_1_DQ0_6_DLY_MAX                         Fld(8, 16) //[23:16]
    #define DQ_CAL_MAX_1_DQ0_7_DLY_MAX                         Fld(8, 24) //[31:24]

#define DRAMC_REG_DQ_CAL_MAX_2                         (DRAMC_NAO_BASE_ADDRESS + 0x0015C)
    #define DQ_CAL_MAX_2_DQ1_0_DLY_MAX                         Fld(8, 0) //[7:0]
    #define DQ_CAL_MAX_2_DQ1_1_DLY_MAX                         Fld(8, 8) //[15:8]
    #define DQ_CAL_MAX_2_DQ1_2_DLY_MAX                         Fld(8, 16) //[23:16]
    #define DQ_CAL_MAX_2_DQ1_3_DLY_MAX                         Fld(8, 24) //[31:24]

#define DRAMC_REG_DQ_CAL_MAX_3                         (DRAMC_NAO_BASE_ADDRESS + 0x00160)
    #define DQ_CAL_MAX_3_DQ1_4_DLY_MAX                         Fld(8, 0) //[7:0]
    #define DQ_CAL_MAX_3_DQ1_5_DLY_MAX                         Fld(8, 8) //[15:8]
    #define DQ_CAL_MAX_3_DQ1_6_DLY_MAX                         Fld(8, 16) //[23:16]
    #define DQ_CAL_MAX_3_DQ1_7_DLY_MAX                         Fld(8, 24) //[31:24]

#define DRAMC_REG_DQ_CAL_MAX_4                         (DRAMC_NAO_BASE_ADDRESS + 0x00164)
    #define DQ_CAL_MAX_4_DQ2_0_DLY_MAX                         Fld(8, 0) //[7:0]
    #define DQ_CAL_MAX_4_DQ2_1_DLY_MAX                         Fld(8, 8) //[15:8]
    #define DQ_CAL_MAX_4_DQ2_2_DLY_MAX                         Fld(8, 16) //[23:16]
    #define DQ_CAL_MAX_4_DQ2_3_DLY_MAX                         Fld(8, 24) //[31:24]

#define DRAMC_REG_DQ_CAL_MAX_5                         (DRAMC_NAO_BASE_ADDRESS + 0x00168)
    #define DQ_CAL_MAX_5_DQ2_4_DLY_MAX                         Fld(8, 0) //[7:0]
    #define DQ_CAL_MAX_5_DQ2_5_DLY_MAX                         Fld(8, 8) //[15:8]
    #define DQ_CAL_MAX_5_DQ2_6_DLY_MAX                         Fld(8, 16) //[23:16]
    #define DQ_CAL_MAX_5_DQ2_7_DLY_MAX                         Fld(8, 24) //[31:24]

#define DRAMC_REG_DQ_CAL_MAX_6                         (DRAMC_NAO_BASE_ADDRESS + 0x0016C)
    #define DQ_CAL_MAX_6_DQ3_0_DLY_MAX                         Fld(8, 0) //[7:0]
    #define DQ_CAL_MAX_6_DQ3_1_DLY_MAX                         Fld(8, 8) //[15:8]
    #define DQ_CAL_MAX_6_DQ3_2_DLY_MAX                         Fld(8, 16) //[23:16]
    #define DQ_CAL_MAX_6_DQ3_3_DLY_MAX                         Fld(8, 24) //[31:24]

#define DRAMC_REG_DQ_CAL_MAX_7                         (DRAMC_NAO_BASE_ADDRESS + 0x00170)
    #define DQ_CAL_MAX_7_DQ3_4_DLY_MAX                         Fld(8, 0) //[7:0]
    #define DQ_CAL_MAX_7_DQ3_5_DLY_MAX                         Fld(8, 8) //[15:8]
    #define DQ_CAL_MAX_7_DQ3_6_DLY_MAX                         Fld(8, 16) //[23:16]
    #define DQ_CAL_MAX_7_DQ3_7_DLY_MAX                         Fld(8, 24) //[31:24]

#define DRAMC_REG_DQS_CAL_MIN_0                        (DRAMC_NAO_BASE_ADDRESS + 0x00174)
    #define DQS_CAL_MIN_0_DQS0_0_DLY_MIN                       Fld(8, 0) //[7:0]
    #define DQS_CAL_MIN_0_DQS0_1_DLY_MIN                       Fld(8, 8) //[15:8]
    #define DQS_CAL_MIN_0_DQS0_2_DLY_MIN                       Fld(8, 16) //[23:16]
    #define DQS_CAL_MIN_0_DQS0_3_DLY_MIN                       Fld(8, 24) //[31:24]

#define DRAMC_REG_DQS_CAL_MIN_1                        (DRAMC_NAO_BASE_ADDRESS + 0x00178)
    #define DQS_CAL_MIN_1_DQS0_4_DLY_MIN                       Fld(8, 0) //[7:0]
    #define DQS_CAL_MIN_1_DQS0_5_DLY_MIN                       Fld(8, 8) //[15:8]
    #define DQS_CAL_MIN_1_DQS0_6_DLY_MIN                       Fld(8, 16) //[23:16]
    #define DQS_CAL_MIN_1_DQS0_7_DLY_MIN                       Fld(8, 24) //[31:24]

#define DRAMC_REG_DQS_CAL_MIN_2                        (DRAMC_NAO_BASE_ADDRESS + 0x0017C)
    #define DQS_CAL_MIN_2_DQS1_0_DLY_MIN                       Fld(8, 0) //[7:0]
    #define DQS_CAL_MIN_2_DQS1_1_DLY_MIN                       Fld(8, 8) //[15:8]
    #define DQS_CAL_MIN_2_DQS1_2_DLY_MIN                       Fld(8, 16) //[23:16]
    #define DQS_CAL_MIN_2_DQS1_3_DLY_MIN                       Fld(8, 24) //[31:24]

#define DRAMC_REG_DQS_CAL_MIN_3                        (DRAMC_NAO_BASE_ADDRESS + 0x00180)
    #define DQS_CAL_MIN_3_DQS1_4_DLY_MIN                       Fld(8, 0) //[7:0]
    #define DQS_CAL_MIN_3_DQS1_5_DLY_MIN                       Fld(8, 8) //[15:8]
    #define DQS_CAL_MIN_3_DQS1_6_DLY_MIN                       Fld(8, 16) //[23:16]
    #define DQS_CAL_MIN_3_DQS1_7_DLY_MIN                       Fld(8, 24) //[31:24]

#define DRAMC_REG_DQS_CAL_MIN_4                        (DRAMC_NAO_BASE_ADDRESS + 0x00184)
    #define DQS_CAL_MIN_4_DQS2_0_DLY_MIN                       Fld(8, 0) //[7:0]
    #define DQS_CAL_MIN_4_DQS2_1_DLY_MIN                       Fld(8, 8) //[15:8]
    #define DQS_CAL_MIN_4_DQS2_2_DLY_MIN                       Fld(8, 16) //[23:16]
    #define DQS_CAL_MIN_4_DQS2_3_DLY_MIN                       Fld(8, 24) //[31:24]

#define DRAMC_REG_DQS_CAL_MIN_5                        (DRAMC_NAO_BASE_ADDRESS + 0x00188)
    #define DQS_CAL_MIN_5_DQS2_4_DLY_MIN                       Fld(8, 0) //[7:0]
    #define DQS_CAL_MIN_5_DQS2_5_DLY_MIN                       Fld(8, 8) //[15:8]
    #define DQS_CAL_MIN_5_DQS2_6_DLY_MIN                       Fld(8, 16) //[23:16]
    #define DQS_CAL_MIN_5_DQS2_7_DLY_MIN                       Fld(8, 24) //[31:24]

#define DRAMC_REG_DQS_CAL_MIN_6                        (DRAMC_NAO_BASE_ADDRESS + 0x0018C)
    #define DQS_CAL_MIN_6_DQS3_0_DLY_MIN                       Fld(8, 0) //[7:0]
    #define DQS_CAL_MIN_6_DQS3_1_DLY_MIN                       Fld(8, 8) //[15:8]
    #define DQS_CAL_MIN_6_DQS3_2_DLY_MIN                       Fld(8, 16) //[23:16]
    #define DQS_CAL_MIN_6_DQS3_3_DLY_MIN                       Fld(8, 24) //[31:24]

#define DRAMC_REG_DQS_CAL_MIN_7                        (DRAMC_NAO_BASE_ADDRESS + 0x00190)
    #define DQS_CAL_MIN_7_DQS3_4_DLY_MIN                       Fld(8, 0) //[7:0]
    #define DQS_CAL_MIN_7_DQS3_5_DLY_MIN                       Fld(8, 8) //[15:8]
    #define DQS_CAL_MIN_7_DQS3_6_DLY_MIN                       Fld(8, 16) //[23:16]
    #define DQS_CAL_MIN_7_DQS3_7_DLY_MIN                       Fld(8, 24) //[31:24]

#define DRAMC_REG_DQS_CAL_MAX_0                        (DRAMC_NAO_BASE_ADDRESS + 0x00194)
    #define DQS_CAL_MAX_0_DQS0_0_DLY_MAX                       Fld(8, 0) //[7:0]
    #define DQS_CAL_MAX_0_DQS0_1_DLY_MAX                       Fld(8, 8) //[15:8]
    #define DQS_CAL_MAX_0_DQS0_2_DLY_MAX                       Fld(8, 16) //[23:16]
    #define DQS_CAL_MAX_0_DQS0_3_DLY_MAX                       Fld(8, 24) //[31:24]

#define DRAMC_REG_DQS_CAL_MAX_1                        (DRAMC_NAO_BASE_ADDRESS + 0x00198)
    #define DQS_CAL_MAX_1_DQS0_4_DLY_MAX                       Fld(8, 0) //[7:0]
    #define DQS_CAL_MAX_1_DQS0_5_DLY_MAX                       Fld(8, 8) //[15:8]
    #define DQS_CAL_MAX_1_DQS0_6_DLY_MAX                       Fld(8, 16) //[23:16]
    #define DQS_CAL_MAX_1_DQS0_7_DLY_MAX                       Fld(8, 24) //[31:24]

#define DRAMC_REG_DQS_CAL_MAX_2                        (DRAMC_NAO_BASE_ADDRESS + 0x0019C)
    #define DQS_CAL_MAX_2_DQS1_0_DLY_MAX                       Fld(8, 0) //[7:0]
    #define DQS_CAL_MAX_2_DQS1_1_DLY_MAX                       Fld(8, 8) //[15:8]
    #define DQS_CAL_MAX_2_DQS1_2_DLY_MAX                       Fld(8, 16) //[23:16]
    #define DQS_CAL_MAX_2_DQS1_3_DLY_MAX                       Fld(8, 24) //[31:24]

#define DRAMC_REG_DQS_CAL_MAX_3                        (DRAMC_NAO_BASE_ADDRESS + 0x001A0)
    #define DQS_CAL_MAX_3_DQS1_4_DLY_MAX                       Fld(8, 0) //[7:0]
    #define DQS_CAL_MAX_3_DQS1_5_DLY_MAX                       Fld(8, 8) //[15:8]
    #define DQS_CAL_MAX_3_DQS1_6_DLY_MAX                       Fld(8, 16) //[23:16]
    #define DQS_CAL_MAX_3_DQS1_7_DLY_MAX                       Fld(8, 24) //[31:24]

#define DRAMC_REG_DQS_CAL_MAX_4                        (DRAMC_NAO_BASE_ADDRESS + 0x001A4)
    #define DQS_CAL_MAX_4_DQS2_0_DLY_MAX                       Fld(8, 0) //[7:0]
    #define DQS_CAL_MAX_4_DQS2_1_DLY_MAX                       Fld(8, 8) //[15:8]
    #define DQS_CAL_MAX_4_DQS2_2_DLY_MAX                       Fld(8, 16) //[23:16]
    #define DQS_CAL_MAX_4_DQS2_3_DLY_MAX                       Fld(8, 24) //[31:24]

#define DRAMC_REG_DQS_CAL_MAX_5                        (DRAMC_NAO_BASE_ADDRESS + 0x001A8)
    #define DQS_CAL_MAX_5_DQS2_4_DLY_MAX                       Fld(8, 0) //[7:0]
    #define DQS_CAL_MAX_5_DQS2_5_DLY_MAX                       Fld(8, 8) //[15:8]
    #define DQS_CAL_MAX_5_DQS2_6_DLY_MAX                       Fld(8, 16) //[23:16]
    #define DQS_CAL_MAX_5_DQS2_7_DLY_MAX                       Fld(8, 24) //[31:24]

#define DRAMC_REG_DQS_CAL_MAX_6                        (DRAMC_NAO_BASE_ADDRESS + 0x001AC)
    #define DQS_CAL_MAX_6_DQS3_0_DLY_MAX                       Fld(8, 0) //[7:0]
    #define DQS_CAL_MAX_6_DQS3_1_DLY_MAX                       Fld(8, 8) //[15:8]
    #define DQS_CAL_MAX_6_DQS3_2_DLY_MAX                       Fld(8, 16) //[23:16]
    #define DQS_CAL_MAX_6_DQS3_3_DLY_MAX                       Fld(8, 24) //[31:24]

#define DRAMC_REG_DQS_CAL_MAX_7                        (DRAMC_NAO_BASE_ADDRESS + 0x001B0)
    #define DQS_CAL_MAX_7_DQS3_4_DLY_MAX                       Fld(8, 0) //[7:0]
    #define DQS_CAL_MAX_7_DQS3_5_DLY_MAX                       Fld(8, 8) //[15:8]
    #define DQS_CAL_MAX_7_DQS3_6_DLY_MAX                       Fld(8, 16) //[23:16]
    #define DQS_CAL_MAX_7_DQS3_7_DLY_MAX                       Fld(8, 24) //[31:24]

#define DRAMC_REG_DQICAL0                              (DRAMC_NAO_BASE_ADDRESS + 0x001B4)
    #define DQICAL0_DQ0_DLY_MAX                                Fld(7, 0) //[6:0]
    #define DQICAL0_DQ1_DLY_MAX                                Fld(7, 8) //[14:8]
    #define DQICAL0_DQ2_DLY_MAX                                Fld(7, 16) //[22:16]
    #define DQICAL0_DQ3_DLY_MAX                                Fld(7, 24) //[30:24]

#define DRAMC_REG_DQICAL1                              (DRAMC_NAO_BASE_ADDRESS + 0x001B8)
    #define DQICAL1_DQS0_DLY_MIN                               Fld(7, 0) //[6:0]
    #define DQICAL1_DQS1_DLY_MIN                               Fld(7, 8) //[14:8]
    #define DQICAL1_DQS2_DLY_MIN                               Fld(7, 16) //[22:16]
    #define DQICAL1_DQS3_DLY_MIN                               Fld(7, 24) //[30:24]

#define DRAMC_REG_DQICAL2                              (DRAMC_NAO_BASE_ADDRESS + 0x001BC)
    #define DQICAL2_DQS0_DLY_MAX                               Fld(7, 0) //[6:0]
    #define DQICAL2_DQS1_DLY_MAX                               Fld(7, 8) //[14:8]
    #define DQICAL2_DQS2_DLY_MAX                               Fld(7, 16) //[22:16]
    #define DQICAL2_DQS3_DLY_MAX                               Fld(7, 24) //[30:24]

#define DRAMC_REG_DQICAL3                              (DRAMC_NAO_BASE_ADDRESS + 0x001C0)
    #define DQICAL3_DQS0_DLY_AVG                               Fld(7, 0) //[6:0]
    #define DQICAL3_DQS1_DLY_AVG                               Fld(7, 8) //[14:8]
    #define DQICAL3_DQS2_DLY_AVG                               Fld(7, 16) //[22:16]
    #define DQICAL3_DQS3_DLY_AVG                               Fld(7, 24) //[30:24]

#define DRAMC_REG_TESTCHIP_DMA_STATUS1                 (DRAMC_NAO_BASE_ADDRESS + 0x00200)
    #define TESTCHIP_DMA_STATUS1_DMASTATUS                     Fld(1, 0) //[0:0]
    #define TESTCHIP_DMA_STATUS1_DMA_BUF_AVAIL                 Fld(1, 2) //[2:2]
    #define TESTCHIP_DMA_STATUS1_DMACMPERR                     Fld(1, 3) //[3:3]
    #define TESTCHIP_DMA_STATUS1_DMA_STATE                     Fld(4, 4) //[7:4]

#define DRAMC_REG_TESTCHIP_DMA_STATUS2                 (DRAMC_NAO_BASE_ADDRESS + 0x00204)
    #define TESTCHIP_DMA_STATUS2_DMACMPERR_BIT                 Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS3                 (DRAMC_NAO_BASE_ADDRESS + 0x00208)
    #define TESTCHIP_DMA_STATUS3_DMA_DATA_BUFFER0_31_0_        Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS4                 (DRAMC_NAO_BASE_ADDRESS + 0x0020C)
    #define TESTCHIP_DMA_STATUS4_DMA_DATA_BUFFER0_63_32_       Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS5                 (DRAMC_NAO_BASE_ADDRESS + 0x00210)
    #define TESTCHIP_DMA_STATUS5_DMA_DATA_BUFFER0_95_64_       Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS6                 (DRAMC_NAO_BASE_ADDRESS + 0x00214)
    #define TESTCHIP_DMA_STATUS6_DMA_DATA_BUFFER0_127_96_      Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS7                 (DRAMC_NAO_BASE_ADDRESS + 0x00218)
    #define TESTCHIP_DMA_STATUS7_DMA_DATA_BUFFER1_31_0_        Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS8                 (DRAMC_NAO_BASE_ADDRESS + 0x0021C)
    #define TESTCHIP_DMA_STATUS8_DMA_DATA_BUFFER1_63_32_       Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS9                 (DRAMC_NAO_BASE_ADDRESS + 0x00220)
    #define TESTCHIP_DMA_STATUS9_DMA_DATA_BUFFER1_95_64_       Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS10                (DRAMC_NAO_BASE_ADDRESS + 0x00224)
    #define TESTCHIP_DMA_STATUS10_DMA_DATA_BUFFE1_127_96_      Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS11                (DRAMC_NAO_BASE_ADDRESS + 0x00228)
    #define TESTCHIP_DMA_STATUS11_DMA_DATA_BUFFER2_31_0_       Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS12                (DRAMC_NAO_BASE_ADDRESS + 0x0022C)
    #define TESTCHIP_DMA_STATUS12_DMA_DATA_BUFFER2_63_32_      Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS13                (DRAMC_NAO_BASE_ADDRESS + 0x00230)
    #define TESTCHIP_DMA_STATUS13_DMA_DATA_BUFFER2_95_64_      Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS14                (DRAMC_NAO_BASE_ADDRESS + 0x00234)
    #define TESTCHIP_DMA_STATUS14_DMA_DATA_BUFFER2_127_96_     Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS15                (DRAMC_NAO_BASE_ADDRESS + 0x00238)
    #define TESTCHIP_DMA_STATUS15_DMA_DATA_BUFFER3_31_0_       Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS16                (DRAMC_NAO_BASE_ADDRESS + 0x0023C)
    #define TESTCHIP_DMA_STATUS16_DMA_DATA_BUFFER3_63_32_      Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS17                (DRAMC_NAO_BASE_ADDRESS + 0x00240)
    #define TESTCHIP_DMA_STATUS17_DMA_DATA_BUFFER3_95_64_      Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS18                (DRAMC_NAO_BASE_ADDRESS + 0x00244)
    #define TESTCHIP_DMA_STATUS18_DMA_DATA_BUFFER3_127_96_     Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS19                (DRAMC_NAO_BASE_ADDRESS + 0x00248)
    #define TESTCHIP_DMA_STATUS19_DMA_DATA_BUFFER4_31_0_       Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS20                (DRAMC_NAO_BASE_ADDRESS + 0x0024C)
    #define TESTCHIP_DMA_STATUS20_DMA_DATA_BUFFER4_63_32_      Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS21                (DRAMC_NAO_BASE_ADDRESS + 0x00250)
    #define TESTCHIP_DMA_STATUS21_DMA_DATA_BUFFER4_95_64_      Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS22                (DRAMC_NAO_BASE_ADDRESS + 0x00254)
    #define TESTCHIP_DMA_STATUS22_DMA_DATA_BUFFER4_127_96_     Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS23                (DRAMC_NAO_BASE_ADDRESS + 0x00258)
    #define TESTCHIP_DMA_STATUS23_DMA_DATA_BUFFER5_31_0_       Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS24                (DRAMC_NAO_BASE_ADDRESS + 0x0025C)
    #define TESTCHIP_DMA_STATUS24_DMA_DATA_BUFFER5_63_32_      Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS25                (DRAMC_NAO_BASE_ADDRESS + 0x00260)
    #define TESTCHIP_DMA_STATUS25_DMA_DATA_BUFFER5_95_64_      Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS26                (DRAMC_NAO_BASE_ADDRESS + 0x00264)
    #define TESTCHIP_DMA_STATUS26_DMA_DATA_BUFFER5_127_96_     Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS27                (DRAMC_NAO_BASE_ADDRESS + 0x00268)
    #define TESTCHIP_DMA_STATUS27_DMA_DATA_BUFFER6_31_0_       Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS28                (DRAMC_NAO_BASE_ADDRESS + 0x0026C)
    #define TESTCHIP_DMA_STATUS28_DMA_DATA_BUFFER6_63_32_      Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS29                (DRAMC_NAO_BASE_ADDRESS + 0x00270)
    #define TESTCHIP_DMA_STATUS29_DMA_DATA_BUFFER6_95_64_      Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS30                (DRAMC_NAO_BASE_ADDRESS + 0x00274)
    #define TESTCHIP_DMA_STATUS30_DMA_DATA_BUFFER6_127_96_     Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS31                (DRAMC_NAO_BASE_ADDRESS + 0x00278)
    #define TESTCHIP_DMA_STATUS31_DMA_DATA_BUFFER7_31_0_       Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS32                (DRAMC_NAO_BASE_ADDRESS + 0x0027C)
    #define TESTCHIP_DMA_STATUS32_DMA_DATA_BUFFER7_63_32_      Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS33                (DRAMC_NAO_BASE_ADDRESS + 0x00280)
    #define TESTCHIP_DMA_STATUS33_DMA_DATA_BUFFER7_95_64_      Fld(32, 0) //[31:0]

#define DRAMC_REG_TESTCHIP_DMA_STATUS34                (DRAMC_NAO_BASE_ADDRESS + 0x00284)
    #define TESTCHIP_DMA_STATUS34_DMA_DATA_BUFFER7_127_96_     Fld(32, 0) //[31:0]

#define DRAMC_REG_REFRESH_POP_COUNTER                  (DRAMC_NAO_BASE_ADDRESS + 0x00300)
    #define REFRESH_POP_COUNTER_REFRESH_POP_COUNTER            Fld(32, 0) //[31:0]

#define DRAMC_REG_FREERUN_26M_COUNTER                  (DRAMC_NAO_BASE_ADDRESS + 0x00304)
    #define FREERUN_26M_COUNTER_FREERUN_26M_COUNTER            Fld(32, 0) //[31:0]

#define DRAMC_REG_DRAMC_IDLE_COUNTER                   (DRAMC_NAO_BASE_ADDRESS + 0x00308)
    #define DRAMC_IDLE_COUNTER_DRAMC_IDLE_COUNTER              Fld(32, 0) //[31:0]

#define DRAMC_REG_R2R_PAGE_HIT_COUNTER                 (DRAMC_NAO_BASE_ADDRESS + 0x0030C)
    #define R2R_PAGE_HIT_COUNTER_R2R_PAGE_HIT_COUNTER          Fld(32, 0) //[31:0]

#define DRAMC_REG_R2R_PAGE_MISS_COUNTER                (DRAMC_NAO_BASE_ADDRESS + 0x00310)
    #define R2R_PAGE_MISS_COUNTER_R2R_PAGE_MISS_COUNTER        Fld(32, 0) //[31:0]

#define DRAMC_REG_R2R_INTERBANK_COUNTER                (DRAMC_NAO_BASE_ADDRESS + 0x00314)
    #define R2R_INTERBANK_COUNTER_R2R_INTERBANK_COUNTER        Fld(32, 0) //[31:0]

#define DRAMC_REG_R2W_PAGE_HIT_COUNTER                 (DRAMC_NAO_BASE_ADDRESS + 0x00318)
    #define R2W_PAGE_HIT_COUNTER_R2W_PAGE_HIT_COUNTER          Fld(32, 0) //[31:0]

#define DRAMC_REG_R2W_PAGE_MISS_COUNTER                (DRAMC_NAO_BASE_ADDRESS + 0x0031C)
    #define R2W_PAGE_MISS_COUNTER_R2W_PAGE_MISS_COUNTER        Fld(32, 0) //[31:0]

#define DRAMC_REG_R2W_INTERBANK_COUNTER                (DRAMC_NAO_BASE_ADDRESS + 0x00320)
    #define R2W_INTERBANK_COUNTER_R2W_INTERBANK_COUNTER        Fld(32, 0) //[31:0]

#define DRAMC_REG_W2R_PAGE_HIT_COUNTER                 (DRAMC_NAO_BASE_ADDRESS + 0x00324)
    #define W2R_PAGE_HIT_COUNTER_W2R_PAGE_HIT_COUNTER          Fld(32, 0) //[31:0]

#define DRAMC_REG_W2R_PAGE_MISS_COUNTER                (DRAMC_NAO_BASE_ADDRESS + 0x00328)
    #define W2R_PAGE_MISS_COUNTER_W2R_PAGE_MISS_COUNTER        Fld(32, 0) //[31:0]

#define DRAMC_REG_W2R_INTERBANK_COUNTER                (DRAMC_NAO_BASE_ADDRESS + 0x0032C)
    #define W2R_INTERBANK_COUNTER_W2R_INTERBANK_COUNTER        Fld(32, 0) //[31:0]

#define DRAMC_REG_W2W_PAGE_HIT_COUNTER                 (DRAMC_NAO_BASE_ADDRESS + 0x00330)
    #define W2W_PAGE_HIT_COUNTER_W2W_PAGE_HIT_COUNTER          Fld(32, 0) //[31:0]

#define DRAMC_REG_W2W_PAGE_MISS_COUNTER                (DRAMC_NAO_BASE_ADDRESS + 0x00334)
    #define W2W_PAGE_MISS_COUNTER_W2W_PAGE_MISS_COUNTER        Fld(32, 0) //[31:0]

#define DRAMC_REG_W2W_INTERBANK_COUNTER                (DRAMC_NAO_BASE_ADDRESS + 0x00338)
    #define W2W_INTERBANK_COUNTER_W2W_INTERBANK_COUNTER        Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_PRE_STANDBY_COUNTER              (DRAMC_NAO_BASE_ADDRESS + 0x0033C)
    #define RK0_PRE_STANDBY_COUNTER_RK0_PRE_STANDBY_COUNTER    Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_PRE_POWERDOWN_COUNTER            (DRAMC_NAO_BASE_ADDRESS + 0x00340)
    #define RK0_PRE_POWERDOWN_COUNTER_RK0_PRE_POWERDOWN_COUNTER Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_ACT_STANDBY_COUNTER              (DRAMC_NAO_BASE_ADDRESS + 0x00344)
    #define RK0_ACT_STANDBY_COUNTER_RK0_ACT_STANDBY_COUNTER    Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_ACT_POWERDOWN_COUNTER            (DRAMC_NAO_BASE_ADDRESS + 0x00348)
    #define RK0_ACT_POWERDOWN_COUNTER_RK0_ACT_POWERDOWN_COUNTER Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_PRE_STANDBY_COUNTER              (DRAMC_NAO_BASE_ADDRESS + 0x0034C)
    #define RK1_PRE_STANDBY_COUNTER_RK1_PRE_STANDBY_COUNTER    Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_PRE_POWERDOWN_COUNTER            (DRAMC_NAO_BASE_ADDRESS + 0x00350)
    #define RK1_PRE_POWERDOWN_COUNTER_RK1_PRE_POWERDOWN_COUNTER Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_ACT_STANDBY_COUNTER              (DRAMC_NAO_BASE_ADDRESS + 0x00354)
    #define RK1_ACT_STANDBY_COUNTER_RK1_ACT_STANDBY_COUNTER    Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_ACT_POWERDOWN_COUNTER            (DRAMC_NAO_BASE_ADDRESS + 0x00358)
    #define RK1_ACT_POWERDOWN_COUNTER_RK1_ACT_POWERDOWN_COUNTER Fld(32, 0) //[31:0]

#define DRAMC_REG_RK2_PRE_STANDBY_COUNTER              (DRAMC_NAO_BASE_ADDRESS + 0x0035C)
    #define RK2_PRE_STANDBY_COUNTER_RK2_PRE_STANDBY_COUNTER    Fld(32, 0) //[31:0]

#define DRAMC_REG_RK2_PRE_POWERDOWN_COUNTER            (DRAMC_NAO_BASE_ADDRESS + 0x00360)
    #define RK2_PRE_POWERDOWN_COUNTER_RK2_PRE_POWERDOWN_COUNTER Fld(32, 0) //[31:0]

#define DRAMC_REG_RK2_ACT_STANDBY_COUNTER              (DRAMC_NAO_BASE_ADDRESS + 0x00364)
    #define RK2_ACT_STANDBY_COUNTER_RK2_ACT_STANDBY_COUNTER    Fld(32, 0) //[31:0]

#define DRAMC_REG_RK2_ACT_POWERDOWN_COUNTER            (DRAMC_NAO_BASE_ADDRESS + 0x00368)
    #define RK2_ACT_POWERDOWN_COUNTER_RK2_ACT_POWERDOWN_COUNTER Fld(32, 0) //[31:0]

#define DRAMC_REG_DQ0_TOGGLE_COUNTER                   (DRAMC_NAO_BASE_ADDRESS + 0x0036C)
    #define DQ0_TOGGLE_COUNTER_DQ0_TOGGLE_COUNTER              Fld(32, 0) //[31:0]

#define DRAMC_REG_DQ1_TOGGLE_COUNTER                   (DRAMC_NAO_BASE_ADDRESS + 0x00370)
    #define DQ1_TOGGLE_COUNTER_DQ1_TOGGLE_COUNTER              Fld(32, 0) //[31:0]

#define DRAMC_REG_DQ2_TOGGLE_COUNTER                   (DRAMC_NAO_BASE_ADDRESS + 0x00374)
    #define DQ2_TOGGLE_COUNTER_DQ2_TOGGLE_COUNTER              Fld(32, 0) //[31:0]

#define DRAMC_REG_DQ3_TOGGLE_COUNTER                   (DRAMC_NAO_BASE_ADDRESS + 0x00378)
    #define DQ3_TOGGLE_COUNTER_DQ3_TOGGLE_COUNTER              Fld(32, 0) //[31:0]

#define DRAMC_REG_DQ0_TOGGLE_COUNTER_R                 (DRAMC_NAO_BASE_ADDRESS + 0x0037C)
    #define DQ0_TOGGLE_COUNTER_R_DQ0_TOGGLE_COUNTER_R          Fld(32, 0) //[31:0]

#define DRAMC_REG_DQ1_TOGGLE_COUNTER_R                 (DRAMC_NAO_BASE_ADDRESS + 0x00380)
    #define DQ1_TOGGLE_COUNTER_R_DQ1_TOGGLE_COUNTER_R          Fld(32, 0) //[31:0]

#define DRAMC_REG_DQ2_TOGGLE_COUNTER_R                 (DRAMC_NAO_BASE_ADDRESS + 0x00384)
    #define DQ2_TOGGLE_COUNTER_R_DQ2_TOGGLE_COUNTER_R          Fld(32, 0) //[31:0]

#define DRAMC_REG_DQ3_TOGGLE_COUNTER_R                 (DRAMC_NAO_BASE_ADDRESS + 0x00388)
    #define DQ3_TOGGLE_COUNTER_R_DQ3_TOGGLE_COUNTER_R          Fld(32, 0) //[31:0]

#define DRAMC_REG_READ_BYTES_COUNTER                   (DRAMC_NAO_BASE_ADDRESS + 0x0038C)
    #define READ_BYTES_COUNTER_READ_BYTES_COUNTER              Fld(32, 0) //[31:0]

#define DRAMC_REG_WRITE_BYTES_COUNTER                  (DRAMC_NAO_BASE_ADDRESS + 0x00390)
    #define WRITE_BYTES_COUNTER_WRITE_BYTES_COUNTER            Fld(32, 0) //[31:0]

#define DRAMC_REG_MAX_SREF_REQ_TO_ACK_LATENCY_COUNTER  (DRAMC_NAO_BASE_ADDRESS + 0x00394)
    #define MAX_SREF_REQ_TO_ACK_LATENCY_COUNTER_SREF_REQTOACK_MAX_COUNTER Fld(32, 0) //[31:0]

#define DRAMC_REG_MAX_RK1_DRS_LONG_REQ_TO_ACK_LATENCY_COUNTER (DRAMC_NAO_BASE_ADDRESS + 0x00398)
    #define MAX_RK1_DRS_LONG_REQ_TO_ACK_LATENCY_COUNTER_DRS_LONG_REQTOACK_MAX_COUNTER Fld(32, 0) //[31:0]

#define DRAMC_REG_MAX_RK1_DRS_REQ_TO_ACK_LATENCY_COUNTER (DRAMC_NAO_BASE_ADDRESS + 0x0039C)
    #define MAX_RK1_DRS_REQ_TO_ACK_LATENCY_COUNTER_DRS_REQTOACK_MAX_COUNTER Fld(32, 0) //[31:0]

#define DRAMC_REG_DRAMC_IDLE_DCM_COUNTER               (DRAMC_NAO_BASE_ADDRESS + 0x003A0)
    #define DRAMC_IDLE_DCM_COUNTER_DRAMC_IDLE_DCM_COUNTER      Fld(32, 0) //[31:0]

#define DRAMC_REG_DDRPHY_CLK_EN_COUNTER                (DRAMC_NAO_BASE_ADDRESS + 0x003A4)
    #define DDRPHY_CLK_EN_COUNTER_DDRPHY_CLK_EN_COUNTER        Fld(32, 0) //[31:0]

#define DRAMC_REG_DDRPHY_CLK_EN_COMB_COUNTER           (DRAMC_NAO_BASE_ADDRESS + 0x003A8)
    #define DDRPHY_CLK_EN_COMB_COUNTER_DDRPHY_CLK_EN_COMB_COUNTER Fld(32, 0) //[31:0]

#define DRAMC_REG_LAT_COUNTER_CMD0                     (DRAMC_NAO_BASE_ADDRESS + 0x003C0)
    #define LAT_COUNTER_CMD0_LAT_CMD0_CNT_MAX                  Fld(16, 0) //[15:0]
    #define LAT_COUNTER_CMD0_LAT_CMD0_CNT_MAX_HPRI             Fld(1, 16) //[16:16]
    #define LAT_COUNTER_CMD0_LAT_CMD0_CNT_MAX_LLAT             Fld(1, 17) //[17:17]
    #define LAT_COUNTER_CMD0_LAT_CMD0_CNT_MAX_REORDER          Fld(1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD1                     (DRAMC_NAO_BASE_ADDRESS + 0x003C4)
    #define LAT_COUNTER_CMD1_LAT_CMD1_CNT_MAX                  Fld(16, 0) //[15:0]
    #define LAT_COUNTER_CMD1_LAT_CMD1_CNT_MAX_HPRI             Fld(1, 16) //[16:16]
    #define LAT_COUNTER_CMD1_LAT_CMD1_CNT_MAX_LLAT             Fld(1, 17) //[17:17]
    #define LAT_COUNTER_CMD1_LAT_CMD1_CNT_MAX_REORDER          Fld(1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD2                     (DRAMC_NAO_BASE_ADDRESS + 0x003C8)
    #define LAT_COUNTER_CMD2_LAT_CMD2_CNT_MAX                  Fld(16, 0) //[15:0]
    #define LAT_COUNTER_CMD2_LAT_CMD2_CNT_MAX_HPRI             Fld(1, 16) //[16:16]
    #define LAT_COUNTER_CMD2_LAT_CMD2_CNT_MAX_LLAT             Fld(1, 17) //[17:17]
    #define LAT_COUNTER_CMD2_LAT_CMD2_CNT_MAX_REORDER          Fld(1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD3                     (DRAMC_NAO_BASE_ADDRESS + 0x003CC)
    #define LAT_COUNTER_CMD3_LAT_CMD3_CNT_MAX                  Fld(16, 0) //[15:0]
    #define LAT_COUNTER_CMD3_LAT_CMD3_CNT_MAX_HPRI             Fld(1, 16) //[16:16]
    #define LAT_COUNTER_CMD3_LAT_CMD3_CNT_MAX_LLAT             Fld(1, 17) //[17:17]
    #define LAT_COUNTER_CMD3_LAT_CMD3_CNT_MAX_REORDER          Fld(1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD4                     (DRAMC_NAO_BASE_ADDRESS + 0x003D0)
    #define LAT_COUNTER_CMD4_LAT_CMD4_CNT_MAX                  Fld(16, 0) //[15:0]
    #define LAT_COUNTER_CMD4_LAT_CMD4_CNT_MAX_HPRI             Fld(1, 16) //[16:16]
    #define LAT_COUNTER_CMD4_LAT_CMD4_CNT_MAX_LLAT             Fld(1, 17) //[17:17]
    #define LAT_COUNTER_CMD4_LAT_CMD4_CNT_MAX_REORDER          Fld(1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD5                     (DRAMC_NAO_BASE_ADDRESS + 0x003D4)
    #define LAT_COUNTER_CMD5_LAT_CMD5_CNT_MAX                  Fld(16, 0) //[15:0]
    #define LAT_COUNTER_CMD5_LAT_CMD5_CNT_MAX_HPRI             Fld(1, 16) //[16:16]
    #define LAT_COUNTER_CMD5_LAT_CMD5_CNT_MAX_LLAT             Fld(1, 17) //[17:17]
    #define LAT_COUNTER_CMD5_LAT_CMD5_CNT_MAX_REORDER          Fld(1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD6                     (DRAMC_NAO_BASE_ADDRESS + 0x003D8)
    #define LAT_COUNTER_CMD6_LAT_CMD6_CNT_MAX                  Fld(16, 0) //[15:0]
    #define LAT_COUNTER_CMD6_LAT_CMD6_CNT_MAX_HPRI             Fld(1, 16) //[16:16]
    #define LAT_COUNTER_CMD6_LAT_CMD6_CNT_MAX_LLAT             Fld(1, 17) //[17:17]
    #define LAT_COUNTER_CMD6_LAT_CMD6_CNT_MAX_REORDER          Fld(1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD7                     (DRAMC_NAO_BASE_ADDRESS + 0x003DC)
    #define LAT_COUNTER_CMD7_LAT_CMD7_CNT_MAX                  Fld(16, 0) //[15:0]
    #define LAT_COUNTER_CMD7_LAT_CMD7_CNT_MAX_HPRI             Fld(1, 16) //[16:16]
    #define LAT_COUNTER_CMD7_LAT_CMD7_CNT_MAX_LLAT             Fld(1, 17) //[17:17]
    #define LAT_COUNTER_CMD7_LAT_CMD7_CNT_MAX_REORDER          Fld(1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_AVER                     (DRAMC_NAO_BASE_ADDRESS + 0x003E0)
    #define LAT_COUNTER_AVER_LAT_CMD_AVER_CNT                  Fld(32, 0) //[31:0]

#define DRAMC_REG_LAT_COUNTER_NUM                      (DRAMC_NAO_BASE_ADDRESS + 0x003E4)
    #define LAT_COUNTER_NUM_LAT_CMD_NUM                        Fld(16, 0) //[15:0]

#define DRAMC_REG_LAT_COUNTER_BLOCK_ALE                (DRAMC_NAO_BASE_ADDRESS + 0x003E8)
    #define LAT_COUNTER_BLOCK_ALE_CTO_BLOCK_CNT_MAX            Fld(16, 0) //[15:0]

#define DRAMC_REG_DQSSAMPLEV                           (DRAMC_NAO_BASE_ADDRESS + 0x00400)
    #define DQSSAMPLEV_SAMPLE_OUT1_DQS0                        Fld(1, 0) //[0:0]
    #define DQSSAMPLEV_SAMPLE_OUT1_DQS1                        Fld(1, 1) //[1:1]
    #define DQSSAMPLEV_SAMPLE_OUT1_DQS2                        Fld(1, 2) //[2:2]
    #define DQSSAMPLEV_SAMPLE_OUT1_DQS3                        Fld(1, 3) //[3:3]
    #define DQSSAMPLEV_PI_OVERFLOW                             Fld(4, 12) //[15:12]

#define DRAMC_REG_DQSGNWCNT0                           (DRAMC_NAO_BASE_ADDRESS + 0x00408)
    #define DQSGNWCNT0_DQS0F_GATING_COUNTER                    Fld(8, 0) //[7:0]
    #define DQSGNWCNT0_DQS0R_GATING_COUNTER                    Fld(8, 8) //[15:8]
    #define DQSGNWCNT0_DQS1F_GATING_COUNTER                    Fld(8, 16) //[23:16]
    #define DQSGNWCNT0_DQS1R_GATING_COUNTER                    Fld(8, 24) //[31:24]

#define DRAMC_REG_DQSGNWCNT1                           (DRAMC_NAO_BASE_ADDRESS + 0x0040C)
    #define DQSGNWCNT1_DQS2F_GATING_COUNTER                    Fld(8, 0) //[7:0]
    #define DQSGNWCNT1_DQS2R_GATING_COUNTER                    Fld(8, 8) //[15:8]
    #define DQSGNWCNT1_DQS3F_GATING_COUNTER                    Fld(8, 16) //[23:16]
    #define DQSGNWCNT1_DQS3R_GATING_COUNTER                    Fld(8, 24) //[31:24]

#define DRAMC_REG_DQSGNWCNT2                           (DRAMC_NAO_BASE_ADDRESS + 0x00410)
    #define DQSGNWCNT2_DQS0F_POS_GATING_COUNTER                Fld(8, 0) //[7:0]
    #define DQSGNWCNT2_DQS0R_POS_GATING_COUNTER                Fld(8, 8) //[15:8]
    #define DQSGNWCNT2_DQS0F_PRE_GATING_COUNTER                Fld(8, 16) //[23:16]
    #define DQSGNWCNT2_DQS0R_PRE_GATING_COUNTER                Fld(8, 24) //[31:24]

#define DRAMC_REG_DQSGNWCNT3                           (DRAMC_NAO_BASE_ADDRESS + 0x00414)
    #define DQSGNWCNT3_DQS1F_POS_GATING_COUNTER                Fld(8, 0) //[7:0]
    #define DQSGNWCNT3_DQS1R_POS_GATING_COUNTER                Fld(8, 8) //[15:8]
    #define DQSGNWCNT3_DQS1F_PRE_GATING_COUNTER                Fld(8, 16) //[23:16]
    #define DQSGNWCNT3_DQS1R_PRE_GATING_COUNTER                Fld(8, 24) //[31:24]

#define DRAMC_REG_DQSGNWCNT4                           (DRAMC_NAO_BASE_ADDRESS + 0x00418)
    #define DQSGNWCNT4_DQS2F_POS_GATING_COUNTER                Fld(8, 0) //[7:0]
    #define DQSGNWCNT4_DQS2R_POS_GATING_COUNTER                Fld(8, 8) //[15:8]
    #define DQSGNWCNT4_DQS2F_PRE_GATING_COUNTER                Fld(8, 16) //[23:16]
    #define DQSGNWCNT4_DQS2R_PRE_GATING_COUNTER                Fld(8, 24) //[31:24]

#define DRAMC_REG_DQSGNWCNT5                           (DRAMC_NAO_BASE_ADDRESS + 0x0041C)
    #define DQSGNWCNT5_DQS3F_POS_GATING_COUNTER                Fld(8, 0) //[7:0]
    #define DQSGNWCNT5_DQS3R_POS_GATING_COUNTER                Fld(8, 8) //[15:8]
    #define DQSGNWCNT5_DQS3F_PRE_GATING_COUNTER                Fld(8, 16) //[23:16]
    #define DQSGNWCNT5_DQS3R_PRE_GATING_COUNTER                Fld(8, 24) //[31:24]

#define DRAMC_REG_TOGGLE_CNT                           (DRAMC_NAO_BASE_ADDRESS + 0x00420)
    #define TOGGLE_CNT_TOGGLE_CNT                              Fld(32, 0) //[31:0]

#define DRAMC_REG_DQS0_ERR_CNT                         (DRAMC_NAO_BASE_ADDRESS + 0x00424)
    #define DQS0_ERR_CNT_DQS0_ERR_CNT                          Fld(32, 0) //[31:0]

#define DRAMC_REG_DQ_ERR_CNT0                          (DRAMC_NAO_BASE_ADDRESS + 0x00428)
    #define DQ_ERR_CNT0_DQ_ERR_CNT0                            Fld(32, 0) //[31:0]

#define DRAMC_REG_DQS1_ERR_CNT                         (DRAMC_NAO_BASE_ADDRESS + 0x0042C)
    #define DQS1_ERR_CNT_DQS1_ERR_CNT                          Fld(32, 0) //[31:0]

#define DRAMC_REG_DQ_ERR_CNT1                          (DRAMC_NAO_BASE_ADDRESS + 0x00430)
    #define DQ_ERR_CNT1_DQ_ERR_CNT1                            Fld(32, 0) //[31:0]

#define DRAMC_REG_DQS2_ERR_CNT                         (DRAMC_NAO_BASE_ADDRESS + 0x00434)
    #define DQS2_ERR_CNT_DQS2_ERR_CNT                          Fld(32, 0) //[31:0]

#define DRAMC_REG_DQ_ERR_CNT2                          (DRAMC_NAO_BASE_ADDRESS + 0x00438)
    #define DQ_ERR_CNT2_DQ_ERR_CNT2                            Fld(32, 0) //[31:0]

#define DRAMC_REG_DQS3_ERR_CNT                         (DRAMC_NAO_BASE_ADDRESS + 0x0043C)
    #define DQS3_ERR_CNT_DQS3_ERR_CNT                          Fld(32, 0) //[31:0]

#define DRAMC_REG_DQ_ERR_CNT3                          (DRAMC_NAO_BASE_ADDRESS + 0x00440)
    #define DQ_ERR_CNT3_DQ_ERR_CNT3                            Fld(32, 0) //[31:0]

#define DRAMC_REG_IORGCNT                              (DRAMC_NAO_BASE_ADDRESS + 0x00450)
    #define IORGCNT_IO_RING_COUNTER_K                          Fld(16, 0) //[15:0]
    #define IORGCNT_IO_RING_COUNTER                            Fld(16, 16) //[31:16]

#define DRAMC_REG_DQSG_RETRY_STATE                     (DRAMC_NAO_BASE_ADDRESS + 0x00454)
    #define DQSG_RETRY_STATE_DQSG_RETRY_1ST_ST                 Fld(8, 0) //[7:0]
    #define DQSG_RETRY_STATE_DQSG_RETRY_2ND_ST                 Fld(8, 8) //[15:8]
    #define DQSG_RETRY_STATE_DQSG_RETRY_3RD_ST                 Fld(8, 16) //[23:16]
    #define DQSG_RETRY_STATE_DQSG_RETRY_4TH_ST                 Fld(8, 24) //[31:24]

#define DRAMC_REG_DQSG_RETRY_STATE1                    (DRAMC_NAO_BASE_ADDRESS + 0x00458)
    #define DQSG_RETRY_STATE1_RETRY_DONE_ALL                   Fld(1, 0) //[0:0]
    #define DQSG_RETRY_STATE1_SELPH_RODTEN_USABLE              Fld(1, 1) //[1:1]
    #define DQSG_RETRY_STATE1_TDQSCK_DONE                      Fld(1, 4) //[4:4]
    #define DQSG_RETRY_STATE1_IMPCAL_N_ERROR                   Fld(1, 8) //[8:8]
    #define DQSG_RETRY_STATE1_IMPCAL_P_ERROR                   Fld(1, 9) //[9:9]
    #define DQSG_RETRY_STATE1_IMPCAL_DONE                      Fld(1, 10) //[10:10]
    #define DQSG_RETRY_STATE1_STB_GATING_ERR                   Fld(1, 16) //[16:16]
    #define DQSG_RETRY_STATE1_R_OTHER_SHU_GP_GATING_ERR        Fld(2, 17) //[18:17]
    #define DQSG_RETRY_STATE1_R_MPDIV_SHU_GP_GATING_ERR        Fld(3, 19) //[21:19]
    #define DQSG_RETRY_STATE1_DQSG_RETRY_5TH_ST                Fld(8, 24) //[31:24]

#define DRAMC_REG_IMPCAL_STATUS1                       (DRAMC_NAO_BASE_ADDRESS + 0x00460)
    #define IMPCAL_STATUS1_DRVNDQ_SAVE2                        Fld(5, 0) //[4:0]
    #define IMPCAL_STATUS1_DRVPDQ_SAVE2                        Fld(5, 5) //[9:5]
    #define IMPCAL_STATUS1_DRVNDQS_SAVE1                       Fld(5, 10) //[14:10]
    #define IMPCAL_STATUS1_DRVPDQS_SAVE1                       Fld(5, 15) //[19:15]
    #define IMPCAL_STATUS1_DRVNDQS_SAVE2                       Fld(5, 20) //[24:20]
    #define IMPCAL_STATUS1_DRVPDQS_SAVE2                       Fld(5, 25) //[29:25]

#define DRAMC_REG_IMPCAL_STATUS2                       (DRAMC_NAO_BASE_ADDRESS + 0x00464)
    #define IMPCAL_STATUS2_DRVNCMD_SAVE1                       Fld(5, 0) //[4:0]
    #define IMPCAL_STATUS2_DRVPCMD_SAVE1                       Fld(5, 5) //[9:5]
    #define IMPCAL_STATUS2_DRVNCMD_SAVE2                       Fld(5, 10) //[14:10]
    #define IMPCAL_STATUS2_DRVPCMD_SAVE2                       Fld(5, 15) //[19:15]
    #define IMPCAL_STATUS2_DRVNDQ_SAVE1                        Fld(5, 20) //[24:20]
    #define IMPCAL_STATUS2_DRVPDQ_SAVE1                        Fld(5, 25) //[29:25]

#define DRAMC_REG_DQDRV_STATUS                         (DRAMC_NAO_BASE_ADDRESS + 0x00468)
    #define DQDRV_STATUS_DRVNDQ_2                              Fld(5, 0) //[4:0]
    #define DQDRV_STATUS_DRVPDQ_2                              Fld(5, 5) //[9:5]
    #define DQDRV_STATUS_DRVNDQS_1                             Fld(5, 10) //[14:10]
    #define DQDRV_STATUS_DRVPDQS_1                             Fld(5, 15) //[19:15]
    #define DQDRV_STATUS_DRVNDQS_2                             Fld(5, 20) //[24:20]
    #define DQDRV_STATUS_DRVPDQS_2                             Fld(5, 25) //[29:25]

#define DRAMC_REG_CMDDRV_STATUS                        (DRAMC_NAO_BASE_ADDRESS + 0x0046C)
    #define CMDDRV_STATUS_DRVNCMD_1                            Fld(5, 0) //[4:0]
    #define CMDDRV_STATUS_DRVPCMD_1                            Fld(5, 5) //[9:5]
    #define CMDDRV_STATUS_DRVNCMD_2                            Fld(5, 10) //[14:10]
    #define CMDDRV_STATUS_DRVPCMD_2                            Fld(5, 15) //[19:15]
    #define CMDDRV_STATUS_DRVNDQ_1                             Fld(5, 20) //[24:20]
    #define CMDDRV_STATUS_DRVPDQ_1                             Fld(5, 25) //[29:25]

#define DRAMC_REG_CMDDRV1                              (DRAMC_NAO_BASE_ADDRESS + 0x00470)
    #define CMDDRV1_CMDDRV1                                    Fld(32, 0) //[31:0]

#define DRAMC_REG_CMDDRV2                              (DRAMC_NAO_BASE_ADDRESS + 0x00474)
    #define CMDDRV2_CMDDRV2                                    Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_DQSOSC_STATUS                    (DRAMC_NAO_BASE_ADDRESS + 0x00600)
    #define RK0_DQSOSC_STATUS_MR18_REG                         Fld(16, 0) //[15:0]
    #define RK0_DQSOSC_STATUS_MR19_REG                         Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_DQSOSC_DELTA                     (DRAMC_NAO_BASE_ADDRESS + 0x00604)
    #define RK0_DQSOSC_DELTA_ABS_RK0_DQSOSC_DELTA              Fld(16, 0) //[15:0]
    #define RK0_DQSOSC_DELTA_SIGN_RK0_DQSOSC_DELTA             Fld(1, 16) //[16:16]
    #define RK0_DQSOSC_DELTA_DQSOCSR_RESPONSE                  Fld(1, 17) //[17:17]
    #define RK0_DQSOSC_DELTA_H_DQSOSCLSBR_REQ                  Fld(1, 18) //[18:18]
    #define RK0_DQSOSC_DELTA_DQSOSC_INT_RK0                    Fld(1, 19) //[19:19]

#define DRAMC_REG_RK0_DQSOSC_DELTA2                    (DRAMC_NAO_BASE_ADDRESS + 0x00608)
    #define RK0_DQSOSC_DELTA2_ABS_RK0_DQSOSC_B1_DELTA          Fld(16, 0) //[15:0]
    #define RK0_DQSOSC_DELTA2_SIGN_RK0_DQSOSC_B1_DELTA         Fld(1, 16) //[16:16]

#define DRAMC_REG_RK0_CURRENT_TX_SETTING1              (DRAMC_NAO_BASE_ADDRESS + 0x00610)
    #define RK0_CURRENT_TX_SETTING1_REG_TX_DLY_DQ0_MOD         Fld(3, 0) //[2:0]
    #define RK0_CURRENT_TX_SETTING1_REG_TX_DLY_DQ1_MOD         Fld(3, 4) //[6:4]
    #define RK0_CURRENT_TX_SETTING1_REG_TX_DLY_DQ2_MOD         Fld(3, 8) //[10:8]
    #define RK0_CURRENT_TX_SETTING1_REG_TX_DLY_DQ3_MOD         Fld(3, 12) //[14:12]
    #define RK0_CURRENT_TX_SETTING1_REG_TX_DLY_DQM0_MOD        Fld(3, 16) //[18:16]
    #define RK0_CURRENT_TX_SETTING1_REG_TX_DLY_DQM1_MOD        Fld(3, 20) //[22:20]
    #define RK0_CURRENT_TX_SETTING1_REG_TX_DLY_DQM2_MOD        Fld(3, 24) //[26:24]
    #define RK0_CURRENT_TX_SETTING1_REG_TX_DLY_DQM3_MOD        Fld(3, 28) //[30:28]

#define DRAMC_REG_RK0_CURRENT_TX_SETTING2              (DRAMC_NAO_BASE_ADDRESS + 0x00614)
    #define RK0_CURRENT_TX_SETTING2_REG_DLY_DQ0_MOD            Fld(3, 0) //[2:0]
    #define RK0_CURRENT_TX_SETTING2_REG_DLY_DQ1_MOD            Fld(3, 4) //[6:4]
    #define RK0_CURRENT_TX_SETTING2_REG_DLY_DQ2_MOD            Fld(3, 8) //[10:8]
    #define RK0_CURRENT_TX_SETTING2_REG_DLY_DQ3_MOD            Fld(3, 12) //[14:12]
    #define RK0_CURRENT_TX_SETTING2_REG_DLY_DQM0_MOD           Fld(3, 16) //[18:16]
    #define RK0_CURRENT_TX_SETTING2_REG_DLY_DQM1_MOD           Fld(3, 20) //[22:20]
    #define RK0_CURRENT_TX_SETTING2_REG_DLY_DQM2_MOD           Fld(3, 24) //[26:24]
    #define RK0_CURRENT_TX_SETTING2_REG_DLY_DQM3_MOD           Fld(3, 28) //[30:28]

#define DRAMC_REG_RK0_CURRENT_TX_SETTING3              (DRAMC_NAO_BASE_ADDRESS + 0x00618)
    #define RK0_CURRENT_TX_SETTING3_REG_TX_DLY_OEN_DQ0_MOD     Fld(3, 0) //[2:0]
    #define RK0_CURRENT_TX_SETTING3_REG_TX_DLY_OEN_DQ1_MOD     Fld(3, 4) //[6:4]
    #define RK0_CURRENT_TX_SETTING3_REG_TX_DLY_OEN_DQ2_MOD     Fld(3, 8) //[10:8]
    #define RK0_CURRENT_TX_SETTING3_REG_TX_DLY_OEN_DQ3_MOD     Fld(3, 12) //[14:12]
    #define RK0_CURRENT_TX_SETTING3_REG_TX_DLY_OEN_DQM0_MOD    Fld(3, 16) //[18:16]
    #define RK0_CURRENT_TX_SETTING3_REG_TX_DLY_OEN_DQM1_MOD    Fld(3, 20) //[22:20]
    #define RK0_CURRENT_TX_SETTING3_REG_TX_DLY_OEN_DQM2_MOD    Fld(3, 24) //[26:24]
    #define RK0_CURRENT_TX_SETTING3_REG_TX_DLY_OEN_DQM3_MOD    Fld(3, 28) //[30:28]

#define DRAMC_REG_RK0_CURRENT_TX_SETTING4              (DRAMC_NAO_BASE_ADDRESS + 0x0061C)
    #define RK0_CURRENT_TX_SETTING4_REG_DLY_OEN_DQ0_MOD        Fld(3, 0) //[2:0]
    #define RK0_CURRENT_TX_SETTING4_REG_DLY_OEN_DQ1_MOD        Fld(3, 4) //[6:4]
    #define RK0_CURRENT_TX_SETTING4_REG_DLY_OEN_DQ2_MOD        Fld(3, 8) //[10:8]
    #define RK0_CURRENT_TX_SETTING4_REG_DLY_OEN_DQ3_MOD        Fld(3, 12) //[14:12]
    #define RK0_CURRENT_TX_SETTING4_REG_DLY_OEN_DQM0_MOD       Fld(3, 16) //[18:16]
    #define RK0_CURRENT_TX_SETTING4_REG_DLY_OEN_DQM1_MOD       Fld(3, 20) //[22:20]
    #define RK0_CURRENT_TX_SETTING4_REG_DLY_OEN_DQM2_MOD       Fld(3, 24) //[26:24]
    #define RK0_CURRENT_TX_SETTING4_REG_DLY_OEN_DQM3_MOD       Fld(3, 28) //[30:28]

#define DRAMC_REG_RK0_DUMMY_RD_DATA0                   (DRAMC_NAO_BASE_ADDRESS + 0x00620)
    #define RK0_DUMMY_RD_DATA0_DUMMY_RD_RK0_DATA0              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_DUMMY_RD_DATA1                   (DRAMC_NAO_BASE_ADDRESS + 0x00624)
    #define RK0_DUMMY_RD_DATA1_DUMMY_RD_RK0_DATA1              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_DUMMY_RD_DATA2                   (DRAMC_NAO_BASE_ADDRESS + 0x00628)
    #define RK0_DUMMY_RD_DATA2_DUMMY_RD_RK0_DATA2              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_DUMMY_RD_DATA3                   (DRAMC_NAO_BASE_ADDRESS + 0x0062C)
    #define RK0_DUMMY_RD_DATA3_DUMMY_RD_RK0_DATA3              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_B0_STB_MAX_MIN_DLY               (DRAMC_NAO_BASE_ADDRESS + 0x00630)
    #define RK0_B0_STB_MAX_MIN_DLY_RK0_B0_STBEN_MIN_DLY        Fld(12, 0) //[11:0]
    #define RK0_B0_STB_MAX_MIN_DLY_RK0_B0_STBEN_MAX_DLY        Fld(12, 16) //[27:16]

#define DRAMC_REG_RK0_B1_STB_MAX_MIN_DLY               (DRAMC_NAO_BASE_ADDRESS + 0x00634)
    #define RK0_B1_STB_MAX_MIN_DLY_RK0_B1_STBEN_MIN_DLY        Fld(12, 0) //[11:0]
    #define RK0_B1_STB_MAX_MIN_DLY_RK0_B1_STBEN_MAX_DLY        Fld(12, 16) //[27:16]

#define DRAMC_REG_RK0_B2_STB_MAX_MIN_DLY               (DRAMC_NAO_BASE_ADDRESS + 0x00638)
    #define RK0_B2_STB_MAX_MIN_DLY_RK0_B2_STBEN_MIN_DLY        Fld(12, 0) //[11:0]
    #define RK0_B2_STB_MAX_MIN_DLY_RK0_B2_STBEN_MAX_DLY        Fld(12, 16) //[27:16]

#define DRAMC_REG_RK0_B3_STB_MAX_MIN_DLY               (DRAMC_NAO_BASE_ADDRESS + 0x0063C)
    #define RK0_B3_STB_MAX_MIN_DLY_RK0_B3_STBEN_MIN_DLY        Fld(12, 0) //[11:0]
    #define RK0_B3_STB_MAX_MIN_DLY_RK0_B3_STBEN_MAX_DLY        Fld(12, 16) //[27:16]

#define DRAMC_REG_RK0_DQSIENDLY                        (DRAMC_NAO_BASE_ADDRESS + 0x00640)
    #define RK0_DQSIENDLY_R0DQS0IENDLY                         Fld(7, 0) //[6:0]
    #define RK0_DQSIENDLY_R0DQS1IENDLY                         Fld(7, 8) //[14:8]
    #define RK0_DQSIENDLY_R0DQS2IENDLY                         Fld(7, 16) //[22:16]
    #define RK0_DQSIENDLY_R0DQS3IENDLY                         Fld(7, 24) //[30:24]

#define DRAMC_REG_RK0_DQSIENUIDLY                      (DRAMC_NAO_BASE_ADDRESS + 0x00644)
    #define RK0_DQSIENUIDLY_R0DQS0IENUIDLY                     Fld(6, 0) //[5:0]
    #define RK0_DQSIENUIDLY_R0DQS1IENUIDLY                     Fld(6, 8) //[13:8]
    #define RK0_DQSIENUIDLY_R0DQS2IENUIDLY                     Fld(6, 16) //[21:16]
    #define RK0_DQSIENUIDLY_R0DQS3IENUIDLY                     Fld(6, 24) //[29:24]

#define DRAMC_REG_RK0_DQSIENUIDLY_P1                   (DRAMC_NAO_BASE_ADDRESS + 0x00648)
    #define RK0_DQSIENUIDLY_P1_R0DQS0IENUIDLY_P1               Fld(6, 0) //[5:0]
    #define RK0_DQSIENUIDLY_P1_R0DQS1IENUIDLY_P1               Fld(6, 8) //[13:8]
    #define RK0_DQSIENUIDLY_P1_R0DQS2IENUIDLY_P1               Fld(6, 16) //[21:16]
    #define RK0_DQSIENUIDLY_P1_R0DQS3IENUIDLY_P1               Fld(6, 24) //[29:24]

#define DRAMC_REG_RK0_DQS_STBCALDEC_CNT1               (DRAMC_NAO_BASE_ADDRESS + 0x00650)
    #define RK0_DQS_STBCALDEC_CNT1_RK0_DQS0_STBCALDEC_CNT      Fld(16, 0) //[15:0]
    #define RK0_DQS_STBCALDEC_CNT1_RK0_DQS1_STBCALDEC_CNT      Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_DQS_STBCALDEC_CNT2               (DRAMC_NAO_BASE_ADDRESS + 0x00654)
    #define RK0_DQS_STBCALDEC_CNT2_RK0_DQS2_STBCALDEC_CNT      Fld(16, 0) //[15:0]
    #define RK0_DQS_STBCALDEC_CNT2_RK0_DQS3_STBCALDEC_CNT      Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_DQS_STBCALINC_CNT1               (DRAMC_NAO_BASE_ADDRESS + 0x00658)
    #define RK0_DQS_STBCALINC_CNT1_RK0_DQS0_STBCALINC_CNT      Fld(16, 0) //[15:0]
    #define RK0_DQS_STBCALINC_CNT1_RK0_DQS1_STBCALINC_CNT      Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_DQS_STBCALINC_CNT2               (DRAMC_NAO_BASE_ADDRESS + 0x0065C)
    #define RK0_DQS_STBCALINC_CNT2_RK0_DQS2_STBCALINC_CNT      Fld(16, 0) //[15:0]
    #define RK0_DQS_STBCALINC_CNT2_RK0_DQS3_STBCALINC_CNT      Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_PI_DQ_CAL                        (DRAMC_NAO_BASE_ADDRESS + 0x00660)
    #define RK0_PI_DQ_CAL_RK0_ARPI_DQ_B0_CAL                   Fld(6, 0) //[5:0]
    #define RK0_PI_DQ_CAL_RK0_ARPI_DQ_B1_CAL                   Fld(6, 8) //[13:8]
    #define RK0_PI_DQ_CAL_PI_DQ_ADJ_RK0                        Fld(6, 16) //[21:16]
    #define RK0_PI_DQ_CAL_PI_DQ_ADJ_RK0_OVERFLOW               Fld(1, 22) //[22:22]
    #define RK0_PI_DQ_CAL_RK0_B0_PI_CHANGE_DBG                 Fld(1, 23) //[23:23]
    #define RK0_PI_DQ_CAL_PI_DQ_ADJ_RK0_B1                     Fld(6, 24) //[29:24]
    #define RK0_PI_DQ_CAL_PI_DQ_ADJ_RK0_B1_OVERFLOW            Fld(1, 30) //[30:30]
    #define RK0_PI_DQ_CAL_RK0_B1_PI_CHANGE_DBG                 Fld(1, 31) //[31:31]

#define DRAMC_REG_RK0_DQSG_RETRY_FLAG                  (DRAMC_NAO_BASE_ADDRESS + 0x00664)
    #define RK0_DQSG_RETRY_FLAG_RK0_RETRY_DONE0                Fld(1, 0) //[0:0]
    #define RK0_DQSG_RETRY_FLAG_RK0_RETRY_DONE1                Fld(1, 1) //[1:1]
    #define RK0_DQSG_RETRY_FLAG_RK0_RETRY_DONE2                Fld(1, 2) //[2:2]
    #define RK0_DQSG_RETRY_FLAG_RK0_RETRY_DONE3                Fld(1, 3) //[3:3]
    #define RK0_DQSG_RETRY_FLAG_RK0_RETRY_FAIL0                Fld(1, 16) //[16:16]
    #define RK0_DQSG_RETRY_FLAG_RK0_RETRY_FAIL1                Fld(1, 17) //[17:17]
    #define RK0_DQSG_RETRY_FLAG_RK0_RETRY_FAIL2                Fld(1, 18) //[18:18]
    #define RK0_DQSG_RETRY_FLAG_RK0_RETRY_FAIL3                Fld(1, 19) //[19:19]

#define DRAMC_REG_RK0_PI_DQM_CAL                       (DRAMC_NAO_BASE_ADDRESS + 0x00668)
    #define RK0_PI_DQM_CAL_RK0_ARPI_DQM_B0_CAL                 Fld(6, 0) //[5:0]
    #define RK0_PI_DQM_CAL_RK0_ARPI_DQM_B1_CAL                 Fld(6, 8) //[13:8]

#define DRAMC_REG_RK0_DQS0_STBCAL_CNT                  (DRAMC_NAO_BASE_ADDRESS + 0x00670)
    #define RK0_DQS0_STBCAL_CNT_R0_DQS0_STBCAL_CNT             Fld(17, 0) //[16:0]

#define DRAMC_REG_RK0_DQS1_STBCAL_CNT                  (DRAMC_NAO_BASE_ADDRESS + 0x00674)
    #define RK0_DQS1_STBCAL_CNT_R0_DQS1_STBCAL_CNT             Fld(17, 0) //[16:0]

#define DRAMC_REG_RK0_DQS2_STBCAL_CNT                  (DRAMC_NAO_BASE_ADDRESS + 0x00678)
    #define RK0_DQS2_STBCAL_CNT_R0_DQS2_STBCAL_CNT             Fld(17, 0) //[16:0]

#define DRAMC_REG_RK0_DQS3_STBCAL_CNT                  (DRAMC_NAO_BASE_ADDRESS + 0x0067C)
    #define RK0_DQS3_STBCAL_CNT_R0_DQS3_STBCAL_CNT             Fld(17, 0) //[16:0]

#define DRAMC_REG_RK0_B01_STB_DBG_INFO_00              (DRAMC_NAO_BASE_ADDRESS + 0x00680)
    #define RK0_B01_STB_DBG_INFO_00_RK0_B0_STB_DBG_INFO_00     Fld(16, 0) //[15:0]
    #define RK0_B01_STB_DBG_INFO_00_RK0_B1_STB_DBG_INFO_00     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B01_STB_DBG_INFO_01              (DRAMC_NAO_BASE_ADDRESS + 0x00684)
    #define RK0_B01_STB_DBG_INFO_01_RK0_B0_STB_DBG_INFO_01     Fld(16, 0) //[15:0]
    #define RK0_B01_STB_DBG_INFO_01_RK0_B1_STB_DBG_INFO_01     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B01_STB_DBG_INFO_02              (DRAMC_NAO_BASE_ADDRESS + 0x00688)
    #define RK0_B01_STB_DBG_INFO_02_RK0_B0_STB_DBG_INFO_02     Fld(16, 0) //[15:0]
    #define RK0_B01_STB_DBG_INFO_02_RK0_B1_STB_DBG_INFO_02     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B01_STB_DBG_INFO_03              (DRAMC_NAO_BASE_ADDRESS + 0x0068C)
    #define RK0_B01_STB_DBG_INFO_03_RK0_B0_STB_DBG_INFO_03     Fld(16, 0) //[15:0]
    #define RK0_B01_STB_DBG_INFO_03_RK0_B1_STB_DBG_INFO_03     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B01_STB_DBG_INFO_04              (DRAMC_NAO_BASE_ADDRESS + 0x00690)
    #define RK0_B01_STB_DBG_INFO_04_RK0_B0_STB_DBG_INFO_04     Fld(16, 0) //[15:0]
    #define RK0_B01_STB_DBG_INFO_04_RK0_B1_STB_DBG_INFO_04     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B01_STB_DBG_INFO_05              (DRAMC_NAO_BASE_ADDRESS + 0x00694)
    #define RK0_B01_STB_DBG_INFO_05_RK0_B0_STB_DBG_INFO_05     Fld(16, 0) //[15:0]
    #define RK0_B01_STB_DBG_INFO_05_RK0_B1_STB_DBG_INFO_05     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B01_STB_DBG_INFO_06              (DRAMC_NAO_BASE_ADDRESS + 0x00698)
    #define RK0_B01_STB_DBG_INFO_06_RK0_B0_STB_DBG_INFO_06     Fld(16, 0) //[15:0]
    #define RK0_B01_STB_DBG_INFO_06_RK0_B1_STB_DBG_INFO_06     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B01_STB_DBG_INFO_07              (DRAMC_NAO_BASE_ADDRESS + 0x0069C)
    #define RK0_B01_STB_DBG_INFO_07_RK0_B0_STB_DBG_INFO_07     Fld(16, 0) //[15:0]
    #define RK0_B01_STB_DBG_INFO_07_RK0_B1_STB_DBG_INFO_07     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B01_STB_DBG_INFO_08              (DRAMC_NAO_BASE_ADDRESS + 0x006A0)
    #define RK0_B01_STB_DBG_INFO_08_RK0_B0_STB_DBG_INFO_08     Fld(16, 0) //[15:0]
    #define RK0_B01_STB_DBG_INFO_08_RK0_B1_STB_DBG_INFO_08     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B01_STB_DBG_INFO_09              (DRAMC_NAO_BASE_ADDRESS + 0x006A4)
    #define RK0_B01_STB_DBG_INFO_09_RK0_B0_STB_DBG_INFO_09     Fld(16, 0) //[15:0]
    #define RK0_B01_STB_DBG_INFO_09_RK0_B1_STB_DBG_INFO_09     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B01_STB_DBG_INFO_10              (DRAMC_NAO_BASE_ADDRESS + 0x006A8)
    #define RK0_B01_STB_DBG_INFO_10_RK0_B0_STB_DBG_INFO_10     Fld(16, 0) //[15:0]
    #define RK0_B01_STB_DBG_INFO_10_RK0_B1_STB_DBG_INFO_10     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B01_STB_DBG_INFO_11              (DRAMC_NAO_BASE_ADDRESS + 0x006AC)
    #define RK0_B01_STB_DBG_INFO_11_RK0_B0_STB_DBG_INFO_11     Fld(16, 0) //[15:0]
    #define RK0_B01_STB_DBG_INFO_11_RK0_B1_STB_DBG_INFO_11     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B01_STB_DBG_INFO_12              (DRAMC_NAO_BASE_ADDRESS + 0x006B0)
    #define RK0_B01_STB_DBG_INFO_12_RK0_B0_STB_DBG_INFO_12     Fld(16, 0) //[15:0]
    #define RK0_B01_STB_DBG_INFO_12_RK0_B1_STB_DBG_INFO_12     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B01_STB_DBG_INFO_13              (DRAMC_NAO_BASE_ADDRESS + 0x006B4)
    #define RK0_B01_STB_DBG_INFO_13_RK0_B0_STB_DBG_INFO_13     Fld(16, 0) //[15:0]
    #define RK0_B01_STB_DBG_INFO_13_RK0_B1_STB_DBG_INFO_13     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B01_STB_DBG_INFO_14              (DRAMC_NAO_BASE_ADDRESS + 0x006B8)
    #define RK0_B01_STB_DBG_INFO_14_RK0_B0_STB_DBG_INFO_14     Fld(16, 0) //[15:0]
    #define RK0_B01_STB_DBG_INFO_14_RK0_B1_STB_DBG_INFO_14     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B01_STB_DBG_INFO_15              (DRAMC_NAO_BASE_ADDRESS + 0x006BC)
    #define RK0_B01_STB_DBG_INFO_15_RK0_B0_STB_DBG_INFO_15     Fld(16, 0) //[15:0]
    #define RK0_B01_STB_DBG_INFO_15_RK0_B1_STB_DBG_INFO_15     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B23_STB_DBG_INFO_00              (DRAMC_NAO_BASE_ADDRESS + 0x006C0)
    #define RK0_B23_STB_DBG_INFO_00_RK0_B2_STB_DBG_INFO_00     Fld(16, 0) //[15:0]
    #define RK0_B23_STB_DBG_INFO_00_RK0_B3_STB_DBG_INFO_00     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B23_STB_DBG_INFO_01              (DRAMC_NAO_BASE_ADDRESS + 0x006C4)
    #define RK0_B23_STB_DBG_INFO_01_RK0_B2_STB_DBG_INFO_01     Fld(16, 0) //[15:0]
    #define RK0_B23_STB_DBG_INFO_01_RK0_B3_STB_DBG_INFO_01     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B23_STB_DBG_INFO_02              (DRAMC_NAO_BASE_ADDRESS + 0x006C8)
    #define RK0_B23_STB_DBG_INFO_02_RK0_B2_STB_DBG_INFO_02     Fld(16, 0) //[15:0]
    #define RK0_B23_STB_DBG_INFO_02_RK0_B3_STB_DBG_INFO_02     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B23_STB_DBG_INFO_03              (DRAMC_NAO_BASE_ADDRESS + 0x006CC)
    #define RK0_B23_STB_DBG_INFO_03_RK0_B2_STB_DBG_INFO_03     Fld(16, 0) //[15:0]
    #define RK0_B23_STB_DBG_INFO_03_RK0_B3_STB_DBG_INFO_03     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B23_STB_DBG_INFO_04              (DRAMC_NAO_BASE_ADDRESS + 0x006D0)
    #define RK0_B23_STB_DBG_INFO_04_RK0_B2_STB_DBG_INFO_04     Fld(16, 0) //[15:0]
    #define RK0_B23_STB_DBG_INFO_04_RK0_B3_STB_DBG_INFO_04     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B23_STB_DBG_INFO_05              (DRAMC_NAO_BASE_ADDRESS + 0x006D4)
    #define RK0_B23_STB_DBG_INFO_05_RK0_B2_STB_DBG_INFO_05     Fld(16, 0) //[15:0]
    #define RK0_B23_STB_DBG_INFO_05_RK0_B3_STB_DBG_INFO_05     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B23_STB_DBG_INFO_06              (DRAMC_NAO_BASE_ADDRESS + 0x006D8)
    #define RK0_B23_STB_DBG_INFO_06_RK0_B2_STB_DBG_INFO_06     Fld(16, 0) //[15:0]
    #define RK0_B23_STB_DBG_INFO_06_RK0_B3_STB_DBG_INFO_06     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B23_STB_DBG_INFO_07              (DRAMC_NAO_BASE_ADDRESS + 0x006DC)
    #define RK0_B23_STB_DBG_INFO_07_RK0_B2_STB_DBG_INFO_07     Fld(16, 0) //[15:0]
    #define RK0_B23_STB_DBG_INFO_07_RK0_B3_STB_DBG_INFO_07     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B23_STB_DBG_INFO_08              (DRAMC_NAO_BASE_ADDRESS + 0x006E0)
    #define RK0_B23_STB_DBG_INFO_08_RK0_B2_STB_DBG_INFO_08     Fld(16, 0) //[15:0]
    #define RK0_B23_STB_DBG_INFO_08_RK0_B3_STB_DBG_INFO_08     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B23_STB_DBG_INFO_09              (DRAMC_NAO_BASE_ADDRESS + 0x006E4)
    #define RK0_B23_STB_DBG_INFO_09_RK0_B2_STB_DBG_INFO_09     Fld(16, 0) //[15:0]
    #define RK0_B23_STB_DBG_INFO_09_RK0_B3_STB_DBG_INFO_09     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B23_STB_DBG_INFO_10              (DRAMC_NAO_BASE_ADDRESS + 0x006E8)
    #define RK0_B23_STB_DBG_INFO_10_RK0_B2_STB_DBG_INFO_10     Fld(16, 0) //[15:0]
    #define RK0_B23_STB_DBG_INFO_10_RK0_B3_STB_DBG_INFO_10     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B23_STB_DBG_INFO_11              (DRAMC_NAO_BASE_ADDRESS + 0x006EC)
    #define RK0_B23_STB_DBG_INFO_11_RK0_B2_STB_DBG_INFO_11     Fld(16, 0) //[15:0]
    #define RK0_B23_STB_DBG_INFO_11_RK0_B3_STB_DBG_INFO_11     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B23_STB_DBG_INFO_12              (DRAMC_NAO_BASE_ADDRESS + 0x006F0)
    #define RK0_B23_STB_DBG_INFO_12_RK0_B2_STB_DBG_INFO_12     Fld(16, 0) //[15:0]
    #define RK0_B23_STB_DBG_INFO_12_RK0_B3_STB_DBG_INFO_12     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B23_STB_DBG_INFO_13              (DRAMC_NAO_BASE_ADDRESS + 0x006F4)
    #define RK0_B23_STB_DBG_INFO_13_RK0_B2_STB_DBG_INFO_13     Fld(16, 0) //[15:0]
    #define RK0_B23_STB_DBG_INFO_13_RK0_B3_STB_DBG_INFO_13     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B23_STB_DBG_INFO_14              (DRAMC_NAO_BASE_ADDRESS + 0x006F8)
    #define RK0_B23_STB_DBG_INFO_14_RK0_B2_STB_DBG_INFO_14     Fld(16, 0) //[15:0]
    #define RK0_B23_STB_DBG_INFO_14_RK0_B3_STB_DBG_INFO_14     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_B23_STB_DBG_INFO_15              (DRAMC_NAO_BASE_ADDRESS + 0x006FC)
    #define RK0_B23_STB_DBG_INFO_15_RK0_B2_STB_DBG_INFO_15     Fld(16, 0) //[15:0]
    #define RK0_B23_STB_DBG_INFO_15_RK0_B3_STB_DBG_INFO_15     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_DQSOSC_STATUS                    (DRAMC_NAO_BASE_ADDRESS + 0x00700)
    #define RK1_DQSOSC_STATUS_MR18_REG_RK1                     Fld(16, 0) //[15:0]
    #define RK1_DQSOSC_STATUS_MR19_REG_RK1                     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_DQSOSC_DELTA                     (DRAMC_NAO_BASE_ADDRESS + 0x00704)
    #define RK1_DQSOSC_DELTA_ABS_RK1_DQSOSC_DELTA              Fld(16, 0) //[15:0]
    #define RK1_DQSOSC_DELTA_SIGN_RK1_DQSOSC_DELTA             Fld(1, 16) //[16:16]
    #define RK1_DQSOSC_DELTA_DQSOSCR_RK1_RESPONSE              Fld(1, 17) //[17:17]
    #define RK1_DQSOSC_DELTA_H_DQSOSCLSBR_RK1_REQ              Fld(1, 18) //[18:18]
    #define RK1_DQSOSC_DELTA_DQSOSC_INT_RK1                    Fld(1, 19) //[19:19]

#define DRAMC_REG_RK1_DQSOSC_DELTA2                    (DRAMC_NAO_BASE_ADDRESS + 0x00708)
    #define RK1_DQSOSC_DELTA2_ABS_RK1_DQSOSC_B1_DELTA          Fld(16, 0) //[15:0]
    #define RK1_DQSOSC_DELTA2_SIGN_RK1_DQSOSC_B1_DELTA         Fld(1, 16) //[16:16]

#define DRAMC_REG_RK1_CURRENT_TX_SETTING1              (DRAMC_NAO_BASE_ADDRESS + 0x00710)
    #define RK1_CURRENT_TX_SETTING1_REG_TX_DLY_R1DQ0_MOD       Fld(3, 0) //[2:0]
    #define RK1_CURRENT_TX_SETTING1_REG_TX_DLY_R1DQ1_MOD       Fld(3, 4) //[6:4]
    #define RK1_CURRENT_TX_SETTING1_REG_TX_DLY_R1DQ2_MOD       Fld(3, 8) //[10:8]
    #define RK1_CURRENT_TX_SETTING1_REG_TX_DLY_R1DQ3_MOD       Fld(3, 12) //[14:12]
    #define RK1_CURRENT_TX_SETTING1_REG_TX_DLY_R1DQM0_MOD      Fld(3, 16) //[18:16]
    #define RK1_CURRENT_TX_SETTING1_REG_TX_DLY_R1DQM1_MOD      Fld(3, 20) //[22:20]
    #define RK1_CURRENT_TX_SETTING1_REG_TX_DLY_R1DQM2_MOD      Fld(3, 24) //[26:24]
    #define RK1_CURRENT_TX_SETTING1_REG_TX_DLY_R1DQM3_MOD      Fld(3, 28) //[30:28]

#define DRAMC_REG_RK1_CURRENT_TX_SETTING2              (DRAMC_NAO_BASE_ADDRESS + 0x00714)
    #define RK1_CURRENT_TX_SETTING2_REG_DLY_R1DQ0_MOD          Fld(3, 0) //[2:0]
    #define RK1_CURRENT_TX_SETTING2_REG_DLY_R1DQ1_MOD          Fld(3, 4) //[6:4]
    #define RK1_CURRENT_TX_SETTING2_REG_DLY_R1DQ2_MOD          Fld(3, 8) //[10:8]
    #define RK1_CURRENT_TX_SETTING2_REG_DLY_R1DQ3_MOD          Fld(3, 12) //[14:12]
    #define RK1_CURRENT_TX_SETTING2_REG_DLY_R1DQM0_MOD         Fld(3, 16) //[18:16]
    #define RK1_CURRENT_TX_SETTING2_REG_DLY_R1DQM1_MOD         Fld(3, 20) //[22:20]
    #define RK1_CURRENT_TX_SETTING2_REG_DLY_R1DQM2_MOD         Fld(3, 24) //[26:24]
    #define RK1_CURRENT_TX_SETTING2_REG_DLY_R1DQM3_MOD         Fld(3, 28) //[30:28]

#define DRAMC_REG_RK1_CURRENT_TX_SETTING3              (DRAMC_NAO_BASE_ADDRESS + 0x00718)
    #define RK1_CURRENT_TX_SETTING3_REG_TX_DLY_OEN_R1DQ0_MOD   Fld(3, 0) //[2:0]
    #define RK1_CURRENT_TX_SETTING3_REG_TX_DLY_OEN_R1DQ1_MOD   Fld(3, 4) //[6:4]
    #define RK1_CURRENT_TX_SETTING3_REG_TX_DLY_OEN_R1DQ2_MOD   Fld(3, 8) //[10:8]
    #define RK1_CURRENT_TX_SETTING3_REG_TX_DLY_OEN_R1DQ3_MOD   Fld(3, 12) //[14:12]
    #define RK1_CURRENT_TX_SETTING3_REG_TX_DLY_OEN_R1DQM0_MOD  Fld(3, 16) //[18:16]
    #define RK1_CURRENT_TX_SETTING3_REG_TX_DLY_OEN_R1DQM1_MOD  Fld(3, 20) //[22:20]
    #define RK1_CURRENT_TX_SETTING3_REG_TX_DLY_OEN_R1DQM2_MOD  Fld(3, 24) //[26:24]
    #define RK1_CURRENT_TX_SETTING3_REG_TX_DLY_OEN_R1DQM3_MOD  Fld(3, 28) //[30:28]

#define DRAMC_REG_RK1_CURRENT_TX_SETTING4              (DRAMC_NAO_BASE_ADDRESS + 0x0071C)
    #define RK1_CURRENT_TX_SETTING4_REG_DLY_OEN_R1DQ0_MOD      Fld(3, 0) //[2:0]
    #define RK1_CURRENT_TX_SETTING4_REG_DLY_OEN_R1DQ1_MOD      Fld(3, 4) //[6:4]
    #define RK1_CURRENT_TX_SETTING4_REG_DLY_OEN_R1DQ2_MOD      Fld(3, 8) //[10:8]
    #define RK1_CURRENT_TX_SETTING4_REG_DLY_OEN_R1DQ3_MOD      Fld(3, 12) //[14:12]
    #define RK1_CURRENT_TX_SETTING4_REG_DLY_OEN_R1DQM0_MOD     Fld(3, 16) //[18:16]
    #define RK1_CURRENT_TX_SETTING4_REG_DLY_OEN_R1DQM1_MOD     Fld(3, 20) //[22:20]
    #define RK1_CURRENT_TX_SETTING4_REG_DLY_OEN_R1DQM2_MOD     Fld(3, 24) //[26:24]
    #define RK1_CURRENT_TX_SETTING4_REG_DLY_OEN_R1DQM3_MOD     Fld(3, 28) //[30:28]

#define DRAMC_REG_RK1_DUMMY_RD_DATA0                   (DRAMC_NAO_BASE_ADDRESS + 0x00720)
    #define RK1_DUMMY_RD_DATA0_DUMMY_RD_RK1_DATA0              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_DUMMY_RD_DATA1                   (DRAMC_NAO_BASE_ADDRESS + 0x00724)
    #define RK1_DUMMY_RD_DATA1_DUMMY_RD_RK1_DATA1              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_DUMMY_RD_DATA2                   (DRAMC_NAO_BASE_ADDRESS + 0x00728)
    #define RK1_DUMMY_RD_DATA2_DUMMY_RD_RK1_DATA2              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_DUMMY_RD_DATA3                   (DRAMC_NAO_BASE_ADDRESS + 0x0072C)
    #define RK1_DUMMY_RD_DATA3_DUMMY_RD_RK1_DATA3              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_B0_STB_MAX_MIN_DLY               (DRAMC_NAO_BASE_ADDRESS + 0x00730)
    #define RK1_B0_STB_MAX_MIN_DLY_RK1_B0_STBEN_MIN_DLY        Fld(12, 0) //[11:0]
    #define RK1_B0_STB_MAX_MIN_DLY_RK1_B0_STBEN_MAX_DLY        Fld(12, 16) //[27:16]

#define DRAMC_REG_RK1_B1_STB_MAX_MIN_DLY               (DRAMC_NAO_BASE_ADDRESS + 0x00734)
    #define RK1_B1_STB_MAX_MIN_DLY_RK1_B1_STBEN_MIN_DLY        Fld(12, 0) //[11:0]
    #define RK1_B1_STB_MAX_MIN_DLY_RK1_B1_STBEN_MAX_DLY        Fld(12, 16) //[27:16]

#define DRAMC_REG_RK1_B2_STB_MAX_MIN_DLY               (DRAMC_NAO_BASE_ADDRESS + 0x00738)
    #define RK1_B2_STB_MAX_MIN_DLY_RK1_B2_STBEN_MIN_DLY        Fld(12, 0) //[11:0]
    #define RK1_B2_STB_MAX_MIN_DLY_RK1_B2_STBEN_MAX_DLY        Fld(12, 16) //[27:16]

#define DRAMC_REG_RK1_B3_STB_MAX_MIN_DLY               (DRAMC_NAO_BASE_ADDRESS + 0x0073C)
    #define RK1_B3_STB_MAX_MIN_DLY_RK1_B3_STBEN_MIN_DLY        Fld(12, 0) //[11:0]
    #define RK1_B3_STB_MAX_MIN_DLY_RK1_B3_STBEN_MAX_DLY        Fld(12, 16) //[27:16]

#define DRAMC_REG_RK1_DQSIENDLY                        (DRAMC_NAO_BASE_ADDRESS + 0x00740)
    #define RK1_DQSIENDLY_R1DQS0IENDLY                         Fld(7, 0) //[6:0]
    #define RK1_DQSIENDLY_R1DQS1IENDLY                         Fld(7, 8) //[14:8]
    #define RK1_DQSIENDLY_R1DQS2IENDLY                         Fld(7, 16) //[22:16]
    #define RK1_DQSIENDLY_R1DQS3IENDLY                         Fld(7, 24) //[30:24]

#define DRAMC_REG_RK1_DQSIENUIDLY                      (DRAMC_NAO_BASE_ADDRESS + 0x00744)
    #define RK1_DQSIENUIDLY_R1DQS0IENUIDLY                     Fld(6, 0) //[5:0]
    #define RK1_DQSIENUIDLY_R1DQS1IENUIDLY                     Fld(6, 8) //[13:8]
    #define RK1_DQSIENUIDLY_R1DQS2IENUIDLY                     Fld(6, 16) //[21:16]
    #define RK1_DQSIENUIDLY_R1DQS3IENUIDLY                     Fld(6, 24) //[29:24]

#define DRAMC_REG_RK1_DQSIENUIDLY_P1                   (DRAMC_NAO_BASE_ADDRESS + 0x00748)
    #define RK1_DQSIENUIDLY_P1_R1DQS0IENUIDLY_P1               Fld(6, 0) //[5:0]
    #define RK1_DQSIENUIDLY_P1_R1DQS1IENUIDLY_P1               Fld(6, 8) //[13:8]
    #define RK1_DQSIENUIDLY_P1_R1DQS2IENUIDLY_P1               Fld(6, 16) //[21:16]
    #define RK1_DQSIENUIDLY_P1_R1DQS3IENUIDLY_P1               Fld(6, 24) //[29:24]

#define DRAMC_REG_RK1_DQS_STBCALDEC_CNT1               (DRAMC_NAO_BASE_ADDRESS + 0x00750)
    #define RK1_DQS_STBCALDEC_CNT1_RK1_DQS0_STBCALDEC_CNT      Fld(16, 0) //[15:0]
    #define RK1_DQS_STBCALDEC_CNT1_RK1_DQS1_STBCALDEC_CNT      Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_DQS_STBCALDEC_CNT2               (DRAMC_NAO_BASE_ADDRESS + 0x00754)
    #define RK1_DQS_STBCALDEC_CNT2_RK1_DQS2_STBCALDEC_CNT      Fld(16, 0) //[15:0]
    #define RK1_DQS_STBCALDEC_CNT2_RK1_DQS3_STBCALDEC_CNT      Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_DQS_STBCALINC_CNT1               (DRAMC_NAO_BASE_ADDRESS + 0x00758)
    #define RK1_DQS_STBCALINC_CNT1_RK1_DQS0_STBCALINC_CNT      Fld(16, 0) //[15:0]
    #define RK1_DQS_STBCALINC_CNT1_RK1_DQS1_STBCALINC_CNT      Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_DQS_STBCALINC_CNT2               (DRAMC_NAO_BASE_ADDRESS + 0x0075C)
    #define RK1_DQS_STBCALINC_CNT2_RK1_DQS2_STBCALINC_CNT      Fld(16, 0) //[15:0]
    #define RK1_DQS_STBCALINC_CNT2_RK1_DQS3_STBCALINC_CNT      Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_PI_DQ_CAL                        (DRAMC_NAO_BASE_ADDRESS + 0x00760)
    #define RK1_PI_DQ_CAL_RK1_ARPI_DQ_B0_CAL                   Fld(6, 0) //[5:0]
    #define RK1_PI_DQ_CAL_RK1_ARPI_DQ_B1_CAL                   Fld(6, 8) //[13:8]
    #define RK1_PI_DQ_CAL_PI_DQ_ADJ_RK1                        Fld(6, 16) //[21:16]
    #define RK1_PI_DQ_CAL_PI_DQ_ADJ_RK1_FLOW                   Fld(1, 22) //[22:22]
    #define RK1_PI_DQ_CAL_RK1_B0_PI_CHANGE_DBG                 Fld(1, 23) //[23:23]
    #define RK1_PI_DQ_CAL_PI_DQ_ADJ_RK1_B1                     Fld(6, 24) //[29:24]
    #define RK1_PI_DQ_CAL_PI_DQ_ADJ_RK1_B1_FLOW                Fld(1, 30) //[30:30]
    #define RK1_PI_DQ_CAL_RK1_B1_PI_CHANGE_DBG                 Fld(1, 31) //[31:31]

#define DRAMC_REG_RK1_DQSG_RETRY_FLAG                  (DRAMC_NAO_BASE_ADDRESS + 0x00764)
    #define RK1_DQSG_RETRY_FLAG_RK1_RETRY_DONE0                Fld(1, 0) //[0:0]
    #define RK1_DQSG_RETRY_FLAG_RK1_RETRY_DONE1                Fld(1, 1) //[1:1]
    #define RK1_DQSG_RETRY_FLAG_RK1_RETRY_DONE2                Fld(1, 2) //[2:2]
    #define RK1_DQSG_RETRY_FLAG_RK1_RETRY_DONE3                Fld(1, 3) //[3:3]
    #define RK1_DQSG_RETRY_FLAG_RK1_RETRY_FAIL0                Fld(1, 16) //[16:16]
    #define RK1_DQSG_RETRY_FLAG_RK1_RETRY_FAIL1                Fld(1, 17) //[17:17]
    #define RK1_DQSG_RETRY_FLAG_RK1_RETRY_FAIL2                Fld(1, 18) //[18:18]
    #define RK1_DQSG_RETRY_FLAG_RK1_RETRY_FAIL3                Fld(1, 19) //[19:19]

#define DRAMC_REG_RK1_PI_DQM_CAL                       (DRAMC_NAO_BASE_ADDRESS + 0x00768)
    #define RK1_PI_DQM_CAL_RK1_ARPI_DQM_B0_CAL                 Fld(6, 0) //[5:0]
    #define RK1_PI_DQM_CAL_RK1_ARPI_DQM_B1_CAL                 Fld(6, 8) //[13:8]

#define DRAMC_REG_RK1_DQS0_STBCAL_CNT                  (DRAMC_NAO_BASE_ADDRESS + 0x00770)
    #define RK1_DQS0_STBCAL_CNT_R1_DQS0_STBCAL_CNT             Fld(17, 0) //[16:0]

#define DRAMC_REG_RK1_DQS1_STBCAL_CNT                  (DRAMC_NAO_BASE_ADDRESS + 0x00774)
    #define RK1_DQS1_STBCAL_CNT_R1_DQS1_STBCAL_CNT             Fld(17, 0) //[16:0]

#define DRAMC_REG_RK1_DQS2_STBCAL_CNT                  (DRAMC_NAO_BASE_ADDRESS + 0x00778)
    #define RK1_DQS2_STBCAL_CNT_R1_DQS2_STBCAL_CNT             Fld(17, 0) //[16:0]

#define DRAMC_REG_RK1_DQS3_STBCAL_CNT                  (DRAMC_NAO_BASE_ADDRESS + 0x0077C)
    #define RK1_DQS3_STBCAL_CNT_R1_DQS3_STBCAL_CNT             Fld(17, 0) //[16:0]

#define DRAMC_REG_RK1_B01_STB_DBG_INFO_00              (DRAMC_NAO_BASE_ADDRESS + 0x00780)
    #define RK1_B01_STB_DBG_INFO_00_RK1_B0_STB_DBG_INFO_00     Fld(16, 0) //[15:0]
    #define RK1_B01_STB_DBG_INFO_00_RK1_B1_STB_DBG_INFO_00     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B01_STB_DBG_INFO_01              (DRAMC_NAO_BASE_ADDRESS + 0x00784)
    #define RK1_B01_STB_DBG_INFO_01_RK1_B0_STB_DBG_INFO_01     Fld(16, 0) //[15:0]
    #define RK1_B01_STB_DBG_INFO_01_RK1_B1_STB_DBG_INFO_01     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B01_STB_DBG_INFO_02              (DRAMC_NAO_BASE_ADDRESS + 0x00788)
    #define RK1_B01_STB_DBG_INFO_02_RK1_B0_STB_DBG_INFO_02     Fld(16, 0) //[15:0]
    #define RK1_B01_STB_DBG_INFO_02_RK1_B1_STB_DBG_INFO_02     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B01_STB_DBG_INFO_03              (DRAMC_NAO_BASE_ADDRESS + 0x0078C)
    #define RK1_B01_STB_DBG_INFO_03_RK1_B0_STB_DBG_INFO_03     Fld(16, 0) //[15:0]
    #define RK1_B01_STB_DBG_INFO_03_RK1_B1_STB_DBG_INFO_03     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B01_STB_DBG_INFO_04              (DRAMC_NAO_BASE_ADDRESS + 0x00790)
    #define RK1_B01_STB_DBG_INFO_04_RK1_B0_STB_DBG_INFO_04     Fld(16, 0) //[15:0]
    #define RK1_B01_STB_DBG_INFO_04_RK1_B1_STB_DBG_INFO_04     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B01_STB_DBG_INFO_05              (DRAMC_NAO_BASE_ADDRESS + 0x00794)
    #define RK1_B01_STB_DBG_INFO_05_RK1_B0_STB_DBG_INFO_05     Fld(16, 0) //[15:0]
    #define RK1_B01_STB_DBG_INFO_05_RK1_B1_STB_DBG_INFO_05     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B01_STB_DBG_INFO_06              (DRAMC_NAO_BASE_ADDRESS + 0x00798)
    #define RK1_B01_STB_DBG_INFO_06_RK1_B0_STB_DBG_INFO_06     Fld(16, 0) //[15:0]
    #define RK1_B01_STB_DBG_INFO_06_RK1_B1_STB_DBG_INFO_06     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B01_STB_DBG_INFO_07              (DRAMC_NAO_BASE_ADDRESS + 0x0079C)
    #define RK1_B01_STB_DBG_INFO_07_RK1_B0_STB_DBG_INFO_07     Fld(16, 0) //[15:0]
    #define RK1_B01_STB_DBG_INFO_07_RK1_B1_STB_DBG_INFO_07     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B01_STB_DBG_INFO_08              (DRAMC_NAO_BASE_ADDRESS + 0x007A0)
    #define RK1_B01_STB_DBG_INFO_08_RK1_B0_STB_DBG_INFO_08     Fld(16, 0) //[15:0]
    #define RK1_B01_STB_DBG_INFO_08_RK1_B1_STB_DBG_INFO_08     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B01_STB_DBG_INFO_09              (DRAMC_NAO_BASE_ADDRESS + 0x007A4)
    #define RK1_B01_STB_DBG_INFO_09_RK1_B0_STB_DBG_INFO_09     Fld(16, 0) //[15:0]
    #define RK1_B01_STB_DBG_INFO_09_RK1_B1_STB_DBG_INFO_09     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B01_STB_DBG_INFO_10              (DRAMC_NAO_BASE_ADDRESS + 0x007A8)
    #define RK1_B01_STB_DBG_INFO_10_RK1_B0_STB_DBG_INFO_10     Fld(16, 0) //[15:0]
    #define RK1_B01_STB_DBG_INFO_10_RK1_B1_STB_DBG_INFO_10     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B01_STB_DBG_INFO_11              (DRAMC_NAO_BASE_ADDRESS + 0x007AC)
    #define RK1_B01_STB_DBG_INFO_11_RK1_B0_STB_DBG_INFO_11     Fld(16, 0) //[15:0]
    #define RK1_B01_STB_DBG_INFO_11_RK1_B1_STB_DBG_INFO_11     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B01_STB_DBG_INFO_12              (DRAMC_NAO_BASE_ADDRESS + 0x007B0)
    #define RK1_B01_STB_DBG_INFO_12_RK1_B0_STB_DBG_INFO_12     Fld(16, 0) //[15:0]
    #define RK1_B01_STB_DBG_INFO_12_RK1_B1_STB_DBG_INFO_12     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B01_STB_DBG_INFO_13              (DRAMC_NAO_BASE_ADDRESS + 0x007B4)
    #define RK1_B01_STB_DBG_INFO_13_RK1_B0_STB_DBG_INFO_13     Fld(16, 0) //[15:0]
    #define RK1_B01_STB_DBG_INFO_13_RK1_B1_STB_DBG_INFO_13     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B01_STB_DBG_INFO_14              (DRAMC_NAO_BASE_ADDRESS + 0x007B8)
    #define RK1_B01_STB_DBG_INFO_14_RK1_B0_STB_DBG_INFO_14     Fld(16, 0) //[15:0]
    #define RK1_B01_STB_DBG_INFO_14_RK1_B1_STB_DBG_INFO_14     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B01_STB_DBG_INFO_15              (DRAMC_NAO_BASE_ADDRESS + 0x007BC)
    #define RK1_B01_STB_DBG_INFO_15_RK1_B0_STB_DBG_INFO_15     Fld(16, 0) //[15:0]
    #define RK1_B01_STB_DBG_INFO_15_RK1_B1_STB_DBG_INFO_15     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B23_STB_DBG_INFO_00              (DRAMC_NAO_BASE_ADDRESS + 0x007C0)
    #define RK1_B23_STB_DBG_INFO_00_RK1_B2_STB_DBG_INFO_00     Fld(16, 0) //[15:0]
    #define RK1_B23_STB_DBG_INFO_00_RK1_B3_STB_DBG_INFO_00     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B23_STB_DBG_INFO_01              (DRAMC_NAO_BASE_ADDRESS + 0x007C4)
    #define RK1_B23_STB_DBG_INFO_01_RK1_B2_STB_DBG_INFO_01     Fld(16, 0) //[15:0]
    #define RK1_B23_STB_DBG_INFO_01_RK1_B3_STB_DBG_INFO_01     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B23_STB_DBG_INFO_02              (DRAMC_NAO_BASE_ADDRESS + 0x007C8)
    #define RK1_B23_STB_DBG_INFO_02_RK1_B2_STB_DBG_INFO_02     Fld(16, 0) //[15:0]
    #define RK1_B23_STB_DBG_INFO_02_RK1_B3_STB_DBG_INFO_02     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B23_STB_DBG_INFO_03              (DRAMC_NAO_BASE_ADDRESS + 0x007CC)
    #define RK1_B23_STB_DBG_INFO_03_RK1_B2_STB_DBG_INFO_03     Fld(16, 0) //[15:0]
    #define RK1_B23_STB_DBG_INFO_03_RK1_B3_STB_DBG_INFO_03     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B23_STB_DBG_INFO_04              (DRAMC_NAO_BASE_ADDRESS + 0x007D0)
    #define RK1_B23_STB_DBG_INFO_04_RK1_B2_STB_DBG_INFO_04     Fld(16, 0) //[15:0]
    #define RK1_B23_STB_DBG_INFO_04_RK1_B3_STB_DBG_INFO_04     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B23_STB_DBG_INFO_05              (DRAMC_NAO_BASE_ADDRESS + 0x007D4)
    #define RK1_B23_STB_DBG_INFO_05_RK1_B2_STB_DBG_INFO_05     Fld(16, 0) //[15:0]
    #define RK1_B23_STB_DBG_INFO_05_RK1_B3_STB_DBG_INFO_05     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B23_STB_DBG_INFO_06              (DRAMC_NAO_BASE_ADDRESS + 0x007D8)
    #define RK1_B23_STB_DBG_INFO_06_RK1_B2_STB_DBG_INFO_06     Fld(16, 0) //[15:0]
    #define RK1_B23_STB_DBG_INFO_06_RK1_B3_STB_DBG_INFO_06     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B23_STB_DBG_INFO_07              (DRAMC_NAO_BASE_ADDRESS + 0x007DC)
    #define RK1_B23_STB_DBG_INFO_07_RK1_B2_STB_DBG_INFO_07     Fld(16, 0) //[15:0]
    #define RK1_B23_STB_DBG_INFO_07_RK1_B3_STB_DBG_INFO_07     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B23_STB_DBG_INFO_08              (DRAMC_NAO_BASE_ADDRESS + 0x007E0)
    #define RK1_B23_STB_DBG_INFO_08_RK1_B2_STB_DBG_INFO_08     Fld(16, 0) //[15:0]
    #define RK1_B23_STB_DBG_INFO_08_RK1_B3_STB_DBG_INFO_08     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B23_STB_DBG_INFO_09              (DRAMC_NAO_BASE_ADDRESS + 0x007E4)
    #define RK1_B23_STB_DBG_INFO_09_RK1_B2_STB_DBG_INFO_09     Fld(16, 0) //[15:0]
    #define RK1_B23_STB_DBG_INFO_09_RK1_B3_STB_DBG_INFO_09     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B23_STB_DBG_INFO_10              (DRAMC_NAO_BASE_ADDRESS + 0x007E8)
    #define RK1_B23_STB_DBG_INFO_10_RK1_B2_STB_DBG_INFO_10     Fld(16, 0) //[15:0]
    #define RK1_B23_STB_DBG_INFO_10_RK1_B3_STB_DBG_INFO_10     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B23_STB_DBG_INFO_11              (DRAMC_NAO_BASE_ADDRESS + 0x007EC)
    #define RK1_B23_STB_DBG_INFO_11_RK1_B2_STB_DBG_INFO_11     Fld(16, 0) //[15:0]
    #define RK1_B23_STB_DBG_INFO_11_RK1_B3_STB_DBG_INFO_11     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B23_STB_DBG_INFO_12              (DRAMC_NAO_BASE_ADDRESS + 0x007F0)
    #define RK1_B23_STB_DBG_INFO_12_RK1_B2_STB_DBG_INFO_12     Fld(16, 0) //[15:0]
    #define RK1_B23_STB_DBG_INFO_12_RK1_B3_STB_DBG_INFO_12     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B23_STB_DBG_INFO_13              (DRAMC_NAO_BASE_ADDRESS + 0x007F4)
    #define RK1_B23_STB_DBG_INFO_13_RK1_B2_STB_DBG_INFO_13     Fld(16, 0) //[15:0]
    #define RK1_B23_STB_DBG_INFO_13_RK1_B3_STB_DBG_INFO_13     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B23_STB_DBG_INFO_14              (DRAMC_NAO_BASE_ADDRESS + 0x007F8)
    #define RK1_B23_STB_DBG_INFO_14_RK1_B2_STB_DBG_INFO_14     Fld(16, 0) //[15:0]
    #define RK1_B23_STB_DBG_INFO_14_RK1_B3_STB_DBG_INFO_14     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_B23_STB_DBG_INFO_15              (DRAMC_NAO_BASE_ADDRESS + 0x007FC)
    #define RK1_B23_STB_DBG_INFO_15_RK1_B2_STB_DBG_INFO_15     Fld(16, 0) //[15:0]
    #define RK1_B23_STB_DBG_INFO_15_RK1_B3_STB_DBG_INFO_15     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_DQSOSC_STATUS                    (DRAMC_NAO_BASE_ADDRESS + 0x00800)
    #define RK2_DQSOSC_STATUS_MR18_REG_RK2                     Fld(16, 0) //[15:0]
    #define RK2_DQSOSC_STATUS_MR19_REG_RK2                     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_DQSOSC_DELTA                     (DRAMC_NAO_BASE_ADDRESS + 0x00804)
    #define RK2_DQSOSC_DELTA_ABS_RK2_DQSOSC_DELTA              Fld(16, 0) //[15:0]
    #define RK2_DQSOSC_DELTA_SIGN_RK2_DQSOSC_DELTA             Fld(1, 16) //[16:16]
    #define RK2_DQSOSC_DELTA_DQSOSCR_RK2_RESPONSE              Fld(1, 17) //[17:17]
    #define RK2_DQSOSC_DELTA_H_DQSOSCLSBR_RK2_REQ              Fld(1, 18) //[18:18]
    #define RK2_DQSOSC_DELTA_DQSOSC_INT_RK2                    Fld(1, 19) //[19:19]

#define DRAMC_REG_RK2_DQSOSC_DELTA2                    (DRAMC_NAO_BASE_ADDRESS + 0x00808)
    #define RK2_DQSOSC_DELTA2_ABS_RK2_DQSOSC_B1_DELTA          Fld(16, 0) //[15:0]
    #define RK2_DQSOSC_DELTA2_SIGN_RK2_DQSOSC_B1_DELTA         Fld(1, 16) //[16:16]

#define DRAMC_REG_RK2_CURRENT_TX_SETTING1              (DRAMC_NAO_BASE_ADDRESS + 0x00810)
    #define RK2_CURRENT_TX_SETTING1_REG_TX_DLY_R2DQ0_MOD       Fld(3, 0) //[2:0]
    #define RK2_CURRENT_TX_SETTING1_REG_TX_DLY_R2DQ1_MOD       Fld(3, 4) //[6:4]
    #define RK2_CURRENT_TX_SETTING1_REG_TX_DLY_R2DQ2_MOD       Fld(3, 8) //[10:8]
    #define RK2_CURRENT_TX_SETTING1_REG_TX_DLY_R2DQ3_MOD       Fld(3, 12) //[14:12]
    #define RK2_CURRENT_TX_SETTING1_REG_TX_DLY_R2DQM0_MOD      Fld(3, 16) //[18:16]
    #define RK2_CURRENT_TX_SETTING1_REG_TX_DLY_R2DQM1_MOD      Fld(3, 20) //[22:20]
    #define RK2_CURRENT_TX_SETTING1_REG_TX_DLY_R2DQM2_MOD      Fld(3, 24) //[26:24]
    #define RK2_CURRENT_TX_SETTING1_REG_TX_DLY_R2DQM3_MOD      Fld(3, 28) //[30:28]

#define DRAMC_REG_RK2_CURRENT_TX_SETTING2              (DRAMC_NAO_BASE_ADDRESS + 0x00814)
    #define RK2_CURRENT_TX_SETTING2_REG_DLY_R2DQ0_MOD          Fld(3, 0) //[2:0]
    #define RK2_CURRENT_TX_SETTING2_REG_DLY_R2DQ1_MOD          Fld(3, 4) //[6:4]
    #define RK2_CURRENT_TX_SETTING2_REG_DLY_R2DQ2_MOD          Fld(3, 8) //[10:8]
    #define RK2_CURRENT_TX_SETTING2_REG_DLY_R2DQ3_MOD          Fld(3, 12) //[14:12]
    #define RK2_CURRENT_TX_SETTING2_REG_DLY_R2DQM0_MOD         Fld(3, 16) //[18:16]
    #define RK2_CURRENT_TX_SETTING2_REG_DLY_R2DQM1_MOD         Fld(3, 20) //[22:20]
    #define RK2_CURRENT_TX_SETTING2_REG_DLY_R2DQM2_MOD         Fld(3, 24) //[26:24]
    #define RK2_CURRENT_TX_SETTING2_REG_DLY_R2DQM3_MOD         Fld(3, 28) //[30:28]

#define DRAMC_REG_RK2_CURRENT_TX_SETTING3              (DRAMC_NAO_BASE_ADDRESS + 0x00818)
    #define RK2_CURRENT_TX_SETTING3_REG_TX_DLY_OEN_R2DQ0_MOD   Fld(3, 0) //[2:0]
    #define RK2_CURRENT_TX_SETTING3_REG_TX_DLY_OEN_R2DQ1_MOD   Fld(3, 4) //[6:4]
    #define RK2_CURRENT_TX_SETTING3_REG_TX_DLY_OEN_R2DQ2_MOD   Fld(3, 8) //[10:8]
    #define RK2_CURRENT_TX_SETTING3_REG_TX_DLY_OEN_R2DQ3_MOD   Fld(3, 12) //[14:12]
    #define RK2_CURRENT_TX_SETTING3_REG_TX_DLY_OEN_R2DQM0_MOD  Fld(3, 16) //[18:16]
    #define RK2_CURRENT_TX_SETTING3_REG_TX_DLY_OEN_R2DQM1_MOD  Fld(3, 20) //[22:20]
    #define RK2_CURRENT_TX_SETTING3_REG_TX_DLY_OEN_R2DQM2_MOD  Fld(3, 24) //[26:24]
    #define RK2_CURRENT_TX_SETTING3_REG_TX_DLY_OEN_R2DQM3_MOD  Fld(3, 28) //[30:28]

#define DRAMC_REG_RK2_CURRENT_TX_SETTING4              (DRAMC_NAO_BASE_ADDRESS + 0x0081C)
    #define RK2_CURRENT_TX_SETTING4_REG_DLY_OEN_R2DQ0_MOD      Fld(3, 0) //[2:0]
    #define RK2_CURRENT_TX_SETTING4_REG_DLY_OEN_R2DQ1_MOD      Fld(3, 4) //[6:4]
    #define RK2_CURRENT_TX_SETTING4_REG_DLY_OEN_R2DQ2_MOD      Fld(3, 8) //[10:8]
    #define RK2_CURRENT_TX_SETTING4_REG_DLY_OEN_R2DQ3_MOD      Fld(3, 12) //[14:12]
    #define RK2_CURRENT_TX_SETTING4_REG_DLY_OEN_R2DQM0_MOD     Fld(3, 16) //[18:16]
    #define RK2_CURRENT_TX_SETTING4_REG_DLY_OEN_R2DQM1_MOD     Fld(3, 20) //[22:20]
    #define RK2_CURRENT_TX_SETTING4_REG_DLY_OEN_R2DQM2_MOD     Fld(3, 24) //[26:24]
    #define RK2_CURRENT_TX_SETTING4_REG_DLY_OEN_R2DQM3_MOD     Fld(3, 28) //[30:28]

#define DRAMC_REG_RK2_DUMMY_RD_DATA0                   (DRAMC_NAO_BASE_ADDRESS + 0x00820)
    #define RK2_DUMMY_RD_DATA0_DUMMY_RD_RK2_DATA0              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK2_DUMMY_RD_DATA1                   (DRAMC_NAO_BASE_ADDRESS + 0x00824)
    #define RK2_DUMMY_RD_DATA1_DUMMY_RD_RK2_DATA1              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK2_DUMMY_RD_DATA2                   (DRAMC_NAO_BASE_ADDRESS + 0x00828)
    #define RK2_DUMMY_RD_DATA2_DUMMY_RD_RK2_DATA2              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK2_DUMMY_RD_DATA3                   (DRAMC_NAO_BASE_ADDRESS + 0x0082C)
    #define RK2_DUMMY_RD_DATA3_DUMMY_RD_RK2_DATA3              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK2_B0_STB_MAX_MIN_DLY               (DRAMC_NAO_BASE_ADDRESS + 0x00830)
    #define RK2_B0_STB_MAX_MIN_DLY_RK2_B0_STBEN_MIN_DLY        Fld(12, 0) //[11:0]
    #define RK2_B0_STB_MAX_MIN_DLY_RK2_B0_STBEN_MAX_DLY        Fld(12, 16) //[27:16]

#define DRAMC_REG_RK2_B1_STB_MAX_MIN_DLY               (DRAMC_NAO_BASE_ADDRESS + 0x00834)
    #define RK2_B1_STB_MAX_MIN_DLY_RK2_B1_STBEN_MIN_DLY        Fld(12, 0) //[11:0]
    #define RK2_B1_STB_MAX_MIN_DLY_RK2_B1_STBEN_MAX_DLY        Fld(12, 16) //[27:16]

#define DRAMC_REG_RK2_B2_STB_MAX_MIN_DLY               (DRAMC_NAO_BASE_ADDRESS + 0x00838)
    #define RK2_B2_STB_MAX_MIN_DLY_RK2_B2_STBEN_MIN_DLY        Fld(12, 0) //[11:0]
    #define RK2_B2_STB_MAX_MIN_DLY_RK2_B2_STBEN_MAX_DLY        Fld(12, 16) //[27:16]

#define DRAMC_REG_RK2_B3_STB_MAX_MIN_DLY               (DRAMC_NAO_BASE_ADDRESS + 0x0083C)
    #define RK2_B3_STB_MAX_MIN_DLY_RK2_B3_STBEN_MIN_DLY        Fld(12, 0) //[11:0]
    #define RK2_B3_STB_MAX_MIN_DLY_RK2_B3_STBEN_MAX_DLY        Fld(12, 16) //[27:16]

#define DRAMC_REG_RK2_DQSIENDLY                        (DRAMC_NAO_BASE_ADDRESS + 0x00840)
    #define RK2_DQSIENDLY_R2DQS0IENDLY                         Fld(7, 0) //[6:0]
    #define RK2_DQSIENDLY_R2DQS1IENDLY                         Fld(7, 8) //[14:8]
    #define RK2_DQSIENDLY_R2DQS2IENDLY                         Fld(7, 16) //[22:16]
    #define RK2_DQSIENDLY_R2DQS3IENDLY                         Fld(7, 24) //[30:24]

#define DRAMC_REG_RK2_DQSIENUIDLY                      (DRAMC_NAO_BASE_ADDRESS + 0x00844)
    #define RK2_DQSIENUIDLY_R2DQS0IENUIDLY                     Fld(6, 0) //[5:0]
    #define RK2_DQSIENUIDLY_R2DQS1IENUIDLY                     Fld(6, 8) //[13:8]
    #define RK2_DQSIENUIDLY_R2DQS2IENUIDLY                     Fld(6, 16) //[21:16]
    #define RK2_DQSIENUIDLY_R2DQS3IENUIDLY                     Fld(6, 24) //[29:24]

#define DRAMC_REG_RK2_DQSIENUIDLY_P1                   (DRAMC_NAO_BASE_ADDRESS + 0x00848)
    #define RK2_DQSIENUIDLY_P1_R2DQS0IENUIDLY_P1               Fld(6, 0) //[5:0]
    #define RK2_DQSIENUIDLY_P1_R2DQS1IENUIDLY_P1               Fld(6, 8) //[13:8]
    #define RK2_DQSIENUIDLY_P1_R2DQS2IENUIDLY_P1               Fld(6, 16) //[21:16]
    #define RK2_DQSIENUIDLY_P1_R2DQS3IENUIDLY_P1               Fld(6, 24) //[29:24]

#define DRAMC_REG_RK2_DQS_STBCALDEC_CNT1               (DRAMC_NAO_BASE_ADDRESS + 0x00850)
    #define RK2_DQS_STBCALDEC_CNT1_RK2_DQS0_STBCALDEC_CNT      Fld(16, 0) //[15:0]
    #define RK2_DQS_STBCALDEC_CNT1_RK2_DQS1_STBCALDEC_CNT      Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_DQS_STBCALDEC_CNT2               (DRAMC_NAO_BASE_ADDRESS + 0x00854)
    #define RK2_DQS_STBCALDEC_CNT2_RK2_DQS2_STBCALDEC_CNT      Fld(16, 0) //[15:0]
    #define RK2_DQS_STBCALDEC_CNT2_RK2_DQS3_STBCALDEC_CNT      Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_DQS_STBCALINC_CNT1               (DRAMC_NAO_BASE_ADDRESS + 0x00858)
    #define RK2_DQS_STBCALINC_CNT1_RK2_DQS0_STBCALINC_CNT      Fld(16, 0) //[15:0]
    #define RK2_DQS_STBCALINC_CNT1_RK2_DQS1_STBCALINC_CNT      Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_DQS_STBCALINC_CNT2               (DRAMC_NAO_BASE_ADDRESS + 0x0085C)
    #define RK2_DQS_STBCALINC_CNT2_RK2_DQS2_STBCALINC_CNT      Fld(16, 0) //[15:0]
    #define RK2_DQS_STBCALINC_CNT2_RK2_DQS3_STBCALINC_CNT      Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_PI_DQ_CAL                        (DRAMC_NAO_BASE_ADDRESS + 0x00860)
    #define RK2_PI_DQ_CAL_RK2_ARPI_DQ_B0_CAL                   Fld(6, 0) //[5:0]
    #define RK2_PI_DQ_CAL_RK2_ARPI_DQ_B1_CAL                   Fld(6, 8) //[13:8]
    #define RK2_PI_DQ_CAL_PI_DQ_ADJ_RK2                        Fld(6, 16) //[21:16]
    #define RK2_PI_DQ_CAL_PI_DQ_ADJ_RK2_OVERFLOW               Fld(1, 22) //[22:22]
    #define RK2_PI_DQ_CAL_PI_DQ_ADJ_RK2_B1                     Fld(6, 24) //[29:24]
    #define RK2_PI_DQ_CAL_PI_DQ_ADJ_RK2_B1_OVERFLOW            Fld(1, 30) //[30:30]

#define DRAMC_REG_RK2_DQSG_RETRY_FLAG                  (DRAMC_NAO_BASE_ADDRESS + 0x00864)
    #define RK2_DQSG_RETRY_FLAG_RK2_RETRY_DONE0                Fld(1, 0) //[0:0]
    #define RK2_DQSG_RETRY_FLAG_RK2_RETRY_DONE1                Fld(1, 1) //[1:1]
    #define RK2_DQSG_RETRY_FLAG_RK2_RETRY_DONE2                Fld(1, 2) //[2:2]
    #define RK2_DQSG_RETRY_FLAG_RK2_RETRY_DONE3                Fld(1, 3) //[3:3]
    #define RK2_DQSG_RETRY_FLAG_RK2_RETRY_FAIL0                Fld(1, 16) //[16:16]
    #define RK2_DQSG_RETRY_FLAG_RK2_RETRY_FAIL1                Fld(1, 17) //[17:17]
    #define RK2_DQSG_RETRY_FLAG_RK2_RETRY_FAIL2                Fld(1, 18) //[18:18]
    #define RK2_DQSG_RETRY_FLAG_RK2_RETRY_FAIL3                Fld(1, 19) //[19:19]

#define DRAMC_REG_RK2_PI_DQM_CAL                       (DRAMC_NAO_BASE_ADDRESS + 0x00868)
    #define RK2_PI_DQM_CAL_RK2_ARPI_DQM_B0_CAL                 Fld(6, 0) //[5:0]
    #define RK2_PI_DQM_CAL_RK2_ARPI_DQM_B1_CAL                 Fld(6, 8) //[13:8]

#define DRAMC_REG_RK2_DQS0_STBCAL_CNT                  (DRAMC_NAO_BASE_ADDRESS + 0x00870)
    #define RK2_DQS0_STBCAL_CNT_R2_DQS0_STBCAL_CNT             Fld(17, 0) //[16:0]

#define DRAMC_REG_RK2_DQS1_STBCAL_CNT                  (DRAMC_NAO_BASE_ADDRESS + 0x00874)
    #define RK2_DQS1_STBCAL_CNT_R2_DQS1_STBCAL_CNT             Fld(17, 0) //[16:0]

#define DRAMC_REG_RK2_DQS2_STBCAL_CNT                  (DRAMC_NAO_BASE_ADDRESS + 0x00878)
    #define RK2_DQS2_STBCAL_CNT_R2_DQS2_STBCAL_CNT             Fld(17, 0) //[16:0]

#define DRAMC_REG_RK2_DQS3_STBCAL_CNT                  (DRAMC_NAO_BASE_ADDRESS + 0x0087C)
    #define RK2_DQS3_STBCAL_CNT_R2_DQS3_STBCAL_CNT             Fld(17, 0) //[16:0]

#define DRAMC_REG_RK2_B01_STB_DBG_INFO_00              (DRAMC_NAO_BASE_ADDRESS + 0x00880)
    #define RK2_B01_STB_DBG_INFO_00_RK2_B0_STB_DBG_INFO_00     Fld(16, 0) //[15:0]
    #define RK2_B01_STB_DBG_INFO_00_RK2_B1_STB_DBG_INFO_00     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B01_STB_DBG_INFO_01              (DRAMC_NAO_BASE_ADDRESS + 0x00884)
    #define RK2_B01_STB_DBG_INFO_01_RK2_B0_STB_DBG_INFO_01     Fld(16, 0) //[15:0]
    #define RK2_B01_STB_DBG_INFO_01_RK2_B1_STB_DBG_INFO_01     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B01_STB_DBG_INFO_02              (DRAMC_NAO_BASE_ADDRESS + 0x00888)
    #define RK2_B01_STB_DBG_INFO_02_RK2_B0_STB_DBG_INFO_02     Fld(16, 0) //[15:0]
    #define RK2_B01_STB_DBG_INFO_02_RK2_B1_STB_DBG_INFO_02     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B01_STB_DBG_INFO_03              (DRAMC_NAO_BASE_ADDRESS + 0x0088C)
    #define RK2_B01_STB_DBG_INFO_03_RK2_B0_STB_DBG_INFO_03     Fld(16, 0) //[15:0]
    #define RK2_B01_STB_DBG_INFO_03_RK2_B1_STB_DBG_INFO_03     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B01_STB_DBG_INFO_04              (DRAMC_NAO_BASE_ADDRESS + 0x00890)
    #define RK2_B01_STB_DBG_INFO_04_RK2_B0_STB_DBG_INFO_04     Fld(16, 0) //[15:0]
    #define RK2_B01_STB_DBG_INFO_04_RK2_B1_STB_DBG_INFO_04     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B01_STB_DBG_INFO_05              (DRAMC_NAO_BASE_ADDRESS + 0x00894)
    #define RK2_B01_STB_DBG_INFO_05_RK2_B0_STB_DBG_INFO_05     Fld(16, 0) //[15:0]
    #define RK2_B01_STB_DBG_INFO_05_RK2_B1_STB_DBG_INFO_05     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B01_STB_DBG_INFO_06              (DRAMC_NAO_BASE_ADDRESS + 0x00898)
    #define RK2_B01_STB_DBG_INFO_06_RK2_B0_STB_DBG_INFO_06     Fld(16, 0) //[15:0]
    #define RK2_B01_STB_DBG_INFO_06_RK2_B1_STB_DBG_INFO_06     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B01_STB_DBG_INFO_07              (DRAMC_NAO_BASE_ADDRESS + 0x0089C)
    #define RK2_B01_STB_DBG_INFO_07_RK2_B0_STB_DBG_INFO_07     Fld(16, 0) //[15:0]
    #define RK2_B01_STB_DBG_INFO_07_RK2_B1_STB_DBG_INFO_07     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B01_STB_DBG_INFO_08              (DRAMC_NAO_BASE_ADDRESS + 0x008A0)
    #define RK2_B01_STB_DBG_INFO_08_RK2_B0_STB_DBG_INFO_08     Fld(16, 0) //[15:0]
    #define RK2_B01_STB_DBG_INFO_08_RK2_B1_STB_DBG_INFO_08     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B01_STB_DBG_INFO_09              (DRAMC_NAO_BASE_ADDRESS + 0x008A4)
    #define RK2_B01_STB_DBG_INFO_09_RK2_B0_STB_DBG_INFO_09     Fld(16, 0) //[15:0]
    #define RK2_B01_STB_DBG_INFO_09_RK2_B1_STB_DBG_INFO_09     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B01_STB_DBG_INFO_10              (DRAMC_NAO_BASE_ADDRESS + 0x008A8)
    #define RK2_B01_STB_DBG_INFO_10_RK2_B0_STB_DBG_INFO_10     Fld(16, 0) //[15:0]
    #define RK2_B01_STB_DBG_INFO_10_RK2_B1_STB_DBG_INFO_10     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B01_STB_DBG_INFO_11              (DRAMC_NAO_BASE_ADDRESS + 0x008AC)
    #define RK2_B01_STB_DBG_INFO_11_RK2_B0_STB_DBG_INFO_11     Fld(16, 0) //[15:0]
    #define RK2_B01_STB_DBG_INFO_11_RK2_B1_STB_DBG_INFO_11     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B01_STB_DBG_INFO_12              (DRAMC_NAO_BASE_ADDRESS + 0x008B0)
    #define RK2_B01_STB_DBG_INFO_12_RK2_B0_STB_DBG_INFO_12     Fld(16, 0) //[15:0]
    #define RK2_B01_STB_DBG_INFO_12_RK2_B1_STB_DBG_INFO_12     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B01_STB_DBG_INFO_13              (DRAMC_NAO_BASE_ADDRESS + 0x008B4)
    #define RK2_B01_STB_DBG_INFO_13_RK2_B0_STB_DBG_INFO_13     Fld(16, 0) //[15:0]
    #define RK2_B01_STB_DBG_INFO_13_RK2_B1_STB_DBG_INFO_13     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B01_STB_DBG_INFO_14              (DRAMC_NAO_BASE_ADDRESS + 0x008B8)
    #define RK2_B01_STB_DBG_INFO_14_RK2_B0_STB_DBG_INFO_14     Fld(16, 0) //[15:0]
    #define RK2_B01_STB_DBG_INFO_14_RK2_B1_STB_DBG_INFO_14     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B01_STB_DBG_INFO_15              (DRAMC_NAO_BASE_ADDRESS + 0x008BC)
    #define RK2_B01_STB_DBG_INFO_15_RK2_B0_STB_DBG_INFO_15     Fld(16, 0) //[15:0]
    #define RK2_B01_STB_DBG_INFO_15_RK2_B1_STB_DBG_INFO_15     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B23_STB_DBG_INFO_00              (DRAMC_NAO_BASE_ADDRESS + 0x008C0)
    #define RK2_B23_STB_DBG_INFO_00_RK2_B2_STB_DBG_INFO_00     Fld(16, 0) //[15:0]
    #define RK2_B23_STB_DBG_INFO_00_RK2_B3_STB_DBG_INFO_00     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B23_STB_DBG_INFO_01              (DRAMC_NAO_BASE_ADDRESS + 0x008C4)
    #define RK2_B23_STB_DBG_INFO_01_RK2_B2_STB_DBG_INFO_01     Fld(16, 0) //[15:0]
    #define RK2_B23_STB_DBG_INFO_01_RK2_B3_STB_DBG_INFO_01     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B23_STB_DBG_INFO_02              (DRAMC_NAO_BASE_ADDRESS + 0x008C8)
    #define RK2_B23_STB_DBG_INFO_02_RK2_B2_STB_DBG_INFO_02     Fld(16, 0) //[15:0]
    #define RK2_B23_STB_DBG_INFO_02_RK2_B3_STB_DBG_INFO_02     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B23_STB_DBG_INFO_03              (DRAMC_NAO_BASE_ADDRESS + 0x008CC)
    #define RK2_B23_STB_DBG_INFO_03_RK2_B2_STB_DBG_INFO_03     Fld(16, 0) //[15:0]
    #define RK2_B23_STB_DBG_INFO_03_RK2_B3_STB_DBG_INFO_03     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B23_STB_DBG_INFO_04              (DRAMC_NAO_BASE_ADDRESS + 0x008D0)
    #define RK2_B23_STB_DBG_INFO_04_RK2_B2_STB_DBG_INFO_04     Fld(16, 0) //[15:0]
    #define RK2_B23_STB_DBG_INFO_04_RK2_B3_STB_DBG_INFO_04     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B23_STB_DBG_INFO_05              (DRAMC_NAO_BASE_ADDRESS + 0x008D4)
    #define RK2_B23_STB_DBG_INFO_05_RK2_B2_STB_DBG_INFO_05     Fld(16, 0) //[15:0]
    #define RK2_B23_STB_DBG_INFO_05_RK2_B3_STB_DBG_INFO_05     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B23_STB_DBG_INFO_06              (DRAMC_NAO_BASE_ADDRESS + 0x008D8)
    #define RK2_B23_STB_DBG_INFO_06_RK2_B2_STB_DBG_INFO_06     Fld(16, 0) //[15:0]
    #define RK2_B23_STB_DBG_INFO_06_RK2_B3_STB_DBG_INFO_06     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B23_STB_DBG_INFO_07              (DRAMC_NAO_BASE_ADDRESS + 0x008DC)
    #define RK2_B23_STB_DBG_INFO_07_RK2_B2_STB_DBG_INFO_07     Fld(16, 0) //[15:0]
    #define RK2_B23_STB_DBG_INFO_07_RK2_B3_STB_DBG_INFO_07     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B23_STB_DBG_INFO_08              (DRAMC_NAO_BASE_ADDRESS + 0x008E0)
    #define RK2_B23_STB_DBG_INFO_08_RK2_B2_STB_DBG_INFO_08     Fld(16, 0) //[15:0]
    #define RK2_B23_STB_DBG_INFO_08_RK2_B3_STB_DBG_INFO_08     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B23_STB_DBG_INFO_09              (DRAMC_NAO_BASE_ADDRESS + 0x008E4)
    #define RK2_B23_STB_DBG_INFO_09_RK2_B2_STB_DBG_INFO_09     Fld(16, 0) //[15:0]
    #define RK2_B23_STB_DBG_INFO_09_RK2_B3_STB_DBG_INFO_09     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B23_STB_DBG_INFO_10              (DRAMC_NAO_BASE_ADDRESS + 0x008E8)
    #define RK2_B23_STB_DBG_INFO_10_RK2_B2_STB_DBG_INFO_10     Fld(16, 0) //[15:0]
    #define RK2_B23_STB_DBG_INFO_10_RK2_B3_STB_DBG_INFO_10     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B23_STB_DBG_INFO_11              (DRAMC_NAO_BASE_ADDRESS + 0x008EC)
    #define RK2_B23_STB_DBG_INFO_11_RK2_B2_STB_DBG_INFO_11     Fld(16, 0) //[15:0]
    #define RK2_B23_STB_DBG_INFO_11_RK2_B3_STB_DBG_INFO_11     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B23_STB_DBG_INFO_12              (DRAMC_NAO_BASE_ADDRESS + 0x008F0)
    #define RK2_B23_STB_DBG_INFO_12_RK2_B2_STB_DBG_INFO_12     Fld(16, 0) //[15:0]
    #define RK2_B23_STB_DBG_INFO_12_RK2_B3_STB_DBG_INFO_12     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B23_STB_DBG_INFO_13              (DRAMC_NAO_BASE_ADDRESS + 0x008F4)
    #define RK2_B23_STB_DBG_INFO_13_RK2_B2_STB_DBG_INFO_13     Fld(16, 0) //[15:0]
    #define RK2_B23_STB_DBG_INFO_13_RK2_B3_STB_DBG_INFO_13     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B23_STB_DBG_INFO_14              (DRAMC_NAO_BASE_ADDRESS + 0x008F8)
    #define RK2_B23_STB_DBG_INFO_14_RK2_B2_STB_DBG_INFO_14     Fld(16, 0) //[15:0]
    #define RK2_B23_STB_DBG_INFO_14_RK2_B3_STB_DBG_INFO_14     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK2_B23_STB_DBG_INFO_15              (DRAMC_NAO_BASE_ADDRESS + 0x008FC)
    #define RK2_B23_STB_DBG_INFO_15_RK2_B2_STB_DBG_INFO_15     Fld(16, 0) //[15:0]
    #define RK2_B23_STB_DBG_INFO_15_RK2_B3_STB_DBG_INFO_15     Fld(16, 16) //[31:16]

#define DRAMC_REG_DVFS_DBG0                            (DRAMC_NAO_BASE_ADDRESS + 0x00C00)
    #define DVFS_DBG0_CUT_PHY_ST_SHU_MASK                      Fld(19, 0) //[18:0]

#define DRAMC_REG_DVFS_DBG1                            (DRAMC_NAO_BASE_ADDRESS + 0x00C04)
    #define DVFS_DBG1_PLL_SEL_MASK                             Fld(1, 0) //[0:0]
    #define DVFS_DBG1_MPDIV_SHU_GP_MASK                        Fld(3, 4) //[6:4]
    #define DVFS_DBG1_PICG_SHUFFLE_MASK                        Fld(1, 8) //[8:8]
    #define DVFS_DBG1_SHUFFLE_PHY_STATE_START_MASK             Fld(1, 9) //[9:9]
    #define DVFS_DBG1_SHUFFLE_PHY_STATE_DONE_MASK              Fld(1, 10) //[10:10]
    #define DVFS_DBG1_SHUFFLE_PERIOD_MASK                      Fld(1, 11) //[11:11]

/*----------------------------------------------
 * Page DRAMC_AO
 *----------------------------------------------
 */
#define DRAMC_AO_BASE_ADDRESS Channel_A_DRAMC_AO_BASE_VIRTUAL

#define DRAMC_REG_DDRCONF0                                 (DRAMC_AO_BASE_ADDRESS + 0x00000)
    #define DDRCONF0_RDATRST                                   Fld(1, 0) //[0:0]
    #define DDRCONF0_DMSW_RST                                  Fld(1, 1) //[1:1]
    #define DDRCONF0_WDT_DBG_RST                               Fld(1, 2) //[2:2]
    #define DDRCONF0_FIFOLEN                                   Fld(2, 4) //[5:4]
    #define DDRCONF0_DRAMEN                                    Fld(1, 7) //[7:7]
    #define DDRCONF0_RDQSIEN                                   Fld(1, 8) //[8:8]
    #define DDRCONF0_DQSGCGM                                   Fld(1, 9) //[9:9]
    #define DDRCONF0_APBL2                                     Fld(1, 10) //[10:10]
    #define DDRCONF0_BG4EN                                     Fld(1, 11) //[11:11]
    #define DDRCONF0_BK8EN                                     Fld(1, 12) //[12:12]
    #define DDRCONF0_BC4OTF_OPT                                Fld(1, 13) //[13:13]
    #define DDRCONF0_AG0MWR                                    Fld(1, 14) //[14:14]
    #define DDRCONF0_BC4OTF                                    Fld(1, 15) //[15:15]
    #define DDRCONF0_GDDR3RST                                  Fld(1, 16) //[16:16]
    #define DDRCONF0_DM16BPHSEL                                Fld(1, 17) //[17:17]
    #define DDRCONF0_DM16BITSWAP                               Fld(1, 18) //[18:18]
    #define DDRCONF0_DQ4BMUX                                   Fld(1, 19) //[19:19]
    #define DDRCONF0_DM64BITEN                                 Fld(1, 20) //[20:20]
    #define DDRCONF0_DM16BITFULL                               Fld(1, 21) //[21:21]
    #define DDRCONF0_DM4TO1MODE                                Fld(1, 22) //[22:22]
    #define DDRCONF0_GDDR3EN                                   Fld(1, 23) //[23:23]
    #define DDRCONF0_LPDDR2EN                                  Fld(1, 24) //[24:24]
    #define DDRCONF0_LPDDR3EN                                  Fld(1, 25) //[25:25]
    #define DDRCONF0_LPDDR4EN                                  Fld(1, 26) //[26:26]
    #define DDRCONF0_LPDDR2_NO_INT                             Fld(1, 27) //[27:27]
    #define DDRCONF0_DDR2EN                                    Fld(1, 28) //[28:28]
    #define DDRCONF0_DDR3EN                                    Fld(1, 29) //[29:29]
    #define DDRCONF0_DDR4EN                                    Fld(1, 30) //[30:30]
    #define DDRCONF0_DRAMC_SW_RST                              Fld(1, 31) //[31:31]

#define DRAMC_REG_DRAMCTRL                                 (DRAMC_AO_BASE_ADDRESS + 0x00004)
    #define DRAMCTRL_CTOREQ_HPRI_OPT                           Fld(1, 0) //[0:0]
    #define DRAMCTRL_ADRDECEN_TARKMODE                         Fld(1, 1) //[1:1]
    #define DRAMCTRL_ADRDECEN                                  Fld(1, 2) //[2:2]
    #define DRAMCTRL_ADRBIT3DEC                                Fld(1, 3) //[3:3]
    #define DRAMCTRL_TMRR2WDIS                                 Fld(1, 4) //[4:4]
    #define DRAMCTRL_WDATRGO                                   Fld(1, 8) //[8:8]
    #define DRAMCTRL_RANK_ASYM                                 Fld(1, 7) //[7:7]
    #define DRAMCTRL_CLKWITRFC                                 Fld(1, 9) //[9:9]
    #define DRAMCTRL_CHKFORPRE                                 Fld(1, 10) //[10:10]
    #define DRAMCTRL_ASYNCEN                                   Fld(1, 12) //[12:12]
    #define DRAMCTRL_DYNMWREN                                  Fld(1, 13) //[13:13]
    #define DRAMCTRL_ALEBLOCK                                  Fld(1, 14) //[14:14]
    #define DRAMCTRL_TMRRICHKDIS                               Fld(1, 15) //[15:15]
    #define DRAMCTRL_DMRCDRSV                                  Fld(1, 16) //[16:16]
    #define DRAMCTRL_TMRRIBYRK_DIS                             Fld(1, 17) //[17:17]
    #define DRAMCTRL_ZQCALL                                    Fld(1, 18) //[18:18]
    #define DRAMCTRL_PREALL_OPTION                             Fld(1, 19) //[19:19]
    #define DRAMCTRL_TCMD                                      Fld(3, 20) //[22:20]
    #define DRAMCTRL_MRRIOPT                                   Fld(1, 23) //[23:23]
    #define DRAMCTRL_FW2R                                      Fld(1, 24) //[24:24]
    #define DRAMCTRL_REQQUE_DEPTH_UPD                          Fld(1, 25) //[25:25]
    #define DRAMCTRL_REQQUE_THD_EN                             Fld(1, 26) //[26:26]
    #define DRAMCTRL_REQQUE_MAXCNT_CHG                         Fld(1, 27) //[27:27]
    #define DRAMCTRL_PREA_RK                                   Fld(2, 28) //[29:28]
    #define DRAMCTRL_IDLE_COND_OPT                             Fld(1, 30) //[30:30]

#define DRAMC_REG_MISCTL0                                  (DRAMC_AO_BASE_ADDRESS + 0x00008)
    #define MISCTL0_R_DMCA_IDLE_EN                             Fld(1, 0) //[0:0]
    #define MISCTL0_IDLE_CNT_OPT                               Fld(1, 16) //[16:16]
    #define MISCTL0_PAGDIS                                     Fld(1, 17) //[17:17]
    #define MISCTL0_IDLEDCM_CNT_OPT                            Fld(1, 18) //[18:18]
    #define MISCTL0_REFA_ARB_EN2                               Fld(1, 19) //[19:19]
    #define MISCTL0_WRBYTE_CNT_OPT                             Fld(1, 20) //[20:20]
    #define MISCTL0_REFA_ARB_EN_OPTION                         Fld(1, 21) //[21:21]
    #define MISCTL0_REORDER_MASK_E1T                           Fld(1, 22) //[22:22]
    #define MISCTL0_PBC_ARB_E1T                                Fld(1, 23) //[23:23]
    #define MISCTL0_PBC_ARB_EN                                 Fld(1, 24) //[24:24]
    #define MISCTL0_REFA_ARB_EN                                Fld(1, 25) //[25:25]
    #define MISCTL0_REFP_ARB_EN                                Fld(1, 26) //[26:26]
    #define MISCTL0_EMIPREEN                                   Fld(1, 27) //[27:27]
    #define MISCTL0_REFP_ARB_EN2                               Fld(1, 31) //[31:31]

#define DRAMC_REG_PERFCTL0                                 (DRAMC_AO_BASE_ADDRESS + 0x0000C)
    #define PERFCTL0_DUALSCHEN                                 Fld(1, 0) //[0:0]
    #define PERFCTL0_DISRDPHASE1                               Fld(1, 1) //[1:1]
    #define PERFCTL0_XRT_05T_OPT                               Fld(1, 2) //[2:2]
    #define PERFCTL0_AIDCHKEN                                  Fld(1, 3) //[3:3]
    #define PERFCTL0_RWOFOEN                                   Fld(1, 4) //[4:4]
    #define PERFCTL0_RWOFOWNUM                                 Fld(3, 5) //[7:5]
    #define PERFCTL0_RWHPRIEN                                  Fld(1, 8) //[8:8]
    #define PERFCTL0_RWLLATEN                                  Fld(1, 9) //[9:9]
    #define PERFCTL0_RWAGEEN                                   Fld(1, 10) //[10:10]
    #define PERFCTL0_EMILLATEN                                 Fld(1, 11) //[11:11]
    #define PERFCTL0_LASTCMDOPT                                Fld(1, 12) //[12:12]
    #define PERFCTL0_RWHPRICTL                                 Fld(1, 13) //[13:13]
    #define PERFCTL0_WFLUSHEN                                  Fld(1, 14) //[14:14]
    #define PERFCTL0_RWSPLIT                                   Fld(1, 15) //[15:15]
    #define PERFCTL0_MWHPRIEN                                  Fld(1, 17) //[17:17]
    #define PERFCTL0_REORDER_MODE                              Fld(1, 18) //[18:18]
    #define PERFCTL0_REORDEREN                                 Fld(1, 19) //[19:19]
    #define PERFCTL0_SBR_MASK_OPT                              Fld(1, 20) //[20:20]
    #define PERFCTL0_SBR_MASK_OPT2                             Fld(1, 21) //[21:21]
    #define PERFCTL0_MAFIXHIGH                                 Fld(1, 22) //[22:22]
    #define PERFCTL0_TESTWRHIGH                                Fld(1, 23) //[23:23]
    #define PERFCTL0_RECORDER_MASK_OPT                         Fld(1, 24) //[24:24]
    #define PERFCTL0_MDMCU_MASK_EN                             Fld(1, 25) //[25:25]
    #define PERFCTL0_WRFIFO_OPT                                Fld(1, 26) //[26:26]
    #define PERFCTL0_WRFIO_MODE2                               Fld(1, 27) //[27:27]
    #define PERFCTL0_RDFIFOEN                                  Fld(1, 30) //[30:30]
    #define PERFCTL0_WRFIFOEN                                  Fld(1, 31) //[31:31]

#define DRAMC_REG_ARBCTL                                   (DRAMC_AO_BASE_ADDRESS + 0x00010)
    #define ARBCTL_MAXPENDCNT                                  Fld(8, 0) //[7:0]
    #define ARBCTL_RDATACNTDIS                                 Fld(1, 8) //[8:8]
    #define ARBCTL_WDATACNTDIS                                 Fld(1, 9) //[9:9]
    #define ARBCTL_RSV_SA0                                     Fld(1, 10) //[10:10]
    #define ARBCTL_RSV_SA1                                     Fld(1, 11) //[11:11]
    #define ARBCTL_RSV_SA2                                     Fld(1, 12) //[12:12]
    #define ARBCTL_RSV_SA3                                     Fld(1, 13) //[13:13]
    #define ARBCTL_DBIWR_IMP_EN                                Fld(1, 14) //[14:14]
    #define ARBCTL_DBIWR_PINMUX_EN                             Fld(1, 15) //[15:15]
    #define ARBCTL_DBIWR_OPT_B0                                Fld(8, 16) //[23:16]
    #define ARBCTL_DBIWR_OPT_B1                                Fld(8, 24) //[31:24]
    #define ARBCTL_RSV_DRAM_CBT					Fld(1, 13) //cc add
    #define ARBCTL_RSV_DRAM_TYPE				Fld(3, 10)//cc add
    #define ARBCTL_DBIWR_OPT_bit1_0 				Fld(2, 16)//cc add
    #define ARBCTL_DBIWR_OPT_bit7				Fld(1, 23)//cc add
    #define ARBCTL_DBIWR_OPT_bit9_8 				Fld(2, 24)//cc add
    #define ARBCTL_DBIWR_OPT_bit15				Fld(1, 31)//cc add

#define DRAMC_REG_RSTMASK                                  (DRAMC_AO_BASE_ADDRESS + 0x0001C)
    #define RSTMASK_WDATKEY0                                   Fld(1, 0) //[0:0]
    #define RSTMASK_WDATKEY1                                   Fld(1, 1) //[1:1]
    #define RSTMASK_WDATKEY2                                   Fld(1, 2) //[2:2]
    #define RSTMASK_WDATKEY3                                   Fld(1, 3) //[3:3]
    #define RSTMASK_WDATKEY4                                   Fld(1, 4) //[4:4]
    #define RSTMASK_WDATKEY5                                   Fld(1, 5) //[5:5]
    #define RSTMASK_WDATKEY6                                   Fld(1, 6) //[6:6]
    #define RSTMASK_WDATKEY7                                   Fld(1, 7) //[7:7]
    #define RSTMASK_WDATITLV                                   Fld(1, 8) //[8:8]
    #define RSTMASK_RETRY_DATRST_MASK                          Fld(1, 21) //[21:21]
    #define RSTMASK_DVFS_SYNC_MASK_FOR_PHY                     Fld(1, 24) //[24:24]
    #define RSTMASK_GT_SYNC_MASK_FOR_PHY                       Fld(1, 25) //[25:25]
    #define RSTMASK_DVFS_SYNC_MASK                             Fld(1, 26) //[26:26]
    #define RSTMASK_GTDMW_SYNC_MASK                            Fld(1, 27) //[27:27]
    #define RSTMASK_GT_SYNC_MASK                               Fld(1, 28) //[28:28]
    #define RSTMASK_DAT_SYNC_MASK                              Fld(1, 29) //[29:29]
    #define RSTMASK_PHY_SYNC_MASK                              Fld(1, 30) //[30:30]
    #define RSTMASK_R_DMSHU_RDATRST_MASK                       Fld(1, 31) //[31:31]
    #define RSTMASK_RSV_SA_BU2					Fld(4, 12) //cc add
    #define RSTMASK_RSV_DRAM_CBT_MIXED				Fld(2, 13) //cc add
    #define RSTMASK_RSV_DRAM_SUPPORT_RANK_NUM			Fld(1, 12) //cc add

#define DRAMC_REG_PADCTRL                                  (DRAMC_AO_BASE_ADDRESS + 0x00020)
    #define PADCTRL_DQIENQKEND                                 Fld(2, 0) //[1:0]
    #define PADCTRL_DQIENLATEBEGIN                             Fld(1, 3) //[3:3]
    #define PADCTRL_DISDMOEDIS                                 Fld(1, 8) //[8:8]
    #define PADCTRL_DRAMOEN                                    Fld(1, 12) //[12:12]
    #define PADCTRL_FIXDQIEN                                   Fld(4, 16) //[19:16]
    #define PADCTRL_DISDQIEN                                   Fld(4, 20) //[23:20]
    #define PADCTRL_PINMUX                                     Fld(3, 28) //[30:28]

#define DRAMC_REG_CKECTRL                                  (DRAMC_AO_BASE_ADDRESS + 0x00024)
    #define CKECTRL_CKEBYCTL                                   Fld(1, 0) //[0:0]
    #define CKECTRL_CKE2RANK_OPT3                              Fld(1, 1) //[1:1]
    #define CKECTRL_CKE2FIXON                                  Fld(1, 2) //[2:2]
    #define CKECTRL_CKE2FIXOFF                                 Fld(1, 3) //[3:3]
    #define CKECTRL_CKE1FIXON                                  Fld(1, 4) //[4:4]
    #define CKECTRL_CKE1FIXOFF                                 Fld(1, 5) //[5:5]
    #define CKECTRL_CKEFIXON                                   Fld(1, 6) //[6:6]
    #define CKECTRL_CKEFIXOFF                                  Fld(1, 7) //[7:7]
    #define CKECTRL_CKE2RANK_OPT5                              Fld(1, 8) //[8:8]
    #define CKECTRL_CKE2RANK_OPT6                              Fld(1, 9) //[9:9]
    #define CKECTRL_CKE2RANK_OPT7                              Fld(1, 10) //[10:10]
    #define CKECTRL_CKE2RANK_OPT8                              Fld(1, 11) //[11:11]
    #define CKECTRL_CKEEXTEND                                  Fld(1, 12) //[12:12]
    #define CKECTRL_CKETIMER_SEL                               Fld(1, 13) //[13:13]
    #define CKECTRL_FASTWAKE_SEL                               Fld(1, 14) //[14:14]
    #define CKECTRL_CKEWAKE_SEL                                Fld(1, 15) //[15:15]
    #define CKECTRL_CKEWAKE_SEL2                               Fld(1, 16) //[16:16]
    #define CKECTRL_CKE2RANK_OPT9                              Fld(1, 17) //[17:17]
    #define CKECTRL_CKE2RANK_OPT10                             Fld(1, 18) //[18:18]
    #define CKECTRL_CKE2RANK_OPT11                             Fld(1, 19) //[19:19]
    #define CKECTRL_CKE2RANK_OPT12                             Fld(1, 20) //[20:20]
    #define CKECTRL_CKE2RANK_OPT13                             Fld(1, 21) //[21:21]
    #define CKECTRL_CKEPBDIS                                   Fld(1, 22) //[22:22]
    #define CKECTRL_CKELCKFIX                                  Fld(1, 23) //[23:23]
    #define CKECTRL_CKELCKCNT                                  Fld(3, 24) //[26:24]
    #define CKECTRL_RUNTIMEMRRCKEFIX                           Fld(1, 27) //[27:27]
    #define CKECTRL_RUNTIMEMRRMIODIS                           Fld(1, 28) //[28:28]
    #define CKECTRL_CKE_H2L_OPT                                Fld(1, 29) //[29:29]
    #define CKECTRL_CKEON                                      Fld(1, 31) //[31:31]

#define DRAMC_REG_DRSCTRL                                  (DRAMC_AO_BASE_ADDRESS + 0x00028)
    #define DRSCTRL_DRSDIS                                     Fld(1, 0) //[0:0]
    #define DRSCTRL_DRSBLOCKOPT                                Fld(1, 1) //[1:1]
    #define DRSCTRL_DRSPB2AB_OPT                               Fld(1, 2) //[2:2]
    #define DRSCTRL_DRSRK1_SW                                  Fld(1, 3) //[3:3]
    #define DRSCTRL_DRSMON_CLR                                 Fld(1, 4) //[4:4]
    #define DRSCTRL_DRSCLR_EN                                  Fld(1, 5) //[5:5]
    #define DRSCTRL_DRSACKWAITREF                              Fld(1, 6) //[6:6]
    #define DRSCTRL_DRSCLR_RK0_EN                              Fld(1, 7) //[7:7]
    #define DRSCTRL_DRSDLY                                     Fld(4, 8) //[11:8]
    #define DRSCTRL_DRS_CNTX                                   Fld(7, 12) //[18:12]
    #define DRSCTRL_DRS_SELFWAKE_DMYRD_DIS                     Fld(1, 19) //[19:19]
    #define DRSCTRL_DRS_DMYRD_MIOCK_OPT                        Fld(1, 20) //[20:20]
    #define DRSCTRL_DRSOPT2                                    Fld(1, 21) //[21:21]
    #define DRSCTRL_DRS_MR4_OPT_B                              Fld(1, 24) //[24:24]
    #define DRSCTRL_RK_SCINPUT_OPT                             Fld(1, 29) //[29:29]

#define DRAMC_REG_RKCFG                                    (DRAMC_AO_BASE_ADDRESS + 0x00034)
    #define RKCFG_TXRANK                                       Fld(2, 0) //[1:0]
    #define RKCFG_CKE2RANK_OPT2                                Fld(1, 2) //[2:2]
    #define RKCFG_TXRANKFIX                                    Fld(1, 3) //[3:3]
    #define RKCFG_RKMODE                                       Fld(3, 4) //[6:4]
    #define RKCFG_RKSWAP                                       Fld(1, 7) //[7:7]
    #define RKCFG_DM3RANK                                      Fld(1, 8) //[8:8]
    #define RKCFG_RANKRDY_OPT                                  Fld(1, 9) //[9:9]
    #define RKCFG_MRS2RK                                       Fld(1, 10) //[10:10]
    #define RKCFG_DQSOSC2RK                                    Fld(1, 11) //[11:11]
    #define RKCFG_CKE2RANK                                     Fld(1, 12) //[12:12]
    #define RKCFG_CS2RANK                                      Fld(1, 13) //[13:13]
    #define RKCFG_SHU2RKOPT                                    Fld(1, 14) //[14:14]
    #define RKCFG_CKE2RANK_OPT                                 Fld(1, 15) //[15:15]
    #define RKCFG_RKSIZE                                       Fld(3, 16) //[18:16]
    #define RKCFG_DMCKEWAKE                                    Fld(1, 19) //[19:19]
    #define RKCFG_RK0SRF                                       Fld(1, 20) //[20:20]
    #define RKCFG_RK1SRF                                       Fld(1, 21) //[21:21]
    #define RKCFG_RK2SRF                                       Fld(1, 22) //[22:22]
    #define RKCFG_SRF_ENTER_MASK_OPT                           Fld(1, 23) //[23:23]
    #define RKCFG_RK0DPD                                       Fld(1, 24) //[24:24]
    #define RKCFG_RK1DPD                                       Fld(1, 25) //[25:25]
    #define RKCFG_RK2DPD                                       Fld(1, 26) //[26:26]
    #define RKCFG_RK0DPDX                                      Fld(1, 28) //[28:28]
    #define RKCFG_RK1DPDX                                      Fld(1, 29) //[29:29]
    #define RKCFG_RK2DPDX                                      Fld(1, 30) //[30:30]
    #define RKCFG_CS0FORCE                                     Fld(1, 31) //[31:31]

#define DRAMC_REG_DRAMC_PD_CTRL                            (DRAMC_AO_BASE_ADDRESS + 0x00038)
    #define DRAMC_PD_CTRL_DCMEN                                Fld(1, 0) //[0:0]
    #define DRAMC_PD_CTRL_DCMEN2                               Fld(1, 1) //[1:1]
    #define DRAMC_PD_CTRL_DCMENNOTRFC                          Fld(1, 2) //[2:2]
    #define DRAMC_PD_CTRL_PHYCLK_REFWKEN                       Fld(1, 4) //[4:4]
    #define DRAMC_PD_CTRL_COMBPHY_CLKENSAME                    Fld(1, 5) //[5:5]
    #define DRAMC_PD_CTRL_DCMREF_OPT                           Fld(1, 8) //[8:8]
    #define DRAMC_PD_CTRL_PG_DCM_OPT                           Fld(1, 9) //[9:9]
    #define DRAMC_PD_CTRL_COMB_DCM                             Fld(1, 10) //[10:10]
    #define DRAMC_PD_CTRL_RDPERIODON                           Fld(1, 19) //[19:19]
    #define DRAMC_PD_CTRL_DQIEN_BUFFEN_OPT                     Fld(2, 20) //[21:20]
    #define DRAMC_PD_CTRL_MIOCKCTRLOFF                         Fld(1, 26) //[26:26]
    #define DRAMC_PD_CTRL_DISSTOP26M                           Fld(1, 27) //[27:27]
    #define DRAMC_PD_CTRL_PHYCLKDYNGEN                         Fld(1, 30) //[30:30]
    #define DRAMC_PD_CTRL_COMBCLKCTRL                          Fld(1, 31) //[31:31]

#define DRAMC_REG_CLKAR                                    (DRAMC_AO_BASE_ADDRESS + 0x0003C)
    #define CLKAR_REQQUE_PACG_DIS                              Fld(15, 0) //[14:0]
    #define CLKAR_SELPH_CMD_CG_DIS                             Fld(1, 15) //[15:15]
    #define CLKAR_RDATCKAR                                     Fld(1, 16) //[16:16]
    #define CLKAR_SRF_CLKRUN                                   Fld(1, 17) //[17:17]
    #define CLKAR_IDLE_OPT                                     Fld(1, 18) //[18:18]
    #define CLKAR_PSELAR                                       Fld(1, 19) //[19:19]
    #define CLKAR_BCLKAR                                       Fld(1, 20) //[20:20]
    #define CLKAR_SELPH_4LCG_DIS                               Fld(1, 21) //[21:21]
    #define CLKAR_SELPH_CG_DIS                                 Fld(1, 22) //[22:22]
    #define CLKAR_TESTCLKRUN                                   Fld(1, 23) //[23:23]
    #define CLKAR_PHYGLUECLKRUN                                Fld(1, 24) //[24:24]
    #define CLKAR_DWCLKRUN                                     Fld(1, 25) //[25:25]
    #define CLKAR_REFCLKRUN                                    Fld(1, 26) //[26:26]
    #define CLKAR_REQQUECLKRUN                                 Fld(1, 27) //[27:27]
    #define CLKAR_SEQCLKRUN                                    Fld(1, 28) //[28:28]
    #define CLKAR_CALCKAR                                      Fld(1, 29) //[29:29]
    #define CLKAR_CMDCKAR                                      Fld(1, 30) //[30:30]
    #define CLKAR_RDYCKAR                                      Fld(1, 31) //[31:31]

#define DRAMC_REG_CLKCTRL                                  (DRAMC_AO_BASE_ADDRESS + 0x00040)
    #define CLKCTRL_PSEL_CNT                                   Fld(6, 0) //[5:0]
    #define CLKCTRL_SEQCLKRUN3                                 Fld(1, 7) //[7:7]
    #define CLKCTRL_SEQCLKRUN2                                 Fld(1, 8) //[8:8]
    #define CLKCTRL_CLK_EN_0                                   Fld(1, 28) //[28:28]
    #define CLKCTRL_CLK_EN_1                                   Fld(1, 29) //[29:29]

#define DRAMC_REG_SELFREF_HWSAVE_FLAG                      (DRAMC_AO_BASE_ADDRESS + 0x00044)
    #define SELFREF_HWSAVE_FLAG_SELFREF_HWSAVE_FLAG_FROM_AO    Fld(1, 0) //[0:0]

#define DRAMC_REG_SREFCTRL                                 (DRAMC_AO_BASE_ADDRESS + 0x00048)
    #define SREFCTRL_HMRRSEL_CGAR                              Fld(1, 12) //[12:12]
    #define SREFCTRL_RDDQSOSC_CGAR                             Fld(1, 13) //[13:13]
    #define SREFCTRL_SCARB_SM_CGAR                             Fld(1, 14) //[14:14]
    #define SREFCTRL_SCSM_CGAR                                 Fld(1, 15) //[15:15]
    #define SREFCTRL_SRFPD_DIS                                 Fld(1, 16) //[16:16]
    #define SREFCTRL_DQSOSC_THRD_OPT                           Fld(1, 17) //[17:17]
    #define SREFCTRL_DQSOSC_C2R_OPT                            Fld(1, 18) //[18:18]
    #define SREFCTRL_SREF3_OPTION                              Fld(1, 20) //[20:20]
    #define SREFCTRL_SREF3_OPTION1                             Fld(1, 21) //[21:21]
    #define SREFCTRL_SREF2_OPTION                              Fld(1, 22) //[22:22]
    #define SREFCTRL_SREFDLY                                   Fld(4, 24) //[27:24]
    #define SREFCTRL_SREF_HW_EN                                Fld(1, 30) //[30:30]
    #define SREFCTRL_SELFREF                                   Fld(1, 31) //[31:31]

#define DRAMC_REG_REFCTRL0                                 (DRAMC_AO_BASE_ADDRESS + 0x0004C)
    #define REFCTRL0_DLLFRZ                                    Fld(1, 0) //[0:0]
    #define REFCTRL0_UPDBYWR                                   Fld(1, 1) //[1:1]
    #define REFCTRL0_DRVCGWREF                                 Fld(1, 2) //[2:2]
    #define REFCTRL0_DQDRVSWUPD                                Fld(1, 3) //[3:3]
    #define REFCTRL0_RFRINTCTL                                 Fld(1, 5) //[5:5]
    #define REFCTRL0_RFRINTEN                                  Fld(1, 6) //[6:6]
    #define REFCTRL0_REFOVERCNT_RST                            Fld(1, 7) //[7:7]
    #define REFCTRL0_DMPGVLD_IG                                Fld(1, 8) //[8:8]
    #define REFCTRL0_REFMODE_MANUAL                            Fld(1, 10) //[10:10]
    #define REFCTRL0_REFMODE_MANUAL_TRIG                       Fld(1, 11) //[11:11]
    #define REFCTRL0_DISBYREFNUM                               Fld(3, 12) //[14:12]
    #define REFCTRL0_PBREF_DISBYREFNUM                         Fld(1, 16) //[16:16]
    #define REFCTRL0_PBREF_DISBYRATE                           Fld(1, 17) //[17:17]
    #define REFCTRL0_PBREFEN                                   Fld(1, 18) //[18:18]
    #define REFCTRL0_ADVREF_CNT                                Fld(4, 20) //[23:20]
    #define REFCTRL0_REF_PREGATE_CNT                           Fld(4, 24) //[27:24]
    #define REFCTRL0_REFNA_OPT                                 Fld(1, 28) //[28:28]
    #define REFCTRL0_REFDIS                                    Fld(1, 29) //[29:29]
    #define REFCTRL0_REFFRERUN                                 Fld(1, 30) //[30:30]
    #define REFCTRL0_REFBW_FREN                                Fld(1, 31) //[31:31]

#define DRAMC_REG_REFCTRL1                                 (DRAMC_AO_BASE_ADDRESS + 0x00050)
    #define REFCTRL1_SLEFREF_AUTOSAVE_EN                       Fld(1, 0) //[0:0]
    #define REFCTRL1_SREF_PRD_OPT                              Fld(1, 1) //[1:1]
    #define REFCTRL1_PSEL_OPT2                                 Fld(1, 2) //[2:2]
    #define REFCTRL1_PSEL_OPT3                                 Fld(1, 3) //[3:3]
    #define REFCTRL1_PRE8REF                                   Fld(1, 4) //[4:4]
    #define REFCTRL1_REF_QUE_AUTOSAVE_EN                       Fld(1, 5) //[5:5]
    #define REFCTRL1_PSEL_OPT1                                 Fld(1, 6) //[6:6]
    #define REFCTRL1_SREF_CG_OPT                               Fld(1, 7) //[7:7]
    #define REFCTRL1_MPENDREF_CNT                              Fld(3, 8) //[10:8]
    #define REFCTRL1_REFRATE_MON_CLR                           Fld(1, 11) //[11:11]
    #define REFCTRL1_REFRATE_MANUAL                            Fld(3, 28) //[30:28]
    #define REFCTRL1_REFRATE_MANUAL_RATE_TRIG                  Fld(1, 31) //[31:31]

#define DRAMC_REG_REFRATRE_FILTER                          (DRAMC_AO_BASE_ADDRESS + 0x00054)
    #define REFRATRE_FILTER_REFRATE_FIL0                       Fld(3, 0) //[2:0]
    #define REFRATRE_FILTER_REFRATE_FIL1                       Fld(3, 4) //[6:4]
    #define REFRATRE_FILTER_REFRATE_FIL2                       Fld(3, 8) //[10:8]
    #define REFRATRE_FILTER_REFRATE_FIL3                       Fld(3, 12) //[14:12]
    #define REFRATRE_FILTER_PB2AB_OPT                          Fld(1, 15) //[15:15]
    #define REFRATRE_FILTER_REFRATE_FIL4                       Fld(3, 16) //[18:16]
    #define REFRATRE_FILTER_REFRATE_FIL5                       Fld(3, 20) //[22:20]
    #define REFRATRE_FILTER_PB2AB_OPT1                         Fld(1, 23) //[23:23]
    #define REFRATRE_FILTER_REFRATE_FIL6                       Fld(3, 24) //[26:24]
    #define REFRATRE_FILTER_REFRATE_FIL7                       Fld(3, 28) //[30:28]
    #define REFRATRE_FILTER_REFRATE_FILEN                      Fld(1, 31) //[31:31]

#define DRAMC_REG_ZQCS                                     (DRAMC_AO_BASE_ADDRESS + 0x00058)
    #define ZQCS_ZQCSOP                                        Fld(8, 0) //[7:0]
    #define ZQCS_ZQCSAD                                        Fld(8, 8) //[15:8]
    #define ZQCS_ZQCS_MASK_SEL                                 Fld(3, 16) //[18:16]
    #define ZQCS_ZQCS_MASK_SEL_CGAR                            Fld(1, 19) //[19:19]
    #define ZQCS_ZQMASK_CGAR                                   Fld(1, 20) //[20:20]
    #define ZQCS_ZQCSMASK_OPT                                  Fld(1, 21) //[21:21]
    #define ZQCS_ZQ_SRF_OPT                                    Fld(1, 22) //[22:22]
    #define ZQCS_ZQCSMASK                                      Fld(1, 30) //[30:30]
    #define ZQCS_ZQCSDUAL                                      Fld(1, 31) //[31:31]

#define DRAMC_REG_MRS                                      (DRAMC_AO_BASE_ADDRESS + 0x0005C)
    #define MRS_MRSOP                                          Fld(8, 0) //[7:0]
    #define MRS_MRSMA                                          Fld(13, 8) //[20:8]
    #define MRS_MRSBA                                          Fld(3, 21) //[23:21]
    #define MRS_MRSRK                                          Fld(2, 24) //[25:24]
    #define MRS_MRRRK                                          Fld(2, 26) //[27:26]
    #define MRS_MPCRK                                          Fld(2, 28) //[29:28]
    #define MRS_MRSBG                                          Fld(2, 30) //[31:30]

#define DRAMC_REG_SPCMD                                    (DRAMC_AO_BASE_ADDRESS + 0x00060)
    #define SPCMD_MRWEN                                        Fld(1, 0) //[0:0]
    #define SPCMD_MRREN                                        Fld(1, 1) //[1:1]
    #define SPCMD_PREAEN                                       Fld(1, 2) //[2:2]
    #define SPCMD_AREFEN                                       Fld(1, 3) //[3:3]
    #define SPCMD_ZQCEN                                        Fld(1, 4) //[4:4]
    #define SPCMD_TCMDEN                                       Fld(1, 5) //[5:5]
    #define SPCMD_ZQLATEN                                      Fld(1, 6) //[6:6]
    #define SPCMD_RDDQCEN                                      Fld(1, 7) //[7:7]
    #define SPCMD_DQSGCNTEN                                    Fld(1, 8) //[8:8]
    #define SPCMD_DQSGCNTRST                                   Fld(1, 9) //[9:9]
    #define SPCMD_DQSOSCENEN                                   Fld(1, 10) //[10:10]
    #define SPCMD_DQSOSCDISEN                                  Fld(1, 11) //[11:11]
    #define SPCMD_ACTEN                                        Fld(1, 12) //[12:12]
    #define SPCMD_MPRWEN                                       Fld(1, 13) //[13:13]

#define DRAMC_REG_SPCMDCTRL                                (DRAMC_AO_BASE_ADDRESS + 0x00064)
    #define SPCMDCTRL_SC_PG_UPD_OPT                            Fld(1, 0) //[0:0]
    #define SPCMDCTRL_SC_PG_MAN_DIS                            Fld(1, 1) //[1:1]
    #define SPCMDCTRL_SPREA_EN                                 Fld(1, 2) //[2:2]
    #define SPCMDCTRL_SCARB_PRI_OPT                            Fld(1, 4) //[4:4]
    #define SPCMDCTRL_MRRSWUPD                                 Fld(1, 5) //[5:5]
    #define SPCMDCTRL_R_DMDVFSMRW_EN                           Fld(1, 6) //[6:6]
    #define SPCMDCTRL_DPDWOSC                                  Fld(1, 7) //[7:7]
    #define SPCMDCTRL_SC_PG_OPT2_DIS                           Fld(1, 8) //[8:8]
    #define SPCMDCTRL_SC_PG_STCMD_AREF_DIS                     Fld(1, 9) //[9:9]
    #define SPCMDCTRL_SC_PG_MPRW_DIS                           Fld(1, 10) //[10:10]
    #define SPCMDCTRL_RDDQCDIS                                 Fld(1, 11) //[11:11]
    #define SPCMDCTRL_HMR4_TOG_OPT                             Fld(1, 18) //[18:18]
    #define SPCMDCTRL_SCPRE                                    Fld(1, 19) //[19:19]
    #define SPCMDCTRL_ZQCS_NONMASK_CLR                         Fld(1, 20) //[20:20]
    #define SPCMDCTRL_ZQCS_MASK_FIX                            Fld(1, 21) //[21:21]
    #define SPCMDCTRL_ZQCS_MASK_VALUE                          Fld(1, 22) //[22:22]
    #define SPCMDCTRL_SRFMR4_CNTKEEP_B                         Fld(1, 24) //[24:24]
    #define SPCMDCTRL_MRWWOPRA                                 Fld(1, 25) //[25:25]
    #define SPCMDCTRL_CLR_EN                                   Fld(1, 26) //[26:26]
    #define SPCMDCTRL_MRRREFUPD_B                              Fld(1, 27) //[27:27]
    #define SPCMDCTRL_REFR_BLOCKEN                             Fld(1, 28) //[28:28]
    #define SPCMDCTRL_REFRDIS                                  Fld(1, 29) //[29:29]
    #define SPCMDCTRL_ZQCALDISB                                Fld(1, 30) //[30:30]
    #define SPCMDCTRL_ZQCSDISB                                 Fld(1, 31) //[31:31]

#define DRAMC_REG_PPR_CTRL                                 (DRAMC_AO_BASE_ADDRESS + 0x00068)
    #define PPR_CTRL_ACTEN_BK                                  Fld(3, 12) //[14:12]
    #define PPR_CTRL_ACTEN_ROW                                 Fld(16, 16) //[31:16]

#define DRAMC_REG_MPC_OPTION                               (DRAMC_AO_BASE_ADDRESS + 0x0006C)
    #define MPC_OPTION_MPC_BLOCKALE_OPT                        Fld(1, 0) //[0:0]
    #define MPC_OPTION_MPC_BLOCKALE_OPT1                       Fld(1, 1) //[1:1]
    #define MPC_OPTION_MPC_BLOCKALE_OPT2                       Fld(1, 2) //[2:2]
    #define MPC_OPTION_ZQ_BLOCKALE_OPT                         Fld(1, 3) //[3:3]
    #define MPC_OPTION_RW2ZQLAT_OPT                            Fld(1, 4) //[4:4]
    #define MPC_OPTION_MPCOP                                   Fld(7, 8) //[14:8]
    #define MPC_OPTION_MPCMANEN                                Fld(1, 15) //[15:15]
    #define MPC_OPTION_MPCMAN_CAS2EN                           Fld(1, 16) //[16:16]
    #define MPC_OPTION_MPCRKEN                                 Fld(1, 17) //[17:17]

#define DRAMC_REG_REFQUE_CNT                               (DRAMC_AO_BASE_ADDRESS + 0x00070)
    #define REFQUE_CNT_REFRESH_QUEUE_CNT_FROM_AO               Fld(4, 0) //[3:0]

#define DRAMC_REG_HW_MRR_FUN                               (DRAMC_AO_BASE_ADDRESS + 0x00074)
    #define HW_MRR_FUN_TMRR_ENA                                Fld(1, 0) //[0:0]
    #define HW_MRR_FUN_TRCDMRR_EN                              Fld(1, 1) //[1:1]
    #define HW_MRR_FUN_TRPMRR_EN                               Fld(1, 2) //[2:2]
    #define HW_MRR_FUN_MANTMRR_EN                              Fld(1, 3) //[3:3]
    #define HW_MRR_FUN_MANTMRR                                 Fld(4, 4) //[7:4]
    #define HW_MRR_FUN_BUFEN_RFC_OPT                           Fld(1, 8) //[8:8]
    #define HW_MRR_FUN_MRR_REQNOPUSH_DIS                       Fld(1, 9) //[9:9]
    #define HW_MRR_FUN_MRR_BLOCK_NOR_DIS                       Fld(1, 10) //[10:10]
    #define HW_MRR_FUN_MRR_HW_HIPRI                            Fld(1, 11) //[11:11]
    #define HW_MRR_FUN_MRR_SPCMD_WAKE_DIS                      Fld(1, 12) //[12:12]
    #define HW_MRR_FUN_TMRR_OE_OPT_DIS                         Fld(1, 13) //[13:13]
    #define HW_MRR_FUN_MRR_PUSH2POP_ENA                        Fld(1, 16) //[16:16]
    #define HW_MRR_FUN_MRR_PUSH2POP_CLR                        Fld(1, 17) //[17:17]
    #define HW_MRR_FUN_MRR_PUSH2POP_ST_CLR                     Fld(1, 18) //[18:18]
    #define HW_MRR_FUN_MRR_PUSH2POP_SEL                        Fld(3, 20) //[22:20]
    #define HW_MRR_FUN_MRR_SBR3_BKVA_DIS                       Fld(1, 23) //[23:23]
    #define HW_MRR_FUN_MRR_DDRCLKCOMB_DIS                      Fld(1, 24) //[24:24]
    #define HW_MRR_FUN_TRPRCD_DIS_OPT1                         Fld(1, 25) //[25:25]
    #define HW_MRR_FUN_TRPRCD_OPT2                             Fld(1, 26) //[26:26]
    #define HW_MRR_FUN_MRR_SBR2_QHIT_DIS                       Fld(1, 27) //[27:27]
    #define HW_MRR_FUN_MRR_INPUT_BANK                          Fld(3, 28) //[30:28]
    #define HW_MRR_FUN_MRR_TZQCS_DIS                           Fld(1, 31) //[31:31]

#define DRAMC_REG_MRR_BIT_MUX1                             (DRAMC_AO_BASE_ADDRESS + 0x00078)
    #define MRR_BIT_MUX1_MRR_BIT0_SEL                          Fld(5, 0) //[4:0]
    #define MRR_BIT_MUX1_MRR_BIT1_SEL                          Fld(5, 8) //[12:8]
    #define MRR_BIT_MUX1_MRR_BIT2_SEL                          Fld(5, 16) //[20:16]
    #define MRR_BIT_MUX1_MRR_BIT3_SEL                          Fld(5, 24) //[28:24]

#define DRAMC_REG_MRR_BIT_MUX2                             (DRAMC_AO_BASE_ADDRESS + 0x0007C)
    #define MRR_BIT_MUX2_MRR_BIT4_SEL                          Fld(5, 0) //[4:0]
    #define MRR_BIT_MUX2_MRR_BIT5_SEL                          Fld(5, 8) //[12:8]
    #define MRR_BIT_MUX2_MRR_BIT6_SEL                          Fld(5, 16) //[20:16]
    #define MRR_BIT_MUX2_MRR_BIT7_SEL                          Fld(5, 24) //[28:24]

#define DRAMC_REG_MRR_BIT_MUX3                             (DRAMC_AO_BASE_ADDRESS + 0x00080)
    #define MRR_BIT_MUX3_MRR_BIT8_SEL                          Fld(5, 0) //[4:0]
    #define MRR_BIT_MUX3_MRR_BIT9_SEL                          Fld(5, 8) //[12:8]
    #define MRR_BIT_MUX3_MRR_BIT10_SEL                         Fld(5, 16) //[20:16]
    #define MRR_BIT_MUX3_MRR_BIT11_SEL                         Fld(5, 24) //[28:24]

#define DRAMC_REG_MRR_BIT_MUX4                             (DRAMC_AO_BASE_ADDRESS + 0x00084)
    #define MRR_BIT_MUX4_MRR_BIT12_SEL                         Fld(5, 0) //[4:0]
    #define MRR_BIT_MUX4_MRR_BIT13_SEL                         Fld(5, 8) //[12:8]
    #define MRR_BIT_MUX4_MRR_BIT14_SEL                         Fld(5, 16) //[20:16]
    #define MRR_BIT_MUX4_MRR_BIT15_SEL                         Fld(5, 24) //[28:24]

#define DRAMC_REG_TEST2_5                                  (DRAMC_AO_BASE_ADDRESS + 0x0008C)
    #define TEST2_5_TEST2_BASE_2                               Fld(28, 4) //[31:4]

#define DRAMC_REG_TEST2_0                                  (DRAMC_AO_BASE_ADDRESS + 0x00090)
    #define TEST2_0_TEST2_PAT1                                 Fld(8, 0) //[7:0]
    #define TEST2_0_TEST2_PAT0                                 Fld(8, 8) //[15:8]

#define DRAMC_REG_TEST2_1                                  (DRAMC_AO_BASE_ADDRESS + 0x00094)
    #define TEST2_1_TEST2_BASE                                 Fld(28, 4) //[31:4]

#define DRAMC_REG_TEST2_2                                  (DRAMC_AO_BASE_ADDRESS + 0x00098)
    #define TEST2_2_TEST2_OFF                                  Fld(28, 4) //[31:4]

#define DRAMC_REG_TEST2_3                                  (DRAMC_AO_BASE_ADDRESS + 0x0009C)
    #define TEST2_3_TESTCNT                                    Fld(4, 0) //[3:0]
    #define TEST2_3_DQSICALEN                                  Fld(1, 4) //[4:4]
    #define TEST2_3_DQSICALUPD                                 Fld(1, 5) //[5:5]
    #define TEST2_3_PSTWR2                                     Fld(1, 6) //[6:6]
    #define TEST2_3_TESTAUDPAT                                 Fld(1, 7) //[7:7]
    #define TEST2_3_DQSICALSTP                                 Fld(3, 8) //[10:8]
    #define TEST2_3_DQDLYAUTO                                  Fld(1, 11) //[11:11]
    #define TEST2_3_MANUDLLFRZ                                 Fld(1, 12) //[12:12]
    #define TEST2_3_MANUDQSUPD                                 Fld(1, 13) //[13:13]
    #define TEST2_3_DQSUPDMODE                                 Fld(1, 14) //[14:14]
    #define TEST2_3_DRDELSWEN                                  Fld(1, 19) //[19:19]
    #define TEST2_3_DRDELSWSEL                                 Fld(3, 20) //[22:20]
    #define TEST2_3_MDQS                                       Fld(1, 23) //[23:23]
    #define TEST2_3_DMPAT32                                    Fld(1, 24) //[24:24]
    #define TEST2_3_TESTADR_SHIFT                              Fld(1, 25) //[25:25]
    #define TEST2_3_TAHPRI_B                                   Fld(1, 26) //[26:26]
    #define TEST2_3_TESTLP                                     Fld(1, 27) //[27:27]
    #define TEST2_3_TEST2WREN2_HW_EN                           Fld(1, 28) //[28:28]
    #define TEST2_3_TEST1                                      Fld(1, 29) //[29:29]
    #define TEST2_3_TEST2R                                     Fld(1, 30) //[30:30]
    #define TEST2_3_TEST2W                                     Fld(1, 31) //[31:31]

#define DRAMC_REG_TEST2_4                                  (DRAMC_AO_BASE_ADDRESS + 0x000A0)
    #define TEST2_4_TESTAUDINC                                 Fld(5, 0) //[4:0]
    #define TEST2_4_TEST2DISSCRAM                              Fld(1, 5) //[5:5]
    #define TEST2_4_TESTSSOPAT                                 Fld(1, 6) //[6:6]
    #define TEST2_4_TESTSSOXTALKPAT                            Fld(1, 7) //[7:7]
    #define TEST2_4_TESTAUDINIT                                Fld(5, 8) //[12:8]
    #define TEST2_4_TESTAUDBITINV                              Fld(1, 14) //[14:14]
    #define TEST2_4_TESTAUDMODE                                Fld(1, 15) //[15:15]
    #define TEST2_4_TESTXTALKPAT                               Fld(1, 16) //[16:16]
    #define TEST2_4_TEST_REQ_LEN1                              Fld(1, 17) //[17:17]
    #define TEST2_4_DISMASK                                    Fld(1, 20) //[20:20]
    #define TEST2_4_DQCALDIS                                   Fld(1, 22) //[22:22]
    #define TEST2_4_NEGDQS                                     Fld(1, 23) //[23:23]
    #define TEST2_4_TESTAGENTRK                                Fld(2, 24) //[25:24]
    #define TEST2_4_TESTAGENTRKSEL                             Fld(3, 28) //[30:28]

#define DRAMC_REG_WDT_DBG_SIGNAL                           (DRAMC_AO_BASE_ADDRESS + 0x000A4)
    #define WDT_DBG_SIGNAL_LATCH_RD_TEST_DM_CMP_CPT2_RK0       Fld(1, 0) //[0:0]
    #define WDT_DBG_SIGNAL_LATCH_RD_TEST_DM_CMP_CPT2_RK1       Fld(1, 1) //[1:1]
    #define WDT_DBG_SIGNAL_LATCH_RD_TEST_DM_CMP_ERR2_RK0       Fld(1, 2) //[2:2]
    #define WDT_DBG_SIGNAL_LATCH_RD_TEST_DM_CMP_ERR2_RK1       Fld(1, 3) //[3:3]
    #define WDT_DBG_SIGNAL_LATCH_RD_TEST_DLE_CNT_OK2_RK0       Fld(1, 4) //[4:4]
    #define WDT_DBG_SIGNAL_LATCH_RD_TEST_DLE_CNT_OK2_RK1       Fld(1, 5) //[5:5]
    #define WDT_DBG_SIGNAL_LATCH_RDWR_TEST_DM_CMP_CPT2_RK0     Fld(1, 8) //[8:8]
    #define WDT_DBG_SIGNAL_LATCH_RDWR_TEST_DM_CMP_CPT2_RK1     Fld(1, 9) //[9:9]
    #define WDT_DBG_SIGNAL_LATCH_RDWR_TEST_DM_CMP_ERR2_RK0     Fld(1, 10) //[10:10]
    #define WDT_DBG_SIGNAL_LATCH_RDWR_TEST_DM_CMP_ERR2_RK1     Fld(1, 11) //[11:11]
    #define WDT_DBG_SIGNAL_LATCH_RDWR_TEST_DLE_CNT_OK2_RK0     Fld(1, 12) //[12:12]
    #define WDT_DBG_SIGNAL_LATCH_RDWR_TEST_DLE_CNT_OK2_RK1     Fld(1, 13) //[13:13]
    #define WDT_DBG_SIGNAL_LATCH_DRAMC_GATING_ERROR            Fld(1, 14) //[14:14]
    #define WDT_DBG_SIGNAL_LATCH_DRAMC_FIFO_STBEN_ERR 		Fld(1, 15) /* cc add */

#define DRAMC_REG_LBTEST                                   (DRAMC_AO_BASE_ADDRESS + 0x000AC)
    #define LBTEST_LBTEST_IGB0                                 Fld(1, 0) //[0:0]
    #define LBTEST_LBTEST_IGB1                                 Fld(1, 1) //[1:1]
    #define LBTEST_LBTEST_IGB2                                 Fld(1, 2) //[2:2]
    #define LBTEST_LBTEST_IGB3                                 Fld(1, 3) //[3:3]
    #define LBTEST_LBTEST                                      Fld(1, 4) //[4:4]
    #define LBTEST_LBTEST_MODE                                 Fld(1, 5) //[5:5]
    #define LBTEST_R_EXTLB_DRAMC_CONF_I                        Fld(5, 8) //[12:8]
    #define LBTEST_OCDPAT                                      Fld(8, 16) //[23:16]
    #define LBTEST_OCDADJ                                      Fld(1, 24) //[24:24]
    #define LBTEST_RDCOLADR                                    Fld(1, 29) //[29:29]
    #define LBTEST_RDWRDATA                                    Fld(1, 30) //[30:30]
    #define LBTEST_RDTGDATA                                    Fld(1, 31) //[31:31]

#define DRAMC_REG_CATRAINING1                              (DRAMC_AO_BASE_ADDRESS + 0x000B0)
    #define CATRAINING1_CATRAINEN                              Fld(1, 1) //[1:1]
    #define CATRAINING1_CATRAINMRS                             Fld(1, 2) //[2:2]
    #define CATRAINING1_TESTCATRAIN                            Fld(1, 5) //[5:5]
    #define CATRAINING1_CSTRAIN_OPTION                         Fld(1, 6) //[6:6]
    #define CATRAINING1_CATRAINCSEXT                           Fld(1, 13) //[13:13]
    #define CATRAINING1_CATRAINLAT                             Fld(4, 20) //[23:20]
    #define CATRAINING1_CATRAIN_INTV                           Fld(8, 24) //[31:24]

#define DRAMC_REG_CATRAINING2                              (DRAMC_AO_BASE_ADDRESS + 0x000B4)
    #define CATRAINING2_CATRAINCA                              Fld(16, 0) //[15:0]
    #define CATRAINING2_CATRAINCA_Y                            Fld(16, 16) //[31:16]

#define DRAMC_REG_WRITE_LEV                                (DRAMC_AO_BASE_ADDRESS + 0x000BC)
    #define WRITE_LEV_WRITE_LEVEL_EN                           Fld(1, 0) //[0:0]
    #define WRITE_LEV_DDRPHY_COMB_CG_SEL                       Fld(1, 2) //[2:2]
    #define WRITE_LEV_BYTEMODECBTEN                            Fld(1, 3) //[3:3]
    #define WRITE_LEV_BTCBTFIXDQSOE                            Fld(1, 4) //[4:4]
    #define WRITE_LEV_CBTMASKDQSOE                             Fld(1, 5) //[5:5]
    #define WRITE_LEV_DQS_OE_WLEV_OP                           Fld(1, 6) //[6:6]
    #define WRITE_LEV_DQS_WLEV                                 Fld(1, 7) //[7:7]
    #define WRITE_LEV_DQSBX_G                                  Fld(4, 8) //[11:8]
    #define WRITE_LEV_DQSBY_G                                  Fld(4, 12) //[15:12]
    #define WRITE_LEV_DQS_SEL                                  Fld(4, 16) //[19:16]
    #define WRITE_LEV_DMVREFCA                                 Fld(8, 20) //[27:20]
    #define WRITE_LEV_DQS_OE_OP1_DIS                           Fld(1, 28) //[28:28]
    #define WRITE_LEV_DQS_OE_OP2_EN                            Fld(1, 29) //[29:29]

#define DRAMC_REG_MR_GOLDEN                                (DRAMC_AO_BASE_ADDRESS + 0x000C0)
    #define MR_GOLDEN_MR20_GOLDEN                              Fld(8, 0) //[7:0]
    #define MR_GOLDEN_MR15_GOLDEN                              Fld(8, 8) //[15:8]
    #define MR_GOLDEN_MR40_GOLDEN                              Fld(8, 16) //[23:16]
    #define MR_GOLDEN_MR32_GOLDEN                              Fld(8, 24) //[31:24]

#define DRAMC_REG_SLP4_TESTMODE                            (DRAMC_AO_BASE_ADDRESS + 0x000C4)
    #define SLP4_TESTMODE_CA0_TEST                             Fld(4, 0) //[3:0]
    #define SLP4_TESTMODE_CA1_TEST                             Fld(4, 4) //[7:4]
    #define SLP4_TESTMODE_CA2_TEST                             Fld(4, 8) //[11:8]
    #define SLP4_TESTMODE_CA3_TEST                             Fld(4, 12) //[15:12]
    #define SLP4_TESTMODE_CA4_TEST                             Fld(4, 16) //[19:16]
    #define SLP4_TESTMODE_CA5_TEST                             Fld(4, 20) //[23:20]
    #define SLP4_TESTMODE_STESTEN                              Fld(1, 24) //[24:24]
    #define SLP4_TESTMODE_SPEC_MODE                            Fld(1, 25) //[25:25]
    #define SLP4_TESTMODE_ARPI_CAL_E2OPT                       Fld(1, 26) //[26:26]
    #define SLP4_TESTMODE_TX_DLY_CAL_E2OPT                     Fld(1, 27) //[27:27]

#define DRAMC_REG_DQSOSCR                                  (DRAMC_AO_BASE_ADDRESS + 0x000C8)
    #define DQSOSCR_DQSOSC_INTEN                               Fld(1, 0) //[0:0]
    #define DQSOSCR_RK2_BYTE_MODE                              Fld(1, 1) //[1:1]
    #define DQSOSCR_TXUPD_BLOCK_SEL                            Fld(2, 2) //[3:2]
    #define DQSOSCR_TXUPD_BLOCK_OPT                            Fld(1, 4) //[4:4]
    #define DQSOSCR_TXUPDMODE                                  Fld(1, 5) //[5:5]
    #define DQSOSCR_MANUTXUPD                                  Fld(1, 6) //[6:6]
    #define DQSOSCR_ARUIDQ_SW                                  Fld(1, 7) //[7:7]
    #define DQSOSCR_DQS2DQ_UPD_BLOCK_CNT                       Fld(5, 8) //[12:8]
    #define DQSOSCR_TDQS2DQ_UPD_BLOCKING                       Fld(1, 13) //[13:13]
    #define DQSOSCR_DQS2DQ_UPD_MON_OPT                         Fld(1, 14) //[14:14]
    #define DQSOSCR_DQS2DQ_UPD_MON_CNT_SEL                     Fld(2, 15) //[16:15]
    #define DQSOSCR_TXUPD_IDLE_SEL                             Fld(2, 17) //[18:17]
    #define DQSOSCR_TXUPD_ABREF_SEL                            Fld(2, 19) //[20:19]
    #define DQSOSCR_TXUPD_IDLE_OPT                             Fld(1, 21) //[21:21]
    #define DQSOSCR_DQS2DQ_SHU_HW_CAL_DIS                      Fld(1, 22) //[22:22]
    #define DQSOSCR_SREF_TXUI_RELOAD_OPT                       Fld(1, 23) //[23:23]
    #define DQSOSCR_DQSOSCRDIS                                 Fld(1, 24) //[24:24]
    #define DQSOSCR_RK1_BYTE_MODE                              Fld(1, 25) //[25:25]
    #define DQSOSCR_RK0_BYTE_MODE                              Fld(1, 26) //[26:26]
    #define DQSOSCR_SREF_TXPI_RELOAD_OPT                       Fld(1, 27) //[27:27]
    #define DQSOSCR_EMPTY_WRITE_OPT                            Fld(1, 28) //[28:28]
    #define DQSOSCR_TXUPD_ABREF_OPT                            Fld(1, 29) //[29:29]
    #define DQSOSCR_DQSOSCLOPAD                                Fld(1, 30) //[30:30]
    #define DQSOSCR_DQSOSC_CALEN                               Fld(1, 31) //[31:31]

#define DRAMC_REG_DUMMY_RD                                 (DRAMC_AO_BASE_ADDRESS + 0x000D0)
    #define DUMMY_RD_SREF_DMYRD_MASK                           Fld(1, 0) //[0:0]
    #define DUMMY_RD_DMYRDOFOEN                                Fld(1, 1) //[1:1]
    #define DUMMY_RD_DUMMY_RD_SW                               Fld(1, 4) //[4:4]
    #define DUMMY_RD_DMYWR_LPRI_EN                             Fld(1, 5) //[5:5]
    #define DUMMY_RD_DMY_WR_DBG                                Fld(1, 6) //[6:6]
    #define DUMMY_RD_DMY_RD_DBG                                Fld(1, 7) //[7:7]
    #define DUMMY_RD_DUMMY_RD_CNT0                             Fld(1, 8) //[8:8]
    #define DUMMY_RD_DUMMY_RD_CNT1                             Fld(1, 9) //[9:9]
    #define DUMMY_RD_DUMMY_RD_CNT2                             Fld(1, 10) //[10:10]
    #define DUMMY_RD_DUMMY_RD_CNT3                             Fld(1, 11) //[11:11]
    #define DUMMY_RD_DUMMY_RD_CNT4                             Fld(1, 12) //[12:12]
    #define DUMMY_RD_DUMMY_RD_CNT5                             Fld(1, 13) //[13:13]
    #define DUMMY_RD_DUMMY_RD_CNT6                             Fld(1, 14) //[14:14]
    #define DUMMY_RD_DUMMY_RD_CNT7                             Fld(1, 15) //[15:15]
    #define DUMMY_RD_RANK_NUM                                  Fld(2, 16) //[17:16]
    #define DUMMY_RD_DUMMY_RD_EN                               Fld(1, 20) //[20:20]
    #define DUMMY_RD_SREF_DMYRD_EN                             Fld(1, 21) //[21:21]
    #define DUMMY_RD_DQSG_DMYRD_EN                             Fld(1, 22) //[22:22]
    #define DUMMY_RD_DQSG_DMYWR_EN                             Fld(1, 23) //[23:23]
    #define DUMMY_RD_DUMMY_RD_PA_OPT                           Fld(1, 24) //[24:24]
    #define DUMMY_RD_DMY_RD_RX_TRACK                           Fld(1, 25) //[25:25]
    #define DUMMY_RD_DMYRD_HPRI_DIS                            Fld(1, 26) //[26:26]
    #define DUMMY_RD_DMYRD_REORDER_DIS                         Fld(1, 27) //[27:27]

#define DRAMC_REG_SHUCTRL                                  (DRAMC_AO_BASE_ADDRESS + 0x000D4)
    #define SHUCTRL_R_SHUFFLE_BLOCK_OPT                        Fld(2, 0) //[1:0]
    #define SHUCTRL_DVFS_CG_OPT                                Fld(1, 2) //[2:2]
    #define SHUCTRL_VRCG_EN                                    Fld(1, 4) //[4:4]
    #define SHUCTRL_SHU_PHYRST_SEL                             Fld(1, 5) //[5:5]
    #define SHUCTRL_R_DVFS_PICG_MARGIN2                        Fld(2, 6) //[7:6]
    #define SHUCTRL_DMSHU_CNT                                  Fld(6, 8) //[13:8]
    #define SHUCTRL_SHUCTRL_RESERVED                           Fld(2, 14) //[15:14]
    #define SHUCTRL_LPSM_BYPASS_B                              Fld(1, 16) //[16:16]
    #define SHUCTRL_R_DRAMC_CHA                                Fld(1, 17) //[17:17]
    #define SHUCTRL_DVFS_CHB_SEL_B                             Fld(1, 18) //[18:18]
    #define SHUCTRL_R_NEW_SHU_MUX_SPM                          Fld(1, 19) //[19:19]
    #define SHUCTRL_R_MPDIV_SHU_GP                             Fld(3, 20) //[22:20]
    #define SHUCTRL_R_OTHER_SHU_GP                             Fld(2, 24) //[25:24]
    #define SHUCTRL_R_DVFS_PICG_MARGIN3                        Fld(2, 26) //[27:26]
    #define SHUCTRL_DMSHU_LOW                                  Fld(1, 29) //[29:29]
    #define SHUCTRL_DMSHU_DRAMC                                Fld(1, 31) //[31:31]

#define DRAMC_REG_SHUCTRL1                                 (DRAMC_AO_BASE_ADDRESS + 0x000D8)
    #define SHUCTRL1_FC_PRDCNT                                 Fld(8, 0) //[7:0]
    #define SHUCTRL1_CKFSPE_PRDCNT                             Fld(8, 8) //[15:8]
    #define SHUCTRL1_CKFSPX_PRDCNT                             Fld(8, 16) //[23:16]
    #define SHUCTRL1_VRCGEN_PRDCNT                             Fld(8, 24) //[31:24]

#define DRAMC_REG_SHUCTRL2                                 (DRAMC_AO_BASE_ADDRESS + 0x000DC)
    #define SHUCTRL2_R_DLL_IDLE                                Fld(7, 0) //[6:0]
    #define SHUCTRL2_R_DVFS_FSM_CLR                            Fld(1, 7) //[7:7]
    #define SHUCTRL2_R_DVFS_SREF_OPT                           Fld(1, 8) //[8:8]
    #define SHUCTRL2_R_DVFS_CDC_OPTION                         Fld(1, 9) //[9:9]
    #define SHUCTRL2_R_DVFS_PICG_MARGIN                        Fld(2, 10) //[11:10]
    #define SHUCTRL2_R_DVFS_DLL_CHA                            Fld(1, 12) //[12:12]
    #define SHUCTRL2_R_CDC_MUX_SEL_OPTION                      Fld(1, 13) //[13:13]
    #define SHUCTRL2_R_DVFS_PARK_N                             Fld(1, 14) //[14:14]
    #define SHUCTRL2_R_DVFS_OPTION                             Fld(1, 15) //[15:15]
    #define SHUCTRL2_SHU_PERIOD_GO_ZERO_CNT                    Fld(8, 16) //[23:16]
    #define SHUCTRL2_HWSET_WLRL                                Fld(1, 24) //[24:24]
    #define SHUCTRL2_MR13_SHU_EN                               Fld(1, 25) //[25:25]
    #define SHUCTRL2_R_DVFS_RG_CDC_TX_SEL                      Fld(1, 26) //[26:26]
    #define SHUCTRL2_R_DVFS_RG_CDC_SYNC_ENABLE                 Fld(1, 27) //[27:27]
    #define SHUCTRL2_R_SHU_RESTORE                             Fld(1, 28) //[28:28]
    #define SHUCTRL2_SHU_CLK_MASK                              Fld(1, 29) //[29:29]
    #define SHUCTRL2_DVFS_CKE_OPT                              Fld(1, 30) //[30:30]
    #define SHUCTRL2_SHORTQ_OPT                                Fld(1, 31) //[31:31]

#define DRAMC_REG_SHUCTRL3                                 (DRAMC_AO_BASE_ADDRESS + 0x000E0)
    #define SHUCTRL3_VRCGDIS_MRSMA                             Fld(13, 0) //[12:0]
    #define SHUCTRL3_VRCGDISOP                                 Fld(8, 16) //[23:16]
    #define SHUCTRL3_VRCGDIS_PRDCNT                            Fld(8, 24) //[31:24]

#define DRAMC_REG_SHUSTATUS                                (DRAMC_AO_BASE_ADDRESS + 0x000E4)
    #define SHUSTATUS_SHUFFLE_END                              Fld(1, 0) //[0:0]
    #define SHUSTATUS_SHUFFLE_START_LOW                        Fld(1, 1) //[1:1]
    #define SHUSTATUS_SHUFFLE_START_LOW_THREE                  Fld(1, 2) //[2:2]
    #define SHUSTATUS_MPDIV_SHU_GP                             Fld(3, 4) //[6:4]
    #define SHUSTATUS_SHUFFLE_LEVEL 				Fld(2, 1) //cc add

#define DRAMC_REG_BYPASS_FSPOP                             (DRAMC_AO_BASE_ADDRESS + 0x00100)
    #define BYPASS_FSPOP_BPFSP_SHU0                            Fld(4, 0) //[3:0]
    #define BYPASS_FSPOP_BPFSP_SHU1                            Fld(4, 4) //[7:4]
    #define BYPASS_FSPOP_BPFSP_SHU2                            Fld(4, 8) //[11:8]
    #define BYPASS_FSPOP_BPFSP_SHU3                            Fld(4, 12) //[15:12]
    #define BYPASS_FSPOP_BPFSP_OPT                             Fld(1, 16) //[16:16]

#define DRAMC_REG_STBCAL                                   (DRAMC_AO_BASE_ADDRESS + 0x00200)
    #define STBCAL_PIMASK_RKCHG_OPT                            Fld(1, 0) //[0:0]
    #define STBCAL_PIMASK_RKCHG_EXT                            Fld(3, 1) //[3:1]
    #define STBCAL_STBDLELAST_OPT                              Fld(1, 4) //[4:4]
    #define STBCAL_DLLFRZIDLE4XUPD                             Fld(1, 5) //[5:5]
    #define STBCAL_FASTDQSG2X                                  Fld(1, 6) //[6:6]
    #define STBCAL_FASTDQSGUPD                                 Fld(1, 7) //[7:7]
    #define STBCAL_STBDLELAST_PULSE                            Fld(4, 8) //[11:8]
    #define STBCAL_STBDLELAST_FILTER                           Fld(1, 12) //[12:12]
    #define STBCAL_STBUPDSTOP                                  Fld(1, 13) //[13:13]
    #define STBCAL_CG_RKEN                                     Fld(1, 14) //[14:14]
    #define STBCAL_STBSTATE_OPT                                Fld(1, 15) //[15:15]
    #define STBCAL_PHYVALID_IG                                 Fld(1, 16) //[16:16]
    #define STBCAL_SREF_DQSGUPD                                Fld(1, 17) //[17:17]
    #define STBCAL_STBCNTRST                                   Fld(1, 18) //[18:18]
    #define STBCAL_RKCHGMASKDIS                                Fld(1, 19) //[19:19]
    #define STBCAL_PICGEN                                      Fld(1, 20) //[20:20]
    #define STBCAL_REFUICHG                                    Fld(1, 21) //[21:21]
    #define STBCAL_STB_SELPHYCALEN                             Fld(1, 22) //[22:22]
    #define STBCAL_STBCAL2R                                    Fld(1, 23) //[23:23]
    #define STBCAL_STBCALEN                                    Fld(1, 24) //[24:24]
    #define STBCAL_STBDLYOUT_OPT                               Fld(1, 25) //[25:25]
    #define STBCAL_PICHGBLOCK_NORD                             Fld(1, 26) //[26:26]
    #define STBCAL_STB_DQIEN_IG                                Fld(1, 27) //[27:27]
    #define STBCAL_DQSIENCG_CHG_EN                             Fld(1, 28) //[28:28]
    #define STBCAL_DQSIENCG_NORMAL_EN                          Fld(1, 29) //[29:29]
    #define STBCAL_DQSIENMODE_SELPH                            Fld(1, 30) //[30:30]
    #define STBCAL_DQSIENMODE                                  Fld(1, 31) //[31:31]

#define DRAMC_REG_STBCAL1                                  (DRAMC_AO_BASE_ADDRESS + 0x00204)
    #define STBCAL1_DIS_PI_TRACK_AS_NOT_RD                     Fld(1, 2) //[2:2]
    #define STBCAL1_STBEN_LP3_DIV2_EN                          Fld(1, 3) //[3:3]
    #define STBCAL1_STBCNT_MODESEL                             Fld(1, 4) //[4:4]
    #define STBCAL1_DQSIEN_7UI_EN                              Fld(1, 5) //[5:5]
    #define STBCAL1_STB_SHIFT_DTCOUT_IG                        Fld(1, 6) //[6:6]
    #define STBCAL1_INPUTRXTRACK_BLOCK                         Fld(1, 7) //[7:7]
    #define STBCAL1_STB_FLAGCLR                                Fld(1, 8) //[8:8]
    #define STBCAL1_STB_DLLFRZ_IG                              Fld(1, 9) //[9:9]
    #define STBCAL1_STBENCMPEN                                 Fld(1, 10) //[10:10]
    #define STBCAL1_STBCNT_LATCH_EN                            Fld(1, 11) //[11:11]
    #define STBCAL1_DLLFRZ_MON_PBREF_OPT                       Fld(1, 12) //[12:12]
    #define STBCAL1_DLLFRZ_BLOCKLONG                           Fld(1, 13) //[13:13]
    #define STBCAL1_DQSERRCNT_DIS                              Fld(1, 14) //[14:14]
    #define STBCAL1_STBCNT_SW_RST                              Fld(1, 15) //[15:15]
    #define STBCAL1_STBCAL_FILTER                              Fld(16, 16) //[31:16]

#define DRAMC_REG_STBCAL2                                  (DRAMC_AO_BASE_ADDRESS + 0x00208)
    #define STBCAL2_STB_PIDLYCG_IG                             Fld(1, 0) //[0:0]
    #define STBCAL2_STB_UIDLYCG_IG                             Fld(1, 1) //[1:1]
    #define STBCAL2_STB_DBG_EN                                 Fld(4, 4) //[7:4]
    #define STBCAL2_STB_DBG_CG_AO                              Fld(1, 8) //[8:8]
    #define STBCAL2_STB_DBG_UIPI_UPD_OPT                       Fld(1, 9) //[9:9]
    #define STBCAL2_DQSGCNT_BYP_REF                            Fld(1, 10) //[10:10]
    #define STBCAL2_STB_DRS_MASK_HW_SAVE                       Fld(1, 12) //[12:12]
    #define STBCAL2_STB_DRS_RK1_FLAG_SYNC_RK0_EN               Fld(1, 13) //[13:13]
    #define STBCAL2_STB_PICG_EARLY_1T_EN                       Fld(1, 16) //[16:16]
    #define STBCAL2_STB_GERRSTOP                               Fld(1, 28) //[28:28]
    #define STBCAL2_STB_GERR_RST                               Fld(1, 29) //[29:29]
    #define STBCAL2_STB_GERR_B01                               Fld(1, 30) //[30:30]
    #define STBCAL2_STB_GERR_B23                               Fld(1, 31) //[31:31]
    #define STBCAL2_STB_DBG_EN_B1			Fld(1, 5)//[5:5]
    #define STBCAL2_STB_DBG_EN_B0			Fld(1, 4)//[4:4]

#define DRAMC_REG_EYESCAN                                  (DRAMC_AO_BASE_ADDRESS + 0x0020C)
    #define EYESCAN_REG_SW_RST                                 Fld(1, 0) //[0:0]
    #define EYESCAN_RG_RX_EYE_SCAN_EN                          Fld(1, 1) //[1:1]
    #define EYESCAN_RG_RX_MIOCK_JIT_EN                         Fld(1, 2) //[2:2]
    #define EYESCAN_EYESCAN_RD_SEL_OPT                         Fld(1, 4) //[4:4]
    #define EYESCAN_EYESCAN_CHK_OPT                            Fld(1, 6) //[6:6]
    #define EYESCAN_EYESCAN_TOG_OPT                            Fld(1, 7) //[7:7]
    #define EYESCAN_EYESCAN_DQ_SYNC_EN                         Fld(1, 8) //[8:8]
    #define EYESCAN_EYESCAN_NEW_DQ_SYNC_EN                     Fld(1, 9) //[9:9]
    #define EYESCAN_EYESCAN_DQS_SYNC_EN                        Fld(1, 10) //[10:10]
    #define EYESCAN_DCBLNCEN                                   Fld(1, 12) //[12:12]
    #define EYESCAN_DCBLNCINS                                  Fld(1, 13) //[13:13]
    #define EYESCAN_RX_DQ_EYE_SEL                              Fld(4, 16) //[19:16]
    #define EYESCAN_RX_DQ_EYE_SEL_B1                           Fld(4, 20) //[23:20]
    #define EYESCAN_RX_DQ_EYE_SEL_B2                           Fld(4, 24) //[27:24]
    #define EYESCAN_RX_DQ_EYE_SEL_B3                           Fld(4, 28) //[31:28]

#define DRAMC_REG_DVFSDLL                                  (DRAMC_AO_BASE_ADDRESS + 0x00210)
    #define DVFSDLL_DLL_LOCK_SHU_EN                            Fld(1, 0) //[0:0]
    #define DVFSDLL_R_BYPASS_1ST_DLL_SHU1                      Fld(1, 1) //[1:1]
    #define DVFSDLL_R_BYPASS_1ST_DLL_SHU2                      Fld(1, 2) //[2:2]
    #define DVFSDLL_R_BYPASS_1ST_DLL_SHU3                      Fld(1, 3) //[3:3]
    #define DVFSDLL_R_BYPASS_1ST_DLL_SHU4                      Fld(1, 4) //[4:4]
    #define DVFSDLL_R_DDRPHY_SHUFFLE_DEBUG_ENABLE              Fld(1, 5) //[5:5]
    #define DVFSDLL_R_RETRY_SAV_MSK                            Fld(1, 6) //[6:6]
    #define DVFSDLL_RG_DLL_SHUFFLE                             Fld(1, 7) //[7:7]
    #define DVFSDLL_DLL_IDLE_SHU2                              Fld(7, 8) //[14:8]
    #define DVFSDLL_DLL_IDLE_SHU3                              Fld(7, 16) //[22:16]
    #define DVFSDLL_R_DMSHUFFLE_CHANGE_FREQ_OPT                Fld(1, 24) //[24:24]
    #define DVFSDLL_R_DVFS_DLL_MARGIN                          Fld(2, 28) //[29:28]
    #define DVFSDLL_R_DVFS_SYNC_MODULE_RST_SEL                 Fld(1, 31) //[31:31]

#define DRAMC_REG_PRE_TDQSCK1                              (DRAMC_AO_BASE_ADDRESS + 0x00218)
    #define PRE_TDQSCK1_FREQ_RATIO_TX_9                        Fld(5, 0) //[4:0]
    #define PRE_TDQSCK1_FREQ_RATIO_TX_10                       Fld(5, 5) //[9:5]
    #define PRE_TDQSCK1_FREQ_RATIO_TX_11                       Fld(5, 10) //[14:10]
    #define PRE_TDQSCK1_TX_TRACKING_OPT                        Fld(1, 15) //[15:15]
    #define PRE_TDQSCK1_SW_UP_TX_NOW_CASE                      Fld(1, 16) //[16:16]
    #define PRE_TDQSCK1_TXUIPI_CAL_CGAR                        Fld(1, 17) //[17:17]
    #define PRE_TDQSCK1_SHU_PRELOAD_TX_START                   Fld(1, 18) //[18:18]
    #define PRE_TDQSCK1_SHU_PRELOAD_TX_HW                      Fld(1, 19) //[19:19]
    #define PRE_TDQSCK1_APHY_CG_OPT1                           Fld(1, 20) //[20:20]
    #define PRE_TDQSCK1_TDQSCK_HW_SW_UP_SEL                    Fld(1, 22) //[22:22]
    #define PRE_TDQSCK1_TDQSCK_SW_UP_CASE                      Fld(1, 23) //[23:23]
    #define PRE_TDQSCK1_TDQSCK_SW_SAVE                         Fld(1, 24) //[24:24]
    #define PRE_TDQSCK1_TDQSCK_REG_DVFS                        Fld(1, 25) //[25:25]
    #define PRE_TDQSCK1_TDQSCK_PRECAL_HW                       Fld(1, 26) //[26:26]
    #define PRE_TDQSCK1_TDQSCK_PRECAL_START                    Fld(1, 27) //[27:27]
    #define PRE_TDQSCK1_R_DQBUG_RANK_SEL                       Fld(2, 28) //[29:28]
    #define PRE_TDQSCK1_R_DQBUG_BYTE_SEL                       Fld(2, 30) //[31:30]

#define DRAMC_REG_PRE_TDQSCK2                              (DRAMC_AO_BASE_ADDRESS + 0x0021C)
    #define PRE_TDQSCK2_TDDQSCK_JUMP_RATIO3                    Fld(8, 0) //[7:0]
    #define PRE_TDQSCK2_TDDQSCK_JUMP_RATIO2                    Fld(8, 8) //[15:8]
    #define PRE_TDQSCK2_TDDQSCK_JUMP_RATIO1                    Fld(8, 16) //[23:16]
    #define PRE_TDQSCK2_TDDQSCK_JUMP_RATIO0                    Fld(8, 24) //[31:24]

#define DRAMC_REG_PRE_TDQSCK3                              (DRAMC_AO_BASE_ADDRESS + 0x00220)
    #define PRE_TDQSCK3_TDDQSCK_JUMP_RATIO7                    Fld(8, 0) //[7:0]
    #define PRE_TDQSCK3_TDDQSCK_JUMP_RATIO6                    Fld(8, 8) //[15:8]
    #define PRE_TDQSCK3_TDDQSCK_JUMP_RATIO5                    Fld(8, 16) //[23:16]
    #define PRE_TDQSCK3_TDDQSCK_JUMP_RATIO4                    Fld(8, 24) //[31:24]

#define DRAMC_REG_PRE_TDQSCK4                              (DRAMC_AO_BASE_ADDRESS + 0x00224)
    #define PRE_TDQSCK4_TDDQSCK_JUMP_RATIO11                   Fld(8, 0) //[7:0]
    #define PRE_TDQSCK4_TDDQSCK_JUMP_RATIO10                   Fld(8, 8) //[15:8]
    #define PRE_TDQSCK4_TDDQSCK_JUMP_RATIO9                    Fld(8, 16) //[23:16]
    #define PRE_TDQSCK4_TDDQSCK_JUMP_RATIO8                    Fld(8, 24) //[31:24]

#define DRAMC_REG_IMPCAL                                   (DRAMC_AO_BASE_ADDRESS + 0x0022C)
    #define IMPCAL_DRV_ECO_OPT                                 Fld(1, 10) //[10:10]
    #define IMPCAL_IMPCAL_CHGDRV_ECO_OPT                       Fld(1, 11) //[11:11]
    #define IMPCAL_IMPCAL_SM_ECO_OPT                           Fld(1, 12) //[12:12]
    #define IMPCAL_IMPCAL_ECO_OPT                              Fld(1, 13) //[13:13]
    #define IMPCAL_DIS_SUS_CH1_DRV                             Fld(1, 14) //[14:14]
    #define IMPCAL_DIS_SUS_CH0_DRV                             Fld(1, 15) //[15:15]
    #define IMPCAL_DIS_SHU_DRV                                 Fld(1, 16) //[16:16]
    #define IMPCAL_IMPCAL_DRVUPDOPT                            Fld(1, 17) //[17:17]
    #define IMPCAL_IMPCAL_USING_SYNC                           Fld(1, 18) //[18:18]
    #define IMPCAL_IMPCAL_BYPASS_UP_CA_DRV                     Fld(1, 19) //[19:19]
    #define IMPCAL_IMPCAL_HWSAVE_EN                            Fld(1, 20) //[20:20]
    #define IMPCAL_IMPCAL_CALI_ENN                             Fld(1, 21) //[21:21]
    #define IMPCAL_IMPCAL_CALI_ENP                             Fld(1, 22) //[22:22]
    #define IMPCAL_IMPCAL_CALI_EN                              Fld(1, 23) //[23:23]
    #define IMPCAL_IMPCAL_IMPPDN                               Fld(1, 24) //[24:24]
    #define IMPCAL_IMPCAL_IMPPDP                               Fld(1, 25) //[25:25]
    #define IMPCAL_IMPCAL_NEW_OLD_SL                           Fld(1, 26) //[26:26]
    #define IMPCAL_IMPCAL_CMP_DIREC                            Fld(2, 27) //[28:27]
    #define IMPCAL_IMPCAL_SWVALUE_EN                           Fld(1, 29) //[29:29]
    #define IMPCAL_IMPCAL_EN                                   Fld(1, 30) //[30:30]
    #define IMPCAL_IMPCAL_HW                                   Fld(1, 31) //[31:31]

#define DRAMC_REG_IMPEDAMCE_CTRL1                          (DRAMC_AO_BASE_ADDRESS + 0x00230)
    #define IMPEDAMCE_CTRL1_DQS1_OFF                           Fld(10, 0) //[9:0]
    #define IMPEDAMCE_CTRL1_DOS2_OFF                           Fld(10, 10) //[19:10]
    #define IMPEDAMCE_CTRL1_DQS1_OFF_SUB                       Fld(2, 28) //[29:28]
    #define IMPEDAMCE_CTRL1_DQS2_OFF_SUB                       Fld(2, 30) //[31:30]

#define DRAMC_REG_IMPEDAMCE_CTRL2                          (DRAMC_AO_BASE_ADDRESS + 0x00234)
    #define IMPEDAMCE_CTRL2_DQ1_OFF                            Fld(10, 0) //[9:0]
    #define IMPEDAMCE_CTRL2_DQ2_OFF                            Fld(10, 10) //[19:10]
    #define IMPEDAMCE_CTRL2_DQ1_OFF_SUB                        Fld(2, 28) //[29:28]
    #define IMPEDAMCE_CTRL2_DQ2_OFF_SUB                        Fld(2, 30) //[31:30]

#define DRAMC_REG_IMPEDAMCE_CTRL3                          (DRAMC_AO_BASE_ADDRESS + 0x00238)
    #define IMPEDAMCE_CTRL3_CMD1_OFF                           Fld(10, 0) //[9:0]
    #define IMPEDAMCE_CTRL3_CMD2_OFF                           Fld(10, 10) //[19:10]
    #define IMPEDAMCE_CTRL3_CMD1_OFF_SUB                       Fld(2, 28) //[29:28]
    #define IMPEDAMCE_CTRL3_CMD2_OFF_SUB                       Fld(2, 30) //[31:30]

#define DRAMC_REG_IMPEDAMCE_CTRL4                          (DRAMC_AO_BASE_ADDRESS + 0x0023C)
    #define IMPEDAMCE_CTRL4_DQC1_OFF                           Fld(10, 0) //[9:0]
    #define IMPEDAMCE_CTRL4_DQC2_OFF                           Fld(10, 10) //[19:10]
    #define IMPEDAMCE_CTRL4_DQC1_OFF_SUB                       Fld(2, 28) //[29:28]
    #define IMPEDAMCE_CTRL4_DQC2_OFF_SUB                       Fld(2, 30) //[31:30]

#define DRAMC_REG_DRAMC_DBG_SEL1                           (DRAMC_AO_BASE_ADDRESS + 0x00240)
    #define DRAMC_DBG_SEL1_DEBUG_SEL_0                         Fld(16, 0) //[15:0]
    #define DRAMC_DBG_SEL1_DEBUG_SEL_1                         Fld(16, 16) //[31:16]

#define DRAMC_REG_DRAMC_DBG_SEL2                           (DRAMC_AO_BASE_ADDRESS + 0x00244)
    #define DRAMC_DBG_SEL2_DEBUG_SEL_2                         Fld(16, 0) //[15:0]
    #define DRAMC_DBG_SEL2_DEBUG_SEL_3                         Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_DQSOSC                               (DRAMC_AO_BASE_ADDRESS + 0x00300)
    #define RK0_DQSOSC_R_DMDQS2DQ_FILT_OPT                     Fld(1, 29) //[29:29]
    #define RK0_DQSOSC_DQSOSCR_RK0EN                           Fld(1, 30) //[30:30]
    #define RK0_DQSOSC_DQSOSC_RK0INTCLR                        Fld(1, 31) //[31:31]

#define DRAMC_REG_RK0_DUMMY_RD_WDATA0                      (DRAMC_AO_BASE_ADDRESS + 0x00318)
    #define RK0_DUMMY_RD_WDATA0_DMY_RD_RK0_WDATA0              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_DUMMY_RD_WDATA1                      (DRAMC_AO_BASE_ADDRESS + 0x0031C)
    #define RK0_DUMMY_RD_WDATA1_DMY_RD_RK0_WDATA1              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_DUMMY_RD_WDATA2                      (DRAMC_AO_BASE_ADDRESS + 0x00320)
    #define RK0_DUMMY_RD_WDATA2_DMY_RD_RK0_WDATA2              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_DUMMY_RD_WDATA3                      (DRAMC_AO_BASE_ADDRESS + 0x00324)
    #define RK0_DUMMY_RD_WDATA3_DMY_RD_RK0_WDATA3              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_DUMMY_RD_ADR                         (DRAMC_AO_BASE_ADDRESS + 0x00328)
    #define RK0_DUMMY_RD_ADR_DMY_RD_RK0_ROW_ADR                Fld(17, 0) //[16:0]
    #define RK0_DUMMY_RD_ADR_DMY_RD_RK0_COL_ADR                Fld(11, 17) //[27:17]
    #define RK0_DUMMY_RD_ADR_DMY_RD_RK0_LEN                    Fld(4, 28) //[31:28]

#define DRAMC_REG_RK0_DUMMY_RD_BK                          (DRAMC_AO_BASE_ADDRESS + 0x0032C)
    #define RK0_DUMMY_RD_BK_DMY_RD_RK0_BK                      Fld(3, 0) //[2:0]
    #define RK0_DUMMY_RD_BK_DUMMY_RD_1_CNT0                    Fld(1, 4) //[4:4]
    #define RK0_DUMMY_RD_BK_DUMMY_RD_1_CNT1                    Fld(1, 5) //[5:5]
    #define RK0_DUMMY_RD_BK_DUMMY_RD_1_CNT2                    Fld(1, 6) //[6:6]
    #define RK0_DUMMY_RD_BK_DUMMY_RD_1_CNT3                    Fld(1, 7) //[7:7]
    #define RK0_DUMMY_RD_BK_DUMMY_RD_1_CNT4                    Fld(1, 8) //[8:8]
    #define RK0_DUMMY_RD_BK_DUMMY_RD_1_CNT5                    Fld(1, 9) //[9:9]
    #define RK0_DUMMY_RD_BK_DUMMY_RD_1_CNT6                    Fld(1, 10) //[10:10]
    #define RK0_DUMMY_RD_BK_DUMMY_RD_1_CNT7                    Fld(1, 11) //[11:11]

#define DRAMC_REG_RK0_PRE_TDQSCK1                          (DRAMC_AO_BASE_ADDRESS + 0x00330)
    #define RK0_PRE_TDQSCK1_TDQSCK_UIFREQ1_B0R0                Fld(6, 0) //[5:0]
    #define RK0_PRE_TDQSCK1_TDQSCK_PIFREQ1_B0R0                Fld(7, 6) //[12:6]
    #define RK0_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R0                Fld(6, 13) //[18:13]
    #define RK0_PRE_TDQSCK1_TDQSCK_PIFREQ2_B0R0                Fld(7, 19) //[25:19]

#define DRAMC_REG_RK0_PRE_TDQSCK2                          (DRAMC_AO_BASE_ADDRESS + 0x00334)
    #define RK0_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R0                Fld(6, 0) //[5:0]
    #define RK0_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R0                Fld(7, 6) //[12:6]
    #define RK0_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R0                Fld(6, 13) //[18:13]
    #define RK0_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R0                Fld(7, 19) //[25:19]

#define DRAMC_REG_RK0_PRE_TDQSCK3                          (DRAMC_AO_BASE_ADDRESS + 0x00338)
    #define RK0_PRE_TDQSCK3_TDQSCK_UIFREQ1_P1_B0R0             Fld(6, 0) //[5:0]
    #define RK0_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R0             Fld(6, 6) //[11:6]
    #define RK0_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R0             Fld(6, 12) //[17:12]
    #define RK0_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R0             Fld(6, 18) //[23:18]

#define DRAMC_REG_RK0_PRE_TDQSCK4                          (DRAMC_AO_BASE_ADDRESS + 0x0033C)
    #define RK0_PRE_TDQSCK4_TDQSCK_UIFREQ1_B1R0                Fld(6, 0) //[5:0]
    #define RK0_PRE_TDQSCK4_TDQSCK_PIFREQ1_B1R0                Fld(7, 6) //[12:6]
    #define RK0_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R0                Fld(6, 13) //[18:13]
    #define RK0_PRE_TDQSCK4_TDQSCK_PIFREQ2_B1R0                Fld(7, 19) //[25:19]

#define DRAMC_REG_RK0_PRE_TDQSCK5                          (DRAMC_AO_BASE_ADDRESS + 0x00340)
    #define RK0_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R0                Fld(6, 0) //[5:0]
    #define RK0_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R0                Fld(7, 6) //[12:6]
    #define RK0_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R0                Fld(6, 13) //[18:13]
    #define RK0_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R0                Fld(7, 19) //[25:19]

#define DRAMC_REG_RK0_PRE_TDQSCK6                          (DRAMC_AO_BASE_ADDRESS + 0x00344)
    #define RK0_PRE_TDQSCK6_TDQSCK_UIFREQ1_P1_B1R0             Fld(6, 0) //[5:0]
    #define RK0_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R0             Fld(6, 6) //[11:6]
    #define RK0_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R0             Fld(6, 12) //[17:12]
    #define RK0_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R0             Fld(6, 18) //[23:18]

#define DRAMC_REG_RK0_PRE_TDQSCK7                          (DRAMC_AO_BASE_ADDRESS + 0x00348)
    #define RK0_PRE_TDQSCK7_TDQSCK_UIFREQ1_B2R0                Fld(6, 0) //[5:0]
    #define RK0_PRE_TDQSCK7_TDQSCK_PIFREQ1_B2R0                Fld(7, 6) //[12:6]
    #define RK0_PRE_TDQSCK7_TDQSCK_UIFREQ2_B2R0                Fld(6, 13) //[18:13]
    #define RK0_PRE_TDQSCK7_TDQSCK_PIFREQ2_B2R0                Fld(7, 19) //[25:19]

#define DRAMC_REG_RK0_PRE_TDQSCK8                          (DRAMC_AO_BASE_ADDRESS + 0x0034C)
    #define RK0_PRE_TDQSCK8_TDQSCK_UIFREQ3_B2R0                Fld(6, 0) //[5:0]
    #define RK0_PRE_TDQSCK8_TDQSCK_PIFREQ3_B2R0                Fld(7, 6) //[12:6]
    #define RK0_PRE_TDQSCK8_TDQSCK_UIFREQ4_B2R0                Fld(6, 13) //[18:13]
    #define RK0_PRE_TDQSCK8_TDQSCK_PIFREQ4_B2R0                Fld(7, 19) //[25:19]

#define DRAMC_REG_RK0_PRE_TDQSCK9                          (DRAMC_AO_BASE_ADDRESS + 0x00350)
    #define RK0_PRE_TDQSCK9_TDQSCK_UIFREQ1_P1_B2R0             Fld(6, 0) //[5:0]
    #define RK0_PRE_TDQSCK9_TDQSCK_UIFREQ2_P1_B2R0             Fld(6, 6) //[11:6]
    #define RK0_PRE_TDQSCK9_TDQSCK_UIFREQ3_P1_B2R0             Fld(6, 12) //[17:12]
    #define RK0_PRE_TDQSCK9_TDQSCK_UIFREQ4_P1_B2R0             Fld(6, 18) //[23:18]

#define DRAMC_REG_RK0_PRE_TDQSCK10                         (DRAMC_AO_BASE_ADDRESS + 0x00354)
    #define RK0_PRE_TDQSCK10_TDQSCK_UIFREQ1_B3R0               Fld(6, 0) //[5:0]
    #define RK0_PRE_TDQSCK10_TDQSCK_PIFREQ1_B3R0               Fld(7, 6) //[12:6]
    #define RK0_PRE_TDQSCK10_TDQSCK_UIFREQ2_B3R0               Fld(6, 13) //[18:13]
    #define RK0_PRE_TDQSCK10_TDQSCK_PIFREQ2_B3R0               Fld(7, 19) //[25:19]

#define DRAMC_REG_RK0_PRE_TDQSCK11                         (DRAMC_AO_BASE_ADDRESS + 0x00358)
    #define RK0_PRE_TDQSCK11_TDQSCK_UIFREQ3_B3R0               Fld(6, 0) //[5:0]
    #define RK0_PRE_TDQSCK11_TDQSCK_PIFREQ3_B3R0               Fld(7, 6) //[12:6]
    #define RK0_PRE_TDQSCK11_TDQSCK_UIFREQ4_B3R0               Fld(6, 13) //[18:13]
    #define RK0_PRE_TDQSCK11_TDQSCK_PIFREQ4_B3R0               Fld(7, 19) //[25:19]

#define DRAMC_REG_RK0_PRE_TDQSCK12                         (DRAMC_AO_BASE_ADDRESS + 0x0035C)
    #define RK0_PRE_TDQSCK12_TDQSCK_UIFREQ1_P1_B3R0            Fld(6, 0) //[5:0]
    #define RK0_PRE_TDQSCK12_TDQSCK_UIFREQ2_P1_B3R0            Fld(6, 6) //[11:6]
    #define RK0_PRE_TDQSCK12_TDQSCK_UIFREQ3_P1_B3R0            Fld(6, 12) //[17:12]
    #define RK0_PRE_TDQSCK12_TDQSCK_UIFREQ4_P1_B3R0            Fld(6, 18) //[23:18]

#define DRAMC_REG_RK1_DQSOSC                               (DRAMC_AO_BASE_ADDRESS + 0x00400)
    #define RK1_DQSOSC_DQSOSCR_RK1EN                           Fld(1, 30) //[30:30]
    #define RK1_DQSOSC_DQSOSC_RK1INTCLR                        Fld(1, 31) //[31:31]

#define DRAMC_REG_RK1_DUMMY_RD_WDATA0                      (DRAMC_AO_BASE_ADDRESS + 0x00418)
    #define RK1_DUMMY_RD_WDATA0_DMY_RD_RK1_WDATA0              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_DUMMY_RD_WDATA1                      (DRAMC_AO_BASE_ADDRESS + 0x0041C)
    #define RK1_DUMMY_RD_WDATA1_DMY_RD_RK1_WDATA1              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_DUMMY_RD_WDATA2                      (DRAMC_AO_BASE_ADDRESS + 0x00420)
    #define RK1_DUMMY_RD_WDATA2_DMY_RD_RK1_WDATA2              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_DUMMY_RD_WDATA3                      (DRAMC_AO_BASE_ADDRESS + 0x00424)
    #define RK1_DUMMY_RD_WDATA3_DMY_RD_RK1_WDATA3              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_DUMMY_RD_ADR                         (DRAMC_AO_BASE_ADDRESS + 0x00428)
    #define RK1_DUMMY_RD_ADR_DMY_RD_RK1_ROW_ADR                Fld(17, 0) //[16:0]
    #define RK1_DUMMY_RD_ADR_DMY_RD_RK1_COL_ADR                Fld(11, 17) //[27:17]
    #define RK1_DUMMY_RD_ADR_DMY_RD_RK1_LEN                    Fld(4, 28) //[31:28]

#define DRAMC_REG_RK1_DUMMY_RD_BK                          (DRAMC_AO_BASE_ADDRESS + 0x0042C)
    #define RK1_DUMMY_RD_BK_DMY_RD_RK1_BK                      Fld(3, 0) //[2:0]

#define DRAMC_REG_RK1_PRE_TDQSCK1                          (DRAMC_AO_BASE_ADDRESS + 0x00430)
    #define RK1_PRE_TDQSCK1_TDQSCK_UIFREQ1_B0R1                Fld(6, 0) //[5:0]
    #define RK1_PRE_TDQSCK1_TDQSCK_PIFREQ1_B0R1                Fld(7, 6) //[12:6]
    #define RK1_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R1                Fld(6, 13) //[18:13]
    #define RK1_PRE_TDQSCK1_TDQSCK_PIFREQ2_B0R1                Fld(7, 19) //[25:19]

#define DRAMC_REG_RK1_PRE_TDQSCK2                          (DRAMC_AO_BASE_ADDRESS + 0x00434)
    #define RK1_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R1                Fld(6, 0) //[5:0]
    #define RK1_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R1                Fld(7, 6) //[12:6]
    #define RK1_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R1                Fld(6, 13) //[18:13]
    #define RK1_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R1                Fld(7, 19) //[25:19]

#define DRAMC_REG_RK1_PRE_TDQSCK3                          (DRAMC_AO_BASE_ADDRESS + 0x00438)
    #define RK1_PRE_TDQSCK3_TDQSCK_UIFREQ1_P1_B0R1             Fld(6, 0) //[5:0]
    #define RK1_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R1             Fld(6, 6) //[11:6]
    #define RK1_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R1             Fld(6, 12) //[17:12]
    #define RK1_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R1             Fld(6, 18) //[23:18]

#define DRAMC_REG_RK1_PRE_TDQSCK4                          (DRAMC_AO_BASE_ADDRESS + 0x0043C)
    #define RK1_PRE_TDQSCK4_TDQSCK_UIFREQ1_B1R1                Fld(6, 0) //[5:0]
    #define RK1_PRE_TDQSCK4_TDQSCK_PIFREQ1_B1R1                Fld(7, 6) //[12:6]
    #define RK1_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R1                Fld(6, 13) //[18:13]
    #define RK1_PRE_TDQSCK4_TDQSCK_PIFREQ2_B1R1                Fld(7, 19) //[25:19]

#define DRAMC_REG_RK1_PRE_TDQSCK5                          (DRAMC_AO_BASE_ADDRESS + 0x00440)
    #define RK1_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R1                Fld(6, 0) //[5:0]
    #define RK1_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R1                Fld(7, 6) //[12:6]
    #define RK1_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R1                Fld(6, 13) //[18:13]
    #define RK1_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R1                Fld(7, 19) //[25:19]

#define DRAMC_REG_RK1_PRE_TDQSCK6                          (DRAMC_AO_BASE_ADDRESS + 0x00444)
    #define RK1_PRE_TDQSCK6_TDQSCK_UIFREQ1_P1_B1R1             Fld(6, 0) //[5:0]
    #define RK1_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R1             Fld(6, 6) //[11:6]
    #define RK1_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R1             Fld(6, 12) //[17:12]
    #define RK1_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R1             Fld(6, 18) //[23:18]

#define DRAMC_REG_RK1_PRE_TDQSCK7                          (DRAMC_AO_BASE_ADDRESS + 0x00448)
    #define RK1_PRE_TDQSCK7_TDQSCK_UIFREQ1_B2R1                Fld(6, 0) //[5:0]
    #define RK1_PRE_TDQSCK7_TDQSCK_PIFREQ1_B2R1                Fld(7, 6) //[12:6]
    #define RK1_PRE_TDQSCK7_TDQSCK_UIFREQ2_B2R1                Fld(6, 13) //[18:13]
    #define RK1_PRE_TDQSCK7_TDQSCK_PIFREQ2_B2R1                Fld(7, 19) //[25:19]

#define DRAMC_REG_RK1_PRE_TDQSCK8                          (DRAMC_AO_BASE_ADDRESS + 0x0044C)
    #define RK1_PRE_TDQSCK8_TDQSCK_UIFREQ3_B2R1                Fld(6, 0) //[5:0]
    #define RK1_PRE_TDQSCK8_TDQSCK_PIFREQ3_B2R1                Fld(7, 6) //[12:6]
    #define RK1_PRE_TDQSCK8_TDQSCK_UIFREQ4_B2R1                Fld(6, 13) //[18:13]
    #define RK1_PRE_TDQSCK8_TDQSCK_PIFREQ4_B2R1                Fld(7, 19) //[25:19]

#define DRAMC_REG_RK1_PRE_TDQSCK9                          (DRAMC_AO_BASE_ADDRESS + 0x00450)
    #define RK1_PRE_TDQSCK9_TDQSCK_UIFREQ1_P1_B2R1             Fld(6, 0) //[5:0]
    #define RK1_PRE_TDQSCK9_TDQSCK_UIFREQ2_P1_B2R1             Fld(6, 6) //[11:6]
    #define RK1_PRE_TDQSCK9_TDQSCK_UIFREQ3_P1_B2R1             Fld(6, 12) //[17:12]
    #define RK1_PRE_TDQSCK9_TDQSCK_UIFREQ4_P1_B2R1             Fld(6, 18) //[23:18]

#define DRAMC_REG_RK1_PRE_TDQSCK10                         (DRAMC_AO_BASE_ADDRESS + 0x00454)
    #define RK1_PRE_TDQSCK10_TDQSCK_UIFREQ1_B3R1               Fld(6, 0) //[5:0]
    #define RK1_PRE_TDQSCK10_TDQSCK_PIFREQ1_B3R1               Fld(7, 6) //[12:6]
    #define RK1_PRE_TDQSCK10_TDQSCK_UIFREQ2_B3R1               Fld(6, 13) //[18:13]
    #define RK1_PRE_TDQSCK10_TDQSCK_PIFREQ2_B3R1               Fld(7, 19) //[25:19]

#define DRAMC_REG_RK1_PRE_TDQSCK11                         (DRAMC_AO_BASE_ADDRESS + 0x00458)
    #define RK1_PRE_TDQSCK11_TDQSCK_UIFREQ3_B3R1               Fld(6, 0) //[5:0]
    #define RK1_PRE_TDQSCK11_TDQSCK_PIFREQ3_B3R1               Fld(7, 6) //[12:6]
    #define RK1_PRE_TDQSCK11_TDQSCK_UIFREQ4_B3R1               Fld(6, 13) //[18:13]
    #define RK1_PRE_TDQSCK11_TDQSCK_PIFREQ4_B3R1               Fld(7, 19) //[25:19]

#define DRAMC_REG_RK1_PRE_TDQSCK12                         (DRAMC_AO_BASE_ADDRESS + 0x0045C)
    #define RK1_PRE_TDQSCK12_TDQSCK_UIFREQ1_P1_B3R1            Fld(6, 0) //[5:0]
    #define RK1_PRE_TDQSCK12_TDQSCK_UIFREQ2_P1_B3R1            Fld(6, 6) //[11:6]
    #define RK1_PRE_TDQSCK12_TDQSCK_UIFREQ3_P1_B3R1            Fld(6, 12) //[17:12]
    #define RK1_PRE_TDQSCK12_TDQSCK_UIFREQ4_P1_B3R1            Fld(6, 18) //[23:18]

#define DRAMC_REG_RK2_DQSOSC                               (DRAMC_AO_BASE_ADDRESS + 0x00500)
    #define RK2_DQSOSC_FREQ_RATIO_TX_0                         Fld(5, 0) //[4:0]
    #define RK2_DQSOSC_FREQ_RATIO_TX_1                         Fld(5, 5) //[9:5]
    #define RK2_DQSOSC_FREQ_RATIO_TX_2                         Fld(5, 10) //[14:10]
    #define RK2_DQSOSC_FREQ_RATIO_TX_3                         Fld(5, 15) //[19:15]
    #define RK2_DQSOSC_FREQ_RATIO_TX_4                         Fld(5, 20) //[24:20]
    #define RK2_DQSOSC_FREQ_RATIO_TX_5                         Fld(5, 25) //[29:25]
    #define RK2_DQSOSC_DQSOSCR_RK2EN                           Fld(1, 30) //[30:30]
    #define RK2_DQSOSC_DQSOSC_RK2INTCLR                        Fld(1, 31) //[31:31]

#define DRAMC_REG_RK2_DUMMY_RD_WDATA0                      (DRAMC_AO_BASE_ADDRESS + 0x00518)
    #define RK2_DUMMY_RD_WDATA0_DMY_RD_RK2_WDATA0              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK2_DUMMY_RD_WDATA1                      (DRAMC_AO_BASE_ADDRESS + 0x0051C)
    #define RK2_DUMMY_RD_WDATA1_DMY_RD_RK2_WDATA1              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK2_DUMMY_RD_WDATA2                      (DRAMC_AO_BASE_ADDRESS + 0x00520)
    #define RK2_DUMMY_RD_WDATA2_DMY_RD_RK2_WDATA2              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK2_DUMMY_RD_WDATA3                      (DRAMC_AO_BASE_ADDRESS + 0x00524)
    #define RK2_DUMMY_RD_WDATA3_DMY_RD_RK2_WDATA3              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK2_DUMMY_RD_ADR                         (DRAMC_AO_BASE_ADDRESS + 0x00528)
    #define RK2_DUMMY_RD_ADR_DMY_RD_RK2_ROW_ADR                Fld(17, 0) //[16:0]
    #define RK2_DUMMY_RD_ADR_DMY_RD_RK2_COL_ADR                Fld(11, 17) //[27:17]
    #define RK2_DUMMY_RD_ADR_DMY_RD_RK2_LEN                    Fld(4, 28) //[31:28]

#define DRAMC_REG_RK2_DUMMY_RD_BK                          (DRAMC_AO_BASE_ADDRESS + 0x0052C)
    #define RK2_DUMMY_RD_BK_DMY_RD_RK2_BK                      Fld(3, 0) //[2:0]
    #define RK2_DUMMY_RD_BK_FREQ_RATIO_TX_6                    Fld(5, 3) //[7:3]
    #define RK2_DUMMY_RD_BK_FREQ_RATIO_TX_7                    Fld(5, 8) //[12:8]
    #define RK2_DUMMY_RD_BK_FREQ_RATIO_TX_8                    Fld(5, 13) //[17:13]

#define DRAMC_REG_RK2_PRE_TDQSCK1                          (DRAMC_AO_BASE_ADDRESS + 0x00530)
    #define RK2_PRE_TDQSCK1_TDQSCK_UIFREQ1_B0R2                Fld(6, 0) //[5:0]
    #define RK2_PRE_TDQSCK1_TDQSCK_PIFREQ1_B0R2                Fld(7, 6) //[12:6]
    #define RK2_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R2                Fld(6, 13) //[18:13]
    #define RK2_PRE_TDQSCK1_TDQSCK_PIFREQ2_B0R2                Fld(7, 19) //[25:19]

#define DRAMC_REG_RK2_PRE_TDQSCK2                          (DRAMC_AO_BASE_ADDRESS + 0x00534)
    #define RK2_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R2                Fld(6, 0) //[5:0]
    #define RK2_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R2                Fld(7, 6) //[12:6]
    #define RK2_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R2                Fld(6, 13) //[18:13]
    #define RK2_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R2                Fld(7, 19) //[25:19]

#define DRAMC_REG_RK2_PRE_TDQSCK3                          (DRAMC_AO_BASE_ADDRESS + 0x00538)
    #define RK2_PRE_TDQSCK3_TDQSCK_UIFREQ1_P1_B0R2             Fld(6, 0) //[5:0]
    #define RK2_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R2             Fld(6, 6) //[11:6]
    #define RK2_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R2             Fld(6, 12) //[17:12]
    #define RK2_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R2             Fld(6, 18) //[23:18]

#define DRAMC_REG_RK2_PRE_TDQSCK4                          (DRAMC_AO_BASE_ADDRESS + 0x0053C)
    #define RK2_PRE_TDQSCK4_TDQSCK_UIFREQ1_B1R2                Fld(6, 0) //[5:0]
    #define RK2_PRE_TDQSCK4_TDQSCK_PIFREQ1_B1R2                Fld(7, 6) //[12:6]
    #define RK2_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R2                Fld(6, 13) //[18:13]
    #define RK2_PRE_TDQSCK4_TDQSCK_PIFREQ2_B1R2                Fld(7, 19) //[25:19]

#define DRAMC_REG_RK2_PRE_TDQSCK5                          (DRAMC_AO_BASE_ADDRESS + 0x00540)
    #define RK2_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R2                Fld(6, 0) //[5:0]
    #define RK2_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R2                Fld(7, 6) //[12:6]
    #define RK2_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R2                Fld(6, 13) //[18:13]
    #define RK2_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R2                Fld(7, 19) //[25:19]

#define DRAMC_REG_RK2_PRE_TDQSCK6                          (DRAMC_AO_BASE_ADDRESS + 0x00544)
    #define RK2_PRE_TDQSCK6_TDQSCK_UIFREQ1_P1_B1R2             Fld(6, 0) //[5:0]
    #define RK2_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R2             Fld(6, 6) //[11:6]
    #define RK2_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R2             Fld(6, 12) //[17:12]
    #define RK2_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R2             Fld(6, 18) //[23:18]

#define DRAMC_REG_RK2_PRE_TDQSCK7                          (DRAMC_AO_BASE_ADDRESS + 0x00548)
    #define RK2_PRE_TDQSCK7_TDQSCK_UIFREQ1_B2R2                Fld(6, 0) //[5:0]
    #define RK2_PRE_TDQSCK7_TDQSCK_PIFREQ1_B2R2                Fld(7, 6) //[12:6]
    #define RK2_PRE_TDQSCK7_TDQSCK_UIFREQ2_B2R2                Fld(6, 13) //[18:13]
    #define RK2_PRE_TDQSCK7_TDQSCK_PIFREQ2_B2R2                Fld(7, 19) //[25:19]

#define DRAMC_REG_RK2_PRE_TDQSCK8                          (DRAMC_AO_BASE_ADDRESS + 0x0054C)
    #define RK2_PRE_TDQSCK8_TDQSCK_UIFREQ3_B2R2                Fld(6, 0) //[5:0]
    #define RK2_PRE_TDQSCK8_TDQSCK_PIFREQ3_B2R2                Fld(7, 6) //[12:6]
    #define RK2_PRE_TDQSCK8_TDQSCK_UIFREQ4_B2R2                Fld(6, 13) //[18:13]
    #define RK2_PRE_TDQSCK8_TDQSCK_PIFREQ4_B2R2                Fld(7, 19) //[25:19]

#define DRAMC_REG_RK2_PRE_TDQSCK9                          (DRAMC_AO_BASE_ADDRESS + 0x00550)
    #define RK2_PRE_TDQSCK9_TDQSCK_UIFREQ1_P1_B2R2             Fld(6, 0) //[5:0]
    #define RK2_PRE_TDQSCK9_TDQSCK_UIFREQ2_P1_B2R2             Fld(6, 6) //[11:6]
    #define RK2_PRE_TDQSCK9_TDQSCK_UIFREQ3_P1_B2R2             Fld(6, 12) //[17:12]
    #define RK2_PRE_TDQSCK9_TDQSCK_UIFREQ4_P1_B2R2             Fld(6, 18) //[23:18]

#define DRAMC_REG_RK2_PRE_TDQSCK10                         (DRAMC_AO_BASE_ADDRESS + 0x00554)
    #define RK2_PRE_TDQSCK10_TDQSCK_UIFREQ1_B3R2               Fld(6, 0) //[5:0]
    #define RK2_PRE_TDQSCK10_TDQSCK_PIFREQ1_B3R2               Fld(7, 6) //[12:6]
    #define RK2_PRE_TDQSCK10_TDQSCK_UIFREQ2_B3R2               Fld(6, 13) //[18:13]
    #define RK2_PRE_TDQSCK10_TDQSCK_PIFREQ2_B3R2               Fld(7, 19) //[25:19]

#define DRAMC_REG_RK2_PRE_TDQSCK11                         (DRAMC_AO_BASE_ADDRESS + 0x00558)
    #define RK2_PRE_TDQSCK11_TDQSCK_UIFREQ3_B3R2               Fld(6, 0) //[5:0]
    #define RK2_PRE_TDQSCK11_TDQSCK_PIFREQ3_B3R2               Fld(7, 6) //[12:6]
    #define RK2_PRE_TDQSCK11_TDQSCK_UIFREQ4_B3R2               Fld(6, 13) //[18:13]
    #define RK2_PRE_TDQSCK11_TDQSCK_PIFREQ4_B3R2               Fld(7, 19) //[25:19]

#define DRAMC_REG_RK2_PRE_TDQSCK12                         (DRAMC_AO_BASE_ADDRESS + 0x0055C)
    #define RK2_PRE_TDQSCK12_TDQSCK_UIFREQ1_P1_B3R2            Fld(6, 0) //[5:0]
    #define RK2_PRE_TDQSCK12_TDQSCK_UIFREQ2_P1_B3R2            Fld(6, 6) //[11:6]
    #define RK2_PRE_TDQSCK12_TDQSCK_UIFREQ3_P1_B3R2            Fld(6, 12) //[17:12]
    #define RK2_PRE_TDQSCK12_TDQSCK_UIFREQ4_P1_B3R2            Fld(6, 18) //[23:18]

#define DRAMC_REG_SHU_ACTIM0                               (DRAMC_AO_BASE_ADDRESS + 0x00800)
    #define SHU_ACTIM0_TWTR                                    Fld(4, 0) //[3:0]
    #define SHU_ACTIM0_TWR                                     Fld(5, 8) //[12:8]
    #define SHU_ACTIM0_TRRD                                    Fld(3, 16) //[18:16]
    #define SHU_ACTIM0_TRCD                                    Fld(4, 24) //[27:24]

#define DRAMC_REG_SHU_ACTIM1                               (DRAMC_AO_BASE_ADDRESS + 0x00804)
    #define SHU_ACTIM1_TRPAB                                   Fld(3, 0) //[2:0]
    #define SHU_ACTIM1_TRP                                     Fld(4, 8) //[11:8]
    #define SHU_ACTIM1_TRAS                                    Fld(4, 16) //[19:16]
    #define SHU_ACTIM1_TRC                                     Fld(5, 24) //[28:24]

#define DRAMC_REG_SHU_ACTIM2                               (DRAMC_AO_BASE_ADDRESS + 0x00808)
    #define SHU_ACTIM2_TXP                                     Fld(3, 0) //[2:0]
    #define SHU_ACTIM2_TRTP                                    Fld(3, 8) //[10:8]
    #define SHU_ACTIM2_TR2W                                    Fld(4, 16) //[19:16]
    #define SHU_ACTIM2_TFAW                                    Fld(5, 24) //[28:24]

#define DRAMC_REG_SHU_ACTIM3                               (DRAMC_AO_BASE_ADDRESS + 0x0080C)
    #define SHU_ACTIM3_TRFCPB                                  Fld(8, 0) //[7:0]
    #define SHU_ACTIM3_TRFC                                    Fld(8, 16) //[23:16]
    #define SHU_ACTIM3_REFCNT                                  Fld(8, 24) //[31:24]

#define DRAMC_REG_SHU_ACTIM4                               (DRAMC_AO_BASE_ADDRESS + 0x00810)
    #define SHU_ACTIM4_TXREFCNT                                Fld(10, 0) //[9:0]
    #define SHU_ACTIM4_REFCNT_FR_CLK                           Fld(8, 16) //[23:16]
    #define SHU_ACTIM4_TZQCS                                   Fld(8, 24) //[31:24]

#define DRAMC_REG_SHU_ACTIM5                               (DRAMC_AO_BASE_ADDRESS + 0x00814)
    #define SHU_ACTIM5_TR2PD                                   Fld(5, 0) //[4:0]
    #define SHU_ACTIM5_TWTPD                                   Fld(5, 8) //[12:8]
    #define SHU_ACTIM5_TMRR2W                                  Fld(4, 24) //[27:24]

#define DRAMC_REG_SHU_ACTIM6                               (DRAMC_AO_BASE_ADDRESS + 0x00818)
    #define SHU_ACTIM6_BGTCCD                                  Fld(2, 0) //[1:0]
    #define SHU_ACTIM6_BGTWTR                                  Fld(4, 4) //[7:4]
    #define SHU_ACTIM6_TWRMPR                                  Fld(4, 8) //[11:8]
    #define SHU_ACTIM6_BGTRRD                                  Fld(3, 12) //[14:12]

#define DRAMC_REG_SHU_ACTIM_XRT                            (DRAMC_AO_BASE_ADDRESS + 0x0081C)
    #define SHU_ACTIM_XRT_XRTR2R                               Fld(5, 0) //[4:0]
    #define SHU_ACTIM_XRT_XRTR2W                               Fld(4, 8) //[11:8]
    #define SHU_ACTIM_XRT_XRTW2R                               Fld(3, 16) //[18:16]
    #define SHU_ACTIM_XRT_XRTW2W                               Fld(4, 24) //[27:24]

#define DRAMC_REG_SHU_AC_TIME_05T                          (DRAMC_AO_BASE_ADDRESS + 0x00820)
    #define SHU_AC_TIME_05T_TRC_05T                            Fld(1, 0) //[0:0]
    #define SHU_AC_TIME_05T_TRFCPB_05T                         Fld(1, 1) //[1:1]
    #define SHU_AC_TIME_05T_TRFC_05T                           Fld(1, 2) //[2:2]
    #define SHU_AC_TIME_05T_TXP_05T                            Fld(1, 4) //[4:4]
    #define SHU_AC_TIME_05T_TRTP_05T                           Fld(1, 5) //[5:5]
    #define SHU_AC_TIME_05T_TRCD_05T                           Fld(1, 6) //[6:6]
    #define SHU_AC_TIME_05T_TRP_05T                            Fld(1, 7) //[7:7]
    #define SHU_AC_TIME_05T_TRPAB_05T                          Fld(1, 8) //[8:8]
    #define SHU_AC_TIME_05T_TRAS_05T                           Fld(1, 9) //[9:9]
    #define SHU_AC_TIME_05T_TWR_M05T                           Fld(1, 10) //[10:10]
    #define SHU_AC_TIME_05T_TRRD_05T                           Fld(1, 12) //[12:12]
    #define SHU_AC_TIME_05T_TFAW_05T                           Fld(1, 13) //[13:13]
    #define SHU_AC_TIME_05T_TR2PD_05T                          Fld(1, 15) //[15:15]
    #define SHU_AC_TIME_05T_TWTPD_M05T                         Fld(1, 16) //[16:16]
    #define SHU_AC_TIME_05T_BGTRRD_05T                         Fld(1, 21) //[21:21]
    #define SHU_AC_TIME_05T_BGTCCD_05T                         Fld(1, 22) //[22:22]
    #define SHU_AC_TIME_05T_BGTWTR_05T                         Fld(1, 23) //[23:23]
    #define SHU_AC_TIME_05T_TR2W_05T                           Fld(1, 24) //[24:24]
    #define SHU_AC_TIME_05T_TWTR_M05T                          Fld(1, 25) //[25:25]
    #define SHU_AC_TIME_05T_XRTR2W_05T                         Fld(1, 26) //[26:26]
    #define SHU_AC_TIME_05T_XRTW2R_M05T                        Fld(1, 27) //[27:27]

#define DRAMC_REG_SHU_AC_DERATING0                         (DRAMC_AO_BASE_ADDRESS + 0x00824)
    #define SHU_AC_DERATING0_ACDERATEEN                        Fld(1, 0) //[0:0]
    #define SHU_AC_DERATING0_TRRD_DERATE                       Fld(3, 16) //[18:16]
    #define SHU_AC_DERATING0_TRCD_DERATE                       Fld(4, 24) //[27:24]

#define DRAMC_REG_SHU_AC_DERATING1                         (DRAMC_AO_BASE_ADDRESS + 0x00828)
    #define SHU_AC_DERATING1_TRPAB_DERATE                      Fld(3, 0) //[2:0]
    #define SHU_AC_DERATING1_TRP_DERATE                        Fld(4, 8) //[11:8]
    #define SHU_AC_DERATING1_TRAS_DERATE                       Fld(4, 16) //[19:16]
    #define SHU_AC_DERATING1_TRC_DERATE                        Fld(5, 24) //[28:24]

#define DRAMC_REG_SHU_AC_DERATING_05T                      (DRAMC_AO_BASE_ADDRESS + 0x00830)
    #define SHU_AC_DERATING_05T_TRC_05T_DERATE                 Fld(1, 0) //[0:0]
    #define SHU_AC_DERATING_05T_TRCD_05T_DERATE                Fld(1, 6) //[6:6]
    #define SHU_AC_DERATING_05T_TRP_05T_DERATE                 Fld(1, 7) //[7:7]
    #define SHU_AC_DERATING_05T_TRPAB_05T_DERATE               Fld(1, 8) //[8:8]
    #define SHU_AC_DERATING_05T_TRAS_05T_DERATE                Fld(1, 9) //[9:9]
    #define SHU_AC_DERATING_05T_TRRD_05T_DERATE                Fld(1, 12) //[12:12]

#define DRAMC_REG_SHU_CONF0                                (DRAMC_AO_BASE_ADDRESS + 0x00840)
    #define SHU_CONF0_DMPGTIM                                  Fld(6, 0) //[5:0]
    #define SHU_CONF0_ADVREFEN                                 Fld(1, 6) //[6:6]
    #define SHU_CONF0_ADVPREEN                                 Fld(1, 7) //[7:7]
    #define SHU_CONF0_TRFCPBIG                                 Fld(1, 9) //[9:9]
    #define SHU_CONF0_REFTHD                                   Fld(4, 12) //[15:12]
    #define SHU_CONF0_REQQUE_DEPTH                             Fld(4, 16) //[19:16]
    #define SHU_CONF0_FREQDIV4                                 Fld(1, 24) //[24:24]
    #define SHU_CONF0_FDIV2                                    Fld(1, 25) //[25:25]
    #define SHU_CONF0_CL2                                      Fld(1, 27) //[27:27]
    #define SHU_CONF0_BL2                                      Fld(1, 28) //[28:28]
    #define SHU_CONF0_BL4                                      Fld(1, 29) //[29:29]
    #define SHU_CONF0_MATYPE                                   Fld(2, 30) //[31:30]

#define DRAMC_REG_SHU_CONF1                                (DRAMC_AO_BASE_ADDRESS + 0x00844)
    #define SHU_CONF1_DATLAT                                   Fld(5, 0) //[4:0]
    #define SHU_CONF1_DATLAT_DSEL                              Fld(5, 8) //[12:8]
    #define SHU_CONF1_REFBW_FR                                 Fld(10, 16) //[25:16]
    #define SHU_CONF1_DATLAT_DSEL_PHY                          Fld(5, 26) //[30:26]
    #define SHU_CONF1_TREFBWIG                                 Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU_CONF2                                (DRAMC_AO_BASE_ADDRESS + 0x00848)
    #define SHU_CONF2_TCMDO1LAT                                Fld(8, 0) //[7:0]
    #define SHU_CONF2_FSPCHG_PRDCNT                            Fld(8, 8) //[15:8]
    #define SHU_CONF2_DCMDLYREF                                Fld(3, 16) //[18:16]
    #define SHU_CONF2_DQCMD                                    Fld(1, 25) //[25:25]
    #define SHU_CONF2_DQ16COM1                                 Fld(1, 26) //[26:26]
    #define SHU_CONF2_RA15TOCS1                                Fld(1, 27) //[27:27]
    #define SHU_CONF2_WPRE2T                                   Fld(1, 28) //[28:28]
    #define SHU_CONF2_FASTWAKE2                                Fld(1, 29) //[29:29]
    #define SHU_CONF2_DAREFEN                                  Fld(1, 30) //[30:30]
    #define SHU_CONF2_FASTWAKE                                 Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU_CONF3                                (DRAMC_AO_BASE_ADDRESS + 0x0084C)
    #define SHU_CONF3_ZQCSCNT                                  Fld(16, 0) //[15:0]
    #define SHU_CONF3_REFRCNT                                  Fld(9, 16) //[24:16]

#define DRAMC_REG_SHU_STBCAL                               (DRAMC_AO_BASE_ADDRESS + 0x00850)
    #define SHU_STBCAL_DMSTBLAT                                Fld(2, 0) //[1:0]
    #define SHU_STBCAL_PICGLAT                                 Fld(3, 4) //[6:4]
    #define SHU_STBCAL_DQSG_MODE                               Fld(1, 8) //[8:8]

#define DRAMC_REG_SHU_DQSOSCTHRD                           (DRAMC_AO_BASE_ADDRESS + 0x00854)
    #define SHU_DQSOSCTHRD_DQSOSCTHRD_INC_RK0                  Fld(12, 0) //[11:0]
    #define SHU_DQSOSCTHRD_DQSOSCTHRD_DEC_RK0                  Fld(12, 12) //[23:12]
    #define SHU_DQSOSCTHRD_DQSOSCTHRD_INC_RK1_7TO0             Fld(8, 24) //[31:24]

#define DRAMC_REG_SHU_RANKCTL                              (DRAMC_AO_BASE_ADDRESS + 0x00858)
    #define SHU_RANKCTL_RANKINCTL_RXDLY                        Fld(4, 0) //[3:0]
    #define SHU_RANKCTL_TXRANKINCTL_TXDLY                      Fld(4, 8) //[11:8]
    #define SHU_RANKCTL_TXRANKINCTL                            Fld(4, 12) //[15:12]
    #define SHU_RANKCTL_TXRANKINCTL_ROOT                       Fld(4, 16) //[19:16]
    #define SHU_RANKCTL_RANKINCTL                              Fld(4, 20) //[23:20]
    #define SHU_RANKCTL_RANKINCTL_ROOT1                        Fld(4, 24) //[27:24]
    #define SHU_RANKCTL_RANKINCTL_PHY                          Fld(4, 28) //[31:28]

#define DRAMC_REG_SHU_CKECTRL                              (DRAMC_AO_BASE_ADDRESS + 0x0085C)
    #define SHU_CKECTRL_CMDCKE                                 Fld(3, 16) //[18:16]
    #define SHU_CKECTRL_CKEPRD                                 Fld(3, 20) //[22:20]
    #define SHU_CKECTRL_TCKESRX                                Fld(2, 24) //[25:24]
    #define SHU_CKECTRL_SREF_CK_DLY                            Fld(2, 28) //[29:28]

#define DRAMC_REG_SHU_ODTCTRL                              (DRAMC_AO_BASE_ADDRESS + 0x00860)
    #define SHU_ODTCTRL_ROEN                                   Fld(1, 0) //[0:0]
    #define SHU_ODTCTRL_WOEN                                   Fld(1, 1) //[1:1]
    #define SHU_ODTCTRL_RODTEN_SELPH_CG_IG                     Fld(1, 2) //[2:2]
    #define SHU_ODTCTRL_RODTENSTB_SELPH_CG_IG                  Fld(1, 3) //[3:3]
    #define SHU_ODTCTRL_RODT                                   Fld(4, 4) //[7:4]
    #define SHU_ODTCTRL_TWODT                                  Fld(7, 16) //[22:16]
    #define SHU_ODTCTRL_FIXRODT                                Fld(1, 27) //[27:27]
    #define SHU_ODTCTRL_RODTE2                                 Fld(1, 30) //[30:30]
    #define SHU_ODTCTRL_RODTE                                  Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU_IMPCAL1                              (DRAMC_AO_BASE_ADDRESS + 0x00864)
    #define SHU_IMPCAL1_IMPCAL_CHKCYCLE                        Fld(3, 0) //[2:0]
    #define SHU_IMPCAL1_IMPDRVP                                Fld(5, 4) //[8:4]
    #define SHU_IMPCAL1_IMPDRVN                                Fld(5, 11) //[15:11]
    #define SHU_IMPCAL1_IMPCAL_CALEN_CYCLE                     Fld(3, 17) //[19:17]
    #define SHU_IMPCAL1_IMPCALCNT                              Fld(8, 20) //[27:20]
    #define SHU_IMPCAL1_IMPCAL_CALICNT                         Fld(4, 28) //[31:28]

#define DRAMC_REG_SHU1_DQSOSC_PRD                          (DRAMC_AO_BASE_ADDRESS + 0x00868)
    #define SHU1_DQSOSC_PRD_DQSOSC_PRDCNT                      Fld(10, 0) //[9:0]
    #define SHU1_DQSOSC_PRD_DQSOSCTHRD_INC_RK1_11TO8           Fld(4, 16) //[19:16]
    #define SHU1_DQSOSC_PRD_DQSOSCTHRD_DEC_RK1                 Fld(12, 20) //[31:20]

#define DRAMC_REG_SHU_DQSOSCR                              (DRAMC_AO_BASE_ADDRESS + 0x0086C)
    #define SHU_DQSOSCR_DQSOSCRCNT                             Fld(8, 0) //[7:0]
    #define SHU_DQSOSCR_DQSOSC_DELTA                           Fld(16, 16) //[31:16]

#define DRAMC_REG_SHU_DQSOSCR2                             (DRAMC_AO_BASE_ADDRESS + 0x00870)
    #define SHU_DQSOSCR2_DQSOSCENCNT                           Fld(16, 0) //[15:0]
    #define SHU_DQSOSCR2_DQSOSC_ADV_SEL                        Fld(2, 16) //[17:16]
    #define SHU_DQSOSCR2_DQSOSC_DRS_ADV_SEL                    Fld(2, 18) //[19:18]

#define DRAMC_REG_SHU_RODTENSTB                            (DRAMC_AO_BASE_ADDRESS + 0x00874)
    #define SHU_RODTENSTB_RODTEN_MCK_MODESEL                   Fld(1, 0) //[0:0]
    #define SHU_RODTENSTB_RODTEN_P1_ENABLE                     Fld(1, 1) //[1:1]
    #define SHU_RODTENSTB_RODTENSTB_OFFSET                     Fld(6, 2) //[7:2]
    #define SHU_RODTENSTB_RODTENSTB_EXT                        Fld(16, 8) //[23:8]
    #define SHU_RODTENSTB_RODTENSTB_4BYTE_EN                   Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU_PIPE                                 (DRAMC_AO_BASE_ADDRESS + 0x00878)
    #define SHU_PIPE_PHYRXPIPE1                                Fld(1, 0) //[0:0]
    #define SHU_PIPE_PHYRXPIPE2                                Fld(1, 1) //[1:1]
    #define SHU_PIPE_PHYRXPIPE3                                Fld(1, 2) //[2:2]
    #define SHU_PIPE_PHYRXRDSLPIPE1                            Fld(1, 4) //[4:4]
    #define SHU_PIPE_PHYRXRDSLPIPE2                            Fld(1, 5) //[5:5]
    #define SHU_PIPE_PHYRXRDSLPIPE3                            Fld(1, 6) //[6:6]
    #define SHU_PIPE_PHYPIPE1EN                                Fld(1, 8) //[8:8]
    #define SHU_PIPE_PHYPIPE2EN                                Fld(1, 9) //[9:9]
    #define SHU_PIPE_PHYPIPE3EN                                Fld(1, 10) //[10:10]
    #define SHU_PIPE_DLE_LAST_EXTEND3                          Fld(1, 26) //[26:26]
    #define SHU_PIPE_READ_START_EXTEND3                        Fld(1, 27) //[27:27]
    #define SHU_PIPE_DLE_LAST_EXTEND2                          Fld(1, 28) //[28:28]
    #define SHU_PIPE_READ_START_EXTEND2                        Fld(1, 29) //[29:29]
    #define SHU_PIPE_DLE_LAST_EXTEND1                          Fld(1, 30) //[30:30]
    #define SHU_PIPE_READ_START_EXTEND1                        Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU_TEST1                                (DRAMC_AO_BASE_ADDRESS + 0x0087C)
    #define SHU_TEST1_LATNORMPOP                               Fld(5, 8) //[12:8]
    #define SHU_TEST1_DQSICALBLCOK_CNT                         Fld(3, 20) //[22:20]
    #define SHU_TEST1_DQSICALI_NEW                             Fld(1, 23) //[23:23]

#define DRAMC_REG_SHU_SELPH_CA1                            (DRAMC_AO_BASE_ADDRESS + 0x00880)
    #define SHU_SELPH_CA1_TXDLY_CS                             Fld(3, 0) //[2:0]
    #define SHU_SELPH_CA1_TXDLY_CKE                            Fld(3, 4) //[6:4]
    #define SHU_SELPH_CA1_TXDLY_ODT                            Fld(3, 8) //[10:8]
    #define SHU_SELPH_CA1_TXDLY_RESET                          Fld(3, 12) //[14:12]
    #define SHU_SELPH_CA1_TXDLY_WE                             Fld(3, 16) //[18:16]
    #define SHU_SELPH_CA1_TXDLY_CAS                            Fld(3, 20) //[22:20]
    #define SHU_SELPH_CA1_TXDLY_RAS                            Fld(3, 24) //[26:24]
    #define SHU_SELPH_CA1_TXDLY_CS1                            Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU_SELPH_CA2                            (DRAMC_AO_BASE_ADDRESS + 0x00884)
    #define SHU_SELPH_CA2_TXDLY_BA0                            Fld(3, 0) //[2:0]
    #define SHU_SELPH_CA2_TXDLY_BA1                            Fld(3, 4) //[6:4]
    #define SHU_SELPH_CA2_TXDLY_BA2                            Fld(3, 8) //[10:8]
    #define SHU_SELPH_CA2_TXDLY_CMD                            Fld(5, 16) //[20:16]
    #define SHU_SELPH_CA2_TXDLY_CKE1                           Fld(3, 24) //[26:24]

#define DRAMC_REG_SHU_SELPH_CA3                            (DRAMC_AO_BASE_ADDRESS + 0x00888)
    #define SHU_SELPH_CA3_TXDLY_RA0                            Fld(3, 0) //[2:0]
    #define SHU_SELPH_CA3_TXDLY_RA1                            Fld(3, 4) //[6:4]
    #define SHU_SELPH_CA3_TXDLY_RA2                            Fld(3, 8) //[10:8]
    #define SHU_SELPH_CA3_TXDLY_RA3                            Fld(3, 12) //[14:12]
    #define SHU_SELPH_CA3_TXDLY_RA4                            Fld(3, 16) //[18:16]
    #define SHU_SELPH_CA3_TXDLY_RA5                            Fld(3, 20) //[22:20]
    #define SHU_SELPH_CA3_TXDLY_RA6                            Fld(3, 24) //[26:24]
    #define SHU_SELPH_CA3_TXDLY_RA7                            Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU_SELPH_CA4                            (DRAMC_AO_BASE_ADDRESS + 0x0088C)
    #define SHU_SELPH_CA4_TXDLY_RA8                            Fld(3, 0) //[2:0]
    #define SHU_SELPH_CA4_TXDLY_RA9                            Fld(3, 4) //[6:4]
    #define SHU_SELPH_CA4_TXDLY_RA10                           Fld(3, 8) //[10:8]
    #define SHU_SELPH_CA4_TXDLY_RA11                           Fld(3, 12) //[14:12]
    #define SHU_SELPH_CA4_TXDLY_RA12                           Fld(3, 16) //[18:16]
    #define SHU_SELPH_CA4_TXDLY_RA13                           Fld(3, 20) //[22:20]
    #define SHU_SELPH_CA4_TXDLY_RA14                           Fld(3, 24) //[26:24]
    #define SHU_SELPH_CA4_TXDLY_RA15                           Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU_SELPH_CA5                            (DRAMC_AO_BASE_ADDRESS + 0x00890)
    #define SHU_SELPH_CA5_DLY_CS                               Fld(3, 0) //[2:0]
    #define SHU_SELPH_CA5_DLY_CKE                              Fld(3, 4) //[6:4]
    #define SHU_SELPH_CA5_DLY_ODT                              Fld(3, 8) //[10:8]
    #define SHU_SELPH_CA5_DLY_RESET                            Fld(3, 12) //[14:12]
    #define SHU_SELPH_CA5_DLY_WE                               Fld(3, 16) //[18:16]
    #define SHU_SELPH_CA5_DLY_CAS                              Fld(3, 20) //[22:20]
    #define SHU_SELPH_CA5_DLY_RAS                              Fld(3, 24) //[26:24]
    #define SHU_SELPH_CA5_DLY_CS1                              Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU_SELPH_CA6                            (DRAMC_AO_BASE_ADDRESS + 0x00894)
    #define SHU_SELPH_CA6_DLY_BA0                              Fld(3, 0) //[2:0]
    #define SHU_SELPH_CA6_DLY_BA1                              Fld(3, 4) //[6:4]
    #define SHU_SELPH_CA6_DLY_BA2                              Fld(3, 8) //[10:8]
    #define SHU_SELPH_CA6_DLY_CKE1                             Fld(3, 24) //[26:24]

#define DRAMC_REG_SHU_SELPH_CA7                            (DRAMC_AO_BASE_ADDRESS + 0x00898)
    #define SHU_SELPH_CA7_DLY_RA0                              Fld(3, 0) //[2:0]
    #define SHU_SELPH_CA7_DLY_RA1                              Fld(3, 4) //[6:4]
    #define SHU_SELPH_CA7_DLY_RA2                              Fld(3, 8) //[10:8]
    #define SHU_SELPH_CA7_DLY_RA3                              Fld(3, 12) //[14:12]
    #define SHU_SELPH_CA7_DLY_RA4                              Fld(3, 16) //[18:16]
    #define SHU_SELPH_CA7_DLY_RA5                              Fld(3, 20) //[22:20]
    #define SHU_SELPH_CA7_DLY_RA6                              Fld(3, 24) //[26:24]
    #define SHU_SELPH_CA7_DLY_RA7                              Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU_SELPH_CA8                            (DRAMC_AO_BASE_ADDRESS + 0x0089C)
    #define SHU_SELPH_CA8_DLY_RA8                              Fld(3, 0) //[2:0]
    #define SHU_SELPH_CA8_DLY_RA9                              Fld(3, 4) //[6:4]
    #define SHU_SELPH_CA8_DLY_RA10                             Fld(3, 8) //[10:8]
    #define SHU_SELPH_CA8_DLY_RA11                             Fld(3, 12) //[14:12]
    #define SHU_SELPH_CA8_DLY_RA12                             Fld(3, 16) //[18:16]
    #define SHU_SELPH_CA8_DLY_RA13                             Fld(3, 20) //[22:20]
    #define SHU_SELPH_CA8_DLY_RA14                             Fld(3, 24) //[26:24]
    #define SHU_SELPH_CA8_DLY_RA15                             Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU_SELPH_DQS0                           (DRAMC_AO_BASE_ADDRESS + 0x008A0)
    #define SHU_SELPH_DQS0_TXDLY_DQS0                          Fld(3, 0) //[2:0]
    #define SHU_SELPH_DQS0_TXDLY_DQS1                          Fld(3, 4) //[6:4]
    #define SHU_SELPH_DQS0_TXDLY_DQS2                          Fld(3, 8) //[10:8]
    #define SHU_SELPH_DQS0_TXDLY_DQS3                          Fld(3, 12) //[14:12]
    #define SHU_SELPH_DQS0_TXDLY_OEN_DQS0                      Fld(3, 16) //[18:16]
    #define SHU_SELPH_DQS0_TXDLY_OEN_DQS1                      Fld(3, 20) //[22:20]
    #define SHU_SELPH_DQS0_TXDLY_OEN_DQS2                      Fld(3, 24) //[26:24]
    #define SHU_SELPH_DQS0_TXDLY_OEN_DQS3                      Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU_SELPH_DQS1                           (DRAMC_AO_BASE_ADDRESS + 0x008A4)
    #define SHU_SELPH_DQS1_DLY_DQS0                            Fld(3, 0) //[2:0]
    #define SHU_SELPH_DQS1_DLY_DQS1                            Fld(3, 4) //[6:4]
    #define SHU_SELPH_DQS1_DLY_DQS2                            Fld(3, 8) //[10:8]
    #define SHU_SELPH_DQS1_DLY_DQS3                            Fld(3, 12) //[14:12]
    #define SHU_SELPH_DQS1_DLY_OEN_DQS0                        Fld(3, 16) //[18:16]
    #define SHU_SELPH_DQS1_DLY_OEN_DQS1                        Fld(3, 20) //[22:20]
    #define SHU_SELPH_DQS1_DLY_OEN_DQS2                        Fld(3, 24) //[26:24]
    #define SHU_SELPH_DQS1_DLY_OEN_DQS3                        Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU1_DRVING1                             (DRAMC_AO_BASE_ADDRESS + 0x008A8)
    #define SHU1_DRVING1_DQDRVN2                               Fld(5, 0) //[4:0]
    #define SHU1_DRVING1_DQDRVP2                               Fld(5, 5) //[9:5]
    #define SHU1_DRVING1_DQSDRVN1                              Fld(5, 10) //[14:10]
    #define SHU1_DRVING1_DQSDRVP1                              Fld(5, 15) //[19:15]
    #define SHU1_DRVING1_DQSDRVN2                              Fld(5, 20) //[24:20]
    #define SHU1_DRVING1_DQSDRVP2                              Fld(5, 25) //[29:25]
    #define SHU1_DRVING1_DIS_IMP_ODTN_TRACK                    Fld(1, 30) //[30:30]
    #define SHU1_DRVING1_DIS_IMPCAL_HW                         Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU1_DRVING2                             (DRAMC_AO_BASE_ADDRESS + 0x008AC)
    #define SHU1_DRVING2_CMDDRVN1                              Fld(5, 0) //[4:0]
    #define SHU1_DRVING2_CMDDRVP1                              Fld(5, 5) //[9:5]
    #define SHU1_DRVING2_CMDDRVN2                              Fld(5, 10) //[14:10]
    #define SHU1_DRVING2_CMDDRVP2                              Fld(5, 15) //[19:15]
    #define SHU1_DRVING2_DQDRVN1                               Fld(5, 20) //[24:20]
    #define SHU1_DRVING2_DQDRVP1                               Fld(5, 25) //[29:25]
    #define SHU1_DRVING2_DIS_IMPCAL_ODT_EN                     Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU1_DRVING3                             (DRAMC_AO_BASE_ADDRESS + 0x008B0)
    #define SHU1_DRVING3_DQODTN2                               Fld(5, 0) //[4:0]
    #define SHU1_DRVING3_DQODTP2                               Fld(5, 5) //[9:5]
    #define SHU1_DRVING3_DQSODTN                               Fld(5, 10) //[14:10]
    #define SHU1_DRVING3_DQSODTP                               Fld(5, 15) //[19:15]
    #define SHU1_DRVING3_DQSODTN2                              Fld(5, 20) //[24:20]
    #define SHU1_DRVING3_DQSODTP2                              Fld(5, 25) //[29:25]

#define DRAMC_REG_SHU1_DRVING4                             (DRAMC_AO_BASE_ADDRESS + 0x008B4)
    #define SHU1_DRVING4_CMDODTN1                              Fld(5, 0) //[4:0]
    #define SHU1_DRVING4_CMDODTP1                              Fld(5, 5) //[9:5]
    #define SHU1_DRVING4_CMDODTN2                              Fld(5, 10) //[14:10]
    #define SHU1_DRVING4_CMDODTP2                              Fld(5, 15) //[19:15]
    #define SHU1_DRVING4_DQODTN1                               Fld(5, 20) //[24:20]
    #define SHU1_DRVING4_DQODTP1                               Fld(5, 25) //[29:25]

#define DRAMC_REG_SHU1_DRVING5                             (DRAMC_AO_BASE_ADDRESS + 0x008B8)
    #define SHU1_DRVING5_DQCODTN2                              Fld(5, 0) //[4:0]
    #define SHU1_DRVING5_DQCODTP2                              Fld(5, 5) //[9:5]
    #define SHU1_DRVING5_DQCDRVN1                              Fld(5, 10) //[14:10]
    #define SHU1_DRVING5_DQCDRVP1                              Fld(5, 15) //[19:15]
    #define SHU1_DRVING5_DQCDRVN2                              Fld(5, 20) //[24:20]
    #define SHU1_DRVING5_DQCDRVP2                              Fld(5, 25) //[29:25]

#define DRAMC_REG_SHU1_DRVING6                             (DRAMC_AO_BASE_ADDRESS + 0x008BC)
    #define SHU1_DRVING6_DQCODTN1                              Fld(5, 20) //[24:20]
    #define SHU1_DRVING6_DQCODTP1                              Fld(5, 25) //[29:25]

#define DRAMC_REG_SHU1_WODT                                (DRAMC_AO_BASE_ADDRESS + 0x008C0)
    #define SHU1_WODT_DISWODT                                  Fld(3, 0) //[2:0]
    #define SHU1_WODT_WODTFIX                                  Fld(1, 3) //[3:3]
    #define SHU1_WODT_WODTFIXOFF                               Fld(1, 4) //[4:4]
    #define SHU1_WODT_DISWODTE                                 Fld(1, 5) //[5:5]
    #define SHU1_WODT_DISWODTE2                                Fld(1, 6) //[6:6]
    #define SHU1_WODT_WODTPDEN                                 Fld(1, 7) //[7:7]
    #define SHU1_WODT_DQOE_CNT                                 Fld(3, 8) //[10:8]
    #define SHU1_WODT_DQOE_OPT                                 Fld(1, 11) //[11:11]
    #define SHU1_WODT_TXUPD_SEL                                Fld(2, 12) //[13:12]
    #define SHU1_WODT_TXUPD_W2R_SEL                            Fld(3, 14) //[16:14]
    #define SHU1_WODT_DBIWR                                    Fld(1, 29) //[29:29]
    #define SHU1_WODT_TWPSTEXT                                 Fld(1, 30) //[30:30]
    #define SHU1_WODT_WPST2T                                   Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU1_DQSG                                (DRAMC_AO_BASE_ADDRESS + 0x008C4)
    #define SHU1_DQSG_DLLFRZRFCOPT                             Fld(2, 0) //[1:0]
    #define SHU1_DQSG_DLLFRZWROPT                              Fld(2, 4) //[5:4]
    #define SHU1_DQSG_R_RSTBCNT_LATCH_OPT                      Fld(3, 8) //[10:8]
    #define SHU1_DQSG_STB_UPDMASK_EN                           Fld(1, 11) //[11:11]
    #define SHU1_DQSG_STB_UPDMASKCYC                           Fld(4, 12) //[15:12]
    #define SHU1_DQSG_DQSINCTL_PRE_SEL                         Fld(1, 16) //[16:16]
    #define SHU1_DQSG_SCINTV                                   Fld(6, 20) //[25:20]

#define DRAMC_REG_SHU_SCINTV                               (DRAMC_AO_BASE_ADDRESS + 0x008C8)
    #define SHU_SCINTV_ODTREN                                  Fld(1, 0) //[0:0]
    #define SHU_SCINTV_TZQLAT                                  Fld(5, 1) //[5:1]
    #define SHU_SCINTV_TZQLAT2                                 Fld(5, 6) //[10:6]
    #define SHU_SCINTV_RDDQC_INTV                              Fld(2, 11) //[12:11]
    #define SHU_SCINTV_MRW_INTV                                Fld(5, 13) //[17:13]
    #define SHU_SCINTV_DQS2DQ_SHU_PITHRD                       Fld(6, 18) //[23:18]
    #define SHU_SCINTV_DQS2DQ_FILT_PITHRD                      Fld(6, 24) //[29:24]
    #define SHU_SCINTV_DQSOSCENDIS                             Fld(1, 30) //[30:30]

#define DRAMC_REG_SHU_MISC                                 (DRAMC_AO_BASE_ADDRESS + 0x008CC)
    #define SHU_MISC_REQQUE_MAXCNT                             Fld(4, 0) //[3:0]
    #define SHU_MISC_CKEHCMD                                   Fld(2, 4) //[5:4]
    #define SHU_MISC_NORMPOP_LEN                               Fld(3, 8) //[10:8]
    #define SHU_MISC_PREA_INTV                                 Fld(5, 12) //[16:12]

#define DRAMC_REG_SHU_DQS2DQ_TX                            (DRAMC_AO_BASE_ADDRESS + 0x008D0)
    #define SHU_DQS2DQ_TX_OE2DQ_OFFSET                         Fld(5, 0) //[4:0]

#define DRAMC_REG_SHU_HWSET_MR2                            (DRAMC_AO_BASE_ADDRESS + 0x008D4)
    #define SHU_HWSET_MR2_HWSET_MR2_MRSMA                      Fld(13, 0) //[12:0]
    #define SHU_HWSET_MR2_HWSET_MR2_OP                         Fld(8, 16) //[23:16]

#define DRAMC_REG_SHU_HWSET_MR13                           (DRAMC_AO_BASE_ADDRESS + 0x008D8)
    #define SHU_HWSET_MR13_HWSET_MR13_MRSMA                    Fld(13, 0) //[12:0]
    #define SHU_HWSET_MR13_HWSET_MR13_OP                       Fld(8, 16) //[23:16]

#define DRAMC_REG_SHU_HWSET_VRCG                           (DRAMC_AO_BASE_ADDRESS + 0x008DC)
    #define SHU_HWSET_VRCG_HWSET_VRCG_MRSMA                    Fld(13, 0) //[12:0]
    #define SHU_HWSET_VRCG_HWSET_VRCG_OP                       Fld(8, 16) //[23:16]

#define DRAMC_REG_SHU_APHY_TX_PICG_CTRL                    (DRAMC_AO_BASE_ADDRESS + 0x008E4)
    #define SHU_APHY_TX_PICG_CTRL_APHYPI_CG_CK_SEL             Fld(4, 20) //[23:20]
    #define SHU_APHY_TX_PICG_CTRL_APHYPI_CG_CK_OPT             Fld(1, 24) //[24:24]
    #define SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_DYN_GATING_SEL    Fld(4, 27) //[30:27]
    #define SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_OPT            Fld(1, 31) //[31:31]

#define DRAMC_REG_SHURK0_DQSCTL                            (DRAMC_AO_BASE_ADDRESS + 0x00A00)
    #define SHURK0_DQSCTL_DQSINCTL                             Fld(4, 0) //[3:0]

#define DRAMC_REG_SHURK0_DQSIEN                            (DRAMC_AO_BASE_ADDRESS + 0x00A04)
    #define SHURK0_DQSIEN_R0DQS0IEN                            Fld(7, 0) //[6:0]
    #define SHURK0_DQSIEN_R0DQS1IEN                            Fld(7, 8) //[14:8]
    #define SHURK0_DQSIEN_R0DQS2IEN                            Fld(7, 16) //[22:16]
    #define SHURK0_DQSIEN_R0DQS3IEN                            Fld(7, 24) //[30:24]

#define DRAMC_REG_SHURK0_DQSCAL                            (DRAMC_AO_BASE_ADDRESS + 0x00A08)
    #define SHURK0_DQSCAL_R0DQSIENLLMT                         Fld(7, 0) //[6:0]
    #define SHURK0_DQSCAL_R0DQSIENLLMTEN                       Fld(1, 7) //[7:7]
    #define SHURK0_DQSCAL_R0DQSIENHLMT                         Fld(7, 8) //[14:8]
    #define SHURK0_DQSCAL_R0DQSIENHLMTEN                       Fld(1, 15) //[15:15]

#define DRAMC_REG_SHU1RK0_PI                               (DRAMC_AO_BASE_ADDRESS + 0x00A0C)
    #define SHU1RK0_PI_RK0_ARPI_DQ_B1                          Fld(6, 0) //[5:0]
    #define SHU1RK0_PI_RK0_ARPI_DQ_B0                          Fld(6, 8) //[13:8]
    #define SHU1RK0_PI_RK0_ARPI_DQM_B1                         Fld(6, 16) //[21:16]
    #define SHU1RK0_PI_RK0_ARPI_DQM_B0                         Fld(6, 24) //[29:24]

#define DRAMC_REG_SHU1RK0_DQSOSC                           (DRAMC_AO_BASE_ADDRESS + 0x00A10)
    #define SHU1RK0_DQSOSC_DQSOSC_BASE_RK0                     Fld(16, 0) //[15:0]
    #define SHU1RK0_DQSOSC_DQSOSC_BASE_RK0_B1                  Fld(16, 16) //[31:16]

#define DRAMC_REG_SHURK0_SELPH_ODTEN0                      (DRAMC_AO_BASE_ADDRESS + 0x00A1C)
    #define SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN                Fld(3, 0) //[2:0]
    #define SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1             Fld(3, 4) //[6:4]
    #define SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN                Fld(3, 8) //[10:8]
    #define SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1             Fld(3, 12) //[14:12]
    #define SHURK0_SELPH_ODTEN0_TXDLY_B2_RODTEN                Fld(3, 16) //[18:16]
    #define SHURK0_SELPH_ODTEN0_TXDLY_B2_RODTEN_P1             Fld(3, 20) //[22:20]
    #define SHURK0_SELPH_ODTEN0_TXDLY_B3_RODTEN                Fld(3, 24) //[26:24]
    #define SHURK0_SELPH_ODTEN0_TXDLY_B3_RODTEN_P1             Fld(3, 28) //[30:28]

#define DRAMC_REG_SHURK0_SELPH_ODTEN1                      (DRAMC_AO_BASE_ADDRESS + 0x00A20)
    #define SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN                  Fld(3, 0) //[2:0]
    #define SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1               Fld(3, 4) //[6:4]
    #define SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN                  Fld(3, 8) //[10:8]
    #define SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1               Fld(3, 12) //[14:12]
    #define SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN                  Fld(3, 16) //[18:16]
    #define SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1               Fld(3, 20) //[22:20]
    #define SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN                  Fld(3, 24) //[26:24]
    #define SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1               Fld(3, 28) //[30:28]

#define DRAMC_REG_SHURK0_SELPH_DQSG0                       (DRAMC_AO_BASE_ADDRESS + 0x00A24)
    #define SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED               Fld(3, 0) //[2:0]
    #define SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1            Fld(3, 4) //[6:4]
    #define SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED               Fld(3, 8) //[10:8]
    #define SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1            Fld(3, 12) //[14:12]
    #define SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED               Fld(3, 16) //[18:16]
    #define SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1            Fld(3, 20) //[22:20]
    #define SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED               Fld(3, 24) //[26:24]
    #define SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1            Fld(3, 28) //[30:28]

#define DRAMC_REG_SHURK0_SELPH_DQSG1                       (DRAMC_AO_BASE_ADDRESS + 0x00A28)
    #define SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED              Fld(3, 0) //[2:0]
    #define SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1           Fld(3, 4) //[6:4]
    #define SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED              Fld(3, 8) //[10:8]
    #define SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1           Fld(3, 12) //[14:12]
    #define SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED              Fld(3, 16) //[18:16]
    #define SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED_P1           Fld(3, 20) //[22:20]
    #define SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED              Fld(3, 24) //[26:24]
    #define SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED_P1           Fld(3, 28) //[30:28]

#define DRAMC_REG_SHURK0_SELPH_DQ0                         (DRAMC_AO_BASE_ADDRESS + 0x00A2C)
    #define SHURK0_SELPH_DQ0_TXDLY_DQ0                         Fld(3, 0) //[2:0]
    #define SHURK0_SELPH_DQ0_TXDLY_DQ1                         Fld(3, 4) //[6:4]
    #define SHURK0_SELPH_DQ0_TXDLY_DQ2                         Fld(3, 8) //[10:8]
    #define SHURK0_SELPH_DQ0_TXDLY_DQ3                         Fld(3, 12) //[14:12]
    #define SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0                     Fld(3, 16) //[18:16]
    #define SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1                     Fld(3, 20) //[22:20]
    #define SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2                     Fld(3, 24) //[26:24]
    #define SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3                     Fld(3, 28) //[30:28]

#define DRAMC_REG_SHURK0_SELPH_DQ1                         (DRAMC_AO_BASE_ADDRESS + 0x00A30)
    #define SHURK0_SELPH_DQ1_TXDLY_DQM0                        Fld(3, 0) //[2:0]
    #define SHURK0_SELPH_DQ1_TXDLY_DQM1                        Fld(3, 4) //[6:4]
    #define SHURK0_SELPH_DQ1_TXDLY_DQM2                        Fld(3, 8) //[10:8]
    #define SHURK0_SELPH_DQ1_TXDLY_DQM3                        Fld(3, 12) //[14:12]
    #define SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0                    Fld(3, 16) //[18:16]
    #define SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1                    Fld(3, 20) //[22:20]
    #define SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2                    Fld(3, 24) //[26:24]
    #define SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3                    Fld(3, 28) //[30:28]

#define DRAMC_REG_SHURK0_SELPH_DQ2                         (DRAMC_AO_BASE_ADDRESS + 0x00A34)
    #define SHURK0_SELPH_DQ2_DLY_DQ0                           Fld(3, 0) //[2:0]
    #define SHURK0_SELPH_DQ2_DLY_DQ1                           Fld(3, 4) //[6:4]
    #define SHURK0_SELPH_DQ2_DLY_DQ2                           Fld(3, 8) //[10:8]
    #define SHURK0_SELPH_DQ2_DLY_DQ3                           Fld(3, 12) //[14:12]
    #define SHURK0_SELPH_DQ2_DLY_OEN_DQ0                       Fld(3, 16) //[18:16]
    #define SHURK0_SELPH_DQ2_DLY_OEN_DQ1                       Fld(3, 20) //[22:20]
    #define SHURK0_SELPH_DQ2_DLY_OEN_DQ2                       Fld(3, 24) //[26:24]
    #define SHURK0_SELPH_DQ2_DLY_OEN_DQ3                       Fld(3, 28) //[30:28]

#define DRAMC_REG_SHURK0_SELPH_DQ3                         (DRAMC_AO_BASE_ADDRESS + 0x00A38)
    #define SHURK0_SELPH_DQ3_DLY_DQM0                          Fld(3, 0) //[2:0]
    #define SHURK0_SELPH_DQ3_DLY_DQM1                          Fld(3, 4) //[6:4]
    #define SHURK0_SELPH_DQ3_DLY_DQM2                          Fld(3, 8) //[10:8]
    #define SHURK0_SELPH_DQ3_DLY_DQM3                          Fld(3, 12) //[14:12]
    #define SHURK0_SELPH_DQ3_DLY_OEN_DQM0                      Fld(3, 16) //[18:16]
    #define SHURK0_SELPH_DQ3_DLY_OEN_DQM1                      Fld(3, 20) //[22:20]
    #define SHURK0_SELPH_DQ3_DLY_OEN_DQM2                      Fld(3, 24) //[26:24]
    #define SHURK0_SELPH_DQ3_DLY_OEN_DQM3                      Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU1RK0_DQS2DQ_CAL1                      (DRAMC_AO_BASE_ADDRESS + 0x00A40)
    #define SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0           Fld(11, 0) //[10:0]
    #define SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1           Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU1RK0_DQS2DQ_CAL2                      (DRAMC_AO_BASE_ADDRESS + 0x00A44)
    #define SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0           Fld(11, 0) //[10:0]
    #define SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1           Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU1RK0_DQS2DQ_CAL3                      (DRAMC_AO_BASE_ADDRESS + 0x00A48)
    #define SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0       Fld(6, 0) //[5:0]
    #define SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1       Fld(6, 6) //[11:6]
    #define SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0_B4TO0 Fld(5, 12) //[16:12]
    #define SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1_B4TO0 Fld(5, 17) //[21:17]

#define DRAMC_REG_SHU1RK0_DQS2DQ_CAL4                      (DRAMC_AO_BASE_ADDRESS + 0x00A4C)
    #define SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0      Fld(6, 0) //[5:0]
    #define SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1      Fld(6, 6) //[11:6]
    #define SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0_B4TO0 Fld(5, 12) //[16:12]
    #define SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1_B4TO0 Fld(5, 17) //[21:17]

#define DRAMC_REG_SHU1RK0_DQS2DQ_CAL5                      (DRAMC_AO_BASE_ADDRESS + 0x00A50)
    #define SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0          Fld(11, 0) //[10:0]
    #define SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1          Fld(11, 16) //[26:16]

#define DRAMC_REG_SHURK1_DQSCTL                            (DRAMC_AO_BASE_ADDRESS + 0x00B00)
    #define SHURK1_DQSCTL_R1DQSINCTL                           Fld(4, 0) //[3:0]

#define DRAMC_REG_SHURK1_DQSIEN                            (DRAMC_AO_BASE_ADDRESS + 0x00B04)
    #define SHURK1_DQSIEN_R1DQS0IEN                            Fld(7, 0) //[6:0]
    #define SHURK1_DQSIEN_R1DQS1IEN                            Fld(7, 8) //[14:8]
    #define SHURK1_DQSIEN_R1DQS2IEN                            Fld(7, 16) //[22:16]
    #define SHURK1_DQSIEN_R1DQS3IEN                            Fld(7, 24) //[30:24]

#define DRAMC_REG_SHURK1_DQSCAL                            (DRAMC_AO_BASE_ADDRESS + 0x00B08)
    #define SHURK1_DQSCAL_R1DQSIENLLMT                         Fld(7, 0) //[6:0]
    #define SHURK1_DQSCAL_R1DQSIENLLMTEN                       Fld(1, 7) //[7:7]
    #define SHURK1_DQSCAL_R1DQSIENHLMT                         Fld(7, 8) //[14:8]
    #define SHURK1_DQSCAL_R1DQSIENHLMTEN                       Fld(1, 15) //[15:15]

#define DRAMC_REG_SHU1RK1_PI                               (DRAMC_AO_BASE_ADDRESS + 0x00B0C)
    #define SHU1RK1_PI_RK1_ARPI_DQ_B1                          Fld(6, 0) //[5:0]
    #define SHU1RK1_PI_RK1_ARPI_DQ_B0                          Fld(6, 8) //[13:8]
    #define SHU1RK1_PI_RK1_ARPI_DQM_B1                         Fld(6, 16) //[21:16]
    #define SHU1RK1_PI_RK1_ARPI_DQM_B0                         Fld(6, 24) //[29:24]

#define DRAMC_REG_SHU1RK1_DQSOSC                           (DRAMC_AO_BASE_ADDRESS + 0x00B10)
    #define SHU1RK1_DQSOSC_DQSOSC_BASE_RK1                     Fld(16, 0) //[15:0]
    #define SHU1RK1_DQSOSC_DQSOSC_BASE_RK1_B1                  Fld(16, 16) //[31:16]

#define DRAMC_REG_SHURK1_SELPH_ODTEN0                      (DRAMC_AO_BASE_ADDRESS + 0x00B1C)
    #define SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN              Fld(3, 0) //[2:0]
    #define SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN_P1           Fld(3, 4) //[6:4]
    #define SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN              Fld(3, 8) //[10:8]
    #define SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN_P1           Fld(3, 12) //[14:12]
    #define SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN              Fld(3, 16) //[18:16]
    #define SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN_P1           Fld(3, 20) //[22:20]
    #define SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN              Fld(3, 24) //[26:24]
    #define SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN_P1           Fld(3, 28) //[30:28]

#define DRAMC_REG_SHURK1_SELPH_ODTEN1                      (DRAMC_AO_BASE_ADDRESS + 0x00B20)
    #define SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN                Fld(3, 0) //[2:0]
    #define SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1             Fld(3, 4) //[6:4]
    #define SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN                Fld(3, 8) //[10:8]
    #define SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1             Fld(3, 12) //[14:12]
    #define SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN                Fld(3, 16) //[18:16]
    #define SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN_P1             Fld(3, 20) //[22:20]
    #define SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN                Fld(3, 24) //[26:24]
    #define SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN_P1             Fld(3, 28) //[30:28]

#define DRAMC_REG_SHURK1_SELPH_DQSG0                       (DRAMC_AO_BASE_ADDRESS + 0x00B24)
    #define SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED             Fld(3, 0) //[2:0]
    #define SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1          Fld(3, 4) //[6:4]
    #define SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED             Fld(3, 8) //[10:8]
    #define SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1          Fld(3, 12) //[14:12]
    #define SHURK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED             Fld(3, 16) //[18:16]
    #define SHURK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED_P1          Fld(3, 20) //[22:20]
    #define SHURK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED             Fld(3, 24) //[26:24]
    #define SHURK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED_P1          Fld(3, 28) //[30:28]

#define DRAMC_REG_SHURK1_SELPH_DQSG1                       (DRAMC_AO_BASE_ADDRESS + 0x00B28)
    #define SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED            Fld(3, 0) //[2:0]
    #define SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1         Fld(3, 4) //[6:4]
    #define SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED            Fld(3, 8) //[10:8]
    #define SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1         Fld(3, 12) //[14:12]
    #define SHURK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED            Fld(3, 16) //[18:16]
    #define SHURK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED_P1         Fld(3, 20) //[22:20]
    #define SHURK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED            Fld(3, 24) //[26:24]
    #define SHURK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED_P1         Fld(3, 28) //[30:28]

#define DRAMC_REG_SHURK1_SELPH_DQ0                         (DRAMC_AO_BASE_ADDRESS + 0x00B2C)
    #define SHURK1_SELPH_DQ0_TX_DLY_R1DQ0                      Fld(3, 0) //[2:0]
    #define SHURK1_SELPH_DQ0_TX_DLY_R1DQ1                      Fld(3, 4) //[6:4]
    #define SHURK1_SELPH_DQ0_TX_DLY_R1DQ2                      Fld(3, 8) //[10:8]
    #define SHURK1_SELPH_DQ0_TX_DLY_R1DQ3                      Fld(3, 12) //[14:12]
    #define SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0                  Fld(3, 16) //[18:16]
    #define SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1                  Fld(3, 20) //[22:20]
    #define SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2                  Fld(3, 24) //[26:24]
    #define SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3                  Fld(3, 28) //[30:28]

#define DRAMC_REG_SHURK1_SELPH_DQ1                         (DRAMC_AO_BASE_ADDRESS + 0x00B30)
    #define SHURK1_SELPH_DQ1_TX_DLY_R1DQM0                     Fld(3, 0) //[2:0]
    #define SHURK1_SELPH_DQ1_TX_DLY_R1DQM1                     Fld(3, 4) //[6:4]
    #define SHURK1_SELPH_DQ1_TX_DLY_R1DQM2                     Fld(3, 8) //[10:8]
    #define SHURK1_SELPH_DQ1_TX_DLY_R1DQM3                     Fld(3, 12) //[14:12]
    #define SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0                 Fld(3, 16) //[18:16]
    #define SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1                 Fld(3, 20) //[22:20]
    #define SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2                 Fld(3, 24) //[26:24]
    #define SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3                 Fld(3, 28) //[30:28]

#define DRAMC_REG_SHURK1_SELPH_DQ2                         (DRAMC_AO_BASE_ADDRESS + 0x00B34)
    #define SHURK1_SELPH_DQ2_DLY_R1DQ0                         Fld(3, 0) //[2:0]
    #define SHURK1_SELPH_DQ2_DLY_R1DQ1                         Fld(3, 4) //[6:4]
    #define SHURK1_SELPH_DQ2_DLY_R1DQ2                         Fld(3, 8) //[10:8]
    #define SHURK1_SELPH_DQ2_DLY_R1DQ3                         Fld(3, 12) //[14:12]
    #define SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0                     Fld(3, 16) //[18:16]
    #define SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1                     Fld(3, 20) //[22:20]
    #define SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2                     Fld(3, 24) //[26:24]
    #define SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3                     Fld(3, 28) //[30:28]

#define DRAMC_REG_SHURK1_SELPH_DQ3                         (DRAMC_AO_BASE_ADDRESS + 0x00B38)
    #define SHURK1_SELPH_DQ3_DLY_R1DQM0                        Fld(3, 0) //[2:0]
    #define SHURK1_SELPH_DQ3_DLY_R1DQM1                        Fld(3, 4) //[6:4]
    #define SHURK1_SELPH_DQ3_DLY_R1DQM2                        Fld(3, 8) //[10:8]
    #define SHURK1_SELPH_DQ3_DLY_R1DQM3                        Fld(3, 12) //[14:12]
    #define SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0                    Fld(3, 16) //[18:16]
    #define SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1                    Fld(3, 20) //[22:20]
    #define SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2                    Fld(3, 24) //[26:24]
    #define SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3                    Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU1RK1_DQS2DQ_CAL1                      (DRAMC_AO_BASE_ADDRESS + 0x00B40)
    #define SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0           Fld(11, 0) //[10:0]
    #define SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1           Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU1RK1_DQS2DQ_CAL2                      (DRAMC_AO_BASE_ADDRESS + 0x00B44)
    #define SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0           Fld(11, 0) //[10:0]
    #define SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1           Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU1RK1_DQS2DQ_CAL3                      (DRAMC_AO_BASE_ADDRESS + 0x00B48)
    #define SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0       Fld(6, 0) //[5:0]
    #define SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1       Fld(6, 6) //[11:6]
    #define SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0_B4TO0 Fld(5, 12) //[16:12]
    #define SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1_B4TO0 Fld(5, 17) //[21:17]

#define DRAMC_REG_SHU1RK1_DQS2DQ_CAL4                      (DRAMC_AO_BASE_ADDRESS + 0x00B4C)
    #define SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0      Fld(6, 0) //[5:0]
    #define SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1      Fld(6, 6) //[11:6]
    #define SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0_B4TO0 Fld(5, 12) //[16:12]
    #define SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1_B4TO0 Fld(5, 17) //[21:17]

#define DRAMC_REG_SHU1RK1_DQS2DQ_CAL5                      (DRAMC_AO_BASE_ADDRESS + 0x00B50)
    #define SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0          Fld(11, 0) //[10:0]
    #define SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1          Fld(11, 16) //[26:16]

#define DRAMC_REG_SHURK2_DQSCTL                            (DRAMC_AO_BASE_ADDRESS + 0x00C00)
    #define SHURK2_DQSCTL_R2DQSINCTL                           Fld(4, 0) //[3:0]

#define DRAMC_REG_SHURK2_DQSIEN                            (DRAMC_AO_BASE_ADDRESS + 0x00C04)
    #define SHURK2_DQSIEN_R2DQS0IEN                            Fld(7, 0) //[6:0]
    #define SHURK2_DQSIEN_R2DQS1IEN                            Fld(7, 8) //[14:8]
    #define SHURK2_DQSIEN_R2DQS2IEN                            Fld(7, 16) //[22:16]
    #define SHURK2_DQSIEN_R2DQS3IEN                            Fld(7, 24) //[30:24]

#define DRAMC_REG_SHURK2_DQSCAL                            (DRAMC_AO_BASE_ADDRESS + 0x00C08)
    #define SHURK2_DQSCAL_R2DQSIENLLMT                         Fld(7, 0) //[6:0]
    #define SHURK2_DQSCAL_R2DQSIENLLMTEN                       Fld(1, 7) //[7:7]
    #define SHURK2_DQSCAL_R2DQSIENHLMT                         Fld(7, 8) //[14:8]
    #define SHURK2_DQSCAL_R2DQSIENHLMTEN                       Fld(1, 15) //[15:15]

#define DRAMC_REG_SHU1RK2_PI                               (DRAMC_AO_BASE_ADDRESS + 0x00C0C)
    #define SHU1RK2_PI_RK2_ARPI_DQ_B1                          Fld(6, 0) //[5:0]
    #define SHU1RK2_PI_RK2_ARPI_DQ_B0                          Fld(6, 8) //[13:8]
    #define SHU1RK2_PI_RK2_ARPI_DQM_B1                         Fld(6, 16) //[21:16]
    #define SHU1RK2_PI_RK2_ARPI_DQM_B0                         Fld(6, 24) //[29:24]

#define DRAMC_REG_SHU1RK2_DQSOSC                           (DRAMC_AO_BASE_ADDRESS + 0x00C10)
    #define SHU1RK2_DQSOSC_DQSOSC_BASE_RK2                     Fld(16, 0) //[15:0]
    #define SHU1RK2_DQSOSC_DQSOSC_BASE_RK2_B1                  Fld(16, 16) //[31:16]

#define DRAMC_REG_SHURK2_SELPH_ODTEN0                      (DRAMC_AO_BASE_ADDRESS + 0x00C1C)
    #define SHURK2_SELPH_ODTEN0_TXDLY_B0_R2RODTEN              Fld(3, 0) //[2:0]
    #define SHURK2_SELPH_ODTEN0_TXDLY_B0_R2RODTEN_P1           Fld(3, 4) //[6:4]
    #define SHURK2_SELPH_ODTEN0_TXDLY_B1_R2RODTEN              Fld(3, 8) //[10:8]
    #define SHURK2_SELPH_ODTEN0_TXDLY_B1_R2RODTEN_P1           Fld(3, 12) //[14:12]
    #define SHURK2_SELPH_ODTEN0_TXDLY_B2_R2RODTEN              Fld(3, 16) //[18:16]
    #define SHURK2_SELPH_ODTEN0_TXDLY_B2_R2RODTEN_P1           Fld(3, 20) //[22:20]
    #define SHURK2_SELPH_ODTEN0_TXDLY_B3_R2RODTEN              Fld(3, 24) //[26:24]
    #define SHURK2_SELPH_ODTEN0_TXDLY_B3_R2RODTEN_P1           Fld(3, 28) //[30:28]

#define DRAMC_REG_SHURK2_SELPH_ODTEN1                      (DRAMC_AO_BASE_ADDRESS + 0x00C20)
    #define SHURK2_SELPH_ODTEN1_DLY_B0_R2RODTEN                Fld(3, 0) //[2:0]
    #define SHURK2_SELPH_ODTEN1_DLY_B0_R2RODTEN_P1             Fld(3, 4) //[6:4]
    #define SHURK2_SELPH_ODTEN1_DLY_B1_R2RODTEN                Fld(3, 8) //[10:8]
    #define SHURK2_SELPH_ODTEN1_DLY_B1_R2RODTEN_P1             Fld(3, 12) //[14:12]
    #define SHURK2_SELPH_ODTEN1_DLY_B2_R2RODTEN                Fld(3, 16) //[18:16]
    #define SHURK2_SELPH_ODTEN1_DLY_B2_R2RODTEN_P1             Fld(3, 20) //[22:20]
    #define SHURK2_SELPH_ODTEN1_DLY_B3_R2RODTEN                Fld(3, 24) //[26:24]
    #define SHURK2_SELPH_ODTEN1_DLY_B3_R2RODTEN_P1             Fld(3, 28) //[30:28]

#define DRAMC_REG_SHURK2_SELPH_DQSG0                       (DRAMC_AO_BASE_ADDRESS + 0x00C24)
    #define SHURK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED             Fld(3, 0) //[2:0]
    #define SHURK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED_P1          Fld(3, 4) //[6:4]
    #define SHURK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED             Fld(3, 8) //[10:8]
    #define SHURK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED_P1          Fld(3, 12) //[14:12]
    #define SHURK2_SELPH_DQSG0_TX_DLY_R2DQS2_GATED             Fld(3, 16) //[18:16]
    #define SHURK2_SELPH_DQSG0_TX_DLY_R2DQS2_GATED_P1          Fld(3, 20) //[22:20]
    #define SHURK2_SELPH_DQSG0_TX_DLY_R2DQS3_GATED             Fld(3, 24) //[26:24]
    #define SHURK2_SELPH_DQSG0_TX_DLY_R2DQS3_GATED_P1          Fld(3, 28) //[30:28]

#define DRAMC_REG_SHURK2_SELPH_DQSG1                       (DRAMC_AO_BASE_ADDRESS + 0x00C28)
    #define SHURK2_SELPH_DQSG1_REG_DLY_R2DQS0_GATED            Fld(3, 0) //[2:0]
    #define SHURK2_SELPH_DQSG1_REG_DLY_R2DQS0_GATED_P1         Fld(3, 4) //[6:4]
    #define SHURK2_SELPH_DQSG1_REG_DLY_R2DQS1_GATED            Fld(3, 8) //[10:8]
    #define SHURK2_SELPH_DQSG1_REG_DLY_R2DQS1_GATED_P1         Fld(3, 12) //[14:12]
    #define SHURK2_SELPH_DQSG1_REG_DLY_R2DQS2_GATED            Fld(3, 16) //[18:16]
    #define SHURK2_SELPH_DQSG1_REG_DLY_R2DQS2_GATED_P1         Fld(3, 20) //[22:20]
    #define SHURK2_SELPH_DQSG1_REG_DLY_R2DQS3_GATED            Fld(3, 24) //[26:24]
    #define SHURK2_SELPH_DQSG1_REG_DLY_R2DQS3_GATED_P1         Fld(3, 28) //[30:28]

#define DRAMC_REG_SHURK2_SELPH_DQ0                         (DRAMC_AO_BASE_ADDRESS + 0x00C2C)
    #define SHURK2_SELPH_DQ0_TX_DLY_R2DQ0                      Fld(3, 0) //[2:0]
    #define SHURK2_SELPH_DQ0_TX_DLY_R2DQ1                      Fld(3, 4) //[6:4]
    #define SHURK2_SELPH_DQ0_TX_DLY_R2DQ2                      Fld(3, 8) //[10:8]
    #define SHURK2_SELPH_DQ0_TX_DLY_R2DQ3                      Fld(3, 12) //[14:12]
    #define SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ0                  Fld(3, 16) //[18:16]
    #define SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ1                  Fld(3, 20) //[22:20]
    #define SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ2                  Fld(3, 24) //[26:24]
    #define SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ3                  Fld(3, 28) //[30:28]

#define DRAMC_REG_SHURK2_SELPH_DQ1                         (DRAMC_AO_BASE_ADDRESS + 0x00C30)
    #define SHURK2_SELPH_DQ1_TX_DLY_R2DQM0                     Fld(3, 0) //[2:0]
    #define SHURK2_SELPH_DQ1_TX_DLY_R2DQM1                     Fld(3, 4) //[6:4]
    #define SHURK2_SELPH_DQ1_TX_DLY_R2DQM2                     Fld(3, 8) //[10:8]
    #define SHURK2_SELPH_DQ1_TX_DLY_R2DQM3                     Fld(3, 12) //[14:12]
    #define SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM0                 Fld(3, 16) //[18:16]
    #define SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM1                 Fld(3, 20) //[22:20]
    #define SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM2                 Fld(3, 24) //[26:24]
    #define SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM3                 Fld(3, 28) //[30:28]

#define DRAMC_REG_SHURK2_SELPH_DQ2                         (DRAMC_AO_BASE_ADDRESS + 0x00C34)
    #define SHURK2_SELPH_DQ2_DLY_R2DQ0                         Fld(3, 0) //[2:0]
    #define SHURK2_SELPH_DQ2_DLY_R2DQ1                         Fld(3, 4) //[6:4]
    #define SHURK2_SELPH_DQ2_DLY_R2DQ2                         Fld(3, 8) //[10:8]
    #define SHURK2_SELPH_DQ2_DLY_R2DQ3                         Fld(3, 12) //[14:12]
    #define SHURK2_SELPH_DQ2_DLY_R2OEN_DQ0                     Fld(3, 16) //[18:16]
    #define SHURK2_SELPH_DQ2_DLY_R2OEN_DQ1                     Fld(3, 20) //[22:20]
    #define SHURK2_SELPH_DQ2_DLY_R2OEN_DQ2                     Fld(3, 24) //[26:24]
    #define SHURK2_SELPH_DQ2_DLY_R2OEN_DQ3                     Fld(3, 28) //[30:28]

#define DRAMC_REG_SHURK2_SELPH_DQ3                         (DRAMC_AO_BASE_ADDRESS + 0x00C38)
    #define SHURK2_SELPH_DQ3_DLY_R2DQM0                        Fld(3, 0) //[2:0]
    #define SHURK2_SELPH_DQ3_DLY_R2DQM1                        Fld(3, 4) //[6:4]
    #define SHURK2_SELPH_DQ3_DLY_R2DQM2                        Fld(3, 8) //[10:8]
    #define SHURK2_SELPH_DQ3_DLY_R2DQM3                        Fld(3, 12) //[14:12]
    #define SHURK2_SELPH_DQ3_DLY_R2OEN_DQM0                    Fld(3, 16) //[18:16]
    #define SHURK2_SELPH_DQ3_DLY_R2OEN_DQM1                    Fld(3, 20) //[22:20]
    #define SHURK2_SELPH_DQ3_DLY_R2OEN_DQM2                    Fld(3, 24) //[26:24]
    #define SHURK2_SELPH_DQ3_DLY_R2OEN_DQM3                    Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU1RK2_DQS2DQ_CAL1                      (DRAMC_AO_BASE_ADDRESS + 0x00C40)
    #define SHU1RK2_DQS2DQ_CAL1_BOOT_ORIG_UI_RK2_DQ0           Fld(11, 0) //[10:0]
    #define SHU1RK2_DQS2DQ_CAL1_BOOT_ORIG_UI_RK2_DQ1           Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU1RK2_DQS2DQ_CAL2                      (DRAMC_AO_BASE_ADDRESS + 0x00C44)
    #define SHU1RK2_DQS2DQ_CAL2_BOOT_TARG_UI_RK2_DQ0           Fld(11, 0) //[10:0]
    #define SHU1RK2_DQS2DQ_CAL2_BOOT_TARG_UI_RK2_DQ1           Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU1RK2_DQS2DQ_CAL3                      (DRAMC_AO_BASE_ADDRESS + 0x00C48)
    #define SHU1RK2_DQS2DQ_CAL3_BOOT_TARG_UI_RK2_OEN_DQ0       Fld(6, 0) //[5:0]
    #define SHU1RK2_DQS2DQ_CAL3_BOOT_TARG_UI_RK2_OEN_DQ1       Fld(6, 6) //[11:6]

#define DRAMC_REG_SHU1RK2_DQS2DQ_CAL4                      (DRAMC_AO_BASE_ADDRESS + 0x00C4C)
    #define SHU1RK2_DQS2DQ_CAL4_BOOT_TARG_UI_RK2_OEN_DQM0      Fld(6, 0) //[5:0]
    #define SHU1RK2_DQS2DQ_CAL4_BOOT_TARG_UI_RK2_OEN_DQM1      Fld(6, 6) //[11:6]

#define DRAMC_REG_SHU1RK2_DQS2DQ_CAL5                      (DRAMC_AO_BASE_ADDRESS + 0x00C50)
    #define SHU1RK2_DQS2DQ_CAL5_BOOT_TARG_UI_RK2_DQM0          Fld(11, 0) //[10:0]
    #define SHU1RK2_DQS2DQ_CAL5_BOOT_TARG_UI_RK2_DQM1          Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU_DQSG_RETRY                           (DRAMC_AO_BASE_ADDRESS + 0x00C54)
    #define SHU_DQSG_RETRY_R_DQSGRETRY_SW_RESET                Fld(1, 0) //[0:0]
    #define SHU_DQSG_RETRY_R_DQSG_RETRY_SW_EN                  Fld(1, 1) //[1:1]
    #define SHU_DQSG_RETRY_R_DDR1866_PLUS                      Fld(1, 2) //[2:2]
    #define SHU_DQSG_RETRY_R_RETRY_ONCE                        Fld(1, 3) //[3:3]
    #define SHU_DQSG_RETRY_R_RETRY_3TIMES                      Fld(1, 4) //[4:4]
    #define SHU_DQSG_RETRY_R_RETRY_1RANK                       Fld(1, 5) //[5:5]
    #define SHU_DQSG_RETRY_R_RETRY_SAV_MSK                     Fld(1, 6) //[6:6]
    #define SHU_DQSG_RETRY_R_DM4BYTE                           Fld(1, 7) //[7:7]
    #define SHU_DQSG_RETRY_R_DQSIENLAT                         Fld(4, 8) //[11:8]
    #define SHU_DQSG_RETRY_R_STBENCMP_ALLBYTE                  Fld(1, 12) //[12:12]
    #define SHU_DQSG_RETRY_R_XSR_DQSG_RETRY_EN                 Fld(1, 13) //[13:13]
    #define SHU_DQSG_RETRY_R_XSR_RETRY_SPM_MODE                Fld(1, 14) //[14:14]
    #define SHU_DQSG_RETRY_R_RETRY_CMP_DATA                    Fld(1, 15) //[15:15]
    #define SHU_DQSG_RETRY_R_RETRY_ALE_BLOCK_MASK              Fld(1, 20) //[20:20]
    #define SHU_DQSG_RETRY_R_RDY_SEL_DLE                       Fld(1, 21) //[21:21]
    #define SHU_DQSG_RETRY_R_RETRY_ROUND_NUM                   Fld(2, 24) //[25:24]
    #define SHU_DQSG_RETRY_R_RETRY_RANKSEL_FROM_PHY            Fld(1, 28) //[28:28]
    #define SHU_DQSG_RETRY_R_RETRY_PA_DSIABLE                  Fld(1, 29) //[29:29]
    #define SHU_DQSG_RETRY_R_RETRY_STBEN_RESET_MSK             Fld(1, 30) //[30:30]
    #define SHU_DQSG_RETRY_R_RETRY_USE_BURST_MDOE              Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU2_ACTIM0                              (DRAMC_AO_BASE_ADDRESS + 0x00E00)
    #define SHU2_ACTIM0_TWTR                                   Fld(4, 0) //[3:0]
    #define SHU2_ACTIM0_TWR                                    Fld(5, 8) //[12:8]
    #define SHU2_ACTIM0_TRRD                                   Fld(3, 16) //[18:16]
    #define SHU2_ACTIM0_TRCD                                   Fld(4, 24) //[27:24]

#define DRAMC_REG_SHU2_ACTIM1                              (DRAMC_AO_BASE_ADDRESS + 0x00E04)
    #define SHU2_ACTIM1_TRPAB                                  Fld(3, 0) //[2:0]
    #define SHU2_ACTIM1_TRP                                    Fld(4, 8) //[11:8]
    #define SHU2_ACTIM1_TRAS                                   Fld(4, 16) //[19:16]
    #define SHU2_ACTIM1_TRC                                    Fld(5, 24) //[28:24]

#define DRAMC_REG_SHU2_ACTIM2                              (DRAMC_AO_BASE_ADDRESS + 0x00E08)
    #define SHU2_ACTIM2_TXP                                    Fld(3, 0) //[2:0]
    #define SHU2_ACTIM2_TRTP                                   Fld(3, 8) //[10:8]
    #define SHU2_ACTIM2_TR2W                                   Fld(4, 16) //[19:16]
    #define SHU2_ACTIM2_TFAW                                   Fld(5, 24) //[28:24]

#define DRAMC_REG_SHU2_ACTIM3                              (DRAMC_AO_BASE_ADDRESS + 0x00E0C)
    #define SHU2_ACTIM3_TRFCPB                                 Fld(8, 0) //[7:0]
    #define SHU2_ACTIM3_TRFC                                   Fld(8, 16) //[23:16]
    #define SHU2_ACTIM3_REFCNT                                 Fld(8, 24) //[31:24]

#define DRAMC_REG_SHU2_ACTIM4                              (DRAMC_AO_BASE_ADDRESS + 0x00E10)
    #define SHU2_ACTIM4_TXREFCNT                               Fld(10, 0) //[9:0]
    #define SHU2_ACTIM4_REFCNT_FR_CLK                          Fld(8, 16) //[23:16]
    #define SHU2_ACTIM4_TZQCS                                  Fld(8, 24) //[31:24]

#define DRAMC_REG_SHU2_ACTIM5                              (DRAMC_AO_BASE_ADDRESS + 0x00E14)
    #define SHU2_ACTIM5_TR2PD                                  Fld(5, 0) //[4:0]
    #define SHU2_ACTIM5_TWTPD                                  Fld(5, 8) //[12:8]
    #define SHU2_ACTIM5_TMRR2W                                 Fld(4, 24) //[27:24]

#define DRAMC_REG_SHU2_ACTIM6                              (DRAMC_AO_BASE_ADDRESS + 0x00E18)
    #define SHU2_ACTIM6_BGTCCD                                 Fld(2, 0) //[1:0]
    #define SHU2_ACTIM6_BGTWTR                                 Fld(4, 4) //[7:4]
    #define SHU2_ACTIM6_TWRMPR                                 Fld(4, 8) //[11:8]
    #define SHU2_ACTIM6_BGTRRD                                 Fld(3, 12) //[14:12]

#define DRAMC_REG_SHU2_ACTIM_XRT                           (DRAMC_AO_BASE_ADDRESS + 0x00E1C)
    #define SHU2_ACTIM_XRT_XRTR2R                              Fld(5, 0) //[4:0]
    #define SHU2_ACTIM_XRT_XRTR2W                              Fld(4, 8) //[11:8]
    #define SHU2_ACTIM_XRT_XRTW2R                              Fld(3, 16) //[18:16]
    #define SHU2_ACTIM_XRT_XRTW2W                              Fld(4, 24) //[27:24]

#define DRAMC_REG_SHU2_AC_TIME_05T                         (DRAMC_AO_BASE_ADDRESS + 0x00E20)
    #define SHU2_AC_TIME_05T_TRC_05T                           Fld(1, 0) //[0:0]
    #define SHU2_AC_TIME_05T_TRFCPB_05T                        Fld(1, 1) //[1:1]
    #define SHU2_AC_TIME_05T_TRFC_05T                          Fld(1, 2) //[2:2]
    #define SHU2_AC_TIME_05T_TXP_05T                           Fld(1, 4) //[4:4]
    #define SHU2_AC_TIME_05T_TRTP_05T                          Fld(1, 5) //[5:5]
    #define SHU2_AC_TIME_05T_TRCD_05T                          Fld(1, 6) //[6:6]
    #define SHU2_AC_TIME_05T_TRP_05T                           Fld(1, 7) //[7:7]
    #define SHU2_AC_TIME_05T_TRPAB_05T                         Fld(1, 8) //[8:8]
    #define SHU2_AC_TIME_05T_TRAS_05T                          Fld(1, 9) //[9:9]
    #define SHU2_AC_TIME_05T_TWR_M05T                          Fld(1, 10) //[10:10]
    #define SHU2_AC_TIME_05T_TRRD_05T                          Fld(1, 12) //[12:12]
    #define SHU2_AC_TIME_05T_TFAW_05T                          Fld(1, 13) //[13:13]
    #define SHU2_AC_TIME_05T_TR2PD_05T                         Fld(1, 15) //[15:15]
    #define SHU2_AC_TIME_05T_TWTPD_M05T                        Fld(1, 16) //[16:16]
    #define SHU2_AC_TIME_05T_BGTRRD_05T                        Fld(1, 21) //[21:21]
    #define SHU2_AC_TIME_05T_BGTCCD_05T                        Fld(1, 22) //[22:22]
    #define SHU2_AC_TIME_05T_BGTWTR_05T                        Fld(1, 23) //[23:23]
    #define SHU2_AC_TIME_05T_TR2W_05T                          Fld(1, 24) //[24:24]
    #define SHU2_AC_TIME_05T_TWTR_M05T                         Fld(1, 25) //[25:25]
    #define SHU2_AC_TIME_05T_XRTR2W_05T                        Fld(1, 26) //[26:26]
    #define SHU2_AC_TIME_05T_XRTW2R_M05T                       Fld(1, 27) //[27:27]

#define DRAMC_REG_SHU2_AC_DERATING0                        (DRAMC_AO_BASE_ADDRESS + 0x00E24)
    #define SHU2_AC_DERATING0_ACDERATEEN                       Fld(1, 0) //[0:0]
    #define SHU2_AC_DERATING0_TRRD_DERATE                      Fld(3, 16) //[18:16]
    #define SHU2_AC_DERATING0_TRCD_DERATE                      Fld(4, 24) //[27:24]

#define DRAMC_REG_SHU2_AC_DERATING1                        (DRAMC_AO_BASE_ADDRESS + 0x00E28)
    #define SHU2_AC_DERATING1_TRPAB_DERATE                     Fld(3, 0) //[2:0]
    #define SHU2_AC_DERATING1_TRP_DERATE                       Fld(4, 8) //[11:8]
    #define SHU2_AC_DERATING1_TRAS_DERATE                      Fld(4, 16) //[19:16]
    #define SHU2_AC_DERATING1_TRC_DERATE                       Fld(5, 24) //[28:24]

#define DRAMC_REG_SHU2_AC_DERATING_05T                     (DRAMC_AO_BASE_ADDRESS + 0x00E30)
    #define SHU2_AC_DERATING_05T_TRC_05T_DERATE                Fld(1, 0) //[0:0]
    #define SHU2_AC_DERATING_05T_TRCD_05T_DERATE               Fld(1, 6) //[6:6]
    #define SHU2_AC_DERATING_05T_TRP_05T_DERATE                Fld(1, 7) //[7:7]
    #define SHU2_AC_DERATING_05T_TRPAB_05T_DERATE              Fld(1, 8) //[8:8]
    #define SHU2_AC_DERATING_05T_TRAS_05T_DERATE               Fld(1, 9) //[9:9]
    #define SHU2_AC_DERATING_05T_TRRD_05T_DERATE               Fld(1, 12) //[12:12]

#define DRAMC_REG_SHU2_CONF0                               (DRAMC_AO_BASE_ADDRESS + 0x00E40)
    #define SHU2_CONF0_DMPGTIM                                 Fld(6, 0) //[5:0]
    #define SHU2_CONF0_ADVREFEN                                Fld(1, 6) //[6:6]
    #define SHU2_CONF0_ADVPREEN                                Fld(1, 7) //[7:7]
    #define SHU2_CONF0_TRFCPBIG                                Fld(1, 9) //[9:9]
    #define SHU2_CONF0_REFTHD                                  Fld(4, 12) //[15:12]
    #define SHU2_CONF0_REQQUE_DEPTH                            Fld(4, 16) //[19:16]
    #define SHU2_CONF0_FREQDIV4                                Fld(1, 24) //[24:24]
    #define SHU2_CONF0_FDIV2                                   Fld(1, 25) //[25:25]
    #define SHU2_CONF0_CL2                                     Fld(1, 27) //[27:27]
    #define SHU2_CONF0_BL2                                     Fld(1, 28) //[28:28]
    #define SHU2_CONF0_BL4                                     Fld(1, 29) //[29:29]
    #define SHU2_CONF0_MATYPE                                  Fld(2, 30) //[31:30]

#define DRAMC_REG_SHU2_CONF1                               (DRAMC_AO_BASE_ADDRESS + 0x00E44)
    #define SHU2_CONF1_DATLAT                                  Fld(5, 0) //[4:0]
    #define SHU2_CONF1_DATLAT_DSEL                             Fld(5, 8) //[12:8]
    #define SHU2_CONF1_REFBW_FR                                Fld(10, 16) //[25:16]
    #define SHU2_CONF1_DATLAT_DSEL_PHY                         Fld(5, 26) //[30:26]
    #define SHU2_CONF1_TREFBWIG                                Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU2_CONF2                               (DRAMC_AO_BASE_ADDRESS + 0x00E48)
    #define SHU2_CONF2_TCMDO1LAT                               Fld(8, 0) //[7:0]
    #define SHU2_CONF2_FSPCHG_PRDCNT                           Fld(8, 8) //[15:8]
    #define SHU2_CONF2_DCMDLYREF                               Fld(3, 16) //[18:16]
    #define SHU2_CONF2_DQCMD                                   Fld(1, 25) //[25:25]
    #define SHU2_CONF2_DQ16COM1                                Fld(1, 26) //[26:26]
    #define SHU2_CONF2_RA15TOCS1                               Fld(1, 27) //[27:27]
    #define SHU2_CONF2_WPRE2T                                  Fld(1, 28) //[28:28]
    #define SHU2_CONF2_FASTWAKE2                               Fld(1, 29) //[29:29]
    #define SHU2_CONF2_DAREFEN                                 Fld(1, 30) //[30:30]
    #define SHU2_CONF2_FASTWAKE                                Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU2_CONF3                               (DRAMC_AO_BASE_ADDRESS + 0x00E4C)
    #define SHU2_CONF3_ZQCSCNT                                 Fld(16, 0) //[15:0]
    #define SHU2_CONF3_REFRCNT                                 Fld(9, 16) //[24:16]

#define DRAMC_REG_SHU2_STBCAL                              (DRAMC_AO_BASE_ADDRESS + 0x00E50)
    #define SHU2_STBCAL_DMSTBLAT                               Fld(2, 0) //[1:0]
    #define SHU2_STBCAL_PICGLAT                                Fld(3, 4) //[6:4]
    #define SHU2_STBCAL_DQSG_MODE                              Fld(1, 8) //[8:8]

#define DRAMC_REG_SHU2_DQSOSCTHRD                          (DRAMC_AO_BASE_ADDRESS + 0x00E54)
    #define SHU2_DQSOSCTHRD_DQSOSCTHRD_INC_RK0                 Fld(12, 0) //[11:0]
    #define SHU2_DQSOSCTHRD_DQSOSCTHRD_DEC_RK0                 Fld(12, 12) //[23:12]
    #define SHU2_DQSOSCTHRD_DQSOSCTHRD_INC_RK1_7TO0            Fld(8, 24) //[31:24]

#define DRAMC_REG_SHU2_RANKCTL                             (DRAMC_AO_BASE_ADDRESS + 0x00E58)
    #define SHU2_RANKCTL_RANKINCTL_RXDLY                       Fld(4, 0) //[3:0]
    #define SHU2_RANKCTL_TXRANKINCTL_TXDLY                     Fld(4, 8) //[11:8]
    #define SHU2_RANKCTL_TXRANKINCTL                           Fld(4, 12) //[15:12]
    #define SHU2_RANKCTL_TXRANKINCTL_ROOT                      Fld(4, 16) //[19:16]
    #define SHU2_RANKCTL_RANKINCTL                             Fld(4, 20) //[23:20]
    #define SHU2_RANKCTL_RANKINCTL_ROOT1                       Fld(4, 24) //[27:24]
    #define SHU2_RANKCTL_RANKINCTL_PHY                         Fld(4, 28) //[31:28]

#define DRAMC_REG_SHU2_CKECTRL                             (DRAMC_AO_BASE_ADDRESS + 0x00E5C)
    #define SHU2_CKECTRL_CMDCKE                                Fld(3, 16) //[18:16]
    #define SHU2_CKECTRL_CKEPRD                                Fld(3, 20) //[22:20]
    #define SHU2_CKECTRL_TCKESRX                               Fld(2, 24) //[25:24]
    #define SHU2_CKECTRL_SREF_CK_DLY                           Fld(2, 28) //[29:28]

#define DRAMC_REG_SHU2_ODTCTRL                             (DRAMC_AO_BASE_ADDRESS + 0x00E60)
    #define SHU2_ODTCTRL_ROEN                                  Fld(1, 0) //[0:0]
    #define SHU2_ODTCTRL_WOEN                                  Fld(1, 1) //[1:1]
    #define SHU2_ODTCTRL_RODTEN_SELPH_CG_IG                    Fld(1, 2) //[2:2]
    #define SHU2_ODTCTRL_RODTENSTB_SELPH_CG_IG                 Fld(1, 3) //[3:3]
    #define SHU2_ODTCTRL_RODT                                  Fld(4, 4) //[7:4]
    #define SHU2_ODTCTRL_TWODT                                 Fld(7, 16) //[22:16]
    #define SHU2_ODTCTRL_FIXRODT                               Fld(1, 27) //[27:27]
    #define SHU2_ODTCTRL_RODTE2                                Fld(1, 30) //[30:30]
    #define SHU2_ODTCTRL_RODTE                                 Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU2_IMPCAL1                             (DRAMC_AO_BASE_ADDRESS + 0x00E64)
    #define SHU2_IMPCAL1_IMPCAL_CHKCYCLE                       Fld(3, 0) //[2:0]
    #define SHU2_IMPCAL1_IMPDRVP                               Fld(5, 4) //[8:4]
    #define SHU2_IMPCAL1_IMPDRVN                               Fld(5, 11) //[15:11]
    #define SHU2_IMPCAL1_IMPCAL_CALEN_CYCLE                    Fld(3, 17) //[19:17]
    #define SHU2_IMPCAL1_IMPCALCNT                             Fld(8, 20) //[27:20]
    #define SHU2_IMPCAL1_IMPCAL_CALICNT                        Fld(4, 28) //[31:28]

#define DRAMC_REG_SHU2_DQSOSC_PRD                          (DRAMC_AO_BASE_ADDRESS + 0x00E68)
    #define SHU2_DQSOSC_PRD_DQSOSC_PRDCNT                      Fld(10, 0) //[9:0]
    #define SHU2_DQSOSC_PRD_DQSOSCTHRD_INC_RK1_11TO8           Fld(4, 16) //[19:16]
    #define SHU2_DQSOSC_PRD_DQSOSCTHRD_DEC_RK1                 Fld(12, 20) //[31:20]

#define DRAMC_REG_SHU2_DQSOSCR                             (DRAMC_AO_BASE_ADDRESS + 0x00E6C)
    #define SHU2_DQSOSCR_DQSOSCRCNT                            Fld(8, 0) //[7:0]
    #define SHU2_DQSOSCR_DQSOSC_DELTA                          Fld(16, 16) //[31:16]

#define DRAMC_REG_SHU2_DQSOSCR2                            (DRAMC_AO_BASE_ADDRESS + 0x00E70)
    #define SHU2_DQSOSCR2_DQSOSCENCNT                          Fld(16, 0) //[15:0]
    #define SHU2_DQSOSCR2_DQSOSC_ADV_SEL                       Fld(2, 16) //[17:16]
    #define SHU2_DQSOSCR2_DQSOSC_DRS_ADV_SEL                   Fld(2, 18) //[19:18]

#define DRAMC_REG_SHU2_RODTENSTB                           (DRAMC_AO_BASE_ADDRESS + 0x00E74)
    #define SHU2_RODTENSTB_RODTEN_MCK_MODESEL                  Fld(1, 0) //[0:0]
    #define SHU2_RODTENSTB_RODTEN_P1_ENABLE                    Fld(1, 1) //[1:1]
    #define SHU2_RODTENSTB_RODTENSTB_OFFSET                    Fld(6, 2) //[7:2]
    #define SHU2_RODTENSTB_RODTENSTB_EXT                       Fld(16, 8) //[23:8]
    #define SHU2_RODTENSTB_RODTENSTB_4BYTE_EN                  Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU2_PIPE                                (DRAMC_AO_BASE_ADDRESS + 0x00E78)
    #define SHU2_PIPE_PHYRXPIPE1                               Fld(1, 0) //[0:0]
    #define SHU2_PIPE_PHYRXPIPE2                               Fld(1, 1) //[1:1]
    #define SHU2_PIPE_PHYRXPIPE3                               Fld(1, 2) //[2:2]
    #define SHU2_PIPE_PHYRXRDSLPIPE1                           Fld(1, 4) //[4:4]
    #define SHU2_PIPE_PHYRXRDSLPIPE2                           Fld(1, 5) //[5:5]
    #define SHU2_PIPE_PHYRXRDSLPIPE3                           Fld(1, 6) //[6:6]
    #define SHU2_PIPE_PHYPIPE1EN                               Fld(1, 8) //[8:8]
    #define SHU2_PIPE_PHYPIPE2EN                               Fld(1, 9) //[9:9]
    #define SHU2_PIPE_PHYPIPE3EN                               Fld(1, 10) //[10:10]
    #define SHU2_PIPE_DLE_LAST_EXTEND3                         Fld(1, 26) //[26:26]
    #define SHU2_PIPE_READ_START_EXTEND3                       Fld(1, 27) //[27:27]
    #define SHU2_PIPE_DLE_LAST_EXTEND2                         Fld(1, 28) //[28:28]
    #define SHU2_PIPE_READ_START_EXTEND2                       Fld(1, 29) //[29:29]
    #define SHU2_PIPE_DLE_LAST_EXTEND1                         Fld(1, 30) //[30:30]
    #define SHU2_PIPE_READ_START_EXTEND1                       Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU2_TEST1                               (DRAMC_AO_BASE_ADDRESS + 0x00E7C)
    #define SHU2_TEST1_LATNORMPOP                              Fld(5, 8) //[12:8]
    #define SHU2_TEST1_DQSICALBLCOK_CNT                        Fld(3, 20) //[22:20]
    #define SHU2_TEST1_DQSICALI_NEW                            Fld(1, 23) //[23:23]

#define DRAMC_REG_SHU2_SELPH_CA1                           (DRAMC_AO_BASE_ADDRESS + 0x00E80)
    #define SHU2_SELPH_CA1_TXDLY_CS                            Fld(3, 0) //[2:0]
    #define SHU2_SELPH_CA1_TXDLY_CKE                           Fld(3, 4) //[6:4]
    #define SHU2_SELPH_CA1_TXDLY_ODT                           Fld(3, 8) //[10:8]
    #define SHU2_SELPH_CA1_TXDLY_RESET                         Fld(3, 12) //[14:12]
    #define SHU2_SELPH_CA1_TXDLY_WE                            Fld(3, 16) //[18:16]
    #define SHU2_SELPH_CA1_TXDLY_CAS                           Fld(3, 20) //[22:20]
    #define SHU2_SELPH_CA1_TXDLY_RAS                           Fld(3, 24) //[26:24]
    #define SHU2_SELPH_CA1_TXDLY_CS1                           Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2_SELPH_CA2                           (DRAMC_AO_BASE_ADDRESS + 0x00E84)
    #define SHU2_SELPH_CA2_TXDLY_BA0                           Fld(3, 0) //[2:0]
    #define SHU2_SELPH_CA2_TXDLY_BA1                           Fld(3, 4) //[6:4]
    #define SHU2_SELPH_CA2_TXDLY_BA2                           Fld(3, 8) //[10:8]
    #define SHU2_SELPH_CA2_TXDLY_CMD                           Fld(5, 16) //[20:16]
    #define SHU2_SELPH_CA2_TXDLY_CKE1                          Fld(3, 24) //[26:24]

#define DRAMC_REG_SHU2_SELPH_CA3                           (DRAMC_AO_BASE_ADDRESS + 0x00E88)
    #define SHU2_SELPH_CA3_TXDLY_RA0                           Fld(3, 0) //[2:0]
    #define SHU2_SELPH_CA3_TXDLY_RA1                           Fld(3, 4) //[6:4]
    #define SHU2_SELPH_CA3_TXDLY_RA2                           Fld(3, 8) //[10:8]
    #define SHU2_SELPH_CA3_TXDLY_RA3                           Fld(3, 12) //[14:12]
    #define SHU2_SELPH_CA3_TXDLY_RA4                           Fld(3, 16) //[18:16]
    #define SHU2_SELPH_CA3_TXDLY_RA5                           Fld(3, 20) //[22:20]
    #define SHU2_SELPH_CA3_TXDLY_RA6                           Fld(3, 24) //[26:24]
    #define SHU2_SELPH_CA3_TXDLY_RA7                           Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2_SELPH_CA4                           (DRAMC_AO_BASE_ADDRESS + 0x00E8C)
    #define SHU2_SELPH_CA4_TXDLY_RA8                           Fld(3, 0) //[2:0]
    #define SHU2_SELPH_CA4_TXDLY_RA9                           Fld(3, 4) //[6:4]
    #define SHU2_SELPH_CA4_TXDLY_RA10                          Fld(3, 8) //[10:8]
    #define SHU2_SELPH_CA4_TXDLY_RA11                          Fld(3, 12) //[14:12]
    #define SHU2_SELPH_CA4_TXDLY_RA12                          Fld(3, 16) //[18:16]
    #define SHU2_SELPH_CA4_TXDLY_RA13                          Fld(3, 20) //[22:20]
    #define SHU2_SELPH_CA4_TXDLY_RA14                          Fld(3, 24) //[26:24]
    #define SHU2_SELPH_CA4_TXDLY_RA15                          Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2_SELPH_CA5                           (DRAMC_AO_BASE_ADDRESS + 0x00E90)
    #define SHU2_SELPH_CA5_DLY_CS                              Fld(3, 0) //[2:0]
    #define SHU2_SELPH_CA5_DLY_CKE                             Fld(3, 4) //[6:4]
    #define SHU2_SELPH_CA5_DLY_ODT                             Fld(3, 8) //[10:8]
    #define SHU2_SELPH_CA5_DLY_RESET                           Fld(3, 12) //[14:12]
    #define SHU2_SELPH_CA5_DLY_WE                              Fld(3, 16) //[18:16]
    #define SHU2_SELPH_CA5_DLY_CAS                             Fld(3, 20) //[22:20]
    #define SHU2_SELPH_CA5_DLY_RAS                             Fld(3, 24) //[26:24]
    #define SHU2_SELPH_CA5_DLY_CS1                             Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2_SELPH_CA6                           (DRAMC_AO_BASE_ADDRESS + 0x00E94)
    #define SHU2_SELPH_CA6_DLY_BA0                             Fld(3, 0) //[2:0]
    #define SHU2_SELPH_CA6_DLY_BA1                             Fld(3, 4) //[6:4]
    #define SHU2_SELPH_CA6_DLY_BA2                             Fld(3, 8) //[10:8]
    #define SHU2_SELPH_CA6_DLY_CKE1                            Fld(3, 24) //[26:24]

#define DRAMC_REG_SHU2_SELPH_CA7                           (DRAMC_AO_BASE_ADDRESS + 0x00E98)
    #define SHU2_SELPH_CA7_DLY_RA0                             Fld(3, 0) //[2:0]
    #define SHU2_SELPH_CA7_DLY_RA1                             Fld(3, 4) //[6:4]
    #define SHU2_SELPH_CA7_DLY_RA2                             Fld(3, 8) //[10:8]
    #define SHU2_SELPH_CA7_DLY_RA3                             Fld(3, 12) //[14:12]
    #define SHU2_SELPH_CA7_DLY_RA4                             Fld(3, 16) //[18:16]
    #define SHU2_SELPH_CA7_DLY_RA5                             Fld(3, 20) //[22:20]
    #define SHU2_SELPH_CA7_DLY_RA6                             Fld(3, 24) //[26:24]
    #define SHU2_SELPH_CA7_DLY_RA7                             Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2_SELPH_CA8                           (DRAMC_AO_BASE_ADDRESS + 0x00E9C)
    #define SHU2_SELPH_CA8_DLY_RA8                             Fld(3, 0) //[2:0]
    #define SHU2_SELPH_CA8_DLY_RA9                             Fld(3, 4) //[6:4]
    #define SHU2_SELPH_CA8_DLY_RA10                            Fld(3, 8) //[10:8]
    #define SHU2_SELPH_CA8_DLY_RA11                            Fld(3, 12) //[14:12]
    #define SHU2_SELPH_CA8_DLY_RA12                            Fld(3, 16) //[18:16]
    #define SHU2_SELPH_CA8_DLY_RA13                            Fld(3, 20) //[22:20]
    #define SHU2_SELPH_CA8_DLY_RA14                            Fld(3, 24) //[26:24]
    #define SHU2_SELPH_CA8_DLY_RA15                            Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2_SELPH_DQS0                          (DRAMC_AO_BASE_ADDRESS + 0x00EA0)
    #define SHU2_SELPH_DQS0_TXDLY_DQS0                         Fld(3, 0) //[2:0]
    #define SHU2_SELPH_DQS0_TXDLY_DQS1                         Fld(3, 4) //[6:4]
    #define SHU2_SELPH_DQS0_TXDLY_DQS2                         Fld(3, 8) //[10:8]
    #define SHU2_SELPH_DQS0_TXDLY_DQS3                         Fld(3, 12) //[14:12]
    #define SHU2_SELPH_DQS0_TXDLY_OEN_DQS0                     Fld(3, 16) //[18:16]
    #define SHU2_SELPH_DQS0_TXDLY_OEN_DQS1                     Fld(3, 20) //[22:20]
    #define SHU2_SELPH_DQS0_TXDLY_OEN_DQS2                     Fld(3, 24) //[26:24]
    #define SHU2_SELPH_DQS0_TXDLY_OEN_DQS3                     Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2_SELPH_DQS1                          (DRAMC_AO_BASE_ADDRESS + 0x00EA4)
    #define SHU2_SELPH_DQS1_DLY_DQS0                           Fld(3, 0) //[2:0]
    #define SHU2_SELPH_DQS1_DLY_DQS1                           Fld(3, 4) //[6:4]
    #define SHU2_SELPH_DQS1_DLY_DQS2                           Fld(3, 8) //[10:8]
    #define SHU2_SELPH_DQS1_DLY_DQS3                           Fld(3, 12) //[14:12]
    #define SHU2_SELPH_DQS1_DLY_OEN_DQS0                       Fld(3, 16) //[18:16]
    #define SHU2_SELPH_DQS1_DLY_OEN_DQS1                       Fld(3, 20) //[22:20]
    #define SHU2_SELPH_DQS1_DLY_OEN_DQS2                       Fld(3, 24) //[26:24]
    #define SHU2_SELPH_DQS1_DLY_OEN_DQS3                       Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2_DRVING1                             (DRAMC_AO_BASE_ADDRESS + 0x00EA8)
    #define SHU2_DRVING1_DQDRVN2                               Fld(5, 0) //[4:0]
    #define SHU2_DRVING1_DQDRVP2                               Fld(5, 5) //[9:5]
    #define SHU2_DRVING1_DQSDRVN1                              Fld(5, 10) //[14:10]
    #define SHU2_DRVING1_DQSDRVP1                              Fld(5, 15) //[19:15]
    #define SHU2_DRVING1_DQSDRVN2                              Fld(5, 20) //[24:20]
    #define SHU2_DRVING1_DQSDRVP2                              Fld(5, 25) //[29:25]
    #define SHU2_DRVING1_DIS_IMP_ODTN_TRACK                    Fld(1, 30) //[30:30]
    #define SHU2_DRVING1_DIS_IMPCAL_HW                         Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU2_DRVING2                             (DRAMC_AO_BASE_ADDRESS + 0x00EAC)
    #define SHU2_DRVING2_CMDDRVN1                              Fld(5, 0) //[4:0]
    #define SHU2_DRVING2_CMDDRVP1                              Fld(5, 5) //[9:5]
    #define SHU2_DRVING2_CMDDRVN2                              Fld(5, 10) //[14:10]
    #define SHU2_DRVING2_CMDDRVP2                              Fld(5, 15) //[19:15]
    #define SHU2_DRVING2_DQDRVN1                               Fld(5, 20) //[24:20]
    #define SHU2_DRVING2_DQDRVP1                               Fld(5, 25) //[29:25]
    #define SHU2_DRVING2_DIS_IMPCAL_ODT_EN                     Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU2_DRVING3                             (DRAMC_AO_BASE_ADDRESS + 0x00EB0)
    #define SHU2_DRVING3_DQODTN2                               Fld(5, 0) //[4:0]
    #define SHU2_DRVING3_DQODTP2                               Fld(5, 5) //[9:5]
    #define SHU2_DRVING3_DQSODTN                               Fld(5, 10) //[14:10]
    #define SHU2_DRVING3_DQSODTP                               Fld(5, 15) //[19:15]
    #define SHU2_DRVING3_DQSODTN2                              Fld(5, 20) //[24:20]
    #define SHU2_DRVING3_DQSODTP2                              Fld(5, 25) //[29:25]

#define DRAMC_REG_SHU2_DRVING4                             (DRAMC_AO_BASE_ADDRESS + 0x00EB4)
    #define SHU2_DRVING4_CMDODTN1                              Fld(5, 0) //[4:0]
    #define SHU2_DRVING4_CMDODTP1                              Fld(5, 5) //[9:5]
    #define SHU2_DRVING4_CMDODTN2                              Fld(5, 10) //[14:10]
    #define SHU2_DRVING4_CMDODTP2                              Fld(5, 15) //[19:15]
    #define SHU2_DRVING4_DQODTN1                               Fld(5, 20) //[24:20]
    #define SHU2_DRVING4_DQODTP1                               Fld(5, 25) //[29:25]

#define DRAMC_REG_SHU2_DRVING5                             (DRAMC_AO_BASE_ADDRESS + 0x00EB8)
    #define SHU2_DRVING5_DQCODTN2                              Fld(5, 0) //[4:0]
    #define SHU2_DRVING5_DQCODTP2                              Fld(5, 5) //[9:5]
    #define SHU2_DRVING5_DQCDRVN1                              Fld(5, 10) //[14:10]
    #define SHU2_DRVING5_DQCDRVP1                              Fld(5, 15) //[19:15]
    #define SHU2_DRVING5_DQCDRVN2                              Fld(5, 20) //[24:20]
    #define SHU2_DRVING5_DQCDRVP2                              Fld(5, 25) //[29:25]

#define DRAMC_REG_SHU2_DRVING6                             (DRAMC_AO_BASE_ADDRESS + 0x00EBC)
    #define SHU2_DRVING6_DQCODTN1                              Fld(5, 20) //[24:20]
    #define SHU2_DRVING6_DQCODTP1                              Fld(5, 25) //[29:25]

#define DRAMC_REG_SHU2_WODT                                (DRAMC_AO_BASE_ADDRESS + 0x00EC0)
    #define SHU2_WODT_DISWODT                                  Fld(3, 0) //[2:0]
    #define SHU2_WODT_WODTFIX                                  Fld(1, 3) //[3:3]
    #define SHU2_WODT_WODTFIXOFF                               Fld(1, 4) //[4:4]
    #define SHU2_WODT_DISWODTE                                 Fld(1, 5) //[5:5]
    #define SHU2_WODT_DISWODTE2                                Fld(1, 6) //[6:6]
    #define SHU2_WODT_WODTPDEN                                 Fld(1, 7) //[7:7]
    #define SHU2_WODT_DQOE_CNT                                 Fld(3, 8) //[10:8]
    #define SHU2_WODT_DQOE_OPT                                 Fld(1, 11) //[11:11]
    #define SHU2_WODT_TXUPD_SEL                                Fld(2, 12) //[13:12]
    #define SHU2_WODT_TXUPD_W2R_SEL                            Fld(3, 14) //[16:14]
    #define SHU2_WODT_DBIWR                                    Fld(1, 29) //[29:29]
    #define SHU2_WODT_TWPSTEXT                                 Fld(1, 30) //[30:30]
    #define SHU2_WODT_WPST2T                                   Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU2_DQSG                                (DRAMC_AO_BASE_ADDRESS + 0x00EC4)
    #define SHU2_DQSG_DLLFRZRFCOPT                             Fld(2, 0) //[1:0]
    #define SHU2_DQSG_DLLFRZWROPT                              Fld(2, 4) //[5:4]
    #define SHU2_DQSG_R_RSTBCNT_LATCH_OPT                      Fld(3, 8) //[10:8]
    #define SHU2_DQSG_STB_UPDMASK_EN                           Fld(1, 11) //[11:11]
    #define SHU2_DQSG_STB_UPDMASKCYC                           Fld(4, 12) //[15:12]
    #define SHU2_DQSG_DQSINCTL_PRE_SEL                         Fld(1, 16) //[16:16]
    #define SHU2_DQSG_SCINTV                                   Fld(6, 20) //[25:20]

#define DRAMC_REG_SHU2_SCINTV                              (DRAMC_AO_BASE_ADDRESS + 0x00EC8)
    #define SHU2_SCINTV_ODTREN                                 Fld(1, 0) //[0:0]
    #define SHU2_SCINTV_TZQLAT                                 Fld(5, 1) //[5:1]
    #define SHU2_SCINTV_TZQLAT2                                Fld(5, 6) //[10:6]
    #define SHU2_SCINTV_RDDQC_INTV                             Fld(2, 11) //[12:11]
    #define SHU2_SCINTV_MRW_INTV                               Fld(5, 13) //[17:13]
    #define SHU2_SCINTV_DQS2DQ_SHU_PITHRD                      Fld(6, 18) //[23:18]
    #define SHU2_SCINTV_DQS2DQ_FILT_PITHRD                     Fld(6, 24) //[29:24]
    #define SHU2_SCINTV_DQSOSCENDIS                            Fld(1, 30) //[30:30]

#define DRAMC_REG_SHU2_MISC                                (DRAMC_AO_BASE_ADDRESS + 0x00ECC)
    #define SHU2_MISC_REQQUE_MAXCNT                            Fld(4, 0) //[3:0]
    #define SHU2_MISC_CKEHCMD                                  Fld(2, 4) //[5:4]
    #define SHU2_MISC_NORMPOP_LEN                              Fld(3, 8) //[10:8]
    #define SHU2_MISC_PREA_INTV                                Fld(5, 12) //[16:12]

#define DRAMC_REG_SHU2_DQS2DQ_TX                           (DRAMC_AO_BASE_ADDRESS + 0x00ED0)
    #define SHU2_DQS2DQ_TX_OE2DQ_OFFSET                        Fld(5, 0) //[4:0]

#define DRAMC_REG_SHU2_HWSET_MR2                           (DRAMC_AO_BASE_ADDRESS + 0x00ED4)
    #define SHU2_HWSET_MR2_HWSET_MR2_MRSMA                     Fld(13, 0) //[12:0]
    #define SHU2_HWSET_MR2_HWSET_MR2_OP                        Fld(8, 16) //[23:16]

#define DRAMC_REG_SHU2_HWSET_MR13                          (DRAMC_AO_BASE_ADDRESS + 0x00ED8)
    #define SHU2_HWSET_MR13_HWSET_MR13_MRSMA                   Fld(13, 0) //[12:0]
    #define SHU2_HWSET_MR13_HWSET_MR13_OP                      Fld(8, 16) //[23:16]

#define DRAMC_REG_SHU2_HWSET_VRCG                          (DRAMC_AO_BASE_ADDRESS + 0x00EDC)
    #define SHU2_HWSET_VRCG_HWSET_VRCG_MRSMA                   Fld(13, 0) //[12:0]
    #define SHU2_HWSET_VRCG_HWSET_VRCG_OP                      Fld(8, 16) //[23:16]

#define DRAMC_REG_SHU2_APHY_TX_PICG_CTRL                   (DRAMC_AO_BASE_ADDRESS + 0x00EE4)
    #define SHU2_APHY_TX_PICG_CTRL_APHYPI_CG_CK_SEL            Fld(4, 20) //[23:20]
    #define SHU2_APHY_TX_PICG_CTRL_APHYPI_CG_CK_OPT            Fld(1, 24) //[24:24]
    #define SHU2_APHY_TX_PICG_CTRL_DDRPHY_CLK_DYN_GATING_SEL   Fld(4, 27) //[30:27]
    #define SHU2_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_OPT           Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU2RK0_DQSCTL                           (DRAMC_AO_BASE_ADDRESS + 0x01000)
    #define SHU2RK0_DQSCTL_DQSINCTL                            Fld(4, 0) //[3:0]

#define DRAMC_REG_SHU2RK0_DQSIEN                           (DRAMC_AO_BASE_ADDRESS + 0x01004)
    #define SHU2RK0_DQSIEN_R0DQS0IEN                           Fld(7, 0) //[6:0]
    #define SHU2RK0_DQSIEN_R0DQS1IEN                           Fld(7, 8) //[14:8]
    #define SHU2RK0_DQSIEN_R0DQS2IEN                           Fld(7, 16) //[22:16]
    #define SHU2RK0_DQSIEN_R0DQS3IEN                           Fld(7, 24) //[30:24]

#define DRAMC_REG_SHU2RK0_DQSCAL                           (DRAMC_AO_BASE_ADDRESS + 0x01008)
    #define SHU2RK0_DQSCAL_R0DQSIENLLMT                        Fld(7, 0) //[6:0]
    #define SHU2RK0_DQSCAL_R0DQSIENLLMTEN                      Fld(1, 7) //[7:7]
    #define SHU2RK0_DQSCAL_R0DQSIENHLMT                        Fld(7, 8) //[14:8]
    #define SHU2RK0_DQSCAL_R0DQSIENHLMTEN                      Fld(1, 15) //[15:15]

#define DRAMC_REG_SHU2RK0_PI                               (DRAMC_AO_BASE_ADDRESS + 0x0100C)
    #define SHU2RK0_PI_RK0_ARPI_DQ_B1                          Fld(6, 0) //[5:0]
    #define SHU2RK0_PI_RK0_ARPI_DQ_B0                          Fld(6, 8) //[13:8]
    #define SHU2RK0_PI_RK0_ARPI_DQM_B1                         Fld(6, 16) //[21:16]
    #define SHU2RK0_PI_RK0_ARPI_DQM_B0                         Fld(6, 24) //[29:24]

#define DRAMC_REG_SHU2RK0_DQSOSC                           (DRAMC_AO_BASE_ADDRESS + 0x01010)
    #define SHU2RK0_DQSOSC_DQSOSC_BASE_RK0                     Fld(16, 0) //[15:0]
    #define SHU2RK0_DQSOSC_DQSOSC_BASE_RK0_B1                  Fld(16, 16) //[31:16]

#define DRAMC_REG_SHU2RK0_SELPH_ODTEN0                     (DRAMC_AO_BASE_ADDRESS + 0x0101C)
    #define SHU2RK0_SELPH_ODTEN0_TXDLY_B0_RODTEN               Fld(3, 0) //[2:0]
    #define SHU2RK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1            Fld(3, 4) //[6:4]
    #define SHU2RK0_SELPH_ODTEN0_TXDLY_B1_RODTEN               Fld(3, 8) //[10:8]
    #define SHU2RK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1            Fld(3, 12) //[14:12]
    #define SHU2RK0_SELPH_ODTEN0_TXDLY_B2_RODTEN               Fld(3, 16) //[18:16]
    #define SHU2RK0_SELPH_ODTEN0_TXDLY_B2_RODTEN_P1            Fld(3, 20) //[22:20]
    #define SHU2RK0_SELPH_ODTEN0_TXDLY_B3_RODTEN               Fld(3, 24) //[26:24]
    #define SHU2RK0_SELPH_ODTEN0_TXDLY_B3_RODTEN_P1            Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2RK0_SELPH_ODTEN1                     (DRAMC_AO_BASE_ADDRESS + 0x01020)
    #define SHU2RK0_SELPH_ODTEN1_DLY_B0_RODTEN                 Fld(3, 0) //[2:0]
    #define SHU2RK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1              Fld(3, 4) //[6:4]
    #define SHU2RK0_SELPH_ODTEN1_DLY_B1_RODTEN                 Fld(3, 8) //[10:8]
    #define SHU2RK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1              Fld(3, 12) //[14:12]
    #define SHU2RK0_SELPH_ODTEN1_DLY_B2_RODTEN                 Fld(3, 16) //[18:16]
    #define SHU2RK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1              Fld(3, 20) //[22:20]
    #define SHU2RK0_SELPH_ODTEN1_DLY_B3_RODTEN                 Fld(3, 24) //[26:24]
    #define SHU2RK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1              Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2RK0_SELPH_DQSG0                      (DRAMC_AO_BASE_ADDRESS + 0x01024)
    #define SHU2RK0_SELPH_DQSG0_TX_DLY_DQS0_GATED              Fld(3, 0) //[2:0]
    #define SHU2RK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1           Fld(3, 4) //[6:4]
    #define SHU2RK0_SELPH_DQSG0_TX_DLY_DQS1_GATED              Fld(3, 8) //[10:8]
    #define SHU2RK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1           Fld(3, 12) //[14:12]
    #define SHU2RK0_SELPH_DQSG0_TX_DLY_DQS2_GATED              Fld(3, 16) //[18:16]
    #define SHU2RK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1           Fld(3, 20) //[22:20]
    #define SHU2RK0_SELPH_DQSG0_TX_DLY_DQS3_GATED              Fld(3, 24) //[26:24]
    #define SHU2RK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1           Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2RK0_SELPH_DQSG1                      (DRAMC_AO_BASE_ADDRESS + 0x01028)
    #define SHU2RK0_SELPH_DQSG1_REG_DLY_DQS0_GATED             Fld(3, 0) //[2:0]
    #define SHU2RK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1          Fld(3, 4) //[6:4]
    #define SHU2RK0_SELPH_DQSG1_REG_DLY_DQS1_GATED             Fld(3, 8) //[10:8]
    #define SHU2RK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1          Fld(3, 12) //[14:12]
    #define SHU2RK0_SELPH_DQSG1_REG_DLY_DQS2_GATED             Fld(3, 16) //[18:16]
    #define SHU2RK0_SELPH_DQSG1_REG_DLY_DQS2_GATED_P1          Fld(3, 20) //[22:20]
    #define SHU2RK0_SELPH_DQSG1_REG_DLY_DQS3_GATED             Fld(3, 24) //[26:24]
    #define SHU2RK0_SELPH_DQSG1_REG_DLY_DQS3_GATED_P1          Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2RK0_SELPH_DQ0                        (DRAMC_AO_BASE_ADDRESS + 0x0102C)
    #define SHU2RK0_SELPH_DQ0_TXDLY_DQ0                        Fld(3, 0) //[2:0]
    #define SHU2RK0_SELPH_DQ0_TXDLY_DQ1                        Fld(3, 4) //[6:4]
    #define SHU2RK0_SELPH_DQ0_TXDLY_DQ2                        Fld(3, 8) //[10:8]
    #define SHU2RK0_SELPH_DQ0_TXDLY_DQ3                        Fld(3, 12) //[14:12]
    #define SHU2RK0_SELPH_DQ0_TXDLY_OEN_DQ0                    Fld(3, 16) //[18:16]
    #define SHU2RK0_SELPH_DQ0_TXDLY_OEN_DQ1                    Fld(3, 20) //[22:20]
    #define SHU2RK0_SELPH_DQ0_TXDLY_OEN_DQ2                    Fld(3, 24) //[26:24]
    #define SHU2RK0_SELPH_DQ0_TXDLY_OEN_DQ3                    Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2RK0_SELPH_DQ1                        (DRAMC_AO_BASE_ADDRESS + 0x01030)
    #define SHU2RK0_SELPH_DQ1_TXDLY_DQM0                       Fld(3, 0) //[2:0]
    #define SHU2RK0_SELPH_DQ1_TXDLY_DQM1                       Fld(3, 4) //[6:4]
    #define SHU2RK0_SELPH_DQ1_TXDLY_DQM2                       Fld(3, 8) //[10:8]
    #define SHU2RK0_SELPH_DQ1_TXDLY_DQM3                       Fld(3, 12) //[14:12]
    #define SHU2RK0_SELPH_DQ1_TXDLY_OEN_DQM0                   Fld(3, 16) //[18:16]
    #define SHU2RK0_SELPH_DQ1_TXDLY_OEN_DQM1                   Fld(3, 20) //[22:20]
    #define SHU2RK0_SELPH_DQ1_TXDLY_OEN_DQM2                   Fld(3, 24) //[26:24]
    #define SHU2RK0_SELPH_DQ1_TXDLY_OEN_DQM3                   Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2RK0_SELPH_DQ2                        (DRAMC_AO_BASE_ADDRESS + 0x01034)
    #define SHU2RK0_SELPH_DQ2_DLY_DQ0                          Fld(3, 0) //[2:0]
    #define SHU2RK0_SELPH_DQ2_DLY_DQ1                          Fld(3, 4) //[6:4]
    #define SHU2RK0_SELPH_DQ2_DLY_DQ2                          Fld(3, 8) //[10:8]
    #define SHU2RK0_SELPH_DQ2_DLY_DQ3                          Fld(3, 12) //[14:12]
    #define SHU2RK0_SELPH_DQ2_DLY_OEN_DQ0                      Fld(3, 16) //[18:16]
    #define SHU2RK0_SELPH_DQ2_DLY_OEN_DQ1                      Fld(3, 20) //[22:20]
    #define SHU2RK0_SELPH_DQ2_DLY_OEN_DQ2                      Fld(3, 24) //[26:24]
    #define SHU2RK0_SELPH_DQ2_DLY_OEN_DQ3                      Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2RK0_SELPH_DQ3                        (DRAMC_AO_BASE_ADDRESS + 0x01038)
    #define SHU2RK0_SELPH_DQ3_DLY_DQM0                         Fld(3, 0) //[2:0]
    #define SHU2RK0_SELPH_DQ3_DLY_DQM1                         Fld(3, 4) //[6:4]
    #define SHU2RK0_SELPH_DQ3_DLY_DQM2                         Fld(3, 8) //[10:8]
    #define SHU2RK0_SELPH_DQ3_DLY_DQM3                         Fld(3, 12) //[14:12]
    #define SHU2RK0_SELPH_DQ3_DLY_OEN_DQM0                     Fld(3, 16) //[18:16]
    #define SHU2RK0_SELPH_DQ3_DLY_OEN_DQM1                     Fld(3, 20) //[22:20]
    #define SHU2RK0_SELPH_DQ3_DLY_OEN_DQM2                     Fld(3, 24) //[26:24]
    #define SHU2RK0_SELPH_DQ3_DLY_OEN_DQM3                     Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2RK0_DQS2DQ_CAL1                      (DRAMC_AO_BASE_ADDRESS + 0x01040)
    #define SHU2RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0           Fld(11, 0) //[10:0]
    #define SHU2RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1           Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU2RK0_DQS2DQ_CAL2                      (DRAMC_AO_BASE_ADDRESS + 0x01044)
    #define SHU2RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0           Fld(11, 0) //[10:0]
    #define SHU2RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1           Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU2RK0_DQS2DQ_CAL3                      (DRAMC_AO_BASE_ADDRESS + 0x01048)
    #define SHU2RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0       Fld(6, 0) //[5:0]
    #define SHU2RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1       Fld(6, 6) //[11:6]
    #define SHU2RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0_B4TO0 Fld(5, 12) //[16:12]
    #define SHU2RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1_B4TO0 Fld(5, 17) //[21:17]

#define DRAMC_REG_SHU2RK0_DQS2DQ_CAL4                      (DRAMC_AO_BASE_ADDRESS + 0x0104C)
    #define SHU2RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0      Fld(6, 0) //[5:0]
    #define SHU2RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1      Fld(6, 6) //[11:6]
    #define SHU2RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0_B4TO0 Fld(5, 12) //[16:12]
    #define SHU2RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1_B4TO0 Fld(5, 17) //[21:17]

#define DRAMC_REG_SHU2RK0_DQS2DQ_CAL5                      (DRAMC_AO_BASE_ADDRESS + 0x01050)
    #define SHU2RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0          Fld(11, 0) //[10:0]
    #define SHU2RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1          Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU2RK1_DQSCTL                           (DRAMC_AO_BASE_ADDRESS + 0x01100)
    #define SHU2RK1_DQSCTL_R1DQSINCTL                          Fld(4, 0) //[3:0]

#define DRAMC_REG_SHU2RK1_DQSIEN                           (DRAMC_AO_BASE_ADDRESS + 0x01104)
    #define SHU2RK1_DQSIEN_R1DQS0IEN                           Fld(7, 0) //[6:0]
    #define SHU2RK1_DQSIEN_R1DQS1IEN                           Fld(7, 8) //[14:8]
    #define SHU2RK1_DQSIEN_R1DQS2IEN                           Fld(7, 16) //[22:16]
    #define SHU2RK1_DQSIEN_R1DQS3IEN                           Fld(7, 24) //[30:24]

#define DRAMC_REG_SHU2RK1_DQSCAL                           (DRAMC_AO_BASE_ADDRESS + 0x01108)
    #define SHU2RK1_DQSCAL_R1DQSIENLLMT                        Fld(7, 0) //[6:0]
    #define SHU2RK1_DQSCAL_R1DQSIENLLMTEN                      Fld(1, 7) //[7:7]
    #define SHU2RK1_DQSCAL_R1DQSIENHLMT                        Fld(7, 8) //[14:8]
    #define SHU2RK1_DQSCAL_R1DQSIENHLMTEN                      Fld(1, 15) //[15:15]

#define DRAMC_REG_SHU2RK1_PI                               (DRAMC_AO_BASE_ADDRESS + 0x0110C)
    #define SHU2RK1_PI_RK1_ARPI_DQ_B1                          Fld(6, 0) //[5:0]
    #define SHU2RK1_PI_RK1_ARPI_DQ_B0                          Fld(6, 8) //[13:8]
    #define SHU2RK1_PI_RK1_ARPI_DQM_B1                         Fld(6, 16) //[21:16]
    #define SHU2RK1_PI_RK1_ARPI_DQM_B0                         Fld(6, 24) //[29:24]

#define DRAMC_REG_SHU2RK1_DQSOSC                           (DRAMC_AO_BASE_ADDRESS + 0x01110)
    #define SHU2RK1_DQSOSC_DQSOSC_BASE_RK1                     Fld(16, 0) //[15:0]
    #define SHU2RK1_DQSOSC_DQSOSC_BASE_RK1_B1                  Fld(16, 16) //[31:16]

#define DRAMC_REG_SHU2RK1_SELPH_ODTEN0                     (DRAMC_AO_BASE_ADDRESS + 0x0111C)
    #define SHU2RK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN             Fld(3, 0) //[2:0]
    #define SHU2RK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN_P1          Fld(3, 4) //[6:4]
    #define SHU2RK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN             Fld(3, 8) //[10:8]
    #define SHU2RK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN_P1          Fld(3, 12) //[14:12]
    #define SHU2RK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN             Fld(3, 16) //[18:16]
    #define SHU2RK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN_P1          Fld(3, 20) //[22:20]
    #define SHU2RK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN             Fld(3, 24) //[26:24]
    #define SHU2RK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN_P1          Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2RK1_SELPH_ODTEN1                     (DRAMC_AO_BASE_ADDRESS + 0x01120)
    #define SHU2RK1_SELPH_ODTEN1_DLY_B0_R1RODTEN               Fld(3, 0) //[2:0]
    #define SHU2RK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1            Fld(3, 4) //[6:4]
    #define SHU2RK1_SELPH_ODTEN1_DLY_B1_R1RODTEN               Fld(3, 8) //[10:8]
    #define SHU2RK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1            Fld(3, 12) //[14:12]
    #define SHU2RK1_SELPH_ODTEN1_DLY_B2_R1RODTEN               Fld(3, 16) //[18:16]
    #define SHU2RK1_SELPH_ODTEN1_DLY_B2_R1RODTEN_P1            Fld(3, 20) //[22:20]
    #define SHU2RK1_SELPH_ODTEN1_DLY_B3_R1RODTEN               Fld(3, 24) //[26:24]
    #define SHU2RK1_SELPH_ODTEN1_DLY_B3_R1RODTEN_P1            Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2RK1_SELPH_DQSG0                      (DRAMC_AO_BASE_ADDRESS + 0x01124)
    #define SHU2RK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED            Fld(3, 0) //[2:0]
    #define SHU2RK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1         Fld(3, 4) //[6:4]
    #define SHU2RK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED            Fld(3, 8) //[10:8]
    #define SHU2RK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1         Fld(3, 12) //[14:12]
    #define SHU2RK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED            Fld(3, 16) //[18:16]
    #define SHU2RK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED_P1         Fld(3, 20) //[22:20]
    #define SHU2RK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED            Fld(3, 24) //[26:24]
    #define SHU2RK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED_P1         Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2RK1_SELPH_DQSG1                      (DRAMC_AO_BASE_ADDRESS + 0x01128)
    #define SHU2RK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED           Fld(3, 0) //[2:0]
    #define SHU2RK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1        Fld(3, 4) //[6:4]
    #define SHU2RK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED           Fld(3, 8) //[10:8]
    #define SHU2RK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1        Fld(3, 12) //[14:12]
    #define SHU2RK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED           Fld(3, 16) //[18:16]
    #define SHU2RK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED_P1        Fld(3, 20) //[22:20]
    #define SHU2RK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED           Fld(3, 24) //[26:24]
    #define SHU2RK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED_P1        Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2RK1_SELPH_DQ0                        (DRAMC_AO_BASE_ADDRESS + 0x0112C)
    #define SHU2RK1_SELPH_DQ0_TX_DLY_R1DQ0                     Fld(3, 0) //[2:0]
    #define SHU2RK1_SELPH_DQ0_TX_DLY_R1DQ1                     Fld(3, 4) //[6:4]
    #define SHU2RK1_SELPH_DQ0_TX_DLY_R1DQ2                     Fld(3, 8) //[10:8]
    #define SHU2RK1_SELPH_DQ0_TX_DLY_R1DQ3                     Fld(3, 12) //[14:12]
    #define SHU2RK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0                 Fld(3, 16) //[18:16]
    #define SHU2RK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1                 Fld(3, 20) //[22:20]
    #define SHU2RK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2                 Fld(3, 24) //[26:24]
    #define SHU2RK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3                 Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2RK1_SELPH_DQ1                        (DRAMC_AO_BASE_ADDRESS + 0x01130)
    #define SHU2RK1_SELPH_DQ1_TX_DLY_R1DQM0                    Fld(3, 0) //[2:0]
    #define SHU2RK1_SELPH_DQ1_TX_DLY_R1DQM1                    Fld(3, 4) //[6:4]
    #define SHU2RK1_SELPH_DQ1_TX_DLY_R1DQM2                    Fld(3, 8) //[10:8]
    #define SHU2RK1_SELPH_DQ1_TX_DLY_R1DQM3                    Fld(3, 12) //[14:12]
    #define SHU2RK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0                Fld(3, 16) //[18:16]
    #define SHU2RK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1                Fld(3, 20) //[22:20]
    #define SHU2RK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2                Fld(3, 24) //[26:24]
    #define SHU2RK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3                Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2RK1_SELPH_DQ2                        (DRAMC_AO_BASE_ADDRESS + 0x01134)
    #define SHU2RK1_SELPH_DQ2_DLY_R1DQ0                        Fld(3, 0) //[2:0]
    #define SHU2RK1_SELPH_DQ2_DLY_R1DQ1                        Fld(3, 4) //[6:4]
    #define SHU2RK1_SELPH_DQ2_DLY_R1DQ2                        Fld(3, 8) //[10:8]
    #define SHU2RK1_SELPH_DQ2_DLY_R1DQ3                        Fld(3, 12) //[14:12]
    #define SHU2RK1_SELPH_DQ2_DLY_R1OEN_DQ0                    Fld(3, 16) //[18:16]
    #define SHU2RK1_SELPH_DQ2_DLY_R1OEN_DQ1                    Fld(3, 20) //[22:20]
    #define SHU2RK1_SELPH_DQ2_DLY_R1OEN_DQ2                    Fld(3, 24) //[26:24]
    #define SHU2RK1_SELPH_DQ2_DLY_R1OEN_DQ3                    Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2RK1_SELPH_DQ3                        (DRAMC_AO_BASE_ADDRESS + 0x01138)
    #define SHU2RK1_SELPH_DQ3_DLY_R1DQM0                       Fld(3, 0) //[2:0]
    #define SHU2RK1_SELPH_DQ3_DLY_R1DQM1                       Fld(3, 4) //[6:4]
    #define SHU2RK1_SELPH_DQ3_DLY_R1DQM2                       Fld(3, 8) //[10:8]
    #define SHU2RK1_SELPH_DQ3_DLY_R1DQM3                       Fld(3, 12) //[14:12]
    #define SHU2RK1_SELPH_DQ3_DLY_R1OEN_DQM0                   Fld(3, 16) //[18:16]
    #define SHU2RK1_SELPH_DQ3_DLY_R1OEN_DQM1                   Fld(3, 20) //[22:20]
    #define SHU2RK1_SELPH_DQ3_DLY_R1OEN_DQM2                   Fld(3, 24) //[26:24]
    #define SHU2RK1_SELPH_DQ3_DLY_R1OEN_DQM3                   Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2RK1_DQS2DQ_CAL1                      (DRAMC_AO_BASE_ADDRESS + 0x01140)
    #define SHU2RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0           Fld(11, 0) //[10:0]
    #define SHU2RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1           Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU2RK1_DQS2DQ_CAL2                      (DRAMC_AO_BASE_ADDRESS + 0x01144)
    #define SHU2RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0           Fld(11, 0) //[10:0]
    #define SHU2RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1           Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU2RK1_DQS2DQ_CAL3                      (DRAMC_AO_BASE_ADDRESS + 0x01148)
    #define SHU2RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0       Fld(6, 0) //[5:0]
    #define SHU2RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1       Fld(6, 6) //[11:6]
    #define SHU2RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0_B4TO0 Fld(5, 12) //[16:12]
    #define SHU2RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1_B4TO0 Fld(5, 17) //[21:17]

#define DRAMC_REG_SHU2RK1_DQS2DQ_CAL4                      (DRAMC_AO_BASE_ADDRESS + 0x0114C)
    #define SHU2RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0      Fld(6, 0) //[5:0]
    #define SHU2RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1      Fld(6, 6) //[11:6]
    #define SHU2RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0_B4TO0 Fld(5, 12) //[16:12]
    #define SHU2RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1_B4TO0 Fld(5, 17) //[21:17]

#define DRAMC_REG_SHU2RK1_DQS2DQ_CAL5                      (DRAMC_AO_BASE_ADDRESS + 0x01150)
    #define SHU2RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0          Fld(11, 0) //[10:0]
    #define SHU2RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1          Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU2RK2_DQSCTL                           (DRAMC_AO_BASE_ADDRESS + 0x01200)
    #define SHU2RK2_DQSCTL_R2DQSINCTL                          Fld(4, 0) //[3:0]

#define DRAMC_REG_SHU2RK2_DQSIEN                           (DRAMC_AO_BASE_ADDRESS + 0x01204)
    #define SHU2RK2_DQSIEN_R2DQS0IEN                           Fld(7, 0) //[6:0]
    #define SHU2RK2_DQSIEN_R2DQS1IEN                           Fld(7, 8) //[14:8]
    #define SHU2RK2_DQSIEN_R2DQS2IEN                           Fld(7, 16) //[22:16]
    #define SHU2RK2_DQSIEN_R2DQS3IEN                           Fld(7, 24) //[30:24]

#define DRAMC_REG_SHU2RK2_DQSCAL                           (DRAMC_AO_BASE_ADDRESS + 0x01208)
    #define SHU2RK2_DQSCAL_R2DQSIENLLMT                        Fld(7, 0) //[6:0]
    #define SHU2RK2_DQSCAL_R2DQSIENLLMTEN                      Fld(1, 7) //[7:7]
    #define SHU2RK2_DQSCAL_R2DQSIENHLMT                        Fld(7, 8) //[14:8]
    #define SHU2RK2_DQSCAL_R2DQSIENHLMTEN                      Fld(1, 15) //[15:15]

#define DRAMC_REG_SHU2RK2_PI                               (DRAMC_AO_BASE_ADDRESS + 0x0120C)
    #define SHU2RK2_PI_RK2_ARPI_DQ_B1                          Fld(6, 0) //[5:0]
    #define SHU2RK2_PI_RK2_ARPI_DQ_B0                          Fld(6, 8) //[13:8]
    #define SHU2RK2_PI_RK2_ARPI_DQM_B1                         Fld(6, 16) //[21:16]
    #define SHU2RK2_PI_RK2_ARPI_DQM_B0                         Fld(6, 24) //[29:24]

#define DRAMC_REG_SHU2RK2_DQSOSC                           (DRAMC_AO_BASE_ADDRESS + 0x01210)
    #define SHU2RK2_DQSOSC_DQSOSC_BASE_RK2                     Fld(16, 0) //[15:0]
    #define SHU2RK2_DQSOSC_DQSOSC_BASE_RK2_B1                  Fld(16, 16) //[31:16]

#define DRAMC_REG_SHU2RK2_SELPH_ODTEN0                     (DRAMC_AO_BASE_ADDRESS + 0x0121C)
    #define SHU2RK2_SELPH_ODTEN0_TXDLY_B0_R2RODTEN             Fld(3, 0) //[2:0]
    #define SHU2RK2_SELPH_ODTEN0_TXDLY_B0_R2RODTEN_P1          Fld(3, 4) //[6:4]
    #define SHU2RK2_SELPH_ODTEN0_TXDLY_B1_R2RODTEN             Fld(3, 8) //[10:8]
    #define SHU2RK2_SELPH_ODTEN0_TXDLY_B1_R2RODTEN_P1          Fld(3, 12) //[14:12]
    #define SHU2RK2_SELPH_ODTEN0_TXDLY_B2_R2RODTEN             Fld(3, 16) //[18:16]
    #define SHU2RK2_SELPH_ODTEN0_TXDLY_B2_R2RODTEN_P1          Fld(3, 20) //[22:20]
    #define SHU2RK2_SELPH_ODTEN0_TXDLY_B3_R2RODTEN             Fld(3, 24) //[26:24]
    #define SHU2RK2_SELPH_ODTEN0_TXDLY_B3_R2RODTEN_P1          Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2RK2_SELPH_ODTEN1                     (DRAMC_AO_BASE_ADDRESS + 0x01220)
    #define SHU2RK2_SELPH_ODTEN1_DLY_B0_R2RODTEN               Fld(3, 0) //[2:0]
    #define SHU2RK2_SELPH_ODTEN1_DLY_B0_R2RODTEN_P1            Fld(3, 4) //[6:4]
    #define SHU2RK2_SELPH_ODTEN1_DLY_B1_R2RODTEN               Fld(3, 8) //[10:8]
    #define SHU2RK2_SELPH_ODTEN1_DLY_B1_R2RODTEN_P1            Fld(3, 12) //[14:12]
    #define SHU2RK2_SELPH_ODTEN1_DLY_B2_R2RODTEN               Fld(3, 16) //[18:16]
    #define SHU2RK2_SELPH_ODTEN1_DLY_B2_R2RODTEN_P1            Fld(3, 20) //[22:20]
    #define SHU2RK2_SELPH_ODTEN1_DLY_B3_R2RODTEN               Fld(3, 24) //[26:24]
    #define SHU2RK2_SELPH_ODTEN1_DLY_B3_R2RODTEN_P1            Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2RK2_SELPH_DQSG0                      (DRAMC_AO_BASE_ADDRESS + 0x01224)
    #define SHU2RK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED            Fld(3, 0) //[2:0]
    #define SHU2RK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED_P1         Fld(3, 4) //[6:4]
    #define SHU2RK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED            Fld(3, 8) //[10:8]
    #define SHU2RK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED_P1         Fld(3, 12) //[14:12]
    #define SHU2RK2_SELPH_DQSG0_TX_DLY_R2DQS2_GATED            Fld(3, 16) //[18:16]
    #define SHU2RK2_SELPH_DQSG0_TX_DLY_R2DQS2_GATED_P1         Fld(3, 20) //[22:20]
    #define SHU2RK2_SELPH_DQSG0_TX_DLY_R2DQS3_GATED            Fld(3, 24) //[26:24]
    #define SHU2RK2_SELPH_DQSG0_TX_DLY_R2DQS3_GATED_P1         Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2RK2_SELPH_DQSG1                      (DRAMC_AO_BASE_ADDRESS + 0x01228)
    #define SHU2RK2_SELPH_DQSG1_REG_DLY_R2DQS0_GATED           Fld(3, 0) //[2:0]
    #define SHU2RK2_SELPH_DQSG1_REG_DLY_R2DQS0_GATED_P1        Fld(3, 4) //[6:4]
    #define SHU2RK2_SELPH_DQSG1_REG_DLY_R2DQS1_GATED           Fld(3, 8) //[10:8]
    #define SHU2RK2_SELPH_DQSG1_REG_DLY_R2DQS1_GATED_P1        Fld(3, 12) //[14:12]
    #define SHU2RK2_SELPH_DQSG1_REG_DLY_R2DQS2_GATED           Fld(3, 16) //[18:16]
    #define SHU2RK2_SELPH_DQSG1_REG_DLY_R2DQS2_GATED_P1        Fld(3, 20) //[22:20]
    #define SHU2RK2_SELPH_DQSG1_REG_DLY_R2DQS3_GATED           Fld(3, 24) //[26:24]
    #define SHU2RK2_SELPH_DQSG1_REG_DLY_R2DQS3_GATED_P1        Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2RK2_SELPH_DQ0                        (DRAMC_AO_BASE_ADDRESS + 0x0122C)
    #define SHU2RK2_SELPH_DQ0_TX_DLY_R2DQ0                     Fld(3, 0) //[2:0]
    #define SHU2RK2_SELPH_DQ0_TX_DLY_R2DQ1                     Fld(3, 4) //[6:4]
    #define SHU2RK2_SELPH_DQ0_TX_DLY_R2DQ2                     Fld(3, 8) //[10:8]
    #define SHU2RK2_SELPH_DQ0_TX_DLY_R2DQ3                     Fld(3, 12) //[14:12]
    #define SHU2RK2_SELPH_DQ0_TX_DLY_R2OEN_DQ0                 Fld(3, 16) //[18:16]
    #define SHU2RK2_SELPH_DQ0_TX_DLY_R2OEN_DQ1                 Fld(3, 20) //[22:20]
    #define SHU2RK2_SELPH_DQ0_TX_DLY_R2OEN_DQ2                 Fld(3, 24) //[26:24]
    #define SHU2RK2_SELPH_DQ0_TX_DLY_R2OEN_DQ3                 Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2RK2_SELPH_DQ1                        (DRAMC_AO_BASE_ADDRESS + 0x01230)
    #define SHU2RK2_SELPH_DQ1_TX_DLY_R2DQM0                    Fld(3, 0) //[2:0]
    #define SHU2RK2_SELPH_DQ1_TX_DLY_R2DQM1                    Fld(3, 4) //[6:4]
    #define SHU2RK2_SELPH_DQ1_TX_DLY_R2DQM2                    Fld(3, 8) //[10:8]
    #define SHU2RK2_SELPH_DQ1_TX_DLY_R2DQM3                    Fld(3, 12) //[14:12]
    #define SHU2RK2_SELPH_DQ1_TX_DLY_R2OEN_DQM0                Fld(3, 16) //[18:16]
    #define SHU2RK2_SELPH_DQ1_TX_DLY_R2OEN_DQM1                Fld(3, 20) //[22:20]
    #define SHU2RK2_SELPH_DQ1_TX_DLY_R2OEN_DQM2                Fld(3, 24) //[26:24]
    #define SHU2RK2_SELPH_DQ1_TX_DLY_R2OEN_DQM3                Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2RK2_SELPH_DQ2                        (DRAMC_AO_BASE_ADDRESS + 0x01234)
    #define SHU2RK2_SELPH_DQ2_DLY_R2DQ0                        Fld(3, 0) //[2:0]
    #define SHU2RK2_SELPH_DQ2_DLY_R2DQ1                        Fld(3, 4) //[6:4]
    #define SHU2RK2_SELPH_DQ2_DLY_R2DQ2                        Fld(3, 8) //[10:8]
    #define SHU2RK2_SELPH_DQ2_DLY_R2DQ3                        Fld(3, 12) //[14:12]
    #define SHU2RK2_SELPH_DQ2_DLY_R2OEN_DQ0                    Fld(3, 16) //[18:16]
    #define SHU2RK2_SELPH_DQ2_DLY_R2OEN_DQ1                    Fld(3, 20) //[22:20]
    #define SHU2RK2_SELPH_DQ2_DLY_R2OEN_DQ2                    Fld(3, 24) //[26:24]
    #define SHU2RK2_SELPH_DQ2_DLY_R2OEN_DQ3                    Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2RK2_SELPH_DQ3                        (DRAMC_AO_BASE_ADDRESS + 0x01238)
    #define SHU2RK2_SELPH_DQ3_DLY_R2DQM0                       Fld(3, 0) //[2:0]
    #define SHU2RK2_SELPH_DQ3_DLY_R2DQM1                       Fld(3, 4) //[6:4]
    #define SHU2RK2_SELPH_DQ3_DLY_R2DQM2                       Fld(3, 8) //[10:8]
    #define SHU2RK2_SELPH_DQ3_DLY_R2DQM3                       Fld(3, 12) //[14:12]
    #define SHU2RK2_SELPH_DQ3_DLY_R2OEN_DQM0                   Fld(3, 16) //[18:16]
    #define SHU2RK2_SELPH_DQ3_DLY_R2OEN_DQM1                   Fld(3, 20) //[22:20]
    #define SHU2RK2_SELPH_DQ3_DLY_R2OEN_DQM2                   Fld(3, 24) //[26:24]
    #define SHU2RK2_SELPH_DQ3_DLY_R2OEN_DQM3                   Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU2RK2_DQS2DQ_CAL1                      (DRAMC_AO_BASE_ADDRESS + 0x01240)
    #define SHU2RK2_DQS2DQ_CAL1_BOOT_ORIG_UI_RK2_DQ0           Fld(11, 0) //[10:0]
    #define SHU2RK2_DQS2DQ_CAL1_BOOT_ORIG_UI_RK2_DQ1           Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU2RK2_DQS2DQ_CAL2                      (DRAMC_AO_BASE_ADDRESS + 0x01244)
    #define SHU2RK2_DQS2DQ_CAL2_BOOT_TARG_UI_RK2_DQ0           Fld(11, 0) //[10:0]
    #define SHU2RK2_DQS2DQ_CAL2_BOOT_TARG_UI_RK2_DQ1           Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU2RK2_DQS2DQ_CAL3                      (DRAMC_AO_BASE_ADDRESS + 0x01248)
    #define SHU2RK2_DQS2DQ_CAL3_BOOT_TARG_UI_RK2_OEN_DQ0       Fld(6, 0) //[5:0]
    #define SHU2RK2_DQS2DQ_CAL3_BOOT_TARG_UI_RK2_OEN_DQ1       Fld(6, 6) //[11:6]

#define DRAMC_REG_SHU2RK2_DQS2DQ_CAL4                      (DRAMC_AO_BASE_ADDRESS + 0x0124C)
    #define SHU2RK2_DQS2DQ_CAL4_BOOT_TARG_UI_RK2_OEN_DQM0      Fld(6, 0) //[5:0]
    #define SHU2RK2_DQS2DQ_CAL4_BOOT_TARG_UI_RK2_OEN_DQM1      Fld(6, 6) //[11:6]

#define DRAMC_REG_SHU2RK2_DQS2DQ_CAL5                      (DRAMC_AO_BASE_ADDRESS + 0x01250)
    #define SHU2RK2_DQS2DQ_CAL5_BOOT_TARG_UI_RK2_DQM0          Fld(11, 0) //[10:0]
    #define SHU2RK2_DQS2DQ_CAL5_BOOT_TARG_UI_RK2_DQM1          Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU2_DQSG_RETRY                          (DRAMC_AO_BASE_ADDRESS + 0x01254)
    #define SHU2_DQSG_RETRY_R_DQSGRETRY_SW_RESET               Fld(1, 0) //[0:0]
    #define SHU2_DQSG_RETRY_R_DQSG_RETRY_SW_EN                 Fld(1, 1) //[1:1]
    #define SHU2_DQSG_RETRY_R_DDR1866_PLUS                     Fld(1, 2) //[2:2]
    #define SHU2_DQSG_RETRY_R_RETRY_ONCE                       Fld(1, 3) //[3:3]
    #define SHU2_DQSG_RETRY_R_RETRY_3TIMES                     Fld(1, 4) //[4:4]
    #define SHU2_DQSG_RETRY_R_RETRY_1RANK                      Fld(1, 5) //[5:5]
    #define SHU2_DQSG_RETRY_R_RETRY_SAV_MSK                    Fld(1, 6) //[6:6]
    #define SHU2_DQSG_RETRY_R_DM4BYTE                          Fld(1, 7) //[7:7]
    #define SHU2_DQSG_RETRY_R_DQSIENLAT                        Fld(4, 8) //[11:8]
    #define SHU2_DQSG_RETRY_R_STBENCMP_ALLBYTE                 Fld(1, 12) //[12:12]
    #define SHU2_DQSG_RETRY_R_XSR_DQSG_RETRY_EN                Fld(1, 13) //[13:13]
    #define SHU2_DQSG_RETRY_R_XSR_RETRY_SPM_MODE               Fld(1, 14) //[14:14]
    #define SHU2_DQSG_RETRY_R_RETRY_CMP_DATA                   Fld(1, 15) //[15:15]
    #define SHU2_DQSG_RETRY_R_RETRY_ALE_BLOCK_MASK             Fld(1, 20) //[20:20]
    #define SHU2_DQSG_RETRY_R_RDY_SEL_DLE                      Fld(1, 21) //[21:21]
    #define SHU2_DQSG_RETRY_R_RETRY_ROUND_NUM                  Fld(2, 24) //[25:24]
    #define SHU2_DQSG_RETRY_R_RETRY_RANKSEL_FROM_PHY           Fld(1, 28) //[28:28]
    #define SHU2_DQSG_RETRY_R_RETRY_PA_DSIABLE                 Fld(1, 29) //[29:29]
    #define SHU2_DQSG_RETRY_R_RETRY_STBEN_RESET_MSK            Fld(1, 30) //[30:30]
    #define SHU2_DQSG_RETRY_R_RETRY_USE_BURST_MDOE             Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU3_ACTIM0                              (DRAMC_AO_BASE_ADDRESS + 0x01400)
    #define SHU3_ACTIM0_TWTR                                   Fld(4, 0) //[3:0]
    #define SHU3_ACTIM0_TWR                                    Fld(5, 8) //[12:8]
    #define SHU3_ACTIM0_TRRD                                   Fld(3, 16) //[18:16]
    #define SHU3_ACTIM0_TRCD                                   Fld(4, 24) //[27:24]

#define DRAMC_REG_SHU3_ACTIM1                              (DRAMC_AO_BASE_ADDRESS + 0x01404)
    #define SHU3_ACTIM1_TRPAB                                  Fld(3, 0) //[2:0]
    #define SHU3_ACTIM1_TRP                                    Fld(4, 8) //[11:8]
    #define SHU3_ACTIM1_TRAS                                   Fld(4, 16) //[19:16]
    #define SHU3_ACTIM1_TRC                                    Fld(5, 24) //[28:24]

#define DRAMC_REG_SHU3_ACTIM2                              (DRAMC_AO_BASE_ADDRESS + 0x01408)
    #define SHU3_ACTIM2_TXP                                    Fld(3, 0) //[2:0]
    #define SHU3_ACTIM2_TRTP                                   Fld(3, 8) //[10:8]
    #define SHU3_ACTIM2_TR2W                                   Fld(4, 16) //[19:16]
    #define SHU3_ACTIM2_TFAW                                   Fld(5, 24) //[28:24]

#define DRAMC_REG_SHU3_ACTIM3                              (DRAMC_AO_BASE_ADDRESS + 0x0140C)
    #define SHU3_ACTIM3_TRFCPB                                 Fld(8, 0) //[7:0]
    #define SHU3_ACTIM3_TRFC                                   Fld(8, 16) //[23:16]
    #define SHU3_ACTIM3_REFCNT                                 Fld(8, 24) //[31:24]

#define DRAMC_REG_SHU3_ACTIM4                              (DRAMC_AO_BASE_ADDRESS + 0x01410)
    #define SHU3_ACTIM4_TXREFCNT                               Fld(10, 0) //[9:0]
    #define SHU3_ACTIM4_REFCNT_FR_CLK                          Fld(8, 16) //[23:16]
    #define SHU3_ACTIM4_TZQCS                                  Fld(8, 24) //[31:24]

#define DRAMC_REG_SHU3_ACTIM5                              (DRAMC_AO_BASE_ADDRESS + 0x01414)
    #define SHU3_ACTIM5_TR2PD                                  Fld(5, 0) //[4:0]
    #define SHU3_ACTIM5_TWTPD                                  Fld(5, 8) //[12:8]
    #define SHU3_ACTIM5_TMRR2W                                 Fld(4, 24) //[27:24]

#define DRAMC_REG_SHU3_ACTIM6                              (DRAMC_AO_BASE_ADDRESS + 0x01418)
    #define SHU3_ACTIM6_BGTCCD                                 Fld(2, 0) //[1:0]
    #define SHU3_ACTIM6_BGTWTR                                 Fld(4, 4) //[7:4]
    #define SHU3_ACTIM6_TWRMPR                                 Fld(4, 8) //[11:8]
    #define SHU3_ACTIM6_BGTRRD                                 Fld(3, 12) //[14:12]

#define DRAMC_REG_SHU3_ACTIM_XRT                           (DRAMC_AO_BASE_ADDRESS + 0x0141C)
    #define SHU3_ACTIM_XRT_XRTR2R                              Fld(5, 0) //[4:0]
    #define SHU3_ACTIM_XRT_XRTR2W                              Fld(4, 8) //[11:8]
    #define SHU3_ACTIM_XRT_XRTW2R                              Fld(3, 16) //[18:16]
    #define SHU3_ACTIM_XRT_XRTW2W                              Fld(4, 24) //[27:24]

#define DRAMC_REG_SHU3_AC_TIME_05T                         (DRAMC_AO_BASE_ADDRESS + 0x01420)
    #define SHU3_AC_TIME_05T_TRC_05T                           Fld(1, 0) //[0:0]
    #define SHU3_AC_TIME_05T_TRFCPB_05T                        Fld(1, 1) //[1:1]
    #define SHU3_AC_TIME_05T_TRFC_05T                          Fld(1, 2) //[2:2]
    #define SHU3_AC_TIME_05T_TXP_05T                           Fld(1, 4) //[4:4]
    #define SHU3_AC_TIME_05T_TRTP_05T                          Fld(1, 5) //[5:5]
    #define SHU3_AC_TIME_05T_TRCD_05T                          Fld(1, 6) //[6:6]
    #define SHU3_AC_TIME_05T_TRP_05T                           Fld(1, 7) //[7:7]
    #define SHU3_AC_TIME_05T_TRPAB_05T                         Fld(1, 8) //[8:8]
    #define SHU3_AC_TIME_05T_TRAS_05T                          Fld(1, 9) //[9:9]
    #define SHU3_AC_TIME_05T_TWR_M05T                          Fld(1, 10) //[10:10]
    #define SHU3_AC_TIME_05T_TRRD_05T                          Fld(1, 12) //[12:12]
    #define SHU3_AC_TIME_05T_TFAW_05T                          Fld(1, 13) //[13:13]
    #define SHU3_AC_TIME_05T_TR2PD_05T                         Fld(1, 15) //[15:15]
    #define SHU3_AC_TIME_05T_TWTPD_M05T                        Fld(1, 16) //[16:16]
    #define SHU3_AC_TIME_05T_BGTRRD_05T                        Fld(1, 21) //[21:21]
    #define SHU3_AC_TIME_05T_BGTCCD_05T                        Fld(1, 22) //[22:22]
    #define SHU3_AC_TIME_05T_BGTWTR_05T                        Fld(1, 23) //[23:23]
    #define SHU3_AC_TIME_05T_TR2W_05T                          Fld(1, 24) //[24:24]
    #define SHU3_AC_TIME_05T_TWTR_M05T                         Fld(1, 25) //[25:25]
    #define SHU3_AC_TIME_05T_XRTR2W_05T                        Fld(1, 26) //[26:26]
    #define SHU3_AC_TIME_05T_XRTW2R_M05T                       Fld(1, 27) //[27:27]

#define DRAMC_REG_SHU3_AC_DERATING0                        (DRAMC_AO_BASE_ADDRESS + 0x01424)
    #define SHU3_AC_DERATING0_ACDERATEEN                       Fld(1, 0) //[0:0]
    #define SHU3_AC_DERATING0_TRRD_DERATE                      Fld(3, 16) //[18:16]
    #define SHU3_AC_DERATING0_TRCD_DERATE                      Fld(4, 24) //[27:24]

#define DRAMC_REG_SHU3_AC_DERATING1                        (DRAMC_AO_BASE_ADDRESS + 0x01428)
    #define SHU3_AC_DERATING1_TRPAB_DERATE                     Fld(3, 0) //[2:0]
    #define SHU3_AC_DERATING1_TRP_DERATE                       Fld(4, 8) //[11:8]
    #define SHU3_AC_DERATING1_TRAS_DERATE                      Fld(4, 16) //[19:16]
    #define SHU3_AC_DERATING1_TRC_DERATE                       Fld(5, 24) //[28:24]

#define DRAMC_REG_SHU3_AC_DERATING_05T                     (DRAMC_AO_BASE_ADDRESS + 0x01430)
    #define SHU3_AC_DERATING_05T_TRC_05T_DERATE                Fld(1, 0) //[0:0]
    #define SHU3_AC_DERATING_05T_TRCD_05T_DERATE               Fld(1, 6) //[6:6]
    #define SHU3_AC_DERATING_05T_TRP_05T_DERATE                Fld(1, 7) //[7:7]
    #define SHU3_AC_DERATING_05T_TRPAB_05T_DERATE              Fld(1, 8) //[8:8]
    #define SHU3_AC_DERATING_05T_TRAS_05T_DERATE               Fld(1, 9) //[9:9]
    #define SHU3_AC_DERATING_05T_TRRD_05T_DERATE               Fld(1, 12) //[12:12]

#define DRAMC_REG_SHU3_CONF0                               (DRAMC_AO_BASE_ADDRESS + 0x01440)
    #define SHU3_CONF0_DMPGTIM                                 Fld(6, 0) //[5:0]
    #define SHU3_CONF0_ADVREFEN                                Fld(1, 6) //[6:6]
    #define SHU3_CONF0_ADVPREEN                                Fld(1, 7) //[7:7]
    #define SHU3_CONF0_TRFCPBIG                                Fld(1, 9) //[9:9]
    #define SHU3_CONF0_REFTHD                                  Fld(4, 12) //[15:12]
    #define SHU3_CONF0_REQQUE_DEPTH                            Fld(4, 16) //[19:16]
    #define SHU3_CONF0_FREQDIV4                                Fld(1, 24) //[24:24]
    #define SHU3_CONF0_FDIV2                                   Fld(1, 25) //[25:25]
    #define SHU3_CONF0_CL2                                     Fld(1, 27) //[27:27]
    #define SHU3_CONF0_BL2                                     Fld(1, 28) //[28:28]
    #define SHU3_CONF0_BL4                                     Fld(1, 29) //[29:29]
    #define SHU3_CONF0_MATYPE                                  Fld(2, 30) //[31:30]

#define DRAMC_REG_SHU3_CONF1                               (DRAMC_AO_BASE_ADDRESS + 0x01444)
    #define SHU3_CONF1_DATLAT                                  Fld(5, 0) //[4:0]
    #define SHU3_CONF1_DATLAT_DSEL                             Fld(5, 8) //[12:8]
    #define SHU3_CONF1_REFBW_FR                                Fld(10, 16) //[25:16]
    #define SHU3_CONF1_DATLAT_DSEL_PHY                         Fld(5, 26) //[30:26]
    #define SHU3_CONF1_TREFBWIG                                Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU3_CONF2                               (DRAMC_AO_BASE_ADDRESS + 0x01448)
    #define SHU3_CONF2_TCMDO1LAT                               Fld(8, 0) //[7:0]
    #define SHU3_CONF2_FSPCHG_PRDCNT                           Fld(8, 8) //[15:8]
    #define SHU3_CONF2_DCMDLYREF                               Fld(3, 16) //[18:16]
    #define SHU3_CONF2_DQCMD                                   Fld(1, 25) //[25:25]
    #define SHU3_CONF2_DQ16COM1                                Fld(1, 26) //[26:26]
    #define SHU3_CONF2_RA15TOCS1                               Fld(1, 27) //[27:27]
    #define SHU3_CONF2_WPRE2T                                  Fld(1, 28) //[28:28]
    #define SHU3_CONF2_FASTWAKE2                               Fld(1, 29) //[29:29]
    #define SHU3_CONF2_DAREFEN                                 Fld(1, 30) //[30:30]
    #define SHU3_CONF2_FASTWAKE                                Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU3_CONF3                               (DRAMC_AO_BASE_ADDRESS + 0x0144C)
    #define SHU3_CONF3_ZQCSCNT                                 Fld(16, 0) //[15:0]
    #define SHU3_CONF3_REFRCNT                                 Fld(9, 16) //[24:16]

#define DRAMC_REG_SHU3_STBCAL                              (DRAMC_AO_BASE_ADDRESS + 0x01450)
    #define SHU3_STBCAL_DMSTBLAT                               Fld(2, 0) //[1:0]
    #define SHU3_STBCAL_PICGLAT                                Fld(3, 4) //[6:4]
    #define SHU3_STBCAL_DQSG_MODE                              Fld(1, 8) //[8:8]

#define DRAMC_REG_SHU3_DQSOSCTHRD                          (DRAMC_AO_BASE_ADDRESS + 0x01454)
    #define SHU3_DQSOSCTHRD_DQSOSCTHRD_INC_RK0                 Fld(12, 0) //[11:0]
    #define SHU3_DQSOSCTHRD_DQSOSCTHRD_DEC_RK0                 Fld(12, 12) //[23:12]
    #define SHU3_DQSOSCTHRD_DQSOSCTHRD_INC_RK1_7TO0            Fld(8, 24) //[31:24]

#define DRAMC_REG_SHU3_RANKCTL                             (DRAMC_AO_BASE_ADDRESS + 0x01458)
    #define SHU3_RANKCTL_RANKINCTL_RXDLY                       Fld(4, 0) //[3:0]
    #define SHU3_RANKCTL_TXRANKINCTL_TXDLY                     Fld(4, 8) //[11:8]
    #define SHU3_RANKCTL_TXRANKINCTL                           Fld(4, 12) //[15:12]
    #define SHU3_RANKCTL_TXRANKINCTL_ROOT                      Fld(4, 16) //[19:16]
    #define SHU3_RANKCTL_RANKINCTL                             Fld(4, 20) //[23:20]
    #define SHU3_RANKCTL_RANKINCTL_ROOT1                       Fld(4, 24) //[27:24]
    #define SHU3_RANKCTL_RANKINCTL_PHY                         Fld(4, 28) //[31:28]

#define DRAMC_REG_SHU3_CKECTRL                             (DRAMC_AO_BASE_ADDRESS + 0x0145C)
    #define SHU3_CKECTRL_CMDCKE                                Fld(3, 16) //[18:16]
    #define SHU3_CKECTRL_CKEPRD                                Fld(3, 20) //[22:20]
    #define SHU3_CKECTRL_TCKESRX                               Fld(2, 24) //[25:24]
    #define SHU3_CKECTRL_SREF_CK_DLY                           Fld(2, 28) //[29:28]

#define DRAMC_REG_SHU3_ODTCTRL                             (DRAMC_AO_BASE_ADDRESS + 0x01460)
    #define SHU3_ODTCTRL_ROEN                                  Fld(1, 0) //[0:0]
    #define SHU3_ODTCTRL_WOEN                                  Fld(1, 1) //[1:1]
    #define SHU3_ODTCTRL_RODTEN_SELPH_CG_IG                    Fld(1, 2) //[2:2]
    #define SHU3_ODTCTRL_RODTENSTB_SELPH_CG_IG                 Fld(1, 3) //[3:3]
    #define SHU3_ODTCTRL_RODT                                  Fld(4, 4) //[7:4]
    #define SHU3_ODTCTRL_TWODT                                 Fld(7, 16) //[22:16]
    #define SHU3_ODTCTRL_FIXRODT                               Fld(1, 27) //[27:27]
    #define SHU3_ODTCTRL_RODTE2                                Fld(1, 30) //[30:30]
    #define SHU3_ODTCTRL_RODTE                                 Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU3_IMPCAL1                             (DRAMC_AO_BASE_ADDRESS + 0x01464)
    #define SHU3_IMPCAL1_IMPCAL_CHKCYCLE                       Fld(3, 0) //[2:0]
    #define SHU3_IMPCAL1_IMPDRVP                               Fld(5, 4) //[8:4]
    #define SHU3_IMPCAL1_IMPDRVN                               Fld(5, 11) //[15:11]
    #define SHU3_IMPCAL1_IMPCAL_CALEN_CYCLE                    Fld(3, 17) //[19:17]
    #define SHU3_IMPCAL1_IMPCALCNT                             Fld(8, 20) //[27:20]
    #define SHU3_IMPCAL1_IMPCAL_CALICNT                        Fld(4, 28) //[31:28]

#define DRAMC_REG_SHU3_DQSOSC_PRD                          (DRAMC_AO_BASE_ADDRESS + 0x01468)
    #define SHU3_DQSOSC_PRD_DQSOSC_PRDCNT                      Fld(10, 0) //[9:0]
    #define SHU3_DQSOSC_PRD_DQSOSCTHRD_INC_RK1_11TO8           Fld(4, 16) //[19:16]
    #define SHU3_DQSOSC_PRD_DQSOSCTHRD_DEC_RK1                 Fld(12, 20) //[31:20]

#define DRAMC_REG_SHU3_DQSOSCR                             (DRAMC_AO_BASE_ADDRESS + 0x0146C)
    #define SHU3_DQSOSCR_DQSOSCRCNT                            Fld(8, 0) //[7:0]
    #define SHU3_DQSOSCR_DQSOSC_DELTA                          Fld(16, 16) //[31:16]

#define DRAMC_REG_SHU3_DQSOSCR2                            (DRAMC_AO_BASE_ADDRESS + 0x01470)
    #define SHU3_DQSOSCR2_DQSOSCENCNT                          Fld(16, 0) //[15:0]
    #define SHU3_DQSOSCR2_DQSOSC_ADV_SEL                       Fld(2, 16) //[17:16]
    #define SHU3_DQSOSCR2_DQSOSC_DRS_ADV_SEL                   Fld(2, 18) //[19:18]

#define DRAMC_REG_SHU3_RODTENSTB                           (DRAMC_AO_BASE_ADDRESS + 0x01474)
    #define SHU3_RODTENSTB_RODTEN_MCK_MODESEL                  Fld(1, 0) //[0:0]
    #define SHU3_RODTENSTB_RODTEN_P1_ENABLE                    Fld(1, 1) //[1:1]
    #define SHU3_RODTENSTB_RODTENSTB_OFFSET                    Fld(6, 2) //[7:2]
    #define SHU3_RODTENSTB_RODTENSTB_EXT                       Fld(16, 8) //[23:8]
    #define SHU3_RODTENSTB_RODTENSTB_4BYTE_EN                  Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU3_PIPE                                (DRAMC_AO_BASE_ADDRESS + 0x01478)
    #define SHU3_PIPE_PHYRXPIPE1                               Fld(1, 0) //[0:0]
    #define SHU3_PIPE_PHYRXPIPE2                               Fld(1, 1) //[1:1]
    #define SHU3_PIPE_PHYRXPIPE3                               Fld(1, 2) //[2:2]
    #define SHU3_PIPE_PHYRXRDSLPIPE1                           Fld(1, 4) //[4:4]
    #define SHU3_PIPE_PHYRXRDSLPIPE2                           Fld(1, 5) //[5:5]
    #define SHU3_PIPE_PHYRXRDSLPIPE3                           Fld(1, 6) //[6:6]
    #define SHU3_PIPE_PHYPIPE1EN                               Fld(1, 8) //[8:8]
    #define SHU3_PIPE_PHYPIPE2EN                               Fld(1, 9) //[9:9]
    #define SHU3_PIPE_PHYPIPE3EN                               Fld(1, 10) //[10:10]
    #define SHU3_PIPE_DLE_LAST_EXTEND3                         Fld(1, 26) //[26:26]
    #define SHU3_PIPE_READ_START_EXTEND3                       Fld(1, 27) //[27:27]
    #define SHU3_PIPE_DLE_LAST_EXTEND2                         Fld(1, 28) //[28:28]
    #define SHU3_PIPE_READ_START_EXTEND2                       Fld(1, 29) //[29:29]
    #define SHU3_PIPE_DLE_LAST_EXTEND1                         Fld(1, 30) //[30:30]
    #define SHU3_PIPE_READ_START_EXTEND1                       Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU3_TEST1                               (DRAMC_AO_BASE_ADDRESS + 0x0147C)
    #define SHU3_TEST1_LATNORMPOP                              Fld(5, 8) //[12:8]
    #define SHU3_TEST1_DQSICALBLCOK_CNT                        Fld(3, 20) //[22:20]
    #define SHU3_TEST1_DQSICALI_NEW                            Fld(1, 23) //[23:23]

#define DRAMC_REG_SHU3_SELPH_CA1                           (DRAMC_AO_BASE_ADDRESS + 0x01480)
    #define SHU3_SELPH_CA1_TXDLY_CS                            Fld(3, 0) //[2:0]
    #define SHU3_SELPH_CA1_TXDLY_CKE                           Fld(3, 4) //[6:4]
    #define SHU3_SELPH_CA1_TXDLY_ODT                           Fld(3, 8) //[10:8]
    #define SHU3_SELPH_CA1_TXDLY_RESET                         Fld(3, 12) //[14:12]
    #define SHU3_SELPH_CA1_TXDLY_WE                            Fld(3, 16) //[18:16]
    #define SHU3_SELPH_CA1_TXDLY_CAS                           Fld(3, 20) //[22:20]
    #define SHU3_SELPH_CA1_TXDLY_RAS                           Fld(3, 24) //[26:24]
    #define SHU3_SELPH_CA1_TXDLY_CS1                           Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3_SELPH_CA2                           (DRAMC_AO_BASE_ADDRESS + 0x01484)
    #define SHU3_SELPH_CA2_TXDLY_BA0                           Fld(3, 0) //[2:0]
    #define SHU3_SELPH_CA2_TXDLY_BA1                           Fld(3, 4) //[6:4]
    #define SHU3_SELPH_CA2_TXDLY_BA2                           Fld(3, 8) //[10:8]
    #define SHU3_SELPH_CA2_TXDLY_CMD                           Fld(5, 16) //[20:16]
    #define SHU3_SELPH_CA2_TXDLY_CKE1                          Fld(3, 24) //[26:24]

#define DRAMC_REG_SHU3_SELPH_CA3                           (DRAMC_AO_BASE_ADDRESS + 0x01488)
    #define SHU3_SELPH_CA3_TXDLY_RA0                           Fld(3, 0) //[2:0]
    #define SHU3_SELPH_CA3_TXDLY_RA1                           Fld(3, 4) //[6:4]
    #define SHU3_SELPH_CA3_TXDLY_RA2                           Fld(3, 8) //[10:8]
    #define SHU3_SELPH_CA3_TXDLY_RA3                           Fld(3, 12) //[14:12]
    #define SHU3_SELPH_CA3_TXDLY_RA4                           Fld(3, 16) //[18:16]
    #define SHU3_SELPH_CA3_TXDLY_RA5                           Fld(3, 20) //[22:20]
    #define SHU3_SELPH_CA3_TXDLY_RA6                           Fld(3, 24) //[26:24]
    #define SHU3_SELPH_CA3_TXDLY_RA7                           Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3_SELPH_CA4                           (DRAMC_AO_BASE_ADDRESS + 0x0148C)
    #define SHU3_SELPH_CA4_TXDLY_RA8                           Fld(3, 0) //[2:0]
    #define SHU3_SELPH_CA4_TXDLY_RA9                           Fld(3, 4) //[6:4]
    #define SHU3_SELPH_CA4_TXDLY_RA10                          Fld(3, 8) //[10:8]
    #define SHU3_SELPH_CA4_TXDLY_RA11                          Fld(3, 12) //[14:12]
    #define SHU3_SELPH_CA4_TXDLY_RA12                          Fld(3, 16) //[18:16]
    #define SHU3_SELPH_CA4_TXDLY_RA13                          Fld(3, 20) //[22:20]
    #define SHU3_SELPH_CA4_TXDLY_RA14                          Fld(3, 24) //[26:24]
    #define SHU3_SELPH_CA4_TXDLY_RA15                          Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3_SELPH_CA5                           (DRAMC_AO_BASE_ADDRESS + 0x01490)
    #define SHU3_SELPH_CA5_DLY_CS                              Fld(3, 0) //[2:0]
    #define SHU3_SELPH_CA5_DLY_CKE                             Fld(3, 4) //[6:4]
    #define SHU3_SELPH_CA5_DLY_ODT                             Fld(3, 8) //[10:8]
    #define SHU3_SELPH_CA5_DLY_RESET                           Fld(3, 12) //[14:12]
    #define SHU3_SELPH_CA5_DLY_WE                              Fld(3, 16) //[18:16]
    #define SHU3_SELPH_CA5_DLY_CAS                             Fld(3, 20) //[22:20]
    #define SHU3_SELPH_CA5_DLY_RAS                             Fld(3, 24) //[26:24]
    #define SHU3_SELPH_CA5_DLY_CS1                             Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3_SELPH_CA6                           (DRAMC_AO_BASE_ADDRESS + 0x01494)
    #define SHU3_SELPH_CA6_DLY_BA0                             Fld(3, 0) //[2:0]
    #define SHU3_SELPH_CA6_DLY_BA1                             Fld(3, 4) //[6:4]
    #define SHU3_SELPH_CA6_DLY_BA2                             Fld(3, 8) //[10:8]
    #define SHU3_SELPH_CA6_DLY_CKE1                            Fld(3, 24) //[26:24]

#define DRAMC_REG_SHU3_SELPH_CA7                           (DRAMC_AO_BASE_ADDRESS + 0x01498)
    #define SHU3_SELPH_CA7_DLY_RA0                             Fld(3, 0) //[2:0]
    #define SHU3_SELPH_CA7_DLY_RA1                             Fld(3, 4) //[6:4]
    #define SHU3_SELPH_CA7_DLY_RA2                             Fld(3, 8) //[10:8]
    #define SHU3_SELPH_CA7_DLY_RA3                             Fld(3, 12) //[14:12]
    #define SHU3_SELPH_CA7_DLY_RA4                             Fld(3, 16) //[18:16]
    #define SHU3_SELPH_CA7_DLY_RA5                             Fld(3, 20) //[22:20]
    #define SHU3_SELPH_CA7_DLY_RA6                             Fld(3, 24) //[26:24]
    #define SHU3_SELPH_CA7_DLY_RA7                             Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3_SELPH_CA8                           (DRAMC_AO_BASE_ADDRESS + 0x0149C)
    #define SHU3_SELPH_CA8_DLY_RA8                             Fld(3, 0) //[2:0]
    #define SHU3_SELPH_CA8_DLY_RA9                             Fld(3, 4) //[6:4]
    #define SHU3_SELPH_CA8_DLY_RA10                            Fld(3, 8) //[10:8]
    #define SHU3_SELPH_CA8_DLY_RA11                            Fld(3, 12) //[14:12]
    #define SHU3_SELPH_CA8_DLY_RA12                            Fld(3, 16) //[18:16]
    #define SHU3_SELPH_CA8_DLY_RA13                            Fld(3, 20) //[22:20]
    #define SHU3_SELPH_CA8_DLY_RA14                            Fld(3, 24) //[26:24]
    #define SHU3_SELPH_CA8_DLY_RA15                            Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3_SELPH_DQS0                          (DRAMC_AO_BASE_ADDRESS + 0x014A0)
    #define SHU3_SELPH_DQS0_TXDLY_DQS0                         Fld(3, 0) //[2:0]
    #define SHU3_SELPH_DQS0_TXDLY_DQS1                         Fld(3, 4) //[6:4]
    #define SHU3_SELPH_DQS0_TXDLY_DQS2                         Fld(3, 8) //[10:8]
    #define SHU3_SELPH_DQS0_TXDLY_DQS3                         Fld(3, 12) //[14:12]
    #define SHU3_SELPH_DQS0_TXDLY_OEN_DQS0                     Fld(3, 16) //[18:16]
    #define SHU3_SELPH_DQS0_TXDLY_OEN_DQS1                     Fld(3, 20) //[22:20]
    #define SHU3_SELPH_DQS0_TXDLY_OEN_DQS2                     Fld(3, 24) //[26:24]
    #define SHU3_SELPH_DQS0_TXDLY_OEN_DQS3                     Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3_SELPH_DQS1                          (DRAMC_AO_BASE_ADDRESS + 0x014A4)
    #define SHU3_SELPH_DQS1_DLY_DQS0                           Fld(3, 0) //[2:0]
    #define SHU3_SELPH_DQS1_DLY_DQS1                           Fld(3, 4) //[6:4]
    #define SHU3_SELPH_DQS1_DLY_DQS2                           Fld(3, 8) //[10:8]
    #define SHU3_SELPH_DQS1_DLY_DQS3                           Fld(3, 12) //[14:12]
    #define SHU3_SELPH_DQS1_DLY_OEN_DQS0                       Fld(3, 16) //[18:16]
    #define SHU3_SELPH_DQS1_DLY_OEN_DQS1                       Fld(3, 20) //[22:20]
    #define SHU3_SELPH_DQS1_DLY_OEN_DQS2                       Fld(3, 24) //[26:24]
    #define SHU3_SELPH_DQS1_DLY_OEN_DQS3                       Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3_DRVING1                             (DRAMC_AO_BASE_ADDRESS + 0x014A8)
    #define SHU3_DRVING1_DQDRVN2                               Fld(5, 0) //[4:0]
    #define SHU3_DRVING1_DQDRVP2                               Fld(5, 5) //[9:5]
    #define SHU3_DRVING1_DQSDRVN1                              Fld(5, 10) //[14:10]
    #define SHU3_DRVING1_DQSDRVP1                              Fld(5, 15) //[19:15]
    #define SHU3_DRVING1_DQSDRVN2                              Fld(5, 20) //[24:20]
    #define SHU3_DRVING1_DQSDRVP2                              Fld(5, 25) //[29:25]
    #define SHU3_DRVING1_DIS_IMP_ODTN_TRACK                    Fld(1, 30) //[30:30]
    #define SHU3_DRVING1_DIS_IMPCAL_HW                         Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU3_DRVING2                             (DRAMC_AO_BASE_ADDRESS + 0x014AC)
    #define SHU3_DRVING2_CMDDRVN1                              Fld(5, 0) //[4:0]
    #define SHU3_DRVING2_CMDDRVP1                              Fld(5, 5) //[9:5]
    #define SHU3_DRVING2_CMDDRVN2                              Fld(5, 10) //[14:10]
    #define SHU3_DRVING2_CMDDRVP2                              Fld(5, 15) //[19:15]
    #define SHU3_DRVING2_DQDRVN1                               Fld(5, 20) //[24:20]
    #define SHU3_DRVING2_DQDRVP1                               Fld(5, 25) //[29:25]
    #define SHU3_DRVING2_DIS_IMPCAL_ODT_EN                     Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU3_DRVING3                             (DRAMC_AO_BASE_ADDRESS + 0x014B0)
    #define SHU3_DRVING3_DQODTN2                               Fld(5, 0) //[4:0]
    #define SHU3_DRVING3_DQODTP2                               Fld(5, 5) //[9:5]
    #define SHU3_DRVING3_DQSODTN                               Fld(5, 10) //[14:10]
    #define SHU3_DRVING3_DQSODTP                               Fld(5, 15) //[19:15]
    #define SHU3_DRVING3_DQSODTN2                              Fld(5, 20) //[24:20]
    #define SHU3_DRVING3_DQSODTP2                              Fld(5, 25) //[29:25]

#define DRAMC_REG_SHU3_DRVING4                             (DRAMC_AO_BASE_ADDRESS + 0x014B4)
    #define SHU3_DRVING4_CMDODTN1                              Fld(5, 0) //[4:0]
    #define SHU3_DRVING4_CMDODTP1                              Fld(5, 5) //[9:5]
    #define SHU3_DRVING4_CMDODTN2                              Fld(5, 10) //[14:10]
    #define SHU3_DRVING4_CMDODTP2                              Fld(5, 15) //[19:15]
    #define SHU3_DRVING4_DQODTN1                               Fld(5, 20) //[24:20]
    #define SHU3_DRVING4_DQODTP1                               Fld(5, 25) //[29:25]

#define DRAMC_REG_SHU3_DRVING5                             (DRAMC_AO_BASE_ADDRESS + 0x014B8)
    #define SHU3_DRVING5_DQCODTN2                              Fld(5, 0) //[4:0]
    #define SHU3_DRVING5_DQCODTP2                              Fld(5, 5) //[9:5]
    #define SHU3_DRVING5_DQCDRVN1                              Fld(5, 10) //[14:10]
    #define SHU3_DRVING5_DQCDRVP1                              Fld(5, 15) //[19:15]
    #define SHU3_DRVING5_DQCDRVN2                              Fld(5, 20) //[24:20]
    #define SHU3_DRVING5_DQCDRVP2                              Fld(5, 25) //[29:25]

#define DRAMC_REG_SHU3_DRVING6                             (DRAMC_AO_BASE_ADDRESS + 0x014BC)
    #define SHU3_DRVING6_DQCODTN1                              Fld(5, 20) //[24:20]
    #define SHU3_DRVING6_DQCODTP1                              Fld(5, 25) //[29:25]

#define DRAMC_REG_SHU3_WODT                                (DRAMC_AO_BASE_ADDRESS + 0x014C0)
    #define SHU3_WODT_DISWODT                                  Fld(3, 0) //[2:0]
    #define SHU3_WODT_WODTFIX                                  Fld(1, 3) //[3:3]
    #define SHU3_WODT_WODTFIXOFF                               Fld(1, 4) //[4:4]
    #define SHU3_WODT_DISWODTE                                 Fld(1, 5) //[5:5]
    #define SHU3_WODT_DISWODTE2                                Fld(1, 6) //[6:6]
    #define SHU3_WODT_WODTPDEN                                 Fld(1, 7) //[7:7]
    #define SHU3_WODT_DQOE_CNT                                 Fld(3, 8) //[10:8]
    #define SHU3_WODT_DQOE_OPT                                 Fld(1, 11) //[11:11]
    #define SHU3_WODT_TXUPD_SEL                                Fld(2, 12) //[13:12]
    #define SHU3_WODT_TXUPD_W2R_SEL                            Fld(3, 14) //[16:14]
    #define SHU3_WODT_DBIWR                                    Fld(1, 29) //[29:29]
    #define SHU3_WODT_TWPSTEXT                                 Fld(1, 30) //[30:30]
    #define SHU3_WODT_WPST2T                                   Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU3_DQSG                                (DRAMC_AO_BASE_ADDRESS + 0x014C4)
    #define SHU3_DQSG_DLLFRZRFCOPT                             Fld(2, 0) //[1:0]
    #define SHU3_DQSG_DLLFRZWROPT                              Fld(2, 4) //[5:4]
    #define SHU3_DQSG_R_RSTBCNT_LATCH_OPT                      Fld(3, 8) //[10:8]
    #define SHU3_DQSG_STB_UPDMASK_EN                           Fld(1, 11) //[11:11]
    #define SHU3_DQSG_STB_UPDMASKCYC                           Fld(4, 12) //[15:12]
    #define SHU3_DQSG_DQSINCTL_PRE_SEL                         Fld(1, 16) //[16:16]
    #define SHU3_DQSG_SCINTV                                   Fld(6, 20) //[25:20]

#define DRAMC_REG_SHU3_SCINTV                              (DRAMC_AO_BASE_ADDRESS + 0x014C8)
    #define SHU3_SCINTV_ODTREN                                 Fld(1, 0) //[0:0]
    #define SHU3_SCINTV_TZQLAT                                 Fld(5, 1) //[5:1]
    #define SHU3_SCINTV_TZQLAT2                                Fld(5, 6) //[10:6]
    #define SHU3_SCINTV_RDDQC_INTV                             Fld(2, 11) //[12:11]
    #define SHU3_SCINTV_MRW_INTV                               Fld(5, 13) //[17:13]
    #define SHU3_SCINTV_DQS2DQ_SHU_PITHRD                      Fld(6, 18) //[23:18]
    #define SHU3_SCINTV_DQS2DQ_FILT_PITHRD                     Fld(6, 24) //[29:24]
    #define SHU3_SCINTV_DQSOSCENDIS                            Fld(1, 30) //[30:30]

#define DRAMC_REG_SHU3_MISC                                (DRAMC_AO_BASE_ADDRESS + 0x014CC)
    #define SHU3_MISC_REQQUE_MAXCNT                            Fld(4, 0) //[3:0]
    #define SHU3_MISC_CKEHCMD                                  Fld(2, 4) //[5:4]
    #define SHU3_MISC_NORMPOP_LEN                              Fld(3, 8) //[10:8]
    #define SHU3_MISC_PREA_INTV                                Fld(5, 12) //[16:12]

#define DRAMC_REG_SHU3_DQS2DQ_TX                           (DRAMC_AO_BASE_ADDRESS + 0x014D0)
    #define SHU3_DQS2DQ_TX_OE2DQ_OFFSET                        Fld(5, 0) //[4:0]

#define DRAMC_REG_SHU3_HWSET_MR2                           (DRAMC_AO_BASE_ADDRESS + 0x014D4)
    #define SHU3_HWSET_MR2_HWSET_MR2_MRSMA                     Fld(13, 0) //[12:0]
    #define SHU3_HWSET_MR2_HWSET_MR2_OP                        Fld(8, 16) //[23:16]

#define DRAMC_REG_SHU3_HWSET_MR13                          (DRAMC_AO_BASE_ADDRESS + 0x014D8)
    #define SHU3_HWSET_MR13_HWSET_MR13_MRSMA                   Fld(13, 0) //[12:0]
    #define SHU3_HWSET_MR13_HWSET_MR13_OP                      Fld(8, 16) //[23:16]

#define DRAMC_REG_SHU3_HWSET_VRCG                          (DRAMC_AO_BASE_ADDRESS + 0x014DC)
    #define SHU3_HWSET_VRCG_HWSET_VRCG_MRSMA                   Fld(13, 0) //[12:0]
    #define SHU3_HWSET_VRCG_HWSET_VRCG_OP                      Fld(8, 16) //[23:16]

#define DRAMC_REG_SHU3_APHY_TX_PICG_CTRL                   (DRAMC_AO_BASE_ADDRESS + 0x014E4)
    #define SHU3_APHY_TX_PICG_CTRL_APHYPI_CG_CK_SEL            Fld(4, 20) //[23:20]
    #define SHU3_APHY_TX_PICG_CTRL_APHYPI_CG_CK_OPT            Fld(1, 24) //[24:24]
    #define SHU3_APHY_TX_PICG_CTRL_DDRPHY_CLK_DYN_GATING_SEL   Fld(4, 27) //[30:27]
    #define SHU3_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_OPT           Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU3RK0_DQSCTL                           (DRAMC_AO_BASE_ADDRESS + 0x01600)
    #define SHU3RK0_DQSCTL_DQSINCTL                            Fld(4, 0) //[3:0]

#define DRAMC_REG_SHU3RK0_DQSIEN                           (DRAMC_AO_BASE_ADDRESS + 0x01604)
    #define SHU3RK0_DQSIEN_R0DQS0IEN                           Fld(7, 0) //[6:0]
    #define SHU3RK0_DQSIEN_R0DQS1IEN                           Fld(7, 8) //[14:8]
    #define SHU3RK0_DQSIEN_R0DQS2IEN                           Fld(7, 16) //[22:16]
    #define SHU3RK0_DQSIEN_R0DQS3IEN                           Fld(7, 24) //[30:24]

#define DRAMC_REG_SHU3RK0_DQSCAL                           (DRAMC_AO_BASE_ADDRESS + 0x01608)
    #define SHU3RK0_DQSCAL_R0DQSIENLLMT                        Fld(7, 0) //[6:0]
    #define SHU3RK0_DQSCAL_R0DQSIENLLMTEN                      Fld(1, 7) //[7:7]
    #define SHU3RK0_DQSCAL_R0DQSIENHLMT                        Fld(7, 8) //[14:8]
    #define SHU3RK0_DQSCAL_R0DQSIENHLMTEN                      Fld(1, 15) //[15:15]

#define DRAMC_REG_SHU3RK0_PI                               (DRAMC_AO_BASE_ADDRESS + 0x0160C)
    #define SHU3RK0_PI_RK0_ARPI_DQ_B1                          Fld(6, 0) //[5:0]
    #define SHU3RK0_PI_RK0_ARPI_DQ_B0                          Fld(6, 8) //[13:8]
    #define SHU3RK0_PI_RK0_ARPI_DQM_B1                         Fld(6, 16) //[21:16]
    #define SHU3RK0_PI_RK0_ARPI_DQM_B0                         Fld(6, 24) //[29:24]

#define DRAMC_REG_SHU3RK0_DQSOSC                           (DRAMC_AO_BASE_ADDRESS + 0x01610)
    #define SHU3RK0_DQSOSC_DQSOSC_BASE_RK0                     Fld(16, 0) //[15:0]
    #define SHU3RK0_DQSOSC_DQSOSC_BASE_RK0_B1                  Fld(16, 16) //[31:16]

#define DRAMC_REG_SHU3RK0_SELPH_ODTEN0                     (DRAMC_AO_BASE_ADDRESS + 0x0161C)
    #define SHU3RK0_SELPH_ODTEN0_TXDLY_B0_RODTEN               Fld(3, 0) //[2:0]
    #define SHU3RK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1            Fld(3, 4) //[6:4]
    #define SHU3RK0_SELPH_ODTEN0_TXDLY_B1_RODTEN               Fld(3, 8) //[10:8]
    #define SHU3RK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1            Fld(3, 12) //[14:12]
    #define SHU3RK0_SELPH_ODTEN0_TXDLY_B2_RODTEN               Fld(3, 16) //[18:16]
    #define SHU3RK0_SELPH_ODTEN0_TXDLY_B2_RODTEN_P1            Fld(3, 20) //[22:20]
    #define SHU3RK0_SELPH_ODTEN0_TXDLY_B3_RODTEN               Fld(3, 24) //[26:24]
    #define SHU3RK0_SELPH_ODTEN0_TXDLY_B3_RODTEN_P1            Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3RK0_SELPH_ODTEN1                     (DRAMC_AO_BASE_ADDRESS + 0x01620)
    #define SHU3RK0_SELPH_ODTEN1_DLY_B0_RODTEN                 Fld(3, 0) //[2:0]
    #define SHU3RK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1              Fld(3, 4) //[6:4]
    #define SHU3RK0_SELPH_ODTEN1_DLY_B1_RODTEN                 Fld(3, 8) //[10:8]
    #define SHU3RK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1              Fld(3, 12) //[14:12]
    #define SHU3RK0_SELPH_ODTEN1_DLY_B2_RODTEN                 Fld(3, 16) //[18:16]
    #define SHU3RK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1              Fld(3, 20) //[22:20]
    #define SHU3RK0_SELPH_ODTEN1_DLY_B3_RODTEN                 Fld(3, 24) //[26:24]
    #define SHU3RK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1              Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3RK0_SELPH_DQSG0                      (DRAMC_AO_BASE_ADDRESS + 0x01624)
    #define SHU3RK0_SELPH_DQSG0_TX_DLY_DQS0_GATED              Fld(3, 0) //[2:0]
    #define SHU3RK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1           Fld(3, 4) //[6:4]
    #define SHU3RK0_SELPH_DQSG0_TX_DLY_DQS1_GATED              Fld(3, 8) //[10:8]
    #define SHU3RK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1           Fld(3, 12) //[14:12]
    #define SHU3RK0_SELPH_DQSG0_TX_DLY_DQS2_GATED              Fld(3, 16) //[18:16]
    #define SHU3RK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1           Fld(3, 20) //[22:20]
    #define SHU3RK0_SELPH_DQSG0_TX_DLY_DQS3_GATED              Fld(3, 24) //[26:24]
    #define SHU3RK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1           Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3RK0_SELPH_DQSG1                      (DRAMC_AO_BASE_ADDRESS + 0x01628)
    #define SHU3RK0_SELPH_DQSG1_REG_DLY_DQS0_GATED             Fld(3, 0) //[2:0]
    #define SHU3RK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1          Fld(3, 4) //[6:4]
    #define SHU3RK0_SELPH_DQSG1_REG_DLY_DQS1_GATED             Fld(3, 8) //[10:8]
    #define SHU3RK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1          Fld(3, 12) //[14:12]
    #define SHU3RK0_SELPH_DQSG1_REG_DLY_DQS2_GATED             Fld(3, 16) //[18:16]
    #define SHU3RK0_SELPH_DQSG1_REG_DLY_DQS2_GATED_P1          Fld(3, 20) //[22:20]
    #define SHU3RK0_SELPH_DQSG1_REG_DLY_DQS3_GATED             Fld(3, 24) //[26:24]
    #define SHU3RK0_SELPH_DQSG1_REG_DLY_DQS3_GATED_P1          Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3RK0_SELPH_DQ0                        (DRAMC_AO_BASE_ADDRESS + 0x0162C)
    #define SHU3RK0_SELPH_DQ0_TXDLY_DQ0                        Fld(3, 0) //[2:0]
    #define SHU3RK0_SELPH_DQ0_TXDLY_DQ1                        Fld(3, 4) //[6:4]
    #define SHU3RK0_SELPH_DQ0_TXDLY_DQ2                        Fld(3, 8) //[10:8]
    #define SHU3RK0_SELPH_DQ0_TXDLY_DQ3                        Fld(3, 12) //[14:12]
    #define SHU3RK0_SELPH_DQ0_TXDLY_OEN_DQ0                    Fld(3, 16) //[18:16]
    #define SHU3RK0_SELPH_DQ0_TXDLY_OEN_DQ1                    Fld(3, 20) //[22:20]
    #define SHU3RK0_SELPH_DQ0_TXDLY_OEN_DQ2                    Fld(3, 24) //[26:24]
    #define SHU3RK0_SELPH_DQ0_TXDLY_OEN_DQ3                    Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3RK0_SELPH_DQ1                        (DRAMC_AO_BASE_ADDRESS + 0x01630)
    #define SHU3RK0_SELPH_DQ1_TXDLY_DQM0                       Fld(3, 0) //[2:0]
    #define SHU3RK0_SELPH_DQ1_TXDLY_DQM1                       Fld(3, 4) //[6:4]
    #define SHU3RK0_SELPH_DQ1_TXDLY_DQM2                       Fld(3, 8) //[10:8]
    #define SHU3RK0_SELPH_DQ1_TXDLY_DQM3                       Fld(3, 12) //[14:12]
    #define SHU3RK0_SELPH_DQ1_TXDLY_OEN_DQM0                   Fld(3, 16) //[18:16]
    #define SHU3RK0_SELPH_DQ1_TXDLY_OEN_DQM1                   Fld(3, 20) //[22:20]
    #define SHU3RK0_SELPH_DQ1_TXDLY_OEN_DQM2                   Fld(3, 24) //[26:24]
    #define SHU3RK0_SELPH_DQ1_TXDLY_OEN_DQM3                   Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3RK0_SELPH_DQ2                        (DRAMC_AO_BASE_ADDRESS + 0x01634)
    #define SHU3RK0_SELPH_DQ2_DLY_DQ0                          Fld(3, 0) //[2:0]
    #define SHU3RK0_SELPH_DQ2_DLY_DQ1                          Fld(3, 4) //[6:4]
    #define SHU3RK0_SELPH_DQ2_DLY_DQ2                          Fld(3, 8) //[10:8]
    #define SHU3RK0_SELPH_DQ2_DLY_DQ3                          Fld(3, 12) //[14:12]
    #define SHU3RK0_SELPH_DQ2_DLY_OEN_DQ0                      Fld(3, 16) //[18:16]
    #define SHU3RK0_SELPH_DQ2_DLY_OEN_DQ1                      Fld(3, 20) //[22:20]
    #define SHU3RK0_SELPH_DQ2_DLY_OEN_DQ2                      Fld(3, 24) //[26:24]
    #define SHU3RK0_SELPH_DQ2_DLY_OEN_DQ3                      Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3RK0_SELPH_DQ3                        (DRAMC_AO_BASE_ADDRESS + 0x01638)
    #define SHU3RK0_SELPH_DQ3_DLY_DQM0                         Fld(3, 0) //[2:0]
    #define SHU3RK0_SELPH_DQ3_DLY_DQM1                         Fld(3, 4) //[6:4]
    #define SHU3RK0_SELPH_DQ3_DLY_DQM2                         Fld(3, 8) //[10:8]
    #define SHU3RK0_SELPH_DQ3_DLY_DQM3                         Fld(3, 12) //[14:12]
    #define SHU3RK0_SELPH_DQ3_DLY_OEN_DQM0                     Fld(3, 16) //[18:16]
    #define SHU3RK0_SELPH_DQ3_DLY_OEN_DQM1                     Fld(3, 20) //[22:20]
    #define SHU3RK0_SELPH_DQ3_DLY_OEN_DQM2                     Fld(3, 24) //[26:24]
    #define SHU3RK0_SELPH_DQ3_DLY_OEN_DQM3                     Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3RK0_DQS2DQ_CAL1                      (DRAMC_AO_BASE_ADDRESS + 0x01640)
    #define SHU3RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0           Fld(11, 0) //[10:0]
    #define SHU3RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1           Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU3RK0_DQS2DQ_CAL2                      (DRAMC_AO_BASE_ADDRESS + 0x01644)
    #define SHU3RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0           Fld(11, 0) //[10:0]
    #define SHU3RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1           Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU3RK0_DQS2DQ_CAL3                      (DRAMC_AO_BASE_ADDRESS + 0x01648)
    #define SHU3RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0       Fld(6, 0) //[5:0]
    #define SHU3RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1       Fld(6, 6) //[11:6]
    #define SHU3RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0_B4TO0 Fld(5, 12) //[16:12]
    #define SHU3RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1_B4TO0 Fld(5, 17) //[21:17]

#define DRAMC_REG_SHU3RK0_DQS2DQ_CAL4                      (DRAMC_AO_BASE_ADDRESS + 0x0164C)
    #define SHU3RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0      Fld(6, 0) //[5:0]
    #define SHU3RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1      Fld(6, 6) //[11:6]
    #define SHU3RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0_B4TO0 Fld(5, 12) //[16:12]
    #define SHU3RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1_B4TO0 Fld(5, 17) //[21:17]

#define DRAMC_REG_SHU3RK0_DQS2DQ_CAL5                      (DRAMC_AO_BASE_ADDRESS + 0x01650)
    #define SHU3RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0          Fld(11, 0) //[10:0]
    #define SHU3RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1          Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU3RK1_DQSCTL                           (DRAMC_AO_BASE_ADDRESS + 0x01700)
    #define SHU3RK1_DQSCTL_R1DQSINCTL                          Fld(4, 0) //[3:0]

#define DRAMC_REG_SHU3RK1_DQSIEN                           (DRAMC_AO_BASE_ADDRESS + 0x01704)
    #define SHU3RK1_DQSIEN_R1DQS0IEN                           Fld(7, 0) //[6:0]
    #define SHU3RK1_DQSIEN_R1DQS1IEN                           Fld(7, 8) //[14:8]
    #define SHU3RK1_DQSIEN_R1DQS2IEN                           Fld(7, 16) //[22:16]
    #define SHU3RK1_DQSIEN_R1DQS3IEN                           Fld(7, 24) //[30:24]

#define DRAMC_REG_SHU3RK1_DQSCAL                           (DRAMC_AO_BASE_ADDRESS + 0x01708)
    #define SHU3RK1_DQSCAL_R1DQSIENLLMT                        Fld(7, 0) //[6:0]
    #define SHU3RK1_DQSCAL_R1DQSIENLLMTEN                      Fld(1, 7) //[7:7]
    #define SHU3RK1_DQSCAL_R1DQSIENHLMT                        Fld(7, 8) //[14:8]
    #define SHU3RK1_DQSCAL_R1DQSIENHLMTEN                      Fld(1, 15) //[15:15]

#define DRAMC_REG_SHU3RK1_PI                               (DRAMC_AO_BASE_ADDRESS + 0x0170C)
    #define SHU3RK1_PI_RK1_ARPI_DQ_B1                          Fld(6, 0) //[5:0]
    #define SHU3RK1_PI_RK1_ARPI_DQ_B0                          Fld(6, 8) //[13:8]
    #define SHU3RK1_PI_RK1_ARPI_DQM_B1                         Fld(6, 16) //[21:16]
    #define SHU3RK1_PI_RK1_ARPI_DQM_B0                         Fld(6, 24) //[29:24]

#define DRAMC_REG_SHU3RK1_DQSOSC                           (DRAMC_AO_BASE_ADDRESS + 0x01710)
    #define SHU3RK1_DQSOSC_DQSOSC_BASE_RK1                     Fld(16, 0) //[15:0]
    #define SHU3RK1_DQSOSC_DQSOSC_BASE_RK1_B1                  Fld(16, 16) //[31:16]

#define DRAMC_REG_SHU3RK1_SELPH_ODTEN0                     (DRAMC_AO_BASE_ADDRESS + 0x0171C)
    #define SHU3RK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN             Fld(3, 0) //[2:0]
    #define SHU3RK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN_P1          Fld(3, 4) //[6:4]
    #define SHU3RK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN             Fld(3, 8) //[10:8]
    #define SHU3RK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN_P1          Fld(3, 12) //[14:12]
    #define SHU3RK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN             Fld(3, 16) //[18:16]
    #define SHU3RK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN_P1          Fld(3, 20) //[22:20]
    #define SHU3RK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN             Fld(3, 24) //[26:24]
    #define SHU3RK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN_P1          Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3RK1_SELPH_ODTEN1                     (DRAMC_AO_BASE_ADDRESS + 0x01720)
    #define SHU3RK1_SELPH_ODTEN1_DLY_B0_R1RODTEN               Fld(3, 0) //[2:0]
    #define SHU3RK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1            Fld(3, 4) //[6:4]
    #define SHU3RK1_SELPH_ODTEN1_DLY_B1_R1RODTEN               Fld(3, 8) //[10:8]
    #define SHU3RK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1            Fld(3, 12) //[14:12]
    #define SHU3RK1_SELPH_ODTEN1_DLY_B2_R1RODTEN               Fld(3, 16) //[18:16]
    #define SHU3RK1_SELPH_ODTEN1_DLY_B2_R1RODTEN_P1            Fld(3, 20) //[22:20]
    #define SHU3RK1_SELPH_ODTEN1_DLY_B3_R1RODTEN               Fld(3, 24) //[26:24]
    #define SHU3RK1_SELPH_ODTEN1_DLY_B3_R1RODTEN_P1            Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3RK1_SELPH_DQSG0                      (DRAMC_AO_BASE_ADDRESS + 0x01724)
    #define SHU3RK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED            Fld(3, 0) //[2:0]
    #define SHU3RK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1         Fld(3, 4) //[6:4]
    #define SHU3RK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED            Fld(3, 8) //[10:8]
    #define SHU3RK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1         Fld(3, 12) //[14:12]
    #define SHU3RK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED            Fld(3, 16) //[18:16]
    #define SHU3RK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED_P1         Fld(3, 20) //[22:20]
    #define SHU3RK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED            Fld(3, 24) //[26:24]
    #define SHU3RK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED_P1         Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3RK1_SELPH_DQSG1                      (DRAMC_AO_BASE_ADDRESS + 0x01728)
    #define SHU3RK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED           Fld(3, 0) //[2:0]
    #define SHU3RK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1        Fld(3, 4) //[6:4]
    #define SHU3RK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED           Fld(3, 8) //[10:8]
    #define SHU3RK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1        Fld(3, 12) //[14:12]
    #define SHU3RK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED           Fld(3, 16) //[18:16]
    #define SHU3RK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED_P1        Fld(3, 20) //[22:20]
    #define SHU3RK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED           Fld(3, 24) //[26:24]
    #define SHU3RK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED_P1        Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3RK1_SELPH_DQ0                        (DRAMC_AO_BASE_ADDRESS + 0x0172C)
    #define SHU3RK1_SELPH_DQ0_TX_DLY_R1DQ0                     Fld(3, 0) //[2:0]
    #define SHU3RK1_SELPH_DQ0_TX_DLY_R1DQ1                     Fld(3, 4) //[6:4]
    #define SHU3RK1_SELPH_DQ0_TX_DLY_R1DQ2                     Fld(3, 8) //[10:8]
    #define SHU3RK1_SELPH_DQ0_TX_DLY_R1DQ3                     Fld(3, 12) //[14:12]
    #define SHU3RK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0                 Fld(3, 16) //[18:16]
    #define SHU3RK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1                 Fld(3, 20) //[22:20]
    #define SHU3RK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2                 Fld(3, 24) //[26:24]
    #define SHU3RK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3                 Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3RK1_SELPH_DQ1                        (DRAMC_AO_BASE_ADDRESS + 0x01730)
    #define SHU3RK1_SELPH_DQ1_TX_DLY_R1DQM0                    Fld(3, 0) //[2:0]
    #define SHU3RK1_SELPH_DQ1_TX_DLY_R1DQM1                    Fld(3, 4) //[6:4]
    #define SHU3RK1_SELPH_DQ1_TX_DLY_R1DQM2                    Fld(3, 8) //[10:8]
    #define SHU3RK1_SELPH_DQ1_TX_DLY_R1DQM3                    Fld(3, 12) //[14:12]
    #define SHU3RK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0                Fld(3, 16) //[18:16]
    #define SHU3RK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1                Fld(3, 20) //[22:20]
    #define SHU3RK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2                Fld(3, 24) //[26:24]
    #define SHU3RK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3                Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3RK1_SELPH_DQ2                        (DRAMC_AO_BASE_ADDRESS + 0x01734)
    #define SHU3RK1_SELPH_DQ2_DLY_R1DQ0                        Fld(3, 0) //[2:0]
    #define SHU3RK1_SELPH_DQ2_DLY_R1DQ1                        Fld(3, 4) //[6:4]
    #define SHU3RK1_SELPH_DQ2_DLY_R1DQ2                        Fld(3, 8) //[10:8]
    #define SHU3RK1_SELPH_DQ2_DLY_R1DQ3                        Fld(3, 12) //[14:12]
    #define SHU3RK1_SELPH_DQ2_DLY_R1OEN_DQ0                    Fld(3, 16) //[18:16]
    #define SHU3RK1_SELPH_DQ2_DLY_R1OEN_DQ1                    Fld(3, 20) //[22:20]
    #define SHU3RK1_SELPH_DQ2_DLY_R1OEN_DQ2                    Fld(3, 24) //[26:24]
    #define SHU3RK1_SELPH_DQ2_DLY_R1OEN_DQ3                    Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3RK1_SELPH_DQ3                        (DRAMC_AO_BASE_ADDRESS + 0x01738)
    #define SHU3RK1_SELPH_DQ3_DLY_R1DQM0                       Fld(3, 0) //[2:0]
    #define SHU3RK1_SELPH_DQ3_DLY_R1DQM1                       Fld(3, 4) //[6:4]
    #define SHU3RK1_SELPH_DQ3_DLY_R1DQM2                       Fld(3, 8) //[10:8]
    #define SHU3RK1_SELPH_DQ3_DLY_R1DQM3                       Fld(3, 12) //[14:12]
    #define SHU3RK1_SELPH_DQ3_DLY_R1OEN_DQM0                   Fld(3, 16) //[18:16]
    #define SHU3RK1_SELPH_DQ3_DLY_R1OEN_DQM1                   Fld(3, 20) //[22:20]
    #define SHU3RK1_SELPH_DQ3_DLY_R1OEN_DQM2                   Fld(3, 24) //[26:24]
    #define SHU3RK1_SELPH_DQ3_DLY_R1OEN_DQM3                   Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3RK1_DQS2DQ_CAL1                      (DRAMC_AO_BASE_ADDRESS + 0x01740)
    #define SHU3RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0           Fld(11, 0) //[10:0]
    #define SHU3RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1           Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU3RK1_DQS2DQ_CAL2                      (DRAMC_AO_BASE_ADDRESS + 0x01744)
    #define SHU3RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0           Fld(11, 0) //[10:0]
    #define SHU3RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1           Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU3RK1_DQS2DQ_CAL3                      (DRAMC_AO_BASE_ADDRESS + 0x01748)
    #define SHU3RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0       Fld(6, 0) //[5:0]
    #define SHU3RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1       Fld(6, 6) //[11:6]
    #define SHU3RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0_B4TO0 Fld(5, 12) //[16:12]
    #define SHU3RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1_B4TO0 Fld(5, 17) //[21:17]

#define DRAMC_REG_SHU3RK1_DQS2DQ_CAL4                      (DRAMC_AO_BASE_ADDRESS + 0x0174C)
    #define SHU3RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0      Fld(6, 0) //[5:0]
    #define SHU3RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1      Fld(6, 6) //[11:6]
    #define SHU3RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0_B4TO0 Fld(5, 12) //[16:12]
    #define SHU3RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1_B4TO0 Fld(5, 17) //[21:17]

#define DRAMC_REG_SHU3RK1_DQS2DQ_CAL5                      (DRAMC_AO_BASE_ADDRESS + 0x01750)
    #define SHU3RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0          Fld(11, 0) //[10:0]
    #define SHU3RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1          Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU3RK2_DQSCTL                           (DRAMC_AO_BASE_ADDRESS + 0x01800)
    #define SHU3RK2_DQSCTL_R2DQSINCTL                          Fld(4, 0) //[3:0]

#define DRAMC_REG_SHU3RK2_DQSIEN                           (DRAMC_AO_BASE_ADDRESS + 0x01804)
    #define SHU3RK2_DQSIEN_R2DQS0IEN                           Fld(7, 0) //[6:0]
    #define SHU3RK2_DQSIEN_R2DQS1IEN                           Fld(7, 8) //[14:8]
    #define SHU3RK2_DQSIEN_R2DQS2IEN                           Fld(7, 16) //[22:16]
    #define SHU3RK2_DQSIEN_R2DQS3IEN                           Fld(7, 24) //[30:24]

#define DRAMC_REG_SHU3RK2_DQSCAL                           (DRAMC_AO_BASE_ADDRESS + 0x01808)
    #define SHU3RK2_DQSCAL_R2DQSIENLLMT                        Fld(7, 0) //[6:0]
    #define SHU3RK2_DQSCAL_R2DQSIENLLMTEN                      Fld(1, 7) //[7:7]
    #define SHU3RK2_DQSCAL_R2DQSIENHLMT                        Fld(7, 8) //[14:8]
    #define SHU3RK2_DQSCAL_R2DQSIENHLMTEN                      Fld(1, 15) //[15:15]

#define DRAMC_REG_SHU3RK2_PI                               (DRAMC_AO_BASE_ADDRESS + 0x0180C)
    #define SHU3RK2_PI_RK2_ARPI_DQ_B1                          Fld(6, 0) //[5:0]
    #define SHU3RK2_PI_RK2_ARPI_DQ_B0                          Fld(6, 8) //[13:8]
    #define SHU3RK2_PI_RK2_ARPI_DQM_B1                         Fld(6, 16) //[21:16]
    #define SHU3RK2_PI_RK2_ARPI_DQM_B0                         Fld(6, 24) //[29:24]

#define DRAMC_REG_SHU3RK2_DQSOSC                           (DRAMC_AO_BASE_ADDRESS + 0x01810)
    #define SHU3RK2_DQSOSC_DQSOSC_BASE_RK2                     Fld(16, 0) //[15:0]
    #define SHU3RK2_DQSOSC_DQSOSC_BASE_RK2_B1                  Fld(16, 16) //[31:16]

#define DRAMC_REG_SHU3RK2_SELPH_ODTEN0                     (DRAMC_AO_BASE_ADDRESS + 0x0181C)
    #define SHU3RK2_SELPH_ODTEN0_TXDLY_B0_R2RODTEN             Fld(3, 0) //[2:0]
    #define SHU3RK2_SELPH_ODTEN0_TXDLY_B0_R2RODTEN_P1          Fld(3, 4) //[6:4]
    #define SHU3RK2_SELPH_ODTEN0_TXDLY_B1_R2RODTEN             Fld(3, 8) //[10:8]
    #define SHU3RK2_SELPH_ODTEN0_TXDLY_B1_R2RODTEN_P1          Fld(3, 12) //[14:12]
    #define SHU3RK2_SELPH_ODTEN0_TXDLY_B2_R2RODTEN             Fld(3, 16) //[18:16]
    #define SHU3RK2_SELPH_ODTEN0_TXDLY_B2_R2RODTEN_P1          Fld(3, 20) //[22:20]
    #define SHU3RK2_SELPH_ODTEN0_TXDLY_B3_R2RODTEN             Fld(3, 24) //[26:24]
    #define SHU3RK2_SELPH_ODTEN0_TXDLY_B3_R2RODTEN_P1          Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3RK2_SELPH_ODTEN1                     (DRAMC_AO_BASE_ADDRESS + 0x01820)
    #define SHU3RK2_SELPH_ODTEN1_DLY_B0_R2RODTEN               Fld(3, 0) //[2:0]
    #define SHU3RK2_SELPH_ODTEN1_DLY_B0_R2RODTEN_P1            Fld(3, 4) //[6:4]
    #define SHU3RK2_SELPH_ODTEN1_DLY_B1_R2RODTEN               Fld(3, 8) //[10:8]
    #define SHU3RK2_SELPH_ODTEN1_DLY_B1_R2RODTEN_P1            Fld(3, 12) //[14:12]
    #define SHU3RK2_SELPH_ODTEN1_DLY_B2_R2RODTEN               Fld(3, 16) //[18:16]
    #define SHU3RK2_SELPH_ODTEN1_DLY_B2_R2RODTEN_P1            Fld(3, 20) //[22:20]
    #define SHU3RK2_SELPH_ODTEN1_DLY_B3_R2RODTEN               Fld(3, 24) //[26:24]
    #define SHU3RK2_SELPH_ODTEN1_DLY_B3_R2RODTEN_P1            Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3RK2_SELPH_DQSG0                      (DRAMC_AO_BASE_ADDRESS + 0x01824)
    #define SHU3RK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED            Fld(3, 0) //[2:0]
    #define SHU3RK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED_P1         Fld(3, 4) //[6:4]
    #define SHU3RK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED            Fld(3, 8) //[10:8]
    #define SHU3RK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED_P1         Fld(3, 12) //[14:12]
    #define SHU3RK2_SELPH_DQSG0_TX_DLY_R2DQS2_GATED            Fld(3, 16) //[18:16]
    #define SHU3RK2_SELPH_DQSG0_TX_DLY_R2DQS2_GATED_P1         Fld(3, 20) //[22:20]
    #define SHU3RK2_SELPH_DQSG0_TX_DLY_R2DQS3_GATED            Fld(3, 24) //[26:24]
    #define SHU3RK2_SELPH_DQSG0_TX_DLY_R2DQS3_GATED_P1         Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3RK2_SELPH_DQSG1                      (DRAMC_AO_BASE_ADDRESS + 0x01828)
    #define SHU3RK2_SELPH_DQSG1_REG_DLY_R2DQS0_GATED           Fld(3, 0) //[2:0]
    #define SHU3RK2_SELPH_DQSG1_REG_DLY_R2DQS0_GATED_P1        Fld(3, 4) //[6:4]
    #define SHU3RK2_SELPH_DQSG1_REG_DLY_R2DQS1_GATED           Fld(3, 8) //[10:8]
    #define SHU3RK2_SELPH_DQSG1_REG_DLY_R2DQS1_GATED_P1        Fld(3, 12) //[14:12]
    #define SHU3RK2_SELPH_DQSG1_REG_DLY_R2DQS2_GATED           Fld(3, 16) //[18:16]
    #define SHU3RK2_SELPH_DQSG1_REG_DLY_R2DQS2_GATED_P1        Fld(3, 20) //[22:20]
    #define SHU3RK2_SELPH_DQSG1_REG_DLY_R2DQS3_GATED           Fld(3, 24) //[26:24]
    #define SHU3RK2_SELPH_DQSG1_REG_DLY_R2DQS3_GATED_P1        Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3RK2_SELPH_DQ0                        (DRAMC_AO_BASE_ADDRESS + 0x0182C)
    #define SHU3RK2_SELPH_DQ0_TX_DLY_R2DQ0                     Fld(3, 0) //[2:0]
    #define SHU3RK2_SELPH_DQ0_TX_DLY_R2DQ1                     Fld(3, 4) //[6:4]
    #define SHU3RK2_SELPH_DQ0_TX_DLY_R2DQ2                     Fld(3, 8) //[10:8]
    #define SHU3RK2_SELPH_DQ0_TX_DLY_R2DQ3                     Fld(3, 12) //[14:12]
    #define SHU3RK2_SELPH_DQ0_TX_DLY_R2OEN_DQ0                 Fld(3, 16) //[18:16]
    #define SHU3RK2_SELPH_DQ0_TX_DLY_R2OEN_DQ1                 Fld(3, 20) //[22:20]
    #define SHU3RK2_SELPH_DQ0_TX_DLY_R2OEN_DQ2                 Fld(3, 24) //[26:24]
    #define SHU3RK2_SELPH_DQ0_TX_DLY_R2OEN_DQ3                 Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3RK2_SELPH_DQ1                        (DRAMC_AO_BASE_ADDRESS + 0x01830)
    #define SHU3RK2_SELPH_DQ1_TX_DLY_R2DQM0                    Fld(3, 0) //[2:0]
    #define SHU3RK2_SELPH_DQ1_TX_DLY_R2DQM1                    Fld(3, 4) //[6:4]
    #define SHU3RK2_SELPH_DQ1_TX_DLY_R2DQM2                    Fld(3, 8) //[10:8]
    #define SHU3RK2_SELPH_DQ1_TX_DLY_R2DQM3                    Fld(3, 12) //[14:12]
    #define SHU3RK2_SELPH_DQ1_TX_DLY_R2OEN_DQM0                Fld(3, 16) //[18:16]
    #define SHU3RK2_SELPH_DQ1_TX_DLY_R2OEN_DQM1                Fld(3, 20) //[22:20]
    #define SHU3RK2_SELPH_DQ1_TX_DLY_R2OEN_DQM2                Fld(3, 24) //[26:24]
    #define SHU3RK2_SELPH_DQ1_TX_DLY_R2OEN_DQM3                Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3RK2_SELPH_DQ2                        (DRAMC_AO_BASE_ADDRESS + 0x01834)
    #define SHU3RK2_SELPH_DQ2_DLY_R2DQ0                        Fld(3, 0) //[2:0]
    #define SHU3RK2_SELPH_DQ2_DLY_R2DQ1                        Fld(3, 4) //[6:4]
    #define SHU3RK2_SELPH_DQ2_DLY_R2DQ2                        Fld(3, 8) //[10:8]
    #define SHU3RK2_SELPH_DQ2_DLY_R2DQ3                        Fld(3, 12) //[14:12]
    #define SHU3RK2_SELPH_DQ2_DLY_R2OEN_DQ0                    Fld(3, 16) //[18:16]
    #define SHU3RK2_SELPH_DQ2_DLY_R2OEN_DQ1                    Fld(3, 20) //[22:20]
    #define SHU3RK2_SELPH_DQ2_DLY_R2OEN_DQ2                    Fld(3, 24) //[26:24]
    #define SHU3RK2_SELPH_DQ2_DLY_R2OEN_DQ3                    Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3RK2_SELPH_DQ3                        (DRAMC_AO_BASE_ADDRESS + 0x01838)
    #define SHU3RK2_SELPH_DQ3_DLY_R2DQM0                       Fld(3, 0) //[2:0]
    #define SHU3RK2_SELPH_DQ3_DLY_R2DQM1                       Fld(3, 4) //[6:4]
    #define SHU3RK2_SELPH_DQ3_DLY_R2DQM2                       Fld(3, 8) //[10:8]
    #define SHU3RK2_SELPH_DQ3_DLY_R2DQM3                       Fld(3, 12) //[14:12]
    #define SHU3RK2_SELPH_DQ3_DLY_R2OEN_DQM0                   Fld(3, 16) //[18:16]
    #define SHU3RK2_SELPH_DQ3_DLY_R2OEN_DQM1                   Fld(3, 20) //[22:20]
    #define SHU3RK2_SELPH_DQ3_DLY_R2OEN_DQM2                   Fld(3, 24) //[26:24]
    #define SHU3RK2_SELPH_DQ3_DLY_R2OEN_DQM3                   Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU3RK2_DQS2DQ_CAL1                      (DRAMC_AO_BASE_ADDRESS + 0x01840)
    #define SHU3RK2_DQS2DQ_CAL1_BOOT_ORIG_UI_RK2_DQ0           Fld(11, 0) //[10:0]
    #define SHU3RK2_DQS2DQ_CAL1_BOOT_ORIG_UI_RK2_DQ1           Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU3RK2_DQS2DQ_CAL2                      (DRAMC_AO_BASE_ADDRESS + 0x01844)
    #define SHU3RK2_DQS2DQ_CAL2_BOOT_TARG_UI_RK2_DQ0           Fld(11, 0) //[10:0]
    #define SHU3RK2_DQS2DQ_CAL2_BOOT_TARG_UI_RK2_DQ1           Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU3RK2_DQS2DQ_CAL3                      (DRAMC_AO_BASE_ADDRESS + 0x01848)
    #define SHU3RK2_DQS2DQ_CAL3_BOOT_TARG_UI_RK2_OEN_DQ0       Fld(6, 0) //[5:0]
    #define SHU3RK2_DQS2DQ_CAL3_BOOT_TARG_UI_RK2_OEN_DQ1       Fld(6, 6) //[11:6]

#define DRAMC_REG_SHU3RK2_DQS2DQ_CAL4                      (DRAMC_AO_BASE_ADDRESS + 0x0184C)
    #define SHU3RK2_DQS2DQ_CAL4_BOOT_TARG_UI_RK2_OEN_DQM0      Fld(6, 0) //[5:0]
    #define SHU3RK2_DQS2DQ_CAL4_BOOT_TARG_UI_RK2_OEN_DQM1      Fld(6, 6) //[11:6]

#define DRAMC_REG_SHU3RK2_DQS2DQ_CAL5                      (DRAMC_AO_BASE_ADDRESS + 0x01850)
    #define SHU3RK2_DQS2DQ_CAL5_BOOT_TARG_UI_RK2_DQM0          Fld(11, 0) //[10:0]
    #define SHU3RK2_DQS2DQ_CAL5_BOOT_TARG_UI_RK2_DQM1          Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU3_DQSG_RETRY                          (DRAMC_AO_BASE_ADDRESS + 0x01854)
    #define SHU3_DQSG_RETRY_R_DQSGRETRY_SW_RESET               Fld(1, 0) //[0:0]
    #define SHU3_DQSG_RETRY_R_DQSG_RETRY_SW_EN                 Fld(1, 1) //[1:1]
    #define SHU3_DQSG_RETRY_R_DDR1866_PLUS                     Fld(1, 2) //[2:2]
    #define SHU3_DQSG_RETRY_R_RETRY_ONCE                       Fld(1, 3) //[3:3]
    #define SHU3_DQSG_RETRY_R_RETRY_3TIMES                     Fld(1, 4) //[4:4]
    #define SHU3_DQSG_RETRY_R_RETRY_1RANK                      Fld(1, 5) //[5:5]
    #define SHU3_DQSG_RETRY_R_RETRY_SAV_MSK                    Fld(1, 6) //[6:6]
    #define SHU3_DQSG_RETRY_R_DM4BYTE                          Fld(1, 7) //[7:7]
    #define SHU3_DQSG_RETRY_R_DQSIENLAT                        Fld(4, 8) //[11:8]
    #define SHU3_DQSG_RETRY_R_STBENCMP_ALLBYTE                 Fld(1, 12) //[12:12]
    #define SHU3_DQSG_RETRY_R_XSR_DQSG_RETRY_EN                Fld(1, 13) //[13:13]
    #define SHU3_DQSG_RETRY_R_XSR_RETRY_SPM_MODE               Fld(1, 14) //[14:14]
    #define SHU3_DQSG_RETRY_R_RETRY_CMP_DATA                   Fld(1, 15) //[15:15]
    #define SHU3_DQSG_RETRY_R_RETRY_ALE_BLOCK_MASK             Fld(1, 20) //[20:20]
    #define SHU3_DQSG_RETRY_R_RDY_SEL_DLE                      Fld(1, 21) //[21:21]
    #define SHU3_DQSG_RETRY_R_RETRY_ROUND_NUM                  Fld(2, 24) //[25:24]
    #define SHU3_DQSG_RETRY_R_RETRY_RANKSEL_FROM_PHY           Fld(1, 28) //[28:28]
    #define SHU3_DQSG_RETRY_R_RETRY_PA_DSIABLE                 Fld(1, 29) //[29:29]
    #define SHU3_DQSG_RETRY_R_RETRY_STBEN_RESET_MSK            Fld(1, 30) //[30:30]
    #define SHU3_DQSG_RETRY_R_RETRY_USE_BURST_MDOE             Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU4_ACTIM0                              (DRAMC_AO_BASE_ADDRESS + 0x01A00)
    #define SHU4_ACTIM0_TWTR                                   Fld(4, 0) //[3:0]
    #define SHU4_ACTIM0_TWR                                    Fld(5, 8) //[12:8]
    #define SHU4_ACTIM0_TRRD                                   Fld(3, 16) //[18:16]
    #define SHU4_ACTIM0_TRCD                                   Fld(4, 24) //[27:24]

#define DRAMC_REG_SHU4_ACTIM1                              (DRAMC_AO_BASE_ADDRESS + 0x01A04)
    #define SHU4_ACTIM1_TRPAB                                  Fld(3, 0) //[2:0]
    #define SHU4_ACTIM1_TRP                                    Fld(4, 8) //[11:8]
    #define SHU4_ACTIM1_TRAS                                   Fld(4, 16) //[19:16]
    #define SHU4_ACTIM1_TRC                                    Fld(5, 24) //[28:24]

#define DRAMC_REG_SHU4_ACTIM2                              (DRAMC_AO_BASE_ADDRESS + 0x01A08)
    #define SHU4_ACTIM2_TXP                                    Fld(3, 0) //[2:0]
    #define SHU4_ACTIM2_TRTP                                   Fld(3, 8) //[10:8]
    #define SHU4_ACTIM2_TR2W                                   Fld(4, 16) //[19:16]
    #define SHU4_ACTIM2_TFAW                                   Fld(5, 24) //[28:24]

#define DRAMC_REG_SHU4_ACTIM3                              (DRAMC_AO_BASE_ADDRESS + 0x01A0C)
    #define SHU4_ACTIM3_TRFCPB                                 Fld(8, 0) //[7:0]
    #define SHU4_ACTIM3_TRFC                                   Fld(8, 16) //[23:16]
    #define SHU4_ACTIM3_REFCNT                                 Fld(8, 24) //[31:24]

#define DRAMC_REG_SHU4_ACTIM4                              (DRAMC_AO_BASE_ADDRESS + 0x01A10)
    #define SHU4_ACTIM4_TXREFCNT                               Fld(10, 0) //[9:0]
    #define SHU4_ACTIM4_REFCNT_FR_CLK                          Fld(8, 16) //[23:16]
    #define SHU4_ACTIM4_TZQCS                                  Fld(8, 24) //[31:24]

#define DRAMC_REG_SHU4_ACTIM5                              (DRAMC_AO_BASE_ADDRESS + 0x01A14)
    #define SHU4_ACTIM5_TR2PD                                  Fld(5, 0) //[4:0]
    #define SHU4_ACTIM5_TWTPD                                  Fld(5, 8) //[12:8]
    #define SHU4_ACTIM5_TMRR2W                                 Fld(4, 24) //[27:24]

#define DRAMC_REG_SHU4_ACTIM6                              (DRAMC_AO_BASE_ADDRESS + 0x01A18)
    #define SHU4_ACTIM6_BGTCCD                                 Fld(2, 0) //[1:0]
    #define SHU4_ACTIM6_BGTWTR                                 Fld(4, 4) //[7:4]
    #define SHU4_ACTIM6_TWRMPR                                 Fld(4, 8) //[11:8]
    #define SHU4_ACTIM6_BGTRRD                                 Fld(3, 12) //[14:12]

#define DRAMC_REG_SHU4_ACTIM_XRT                           (DRAMC_AO_BASE_ADDRESS + 0x01A1C)
    #define SHU4_ACTIM_XRT_XRTR2R                              Fld(5, 0) //[4:0]
    #define SHU4_ACTIM_XRT_XRTR2W                              Fld(4, 8) //[11:8]
    #define SHU4_ACTIM_XRT_XRTW2R                              Fld(3, 16) //[18:16]
    #define SHU4_ACTIM_XRT_XRTW2W                              Fld(4, 24) //[27:24]

#define DRAMC_REG_SHU4_AC_TIME_05T                         (DRAMC_AO_BASE_ADDRESS + 0x01A20)
    #define SHU4_AC_TIME_05T_TRC_05T                           Fld(1, 0) //[0:0]
    #define SHU4_AC_TIME_05T_TRFCPB_05T                        Fld(1, 1) //[1:1]
    #define SHU4_AC_TIME_05T_TRFC_05T                          Fld(1, 2) //[2:2]
    #define SHU4_AC_TIME_05T_TXP_05T                           Fld(1, 4) //[4:4]
    #define SHU4_AC_TIME_05T_TRTP_05T                          Fld(1, 5) //[5:5]
    #define SHU4_AC_TIME_05T_TRCD_05T                          Fld(1, 6) //[6:6]
    #define SHU4_AC_TIME_05T_TRP_05T                           Fld(1, 7) //[7:7]
    #define SHU4_AC_TIME_05T_TRPAB_05T                         Fld(1, 8) //[8:8]
    #define SHU4_AC_TIME_05T_TRAS_05T                          Fld(1, 9) //[9:9]
    #define SHU4_AC_TIME_05T_TWR_M05T                          Fld(1, 10) //[10:10]
    #define SHU4_AC_TIME_05T_TRRD_05T                          Fld(1, 12) //[12:12]
    #define SHU4_AC_TIME_05T_TFAW_05T                          Fld(1, 13) //[13:13]
    #define SHU4_AC_TIME_05T_TR2PD_05T                         Fld(1, 15) //[15:15]
    #define SHU4_AC_TIME_05T_TWTPD_M05T                        Fld(1, 16) //[16:16]
    #define SHU4_AC_TIME_05T_BGTRRD_05T                        Fld(1, 21) //[21:21]
    #define SHU4_AC_TIME_05T_BGTCCD_05T                        Fld(1, 22) //[22:22]
    #define SHU4_AC_TIME_05T_BGTWTR_05T                        Fld(1, 23) //[23:23]
    #define SHU4_AC_TIME_05T_TR2W_05T                          Fld(1, 24) //[24:24]
    #define SHU4_AC_TIME_05T_TWTR_M05T                         Fld(1, 25) //[25:25]
    #define SHU4_AC_TIME_05T_XRTR2W_05T                        Fld(1, 26) //[26:26]
    #define SHU4_AC_TIME_05T_XRTW2R_M05T                       Fld(1, 27) //[27:27]

#define DRAMC_REG_SHU4_AC_DERATING0                        (DRAMC_AO_BASE_ADDRESS + 0x01A24)
    #define SHU4_AC_DERATING0_ACDERATEEN                       Fld(1, 0) //[0:0]
    #define SHU4_AC_DERATING0_TRRD_DERATE                      Fld(3, 16) //[18:16]
    #define SHU4_AC_DERATING0_TRCD_DERATE                      Fld(4, 24) //[27:24]

#define DRAMC_REG_SHU4_AC_DERATING1                        (DRAMC_AO_BASE_ADDRESS + 0x01A28)
    #define SHU4_AC_DERATING1_TRPAB_DERATE                     Fld(3, 0) //[2:0]
    #define SHU4_AC_DERATING1_TRP_DERATE                       Fld(4, 8) //[11:8]
    #define SHU4_AC_DERATING1_TRAS_DERATE                      Fld(4, 16) //[19:16]
    #define SHU4_AC_DERATING1_TRC_DERATE                       Fld(5, 24) //[28:24]

#define DRAMC_REG_SHU4_AC_DERATING_05T                     (DRAMC_AO_BASE_ADDRESS + 0x01A30)
    #define SHU4_AC_DERATING_05T_TRC_05T_DERATE                Fld(1, 0) //[0:0]
    #define SHU4_AC_DERATING_05T_TRCD_05T_DERATE               Fld(1, 6) //[6:6]
    #define SHU4_AC_DERATING_05T_TRP_05T_DERATE                Fld(1, 7) //[7:7]
    #define SHU4_AC_DERATING_05T_TRPAB_05T_DERATE              Fld(1, 8) //[8:8]
    #define SHU4_AC_DERATING_05T_TRAS_05T_DERATE               Fld(1, 9) //[9:9]
    #define SHU4_AC_DERATING_05T_TRRD_05T_DERATE               Fld(1, 12) //[12:12]

#define DRAMC_REG_SHU4_CONF0                               (DRAMC_AO_BASE_ADDRESS + 0x01A40)
    #define SHU4_CONF0_DMPGTIM                                 Fld(6, 0) //[5:0]
    #define SHU4_CONF0_ADVREFEN                                Fld(1, 6) //[6:6]
    #define SHU4_CONF0_ADVPREEN                                Fld(1, 7) //[7:7]
    #define SHU4_CONF0_TRFCPBIG                                Fld(1, 9) //[9:9]
    #define SHU4_CONF0_REFTHD                                  Fld(4, 12) //[15:12]
    #define SHU4_CONF0_REQQUE_DEPTH                            Fld(4, 16) //[19:16]
    #define SHU4_CONF0_FREQDIV4                                Fld(1, 24) //[24:24]
    #define SHU4_CONF0_FDIV2                                   Fld(1, 25) //[25:25]
    #define SHU4_CONF0_CL2                                     Fld(1, 27) //[27:27]
    #define SHU4_CONF0_BL2                                     Fld(1, 28) //[28:28]
    #define SHU4_CONF0_BL4                                     Fld(1, 29) //[29:29]
    #define SHU4_CONF0_MATYPE                                  Fld(2, 30) //[31:30]

#define DRAMC_REG_SHU4_CONF1                               (DRAMC_AO_BASE_ADDRESS + 0x01A44)
    #define SHU4_CONF1_DATLAT                                  Fld(5, 0) //[4:0]
    #define SHU4_CONF1_DATLAT_DSEL                             Fld(5, 8) //[12:8]
    #define SHU4_CONF1_REFBW_FR                                Fld(10, 16) //[25:16]
    #define SHU4_CONF1_DATLAT_DSEL_PHY                         Fld(5, 26) //[30:26]
    #define SHU4_CONF1_TREFBWIG                                Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU4_CONF2                               (DRAMC_AO_BASE_ADDRESS + 0x01A48)
    #define SHU4_CONF2_TCMDO1LAT                               Fld(8, 0) //[7:0]
    #define SHU4_CONF2_FSPCHG_PRDCNT                           Fld(8, 8) //[15:8]
    #define SHU4_CONF2_DCMDLYREF                               Fld(3, 16) //[18:16]
    #define SHU4_CONF2_DQCMD                                   Fld(1, 25) //[25:25]
    #define SHU4_CONF2_DQ16COM1                                Fld(1, 26) //[26:26]
    #define SHU4_CONF2_RA15TOCS1                               Fld(1, 27) //[27:27]
    #define SHU4_CONF2_WPRE2T                                  Fld(1, 28) //[28:28]
    #define SHU4_CONF2_FASTWAKE2                               Fld(1, 29) //[29:29]
    #define SHU4_CONF2_DAREFEN                                 Fld(1, 30) //[30:30]
    #define SHU4_CONF2_FASTWAKE                                Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU4_CONF3                               (DRAMC_AO_BASE_ADDRESS + 0x01A4C)
    #define SHU4_CONF3_ZQCSCNT                                 Fld(16, 0) //[15:0]
    #define SHU4_CONF3_REFRCNT                                 Fld(9, 16) //[24:16]

#define DRAMC_REG_SHU4_STBCAL                              (DRAMC_AO_BASE_ADDRESS + 0x01A50)
    #define SHU4_STBCAL_DMSTBLAT                               Fld(2, 0) //[1:0]
    #define SHU4_STBCAL_PICGLAT                                Fld(3, 4) //[6:4]
    #define SHU4_STBCAL_DQSG_MODE                              Fld(1, 8) //[8:8]

#define DRAMC_REG_SHU4_DQSOSCTHRD                          (DRAMC_AO_BASE_ADDRESS + 0x01A54)
    #define SHU4_DQSOSCTHRD_DQSOSCTHRD_INC_RK0                 Fld(12, 0) //[11:0]
    #define SHU4_DQSOSCTHRD_DQSOSCTHRD_DEC_RK0                 Fld(12, 12) //[23:12]
    #define SHU4_DQSOSCTHRD_DQSOSCTHRD_INC_RK1_7TO0            Fld(8, 24) //[31:24]

#define DRAMC_REG_SHU4_RANKCTL                             (DRAMC_AO_BASE_ADDRESS + 0x01A58)
    #define SHU4_RANKCTL_RANKINCTL_RXDLY                       Fld(4, 0) //[3:0]
    #define SHU4_RANKCTL_TXRANKINCTL_TXDLY                     Fld(4, 8) //[11:8]
    #define SHU4_RANKCTL_TXRANKINCTL                           Fld(4, 12) //[15:12]
    #define SHU4_RANKCTL_TXRANKINCTL_ROOT                      Fld(4, 16) //[19:16]
    #define SHU4_RANKCTL_RANKINCTL                             Fld(4, 20) //[23:20]
    #define SHU4_RANKCTL_RANKINCTL_ROOT1                       Fld(4, 24) //[27:24]
    #define SHU4_RANKCTL_RANKINCTL_PHY                         Fld(4, 28) //[31:28]

#define DRAMC_REG_SHU4_CKECTRL                             (DRAMC_AO_BASE_ADDRESS + 0x01A5C)
    #define SHU4_CKECTRL_CMDCKE                                Fld(3, 16) //[18:16]
    #define SHU4_CKECTRL_CKEPRD                                Fld(3, 20) //[22:20]
    #define SHU4_CKECTRL_TCKESRX                               Fld(2, 24) //[25:24]
    #define SHU4_CKECTRL_SREF_CK_DLY                           Fld(2, 28) //[29:28]

#define DRAMC_REG_SHU4_ODTCTRL                             (DRAMC_AO_BASE_ADDRESS + 0x01A60)
    #define SHU4_ODTCTRL_ROEN                                  Fld(1, 0) //[0:0]
    #define SHU4_ODTCTRL_WOEN                                  Fld(1, 1) //[1:1]
    #define SHU4_ODTCTRL_RODTEN_SELPH_CG_IG                    Fld(1, 2) //[2:2]
    #define SHU4_ODTCTRL_RODTENSTB_SELPH_CG_IG                 Fld(1, 3) //[3:3]
    #define SHU4_ODTCTRL_RODT                                  Fld(4, 4) //[7:4]
    #define SHU4_ODTCTRL_TWODT                                 Fld(7, 16) //[22:16]
    #define SHU4_ODTCTRL_FIXRODT                               Fld(1, 27) //[27:27]
    #define SHU4_ODTCTRL_RODTE2                                Fld(1, 30) //[30:30]
    #define SHU4_ODTCTRL_RODTE                                 Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU4_IMPCAL1                             (DRAMC_AO_BASE_ADDRESS + 0x01A64)
    #define SHU4_IMPCAL1_IMPCAL_CHKCYCLE                       Fld(3, 0) //[2:0]
    #define SHU4_IMPCAL1_IMPDRVP                               Fld(5, 4) //[8:4]
    #define SHU4_IMPCAL1_IMPDRVN                               Fld(5, 11) //[15:11]
    #define SHU4_IMPCAL1_IMPCAL_CALEN_CYCLE                    Fld(3, 17) //[19:17]
    #define SHU4_IMPCAL1_IMPCALCNT                             Fld(8, 20) //[27:20]
    #define SHU4_IMPCAL1_IMPCAL_CALICNT                        Fld(4, 28) //[31:28]

#define DRAMC_REG_SHU4_DQSOSC_PRD                          (DRAMC_AO_BASE_ADDRESS + 0x01A68)
    #define SHU4_DQSOSC_PRD_DQSOSC_PRDCNT                      Fld(10, 0) //[9:0]
    #define SHU4_DQSOSC_PRD_DQSOSCTHRD_INC_RK1_11TO8           Fld(4, 16) //[19:16]
    #define SHU4_DQSOSC_PRD_DQSOSCTHRD_DEC_RK1                 Fld(12, 20) //[31:20]

#define DRAMC_REG_SHU4_DQSOSCR                             (DRAMC_AO_BASE_ADDRESS + 0x01A6C)
    #define SHU4_DQSOSCR_DQSOSCRCNT                            Fld(8, 0) //[7:0]
    #define SHU4_DQSOSCR_DQSOSC_DELTA                          Fld(16, 16) //[31:16]

#define DRAMC_REG_SHU4_DQSOSCR2                            (DRAMC_AO_BASE_ADDRESS + 0x01A70)
    #define SHU4_DQSOSCR2_DQSOSCENCNT                          Fld(16, 0) //[15:0]
    #define SHU4_DQSOSCR2_DQSOSC_ADV_SEL                       Fld(2, 16) //[17:16]
    #define SHU4_DQSOSCR2_DQSOSC_DRS_ADV_SEL                   Fld(2, 18) //[19:18]

#define DRAMC_REG_SHU4_RODTENSTB                           (DRAMC_AO_BASE_ADDRESS + 0x01A74)
    #define SHU4_RODTENSTB_RODTEN_MCK_MODESEL                  Fld(1, 0) //[0:0]
    #define SHU4_RODTENSTB_RODTEN_P1_ENABLE                    Fld(1, 1) //[1:1]
    #define SHU4_RODTENSTB_RODTENSTB_OFFSET                    Fld(6, 2) //[7:2]
    #define SHU4_RODTENSTB_RODTENSTB_EXT                       Fld(16, 8) //[23:8]
    #define SHU4_RODTENSTB_RODTENSTB_4BYTE_EN                  Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU4_PIPE                                (DRAMC_AO_BASE_ADDRESS + 0x01A78)
    #define SHU4_PIPE_PHYRXPIPE1                               Fld(1, 0) //[0:0]
    #define SHU4_PIPE_PHYRXPIPE2                               Fld(1, 1) //[1:1]
    #define SHU4_PIPE_PHYRXPIPE3                               Fld(1, 2) //[2:2]
    #define SHU4_PIPE_PHYRXRDSLPIPE1                           Fld(1, 4) //[4:4]
    #define SHU4_PIPE_PHYRXRDSLPIPE2                           Fld(1, 5) //[5:5]
    #define SHU4_PIPE_PHYRXRDSLPIPE3                           Fld(1, 6) //[6:6]
    #define SHU4_PIPE_PHYPIPE1EN                               Fld(1, 8) //[8:8]
    #define SHU4_PIPE_PHYPIPE2EN                               Fld(1, 9) //[9:9]
    #define SHU4_PIPE_PHYPIPE3EN                               Fld(1, 10) //[10:10]
    #define SHU4_PIPE_DLE_LAST_EXTEND3                         Fld(1, 26) //[26:26]
    #define SHU4_PIPE_READ_START_EXTEND3                       Fld(1, 27) //[27:27]
    #define SHU4_PIPE_DLE_LAST_EXTEND2                         Fld(1, 28) //[28:28]
    #define SHU4_PIPE_READ_START_EXTEND2                       Fld(1, 29) //[29:29]
    #define SHU4_PIPE_DLE_LAST_EXTEND1                         Fld(1, 30) //[30:30]
    #define SHU4_PIPE_READ_START_EXTEND1                       Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU4_TEST1                               (DRAMC_AO_BASE_ADDRESS + 0x01A7C)
    #define SHU4_TEST1_LATNORMPOP                              Fld(5, 8) //[12:8]
    #define SHU4_TEST1_DQSICALBLCOK_CNT                        Fld(3, 20) //[22:20]
    #define SHU4_TEST1_DQSICALI_NEW                            Fld(1, 23) //[23:23]

#define DRAMC_REG_SHU4_SELPH_CA1                           (DRAMC_AO_BASE_ADDRESS + 0x01A80)
    #define SHU4_SELPH_CA1_TXDLY_CS                            Fld(3, 0) //[2:0]
    #define SHU4_SELPH_CA1_TXDLY_CKE                           Fld(3, 4) //[6:4]
    #define SHU4_SELPH_CA1_TXDLY_ODT                           Fld(3, 8) //[10:8]
    #define SHU4_SELPH_CA1_TXDLY_RESET                         Fld(3, 12) //[14:12]
    #define SHU4_SELPH_CA1_TXDLY_WE                            Fld(3, 16) //[18:16]
    #define SHU4_SELPH_CA1_TXDLY_CAS                           Fld(3, 20) //[22:20]
    #define SHU4_SELPH_CA1_TXDLY_RAS                           Fld(3, 24) //[26:24]
    #define SHU4_SELPH_CA1_TXDLY_CS1                           Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4_SELPH_CA2                           (DRAMC_AO_BASE_ADDRESS + 0x01A84)
    #define SHU4_SELPH_CA2_TXDLY_BA0                           Fld(3, 0) //[2:0]
    #define SHU4_SELPH_CA2_TXDLY_BA1                           Fld(3, 4) //[6:4]
    #define SHU4_SELPH_CA2_TXDLY_BA2                           Fld(3, 8) //[10:8]
    #define SHU4_SELPH_CA2_TXDLY_CMD                           Fld(5, 16) //[20:16]
    #define SHU4_SELPH_CA2_TXDLY_CKE1                          Fld(3, 24) //[26:24]

#define DRAMC_REG_SHU4_SELPH_CA3                           (DRAMC_AO_BASE_ADDRESS + 0x01A88)
    #define SHU4_SELPH_CA3_TXDLY_RA0                           Fld(3, 0) //[2:0]
    #define SHU4_SELPH_CA3_TXDLY_RA1                           Fld(3, 4) //[6:4]
    #define SHU4_SELPH_CA3_TXDLY_RA2                           Fld(3, 8) //[10:8]
    #define SHU4_SELPH_CA3_TXDLY_RA3                           Fld(3, 12) //[14:12]
    #define SHU4_SELPH_CA3_TXDLY_RA4                           Fld(3, 16) //[18:16]
    #define SHU4_SELPH_CA3_TXDLY_RA5                           Fld(3, 20) //[22:20]
    #define SHU4_SELPH_CA3_TXDLY_RA6                           Fld(3, 24) //[26:24]
    #define SHU4_SELPH_CA3_TXDLY_RA7                           Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4_SELPH_CA4                           (DRAMC_AO_BASE_ADDRESS + 0x01A8C)
    #define SHU4_SELPH_CA4_TXDLY_RA8                           Fld(3, 0) //[2:0]
    #define SHU4_SELPH_CA4_TXDLY_RA9                           Fld(3, 4) //[6:4]
    #define SHU4_SELPH_CA4_TXDLY_RA10                          Fld(3, 8) //[10:8]
    #define SHU4_SELPH_CA4_TXDLY_RA11                          Fld(3, 12) //[14:12]
    #define SHU4_SELPH_CA4_TXDLY_RA12                          Fld(3, 16) //[18:16]
    #define SHU4_SELPH_CA4_TXDLY_RA13                          Fld(3, 20) //[22:20]
    #define SHU4_SELPH_CA4_TXDLY_RA14                          Fld(3, 24) //[26:24]
    #define SHU4_SELPH_CA4_TXDLY_RA15                          Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4_SELPH_CA5                           (DRAMC_AO_BASE_ADDRESS + 0x01A90)
    #define SHU4_SELPH_CA5_DLY_CS                              Fld(3, 0) //[2:0]
    #define SHU4_SELPH_CA5_DLY_CKE                             Fld(3, 4) //[6:4]
    #define SHU4_SELPH_CA5_DLY_ODT                             Fld(3, 8) //[10:8]
    #define SHU4_SELPH_CA5_DLY_RESET                           Fld(3, 12) //[14:12]
    #define SHU4_SELPH_CA5_DLY_WE                              Fld(3, 16) //[18:16]
    #define SHU4_SELPH_CA5_DLY_CAS                             Fld(3, 20) //[22:20]
    #define SHU4_SELPH_CA5_DLY_RAS                             Fld(3, 24) //[26:24]
    #define SHU4_SELPH_CA5_DLY_CS1                             Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4_SELPH_CA6                           (DRAMC_AO_BASE_ADDRESS + 0x01A94)
    #define SHU4_SELPH_CA6_DLY_BA0                             Fld(3, 0) //[2:0]
    #define SHU4_SELPH_CA6_DLY_BA1                             Fld(3, 4) //[6:4]
    #define SHU4_SELPH_CA6_DLY_BA2                             Fld(3, 8) //[10:8]
    #define SHU4_SELPH_CA6_DLY_CKE1                            Fld(3, 24) //[26:24]

#define DRAMC_REG_SHU4_SELPH_CA7                           (DRAMC_AO_BASE_ADDRESS + 0x01A98)
    #define SHU4_SELPH_CA7_DLY_RA0                             Fld(3, 0) //[2:0]
    #define SHU4_SELPH_CA7_DLY_RA1                             Fld(3, 4) //[6:4]
    #define SHU4_SELPH_CA7_DLY_RA2                             Fld(3, 8) //[10:8]
    #define SHU4_SELPH_CA7_DLY_RA3                             Fld(3, 12) //[14:12]
    #define SHU4_SELPH_CA7_DLY_RA4                             Fld(3, 16) //[18:16]
    #define SHU4_SELPH_CA7_DLY_RA5                             Fld(3, 20) //[22:20]
    #define SHU4_SELPH_CA7_DLY_RA6                             Fld(3, 24) //[26:24]
    #define SHU4_SELPH_CA7_DLY_RA7                             Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4_SELPH_CA8                           (DRAMC_AO_BASE_ADDRESS + 0x01A9C)
    #define SHU4_SELPH_CA8_DLY_RA8                             Fld(3, 0) //[2:0]
    #define SHU4_SELPH_CA8_DLY_RA9                             Fld(3, 4) //[6:4]
    #define SHU4_SELPH_CA8_DLY_RA10                            Fld(3, 8) //[10:8]
    #define SHU4_SELPH_CA8_DLY_RA11                            Fld(3, 12) //[14:12]
    #define SHU4_SELPH_CA8_DLY_RA12                            Fld(3, 16) //[18:16]
    #define SHU4_SELPH_CA8_DLY_RA13                            Fld(3, 20) //[22:20]
    #define SHU4_SELPH_CA8_DLY_RA14                            Fld(3, 24) //[26:24]
    #define SHU4_SELPH_CA8_DLY_RA15                            Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4_SELPH_DQS0                          (DRAMC_AO_BASE_ADDRESS + 0x01AA0)
    #define SHU4_SELPH_DQS0_TXDLY_DQS0                         Fld(3, 0) //[2:0]
    #define SHU4_SELPH_DQS0_TXDLY_DQS1                         Fld(3, 4) //[6:4]
    #define SHU4_SELPH_DQS0_TXDLY_DQS2                         Fld(3, 8) //[10:8]
    #define SHU4_SELPH_DQS0_TXDLY_DQS3                         Fld(3, 12) //[14:12]
    #define SHU4_SELPH_DQS0_TXDLY_OEN_DQS0                     Fld(3, 16) //[18:16]
    #define SHU4_SELPH_DQS0_TXDLY_OEN_DQS1                     Fld(3, 20) //[22:20]
    #define SHU4_SELPH_DQS0_TXDLY_OEN_DQS2                     Fld(3, 24) //[26:24]
    #define SHU4_SELPH_DQS0_TXDLY_OEN_DQS3                     Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4_SELPH_DQS1                          (DRAMC_AO_BASE_ADDRESS + 0x01AA4)
    #define SHU4_SELPH_DQS1_DLY_DQS0                           Fld(3, 0) //[2:0]
    #define SHU4_SELPH_DQS1_DLY_DQS1                           Fld(3, 4) //[6:4]
    #define SHU4_SELPH_DQS1_DLY_DQS2                           Fld(3, 8) //[10:8]
    #define SHU4_SELPH_DQS1_DLY_DQS3                           Fld(3, 12) //[14:12]
    #define SHU4_SELPH_DQS1_DLY_OEN_DQS0                       Fld(3, 16) //[18:16]
    #define SHU4_SELPH_DQS1_DLY_OEN_DQS1                       Fld(3, 20) //[22:20]
    #define SHU4_SELPH_DQS1_DLY_OEN_DQS2                       Fld(3, 24) //[26:24]
    #define SHU4_SELPH_DQS1_DLY_OEN_DQS3                       Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4_DRVING1                             (DRAMC_AO_BASE_ADDRESS + 0x01AA8)
    #define SHU4_DRVING1_DQDRVN2                               Fld(5, 0) //[4:0]
    #define SHU4_DRVING1_DQDRVP2                               Fld(5, 5) //[9:5]
    #define SHU4_DRVING1_DQSDRVN1                              Fld(5, 10) //[14:10]
    #define SHU4_DRVING1_DQSDRVP1                              Fld(5, 15) //[19:15]
    #define SHU4_DRVING1_DQSDRVN2                              Fld(5, 20) //[24:20]
    #define SHU4_DRVING1_DQSDRVP2                              Fld(5, 25) //[29:25]
    #define SHU4_DRVING1_DIS_IMP_ODTN_TRACK                    Fld(1, 30) //[30:30]
    #define SHU4_DRVING1_DIS_IMPCAL_HW                         Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU4_DRVING2                             (DRAMC_AO_BASE_ADDRESS + 0x01AAC)
    #define SHU4_DRVING2_CMDDRVN1                              Fld(5, 0) //[4:0]
    #define SHU4_DRVING2_CMDDRVP1                              Fld(5, 5) //[9:5]
    #define SHU4_DRVING2_CMDDRVN2                              Fld(5, 10) //[14:10]
    #define SHU4_DRVING2_CMDDRVP2                              Fld(5, 15) //[19:15]
    #define SHU4_DRVING2_DQDRVN1                               Fld(5, 20) //[24:20]
    #define SHU4_DRVING2_DQDRVP1                               Fld(5, 25) //[29:25]
    #define SHU4_DRVING2_DIS_IMPCAL_ODT_EN                     Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU4_DRVING3                             (DRAMC_AO_BASE_ADDRESS + 0x01AB0)
    #define SHU4_DRVING3_DQODTN2                               Fld(5, 0) //[4:0]
    #define SHU4_DRVING3_DQODTP2                               Fld(5, 5) //[9:5]
    #define SHU4_DRVING3_DQSODTN                               Fld(5, 10) //[14:10]
    #define SHU4_DRVING3_DQSODTP                               Fld(5, 15) //[19:15]
    #define SHU4_DRVING3_DQSODTN2                              Fld(5, 20) //[24:20]
    #define SHU4_DRVING3_DQSODTP2                              Fld(5, 25) //[29:25]

#define DRAMC_REG_SHU4_DRVING4                             (DRAMC_AO_BASE_ADDRESS + 0x01AB4)
    #define SHU4_DRVING4_CMDODTN1                              Fld(5, 0) //[4:0]
    #define SHU4_DRVING4_CMDODTP1                              Fld(5, 5) //[9:5]
    #define SHU4_DRVING4_CMDODTN2                              Fld(5, 10) //[14:10]
    #define SHU4_DRVING4_CMDODTP2                              Fld(5, 15) //[19:15]
    #define SHU4_DRVING4_DQODTN1                               Fld(5, 20) //[24:20]
    #define SHU4_DRVING4_DQODTP1                               Fld(5, 25) //[29:25]

#define DRAMC_REG_SHU4_DRVING5                             (DRAMC_AO_BASE_ADDRESS + 0x01AB8)
    #define SHU4_DRVING5_DQCODTN2                              Fld(5, 0) //[4:0]
    #define SHU4_DRVING5_DQCODTP2                              Fld(5, 5) //[9:5]
    #define SHU4_DRVING5_DQCDRVN1                              Fld(5, 10) //[14:10]
    #define SHU4_DRVING5_DQCDRVP1                              Fld(5, 15) //[19:15]
    #define SHU4_DRVING5_DQCDRVN2                              Fld(5, 20) //[24:20]
    #define SHU4_DRVING5_DQCDRVP2                              Fld(5, 25) //[29:25]

#define DRAMC_REG_SHU4_DRVING6                             (DRAMC_AO_BASE_ADDRESS + 0x01ABC)
    #define SHU4_DRVING6_DQCODTN1                              Fld(5, 20) //[24:20]
    #define SHU4_DRVING6_DQCODTP1                              Fld(5, 25) //[29:25]

#define DRAMC_REG_SHU4_WODT                                (DRAMC_AO_BASE_ADDRESS + 0x01AC0)
    #define SHU4_WODT_DISWODT                                  Fld(3, 0) //[2:0]
    #define SHU4_WODT_WODTFIX                                  Fld(1, 3) //[3:3]
    #define SHU4_WODT_WODTFIXOFF                               Fld(1, 4) //[4:4]
    #define SHU4_WODT_DISWODTE                                 Fld(1, 5) //[5:5]
    #define SHU4_WODT_DISWODTE2                                Fld(1, 6) //[6:6]
    #define SHU4_WODT_WODTPDEN                                 Fld(1, 7) //[7:7]
    #define SHU4_WODT_DQOE_CNT                                 Fld(3, 8) //[10:8]
    #define SHU4_WODT_DQOE_OPT                                 Fld(1, 11) //[11:11]
    #define SHU4_WODT_TXUPD_SEL                                Fld(2, 12) //[13:12]
    #define SHU4_WODT_TXUPD_W2R_SEL                            Fld(3, 14) //[16:14]
    #define SHU4_WODT_DBIWR                                    Fld(1, 29) //[29:29]
    #define SHU4_WODT_TWPSTEXT                                 Fld(1, 30) //[30:30]
    #define SHU4_WODT_WPST2T                                   Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU4_DQSG                                (DRAMC_AO_BASE_ADDRESS + 0x01AC4)
    #define SHU4_DQSG_DLLFRZRFCOPT                             Fld(2, 0) //[1:0]
    #define SHU4_DQSG_DLLFRZWROPT                              Fld(2, 4) //[5:4]
    #define SHU4_DQSG_R_RSTBCNT_LATCH_OPT                      Fld(3, 8) //[10:8]
    #define SHU4_DQSG_STB_UPDMASK_EN                           Fld(1, 11) //[11:11]
    #define SHU4_DQSG_STB_UPDMASKCYC                           Fld(4, 12) //[15:12]
    #define SHU4_DQSG_DQSINCTL_PRE_SEL                         Fld(1, 16) //[16:16]
    #define SHU4_DQSG_SCINTV                                   Fld(6, 20) //[25:20]

#define DRAMC_REG_SHU4_SCINTV                              (DRAMC_AO_BASE_ADDRESS + 0x01AC8)
    #define SHU4_SCINTV_ODTREN                                 Fld(1, 0) //[0:0]
    #define SHU4_SCINTV_TZQLAT                                 Fld(5, 1) //[5:1]
    #define SHU4_SCINTV_TZQLAT2                                Fld(5, 6) //[10:6]
    #define SHU4_SCINTV_RDDQC_INTV                             Fld(2, 11) //[12:11]
    #define SHU4_SCINTV_MRW_INTV                               Fld(5, 13) //[17:13]
    #define SHU4_SCINTV_DQS2DQ_SHU_PITHRD                      Fld(6, 18) //[23:18]
    #define SHU4_SCINTV_DQS2DQ_FILT_PITHRD                     Fld(6, 24) //[29:24]
    #define SHU4_SCINTV_DQSOSCENDIS                            Fld(1, 30) //[30:30]

#define DRAMC_REG_SHU4_MISC                                (DRAMC_AO_BASE_ADDRESS + 0x01ACC)
    #define SHU4_MISC_REQQUE_MAXCNT                            Fld(4, 0) //[3:0]
    #define SHU4_MISC_CKEHCMD                                  Fld(2, 4) //[5:4]
    #define SHU4_MISC_NORMPOP_LEN                              Fld(3, 8) //[10:8]
    #define SHU4_MISC_PREA_INTV                                Fld(5, 12) //[16:12]

#define DRAMC_REG_SHU4_DQS2DQ_TX                           (DRAMC_AO_BASE_ADDRESS + 0x01AD0)
    #define SHU4_DQS2DQ_TX_OE2DQ_OFFSET                        Fld(5, 0) //[4:0]

#define DRAMC_REG_SHU4_HWSET_MR2                           (DRAMC_AO_BASE_ADDRESS + 0x01AD4)
    #define SHU4_HWSET_MR2_HWSET_MR2_MRSMA                     Fld(13, 0) //[12:0]
    #define SHU4_HWSET_MR2_HWSET_MR2_OP                        Fld(8, 16) //[23:16]

#define DRAMC_REG_SHU4_HWSET_MR13                          (DRAMC_AO_BASE_ADDRESS + 0x01AD8)
    #define SHU4_HWSET_MR13_HWSET_MR13_MRSMA                   Fld(13, 0) //[12:0]
    #define SHU4_HWSET_MR13_HWSET_MR13_OP                      Fld(8, 16) //[23:16]

#define DRAMC_REG_SHU4_HWSET_VRCG                          (DRAMC_AO_BASE_ADDRESS + 0x01ADC)
    #define SHU4_HWSET_VRCG_HWSET_VRCG_MRSMA                   Fld(13, 0) //[12:0]
    #define SHU4_HWSET_VRCG_HWSET_VRCG_OP                      Fld(8, 16) //[23:16]

#define DRAMC_REG_SHU4_APHY_TX_PICG_CTRL                   (DRAMC_AO_BASE_ADDRESS + 0x01AE4)
    #define SHU4_APHY_TX_PICG_CTRL_APHYPI_CG_CK_SEL            Fld(4, 20) //[23:20]
    #define SHU4_APHY_TX_PICG_CTRL_APHYPI_CG_CK_OPT            Fld(1, 24) //[24:24]
    #define SHU4_APHY_TX_PICG_CTRL_DDRPHY_CLK_DYN_GATING_SEL   Fld(4, 27) //[30:27]
    #define SHU4_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_OPT           Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU4RK0_DQSCTL                           (DRAMC_AO_BASE_ADDRESS + 0x01C00)
    #define SHU4RK0_DQSCTL_DQSINCTL                            Fld(4, 0) //[3:0]

#define DRAMC_REG_SHU4RK0_DQSIEN                           (DRAMC_AO_BASE_ADDRESS + 0x01C04)
    #define SHU4RK0_DQSIEN_R0DQS0IEN                           Fld(7, 0) //[6:0]
    #define SHU4RK0_DQSIEN_R0DQS1IEN                           Fld(7, 8) //[14:8]
    #define SHU4RK0_DQSIEN_R0DQS2IEN                           Fld(7, 16) //[22:16]
    #define SHU4RK0_DQSIEN_R0DQS3IEN                           Fld(7, 24) //[30:24]

#define DRAMC_REG_SHU4RK0_DQSCAL                           (DRAMC_AO_BASE_ADDRESS + 0x01C08)
    #define SHU4RK0_DQSCAL_R0DQSIENLLMT                        Fld(7, 0) //[6:0]
    #define SHU4RK0_DQSCAL_R0DQSIENLLMTEN                      Fld(1, 7) //[7:7]
    #define SHU4RK0_DQSCAL_R0DQSIENHLMT                        Fld(7, 8) //[14:8]
    #define SHU4RK0_DQSCAL_R0DQSIENHLMTEN                      Fld(1, 15) //[15:15]

#define DRAMC_REG_SHU4RK0_PI                               (DRAMC_AO_BASE_ADDRESS + 0x01C0C)
    #define SHU4RK0_PI_RK0_ARPI_DQ_B1                          Fld(6, 0) //[5:0]
    #define SHU4RK0_PI_RK0_ARPI_DQ_B0                          Fld(6, 8) //[13:8]
    #define SHU4RK0_PI_RK0_ARPI_DQM_B1                         Fld(6, 16) //[21:16]
    #define SHU4RK0_PI_RK0_ARPI_DQM_B0                         Fld(6, 24) //[29:24]

#define DRAMC_REG_SHU4RK0_DQSOSC                           (DRAMC_AO_BASE_ADDRESS + 0x01C10)
    #define SHU4RK0_DQSOSC_DQSOSC_BASE_RK0                     Fld(16, 0) //[15:0]
    #define SHU4RK0_DQSOSC_DQSOSC_BASE_RK0_B1                  Fld(16, 16) //[31:16]

#define DRAMC_REG_SHU4RK0_SELPH_ODTEN0                     (DRAMC_AO_BASE_ADDRESS + 0x01C1C)
    #define SHU4RK0_SELPH_ODTEN0_TXDLY_B0_RODTEN               Fld(3, 0) //[2:0]
    #define SHU4RK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1            Fld(3, 4) //[6:4]
    #define SHU4RK0_SELPH_ODTEN0_TXDLY_B1_RODTEN               Fld(3, 8) //[10:8]
    #define SHU4RK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1            Fld(3, 12) //[14:12]
    #define SHU4RK0_SELPH_ODTEN0_TXDLY_B2_RODTEN               Fld(3, 16) //[18:16]
    #define SHU4RK0_SELPH_ODTEN0_TXDLY_B2_RODTEN_P1            Fld(3, 20) //[22:20]
    #define SHU4RK0_SELPH_ODTEN0_TXDLY_B3_RODTEN               Fld(3, 24) //[26:24]
    #define SHU4RK0_SELPH_ODTEN0_TXDLY_B3_RODTEN_P1            Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4RK0_SELPH_ODTEN1                     (DRAMC_AO_BASE_ADDRESS + 0x01C20)
    #define SHU4RK0_SELPH_ODTEN1_DLY_B0_RODTEN                 Fld(3, 0) //[2:0]
    #define SHU4RK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1              Fld(3, 4) //[6:4]
    #define SHU4RK0_SELPH_ODTEN1_DLY_B1_RODTEN                 Fld(3, 8) //[10:8]
    #define SHU4RK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1              Fld(3, 12) //[14:12]
    #define SHU4RK0_SELPH_ODTEN1_DLY_B2_RODTEN                 Fld(3, 16) //[18:16]
    #define SHU4RK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1              Fld(3, 20) //[22:20]
    #define SHU4RK0_SELPH_ODTEN1_DLY_B3_RODTEN                 Fld(3, 24) //[26:24]
    #define SHU4RK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1              Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4RK0_SELPH_DQSG0                      (DRAMC_AO_BASE_ADDRESS + 0x01C24)
    #define SHU4RK0_SELPH_DQSG0_TX_DLY_DQS0_GATED              Fld(3, 0) //[2:0]
    #define SHU4RK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1           Fld(3, 4) //[6:4]
    #define SHU4RK0_SELPH_DQSG0_TX_DLY_DQS1_GATED              Fld(3, 8) //[10:8]
    #define SHU4RK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1           Fld(3, 12) //[14:12]
    #define SHU4RK0_SELPH_DQSG0_TX_DLY_DQS2_GATED              Fld(3, 16) //[18:16]
    #define SHU4RK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1           Fld(3, 20) //[22:20]
    #define SHU4RK0_SELPH_DQSG0_TX_DLY_DQS3_GATED              Fld(3, 24) //[26:24]
    #define SHU4RK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1           Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4RK0_SELPH_DQSG1                      (DRAMC_AO_BASE_ADDRESS + 0x01C28)
    #define SHU4RK0_SELPH_DQSG1_REG_DLY_DQS0_GATED             Fld(3, 0) //[2:0]
    #define SHU4RK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1          Fld(3, 4) //[6:4]
    #define SHU4RK0_SELPH_DQSG1_REG_DLY_DQS1_GATED             Fld(3, 8) //[10:8]
    #define SHU4RK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1          Fld(3, 12) //[14:12]
    #define SHU4RK0_SELPH_DQSG1_REG_DLY_DQS2_GATED             Fld(3, 16) //[18:16]
    #define SHU4RK0_SELPH_DQSG1_REG_DLY_DQS2_GATED_P1          Fld(3, 20) //[22:20]
    #define SHU4RK0_SELPH_DQSG1_REG_DLY_DQS3_GATED             Fld(3, 24) //[26:24]
    #define SHU4RK0_SELPH_DQSG1_REG_DLY_DQS3_GATED_P1          Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4RK0_SELPH_DQ0                        (DRAMC_AO_BASE_ADDRESS + 0x01C2C)
    #define SHU4RK0_SELPH_DQ0_TXDLY_DQ0                        Fld(3, 0) //[2:0]
    #define SHU4RK0_SELPH_DQ0_TXDLY_DQ1                        Fld(3, 4) //[6:4]
    #define SHU4RK0_SELPH_DQ0_TXDLY_DQ2                        Fld(3, 8) //[10:8]
    #define SHU4RK0_SELPH_DQ0_TXDLY_DQ3                        Fld(3, 12) //[14:12]
    #define SHU4RK0_SELPH_DQ0_TXDLY_OEN_DQ0                    Fld(3, 16) //[18:16]
    #define SHU4RK0_SELPH_DQ0_TXDLY_OEN_DQ1                    Fld(3, 20) //[22:20]
    #define SHU4RK0_SELPH_DQ0_TXDLY_OEN_DQ2                    Fld(3, 24) //[26:24]
    #define SHU4RK0_SELPH_DQ0_TXDLY_OEN_DQ3                    Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4RK0_SELPH_DQ1                        (DRAMC_AO_BASE_ADDRESS + 0x01C30)
    #define SHU4RK0_SELPH_DQ1_TXDLY_DQM0                       Fld(3, 0) //[2:0]
    #define SHU4RK0_SELPH_DQ1_TXDLY_DQM1                       Fld(3, 4) //[6:4]
    #define SHU4RK0_SELPH_DQ1_TXDLY_DQM2                       Fld(3, 8) //[10:8]
    #define SHU4RK0_SELPH_DQ1_TXDLY_DQM3                       Fld(3, 12) //[14:12]
    #define SHU4RK0_SELPH_DQ1_TXDLY_OEN_DQM0                   Fld(3, 16) //[18:16]
    #define SHU4RK0_SELPH_DQ1_TXDLY_OEN_DQM1                   Fld(3, 20) //[22:20]
    #define SHU4RK0_SELPH_DQ1_TXDLY_OEN_DQM2                   Fld(3, 24) //[26:24]
    #define SHU4RK0_SELPH_DQ1_TXDLY_OEN_DQM3                   Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4RK0_SELPH_DQ2                        (DRAMC_AO_BASE_ADDRESS + 0x01C34)
    #define SHU4RK0_SELPH_DQ2_DLY_DQ0                          Fld(3, 0) //[2:0]
    #define SHU4RK0_SELPH_DQ2_DLY_DQ1                          Fld(3, 4) //[6:4]
    #define SHU4RK0_SELPH_DQ2_DLY_DQ2                          Fld(3, 8) //[10:8]
    #define SHU4RK0_SELPH_DQ2_DLY_DQ3                          Fld(3, 12) //[14:12]
    #define SHU4RK0_SELPH_DQ2_DLY_OEN_DQ0                      Fld(3, 16) //[18:16]
    #define SHU4RK0_SELPH_DQ2_DLY_OEN_DQ1                      Fld(3, 20) //[22:20]
    #define SHU4RK0_SELPH_DQ2_DLY_OEN_DQ2                      Fld(3, 24) //[26:24]
    #define SHU4RK0_SELPH_DQ2_DLY_OEN_DQ3                      Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4RK0_SELPH_DQ3                        (DRAMC_AO_BASE_ADDRESS + 0x01C38)
    #define SHU4RK0_SELPH_DQ3_DLY_DQM0                         Fld(3, 0) //[2:0]
    #define SHU4RK0_SELPH_DQ3_DLY_DQM1                         Fld(3, 4) //[6:4]
    #define SHU4RK0_SELPH_DQ3_DLY_DQM2                         Fld(3, 8) //[10:8]
    #define SHU4RK0_SELPH_DQ3_DLY_DQM3                         Fld(3, 12) //[14:12]
    #define SHU4RK0_SELPH_DQ3_DLY_OEN_DQM0                     Fld(3, 16) //[18:16]
    #define SHU4RK0_SELPH_DQ3_DLY_OEN_DQM1                     Fld(3, 20) //[22:20]
    #define SHU4RK0_SELPH_DQ3_DLY_OEN_DQM2                     Fld(3, 24) //[26:24]
    #define SHU4RK0_SELPH_DQ3_DLY_OEN_DQM3                     Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4RK0_DQS2DQ_CAL1                      (DRAMC_AO_BASE_ADDRESS + 0x01C40)
    #define SHU4RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0           Fld(11, 0) //[10:0]
    #define SHU4RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1           Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU4RK0_DQS2DQ_CAL2                      (DRAMC_AO_BASE_ADDRESS + 0x01C44)
    #define SHU4RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0           Fld(11, 0) //[10:0]
    #define SHU4RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1           Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU4RK0_DQS2DQ_CAL3                      (DRAMC_AO_BASE_ADDRESS + 0x01C48)
    #define SHU4RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0       Fld(6, 0) //[5:0]
    #define SHU4RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1       Fld(6, 6) //[11:6]
    #define SHU4RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0_B4TO0 Fld(5, 12) //[16:12]
    #define SHU4RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1_B4TO0 Fld(5, 17) //[21:17]

#define DRAMC_REG_SHU4RK0_DQS2DQ_CAL4                      (DRAMC_AO_BASE_ADDRESS + 0x01C4C)
    #define SHU4RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0      Fld(6, 0) //[5:0]
    #define SHU4RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1      Fld(6, 6) //[11:6]
    #define SHU4RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0_B4TO0 Fld(5, 12) //[16:12]
    #define SHU4RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1_B4TO0 Fld(5, 17) //[21:17]

#define DRAMC_REG_SHU4RK0_DQS2DQ_CAL5                      (DRAMC_AO_BASE_ADDRESS + 0x01C50)
    #define SHU4RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0          Fld(11, 0) //[10:0]
    #define SHU4RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1          Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU4RK1_DQSCTL                           (DRAMC_AO_BASE_ADDRESS + 0x01D00)
    #define SHU4RK1_DQSCTL_R1DQSINCTL                          Fld(4, 0) //[3:0]

#define DRAMC_REG_SHU4RK1_DQSIEN                           (DRAMC_AO_BASE_ADDRESS + 0x01D04)
    #define SHU4RK1_DQSIEN_R1DQS0IEN                           Fld(7, 0) //[6:0]
    #define SHU4RK1_DQSIEN_R1DQS1IEN                           Fld(7, 8) //[14:8]
    #define SHU4RK1_DQSIEN_R1DQS2IEN                           Fld(7, 16) //[22:16]
    #define SHU4RK1_DQSIEN_R1DQS3IEN                           Fld(7, 24) //[30:24]

#define DRAMC_REG_SHU4RK1_DQSCAL                           (DRAMC_AO_BASE_ADDRESS + 0x01D08)
    #define SHU4RK1_DQSCAL_R1DQSIENLLMT                        Fld(7, 0) //[6:0]
    #define SHU4RK1_DQSCAL_R1DQSIENLLMTEN                      Fld(1, 7) //[7:7]
    #define SHU4RK1_DQSCAL_R1DQSIENHLMT                        Fld(7, 8) //[14:8]
    #define SHU4RK1_DQSCAL_R1DQSIENHLMTEN                      Fld(1, 15) //[15:15]

#define DRAMC_REG_SHU4RK1_PI                               (DRAMC_AO_BASE_ADDRESS + 0x01D0C)
    #define SHU4RK1_PI_RK1_ARPI_DQ_B1                          Fld(6, 0) //[5:0]
    #define SHU4RK1_PI_RK1_ARPI_DQ_B0                          Fld(6, 8) //[13:8]
    #define SHU4RK1_PI_RK1_ARPI_DQM_B1                         Fld(6, 16) //[21:16]
    #define SHU4RK1_PI_RK1_ARPI_DQM_B0                         Fld(6, 24) //[29:24]

#define DRAMC_REG_SHU4RK1_DQSOSC                           (DRAMC_AO_BASE_ADDRESS + 0x01D10)
    #define SHU4RK1_DQSOSC_DQSOSC_BASE_RK1                     Fld(16, 0) //[15:0]
    #define SHU4RK1_DQSOSC_DQSOSC_BASE_RK1_B1                  Fld(16, 16) //[31:16]

#define DRAMC_REG_SHU4RK1_SELPH_ODTEN0                     (DRAMC_AO_BASE_ADDRESS + 0x01D1C)
    #define SHU4RK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN             Fld(3, 0) //[2:0]
    #define SHU4RK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN_P1          Fld(3, 4) //[6:4]
    #define SHU4RK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN             Fld(3, 8) //[10:8]
    #define SHU4RK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN_P1          Fld(3, 12) //[14:12]
    #define SHU4RK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN             Fld(3, 16) //[18:16]
    #define SHU4RK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN_P1          Fld(3, 20) //[22:20]
    #define SHU4RK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN             Fld(3, 24) //[26:24]
    #define SHU4RK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN_P1          Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4RK1_SELPH_ODTEN1                     (DRAMC_AO_BASE_ADDRESS + 0x01D20)
    #define SHU4RK1_SELPH_ODTEN1_DLY_B0_R1RODTEN               Fld(3, 0) //[2:0]
    #define SHU4RK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1            Fld(3, 4) //[6:4]
    #define SHU4RK1_SELPH_ODTEN1_DLY_B1_R1RODTEN               Fld(3, 8) //[10:8]
    #define SHU4RK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1            Fld(3, 12) //[14:12]
    #define SHU4RK1_SELPH_ODTEN1_DLY_B2_R1RODTEN               Fld(3, 16) //[18:16]
    #define SHU4RK1_SELPH_ODTEN1_DLY_B2_R1RODTEN_P1            Fld(3, 20) //[22:20]
    #define SHU4RK1_SELPH_ODTEN1_DLY_B3_R1RODTEN               Fld(3, 24) //[26:24]
    #define SHU4RK1_SELPH_ODTEN1_DLY_B3_R1RODTEN_P1            Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4RK1_SELPH_DQSG0                      (DRAMC_AO_BASE_ADDRESS + 0x01D24)
    #define SHU4RK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED            Fld(3, 0) //[2:0]
    #define SHU4RK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1         Fld(3, 4) //[6:4]
    #define SHU4RK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED            Fld(3, 8) //[10:8]
    #define SHU4RK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1         Fld(3, 12) //[14:12]
    #define SHU4RK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED            Fld(3, 16) //[18:16]
    #define SHU4RK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED_P1         Fld(3, 20) //[22:20]
    #define SHU4RK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED            Fld(3, 24) //[26:24]
    #define SHU4RK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED_P1         Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4RK1_SELPH_DQSG1                      (DRAMC_AO_BASE_ADDRESS + 0x01D28)
    #define SHU4RK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED           Fld(3, 0) //[2:0]
    #define SHU4RK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1        Fld(3, 4) //[6:4]
    #define SHU4RK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED           Fld(3, 8) //[10:8]
    #define SHU4RK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1        Fld(3, 12) //[14:12]
    #define SHU4RK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED           Fld(3, 16) //[18:16]
    #define SHU4RK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED_P1        Fld(3, 20) //[22:20]
    #define SHU4RK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED           Fld(3, 24) //[26:24]
    #define SHU4RK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED_P1        Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4RK1_SELPH_DQ0                        (DRAMC_AO_BASE_ADDRESS + 0x01D2C)
    #define SHU4RK1_SELPH_DQ0_TX_DLY_R1DQ0                     Fld(3, 0) //[2:0]
    #define SHU4RK1_SELPH_DQ0_TX_DLY_R1DQ1                     Fld(3, 4) //[6:4]
    #define SHU4RK1_SELPH_DQ0_TX_DLY_R1DQ2                     Fld(3, 8) //[10:8]
    #define SHU4RK1_SELPH_DQ0_TX_DLY_R1DQ3                     Fld(3, 12) //[14:12]
    #define SHU4RK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0                 Fld(3, 16) //[18:16]
    #define SHU4RK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1                 Fld(3, 20) //[22:20]
    #define SHU4RK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2                 Fld(3, 24) //[26:24]
    #define SHU4RK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3                 Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4RK1_SELPH_DQ1                        (DRAMC_AO_BASE_ADDRESS + 0x01D30)
    #define SHU4RK1_SELPH_DQ1_TX_DLY_R1DQM0                    Fld(3, 0) //[2:0]
    #define SHU4RK1_SELPH_DQ1_TX_DLY_R1DQM1                    Fld(3, 4) //[6:4]
    #define SHU4RK1_SELPH_DQ1_TX_DLY_R1DQM2                    Fld(3, 8) //[10:8]
    #define SHU4RK1_SELPH_DQ1_TX_DLY_R1DQM3                    Fld(3, 12) //[14:12]
    #define SHU4RK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0                Fld(3, 16) //[18:16]
    #define SHU4RK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1                Fld(3, 20) //[22:20]
    #define SHU4RK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2                Fld(3, 24) //[26:24]
    #define SHU4RK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3                Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4RK1_SELPH_DQ2                        (DRAMC_AO_BASE_ADDRESS + 0x01D34)
    #define SHU4RK1_SELPH_DQ2_DLY_R1DQ0                        Fld(3, 0) //[2:0]
    #define SHU4RK1_SELPH_DQ2_DLY_R1DQ1                        Fld(3, 4) //[6:4]
    #define SHU4RK1_SELPH_DQ2_DLY_R1DQ2                        Fld(3, 8) //[10:8]
    #define SHU4RK1_SELPH_DQ2_DLY_R1DQ3                        Fld(3, 12) //[14:12]
    #define SHU4RK1_SELPH_DQ2_DLY_R1OEN_DQ0                    Fld(3, 16) //[18:16]
    #define SHU4RK1_SELPH_DQ2_DLY_R1OEN_DQ1                    Fld(3, 20) //[22:20]
    #define SHU4RK1_SELPH_DQ2_DLY_R1OEN_DQ2                    Fld(3, 24) //[26:24]
    #define SHU4RK1_SELPH_DQ2_DLY_R1OEN_DQ3                    Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4RK1_SELPH_DQ3                        (DRAMC_AO_BASE_ADDRESS + 0x01D38)
    #define SHU4RK1_SELPH_DQ3_DLY_R1DQM0                       Fld(3, 0) //[2:0]
    #define SHU4RK1_SELPH_DQ3_DLY_R1DQM1                       Fld(3, 4) //[6:4]
    #define SHU4RK1_SELPH_DQ3_DLY_R1DQM2                       Fld(3, 8) //[10:8]
    #define SHU4RK1_SELPH_DQ3_DLY_R1DQM3                       Fld(3, 12) //[14:12]
    #define SHU4RK1_SELPH_DQ3_DLY_R1OEN_DQM0                   Fld(3, 16) //[18:16]
    #define SHU4RK1_SELPH_DQ3_DLY_R1OEN_DQM1                   Fld(3, 20) //[22:20]
    #define SHU4RK1_SELPH_DQ3_DLY_R1OEN_DQM2                   Fld(3, 24) //[26:24]
    #define SHU4RK1_SELPH_DQ3_DLY_R1OEN_DQM3                   Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4RK1_DQS2DQ_CAL1                      (DRAMC_AO_BASE_ADDRESS + 0x01D40)
    #define SHU4RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0           Fld(11, 0) //[10:0]
    #define SHU4RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1           Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU4RK1_DQS2DQ_CAL2                      (DRAMC_AO_BASE_ADDRESS + 0x01D44)
    #define SHU4RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0           Fld(11, 0) //[10:0]
    #define SHU4RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1           Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU4RK1_DQS2DQ_CAL3                      (DRAMC_AO_BASE_ADDRESS + 0x01D48)
    #define SHU4RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0       Fld(6, 0) //[5:0]
    #define SHU4RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1       Fld(6, 6) //[11:6]
    #define SHU4RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0_B4TO0 Fld(5, 12) //[16:12]
    #define SHU4RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1_B4TO0 Fld(5, 17) //[21:17]

#define DRAMC_REG_SHU4RK1_DQS2DQ_CAL4                      (DRAMC_AO_BASE_ADDRESS + 0x01D4C)
    #define SHU4RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0      Fld(6, 0) //[5:0]
    #define SHU4RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1      Fld(6, 6) //[11:6]
    #define SHU4RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0_B4TO0 Fld(5, 12) //[16:12]
    #define SHU4RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1_B4TO0 Fld(5, 17) //[21:17]

#define DRAMC_REG_SHU4RK1_DQS2DQ_CAL5                      (DRAMC_AO_BASE_ADDRESS + 0x01D50)
    #define SHU4RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0          Fld(11, 0) //[10:0]
    #define SHU4RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1          Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU4RK2_DQSCTL                           (DRAMC_AO_BASE_ADDRESS + 0x01E00)
    #define SHU4RK2_DQSCTL_R2DQSINCTL                          Fld(4, 0) //[3:0]

#define DRAMC_REG_SHU4RK2_DQSIEN                           (DRAMC_AO_BASE_ADDRESS + 0x01E04)
    #define SHU4RK2_DQSIEN_R2DQS0IEN                           Fld(7, 0) //[6:0]
    #define SHU4RK2_DQSIEN_R2DQS1IEN                           Fld(7, 8) //[14:8]
    #define SHU4RK2_DQSIEN_R2DQS2IEN                           Fld(7, 16) //[22:16]
    #define SHU4RK2_DQSIEN_R2DQS3IEN                           Fld(7, 24) //[30:24]

#define DRAMC_REG_SHU4RK2_DQSCAL                           (DRAMC_AO_BASE_ADDRESS + 0x01E08)
    #define SHU4RK2_DQSCAL_R2DQSIENLLMT                        Fld(7, 0) //[6:0]
    #define SHU4RK2_DQSCAL_R2DQSIENLLMTEN                      Fld(1, 7) //[7:7]
    #define SHU4RK2_DQSCAL_R2DQSIENHLMT                        Fld(7, 8) //[14:8]
    #define SHU4RK2_DQSCAL_R2DQSIENHLMTEN                      Fld(1, 15) //[15:15]

#define DRAMC_REG_SHU4RK2_PI                               (DRAMC_AO_BASE_ADDRESS + 0x01E0C)
    #define SHU4RK2_PI_RK2_ARPI_DQ_B1                          Fld(6, 0) //[5:0]
    #define SHU4RK2_PI_RK2_ARPI_DQ_B0                          Fld(6, 8) //[13:8]
    #define SHU4RK2_PI_RK2_ARPI_DQM_B1                         Fld(6, 16) //[21:16]
    #define SHU4RK2_PI_RK2_ARPI_DQM_B0                         Fld(6, 24) //[29:24]

#define DRAMC_REG_SHU4RK2_DQSOSC                           (DRAMC_AO_BASE_ADDRESS + 0x01E10)
    #define SHU4RK2_DQSOSC_DQSOSC_BASE_RK2                     Fld(16, 0) //[15:0]
    #define SHU4RK2_DQSOSC_DQSOSC_BASE_RK2_B1                  Fld(16, 16) //[31:16]

#define DRAMC_REG_SHU4RK2_SELPH_ODTEN0                     (DRAMC_AO_BASE_ADDRESS + 0x01E1C)
    #define SHU4RK2_SELPH_ODTEN0_TXDLY_B0_R2RODTEN             Fld(3, 0) //[2:0]
    #define SHU4RK2_SELPH_ODTEN0_TXDLY_B0_R2RODTEN_P1          Fld(3, 4) //[6:4]
    #define SHU4RK2_SELPH_ODTEN0_TXDLY_B1_R2RODTEN             Fld(3, 8) //[10:8]
    #define SHU4RK2_SELPH_ODTEN0_TXDLY_B1_R2RODTEN_P1          Fld(3, 12) //[14:12]
    #define SHU4RK2_SELPH_ODTEN0_TXDLY_B2_R2RODTEN             Fld(3, 16) //[18:16]
    #define SHU4RK2_SELPH_ODTEN0_TXDLY_B2_R2RODTEN_P1          Fld(3, 20) //[22:20]
    #define SHU4RK2_SELPH_ODTEN0_TXDLY_B3_R2RODTEN             Fld(3, 24) //[26:24]
    #define SHU4RK2_SELPH_ODTEN0_TXDLY_B3_R2RODTEN_P1          Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4RK2_SELPH_ODTEN1                     (DRAMC_AO_BASE_ADDRESS + 0x01E20)
    #define SHU4RK2_SELPH_ODTEN1_DLY_B0_R2RODTEN               Fld(3, 0) //[2:0]
    #define SHU4RK2_SELPH_ODTEN1_DLY_B0_R2RODTEN_P1            Fld(3, 4) //[6:4]
    #define SHU4RK2_SELPH_ODTEN1_DLY_B1_R2RODTEN               Fld(3, 8) //[10:8]
    #define SHU4RK2_SELPH_ODTEN1_DLY_B1_R2RODTEN_P1            Fld(3, 12) //[14:12]
    #define SHU4RK2_SELPH_ODTEN1_DLY_B2_R2RODTEN               Fld(3, 16) //[18:16]
    #define SHU4RK2_SELPH_ODTEN1_DLY_B2_R2RODTEN_P1            Fld(3, 20) //[22:20]
    #define SHU4RK2_SELPH_ODTEN1_DLY_B3_R2RODTEN               Fld(3, 24) //[26:24]
    #define SHU4RK2_SELPH_ODTEN1_DLY_B3_R2RODTEN_P1            Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4RK2_SELPH_DQSG0                      (DRAMC_AO_BASE_ADDRESS + 0x01E24)
    #define SHU4RK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED            Fld(3, 0) //[2:0]
    #define SHU4RK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED_P1         Fld(3, 4) //[6:4]
    #define SHU4RK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED            Fld(3, 8) //[10:8]
    #define SHU4RK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED_P1         Fld(3, 12) //[14:12]
    #define SHU4RK2_SELPH_DQSG0_TX_DLY_R2DQS2_GATED            Fld(3, 16) //[18:16]
    #define SHU4RK2_SELPH_DQSG0_TX_DLY_R2DQS2_GATED_P1         Fld(3, 20) //[22:20]
    #define SHU4RK2_SELPH_DQSG0_TX_DLY_R2DQS3_GATED            Fld(3, 24) //[26:24]
    #define SHU4RK2_SELPH_DQSG0_TX_DLY_R2DQS3_GATED_P1         Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4RK2_SELPH_DQSG1                      (DRAMC_AO_BASE_ADDRESS + 0x01E28)
    #define SHU4RK2_SELPH_DQSG1_REG_DLY_R2DQS0_GATED           Fld(3, 0) //[2:0]
    #define SHU4RK2_SELPH_DQSG1_REG_DLY_R2DQS0_GATED_P1        Fld(3, 4) //[6:4]
    #define SHU4RK2_SELPH_DQSG1_REG_DLY_R2DQS1_GATED           Fld(3, 8) //[10:8]
    #define SHU4RK2_SELPH_DQSG1_REG_DLY_R2DQS1_GATED_P1        Fld(3, 12) //[14:12]
    #define SHU4RK2_SELPH_DQSG1_REG_DLY_R2DQS2_GATED           Fld(3, 16) //[18:16]
    #define SHU4RK2_SELPH_DQSG1_REG_DLY_R2DQS2_GATED_P1        Fld(3, 20) //[22:20]
    #define SHU4RK2_SELPH_DQSG1_REG_DLY_R2DQS3_GATED           Fld(3, 24) //[26:24]
    #define SHU4RK2_SELPH_DQSG1_REG_DLY_R2DQS3_GATED_P1        Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4RK2_SELPH_DQ0                        (DRAMC_AO_BASE_ADDRESS + 0x01E2C)
    #define SHU4RK2_SELPH_DQ0_TX_DLY_R2DQ0                     Fld(3, 0) //[2:0]
    #define SHU4RK2_SELPH_DQ0_TX_DLY_R2DQ1                     Fld(3, 4) //[6:4]
    #define SHU4RK2_SELPH_DQ0_TX_DLY_R2DQ2                     Fld(3, 8) //[10:8]
    #define SHU4RK2_SELPH_DQ0_TX_DLY_R2DQ3                     Fld(3, 12) //[14:12]
    #define SHU4RK2_SELPH_DQ0_TX_DLY_R2OEN_DQ0                 Fld(3, 16) //[18:16]
    #define SHU4RK2_SELPH_DQ0_TX_DLY_R2OEN_DQ1                 Fld(3, 20) //[22:20]
    #define SHU4RK2_SELPH_DQ0_TX_DLY_R2OEN_DQ2                 Fld(3, 24) //[26:24]
    #define SHU4RK2_SELPH_DQ0_TX_DLY_R2OEN_DQ3                 Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4RK2_SELPH_DQ1                        (DRAMC_AO_BASE_ADDRESS + 0x01E30)
    #define SHU4RK2_SELPH_DQ1_TX_DLY_R2DQM0                    Fld(3, 0) //[2:0]
    #define SHU4RK2_SELPH_DQ1_TX_DLY_R2DQM1                    Fld(3, 4) //[6:4]
    #define SHU4RK2_SELPH_DQ1_TX_DLY_R2DQM2                    Fld(3, 8) //[10:8]
    #define SHU4RK2_SELPH_DQ1_TX_DLY_R2DQM3                    Fld(3, 12) //[14:12]
    #define SHU4RK2_SELPH_DQ1_TX_DLY_R2OEN_DQM0                Fld(3, 16) //[18:16]
    #define SHU4RK2_SELPH_DQ1_TX_DLY_R2OEN_DQM1                Fld(3, 20) //[22:20]
    #define SHU4RK2_SELPH_DQ1_TX_DLY_R2OEN_DQM2                Fld(3, 24) //[26:24]
    #define SHU4RK2_SELPH_DQ1_TX_DLY_R2OEN_DQM3                Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4RK2_SELPH_DQ2                        (DRAMC_AO_BASE_ADDRESS + 0x01E34)
    #define SHU4RK2_SELPH_DQ2_DLY_R2DQ0                        Fld(3, 0) //[2:0]
    #define SHU4RK2_SELPH_DQ2_DLY_R2DQ1                        Fld(3, 4) //[6:4]
    #define SHU4RK2_SELPH_DQ2_DLY_R2DQ2                        Fld(3, 8) //[10:8]
    #define SHU4RK2_SELPH_DQ2_DLY_R2DQ3                        Fld(3, 12) //[14:12]
    #define SHU4RK2_SELPH_DQ2_DLY_R2OEN_DQ0                    Fld(3, 16) //[18:16]
    #define SHU4RK2_SELPH_DQ2_DLY_R2OEN_DQ1                    Fld(3, 20) //[22:20]
    #define SHU4RK2_SELPH_DQ2_DLY_R2OEN_DQ2                    Fld(3, 24) //[26:24]
    #define SHU4RK2_SELPH_DQ2_DLY_R2OEN_DQ3                    Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4RK2_SELPH_DQ3                        (DRAMC_AO_BASE_ADDRESS + 0x01E38)
    #define SHU4RK2_SELPH_DQ3_DLY_R2DQM0                       Fld(3, 0) //[2:0]
    #define SHU4RK2_SELPH_DQ3_DLY_R2DQM1                       Fld(3, 4) //[6:4]
    #define SHU4RK2_SELPH_DQ3_DLY_R2DQM2                       Fld(3, 8) //[10:8]
    #define SHU4RK2_SELPH_DQ3_DLY_R2DQM3                       Fld(3, 12) //[14:12]
    #define SHU4RK2_SELPH_DQ3_DLY_R2OEN_DQM0                   Fld(3, 16) //[18:16]
    #define SHU4RK2_SELPH_DQ3_DLY_R2OEN_DQM1                   Fld(3, 20) //[22:20]
    #define SHU4RK2_SELPH_DQ3_DLY_R2OEN_DQM2                   Fld(3, 24) //[26:24]
    #define SHU4RK2_SELPH_DQ3_DLY_R2OEN_DQM3                   Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU4RK2_DQS2DQ_CAL1                      (DRAMC_AO_BASE_ADDRESS + 0x01E40)
    #define SHU4RK2_DQS2DQ_CAL1_BOOT_ORIG_UI_RK2_DQ0           Fld(11, 0) //[10:0]
    #define SHU4RK2_DQS2DQ_CAL1_BOOT_ORIG_UI_RK2_DQ1           Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU4RK2_DQS2DQ_CAL2                      (DRAMC_AO_BASE_ADDRESS + 0x01E44)
    #define SHU4RK2_DQS2DQ_CAL2_BOOT_TARG_UI_RK2_DQ0           Fld(11, 0) //[10:0]
    #define SHU4RK2_DQS2DQ_CAL2_BOOT_TARG_UI_RK2_DQ1           Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU4RK2_DQS2DQ_CAL3                      (DRAMC_AO_BASE_ADDRESS + 0x01E48)
    #define SHU4RK2_DQS2DQ_CAL3_BOOT_TARG_UI_RK2_OEN_DQ0       Fld(6, 0) //[5:0]
    #define SHU4RK2_DQS2DQ_CAL3_BOOT_TARG_UI_RK2_OEN_DQ1       Fld(6, 6) //[11:6]

#define DRAMC_REG_SHU4RK2_DQS2DQ_CAL4                      (DRAMC_AO_BASE_ADDRESS + 0x01E4C)
    #define SHU4RK2_DQS2DQ_CAL4_BOOT_TARG_UI_RK2_OEN_DQM0      Fld(6, 0) //[5:0]
    #define SHU4RK2_DQS2DQ_CAL4_BOOT_TARG_UI_RK2_OEN_DQM1      Fld(6, 6) //[11:6]

#define DRAMC_REG_SHU4RK2_DQS2DQ_CAL5                      (DRAMC_AO_BASE_ADDRESS + 0x01E50)
    #define SHU4RK2_DQS2DQ_CAL5_BOOT_TARG_UI_RK2_DQM0          Fld(11, 0) //[10:0]
    #define SHU4RK2_DQS2DQ_CAL5_BOOT_TARG_UI_RK2_DQM1          Fld(11, 16) //[26:16]

#define DRAMC_REG_SHU4_DQSG_RETRY                          (DRAMC_AO_BASE_ADDRESS + 0x01E54)
    #define SHU4_DQSG_RETRY_R_DQSGRETRY_SW_RESET               Fld(1, 0) //[0:0]
    #define SHU4_DQSG_RETRY_R_DQSG_RETRY_SW_EN                 Fld(1, 1) //[1:1]
    #define SHU4_DQSG_RETRY_R_DDR1866_PLUS                     Fld(1, 2) //[2:2]
    #define SHU4_DQSG_RETRY_R_RETRY_ONCE                       Fld(1, 3) //[3:3]
    #define SHU4_DQSG_RETRY_R_RETRY_3TIMES                     Fld(1, 4) //[4:4]
    #define SHU4_DQSG_RETRY_R_RETRY_1RANK                      Fld(1, 5) //[5:5]
    #define SHU4_DQSG_RETRY_R_RETRY_SAV_MSK                    Fld(1, 6) //[6:6]
    #define SHU4_DQSG_RETRY_R_DM4BYTE                          Fld(1, 7) //[7:7]
    #define SHU4_DQSG_RETRY_R_DQSIENLAT                        Fld(4, 8) //[11:8]
    #define SHU4_DQSG_RETRY_R_STBENCMP_ALLBYTE                 Fld(1, 12) //[12:12]
    #define SHU4_DQSG_RETRY_R_XSR_DQSG_RETRY_EN                Fld(1, 13) //[13:13]
    #define SHU4_DQSG_RETRY_R_XSR_RETRY_SPM_MODE               Fld(1, 14) //[14:14]
    #define SHU4_DQSG_RETRY_R_RETRY_CMP_DATA                   Fld(1, 15) //[15:15]
    #define SHU4_DQSG_RETRY_R_RETRY_ALE_BLOCK_MASK             Fld(1, 20) //[20:20]
    #define SHU4_DQSG_RETRY_R_RDY_SEL_DLE                      Fld(1, 21) //[21:21]
    #define SHU4_DQSG_RETRY_R_RETRY_ROUND_NUM                  Fld(2, 24) //[25:24]
    #define SHU4_DQSG_RETRY_R_RETRY_RANKSEL_FROM_PHY           Fld(1, 28) //[28:28]
    #define SHU4_DQSG_RETRY_R_RETRY_PA_DSIABLE                 Fld(1, 29) //[29:29]
    #define SHU4_DQSG_RETRY_R_RETRY_STBEN_RESET_MSK            Fld(1, 30) //[30:30]
    #define SHU4_DQSG_RETRY_R_RETRY_USE_BURST_MDOE             Fld(1, 31) //[31:31]

/*-----------------------------------------------
 * Page DDRPHY_AO
 *-----------------------------------------------
 */
#define DDRPHY_AO_BASE_ADDRESS Channel_A_PHY_AO_BASE_VIRTUAL

#define DDRPHY_PLL1                                    (DDRPHY_AO_BASE_ADDRESS + 0x00000)
    #define PLL1_RG_RPHYPLL_SDM_SSC_EN                         Fld(1, 2) //[2:2]
    #define PLL1_RG_RPHYPLL_EN                                 Fld(1, 31) //[31:31]

#define DDRPHY_PLL2                                    (DDRPHY_AO_BASE_ADDRESS + 0x00004)
    #define PLL2_RG_RCLRPLL_SDM_SSC_EN                         Fld(1, 2) //[2:2]
    #define PLL2_RG_RCLRPLL_EN                                 Fld(1, 31) //[31:31]

#define DDRPHY_PLL3                                    (DDRPHY_AO_BASE_ADDRESS + 0x00008)
    #define PLL3_RG_RPHYPLL_TSTOP_EN                           Fld(1, 0) //[0:0]
    #define PLL3_RG_RPHYPLL_TSTOD_EN                           Fld(1, 1) //[1:1]
    #define PLL3_RG_RPHYPLL_TSTFM_EN                           Fld(1, 2) //[2:2]
    #define PLL3_RG_RPHYPLL_TSTCK_EN                           Fld(1, 3) //[3:3]
    #define PLL3_RG_RPHYPLL_TST_EN                             Fld(1, 4) //[4:4]
    #define PLL3_RG_RPHYPLL_TSTLVROD_EN                        Fld(1, 5) //[5:5]
    #define PLL3_RG_RPHYPLL_TST_SEL                            Fld(4, 8) //[11:8]

#define DDRPHY_PLL4                                    (DDRPHY_AO_BASE_ADDRESS + 0x0000C)
    #define PLL4_RG_RPHYPLL_RESETB                             Fld(1, 16) //[16:16]
    #define PLL4_RG_RPHYPLL_ATPG_EN                            Fld(1, 17) //[17:17]
    #define PLL4_RG_RPHYPLL_MCK8X_SEL                          Fld(1, 18) //[18:18]
    #define PLL4_PLL4_RFU                                      Fld(1, 19) //[19:19]
    #define PLL4_RG_RPHYPLL_SER_MODE                           Fld(1, 20) //[20:20]
    #define PLL4_RG_RPHYPLL_AD_MCK8X_EN                        Fld(1, 21) //[21:21]
    #define PLL4_RG_RPHYPLL_ADA_MCK8X_EN                       Fld(1, 22) //[22:22]
    #define PLL4_RESERVED_0X0C                                 Fld(1, 24) //[24:24]

#define DDRPHY_PLL5                                    (DDRPHY_AO_BASE_ADDRESS + 0x00010)
    #define PLL5_RESERVED_0X010                                Fld(32, 0) //[31:0]

#define DDRPHY_PLL6                                    (DDRPHY_AO_BASE_ADDRESS + 0x00014)
    #define PLL6_RESERVED_0X014                                Fld(32, 0) //[31:0]

#define DDRPHY_PLL7                                    (DDRPHY_AO_BASE_ADDRESS + 0x00018)
    #define PLL7_RESERVED_0X018                                Fld(32, 0) //[31:0]

#define DDRPHY_PLL8                                    (DDRPHY_AO_BASE_ADDRESS + 0x0001C)
    #define PLL8_RESERVED_0X01C                                Fld(32, 0) //[31:0]

#define DDRPHY_PLL9                                    (DDRPHY_AO_BASE_ADDRESS + 0x00020)
    #define PLL9_RESERVED_0X020                                Fld(32, 0) //[31:0]

#define DDRPHY_PLL10                                   (DDRPHY_AO_BASE_ADDRESS + 0x00024)
    #define PLL10_RESERVED_0X024                               Fld(32, 0) //[31:0]

#define DDRPHY_PLL11                                   (DDRPHY_AO_BASE_ADDRESS + 0x00028)
    #define PLL11_RESERVED_0X028                               Fld(32, 0) //[31:0]

#define DDRPHY_PLL12                                   (DDRPHY_AO_BASE_ADDRESS + 0x0002C)
    #define PLL12_RESERVED_0X02C                               Fld(32, 0) //[31:0]

#define DDRPHY_PLL13                                   (DDRPHY_AO_BASE_ADDRESS + 0x00030)
    #define PLL13_RESERVED_0X030                               Fld(32, 0) //[31:0]

#define DDRPHY_PLL14                                   (DDRPHY_AO_BASE_ADDRESS + 0x00034)
    #define PLL14_RESERVED_0X034                               Fld(32, 0) //[31:0]

#define DDRPHY_PLL15                                   (DDRPHY_AO_BASE_ADDRESS + 0x00038)
    #define PLL15_RESERVED_0X038                               Fld(32, 0) //[31:0]

#define DDRPHY_PLL16                                   (DDRPHY_AO_BASE_ADDRESS + 0x0003C)
    #define PLL16_RESERVED_0X03C                               Fld(32, 0) //[31:0]

#define DDRPHY_SHU1_PLL0                               (DDRPHY_AO_BASE_ADDRESS + 0x00D80)
    #define SHU1_PLL0_RG_RPHYPLL_TOP_REV                       Fld(16, 0) //[15:0]
    #define SHU1_PLL0_RG_RPHYPLL_LOAD_EN                       Fld(1, 19) //[19:19]

#define DDRPHY_SHU1_PLL1                               (DDRPHY_AO_BASE_ADDRESS + 0x00D84)
    #define SHU1_PLL1_RG_RPHYPLLGP_CK_SEL                      Fld(1, 0) //[0:0]
    #define SHU1_PLL1_SHU1_PLL1_RFU                            Fld(3, 1) //[3:1]
    #define SHU1_PLL1_R_SHU_AUTO_PLL_MUX                       Fld(1, 4) //[4:4]
    #define SHU1_PLL1_RESERVED_0XD84                           Fld(27, 5) //[31:5]

#define DDRPHY_SHU1_PLL2                               (DDRPHY_AO_BASE_ADDRESS + 0x00D88)
    #define SHU1_PLL2_RG_RCLRPLL_LOAD_EN                       Fld(1, 19) //[19:19]

#define DDRPHY_SHU1_PLL3                               (DDRPHY_AO_BASE_ADDRESS + 0x00D8C)
    #define SHU1_PLL3_RESERVED_0XD8C                           Fld(32, 0) //[31:0]

#define DDRPHY_SHU1_PLL4                               (DDRPHY_AO_BASE_ADDRESS + 0x00D90)
    #define SHU1_PLL4_RG_RPHYPLL_RESERVED                      Fld(16, 0) //[15:0]
    #define SHU1_PLL4_RG_RPHYPLL_FS                            Fld(2, 18) //[19:18]
    #define SHU1_PLL4_RG_RPHYPLL_BW                            Fld(3, 20) //[22:20]
    #define SHU1_PLL4_RG_RPHYPLL_ICHP                          Fld(2, 24) //[25:24]
    #define SHU1_PLL4_RG_RPHYPLL_IBIAS                         Fld(2, 26) //[27:26]
    #define SHU1_PLL4_RG_RPHYPLL_BLP                           Fld(1, 29) //[29:29]
    #define SHU1_PLL4_RG_RPHYPLL_BR                            Fld(1, 30) //[30:30]
    #define SHU1_PLL4_RG_RPHYPLL_BP                            Fld(1, 31) //[31:31]

#define DDRPHY_SHU1_PLL5                               (DDRPHY_AO_BASE_ADDRESS + 0x00D94)
    #define SHU1_PLL5_RG_RPHYPLL_SDM_FRA_EN                    Fld(1, 0) //[0:0]
    #define SHU1_PLL5_RG_RPHYPLL_SDM_PCW_CHG                   Fld(1, 1) //[1:1]
    #define SHU1_PLL5_RG_RPHYPLL_SDM_PCW                       Fld(16, 16) //[31:16]

#define DDRPHY_SHU1_PLL6                               (DDRPHY_AO_BASE_ADDRESS + 0x00D98)
    #define SHU1_PLL6_RG_RCLRPLL_RESERVED                      Fld(16, 0) //[15:0]
    #define SHU1_PLL6_RG_RCLRPLL_FS                            Fld(2, 18) //[19:18]
    #define SHU1_PLL6_RG_RCLRPLL_BW                            Fld(3, 20) //[22:20]
    #define SHU1_PLL6_RG_RCLRPLL_ICHP                          Fld(2, 24) //[25:24]
    #define SHU1_PLL6_RG_RCLRPLL_IBIAS                         Fld(2, 26) //[27:26]
    #define SHU1_PLL6_RG_RCLRPLL_BLP                           Fld(1, 29) //[29:29]
    #define SHU1_PLL6_RG_RCLRPLL_BR                            Fld(1, 30) //[30:30]
    #define SHU1_PLL6_RG_RCLRPLL_BP                            Fld(1, 31) //[31:31]

#define DDRPHY_SHU1_PLL7                               (DDRPHY_AO_BASE_ADDRESS + 0x00D9C)
    #define SHU1_PLL7_RG_RCLRPLL_SDM_FRA_EN                    Fld(1, 0) //[0:0]
    #define SHU1_PLL7_RG_RCLRPLL_SDM_PCW_CHG                   Fld(1, 1) //[1:1]
    #define SHU1_PLL7_RG_RCLRPLL_SDM_PCW                       Fld(16, 16) //[31:16]

#define DDRPHY_SHU1_PLL8                               (DDRPHY_AO_BASE_ADDRESS + 0x00DA0)
    #define SHU1_PLL8_RG_RPHYPLL_POSDIV                        Fld(3, 0) //[2:0]
    #define SHU1_PLL8_RG_RPHYPLL_PREDIV                        Fld(2, 18) //[19:18]

#define DDRPHY_SHU1_PLL9                               (DDRPHY_AO_BASE_ADDRESS + 0x00DA4)
    #define SHU1_PLL9_RG_RPHYPLL_RST_DLY                       Fld(2, 8) //[9:8]
    #define SHU1_PLL9_RG_RPHYPLL_LVROD_EN                      Fld(1, 12) //[12:12]
    #define SHU1_PLL9_RG_RPHYPLL_MONREF_EN                     Fld(1, 13) //[13:13]
    #define SHU1_PLL9_RG_RPHYPLL_MONVC_EN                      Fld(2, 14) //[15:14]
    #define SHU1_PLL9_RG_RPHYPLL_MONCK_EN                      Fld(1, 16) //[16:16]

#define DDRPHY_SHU1_PLL10                              (DDRPHY_AO_BASE_ADDRESS + 0x00DA8)
    #define SHU1_PLL10_RG_RCLRPLL_POSDIV                       Fld(3, 0) //[2:0]
    #define SHU1_PLL10_RG_RCLRPLL_PREDIV                       Fld(2, 18) //[19:18]

#define DDRPHY_SHU1_PLL11                              (DDRPHY_AO_BASE_ADDRESS + 0x00DAC)
    #define SHU1_PLL11_RG_RCLRPLL_RST_DLY                      Fld(2, 8) //[9:8]
    #define SHU1_PLL11_RG_RCLRPLL_LVROD_EN                     Fld(1, 12) //[12:12]
    #define SHU1_PLL11_RG_RCLRPLL_MONREF_EN                    Fld(1, 13) //[13:13]
    #define SHU1_PLL11_RG_RCLRPLL_MONVC_EN                     Fld(2, 14) //[15:14]
    #define SHU1_PLL11_RG_RCLRPLL_MONCK_EN                     Fld(1, 16) //[16:16]

#define DDRPHY_SHU1_PLL12                              (DDRPHY_AO_BASE_ADDRESS + 0x00DB0)
    #define SHU1_PLL12_RG_RCLRPLL_EXT_PODIV                    Fld(6, 0) //[5:0]
    #define SHU1_PLL12_RG_RCLRPLL_BYPASS                       Fld(1, 6) //[6:6]
    #define SHU1_PLL12_RG_RCLRPLL_EXTPODIV_EN                  Fld(1, 7) //[7:7]
    #define SHU1_PLL12_RG_RCLRPLL_EXT_FBDIV                    Fld(6, 8) //[13:8]
    #define SHU1_PLL12_RG_RCLRPLL_EXTFBDIV_EN                  Fld(1, 15) //[15:15]
    #define SHU1_PLL12_RG_RPHYPLL_EXT_FBDIV                    Fld(6, 16) //[21:16]
    #define SHU1_PLL12_RG_RPHYPLL_EXTFBDIV_EN                  Fld(1, 22) //[22:22]

#define DDRPHY_SHU1_PLL13                              (DDRPHY_AO_BASE_ADDRESS + 0x00DB4)
    #define SHU1_PLL13_RG_RCLRPLL_FB_DL                        Fld(6, 0) //[5:0]
    #define SHU1_PLL13_RG_RCLRPLL_REF_DL                       Fld(6, 8) //[13:8]
    #define SHU1_PLL13_RG_RPHYPLL_FB_DL                        Fld(6, 16) //[21:16]
    #define SHU1_PLL13_RG_RPHYPLL_REF_DL                       Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_PLL14                              (DDRPHY_AO_BASE_ADDRESS + 0x00DB8)
    #define SHU1_PLL14_RG_RPHYPLL_SDM_HREN                     Fld(1, 0) //[0:0]
    #define SHU1_PLL14_RG_RPHYPLL_SDM_SSC_PH_INIT              Fld(1, 1) //[1:1]
    #define SHU1_PLL14_RG_RPHYPLL_SDM_SSC_PRD                  Fld(16, 16) //[31:16]

#define DDRPHY_SHU1_PLL15                              (DDRPHY_AO_BASE_ADDRESS + 0x00DBC)
    #define SHU1_PLL15_RG_RPHYPLL_SDM_SSC_DELTA                Fld(16, 0) //[15:0]
    #define SHU1_PLL15_RG_RPHYPLL_SDM_SSC_DELTA1               Fld(16, 16) //[31:16]

#define DDRPHY_SHU1_PLL20                              (DDRPHY_AO_BASE_ADDRESS + 0x00DD0)
    #define SHU1_PLL20_RG_RCLRPLL_SDM_HREN                     Fld(1, 0) //[0:0]
    #define SHU1_PLL20_RG_RCLRPLL_SDM_SSC_PH_INIT              Fld(1, 1) //[1:1]
    #define SHU1_PLL20_RG_RCLRPLL_SDM_SSC_PRD                  Fld(16, 16) //[31:16]

#define DDRPHY_SHU1_PLL21                              (DDRPHY_AO_BASE_ADDRESS + 0x00DD4)
    #define SHU1_PLL21_RG_RCLRPLL_SDM_SSC_DELTA                Fld(16, 0) //[15:0]
    #define SHU1_PLL21_RG_RCLRPLL_SDM_SSC_DELTA1               Fld(16, 16) //[31:16]

#define DDRPHY_SHU2_PLL0                               (DDRPHY_AO_BASE_ADDRESS + 0x01280)
    #define SHU2_PLL0_RG_RPHYPLL_TOP_REV                       Fld(16, 0) //[15:0]
    #define SHU2_PLL0_RG_RPHYPLL_LOAD_EN                       Fld(1, 19) //[19:19]

#define DDRPHY_SHU2_PLL1                               (DDRPHY_AO_BASE_ADDRESS + 0x01284)
    #define SHU2_PLL1_RG_RPHYPLLGP_CK_SEL                      Fld(1, 0) //[0:0]
    #define SHU2_PLL1_SHU2_PLL1_RFU                            Fld(3, 1) //[3:1]
    #define SHU2_PLL1_R_SHU_AUTO_PLL_MUX                       Fld(1, 4) //[4:4]
    #define SHU2_PLL1_RESERVED_0X1284                          Fld(27, 5) //[31:5]

#define DDRPHY_SHU2_PLL2                               (DDRPHY_AO_BASE_ADDRESS + 0x01288)
    #define SHU2_PLL2_RG_RCLRPLL_LOAD_EN                       Fld(1, 19) //[19:19]

#define DDRPHY_SHU2_PLL3                               (DDRPHY_AO_BASE_ADDRESS + 0x0128C)
    #define SHU2_PLL3_RESERVED_0X128C                          Fld(32, 0) //[31:0]

#define DDRPHY_SHU2_PLL4                               (DDRPHY_AO_BASE_ADDRESS + 0x01290)
    #define SHU2_PLL4_RG_RPHYPLL_RESERVED                      Fld(16, 0) //[15:0]
    #define SHU2_PLL4_RG_RPHYPLL_FS                            Fld(2, 18) //[19:18]
    #define SHU2_PLL4_RG_RPHYPLL_BW                            Fld(3, 20) //[22:20]
    #define SHU2_PLL4_RG_RPHYPLL_ICHP                          Fld(2, 24) //[25:24]
    #define SHU2_PLL4_RG_RPHYPLL_IBIAS                         Fld(2, 26) //[27:26]
    #define SHU2_PLL4_RG_RPHYPLL_BLP                           Fld(1, 29) //[29:29]
    #define SHU2_PLL4_RG_RPHYPLL_BR                            Fld(1, 30) //[30:30]
    #define SHU2_PLL4_RG_RPHYPLL_BP                            Fld(1, 31) //[31:31]

#define DDRPHY_SHU2_PLL5                               (DDRPHY_AO_BASE_ADDRESS + 0x01294)
    #define SHU2_PLL5_RG_RPHYPLL_SDM_FRA_EN                    Fld(1, 0) //[0:0]
    #define SHU2_PLL5_RG_RPHYPLL_SDM_PCW_CHG                   Fld(1, 1) //[1:1]
    #define SHU2_PLL5_RG_RPHYPLL_SDM_PCW                       Fld(16, 16) //[31:16]

#define DDRPHY_SHU2_PLL6                               (DDRPHY_AO_BASE_ADDRESS + 0x01298)
    #define SHU2_PLL6_RG_RCLRPLL_RESERVED                      Fld(16, 0) //[15:0]
    #define SHU2_PLL6_RG_RCLRPLL_FS                            Fld(2, 18) //[19:18]
    #define SHU2_PLL6_RG_RCLRPLL_BW                            Fld(3, 20) //[22:20]
    #define SHU2_PLL6_RG_RCLRPLL_ICHP                          Fld(2, 24) //[25:24]
    #define SHU2_PLL6_RG_RCLRPLL_IBIAS                         Fld(2, 26) //[27:26]
    #define SHU2_PLL6_RG_RCLRPLL_BLP                           Fld(1, 29) //[29:29]
    #define SHU2_PLL6_RG_RCLRPLL_BR                            Fld(1, 30) //[30:30]
    #define SHU2_PLL6_RG_RCLRPLL_BP                            Fld(1, 31) //[31:31]

#define DDRPHY_SHU2_PLL7                               (DDRPHY_AO_BASE_ADDRESS + 0x0129C)
    #define SHU2_PLL7_RG_RCLRPLL_SDM_FRA_EN                    Fld(1, 0) //[0:0]
    #define SHU2_PLL7_RG_RCLRPLL_SDM_PCW_CHG                   Fld(1, 1) //[1:1]
    #define SHU2_PLL7_RG_RCLRPLL_SDM_PCW                       Fld(16, 16) //[31:16]

#define DDRPHY_SHU2_PLL8                               (DDRPHY_AO_BASE_ADDRESS + 0x012A0)
    #define SHU2_PLL8_RG_RPHYPLL_POSDIV                        Fld(3, 0) //[2:0]
    #define SHU2_PLL8_RG_RPHYPLL_PREDIV                        Fld(2, 18) //[19:18]

#define DDRPHY_SHU2_PLL9                               (DDRPHY_AO_BASE_ADDRESS + 0x012A4)
    #define SHU2_PLL9_RG_RPHYPLL_RST_DLY                       Fld(2, 8) //[9:8]
    #define SHU2_PLL9_RG_RPHYPLL_LVROD_EN                      Fld(1, 12) //[12:12]
    #define SHU2_PLL9_RG_RPHYPLL_MONREF_EN                     Fld(1, 13) //[13:13]
    #define SHU2_PLL9_RG_RPHYPLL_MONVC_EN                      Fld(2, 14) //[15:14]
    #define SHU2_PLL9_RG_RPHYPLL_MONCK_EN                      Fld(1, 16) //[16:16]

#define DDRPHY_SHU2_PLL10                              (DDRPHY_AO_BASE_ADDRESS + 0x012A8)
    #define SHU2_PLL10_RG_RCLRPLL_POSDIV                       Fld(3, 0) //[2:0]
    #define SHU2_PLL10_RG_RCLRPLL_PREDIV                       Fld(2, 18) //[19:18]

#define DDRPHY_SHU2_PLL11                              (DDRPHY_AO_BASE_ADDRESS + 0x012AC)
    #define SHU2_PLL11_RG_RCLRPLL_RST_DLY                      Fld(2, 8) //[9:8]
    #define SHU2_PLL11_RG_RCLRPLL_LVROD_EN                     Fld(1, 12) //[12:12]
    #define SHU2_PLL11_RG_RCLRPLL_MONREF_EN                    Fld(1, 13) //[13:13]
    #define SHU2_PLL11_RG_RCLRPLL_MONVC_EN                     Fld(2, 14) //[15:14]
    #define SHU2_PLL11_RG_RCLRPLL_MONCK_EN                     Fld(1, 16) //[16:16]

#define DDRPHY_SHU2_PLL12                              (DDRPHY_AO_BASE_ADDRESS + 0x012B0)
    #define SHU2_PLL12_RG_RCLRPLL_EXT_PODIV                    Fld(6, 0) //[5:0]
    #define SHU2_PLL12_RG_RCLRPLL_BYPASS                       Fld(1, 6) //[6:6]
    #define SHU2_PLL12_RG_RCLRPLL_EXTPODIV_EN                  Fld(1, 7) //[7:7]
    #define SHU2_PLL12_RG_RCLRPLL_EXT_FBDIV                    Fld(6, 8) //[13:8]
    #define SHU2_PLL12_RG_RCLRPLL_EXTFBDIV_EN                  Fld(1, 15) //[15:15]
    #define SHU2_PLL12_RG_RPHYPLL_EXT_FBDIV                    Fld(6, 16) //[21:16]
    #define SHU2_PLL12_RG_RPHYPLL_EXTFBDIV_EN                  Fld(1, 22) //[22:22]

#define DDRPHY_SHU2_PLL13                              (DDRPHY_AO_BASE_ADDRESS + 0x012B4)
    #define SHU2_PLL13_RG_RCLRPLL_FB_DL                        Fld(6, 0) //[5:0]
    #define SHU2_PLL13_RG_RCLRPLL_REF_DL                       Fld(6, 8) //[13:8]
    #define SHU2_PLL13_RG_RPHYPLL_FB_DL                        Fld(6, 16) //[21:16]
    #define SHU2_PLL13_RG_RPHYPLL_REF_DL                       Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_PLL14                              (DDRPHY_AO_BASE_ADDRESS + 0x012B8)
    #define SHU2_PLL14_RG_RPHYPLL_SDM_HREN                     Fld(1, 0) //[0:0]
    #define SHU2_PLL14_RG_RPHYPLL_SDM_SSC_PH_INIT              Fld(1, 1) //[1:1]
    #define SHU2_PLL14_RG_RPHYPLL_SDM_SSC_PRD                  Fld(16, 16) //[31:16]

#define DDRPHY_SHU2_PLL15                              (DDRPHY_AO_BASE_ADDRESS + 0x012BC)
    #define SHU2_PLL15_RG_RPHYPLL_SDM_SSC_DELTA                Fld(16, 0) //[15:0]
    #define SHU2_PLL15_RG_RPHYPLL_SDM_SSC_DELTA1               Fld(16, 16) //[31:16]

#define DDRPHY_SHU2_PLL20                              (DDRPHY_AO_BASE_ADDRESS + 0x012D0)
    #define SHU2_PLL20_RG_RCLRPLL_SDM_HREN                     Fld(1, 0) //[0:0]
    #define SHU2_PLL20_RG_RCLRPLL_SDM_SSC_PH_INIT              Fld(1, 1) //[1:1]
    #define SHU2_PLL20_RG_RCLRPLL_SDM_SSC_PRD                  Fld(16, 16) //[31:16]

#define DDRPHY_SHU2_PLL21                              (DDRPHY_AO_BASE_ADDRESS + 0x012D4)
    #define SHU2_PLL21_RG_RCLRPLL_SDM_SSC_DELTA                Fld(16, 0) //[15:0]
    #define SHU2_PLL21_RG_RCLRPLL_SDM_SSC_DELTA1               Fld(16, 16) //[31:16]

#define DDRPHY_SHU3_PLL0                               (DDRPHY_AO_BASE_ADDRESS + 0x01780)
    #define SHU3_PLL0_RG_RPHYPLL_TOP_REV                       Fld(16, 0) //[15:0]
    #define SHU3_PLL0_RG_RPHYPLL_LOAD_EN                       Fld(1, 19) //[19:19]

#define DDRPHY_SHU3_PLL1                               (DDRPHY_AO_BASE_ADDRESS + 0x01784)
    #define SHU3_PLL1_RG_RPHYPLLGP_CK_SEL                      Fld(1, 0) //[0:0]
    #define SHU3_PLL1_SHU3_PLL1_RFU                            Fld(3, 1) //[3:1]
    #define SHU3_PLL1_R_SHU_AUTO_PLL_MUX                       Fld(1, 4) //[4:4]
    #define SHU3_PLL1_RESERVED_0X1784                          Fld(27, 5) //[31:5]

#define DDRPHY_SHU3_PLL2                               (DDRPHY_AO_BASE_ADDRESS + 0x01788)
    #define SHU3_PLL2_RG_RCLRPLL_LOAD_EN                       Fld(1, 19) //[19:19]

#define DDRPHY_SHU3_PLL3                               (DDRPHY_AO_BASE_ADDRESS + 0x0178C)
    #define SHU3_PLL3_RESERVED_0X178C                          Fld(32, 0) //[31:0]

#define DDRPHY_SHU3_PLL4                               (DDRPHY_AO_BASE_ADDRESS + 0x01790)
    #define SHU3_PLL4_RG_RPHYPLL_RESERVED                      Fld(16, 0) //[15:0]
    #define SHU3_PLL4_RG_RPHYPLL_FS                            Fld(2, 18) //[19:18]
    #define SHU3_PLL4_RG_RPHYPLL_BW                            Fld(3, 20) //[22:20]
    #define SHU3_PLL4_RG_RPHYPLL_ICHP                          Fld(2, 24) //[25:24]
    #define SHU3_PLL4_RG_RPHYPLL_IBIAS                         Fld(2, 26) //[27:26]
    #define SHU3_PLL4_RG_RPHYPLL_BLP                           Fld(1, 29) //[29:29]
    #define SHU3_PLL4_RG_RPHYPLL_BR                            Fld(1, 30) //[30:30]
    #define SHU3_PLL4_RG_RPHYPLL_BP                            Fld(1, 31) //[31:31]

#define DDRPHY_SHU3_PLL5                               (DDRPHY_AO_BASE_ADDRESS + 0x01794)
    #define SHU3_PLL5_RG_RPHYPLL_SDM_FRA_EN                    Fld(1, 0) //[0:0]
    #define SHU3_PLL5_RG_RPHYPLL_SDM_PCW_CHG                   Fld(1, 1) //[1:1]
    #define SHU3_PLL5_RG_RPHYPLL_SDM_PCW                       Fld(16, 16) //[31:16]

#define DDRPHY_SHU3_PLL6                               (DDRPHY_AO_BASE_ADDRESS + 0x01798)
    #define SHU3_PLL6_RG_RCLRPLL_RESERVED                      Fld(16, 0) //[15:0]
    #define SHU3_PLL6_RG_RCLRPLL_FS                            Fld(2, 18) //[19:18]
    #define SHU3_PLL6_RG_RCLRPLL_BW                            Fld(3, 20) //[22:20]
    #define SHU3_PLL6_RG_RCLRPLL_ICHP                          Fld(2, 24) //[25:24]
    #define SHU3_PLL6_RG_RCLRPLL_IBIAS                         Fld(2, 26) //[27:26]
    #define SHU3_PLL6_RG_RCLRPLL_BLP                           Fld(1, 29) //[29:29]
    #define SHU3_PLL6_RG_RCLRPLL_BR                            Fld(1, 30) //[30:30]
    #define SHU3_PLL6_RG_RCLRPLL_BP                            Fld(1, 31) //[31:31]

#define DDRPHY_SHU3_PLL7                               (DDRPHY_AO_BASE_ADDRESS + 0x0179C)
    #define SHU3_PLL7_RG_RCLRPLL_SDM_FRA_EN                    Fld(1, 0) //[0:0]
    #define SHU3_PLL7_RG_RCLRPLL_SDM_PCW_CHG                   Fld(1, 1) //[1:1]
    #define SHU3_PLL7_RG_RCLRPLL_SDM_PCW                       Fld(16, 16) //[31:16]

#define DDRPHY_SHU3_PLL8                               (DDRPHY_AO_BASE_ADDRESS + 0x017A0)
    #define SHU3_PLL8_RG_RPHYPLL_POSDIV                        Fld(3, 0) //[2:0]
    #define SHU3_PLL8_RG_RPHYPLL_PREDIV                        Fld(2, 18) //[19:18]

#define DDRPHY_SHU3_PLL9                               (DDRPHY_AO_BASE_ADDRESS + 0x017A4)
    #define SHU3_PLL9_RG_RPHYPLL_RST_DLY                       Fld(2, 8) //[9:8]
    #define SHU3_PLL9_RG_RPHYPLL_LVROD_EN                      Fld(1, 12) //[12:12]
    #define SHU3_PLL9_RG_RPHYPLL_MONREF_EN                     Fld(1, 13) //[13:13]
    #define SHU3_PLL9_RG_RPHYPLL_MONVC_EN                      Fld(2, 14) //[15:14]
    #define SHU3_PLL9_RG_RPHYPLL_MONCK_EN                      Fld(1, 16) //[16:16]

#define DDRPHY_SHU3_PLL10                              (DDRPHY_AO_BASE_ADDRESS + 0x017A8)
    #define SHU3_PLL10_RG_RCLRPLL_POSDIV                       Fld(3, 0) //[2:0]
    #define SHU3_PLL10_RG_RCLRPLL_PREDIV                       Fld(2, 18) //[19:18]

#define DDRPHY_SHU3_PLL11                              (DDRPHY_AO_BASE_ADDRESS + 0x017AC)
    #define SHU3_PLL11_RG_RCLRPLL_RST_DLY                      Fld(2, 8) //[9:8]
    #define SHU3_PLL11_RG_RCLRPLL_LVROD_EN                     Fld(1, 12) //[12:12]
    #define SHU3_PLL11_RG_RCLRPLL_MONREF_EN                    Fld(1, 13) //[13:13]
    #define SHU3_PLL11_RG_RCLRPLL_MONVC_EN                     Fld(2, 14) //[15:14]
    #define SHU3_PLL11_RG_RCLRPLL_MONCK_EN                     Fld(1, 16) //[16:16]

#define DDRPHY_SHU3_PLL12                              (DDRPHY_AO_BASE_ADDRESS + 0x017B0)
    #define SHU3_PLL12_RG_RCLRPLL_EXT_PODIV                    Fld(6, 0) //[5:0]
    #define SHU3_PLL12_RG_RCLRPLL_BYPASS                       Fld(1, 6) //[6:6]
    #define SHU3_PLL12_RG_RCLRPLL_EXTPODIV_EN                  Fld(1, 7) //[7:7]
    #define SHU3_PLL12_RG_RCLRPLL_EXT_FBDIV                    Fld(6, 8) //[13:8]
    #define SHU3_PLL12_RG_RCLRPLL_EXTFBDIV_EN                  Fld(1, 15) //[15:15]
    #define SHU3_PLL12_RG_RPHYPLL_EXT_FBDIV                    Fld(6, 16) //[21:16]
    #define SHU3_PLL12_RG_RPHYPLL_EXTFBDIV_EN                  Fld(1, 22) //[22:22]

#define DDRPHY_SHU3_PLL13                              (DDRPHY_AO_BASE_ADDRESS + 0x017B4)
    #define SHU3_PLL13_RG_RCLRPLL_FB_DL                        Fld(6, 0) //[5:0]
    #define SHU3_PLL13_RG_RCLRPLL_REF_DL                       Fld(6, 8) //[13:8]
    #define SHU3_PLL13_RG_RPHYPLL_FB_DL                        Fld(6, 16) //[21:16]
    #define SHU3_PLL13_RG_RPHYPLL_REF_DL                       Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_PLL14                              (DDRPHY_AO_BASE_ADDRESS + 0x017B8)
    #define SHU3_PLL14_RG_RPHYPLL_SDM_HREN                     Fld(1, 0) //[0:0]
    #define SHU3_PLL14_RG_RPHYPLL_SDM_SSC_PH_INIT              Fld(1, 1) //[1:1]
    #define SHU3_PLL14_RG_RPHYPLL_SDM_SSC_PRD                  Fld(16, 16) //[31:16]

#define DDRPHY_SHU3_PLL15                              (DDRPHY_AO_BASE_ADDRESS + 0x017BC)
    #define SHU3_PLL15_RG_RPHYPLL_SDM_SSC_DELTA                Fld(16, 0) //[15:0]
    #define SHU3_PLL15_RG_RPHYPLL_SDM_SSC_DELTA1               Fld(16, 16) //[31:16]

#define DDRPHY_SHU3_PLL20                              (DDRPHY_AO_BASE_ADDRESS + 0x017D0)
    #define SHU3_PLL20_RG_RCLRPLL_SDM_HREN                     Fld(1, 0) //[0:0]
    #define SHU3_PLL20_RG_RCLRPLL_SDM_SSC_PH_INIT              Fld(1, 1) //[1:1]
    #define SHU3_PLL20_RG_RCLRPLL_SDM_SSC_PRD                  Fld(16, 16) //[31:16]

#define DDRPHY_SHU3_PLL21                              (DDRPHY_AO_BASE_ADDRESS + 0x017D4)
    #define SHU3_PLL21_RG_RCLRPLL_SDM_SSC_DELTA                Fld(16, 0) //[15:0]
    #define SHU3_PLL21_RG_RCLRPLL_SDM_SSC_DELTA1               Fld(16, 16) //[31:16]

#define DDRPHY_SHU4_PLL0                               (DDRPHY_AO_BASE_ADDRESS + 0x01C80)
    #define SHU4_PLL0_RG_RPHYPLL_TOP_REV                       Fld(16, 0) //[15:0]
    #define SHU4_PLL0_RG_RPHYPLL_LOAD_EN                       Fld(1, 19) //[19:19]

#define DDRPHY_SHU4_PLL1                               (DDRPHY_AO_BASE_ADDRESS + 0x01C84)
    #define SHU4_PLL1_RG_RPHYPLLGP_CK_SEL                      Fld(1, 0) //[0:0]
    #define SHU4_PLL1_SHU4_PLL1_RFU                            Fld(3, 1) //[3:1]
    #define SHU4_PLL1_R_SHU_AUTO_PLL_MUX                       Fld(1, 4) //[4:4]
    #define SHU4_PLL1_RESERVED_0X1C84                          Fld(27, 5) //[31:5]

#define DDRPHY_SHU4_PLL2                               (DDRPHY_AO_BASE_ADDRESS + 0x01C88)
    #define SHU4_PLL2_RG_RCLRPLL_LOAD_EN                       Fld(1, 19) //[19:19]

#define DDRPHY_SHU4_PLL3                               (DDRPHY_AO_BASE_ADDRESS + 0x01C8C)
    #define SHU4_PLL3_RESERVED_0X1C8C                          Fld(32, 0) //[31:0]

#define DDRPHY_SHU4_PLL4                               (DDRPHY_AO_BASE_ADDRESS + 0x01C90)
    #define SHU4_PLL4_RG_RPHYPLL_RESERVED                      Fld(16, 0) //[15:0]
    #define SHU4_PLL4_RG_RPHYPLL_FS                            Fld(2, 18) //[19:18]
    #define SHU4_PLL4_RG_RPHYPLL_BW                            Fld(3, 20) //[22:20]
    #define SHU4_PLL4_RG_RPHYPLL_ICHP                          Fld(2, 24) //[25:24]
    #define SHU4_PLL4_RG_RPHYPLL_IBIAS                         Fld(2, 26) //[27:26]
    #define SHU4_PLL4_RG_RPHYPLL_BLP                           Fld(1, 29) //[29:29]
    #define SHU4_PLL4_RG_RPHYPLL_BR                            Fld(1, 30) //[30:30]
    #define SHU4_PLL4_RG_RPHYPLL_BP                            Fld(1, 31) //[31:31]

#define DDRPHY_SHU4_PLL5                               (DDRPHY_AO_BASE_ADDRESS + 0x01C94)
    #define SHU4_PLL5_RG_RPHYPLL_SDM_FRA_EN                    Fld(1, 0) //[0:0]
    #define SHU4_PLL5_RG_RPHYPLL_SDM_PCW_CHG                   Fld(1, 1) //[1:1]
    #define SHU4_PLL5_RG_RPHYPLL_SDM_PCW                       Fld(16, 16) //[31:16]

#define DDRPHY_SHU4_PLL6                               (DDRPHY_AO_BASE_ADDRESS + 0x01C98)
    #define SHU4_PLL6_RG_RCLRPLL_RESERVED                      Fld(16, 0) //[15:0]
    #define SHU4_PLL6_RG_RCLRPLL_FS                            Fld(2, 18) //[19:18]
    #define SHU4_PLL6_RG_RCLRPLL_BW                            Fld(3, 20) //[22:20]
    #define SHU4_PLL6_RG_RCLRPLL_ICHP                          Fld(2, 24) //[25:24]
    #define SHU4_PLL6_RG_RCLRPLL_IBIAS                         Fld(2, 26) //[27:26]
    #define SHU4_PLL6_RG_RCLRPLL_BLP                           Fld(1, 29) //[29:29]
    #define SHU4_PLL6_RG_RCLRPLL_BR                            Fld(1, 30) //[30:30]
    #define SHU4_PLL6_RG_RCLRPLL_BP                            Fld(1, 31) //[31:31]

#define DDRPHY_SHU4_PLL7                               (DDRPHY_AO_BASE_ADDRESS + 0x01C9C)
    #define SHU4_PLL7_RG_RCLRPLL_SDM_FRA_EN                    Fld(1, 0) //[0:0]
    #define SHU4_PLL7_RG_RCLRPLL_SDM_PCW_CHG                   Fld(1, 1) //[1:1]
    #define SHU4_PLL7_RG_RCLRPLL_SDM_PCW                       Fld(16, 16) //[31:16]

#define DDRPHY_SHU4_PLL8                               (DDRPHY_AO_BASE_ADDRESS + 0x01CA0)
    #define SHU4_PLL8_RG_RPHYPLL_POSDIV                        Fld(3, 0) //[2:0]
    #define SHU4_PLL8_RG_RPHYPLL_PREDIV                        Fld(2, 18) //[19:18]

#define DDRPHY_SHU4_PLL9                               (DDRPHY_AO_BASE_ADDRESS + 0x01CA4)
    #define SHU4_PLL9_RG_RPHYPLL_RST_DLY                       Fld(2, 8) //[9:8]
    #define SHU4_PLL9_RG_RPHYPLL_LVROD_EN                      Fld(1, 12) //[12:12]
    #define SHU4_PLL9_RG_RPHYPLL_MONREF_EN                     Fld(1, 13) //[13:13]
    #define SHU4_PLL9_RG_RPHYPLL_MONVC_EN                      Fld(2, 14) //[15:14]
    #define SHU4_PLL9_RG_RPHYPLL_MONCK_EN                      Fld(1, 16) //[16:16]

#define DDRPHY_SHU4_PLL10                              (DDRPHY_AO_BASE_ADDRESS + 0x01CA8)
    #define SHU4_PLL10_RG_RCLRPLL_POSDIV                       Fld(3, 0) //[2:0]
    #define SHU4_PLL10_RG_RCLRPLL_PREDIV                       Fld(2, 18) //[19:18]

#define DDRPHY_SHU4_PLL11                              (DDRPHY_AO_BASE_ADDRESS + 0x01CAC)
    #define SHU4_PLL11_RG_RCLRPLL_RST_DLY                      Fld(2, 8) //[9:8]
    #define SHU4_PLL11_RG_RCLRPLL_LVROD_EN                     Fld(1, 12) //[12:12]
    #define SHU4_PLL11_RG_RCLRPLL_MONREF_EN                    Fld(1, 13) //[13:13]
    #define SHU4_PLL11_RG_RCLRPLL_MONVC_EN                     Fld(2, 14) //[15:14]
    #define SHU4_PLL11_RG_RCLRPLL_MONCK_EN                     Fld(1, 16) //[16:16]

#define DDRPHY_SHU4_PLL12                              (DDRPHY_AO_BASE_ADDRESS + 0x01CB0)
    #define SHU4_PLL12_RG_RCLRPLL_EXT_PODIV                    Fld(6, 0) //[5:0]
    #define SHU4_PLL12_RG_RCLRPLL_BYPASS                       Fld(1, 6) //[6:6]
    #define SHU4_PLL12_RG_RCLRPLL_EXTPODIV_EN                  Fld(1, 7) //[7:7]
    #define SHU4_PLL12_RG_RCLRPLL_EXT_FBDIV                    Fld(6, 8) //[13:8]
    #define SHU4_PLL12_RG_RCLRPLL_EXTFBDIV_EN                  Fld(1, 15) //[15:15]
    #define SHU4_PLL12_RG_RPHYPLL_EXT_FBDIV                    Fld(6, 16) //[21:16]
    #define SHU4_PLL12_RG_RPHYPLL_EXTFBDIV_EN                  Fld(1, 22) //[22:22]

#define DDRPHY_SHU4_PLL13                              (DDRPHY_AO_BASE_ADDRESS + 0x01CB4)
    #define SHU4_PLL13_RG_RCLRPLL_FB_DL                        Fld(6, 0) //[5:0]
    #define SHU4_PLL13_RG_RCLRPLL_REF_DL                       Fld(6, 8) //[13:8]
    #define SHU4_PLL13_RG_RPHYPLL_FB_DL                        Fld(6, 16) //[21:16]
    #define SHU4_PLL13_RG_RPHYPLL_REF_DL                       Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_PLL14                              (DDRPHY_AO_BASE_ADDRESS + 0x01CB8)
    #define SHU4_PLL14_RG_RPHYPLL_SDM_HREN                     Fld(1, 0) //[0:0]
    #define SHU4_PLL14_RG_RPHYPLL_SDM_SSC_PH_INIT              Fld(1, 1) //[1:1]
    #define SHU4_PLL14_RG_RPHYPLL_SDM_SSC_PRD                  Fld(16, 16) //[31:16]

#define DDRPHY_SHU4_PLL15                              (DDRPHY_AO_BASE_ADDRESS + 0x01CBC)
    #define SHU4_PLL15_RG_RPHYPLL_SDM_SSC_DELTA                Fld(16, 0) //[15:0]
    #define SHU4_PLL15_RG_RPHYPLL_SDM_SSC_DELTA1               Fld(16, 16) //[31:16]

#define DDRPHY_SHU4_PLL20                              (DDRPHY_AO_BASE_ADDRESS + 0x01CD0)
    #define SHU4_PLL20_RG_RCLRPLL_SDM_HREN                     Fld(1, 0) //[0:0]
    #define SHU4_PLL20_RG_RCLRPLL_SDM_SSC_PH_INIT              Fld(1, 1) //[1:1]
    #define SHU4_PLL20_RG_RCLRPLL_SDM_SSC_PRD                  Fld(16, 16) //[31:16]

#define DDRPHY_SHU4_PLL21                              (DDRPHY_AO_BASE_ADDRESS + 0x01CD4)
    #define SHU4_PLL21_RG_RCLRPLL_SDM_SSC_DELTA                Fld(16, 0) //[15:0]
    #define SHU4_PLL21_RG_RCLRPLL_SDM_SSC_DELTA1               Fld(16, 16) //[31:16]


#define DDRPHY_B0_DLL_ARPI0                         (DDRPHY_AO_BASE_ADDRESS + 0x00080)
    #define B0_DLL_ARPI0_RG_ARMCTLPLL_CK_SEL_B0                Fld(1, 1) //[1:1]
    #define B0_DLL_ARPI0_RG_ARPI_RESETB_B0                     Fld(1, 3) //[3:3]
    #define B0_DLL_ARPI0_RG_ARPI_LS_EN_B0                      Fld(1, 4) //[4:4]
    #define B0_DLL_ARPI0_RG_ARPI_LS_SEL_B0                     Fld(1, 5) //[5:5]
    #define B0_DLL_ARPI0_RG_ARPI_MCK8X_SEL_B0                  Fld(1, 6) //[6:6]

#define DDRPHY_B0_DLL_ARPI1                         (DDRPHY_AO_BASE_ADDRESS + 0x00084)
    #define B0_DLL_ARPI1_RG_ARPI_DQSIEN_JUMP_EN_B0             Fld(1, 11) //[11:11]
    #define B0_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B0                 Fld(1, 13) //[13:13]
    #define B0_DLL_ARPI1_RG_ARPI_DQM_JUMP_EN_B0                Fld(1, 14) //[14:14]
    #define B0_DLL_ARPI1_RG_ARPI_DQS_JUMP_EN_B0                Fld(1, 15) //[15:15]
    #define B0_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_B0                 Fld(1, 17) //[17:17]
    #define B0_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B0               Fld(1, 19) //[19:19]
    #define B0_DLL_ARPI1_RG_ARPISM_MCK_SEL_B0_REG_OPT          Fld(1, 20) //[20:20]
    #define B0_DLL_ARPI1_RG_ARPISM_MCK_SEL_B0                  Fld(1, 21) //[21:21]
    #define B0_DLL_ARPI1_RG_ARPI_SET_UPDN_B0                   Fld(3, 28) //[30:28]

#define DDRPHY_B0_DLL_ARPI2                         (DDRPHY_AO_BASE_ADDRESS + 0x00088)
    #define B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0                  Fld(1, 0) //[0:0]
    #define B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0                   Fld(1, 10) //[10:10]
    #define B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0                  Fld(1, 11) //[11:11]
    #define B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0                      Fld(1, 13) //[13:13]
    #define B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0                     Fld(1, 14) //[14:14]
    #define B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0                     Fld(1, 15) //[15:15]
    #define B0_DLL_ARPI2_RG_ARPI_CG_FB_B0                      Fld(1, 17) //[17:17]
    #define B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0                    Fld(1, 19) //[19:19]
    #define B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0              Fld(1, 27) //[27:27]
    #define B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0                     Fld(1, 31) //[31:31]

#define DDRPHY_B0_DLL_ARPI3                         (DDRPHY_AO_BASE_ADDRESS + 0x0008C)
    #define B0_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B0                  Fld(1, 11) //[11:11]
    #define B0_DLL_ARPI3_RG_ARPI_DQ_EN_B0                      Fld(1, 13) //[13:13]
    #define B0_DLL_ARPI3_RG_ARPI_DQM_EN_B0                     Fld(1, 14) //[14:14]
    #define B0_DLL_ARPI3_RG_ARPI_DQS_EN_B0                     Fld(1, 15) //[15:15]
    #define B0_DLL_ARPI3_RG_ARPI_FB_EN_B0                      Fld(1, 17) //[17:17]
    #define B0_DLL_ARPI3_RG_ARPI_MCTL_EN_B0                    Fld(1, 19) //[19:19]

#define DDRPHY_B0_DLL_ARPI4                         (DDRPHY_AO_BASE_ADDRESS + 0x00090)
    #define B0_DLL_ARPI4_RG_ARPI_BYPASS_DQSIEN_B0              Fld(1, 11) //[11:11]
    #define B0_DLL_ARPI4_RG_ARPI_BYPASS_DQ_B0                  Fld(1, 13) //[13:13]
    #define B0_DLL_ARPI4_RG_ARPI_BYPASS_DQM_B0                 Fld(1, 14) //[14:14]
    #define B0_DLL_ARPI4_RG_ARPI_BYPASS_DQS_B0                 Fld(1, 15) //[15:15]
    #define B0_DLL_ARPI4_RG_ARPI_BYPASS_FB_B0                  Fld(1, 17) //[17:17]
    #define B0_DLL_ARPI4_RG_ARPI_BYPASS_MCTL_B0                Fld(1, 19) //[19:19]

#define DDRPHY_B0_DLL_ARPI5                         (DDRPHY_AO_BASE_ADDRESS + 0x00094)
    #define B0_DLL_ARPI5_RG_ARDLL_DIV_MCTL_B0                  Fld(2, 2) //[3:2]
    #define B0_DLL_ARPI5_RG_ARDLL_MON_SEL_B0                   Fld(4, 4) //[7:4]
    #define B0_DLL_ARPI5_RG_ARDLL_DIV_DEC_B0                   Fld(1, 8) //[8:8]
    #define B0_DLL_ARPI5_B0_DLL_ARPI5_RFU                      Fld(12, 12) //[23:12]
    #define B0_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_SEL_B0           Fld(1, 25) //[25:25]
    #define B0_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_B0               Fld(1, 26) //[26:26]
    #define B0_DLL_ARPI5_B0_DLL_ARPI5_RFU1                     Fld(1, 31) //[31:31]

#define DDRPHY_B0_DQ0                               (DDRPHY_AO_BASE_ADDRESS + 0x00098)
    #define B0_DQ0_RG_RX_ARDQ0_OFFC_B0                         Fld(4, 0) //[3:0]
    #define B0_DQ0_RG_RX_ARDQ1_OFFC_B0                         Fld(4, 4) //[7:4]
    #define B0_DQ0_RG_RX_ARDQ2_OFFC_B0                         Fld(4, 8) //[11:8]
    #define B0_DQ0_RG_RX_ARDQ3_OFFC_B0                         Fld(4, 12) //[15:12]
    #define B0_DQ0_RG_RX_ARDQ4_OFFC_B0                         Fld(4, 16) //[19:16]
    #define B0_DQ0_RG_RX_ARDQ5_OFFC_B0                         Fld(4, 20) //[23:20]
    #define B0_DQ0_RG_RX_ARDQ6_OFFC_B0                         Fld(4, 24) //[27:24]
    #define B0_DQ0_RG_RX_ARDQ7_OFFC_B0                         Fld(4, 28) //[31:28]

#define DDRPHY_B0_DQ1                               (DDRPHY_AO_BASE_ADDRESS + 0x0009C)
    #define B0_DQ1_RG_RX_ARDQM0_OFFC_B0                        Fld(4, 0) //[3:0]

#define DDRPHY_B0_DQ2                               (DDRPHY_AO_BASE_ADDRESS + 0x000A0)
    #define B0_DQ2_RG_TX_ARDQS0_ODTEN_DIS_B0                   Fld(1, 16) //[16:16]
    #define B0_DQ2_RG_TX_ARDQS0_OE_DIS_B0                      Fld(1, 17) //[17:17]
    #define B0_DQ2_RG_TX_ARDQM0_ODTEN_DIS_B0                   Fld(1, 18) //[18:18]
    #define B0_DQ2_RG_TX_ARDQM0_OE_DIS_B0                      Fld(1, 19) //[19:19]
    #define B0_DQ2_RG_TX_ARDQ_ODTEN_DIS_B0                     Fld(1, 20) //[20:20]
    #define B0_DQ2_RG_TX_ARDQ_OE_DIS_B0                        Fld(1, 21) //[21:21]

#define DDRPHY_B0_DQ3                               (DDRPHY_AO_BASE_ADDRESS + 0x000A4)
    #define B0_DQ3_RG_ARDQ_ATPG_EN_B0                          Fld(1, 0) //[0:0]
    #define B0_DQ3_RG_RX_ARDQ_SMT_EN_B0                        Fld(1, 1) //[1:1]
    #define B0_DQ3_RG_TX_ARDQ_EN_B0                            Fld(1, 2) //[2:2]
    #define B0_DQ3_RG_ARDQ_RESETB_B0                           Fld(1, 3) //[3:3]
    #define B0_DQ3_RG_RX_ARDQS0_IN_BUFF_EN_B0                  Fld(1, 5) //[5:5]
    #define B0_DQ3_RG_RX_ARDQM0_IN_BUFF_EN_B0                  Fld(1, 6) //[6:6]
    #define B0_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B0                    Fld(1, 7) //[7:7]
    #define B0_DQ3_RG_RX_ARDQ_STBENCMP_EN_B0                   Fld(1, 10) //[10:10]
    #define B0_DQ3_RG_RX_ARDQ_OFFC_EN_B0                       Fld(1, 11) //[11:11]
    #define B0_DQ3_RG_RX_ARDQS0_SWAP_EN_B0                     Fld(1, 15) //[15:15]

#define DDRPHY_B0_DQ4                               (DDRPHY_AO_BASE_ADDRESS + 0x000A8)
    #define B0_DQ4_RG_RX_ARDQS_EYE_R_DLY_B0                    Fld(7, 0) //[6:0]
    #define B0_DQ4_RG_RX_ARDQS_EYE_F_DLY_B0                    Fld(7, 8) //[14:8]
    #define B0_DQ4_RG_RX_ARDQ_EYE_R_DLY_B0                     Fld(6, 16) //[21:16]
    #define B0_DQ4_RG_RX_ARDQ_EYE_F_DLY_B0                     Fld(6, 24) //[29:24]

#define DDRPHY_B0_DQ5                               (DDRPHY_AO_BASE_ADDRESS + 0x000AC)
    #define B0_DQ5_B0_DQ5_RFU                                  Fld(8, 0) //[7:0]
    #define B0_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B0                  Fld(6, 8) //[13:8]
    #define B0_DQ5_RG_RX_ARDQ_VREF_EN_B0                       Fld(1, 16) //[16:16]
    #define B0_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B0                   Fld(1, 17) //[17:17]
    #define B0_DQ5_RG_RX_ARDQ_EYE_SEL_B0                       Fld(4, 20) //[23:20]
    #define B0_DQ5_RG_RX_ARDQ_EYE_EN_B0                        Fld(1, 24) //[24:24]
    #define B0_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B0              Fld(1, 25) //[25:25]
    #define B0_DQ5_RG_RX_ARDQS0_DVS_EN_B0                      Fld(1, 31) //[31:31]

#define DDRPHY_B0_DQ6                               (DDRPHY_AO_BASE_ADDRESS + 0x000B0)
    #define B0_DQ6_RG_RX_ARDQ_BIAS_PS_B0                       Fld(2, 0) //[1:0]
    #define B0_DQ6_RG_TX_ARDQ_OE_EXT_DIS_B0                    Fld(1, 2) //[2:2]
    #define B0_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B0                 Fld(1, 3) //[3:3]
    #define B0_DQ6_RG_TX_ARDQ_SER_MODE_B0                      Fld(1, 4) //[4:4]
    #define B0_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B0                   Fld(1, 5) //[5:5]
    #define B0_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B0                   Fld(1, 6) //[6:6]
    #define B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0                 Fld(1, 7) //[7:7]
    #define B0_DQ6_RG_RX_ARDQ_LPBK_EN_B0                       Fld(1, 8) //[8:8]
    #define B0_DQ6_RG_RX_ARDQ_O1_SEL_B0                        Fld(1, 9) //[9:9]
    #define B0_DQ6_RG_RX_ARDQ_JM_SEL_B0                        Fld(1, 11) //[11:11]
    #define B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0                       Fld(1, 12) //[12:12]
    #define B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0                 Fld(2, 14) //[15:14]
    #define B0_DQ6_RG_RX_ARDQ_DDR4_SEL_B0                      Fld(1, 16) //[16:16]
    #define B0_DQ6_RG_TX_ARDQ_DDR4_SEL_B0                      Fld(1, 17) //[17:17]
    #define B0_DQ6_RG_RX_ARDQ_DDR3_SEL_B0                      Fld(1, 18) //[18:18]
    #define B0_DQ6_RG_TX_ARDQ_DDR3_SEL_B0                      Fld(1, 19) //[19:19]
    #define B0_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B0            Fld(1, 24) //[24:24]
    #define B0_DQ6_RG_RX_ARDQ_EYE_OE_GATE_EN_B0                Fld(1, 28) //[28:28]
    #define B0_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B0                 Fld(1, 31) //[31:31]

#define DDRPHY_B0_DQ7                               (DDRPHY_AO_BASE_ADDRESS + 0x000B4)
    #define B0_DQ7_RG_TX_ARDQS0B_PULL_DN_B0                    Fld(1, 0) //[0:0]
    #define B0_DQ7_RG_TX_ARDQS0B_PULL_UP_B0                    Fld(1, 1) //[1:1]
    #define B0_DQ7_RG_TX_ARDQS0_PULL_DN_B0                     Fld(1, 2) //[2:2]
    #define B0_DQ7_RG_TX_ARDQS0_PULL_UP_B0                     Fld(1, 3) //[3:3]
    #define B0_DQ7_RG_TX_ARDQM0_PULL_DN_B0                     Fld(1, 4) //[4:4]
    #define B0_DQ7_RG_TX_ARDQM0_PULL_UP_B0                     Fld(1, 5) //[5:5]
    #define B0_DQ7_RG_TX_ARDQ_PULL_DN_B0                       Fld(1, 6) //[6:6]
    #define B0_DQ7_RG_TX_ARDQ_PULL_UP_B0                       Fld(1, 7) //[7:7]
    #define B0_DQ7_RG_TX_ARDQS0B_PULL_DN_B0_LP4Y               Fld(1, 16) //[16:16]

#define DDRPHY_B0_DQ8                               (DDRPHY_AO_BASE_ADDRESS + 0x000B8)
    #define B0_DQ8_RG_TX_ARDQ_EN_LP4P_B0                       Fld(1, 0) //[0:0]
    #define B0_DQ8_RG_TX_ARDQ_EN_CAP_LP4P_B0                   Fld(1, 1) //[1:1]
    #define B0_DQ8_RG_TX_ARDQ_CAP_DET_B0                       Fld(1, 2) //[2:2]
    #define B0_DQ8_RG_TX_ARDQ_CKE_MCK4X_SEL_B0                 Fld(2, 3) //[4:3]
    #define B0_DQ8_RG_ARPI_TX_CG_DQ_EN_B0                      Fld(1, 5) //[5:5]
    #define B0_DQ8_RG_ARPI_TX_CG_DQM_EN_B0                     Fld(1, 6) //[6:6]
    #define B0_DQ8_RG_ARPI_TX_CG_DQS_EN_B0                     Fld(1, 7) //[7:7]
    #define B0_DQ8_RG_RX_ARDQS_BURST_E1_EN_B0                  Fld(1, 8) //[8:8]
    #define B0_DQ8_RG_RX_ARDQS_BURST_E2_EN_B0                  Fld(1, 9) //[9:9]
    #define B0_DQ8_RG_RX_ARDQS_DQSSTB_CG_EN_B0                 Fld(1, 10) //[10:10]
    #define B0_DQ8_RG_RX_ARDQS_GATE_EN_MODE_B0                 Fld(1, 12) //[12:12]
    #define B0_DQ8_RG_RX_ARDQS_SER_RST_MODE_B0                 Fld(1, 13) //[13:13]
    #define B0_DQ8_RG_ARDLL_RESETB_B0                          Fld(1, 15) //[15:15]

#define DDRPHY_B0_DQ9                               (DDRPHY_AO_BASE_ADDRESS + 0x000BC)
    #define B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0                  Fld(1, 0) //[0:0]
    #define B0_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B0                Fld(1, 4) //[4:4]
    #define B0_DQ9_RG_RX_ARDQS0_DQSIENMODE_B0                  Fld(1, 5) //[5:5]
    #define B0_DQ9_R_DMRXFIFO_STBENCMP_EN_B0                   Fld(1, 7) //[7:7]
    #define B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0                     Fld(8, 8) //[15:8]
    #define B0_DQ9_R_DMDQSIEN_VALID_LAT_B0                     Fld(3, 16) //[18:16]
    #define B0_DQ9_R_DMDQSIEN_RDSEL_LAT_B0                     Fld(3, 20) //[22:20]
    #define B0_DQ9_R_DMRXDVS_VALID_LAT_B0                      Fld(3, 24) //[26:24]
    #define B0_DQ9_R_DMRXDVS_RDSEL_LAT_B0                      Fld(3, 28) //[30:28]

#define DDRPHY_RFU_0X0C0                            (DDRPHY_AO_BASE_ADDRESS + 0x000C0)
    #define RFU_0X0C0_RESERVED_0X0C0                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X0C4                            (DDRPHY_AO_BASE_ADDRESS + 0x000C4)
    #define RFU_0X0C4_RESERVED_0X0C4                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X0C8                            (DDRPHY_AO_BASE_ADDRESS + 0x000C8)
    #define RFU_0X0C8_RESERVED_0X0C8                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X0CC                            (DDRPHY_AO_BASE_ADDRESS + 0x000CC)
    #define RFU_0X0CC_RESERVED_0X0CC                           Fld(32, 0) //[31:0]

#define DDRPHY_B0_TX_MCK                            (DDRPHY_AO_BASE_ADDRESS + 0x000D0)
    #define B0_TX_MCK_R_DM_TX_MCK_FRUN_B0                      Fld(10, 0) //[9:0]

#define DDRPHY_RFU_0X0D4                            (DDRPHY_AO_BASE_ADDRESS + 0x000D4)
    #define RFU_0X0D4_RESERVED_0X0D4                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X0D8                            (DDRPHY_AO_BASE_ADDRESS + 0x000D8)
    #define RFU_0X0D8_RESERVED_0X0D8                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X0DC                            (DDRPHY_AO_BASE_ADDRESS + 0x000DC)
    #define RFU_0X0DC_RESERVED_0X0DC                           Fld(32, 0) //[31:0]

#define DDRPHY_B1_DLL_ARPI0                         (DDRPHY_AO_BASE_ADDRESS + 0x00100)
    #define B1_DLL_ARPI0_RG_ARMCTLPLL_CK_SEL_B1                Fld(1, 1) //[1:1]
    #define B1_DLL_ARPI0_RG_ARPI_RESETB_B1                     Fld(1, 3) //[3:3]
    #define B1_DLL_ARPI0_RG_ARPI_LS_EN_B1                      Fld(1, 4) //[4:4]
    #define B1_DLL_ARPI0_RG_ARPI_LS_SEL_B1                     Fld(1, 5) //[5:5]
    #define B1_DLL_ARPI0_RG_ARPI_MCK8X_SEL_B1                  Fld(1, 6) //[6:6]

#define DDRPHY_B1_DLL_ARPI1                         (DDRPHY_AO_BASE_ADDRESS + 0x00104)
    #define B1_DLL_ARPI1_RG_ARPI_DQSIEN_JUMP_EN_B1             Fld(1, 11) //[11:11]
    #define B1_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B1                 Fld(1, 13) //[13:13]
    #define B1_DLL_ARPI1_RG_ARPI_DQM_JUMP_EN_B1                Fld(1, 14) //[14:14]
    #define B1_DLL_ARPI1_RG_ARPI_DQS_JUMP_EN_B1                Fld(1, 15) //[15:15]
    #define B1_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_B1                 Fld(1, 17) //[17:17]
    #define B1_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B1               Fld(1, 19) //[19:19]
    #define B1_DLL_ARPI1_RG_ARPISM_MCK_SEL_B1_REG_OPT          Fld(1, 20) //[20:20]
    #define B1_DLL_ARPI1_RG_ARPISM_MCK_SEL_B1                  Fld(1, 21) //[21:21]
    #define B1_DLL_ARPI1_RG_ARPI_SET_UPDN_B1                   Fld(3, 28) //[30:28]

#define DDRPHY_B1_DLL_ARPI2                         (DDRPHY_AO_BASE_ADDRESS + 0x00108)
    #define B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1                  Fld(1, 0) //[0:0]
    #define B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1                   Fld(1, 10) //[10:10]
    #define B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1                  Fld(1, 11) //[11:11]
    #define B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1                      Fld(1, 13) //[13:13]
    #define B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1                     Fld(1, 14) //[14:14]
    #define B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1                     Fld(1, 15) //[15:15]
    #define B1_DLL_ARPI2_RG_ARPI_CG_FB_B1                      Fld(1, 17) //[17:17]
    #define B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1                    Fld(1, 19) //[19:19]
    #define B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1              Fld(1, 27) //[27:27]
    #define B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1                     Fld(1, 31) //[31:31]

#define DDRPHY_B1_DLL_ARPI3                         (DDRPHY_AO_BASE_ADDRESS + 0x0010C)
    #define B1_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B1                  Fld(1, 11) //[11:11]
    #define B1_DLL_ARPI3_RG_ARPI_DQ_EN_B1                      Fld(1, 13) //[13:13]
    #define B1_DLL_ARPI3_RG_ARPI_DQM_EN_B1                     Fld(1, 14) //[14:14]
    #define B1_DLL_ARPI3_RG_ARPI_DQS_EN_B1                     Fld(1, 15) //[15:15]
    #define B1_DLL_ARPI3_RG_ARPI_FB_EN_B1                      Fld(1, 17) //[17:17]
    #define B1_DLL_ARPI3_RG_ARPI_MCTL_EN_B1                    Fld(1, 19) //[19:19]

#define DDRPHY_B1_DLL_ARPI4                         (DDRPHY_AO_BASE_ADDRESS + 0x00110)
    #define B1_DLL_ARPI4_RG_ARPI_BYPASS_DQSIEN_B1              Fld(1, 11) //[11:11]
    #define B1_DLL_ARPI4_RG_ARPI_BYPASS_DQ_B1                  Fld(1, 13) //[13:13]
    #define B1_DLL_ARPI4_RG_ARPI_BYPASS_DQM_B1                 Fld(1, 14) //[14:14]
    #define B1_DLL_ARPI4_RG_ARPI_BYPASS_DQS_B1                 Fld(1, 15) //[15:15]
    #define B1_DLL_ARPI4_RG_ARPI_BYPASS_FB_B1                  Fld(1, 17) //[17:17]
    #define B1_DLL_ARPI4_RG_ARPI_BYPASS_MCTL_B1                Fld(1, 19) //[19:19]

#define DDRPHY_B1_DLL_ARPI5                         (DDRPHY_AO_BASE_ADDRESS + 0x00114)
    #define B1_DLL_ARPI5_RG_ARDLL_DIV_MCTL_B1                  Fld(2, 2) //[3:2]
    #define B1_DLL_ARPI5_RG_ARDLL_MON_SEL_B1                   Fld(4, 4) //[7:4]
    #define B1_DLL_ARPI5_RG_ARDLL_DIV_DEC_B1                   Fld(1, 8) //[8:8]
    #define B1_DLL_ARPI5_B1_DLL_ARPI5_RFU                      Fld(12, 12) //[23:12]
    #define B1_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_SEL_B1           Fld(1, 25) //[25:25]
    #define B1_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_B1               Fld(1, 26) //[26:26]
    #define B1_DLL_ARPI5_B1_DLL_ARPI5_RFU1                     Fld(1, 31) //[31:31]

#define DDRPHY_B1_DQ0                               (DDRPHY_AO_BASE_ADDRESS + 0x00118)
    #define B1_DQ0_RG_RX_ARDQ0_OFFC_B1                         Fld(4, 0) //[3:0]
    #define B1_DQ0_RG_RX_ARDQ1_OFFC_B1                         Fld(4, 4) //[7:4]
    #define B1_DQ0_RG_RX_ARDQ2_OFFC_B1                         Fld(4, 8) //[11:8]
    #define B1_DQ0_RG_RX_ARDQ3_OFFC_B1                         Fld(4, 12) //[15:12]
    #define B1_DQ0_RG_RX_ARDQ4_OFFC_B1                         Fld(4, 16) //[19:16]
    #define B1_DQ0_RG_RX_ARDQ5_OFFC_B1                         Fld(4, 20) //[23:20]
    #define B1_DQ0_RG_RX_ARDQ6_OFFC_B1                         Fld(4, 24) //[27:24]
    #define B1_DQ0_RG_RX_ARDQ7_OFFC_B1                         Fld(4, 28) //[31:28]

#define DDRPHY_B1_DQ1                               (DDRPHY_AO_BASE_ADDRESS + 0x0011C)
    #define B1_DQ1_RG_RX_ARDQM0_OFFC_B1                        Fld(4, 0) //[3:0]

#define DDRPHY_B1_DQ2                               (DDRPHY_AO_BASE_ADDRESS + 0x00120)
    #define B1_DQ2_RG_TX_ARDQS0_ODTEN_DIS_B1                   Fld(1, 16) //[16:16]
    #define B1_DQ2_RG_TX_ARDQS0_OE_DIS_B1                      Fld(1, 17) //[17:17]
    #define B1_DQ2_RG_TX_ARDQM0_ODTEN_DIS_B1                   Fld(1, 18) //[18:18]
    #define B1_DQ2_RG_TX_ARDQM0_OE_DIS_B1                      Fld(1, 19) //[19:19]
    #define B1_DQ2_RG_TX_ARDQ_ODTEN_DIS_B1                     Fld(1, 20) //[20:20]
    #define B1_DQ2_RG_TX_ARDQ_OE_DIS_B1                        Fld(1, 21) //[21:21]

#define DDRPHY_B1_DQ3                               (DDRPHY_AO_BASE_ADDRESS + 0x00124)
    #define B1_DQ3_RG_ARDQ_ATPG_EN_B1                          Fld(1, 0) //[0:0]
    #define B1_DQ3_RG_RX_ARDQ_SMT_EN_B1                        Fld(1, 1) //[1:1]
    #define B1_DQ3_RG_TX_ARDQ_EN_B1                            Fld(1, 2) //[2:2]
    #define B1_DQ3_RG_ARDQ_RESETB_B1                           Fld(1, 3) //[3:3]
    #define B1_DQ3_RG_RX_ARDQS0_IN_BUFF_EN_B1                  Fld(1, 5) //[5:5]
    #define B1_DQ3_RG_RX_ARDQM0_IN_BUFF_EN_B1                  Fld(1, 6) //[6:6]
    #define B1_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B1                    Fld(1, 7) //[7:7]
    #define B1_DQ3_RG_RX_ARDQ_STBENCMP_EN_B1                   Fld(1, 10) //[10:10]
    #define B1_DQ3_RG_RX_ARDQ_OFFC_EN_B1                       Fld(1, 11) //[11:11]
    #define B1_DQ3_RG_RX_ARDQS0_SWAP_EN_B1                     Fld(1, 15) //[15:15]

#define DDRPHY_B1_DQ4                               (DDRPHY_AO_BASE_ADDRESS + 0x00128)
    #define B1_DQ4_RG_RX_ARDQS_EYE_R_DLY_B1                    Fld(7, 0) //[6:0]
    #define B1_DQ4_RG_RX_ARDQS_EYE_F_DLY_B1                    Fld(7, 8) //[14:8]
    #define B1_DQ4_RG_RX_ARDQ_EYE_R_DLY_B1                     Fld(6, 16) //[21:16]
    #define B1_DQ4_RG_RX_ARDQ_EYE_F_DLY_B1                     Fld(6, 24) //[29:24]

#define DDRPHY_B1_DQ5                               (DDRPHY_AO_BASE_ADDRESS + 0x0012C)
    #define B1_DQ5_B1_DQ5_RFU                                  Fld(8, 0) //[7:0]
    #define B1_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B1                  Fld(6, 8) //[13:8]
    #define B1_DQ5_RG_RX_ARDQ_VREF_EN_B1                       Fld(1, 16) //[16:16]
    #define B1_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B1                   Fld(1, 17) //[17:17]
    #define B1_DQ5_RG_RX_ARDQ_EYE_SEL_B1                       Fld(4, 20) //[23:20]
    #define B1_DQ5_RG_RX_ARDQ_EYE_EN_B1                        Fld(1, 24) //[24:24]
    #define B1_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B1              Fld(1, 25) //[25:25]
    #define B1_DQ5_RG_RX_ARDQS0_DVS_EN_B1                      Fld(1, 31) //[31:31]

#define DDRPHY_B1_DQ6                               (DDRPHY_AO_BASE_ADDRESS + 0x00130)
    #define B1_DQ6_RG_RX_ARDQ_BIAS_PS_B1                       Fld(2, 0) //[1:0]
    #define B1_DQ6_RG_TX_ARDQ_OE_EXT_DIS_B1                    Fld(1, 2) //[2:2]
    #define B1_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B1                 Fld(1, 3) //[3:3]
    #define B1_DQ6_RG_TX_ARDQ_SER_MODE_B1                      Fld(1, 4) //[4:4]
    #define B1_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B1                   Fld(1, 5) //[5:5]
    #define B1_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B1                   Fld(1, 6) //[6:6]
    #define B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1                 Fld(1, 7) //[7:7]
    #define B1_DQ6_RG_RX_ARDQ_LPBK_EN_B1                       Fld(1, 8) //[8:8]
    #define B1_DQ6_RG_RX_ARDQ_O1_SEL_B1                        Fld(1, 9) //[9:9]
    #define B1_DQ6_RG_RX_ARDQ_JM_SEL_B1                        Fld(1, 11) //[11:11]
    #define B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1                       Fld(1, 12) //[12:12]
    #define B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1                 Fld(2, 14) //[15:14]
    #define B1_DQ6_RG_RX_ARDQ_DDR4_SEL_B1                      Fld(1, 16) //[16:16]
    #define B1_DQ6_RG_TX_ARDQ_DDR4_SEL_B1                      Fld(1, 17) //[17:17]
    #define B1_DQ6_RG_RX_ARDQ_DDR3_SEL_B1                      Fld(1, 18) //[18:18]
    #define B1_DQ6_RG_TX_ARDQ_DDR3_SEL_B1                      Fld(1, 19) //[19:19]
    #define B1_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B1            Fld(1, 24) //[24:24]
    #define B1_DQ6_RG_RX_ARDQ_EYE_OE_GATE_EN_B1                Fld(1, 28) //[28:28]
    #define B1_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B1                 Fld(1, 31) //[31:31]

#define DDRPHY_B1_DQ7                               (DDRPHY_AO_BASE_ADDRESS + 0x00134)
    #define B1_DQ7_RG_TX_ARDQS0B_PULL_DN_B1                    Fld(1, 0) //[0:0]
    #define B1_DQ7_RG_TX_ARDQS0B_PULL_UP_B1                    Fld(1, 1) //[1:1]
    #define B1_DQ7_RG_TX_ARDQS0_PULL_DN_B1                     Fld(1, 2) //[2:2]
    #define B1_DQ7_RG_TX_ARDQS0_PULL_UP_B1                     Fld(1, 3) //[3:3]
    #define B1_DQ7_RG_TX_ARDQM0_PULL_DN_B1                     Fld(1, 4) //[4:4]
    #define B1_DQ7_RG_TX_ARDQM0_PULL_UP_B1                     Fld(1, 5) //[5:5]
    #define B1_DQ7_RG_TX_ARDQ_PULL_DN_B1                       Fld(1, 6) //[6:6]
    #define B1_DQ7_RG_TX_ARDQ_PULL_UP_B1                       Fld(1, 7) //[7:7]
    #define B1_DQ7_RG_TX_ARDQS0B_PULL_DN_B1_LP4Y               Fld(1, 16) //[16:16]

#define DDRPHY_B1_DQ8                               (DDRPHY_AO_BASE_ADDRESS + 0x00138)
    #define B1_DQ8_RG_TX_ARDQ_EN_LP4P_B1                       Fld(1, 0) //[0:0]
    #define B1_DQ8_RG_TX_ARDQ_EN_CAP_LP4P_B1                   Fld(1, 1) //[1:1]
    #define B1_DQ8_RG_TX_ARDQ_CAP_DET_B1                       Fld(1, 2) //[2:2]
    #define B1_DQ8_RG_TX_ARDQ_CKE_MCK4X_SEL_B1                 Fld(2, 3) //[4:3]
    #define B1_DQ8_RG_ARPI_TX_CG_DQ_EN_B1                      Fld(1, 5) //[5:5]
    #define B1_DQ8_RG_ARPI_TX_CG_DQM_EN_B1                     Fld(1, 6) //[6:6]
    #define B1_DQ8_RG_ARPI_TX_CG_DQS_EN_B1                     Fld(1, 7) //[7:7]
    #define B1_DQ8_RG_RX_ARDQS_BURST_E1_EN_B1                  Fld(1, 8) //[8:8]
    #define B1_DQ8_RG_RX_ARDQS_BURST_E2_EN_B1                  Fld(1, 9) //[9:9]
    #define B1_DQ8_RG_RX_ARDQS_DQSSTB_CG_EN_B1                 Fld(1, 10) //[10:10]
    #define B1_DQ8_RG_RX_ARDQS_GATE_EN_MODE_B1                 Fld(1, 12) //[12:12]
    #define B1_DQ8_RG_RX_ARDQS_SER_RST_MODE_B1                 Fld(1, 13) //[13:13]
    #define B1_DQ8_RG_ARDLL_RESETB_B1                          Fld(1, 15) //[15:15]

#define DDRPHY_B1_DQ9                               (DDRPHY_AO_BASE_ADDRESS + 0x0013C)
    #define B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1                  Fld(1, 0) //[0:0]
    #define B1_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B1                Fld(1, 4) //[4:4]
    #define B1_DQ9_RG_RX_ARDQS0_DQSIENMODE_B1                  Fld(1, 5) //[5:5]
    #define B1_DQ9_R_DMRXFIFO_STBENCMP_EN_B1                   Fld(1, 7) //[7:7]
    #define B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1                     Fld(8, 8) //[15:8]
    #define B1_DQ9_R_DMDQSIEN_VALID_LAT_B1                     Fld(3, 16) //[18:16]
    #define B1_DQ9_R_DMDQSIEN_RDSEL_LAT_B1                     Fld(3, 20) //[22:20]
    #define B1_DQ9_R_DMRXDVS_VALID_LAT_B1                      Fld(3, 24) //[26:24]
    #define B1_DQ9_R_DMRXDVS_RDSEL_LAT_B1                      Fld(3, 28) //[30:28]

#define DDRPHY_RFU_0X140                            (DDRPHY_AO_BASE_ADDRESS + 0x00140)
    #define RFU_0X140_RESERVED_0X140                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X144                            (DDRPHY_AO_BASE_ADDRESS + 0x00144)
    #define RFU_0X144_RESERVED_0X144                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X148                            (DDRPHY_AO_BASE_ADDRESS + 0x00148)
    #define RFU_0X148_RESERVED_0X148                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X14C                            (DDRPHY_AO_BASE_ADDRESS + 0x0014C)
    #define RFU_0X14C_RESERVED_0X14C                           Fld(32, 0) //[31:0]

#define DDRPHY_B1_TX_MCK                            (DDRPHY_AO_BASE_ADDRESS + 0x00150)
    #define B1_TX_MCK_R_DM_TX_MCK_FRUN_B1                      Fld(10, 0) //[9:0]

#define DDRPHY_RFU_0X154                            (DDRPHY_AO_BASE_ADDRESS + 0x00154)
    #define RFU_0X154_RESERVED_0X154                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X158                            (DDRPHY_AO_BASE_ADDRESS + 0x00158)
    #define RFU_0X158_RESERVED_0X158                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X15C                            (DDRPHY_AO_BASE_ADDRESS + 0x0015C)
    #define RFU_0X15C_RESERVED_0X15C                           Fld(32, 0) //[31:0]

#define DDRPHY_CA_DLL_ARPI0                         (DDRPHY_AO_BASE_ADDRESS + 0x00180)
    #define CA_DLL_ARPI0_RG_ARMCTLPLL_CK_SEL_CA                Fld(1, 1) //[1:1]
    #define CA_DLL_ARPI0_RG_ARPI_RESETB_CA                     Fld(1, 3) //[3:3]
    #define CA_DLL_ARPI0_RG_ARPI_LS_EN_CA                      Fld(1, 4) //[4:4]
    #define CA_DLL_ARPI0_RG_ARPI_LS_SEL_CA                     Fld(1, 5) //[5:5]
    #define CA_DLL_ARPI0_RG_ARPI_MCK8X_SEL_CA                  Fld(1, 6) //[6:6]

#define DDRPHY_CA_DLL_ARPI1                         (DDRPHY_AO_BASE_ADDRESS + 0x00184)
    #define CA_DLL_ARPI1_RG_ARPI_CLKIEN_JUMP_EN                Fld(1, 11) //[11:11]
    #define CA_DLL_ARPI1_RG_ARPI_CMD_JUMP_EN                   Fld(1, 13) //[13:13]
    #define CA_DLL_ARPI1_RG_ARPI_CLK_JUMP_EN                   Fld(1, 15) //[15:15]
    #define CA_DLL_ARPI1_RG_ARPI_CS_JUMP_EN                    Fld(1, 16) //[16:16]
    #define CA_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_CA                 Fld(1, 17) //[17:17]
    #define CA_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_CA               Fld(1, 19) //[19:19]
    #define CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA_REG_OPT          Fld(1, 20) //[20:20]
    #define CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA                  Fld(1, 21) //[21:21]
    #define CA_DLL_ARPI1_RG_ARPI_SET_UPDN_CA                   Fld(3, 28) //[30:28]

#define DDRPHY_CA_DLL_ARPI2                         (DDRPHY_AO_BASE_ADDRESS + 0x00188)
    #define CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA                  Fld(1, 0) //[0:0]
    #define CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA                   Fld(1, 10) //[10:10]
    #define CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN                     Fld(1, 11) //[11:11]
    #define CA_DLL_ARPI2_RG_ARPI_CG_CMD                        Fld(1, 13) //[13:13]
    #define CA_DLL_ARPI2_RG_ARPI_CG_CLK                        Fld(1, 15) //[15:15]
    #define CA_DLL_ARPI2_RG_ARPI_CG_CS                         Fld(1, 16) //[16:16]
    #define CA_DLL_ARPI2_RG_ARPI_CG_FB_CA                      Fld(1, 17) //[17:17]
    #define CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA                    Fld(1, 19) //[19:19]
    #define CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA              Fld(1, 27) //[27:27]
    #define CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA                     Fld(1, 31) //[31:31]

#define DDRPHY_CA_DLL_ARPI3                         (DDRPHY_AO_BASE_ADDRESS + 0x0018C)
    #define CA_DLL_ARPI3_RG_ARPI_CLKIEN_EN                     Fld(1, 11) //[11:11]
    #define CA_DLL_ARPI3_RG_ARPI_CMD_EN                        Fld(1, 13) //[13:13]
    #define CA_DLL_ARPI3_RG_ARPI_CLK_EN                        Fld(1, 15) //[15:15]
    #define CA_DLL_ARPI3_RG_ARPI_CS_EN                         Fld(1, 16) //[16:16]
    #define CA_DLL_ARPI3_RG_ARPI_FB_EN_CA                      Fld(1, 17) //[17:17]
    #define CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA                    Fld(1, 19) //[19:19]

#define DDRPHY_CA_DLL_ARPI4                         (DDRPHY_AO_BASE_ADDRESS + 0x00190)
    #define CA_DLL_ARPI4_RG_ARPI_BYPASS_CLKIEN                 Fld(1, 11) //[11:11]
    #define CA_DLL_ARPI4_RG_ARPI_BYPASS_CMD                    Fld(1, 13) //[13:13]
    #define CA_DLL_ARPI4_RG_ARPI_BYPASS_CLK                    Fld(1, 15) //[15:15]
    #define CA_DLL_ARPI4_RG_ARPI_BYPASS_CS                     Fld(1, 16) //[16:16]
    #define CA_DLL_ARPI4_RG_ARPI_BYPASS_FB_CA                  Fld(1, 17) //[17:17]
    #define CA_DLL_ARPI4_RG_ARPI_BYPASS_MCTL_CA                Fld(1, 19) //[19:19]

#define DDRPHY_CA_DLL_ARPI5                         (DDRPHY_AO_BASE_ADDRESS + 0x00194)
    #define CA_DLL_ARPI5_RG_ARDLL_DIV_MCTL_CA                  Fld(2, 2) //[3:2]
    #define CA_DLL_ARPI5_RG_ARDLL_MON_SEL_CA                   Fld(4, 4) //[7:4]
    #define CA_DLL_ARPI5_RG_ARDLL_DIV_DEC_CA                   Fld(1, 8) //[8:8]
    #define CA_DLL_ARPI5_CA_DLL_ARPI5_RFU                      Fld(12, 12) //[23:12]
    #define CA_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_SEL_CA           Fld(1, 25) //[25:25]
    #define CA_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_CA               Fld(1, 26) //[26:26]

#define DDRPHY_CA_CMD0                              (DDRPHY_AO_BASE_ADDRESS + 0x00198)
    #define CA_CMD0_RG_RX_ARCA0_OFFC                           Fld(4, 0) //[3:0]
    #define CA_CMD0_RG_RX_ARCA1_OFFC                           Fld(4, 4) //[7:4]
    #define CA_CMD0_RG_RX_ARCA2_OFFC                           Fld(4, 8) //[11:8]
    #define CA_CMD0_RG_RX_ARCA3_OFFC                           Fld(4, 12) //[15:12]
    #define CA_CMD0_RG_RX_ARCA4_OFFC                           Fld(4, 16) //[19:16]
    #define CA_CMD0_RG_RX_ARCA5_OFFC                           Fld(4, 20) //[23:20]

#define DDRPHY_CA_CMD1                              (DDRPHY_AO_BASE_ADDRESS + 0x0019C)
    #define CA_CMD1_RG_RX_ARCS0_OFFC                           Fld(4, 0) //[3:0]
    #define CA_CMD1_RG_RX_ARCS1_OFFC                           Fld(4, 4) //[7:4]
    #define CA_CMD1_RG_RX_ARCS2_OFFC                           Fld(4, 8) //[11:8]
    #define CA_CMD1_RG_RX_ARCKE0_OFFC                          Fld(4, 12) //[15:12]
    #define CA_CMD1_RG_RX_ARCKE1_OFFC                          Fld(4, 16) //[19:16]
    #define CA_CMD1_RG_RX_ARCKE2_OFFC                          Fld(4, 20) //[23:20]

#define DDRPHY_CA_CMD2                              (DDRPHY_AO_BASE_ADDRESS + 0x001A0)
    #define CA_CMD2_RG_TX_ARCLK_ODTEN_DIS                      Fld(1, 16) //[16:16]
    #define CA_CMD2_RG_TX_ARCLK_OE_DIS                         Fld(1, 17) //[17:17]
    #define CA_CMD2_RG_TX_ARCMD_ODTEN_DIS                      Fld(1, 20) //[20:20]
    #define CA_CMD2_RG_TX_ARCMD_OE_DIS                         Fld(1, 21) //[21:21]

#define DDRPHY_CA_CMD3                              (DDRPHY_AO_BASE_ADDRESS + 0x001A4)
    #define CA_CMD3_RG_ARCMD_ATPG_EN                           Fld(1, 0) //[0:0]
    #define CA_CMD3_RG_RX_ARCMD_SMT_EN                         Fld(1, 1) //[1:1]
    #define CA_CMD3_RG_TX_ARCMD_EN                             Fld(1, 2) //[2:2]
    #define CA_CMD3_RG_ARCMD_RESETB                            Fld(1, 3) //[3:3]
    #define CA_CMD3_RG_RX_ARCLK_IN_BUFF_EN                     Fld(1, 5) //[5:5]
    #define CA_CMD3_RG_RX_ARCMD_IN_BUFF_EN                     Fld(1, 7) //[7:7]
    #define CA_CMD3_RG_RX_ARCMD_STBENCMP_EN                    Fld(1, 10) //[10:10]
    #define CA_CMD3_RG_RX_ARCMD_OFFC_EN                        Fld(1, 11) //[11:11]
    #define CA_CMD3_RG_RX_ARCLK_SWAP_EN                        Fld(1, 15) //[15:15]

#define DDRPHY_CA_CMD4                              (DDRPHY_AO_BASE_ADDRESS + 0x001A8)
    #define CA_CMD4_RG_RX_ARCLK_EYE_R_DLY                      Fld(7, 0) //[6:0]
    #define CA_CMD4_RG_RX_ARCLK_EYE_F_DLY                      Fld(7, 8) //[14:8]
    #define CA_CMD4_RG_RX_ARCMD_EYE_R_DLY                      Fld(6, 16) //[21:16]
    #define CA_CMD4_RG_RX_ARCMD_EYE_F_DLY                      Fld(6, 24) //[29:24]

#define DDRPHY_CA_CMD5                              (DDRPHY_AO_BASE_ADDRESS + 0x001AC)
    #define CA_CMD5_CA_CMD5_RFU                                Fld(8, 0) //[7:0]
    #define CA_CMD5_RG_RX_ARCMD_EYE_VREF_SEL                   Fld(6, 8) //[13:8]
    #define CA_CMD5_RG_RX_ARCMD_VREF_EN                        Fld(1, 16) //[16:16]
    #define CA_CMD5_RG_RX_ARCMD_EYE_VREF_EN                    Fld(1, 17) //[17:17]
    #define CA_CMD5_RG_RX_ARCMD_EYE_SEL                        Fld(4, 20) //[23:20]
    #define CA_CMD5_RG_RX_ARCMD_EYE_EN                         Fld(1, 24) //[24:24]
    #define CA_CMD5_RG_RX_ARCMD_EYE_STBEN_RESETB               Fld(1, 25) //[25:25]
    #define CA_CMD5_RG_RX_ARCLK_DVS_EN                         Fld(1, 31) //[31:31]

#define DDRPHY_CA_CMD6                              (DDRPHY_AO_BASE_ADDRESS + 0x001B0)
    #define CA_CMD6_RG_RX_ARCMD_BIAS_PS                        Fld(2, 0) //[1:0]
    #define CA_CMD6_RG_TX_ARCMD_OE_EXT_DIS                     Fld(1, 2) //[2:2]
    #define CA_CMD6_RG_TX_ARCMD_ODTEN_EXT_DIS                  Fld(1, 3) //[3:3]
    #define CA_CMD6_RG_TX_ARCMD_SER_MODE                       Fld(1, 4) //[4:4]
    #define CA_CMD6_RG_RX_ARCMD_RPRE_TOG_EN                    Fld(1, 5) //[5:5]
    #define CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN                    Fld(1, 6) //[6:6]
    #define CA_CMD6_RG_RX_ARCMD_OP_BIAS_SW_EN                  Fld(1, 7) //[7:7]
    #define CA_CMD6_RG_RX_ARCMD_LPBK_EN                        Fld(1, 8) //[8:8]
    #define CA_CMD6_RG_RX_ARCMD_O1_SEL                         Fld(1, 9) //[9:9]
    #define CA_CMD6_RG_RX_ARCMD_JM_SEL                         Fld(1, 11) //[11:11]
    #define CA_CMD6_RG_RX_ARCMD_BIAS_EN                        Fld(1, 12) //[12:12]
    #define CA_CMD6_RG_RX_ARCMD_BIAS_VREF_SEL                  Fld(2, 14) //[15:14]
    #define CA_CMD6_RG_RX_ARCMD_DDR4_SEL                       Fld(1, 16) //[16:16]
    #define CA_CMD6_RG_TX_ARCMD_DDR4_SEL                       Fld(1, 17) //[17:17]
    #define CA_CMD6_RG_RX_ARCMD_DDR3_SEL                       Fld(1, 18) //[18:18]
    #define CA_CMD6_RG_TX_ARCMD_DDR3_SEL                       Fld(1, 19) //[19:19]
    #define CA_CMD6_RG_RX_ARCMD_EYE_DLY_DQS_BYPASS             Fld(1, 24) //[24:24]
    #define CA_CMD6_RG_RX_ARCMD_EYE_OE_GATE_EN                 Fld(1, 28) //[28:28]
    #define CA_CMD6_RG_RX_ARCMD_DMRANK_OUTSEL                  Fld(1, 31) //[31:31]

#define DDRPHY_CA_CMD7                              (DDRPHY_AO_BASE_ADDRESS + 0x001B4)
    #define CA_CMD7_RG_TX_ARCLKB_PULL_DN                       Fld(1, 0) //[0:0]
    #define CA_CMD7_RG_TX_ARCLKB_PULL_UP                       Fld(1, 1) //[1:1]
    #define CA_CMD7_RG_TX_ARCLK_PULL_DN                        Fld(1, 2) //[2:2]
    #define CA_CMD7_RG_TX_ARCLK_PULL_UP                        Fld(1, 3) //[3:3]
    #define CA_CMD7_RG_TX_ARCS_PULL_DN                         Fld(1, 4) //[4:4]
    #define CA_CMD7_RG_TX_ARCS_PULL_UP                         Fld(1, 5) //[5:5]
    #define CA_CMD7_RG_TX_ARCMD_PULL_DN                        Fld(1, 6) //[6:6]
    #define CA_CMD7_RG_TX_ARCMD_PULL_UP                        Fld(1, 7) //[7:7]
    #define CA_CMD7_RG_TX_ARCLKB_PULL_DN_LP4Y                  Fld(1, 16) //[16:16]

#define DDRPHY_CA_CMD8                              (DDRPHY_AO_BASE_ADDRESS + 0x001B8)
    #define CA_CMD8_RG_RRESETB_DRVP                            Fld(5, 0) //[4:0]
    #define CA_CMD8_RG_RRESETB_DRVN                            Fld(5, 8) //[12:8]
    #define CA_CMD8_RG_RX_RRESETB_SMT_EN                       Fld(1, 16) //[16:16]
    #define CA_CMD8_RG_TX_RRESETB_SCAN_IN_EN                   Fld(1, 17) //[17:17]
    #define CA_CMD8_RG_TX_RRESETB_DDR4_SEL                     Fld(1, 18) //[18:18]
    #define CA_CMD8_RG_TX_RRESETB_DDR3_SEL                     Fld(1, 19) //[19:19]
    #define CA_CMD8_RG_TX_RRESETB_PULL_DN                      Fld(1, 20) //[20:20]
    #define CA_CMD8_RG_TX_RRESETB_PULL_UP                      Fld(1, 21) //[21:21]

#define DDRPHY_CA_CMD9                              (DDRPHY_AO_BASE_ADDRESS + 0x001BC)
    #define CA_CMD9_RG_TX_ARCMD_EN_LP4P                        Fld(1, 0) //[0:0]
    #define CA_CMD9_RG_TX_ARCMD_EN_CAP_LP4P                    Fld(1, 1) //[1:1]
    #define CA_CMD9_RG_TX_ARCMD_CAP_DET                        Fld(1, 2) //[2:2]
    #define CA_CMD9_RG_TX_ARCMD_CKE_MCK4X_SEL                  Fld(2, 3) //[4:3]
    #define CA_CMD9_RG_ARPI_TX_CG_CS_EN                        Fld(1, 5) //[5:5]
    #define CA_CMD9_RG_ARPI_TX_CG_CA_EN                        Fld(1, 6) //[6:6]
    #define CA_CMD9_RG_ARPI_TX_CG_CLK_EN                       Fld(1, 7) //[7:7]
    #define CA_CMD9_RG_RX_ARCLK_DQSIEN_BURST_E1_EN             Fld(1, 8) //[8:8]
    #define CA_CMD9_RG_RX_ARCLK_DQSIEN_BURST_E2_EN             Fld(1, 9) //[9:9]
    #define CA_CMD9_RG_RX_ARCLK_DQSSTB_CG_EN                   Fld(1, 10) //[10:10]
    #define CA_CMD9_RG_RX_ARCLK_GATE_EN_MODE                   Fld(1, 12) //[12:12]
    #define CA_CMD9_RG_RX_ARCLK_SER_RST_MODE                   Fld(1, 13) //[13:13]
    #define CA_CMD9_RG_ARDLL_RESETB_CA                         Fld(1, 15) //[15:15]
    #define CA_CMD9_RG_TX_ARCMD_LP3_CKE_SEL                    Fld(1, 16) //[16:16]
    #define CA_CMD9_RG_TX_ARCMD_LP4_CKE_SEL                    Fld(1, 17) //[17:17]
    #define CA_CMD9_RG_TX_ARCMD_LP4X_CKE_SEL                   Fld(1, 18) //[18:18]
    #define CA_CMD9_RG_TX_ARCMD_LSH_DQM_CG_EN                  Fld(1, 20) //[20:20]
    #define CA_CMD9_RG_TX_ARCMD_LSH_DQS_CG_EN                  Fld(1, 21) //[21:21]
    #define CA_CMD9_RG_TX_ARCMD_LSH_DQ_CG_EN                   Fld(1, 22) //[22:22]
    #define CA_CMD9_RG_TX_ARCMD_OE_SUS_EN                      Fld(1, 24) //[24:24]
    #define CA_CMD9_RG_TX_ARCMD_ODTEN_OE_SUS_EN                Fld(1, 25) //[25:25]

#define DDRPHY_CA_CMD10                             (DDRPHY_AO_BASE_ADDRESS + 0x001C0)
    #define CA_CMD10_RG_RX_ARCMD_STBEN_RESETB                  Fld(1, 0) //[0:0]
    #define CA_CMD10_RG_RX_ARCLK_STBEN_RESETB                  Fld(1, 4) //[4:4]
    #define CA_CMD10_RG_RX_ARCLK_DQSIENMODE                    Fld(1, 5) //[5:5]
    #define CA_CMD10_R_DMRXFIFO_STBENCMP_EN_CA                 Fld(1, 7) //[7:7]
    #define CA_CMD10_R_IN_GATE_EN_LOW_OPT_CA                   Fld(8, 8) //[15:8]
    #define CA_CMD10_R_DMDQSIEN_VALID_LAT_CA                   Fld(3, 16) //[18:16]
    #define CA_CMD10_R_DMDQSIEN_RDSEL_LAT_CA                   Fld(3, 20) //[22:20]
    #define CA_CMD10_R_DMRXDVS_VALID_LAT_CA                    Fld(3, 24) //[26:24]
    #define CA_CMD10_R_DMRXDVS_RDSEL_LAT_CA                    Fld(3, 28) //[30:28]

#define DDRPHY_RFU_0X1C4                            (DDRPHY_AO_BASE_ADDRESS + 0x001C4)
    #define RFU_0X1C4_RESERVED_0X1C4                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1C8                            (DDRPHY_AO_BASE_ADDRESS + 0x001C8)
    #define RFU_0X1C8_RESERVED_0X1C8                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1CC                            (DDRPHY_AO_BASE_ADDRESS + 0x001CC)
    #define RFU_0X1CC_RESERVED_0X1CC                           Fld(32, 0) //[31:0]

#define DDRPHY_CA_TX_MCK                            (DDRPHY_AO_BASE_ADDRESS + 0x001D0)
    #define CA_TX_MCK_R_DM_TX_MCK_FRUN_CA                      Fld(13, 0) //[12:0]
    #define CA_TX_MCK_R_DMRESETB_DRVP_FRPHY                    Fld(5, 21) //[25:21]
    #define CA_TX_MCK_R_DMRESETB_DRVN_FRPHY                    Fld(5, 26) //[30:26]
    #define CA_TX_MCK_R_DMRESET_FRPHY_OPT                      Fld(1, 31) //[31:31]

#define DDRPHY_RFU_0X1D4                            (DDRPHY_AO_BASE_ADDRESS + 0x001D4)
    #define RFU_0X1D4_RESERVED_0X1D4                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1D8                            (DDRPHY_AO_BASE_ADDRESS + 0x001D8)
    #define RFU_0X1D8_RESERVED_0X1D8                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1DC                            (DDRPHY_AO_BASE_ADDRESS + 0x001DC)
    #define RFU_0X1DC_RESERVED_0X1DC                           Fld(32, 0) //[31:0]

#define DDRPHY_MISC_EXTLB0                          (DDRPHY_AO_BASE_ADDRESS + 0x00200)
    #define MISC_EXTLB0_R_EXTLB_LFSR_INI_1                     Fld(16, 16) //[31:16]
    #define MISC_EXTLB0_R_EXTLB_LFSR_INI_0                     Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB1                          (DDRPHY_AO_BASE_ADDRESS + 0x00204)
    #define MISC_EXTLB1_R_EXTLB_LFSR_INI_3                     Fld(16, 16) //[31:16]
    #define MISC_EXTLB1_R_EXTLB_LFSR_INI_2                     Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB2                          (DDRPHY_AO_BASE_ADDRESS + 0x00208)
    #define MISC_EXTLB2_R_EXTLB_LFSR_INI_5                     Fld(16, 16) //[31:16]
    #define MISC_EXTLB2_R_EXTLB_LFSR_INI_4                     Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB3                          (DDRPHY_AO_BASE_ADDRESS + 0x0020C)
    #define MISC_EXTLB3_R_EXTLB_LFSR_INI_7                     Fld(16, 16) //[31:16]
    #define MISC_EXTLB3_R_EXTLB_LFSR_INI_6                     Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB4                          (DDRPHY_AO_BASE_ADDRESS + 0x00210)
    #define MISC_EXTLB4_R_EXTLB_LFSR_INI_9                     Fld(16, 16) //[31:16]
    #define MISC_EXTLB4_R_EXTLB_LFSR_INI_8                     Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB5                          (DDRPHY_AO_BASE_ADDRESS + 0x00214)
    #define MISC_EXTLB5_R_EXTLB_LFSR_INI_11                    Fld(16, 16) //[31:16]
    #define MISC_EXTLB5_R_EXTLB_LFSR_INI_10                    Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB6                          (DDRPHY_AO_BASE_ADDRESS + 0x00218)
    #define MISC_EXTLB6_R_EXTLB_LFSR_INI_13                    Fld(16, 16) //[31:16]
    #define MISC_EXTLB6_R_EXTLB_LFSR_INI_12                    Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB7                          (DDRPHY_AO_BASE_ADDRESS + 0x0021C)
    #define MISC_EXTLB7_R_EXTLB_LFSR_INI_15                    Fld(16, 16) //[31:16]
    #define MISC_EXTLB7_R_EXTLB_LFSR_INI_14                    Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB8                          (DDRPHY_AO_BASE_ADDRESS + 0x00220)
    #define MISC_EXTLB8_R_EXTLB_LFSR_INI_17                    Fld(16, 16) //[31:16]
    #define MISC_EXTLB8_R_EXTLB_LFSR_INI_16                    Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB9                          (DDRPHY_AO_BASE_ADDRESS + 0x00224)
    #define MISC_EXTLB9_R_EXTLB_LFSR_INI_19                    Fld(16, 16) //[31:16]
    #define MISC_EXTLB9_R_EXTLB_LFSR_INI_18                    Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB10                         (DDRPHY_AO_BASE_ADDRESS + 0x00228)
    #define MISC_EXTLB10_R_EXTLB_LFSR_INI_21                   Fld(16, 16) //[31:16]
    #define MISC_EXTLB10_R_EXTLB_LFSR_INI_20                   Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB11                         (DDRPHY_AO_BASE_ADDRESS + 0x0022C)
    #define MISC_EXTLB11_R_EXTLB_LFSR_INI_23                   Fld(16, 16) //[31:16]
    #define MISC_EXTLB11_R_EXTLB_LFSR_INI_22                   Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB12                         (DDRPHY_AO_BASE_ADDRESS + 0x00230)
    #define MISC_EXTLB12_R_EXTLB_LFSR_INI_25                   Fld(16, 16) //[31:16]
    #define MISC_EXTLB12_R_EXTLB_LFSR_INI_24                   Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB13                         (DDRPHY_AO_BASE_ADDRESS + 0x00234)
    #define MISC_EXTLB13_R_EXTLB_LFSR_INI_27                   Fld(16, 16) //[31:16]
    #define MISC_EXTLB13_R_EXTLB_LFSR_INI_26                   Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB14                         (DDRPHY_AO_BASE_ADDRESS + 0x00238)
    #define MISC_EXTLB14_R_EXTLB_LFSR_INI_29                   Fld(16, 16) //[31:16]
    #define MISC_EXTLB14_R_EXTLB_LFSR_INI_28                   Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB15                         (DDRPHY_AO_BASE_ADDRESS + 0x0023C)
    #define MISC_EXTLB15_R_EXTLB_LFSR_INI_30                   Fld(16, 0) //[15:0]
    #define MISC_EXTLB15_MISC_EXTLB15_RFU                      Fld(16, 16) //[31:16]

#define DDRPHY_MISC_EXTLB16                         (DDRPHY_AO_BASE_ADDRESS + 0x00240)
    #define MISC_EXTLB16_R_EXTLB_ODTEN_DQS1_ON                 Fld(1, 27) //[27:27]
    #define MISC_EXTLB16_R_EXTLB_ODTEN_DQM1_ON                 Fld(1, 26) //[26:26]
    #define MISC_EXTLB16_R_EXTLB_ODTEN_DQB1_ON                 Fld(1, 25) //[25:25]
    #define MISC_EXTLB16_R_EXTLB_ODTEN_DQS0_ON                 Fld(1, 24) //[24:24]
    #define MISC_EXTLB16_R_EXTLB_ODTEN_DQM0_ON                 Fld(1, 23) //[23:23]
    #define MISC_EXTLB16_R_EXTLB_ODTEN_DQB0_ON                 Fld(1, 22) //[22:22]
    #define MISC_EXTLB16_R_EXTLB_OE_DQS1_ON                    Fld(1, 21) //[21:21]
    #define MISC_EXTLB16_R_EXTLB_OE_DQM1_ON                    Fld(1, 20) //[20:20]
    #define MISC_EXTLB16_R_EXTLB_OE_DQB1_ON                    Fld(1, 19) //[19:19]
    #define MISC_EXTLB16_R_EXTLB_OE_DQS0_ON                    Fld(1, 18) //[18:18]
    #define MISC_EXTLB16_R_EXTLB_OE_DQM0_ON                    Fld(1, 17) //[17:17]
    #define MISC_EXTLB16_R_EXTLB_OE_DQB0_ON                    Fld(1, 16) //[16:16]
    #define MISC_EXTLB16_R_EXTLB_LFSR_TAP                      Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB17                         (DDRPHY_AO_BASE_ADDRESS + 0x00244)
    #define MISC_EXTLB17_R_EXTLB_RX_LENGTH_M1                  Fld(24, 8) //[31:8]
    #define MISC_EXTLB17_R_EXTLB_TX_PRE_ON                     Fld(1, 7) //[7:7]
    #define MISC_EXTLB17_R_INTLB_DRDF_CA_MUXSEL                Fld(1, 5) //[5:5]
    #define MISC_EXTLB17_R_INTLB_ARCLK_MUXSEL                  Fld(1, 4) //[4:4]
    #define MISC_EXTLB17_R_EXTLB_TX_EN_OTHERCH_SEL             Fld(1, 3) //[3:3]
    #define MISC_EXTLB17_R_EXTLB_TX_EN                         Fld(1, 2) //[2:2]
    #define MISC_EXTLB17_R_EXTLB_RX_SWRST                      Fld(1, 1) //[1:1]
    #define MISC_EXTLB17_R_EXTLB                               Fld(1, 0) //[0:0]

#define DDRPHY_MISC_EXTLB18                         (DDRPHY_AO_BASE_ADDRESS + 0x00248)
    #define MISC_EXTLB18_R_TX_EN_SRC_SEL                       Fld(1, 0) //[0:0]
    #define MISC_EXTLB18_R_OTH_TX_EN_SRC_SEL                   Fld(1, 1) //[1:1]
    #define MISC_EXTLB18_R_LPBK_DQ_MODE_FOR_CA                 Fld(1, 3) //[3:3]
    #define MISC_EXTLB18_R_LPBK_DQ_TX_MODE                     Fld(1, 4) //[4:4]
    #define MISC_EXTLB18_R_LPBK_CA_TX_MODE                     Fld(1, 5) //[5:5]
    #define MISC_EXTLB18_R_LPBK_DQ_RX_MODE                     Fld(1, 8) //[8:8]
    #define MISC_EXTLB18_R_LPBK_CA_RX_MODE                     Fld(1, 9) //[9:9]
    #define MISC_EXTLB18_R_TX_TRIG_SRC_SEL                     Fld(4, 16) //[19:16]
    #define MISC_EXTLB18_R_OTH_TX_TRIG_SRC_SEL                 Fld(4, 20) //[23:20]

#define DDRPHY_MISC_EXTLB19                         (DDRPHY_AO_BASE_ADDRESS + 0x0024C)
    #define MISC_EXTLB19_R_EXTLB_LFSR_ENABLE                   Fld(1, 0) //[0:0]
    #define MISC_EXTLB19_R_EXTLB_SSO_ENABLE                    Fld(1, 1) //[1:1]
    #define MISC_EXTLB19_R_EXTLB_XTALK_ENABLE                  Fld(1, 2) //[2:2]
    #define MISC_EXTLB19_R_EXTLB_LEADLAG_DBG_ENABLE            Fld(1, 3) //[3:3]
    #define MISC_EXTLB19_R_EXTLB_DBG_SEL                       Fld(5, 16) //[20:16]
    #define MISC_EXTLB19_R_LPBK_DC_TOG_MODE                    Fld(1, 23) //[23:23]
    #define MISC_EXTLB19_R_LPBK_DC_TOG_TIMER                   Fld(8, 24) //[31:24]

#define DDRPHY_MISC_EXTLB20                         (DDRPHY_AO_BASE_ADDRESS + 0x00250)
    #define MISC_EXTLB20_R_XTALK_TX_00_TOG_CYCLE               Fld(4, 0) //[3:0]
    #define MISC_EXTLB20_R_XTALK_TX_01_TOG_CYCLE               Fld(4, 4) //[7:4]
    #define MISC_EXTLB20_R_XTALK_TX_02_TOG_CYCLE               Fld(4, 8) //[11:8]
    #define MISC_EXTLB20_R_XTALK_TX_03_TOG_CYCLE               Fld(4, 12) //[15:12]
    #define MISC_EXTLB20_R_XTALK_TX_04_TOG_CYCLE               Fld(4, 16) //[19:16]
    #define MISC_EXTLB20_R_XTALK_TX_05_TOG_CYCLE               Fld(4, 20) //[23:20]
    #define MISC_EXTLB20_R_XTALK_TX_06_TOG_CYCLE               Fld(4, 24) //[27:24]
    #define MISC_EXTLB20_R_XTALK_TX_07_TOG_CYCLE               Fld(4, 28) //[31:28]

#define DDRPHY_MISC_EXTLB21                         (DDRPHY_AO_BASE_ADDRESS + 0x00254)
    #define MISC_EXTLB21_R_XTALK_TX_08_TOG_CYCLE               Fld(4, 0) //[3:0]
    #define MISC_EXTLB21_R_XTALK_TX_09_TOG_CYCLE               Fld(4, 4) //[7:4]
    #define MISC_EXTLB21_R_XTALK_TX_10_TOG_CYCLE               Fld(4, 8) //[11:8]
    #define MISC_EXTLB21_R_XTALK_TX_11_TOG_CYCLE               Fld(4, 12) //[15:12]
    #define MISC_EXTLB21_R_XTALK_TX_12_TOG_CYCLE               Fld(4, 16) //[19:16]
    #define MISC_EXTLB21_R_XTALK_TX_13_TOG_CYCLE               Fld(4, 20) //[23:20]
    #define MISC_EXTLB21_R_XTALK_TX_14_TOG_CYCLE               Fld(4, 24) //[27:24]
    #define MISC_EXTLB21_R_XTALK_TX_15_TOG_CYCLE               Fld(4, 28) //[31:28]

#define DDRPHY_MISC_EXTLB22                         (DDRPHY_AO_BASE_ADDRESS + 0x00258)
    #define MISC_EXTLB22_R_XTALK_TX_16_TOG_CYCLE               Fld(4, 0) //[3:0]
    #define MISC_EXTLB22_R_XTALK_TX_17_TOG_CYCLE               Fld(4, 4) //[7:4]
    #define MISC_EXTLB22_R_XTALK_TX_18_TOG_CYCLE               Fld(4, 8) //[11:8]
    #define MISC_EXTLB22_R_XTALK_TX_19_TOG_CYCLE               Fld(4, 12) //[15:12]
    #define MISC_EXTLB22_R_XTALK_TX_20_TOG_CYCLE               Fld(4, 16) //[19:16]
    #define MISC_EXTLB22_R_XTALK_TX_21_TOG_CYCLE               Fld(4, 20) //[23:20]
    #define MISC_EXTLB22_R_XTALK_TX_22_TOG_CYCLE               Fld(4, 24) //[27:24]
    #define MISC_EXTLB22_R_XTALK_TX_23_TOG_CYCLE               Fld(4, 28) //[31:28]

#define DDRPHY_MISC_EXTLB23                         (DDRPHY_AO_BASE_ADDRESS + 0x0025C)
    #define MISC_EXTLB23_R_XTALK_TX_24_TOG_CYCLE               Fld(4, 0) //[3:0]
    #define MISC_EXTLB23_R_XTALK_TX_25_TOG_CYCLE               Fld(4, 4) //[7:4]
    #define MISC_EXTLB23_R_XTALK_TX_26_TOG_CYCLE               Fld(4, 8) //[11:8]
    #define MISC_EXTLB23_R_XTALK_TX_27_TOG_CYCLE               Fld(4, 12) //[15:12]
    #define MISC_EXTLB23_R_XTALK_TX_28_TOG_CYCLE               Fld(4, 16) //[19:16]
    #define MISC_EXTLB23_R_XTALK_TX_29_TOG_CYCLE               Fld(4, 20) //[23:20]
    #define MISC_EXTLB23_R_XTALK_TX_30_TOG_CYCLE               Fld(4, 24) //[27:24]
    #define MISC_EXTLB23_R_XTALK_TX_31_TOG_CYCLE               Fld(4, 28) //[31:28]

#define DDRPHY_DVFS_EMI_CLK                         (DDRPHY_AO_BASE_ADDRESS + 0x00260)
    #define DVFS_EMI_CLK_RG_DLL_SHUFFLE                        Fld(1, 24) //[24:24]
    #define DVFS_EMI_CLK_RG_52M_104M_SEL                       Fld(1, 29) //[29:29]
    #define DVFS_EMI_CLK_RG_GATING_EMI_NEW                     Fld(2, 30) //[31:30]

#define DDRPHY_MISC_VREF_CTRL                       (DDRPHY_AO_BASE_ADDRESS + 0x00264)
    #define MISC_VREF_CTRL_VREF_CTRL_RFU                       Fld(15, 16) //[30:16]
    #define MISC_VREF_CTRL_RG_RVREF_VREF_EN                    Fld(1, 31) //[31:31]

#define DDRPHY_MISC_IMP_CTRL0                       (DDRPHY_AO_BASE_ADDRESS + 0x00268)
    #define MISC_IMP_CTRL0_RG_IMP_OCD_PUCMP_EN                 Fld(1, 3) //[3:3]
    #define MISC_IMP_CTRL0_RG_IMP_EN                           Fld(1, 4) //[4:4]
    #define MISC_IMP_CTRL0_RG_RIMP_DDR4_SEL                    Fld(1, 5) //[5:5]
    #define MISC_IMP_CTRL0_RG_RIMP_DDR3_SEL                    Fld(1, 6) //[6:6]

#define DDRPHY_MISC_IMP_CTRL1                       (DDRPHY_AO_BASE_ADDRESS + 0x0026C)
    #define MISC_IMP_CTRL1_RG_RIMP_BIAS_EN                     Fld(1, 4) //[4:4]
    #define MISC_IMP_CTRL1_RG_RIMP_ODT_EN                      Fld(1, 5) //[5:5]
    #define MISC_IMP_CTRL1_RG_RIMP_PRE_EN                      Fld(1, 6) //[6:6]
    #define MISC_IMP_CTRL1_RG_RIMP_VREF_EN                     Fld(1, 7) //[7:7]
    #define MISC_IMP_CTRL1_RG_RIMP_DRV05                       Fld(1, 16) //[16:16]
    #define MISC_IMP_CTRL1_RG_RIMP_SUS_ECO_OPT                 Fld(1, 31) //[31:31]

#define DDRPHY_MISC_SHU_OPT                         (DDRPHY_AO_BASE_ADDRESS + 0x00270)
    #define MISC_SHU_OPT_R_DQB0_SHU_PHY_GATING_RESETB_SPM_EN   Fld(1, 0) //[0:0]
    #define MISC_SHU_OPT_R_DQB0_SHU_PHDET_SPM_EN               Fld(2, 2) //[3:2]
    #define MISC_SHU_OPT_R_DQB1_SHU_PHY_GATING_RESETB_SPM_EN   Fld(1, 8) //[8:8]
    #define MISC_SHU_OPT_R_DQB1_SHU_PHDET_SPM_EN               Fld(2, 10) //[11:10]
    #define MISC_SHU_OPT_R_CA_SHU_PHY_GATING_RESETB_SPM_EN     Fld(1, 16) //[16:16]
    #define MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN                 Fld(2, 18) //[19:18]

#define DDRPHY_MISC_SPM_CTRL0                       (DDRPHY_AO_BASE_ADDRESS + 0x00274)
    #define MISC_SPM_CTRL0_PHY_SPM_CTL0                        Fld(32, 0) //[31:0]

#define DDRPHY_MISC_SPM_CTRL1                       (DDRPHY_AO_BASE_ADDRESS + 0x00278)
    #define MISC_SPM_CTRL1_RG_ARDMSUS_10                       Fld(1, 0) //[0:0]
    #define MISC_SPM_CTRL1_RG_ARDMSUS_10_B0                    Fld(1, 1) //[1:1]
    #define MISC_SPM_CTRL1_RG_ARDMSUS_10_B1                    Fld(1, 2) //[2:2]
    #define MISC_SPM_CTRL1_RG_ARDMSUS_10_CA                    Fld(1, 3) //[3:3]
    #define MISC_SPM_CTRL1_SPM_DVFS_CONTROL_SEL                Fld(1, 16) //[16:16]
    #define MISC_SPM_CTRL1_RG_DR_SHU_LEVEL                     Fld(2, 17) //[18:17]
    #define MISC_SPM_CTRL1_RG_PHYPLL_SHU_EN                    Fld(1, 19) //[19:19]
    #define MISC_SPM_CTRL1_RG_PHYPLL2_SHU_EN                   Fld(1, 20) //[20:20]
    #define MISC_SPM_CTRL1_RG_PHYPLL_MODE_SW                   Fld(1, 21) //[21:21]
    #define MISC_SPM_CTRL1_RG_PHYPLL2_MODE_SW                  Fld(1, 22) //[22:22]
    #define MISC_SPM_CTRL1_RG_DR_SHORT_QUEUE                   Fld(1, 23) //[23:23]
    #define MISC_SPM_CTRL1_RG_DR_SHU_EN                        Fld(1, 24) //[24:24]
    #define MISC_SPM_CTRL1_RG_DDRPHY_DB_CK_CH0_EN              Fld(1, 25) //[25:25]
    #define MISC_SPM_CTRL1_RG_DDRPHY_DB_CK_CH1_EN              Fld(1, 26) //[26:26]

#define DDRPHY_MISC_SPM_CTRL2                       (DDRPHY_AO_BASE_ADDRESS + 0x0027C)
    #define MISC_SPM_CTRL2_PHY_SPM_CTL2                        Fld(32, 0) //[31:0]

#define DDRPHY_MISC_SPM_CTRL3                       (DDRPHY_AO_BASE_ADDRESS + 0x00280)
    #define MISC_SPM_CTRL3_PHY_SPM_CTL3                        Fld(32, 0) //[31:0]

#define DDRPHY_MISC_CG_CTRL0                        (DDRPHY_AO_BASE_ADDRESS + 0x00284)
    #define MISC_CG_CTRL0_W_CHG_MEM                            Fld(1, 0) //[0:0]
    #define MISC_CG_CTRL0_CLK_MEM_SEL                          Fld(2, 4) //[5:4]
    #define MISC_CG_CTRL0_CLK_MEM_INV                          Fld(1, 6) //[6:6]
    #define MISC_CG_CTRL0_RG_CG_EMI_OFF_DISABLE                Fld(1, 8) //[8:8]
    #define MISC_CG_CTRL0_RG_CG_DRAMC_OFF_DISABLE              Fld(1, 9) //[9:9]
    #define MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE                 Fld(1, 10) //[10:10]
    #define MISC_CG_CTRL0_RG_CG_COMB_OFF_DISABLE               Fld(1, 11) //[11:11]
    #define MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE                Fld(1, 12) //[12:12]
    #define MISC_CG_CTRL0_RG_CG_COMB0_OFF_DISABLE              Fld(1, 13) //[13:13]
    #define MISC_CG_CTRL0_RG_CG_COMB1_OFF_DISABLE              Fld(1, 14) //[14:14]
    #define MISC_CG_CTRL0_RG_CG_RX_CMD_OFF_DISABLE             Fld(1, 15) //[15:15]
    #define MISC_CG_CTRL0_RG_CG_RX_COMB0_OFF_DISABLE           Fld(1, 16) //[16:16]
    #define MISC_CG_CTRL0_RG_CG_RX_COMB1_OFF_DISABLE           Fld(1, 17) //[17:17]
    #define MISC_CG_CTRL0_RG_CG_IDLE_SYNC_EN                   Fld(1, 18) //[18:18]
    #define MISC_CG_CTRL0_RG_CG_INFRA_OFF_DISABLE              Fld(1, 19) //[19:19]
    #define MISC_CG_CTRL0_RG_CG_DRAMC_CHB_CK_OFF               Fld(1, 20) //[20:20]
    #define MISC_CG_CTRL0_RG_DBG_OUT_SEL                       Fld(1, 21) //[21:21]
    #define MISC_CG_CTRL0_RG_CG_NAO_FORCE_OFF                  Fld(1, 22) //[22:22]
    #define MISC_CG_CTRL0_RG_DA_RREF_CK_SEL                    Fld(1, 28) //[28:28]
    #define MISC_CG_CTRL0_RG_FREERUN_MCK_CG                    Fld(1, 29) //[29:29]
    #define MISC_CG_CTRL0_RG_FREERUN_MCK_CHB_SEL               Fld(1, 30) //[30:30]
    #define MISC_CG_CTRL0_CLK_MEM_DFS_CFG			Fld(32, 0) //cc add

#define DDRPHY_MISC_CG_CTRL1                        (DDRPHY_AO_BASE_ADDRESS + 0x00288)
    #define MISC_CG_CTRL1_R_DVS_DIV4_CG_CTRL                   Fld(32, 0) //[31:0]

#define DDRPHY_MISC_CG_CTRL2                        (DDRPHY_AO_BASE_ADDRESS + 0x0028C)
    #define MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG                   Fld(1, 0) //[0:0]
    #define MISC_CG_CTRL2_RG_MEM_DCM_APB_SEL                   Fld(5, 1) //[5:1]
    #define MISC_CG_CTRL2_RG_MEM_DCM_FORCE_ON                  Fld(1, 6) //[6:6]
    #define MISC_CG_CTRL2_RG_MEM_DCM_DCM_EN                    Fld(1, 7) //[7:7]
    #define MISC_CG_CTRL2_RG_MEM_DCM_DBC_EN                    Fld(1, 8) //[8:8]
    #define MISC_CG_CTRL2_RG_MEM_DCM_DBC_CNT                   Fld(7, 9) //[15:9]
    #define MISC_CG_CTRL2_RG_MEM_DCM_FSEL                      Fld(5, 16) //[20:16]
    #define MISC_CG_CTRL2_RG_MEM_DCM_IDLE_FSEL                 Fld(5, 21) //[25:21]
    #define MISC_CG_CTRL2_RG_MEM_DCM_FORCE_OFF                 Fld(1, 26) //[26:26]
    #define MISC_CG_CTRL2_RG_PHY_CG_OFF_DISABLE                Fld(1, 28) //[28:28]
    #define MISC_CG_CTRL2_RG_PIPE0_CG_OFF_DISABLE              Fld(1, 29) //[29:29]
    #define MISC_CG_CTRL2_RG_MEM_DCM_CG_OFF_DISABLE            Fld(1, 31) //[31:31]
    #define MISC_CG_CTRL2_RG_MEM_DCM_CTL			Fld(32, 0)

#define DDRPHY_MISC_CG_CTRL3                        (DDRPHY_AO_BASE_ADDRESS + 0x00290)
    #define MISC_CG_CTRL3_R_LBK_CG_CTRL                        Fld(32, 0) //[31:0]

#define DDRPHY_MISC_CG_CTRL4                        (DDRPHY_AO_BASE_ADDRESS + 0x00294)
    #define MISC_CG_CTRL4_R_PHY_MCK_CG_CTRL                    Fld(32, 0) //[31:0]

#define DDRPHY_MISC_CG_CTRL5                        (DDRPHY_AO_BASE_ADDRESS + 0x00298)
    #define MISC_CG_CTRL5_RESERVE                              Fld(16, 0) //[15:0]
    #define MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN                     Fld(1, 16) //[16:16]
    #define MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN                     Fld(1, 17) //[17:17]
    #define MISC_CG_CTRL5_R_CA_DLY_DCM_EN                      Fld(1, 18) //[18:18]
    #define MISC_CG_CTRL5_R_DQ1_PI_DCM_EN                      Fld(1, 20) //[20:20]
    #define MISC_CG_CTRL5_R_DQ0_PI_DCM_EN                      Fld(1, 21) //[21:21]
    #define MISC_CG_CTRL5_R_CA_PI_DCM_EN                       Fld(1, 22) //[22:22]

#define DDRPHY_MISC_CTRL0                           (DDRPHY_AO_BASE_ADDRESS + 0x0029C)
    #define MISC_CTRL0_R_DMDQSIEN_SYNCOPT                      Fld(4, 0) //[3:0]
    #define MISC_CTRL0_R_DMDQSIEN_OUTSEL                       Fld(4, 4) //[7:4]
    #define MISC_CTRL0_R_DMSTBEN_SYNCOPT                       Fld(1, 8) //[8:8]
    #define MISC_CTRL0_R_DMSTBEN_OUTSEL                        Fld(1, 9) //[9:9]
    #define MISC_CTRL0_IMPCAL_CHAB_EN                          Fld(1, 10) //[10:10]
    #define MISC_CTRL0_R_DMVALID_DLY_OPT                       Fld(1, 11) //[11:11]
    #define MISC_CTRL0_R_DMVALID_NARROW_IG                     Fld(1, 12) //[12:12]
    #define MISC_CTRL0_R_DMVALID_DLY                           Fld(3, 13) //[15:13]
    #define MISC_CTRL0_R_DMDQSIEN_DEPTH_HALF                   Fld(1, 16) //[16:16]
    #define MISC_CTRL0_R_DMRDSEL_DIV2_OPT                      Fld(1, 17) //[17:17]
    #define MISC_CTRL0_IMPCAL_LP_ECO_OPT                       Fld(1, 18) //[18:18]
    #define MISC_CTRL0_IMPCAL_CDC_ECO_OPT                      Fld(1, 19) //[19:19]
    #define MISC_CTRL0_IMPCAL_CTL_CK_SEL                       Fld(1, 21) //[21:21]
    #define MISC_CTRL0_IDLE_DCM_CHB_CDC_ECO_OPT			Fld(1, 20)
    #define MISC_CTRL0_R_DMDQSIEN_FIFO_EN                      Fld(1, 24) //[24:24]
    #define MISC_CTRL0_R_DMSTBENCMP_FIFO_EN                    Fld(1, 25) //[25:25]
    #define MISC_CTRL0_R_DMSTBENCMP_RK_FIFO_EN                 Fld(1, 26) //[26:26]
    #define MISC_CTRL0_R_DMSHU_PHYDCM_FORCEOFF                 Fld(1, 27) //[27:27]
    #define MISC_CTRL0_R_DQS0IEN_DIV4_CK_CG_CTRL               Fld(1, 28) //[28:28]
    #define MISC_CTRL0_R_DQS1IEN_DIV4_CK_CG_CTRL               Fld(1, 29) //[29:29]
    #define MISC_CTRL0_R_CLKIEN_DIV4_CK_CG_CTRL                Fld(1, 30) //[30:30]
    #define MISC_CTRL0_R_STBENCMP_DIV4CK_EN                    Fld(1, 31) //[31:31]

#define DDRPHY_MISC_CTRL1                           (DDRPHY_AO_BASE_ADDRESS + 0x002A0)
    #define MISC_CTRL1_R_DMPHYRST                              Fld(1, 1) //[1:1]
    #define MISC_CTRL1_R_DM_TX_ARCLK_OE                        Fld(1, 2) //[2:2]
    #define MISC_CTRL1_R_DM_TX_ARCMD_OE                        Fld(1, 3) //[3:3]
    #define MISC_CTRL1_R_DMMCTLPLL_CKSEL                       Fld(2, 4) //[5:4]
    #define MISC_CTRL1_R_DMMUXCA                               Fld(1, 6) //[6:6]
    #define MISC_CTRL1_R_DMARPIDQ_SW                           Fld(1, 7) //[7:7]
    #define MISC_CTRL1_R_DMPINMUX                              Fld(2, 8) //[9:8]
    #define MISC_CTRL1_R_DMARPICA_SW_UPDX                      Fld(1, 10) //[10:10]
    #define MISC_CTRL1_CK_BFE_DCM_EN                           Fld(1, 11) //[11:11]
    #define MISC_CTRL1_R_DMRRESETB_I_OPT                       Fld(1, 12) //[12:12]
    #define MISC_CTRL1_R_DMDA_RRESETB_I                        Fld(1, 13) //[13:13]
    #define MISC_CTRL1_R_DMMUXCA_SEC                           Fld(1, 14) //[14:14]
    #define MISC_CTRL1_R_DQ2DM_SWAP                            Fld(1, 15) //[15:15]
    #define MISC_CTRL1_R_DMDRAMCLKEN0                          Fld(4, 16) //[19:16]
    #define MISC_CTRL1_R_DMDRAMCLKEN1                          Fld(4, 20) //[23:20]
    #define MISC_CTRL1_R_DMDQSIENCG_EN                         Fld(1, 24) //[24:24]
    #define MISC_CTRL1_R_DMSTBENCMP_RK_OPT                     Fld(1, 25) //[25:25]
    #define MISC_CTRL1_R_WL_DOWNSP                             Fld(1, 26) //[26:26]
    #define MISC_CTRL1_R_DMODTDISOE_A                          Fld(1, 27) //[27:27]
    #define MISC_CTRL1_R_DMODTDISOE_B                          Fld(1, 28) //[28:28]
    #define MISC_CTRL1_R_DMODTDISOE_C                          Fld(1, 29) //[29:29]
    #define MISC_CTRL1_R_DMDA_RRESETB_E                        Fld(1, 31) //[31:31]

#define DDRPHY_MISC_CTRL2                           (DDRPHY_AO_BASE_ADDRESS + 0x002A4)
    #define MISC_CTRL2_PLL_SHU_GP                              Fld(2, 0) //[1:0]

#define DDRPHY_MISC_CTRL3                           (DDRPHY_AO_BASE_ADDRESS + 0x002A8)
    #define MISC_CTRL3_ARPI_CG_CMD_OPT                         Fld(2, 0) //[1:0]
    #define MISC_CTRL3_ARPI_CG_CLK_OPT                         Fld(2, 2) //[3:2]
    #define MISC_CTRL3_ARPI_MPDIV_CG_CA_OPT                    Fld(1, 4) //[4:4]
    #define MISC_CTRL3_ARPI_CG_MCK_CA_OPT                      Fld(1, 5) //[5:5]
    #define MISC_CTRL3_ARPI_CG_MCTL_CA_OPT                     Fld(1, 6) //[6:6]
    #define MISC_CTRL3_DDRPHY_MCK_MPDIV_CG_CA_SEL              Fld(2, 8) //[9:8]
    #define MISC_CTRL3_DRAM_CLK_NEW_CA_EN_SEL                  Fld(4, 12) //[15:12]
    #define MISC_CTRL3_ARPI_CG_DQ_OPT                          Fld(2, 16) //[17:16]
    #define MISC_CTRL3_ARPI_CG_DQS_OPT                         Fld(2, 18) //[19:18]
    #define MISC_CTRL3_ARPI_MPDIV_CG_DQ_OPT                    Fld(1, 20) //[20:20]
    #define MISC_CTRL3_ARPI_CG_MCK_DQ_OPT                      Fld(1, 21) //[21:21]
    #define MISC_CTRL3_ARPI_CG_MCTL_DQ_OPT                     Fld(1, 22) //[22:22]
    #define MISC_CTRL3_DDRPHY_MCK_MPDIV_CG_DQ_SEL              Fld(2, 24) //[25:24]
    #define MISC_CTRL3_R_DDRPHY_COMB_CG_IG                     Fld(1, 26) //[26:26]
    #define MISC_CTRL3_R_DDRPHY_RX_PIPE_CG_IG                  Fld(1, 27) //[27:27]
    #define MISC_CTRL3_DRAM_CLK_NEW_DQ_EN_SEL                  Fld(4, 28) //[31:28]

#define DDRPHY_MISC_CTRL4                           (DDRPHY_AO_BASE_ADDRESS + 0x002AC)
    #define MISC_CTRL4_RG_PW_CON_CHA_0                         Fld(32, 0) //[31:0]

#define DDRPHY_MISC_CTRL5                           (DDRPHY_AO_BASE_ADDRESS + 0x002B0)
    #define MISC_CTRL5_RG_PW_CON_CHA_1                         Fld(32, 0) //[31:0]

#define DDRPHY_MISC_EXTLB_RX0                       (DDRPHY_AO_BASE_ADDRESS + 0x002B4)
    #define MISC_EXTLB_RX0_R_EXTLB_LFSR_RX_INI_1               Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX0_R_EXTLB_LFSR_RX_INI_0               Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB_RX1                       (DDRPHY_AO_BASE_ADDRESS + 0x002B8)
    #define MISC_EXTLB_RX1_R_EXTLB_LFSR_RX_INI_3               Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX1_R_EXTLB_LFSR_RX_INI_2               Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB_RX2                       (DDRPHY_AO_BASE_ADDRESS + 0x002BC)
    #define MISC_EXTLB_RX2_R_EXTLB_LFSR_RX_INI_5               Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX2_R_EXTLB_LFSR_RX_INI_4               Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB_RX3                       (DDRPHY_AO_BASE_ADDRESS + 0x002C0)
    #define MISC_EXTLB_RX3_R_EXTLB_LFSR_RX_INI_7               Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX3_R_EXTLB_LFSR_RX_INI_6               Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB_RX4                       (DDRPHY_AO_BASE_ADDRESS + 0x002C4)
    #define MISC_EXTLB_RX4_R_EXTLB_LFSR_RX_INI_9               Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX4_R_EXTLB_LFSR_RX_INI_8               Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB_RX5                       (DDRPHY_AO_BASE_ADDRESS + 0x002C8)
    #define MISC_EXTLB_RX5_R_EXTLB_LFSR_RX_INI_11              Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX5_R_EXTLB_LFSR_RX_INI_10              Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB_RX6                       (DDRPHY_AO_BASE_ADDRESS + 0x002CC)
    #define MISC_EXTLB_RX6_R_EXTLB_LFSR_RX_INI_13              Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX6_R_EXTLB_LFSR_RX_INI_12              Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB_RX7                       (DDRPHY_AO_BASE_ADDRESS + 0x002D0)
    #define MISC_EXTLB_RX7_R_EXTLB_LFSR_RX_INI_15              Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX7_R_EXTLB_LFSR_RX_INI_14              Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB_RX8                       (DDRPHY_AO_BASE_ADDRESS + 0x002D4)
    #define MISC_EXTLB_RX8_R_EXTLB_LFSR_RX_INI_17              Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX8_R_EXTLB_LFSR_RX_INI_16              Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB_RX9                       (DDRPHY_AO_BASE_ADDRESS + 0x002D8)
    #define MISC_EXTLB_RX9_R_EXTLB_LFSR_RX_INI_19              Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX9_R_EXTLB_LFSR_RX_INI_18              Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB_RX10                      (DDRPHY_AO_BASE_ADDRESS + 0x002DC)
    #define MISC_EXTLB_RX10_R_EXTLB_LFSR_RX_INI_21             Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX10_R_EXTLB_LFSR_RX_INI_20             Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB_RX11                      (DDRPHY_AO_BASE_ADDRESS + 0x002E0)
    #define MISC_EXTLB_RX11_R_EXTLB_LFSR_RX_INI_23             Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX11_R_EXTLB_LFSR_RX_INI_22             Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB_RX12                      (DDRPHY_AO_BASE_ADDRESS + 0x002E4)
    #define MISC_EXTLB_RX12_R_EXTLB_LFSR_RX_INI_25             Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX12_R_EXTLB_LFSR_RX_INI_24             Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB_RX13                      (DDRPHY_AO_BASE_ADDRESS + 0x002E8)
    #define MISC_EXTLB_RX13_R_EXTLB_LFSR_RX_INI_27             Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX13_R_EXTLB_LFSR_RX_INI_26             Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB_RX14                      (DDRPHY_AO_BASE_ADDRESS + 0x002EC)
    #define MISC_EXTLB_RX14_R_EXTLB_LFSR_RX_INI_29             Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX14_R_EXTLB_LFSR_RX_INI_28             Fld(16, 0) //[15:0]

#define DDRPHY_MISC_EXTLB_RX15                      (DDRPHY_AO_BASE_ADDRESS + 0x002F0)
    #define MISC_EXTLB_RX15_R_EXTLB_LFSR_RX_INI_30             Fld(16, 0) //[15:0]
    #define MISC_EXTLB_RX15_MISC_EXTLB_RX15_RFU                Fld(16, 16) //[31:16]

#define DDRPHY_MISC_EXTLB_RX16                      (DDRPHY_AO_BASE_ADDRESS + 0x002F4)
    #define MISC_EXTLB_RX16_R_EXTLB_RX_GATE_DELSEL_DQB0        Fld(7, 0) //[6:0]
    #define MISC_EXTLB_RX16_R_EXTLB_RX_GATE_DELSEL_DQB1        Fld(7, 8) //[14:8]
    #define MISC_EXTLB_RX16_R_EXTLB_RX_GATE_DELSEL_CA          Fld(7, 16) //[22:16]

#define DDRPHY_MISC_EXTLB_RX17                      (DDRPHY_AO_BASE_ADDRESS + 0x002F8)
    #define MISC_EXTLB_RX17_R_XTALK_RX_00_TOG_CYCLE            Fld(4, 0) //[3:0]
    #define MISC_EXTLB_RX17_R_XTALK_RX_01_TOG_CYCLE            Fld(4, 4) //[7:4]
    #define MISC_EXTLB_RX17_R_XTALK_RX_02_TOG_CYCLE            Fld(4, 8) //[11:8]
    #define MISC_EXTLB_RX17_R_XTALK_RX_03_TOG_CYCLE            Fld(4, 12) //[15:12]
    #define MISC_EXTLB_RX17_R_XTALK_RX_04_TOG_CYCLE            Fld(4, 16) //[19:16]
    #define MISC_EXTLB_RX17_R_XTALK_RX_05_TOG_CYCLE            Fld(4, 20) //[23:20]
    #define MISC_EXTLB_RX17_R_XTALK_RX_06_TOG_CYCLE            Fld(4, 24) //[27:24]
    #define MISC_EXTLB_RX17_R_XTALK_RX_07_TOG_CYCLE            Fld(4, 28) //[31:28]

#define DDRPHY_MISC_EXTLB_RX18                      (DDRPHY_AO_BASE_ADDRESS + 0x002FC)
    #define MISC_EXTLB_RX18_R_XTALK_RX_08_TOG_CYCLE            Fld(4, 0) //[3:0]
    #define MISC_EXTLB_RX18_R_XTALK_RX_09_TOG_CYCLE            Fld(4, 4) //[7:4]
    #define MISC_EXTLB_RX18_R_XTALK_RX_10_TOG_CYCLE            Fld(4, 8) //[11:8]
    #define MISC_EXTLB_RX18_R_XTALK_RX_11_TOG_CYCLE            Fld(4, 12) //[15:12]
    #define MISC_EXTLB_RX18_R_XTALK_RX_12_TOG_CYCLE            Fld(4, 16) //[19:16]
    #define MISC_EXTLB_RX18_R_XTALK_RX_13_TOG_CYCLE            Fld(4, 20) //[23:20]
    #define MISC_EXTLB_RX18_R_XTALK_RX_14_TOG_CYCLE            Fld(4, 24) //[27:24]
    #define MISC_EXTLB_RX18_R_XTALK_RX_15_TOG_CYCLE            Fld(4, 28) //[31:28]

#define DDRPHY_MISC_EXTLB_RX19                      (DDRPHY_AO_BASE_ADDRESS + 0x00300)
    #define MISC_EXTLB_RX19_R_XTALK_RX_16_TOG_CYCLE            Fld(4, 0) //[3:0]
    #define MISC_EXTLB_RX19_R_XTALK_RX_17_TOG_CYCLE            Fld(4, 4) //[7:4]
    #define MISC_EXTLB_RX19_R_XTALK_RX_18_TOG_CYCLE            Fld(4, 8) //[11:8]
    #define MISC_EXTLB_RX19_R_XTALK_RX_19_TOG_CYCLE            Fld(4, 12) //[15:12]
    #define MISC_EXTLB_RX19_R_XTALK_RX_20_TOG_CYCLE            Fld(4, 16) //[19:16]
    #define MISC_EXTLB_RX19_R_XTALK_RX_21_TOG_CYCLE            Fld(4, 20) //[23:20]
    #define MISC_EXTLB_RX19_R_XTALK_RX_22_TOG_CYCLE            Fld(4, 24) //[27:24]
    #define MISC_EXTLB_RX19_R_XTALK_RX_23_TOG_CYCLE            Fld(4, 28) //[31:28]

#define DDRPHY_MISC_EXTLB_RX20                      (DDRPHY_AO_BASE_ADDRESS + 0x00304)
    #define MISC_EXTLB_RX20_R_XTALK_RX_24_TOG_CYCLE            Fld(4, 0) //[3:0]
    #define MISC_EXTLB_RX20_R_XTALK_RX_25_TOG_CYCLE            Fld(4, 4) //[7:4]
    #define MISC_EXTLB_RX20_R_XTALK_RX_26_TOG_CYCLE            Fld(4, 8) //[11:8]
    #define MISC_EXTLB_RX20_R_XTALK_RX_27_TOG_CYCLE            Fld(4, 12) //[15:12]
    #define MISC_EXTLB_RX20_R_XTALK_RX_28_TOG_CYCLE            Fld(4, 16) //[19:16]
    #define MISC_EXTLB_RX20_R_XTALK_RX_29_TOG_CYCLE            Fld(4, 20) //[23:20]
    #define MISC_EXTLB_RX20_R_XTALK_RX_30_TOG_CYCLE            Fld(4, 24) //[27:24]
    #define MISC_EXTLB_RX20_R_XTALK_RX_31_TOG_CYCLE            Fld(4, 28) //[31:28]

#define DDRPHY_CKMUX_SEL                            (DDRPHY_AO_BASE_ADDRESS + 0x00308)
    #define CKMUX_SEL_R_PHYCTRLMUX                             Fld(1, 0) //[0:0]
    #define CKMUX_SEL_R_PHYCTRLDCM                             Fld(1, 1) //[1:1]
    #define CKMUX_SEL_FB_CK_MUX                                Fld(2, 16) //[17:16]
    #define CKMUX_SEL_FMEM_CK_MUX                              Fld(2, 18) //[19:18]

#define DDRPHY_RFU_0X30C                            (DDRPHY_AO_BASE_ADDRESS + 0x0030C)
    #define RFU_0X30C_RESERVED_0X30C                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X310                            (DDRPHY_AO_BASE_ADDRESS + 0x00310)
    #define RFU_0X310_RESERVED_0X310                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X314                            (DDRPHY_AO_BASE_ADDRESS + 0x00314)
    #define RFU_0X314_RESERVED_0X314                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X318                            (DDRPHY_AO_BASE_ADDRESS + 0x00318)
    #define RFU_0X318_RESERVED_0X318                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X31C                            (DDRPHY_AO_BASE_ADDRESS + 0x0031C)
    #define RFU_0X31C_RESERVED_0X31C                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X320                            (DDRPHY_AO_BASE_ADDRESS + 0x00320)
    #define RFU_0X320_RESERVED_0X320                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X324                            (DDRPHY_AO_BASE_ADDRESS + 0x00324)
    #define RFU_0X324_RESERVED_0X324                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X328                            (DDRPHY_AO_BASE_ADDRESS + 0x00328)
    #define RFU_0X328_RESERVED_0X328                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X32C                            (DDRPHY_AO_BASE_ADDRESS + 0x0032C)
    #define RFU_0X32C_RESERVED_0X32C                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X330                            (DDRPHY_AO_BASE_ADDRESS + 0x00330)
    #define RFU_0X330_RESERVED_0X330                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X334                            (DDRPHY_AO_BASE_ADDRESS + 0x00334)
    #define RFU_0X334_RESERVED_0X334                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X338                            (DDRPHY_AO_BASE_ADDRESS + 0x00338)
    #define RFU_0X338_RESERVED_0X338                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X33C                            (DDRPHY_AO_BASE_ADDRESS + 0x0033C)
    #define RFU_0X33C_RESERVED_0X33C                           Fld(32, 0) //[31:0]

#define DDRPHY_MISC_STBERR_RK0_R                    (DDRPHY_AO_BASE_ADDRESS + 0x00510)
    #define MISC_STBERR_RK0_R_STBERR_RK0_R                     Fld(16, 0) //[15:0]
    #define MISC_STBERR_RK0_R_STBENERR_ALL                     Fld(1, 16) //[16:16]
    #define MISC_STBERR_RK0_R_RX_ARDQ0_FIFO_STBEN_ERR_B0       Fld(1, 24) //[24:24]
    #define MISC_STBERR_RK0_R_RX_ARDQ4_FIFO_STBEN_ERR_B0       Fld(1, 25) //[25:25]
    #define MISC_STBERR_RK0_R_RX_ARDQ0_FIFO_STBEN_ERR_B1       Fld(1, 26) //[26:26]
    #define MISC_STBERR_RK0_R_RX_ARDQ4_FIFO_STBEN_ERR_B1       Fld(1, 27) //[27:27]
    #define MISC_STBERR_RK0_R_RX_ARCA0_FIFO_STBEN_ERR          Fld(1, 28) //[28:28]
    #define MISC_STBERR_RK0_R_RX_ARCA4_FIFO_STBEN_ERR          Fld(1, 29) //[29:29]
    #define MISC_STBERR_RK0_R_DA_RPHYPLLGP_CK_SEL              Fld(1, 31) //[31:31]

#define DDRPHY_MISC_STBERR_RK0_F                    (DDRPHY_AO_BASE_ADDRESS + 0x00514)
    #define MISC_STBERR_RK0_F_STBERR_RK0_F                     Fld(16, 0) //[15:0]

#define DDRPHY_MISC_STBERR_RK1_R                    (DDRPHY_AO_BASE_ADDRESS + 0x00518)
    #define MISC_STBERR_RK1_R_STBERR_RK1_R                     Fld(16, 0) //[15:0]

#define DDRPHY_MISC_STBERR_RK1_F                    (DDRPHY_AO_BASE_ADDRESS + 0x0051C)
    #define MISC_STBERR_RK1_F_STBERR_RK1_F                     Fld(16, 0) //[15:0]

#define DDRPHY_MISC_STBERR_RK2_R                    (DDRPHY_AO_BASE_ADDRESS + 0x00520)
    #define MISC_STBERR_RK2_R_STBERR_RK2_R                     Fld(16, 0) //[15:0]

#define DDRPHY_MISC_STBERR_RK2_F                    (DDRPHY_AO_BASE_ADDRESS + 0x00524)
    #define MISC_STBERR_RK2_F_STBERR_RK2_F                     Fld(16, 0) //[15:0]

#define DDRPHY_MISC_RXDVS0                          (DDRPHY_AO_BASE_ADDRESS + 0x005E0)
    #define MISC_RXDVS0_R_RX_DLY_TRACK_RO_SEL                  Fld(3, 0) //[2:0]
    #define MISC_RXDVS0_R_DA_DQX_R_DLY_RO_SEL                  Fld(4, 8) //[11:8]
    #define MISC_RXDVS0_R_DA_CAX_R_DLY_RO_SEL                  Fld(4, 12) //[15:12]

#define DDRPHY_MISC_RXDVS2                          (DDRPHY_AO_BASE_ADDRESS + 0x005E8)
    #define MISC_RXDVS2_R_DMRXDVS_DEPTH_HALF                   Fld(1, 0) //[0:0]
    #define MISC_RXDVS2_R_DMRXDVS_SHUFFLE_CTRL_CG_IG           Fld(1, 8) //[8:8]
    #define MISC_RXDVS2_R_DMRXDVS_DBG_MON_EN                   Fld(1, 16) //[16:16]
    #define MISC_RXDVS2_R_DMRXDVS_DBG_MON_CLR                  Fld(1, 17) //[17:17]
    #define MISC_RXDVS2_R_DMRXDVS_DBG_PAUSE_EN                 Fld(1, 18) //[18:18]

#define DDRPHY_RFU_0X5EC                            (DDRPHY_AO_BASE_ADDRESS + 0x005EC)
    #define RFU_0X5EC_RESERVED_0X5EC                           Fld(32, 0) //[31:0]

#define DDRPHY_B0_RXDVS0                            (DDRPHY_AO_BASE_ADDRESS + 0x005F0)
    #define B0_RXDVS0_R_RX_RANKINSEL_B0                        Fld(1, 0) //[0:0]
    #define B0_RXDVS0_B0_RXDVS0_RFU                            Fld(3, 1) //[3:1]
    #define B0_RXDVS0_R_RX_RANKINCTL_B0                        Fld(4, 4) //[7:4]
    #define B0_RXDVS0_R_DVS_SW_UP_B0                           Fld(1, 8) //[8:8]
    #define B0_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B0                Fld(1, 9) //[9:9]
    #define B0_RXDVS0_R_DMRXDVS_PBYTESTUCK_RST_B0              Fld(1, 10) //[10:10]
    #define B0_RXDVS0_R_DMRXDVS_PBYTESTUCK_IG_B0               Fld(1, 11) //[11:11]
    #define B0_RXDVS0_R_DMRXDVS_DQIENPOST_OPT_B0               Fld(2, 12) //[13:12]
    #define B0_RXDVS0_R_RX_DLY_RANK_ERR_ST_CLR_B0              Fld(3, 16) //[18:16]
    #define B0_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B0                  Fld(1, 19) //[19:19]
    #define B0_RXDVS0_R_RX_DLY_RK_OPT_B0                       Fld(2, 20) //[21:20]
    #define B0_RXDVS0_R_HWRESTORE_ENA_B0                       Fld(1, 22) //[22:22]
    #define B0_RXDVS0_R_HWSAVE_MODE_ENA_B0                     Fld(1, 24) //[24:24]
    #define B0_RXDVS0_R_RX_DLY_DVS_MODE_SYNC_DIS_B0            Fld(1, 26) //[26:26]
    #define B0_RXDVS0_R_RX_DLY_TRACK_BYPASS_MODESYNC_B0        Fld(1, 27) //[27:27]
    #define B0_RXDVS0_R_RX_DLY_TRACK_CG_EN_B0                  Fld(1, 28) //[28:28]
    #define B0_RXDVS0_R_RX_DLY_TRACK_SPM_CTRL_B0               Fld(1, 29) //[29:29]
    #define B0_RXDVS0_R_RX_DLY_TRACK_CLR_B0                    Fld(1, 30) //[30:30]
    #define B0_RXDVS0_R_RX_DLY_TRACK_ENA_B0                    Fld(1, 31) //[31:31]

#define DDRPHY_B0_RXDVS1                            (DDRPHY_AO_BASE_ADDRESS + 0x005F4)
    #define B0_RXDVS1_B0_RXDVS1_RFU                            Fld(16, 0) //[15:0]
    #define B0_RXDVS1_R_DMRXDVS_UPD_CLR_ACK_B0                 Fld(1, 16) //[16:16]
    #define B0_RXDVS1_R_DMRXDVS_UPD_CLR_NORD_B0                Fld(1, 17) //[17:17]

#define DDRPHY_RFU_0X5F8                            (DDRPHY_AO_BASE_ADDRESS + 0x005F8)
    #define RFU_0X5F8_RESERVED_0X5F8                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X5FC                            (DDRPHY_AO_BASE_ADDRESS + 0x005FC)
    #define RFU_0X5FC_RESERVED_0X5FC                           Fld(32, 0) //[31:0]

#define DDRPHY_R0_B0_RXDVS0                         (DDRPHY_AO_BASE_ADDRESS + 0x00600)
    #define R0_B0_RXDVS0_R_RK0_B0_DVS_LEAD_LAG_CNT_CLR         Fld(1, 26) //[26:26]
    #define R0_B0_RXDVS0_R_RK0_B0_DVS_SW_CNT_CLR               Fld(1, 27) //[27:27]
    #define R0_B0_RXDVS0_R_RK0_B0_DVS_SW_CNT_ENA               Fld(1, 31) //[31:31]

#define DDRPHY_R0_B0_RXDVS1                         (DDRPHY_AO_BASE_ADDRESS + 0x00604)
    #define R0_B0_RXDVS1_R_RK0_B0_DVS_TH_LAG                   Fld(16, 0) //[15:0]
    #define R0_B0_RXDVS1_R_RK0_B0_DVS_TH_LEAD                  Fld(16, 16) //[31:16]

#define DDRPHY_R0_B0_RXDVS2                         (DDRPHY_AO_BASE_ADDRESS + 0x00608)
    #define R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_DQS_SCALE_B0         Fld(2, 16) //[17:16]
    #define R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_DQ_SCALE_B0          Fld(2, 18) //[19:18]
    #define R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0    Fld(1, 23) //[23:23]
    #define R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_DQS_SCALE_B0         Fld(2, 24) //[25:24]
    #define R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_DQ_SCALE_B0          Fld(2, 26) //[27:26]
    #define R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0    Fld(1, 28) //[28:28]
    #define R0_B0_RXDVS2_R_RK0_DVS_FDLY_MODE_B0                Fld(1, 29) //[29:29]
    #define R0_B0_RXDVS2_R_RK0_DVS_MODE_B0                     Fld(2, 30) //[31:30]

#define DDRPHY_R0_B0_RXDVS7                         (DDRPHY_AO_BASE_ADDRESS + 0x0061C)
    #define R0_B0_RXDVS7_RG_RK0_ARDQ_MIN_DLY_B0                Fld(6, 0) //[5:0]
    #define R0_B0_RXDVS7_RG_RK0_ARDQ_MIN_DLY_B0_RFU            Fld(2, 6) //[7:6]
    #define R0_B0_RXDVS7_RG_RK0_ARDQ_MAX_DLY_B0                Fld(6, 8) //[13:8]
    #define R0_B0_RXDVS7_RG_RK0_ARDQ_MAX_DLY_B0_RFU            Fld(2, 14) //[15:14]
    #define R0_B0_RXDVS7_RG_RK0_ARDQS0_MIN_DLY_B0              Fld(7, 16) //[22:16]
    #define R0_B0_RXDVS7_RG_RK0_ARDQS0_MIN_DLY_B0_RFU          Fld(1, 23) //[23:23]
    #define R0_B0_RXDVS7_RG_RK0_ARDQS0_MAX_DLY_B0              Fld(7, 24) //[30:24]
    #define R0_B0_RXDVS7_RG_RK0_ARDQS0_MAX_DLY_B0_RFU          Fld(1, 31) //[31:31]

#define DDRPHY_RFU_0X620                            (DDRPHY_AO_BASE_ADDRESS + 0x00620)
    #define RFU_0X620_RESERVED_0X620                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X624                            (DDRPHY_AO_BASE_ADDRESS + 0x00624)
    #define RFU_0X624_RESERVED_0X624                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X628                            (DDRPHY_AO_BASE_ADDRESS + 0x00628)
    #define RFU_0X628_RESERVED_0X628                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X62C                            (DDRPHY_AO_BASE_ADDRESS + 0x0062C)
    #define RFU_0X62C_RESERVED_0X62C                           Fld(32, 0) //[31:0]

#define DDRPHY_B1_RXDVS0                            (DDRPHY_AO_BASE_ADDRESS + 0x00670)
    #define B1_RXDVS0_R_RX_RANKINSEL_B1                        Fld(1, 0) //[0:0]
    #define B1_RXDVS0_B1_RXDVS0_RFU                            Fld(3, 1) //[3:1]
    #define B1_RXDVS0_R_RX_RANKINCTL_B1                        Fld(4, 4) //[7:4]
    #define B1_RXDVS0_R_DVS_SW_UP_B1                           Fld(1, 8) //[8:8]
    #define B1_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B1                Fld(1, 9) //[9:9]
    #define B1_RXDVS0_R_DMRXDVS_PBYTESTUCK_RST_B1              Fld(1, 10) //[10:10]
    #define B1_RXDVS0_R_DMRXDVS_PBYTESTUCK_IG_B1               Fld(1, 11) //[11:11]
    #define B1_RXDVS0_R_DMRXDVS_DQIENPOST_OPT_B1               Fld(2, 12) //[13:12]
    #define B1_RXDVS0_R_RX_DLY_RANK_ERR_ST_CLR_B1              Fld(3, 16) //[18:16]
    #define B1_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B1                  Fld(1, 19) //[19:19]
    #define B1_RXDVS0_R_RX_DLY_RK_OPT_B1                       Fld(2, 20) //[21:20]
    #define B1_RXDVS0_R_HWRESTORE_ENA_B1                       Fld(1, 22) //[22:22]
    #define B1_RXDVS0_R_HWSAVE_MODE_ENA_B1                     Fld(1, 24) //[24:24]
    #define B1_RXDVS0_R_RX_DLY_DVS_MODE_SYNC_DIS_B1            Fld(1, 26) //[26:26]
    #define B1_RXDVS0_R_RX_DLY_TRACK_BYPASS_MODESYNC_B1        Fld(1, 27) //[27:27]
    #define B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1                  Fld(1, 28) //[28:28]
    #define B1_RXDVS0_R_RX_DLY_TRACK_SPM_CTRL_B1               Fld(1, 29) //[29:29]
    #define B1_RXDVS0_R_RX_DLY_TRACK_CLR_B1                    Fld(1, 30) //[30:30]
    #define B1_RXDVS0_R_RX_DLY_TRACK_ENA_B1                    Fld(1, 31) //[31:31]

#define DDRPHY_B1_RXDVS1                            (DDRPHY_AO_BASE_ADDRESS + 0x00674)
    #define B1_RXDVS1_B1_RXDVS1_RFU                            Fld(16, 0) //[15:0]
    #define B1_RXDVS1_R_DMRXDVS_UPD_CLR_ACK_B1                 Fld(1, 16) //[16:16]
    #define B1_RXDVS1_R_DMRXDVS_UPD_CLR_NORD_B1                Fld(1, 17) //[17:17]

#define DDRPHY_RFU_0X678                            (DDRPHY_AO_BASE_ADDRESS + 0x00678)
    #define RFU_0X678_RESERVED_0X678                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X67C                            (DDRPHY_AO_BASE_ADDRESS + 0x0067C)
    #define RFU_0X67C_RESERVED_0X67C                           Fld(32, 0) //[31:0]

#define DDRPHY_R0_B1_RXDVS0                         (DDRPHY_AO_BASE_ADDRESS + 0x00680)
    #define R0_B1_RXDVS0_R_RK0_B1_DVS_LEAD_LAG_CNT_CLR         Fld(1, 26) //[26:26]
    #define R0_B1_RXDVS0_R_RK0_B1_DVS_SW_CNT_CLR               Fld(1, 27) //[27:27]
    #define R0_B1_RXDVS0_R_RK0_B1_DVS_SW_CNT_ENA               Fld(1, 31) //[31:31]

#define DDRPHY_R0_B1_RXDVS1                         (DDRPHY_AO_BASE_ADDRESS + 0x00684)
    #define R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LAG                   Fld(16, 0) //[15:0]
    #define R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LEAD                  Fld(16, 16) //[31:16]

#define DDRPHY_R0_B1_RXDVS2                         (DDRPHY_AO_BASE_ADDRESS + 0x00688)
    #define R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_DQS_SCALE_B1         Fld(2, 16) //[17:16]
    #define R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_DQ_SCALE_B1          Fld(2, 18) //[19:18]
    #define R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1    Fld(1, 23) //[23:23]
    #define R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_DQS_SCALE_B1         Fld(2, 24) //[25:24]
    #define R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_DQ_SCALE_B1          Fld(2, 26) //[27:26]
    #define R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1    Fld(1, 28) //[28:28]
    #define R0_B1_RXDVS2_R_RK0_DVS_FDLY_MODE_B1                Fld(1, 29) //[29:29]
    #define R0_B1_RXDVS2_R_RK0_DVS_MODE_B1                     Fld(2, 30) //[31:30]

#define DDRPHY_R0_B1_RXDVS7                         (DDRPHY_AO_BASE_ADDRESS + 0x0069C)
    #define R0_B1_RXDVS7_RG_RK0_ARDQ_MIN_DLY_B1                Fld(6, 0) //[5:0]
    #define R0_B1_RXDVS7_RG_RK0_ARDQ_MIN_DLY_B1_RFU            Fld(2, 6) //[7:6]
    #define R0_B1_RXDVS7_RG_RK0_ARDQ_MAX_DLY_B1                Fld(6, 8) //[13:8]
    #define R0_B1_RXDVS7_RG_RK0_ARDQ_MAX_DLY_B1_RFU            Fld(2, 14) //[15:14]
    #define R0_B1_RXDVS7_RG_RK0_ARDQS0_MIN_DLY_B1              Fld(7, 16) //[22:16]
    #define R0_B1_RXDVS7_RG_RK0_ARDQS0_MIN_DLY_B1_RFU          Fld(1, 23) //[23:23]
    #define R0_B1_RXDVS7_RG_RK0_ARDQS0_MAX_DLY_B1              Fld(7, 24) //[30:24]
    #define R0_B1_RXDVS7_RG_RK0_ARDQS0_MAX_DLY_B1_RFU          Fld(1, 31) //[31:31]

#define DDRPHY_RFU_0X6A0                            (DDRPHY_AO_BASE_ADDRESS + 0x006A0)
    #define RFU_0X6A0_RESERVED_0X6A0                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X6A4                            (DDRPHY_AO_BASE_ADDRESS + 0x006A4)
    #define RFU_0X6A4_RESERVED_0X6A4                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X6A8                            (DDRPHY_AO_BASE_ADDRESS + 0x006A8)
    #define RFU_0X6A8_RESERVED_0X6A8                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X6AC                            (DDRPHY_AO_BASE_ADDRESS + 0x006AC)
    #define RFU_0X6AC_RESERVED_0X6AC                           Fld(32, 0) //[31:0]

#define DDRPHY_CA_RXDVS0                            (DDRPHY_AO_BASE_ADDRESS + 0x006F0)
    #define CA_RXDVS0_R_RX_RANKINSEL_CA                        Fld(1, 0) //[0:0]
    #define CA_RXDVS0_CA_RXDVS0_RFU                            Fld(3, 1) //[3:1]
    #define CA_RXDVS0_R_RX_RANKINCTL_CA                        Fld(4, 4) //[7:4]
    #define CA_RXDVS0_R_DVS_SW_UP_CA                           Fld(1, 8) //[8:8]
    #define CA_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_CA                Fld(1, 9) //[9:9]
    #define CA_RXDVS0_R_DMRXDVS_PBYTESTUCK_RST_CA              Fld(1, 10) //[10:10]
    #define CA_RXDVS0_R_DMRXDVS_PBYTESTUCK_IG_CA               Fld(1, 11) //[11:11]
    #define CA_RXDVS0_R_DMRXDVS_DQIENPOST_OPT_CA               Fld(2, 12) //[13:12]
    #define CA_RXDVS0_R_RX_DLY_RANK_ERR_ST_CLR_CA              Fld(3, 16) //[18:16]
    #define CA_RXDVS0_R_DMRXDVS_CNTCMP_OPT_CA                  Fld(1, 19) //[19:19]
    #define CA_RXDVS0_R_RX_DLY_RK_OPT_CA                       Fld(2, 20) //[21:20]
    #define CA_RXDVS0_R_HWRESTORE_ENA_CA                       Fld(1, 22) //[22:22]
    #define CA_RXDVS0_R_HWSAVE_MODE_ENA_CA                     Fld(1, 24) //[24:24]
    #define CA_RXDVS0_R_RX_DLY_DVS_MODE_SYNC_DIS_CA            Fld(1, 26) //[26:26]
    #define CA_RXDVS0_R_RX_DLY_TRACK_BYPASS_MODESYNC_CA        Fld(1, 27) //[27:27]
    #define CA_RXDVS0_R_RX_DLY_TRACK_CG_EN_CA                  Fld(1, 28) //[28:28]
    #define CA_RXDVS0_R_RX_DLY_TRACK_SPM_CTRL_CA               Fld(1, 29) //[29:29]
    #define CA_RXDVS0_R_RX_DLY_TRACK_CLR_CA                    Fld(1, 30) //[30:30]
    #define CA_RXDVS0_R_RX_DLY_TRACK_ENA_CA                    Fld(1, 31) //[31:31]

#define DDRPHY_CA_RXDVS1                            (DDRPHY_AO_BASE_ADDRESS + 0x006F4)
    #define CA_RXDVS1_CA_RXDVS1_RFU                            Fld(16, 0) //[15:0]
    #define CA_RXDVS1_R_DMRXDVS_UPD_CLR_ACK_CA                 Fld(1, 16) //[16:16]
    #define CA_RXDVS1_R_DMRXDVS_UPD_CLR_NORD_CA                Fld(1, 17) //[17:17]

#define DDRPHY_RFU_0X6F8                            (DDRPHY_AO_BASE_ADDRESS + 0x006F8)
    #define RFU_0X6F8_RESERVED_0X6F8                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X6FC                            (DDRPHY_AO_BASE_ADDRESS + 0x006FC)
    #define RFU_0X6FC_RESERVED_0X6FC                           Fld(32, 0) //[31:0]

#define DDRPHY_R0_CA_RXDVS0                         (DDRPHY_AO_BASE_ADDRESS + 0x00700)
    #define R0_CA_RXDVS0_R_RK0_CA_DVS_LEAD_LAG_CNT_CLR         Fld(1, 26) //[26:26]
    #define R0_CA_RXDVS0_R_RK0_CA_DVS_SW_CNT_CLR               Fld(1, 27) //[27:27]
    #define R0_CA_RXDVS0_R_RK0_CA_DVS_SW_CNT_ENA               Fld(1, 31) //[31:31]

#define DDRPHY_R0_CA_RXDVS1                         (DDRPHY_AO_BASE_ADDRESS + 0x00704)
    #define R0_CA_RXDVS1_R_RK0_CA_DVS_TH_LAG                   Fld(16, 0) //[15:0]
    #define R0_CA_RXDVS1_R_RK0_CA_DVS_TH_LEAD                  Fld(16, 16) //[31:16]

#define DDRPHY_R0_CA_RXDVS2                         (DDRPHY_AO_BASE_ADDRESS + 0x00708)
    #define R0_CA_RXDVS2_R_RK0_RX_DLY_FAL_DQS_SCALE_CA         Fld(2, 16) //[17:16]
    #define R0_CA_RXDVS2_R_RK0_RX_DLY_FAL_DQ_SCALE_CA          Fld(2, 18) //[19:18]
    #define R0_CA_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_CA    Fld(1, 23) //[23:23]
    #define R0_CA_RXDVS2_R_RK0_RX_DLY_RIS_DQS_SCALE_CA         Fld(2, 24) //[25:24]
    #define R0_CA_RXDVS2_R_RK0_RX_DLY_RIS_DQ_SCALE_CA          Fld(2, 26) //[27:26]
    #define R0_CA_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_CA    Fld(1, 28) //[28:28]
    #define R0_CA_RXDVS2_R_RK0_DVS_FDLY_MODE_CA                Fld(1, 29) //[29:29]
    #define R0_CA_RXDVS2_R_RK0_DVS_MODE_CA                     Fld(2, 30) //[31:30]

#define DDRPHY_R0_CA_RXDVS9                         (DDRPHY_AO_BASE_ADDRESS + 0x00724)
    #define R0_CA_RXDVS9_RG_RK0_ARCMD_MIN_DLY                  Fld(6, 0) //[5:0]
    #define R0_CA_RXDVS9_RG_RK0_ARCMD_MIN_DLY_RFU              Fld(2, 6) //[7:6]
    #define R0_CA_RXDVS9_RG_RK0_ARCMD_MAX_DLY                  Fld(6, 8) //[13:8]
    #define R0_CA_RXDVS9_RG_RK0_ARCMD_MAX_DLY_RFU              Fld(2, 14) //[15:14]
    #define R0_CA_RXDVS9_RG_RK0_ARCLK_MIN_DLY                  Fld(7, 16) //[22:16]
    #define R0_CA_RXDVS9_RG_RK0_ARCLK_MIN_DLY_RFU              Fld(1, 23) //[23:23]
    #define R0_CA_RXDVS9_RG_RK0_ARCLK_MAX_DLY                  Fld(7, 24) //[30:24]
    #define R0_CA_RXDVS9_RG_RK0_ARCLK_MAX_DLY_RFU              Fld(1, 31) //[31:31]

#define DDRPHY_RFU_0X728                            (DDRPHY_AO_BASE_ADDRESS + 0x00728)
    #define RFU_0X728_RESERVED_0X728                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X72C                            (DDRPHY_AO_BASE_ADDRESS + 0x0072C)
    #define RFU_0X72C_RESERVED_0X72C                           Fld(32, 0) //[31:0]

#define DDRPHY_R1_B0_RXDVS0                         (DDRPHY_AO_BASE_ADDRESS + 0x00800)
    #define R1_B0_RXDVS0_R_RK1_B0_DVS_LEAD_LAG_CNT_CLR         Fld(1, 26) //[26:26]
    #define R1_B0_RXDVS0_R_RK1_B0_DVS_SW_CNT_CLR               Fld(1, 27) //[27:27]
    #define R1_B0_RXDVS0_R_RK1_B0_DVS_SW_CNT_ENA               Fld(1, 31) //[31:31]

#define DDRPHY_R1_B0_RXDVS1                         (DDRPHY_AO_BASE_ADDRESS + 0x00804)
    #define R1_B0_RXDVS1_R_RK1_B0_DVS_TH_LAG                   Fld(16, 0) //[15:0]
    #define R1_B0_RXDVS1_R_RK1_B0_DVS_TH_LEAD                  Fld(16, 16) //[31:16]

#define DDRPHY_R1_B0_RXDVS2                         (DDRPHY_AO_BASE_ADDRESS + 0x00808)
    #define R1_B0_RXDVS2_R_RK1_RX_DLY_FAL_DQS_SCALE_B0         Fld(2, 16) //[17:16]
    #define R1_B0_RXDVS2_R_RK1_RX_DLY_FAL_DQ_SCALE_B0          Fld(2, 18) //[19:18]
    #define R1_B0_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B0    Fld(1, 23) //[23:23]
    #define R1_B0_RXDVS2_R_RK1_RX_DLY_RIS_DQS_SCALE_B0         Fld(2, 24) //[25:24]
    #define R1_B0_RXDVS2_R_RK1_RX_DLY_RIS_DQ_SCALE_B0          Fld(2, 26) //[27:26]
    #define R1_B0_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B0    Fld(1, 28) //[28:28]
    #define R1_B0_RXDVS2_R_RK1_DVS_FDLY_MODE_B0                Fld(1, 29) //[29:29]
    #define R1_B0_RXDVS2_R_RK1_DVS_MODE_B0                     Fld(2, 30) //[31:30]

#define DDRPHY_R1_B0_RXDVS7                         (DDRPHY_AO_BASE_ADDRESS + 0x0081C)
    #define R1_B0_RXDVS7_RG_RK1_ARDQ_MIN_DLY_B0                Fld(6, 0) //[5:0]
    #define R1_B0_RXDVS7_RG_RK1_ARDQ_MIN_DLY_B0_RFU            Fld(2, 6) //[7:6]
    #define R1_B0_RXDVS7_RG_RK1_ARDQ_MAX_DLY_B0                Fld(6, 8) //[13:8]
    #define R1_B0_RXDVS7_RG_RK1_ARDQ_MAX_DLY_B0_RFU            Fld(2, 14) //[15:14]
    #define R1_B0_RXDVS7_RG_RK1_ARDQS0_MIN_DLY_B0              Fld(7, 16) //[22:16]
    #define R1_B0_RXDVS7_RG_RK1_ARDQS0_MIN_DLY_B0_RFU          Fld(1, 23) //[23:23]
    #define R1_B0_RXDVS7_RG_RK1_ARDQS0_MAX_DLY_B0              Fld(7, 24) //[30:24]
    #define R1_B0_RXDVS7_RG_RK1_ARDQS0_MAX_DLY_B0_RFU          Fld(1, 31) //[31:31]

#define DDRPHY_RFU_0X820                            (DDRPHY_AO_BASE_ADDRESS + 0x00820)
    #define RFU_0X820_RESERVED_0X820                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X824                            (DDRPHY_AO_BASE_ADDRESS + 0x00824)
    #define RFU_0X824_RESERVED_0X824                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X828                            (DDRPHY_AO_BASE_ADDRESS + 0x00828)
    #define RFU_0X828_RESERVED_0X828                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X82C                            (DDRPHY_AO_BASE_ADDRESS + 0x0082C)
    #define RFU_0X82C_RESERVED_0X82C                           Fld(32, 0) //[31:0]

#define DDRPHY_R1_B1_RXDVS0                         (DDRPHY_AO_BASE_ADDRESS + 0x00880)
    #define R1_B1_RXDVS0_R_RK1_B1_DVS_LEAD_LAG_CNT_CLR         Fld(1, 26) //[26:26]
    #define R1_B1_RXDVS0_R_RK1_B1_DVS_SW_CNT_CLR               Fld(1, 27) //[27:27]
    #define R1_B1_RXDVS0_R_RK1_B1_DVS_SW_CNT_ENA               Fld(1, 31) //[31:31]

#define DDRPHY_R1_B1_RXDVS1                         (DDRPHY_AO_BASE_ADDRESS + 0x00884)
    #define R1_B1_RXDVS1_R_RK1_B1_DVS_TH_LAG                   Fld(16, 0) //[15:0]
    #define R1_B1_RXDVS1_R_RK1_B1_DVS_TH_LEAD                  Fld(16, 16) //[31:16]

#define DDRPHY_R1_B1_RXDVS2                         (DDRPHY_AO_BASE_ADDRESS + 0x00888)
    #define R1_B1_RXDVS2_R_RK1_RX_DLY_FAL_DQS_SCALE_B1         Fld(2, 16) //[17:16]
    #define R1_B1_RXDVS2_R_RK1_RX_DLY_FAL_DQ_SCALE_B1          Fld(2, 18) //[19:18]
    #define R1_B1_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B1    Fld(1, 23) //[23:23]
    #define R1_B1_RXDVS2_R_RK1_RX_DLY_RIS_DQS_SCALE_B1         Fld(2, 24) //[25:24]
    #define R1_B1_RXDVS2_R_RK1_RX_DLY_RIS_DQ_SCALE_B1          Fld(2, 26) //[27:26]
    #define R1_B1_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B1    Fld(1, 28) //[28:28]
    #define R1_B1_RXDVS2_R_RK1_DVS_FDLY_MODE_B1                Fld(1, 29) //[29:29]
    #define R1_B1_RXDVS2_R_RK1_DVS_MODE_B1                     Fld(2, 30) //[31:30]

#define DDRPHY_R1_B1_RXDVS7                         (DDRPHY_AO_BASE_ADDRESS + 0x0089C)
    #define R1_B1_RXDVS7_RG_RK1_ARDQ_MIN_DLY_B1                Fld(6, 0) //[5:0]
    #define R1_B1_RXDVS7_RG_RK1_ARDQ_MIN_DLY_B1_RFU            Fld(2, 6) //[7:6]
    #define R1_B1_RXDVS7_RG_RK1_ARDQ_MAX_DLY_B1                Fld(6, 8) //[13:8]
    #define R1_B1_RXDVS7_RG_RK1_ARDQ_MAX_DLY_B1_RFU            Fld(2, 14) //[15:14]
    #define R1_B1_RXDVS7_RG_RK1_ARDQS0_MIN_DLY_B1              Fld(7, 16) //[22:16]
    #define R1_B1_RXDVS7_RG_RK1_ARDQS0_MIN_DLY_B1_RFU          Fld(1, 23) //[23:23]
    #define R1_B1_RXDVS7_RG_RK1_ARDQS0_MAX_DLY_B1              Fld(7, 24) //[30:24]
    #define R1_B1_RXDVS7_RG_RK1_ARDQS0_MAX_DLY_B1_RFU          Fld(1, 31) //[31:31]

#define DDRPHY_RFU_0X8A0                            (DDRPHY_AO_BASE_ADDRESS + 0x008A0)
    #define RFU_0X8A0_RESERVED_0X8A0                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X8A4                            (DDRPHY_AO_BASE_ADDRESS + 0x008A4)
    #define RFU_0X8A4_RESERVED_0X8A4                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X8A8                            (DDRPHY_AO_BASE_ADDRESS + 0x008A8)
    #define RFU_0X8A8_RESERVED_0X8A8                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X8AC                            (DDRPHY_AO_BASE_ADDRESS + 0x008AC)
    #define RFU_0X8AC_RESERVED_0X8AC                           Fld(32, 0) //[31:0]

#define DDRPHY_R1_CA_RXDVS0                         (DDRPHY_AO_BASE_ADDRESS + 0x00900)
    #define R1_CA_RXDVS0_R_RK1_CA_DVS_LEAD_LAG_CNT_CLR         Fld(1, 26) //[26:26]
    #define R1_CA_RXDVS0_R_RK1_CA_DVS_SW_CNT_CLR               Fld(1, 27) //[27:27]
    #define R1_CA_RXDVS0_R_RK1_CA_DVS_SW_CNT_ENA               Fld(1, 31) //[31:31]

#define DDRPHY_R1_CA_RXDVS1                         (DDRPHY_AO_BASE_ADDRESS + 0x00904)
    #define R1_CA_RXDVS1_R_RK1_CA_DVS_TH_LAG                   Fld(16, 0) //[15:0]
    #define R1_CA_RXDVS1_R_RK1_CA_DVS_TH_LEAD                  Fld(16, 16) //[31:16]

#define DDRPHY_R1_CA_RXDVS2                         (DDRPHY_AO_BASE_ADDRESS + 0x00908)
    #define R1_CA_RXDVS2_R_RK1_RX_DLY_FAL_DQS_SCALE_CA         Fld(2, 16) //[17:16]
    #define R1_CA_RXDVS2_R_RK1_RX_DLY_FAL_DQ_SCALE_CA          Fld(2, 18) //[19:18]
    #define R1_CA_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_CA    Fld(1, 23) //[23:23]
    #define R1_CA_RXDVS2_R_RK1_RX_DLY_RIS_DQS_SCALE_CA         Fld(2, 24) //[25:24]
    #define R1_CA_RXDVS2_R_RK1_RX_DLY_RIS_DQ_SCALE_CA          Fld(2, 26) //[27:26]
    #define R1_CA_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_CA    Fld(1, 28) //[28:28]
    #define R1_CA_RXDVS2_R_RK1_DVS_FDLY_MODE_CA                Fld(1, 29) //[29:29]
    #define R1_CA_RXDVS2_R_RK1_DVS_MODE_CA                     Fld(2, 30) //[31:30]

#define DDRPHY_R1_CA_RXDVS9                         (DDRPHY_AO_BASE_ADDRESS + 0x00924)
    #define R1_CA_RXDVS9_RG_RK1_ARCMD_MIN_DLY                  Fld(6, 0) //[5:0]
    #define R1_CA_RXDVS9_RG_RK1_ARCMD_MIN_DLY_RFU              Fld(2, 6) //[7:6]
    #define R1_CA_RXDVS9_RG_RK1_ARCMD_MAX_DLY                  Fld(6, 8) //[13:8]
    #define R1_CA_RXDVS9_RG_RK1_ARCMD_MAX_DLY_RFU              Fld(2, 14) //[15:14]
    #define R1_CA_RXDVS9_RG_RK1_ARCLK_MIN_DLY                  Fld(7, 16) //[22:16]
    #define R1_CA_RXDVS9_RG_RK1_ARCLK_MIN_DLY_RFU              Fld(1, 23) //[23:23]
    #define R1_CA_RXDVS9_RG_RK1_ARCLK_MAX_DLY                  Fld(7, 24) //[30:24]
    #define R1_CA_RXDVS9_RG_RK1_ARCLK_MAX_DLY_RFU              Fld(1, 31) //[31:31]

#define DDRPHY_RFU_0X928                            (DDRPHY_AO_BASE_ADDRESS + 0x00928)
    #define RFU_0X928_RESERVED_0X928                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X92C                            (DDRPHY_AO_BASE_ADDRESS + 0x0092C)
    #define RFU_0X92C_RESERVED_0X92C                           Fld(32, 0) //[31:0]

#define DDRPHY_R2_B0_RXDVS0                         (DDRPHY_AO_BASE_ADDRESS + 0x00A00)
    #define R2_B0_RXDVS0_R_RK2_B0_DVS_LEAD_LAG_CNT_CLR         Fld(1, 26) //[26:26]
    #define R2_B0_RXDVS0_R_RK2_B0_DVS_SW_CNT_CLR               Fld(1, 27) //[27:27]
    #define R2_B0_RXDVS0_R_RK2_B0_DVS_SW_CNT_ENA               Fld(1, 31) //[31:31]

#define DDRPHY_R2_B0_RXDVS1                         (DDRPHY_AO_BASE_ADDRESS + 0x00A04)
    #define R2_B0_RXDVS1_R_RK2_B0_DVS_TH_LAG                   Fld(16, 0) //[15:0]
    #define R2_B0_RXDVS1_R_RK2_B0_DVS_TH_LEAD                  Fld(16, 16) //[31:16]

#define DDRPHY_R2_B0_RXDVS2                         (DDRPHY_AO_BASE_ADDRESS + 0x00A08)
    #define R2_B0_RXDVS2_R_RK2_RX_DLY_FAL_DQS_SCALE_B0         Fld(2, 16) //[17:16]
    #define R2_B0_RXDVS2_R_RK2_RX_DLY_FAL_DQ_SCALE_B0          Fld(2, 18) //[19:18]
    #define R2_B0_RXDVS2_R_RK2_RX_DLY_FAL_TRACK_GATE_ENA_B0    Fld(1, 23) //[23:23]
    #define R2_B0_RXDVS2_R_RK2_RX_DLY_RIS_DQS_SCALE_B0         Fld(2, 24) //[25:24]
    #define R2_B0_RXDVS2_R_RK2_RX_DLY_RIS_DQ_SCALE_B0          Fld(2, 26) //[27:26]
    #define R2_B0_RXDVS2_R_RK2_RX_DLY_RIS_TRACK_GATE_ENA_B0    Fld(1, 28) //[28:28]
    #define R2_B0_RXDVS2_R_RK2_DVS_FDLY_MODE_B0                Fld(1, 29) //[29:29]
    #define R2_B0_RXDVS2_R_RK2_DVS_MODE_B0                     Fld(2, 30) //[31:30]

#define DDRPHY_R2_B0_RXDVS7                         (DDRPHY_AO_BASE_ADDRESS + 0x00A1C)
    #define R2_B0_RXDVS7_RG_RK2_ARDQ_MIN_DLY_B0                Fld(6, 0) //[5:0]
    #define R2_B0_RXDVS7_RG_RK2_ARDQ_MIN_DLY_B0_RFU            Fld(2, 6) //[7:6]
    #define R2_B0_RXDVS7_RG_RK2_ARDQ_MAX_DLY_B0                Fld(6, 8) //[13:8]
    #define R2_B0_RXDVS7_RG_RK2_ARDQ_MAX_DLY_B0_RFU            Fld(2, 14) //[15:14]
    #define R2_B0_RXDVS7_RG_RK2_ARDQS0_MIN_DLY_B0              Fld(7, 16) //[22:16]
    #define R2_B0_RXDVS7_RG_RK2_ARDQS0_MIN_DLY_B0_RFU          Fld(1, 23) //[23:23]
    #define R2_B0_RXDVS7_RG_RK2_ARDQS0_MAX_DLY_B0              Fld(7, 24) //[30:24]
    #define R2_B0_RXDVS7_RG_RK2_ARDQS0_MAX_DLY_B0_RFU          Fld(1, 31) //[31:31]

#define DDRPHY_RFU_0XA20                            (DDRPHY_AO_BASE_ADDRESS + 0x00A20)
    #define RFU_0XA20_RESERVED_0XA20                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0XA24                            (DDRPHY_AO_BASE_ADDRESS + 0x00A24)
    #define RFU_0XA24_RESERVED_0XA24                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0XA28                            (DDRPHY_AO_BASE_ADDRESS + 0x00A28)
    #define RFU_0XA28_RESERVED_0XA28                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0XA2C                            (DDRPHY_AO_BASE_ADDRESS + 0x00A2C)
    #define RFU_0XA2C_RESERVED_0XA2C                           Fld(32, 0) //[31:0]

#define DDRPHY_R2_B1_RXDVS0                         (DDRPHY_AO_BASE_ADDRESS + 0x00A80)
    #define R2_B1_RXDVS0_R_RK2_B1_DVS_LEAD_LAG_CNT_CLR         Fld(1, 26) //[26:26]
    #define R2_B1_RXDVS0_R_RK2_B1_DVS_SW_CNT_CLR               Fld(1, 27) //[27:27]
    #define R2_B1_RXDVS0_R_RK2_B1_DVS_SW_CNT_ENA               Fld(1, 31) //[31:31]

#define DDRPHY_R2_B1_RXDVS1                         (DDRPHY_AO_BASE_ADDRESS + 0x00A84)
    #define R2_B1_RXDVS1_R_RK2_B1_DVS_TH_LAG                   Fld(16, 0) //[15:0]
    #define R2_B1_RXDVS1_R_RK2_B1_DVS_TH_LEAD                  Fld(16, 16) //[31:16]

#define DDRPHY_R2_B1_RXDVS2                         (DDRPHY_AO_BASE_ADDRESS + 0x00A88)
    #define R2_B1_RXDVS2_R_RK2_RX_DLY_FAL_DQS_SCALE_B1         Fld(2, 16) //[17:16]
    #define R2_B1_RXDVS2_R_RK2_RX_DLY_FAL_DQ_SCALE_B1          Fld(2, 18) //[19:18]
    #define R2_B1_RXDVS2_R_RK2_RX_DLY_FAL_TRACK_GATE_ENA_B1    Fld(1, 23) //[23:23]
    #define R2_B1_RXDVS2_R_RK2_RX_DLY_RIS_DQS_SCALE_B1         Fld(2, 24) //[25:24]
    #define R2_B1_RXDVS2_R_RK2_RX_DLY_RIS_DQ_SCALE_B1          Fld(2, 26) //[27:26]
    #define R2_B1_RXDVS2_R_RK2_RX_DLY_RIS_TRACK_GATE_ENA_B1    Fld(1, 28) //[28:28]
    #define R2_B1_RXDVS2_R_RK2_DVS_FDLY_MODE_B1                Fld(1, 29) //[29:29]
    #define R2_B1_RXDVS2_R_RK2_DVS_MODE_B1                     Fld(2, 30) //[31:30]

#define DDRPHY_R2_B1_RXDVS7                         (DDRPHY_AO_BASE_ADDRESS + 0x00A9C)
    #define R2_B1_RXDVS7_RG_RK2_ARDQ_MIN_DLY_B1                Fld(6, 0) //[5:0]
    #define R2_B1_RXDVS7_RG_RK2_ARDQ_MIN_DLY_B1_RFU            Fld(2, 6) //[7:6]
    #define R2_B1_RXDVS7_RG_RK2_ARDQ_MAX_DLY_B1                Fld(6, 8) //[13:8]
    #define R2_B1_RXDVS7_RG_RK2_ARDQ_MAX_DLY_B1_RFU            Fld(2, 14) //[15:14]
    #define R2_B1_RXDVS7_RG_RK2_ARDQS0_MIN_DLY_B1              Fld(7, 16) //[22:16]
    #define R2_B1_RXDVS7_RG_RK2_ARDQS0_MIN_DLY_B1_RFU          Fld(1, 23) //[23:23]
    #define R2_B1_RXDVS7_RG_RK2_ARDQS0_MAX_DLY_B1              Fld(7, 24) //[30:24]
    #define R2_B1_RXDVS7_RG_RK2_ARDQS0_MAX_DLY_B1_RFU          Fld(1, 31) //[31:31]

#define DDRPHY_RFU_0XAA0                            (DDRPHY_AO_BASE_ADDRESS + 0x00AA0)
    #define RFU_0XAA0_RESERVED_0XAA0                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0XAA4                            (DDRPHY_AO_BASE_ADDRESS + 0x00AA4)
    #define RFU_0XAA4_RESERVED_0XAA4                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0XAA8                            (DDRPHY_AO_BASE_ADDRESS + 0x00AA8)
    #define RFU_0XAA8_RESERVED_0XAA8                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0XAAC                            (DDRPHY_AO_BASE_ADDRESS + 0x00AAC)
    #define RFU_0XAAC_RESERVED_0XAAC                           Fld(32, 0) //[31:0]

#define DDRPHY_R2_CA_RXDVS0                         (DDRPHY_AO_BASE_ADDRESS + 0x00B00)
    #define R2_CA_RXDVS0_R_RK2_CA_DVS_LEAD_LAG_CNT_CLR         Fld(1, 26) //[26:26]
    #define R2_CA_RXDVS0_R_RK2_CA_DVS_SW_CNT_CLR               Fld(1, 27) //[27:27]
    #define R2_CA_RXDVS0_R_RK2_CA_DVS_SW_CNT_ENA               Fld(1, 31) //[31:31]

#define DDRPHY_R2_CA_RXDVS1                         (DDRPHY_AO_BASE_ADDRESS + 0x00B04)
    #define R2_CA_RXDVS1_R_RK2_CA_DVS_TH_LAG                   Fld(16, 0) //[15:0]
    #define R2_CA_RXDVS1_R_RK2_CA_DVS_TH_LEAD                  Fld(16, 16) //[31:16]

#define DDRPHY_R2_CA_RXDVS2                         (DDRPHY_AO_BASE_ADDRESS + 0x00B08)
    #define R2_CA_RXDVS2_R_RK2_RX_DLY_FAL_DQS_SCALE_CA         Fld(2, 16) //[17:16]
    #define R2_CA_RXDVS2_R_RK2_RX_DLY_FAL_DQ_SCALE_CA          Fld(2, 18) //[19:18]
    #define R2_CA_RXDVS2_R_RK2_RX_DLY_FAL_TRACK_GATE_ENA_CA    Fld(1, 23) //[23:23]
    #define R2_CA_RXDVS2_R_RK2_RX_DLY_RIS_DQS_SCALE_CA         Fld(2, 24) //[25:24]
    #define R2_CA_RXDVS2_R_RK2_RX_DLY_RIS_DQ_SCALE_CA          Fld(2, 26) //[27:26]
    #define R2_CA_RXDVS2_R_RK2_RX_DLY_RIS_TRACK_GATE_ENA_CA    Fld(1, 28) //[28:28]
    #define R2_CA_RXDVS2_R_RK2_DVS_FDLY_MODE_CA                Fld(1, 29) //[29:29]
    #define R2_CA_RXDVS2_R_RK2_DVS_MODE_CA                     Fld(2, 30) //[31:30]

#define DDRPHY_R2_CA_RXDVS9                         (DDRPHY_AO_BASE_ADDRESS + 0x00B24)
    #define R2_CA_RXDVS9_RG_RK2_ARCMD_MIN_DLY                  Fld(6, 0) //[5:0]
    #define R2_CA_RXDVS9_RG_RK2_ARCMD_MIN_DLY_RFU              Fld(2, 6) //[7:6]
    #define R2_CA_RXDVS9_RG_RK2_ARCMD_MAX_DLY                  Fld(6, 8) //[13:8]
    #define R2_CA_RXDVS9_RG_RK2_ARCMD_MAX_DLY_RFU              Fld(2, 14) //[15:14]
    #define R2_CA_RXDVS9_RG_RK2_ARCLK_MIN_DLY                  Fld(7, 16) //[22:16]
    #define R2_CA_RXDVS9_RG_RK2_ARCLK_MIN_DLY_RFU              Fld(1, 23) //[23:23]
    #define R2_CA_RXDVS9_RG_RK2_ARCLK_MAX_DLY                  Fld(7, 24) //[30:24]
    #define R2_CA_RXDVS9_RG_RK2_ARCLK_MAX_DLY_RFU              Fld(1, 31) //[31:31]

#define DDRPHY_RFU_0XB28                            (DDRPHY_AO_BASE_ADDRESS + 0x00B28)
    #define RFU_0XB28_RESERVED_0XB28                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0XB2C                            (DDRPHY_AO_BASE_ADDRESS + 0x00B2C)
    #define RFU_0XB2C_RESERVED_0XB2C                           Fld(32, 0) //[31:0]

#define DDRPHY_SHU1_B0_DQ0                          (DDRPHY_AO_BASE_ADDRESS + 0x00C00)
    #define SHU1_B0_DQ0_RG_TX_ARDQS0_PRE_EN_B0                 Fld(1, 4) //[4:4]
    #define SHU1_B0_DQ0_RG_TX_ARDQS0_DRVP_PRE_B0               Fld(3, 8) //[10:8]
    #define SHU1_B0_DQ0_RG_TX_ARDQS0_DRVN_PRE_B0               Fld(3, 12) //[14:12]
    #define SHU1_B0_DQ0_RG_TX_ARDQ_PRE_EN_B0                   Fld(1, 20) //[20:20]
    #define SHU1_B0_DQ0_RG_TX_ARDQ_DRVP_PRE_B0                 Fld(3, 24) //[26:24]
    #define SHU1_B0_DQ0_RG_TX_ARDQ_DRVN_PRE_B0                 Fld(3, 28) //[30:28]
    #define SHU1_B0_DQ0_R_LP4Y_WDN_MODE_DQS0                   Fld(1, 31) //[31:31]
    #define SHU1_B0_DQ0_RG_TX_ARDQS0_DRVP_PRE_B0_BIT2          Fld(1, 10)//[10:10]	 //Francis added
    #define SHU1_B0_DQ0_RG_TX_ARDQS0_DRVP_PRE_B0_BIT1          Fld(1, 9)//[9:9]    //Francis added
    #define SHU1_B0_DQ0_RG_TX_ARDQS0_DRVP_PRE_B0_BIT0          Fld(1, 8)//[8:8]    //Francis added

#define DDRPHY_SHU1_B0_DQ1                          (DDRPHY_AO_BASE_ADDRESS + 0x00C04)
    #define SHU1_B0_DQ1_RG_TX_ARDQ_DRVP_B0                     Fld(5, 0) //[4:0]
    #define SHU1_B0_DQ1_RG_TX_ARDQ_DRVN_B0                     Fld(5, 8) //[12:8]
    #define SHU1_B0_DQ1_RG_TX_ARDQ_ODTP_B0                     Fld(5, 16) //[20:16]
    #define SHU1_B0_DQ1_RG_TX_ARDQ_ODTN_B0                     Fld(5, 24) //[28:24]

#define DDRPHY_SHU1_B0_DQ2                          (DDRPHY_AO_BASE_ADDRESS + 0x00C08)
    #define SHU1_B0_DQ2_RG_TX_ARDQS0_DRVP_B0                   Fld(5, 0) //[4:0]
    #define SHU1_B0_DQ2_RG_TX_ARDQS0_DRVN_B0                   Fld(5, 8) //[12:8]
    #define SHU1_B0_DQ2_RG_TX_ARDQS0_ODTP_B0                   Fld(5, 16) //[20:16]
    #define SHU1_B0_DQ2_RG_TX_ARDQS0_ODTN_B0                   Fld(5, 24) //[28:24]

#define DDRPHY_SHU1_B0_DQ3                          (DDRPHY_AO_BASE_ADDRESS + 0x00C0C)
    #define SHU1_B0_DQ3_RG_TX_ARDQS0_PU_B0                     Fld(2, 0) //[1:0]
    #define SHU1_B0_DQ3_RG_TX_ARDQS0_PU_PRE_B0                 Fld(2, 2) //[3:2]
    #define SHU1_B0_DQ3_RG_TX_ARDQS0_PDB_B0                    Fld(2, 4) //[5:4]
    #define SHU1_B0_DQ3_RG_TX_ARDQS0_PDB_PRE_B0                Fld(2, 6) //[7:6]
    #define SHU1_B0_DQ3_RG_TX_ARDQ_PU_B0                       Fld(2, 8) //[9:8]
    #define SHU1_B0_DQ3_RG_TX_ARDQ_PU_PRE_B0                   Fld(2, 10) //[11:10]
    #define SHU1_B0_DQ3_RG_TX_ARDQ_PDB_B0                      Fld(2, 12) //[13:12]
    #define SHU1_B0_DQ3_RG_TX_ARDQ_PDB_PRE_B0                  Fld(2, 14) //[15:14]

#define DDRPHY_SHU1_B0_DQ4                          (DDRPHY_AO_BASE_ADDRESS + 0x00C10)
    #define SHU1_B0_DQ4_RG_ARPI_AA_MCK_DL_B0                   Fld(6, 0) //[5:0]
    #define SHU1_B0_DQ4_RG_ARPI_AA_MCK_FB_DL_B0                Fld(6, 8) //[13:8]
    #define SHU1_B0_DQ4_RG_ARPI_DA_MCK_FB_DL_B0                Fld(6, 16) //[21:16]

#define DDRPHY_SHU1_B0_DQ5                          (DDRPHY_AO_BASE_ADDRESS + 0x00C14)
    #define SHU1_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0                 Fld(6, 0) //[5:0]
    #define SHU1_B0_DQ5_RG_RX_ARDQ_VREF_BYPASS_B0              Fld(1, 6) //[6:6]
    #define SHU1_B0_DQ5_RG_ARPI_FB_B0                          Fld(6, 8) //[13:8]
    #define SHU1_B0_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B0             Fld(3, 16) //[18:16]
    #define SHU1_B0_DQ5_DA_RX_ARDQS_DQSIEN_RB_DLY_B0           Fld(1, 19) //[19:19]
    #define SHU1_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0                Fld(3, 20) //[22:20]
    #define SHU1_B0_DQ5_RG_ARPI_MCTL_B0                        Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_B0_DQ6                          (DDRPHY_AO_BASE_ADDRESS + 0x00C18)
    #define SHU1_B0_DQ6_RG_ARPI_OFFSET_DQSIEN_B0               Fld(6, 0) //[5:0]
    #define SHU1_B0_DQ6_RG_ARPI_RESERVE_B0                     Fld(16, 6) //[21:6]
    #define SHU1_B0_DQ6_RG_ARPI_MIDPI_CAP_SEL_B0               Fld(2, 22) //[23:22]
    #define SHU1_B0_DQ6_RG_ARPI_MIDPI_VTH_SEL_B0               Fld(2, 24) //[25:24]
    #define SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0                    Fld(1, 26) //[26:26]
    #define SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0             Fld(1, 27) //[27:27]
    #define SHU1_B0_DQ6_RG_ARPI_CAP_SEL_B0                     Fld(2, 28) //[29:28]
    #define SHU1_B0_DQ6_RG_ARPI_MIDPI_BYPASS_EN_B0             Fld(1, 31) //[31:31]

#define DDRPHY_SHU1_B0_DQ7                          (DDRPHY_AO_BASE_ADDRESS + 0x00C1C)
    #define SHU1_B0_DQ7_R_DMRANKRXDVS_B0                       Fld(4, 0) //[3:0]
    #define SHU1_B0_DQ7_MIDPI_ENABLE                           Fld(1, 4) //[4:4]
    #define SHU1_B0_DQ7_MIDPI_DIV4_ENABLE                      Fld(1, 5) //[5:5]
    #define SHU1_B0_DQ7_R_DMDQMDBI_EYE_SHU_B0                  Fld(1, 6) //[6:6]
    #define SHU1_B0_DQ7_R_DMDQMDBI_SHU_B0                      Fld(1, 7) //[7:7]
    #define SHU1_B0_DQ7_R_DMRXDVS_DQM_FLAGSEL_B0               Fld(4, 8) //[11:8]
    #define SHU1_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0            Fld(1, 12) //[12:12]
    #define SHU1_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0              Fld(1, 13) //[13:13]
    #define SHU1_B0_DQ7_R_DMRXTRACK_DQM_EN_B0                  Fld(1, 14) //[14:14]
    #define SHU1_B0_DQ7_R_DMRODTEN_B0                          Fld(1, 15) //[15:15]
    #define SHU1_B0_DQ7_R_DMARPI_CG_FB2DLL_DCM_EN_B0           Fld(1, 16) //[16:16]
    #define SHU1_B0_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B0               Fld(1, 17) //[17:17]
    #define SHU1_B0_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B0              Fld(1, 18) //[18:18]
    #define SHU1_B0_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B0              Fld(1, 19) //[19:19]
    #define SHU1_B0_DQ7_R_LP4Y_SDN_MODE_DQS0                   Fld(1, 20) //[20:20]
    #define SHU1_B0_DQ7_R_DMRXRANK_DQ_EN_B0                    Fld(1, 24) //[24:24]
    #define SHU1_B0_DQ7_R_DMRXRANK_DQ_LAT_B0                   Fld(3, 25) //[27:25]
    #define SHU1_B0_DQ7_R_DMRXRANK_DQS_EN_B0                   Fld(1, 28) //[28:28]
    #define SHU1_B0_DQ7_R_DMRXRANK_DQS_LAT_B0                  Fld(3, 29) //[31:29]

#define DDRPHY_SHU1_B0_DQ8                          (DDRPHY_AO_BASE_ADDRESS + 0x00C20)
    #define SHU1_B0_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B0             Fld(15, 0) //[14:0]
    #define SHU1_B0_DQ8_R_DMRXDVS_UPD_FORCE_EN_B0              Fld(1, 15) //[15:15]
    #define SHU1_B0_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B0           Fld(1, 19) //[19:19]
    #define SHU1_B0_DQ8_R_RMRODTEN_CG_IG_B0                    Fld(1, 20) //[20:20]
    #define SHU1_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0                  Fld(1, 21) //[21:21]
    #define SHU1_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0          Fld(1, 22) //[22:22]
    #define SHU1_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0      Fld(1, 23) //[23:23]
    #define SHU1_B0_DQ8_R_DMRXDLY_CG_IG_B0                     Fld(1, 24) //[24:24]
    #define SHU1_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0                Fld(1, 25) //[25:25]
    #define SHU1_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0          Fld(1, 26) //[26:26]
    #define SHU1_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0          Fld(1, 27) //[27:27]
    #define SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0         Fld(1, 28) //[28:28]
    #define SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0     Fld(1, 29) //[29:29]
    #define SHU1_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0                 Fld(1, 30) //[30:30]
    #define SHU1_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0             Fld(1, 31) //[31:31]

#define DDRPHY_SHU1_B0_DQ9                          (DDRPHY_AO_BASE_ADDRESS + 0x00C24)
    #define SHU1_B0_DQ9_RESERVED_0XC24                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU1_B0_DQ10                         (DDRPHY_AO_BASE_ADDRESS + 0x00C28)
    #define SHU1_B0_DQ10_RESERVED_0XC28                        Fld(32, 0) //[31:0]

#define DDRPHY_SHU1_B0_DQ11                         (DDRPHY_AO_BASE_ADDRESS + 0x00C2C)
    #define SHU1_B0_DQ11_RESERVED_0XC2C                        Fld(32, 0) //[31:0]

#define DDRPHY_SHU1_B0_DQ12                         (DDRPHY_AO_BASE_ADDRESS + 0x00C30)
    #define SHU1_B0_DQ12_RESERVED_0XC30                        Fld(32, 0) //[31:0]

#define DDRPHY_SHU1_B0_DLL0                         (DDRPHY_AO_BASE_ADDRESS + 0x00C34)
    #define SHU1_B0_DLL0_RG_ARPISM_MCK_SEL_B0_SHU              Fld(1, 0) //[0:0]
    #define SHU1_B0_DLL0_B0_DLL0_RFU                           Fld(1, 3) //[3:3]
    #define SHU1_B0_DLL0_RG_ARDLL_FAST_PSJP_B0                 Fld(1, 4) //[4:4]
    #define SHU1_B0_DLL0_RG_ARDLL_PHDIV_B0                     Fld(1, 9) //[9:9]
    #define SHU1_B0_DLL0_RG_ARDLL_PHJUMP_EN_B0                 Fld(1, 10) //[10:10]
    #define SHU1_B0_DLL0_RG_ARDLL_P_GAIN_B0                    Fld(4, 12) //[15:12]
    #define SHU1_B0_DLL0_RG_ARDLL_IDLECNT_B0                   Fld(4, 16) //[19:16]
    #define SHU1_B0_DLL0_RG_ARDLL_GAIN_B0                      Fld(4, 20) //[23:20]
    #define SHU1_B0_DLL0_RG_ARDLL_PHDET_IN_SWAP_B0             Fld(1, 30) //[30:30]
    #define SHU1_B0_DLL0_RG_ARDLL_PHDET_OUT_SEL_B0             Fld(1, 31) //[31:31]

#define DDRPHY_SHU1_B0_DLL1                         (DDRPHY_AO_BASE_ADDRESS + 0x00C38)
    #define SHU1_B0_DLL1_RG_ARDLL_FASTPJ_CK_SEL_B0             Fld(1, 0) //[0:0]
    #define SHU1_B0_DLL1_RG_ARDLL_PS_EN_B0                     Fld(1, 1) //[1:1]
    #define SHU1_B0_DLL1_RG_ARDLL_PD_CK_SEL_B0                 Fld(1, 2) //[2:2]
    #define SHU1_B0_DLL1_RG_ARDQ_REV_B0                        Fld(24, 8) //[31:8]

#define DDRPHY_SHU1_B1_DQ0                          (DDRPHY_AO_BASE_ADDRESS + 0x00C80)
    #define SHU1_B1_DQ0_RG_TX_ARDQS0_PRE_EN_B1                 Fld(1, 4) //[4:4]
    #define SHU1_B1_DQ0_RG_TX_ARDQS0_DRVP_PRE_B1               Fld(3, 8) //[10:8]
    #define SHU1_B1_DQ0_RG_TX_ARDQS0_DRVN_PRE_B1               Fld(3, 12) //[14:12]
    #define SHU1_B1_DQ0_RG_TX_ARDQ_PRE_EN_B1                   Fld(1, 20) //[20:20]
    #define SHU1_B1_DQ0_RG_TX_ARDQ_DRVP_PRE_B1                 Fld(3, 24) //[26:24]
    #define SHU1_B1_DQ0_RG_TX_ARDQ_DRVN_PRE_B1                 Fld(3, 28) //[30:28]
    #define SHU1_B1_DQ0_R_LP4Y_WDN_MODE_DQS1                   Fld(1, 31) //[31:31]
    #define SHU1_B1_DQ0_RG_TX_ARDQS0_DRVP_PRE_B1_BIT2 Fld(1, 10)//[10:10]	 //Francis added
    #define SHU1_B1_DQ0_RG_TX_ARDQS0_DRVP_PRE_B1_BIT1 Fld(1, 9)//[9:9]    //Francis added
    #define SHU1_B1_DQ0_RG_TX_ARDQS0_DRVP_PRE_B1_BIT0 Fld(1, 8)//[8:8]    //Francis added

#define DDRPHY_SHU1_B1_DQ1                          (DDRPHY_AO_BASE_ADDRESS + 0x00C84)
    #define SHU1_B1_DQ1_RG_TX_ARDQ_DRVP_B1                     Fld(5, 0) //[4:0]
    #define SHU1_B1_DQ1_RG_TX_ARDQ_DRVN_B1                     Fld(5, 8) //[12:8]
    #define SHU1_B1_DQ1_RG_TX_ARDQ_ODTP_B1                     Fld(5, 16) //[20:16]
    #define SHU1_B1_DQ1_RG_TX_ARDQ_ODTN_B1                     Fld(5, 24) //[28:24]

#define DDRPHY_SHU1_B1_DQ2                          (DDRPHY_AO_BASE_ADDRESS + 0x00C88)
    #define SHU1_B1_DQ2_RG_TX_ARDQS0_DRVP_B1                   Fld(5, 0) //[4:0]
    #define SHU1_B1_DQ2_RG_TX_ARDQS0_DRVN_B1                   Fld(5, 8) //[12:8]
    #define SHU1_B1_DQ2_RG_TX_ARDQS0_ODTP_B1                   Fld(5, 16) //[20:16]
    #define SHU1_B1_DQ2_RG_TX_ARDQS0_ODTN_B1                   Fld(5, 24) //[28:24]

#define DDRPHY_SHU1_B1_DQ3                          (DDRPHY_AO_BASE_ADDRESS + 0x00C8C)
    #define SHU1_B1_DQ3_RG_TX_ARDQS0_PU_B1                     Fld(2, 0) //[1:0]
    #define SHU1_B1_DQ3_RG_TX_ARDQS0_PU_PRE_B1                 Fld(2, 2) //[3:2]
    #define SHU1_B1_DQ3_RG_TX_ARDQS0_PDB_B1                    Fld(2, 4) //[5:4]
    #define SHU1_B1_DQ3_RG_TX_ARDQS0_PDB_PRE_B1                Fld(2, 6) //[7:6]
    #define SHU1_B1_DQ3_RG_TX_ARDQ_PU_B1                       Fld(2, 8) //[9:8]
    #define SHU1_B1_DQ3_RG_TX_ARDQ_PU_PRE_B1                   Fld(2, 10) //[11:10]
    #define SHU1_B1_DQ3_RG_TX_ARDQ_PDB_B1                      Fld(2, 12) //[13:12]
    #define SHU1_B1_DQ3_RG_TX_ARDQ_PDB_PRE_B1                  Fld(2, 14) //[15:14]

#define DDRPHY_SHU1_B1_DQ4                          (DDRPHY_AO_BASE_ADDRESS + 0x00C90)
    #define SHU1_B1_DQ4_RG_ARPI_AA_MCK_DL_B1                   Fld(6, 0) //[5:0]
    #define SHU1_B1_DQ4_RG_ARPI_AA_MCK_FB_DL_B1                Fld(6, 8) //[13:8]
    #define SHU1_B1_DQ4_RG_ARPI_DA_MCK_FB_DL_B1                Fld(6, 16) //[21:16]

#define DDRPHY_SHU1_B1_DQ5                          (DDRPHY_AO_BASE_ADDRESS + 0x00C94)
    #define SHU1_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1                 Fld(6, 0) //[5:0]
    #define SHU1_B1_DQ5_RG_RX_ARDQ_VREF_BYPASS_B1              Fld(1, 6) //[6:6]
    #define SHU1_B1_DQ5_RG_ARPI_FB_B1                          Fld(6, 8) //[13:8]
    #define SHU1_B1_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B1             Fld(3, 16) //[18:16]
    #define SHU1_B1_DQ5_DA_RX_ARDQS_DQSIEN_RB_DLY_B1           Fld(1, 19) //[19:19]
    #define SHU1_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1                Fld(3, 20) //[22:20]
    #define SHU1_B1_DQ5_RG_ARPI_MCTL_B1                        Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_B1_DQ6                          (DDRPHY_AO_BASE_ADDRESS + 0x00C98)
    #define SHU1_B1_DQ6_RG_ARPI_OFFSET_DQSIEN_B1               Fld(6, 0) //[5:0]
    #define SHU1_B1_DQ6_RG_ARPI_RESERVE_B1                     Fld(16, 6) //[21:6]
    #define SHU1_B1_DQ6_RG_ARPI_MIDPI_CAP_SEL_B1               Fld(2, 22) //[23:22]
    #define SHU1_B1_DQ6_RG_ARPI_MIDPI_VTH_SEL_B1               Fld(2, 24) //[25:24]
    #define SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1                    Fld(1, 26) //[26:26]
    #define SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1             Fld(1, 27) //[27:27]
    #define SHU1_B1_DQ6_RG_ARPI_CAP_SEL_B1                     Fld(2, 28) //[29:28]
    #define SHU1_B1_DQ6_RG_ARPI_MIDPI_BYPASS_EN_B1             Fld(1, 31) //[31:31]

#define DDRPHY_SHU1_B1_DQ7                          (DDRPHY_AO_BASE_ADDRESS + 0x00C9C)
    #define SHU1_B1_DQ7_R_DMRANKRXDVS_B1                       Fld(4, 0) //[3:0]
    #define SHU1_B1_DQ7_R_DMDQMDBI_EYE_SHU_B1                  Fld(1, 6) //[6:6]
    #define SHU1_B1_DQ7_R_DMDQMDBI_SHU_B1                      Fld(1, 7) //[7:7]
    #define SHU1_B1_DQ7_R_DMRXDVS_DQM_FLAGSEL_B1               Fld(4, 8) //[11:8]
    #define SHU1_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1            Fld(1, 12) //[12:12]
    #define SHU1_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1              Fld(1, 13) //[13:13]
    #define SHU1_B1_DQ7_R_DMRXTRACK_DQM_EN_B1                  Fld(1, 14) //[14:14]
    #define SHU1_B1_DQ7_R_DMRODTEN_B1                          Fld(1, 15) //[15:15]
    #define SHU1_B1_DQ7_R_DMARPI_CG_FB2DLL_DCM_EN_B1           Fld(1, 16) //[16:16]
    #define SHU1_B1_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B1               Fld(1, 17) //[17:17]
    #define SHU1_B1_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B1              Fld(1, 18) //[18:18]
    #define SHU1_B1_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B1              Fld(1, 19) //[19:19]
    #define SHU1_B1_DQ7_R_LP4Y_SDN_MODE_DQS1                   Fld(1, 20) //[20:20]
    #define SHU1_B1_DQ7_R_DMRXRANK_DQ_EN_B1                    Fld(1, 24) //[24:24]
    #define SHU1_B1_DQ7_R_DMRXRANK_DQ_LAT_B1                   Fld(3, 25) //[27:25]
    #define SHU1_B1_DQ7_R_DMRXRANK_DQS_EN_B1                   Fld(1, 28) //[28:28]
    #define SHU1_B1_DQ7_R_DMRXRANK_DQS_LAT_B1                  Fld(3, 29) //[31:29]

#define DDRPHY_SHU1_B1_DQ8                          (DDRPHY_AO_BASE_ADDRESS + 0x00CA0)
    #define SHU1_B1_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B1             Fld(15, 0) //[14:0]
    #define SHU1_B1_DQ8_R_DMRXDVS_UPD_FORCE_EN_B1              Fld(1, 15) //[15:15]
    #define SHU1_B1_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B1           Fld(1, 19) //[19:19]
    #define SHU1_B1_DQ8_R_RMRODTEN_CG_IG_B1                    Fld(1, 20) //[20:20]
    #define SHU1_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1                  Fld(1, 21) //[21:21]
    #define SHU1_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1          Fld(1, 22) //[22:22]
    #define SHU1_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1      Fld(1, 23) //[23:23]
    #define SHU1_B1_DQ8_R_DMRXDLY_CG_IG_B1                     Fld(1, 24) //[24:24]
    #define SHU1_B1_DQ8_R_DMSTBEN_SYNC_CG_IG_B1                Fld(1, 25) //[25:25]
    #define SHU1_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1          Fld(1, 26) //[26:26]
    #define SHU1_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1          Fld(1, 27) //[27:27]
    #define SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1         Fld(1, 28) //[28:28]
    #define SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1     Fld(1, 29) //[29:29]
    #define SHU1_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1                 Fld(1, 30) //[30:30]
    #define SHU1_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1             Fld(1, 31) //[31:31]

#define DDRPHY_SHU1_B1_DQ9                          (DDRPHY_AO_BASE_ADDRESS + 0x00CA4)
    #define SHU1_B1_DQ9_RESERVED_0XCA4                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU1_B1_DQ10                         (DDRPHY_AO_BASE_ADDRESS + 0x00CA8)
    #define SHU1_B1_DQ10_RESERVED_0XCA8                        Fld(32, 0) //[31:0]

#define DDRPHY_SHU1_B1_DQ11                         (DDRPHY_AO_BASE_ADDRESS + 0x00CAC)
    #define SHU1_B1_DQ11_RESERVED_0XCAC                        Fld(32, 0) //[31:0]

#define DDRPHY_SHU1_B1_DQ12                         (DDRPHY_AO_BASE_ADDRESS + 0x00CB0)
    #define SHU1_B1_DQ12_RESERVED_0XCB0                        Fld(32, 0) //[31:0]

#define DDRPHY_SHU1_B1_DLL0                         (DDRPHY_AO_BASE_ADDRESS + 0x00CB4)
    #define SHU1_B1_DLL0_RG_ARPISM_MCK_SEL_B1_SHU              Fld(1, 0) //[0:0]
    #define SHU1_B1_DLL0_B1_DLL0_RFU                           Fld(1, 3) //[3:3]
    #define SHU1_B1_DLL0_RG_ARDLL_FAST_PSJP_B1                 Fld(1, 4) //[4:4]
    #define SHU1_B1_DLL0_RG_ARDLL_PHDIV_B1                     Fld(1, 9) //[9:9]
    #define SHU1_B1_DLL0_RG_ARDLL_PHJUMP_EN_B1                 Fld(1, 10) //[10:10]
    #define SHU1_B1_DLL0_RG_ARDLL_P_GAIN_B1                    Fld(4, 12) //[15:12]
    #define SHU1_B1_DLL0_RG_ARDLL_IDLECNT_B1                   Fld(4, 16) //[19:16]
    #define SHU1_B1_DLL0_RG_ARDLL_GAIN_B1                      Fld(4, 20) //[23:20]
    #define SHU1_B1_DLL0_RG_ARDLL_PHDET_IN_SWAP_B1             Fld(1, 30) //[30:30]
    #define SHU1_B1_DLL0_RG_ARDLL_PHDET_OUT_SEL_B1             Fld(1, 31) //[31:31]

#define DDRPHY_SHU1_B1_DLL1                         (DDRPHY_AO_BASE_ADDRESS + 0x00CB8)
    #define SHU1_B1_DLL1_RG_ARDLL_FASTPJ_CK_SEL_B1             Fld(1, 0) //[0:0]
    #define SHU1_B1_DLL1_RG_ARDLL_PS_EN_B1                     Fld(1, 1) //[1:1]
    #define SHU1_B1_DLL1_RG_ARDLL_PD_CK_SEL_B1                 Fld(1, 2) //[2:2]
    #define SHU1_B1_DLL1_RG_ARDQ_REV_B1                        Fld(24, 8) //[31:8]

#define DDRPHY_SHU1_CA_CMD0                         (DDRPHY_AO_BASE_ADDRESS + 0x00D00)
    #define SHU1_CA_CMD0_RG_TX_ARCLK_PRE_EN                    Fld(1, 4) //[4:4]
    #define SHU1_CA_CMD0_RG_TX_ARCLK_DRVP_PRE                  Fld(3, 8) //[10:8]
    #define SHU1_CA_CMD0_RG_TX_ARCLK_DRVN_PRE                  Fld(3, 12) //[14:12]
    #define SHU1_CA_CMD0_RG_CGEN_FMEM_CK_CG_DLL                Fld(1, 17) //[17:17]
    #define SHU1_CA_CMD0_RG_FB_CK_MUX                          Fld(2, 18) //[19:18]
    #define SHU1_CA_CMD0_RG_TX_ARCMD_PRE_EN                    Fld(1, 20) //[20:20]
    #define SHU1_CA_CMD0_RG_TX_ARCMD_DRVP_PRE                  Fld(3, 24) //[26:24]
    #define SHU1_CA_CMD0_RG_TX_ARCMD_DRVN_PRE                  Fld(3, 28) //[30:28]
    #define SHU1_CA_CMD0_R_LP4Y_WDN_MODE_CLK                   Fld(1, 31) //[31:31]
    #define SHU1_CA_CMD0_RG_TX_ARCLK_DRVP_PRE_BIT2 Fld(1, 10)//[10:10]    //Francis added
    #define SHU1_CA_CMD0_RG_TX_ARCLK_DRVP_PRE_BIT1 Fld(1, 9)//[9:9]    //Francis added
    #define SHU1_CA_CMD0_RG_TX_ARCLK_DRVP_PRE_BIT0 Fld(1, 8)//[8:8]    //Francis added

#define DDRPHY_SHU1_CA_CMD1                         (DDRPHY_AO_BASE_ADDRESS + 0x00D04)
    #define SHU1_CA_CMD1_RG_TX_ARCMD_DRVP                      Fld(5, 0) //[4:0]
    #define SHU1_CA_CMD1_RG_TX_ARCMD_DRVN                      Fld(5, 8) //[12:8]
    #define SHU1_CA_CMD1_RG_TX_ARCMD_ODTP                      Fld(5, 16) //[20:16]
    #define SHU1_CA_CMD1_RG_TX_ARCMD_ODTN                      Fld(5, 24) //[28:24]

#define DDRPHY_SHU1_CA_CMD2                         (DDRPHY_AO_BASE_ADDRESS + 0x00D08)
    #define SHU1_CA_CMD2_RG_TX_ARCLK_DRVP                      Fld(5, 0) //[4:0]
    #define SHU1_CA_CMD2_RG_TX_ARCLK_DRVN                      Fld(5, 8) //[12:8]
    #define SHU1_CA_CMD2_RG_TX_ARCLK_ODTP                      Fld(5, 16) //[20:16]
    #define SHU1_CA_CMD2_RG_TX_ARCLK_ODTN                      Fld(5, 24) //[28:24]

#define DDRPHY_SHU1_CA_CMD3                         (DDRPHY_AO_BASE_ADDRESS + 0x00D0C)
    #define SHU1_CA_CMD3_RG_TX_ARCLK_PU                        Fld(2, 0) //[1:0]
    #define SHU1_CA_CMD3_RG_TX_ARCLK_PU_PRE                    Fld(2, 2) //[3:2]
    #define SHU1_CA_CMD3_RG_TX_ARCLK_PDB                       Fld(2, 4) //[5:4]
    #define SHU1_CA_CMD3_RG_TX_ARCLK_PDB_PRE                   Fld(2, 6) //[7:6]
    #define SHU1_CA_CMD3_RG_TX_ARCMD_PU                        Fld(2, 8) //[9:8]
    #define SHU1_CA_CMD3_RG_TX_ARCMD_PU_PRE                    Fld(2, 10) //[11:10]
    #define SHU1_CA_CMD3_RG_TX_ARCMD_PDB                       Fld(2, 12) //[13:12]
    #define SHU1_CA_CMD3_RG_TX_ARCMD_PDB_PRE                   Fld(2, 14) //[15:14]
    #define SHU1_CA_CMD3_RG_TX_ARCMD_PU_BIT0	        Fld(1, 8)//[9:9]	     //Francis added
    #define SHU1_CA_CMD3_RG_TX_ARCMD_PU_BIT1		Fld(1, 9)//[9:9]	 //Francis added
    #define SHU1_CA_CMD3_ARCMD_REV_BIT_06		Fld(1, 22)//[22:22]	//Francis added
    #define SHU1_CA_CMD3_ARCMD_REV_BIT_05		Fld(1, 21)//[21:21]	//Francis added
    #define SHU1_CA_CMD3_ARCMD_REV_BIT_04		Fld(1, 20)//[20:20]	//Francis added
    #define SHU1_CA_CMD3_ARCMD_REV_BIT_03		Fld(1, 19)//[19:19]	//Francis added

#define DDRPHY_SHU1_CA_CMD4                         (DDRPHY_AO_BASE_ADDRESS + 0x00D10)
    #define SHU1_CA_CMD4_RG_ARPI_AA_MCK_DL_CA                  Fld(6, 0) //[5:0]
    #define SHU1_CA_CMD4_RG_ARPI_AA_MCK_FB_DL_CA               Fld(6, 8) //[13:8]
    #define SHU1_CA_CMD4_RG_ARPI_DA_MCK_FB_DL_CA               Fld(6, 16) //[21:16]

#define DDRPHY_SHU1_CA_CMD5                         (DDRPHY_AO_BASE_ADDRESS + 0x00D14)
    #define SHU1_CA_CMD5_RG_RX_ARCMD_VREF_SEL                  Fld(6, 0) //[5:0]
    #define SHU1_CA_CMD5_RG_RX_ARCMD_VREF_BYPASS               Fld(1, 6) //[6:6]
    #define SHU1_CA_CMD5_RG_ARPI_FB_CA                         Fld(6, 8) //[13:8]
    #define SHU1_CA_CMD5_RG_RX_ARCLK_DQSIEN_DLY                Fld(3, 16) //[18:16]
    #define SHU1_CA_CMD5_DA_RX_ARCLK_DQSIEN_RB_DLY             Fld(1, 19) //[19:19]
    #define SHU1_CA_CMD5_RG_RX_ARCLK_DVS_DLY                   Fld(3, 20) //[22:20]
    #define SHU1_CA_CMD5_RG_ARPI_MCTL_CA                       Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_CA_CMD6                         (DDRPHY_AO_BASE_ADDRESS + 0x00D18)
    #define SHU1_CA_CMD6_RG_ARPI_OFFSET_CLKIEN                 Fld(6, 0) //[5:0]
    #define SHU1_CA_CMD6_RG_ARPI_RESERVE_CA                    Fld(16, 6) //[21:6]
    #define SHU1_CA_CMD6_RG_ARPI_MIDPI_CAP_SEL_CA              Fld(2, 22) //[23:22]
    #define SHU1_CA_CMD6_RG_ARPI_MIDPI_VTH_SEL_CA              Fld(2, 24) //[25:24]
    #define SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA                   Fld(1, 26) //[26:26]
    #define SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA            Fld(1, 27) //[27:27]
    #define SHU1_CA_CMD6_RG_ARPI_CAP_SEL_CA                    Fld(2, 28) //[29:28]
    #define SHU1_CA_CMD6_RG_ARPI_MIDPI_BYPASS_EN_CA            Fld(1, 31) //[31:31]

#define DDRPHY_SHU1_CA_CMD7                         (DDRPHY_AO_BASE_ADDRESS + 0x00D1C)
    #define SHU1_CA_CMD7_R_DMRANKRXDVS_CA                      Fld(4, 0) //[3:0]
    #define SHU1_CA_CMD7_R_DMRXDVS_PBYTE_FLAG_OPT_CA           Fld(1, 12) //[12:12]
    #define SHU1_CA_CMD7_R_DMRODTEN_CA                         Fld(1, 15) //[15:15]
    #define SHU1_CA_CMD7_R_DMARPI_CG_FB2DLL_DCM_EN_CA          Fld(1, 16) //[16:16]
    #define SHU1_CA_CMD7_R_DMTX_ARPI_CG_CMD_NEW                Fld(1, 17) //[17:17]
    #define SHU1_CA_CMD7_R_DMTX_ARPI_CG_CLK_NEW                Fld(1, 18) //[18:18]
    #define SHU1_CA_CMD7_R_DMTX_ARPI_CG_CS_NEW                 Fld(1, 19) //[19:19]
    #define SHU1_CA_CMD7_R_LP4Y_SDN_MODE_CLK                   Fld(1, 20) //[20:20]
    #define SHU1_CA_CMD7_R_DMRXRANK_CMD_EN                     Fld(1, 24) //[24:24]
    #define SHU1_CA_CMD7_R_DMRXRANK_CMD_LAT                    Fld(3, 25) //[27:25]
    #define SHU1_CA_CMD7_R_DMRXRANK_CLK_EN                     Fld(1, 28) //[28:28]
    #define SHU1_CA_CMD7_R_DMRXRANK_CLK_LAT                    Fld(3, 29) //[31:29]

#define DDRPHY_SHU1_CA_CMD8                         (DDRPHY_AO_BASE_ADDRESS + 0x00D20)
    #define SHU1_CA_CMD8_R_DMRXDVS_UPD_FORCE_CYC_CA            Fld(15, 0) //[14:0]
    #define SHU1_CA_CMD8_R_DMRXDVS_UPD_FORCE_EN_CA             Fld(1, 15) //[15:15]
    #define SHU1_CA_CMD8_R_DMRANK_RXDLY_PIPE_CG_IG_CA          Fld(1, 19) //[19:19]
    #define SHU1_CA_CMD8_R_RMRODTEN_CG_IG_CA                   Fld(1, 20) //[20:20]
    #define SHU1_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA                 Fld(1, 21) //[21:21]
    #define SHU1_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA         Fld(1, 22) //[22:22]
    #define SHU1_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA     Fld(1, 23) //[23:23]
    #define SHU1_CA_CMD8_R_DMRXDLY_CG_IG_CA                    Fld(1, 24) //[24:24]
    #define SHU1_CA_CMD8_R_DMSTBEN_SYNC_CG_IG_CA               Fld(1, 25) //[25:25]
    #define SHU1_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA         Fld(1, 26) //[26:26]
    #define SHU1_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA         Fld(1, 27) //[27:27]
    #define SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA        Fld(1, 28) //[28:28]
    #define SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA    Fld(1, 29) //[29:29]
    #define SHU1_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA                Fld(1, 30) //[30:30]
    #define SHU1_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA            Fld(1, 31) //[31:31]

#define DDRPHY_SHU1_CA_CMD9                         (DDRPHY_AO_BASE_ADDRESS + 0x00D24)
    #define SHU1_CA_CMD9_RESERVED_0XD24                        Fld(32, 0) //[31:0]

#define DDRPHY_SHU1_CA_CMD10                        (DDRPHY_AO_BASE_ADDRESS + 0x00D28)
    #define SHU1_CA_CMD10_RESERVED_0XD28                       Fld(32, 0) //[31:0]

#define DDRPHY_SHU1_CA_CMD11                        (DDRPHY_AO_BASE_ADDRESS + 0x00D2C)
    #define SHU1_CA_CMD11_RG_RIMP_REV                          Fld(8, 0) //[7:0]
    #define SHU1_CA_CMD11_RG_RIMP_VREF_SEL                     Fld(6, 8) //[13:8]
    #define SHU1_CA_CMD11_RG_TX_ARCKE_DRVP                     Fld(5, 17) //[21:17]
    #define SHU1_CA_CMD11_RG_TX_ARCKE_DRVN                     Fld(5, 22) //[26:22]

#define DDRPHY_SHU1_CA_CMD12                        (DDRPHY_AO_BASE_ADDRESS + 0x00D30)
    #define SHU1_CA_CMD12_RESERVED_0XD30                       Fld(32, 0) //[31:0]

#define DDRPHY_SHU1_CA_DLL0                         (DDRPHY_AO_BASE_ADDRESS + 0x00D34)
    #define SHU1_CA_DLL0_RG_ARPISM_MCK_SEL_CA_SHU              Fld(1, 0) //[0:0]
    #define SHU1_CA_DLL0_CA_DLL0_RFU                           Fld(1, 3) //[3:3]
    #define SHU1_CA_DLL0_RG_ARDLL_FAST_PSJP_CA                 Fld(1, 4) //[4:4]
    #define SHU1_CA_DLL0_RG_ARDLL_PHDIV_CA                     Fld(1, 9) //[9:9]
    #define SHU1_CA_DLL0_RG_ARDLL_PHJUMP_EN_CA                 Fld(1, 10) //[10:10]
    #define SHU1_CA_DLL0_RG_ARDLL_P_GAIN_CA                    Fld(4, 12) //[15:12]
    #define SHU1_CA_DLL0_RG_ARDLL_IDLECNT_CA                   Fld(4, 16) //[19:16]
    #define SHU1_CA_DLL0_RG_ARDLL_GAIN_CA                      Fld(4, 20) //[23:20]
    #define SHU1_CA_DLL0_RG_ARDLL_PHDET_IN_SWAP_CA             Fld(1, 30) //[30:30]
    #define SHU1_CA_DLL0_RG_ARDLL_PHDET_OUT_SEL_CA             Fld(1, 31) //[31:31]

#define DDRPHY_SHU1_CA_DLL1                         (DDRPHY_AO_BASE_ADDRESS + 0x00D38)
    #define SHU1_CA_DLL1_RG_ARDLL_FASTPJ_CK_SEL_CA             Fld(1, 0) //[0:0]
    #define SHU1_CA_DLL1_RG_ARDLL_PS_EN_CA                     Fld(1, 1) //[1:1]
    #define SHU1_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA                 Fld(1, 2) //[2:2]
    #define SHU1_CA_DLL1_RG_ARCMD_REV                          Fld(24, 8) //[31:8]

#define DDRPHY_SHU1_MISC0                           (DDRPHY_AO_BASE_ADDRESS + 0x00DF0)
    #define SHU1_MISC0_R_RX_PIPE_BYPASS_EN                     Fld(1, 1) //[1:1]
    #define SHU1_MISC0_RG_CMD_TXPIPE_BYPASS_EN                 Fld(1, 2) //[2:2]
    #define SHU1_MISC0_RG_CK_TXPIPE_BYPASS_EN                  Fld(1, 3) //[3:3]
    #define SHU1_MISC0_RG_RVREF_SEL_DQ                         Fld(6, 16) //[21:16]
    #define SHU1_MISC0_RG_RVREF_DDR4_SEL                       Fld(1, 22) //[22:22]
    #define SHU1_MISC0_RG_RVREF_DDR3_SEL                       Fld(1, 23) //[23:23]
    #define SHU1_MISC0_RG_RVREF_SEL_CMD                        Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R0_B0_DQ0                       (DDRPHY_AO_BASE_ADDRESS + 0x00E00)
    #define SHU1_R0_B0_DQ0_RK0_TX_ARDQ0_DLY_B0                 Fld(4, 0) //[3:0]
    #define SHU1_R0_B0_DQ0_RK0_TX_ARDQ1_DLY_B0                 Fld(4, 4) //[7:4]
    #define SHU1_R0_B0_DQ0_RK0_TX_ARDQ2_DLY_B0                 Fld(4, 8) //[11:8]
    #define SHU1_R0_B0_DQ0_RK0_TX_ARDQ3_DLY_B0                 Fld(4, 12) //[15:12]
    #define SHU1_R0_B0_DQ0_RK0_TX_ARDQ4_DLY_B0                 Fld(4, 16) //[19:16]
    #define SHU1_R0_B0_DQ0_RK0_TX_ARDQ5_DLY_B0                 Fld(4, 20) //[23:20]
    #define SHU1_R0_B0_DQ0_RK0_TX_ARDQ6_DLY_B0                 Fld(4, 24) //[27:24]
    #define SHU1_R0_B0_DQ0_RK0_TX_ARDQ7_DLY_B0                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU1_R0_B0_DQ1                       (DDRPHY_AO_BASE_ADDRESS + 0x00E04)
    #define SHU1_R0_B0_DQ1_RK0_TX_ARDQM0_DLY_B0                Fld(4, 0) //[3:0]
    #define SHU1_R0_B0_DQ1_RK0_TX_ARDQS0_DLYB_B0               Fld(4, 16) //[19:16]
    #define SHU1_R0_B0_DQ1_RK0_TX_ARDQS0B_DLYB_B0              Fld(4, 20) //[23:20]
    #define SHU1_R0_B0_DQ1_RK0_TX_ARDQS0_DLY_B0                Fld(4, 24) //[27:24]
    #define SHU1_R0_B0_DQ1_RK0_TX_ARDQS0B_DLY_B0               Fld(4, 28) //[31:28]

#define DDRPHY_SHU1_R0_B0_DQ2                       (DDRPHY_AO_BASE_ADDRESS + 0x00E08)
    #define SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R0_B0_DQ3                       (DDRPHY_AO_BASE_ADDRESS + 0x00E0C)
    #define SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R0_B0_DQ4                       (DDRPHY_AO_BASE_ADDRESS + 0x00E10)
    #define SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R0_B0_DQ5                       (DDRPHY_AO_BASE_ADDRESS + 0x00E14)
    #define SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R0_B0_DQ6                       (DDRPHY_AO_BASE_ADDRESS + 0x00E18)
    #define SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0              Fld(6, 0) //[5:0]
    #define SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0              Fld(6, 8) //[13:8]
    #define SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0              Fld(7, 16) //[22:16]
    #define SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0              Fld(7, 24) //[30:24]

#define DDRPHY_SHU1_R0_B0_DQ7                       (DDRPHY_AO_BASE_ADDRESS + 0x00E1C)
    #define SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0                      Fld(6, 8) //[13:8]
    #define SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0                     Fld(6, 16) //[21:16]
    #define SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0                   Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0XE20                            (DDRPHY_AO_BASE_ADDRESS + 0x00E20)
    #define RFU_0XE20_RESERVED_0XE20                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0XE24                            (DDRPHY_AO_BASE_ADDRESS + 0x00E24)
    #define RFU_0XE24_RESERVED_0XE24                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0XE28                            (DDRPHY_AO_BASE_ADDRESS + 0x00E28)
    #define RFU_0XE28_RESERVED_0XE28                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0XE2C                            (DDRPHY_AO_BASE_ADDRESS + 0x00E2C)
    #define RFU_0XE2C_RESERVED_0XE2C                           Fld(32, 0) //[31:0]

#define DDRPHY_SHU1_R0_B1_DQ0                       (DDRPHY_AO_BASE_ADDRESS + 0x00E50)
    #define SHU1_R0_B1_DQ0_RK0_TX_ARDQ0_DLY_B1                 Fld(4, 0) //[3:0]
    #define SHU1_R0_B1_DQ0_RK0_TX_ARDQ1_DLY_B1                 Fld(4, 4) //[7:4]
    #define SHU1_R0_B1_DQ0_RK0_TX_ARDQ2_DLY_B1                 Fld(4, 8) //[11:8]
    #define SHU1_R0_B1_DQ0_RK0_TX_ARDQ3_DLY_B1                 Fld(4, 12) //[15:12]
    #define SHU1_R0_B1_DQ0_RK0_TX_ARDQ4_DLY_B1                 Fld(4, 16) //[19:16]
    #define SHU1_R0_B1_DQ0_RK0_TX_ARDQ5_DLY_B1                 Fld(4, 20) //[23:20]
    #define SHU1_R0_B1_DQ0_RK0_TX_ARDQ6_DLY_B1                 Fld(4, 24) //[27:24]
    #define SHU1_R0_B1_DQ0_RK0_TX_ARDQ7_DLY_B1                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU1_R0_B1_DQ1                       (DDRPHY_AO_BASE_ADDRESS + 0x00E54)
    #define SHU1_R0_B1_DQ1_RK0_TX_ARDQM0_DLY_B1                Fld(4, 0) //[3:0]
    #define SHU1_R0_B1_DQ1_RK0_TX_ARDQS0_DLYB_B1               Fld(4, 16) //[19:16]
    #define SHU1_R0_B1_DQ1_RK0_TX_ARDQS0B_DLYB_B1              Fld(4, 20) //[23:20]
    #define SHU1_R0_B1_DQ1_RK0_TX_ARDQS0_DLY_B1                Fld(4, 24) //[27:24]
    #define SHU1_R0_B1_DQ1_RK0_TX_ARDQS0B_DLY_B1               Fld(4, 28) //[31:28]

#define DDRPHY_SHU1_R0_B1_DQ2                       (DDRPHY_AO_BASE_ADDRESS + 0x00E58)
    #define SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R0_B1_DQ3                       (DDRPHY_AO_BASE_ADDRESS + 0x00E5C)
    #define SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R0_B1_DQ4                       (DDRPHY_AO_BASE_ADDRESS + 0x00E60)
    #define SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R0_B1_DQ5                       (DDRPHY_AO_BASE_ADDRESS + 0x00E64)
    #define SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R0_B1_DQ6                       (DDRPHY_AO_BASE_ADDRESS + 0x00E68)
    #define SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1              Fld(6, 0) //[5:0]
    #define SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1              Fld(6, 8) //[13:8]
    #define SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1              Fld(7, 16) //[22:16]
    #define SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1              Fld(7, 24) //[30:24]

#define DDRPHY_SHU1_R0_B1_DQ7                       (DDRPHY_AO_BASE_ADDRESS + 0x00E6C)
    #define SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1                      Fld(6, 8) //[13:8]
    #define SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1                     Fld(6, 16) //[21:16]
    #define SHU1_R0_B1_DQ7_RK0_ARPI_PBYTE_B1                   Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0XE70                            (DDRPHY_AO_BASE_ADDRESS + 0x00E70)
    #define RFU_0XE70_RESERVED_0XE70                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0XE74                            (DDRPHY_AO_BASE_ADDRESS + 0x00E74)
    #define RFU_0XE74_RESERVED_0XE74                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0XE78                            (DDRPHY_AO_BASE_ADDRESS + 0x00E78)
    #define RFU_0XE78_RESERVED_0XE78                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0XE7C                            (DDRPHY_AO_BASE_ADDRESS + 0x00E7C)
    #define RFU_0XE7C_RESERVED_0XE7C                           Fld(32, 0) //[31:0]

#define DDRPHY_SHU1_R0_CA_CMD0                      (DDRPHY_AO_BASE_ADDRESS + 0x00EA0)
    #define SHU1_R0_CA_CMD0_RK0_TX_ARCA0_DLY                   Fld(4, 0) //[3:0]
    #define SHU1_R0_CA_CMD0_RK0_TX_ARCA1_DLY                   Fld(4, 4) //[7:4]
    #define SHU1_R0_CA_CMD0_RK0_TX_ARCA2_DLY                   Fld(4, 8) //[11:8]
    #define SHU1_R0_CA_CMD0_RK0_TX_ARCA3_DLY                   Fld(4, 12) //[15:12]
    #define SHU1_R0_CA_CMD0_RK0_TX_ARCA4_DLY                   Fld(4, 16) //[19:16]
    #define SHU1_R0_CA_CMD0_RK0_TX_ARCA5_DLY                   Fld(4, 20) //[23:20]
    #define SHU1_R0_CA_CMD0_RK0_TX_ARCLK_DLYB                  Fld(4, 24) //[27:24]
    #define SHU1_R0_CA_CMD0_RK0_TX_ARCLKB_DLYB                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU1_R0_CA_CMD1                      (DDRPHY_AO_BASE_ADDRESS + 0x00EA4)
    #define SHU1_R0_CA_CMD1_RK0_TX_ARCKE0_DLY                  Fld(4, 0) //[3:0]
    #define SHU1_R0_CA_CMD1_RK0_TX_ARCKE1_DLY                  Fld(4, 4) //[7:4]
    #define SHU1_R0_CA_CMD1_RK0_TX_ARCKE2_DLY                  Fld(4, 8) //[11:8]
    #define SHU1_R0_CA_CMD1_RK0_TX_ARCS0_DLY                   Fld(4, 12) //[15:12]
    #define SHU1_R0_CA_CMD1_RK0_TX_ARCS1_DLY                   Fld(4, 16) //[19:16]
    #define SHU1_R0_CA_CMD1_RK0_TX_ARCS2_DLY                   Fld(4, 20) //[23:20]
    #define SHU1_R0_CA_CMD1_RK0_TX_ARCLK_DLY                   Fld(4, 24) //[27:24]
    #define SHU1_R0_CA_CMD1_RK0_TX_ARCLKB_DLY                  Fld(4, 28) //[31:28]

#define DDRPHY_SHU1_R0_CA_CMD2                      (DDRPHY_AO_BASE_ADDRESS + 0x00EA8)
    #define SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA0_R_DLY              Fld(6, 0) //[5:0]
    #define SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA0_F_DLY              Fld(6, 8) //[13:8]
    #define SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA1_R_DLY              Fld(6, 16) //[21:16]
    #define SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA1_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R0_CA_CMD3                      (DDRPHY_AO_BASE_ADDRESS + 0x00EAC)
    #define SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA2_R_DLY              Fld(6, 0) //[5:0]
    #define SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA2_F_DLY              Fld(6, 8) //[13:8]
    #define SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA3_R_DLY              Fld(6, 16) //[21:16]
    #define SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA3_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R0_CA_CMD4                      (DDRPHY_AO_BASE_ADDRESS + 0x00EB0)
    #define SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA4_R_DLY              Fld(6, 0) //[5:0]
    #define SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA4_F_DLY              Fld(6, 8) //[13:8]
    #define SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA5_R_DLY              Fld(6, 16) //[21:16]
    #define SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA5_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R0_CA_CMD5                      (DDRPHY_AO_BASE_ADDRESS + 0x00EB4)
    #define SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE0_R_DLY             Fld(6, 0) //[5:0]
    #define SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE0_F_DLY             Fld(6, 8) //[13:8]
    #define SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE1_R_DLY             Fld(6, 16) //[21:16]
    #define SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE1_F_DLY             Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R0_CA_CMD6                      (DDRPHY_AO_BASE_ADDRESS + 0x00EB8)
    #define SHU1_R0_CA_CMD6_RG_RK0_RX_ARCKE2_R_DLY             Fld(6, 0) //[5:0]
    #define SHU1_R0_CA_CMD6_RG_RK0_RX_ARCKE2_F_DLY             Fld(6, 8) //[13:8]
    #define SHU1_R0_CA_CMD6_RG_RK0_RX_ARCS0_R_DLY              Fld(6, 16) //[21:16]
    #define SHU1_R0_CA_CMD6_RG_RK0_RX_ARCS0_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R0_CA_CMD7                      (DDRPHY_AO_BASE_ADDRESS + 0x00EBC)
    #define SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS1_R_DLY              Fld(6, 0) //[5:0]
    #define SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS1_F_DLY              Fld(6, 8) //[13:8]
    #define SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS2_R_DLY              Fld(6, 16) //[21:16]
    #define SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS2_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R0_CA_CMD8                      (DDRPHY_AO_BASE_ADDRESS + 0x00EC0)
    #define SHU1_R0_CA_CMD8_RG_RK0_RX_ARCLK_R_DLY              Fld(7, 16) //[22:16]
    #define SHU1_R0_CA_CMD8_RG_RK0_RX_ARCLK_F_DLY              Fld(7, 24) //[30:24]

#define DDRPHY_SHU1_R0_CA_CMD9                      (DDRPHY_AO_BASE_ADDRESS + 0x00EC4)
    #define SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS                     Fld(6, 0) //[5:0]
    #define SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD                    Fld(6, 8) //[13:8]
    #define SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK                    Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0XEC8                            (DDRPHY_AO_BASE_ADDRESS + 0x00EC8)
    #define RFU_0XEC8_RESERVED_0XEC8                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0XECC                            (DDRPHY_AO_BASE_ADDRESS + 0x00ECC)
    #define RFU_0XECC_RESERVED_0XECC                           Fld(32, 0) //[31:0]

#define DDRPHY_SHU1_R1_B0_DQ0                       (DDRPHY_AO_BASE_ADDRESS + 0x00F00)
    #define SHU1_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0                 Fld(4, 0) //[3:0]
    #define SHU1_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0                 Fld(4, 4) //[7:4]
    #define SHU1_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0                 Fld(4, 8) //[11:8]
    #define SHU1_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0                 Fld(4, 12) //[15:12]
    #define SHU1_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0                 Fld(4, 16) //[19:16]
    #define SHU1_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0                 Fld(4, 20) //[23:20]
    #define SHU1_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0                 Fld(4, 24) //[27:24]
    #define SHU1_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU1_R1_B0_DQ1                       (DDRPHY_AO_BASE_ADDRESS + 0x00F04)
    #define SHU1_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0                Fld(4, 0) //[3:0]
    #define SHU1_R1_B0_DQ1_RK1_TX_ARDQS0_DLYB_B0               Fld(4, 16) //[19:16]
    #define SHU1_R1_B0_DQ1_RK1_TX_ARDQS0B_DLYB_B0              Fld(4, 20) //[23:20]
    #define SHU1_R1_B0_DQ1_RK1_TX_ARDQS0_DLY_B0                Fld(4, 24) //[27:24]
    #define SHU1_R1_B0_DQ1_RK1_TX_ARDQS0B_DLY_B0               Fld(4, 28) //[31:28]

#define DDRPHY_SHU1_R1_B0_DQ2                       (DDRPHY_AO_BASE_ADDRESS + 0x00F08)
    #define SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R1_B0_DQ3                       (DDRPHY_AO_BASE_ADDRESS + 0x00F0C)
    #define SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R1_B0_DQ4                       (DDRPHY_AO_BASE_ADDRESS + 0x00F10)
    #define SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R1_B0_DQ5                       (DDRPHY_AO_BASE_ADDRESS + 0x00F14)
    #define SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R1_B0_DQ6                       (DDRPHY_AO_BASE_ADDRESS + 0x00F18)
    #define SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_R_DLY_B0              Fld(6, 0) //[5:0]
    #define SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_F_DLY_B0              Fld(6, 8) //[13:8]
    #define SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0              Fld(7, 16) //[22:16]
    #define SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_F_DLY_B0              Fld(7, 24) //[30:24]

#define DDRPHY_SHU1_R1_B0_DQ7                       (DDRPHY_AO_BASE_ADDRESS + 0x00F1C)
    #define SHU1_R1_B0_DQ7_RK1_ARPI_DQ_B0                      Fld(6, 8) //[13:8]
    #define SHU1_R1_B0_DQ7_RK1_ARPI_DQM_B0                     Fld(6, 16) //[21:16]
    #define SHU1_R1_B0_DQ7_RK1_ARPI_PBYTE_B0                   Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0XF20                            (DDRPHY_AO_BASE_ADDRESS + 0x00F20)
    #define RFU_0XF20_RESERVED_0XF20                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0XF24                            (DDRPHY_AO_BASE_ADDRESS + 0x00F24)
    #define RFU_0XF24_RESERVED_0XF24                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0XF28                            (DDRPHY_AO_BASE_ADDRESS + 0x00F28)
    #define RFU_0XF28_RESERVED_0XF28                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0XF2C                            (DDRPHY_AO_BASE_ADDRESS + 0x00F2C)
    #define RFU_0XF2C_RESERVED_0XF2C                           Fld(32, 0) //[31:0]

#define DDRPHY_SHU1_R1_B1_DQ0                       (DDRPHY_AO_BASE_ADDRESS + 0x00F50)
    #define SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1                 Fld(4, 0) //[3:0]
    #define SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1                 Fld(4, 4) //[7:4]
    #define SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1                 Fld(4, 8) //[11:8]
    #define SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1                 Fld(4, 12) //[15:12]
    #define SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1                 Fld(4, 16) //[19:16]
    #define SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1                 Fld(4, 20) //[23:20]
    #define SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1                 Fld(4, 24) //[27:24]
    #define SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU1_R1_B1_DQ1                       (DDRPHY_AO_BASE_ADDRESS + 0x00F54)
    #define SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1                Fld(4, 0) //[3:0]
    #define SHU1_R1_B1_DQ1_RK1_TX_ARDQS0_DLYB_B1               Fld(4, 16) //[19:16]
    #define SHU1_R1_B1_DQ1_RK1_TX_ARDQS0B_DLYB_B1              Fld(4, 20) //[23:20]
    #define SHU1_R1_B1_DQ1_RK1_TX_ARDQS0_DLY_B1                Fld(4, 24) //[27:24]
    #define SHU1_R1_B1_DQ1_RK1_TX_ARDQS0B_DLY_B1               Fld(4, 28) //[31:28]

#define DDRPHY_SHU1_R1_B1_DQ2                       (DDRPHY_AO_BASE_ADDRESS + 0x00F58)
    #define SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R1_B1_DQ3                       (DDRPHY_AO_BASE_ADDRESS + 0x00F5C)
    #define SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R1_B1_DQ4                       (DDRPHY_AO_BASE_ADDRESS + 0x00F60)
    #define SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R1_B1_DQ5                       (DDRPHY_AO_BASE_ADDRESS + 0x00F64)
    #define SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R1_B1_DQ6                       (DDRPHY_AO_BASE_ADDRESS + 0x00F68)
    #define SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1              Fld(6, 0) //[5:0]
    #define SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_F_DLY_B1              Fld(6, 8) //[13:8]
    #define SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1              Fld(7, 16) //[22:16]
    #define SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1              Fld(7, 24) //[30:24]

#define DDRPHY_SHU1_R1_B1_DQ7                       (DDRPHY_AO_BASE_ADDRESS + 0x00F6C)
    #define SHU1_R1_B1_DQ7_RK1_ARPI_DQ_B1                      Fld(6, 8) //[13:8]
    #define SHU1_R1_B1_DQ7_RK1_ARPI_DQM_B1                     Fld(6, 16) //[21:16]
    #define SHU1_R1_B1_DQ7_RK1_ARPI_PBYTE_B1                   Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0XF70                            (DDRPHY_AO_BASE_ADDRESS + 0x00F70)
    #define RFU_0XF70_RESERVED_0XF70                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0XF74                            (DDRPHY_AO_BASE_ADDRESS + 0x00F74)
    #define RFU_0XF74_RESERVED_0XF74                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0XF78                            (DDRPHY_AO_BASE_ADDRESS + 0x00F78)
    #define RFU_0XF78_RESERVED_0XF78                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0XF7C                            (DDRPHY_AO_BASE_ADDRESS + 0x00F7C)
    #define RFU_0XF7C_RESERVED_0XF7C                           Fld(32, 0) //[31:0]

#define DDRPHY_SHU1_R1_CA_CMD0                      (DDRPHY_AO_BASE_ADDRESS + 0x00FA0)
    #define SHU1_R1_CA_CMD0_RK1_TX_ARCA0_DLY                   Fld(4, 0) //[3:0]
    #define SHU1_R1_CA_CMD0_RK1_TX_ARCA1_DLY                   Fld(4, 4) //[7:4]
    #define SHU1_R1_CA_CMD0_RK1_TX_ARCA2_DLY                   Fld(4, 8) //[11:8]
    #define SHU1_R1_CA_CMD0_RK1_TX_ARCA3_DLY                   Fld(4, 12) //[15:12]
    #define SHU1_R1_CA_CMD0_RK1_TX_ARCA4_DLY                   Fld(4, 16) //[19:16]
    #define SHU1_R1_CA_CMD0_RK1_TX_ARCA5_DLY                   Fld(4, 20) //[23:20]
    #define SHU1_R1_CA_CMD0_RK1_TX_ARCLK_DLYB                  Fld(4, 24) //[27:24]
    #define SHU1_R1_CA_CMD0_RK1_TX_ARCLKB_DLYB                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU1_R1_CA_CMD1                      (DDRPHY_AO_BASE_ADDRESS + 0x00FA4)
    #define SHU1_R1_CA_CMD1_RK1_TX_ARCKE0_DLY                  Fld(4, 0) //[3:0]
    #define SHU1_R1_CA_CMD1_RK1_TX_ARCKE1_DLY                  Fld(4, 4) //[7:4]
    #define SHU1_R1_CA_CMD1_RK1_TX_ARCKE2_DLY                  Fld(4, 8) //[11:8]
    #define SHU1_R1_CA_CMD1_RK1_TX_ARCS0_DLY                   Fld(4, 12) //[15:12]
    #define SHU1_R1_CA_CMD1_RK1_TX_ARCS1_DLY                   Fld(4, 16) //[19:16]
    #define SHU1_R1_CA_CMD1_RK1_TX_ARCS2_DLY                   Fld(4, 20) //[23:20]
    #define SHU1_R1_CA_CMD1_RK1_TX_ARCLK_DLY                   Fld(4, 24) //[27:24]
    #define SHU1_R1_CA_CMD1_RK1_TX_ARCLKB_DLY                  Fld(4, 28) //[31:28]

#define DDRPHY_SHU1_R1_CA_CMD2                      (DDRPHY_AO_BASE_ADDRESS + 0x00FA8)
    #define SHU1_R1_CA_CMD2_RG_RK1_RX_ARCA0_R_DLY              Fld(6, 0) //[5:0]
    #define SHU1_R1_CA_CMD2_RG_RK1_RX_ARCA0_F_DLY              Fld(6, 8) //[13:8]
    #define SHU1_R1_CA_CMD2_RG_RK1_RX_ARCA1_R_DLY              Fld(6, 16) //[21:16]
    #define SHU1_R1_CA_CMD2_RG_RK1_RX_ARCA1_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R1_CA_CMD3                      (DDRPHY_AO_BASE_ADDRESS + 0x00FAC)
    #define SHU1_R1_CA_CMD3_RG_RK1_RX_ARCA2_R_DLY              Fld(6, 0) //[5:0]
    #define SHU1_R1_CA_CMD3_RG_RK1_RX_ARCA2_F_DLY              Fld(6, 8) //[13:8]
    #define SHU1_R1_CA_CMD3_RG_RK1_RX_ARCA3_R_DLY              Fld(6, 16) //[21:16]
    #define SHU1_R1_CA_CMD3_RG_RK1_RX_ARCA3_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R1_CA_CMD4                      (DDRPHY_AO_BASE_ADDRESS + 0x00FB0)
    #define SHU1_R1_CA_CMD4_RG_RK1_RX_ARCA4_R_DLY              Fld(6, 0) //[5:0]
    #define SHU1_R1_CA_CMD4_RG_RK1_RX_ARCA4_F_DLY              Fld(6, 8) //[13:8]
    #define SHU1_R1_CA_CMD4_RG_RK1_RX_ARCA5_R_DLY              Fld(6, 16) //[21:16]
    #define SHU1_R1_CA_CMD4_RG_RK1_RX_ARCA5_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R1_CA_CMD5                      (DDRPHY_AO_BASE_ADDRESS + 0x00FB4)
    #define SHU1_R1_CA_CMD5_RG_RK1_RX_ARCKE0_R_DLY             Fld(6, 0) //[5:0]
    #define SHU1_R1_CA_CMD5_RG_RK1_RX_ARCKE0_F_DLY             Fld(6, 8) //[13:8]
    #define SHU1_R1_CA_CMD5_RG_RK1_RX_ARCKE1_R_DLY             Fld(6, 16) //[21:16]
    #define SHU1_R1_CA_CMD5_RG_RK1_RX_ARCKE1_F_DLY             Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R1_CA_CMD6                      (DDRPHY_AO_BASE_ADDRESS + 0x00FB8)
    #define SHU1_R1_CA_CMD6_RG_RK1_RX_ARCKE2_R_DLY             Fld(6, 0) //[5:0]
    #define SHU1_R1_CA_CMD6_RG_RK1_RX_ARCKE2_F_DLY             Fld(6, 8) //[13:8]
    #define SHU1_R1_CA_CMD6_RG_RK1_RX_ARCS0_R_DLY              Fld(6, 16) //[21:16]
    #define SHU1_R1_CA_CMD6_RG_RK1_RX_ARCS0_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R1_CA_CMD7                      (DDRPHY_AO_BASE_ADDRESS + 0x00FBC)
    #define SHU1_R1_CA_CMD7_RG_RK1_RX_ARCS1_R_DLY              Fld(6, 0) //[5:0]
    #define SHU1_R1_CA_CMD7_RG_RK1_RX_ARCS1_F_DLY              Fld(6, 8) //[13:8]
    #define SHU1_R1_CA_CMD7_RG_RK1_RX_ARCS2_R_DLY              Fld(6, 16) //[21:16]
    #define SHU1_R1_CA_CMD7_RG_RK1_RX_ARCS2_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R1_CA_CMD8                      (DDRPHY_AO_BASE_ADDRESS + 0x00FC0)
    #define SHU1_R1_CA_CMD8_RG_RK1_RX_ARCLK_R_DLY              Fld(7, 16) //[22:16]
    #define SHU1_R1_CA_CMD8_RG_RK1_RX_ARCLK_F_DLY              Fld(7, 24) //[30:24]

#define DDRPHY_SHU1_R1_CA_CMD9                      (DDRPHY_AO_BASE_ADDRESS + 0x00FC4)
    #define SHU1_R1_CA_CMD9_RG_RK1_ARPI_CS                     Fld(6, 0) //[5:0]
    #define SHU1_R1_CA_CMD9_RG_RK1_ARPI_CMD                    Fld(6, 8) //[13:8]
    #define SHU1_R1_CA_CMD9_RG_RK1_ARPI_CLK                    Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0XFC8                            (DDRPHY_AO_BASE_ADDRESS + 0x00FC8)
    #define RFU_0XFC8_RESERVED_0XFC8                           Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0XFCC                            (DDRPHY_AO_BASE_ADDRESS + 0x00FCC)
    #define RFU_0XFCC_RESERVED_0XFCC                           Fld(32, 0) //[31:0]

#define DDRPHY_SHU1_R2_B0_DQ0                       (DDRPHY_AO_BASE_ADDRESS + 0x01000)
    #define SHU1_R2_B0_DQ0_RK2_TX_ARDQ0_DLY_B0                 Fld(4, 0) //[3:0]
    #define SHU1_R2_B0_DQ0_RK2_TX_ARDQ1_DLY_B0                 Fld(4, 4) //[7:4]
    #define SHU1_R2_B0_DQ0_RK2_TX_ARDQ2_DLY_B0                 Fld(4, 8) //[11:8]
    #define SHU1_R2_B0_DQ0_RK2_TX_ARDQ3_DLY_B0                 Fld(4, 12) //[15:12]
    #define SHU1_R2_B0_DQ0_RK2_TX_ARDQ4_DLY_B0                 Fld(4, 16) //[19:16]
    #define SHU1_R2_B0_DQ0_RK2_TX_ARDQ5_DLY_B0                 Fld(4, 20) //[23:20]
    #define SHU1_R2_B0_DQ0_RK2_TX_ARDQ6_DLY_B0                 Fld(4, 24) //[27:24]
    #define SHU1_R2_B0_DQ0_RK2_TX_ARDQ7_DLY_B0                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU1_R2_B0_DQ1                       (DDRPHY_AO_BASE_ADDRESS + 0x01004)
    #define SHU1_R2_B0_DQ1_RK2_TX_ARDQM0_DLY_B0                Fld(4, 0) //[3:0]
    #define SHU1_R2_B0_DQ1_RK2_TX_ARDQS0_DLYB_B0               Fld(4, 16) //[19:16]
    #define SHU1_R2_B0_DQ1_RK2_TX_ARDQS0B_DLYB_B0              Fld(4, 20) //[23:20]
    #define SHU1_R2_B0_DQ1_RK2_TX_ARDQS0_DLY_B0                Fld(4, 24) //[27:24]
    #define SHU1_R2_B0_DQ1_RK2_TX_ARDQS0B_DLY_B0               Fld(4, 28) //[31:28]

#define DDRPHY_SHU1_R2_B0_DQ2                       (DDRPHY_AO_BASE_ADDRESS + 0x01008)
    #define SHU1_R2_B0_DQ2_RK2_RX_ARDQ0_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU1_R2_B0_DQ2_RK2_RX_ARDQ0_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU1_R2_B0_DQ2_RK2_RX_ARDQ1_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU1_R2_B0_DQ2_RK2_RX_ARDQ1_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R2_B0_DQ3                       (DDRPHY_AO_BASE_ADDRESS + 0x0100C)
    #define SHU1_R2_B0_DQ3_RK2_RX_ARDQ2_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU1_R2_B0_DQ3_RK2_RX_ARDQ2_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU1_R2_B0_DQ3_RK2_RX_ARDQ3_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU1_R2_B0_DQ3_RK2_RX_ARDQ3_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R2_B0_DQ4                       (DDRPHY_AO_BASE_ADDRESS + 0x01010)
    #define SHU1_R2_B0_DQ4_RK2_RX_ARDQ4_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU1_R2_B0_DQ4_RK2_RX_ARDQ4_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU1_R2_B0_DQ4_RK2_RX_ARDQ5_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU1_R2_B0_DQ4_RK2_RX_ARDQ5_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R2_B0_DQ5                       (DDRPHY_AO_BASE_ADDRESS + 0x01014)
    #define SHU1_R2_B0_DQ5_RK2_RX_ARDQ6_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU1_R2_B0_DQ5_RK2_RX_ARDQ6_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU1_R2_B0_DQ5_RK2_RX_ARDQ7_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU1_R2_B0_DQ5_RK2_RX_ARDQ7_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R2_B0_DQ6                       (DDRPHY_AO_BASE_ADDRESS + 0x01018)
    #define SHU1_R2_B0_DQ6_RK2_RX_ARDQM0_R_DLY_B0              Fld(6, 0) //[5:0]
    #define SHU1_R2_B0_DQ6_RK2_RX_ARDQM0_F_DLY_B0              Fld(6, 8) //[13:8]
    #define SHU1_R2_B0_DQ6_RK2_RX_ARDQS0_R_DLY_B0              Fld(7, 16) //[22:16]
    #define SHU1_R2_B0_DQ6_RK2_RX_ARDQS0_F_DLY_B0              Fld(7, 24) //[30:24]

#define DDRPHY_SHU1_R2_B0_DQ7                       (DDRPHY_AO_BASE_ADDRESS + 0x0101C)
    #define SHU1_R2_B0_DQ7_RK2_ARPI_DQ_B0                      Fld(6, 8) //[13:8]
    #define SHU1_R2_B0_DQ7_RK2_ARPI_DQM_B0                     Fld(6, 16) //[21:16]
    #define SHU1_R2_B0_DQ7_RK2_ARPI_PBYTE_B0                   Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X1020                           (DDRPHY_AO_BASE_ADDRESS + 0x01020)
    #define RFU_0X1020_RESERVED_0X1020                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1024                           (DDRPHY_AO_BASE_ADDRESS + 0x01024)
    #define RFU_0X1024_RESERVED_0X1024                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1028                           (DDRPHY_AO_BASE_ADDRESS + 0x01028)
    #define RFU_0X1028_RESERVED_0X1028                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X102C                           (DDRPHY_AO_BASE_ADDRESS + 0x0102C)
    #define RFU_0X102C_RESERVED_0X102C                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU1_R2_B1_DQ0                       (DDRPHY_AO_BASE_ADDRESS + 0x01050)
    #define SHU1_R2_B1_DQ0_RK2_TX_ARDQ0_DLY_B1                 Fld(4, 0) //[3:0]
    #define SHU1_R2_B1_DQ0_RK2_TX_ARDQ1_DLY_B1                 Fld(4, 4) //[7:4]
    #define SHU1_R2_B1_DQ0_RK2_TX_ARDQ2_DLY_B1                 Fld(4, 8) //[11:8]
    #define SHU1_R2_B1_DQ0_RK2_TX_ARDQ3_DLY_B1                 Fld(4, 12) //[15:12]
    #define SHU1_R2_B1_DQ0_RK2_TX_ARDQ4_DLY_B1                 Fld(4, 16) //[19:16]
    #define SHU1_R2_B1_DQ0_RK2_TX_ARDQ5_DLY_B1                 Fld(4, 20) //[23:20]
    #define SHU1_R2_B1_DQ0_RK2_TX_ARDQ6_DLY_B1                 Fld(4, 24) //[27:24]
    #define SHU1_R2_B1_DQ0_RK2_TX_ARDQ7_DLY_B1                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU1_R2_B1_DQ1                       (DDRPHY_AO_BASE_ADDRESS + 0x01054)
    #define SHU1_R2_B1_DQ1_RK2_TX_ARDQM0_DLY_B1                Fld(4, 0) //[3:0]
    #define SHU1_R2_B1_DQ1_RK2_TX_ARDQS0_DLY_B1                Fld(4, 24) //[27:24]
    #define SHU1_R2_B1_DQ1_RK2_TX_ARDQS0B_DLY_B1               Fld(4, 28) //[31:28]

#define DDRPHY_SHU1_R2_B1_DQ2                       (DDRPHY_AO_BASE_ADDRESS + 0x01058)
    #define SHU1_R2_B1_DQ2_RK2_RX_ARDQ0_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU1_R2_B1_DQ2_RK2_RX_ARDQ0_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU1_R2_B1_DQ2_RK2_RX_ARDQ1_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU1_R2_B1_DQ2_RK2_RX_ARDQ1_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R2_B1_DQ3                       (DDRPHY_AO_BASE_ADDRESS + 0x0105C)
    #define SHU1_R2_B1_DQ3_RK2_RX_ARDQ2_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU1_R2_B1_DQ3_RK2_RX_ARDQ2_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU1_R2_B1_DQ3_RK2_RX_ARDQ3_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU1_R2_B1_DQ3_RK2_RX_ARDQ3_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R2_B1_DQ4                       (DDRPHY_AO_BASE_ADDRESS + 0x01060)
    #define SHU1_R2_B1_DQ4_RK2_RX_ARDQ4_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU1_R2_B1_DQ4_RK2_RX_ARDQ4_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU1_R2_B1_DQ4_RK2_RX_ARDQ5_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU1_R2_B1_DQ4_RK2_RX_ARDQ5_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R2_B1_DQ5                       (DDRPHY_AO_BASE_ADDRESS + 0x01064)
    #define SHU1_R2_B1_DQ5_RK2_RX_ARDQ6_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU1_R2_B1_DQ5_RK2_RX_ARDQ6_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU1_R2_B1_DQ5_RK2_RX_ARDQ7_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU1_R2_B1_DQ5_RK2_RX_ARDQ7_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R2_B1_DQ6                       (DDRPHY_AO_BASE_ADDRESS + 0x01068)
    #define SHU1_R2_B1_DQ6_RK2_RX_ARDQM0_R_DLY_B1              Fld(6, 0) //[5:0]
    #define SHU1_R2_B1_DQ6_RK2_RX_ARDQM0_F_DLY_B1              Fld(6, 8) //[13:8]
    #define SHU1_R2_B1_DQ6_RK2_RX_ARDQS0_R_DLY_B1              Fld(7, 16) //[22:16]
    #define SHU1_R2_B1_DQ6_RK2_RX_ARDQS0_F_DLY_B1              Fld(7, 24) //[30:24]

#define DDRPHY_SHU1_R2_B1_DQ7                       (DDRPHY_AO_BASE_ADDRESS + 0x0106C)
    #define SHU1_R2_B1_DQ7_RK2_ARPI_DQ_B1                      Fld(6, 8) //[13:8]
    #define SHU1_R2_B1_DQ7_RK2_ARPI_DQM_B1                     Fld(6, 16) //[21:16]
    #define SHU1_R2_B1_DQ7_RK2_ARPI_PBYTE_B1                   Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X1070                           (DDRPHY_AO_BASE_ADDRESS + 0x01070)
    #define RFU_0X1070_RESERVED_0X1070                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1074                           (DDRPHY_AO_BASE_ADDRESS + 0x01074)
    #define RFU_0X1074_RESERVED_0X1074                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1078                           (DDRPHY_AO_BASE_ADDRESS + 0x01078)
    #define RFU_0X1078_RESERVED_0X1078                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X107C                           (DDRPHY_AO_BASE_ADDRESS + 0x0107C)
    #define RFU_0X107C_RESERVED_0X107C                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU1_R2_CA_CMD0                      (DDRPHY_AO_BASE_ADDRESS + 0x010A0)
    #define SHU1_R2_CA_CMD0_RK2_TX_ARCA0_DLY                   Fld(4, 0) //[3:0]
    #define SHU1_R2_CA_CMD0_RK2_TX_ARCA1_DLY                   Fld(4, 4) //[7:4]
    #define SHU1_R2_CA_CMD0_RK2_TX_ARCA2_DLY                   Fld(4, 8) //[11:8]
    #define SHU1_R2_CA_CMD0_RK2_TX_ARCA3_DLY                   Fld(4, 12) //[15:12]
    #define SHU1_R2_CA_CMD0_RK2_TX_ARCA4_DLY                   Fld(4, 16) //[19:16]
    #define SHU1_R2_CA_CMD0_RK2_TX_ARCA5_DLY                   Fld(4, 20) //[23:20]
    #define SHU1_R2_CA_CMD0_RK2_TX_ARCLK_DLYB                  Fld(4, 24) //[27:24]
    #define SHU1_R2_CA_CMD0_RK2_TX_ARCLKB_DLYB                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU1_R2_CA_CMD1                      (DDRPHY_AO_BASE_ADDRESS + 0x010A4)
    #define SHU1_R2_CA_CMD1_RK2_TX_ARCKE0_DLY                  Fld(4, 0) //[3:0]
    #define SHU1_R2_CA_CMD1_RK2_TX_ARCKE1_DLY                  Fld(4, 4) //[7:4]
    #define SHU1_R2_CA_CMD1_RK2_TX_ARCKE2_DLY                  Fld(4, 8) //[11:8]
    #define SHU1_R2_CA_CMD1_RK2_TX_ARCS0_DLY                   Fld(4, 12) //[15:12]
    #define SHU1_R2_CA_CMD1_RK2_TX_ARCS1_DLY                   Fld(4, 16) //[19:16]
    #define SHU1_R2_CA_CMD1_RK2_TX_ARCS2_DLY                   Fld(4, 20) //[23:20]
    #define SHU1_R2_CA_CMD1_RK2_TX_ARCLK_DLY                   Fld(4, 24) //[27:24]
    #define SHU1_R2_CA_CMD1_RK2_TX_ARCLKB_DLY                  Fld(4, 28) //[31:28]

#define DDRPHY_SHU1_R2_CA_CMD2                      (DDRPHY_AO_BASE_ADDRESS + 0x010A8)
    #define SHU1_R2_CA_CMD2_RG_RK2_RX_ARCA0_R_DLY              Fld(6, 0) //[5:0]
    #define SHU1_R2_CA_CMD2_RG_RK2_RX_ARCA0_F_DLY              Fld(6, 8) //[13:8]
    #define SHU1_R2_CA_CMD2_RG_RK2_RX_ARCA1_R_DLY              Fld(6, 16) //[21:16]
    #define SHU1_R2_CA_CMD2_RG_RK2_RX_ARCA1_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R2_CA_CMD3                      (DDRPHY_AO_BASE_ADDRESS + 0x010AC)
    #define SHU1_R2_CA_CMD3_RG_RK2_RX_ARCA2_R_DLY              Fld(6, 0) //[5:0]
    #define SHU1_R2_CA_CMD3_RG_RK2_RX_ARCA2_F_DLY              Fld(6, 8) //[13:8]
    #define SHU1_R2_CA_CMD3_RG_RK2_RX_ARCA3_R_DLY              Fld(6, 16) //[21:16]
    #define SHU1_R2_CA_CMD3_RG_RK2_RX_ARCA3_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R2_CA_CMD4                      (DDRPHY_AO_BASE_ADDRESS + 0x010B0)
    #define SHU1_R2_CA_CMD4_RG_RK2_RX_ARCA4_R_DLY              Fld(6, 0) //[5:0]
    #define SHU1_R2_CA_CMD4_RG_RK2_RX_ARCA4_F_DLY              Fld(6, 8) //[13:8]
    #define SHU1_R2_CA_CMD4_RG_RK2_RX_ARCA5_R_DLY              Fld(6, 16) //[21:16]
    #define SHU1_R2_CA_CMD4_RG_RK2_RX_ARCA5_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R2_CA_CMD5                      (DDRPHY_AO_BASE_ADDRESS + 0x010B4)
    #define SHU1_R2_CA_CMD5_RG_RK2_RX_ARCKE0_R_DLY             Fld(6, 0) //[5:0]
    #define SHU1_R2_CA_CMD5_RG_RK2_RX_ARCKE0_F_DLY             Fld(6, 8) //[13:8]
    #define SHU1_R2_CA_CMD5_RG_RK2_RX_ARCKE1_R_DLY             Fld(6, 16) //[21:16]
    #define SHU1_R2_CA_CMD5_RG_RK2_RX_ARCKE1_F_DLY             Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R2_CA_CMD6                      (DDRPHY_AO_BASE_ADDRESS + 0x010B8)
    #define SHU1_R2_CA_CMD6_RG_RK2_RX_ARCKE2_R_DLY             Fld(6, 0) //[5:0]
    #define SHU1_R2_CA_CMD6_RG_RK2_RX_ARCKE2_F_DLY             Fld(6, 8) //[13:8]
    #define SHU1_R2_CA_CMD6_RG_RK2_RX_ARCS0_R_DLY              Fld(6, 16) //[21:16]
    #define SHU1_R2_CA_CMD6_RG_RK2_RX_ARCS0_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R2_CA_CMD7                      (DDRPHY_AO_BASE_ADDRESS + 0x010BC)
    #define SHU1_R2_CA_CMD7_RG_RK2_RX_ARCS1_R_DLY              Fld(6, 0) //[5:0]
    #define SHU1_R2_CA_CMD7_RG_RK2_RX_ARCS1_F_DLY              Fld(6, 8) //[13:8]
    #define SHU1_R2_CA_CMD7_RG_RK2_RX_ARCS2_R_DLY              Fld(6, 16) //[21:16]
    #define SHU1_R2_CA_CMD7_RG_RK2_RX_ARCS2_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU1_R2_CA_CMD8                      (DDRPHY_AO_BASE_ADDRESS + 0x010C0)
    #define SHU1_R2_CA_CMD8_RG_RK2_RX_ARCLK_R_DLY              Fld(7, 16) //[22:16]
    #define SHU1_R2_CA_CMD8_RG_RK2_RX_ARCLK_F_DLY              Fld(7, 24) //[30:24]

#define DDRPHY_SHU1_R2_CA_CMD9                      (DDRPHY_AO_BASE_ADDRESS + 0x010C4)
    #define SHU1_R2_CA_CMD9_RG_RK2_ARPI_CS                     Fld(6, 0) //[5:0]
    #define SHU1_R2_CA_CMD9_RG_RK2_ARPI_CMD                    Fld(6, 8) //[13:8]
    #define SHU1_R2_CA_CMD9_RG_RK2_ARPI_CLK                    Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X10C8                           (DDRPHY_AO_BASE_ADDRESS + 0x010C8)
    #define RFU_0X10C8_RESERVED_0X10C8                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X10CC                           (DDRPHY_AO_BASE_ADDRESS + 0x010CC)
    #define RFU_0X10CC_RESERVED_0X10CC                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU2_B0_DQ0                          (DDRPHY_AO_BASE_ADDRESS + 0x01100)
    #define SHU2_B0_DQ0_RG_TX_ARDQS0_PRE_EN_B0                 Fld(1, 4) //[4:4]
    #define SHU2_B0_DQ0_RG_TX_ARDQS0_DRVP_PRE_B0               Fld(3, 8) //[10:8]
    #define SHU2_B0_DQ0_RG_TX_ARDQS0_DRVN_PRE_B0               Fld(3, 12) //[14:12]
    #define SHU2_B0_DQ0_RG_TX_ARDQ_PRE_EN_B0                   Fld(1, 20) //[20:20]
    #define SHU2_B0_DQ0_RG_TX_ARDQ_DRVP_PRE_B0                 Fld(3, 24) //[26:24]
    #define SHU2_B0_DQ0_RG_TX_ARDQ_DRVN_PRE_B0                 Fld(3, 28) //[30:28]
    #define SHU2_B0_DQ0_R_LP4Y_WDN_MODE_DQS0                   Fld(1, 31) //[31:31]

#define DDRPHY_SHU2_B0_DQ1                          (DDRPHY_AO_BASE_ADDRESS + 0x01104)
    #define SHU2_B0_DQ1_RG_TX_ARDQ_DRVP_B0                     Fld(5, 0) //[4:0]
    #define SHU2_B0_DQ1_RG_TX_ARDQ_DRVN_B0                     Fld(5, 8) //[12:8]
    #define SHU2_B0_DQ1_RG_TX_ARDQ_ODTP_B0                     Fld(5, 16) //[20:16]
    #define SHU2_B0_DQ1_RG_TX_ARDQ_ODTN_B0                     Fld(5, 24) //[28:24]

#define DDRPHY_SHU2_B0_DQ2                          (DDRPHY_AO_BASE_ADDRESS + 0x01108)
    #define SHU2_B0_DQ2_RG_TX_ARDQS0_DRVP_B0                   Fld(5, 0) //[4:0]
    #define SHU2_B0_DQ2_RG_TX_ARDQS0_DRVN_B0                   Fld(5, 8) //[12:8]
    #define SHU2_B0_DQ2_RG_TX_ARDQS0_ODTP_B0                   Fld(5, 16) //[20:16]
    #define SHU2_B0_DQ2_RG_TX_ARDQS0_ODTN_B0                   Fld(5, 24) //[28:24]

#define DDRPHY_SHU2_B0_DQ3                          (DDRPHY_AO_BASE_ADDRESS + 0x0110C)
    #define SHU2_B0_DQ3_RG_TX_ARDQS0_PU_B0                     Fld(2, 0) //[1:0]
    #define SHU2_B0_DQ3_RG_TX_ARDQS0_PU_PRE_B0                 Fld(2, 2) //[3:2]
    #define SHU2_B0_DQ3_RG_TX_ARDQS0_PDB_B0                    Fld(2, 4) //[5:4]
    #define SHU2_B0_DQ3_RG_TX_ARDQS0_PDB_PRE_B0                Fld(2, 6) //[7:6]
    #define SHU2_B0_DQ3_RG_TX_ARDQ_PU_B0                       Fld(2, 8) //[9:8]
    #define SHU2_B0_DQ3_RG_TX_ARDQ_PU_PRE_B0                   Fld(2, 10) //[11:10]
    #define SHU2_B0_DQ3_RG_TX_ARDQ_PDB_B0                      Fld(2, 12) //[13:12]
    #define SHU2_B0_DQ3_RG_TX_ARDQ_PDB_PRE_B0                  Fld(2, 14) //[15:14]

#define DDRPHY_SHU2_B0_DQ4                          (DDRPHY_AO_BASE_ADDRESS + 0x01110)
    #define SHU2_B0_DQ4_RG_ARPI_AA_MCK_DL_B0                   Fld(6, 0) //[5:0]
    #define SHU2_B0_DQ4_RG_ARPI_AA_MCK_FB_DL_B0                Fld(6, 8) //[13:8]
    #define SHU2_B0_DQ4_RG_ARPI_DA_MCK_FB_DL_B0                Fld(6, 16) //[21:16]

#define DDRPHY_SHU2_B0_DQ5                          (DDRPHY_AO_BASE_ADDRESS + 0x01114)
    #define SHU2_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0                 Fld(6, 0) //[5:0]
    #define SHU2_B0_DQ5_RG_RX_ARDQ_VREF_BYPASS_B0              Fld(1, 6) //[6:6]
    #define SHU2_B0_DQ5_RG_ARPI_FB_B0                          Fld(6, 8) //[13:8]
    #define SHU2_B0_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B0             Fld(3, 16) //[18:16]
    #define SHU2_B0_DQ5_DA_RX_ARDQS_DQSIEN_RB_DLY_B0           Fld(1, 19) //[19:19]
    #define SHU2_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0                Fld(3, 20) //[22:20]
    #define SHU2_B0_DQ5_RG_ARPI_MCTL_B0                        Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_B0_DQ6                          (DDRPHY_AO_BASE_ADDRESS + 0x01118)
    #define SHU2_B0_DQ6_RG_ARPI_OFFSET_DQSIEN_B0               Fld(6, 0) //[5:0]
    #define SHU2_B0_DQ6_RG_ARPI_RESERVE_B0                     Fld(16, 6) //[21:6]
    #define SHU2_B0_DQ6_RG_ARPI_MIDPI_CAP_SEL_B0               Fld(2, 22) //[23:22]
    #define SHU2_B0_DQ6_RG_ARPI_MIDPI_VTH_SEL_B0               Fld(2, 24) //[25:24]
    #define SHU2_B0_DQ6_RG_ARPI_MIDPI_EN_B0                    Fld(1, 26) //[26:26]
    #define SHU2_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0             Fld(1, 27) //[27:27]
    #define SHU2_B0_DQ6_RG_ARPI_CAP_SEL_B0                     Fld(2, 28) //[29:28]
    #define SHU2_B0_DQ6_RG_ARPI_MIDPI_BYPASS_EN_B0             Fld(1, 31) //[31:31]

#define DDRPHY_SHU2_B0_DQ7                          (DDRPHY_AO_BASE_ADDRESS + 0x0111C)
    #define SHU2_B0_DQ7_R_DMRANKRXDVS_B0                       Fld(4, 0) //[3:0]
    #define SHU2_B0_DQ7_MIDPI_ENABLE                           Fld(1, 4) //[4:4]
    #define SHU2_B0_DQ7_MIDPI_DIV4_ENABLE                      Fld(1, 5) //[5:5]
    #define SHU2_B0_DQ7_R_DMDQMDBI_EYE_SHU_B0                  Fld(1, 6) //[6:6]
    #define SHU2_B0_DQ7_R_DMDQMDBI_SHU_B0                      Fld(1, 7) //[7:7]
    #define SHU2_B0_DQ7_R_DMRXDVS_DQM_FLAGSEL_B0               Fld(4, 8) //[11:8]
    #define SHU2_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0            Fld(1, 12) //[12:12]
    #define SHU2_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0              Fld(1, 13) //[13:13]
    #define SHU2_B0_DQ7_R_DMRXTRACK_DQM_EN_B0                  Fld(1, 14) //[14:14]
    #define SHU2_B0_DQ7_R_DMRODTEN_B0                          Fld(1, 15) //[15:15]
    #define SHU2_B0_DQ7_R_DMARPI_CG_FB2DLL_DCM_EN_B0           Fld(1, 16) //[16:16]
    #define SHU2_B0_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B0               Fld(1, 17) //[17:17]
    #define SHU2_B0_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B0              Fld(1, 18) //[18:18]
    #define SHU2_B0_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B0              Fld(1, 19) //[19:19]
    #define SHU2_B0_DQ7_R_LP4Y_SDN_MODE_DQS0                   Fld(1, 20) //[20:20]
    #define SHU2_B0_DQ7_R_DMRXRANK_DQ_EN_B0                    Fld(1, 24) //[24:24]
    #define SHU2_B0_DQ7_R_DMRXRANK_DQ_LAT_B0                   Fld(3, 25) //[27:25]
    #define SHU2_B0_DQ7_R_DMRXRANK_DQS_EN_B0                   Fld(1, 28) //[28:28]
    #define SHU2_B0_DQ7_R_DMRXRANK_DQS_LAT_B0                  Fld(3, 29) //[31:29]

#define DDRPHY_SHU2_B0_DQ8                          (DDRPHY_AO_BASE_ADDRESS + 0x01120)
    #define SHU2_B0_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B0             Fld(15, 0) //[14:0]
    #define SHU2_B0_DQ8_R_DMRXDVS_UPD_FORCE_EN_B0              Fld(1, 15) //[15:15]
    #define SHU2_B0_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B0           Fld(1, 19) //[19:19]
    #define SHU2_B0_DQ8_R_RMRODTEN_CG_IG_B0                    Fld(1, 20) //[20:20]
    #define SHU2_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0                  Fld(1, 21) //[21:21]
    #define SHU2_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0          Fld(1, 22) //[22:22]
    #define SHU2_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0      Fld(1, 23) //[23:23]
    #define SHU2_B0_DQ8_R_DMRXDLY_CG_IG_B0                     Fld(1, 24) //[24:24]
    #define SHU2_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0                Fld(1, 25) //[25:25]
    #define SHU2_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0          Fld(1, 26) //[26:26]
    #define SHU2_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0          Fld(1, 27) //[27:27]
    #define SHU2_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0         Fld(1, 28) //[28:28]
    #define SHU2_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0     Fld(1, 29) //[29:29]
    #define SHU2_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0                 Fld(1, 30) //[30:30]
    #define SHU2_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0             Fld(1, 31) //[31:31]

#define DDRPHY_SHU2_B0_DQ9                          (DDRPHY_AO_BASE_ADDRESS + 0x01124)
    #define SHU2_B0_DQ9_RESERVED_0X1124                        Fld(32, 0) //[31:0]

#define DDRPHY_SHU2_B0_DQ10                         (DDRPHY_AO_BASE_ADDRESS + 0x01128)
    #define SHU2_B0_DQ10_RESERVED_0X1128                       Fld(32, 0) //[31:0]

#define DDRPHY_SHU2_B0_DQ11                         (DDRPHY_AO_BASE_ADDRESS + 0x0112C)
    #define SHU2_B0_DQ11_RESERVED_0X112C                       Fld(32, 0) //[31:0]

#define DDRPHY_SHU2_B0_DQ12                         (DDRPHY_AO_BASE_ADDRESS + 0x01130)
    #define SHU2_B0_DQ12_RESERVED_0X1130                       Fld(32, 0) //[31:0]

#define DDRPHY_SHU2_B0_DLL0                         (DDRPHY_AO_BASE_ADDRESS + 0x01134)
    #define SHU2_B0_DLL0_RG_ARPISM_MCK_SEL_B0_SHU              Fld(1, 0) //[0:0]
    #define SHU2_B0_DLL0_B0_DLL0_RFU                           Fld(1, 3) //[3:3]
    #define SHU2_B0_DLL0_RG_ARDLL_FAST_PSJP_B0                 Fld(1, 4) //[4:4]
    #define SHU2_B0_DLL0_RG_ARDLL_PHDIV_B0                     Fld(1, 9) //[9:9]
    #define SHU2_B0_DLL0_RG_ARDLL_PHJUMP_EN_B0                 Fld(1, 10) //[10:10]
    #define SHU2_B0_DLL0_RG_ARDLL_P_GAIN_B0                    Fld(4, 12) //[15:12]
    #define SHU2_B0_DLL0_RG_ARDLL_IDLECNT_B0                   Fld(4, 16) //[19:16]
    #define SHU2_B0_DLL0_RG_ARDLL_GAIN_B0                      Fld(4, 20) //[23:20]
    #define SHU2_B0_DLL0_RG_ARDLL_PHDET_IN_SWAP_B0             Fld(1, 30) //[30:30]
    #define SHU2_B0_DLL0_RG_ARDLL_PHDET_OUT_SEL_B0             Fld(1, 31) //[31:31]

#define DDRPHY_SHU2_B0_DLL1                         (DDRPHY_AO_BASE_ADDRESS + 0x01138)
    #define SHU2_B0_DLL1_RG_ARDLL_FASTPJ_CK_SEL_B0             Fld(1, 0) //[0:0]
    #define SHU2_B0_DLL1_RG_ARDLL_PS_EN_B0                     Fld(1, 1) //[1:1]
    #define SHU2_B0_DLL1_RG_ARDLL_PD_CK_SEL_B0                 Fld(1, 2) //[2:2]
    #define SHU2_B0_DLL1_RG_ARDQ_REV_B0                        Fld(24, 8) //[31:8]

#define DDRPHY_SHU2_B1_DQ0                          (DDRPHY_AO_BASE_ADDRESS + 0x01180)
    #define SHU2_B1_DQ0_RG_TX_ARDQS0_PRE_EN_B1                 Fld(1, 4) //[4:4]
    #define SHU2_B1_DQ0_RG_TX_ARDQS0_DRVP_PRE_B1               Fld(3, 8) //[10:8]
    #define SHU2_B1_DQ0_RG_TX_ARDQS0_DRVN_PRE_B1               Fld(3, 12) //[14:12]
    #define SHU2_B1_DQ0_RG_TX_ARDQ_PRE_EN_B1                   Fld(1, 20) //[20:20]
    #define SHU2_B1_DQ0_RG_TX_ARDQ_DRVP_PRE_B1                 Fld(3, 24) //[26:24]
    #define SHU2_B1_DQ0_RG_TX_ARDQ_DRVN_PRE_B1                 Fld(3, 28) //[30:28]
    #define SHU2_B1_DQ0_R_LP4Y_WDN_MODE_DQS1                   Fld(1, 31) //[31:31]

#define DDRPHY_SHU2_B1_DQ1                          (DDRPHY_AO_BASE_ADDRESS + 0x01184)
    #define SHU2_B1_DQ1_RG_TX_ARDQ_DRVP_B1                     Fld(5, 0) //[4:0]
    #define SHU2_B1_DQ1_RG_TX_ARDQ_DRVN_B1                     Fld(5, 8) //[12:8]
    #define SHU2_B1_DQ1_RG_TX_ARDQ_ODTP_B1                     Fld(5, 16) //[20:16]
    #define SHU2_B1_DQ1_RG_TX_ARDQ_ODTN_B1                     Fld(5, 24) //[28:24]

#define DDRPHY_SHU2_B1_DQ2                          (DDRPHY_AO_BASE_ADDRESS + 0x01188)
    #define SHU2_B1_DQ2_RG_TX_ARDQS0_DRVP_B1                   Fld(5, 0) //[4:0]
    #define SHU2_B1_DQ2_RG_TX_ARDQS0_DRVN_B1                   Fld(5, 8) //[12:8]
    #define SHU2_B1_DQ2_RG_TX_ARDQS0_ODTP_B1                   Fld(5, 16) //[20:16]
    #define SHU2_B1_DQ2_RG_TX_ARDQS0_ODTN_B1                   Fld(5, 24) //[28:24]

#define DDRPHY_SHU2_B1_DQ3                          (DDRPHY_AO_BASE_ADDRESS + 0x0118C)
    #define SHU2_B1_DQ3_RG_TX_ARDQS0_PU_B1                     Fld(2, 0) //[1:0]
    #define SHU2_B1_DQ3_RG_TX_ARDQS0_PU_PRE_B1                 Fld(2, 2) //[3:2]
    #define SHU2_B1_DQ3_RG_TX_ARDQS0_PDB_B1                    Fld(2, 4) //[5:4]
    #define SHU2_B1_DQ3_RG_TX_ARDQS0_PDB_PRE_B1                Fld(2, 6) //[7:6]
    #define SHU2_B1_DQ3_RG_TX_ARDQ_PU_B1                       Fld(2, 8) //[9:8]
    #define SHU2_B1_DQ3_RG_TX_ARDQ_PU_PRE_B1                   Fld(2, 10) //[11:10]
    #define SHU2_B1_DQ3_RG_TX_ARDQ_PDB_B1                      Fld(2, 12) //[13:12]
    #define SHU2_B1_DQ3_RG_TX_ARDQ_PDB_PRE_B1                  Fld(2, 14) //[15:14]

#define DDRPHY_SHU2_B1_DQ4                          (DDRPHY_AO_BASE_ADDRESS + 0x01190)
    #define SHU2_B1_DQ4_RG_ARPI_AA_MCK_DL_B1                   Fld(6, 0) //[5:0]
    #define SHU2_B1_DQ4_RG_ARPI_AA_MCK_FB_DL_B1                Fld(6, 8) //[13:8]
    #define SHU2_B1_DQ4_RG_ARPI_DA_MCK_FB_DL_B1                Fld(6, 16) //[21:16]

#define DDRPHY_SHU2_B1_DQ5                          (DDRPHY_AO_BASE_ADDRESS + 0x01194)
    #define SHU2_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1                 Fld(6, 0) //[5:0]
    #define SHU2_B1_DQ5_RG_RX_ARDQ_VREF_BYPASS_B1              Fld(1, 6) //[6:6]
    #define SHU2_B1_DQ5_RG_ARPI_FB_B1                          Fld(6, 8) //[13:8]
    #define SHU2_B1_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B1             Fld(3, 16) //[18:16]
    #define SHU2_B1_DQ5_DA_RX_ARDQS_DQSIEN_RB_DLY_B1           Fld(1, 19) //[19:19]
    #define SHU2_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1                Fld(3, 20) //[22:20]
    #define SHU2_B1_DQ5_RG_ARPI_MCTL_B1                        Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_B1_DQ6                          (DDRPHY_AO_BASE_ADDRESS + 0x01198)
    #define SHU2_B1_DQ6_RG_ARPI_OFFSET_DQSIEN_B1               Fld(6, 0) //[5:0]
    #define SHU2_B1_DQ6_RG_ARPI_RESERVE_B1                     Fld(16, 6) //[21:6]
    #define SHU2_B1_DQ6_RG_ARPI_MIDPI_CAP_SEL_B1               Fld(2, 22) //[23:22]
    #define SHU2_B1_DQ6_RG_ARPI_MIDPI_VTH_SEL_B1               Fld(2, 24) //[25:24]
    #define SHU2_B1_DQ6_RG_ARPI_MIDPI_EN_B1                    Fld(1, 26) //[26:26]
    #define SHU2_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1             Fld(1, 27) //[27:27]
    #define SHU2_B1_DQ6_RG_ARPI_CAP_SEL_B1                     Fld(2, 28) //[29:28]
    #define SHU2_B1_DQ6_RG_ARPI_MIDPI_BYPASS_EN_B1             Fld(1, 31) //[31:31]

#define DDRPHY_SHU2_B1_DQ7                          (DDRPHY_AO_BASE_ADDRESS + 0x0119C)
    #define SHU2_B1_DQ7_R_DMRANKRXDVS_B1                       Fld(4, 0) //[3:0]
    #define SHU2_B1_DQ7_R_DMDQMDBI_EYE_SHU_B1                  Fld(1, 6) //[6:6]
    #define SHU2_B1_DQ7_R_DMDQMDBI_SHU_B1                      Fld(1, 7) //[7:7]
    #define SHU2_B1_DQ7_R_DMRXDVS_DQM_FLAGSEL_B1               Fld(4, 8) //[11:8]
    #define SHU2_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1            Fld(1, 12) //[12:12]
    #define SHU2_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1              Fld(1, 13) //[13:13]
    #define SHU2_B1_DQ7_R_DMRXTRACK_DQM_EN_B1                  Fld(1, 14) //[14:14]
    #define SHU2_B1_DQ7_R_DMRODTEN_B1                          Fld(1, 15) //[15:15]
    #define SHU2_B1_DQ7_R_DMARPI_CG_FB2DLL_DCM_EN_B1           Fld(1, 16) //[16:16]
    #define SHU2_B1_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B1               Fld(1, 17) //[17:17]
    #define SHU2_B1_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B1              Fld(1, 18) //[18:18]
    #define SHU2_B1_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B1              Fld(1, 19) //[19:19]
    #define SHU2_B1_DQ7_R_LP4Y_SDN_MODE_DQS1                   Fld(1, 20) //[20:20]
    #define SHU2_B1_DQ7_R_DMRXRANK_DQ_EN_B1                    Fld(1, 24) //[24:24]
    #define SHU2_B1_DQ7_R_DMRXRANK_DQ_LAT_B1                   Fld(3, 25) //[27:25]
    #define SHU2_B1_DQ7_R_DMRXRANK_DQS_EN_B1                   Fld(1, 28) //[28:28]
    #define SHU2_B1_DQ7_R_DMRXRANK_DQS_LAT_B1                  Fld(3, 29) //[31:29]

#define DDRPHY_SHU2_B1_DQ8                          (DDRPHY_AO_BASE_ADDRESS + 0x011A0)
    #define SHU2_B1_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B1             Fld(15, 0) //[14:0]
    #define SHU2_B1_DQ8_R_DMRXDVS_UPD_FORCE_EN_B1              Fld(1, 15) //[15:15]
    #define SHU2_B1_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B1           Fld(1, 19) //[19:19]
    #define SHU2_B1_DQ8_R_RMRODTEN_CG_IG_B1                    Fld(1, 20) //[20:20]
    #define SHU2_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1                  Fld(1, 21) //[21:21]
    #define SHU2_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1          Fld(1, 22) //[22:22]
    #define SHU2_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1      Fld(1, 23) //[23:23]
    #define SHU2_B1_DQ8_R_DMRXDLY_CG_IG_B1                     Fld(1, 24) //[24:24]
    #define SHU2_B1_DQ8_R_DMSTBEN_SYNC_CG_IG_B1                Fld(1, 25) //[25:25]
    #define SHU2_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1          Fld(1, 26) //[26:26]
    #define SHU2_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1          Fld(1, 27) //[27:27]
    #define SHU2_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1         Fld(1, 28) //[28:28]
    #define SHU2_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1     Fld(1, 29) //[29:29]
    #define SHU2_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1                 Fld(1, 30) //[30:30]
    #define SHU2_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1             Fld(1, 31) //[31:31]

#define DDRPHY_SHU2_B1_DQ9                          (DDRPHY_AO_BASE_ADDRESS + 0x011A4)
    #define SHU2_B1_DQ9_RESERVED_0X11A4                        Fld(32, 0) //[31:0]

#define DDRPHY_SHU2_B1_DQ10                         (DDRPHY_AO_BASE_ADDRESS + 0x011A8)
    #define SHU2_B1_DQ10_RESERVED_0X11A8                       Fld(32, 0) //[31:0]

#define DDRPHY_SHU2_B1_DQ11                         (DDRPHY_AO_BASE_ADDRESS + 0x011AC)
    #define SHU2_B1_DQ11_RESERVED_0X11AC                       Fld(32, 0) //[31:0]

#define DDRPHY_SHU2_B1_DQ12                         (DDRPHY_AO_BASE_ADDRESS + 0x011B0)
    #define SHU2_B1_DQ12_RESERVED_0X11B0                       Fld(32, 0) //[31:0]

#define DDRPHY_SHU2_B1_DLL0                         (DDRPHY_AO_BASE_ADDRESS + 0x011B4)
    #define SHU2_B1_DLL0_RG_ARPISM_MCK_SEL_B1_SHU              Fld(1, 0) //[0:0]
    #define SHU2_B1_DLL0_B1_DLL0_RFU                           Fld(1, 3) //[3:3]
    #define SHU2_B1_DLL0_RG_ARDLL_FAST_PSJP_B1                 Fld(1, 4) //[4:4]
    #define SHU2_B1_DLL0_RG_ARDLL_PHDIV_B1                     Fld(1, 9) //[9:9]
    #define SHU2_B1_DLL0_RG_ARDLL_PHJUMP_EN_B1                 Fld(1, 10) //[10:10]
    #define SHU2_B1_DLL0_RG_ARDLL_P_GAIN_B1                    Fld(4, 12) //[15:12]
    #define SHU2_B1_DLL0_RG_ARDLL_IDLECNT_B1                   Fld(4, 16) //[19:16]
    #define SHU2_B1_DLL0_RG_ARDLL_GAIN_B1                      Fld(4, 20) //[23:20]
    #define SHU2_B1_DLL0_RG_ARDLL_PHDET_IN_SWAP_B1             Fld(1, 30) //[30:30]
    #define SHU2_B1_DLL0_RG_ARDLL_PHDET_OUT_SEL_B1             Fld(1, 31) //[31:31]

#define DDRPHY_SHU2_B1_DLL1                         (DDRPHY_AO_BASE_ADDRESS + 0x011B8)
    #define SHU2_B1_DLL1_RG_ARDLL_FASTPJ_CK_SEL_B1             Fld(1, 0) //[0:0]
    #define SHU2_B1_DLL1_RG_ARDLL_PS_EN_B1                     Fld(1, 1) //[1:1]
    #define SHU2_B1_DLL1_RG_ARDLL_PD_CK_SEL_B1                 Fld(1, 2) //[2:2]
    #define SHU2_B1_DLL1_RG_ARDQ_REV_B1                        Fld(24, 8) //[31:8]

#define DDRPHY_SHU2_CA_CMD0                         (DDRPHY_AO_BASE_ADDRESS + 0x01200)
    #define SHU2_CA_CMD0_RG_TX_ARCLK_PRE_EN                    Fld(1, 4) //[4:4]
    #define SHU2_CA_CMD0_RG_TX_ARCLK_DRVP_PRE                  Fld(3, 8) //[10:8]
    #define SHU2_CA_CMD0_RG_TX_ARCLK_DRVN_PRE                  Fld(3, 12) //[14:12]
    #define SHU2_CA_CMD0_RG_CGEN_FMEM_CK_CG_DLL                Fld(1, 17) //[17:17]
    #define SHU2_CA_CMD0_RG_FB_CK_MUX                          Fld(2, 18) //[19:18]
    #define SHU2_CA_CMD0_RG_TX_ARCMD_PRE_EN                    Fld(1, 20) //[20:20]
    #define SHU2_CA_CMD0_RG_TX_ARCMD_DRVP_PRE                  Fld(3, 24) //[26:24]
    #define SHU2_CA_CMD0_RG_TX_ARCMD_DRVN_PRE                  Fld(3, 28) //[30:28]
    #define SHU2_CA_CMD0_R_LP4Y_WDN_MODE_CLK                   Fld(1, 31) //[31:31]

#define DDRPHY_SHU2_CA_CMD1                         (DDRPHY_AO_BASE_ADDRESS + 0x01204)
    #define SHU2_CA_CMD1_RG_TX_ARCMD_DRVP                      Fld(5, 0) //[4:0]
    #define SHU2_CA_CMD1_RG_TX_ARCMD_DRVN                      Fld(5, 8) //[12:8]
    #define SHU2_CA_CMD1_RG_TX_ARCMD_ODTP                      Fld(5, 16) //[20:16]
    #define SHU2_CA_CMD1_RG_TX_ARCMD_ODTN                      Fld(5, 24) //[28:24]

#define DDRPHY_SHU2_CA_CMD2                         (DDRPHY_AO_BASE_ADDRESS + 0x01208)
    #define SHU2_CA_CMD2_RG_TX_ARCLK_DRVP                      Fld(5, 0) //[4:0]
    #define SHU2_CA_CMD2_RG_TX_ARCLK_DRVN                      Fld(5, 8) //[12:8]
    #define SHU2_CA_CMD2_RG_TX_ARCLK_ODTP                      Fld(5, 16) //[20:16]
    #define SHU2_CA_CMD2_RG_TX_ARCLK_ODTN                      Fld(5, 24) //[28:24]

#define DDRPHY_SHU2_CA_CMD3                         (DDRPHY_AO_BASE_ADDRESS + 0x0120C)
    #define SHU2_CA_CMD3_RG_TX_ARCLK_PU                        Fld(2, 0) //[1:0]
    #define SHU2_CA_CMD3_RG_TX_ARCLK_PU_PRE                    Fld(2, 2) //[3:2]
    #define SHU2_CA_CMD3_RG_TX_ARCLK_PDB                       Fld(2, 4) //[5:4]
    #define SHU2_CA_CMD3_RG_TX_ARCLK_PDB_PRE                   Fld(2, 6) //[7:6]
    #define SHU2_CA_CMD3_RG_TX_ARCMD_PU                        Fld(2, 8) //[9:8]
    #define SHU2_CA_CMD3_RG_TX_ARCMD_PU_PRE                    Fld(2, 10) //[11:10]
    #define SHU2_CA_CMD3_RG_TX_ARCMD_PDB                       Fld(2, 12) //[13:12]
    #define SHU2_CA_CMD3_RG_TX_ARCMD_PDB_PRE                   Fld(2, 14) //[15:14]

#define DDRPHY_SHU2_CA_CMD4                         (DDRPHY_AO_BASE_ADDRESS + 0x01210)
    #define SHU2_CA_CMD4_RG_ARPI_AA_MCK_DL_CA                  Fld(6, 0) //[5:0]
    #define SHU2_CA_CMD4_RG_ARPI_AA_MCK_FB_DL_CA               Fld(6, 8) //[13:8]
    #define SHU2_CA_CMD4_RG_ARPI_DA_MCK_FB_DL_CA               Fld(6, 16) //[21:16]

#define DDRPHY_SHU2_CA_CMD5                         (DDRPHY_AO_BASE_ADDRESS + 0x01214)
    #define SHU2_CA_CMD5_RG_RX_ARCMD_VREF_SEL                  Fld(6, 0) //[5:0]
    #define SHU2_CA_CMD5_RG_RX_ARCMD_VREF_BYPASS               Fld(1, 6) //[6:6]
    #define SHU2_CA_CMD5_RG_ARPI_FB_CA                         Fld(6, 8) //[13:8]
    #define SHU2_CA_CMD5_RG_RX_ARCLK_DQSIEN_DLY                Fld(3, 16) //[18:16]
    #define SHU2_CA_CMD5_DA_RX_ARCLK_DQSIEN_RB_DLY             Fld(1, 19) //[19:19]
    #define SHU2_CA_CMD5_RG_RX_ARCLK_DVS_DLY                   Fld(3, 20) //[22:20]
    #define SHU2_CA_CMD5_RG_ARPI_MCTL_CA                       Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_CA_CMD6                         (DDRPHY_AO_BASE_ADDRESS + 0x01218)
    #define SHU2_CA_CMD6_RG_ARPI_OFFSET_CLKIEN                 Fld(6, 0) //[5:0]
    #define SHU2_CA_CMD6_RG_ARPI_RESERVE_CA                    Fld(16, 6) //[21:6]
    #define SHU2_CA_CMD6_RG_ARPI_MIDPI_CAP_SEL_CA              Fld(2, 22) //[23:22]
    #define SHU2_CA_CMD6_RG_ARPI_MIDPI_VTH_SEL_CA              Fld(2, 24) //[25:24]
    #define SHU2_CA_CMD6_RG_ARPI_MIDPI_EN_CA                   Fld(1, 26) //[26:26]
    #define SHU2_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA            Fld(1, 27) //[27:27]
    #define SHU2_CA_CMD6_RG_ARPI_CAP_SEL_CA                    Fld(2, 28) //[29:28]
    #define SHU2_CA_CMD6_RG_ARPI_MIDPI_BYPASS_EN_CA            Fld(1, 31) //[31:31]

#define DDRPHY_SHU2_CA_CMD7                         (DDRPHY_AO_BASE_ADDRESS + 0x0121C)
    #define SHU2_CA_CMD7_R_DMRANKRXDVS_CA                      Fld(4, 0) //[3:0]
    #define SHU2_CA_CMD7_R_DMRXDVS_PBYTE_FLAG_OPT_CA           Fld(1, 12) //[12:12]
    #define SHU2_CA_CMD7_R_DMRODTEN_CA                         Fld(1, 15) //[15:15]
    #define SHU2_CA_CMD7_R_DMARPI_CG_FB2DLL_DCM_EN_CA          Fld(1, 16) //[16:16]
    #define SHU2_CA_CMD7_R_DMTX_ARPI_CG_CMD_NEW                Fld(1, 17) //[17:17]
    #define SHU2_CA_CMD7_R_DMTX_ARPI_CG_CLK_NEW                Fld(1, 18) //[18:18]
    #define SHU2_CA_CMD7_R_DMTX_ARPI_CG_CS_NEW                 Fld(1, 19) //[19:19]
    #define SHU2_CA_CMD7_R_LP4Y_SDN_MODE_CLK                   Fld(1, 20) //[20:20]
    #define SHU2_CA_CMD7_R_DMRXRANK_CMD_EN                     Fld(1, 24) //[24:24]
    #define SHU2_CA_CMD7_R_DMRXRANK_CMD_LAT                    Fld(3, 25) //[27:25]
    #define SHU2_CA_CMD7_R_DMRXRANK_CLK_EN                     Fld(1, 28) //[28:28]
    #define SHU2_CA_CMD7_R_DMRXRANK_CLK_LAT                    Fld(3, 29) //[31:29]

#define DDRPHY_SHU2_CA_CMD8                         (DDRPHY_AO_BASE_ADDRESS + 0x01220)
    #define SHU2_CA_CMD8_R_DMRXDVS_UPD_FORCE_CYC_CA            Fld(15, 0) //[14:0]
    #define SHU2_CA_CMD8_R_DMRXDVS_UPD_FORCE_EN_CA             Fld(1, 15) //[15:15]
    #define SHU2_CA_CMD8_R_DMRANK_RXDLY_PIPE_CG_IG_CA          Fld(1, 19) //[19:19]
    #define SHU2_CA_CMD8_R_RMRODTEN_CG_IG_CA                   Fld(1, 20) //[20:20]
    #define SHU2_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA                 Fld(1, 21) //[21:21]
    #define SHU2_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA         Fld(1, 22) //[22:22]
    #define SHU2_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA     Fld(1, 23) //[23:23]
    #define SHU2_CA_CMD8_R_DMRXDLY_CG_IG_CA                    Fld(1, 24) //[24:24]
    #define SHU2_CA_CMD8_R_DMSTBEN_SYNC_CG_IG_CA               Fld(1, 25) //[25:25]
    #define SHU2_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA         Fld(1, 26) //[26:26]
    #define SHU2_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA         Fld(1, 27) //[27:27]
    #define SHU2_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA        Fld(1, 28) //[28:28]
    #define SHU2_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA    Fld(1, 29) //[29:29]
    #define SHU2_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA                Fld(1, 30) //[30:30]
    #define SHU2_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA            Fld(1, 31) //[31:31]

#define DDRPHY_SHU2_CA_CMD9                         (DDRPHY_AO_BASE_ADDRESS + 0x01224)
    #define SHU2_CA_CMD9_RESERVED_0X1224                       Fld(32, 0) //[31:0]

#define DDRPHY_SHU2_CA_CMD10                        (DDRPHY_AO_BASE_ADDRESS + 0x01228)
    #define SHU2_CA_CMD10_RESERVED_0X1228                      Fld(32, 0) //[31:0]

#define DDRPHY_SHU2_CA_CMD11                        (DDRPHY_AO_BASE_ADDRESS + 0x0122C)
    #define SHU2_CA_CMD11_RG_RIMP_REV                          Fld(8, 0) //[7:0]
    #define SHU2_CA_CMD11_RG_RIMP_VREF_SEL                     Fld(6, 8) //[13:8]
    #define SHU2_CA_CMD11_RG_TX_ARCKE_DRVP                     Fld(5, 17) //[21:17]
    #define SHU2_CA_CMD11_RG_TX_ARCKE_DRVN                     Fld(5, 22) //[26:22]

#define DDRPHY_SHU2_CA_CMD12                        (DDRPHY_AO_BASE_ADDRESS + 0x01230)
    #define SHU2_CA_CMD12_RESERVED_0X1230                      Fld(32, 0) //[31:0]

#define DDRPHY_SHU2_CA_DLL0                         (DDRPHY_AO_BASE_ADDRESS + 0x01234)
    #define SHU2_CA_DLL0_RG_ARPISM_MCK_SEL_CA_SHU              Fld(1, 0) //[0:0]
    #define SHU2_CA_DLL0_CA_DLL0_RFU                           Fld(1, 3) //[3:3]
    #define SHU2_CA_DLL0_RG_ARDLL_FAST_PSJP_CA                 Fld(1, 4) //[4:4]
    #define SHU2_CA_DLL0_RG_ARDLL_PHDIV_CA                     Fld(1, 9) //[9:9]
    #define SHU2_CA_DLL0_RG_ARDLL_PHJUMP_EN_CA                 Fld(1, 10) //[10:10]
    #define SHU2_CA_DLL0_RG_ARDLL_P_GAIN_CA                    Fld(4, 12) //[15:12]
    #define SHU2_CA_DLL0_RG_ARDLL_IDLECNT_CA                   Fld(4, 16) //[19:16]
    #define SHU2_CA_DLL0_RG_ARDLL_GAIN_CA                      Fld(4, 20) //[23:20]
    #define SHU2_CA_DLL0_RG_ARDLL_PHDET_IN_SWAP_CA             Fld(1, 30) //[30:30]
    #define SHU2_CA_DLL0_RG_ARDLL_PHDET_OUT_SEL_CA             Fld(1, 31) //[31:31]

#define DDRPHY_SHU2_CA_DLL1                         (DDRPHY_AO_BASE_ADDRESS + 0x01238)
    #define SHU2_CA_DLL1_RG_ARDLL_FASTPJ_CK_SEL_CA             Fld(1, 0) //[0:0]
    #define SHU2_CA_DLL1_RG_ARDLL_PS_EN_CA                     Fld(1, 1) //[1:1]
    #define SHU2_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA                 Fld(1, 2) //[2:2]
    #define SHU2_CA_DLL1_RG_ARCMD_REV                          Fld(24, 8) //[31:8]

#define DDRPHY_SHU2_MISC0                           (DDRPHY_AO_BASE_ADDRESS + 0x012F0)
    #define SHU2_MISC0_R_RX_PIPE_BYPASS_EN                     Fld(1, 1) //[1:1]
    #define SHU2_MISC0_RG_CMD_TXPIPE_BYPASS_EN                 Fld(1, 2) //[2:2]
    #define SHU2_MISC0_RG_CK_TXPIPE_BYPASS_EN                  Fld(1, 3) //[3:3]
    #define SHU2_MISC0_RG_RVREF_SEL_DQ                         Fld(6, 16) //[21:16]
    #define SHU2_MISC0_RG_RVREF_DDR4_SEL                       Fld(1, 22) //[22:22]
    #define SHU2_MISC0_RG_RVREF_DDR3_SEL                       Fld(1, 23) //[23:23]
    #define SHU2_MISC0_RG_RVREF_SEL_CMD                        Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R0_B0_DQ0                       (DDRPHY_AO_BASE_ADDRESS + 0x01300)
    #define SHU2_R0_B0_DQ0_RK0_TX_ARDQ0_DLY_B0                 Fld(4, 0) //[3:0]
    #define SHU2_R0_B0_DQ0_RK0_TX_ARDQ1_DLY_B0                 Fld(4, 4) //[7:4]
    #define SHU2_R0_B0_DQ0_RK0_TX_ARDQ2_DLY_B0                 Fld(4, 8) //[11:8]
    #define SHU2_R0_B0_DQ0_RK0_TX_ARDQ3_DLY_B0                 Fld(4, 12) //[15:12]
    #define SHU2_R0_B0_DQ0_RK0_TX_ARDQ4_DLY_B0                 Fld(4, 16) //[19:16]
    #define SHU2_R0_B0_DQ0_RK0_TX_ARDQ5_DLY_B0                 Fld(4, 20) //[23:20]
    #define SHU2_R0_B0_DQ0_RK0_TX_ARDQ6_DLY_B0                 Fld(4, 24) //[27:24]
    #define SHU2_R0_B0_DQ0_RK0_TX_ARDQ7_DLY_B0                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU2_R0_B0_DQ1                       (DDRPHY_AO_BASE_ADDRESS + 0x01304)
    #define SHU2_R0_B0_DQ1_RK0_TX_ARDQM0_DLY_B0                Fld(4, 0) //[3:0]
    #define SHU2_R0_B0_DQ1_RK0_TX_ARDQS0_DLYB_B0               Fld(4, 16) //[19:16]
    #define SHU2_R0_B0_DQ1_RK0_TX_ARDQS0B_DLYB_B0              Fld(4, 20) //[23:20]
    #define SHU2_R0_B0_DQ1_RK0_TX_ARDQS0_DLY_B0                Fld(4, 24) //[27:24]
    #define SHU2_R0_B0_DQ1_RK0_TX_ARDQS0B_DLY_B0               Fld(4, 28) //[31:28]

#define DDRPHY_SHU2_R0_B0_DQ2                       (DDRPHY_AO_BASE_ADDRESS + 0x01308)
    #define SHU2_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU2_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU2_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU2_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R0_B0_DQ3                       (DDRPHY_AO_BASE_ADDRESS + 0x0130C)
    #define SHU2_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU2_R0_B0_DQ3_RK0_RX_ARDQ2_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU2_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU2_R0_B0_DQ3_RK0_RX_ARDQ3_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R0_B0_DQ4                       (DDRPHY_AO_BASE_ADDRESS + 0x01310)
    #define SHU2_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU2_R0_B0_DQ4_RK0_RX_ARDQ4_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU2_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU2_R0_B0_DQ4_RK0_RX_ARDQ5_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R0_B0_DQ5                       (DDRPHY_AO_BASE_ADDRESS + 0x01314)
    #define SHU2_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU2_R0_B0_DQ5_RK0_RX_ARDQ6_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU2_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU2_R0_B0_DQ5_RK0_RX_ARDQ7_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R0_B0_DQ6                       (DDRPHY_AO_BASE_ADDRESS + 0x01318)
    #define SHU2_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0              Fld(6, 0) //[5:0]
    #define SHU2_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0              Fld(6, 8) //[13:8]
    #define SHU2_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0              Fld(7, 16) //[22:16]
    #define SHU2_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0              Fld(7, 24) //[30:24]

#define DDRPHY_SHU2_R0_B0_DQ7                       (DDRPHY_AO_BASE_ADDRESS + 0x0131C)
    #define SHU2_R0_B0_DQ7_RK0_ARPI_DQ_B0                      Fld(6, 8) //[13:8]
    #define SHU2_R0_B0_DQ7_RK0_ARPI_DQM_B0                     Fld(6, 16) //[21:16]
    #define SHU2_R0_B0_DQ7_RK0_ARPI_PBYTE_B0                   Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X1320                           (DDRPHY_AO_BASE_ADDRESS + 0x01320)
    #define RFU_0X1320_RESERVED_0X1320                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1324                           (DDRPHY_AO_BASE_ADDRESS + 0x01324)
    #define RFU_0X1324_RESERVED_0X1324                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1328                           (DDRPHY_AO_BASE_ADDRESS + 0x01328)
    #define RFU_0X1328_RESERVED_0X1328                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X132C                           (DDRPHY_AO_BASE_ADDRESS + 0x0132C)
    #define RFU_0X132C_RESERVED_0X132C                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU2_R0_B1_DQ0                       (DDRPHY_AO_BASE_ADDRESS + 0x01350)
    #define SHU2_R0_B1_DQ0_RK0_TX_ARDQ0_DLY_B1                 Fld(4, 0) //[3:0]
    #define SHU2_R0_B1_DQ0_RK0_TX_ARDQ1_DLY_B1                 Fld(4, 4) //[7:4]
    #define SHU2_R0_B1_DQ0_RK0_TX_ARDQ2_DLY_B1                 Fld(4, 8) //[11:8]
    #define SHU2_R0_B1_DQ0_RK0_TX_ARDQ3_DLY_B1                 Fld(4, 12) //[15:12]
    #define SHU2_R0_B1_DQ0_RK0_TX_ARDQ4_DLY_B1                 Fld(4, 16) //[19:16]
    #define SHU2_R0_B1_DQ0_RK0_TX_ARDQ5_DLY_B1                 Fld(4, 20) //[23:20]
    #define SHU2_R0_B1_DQ0_RK0_TX_ARDQ6_DLY_B1                 Fld(4, 24) //[27:24]
    #define SHU2_R0_B1_DQ0_RK0_TX_ARDQ7_DLY_B1                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU2_R0_B1_DQ1                       (DDRPHY_AO_BASE_ADDRESS + 0x01354)
    #define SHU2_R0_B1_DQ1_RK0_TX_ARDQM0_DLY_B1                Fld(4, 0) //[3:0]
    #define SHU2_R0_B1_DQ1_RK0_TX_ARDQS0_DLYB_B1               Fld(4, 16) //[19:16]
    #define SHU2_R0_B1_DQ1_RK0_TX_ARDQS0B_DLYB_B1              Fld(4, 20) //[23:20]
    #define SHU2_R0_B1_DQ1_RK0_TX_ARDQS0_DLY_B1                Fld(4, 24) //[27:24]
    #define SHU2_R0_B1_DQ1_RK0_TX_ARDQS0B_DLY_B1               Fld(4, 28) //[31:28]

#define DDRPHY_SHU2_R0_B1_DQ2                       (DDRPHY_AO_BASE_ADDRESS + 0x01358)
    #define SHU2_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU2_R0_B1_DQ2_RK0_RX_ARDQ0_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU2_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU2_R0_B1_DQ2_RK0_RX_ARDQ1_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R0_B1_DQ3                       (DDRPHY_AO_BASE_ADDRESS + 0x0135C)
    #define SHU2_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU2_R0_B1_DQ3_RK0_RX_ARDQ2_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU2_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU2_R0_B1_DQ3_RK0_RX_ARDQ3_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R0_B1_DQ4                       (DDRPHY_AO_BASE_ADDRESS + 0x01360)
    #define SHU2_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU2_R0_B1_DQ4_RK0_RX_ARDQ4_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU2_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU2_R0_B1_DQ4_RK0_RX_ARDQ5_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R0_B1_DQ5                       (DDRPHY_AO_BASE_ADDRESS + 0x01364)
    #define SHU2_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU2_R0_B1_DQ5_RK0_RX_ARDQ6_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU2_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU2_R0_B1_DQ5_RK0_RX_ARDQ7_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R0_B1_DQ6                       (DDRPHY_AO_BASE_ADDRESS + 0x01368)
    #define SHU2_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1              Fld(6, 0) //[5:0]
    #define SHU2_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1              Fld(6, 8) //[13:8]
    #define SHU2_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1              Fld(7, 16) //[22:16]
    #define SHU2_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1              Fld(7, 24) //[30:24]

#define DDRPHY_SHU2_R0_B1_DQ7                       (DDRPHY_AO_BASE_ADDRESS + 0x0136C)
    #define SHU2_R0_B1_DQ7_RK0_ARPI_DQ_B1                      Fld(6, 8) //[13:8]
    #define SHU2_R0_B1_DQ7_RK0_ARPI_DQM_B1                     Fld(6, 16) //[21:16]
    #define SHU2_R0_B1_DQ7_RK0_ARPI_PBYTE_B1                   Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X1370                           (DDRPHY_AO_BASE_ADDRESS + 0x01370)
    #define RFU_0X1370_RESERVED_0X1370                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1374                           (DDRPHY_AO_BASE_ADDRESS + 0x01374)
    #define RFU_0X1374_RESERVED_0X1374                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1378                           (DDRPHY_AO_BASE_ADDRESS + 0x01378)
    #define RFU_0X1378_RESERVED_0X1378                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X137C                           (DDRPHY_AO_BASE_ADDRESS + 0x0137C)
    #define RFU_0X137C_RESERVED_0X137C                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU2_R0_CA_CMD0                      (DDRPHY_AO_BASE_ADDRESS + 0x013A0)
    #define SHU2_R0_CA_CMD0_RK0_TX_ARCA0_DLY                   Fld(4, 0) //[3:0]
    #define SHU2_R0_CA_CMD0_RK0_TX_ARCA1_DLY                   Fld(4, 4) //[7:4]
    #define SHU2_R0_CA_CMD0_RK0_TX_ARCA2_DLY                   Fld(4, 8) //[11:8]
    #define SHU2_R0_CA_CMD0_RK0_TX_ARCA3_DLY                   Fld(4, 12) //[15:12]
    #define SHU2_R0_CA_CMD0_RK0_TX_ARCA4_DLY                   Fld(4, 16) //[19:16]
    #define SHU2_R0_CA_CMD0_RK0_TX_ARCA5_DLY                   Fld(4, 20) //[23:20]
    #define SHU2_R0_CA_CMD0_RK0_TX_ARCLK_DLYB                  Fld(4, 24) //[27:24]
    #define SHU2_R0_CA_CMD0_RK0_TX_ARCLKB_DLYB                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU2_R0_CA_CMD1                      (DDRPHY_AO_BASE_ADDRESS + 0x013A4)
    #define SHU2_R0_CA_CMD1_RK0_TX_ARCKE0_DLY                  Fld(4, 0) //[3:0]
    #define SHU2_R0_CA_CMD1_RK0_TX_ARCKE1_DLY                  Fld(4, 4) //[7:4]
    #define SHU2_R0_CA_CMD1_RK0_TX_ARCKE2_DLY                  Fld(4, 8) //[11:8]
    #define SHU2_R0_CA_CMD1_RK0_TX_ARCS0_DLY                   Fld(4, 12) //[15:12]
    #define SHU2_R0_CA_CMD1_RK0_TX_ARCS1_DLY                   Fld(4, 16) //[19:16]
    #define SHU2_R0_CA_CMD1_RK0_TX_ARCS2_DLY                   Fld(4, 20) //[23:20]
    #define SHU2_R0_CA_CMD1_RK0_TX_ARCLK_DLY                   Fld(4, 24) //[27:24]
    #define SHU2_R0_CA_CMD1_RK0_TX_ARCLKB_DLY                  Fld(4, 28) //[31:28]

#define DDRPHY_SHU2_R0_CA_CMD2                      (DDRPHY_AO_BASE_ADDRESS + 0x013A8)
    #define SHU2_R0_CA_CMD2_RG_RK0_RX_ARCA0_R_DLY              Fld(6, 0) //[5:0]
    #define SHU2_R0_CA_CMD2_RG_RK0_RX_ARCA0_F_DLY              Fld(6, 8) //[13:8]
    #define SHU2_R0_CA_CMD2_RG_RK0_RX_ARCA1_R_DLY              Fld(6, 16) //[21:16]
    #define SHU2_R0_CA_CMD2_RG_RK0_RX_ARCA1_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R0_CA_CMD3                      (DDRPHY_AO_BASE_ADDRESS + 0x013AC)
    #define SHU2_R0_CA_CMD3_RG_RK0_RX_ARCA2_R_DLY              Fld(6, 0) //[5:0]
    #define SHU2_R0_CA_CMD3_RG_RK0_RX_ARCA2_F_DLY              Fld(6, 8) //[13:8]
    #define SHU2_R0_CA_CMD3_RG_RK0_RX_ARCA3_R_DLY              Fld(6, 16) //[21:16]
    #define SHU2_R0_CA_CMD3_RG_RK0_RX_ARCA3_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R0_CA_CMD4                      (DDRPHY_AO_BASE_ADDRESS + 0x013B0)
    #define SHU2_R0_CA_CMD4_RG_RK0_RX_ARCA4_R_DLY              Fld(6, 0) //[5:0]
    #define SHU2_R0_CA_CMD4_RG_RK0_RX_ARCA4_F_DLY              Fld(6, 8) //[13:8]
    #define SHU2_R0_CA_CMD4_RG_RK0_RX_ARCA5_R_DLY              Fld(6, 16) //[21:16]
    #define SHU2_R0_CA_CMD4_RG_RK0_RX_ARCA5_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R0_CA_CMD5                      (DDRPHY_AO_BASE_ADDRESS + 0x013B4)
    #define SHU2_R0_CA_CMD5_RG_RK0_RX_ARCKE0_R_DLY             Fld(6, 0) //[5:0]
    #define SHU2_R0_CA_CMD5_RG_RK0_RX_ARCKE0_F_DLY             Fld(6, 8) //[13:8]
    #define SHU2_R0_CA_CMD5_RG_RK0_RX_ARCKE1_R_DLY             Fld(6, 16) //[21:16]
    #define SHU2_R0_CA_CMD5_RG_RK0_RX_ARCKE1_F_DLY             Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R0_CA_CMD6                      (DDRPHY_AO_BASE_ADDRESS + 0x013B8)
    #define SHU2_R0_CA_CMD6_RG_RK0_RX_ARCKE2_R_DLY             Fld(6, 0) //[5:0]
    #define SHU2_R0_CA_CMD6_RG_RK0_RX_ARCKE2_F_DLY             Fld(6, 8) //[13:8]
    #define SHU2_R0_CA_CMD6_RG_RK0_RX_ARCS0_R_DLY              Fld(6, 16) //[21:16]
    #define SHU2_R0_CA_CMD6_RG_RK0_RX_ARCS0_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R0_CA_CMD7                      (DDRPHY_AO_BASE_ADDRESS + 0x013BC)
    #define SHU2_R0_CA_CMD7_RG_RK0_RX_ARCS1_R_DLY              Fld(6, 0) //[5:0]
    #define SHU2_R0_CA_CMD7_RG_RK0_RX_ARCS1_F_DLY              Fld(6, 8) //[13:8]
    #define SHU2_R0_CA_CMD7_RG_RK0_RX_ARCS2_R_DLY              Fld(6, 16) //[21:16]
    #define SHU2_R0_CA_CMD7_RG_RK0_RX_ARCS2_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R0_CA_CMD8                      (DDRPHY_AO_BASE_ADDRESS + 0x013C0)
    #define SHU2_R0_CA_CMD8_RG_RK0_RX_ARCLK_R_DLY              Fld(7, 16) //[22:16]
    #define SHU2_R0_CA_CMD8_RG_RK0_RX_ARCLK_F_DLY              Fld(7, 24) //[30:24]

#define DDRPHY_SHU2_R0_CA_CMD9                      (DDRPHY_AO_BASE_ADDRESS + 0x013C4)
    #define SHU2_R0_CA_CMD9_RG_RK0_ARPI_CS                     Fld(6, 0) //[5:0]
    #define SHU2_R0_CA_CMD9_RG_RK0_ARPI_CMD                    Fld(6, 8) //[13:8]
    #define SHU2_R0_CA_CMD9_RG_RK0_ARPI_CLK                    Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X13C8                           (DDRPHY_AO_BASE_ADDRESS + 0x013C8)
    #define RFU_0X13C8_RESERVED_0X13C8                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X13CC                           (DDRPHY_AO_BASE_ADDRESS + 0x013CC)
    #define RFU_0X13CC_RESERVED_0X13CC                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU2_R1_B0_DQ0                       (DDRPHY_AO_BASE_ADDRESS + 0x01400)
    #define SHU2_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0                 Fld(4, 0) //[3:0]
    #define SHU2_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0                 Fld(4, 4) //[7:4]
    #define SHU2_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0                 Fld(4, 8) //[11:8]
    #define SHU2_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0                 Fld(4, 12) //[15:12]
    #define SHU2_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0                 Fld(4, 16) //[19:16]
    #define SHU2_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0                 Fld(4, 20) //[23:20]
    #define SHU2_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0                 Fld(4, 24) //[27:24]
    #define SHU2_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU2_R1_B0_DQ1                       (DDRPHY_AO_BASE_ADDRESS + 0x01404)
    #define SHU2_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0                Fld(4, 0) //[3:0]
    #define SHU2_R1_B0_DQ1_RK1_TX_ARDQS0_DLYB_B0               Fld(4, 16) //[19:16]
    #define SHU2_R1_B0_DQ1_RK1_TX_ARDQS0B_DLYB_B0              Fld(4, 20) //[23:20]
    #define SHU2_R1_B0_DQ1_RK1_TX_ARDQS0_DLY_B0                Fld(4, 24) //[27:24]
    #define SHU2_R1_B0_DQ1_RK1_TX_ARDQS0B_DLY_B0               Fld(4, 28) //[31:28]

#define DDRPHY_SHU2_R1_B0_DQ2                       (DDRPHY_AO_BASE_ADDRESS + 0x01408)
    #define SHU2_R1_B0_DQ2_RK1_RX_ARDQ0_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU2_R1_B0_DQ2_RK1_RX_ARDQ0_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU2_R1_B0_DQ2_RK1_RX_ARDQ1_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU2_R1_B0_DQ2_RK1_RX_ARDQ1_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R1_B0_DQ3                       (DDRPHY_AO_BASE_ADDRESS + 0x0140C)
    #define SHU2_R1_B0_DQ3_RK1_RX_ARDQ2_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU2_R1_B0_DQ3_RK1_RX_ARDQ2_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU2_R1_B0_DQ3_RK1_RX_ARDQ3_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU2_R1_B0_DQ3_RK1_RX_ARDQ3_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R1_B0_DQ4                       (DDRPHY_AO_BASE_ADDRESS + 0x01410)
    #define SHU2_R1_B0_DQ4_RK1_RX_ARDQ4_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU2_R1_B0_DQ4_RK1_RX_ARDQ4_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU2_R1_B0_DQ4_RK1_RX_ARDQ5_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU2_R1_B0_DQ4_RK1_RX_ARDQ5_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R1_B0_DQ5                       (DDRPHY_AO_BASE_ADDRESS + 0x01414)
    #define SHU2_R1_B0_DQ5_RK1_RX_ARDQ6_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU2_R1_B0_DQ5_RK1_RX_ARDQ6_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU2_R1_B0_DQ5_RK1_RX_ARDQ7_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU2_R1_B0_DQ5_RK1_RX_ARDQ7_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R1_B0_DQ6                       (DDRPHY_AO_BASE_ADDRESS + 0x01418)
    #define SHU2_R1_B0_DQ6_RK1_RX_ARDQM0_R_DLY_B0              Fld(6, 0) //[5:0]
    #define SHU2_R1_B0_DQ6_RK1_RX_ARDQM0_F_DLY_B0              Fld(6, 8) //[13:8]
    #define SHU2_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0              Fld(7, 16) //[22:16]
    #define SHU2_R1_B0_DQ6_RK1_RX_ARDQS0_F_DLY_B0              Fld(7, 24) //[30:24]

#define DDRPHY_SHU2_R1_B0_DQ7                       (DDRPHY_AO_BASE_ADDRESS + 0x0141C)
    #define SHU2_R1_B0_DQ7_RK1_ARPI_DQ_B0                      Fld(6, 8) //[13:8]
    #define SHU2_R1_B0_DQ7_RK1_ARPI_DQM_B0                     Fld(6, 16) //[21:16]
    #define SHU2_R1_B0_DQ7_RK1_ARPI_PBYTE_B0                   Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X1420                           (DDRPHY_AO_BASE_ADDRESS + 0x01420)
    #define RFU_0X1420_RESERVED_0X1420                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1424                           (DDRPHY_AO_BASE_ADDRESS + 0x01424)
    #define RFU_0X1424_RESERVED_0X1424                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1428                           (DDRPHY_AO_BASE_ADDRESS + 0x01428)
    #define RFU_0X1428_RESERVED_0X1428                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X142C                           (DDRPHY_AO_BASE_ADDRESS + 0x0142C)
    #define RFU_0X142C_RESERVED_0X142C                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU2_R1_B1_DQ0                       (DDRPHY_AO_BASE_ADDRESS + 0x01450)
    #define SHU2_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1                 Fld(4, 0) //[3:0]
    #define SHU2_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1                 Fld(4, 4) //[7:4]
    #define SHU2_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1                 Fld(4, 8) //[11:8]
    #define SHU2_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1                 Fld(4, 12) //[15:12]
    #define SHU2_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1                 Fld(4, 16) //[19:16]
    #define SHU2_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1                 Fld(4, 20) //[23:20]
    #define SHU2_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1                 Fld(4, 24) //[27:24]
    #define SHU2_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU2_R1_B1_DQ1                       (DDRPHY_AO_BASE_ADDRESS + 0x01454)
    #define SHU2_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1                Fld(4, 0) //[3:0]
    #define SHU2_R1_B1_DQ1_RK1_TX_ARDQS0_DLYB_B1               Fld(4, 16) //[19:16]
    #define SHU2_R1_B1_DQ1_RK1_TX_ARDQS0B_DLYB_B1              Fld(4, 20) //[23:20]
    #define SHU2_R1_B1_DQ1_RK1_TX_ARDQS0_DLY_B1                Fld(4, 24) //[27:24]
    #define SHU2_R1_B1_DQ1_RK1_TX_ARDQS0B_DLY_B1               Fld(4, 28) //[31:28]

#define DDRPHY_SHU2_R1_B1_DQ2                       (DDRPHY_AO_BASE_ADDRESS + 0x01458)
    #define SHU2_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU2_R1_B1_DQ2_RK1_RX_ARDQ0_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU2_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU2_R1_B1_DQ2_RK1_RX_ARDQ1_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R1_B1_DQ3                       (DDRPHY_AO_BASE_ADDRESS + 0x0145C)
    #define SHU2_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU2_R1_B1_DQ3_RK1_RX_ARDQ2_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU2_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU2_R1_B1_DQ3_RK1_RX_ARDQ3_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R1_B1_DQ4                       (DDRPHY_AO_BASE_ADDRESS + 0x01460)
    #define SHU2_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU2_R1_B1_DQ4_RK1_RX_ARDQ4_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU2_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU2_R1_B1_DQ4_RK1_RX_ARDQ5_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R1_B1_DQ5                       (DDRPHY_AO_BASE_ADDRESS + 0x01464)
    #define SHU2_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU2_R1_B1_DQ5_RK1_RX_ARDQ6_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU2_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU2_R1_B1_DQ5_RK1_RX_ARDQ7_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R1_B1_DQ6                       (DDRPHY_AO_BASE_ADDRESS + 0x01468)
    #define SHU2_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1              Fld(6, 0) //[5:0]
    #define SHU2_R1_B1_DQ6_RK1_RX_ARDQM0_F_DLY_B1              Fld(6, 8) //[13:8]
    #define SHU2_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1              Fld(7, 16) //[22:16]
    #define SHU2_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1              Fld(7, 24) //[30:24]

#define DDRPHY_SHU2_R1_B1_DQ7                       (DDRPHY_AO_BASE_ADDRESS + 0x0146C)
    #define SHU2_R1_B1_DQ7_RK1_ARPI_DQ_B1                      Fld(6, 8) //[13:8]
    #define SHU2_R1_B1_DQ7_RK1_ARPI_DQM_B1                     Fld(6, 16) //[21:16]
    #define SHU2_R1_B1_DQ7_RK1_ARPI_PBYTE_B1                   Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X1470                           (DDRPHY_AO_BASE_ADDRESS + 0x01470)
    #define RFU_0X1470_RESERVED_0X1470                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1474                           (DDRPHY_AO_BASE_ADDRESS + 0x01474)
    #define RFU_0X1474_RESERVED_0X1474                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1478                           (DDRPHY_AO_BASE_ADDRESS + 0x01478)
    #define RFU_0X1478_RESERVED_0X1478                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X147C                           (DDRPHY_AO_BASE_ADDRESS + 0x0147C)
    #define RFU_0X147C_RESERVED_0X147C                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU2_R1_CA_CMD0                      (DDRPHY_AO_BASE_ADDRESS + 0x014A0)
    #define SHU2_R1_CA_CMD0_RK1_TX_ARCA0_DLY                   Fld(4, 0) //[3:0]
    #define SHU2_R1_CA_CMD0_RK1_TX_ARCA1_DLY                   Fld(4, 4) //[7:4]
    #define SHU2_R1_CA_CMD0_RK1_TX_ARCA2_DLY                   Fld(4, 8) //[11:8]
    #define SHU2_R1_CA_CMD0_RK1_TX_ARCA3_DLY                   Fld(4, 12) //[15:12]
    #define SHU2_R1_CA_CMD0_RK1_TX_ARCA4_DLY                   Fld(4, 16) //[19:16]
    #define SHU2_R1_CA_CMD0_RK1_TX_ARCA5_DLY                   Fld(4, 20) //[23:20]
    #define SHU2_R1_CA_CMD0_RK1_TX_ARCLK_DLYB                  Fld(4, 24) //[27:24]
    #define SHU2_R1_CA_CMD0_RK1_TX_ARCLKB_DLYB                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU2_R1_CA_CMD1                      (DDRPHY_AO_BASE_ADDRESS + 0x014A4)
    #define SHU2_R1_CA_CMD1_RK1_TX_ARCKE0_DLY                  Fld(4, 0) //[3:0]
    #define SHU2_R1_CA_CMD1_RK1_TX_ARCKE1_DLY                  Fld(4, 4) //[7:4]
    #define SHU2_R1_CA_CMD1_RK1_TX_ARCKE2_DLY                  Fld(4, 8) //[11:8]
    #define SHU2_R1_CA_CMD1_RK1_TX_ARCS0_DLY                   Fld(4, 12) //[15:12]
    #define SHU2_R1_CA_CMD1_RK1_TX_ARCS1_DLY                   Fld(4, 16) //[19:16]
    #define SHU2_R1_CA_CMD1_RK1_TX_ARCS2_DLY                   Fld(4, 20) //[23:20]
    #define SHU2_R1_CA_CMD1_RK1_TX_ARCLK_DLY                   Fld(4, 24) //[27:24]
    #define SHU2_R1_CA_CMD1_RK1_TX_ARCLKB_DLY                  Fld(4, 28) //[31:28]

#define DDRPHY_SHU2_R1_CA_CMD2                      (DDRPHY_AO_BASE_ADDRESS + 0x014A8)
    #define SHU2_R1_CA_CMD2_RG_RK1_RX_ARCA0_R_DLY              Fld(6, 0) //[5:0]
    #define SHU2_R1_CA_CMD2_RG_RK1_RX_ARCA0_F_DLY              Fld(6, 8) //[13:8]
    #define SHU2_R1_CA_CMD2_RG_RK1_RX_ARCA1_R_DLY              Fld(6, 16) //[21:16]
    #define SHU2_R1_CA_CMD2_RG_RK1_RX_ARCA1_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R1_CA_CMD3                      (DDRPHY_AO_BASE_ADDRESS + 0x014AC)
    #define SHU2_R1_CA_CMD3_RG_RK1_RX_ARCA2_R_DLY              Fld(6, 0) //[5:0]
    #define SHU2_R1_CA_CMD3_RG_RK1_RX_ARCA2_F_DLY              Fld(6, 8) //[13:8]
    #define SHU2_R1_CA_CMD3_RG_RK1_RX_ARCA3_R_DLY              Fld(6, 16) //[21:16]
    #define SHU2_R1_CA_CMD3_RG_RK1_RX_ARCA3_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R1_CA_CMD4                      (DDRPHY_AO_BASE_ADDRESS + 0x014B0)
    #define SHU2_R1_CA_CMD4_RG_RK1_RX_ARCA4_R_DLY              Fld(6, 0) //[5:0]
    #define SHU2_R1_CA_CMD4_RG_RK1_RX_ARCA4_F_DLY              Fld(6, 8) //[13:8]
    #define SHU2_R1_CA_CMD4_RG_RK1_RX_ARCA5_R_DLY              Fld(6, 16) //[21:16]
    #define SHU2_R1_CA_CMD4_RG_RK1_RX_ARCA5_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R1_CA_CMD5                      (DDRPHY_AO_BASE_ADDRESS + 0x014B4)
    #define SHU2_R1_CA_CMD5_RG_RK1_RX_ARCKE0_R_DLY             Fld(6, 0) //[5:0]
    #define SHU2_R1_CA_CMD5_RG_RK1_RX_ARCKE0_F_DLY             Fld(6, 8) //[13:8]
    #define SHU2_R1_CA_CMD5_RG_RK1_RX_ARCKE1_R_DLY             Fld(6, 16) //[21:16]
    #define SHU2_R1_CA_CMD5_RG_RK1_RX_ARCKE1_F_DLY             Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R1_CA_CMD6                      (DDRPHY_AO_BASE_ADDRESS + 0x014B8)
    #define SHU2_R1_CA_CMD6_RG_RK1_RX_ARCKE2_R_DLY             Fld(6, 0) //[5:0]
    #define SHU2_R1_CA_CMD6_RG_RK1_RX_ARCKE2_F_DLY             Fld(6, 8) //[13:8]
    #define SHU2_R1_CA_CMD6_RG_RK1_RX_ARCS0_R_DLY              Fld(6, 16) //[21:16]
    #define SHU2_R1_CA_CMD6_RG_RK1_RX_ARCS0_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R1_CA_CMD7                      (DDRPHY_AO_BASE_ADDRESS + 0x014BC)
    #define SHU2_R1_CA_CMD7_RG_RK1_RX_ARCS1_R_DLY              Fld(6, 0) //[5:0]
    #define SHU2_R1_CA_CMD7_RG_RK1_RX_ARCS1_F_DLY              Fld(6, 8) //[13:8]
    #define SHU2_R1_CA_CMD7_RG_RK1_RX_ARCS2_R_DLY              Fld(6, 16) //[21:16]
    #define SHU2_R1_CA_CMD7_RG_RK1_RX_ARCS2_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R1_CA_CMD8                      (DDRPHY_AO_BASE_ADDRESS + 0x014C0)
    #define SHU2_R1_CA_CMD8_RG_RK1_RX_ARCLK_R_DLY              Fld(7, 16) //[22:16]
    #define SHU2_R1_CA_CMD8_RG_RK1_RX_ARCLK_F_DLY              Fld(7, 24) //[30:24]

#define DDRPHY_SHU2_R1_CA_CMD9                      (DDRPHY_AO_BASE_ADDRESS + 0x014C4)
    #define SHU2_R1_CA_CMD9_RG_RK1_ARPI_CS                     Fld(6, 0) //[5:0]
    #define SHU2_R1_CA_CMD9_RG_RK1_ARPI_CMD                    Fld(6, 8) //[13:8]
    #define SHU2_R1_CA_CMD9_RG_RK1_ARPI_CLK                    Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X14C8                           (DDRPHY_AO_BASE_ADDRESS + 0x014C8)
    #define RFU_0X14C8_RESERVED_0X14C8                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X14CC                           (DDRPHY_AO_BASE_ADDRESS + 0x014CC)
    #define RFU_0X14CC_RESERVED_0X14CC                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU2_R2_B0_DQ0                       (DDRPHY_AO_BASE_ADDRESS + 0x01500)
    #define SHU2_R2_B0_DQ0_RK2_TX_ARDQ0_DLY_B0                 Fld(4, 0) //[3:0]
    #define SHU2_R2_B0_DQ0_RK2_TX_ARDQ1_DLY_B0                 Fld(4, 4) //[7:4]
    #define SHU2_R2_B0_DQ0_RK2_TX_ARDQ2_DLY_B0                 Fld(4, 8) //[11:8]
    #define SHU2_R2_B0_DQ0_RK2_TX_ARDQ3_DLY_B0                 Fld(4, 12) //[15:12]
    #define SHU2_R2_B0_DQ0_RK2_TX_ARDQ4_DLY_B0                 Fld(4, 16) //[19:16]
    #define SHU2_R2_B0_DQ0_RK2_TX_ARDQ5_DLY_B0                 Fld(4, 20) //[23:20]
    #define SHU2_R2_B0_DQ0_RK2_TX_ARDQ6_DLY_B0                 Fld(4, 24) //[27:24]
    #define SHU2_R2_B0_DQ0_RK2_TX_ARDQ7_DLY_B0                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU2_R2_B0_DQ1                       (DDRPHY_AO_BASE_ADDRESS + 0x01504)
    #define SHU2_R2_B0_DQ1_RK2_TX_ARDQM0_DLY_B0                Fld(4, 0) //[3:0]
    #define SHU2_R2_B0_DQ1_RK2_TX_ARDQS0_DLYB_B0               Fld(4, 16) //[19:16]
    #define SHU2_R2_B0_DQ1_RK2_TX_ARDQS0B_DLYB_B0              Fld(4, 20) //[23:20]
    #define SHU2_R2_B0_DQ1_RK2_TX_ARDQS0_DLY_B0                Fld(4, 24) //[27:24]
    #define SHU2_R2_B0_DQ1_RK2_TX_ARDQS0B_DLY_B0               Fld(4, 28) //[31:28]

#define DDRPHY_SHU2_R2_B0_DQ2                       (DDRPHY_AO_BASE_ADDRESS + 0x01508)
    #define SHU2_R2_B0_DQ2_RK2_RX_ARDQ0_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU2_R2_B0_DQ2_RK2_RX_ARDQ0_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU2_R2_B0_DQ2_RK2_RX_ARDQ1_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU2_R2_B0_DQ2_RK2_RX_ARDQ1_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R2_B0_DQ3                       (DDRPHY_AO_BASE_ADDRESS + 0x0150C)
    #define SHU2_R2_B0_DQ3_RK2_RX_ARDQ2_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU2_R2_B0_DQ3_RK2_RX_ARDQ2_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU2_R2_B0_DQ3_RK2_RX_ARDQ3_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU2_R2_B0_DQ3_RK2_RX_ARDQ3_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R2_B0_DQ4                       (DDRPHY_AO_BASE_ADDRESS + 0x01510)
    #define SHU2_R2_B0_DQ4_RK2_RX_ARDQ4_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU2_R2_B0_DQ4_RK2_RX_ARDQ4_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU2_R2_B0_DQ4_RK2_RX_ARDQ5_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU2_R2_B0_DQ4_RK2_RX_ARDQ5_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R2_B0_DQ5                       (DDRPHY_AO_BASE_ADDRESS + 0x01514)
    #define SHU2_R2_B0_DQ5_RK2_RX_ARDQ6_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU2_R2_B0_DQ5_RK2_RX_ARDQ6_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU2_R2_B0_DQ5_RK2_RX_ARDQ7_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU2_R2_B0_DQ5_RK2_RX_ARDQ7_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R2_B0_DQ6                       (DDRPHY_AO_BASE_ADDRESS + 0x01518)
    #define SHU2_R2_B0_DQ6_RK2_RX_ARDQM0_R_DLY_B0              Fld(6, 0) //[5:0]
    #define SHU2_R2_B0_DQ6_RK2_RX_ARDQM0_F_DLY_B0              Fld(6, 8) //[13:8]
    #define SHU2_R2_B0_DQ6_RK2_RX_ARDQS0_R_DLY_B0              Fld(7, 16) //[22:16]
    #define SHU2_R2_B0_DQ6_RK2_RX_ARDQS0_F_DLY_B0              Fld(7, 24) //[30:24]

#define DDRPHY_SHU2_R2_B0_DQ7                       (DDRPHY_AO_BASE_ADDRESS + 0x0151C)
    #define SHU2_R2_B0_DQ7_RK2_ARPI_DQ_B0                      Fld(6, 8) //[13:8]
    #define SHU2_R2_B0_DQ7_RK2_ARPI_DQM_B0                     Fld(6, 16) //[21:16]
    #define SHU2_R2_B0_DQ7_RK2_ARPI_PBYTE_B0                   Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X1520                           (DDRPHY_AO_BASE_ADDRESS + 0x01520)
    #define RFU_0X1520_RESERVED_0X1520                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1524                           (DDRPHY_AO_BASE_ADDRESS + 0x01524)
    #define RFU_0X1524_RESERVED_0X1524                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1528                           (DDRPHY_AO_BASE_ADDRESS + 0x01528)
    #define RFU_0X1528_RESERVED_0X1528                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X152C                           (DDRPHY_AO_BASE_ADDRESS + 0x0152C)
    #define RFU_0X152C_RESERVED_0X152C                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU2_R2_B1_DQ0                       (DDRPHY_AO_BASE_ADDRESS + 0x01550)
    #define SHU2_R2_B1_DQ0_RK2_TX_ARDQ0_DLY_B1                 Fld(4, 0) //[3:0]
    #define SHU2_R2_B1_DQ0_RK2_TX_ARDQ1_DLY_B1                 Fld(4, 4) //[7:4]
    #define SHU2_R2_B1_DQ0_RK2_TX_ARDQ2_DLY_B1                 Fld(4, 8) //[11:8]
    #define SHU2_R2_B1_DQ0_RK2_TX_ARDQ3_DLY_B1                 Fld(4, 12) //[15:12]
    #define SHU2_R2_B1_DQ0_RK2_TX_ARDQ4_DLY_B1                 Fld(4, 16) //[19:16]
    #define SHU2_R2_B1_DQ0_RK2_TX_ARDQ5_DLY_B1                 Fld(4, 20) //[23:20]
    #define SHU2_R2_B1_DQ0_RK2_TX_ARDQ6_DLY_B1                 Fld(4, 24) //[27:24]
    #define SHU2_R2_B1_DQ0_RK2_TX_ARDQ7_DLY_B1                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU2_R2_B1_DQ1                       (DDRPHY_AO_BASE_ADDRESS + 0x01554)
    #define SHU2_R2_B1_DQ1_RK2_TX_ARDQM0_DLY_B1                Fld(4, 0) //[3:0]
    #define SHU2_R2_B1_DQ1_RK2_TX_ARDQS0_DLYB_B1               Fld(4, 16) //[19:16]
    #define SHU2_R2_B1_DQ1_RK2_TX_ARDQS0B_DLYB_B1              Fld(4, 20) //[23:20]
    #define SHU2_R2_B1_DQ1_RK2_TX_ARDQS0_DLY_B1                Fld(4, 24) //[27:24]
    #define SHU2_R2_B1_DQ1_RK2_TX_ARDQS0B_DLY_B1               Fld(4, 28) //[31:28]

#define DDRPHY_SHU2_R2_B1_DQ2                       (DDRPHY_AO_BASE_ADDRESS + 0x01558)
    #define SHU2_R2_B1_DQ2_RK2_RX_ARDQ0_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU2_R2_B1_DQ2_RK2_RX_ARDQ0_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU2_R2_B1_DQ2_RK2_RX_ARDQ1_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU2_R2_B1_DQ2_RK2_RX_ARDQ1_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R2_B1_DQ3                       (DDRPHY_AO_BASE_ADDRESS + 0x0155C)
    #define SHU2_R2_B1_DQ3_RK2_RX_ARDQ2_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU2_R2_B1_DQ3_RK2_RX_ARDQ2_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU2_R2_B1_DQ3_RK2_RX_ARDQ3_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU2_R2_B1_DQ3_RK2_RX_ARDQ3_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R2_B1_DQ4                       (DDRPHY_AO_BASE_ADDRESS + 0x01560)
    #define SHU2_R2_B1_DQ4_RK2_RX_ARDQ4_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU2_R2_B1_DQ4_RK2_RX_ARDQ4_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU2_R2_B1_DQ4_RK2_RX_ARDQ5_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU2_R2_B1_DQ4_RK2_RX_ARDQ5_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R2_B1_DQ5                       (DDRPHY_AO_BASE_ADDRESS + 0x01564)
    #define SHU2_R2_B1_DQ5_RK2_RX_ARDQ6_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU2_R2_B1_DQ5_RK2_RX_ARDQ6_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU2_R2_B1_DQ5_RK2_RX_ARDQ7_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU2_R2_B1_DQ5_RK2_RX_ARDQ7_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R2_B1_DQ6                       (DDRPHY_AO_BASE_ADDRESS + 0x01568)
    #define SHU2_R2_B1_DQ6_RK2_RX_ARDQM0_R_DLY_B1              Fld(6, 0) //[5:0]
    #define SHU2_R2_B1_DQ6_RK2_RX_ARDQM0_F_DLY_B1              Fld(6, 8) //[13:8]
    #define SHU2_R2_B1_DQ6_RK2_RX_ARDQS0_R_DLY_B1              Fld(7, 16) //[22:16]
    #define SHU2_R2_B1_DQ6_RK2_RX_ARDQS0_F_DLY_B1              Fld(7, 24) //[30:24]

#define DDRPHY_SHU2_R2_B1_DQ7                       (DDRPHY_AO_BASE_ADDRESS + 0x0156C)
    #define SHU2_R2_B1_DQ7_RK2_ARPI_DQ_B1                      Fld(6, 8) //[13:8]
    #define SHU2_R2_B1_DQ7_RK2_ARPI_DQM_B1                     Fld(6, 16) //[21:16]
    #define SHU2_R2_B1_DQ7_RK2_ARPI_PBYTE_B1                   Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X1570                           (DDRPHY_AO_BASE_ADDRESS + 0x01570)
    #define RFU_0X1570_RESERVED_0X1570                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1574                           (DDRPHY_AO_BASE_ADDRESS + 0x01574)
    #define RFU_0X1574_RESERVED_0X1574                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1578                           (DDRPHY_AO_BASE_ADDRESS + 0x01578)
    #define RFU_0X1578_RESERVED_0X1578                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X157C                           (DDRPHY_AO_BASE_ADDRESS + 0x0157C)
    #define RFU_0X157C_RESERVED_0X157C                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU2_R2_CA_CMD0                      (DDRPHY_AO_BASE_ADDRESS + 0x015A0)
    #define SHU2_R2_CA_CMD0_RK2_TX_ARCA0_DLY                   Fld(4, 0) //[3:0]
    #define SHU2_R2_CA_CMD0_RK2_TX_ARCA1_DLY                   Fld(4, 4) //[7:4]
    #define SHU2_R2_CA_CMD0_RK2_TX_ARCA2_DLY                   Fld(4, 8) //[11:8]
    #define SHU2_R2_CA_CMD0_RK2_TX_ARCA3_DLY                   Fld(4, 12) //[15:12]
    #define SHU2_R2_CA_CMD0_RK2_TX_ARCA4_DLY                   Fld(4, 16) //[19:16]
    #define SHU2_R2_CA_CMD0_RK2_TX_ARCA5_DLY                   Fld(4, 20) //[23:20]
    #define SHU2_R2_CA_CMD0_RK2_TX_ARCLK_DLYB                  Fld(4, 24) //[27:24]
    #define SHU2_R2_CA_CMD0_RK2_TX_ARCLKB_DLYB                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU2_R2_CA_CMD1                      (DDRPHY_AO_BASE_ADDRESS + 0x015A4)
    #define SHU2_R2_CA_CMD1_RK2_TX_ARCKE0_DLY                  Fld(4, 0) //[3:0]
    #define SHU2_R2_CA_CMD1_RK2_TX_ARCKE1_DLY                  Fld(4, 4) //[7:4]
    #define SHU2_R2_CA_CMD1_RK2_TX_ARCKE2_DLY                  Fld(4, 8) //[11:8]
    #define SHU2_R2_CA_CMD1_RK2_TX_ARCS0_DLY                   Fld(4, 12) //[15:12]
    #define SHU2_R2_CA_CMD1_RK2_TX_ARCS1_DLY                   Fld(4, 16) //[19:16]
    #define SHU2_R2_CA_CMD1_RK2_TX_ARCS2_DLY                   Fld(4, 20) //[23:20]
    #define SHU2_R2_CA_CMD1_RK2_TX_ARCLK_DLY                   Fld(4, 24) //[27:24]
    #define SHU2_R2_CA_CMD1_RK2_TX_ARCLKB_DLY                  Fld(4, 28) //[31:28]

#define DDRPHY_SHU2_R2_CA_CMD2                      (DDRPHY_AO_BASE_ADDRESS + 0x015A8)
    #define SHU2_R2_CA_CMD2_RG_RK2_RX_ARCA0_R_DLY              Fld(6, 0) //[5:0]
    #define SHU2_R2_CA_CMD2_RG_RK2_RX_ARCA0_F_DLY              Fld(6, 8) //[13:8]
    #define SHU2_R2_CA_CMD2_RG_RK2_RX_ARCA1_R_DLY              Fld(6, 16) //[21:16]
    #define SHU2_R2_CA_CMD2_RG_RK2_RX_ARCA1_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R2_CA_CMD3                      (DDRPHY_AO_BASE_ADDRESS + 0x015AC)
    #define SHU2_R2_CA_CMD3_RG_RK2_RX_ARCA2_R_DLY              Fld(6, 0) //[5:0]
    #define SHU2_R2_CA_CMD3_RG_RK2_RX_ARCA2_F_DLY              Fld(6, 8) //[13:8]
    #define SHU2_R2_CA_CMD3_RG_RK2_RX_ARCA3_R_DLY              Fld(6, 16) //[21:16]
    #define SHU2_R2_CA_CMD3_RG_RK2_RX_ARCA3_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R2_CA_CMD4                      (DDRPHY_AO_BASE_ADDRESS + 0x015B0)
    #define SHU2_R2_CA_CMD4_RG_RK2_RX_ARCA4_R_DLY              Fld(6, 0) //[5:0]
    #define SHU2_R2_CA_CMD4_RG_RK2_RX_ARCA4_F_DLY              Fld(6, 8) //[13:8]
    #define SHU2_R2_CA_CMD4_RG_RK2_RX_ARCA5_R_DLY              Fld(6, 16) //[21:16]
    #define SHU2_R2_CA_CMD4_RG_RK2_RX_ARCA5_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R2_CA_CMD5                      (DDRPHY_AO_BASE_ADDRESS + 0x015B4)
    #define SHU2_R2_CA_CMD5_RG_RK2_RX_ARCKE0_R_DLY             Fld(6, 0) //[5:0]
    #define SHU2_R2_CA_CMD5_RG_RK2_RX_ARCKE0_F_DLY             Fld(6, 8) //[13:8]
    #define SHU2_R2_CA_CMD5_RG_RK2_RX_ARCKE1_R_DLY             Fld(6, 16) //[21:16]
    #define SHU2_R2_CA_CMD5_RG_RK2_RX_ARCKE1_F_DLY             Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R2_CA_CMD6                      (DDRPHY_AO_BASE_ADDRESS + 0x015B8)
    #define SHU2_R2_CA_CMD6_RG_RK2_RX_ARCKE2_R_DLY             Fld(6, 0) //[5:0]
    #define SHU2_R2_CA_CMD6_RG_RK2_RX_ARCKE2_F_DLY             Fld(6, 8) //[13:8]
    #define SHU2_R2_CA_CMD6_RG_RK2_RX_ARCS0_R_DLY              Fld(6, 16) //[21:16]
    #define SHU2_R2_CA_CMD6_RG_RK2_RX_ARCS0_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R2_CA_CMD7                      (DDRPHY_AO_BASE_ADDRESS + 0x015BC)
    #define SHU2_R2_CA_CMD7_RG_RK2_RX_ARCS1_R_DLY              Fld(6, 0) //[5:0]
    #define SHU2_R2_CA_CMD7_RG_RK2_RX_ARCS1_F_DLY              Fld(6, 8) //[13:8]
    #define SHU2_R2_CA_CMD7_RG_RK2_RX_ARCS2_R_DLY              Fld(6, 16) //[21:16]
    #define SHU2_R2_CA_CMD7_RG_RK2_RX_ARCS2_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU2_R2_CA_CMD8                      (DDRPHY_AO_BASE_ADDRESS + 0x015C0)
    #define SHU2_R2_CA_CMD8_RG_RK2_RX_ARCLK_R_DLY              Fld(7, 16) //[22:16]
    #define SHU2_R2_CA_CMD8_RG_RK2_RX_ARCLK_F_DLY              Fld(7, 24) //[30:24]

#define DDRPHY_SHU2_R2_CA_CMD9                      (DDRPHY_AO_BASE_ADDRESS + 0x015C4)
    #define SHU2_R2_CA_CMD9_RG_RK2_ARPI_CS                     Fld(6, 0) //[5:0]
    #define SHU2_R2_CA_CMD9_RG_RK2_ARPI_CMD                    Fld(6, 8) //[13:8]
    #define SHU2_R2_CA_CMD9_RG_RK2_ARPI_CLK                    Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X15C8                           (DDRPHY_AO_BASE_ADDRESS + 0x015C8)
    #define RFU_0X15C8_RESERVED_0X15C8                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X15CC                           (DDRPHY_AO_BASE_ADDRESS + 0x015CC)
    #define RFU_0X15CC_RESERVED_0X15CC                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU3_B0_DQ0                          (DDRPHY_AO_BASE_ADDRESS + 0x01600)
    #define SHU3_B0_DQ0_RG_TX_ARDQS0_PRE_EN_B0                 Fld(1, 4) //[4:4]
    #define SHU3_B0_DQ0_RG_TX_ARDQS0_DRVP_PRE_B0               Fld(3, 8) //[10:8]
    #define SHU3_B0_DQ0_RG_TX_ARDQS0_DRVN_PRE_B0               Fld(3, 12) //[14:12]
    #define SHU3_B0_DQ0_RG_TX_ARDQ_PRE_EN_B0                   Fld(1, 20) //[20:20]
    #define SHU3_B0_DQ0_RG_TX_ARDQ_DRVP_PRE_B0                 Fld(3, 24) //[26:24]
    #define SHU3_B0_DQ0_RG_TX_ARDQ_DRVN_PRE_B0                 Fld(3, 28) //[30:28]
    #define SHU3_B0_DQ0_R_LP4Y_WDN_MODE_DQS0                   Fld(1, 31) //[31:31]

#define DDRPHY_SHU3_B0_DQ1                          (DDRPHY_AO_BASE_ADDRESS + 0x01604)
    #define SHU3_B0_DQ1_RG_TX_ARDQ_DRVP_B0                     Fld(5, 0) //[4:0]
    #define SHU3_B0_DQ1_RG_TX_ARDQ_DRVN_B0                     Fld(5, 8) //[12:8]
    #define SHU3_B0_DQ1_RG_TX_ARDQ_ODTP_B0                     Fld(5, 16) //[20:16]
    #define SHU3_B0_DQ1_RG_TX_ARDQ_ODTN_B0                     Fld(5, 24) //[28:24]

#define DDRPHY_SHU3_B0_DQ2                          (DDRPHY_AO_BASE_ADDRESS + 0x01608)
    #define SHU3_B0_DQ2_RG_TX_ARDQS0_DRVP_B0                   Fld(5, 0) //[4:0]
    #define SHU3_B0_DQ2_RG_TX_ARDQS0_DRVN_B0                   Fld(5, 8) //[12:8]
    #define SHU3_B0_DQ2_RG_TX_ARDQS0_ODTP_B0                   Fld(5, 16) //[20:16]
    #define SHU3_B0_DQ2_RG_TX_ARDQS0_ODTN_B0                   Fld(5, 24) //[28:24]

#define DDRPHY_SHU3_B0_DQ3                          (DDRPHY_AO_BASE_ADDRESS + 0x0160C)
    #define SHU3_B0_DQ3_RG_TX_ARDQS0_PU_B0                     Fld(2, 0) //[1:0]
    #define SHU3_B0_DQ3_RG_TX_ARDQS0_PU_PRE_B0                 Fld(2, 2) //[3:2]
    #define SHU3_B0_DQ3_RG_TX_ARDQS0_PDB_B0                    Fld(2, 4) //[5:4]
    #define SHU3_B0_DQ3_RG_TX_ARDQS0_PDB_PRE_B0                Fld(2, 6) //[7:6]
    #define SHU3_B0_DQ3_RG_TX_ARDQ_PU_B0                       Fld(2, 8) //[9:8]
    #define SHU3_B0_DQ3_RG_TX_ARDQ_PU_PRE_B0                   Fld(2, 10) //[11:10]
    #define SHU3_B0_DQ3_RG_TX_ARDQ_PDB_B0                      Fld(2, 12) //[13:12]
    #define SHU3_B0_DQ3_RG_TX_ARDQ_PDB_PRE_B0                  Fld(2, 14) //[15:14]

#define DDRPHY_SHU3_B0_DQ4                          (DDRPHY_AO_BASE_ADDRESS + 0x01610)
    #define SHU3_B0_DQ4_RG_ARPI_AA_MCK_DL_B0                   Fld(6, 0) //[5:0]
    #define SHU3_B0_DQ4_RG_ARPI_AA_MCK_FB_DL_B0                Fld(6, 8) //[13:8]
    #define SHU3_B0_DQ4_RG_ARPI_DA_MCK_FB_DL_B0                Fld(6, 16) //[21:16]

#define DDRPHY_SHU3_B0_DQ5                          (DDRPHY_AO_BASE_ADDRESS + 0x01614)
    #define SHU3_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0                 Fld(6, 0) //[5:0]
    #define SHU3_B0_DQ5_RG_RX_ARDQ_VREF_BYPASS_B0              Fld(1, 6) //[6:6]
    #define SHU3_B0_DQ5_RG_ARPI_FB_B0                          Fld(6, 8) //[13:8]
    #define SHU3_B0_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B0             Fld(3, 16) //[18:16]
    #define SHU3_B0_DQ5_DA_RX_ARDQS_DQSIEN_RB_DLY_B0           Fld(1, 19) //[19:19]
    #define SHU3_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0                Fld(3, 20) //[22:20]
    #define SHU3_B0_DQ5_RG_ARPI_MCTL_B0                        Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_B0_DQ6                          (DDRPHY_AO_BASE_ADDRESS + 0x01618)
    #define SHU3_B0_DQ6_RG_ARPI_OFFSET_DQSIEN_B0               Fld(6, 0) //[5:0]
    #define SHU3_B0_DQ6_RG_ARPI_RESERVE_B0                     Fld(16, 6) //[21:6]
    #define SHU3_B0_DQ6_RG_ARPI_MIDPI_CAP_SEL_B0               Fld(2, 22) //[23:22]
    #define SHU3_B0_DQ6_RG_ARPI_MIDPI_VTH_SEL_B0               Fld(2, 24) //[25:24]
    #define SHU3_B0_DQ6_RG_ARPI_MIDPI_EN_B0                    Fld(1, 26) //[26:26]
    #define SHU3_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0             Fld(1, 27) //[27:27]
    #define SHU3_B0_DQ6_RG_ARPI_CAP_SEL_B0                     Fld(2, 28) //[29:28]
    #define SHU3_B0_DQ6_RG_ARPI_MIDPI_BYPASS_EN_B0             Fld(1, 31) //[31:31]

#define DDRPHY_SHU3_B0_DQ7                          (DDRPHY_AO_BASE_ADDRESS + 0x0161C)
    #define SHU3_B0_DQ7_R_DMRANKRXDVS_B0                       Fld(4, 0) //[3:0]
    #define SHU3_B0_DQ7_MIDPI_ENABLE                           Fld(1, 4) //[4:4]
    #define SHU3_B0_DQ7_MIDPI_DIV4_ENABLE                      Fld(1, 5) //[5:5]
    #define SHU3_B0_DQ7_R_DMDQMDBI_EYE_SHU_B0                  Fld(1, 6) //[6:6]
    #define SHU3_B0_DQ7_R_DMDQMDBI_SHU_B0                      Fld(1, 7) //[7:7]
    #define SHU3_B0_DQ7_R_DMRXDVS_DQM_FLAGSEL_B0               Fld(4, 8) //[11:8]
    #define SHU3_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0            Fld(1, 12) //[12:12]
    #define SHU3_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0              Fld(1, 13) //[13:13]
    #define SHU3_B0_DQ7_R_DMRXTRACK_DQM_EN_B0                  Fld(1, 14) //[14:14]
    #define SHU3_B0_DQ7_R_DMRODTEN_B0                          Fld(1, 15) //[15:15]
    #define SHU3_B0_DQ7_R_DMARPI_CG_FB2DLL_DCM_EN_B0           Fld(1, 16) //[16:16]
    #define SHU3_B0_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B0               Fld(1, 17) //[17:17]
    #define SHU3_B0_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B0              Fld(1, 18) //[18:18]
    #define SHU3_B0_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B0              Fld(1, 19) //[19:19]
    #define SHU3_B0_DQ7_R_LP4Y_SDN_MODE_DQS0                   Fld(1, 20) //[20:20]
    #define SHU3_B0_DQ7_R_DMRXRANK_DQ_EN_B0                    Fld(1, 24) //[24:24]
    #define SHU3_B0_DQ7_R_DMRXRANK_DQ_LAT_B0                   Fld(3, 25) //[27:25]
    #define SHU3_B0_DQ7_R_DMRXRANK_DQS_EN_B0                   Fld(1, 28) //[28:28]
    #define SHU3_B0_DQ7_R_DMRXRANK_DQS_LAT_B0                  Fld(3, 29) //[31:29]

#define DDRPHY_SHU3_B0_DQ8                          (DDRPHY_AO_BASE_ADDRESS + 0x01620)
    #define SHU3_B0_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B0             Fld(15, 0) //[14:0]
    #define SHU3_B0_DQ8_R_DMRXDVS_UPD_FORCE_EN_B0              Fld(1, 15) //[15:15]
    #define SHU3_B0_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B0           Fld(1, 19) //[19:19]
    #define SHU3_B0_DQ8_R_RMRODTEN_CG_IG_B0                    Fld(1, 20) //[20:20]
    #define SHU3_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0                  Fld(1, 21) //[21:21]
    #define SHU3_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0          Fld(1, 22) //[22:22]
    #define SHU3_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0      Fld(1, 23) //[23:23]
    #define SHU3_B0_DQ8_R_DMRXDLY_CG_IG_B0                     Fld(1, 24) //[24:24]
    #define SHU3_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0                Fld(1, 25) //[25:25]
    #define SHU3_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0          Fld(1, 26) //[26:26]
    #define SHU3_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0          Fld(1, 27) //[27:27]
    #define SHU3_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0         Fld(1, 28) //[28:28]
    #define SHU3_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0     Fld(1, 29) //[29:29]
    #define SHU3_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0                 Fld(1, 30) //[30:30]
    #define SHU3_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0             Fld(1, 31) //[31:31]

#define DDRPHY_SHU3_B0_DQ9                          (DDRPHY_AO_BASE_ADDRESS + 0x01624)
    #define SHU3_B0_DQ9_RESERVED_0X1624                        Fld(32, 0) //[31:0]

#define DDRPHY_SHU3_B0_DQ10                         (DDRPHY_AO_BASE_ADDRESS + 0x01628)
    #define SHU3_B0_DQ10_RESERVED_0X1628                       Fld(32, 0) //[31:0]

#define DDRPHY_SHU3_B0_DQ11                         (DDRPHY_AO_BASE_ADDRESS + 0x0162C)
    #define SHU3_B0_DQ11_RESERVED_0X162C                       Fld(32, 0) //[31:0]

#define DDRPHY_SHU3_B0_DQ12                         (DDRPHY_AO_BASE_ADDRESS + 0x01630)
    #define SHU3_B0_DQ12_RESERVED_0X1630                       Fld(32, 0) //[31:0]

#define DDRPHY_SHU3_B0_DLL0                         (DDRPHY_AO_BASE_ADDRESS + 0x01634)
    #define SHU3_B0_DLL0_RG_ARPISM_MCK_SEL_B0_SHU              Fld(1, 0) //[0:0]
    #define SHU3_B0_DLL0_B0_DLL0_RFU                           Fld(1, 3) //[3:3]
    #define SHU3_B0_DLL0_RG_ARDLL_FAST_PSJP_B0                 Fld(1, 4) //[4:4]
    #define SHU3_B0_DLL0_RG_ARDLL_PHDIV_B0                     Fld(1, 9) //[9:9]
    #define SHU3_B0_DLL0_RG_ARDLL_PHJUMP_EN_B0                 Fld(1, 10) //[10:10]
    #define SHU3_B0_DLL0_RG_ARDLL_P_GAIN_B0                    Fld(4, 12) //[15:12]
    #define SHU3_B0_DLL0_RG_ARDLL_IDLECNT_B0                   Fld(4, 16) //[19:16]
    #define SHU3_B0_DLL0_RG_ARDLL_GAIN_B0                      Fld(4, 20) //[23:20]
    #define SHU3_B0_DLL0_RG_ARDLL_PHDET_IN_SWAP_B0             Fld(1, 30) //[30:30]
    #define SHU3_B0_DLL0_RG_ARDLL_PHDET_OUT_SEL_B0             Fld(1, 31) //[31:31]

#define DDRPHY_SHU3_B0_DLL1                         (DDRPHY_AO_BASE_ADDRESS + 0x01638)
    #define SHU3_B0_DLL1_RG_ARDLL_FASTPJ_CK_SEL_B0             Fld(1, 0) //[0:0]
    #define SHU3_B0_DLL1_RG_ARDLL_PS_EN_B0                     Fld(1, 1) //[1:1]
    #define SHU3_B0_DLL1_RG_ARDLL_PD_CK_SEL_B0                 Fld(1, 2) //[2:2]
    #define SHU3_B0_DLL1_RG_ARDQ_REV_B0                        Fld(24, 8) //[31:8]

#define DDRPHY_SHU3_B1_DQ0                          (DDRPHY_AO_BASE_ADDRESS + 0x01680)
    #define SHU3_B1_DQ0_RG_TX_ARDQS0_PRE_EN_B1                 Fld(1, 4) //[4:4]
    #define SHU3_B1_DQ0_RG_TX_ARDQS0_DRVP_PRE_B1               Fld(3, 8) //[10:8]
    #define SHU3_B1_DQ0_RG_TX_ARDQS0_DRVN_PRE_B1               Fld(3, 12) //[14:12]
    #define SHU3_B1_DQ0_RG_TX_ARDQ_PRE_EN_B1                   Fld(1, 20) //[20:20]
    #define SHU3_B1_DQ0_RG_TX_ARDQ_DRVP_PRE_B1                 Fld(3, 24) //[26:24]
    #define SHU3_B1_DQ0_RG_TX_ARDQ_DRVN_PRE_B1                 Fld(3, 28) //[30:28]
    #define SHU3_B1_DQ0_R_LP4Y_WDN_MODE_DQS1                   Fld(1, 31) //[31:31]

#define DDRPHY_SHU3_B1_DQ1                          (DDRPHY_AO_BASE_ADDRESS + 0x01684)
    #define SHU3_B1_DQ1_RG_TX_ARDQ_DRVP_B1                     Fld(5, 0) //[4:0]
    #define SHU3_B1_DQ1_RG_TX_ARDQ_DRVN_B1                     Fld(5, 8) //[12:8]
    #define SHU3_B1_DQ1_RG_TX_ARDQ_ODTP_B1                     Fld(5, 16) //[20:16]
    #define SHU3_B1_DQ1_RG_TX_ARDQ_ODTN_B1                     Fld(5, 24) //[28:24]

#define DDRPHY_SHU3_B1_DQ2                          (DDRPHY_AO_BASE_ADDRESS + 0x01688)
    #define SHU3_B1_DQ2_RG_TX_ARDQS0_DRVP_B1                   Fld(5, 0) //[4:0]
    #define SHU3_B1_DQ2_RG_TX_ARDQS0_DRVN_B1                   Fld(5, 8) //[12:8]
    #define SHU3_B1_DQ2_RG_TX_ARDQS0_ODTP_B1                   Fld(5, 16) //[20:16]
    #define SHU3_B1_DQ2_RG_TX_ARDQS0_ODTN_B1                   Fld(5, 24) //[28:24]

#define DDRPHY_SHU3_B1_DQ3                          (DDRPHY_AO_BASE_ADDRESS + 0x0168C)
    #define SHU3_B1_DQ3_RG_TX_ARDQS0_PU_B1                     Fld(2, 0) //[1:0]
    #define SHU3_B1_DQ3_RG_TX_ARDQS0_PU_PRE_B1                 Fld(2, 2) //[3:2]
    #define SHU3_B1_DQ3_RG_TX_ARDQS0_PDB_B1                    Fld(2, 4) //[5:4]
    #define SHU3_B1_DQ3_RG_TX_ARDQS0_PDB_PRE_B1                Fld(2, 6) //[7:6]
    #define SHU3_B1_DQ3_RG_TX_ARDQ_PU_B1                       Fld(2, 8) //[9:8]
    #define SHU3_B1_DQ3_RG_TX_ARDQ_PU_PRE_B1                   Fld(2, 10) //[11:10]
    #define SHU3_B1_DQ3_RG_TX_ARDQ_PDB_B1                      Fld(2, 12) //[13:12]
    #define SHU3_B1_DQ3_RG_TX_ARDQ_PDB_PRE_B1                  Fld(2, 14) //[15:14]

#define DDRPHY_SHU3_B1_DQ4                          (DDRPHY_AO_BASE_ADDRESS + 0x01690)
    #define SHU3_B1_DQ4_RG_ARPI_AA_MCK_DL_B1                   Fld(6, 0) //[5:0]
    #define SHU3_B1_DQ4_RG_ARPI_AA_MCK_FB_DL_B1                Fld(6, 8) //[13:8]
    #define SHU3_B1_DQ4_RG_ARPI_DA_MCK_FB_DL_B1                Fld(6, 16) //[21:16]

#define DDRPHY_SHU3_B1_DQ5                          (DDRPHY_AO_BASE_ADDRESS + 0x01694)
    #define SHU3_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1                 Fld(6, 0) //[5:0]
    #define SHU3_B1_DQ5_RG_RX_ARDQ_VREF_BYPASS_B1              Fld(1, 6) //[6:6]
    #define SHU3_B1_DQ5_RG_ARPI_FB_B1                          Fld(6, 8) //[13:8]
    #define SHU3_B1_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B1             Fld(3, 16) //[18:16]
    #define SHU3_B1_DQ5_DA_RX_ARDQS_DQSIEN_RB_DLY_B1           Fld(1, 19) //[19:19]
    #define SHU3_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1                Fld(3, 20) //[22:20]
    #define SHU3_B1_DQ5_RG_ARPI_MCTL_B1                        Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_B1_DQ6                          (DDRPHY_AO_BASE_ADDRESS + 0x01698)
    #define SHU3_B1_DQ6_RG_ARPI_OFFSET_DQSIEN_B1               Fld(6, 0) //[5:0]
    #define SHU3_B1_DQ6_RG_ARPI_RESERVE_B1                     Fld(16, 6) //[21:6]
    #define SHU3_B1_DQ6_RG_ARPI_MIDPI_CAP_SEL_B1               Fld(2, 22) //[23:22]
    #define SHU3_B1_DQ6_RG_ARPI_MIDPI_VTH_SEL_B1               Fld(2, 24) //[25:24]
    #define SHU3_B1_DQ6_RG_ARPI_MIDPI_EN_B1                    Fld(1, 26) //[26:26]
    #define SHU3_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1             Fld(1, 27) //[27:27]
    #define SHU3_B1_DQ6_RG_ARPI_CAP_SEL_B1                     Fld(2, 28) //[29:28]
    #define SHU3_B1_DQ6_RG_ARPI_MIDPI_BYPASS_EN_B1             Fld(1, 31) //[31:31]

#define DDRPHY_SHU3_B1_DQ7                          (DDRPHY_AO_BASE_ADDRESS + 0x0169C)
    #define SHU3_B1_DQ7_R_DMRANKRXDVS_B1                       Fld(4, 0) //[3:0]
    #define SHU3_B1_DQ7_R_DMDQMDBI_EYE_SHU_B1                  Fld(1, 6) //[6:6]
    #define SHU3_B1_DQ7_R_DMDQMDBI_SHU_B1                      Fld(1, 7) //[7:7]
    #define SHU3_B1_DQ7_R_DMRXDVS_DQM_FLAGSEL_B1               Fld(4, 8) //[11:8]
    #define SHU3_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1            Fld(1, 12) //[12:12]
    #define SHU3_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1              Fld(1, 13) //[13:13]
    #define SHU3_B1_DQ7_R_DMRXTRACK_DQM_EN_B1                  Fld(1, 14) //[14:14]
    #define SHU3_B1_DQ7_R_DMRODTEN_B1                          Fld(1, 15) //[15:15]
    #define SHU3_B1_DQ7_R_DMARPI_CG_FB2DLL_DCM_EN_B1           Fld(1, 16) //[16:16]
    #define SHU3_B1_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B1               Fld(1, 17) //[17:17]
    #define SHU3_B1_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B1              Fld(1, 18) //[18:18]
    #define SHU3_B1_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B1              Fld(1, 19) //[19:19]
    #define SHU3_B1_DQ7_R_LP4Y_SDN_MODE_DQS1                   Fld(1, 20) //[20:20]
    #define SHU3_B1_DQ7_R_DMRXRANK_DQ_EN_B1                    Fld(1, 24) //[24:24]
    #define SHU3_B1_DQ7_R_DMRXRANK_DQ_LAT_B1                   Fld(3, 25) //[27:25]
    #define SHU3_B1_DQ7_R_DMRXRANK_DQS_EN_B1                   Fld(1, 28) //[28:28]
    #define SHU3_B1_DQ7_R_DMRXRANK_DQS_LAT_B1                  Fld(3, 29) //[31:29]

#define DDRPHY_SHU3_B1_DQ8                          (DDRPHY_AO_BASE_ADDRESS + 0x016A0)
    #define SHU3_B1_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B1             Fld(15, 0) //[14:0]
    #define SHU3_B1_DQ8_R_DMRXDVS_UPD_FORCE_EN_B1              Fld(1, 15) //[15:15]
    #define SHU3_B1_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B1           Fld(1, 19) //[19:19]
    #define SHU3_B1_DQ8_R_RMRODTEN_CG_IG_B1                    Fld(1, 20) //[20:20]
    #define SHU3_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1                  Fld(1, 21) //[21:21]
    #define SHU3_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1          Fld(1, 22) //[22:22]
    #define SHU3_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1      Fld(1, 23) //[23:23]
    #define SHU3_B1_DQ8_R_DMRXDLY_CG_IG_B1                     Fld(1, 24) //[24:24]
    #define SHU3_B1_DQ8_R_DMSTBEN_SYNC_CG_IG_B1                Fld(1, 25) //[25:25]
    #define SHU3_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1          Fld(1, 26) //[26:26]
    #define SHU3_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1          Fld(1, 27) //[27:27]
    #define SHU3_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1         Fld(1, 28) //[28:28]
    #define SHU3_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1     Fld(1, 29) //[29:29]
    #define SHU3_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1                 Fld(1, 30) //[30:30]
    #define SHU3_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1             Fld(1, 31) //[31:31]

#define DDRPHY_SHU3_B1_DQ9                          (DDRPHY_AO_BASE_ADDRESS + 0x016A4)
    #define SHU3_B1_DQ9_RESERVED_0X16A4                        Fld(32, 0) //[31:0]

#define DDRPHY_SHU3_B1_DQ10                         (DDRPHY_AO_BASE_ADDRESS + 0x016A8)
    #define SHU3_B1_DQ10_RESERVED_0X16A8                       Fld(32, 0) //[31:0]

#define DDRPHY_SHU3_B1_DQ11                         (DDRPHY_AO_BASE_ADDRESS + 0x016AC)
    #define SHU3_B1_DQ11_RESERVED_0X16AC                       Fld(32, 0) //[31:0]

#define DDRPHY_SHU3_B1_DQ12                         (DDRPHY_AO_BASE_ADDRESS + 0x016B0)
    #define SHU3_B1_DQ12_RESERVED_0X16B0                       Fld(32, 0) //[31:0]

#define DDRPHY_SHU3_B1_DLL0                         (DDRPHY_AO_BASE_ADDRESS + 0x016B4)
    #define SHU3_B1_DLL0_RG_ARPISM_MCK_SEL_B1_SHU              Fld(1, 0) //[0:0]
    #define SHU3_B1_DLL0_B1_DLL0_RFU                           Fld(1, 3) //[3:3]
    #define SHU3_B1_DLL0_RG_ARDLL_FAST_PSJP_B1                 Fld(1, 4) //[4:4]
    #define SHU3_B1_DLL0_RG_ARDLL_PHDIV_B1                     Fld(1, 9) //[9:9]
    #define SHU3_B1_DLL0_RG_ARDLL_PHJUMP_EN_B1                 Fld(1, 10) //[10:10]
    #define SHU3_B1_DLL0_RG_ARDLL_P_GAIN_B1                    Fld(4, 12) //[15:12]
    #define SHU3_B1_DLL0_RG_ARDLL_IDLECNT_B1                   Fld(4, 16) //[19:16]
    #define SHU3_B1_DLL0_RG_ARDLL_GAIN_B1                      Fld(4, 20) //[23:20]
    #define SHU3_B1_DLL0_RG_ARDLL_PHDET_IN_SWAP_B1             Fld(1, 30) //[30:30]
    #define SHU3_B1_DLL0_RG_ARDLL_PHDET_OUT_SEL_B1             Fld(1, 31) //[31:31]

#define DDRPHY_SHU3_B1_DLL1                         (DDRPHY_AO_BASE_ADDRESS + 0x016B8)
    #define SHU3_B1_DLL1_RG_ARDLL_FASTPJ_CK_SEL_B1             Fld(1, 0) //[0:0]
    #define SHU3_B1_DLL1_RG_ARDLL_PS_EN_B1                     Fld(1, 1) //[1:1]
    #define SHU3_B1_DLL1_RG_ARDLL_PD_CK_SEL_B1                 Fld(1, 2) //[2:2]
    #define SHU3_B1_DLL1_RG_ARDQ_REV_B1                        Fld(24, 8) //[31:8]

#define DDRPHY_SHU3_CA_CMD0                         (DDRPHY_AO_BASE_ADDRESS + 0x01700)
    #define SHU3_CA_CMD0_RG_TX_ARCLK_PRE_EN                    Fld(1, 4) //[4:4]
    #define SHU3_CA_CMD0_RG_TX_ARCLK_DRVP_PRE                  Fld(3, 8) //[10:8]
    #define SHU3_CA_CMD0_RG_TX_ARCLK_DRVN_PRE                  Fld(3, 12) //[14:12]
    #define SHU3_CA_CMD0_RG_CGEN_FMEM_CK_CG_DLL                Fld(1, 17) //[17:17]
    #define SHU3_CA_CMD0_RG_FB_CK_MUX                          Fld(2, 18) //[19:18]
    #define SHU3_CA_CMD0_RG_TX_ARCMD_PRE_EN                    Fld(1, 20) //[20:20]
    #define SHU3_CA_CMD0_RG_TX_ARCMD_DRVP_PRE                  Fld(3, 24) //[26:24]
    #define SHU3_CA_CMD0_RG_TX_ARCMD_DRVN_PRE                  Fld(3, 28) //[30:28]
    #define SHU3_CA_CMD0_R_LP4Y_WDN_MODE_CLK                   Fld(1, 31) //[31:31]

#define DDRPHY_SHU3_CA_CMD1                         (DDRPHY_AO_BASE_ADDRESS + 0x01704)
    #define SHU3_CA_CMD1_RG_TX_ARCMD_DRVP                      Fld(5, 0) //[4:0]
    #define SHU3_CA_CMD1_RG_TX_ARCMD_DRVN                      Fld(5, 8) //[12:8]
    #define SHU3_CA_CMD1_RG_TX_ARCMD_ODTP                      Fld(5, 16) //[20:16]
    #define SHU3_CA_CMD1_RG_TX_ARCMD_ODTN                      Fld(5, 24) //[28:24]

#define DDRPHY_SHU3_CA_CMD2                         (DDRPHY_AO_BASE_ADDRESS + 0x01708)
    #define SHU3_CA_CMD2_RG_TX_ARCLK_DRVP                      Fld(5, 0) //[4:0]
    #define SHU3_CA_CMD2_RG_TX_ARCLK_DRVN                      Fld(5, 8) //[12:8]
    #define SHU3_CA_CMD2_RG_TX_ARCLK_ODTP                      Fld(5, 16) //[20:16]
    #define SHU3_CA_CMD2_RG_TX_ARCLK_ODTN                      Fld(5, 24) //[28:24]

#define DDRPHY_SHU3_CA_CMD3                         (DDRPHY_AO_BASE_ADDRESS + 0x0170C)
    #define SHU3_CA_CMD3_RG_TX_ARCLK_PU                        Fld(2, 0) //[1:0]
    #define SHU3_CA_CMD3_RG_TX_ARCLK_PU_PRE                    Fld(2, 2) //[3:2]
    #define SHU3_CA_CMD3_RG_TX_ARCLK_PDB                       Fld(2, 4) //[5:4]
    #define SHU3_CA_CMD3_RG_TX_ARCLK_PDB_PRE                   Fld(2, 6) //[7:6]
    #define SHU3_CA_CMD3_RG_TX_ARCMD_PU                        Fld(2, 8) //[9:8]
    #define SHU3_CA_CMD3_RG_TX_ARCMD_PU_PRE                    Fld(2, 10) //[11:10]
    #define SHU3_CA_CMD3_RG_TX_ARCMD_PDB                       Fld(2, 12) //[13:12]
    #define SHU3_CA_CMD3_RG_TX_ARCMD_PDB_PRE                   Fld(2, 14) //[15:14]

#define DDRPHY_SHU3_CA_CMD4                         (DDRPHY_AO_BASE_ADDRESS + 0x01710)
    #define SHU3_CA_CMD4_RG_ARPI_AA_MCK_DL_CA                  Fld(6, 0) //[5:0]
    #define SHU3_CA_CMD4_RG_ARPI_AA_MCK_FB_DL_CA               Fld(6, 8) //[13:8]
    #define SHU3_CA_CMD4_RG_ARPI_DA_MCK_FB_DL_CA               Fld(6, 16) //[21:16]

#define DDRPHY_SHU3_CA_CMD5                         (DDRPHY_AO_BASE_ADDRESS + 0x01714)
    #define SHU3_CA_CMD5_RG_RX_ARCMD_VREF_SEL                  Fld(6, 0) //[5:0]
    #define SHU3_CA_CMD5_RG_RX_ARCMD_VREF_BYPASS               Fld(1, 6) //[6:6]
    #define SHU3_CA_CMD5_RG_ARPI_FB_CA                         Fld(6, 8) //[13:8]
    #define SHU3_CA_CMD5_RG_RX_ARCLK_DQSIEN_DLY                Fld(3, 16) //[18:16]
    #define SHU3_CA_CMD5_DA_RX_ARCLK_DQSIEN_RB_DLY             Fld(1, 19) //[19:19]
    #define SHU3_CA_CMD5_RG_RX_ARCLK_DVS_DLY                   Fld(3, 20) //[22:20]
    #define SHU3_CA_CMD5_RG_ARPI_MCTL_CA                       Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_CA_CMD6                         (DDRPHY_AO_BASE_ADDRESS + 0x01718)
    #define SHU3_CA_CMD6_RG_ARPI_OFFSET_CLKIEN                 Fld(6, 0) //[5:0]
    #define SHU3_CA_CMD6_RG_ARPI_RESERVE_CA                    Fld(16, 6) //[21:6]
    #define SHU3_CA_CMD6_RG_ARPI_MIDPI_CAP_SEL_CA              Fld(2, 22) //[23:22]
    #define SHU3_CA_CMD6_RG_ARPI_MIDPI_VTH_SEL_CA              Fld(2, 24) //[25:24]
    #define SHU3_CA_CMD6_RG_ARPI_MIDPI_EN_CA                   Fld(1, 26) //[26:26]
    #define SHU3_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA            Fld(1, 27) //[27:27]
    #define SHU3_CA_CMD6_RG_ARPI_CAP_SEL_CA                    Fld(2, 28) //[29:28]
    #define SHU3_CA_CMD6_RG_ARPI_MIDPI_BYPASS_EN_CA            Fld(1, 31) //[31:31]

#define DDRPHY_SHU3_CA_CMD7                         (DDRPHY_AO_BASE_ADDRESS + 0x0171C)
    #define SHU3_CA_CMD7_R_DMRANKRXDVS_CA                      Fld(4, 0) //[3:0]
    #define SHU3_CA_CMD7_R_DMRXDVS_PBYTE_FLAG_OPT_CA           Fld(1, 12) //[12:12]
    #define SHU3_CA_CMD7_R_DMRODTEN_CA                         Fld(1, 15) //[15:15]
    #define SHU3_CA_CMD7_R_DMARPI_CG_FB2DLL_DCM_EN_CA          Fld(1, 16) //[16:16]
    #define SHU3_CA_CMD7_R_DMTX_ARPI_CG_CMD_NEW                Fld(1, 17) //[17:17]
    #define SHU3_CA_CMD7_R_DMTX_ARPI_CG_CLK_NEW                Fld(1, 18) //[18:18]
    #define SHU3_CA_CMD7_R_DMTX_ARPI_CG_CS_NEW                 Fld(1, 19) //[19:19]
    #define SHU3_CA_CMD7_R_LP4Y_SDN_MODE_CLK                   Fld(1, 20) //[20:20]
    #define SHU3_CA_CMD7_R_DMRXRANK_CMD_EN                     Fld(1, 24) //[24:24]
    #define SHU3_CA_CMD7_R_DMRXRANK_CMD_LAT                    Fld(3, 25) //[27:25]
    #define SHU3_CA_CMD7_R_DMRXRANK_CLK_EN                     Fld(1, 28) //[28:28]
    #define SHU3_CA_CMD7_R_DMRXRANK_CLK_LAT                    Fld(3, 29) //[31:29]

#define DDRPHY_SHU3_CA_CMD8                         (DDRPHY_AO_BASE_ADDRESS + 0x01720)
    #define SHU3_CA_CMD8_R_DMRXDVS_UPD_FORCE_CYC_CA            Fld(15, 0) //[14:0]
    #define SHU3_CA_CMD8_R_DMRXDVS_UPD_FORCE_EN_CA             Fld(1, 15) //[15:15]
    #define SHU3_CA_CMD8_R_DMRANK_RXDLY_PIPE_CG_IG_CA          Fld(1, 19) //[19:19]
    #define SHU3_CA_CMD8_R_RMRODTEN_CG_IG_CA                   Fld(1, 20) //[20:20]
    #define SHU3_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA                 Fld(1, 21) //[21:21]
    #define SHU3_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA         Fld(1, 22) //[22:22]
    #define SHU3_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA     Fld(1, 23) //[23:23]
    #define SHU3_CA_CMD8_R_DMRXDLY_CG_IG_CA                    Fld(1, 24) //[24:24]
    #define SHU3_CA_CMD8_R_DMSTBEN_SYNC_CG_IG_CA               Fld(1, 25) //[25:25]
    #define SHU3_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA         Fld(1, 26) //[26:26]
    #define SHU3_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA         Fld(1, 27) //[27:27]
    #define SHU3_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA        Fld(1, 28) //[28:28]
    #define SHU3_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA    Fld(1, 29) //[29:29]
    #define SHU3_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA                Fld(1, 30) //[30:30]
    #define SHU3_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA            Fld(1, 31) //[31:31]

#define DDRPHY_SHU3_CA_CMD9                         (DDRPHY_AO_BASE_ADDRESS + 0x01724)
    #define SHU3_CA_CMD9_RESERVED_0X1724                       Fld(32, 0) //[31:0]

#define DDRPHY_SHU3_CA_CMD10                        (DDRPHY_AO_BASE_ADDRESS + 0x01728)
    #define SHU3_CA_CMD10_RESERVED_0X1728                      Fld(32, 0) //[31:0]

#define DDRPHY_SHU3_CA_CMD11                        (DDRPHY_AO_BASE_ADDRESS + 0x0172C)
    #define SHU3_CA_CMD11_RG_RIMP_REV                          Fld(8, 0) //[7:0]
    #define SHU3_CA_CMD11_RG_RIMP_VREF_SEL                     Fld(6, 8) //[13:8]
    #define SHU3_CA_CMD11_RG_TX_ARCKE_DRVP                     Fld(5, 17) //[21:17]
    #define SHU3_CA_CMD11_RG_TX_ARCKE_DRVN                     Fld(5, 22) //[26:22]

#define DDRPHY_SHU3_CA_CMD12                        (DDRPHY_AO_BASE_ADDRESS + 0x01730)
    #define SHU3_CA_CMD12_RESERVED_0X1730                      Fld(32, 0) //[31:0]

#define DDRPHY_SHU3_CA_DLL0                         (DDRPHY_AO_BASE_ADDRESS + 0x01734)
    #define SHU3_CA_DLL0_RG_ARPISM_MCK_SEL_CA_SHU              Fld(1, 0) //[0:0]
    #define SHU3_CA_DLL0_CA_DLL0_RFU                           Fld(1, 3) //[3:3]
    #define SHU3_CA_DLL0_RG_ARDLL_FAST_PSJP_CA                 Fld(1, 4) //[4:4]
    #define SHU3_CA_DLL0_RG_ARDLL_PHDIV_CA                     Fld(1, 9) //[9:9]
    #define SHU3_CA_DLL0_RG_ARDLL_PHJUMP_EN_CA                 Fld(1, 10) //[10:10]
    #define SHU3_CA_DLL0_RG_ARDLL_P_GAIN_CA                    Fld(4, 12) //[15:12]
    #define SHU3_CA_DLL0_RG_ARDLL_IDLECNT_CA                   Fld(4, 16) //[19:16]
    #define SHU3_CA_DLL0_RG_ARDLL_GAIN_CA                      Fld(4, 20) //[23:20]
    #define SHU3_CA_DLL0_RG_ARDLL_PHDET_IN_SWAP_CA             Fld(1, 30) //[30:30]
    #define SHU3_CA_DLL0_RG_ARDLL_PHDET_OUT_SEL_CA             Fld(1, 31) //[31:31]

#define DDRPHY_SHU3_CA_DLL1                         (DDRPHY_AO_BASE_ADDRESS + 0x01738)
    #define SHU3_CA_DLL1_RG_ARDLL_FASTPJ_CK_SEL_CA             Fld(1, 0) //[0:0]
    #define SHU3_CA_DLL1_RG_ARDLL_PS_EN_CA                     Fld(1, 1) //[1:1]
    #define SHU3_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA                 Fld(1, 2) //[2:2]
    #define SHU3_CA_DLL1_RG_ARCMD_REV                          Fld(24, 8) //[31:8]

#define DDRPHY_SHU3_MISC0                           (DDRPHY_AO_BASE_ADDRESS + 0x017F0)
    #define SHU3_MISC0_R_RX_PIPE_BYPASS_EN                     Fld(1, 1) //[1:1]
    #define SHU3_MISC0_RG_CMD_TXPIPE_BYPASS_EN                 Fld(1, 2) //[2:2]
    #define SHU3_MISC0_RG_CK_TXPIPE_BYPASS_EN                  Fld(1, 3) //[3:3]
    #define SHU3_MISC0_RG_RVREF_SEL_DQ                         Fld(6, 16) //[21:16]
    #define SHU3_MISC0_RG_RVREF_DDR4_SEL                       Fld(1, 22) //[22:22]
    #define SHU3_MISC0_RG_RVREF_DDR3_SEL                       Fld(1, 23) //[23:23]
    #define SHU3_MISC0_RG_RVREF_SEL_CMD                        Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R0_B0_DQ0                       (DDRPHY_AO_BASE_ADDRESS + 0x01800)
    #define SHU3_R0_B0_DQ0_RK0_TX_ARDQ0_DLY_B0                 Fld(4, 0) //[3:0]
    #define SHU3_R0_B0_DQ0_RK0_TX_ARDQ1_DLY_B0                 Fld(4, 4) //[7:4]
    #define SHU3_R0_B0_DQ0_RK0_TX_ARDQ2_DLY_B0                 Fld(4, 8) //[11:8]
    #define SHU3_R0_B0_DQ0_RK0_TX_ARDQ3_DLY_B0                 Fld(4, 12) //[15:12]
    #define SHU3_R0_B0_DQ0_RK0_TX_ARDQ4_DLY_B0                 Fld(4, 16) //[19:16]
    #define SHU3_R0_B0_DQ0_RK0_TX_ARDQ5_DLY_B0                 Fld(4, 20) //[23:20]
    #define SHU3_R0_B0_DQ0_RK0_TX_ARDQ6_DLY_B0                 Fld(4, 24) //[27:24]
    #define SHU3_R0_B0_DQ0_RK0_TX_ARDQ7_DLY_B0                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU3_R0_B0_DQ1                       (DDRPHY_AO_BASE_ADDRESS + 0x01804)
    #define SHU3_R0_B0_DQ1_RK0_TX_ARDQM0_DLY_B0                Fld(4, 0) //[3:0]
    #define SHU3_R0_B0_DQ1_RK0_TX_ARDQS0_DLYB_B0               Fld(4, 16) //[19:16]
    #define SHU3_R0_B0_DQ1_RK0_TX_ARDQS0B_DLYB_B0              Fld(4, 20) //[23:20]
    #define SHU3_R0_B0_DQ1_RK0_TX_ARDQS0_DLY_B0                Fld(4, 24) //[27:24]
    #define SHU3_R0_B0_DQ1_RK0_TX_ARDQS0B_DLY_B0               Fld(4, 28) //[31:28]

#define DDRPHY_SHU3_R0_B0_DQ2                       (DDRPHY_AO_BASE_ADDRESS + 0x01808)
    #define SHU3_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU3_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU3_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU3_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R0_B0_DQ3                       (DDRPHY_AO_BASE_ADDRESS + 0x0180C)
    #define SHU3_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU3_R0_B0_DQ3_RK0_RX_ARDQ2_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU3_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU3_R0_B0_DQ3_RK0_RX_ARDQ3_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R0_B0_DQ4                       (DDRPHY_AO_BASE_ADDRESS + 0x01810)
    #define SHU3_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU3_R0_B0_DQ4_RK0_RX_ARDQ4_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU3_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU3_R0_B0_DQ4_RK0_RX_ARDQ5_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R0_B0_DQ5                       (DDRPHY_AO_BASE_ADDRESS + 0x01814)
    #define SHU3_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU3_R0_B0_DQ5_RK0_RX_ARDQ6_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU3_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU3_R0_B0_DQ5_RK0_RX_ARDQ7_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R0_B0_DQ6                       (DDRPHY_AO_BASE_ADDRESS + 0x01818)
    #define SHU3_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0              Fld(6, 0) //[5:0]
    #define SHU3_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0              Fld(6, 8) //[13:8]
    #define SHU3_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0              Fld(7, 16) //[22:16]
    #define SHU3_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0              Fld(7, 24) //[30:24]

#define DDRPHY_SHU3_R0_B0_DQ7                       (DDRPHY_AO_BASE_ADDRESS + 0x0181C)
    #define SHU3_R0_B0_DQ7_RK0_ARPI_DQ_B0                      Fld(6, 8) //[13:8]
    #define SHU3_R0_B0_DQ7_RK0_ARPI_DQM_B0                     Fld(6, 16) //[21:16]
    #define SHU3_R0_B0_DQ7_RK0_ARPI_PBYTE_B0                   Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X1820                           (DDRPHY_AO_BASE_ADDRESS + 0x01820)
    #define RFU_0X1820_RESERVED_0X1820                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1824                           (DDRPHY_AO_BASE_ADDRESS + 0x01824)
    #define RFU_0X1824_RESERVED_0X1824                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1828                           (DDRPHY_AO_BASE_ADDRESS + 0x01828)
    #define RFU_0X1828_RESERVED_0X1828                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X182C                           (DDRPHY_AO_BASE_ADDRESS + 0x0182C)
    #define RFU_0X182C_RESERVED_0X182C                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU3_R0_B1_DQ0                       (DDRPHY_AO_BASE_ADDRESS + 0x01850)
    #define SHU3_R0_B1_DQ0_RK0_TX_ARDQ0_DLY_B1                 Fld(4, 0) //[3:0]
    #define SHU3_R0_B1_DQ0_RK0_TX_ARDQ1_DLY_B1                 Fld(4, 4) //[7:4]
    #define SHU3_R0_B1_DQ0_RK0_TX_ARDQ2_DLY_B1                 Fld(4, 8) //[11:8]
    #define SHU3_R0_B1_DQ0_RK0_TX_ARDQ3_DLY_B1                 Fld(4, 12) //[15:12]
    #define SHU3_R0_B1_DQ0_RK0_TX_ARDQ4_DLY_B1                 Fld(4, 16) //[19:16]
    #define SHU3_R0_B1_DQ0_RK0_TX_ARDQ5_DLY_B1                 Fld(4, 20) //[23:20]
    #define SHU3_R0_B1_DQ0_RK0_TX_ARDQ6_DLY_B1                 Fld(4, 24) //[27:24]
    #define SHU3_R0_B1_DQ0_RK0_TX_ARDQ7_DLY_B1                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU3_R0_B1_DQ1                       (DDRPHY_AO_BASE_ADDRESS + 0x01854)
    #define SHU3_R0_B1_DQ1_RK0_TX_ARDQM0_DLY_B1                Fld(4, 0) //[3:0]
    #define SHU3_R0_B1_DQ1_RK0_TX_ARDQS0_DLYB_B1               Fld(4, 16) //[19:16]
    #define SHU3_R0_B1_DQ1_RK0_TX_ARDQS0B_DLYB_B1              Fld(4, 20) //[23:20]
    #define SHU3_R0_B1_DQ1_RK0_TX_ARDQS0_DLY_B1                Fld(4, 24) //[27:24]
    #define SHU3_R0_B1_DQ1_RK0_TX_ARDQS0B_DLY_B1               Fld(4, 28) //[31:28]

#define DDRPHY_SHU3_R0_B1_DQ2                       (DDRPHY_AO_BASE_ADDRESS + 0x01858)
    #define SHU3_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU3_R0_B1_DQ2_RK0_RX_ARDQ0_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU3_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU3_R0_B1_DQ2_RK0_RX_ARDQ1_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R0_B1_DQ3                       (DDRPHY_AO_BASE_ADDRESS + 0x0185C)
    #define SHU3_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU3_R0_B1_DQ3_RK0_RX_ARDQ2_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU3_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU3_R0_B1_DQ3_RK0_RX_ARDQ3_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R0_B1_DQ4                       (DDRPHY_AO_BASE_ADDRESS + 0x01860)
    #define SHU3_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU3_R0_B1_DQ4_RK0_RX_ARDQ4_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU3_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU3_R0_B1_DQ4_RK0_RX_ARDQ5_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R0_B1_DQ5                       (DDRPHY_AO_BASE_ADDRESS + 0x01864)
    #define SHU3_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU3_R0_B1_DQ5_RK0_RX_ARDQ6_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU3_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU3_R0_B1_DQ5_RK0_RX_ARDQ7_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R0_B1_DQ6                       (DDRPHY_AO_BASE_ADDRESS + 0x01868)
    #define SHU3_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1              Fld(6, 0) //[5:0]
    #define SHU3_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1              Fld(6, 8) //[13:8]
    #define SHU3_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1              Fld(7, 16) //[22:16]
    #define SHU3_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1              Fld(7, 24) //[30:24]

#define DDRPHY_SHU3_R0_B1_DQ7                       (DDRPHY_AO_BASE_ADDRESS + 0x0186C)
    #define SHU3_R0_B1_DQ7_RK0_ARPI_DQ_B1                      Fld(6, 8) //[13:8]
    #define SHU3_R0_B1_DQ7_RK0_ARPI_DQM_B1                     Fld(6, 16) //[21:16]
    #define SHU3_R0_B1_DQ7_RK0_ARPI_PBYTE_B1                   Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X1870                           (DDRPHY_AO_BASE_ADDRESS + 0x01870)
    #define RFU_0X1870_RESERVED_0X1870                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1874                           (DDRPHY_AO_BASE_ADDRESS + 0x01874)
    #define RFU_0X1874_RESERVED_0X1874                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1878                           (DDRPHY_AO_BASE_ADDRESS + 0x01878)
    #define RFU_0X1878_RESERVED_0X1878                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X187C                           (DDRPHY_AO_BASE_ADDRESS + 0x0187C)
    #define RFU_0X187C_RESERVED_0X187C                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU3_R0_CA_CMD0                      (DDRPHY_AO_BASE_ADDRESS + 0x018A0)
    #define SHU3_R0_CA_CMD0_RK0_TX_ARCA0_DLY                   Fld(4, 0) //[3:0]
    #define SHU3_R0_CA_CMD0_RK0_TX_ARCA1_DLY                   Fld(4, 4) //[7:4]
    #define SHU3_R0_CA_CMD0_RK0_TX_ARCA2_DLY                   Fld(4, 8) //[11:8]
    #define SHU3_R0_CA_CMD0_RK0_TX_ARCA3_DLY                   Fld(4, 12) //[15:12]
    #define SHU3_R0_CA_CMD0_RK0_TX_ARCA4_DLY                   Fld(4, 16) //[19:16]
    #define SHU3_R0_CA_CMD0_RK0_TX_ARCA5_DLY                   Fld(4, 20) //[23:20]
    #define SHU3_R0_CA_CMD0_RK0_TX_ARCLK_DLYB                  Fld(4, 24) //[27:24]
    #define SHU3_R0_CA_CMD0_RK0_TX_ARCLKB_DLYB                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU3_R0_CA_CMD1                      (DDRPHY_AO_BASE_ADDRESS + 0x018A4)
    #define SHU3_R0_CA_CMD1_RK0_TX_ARCKE0_DLY                  Fld(4, 0) //[3:0]
    #define SHU3_R0_CA_CMD1_RK0_TX_ARCKE1_DLY                  Fld(4, 4) //[7:4]
    #define SHU3_R0_CA_CMD1_RK0_TX_ARCKE2_DLY                  Fld(4, 8) //[11:8]
    #define SHU3_R0_CA_CMD1_RK0_TX_ARCS0_DLY                   Fld(4, 12) //[15:12]
    #define SHU3_R0_CA_CMD1_RK0_TX_ARCS1_DLY                   Fld(4, 16) //[19:16]
    #define SHU3_R0_CA_CMD1_RK0_TX_ARCS2_DLY                   Fld(4, 20) //[23:20]
    #define SHU3_R0_CA_CMD1_RK0_TX_ARCLK_DLY                   Fld(4, 24) //[27:24]
    #define SHU3_R0_CA_CMD1_RK0_TX_ARCLKB_DLY                  Fld(4, 28) //[31:28]

#define DDRPHY_SHU3_R0_CA_CMD2                      (DDRPHY_AO_BASE_ADDRESS + 0x018A8)
    #define SHU3_R0_CA_CMD2_RG_RK0_RX_ARCA0_R_DLY              Fld(6, 0) //[5:0]
    #define SHU3_R0_CA_CMD2_RG_RK0_RX_ARCA0_F_DLY              Fld(6, 8) //[13:8]
    #define SHU3_R0_CA_CMD2_RG_RK0_RX_ARCA1_R_DLY              Fld(6, 16) //[21:16]
    #define SHU3_R0_CA_CMD2_RG_RK0_RX_ARCA1_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R0_CA_CMD3                      (DDRPHY_AO_BASE_ADDRESS + 0x018AC)
    #define SHU3_R0_CA_CMD3_RG_RK0_RX_ARCA2_R_DLY              Fld(6, 0) //[5:0]
    #define SHU3_R0_CA_CMD3_RG_RK0_RX_ARCA2_F_DLY              Fld(6, 8) //[13:8]
    #define SHU3_R0_CA_CMD3_RG_RK0_RX_ARCA3_R_DLY              Fld(6, 16) //[21:16]
    #define SHU3_R0_CA_CMD3_RG_RK0_RX_ARCA3_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R0_CA_CMD4                      (DDRPHY_AO_BASE_ADDRESS + 0x018B0)
    #define SHU3_R0_CA_CMD4_RG_RK0_RX_ARCA4_R_DLY              Fld(6, 0) //[5:0]
    #define SHU3_R0_CA_CMD4_RG_RK0_RX_ARCA4_F_DLY              Fld(6, 8) //[13:8]
    #define SHU3_R0_CA_CMD4_RG_RK0_RX_ARCA5_R_DLY              Fld(6, 16) //[21:16]
    #define SHU3_R0_CA_CMD4_RG_RK0_RX_ARCA5_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R0_CA_CMD5                      (DDRPHY_AO_BASE_ADDRESS + 0x018B4)
    #define SHU3_R0_CA_CMD5_RG_RK0_RX_ARCKE0_R_DLY             Fld(6, 0) //[5:0]
    #define SHU3_R0_CA_CMD5_RG_RK0_RX_ARCKE0_F_DLY             Fld(6, 8) //[13:8]
    #define SHU3_R0_CA_CMD5_RG_RK0_RX_ARCKE1_R_DLY             Fld(6, 16) //[21:16]
    #define SHU3_R0_CA_CMD5_RG_RK0_RX_ARCKE1_F_DLY             Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R0_CA_CMD6                      (DDRPHY_AO_BASE_ADDRESS + 0x018B8)
    #define SHU3_R0_CA_CMD6_RG_RK0_RX_ARCKE2_R_DLY             Fld(6, 0) //[5:0]
    #define SHU3_R0_CA_CMD6_RG_RK0_RX_ARCKE2_F_DLY             Fld(6, 8) //[13:8]
    #define SHU3_R0_CA_CMD6_RG_RK0_RX_ARCS0_R_DLY              Fld(6, 16) //[21:16]
    #define SHU3_R0_CA_CMD6_RG_RK0_RX_ARCS0_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R0_CA_CMD7                      (DDRPHY_AO_BASE_ADDRESS + 0x018BC)
    #define SHU3_R0_CA_CMD7_RG_RK0_RX_ARCS1_R_DLY              Fld(6, 0) //[5:0]
    #define SHU3_R0_CA_CMD7_RG_RK0_RX_ARCS1_F_DLY              Fld(6, 8) //[13:8]
    #define SHU3_R0_CA_CMD7_RG_RK0_RX_ARCS2_R_DLY              Fld(6, 16) //[21:16]
    #define SHU3_R0_CA_CMD7_RG_RK0_RX_ARCS2_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R0_CA_CMD8                      (DDRPHY_AO_BASE_ADDRESS + 0x018C0)
    #define SHU3_R0_CA_CMD8_RG_RK0_RX_ARCLK_R_DLY              Fld(7, 16) //[22:16]
    #define SHU3_R0_CA_CMD8_RG_RK0_RX_ARCLK_F_DLY              Fld(7, 24) //[30:24]

#define DDRPHY_SHU3_R0_CA_CMD9                      (DDRPHY_AO_BASE_ADDRESS + 0x018C4)
    #define SHU3_R0_CA_CMD9_RG_RK0_ARPI_CS                     Fld(6, 0) //[5:0]
    #define SHU3_R0_CA_CMD9_RG_RK0_ARPI_CMD                    Fld(6, 8) //[13:8]
    #define SHU3_R0_CA_CMD9_RG_RK0_ARPI_CLK                    Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X18C8                           (DDRPHY_AO_BASE_ADDRESS + 0x018C8)
    #define RFU_0X18C8_RESERVED_0X18C8                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X18CC                           (DDRPHY_AO_BASE_ADDRESS + 0x018CC)
    #define RFU_0X18CC_RESERVED_0X18CC                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU3_R1_B0_DQ0                       (DDRPHY_AO_BASE_ADDRESS + 0x01900)
    #define SHU3_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0                 Fld(4, 0) //[3:0]
    #define SHU3_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0                 Fld(4, 4) //[7:4]
    #define SHU3_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0                 Fld(4, 8) //[11:8]
    #define SHU3_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0                 Fld(4, 12) //[15:12]
    #define SHU3_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0                 Fld(4, 16) //[19:16]
    #define SHU3_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0                 Fld(4, 20) //[23:20]
    #define SHU3_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0                 Fld(4, 24) //[27:24]
    #define SHU3_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU3_R1_B0_DQ1                       (DDRPHY_AO_BASE_ADDRESS + 0x01904)
    #define SHU3_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0                Fld(4, 0) //[3:0]
    #define SHU3_R1_B0_DQ1_RK1_TX_ARDQS0_DLYB_B0               Fld(4, 16) //[19:16]
    #define SHU3_R1_B0_DQ1_RK1_TX_ARDQS0B_DLYB_B0              Fld(4, 20) //[23:20]
    #define SHU3_R1_B0_DQ1_RK1_TX_ARDQS0_DLY_B0                Fld(4, 24) //[27:24]
    #define SHU3_R1_B0_DQ1_RK1_TX_ARDQS0B_DLY_B0               Fld(4, 28) //[31:28]

#define DDRPHY_SHU3_R1_B0_DQ2                       (DDRPHY_AO_BASE_ADDRESS + 0x01908)
    #define SHU3_R1_B0_DQ2_RK1_RX_ARDQ0_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU3_R1_B0_DQ2_RK1_RX_ARDQ0_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU3_R1_B0_DQ2_RK1_RX_ARDQ1_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU3_R1_B0_DQ2_RK1_RX_ARDQ1_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R1_B0_DQ3                       (DDRPHY_AO_BASE_ADDRESS + 0x0190C)
    #define SHU3_R1_B0_DQ3_RK1_RX_ARDQ2_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU3_R1_B0_DQ3_RK1_RX_ARDQ2_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU3_R1_B0_DQ3_RK1_RX_ARDQ3_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU3_R1_B0_DQ3_RK1_RX_ARDQ3_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R1_B0_DQ4                       (DDRPHY_AO_BASE_ADDRESS + 0x01910)
    #define SHU3_R1_B0_DQ4_RK1_RX_ARDQ4_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU3_R1_B0_DQ4_RK1_RX_ARDQ4_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU3_R1_B0_DQ4_RK1_RX_ARDQ5_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU3_R1_B0_DQ4_RK1_RX_ARDQ5_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R1_B0_DQ5                       (DDRPHY_AO_BASE_ADDRESS + 0x01914)
    #define SHU3_R1_B0_DQ5_RK1_RX_ARDQ6_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU3_R1_B0_DQ5_RK1_RX_ARDQ6_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU3_R1_B0_DQ5_RK1_RX_ARDQ7_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU3_R1_B0_DQ5_RK1_RX_ARDQ7_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R1_B0_DQ6                       (DDRPHY_AO_BASE_ADDRESS + 0x01918)
    #define SHU3_R1_B0_DQ6_RK1_RX_ARDQM0_R_DLY_B0              Fld(6, 0) //[5:0]
    #define SHU3_R1_B0_DQ6_RK1_RX_ARDQM0_F_DLY_B0              Fld(6, 8) //[13:8]
    #define SHU3_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0              Fld(7, 16) //[22:16]
    #define SHU3_R1_B0_DQ6_RK1_RX_ARDQS0_F_DLY_B0              Fld(7, 24) //[30:24]

#define DDRPHY_SHU3_R1_B0_DQ7                       (DDRPHY_AO_BASE_ADDRESS + 0x0191C)
    #define SHU3_R1_B0_DQ7_RK1_ARPI_DQ_B0                      Fld(6, 8) //[13:8]
    #define SHU3_R1_B0_DQ7_RK1_ARPI_DQM_B0                     Fld(6, 16) //[21:16]
    #define SHU3_R1_B0_DQ7_RK1_ARPI_PBYTE_B0                   Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X1920                           (DDRPHY_AO_BASE_ADDRESS + 0x01920)
    #define RFU_0X1920_RESERVED_0X1920                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1924                           (DDRPHY_AO_BASE_ADDRESS + 0x01924)
    #define RFU_0X1924_RESERVED_0X1924                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1928                           (DDRPHY_AO_BASE_ADDRESS + 0x01928)
    #define RFU_0X1928_RESERVED_0X1928                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X192C                           (DDRPHY_AO_BASE_ADDRESS + 0x0192C)
    #define RFU_0X192C_RESERVED_0X192C                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU3_R1_B1_DQ0                       (DDRPHY_AO_BASE_ADDRESS + 0x01950)
    #define SHU3_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1                 Fld(4, 0) //[3:0]
    #define SHU3_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1                 Fld(4, 4) //[7:4]
    #define SHU3_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1                 Fld(4, 8) //[11:8]
    #define SHU3_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1                 Fld(4, 12) //[15:12]
    #define SHU3_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1                 Fld(4, 16) //[19:16]
    #define SHU3_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1                 Fld(4, 20) //[23:20]
    #define SHU3_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1                 Fld(4, 24) //[27:24]
    #define SHU3_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU3_R1_B1_DQ1                       (DDRPHY_AO_BASE_ADDRESS + 0x01954)
    #define SHU3_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1                Fld(4, 0) //[3:0]
    #define SHU3_R1_B1_DQ1_RK1_TX_ARDQS0_DLYB_B1               Fld(4, 16) //[19:16]
    #define SHU3_R1_B1_DQ1_RK1_TX_ARDQS0B_DLYB_B1              Fld(4, 20) //[23:20]
    #define SHU3_R1_B1_DQ1_RK1_TX_ARDQS0_DLY_B1                Fld(4, 24) //[27:24]
    #define SHU3_R1_B1_DQ1_RK1_TX_ARDQS0B_DLY_B1               Fld(4, 28) //[31:28]

#define DDRPHY_SHU3_R1_B1_DQ2                       (DDRPHY_AO_BASE_ADDRESS + 0x01958)
    #define SHU3_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU3_R1_B1_DQ2_RK1_RX_ARDQ0_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU3_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU3_R1_B1_DQ2_RK1_RX_ARDQ1_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R1_B1_DQ3                       (DDRPHY_AO_BASE_ADDRESS + 0x0195C)
    #define SHU3_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU3_R1_B1_DQ3_RK1_RX_ARDQ2_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU3_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU3_R1_B1_DQ3_RK1_RX_ARDQ3_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R1_B1_DQ4                       (DDRPHY_AO_BASE_ADDRESS + 0x01960)
    #define SHU3_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU3_R1_B1_DQ4_RK1_RX_ARDQ4_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU3_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU3_R1_B1_DQ4_RK1_RX_ARDQ5_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R1_B1_DQ5                       (DDRPHY_AO_BASE_ADDRESS + 0x01964)
    #define SHU3_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU3_R1_B1_DQ5_RK1_RX_ARDQ6_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU3_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU3_R1_B1_DQ5_RK1_RX_ARDQ7_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R1_B1_DQ6                       (DDRPHY_AO_BASE_ADDRESS + 0x01968)
    #define SHU3_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1              Fld(6, 0) //[5:0]
    #define SHU3_R1_B1_DQ6_RK1_RX_ARDQM0_F_DLY_B1              Fld(6, 8) //[13:8]
    #define SHU3_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1              Fld(7, 16) //[22:16]
    #define SHU3_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1              Fld(7, 24) //[30:24]

#define DDRPHY_SHU3_R1_B1_DQ7                       (DDRPHY_AO_BASE_ADDRESS + 0x0196C)
    #define SHU3_R1_B1_DQ7_RK1_ARPI_DQ_B1                      Fld(6, 8) //[13:8]
    #define SHU3_R1_B1_DQ7_RK1_ARPI_DQM_B1                     Fld(6, 16) //[21:16]
    #define SHU3_R1_B1_DQ7_RK1_ARPI_PBYTE_B1                   Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X1970                           (DDRPHY_AO_BASE_ADDRESS + 0x01970)
    #define RFU_0X1970_RESERVED_0X1970                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1974                           (DDRPHY_AO_BASE_ADDRESS + 0x01974)
    #define RFU_0X1974_RESERVED_0X1974                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1978                           (DDRPHY_AO_BASE_ADDRESS + 0x01978)
    #define RFU_0X1978_RESERVED_0X1978                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X197C                           (DDRPHY_AO_BASE_ADDRESS + 0x0197C)
    #define RFU_0X197C_RESERVED_0X197C                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU3_R1_CA_CMD0                      (DDRPHY_AO_BASE_ADDRESS + 0x019A0)
    #define SHU3_R1_CA_CMD0_RK1_TX_ARCA0_DLY                   Fld(4, 0) //[3:0]
    #define SHU3_R1_CA_CMD0_RK1_TX_ARCA1_DLY                   Fld(4, 4) //[7:4]
    #define SHU3_R1_CA_CMD0_RK1_TX_ARCA2_DLY                   Fld(4, 8) //[11:8]
    #define SHU3_R1_CA_CMD0_RK1_TX_ARCA3_DLY                   Fld(4, 12) //[15:12]
    #define SHU3_R1_CA_CMD0_RK1_TX_ARCA4_DLY                   Fld(4, 16) //[19:16]
    #define SHU3_R1_CA_CMD0_RK1_TX_ARCA5_DLY                   Fld(4, 20) //[23:20]
    #define SHU3_R1_CA_CMD0_RK1_TX_ARCLK_DLYB                  Fld(4, 24) //[27:24]
    #define SHU3_R1_CA_CMD0_RK1_TX_ARCLKB_DLYB                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU3_R1_CA_CMD1                      (DDRPHY_AO_BASE_ADDRESS + 0x019A4)
    #define SHU3_R1_CA_CMD1_RK1_TX_ARCKE0_DLY                  Fld(4, 0) //[3:0]
    #define SHU3_R1_CA_CMD1_RK1_TX_ARCKE1_DLY                  Fld(4, 4) //[7:4]
    #define SHU3_R1_CA_CMD1_RK1_TX_ARCKE2_DLY                  Fld(4, 8) //[11:8]
    #define SHU3_R1_CA_CMD1_RK1_TX_ARCS0_DLY                   Fld(4, 12) //[15:12]
    #define SHU3_R1_CA_CMD1_RK1_TX_ARCS1_DLY                   Fld(4, 16) //[19:16]
    #define SHU3_R1_CA_CMD1_RK1_TX_ARCS2_DLY                   Fld(4, 20) //[23:20]
    #define SHU3_R1_CA_CMD1_RK1_TX_ARCLK_DLY                   Fld(4, 24) //[27:24]
    #define SHU3_R1_CA_CMD1_RK1_TX_ARCLKB_DLY                  Fld(4, 28) //[31:28]

#define DDRPHY_SHU3_R1_CA_CMD2                      (DDRPHY_AO_BASE_ADDRESS + 0x019A8)
    #define SHU3_R1_CA_CMD2_RG_RK1_RX_ARCA0_R_DLY              Fld(6, 0) //[5:0]
    #define SHU3_R1_CA_CMD2_RG_RK1_RX_ARCA0_F_DLY              Fld(6, 8) //[13:8]
    #define SHU3_R1_CA_CMD2_RG_RK1_RX_ARCA1_R_DLY              Fld(6, 16) //[21:16]
    #define SHU3_R1_CA_CMD2_RG_RK1_RX_ARCA1_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R1_CA_CMD3                      (DDRPHY_AO_BASE_ADDRESS + 0x019AC)
    #define SHU3_R1_CA_CMD3_RG_RK1_RX_ARCA2_R_DLY              Fld(6, 0) //[5:0]
    #define SHU3_R1_CA_CMD3_RG_RK1_RX_ARCA2_F_DLY              Fld(6, 8) //[13:8]
    #define SHU3_R1_CA_CMD3_RG_RK1_RX_ARCA3_R_DLY              Fld(6, 16) //[21:16]
    #define SHU3_R1_CA_CMD3_RG_RK1_RX_ARCA3_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R1_CA_CMD4                      (DDRPHY_AO_BASE_ADDRESS + 0x019B0)
    #define SHU3_R1_CA_CMD4_RG_RK1_RX_ARCA4_R_DLY              Fld(6, 0) //[5:0]
    #define SHU3_R1_CA_CMD4_RG_RK1_RX_ARCA4_F_DLY              Fld(6, 8) //[13:8]
    #define SHU3_R1_CA_CMD4_RG_RK1_RX_ARCA5_R_DLY              Fld(6, 16) //[21:16]
    #define SHU3_R1_CA_CMD4_RG_RK1_RX_ARCA5_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R1_CA_CMD5                      (DDRPHY_AO_BASE_ADDRESS + 0x019B4)
    #define SHU3_R1_CA_CMD5_RG_RK1_RX_ARCKE0_R_DLY             Fld(6, 0) //[5:0]
    #define SHU3_R1_CA_CMD5_RG_RK1_RX_ARCKE0_F_DLY             Fld(6, 8) //[13:8]
    #define SHU3_R1_CA_CMD5_RG_RK1_RX_ARCKE1_R_DLY             Fld(6, 16) //[21:16]
    #define SHU3_R1_CA_CMD5_RG_RK1_RX_ARCKE1_F_DLY             Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R1_CA_CMD6                      (DDRPHY_AO_BASE_ADDRESS + 0x019B8)
    #define SHU3_R1_CA_CMD6_RG_RK1_RX_ARCKE2_R_DLY             Fld(6, 0) //[5:0]
    #define SHU3_R1_CA_CMD6_RG_RK1_RX_ARCKE2_F_DLY             Fld(6, 8) //[13:8]
    #define SHU3_R1_CA_CMD6_RG_RK1_RX_ARCS0_R_DLY              Fld(6, 16) //[21:16]
    #define SHU3_R1_CA_CMD6_RG_RK1_RX_ARCS0_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R1_CA_CMD7                      (DDRPHY_AO_BASE_ADDRESS + 0x019BC)
    #define SHU3_R1_CA_CMD7_RG_RK1_RX_ARCS1_R_DLY              Fld(6, 0) //[5:0]
    #define SHU3_R1_CA_CMD7_RG_RK1_RX_ARCS1_F_DLY              Fld(6, 8) //[13:8]
    #define SHU3_R1_CA_CMD7_RG_RK1_RX_ARCS2_R_DLY              Fld(6, 16) //[21:16]
    #define SHU3_R1_CA_CMD7_RG_RK1_RX_ARCS2_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R1_CA_CMD8                      (DDRPHY_AO_BASE_ADDRESS + 0x019C0)
    #define SHU3_R1_CA_CMD8_RG_RK1_RX_ARCLK_R_DLY              Fld(7, 16) //[22:16]
    #define SHU3_R1_CA_CMD8_RG_RK1_RX_ARCLK_F_DLY              Fld(7, 24) //[30:24]

#define DDRPHY_SHU3_R1_CA_CMD9                      (DDRPHY_AO_BASE_ADDRESS + 0x019C4)
    #define SHU3_R1_CA_CMD9_RG_RK1_ARPI_CS                     Fld(6, 0) //[5:0]
    #define SHU3_R1_CA_CMD9_RG_RK1_ARPI_CMD                    Fld(6, 8) //[13:8]
    #define SHU3_R1_CA_CMD9_RG_RK1_ARPI_CLK                    Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X19C8                           (DDRPHY_AO_BASE_ADDRESS + 0x019C8)
    #define RFU_0X19C8_RESERVED_0X19C8                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X19CC                           (DDRPHY_AO_BASE_ADDRESS + 0x019CC)
    #define RFU_0X19CC_RESERVED_0X19CC                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU3_R2_B0_DQ0                       (DDRPHY_AO_BASE_ADDRESS + 0x01A00)
    #define SHU3_R2_B0_DQ0_RK2_TX_ARDQ0_DLY_B0                 Fld(4, 0) //[3:0]
    #define SHU3_R2_B0_DQ0_RK2_TX_ARDQ1_DLY_B0                 Fld(4, 4) //[7:4]
    #define SHU3_R2_B0_DQ0_RK2_TX_ARDQ2_DLY_B0                 Fld(4, 8) //[11:8]
    #define SHU3_R2_B0_DQ0_RK2_TX_ARDQ3_DLY_B0                 Fld(4, 12) //[15:12]
    #define SHU3_R2_B0_DQ0_RK2_TX_ARDQ4_DLY_B0                 Fld(4, 16) //[19:16]
    #define SHU3_R2_B0_DQ0_RK2_TX_ARDQ5_DLY_B0                 Fld(4, 20) //[23:20]
    #define SHU3_R2_B0_DQ0_RK2_TX_ARDQ6_DLY_B0                 Fld(4, 24) //[27:24]
    #define SHU3_R2_B0_DQ0_RK2_TX_ARDQ7_DLY_B0                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU3_R2_B0_DQ1                       (DDRPHY_AO_BASE_ADDRESS + 0x01A04)
    #define SHU3_R2_B0_DQ1_RK2_TX_ARDQM0_DLY_B0                Fld(4, 0) //[3:0]
    #define SHU3_R2_B0_DQ1_RK2_TX_ARDQS0_DLYB_B0               Fld(4, 16) //[19:16]
    #define SHU3_R2_B0_DQ1_RK2_TX_ARDQS0B_DLYB_B0              Fld(4, 20) //[23:20]
    #define SHU3_R2_B0_DQ1_RK2_TX_ARDQS0_DLY_B0                Fld(4, 24) //[27:24]
    #define SHU3_R2_B0_DQ1_RK2_TX_ARDQS0B_DLY_B0               Fld(4, 28) //[31:28]

#define DDRPHY_SHU3_R2_B0_DQ2                       (DDRPHY_AO_BASE_ADDRESS + 0x01A08)
    #define SHU3_R2_B0_DQ2_RK2_RX_ARDQ0_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU3_R2_B0_DQ2_RK2_RX_ARDQ0_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU3_R2_B0_DQ2_RK2_RX_ARDQ1_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU3_R2_B0_DQ2_RK2_RX_ARDQ1_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R2_B0_DQ3                       (DDRPHY_AO_BASE_ADDRESS + 0x01A0C)
    #define SHU3_R2_B0_DQ3_RK2_RX_ARDQ2_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU3_R2_B0_DQ3_RK2_RX_ARDQ2_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU3_R2_B0_DQ3_RK2_RX_ARDQ3_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU3_R2_B0_DQ3_RK2_RX_ARDQ3_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R2_B0_DQ4                       (DDRPHY_AO_BASE_ADDRESS + 0x01A10)
    #define SHU3_R2_B0_DQ4_RK2_RX_ARDQ4_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU3_R2_B0_DQ4_RK2_RX_ARDQ4_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU3_R2_B0_DQ4_RK2_RX_ARDQ5_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU3_R2_B0_DQ4_RK2_RX_ARDQ5_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R2_B0_DQ5                       (DDRPHY_AO_BASE_ADDRESS + 0x01A14)
    #define SHU3_R2_B0_DQ5_RK2_RX_ARDQ6_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU3_R2_B0_DQ5_RK2_RX_ARDQ6_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU3_R2_B0_DQ5_RK2_RX_ARDQ7_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU3_R2_B0_DQ5_RK2_RX_ARDQ7_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R2_B0_DQ6                       (DDRPHY_AO_BASE_ADDRESS + 0x01A18)
    #define SHU3_R2_B0_DQ6_RK2_RX_ARDQM0_R_DLY_B0              Fld(6, 0) //[5:0]
    #define SHU3_R2_B0_DQ6_RK2_RX_ARDQM0_F_DLY_B0              Fld(6, 8) //[13:8]
    #define SHU3_R2_B0_DQ6_RK2_RX_ARDQS0_R_DLY_B0              Fld(7, 16) //[22:16]
    #define SHU3_R2_B0_DQ6_RK2_RX_ARDQS0_F_DLY_B0              Fld(7, 24) //[30:24]

#define DDRPHY_SHU3_R2_B0_DQ7                       (DDRPHY_AO_BASE_ADDRESS + 0x01A1C)
    #define SHU3_R2_B0_DQ7_RK2_ARPI_DQ_B0                      Fld(6, 8) //[13:8]
    #define SHU3_R2_B0_DQ7_RK2_ARPI_DQM_B0                     Fld(6, 16) //[21:16]
    #define SHU3_R2_B0_DQ7_RK2_ARPI_PBYTE_B0                   Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X1A20                           (DDRPHY_AO_BASE_ADDRESS + 0x01A20)
    #define RFU_0X1A20_RESERVED_0X1A20                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1A24                           (DDRPHY_AO_BASE_ADDRESS + 0x01A24)
    #define RFU_0X1A24_RESERVED_0X1A24                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1A28                           (DDRPHY_AO_BASE_ADDRESS + 0x01A28)
    #define RFU_0X1A28_RESERVED_0X1A28                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1A2C                           (DDRPHY_AO_BASE_ADDRESS + 0x01A2C)
    #define RFU_0X1A2C_RESERVED_0X1A2C                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU3_R2_B1_DQ0                       (DDRPHY_AO_BASE_ADDRESS + 0x01A50)
    #define SHU3_R2_B1_DQ0_RK2_TX_ARDQ0_DLY_B1                 Fld(4, 0) //[3:0]
    #define SHU3_R2_B1_DQ0_RK2_TX_ARDQ1_DLY_B1                 Fld(4, 4) //[7:4]
    #define SHU3_R2_B1_DQ0_RK2_TX_ARDQ2_DLY_B1                 Fld(4, 8) //[11:8]
    #define SHU3_R2_B1_DQ0_RK2_TX_ARDQ3_DLY_B1                 Fld(4, 12) //[15:12]
    #define SHU3_R2_B1_DQ0_RK2_TX_ARDQ4_DLY_B1                 Fld(4, 16) //[19:16]
    #define SHU3_R2_B1_DQ0_RK2_TX_ARDQ5_DLY_B1                 Fld(4, 20) //[23:20]
    #define SHU3_R2_B1_DQ0_RK2_TX_ARDQ6_DLY_B1                 Fld(4, 24) //[27:24]
    #define SHU3_R2_B1_DQ0_RK2_TX_ARDQ7_DLY_B1                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU3_R2_B1_DQ1                       (DDRPHY_AO_BASE_ADDRESS + 0x01A54)
    #define SHU3_R2_B1_DQ1_RK2_TX_ARDQM0_DLY_B1                Fld(4, 0) //[3:0]
    #define SHU3_R2_B1_DQ1_RK2_TX_ARDQS0_DLYB_B1               Fld(4, 16) //[19:16]
    #define SHU3_R2_B1_DQ1_RK2_TX_ARDQS0B_DLYB_B1              Fld(4, 20) //[23:20]
    #define SHU3_R2_B1_DQ1_RK2_TX_ARDQS0_DLY_B1                Fld(4, 24) //[27:24]
    #define SHU3_R2_B1_DQ1_RK2_TX_ARDQS0B_DLY_B1               Fld(4, 28) //[31:28]

#define DDRPHY_SHU3_R2_B1_DQ2                       (DDRPHY_AO_BASE_ADDRESS + 0x01A58)
    #define SHU3_R2_B1_DQ2_RK2_RX_ARDQ0_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU3_R2_B1_DQ2_RK2_RX_ARDQ0_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU3_R2_B1_DQ2_RK2_RX_ARDQ1_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU3_R2_B1_DQ2_RK2_RX_ARDQ1_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R2_B1_DQ3                       (DDRPHY_AO_BASE_ADDRESS + 0x01A5C)
    #define SHU3_R2_B1_DQ3_RK2_RX_ARDQ2_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU3_R2_B1_DQ3_RK2_RX_ARDQ2_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU3_R2_B1_DQ3_RK2_RX_ARDQ3_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU3_R2_B1_DQ3_RK2_RX_ARDQ3_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R2_B1_DQ4                       (DDRPHY_AO_BASE_ADDRESS + 0x01A60)
    #define SHU3_R2_B1_DQ4_RK2_RX_ARDQ4_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU3_R2_B1_DQ4_RK2_RX_ARDQ4_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU3_R2_B1_DQ4_RK2_RX_ARDQ5_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU3_R2_B1_DQ4_RK2_RX_ARDQ5_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R2_B1_DQ5                       (DDRPHY_AO_BASE_ADDRESS + 0x01A64)
    #define SHU3_R2_B1_DQ5_RK2_RX_ARDQ6_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU3_R2_B1_DQ5_RK2_RX_ARDQ6_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU3_R2_B1_DQ5_RK2_RX_ARDQ7_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU3_R2_B1_DQ5_RK2_RX_ARDQ7_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R2_B1_DQ6                       (DDRPHY_AO_BASE_ADDRESS + 0x01A68)
    #define SHU3_R2_B1_DQ6_RK2_RX_ARDQM0_R_DLY_B1              Fld(6, 0) //[5:0]
    #define SHU3_R2_B1_DQ6_RK2_RX_ARDQM0_F_DLY_B1              Fld(6, 8) //[13:8]
    #define SHU3_R2_B1_DQ6_RK2_RX_ARDQS0_R_DLY_B1              Fld(7, 16) //[22:16]
    #define SHU3_R2_B1_DQ6_RK2_RX_ARDQS0_F_DLY_B1              Fld(7, 24) //[30:24]

#define DDRPHY_SHU3_R2_B1_DQ7                       (DDRPHY_AO_BASE_ADDRESS + 0x01A6C)
    #define SHU3_R2_B1_DQ7_RK2_ARPI_DQ_B1                      Fld(6, 8) //[13:8]
    #define SHU3_R2_B1_DQ7_RK2_ARPI_DQM_B1                     Fld(6, 16) //[21:16]
    #define SHU3_R2_B1_DQ7_RK2_ARPI_PBYTE_B1                   Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X1A70                           (DDRPHY_AO_BASE_ADDRESS + 0x01A70)
    #define RFU_0X1A70_RESERVED_0X1A70                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1A74                           (DDRPHY_AO_BASE_ADDRESS + 0x01A74)
    #define RFU_0X1A74_RESERVED_0X1A74                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1A78                           (DDRPHY_AO_BASE_ADDRESS + 0x01A78)
    #define RFU_0X1A78_RESERVED_0X1A78                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1A7C                           (DDRPHY_AO_BASE_ADDRESS + 0x01A7C)
    #define RFU_0X1A7C_RESERVED_0X1A7C                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU3_R2_CA_CMD0                      (DDRPHY_AO_BASE_ADDRESS + 0x01AA0)
    #define SHU3_R2_CA_CMD0_RK2_TX_ARCA0_DLY                   Fld(4, 0) //[3:0]
    #define SHU3_R2_CA_CMD0_RK2_TX_ARCA1_DLY                   Fld(4, 4) //[7:4]
    #define SHU3_R2_CA_CMD0_RK2_TX_ARCA2_DLY                   Fld(4, 8) //[11:8]
    #define SHU3_R2_CA_CMD0_RK2_TX_ARCA3_DLY                   Fld(4, 12) //[15:12]
    #define SHU3_R2_CA_CMD0_RK2_TX_ARCA4_DLY                   Fld(4, 16) //[19:16]
    #define SHU3_R2_CA_CMD0_RK2_TX_ARCA5_DLY                   Fld(4, 20) //[23:20]
    #define SHU3_R2_CA_CMD0_RK2_TX_ARCLK_DLYB                  Fld(4, 24) //[27:24]
    #define SHU3_R2_CA_CMD0_RK2_TX_ARCLKB_DLYB                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU3_R2_CA_CMD1                      (DDRPHY_AO_BASE_ADDRESS + 0x01AA4)
    #define SHU3_R2_CA_CMD1_RK2_TX_ARCKE0_DLY                  Fld(4, 0) //[3:0]
    #define SHU3_R2_CA_CMD1_RK2_TX_ARCKE1_DLY                  Fld(4, 4) //[7:4]
    #define SHU3_R2_CA_CMD1_RK2_TX_ARCKE2_DLY                  Fld(4, 8) //[11:8]
    #define SHU3_R2_CA_CMD1_RK2_TX_ARCS0_DLY                   Fld(4, 12) //[15:12]
    #define SHU3_R2_CA_CMD1_RK2_TX_ARCS1_DLY                   Fld(4, 16) //[19:16]
    #define SHU3_R2_CA_CMD1_RK2_TX_ARCS2_DLY                   Fld(4, 20) //[23:20]
    #define SHU3_R2_CA_CMD1_RK2_TX_ARCLK_DLY                   Fld(4, 24) //[27:24]
    #define SHU3_R2_CA_CMD1_RK2_TX_ARCLKB_DLY                  Fld(4, 28) //[31:28]

#define DDRPHY_SHU3_R2_CA_CMD2                      (DDRPHY_AO_BASE_ADDRESS + 0x01AA8)
    #define SHU3_R2_CA_CMD2_RG_RK2_RX_ARCA0_R_DLY              Fld(6, 0) //[5:0]
    #define SHU3_R2_CA_CMD2_RG_RK2_RX_ARCA0_F_DLY              Fld(6, 8) //[13:8]
    #define SHU3_R2_CA_CMD2_RG_RK2_RX_ARCA1_R_DLY              Fld(6, 16) //[21:16]
    #define SHU3_R2_CA_CMD2_RG_RK2_RX_ARCA1_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R2_CA_CMD3                      (DDRPHY_AO_BASE_ADDRESS + 0x01AAC)
    #define SHU3_R2_CA_CMD3_RG_RK2_RX_ARCA2_R_DLY              Fld(6, 0) //[5:0]
    #define SHU3_R2_CA_CMD3_RG_RK2_RX_ARCA2_F_DLY              Fld(6, 8) //[13:8]
    #define SHU3_R2_CA_CMD3_RG_RK2_RX_ARCA3_R_DLY              Fld(6, 16) //[21:16]
    #define SHU3_R2_CA_CMD3_RG_RK2_RX_ARCA3_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R2_CA_CMD4                      (DDRPHY_AO_BASE_ADDRESS + 0x01AB0)
    #define SHU3_R2_CA_CMD4_RG_RK2_RX_ARCA4_R_DLY              Fld(6, 0) //[5:0]
    #define SHU3_R2_CA_CMD4_RG_RK2_RX_ARCA4_F_DLY              Fld(6, 8) //[13:8]
    #define SHU3_R2_CA_CMD4_RG_RK2_RX_ARCA5_R_DLY              Fld(6, 16) //[21:16]
    #define SHU3_R2_CA_CMD4_RG_RK2_RX_ARCA5_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R2_CA_CMD5                      (DDRPHY_AO_BASE_ADDRESS + 0x01AB4)
    #define SHU3_R2_CA_CMD5_RG_RK2_RX_ARCKE0_R_DLY             Fld(6, 0) //[5:0]
    #define SHU3_R2_CA_CMD5_RG_RK2_RX_ARCKE0_F_DLY             Fld(6, 8) //[13:8]
    #define SHU3_R2_CA_CMD5_RG_RK2_RX_ARCKE1_R_DLY             Fld(6, 16) //[21:16]
    #define SHU3_R2_CA_CMD5_RG_RK2_RX_ARCKE1_F_DLY             Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R2_CA_CMD6                      (DDRPHY_AO_BASE_ADDRESS + 0x01AB8)
    #define SHU3_R2_CA_CMD6_RG_RK2_RX_ARCKE2_R_DLY             Fld(6, 0) //[5:0]
    #define SHU3_R2_CA_CMD6_RG_RK2_RX_ARCKE2_F_DLY             Fld(6, 8) //[13:8]
    #define SHU3_R2_CA_CMD6_RG_RK2_RX_ARCS0_R_DLY              Fld(6, 16) //[21:16]
    #define SHU3_R2_CA_CMD6_RG_RK2_RX_ARCS0_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R2_CA_CMD7                      (DDRPHY_AO_BASE_ADDRESS + 0x01ABC)
    #define SHU3_R2_CA_CMD7_RG_RK2_RX_ARCS1_R_DLY              Fld(6, 0) //[5:0]
    #define SHU3_R2_CA_CMD7_RG_RK2_RX_ARCS1_F_DLY              Fld(6, 8) //[13:8]
    #define SHU3_R2_CA_CMD7_RG_RK2_RX_ARCS2_R_DLY              Fld(6, 16) //[21:16]
    #define SHU3_R2_CA_CMD7_RG_RK2_RX_ARCS2_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU3_R2_CA_CMD8                      (DDRPHY_AO_BASE_ADDRESS + 0x01AC0)
    #define SHU3_R2_CA_CMD8_RG_RK2_RX_ARCLK_R_DLY              Fld(7, 16) //[22:16]
    #define SHU3_R2_CA_CMD8_RG_RK2_RX_ARCLK_F_DLY              Fld(7, 24) //[30:24]

#define DDRPHY_SHU3_R2_CA_CMD9                      (DDRPHY_AO_BASE_ADDRESS + 0x01AC4)
    #define SHU3_R2_CA_CMD9_RG_RK2_ARPI_CS                     Fld(6, 0) //[5:0]
    #define SHU3_R2_CA_CMD9_RG_RK2_ARPI_CMD                    Fld(6, 8) //[13:8]
    #define SHU3_R2_CA_CMD9_RG_RK2_ARPI_CLK                    Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X1AC8                           (DDRPHY_AO_BASE_ADDRESS + 0x01AC8)
    #define RFU_0X1AC8_RESERVED_0X1AC8                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1ACC                           (DDRPHY_AO_BASE_ADDRESS + 0x01ACC)
    #define RFU_0X1ACC_RESERVED_0X1ACC                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU4_B0_DQ0                          (DDRPHY_AO_BASE_ADDRESS + 0x01B00)
    #define SHU4_B0_DQ0_RG_TX_ARDQS0_PRE_EN_B0                 Fld(1, 4) //[4:4]
    #define SHU4_B0_DQ0_RG_TX_ARDQS0_DRVP_PRE_B0               Fld(3, 8) //[10:8]
    #define SHU4_B0_DQ0_RG_TX_ARDQS0_DRVN_PRE_B0               Fld(3, 12) //[14:12]
    #define SHU4_B0_DQ0_RG_TX_ARDQ_PRE_EN_B0                   Fld(1, 20) //[20:20]
    #define SHU4_B0_DQ0_RG_TX_ARDQ_DRVP_PRE_B0                 Fld(3, 24) //[26:24]
    #define SHU4_B0_DQ0_RG_TX_ARDQ_DRVN_PRE_B0                 Fld(3, 28) //[30:28]
    #define SHU4_B0_DQ0_R_LP4Y_WDN_MODE_DQS0                   Fld(1, 31) //[31:31]

#define DDRPHY_SHU4_B0_DQ1                          (DDRPHY_AO_BASE_ADDRESS + 0x01B04)
    #define SHU4_B0_DQ1_RG_TX_ARDQ_DRVP_B0                     Fld(5, 0) //[4:0]
    #define SHU4_B0_DQ1_RG_TX_ARDQ_DRVN_B0                     Fld(5, 8) //[12:8]
    #define SHU4_B0_DQ1_RG_TX_ARDQ_ODTP_B0                     Fld(5, 16) //[20:16]
    #define SHU4_B0_DQ1_RG_TX_ARDQ_ODTN_B0                     Fld(5, 24) //[28:24]

#define DDRPHY_SHU4_B0_DQ2                          (DDRPHY_AO_BASE_ADDRESS + 0x01B08)
    #define SHU4_B0_DQ2_RG_TX_ARDQS0_DRVP_B0                   Fld(5, 0) //[4:0]
    #define SHU4_B0_DQ2_RG_TX_ARDQS0_DRVN_B0                   Fld(5, 8) //[12:8]
    #define SHU4_B0_DQ2_RG_TX_ARDQS0_ODTP_B0                   Fld(5, 16) //[20:16]
    #define SHU4_B0_DQ2_RG_TX_ARDQS0_ODTN_B0                   Fld(5, 24) //[28:24]

#define DDRPHY_SHU4_B0_DQ3                          (DDRPHY_AO_BASE_ADDRESS + 0x01B0C)
    #define SHU4_B0_DQ3_RG_TX_ARDQS0_PU_B0                     Fld(2, 0) //[1:0]
    #define SHU4_B0_DQ3_RG_TX_ARDQS0_PU_PRE_B0                 Fld(2, 2) //[3:2]
    #define SHU4_B0_DQ3_RG_TX_ARDQS0_PDB_B0                    Fld(2, 4) //[5:4]
    #define SHU4_B0_DQ3_RG_TX_ARDQS0_PDB_PRE_B0                Fld(2, 6) //[7:6]
    #define SHU4_B0_DQ3_RG_TX_ARDQ_PU_B0                       Fld(2, 8) //[9:8]
    #define SHU4_B0_DQ3_RG_TX_ARDQ_PU_PRE_B0                   Fld(2, 10) //[11:10]
    #define SHU4_B0_DQ3_RG_TX_ARDQ_PDB_B0                      Fld(2, 12) //[13:12]
    #define SHU4_B0_DQ3_RG_TX_ARDQ_PDB_PRE_B0                  Fld(2, 14) //[15:14]

#define DDRPHY_SHU4_B0_DQ4                          (DDRPHY_AO_BASE_ADDRESS + 0x01B10)
    #define SHU4_B0_DQ4_RG_ARPI_AA_MCK_DL_B0                   Fld(6, 0) //[5:0]
    #define SHU4_B0_DQ4_RG_ARPI_AA_MCK_FB_DL_B0                Fld(6, 8) //[13:8]
    #define SHU4_B0_DQ4_RG_ARPI_DA_MCK_FB_DL_B0                Fld(6, 16) //[21:16]

#define DDRPHY_SHU4_B0_DQ5                          (DDRPHY_AO_BASE_ADDRESS + 0x01B14)
    #define SHU4_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0                 Fld(6, 0) //[5:0]
    #define SHU4_B0_DQ5_RG_RX_ARDQ_VREF_BYPASS_B0              Fld(1, 6) //[6:6]
    #define SHU4_B0_DQ5_RG_ARPI_FB_B0                          Fld(6, 8) //[13:8]
    #define SHU4_B0_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B0             Fld(3, 16) //[18:16]
    #define SHU4_B0_DQ5_DA_RX_ARDQS_DQSIEN_RB_DLY_B0           Fld(1, 19) //[19:19]
    #define SHU4_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0                Fld(3, 20) //[22:20]
    #define SHU4_B0_DQ5_RG_ARPI_MCTL_B0                        Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_B0_DQ6                          (DDRPHY_AO_BASE_ADDRESS + 0x01B18)
    #define SHU4_B0_DQ6_RG_ARPI_OFFSET_DQSIEN_B0               Fld(6, 0) //[5:0]
    #define SHU4_B0_DQ6_RG_ARPI_RESERVE_B0                     Fld(16, 6) //[21:6]
    #define SHU4_B0_DQ6_RG_ARPI_MIDPI_CAP_SEL_B0               Fld(2, 22) //[23:22]
    #define SHU4_B0_DQ6_RG_ARPI_MIDPI_VTH_SEL_B0               Fld(2, 24) //[25:24]
    #define SHU4_B0_DQ6_RG_ARPI_MIDPI_EN_B0                    Fld(1, 26) //[26:26]
    #define SHU4_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0             Fld(1, 27) //[27:27]
    #define SHU4_B0_DQ6_RG_ARPI_CAP_SEL_B0                     Fld(2, 28) //[29:28]
    #define SHU4_B0_DQ6_RG_ARPI_MIDPI_BYPASS_EN_B0             Fld(1, 31) //[31:31]

#define DDRPHY_SHU4_B0_DQ7                          (DDRPHY_AO_BASE_ADDRESS + 0x01B1C)
    #define SHU4_B0_DQ7_R_DMRANKRXDVS_B0                       Fld(4, 0) //[3:0]
    #define SHU4_B0_DQ7_MIDPI_ENABLE                           Fld(1, 4) //[4:4]
    #define SHU4_B0_DQ7_MIDPI_DIV4_ENABLE                      Fld(1, 5) //[5:5]
    #define SHU4_B0_DQ7_R_DMDQMDBI_EYE_SHU_B0                  Fld(1, 6) //[6:6]
    #define SHU4_B0_DQ7_R_DMDQMDBI_SHU_B0                      Fld(1, 7) //[7:7]
    #define SHU4_B0_DQ7_R_DMRXDVS_DQM_FLAGSEL_B0               Fld(4, 8) //[11:8]
    #define SHU4_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0            Fld(1, 12) //[12:12]
    #define SHU4_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0              Fld(1, 13) //[13:13]
    #define SHU4_B0_DQ7_R_DMRXTRACK_DQM_EN_B0                  Fld(1, 14) //[14:14]
    #define SHU4_B0_DQ7_R_DMRODTEN_B0                          Fld(1, 15) //[15:15]
    #define SHU4_B0_DQ7_R_DMARPI_CG_FB2DLL_DCM_EN_B0           Fld(1, 16) //[16:16]
    #define SHU4_B0_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B0               Fld(1, 17) //[17:17]
    #define SHU4_B0_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B0              Fld(1, 18) //[18:18]
    #define SHU4_B0_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B0              Fld(1, 19) //[19:19]
    #define SHU4_B0_DQ7_R_LP4Y_SDN_MODE_DQS0                   Fld(1, 20) //[20:20]
    #define SHU4_B0_DQ7_R_DMRXRANK_DQ_EN_B0                    Fld(1, 24) //[24:24]
    #define SHU4_B0_DQ7_R_DMRXRANK_DQ_LAT_B0                   Fld(3, 25) //[27:25]
    #define SHU4_B0_DQ7_R_DMRXRANK_DQS_EN_B0                   Fld(1, 28) //[28:28]
    #define SHU4_B0_DQ7_R_DMRXRANK_DQS_LAT_B0                  Fld(3, 29) //[31:29]

#define DDRPHY_SHU4_B0_DQ8                          (DDRPHY_AO_BASE_ADDRESS + 0x01B20)
    #define SHU4_B0_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B0             Fld(15, 0) //[14:0]
    #define SHU4_B0_DQ8_R_DMRXDVS_UPD_FORCE_EN_B0              Fld(1, 15) //[15:15]
    #define SHU4_B0_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B0           Fld(1, 19) //[19:19]
    #define SHU4_B0_DQ8_R_RMRODTEN_CG_IG_B0                    Fld(1, 20) //[20:20]
    #define SHU4_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0                  Fld(1, 21) //[21:21]
    #define SHU4_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0          Fld(1, 22) //[22:22]
    #define SHU4_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0      Fld(1, 23) //[23:23]
    #define SHU4_B0_DQ8_R_DMRXDLY_CG_IG_B0                     Fld(1, 24) //[24:24]
    #define SHU4_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0                Fld(1, 25) //[25:25]
    #define SHU4_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0          Fld(1, 26) //[26:26]
    #define SHU4_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0          Fld(1, 27) //[27:27]
    #define SHU4_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0         Fld(1, 28) //[28:28]
    #define SHU4_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0     Fld(1, 29) //[29:29]
    #define SHU4_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0                 Fld(1, 30) //[30:30]
    #define SHU4_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0             Fld(1, 31) //[31:31]

#define DDRPHY_SHU4_B0_DQ9                          (DDRPHY_AO_BASE_ADDRESS + 0x01B24)
    #define SHU4_B0_DQ9_RESERVED_0X1B24                        Fld(32, 0) //[31:0]

#define DDRPHY_SHU4_B0_DQ10                         (DDRPHY_AO_BASE_ADDRESS + 0x01B28)
    #define SHU4_B0_DQ10_RESERVED_0X1B28                       Fld(32, 0) //[31:0]

#define DDRPHY_SHU4_B0_DQ11                         (DDRPHY_AO_BASE_ADDRESS + 0x01B2C)
    #define SHU4_B0_DQ11_RESERVED_0X1B2C                       Fld(32, 0) //[31:0]

#define DDRPHY_SHU4_B0_DQ12                         (DDRPHY_AO_BASE_ADDRESS + 0x01B30)
    #define SHU4_B0_DQ12_RESERVED_0X1B30                       Fld(32, 0) //[31:0]

#define DDRPHY_SHU4_B0_DLL0                         (DDRPHY_AO_BASE_ADDRESS + 0x01B34)
    #define SHU4_B0_DLL0_RG_ARPISM_MCK_SEL_B0_SHU              Fld(1, 0) //[0:0]
    #define SHU4_B0_DLL0_B0_DLL0_RFU                           Fld(1, 3) //[3:3]
    #define SHU4_B0_DLL0_RG_ARDLL_FAST_PSJP_B0                 Fld(1, 4) //[4:4]
    #define SHU4_B0_DLL0_RG_ARDLL_PHDIV_B0                     Fld(1, 9) //[9:9]
    #define SHU4_B0_DLL0_RG_ARDLL_PHJUMP_EN_B0                 Fld(1, 10) //[10:10]
    #define SHU4_B0_DLL0_RG_ARDLL_P_GAIN_B0                    Fld(4, 12) //[15:12]
    #define SHU4_B0_DLL0_RG_ARDLL_IDLECNT_B0                   Fld(4, 16) //[19:16]
    #define SHU4_B0_DLL0_RG_ARDLL_GAIN_B0                      Fld(4, 20) //[23:20]
    #define SHU4_B0_DLL0_RG_ARDLL_PHDET_IN_SWAP_B0             Fld(1, 30) //[30:30]
    #define SHU4_B0_DLL0_RG_ARDLL_PHDET_OUT_SEL_B0             Fld(1, 31) //[31:31]

#define DDRPHY_SHU4_B0_DLL1                         (DDRPHY_AO_BASE_ADDRESS + 0x01B38)
    #define SHU4_B0_DLL1_RG_ARDLL_FASTPJ_CK_SEL_B0             Fld(1, 0) //[0:0]
    #define SHU4_B0_DLL1_RG_ARDLL_PS_EN_B0                     Fld(1, 1) //[1:1]
    #define SHU4_B0_DLL1_RG_ARDLL_PD_CK_SEL_B0                 Fld(1, 2) //[2:2]
    #define SHU4_B0_DLL1_RG_ARDQ_REV_B0                        Fld(24, 8) //[31:8]

#define DDRPHY_SHU4_B1_DQ0                          (DDRPHY_AO_BASE_ADDRESS + 0x01B80)
    #define SHU4_B1_DQ0_RG_TX_ARDQS0_PRE_EN_B1                 Fld(1, 4) //[4:4]
    #define SHU4_B1_DQ0_RG_TX_ARDQS0_DRVP_PRE_B1               Fld(3, 8) //[10:8]
    #define SHU4_B1_DQ0_RG_TX_ARDQS0_DRVN_PRE_B1               Fld(3, 12) //[14:12]
    #define SHU4_B1_DQ0_RG_TX_ARDQ_PRE_EN_B1                   Fld(1, 20) //[20:20]
    #define SHU4_B1_DQ0_RG_TX_ARDQ_DRVP_PRE_B1                 Fld(3, 24) //[26:24]
    #define SHU4_B1_DQ0_RG_TX_ARDQ_DRVN_PRE_B1                 Fld(3, 28) //[30:28]
    #define SHU4_B1_DQ0_R_LP4Y_WDN_MODE_DQS1                   Fld(1, 31) //[31:31]

#define DDRPHY_SHU4_B1_DQ1                          (DDRPHY_AO_BASE_ADDRESS + 0x01B84)
    #define SHU4_B1_DQ1_RG_TX_ARDQ_DRVP_B1                     Fld(5, 0) //[4:0]
    #define SHU4_B1_DQ1_RG_TX_ARDQ_DRVN_B1                     Fld(5, 8) //[12:8]
    #define SHU4_B1_DQ1_RG_TX_ARDQ_ODTP_B1                     Fld(5, 16) //[20:16]
    #define SHU4_B1_DQ1_RG_TX_ARDQ_ODTN_B1                     Fld(5, 24) //[28:24]

#define DDRPHY_SHU4_B1_DQ2                          (DDRPHY_AO_BASE_ADDRESS + 0x01B88)
    #define SHU4_B1_DQ2_RG_TX_ARDQS0_DRVP_B1                   Fld(5, 0) //[4:0]
    #define SHU4_B1_DQ2_RG_TX_ARDQS0_DRVN_B1                   Fld(5, 8) //[12:8]
    #define SHU4_B1_DQ2_RG_TX_ARDQS0_ODTP_B1                   Fld(5, 16) //[20:16]
    #define SHU4_B1_DQ2_RG_TX_ARDQS0_ODTN_B1                   Fld(5, 24) //[28:24]

#define DDRPHY_SHU4_B1_DQ3                          (DDRPHY_AO_BASE_ADDRESS + 0x01B8C)
    #define SHU4_B1_DQ3_RG_TX_ARDQS0_PU_B1                     Fld(2, 0) //[1:0]
    #define SHU4_B1_DQ3_RG_TX_ARDQS0_PU_PRE_B1                 Fld(2, 2) //[3:2]
    #define SHU4_B1_DQ3_RG_TX_ARDQS0_PDB_B1                    Fld(2, 4) //[5:4]
    #define SHU4_B1_DQ3_RG_TX_ARDQS0_PDB_PRE_B1                Fld(2, 6) //[7:6]
    #define SHU4_B1_DQ3_RG_TX_ARDQ_PU_B1                       Fld(2, 8) //[9:8]
    #define SHU4_B1_DQ3_RG_TX_ARDQ_PU_PRE_B1                   Fld(2, 10) //[11:10]
    #define SHU4_B1_DQ3_RG_TX_ARDQ_PDB_B1                      Fld(2, 12) //[13:12]
    #define SHU4_B1_DQ3_RG_TX_ARDQ_PDB_PRE_B1                  Fld(2, 14) //[15:14]

#define DDRPHY_SHU4_B1_DQ4                          (DDRPHY_AO_BASE_ADDRESS + 0x01B90)
    #define SHU4_B1_DQ4_RG_ARPI_AA_MCK_DL_B1                   Fld(6, 0) //[5:0]
    #define SHU4_B1_DQ4_RG_ARPI_AA_MCK_FB_DL_B1                Fld(6, 8) //[13:8]
    #define SHU4_B1_DQ4_RG_ARPI_DA_MCK_FB_DL_B1                Fld(6, 16) //[21:16]

#define DDRPHY_SHU4_B1_DQ5                          (DDRPHY_AO_BASE_ADDRESS + 0x01B94)
    #define SHU4_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1                 Fld(6, 0) //[5:0]
    #define SHU4_B1_DQ5_RG_RX_ARDQ_VREF_BYPASS_B1              Fld(1, 6) //[6:6]
    #define SHU4_B1_DQ5_RG_ARPI_FB_B1                          Fld(6, 8) //[13:8]
    #define SHU4_B1_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B1             Fld(3, 16) //[18:16]
    #define SHU4_B1_DQ5_DA_RX_ARDQS_DQSIEN_RB_DLY_B1           Fld(1, 19) //[19:19]
    #define SHU4_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1                Fld(3, 20) //[22:20]
    #define SHU4_B1_DQ5_RG_ARPI_MCTL_B1                        Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_B1_DQ6                          (DDRPHY_AO_BASE_ADDRESS + 0x01B98)
    #define SHU4_B1_DQ6_RG_ARPI_OFFSET_DQSIEN_B1               Fld(6, 0) //[5:0]
    #define SHU4_B1_DQ6_RG_ARPI_RESERVE_B1                     Fld(16, 6) //[21:6]
    #define SHU4_B1_DQ6_RG_ARPI_MIDPI_CAP_SEL_B1               Fld(2, 22) //[23:22]
    #define SHU4_B1_DQ6_RG_ARPI_MIDPI_VTH_SEL_B1               Fld(2, 24) //[25:24]
    #define SHU4_B1_DQ6_RG_ARPI_MIDPI_EN_B1                    Fld(1, 26) //[26:26]
    #define SHU4_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1             Fld(1, 27) //[27:27]
    #define SHU4_B1_DQ6_RG_ARPI_CAP_SEL_B1                     Fld(2, 28) //[29:28]
    #define SHU4_B1_DQ6_RG_ARPI_MIDPI_BYPASS_EN_B1             Fld(1, 31) //[31:31]

#define DDRPHY_SHU4_B1_DQ7                          (DDRPHY_AO_BASE_ADDRESS + 0x01B9C)
    #define SHU4_B1_DQ7_R_DMRANKRXDVS_B1                       Fld(4, 0) //[3:0]
    #define SHU4_B1_DQ7_R_DMDQMDBI_EYE_SHU_B1                  Fld(1, 6) //[6:6]
    #define SHU4_B1_DQ7_R_DMDQMDBI_SHU_B1                      Fld(1, 7) //[7:7]
    #define SHU4_B1_DQ7_R_DMRXDVS_DQM_FLAGSEL_B1               Fld(4, 8) //[11:8]
    #define SHU4_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1            Fld(1, 12) //[12:12]
    #define SHU4_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1              Fld(1, 13) //[13:13]
    #define SHU4_B1_DQ7_R_DMRXTRACK_DQM_EN_B1                  Fld(1, 14) //[14:14]
    #define SHU4_B1_DQ7_R_DMRODTEN_B1                          Fld(1, 15) //[15:15]
    #define SHU4_B1_DQ7_R_DMARPI_CG_FB2DLL_DCM_EN_B1           Fld(1, 16) //[16:16]
    #define SHU4_B1_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B1               Fld(1, 17) //[17:17]
    #define SHU4_B1_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B1              Fld(1, 18) //[18:18]
    #define SHU4_B1_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B1              Fld(1, 19) //[19:19]
    #define SHU4_B1_DQ7_R_LP4Y_SDN_MODE_DQS1                   Fld(1, 20) //[20:20]
    #define SHU4_B1_DQ7_R_DMRXRANK_DQ_EN_B1                    Fld(1, 24) //[24:24]
    #define SHU4_B1_DQ7_R_DMRXRANK_DQ_LAT_B1                   Fld(3, 25) //[27:25]
    #define SHU4_B1_DQ7_R_DMRXRANK_DQS_EN_B1                   Fld(1, 28) //[28:28]
    #define SHU4_B1_DQ7_R_DMRXRANK_DQS_LAT_B1                  Fld(3, 29) //[31:29]

#define DDRPHY_SHU4_B1_DQ8                          (DDRPHY_AO_BASE_ADDRESS + 0x01BA0)
    #define SHU4_B1_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B1             Fld(15, 0) //[14:0]
    #define SHU4_B1_DQ8_R_DMRXDVS_UPD_FORCE_EN_B1              Fld(1, 15) //[15:15]
    #define SHU4_B1_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B1           Fld(1, 19) //[19:19]
    #define SHU4_B1_DQ8_R_RMRODTEN_CG_IG_B1                    Fld(1, 20) //[20:20]
    #define SHU4_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1                  Fld(1, 21) //[21:21]
    #define SHU4_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1          Fld(1, 22) //[22:22]
    #define SHU4_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1      Fld(1, 23) //[23:23]
    #define SHU4_B1_DQ8_R_DMRXDLY_CG_IG_B1                     Fld(1, 24) //[24:24]
    #define SHU4_B1_DQ8_R_DMSTBEN_SYNC_CG_IG_B1                Fld(1, 25) //[25:25]
    #define SHU4_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1          Fld(1, 26) //[26:26]
    #define SHU4_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1          Fld(1, 27) //[27:27]
    #define SHU4_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1         Fld(1, 28) //[28:28]
    #define SHU4_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1     Fld(1, 29) //[29:29]
    #define SHU4_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1                 Fld(1, 30) //[30:30]
    #define SHU4_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1             Fld(1, 31) //[31:31]

#define DDRPHY_SHU4_B1_DQ9                          (DDRPHY_AO_BASE_ADDRESS + 0x01BA4)
    #define SHU4_B1_DQ9_RESERVED_0X1BA4                        Fld(32, 0) //[31:0]

#define DDRPHY_SHU4_B1_DQ10                         (DDRPHY_AO_BASE_ADDRESS + 0x01BA8)
    #define SHU4_B1_DQ10_RESERVED_0X1BA8                       Fld(32, 0) //[31:0]

#define DDRPHY_SHU4_B1_DQ11                         (DDRPHY_AO_BASE_ADDRESS + 0x01BAC)
    #define SHU4_B1_DQ11_RESERVED_0X1BAC                       Fld(32, 0) //[31:0]

#define DDRPHY_SHU4_B1_DQ12                         (DDRPHY_AO_BASE_ADDRESS + 0x01BB0)
    #define SHU4_B1_DQ12_RESERVED_0X1BB0                       Fld(32, 0) //[31:0]

#define DDRPHY_SHU4_B1_DLL0                         (DDRPHY_AO_BASE_ADDRESS + 0x01BB4)
    #define SHU4_B1_DLL0_RG_ARPISM_MCK_SEL_B1_SHU              Fld(1, 0) //[0:0]
    #define SHU4_B1_DLL0_B1_DLL0_RFU                           Fld(1, 3) //[3:3]
    #define SHU4_B1_DLL0_RG_ARDLL_FAST_PSJP_B1                 Fld(1, 4) //[4:4]
    #define SHU4_B1_DLL0_RG_ARDLL_PHDIV_B1                     Fld(1, 9) //[9:9]
    #define SHU4_B1_DLL0_RG_ARDLL_PHJUMP_EN_B1                 Fld(1, 10) //[10:10]
    #define SHU4_B1_DLL0_RG_ARDLL_P_GAIN_B1                    Fld(4, 12) //[15:12]
    #define SHU4_B1_DLL0_RG_ARDLL_IDLECNT_B1                   Fld(4, 16) //[19:16]
    #define SHU4_B1_DLL0_RG_ARDLL_GAIN_B1                      Fld(4, 20) //[23:20]
    #define SHU4_B1_DLL0_RG_ARDLL_PHDET_IN_SWAP_B1             Fld(1, 30) //[30:30]
    #define SHU4_B1_DLL0_RG_ARDLL_PHDET_OUT_SEL_B1             Fld(1, 31) //[31:31]

#define DDRPHY_SHU4_B1_DLL1                         (DDRPHY_AO_BASE_ADDRESS + 0x01BB8)
    #define SHU4_B1_DLL1_RG_ARDLL_FASTPJ_CK_SEL_B1             Fld(1, 0) //[0:0]
    #define SHU4_B1_DLL1_RG_ARDLL_PS_EN_B1                     Fld(1, 1) //[1:1]
    #define SHU4_B1_DLL1_RG_ARDLL_PD_CK_SEL_B1                 Fld(1, 2) //[2:2]
    #define SHU4_B1_DLL1_RG_ARDQ_REV_B1                        Fld(24, 8) //[31:8]

#define DDRPHY_SHU4_CA_CMD0                         (DDRPHY_AO_BASE_ADDRESS + 0x01C00)
    #define SHU4_CA_CMD0_RG_TX_ARCLK_PRE_EN                    Fld(1, 4) //[4:4]
    #define SHU4_CA_CMD0_RG_TX_ARCLK_DRVP_PRE                  Fld(3, 8) //[10:8]
    #define SHU4_CA_CMD0_RG_TX_ARCLK_DRVN_PRE                  Fld(3, 12) //[14:12]
    #define SHU4_CA_CMD0_RG_CGEN_FMEM_CK_CG_DLL                Fld(1, 17) //[17:17]
    #define SHU4_CA_CMD0_RG_FB_CK_MUX                          Fld(2, 18) //[19:18]
    #define SHU4_CA_CMD0_RG_TX_ARCMD_PRE_EN                    Fld(1, 20) //[20:20]
    #define SHU4_CA_CMD0_RG_TX_ARCMD_DRVP_PRE                  Fld(3, 24) //[26:24]
    #define SHU4_CA_CMD0_RG_TX_ARCMD_DRVN_PRE                  Fld(3, 28) //[30:28]
    #define SHU4_CA_CMD0_R_LP4Y_WDN_MODE_CLK                   Fld(1, 31) //[31:31]

#define DDRPHY_SHU4_CA_CMD1                         (DDRPHY_AO_BASE_ADDRESS + 0x01C04)
    #define SHU4_CA_CMD1_RG_TX_ARCMD_DRVP                      Fld(5, 0) //[4:0]
    #define SHU4_CA_CMD1_RG_TX_ARCMD_DRVN                      Fld(5, 8) //[12:8]
    #define SHU4_CA_CMD1_RG_TX_ARCMD_ODTP                      Fld(5, 16) //[20:16]
    #define SHU4_CA_CMD1_RG_TX_ARCMD_ODTN                      Fld(5, 24) //[28:24]

#define DDRPHY_SHU4_CA_CMD2                         (DDRPHY_AO_BASE_ADDRESS + 0x01C08)
    #define SHU4_CA_CMD2_RG_TX_ARCLK_DRVP                      Fld(5, 0) //[4:0]
    #define SHU4_CA_CMD2_RG_TX_ARCLK_DRVN                      Fld(5, 8) //[12:8]
    #define SHU4_CA_CMD2_RG_TX_ARCLK_ODTP                      Fld(5, 16) //[20:16]
    #define SHU4_CA_CMD2_RG_TX_ARCLK_ODTN                      Fld(5, 24) //[28:24]

#define DDRPHY_SHU4_CA_CMD3                         (DDRPHY_AO_BASE_ADDRESS + 0x01C0C)
    #define SHU4_CA_CMD3_RG_TX_ARCLK_PU                        Fld(2, 0) //[1:0]
    #define SHU4_CA_CMD3_RG_TX_ARCLK_PU_PRE                    Fld(2, 2) //[3:2]
    #define SHU4_CA_CMD3_RG_TX_ARCLK_PDB                       Fld(2, 4) //[5:4]
    #define SHU4_CA_CMD3_RG_TX_ARCLK_PDB_PRE                   Fld(2, 6) //[7:6]
    #define SHU4_CA_CMD3_RG_TX_ARCMD_PU                        Fld(2, 8) //[9:8]
    #define SHU4_CA_CMD3_RG_TX_ARCMD_PU_PRE                    Fld(2, 10) //[11:10]
    #define SHU4_CA_CMD3_RG_TX_ARCMD_PDB                       Fld(2, 12) //[13:12]
    #define SHU4_CA_CMD3_RG_TX_ARCMD_PDB_PRE                   Fld(2, 14) //[15:14]

#define DDRPHY_SHU4_CA_CMD4                         (DDRPHY_AO_BASE_ADDRESS + 0x01C10)
    #define SHU4_CA_CMD4_RG_ARPI_AA_MCK_DL_CA                  Fld(6, 0) //[5:0]
    #define SHU4_CA_CMD4_RG_ARPI_AA_MCK_FB_DL_CA               Fld(6, 8) //[13:8]
    #define SHU4_CA_CMD4_RG_ARPI_DA_MCK_FB_DL_CA               Fld(6, 16) //[21:16]

#define DDRPHY_SHU4_CA_CMD5                         (DDRPHY_AO_BASE_ADDRESS + 0x01C14)
    #define SHU4_CA_CMD5_RG_RX_ARCMD_VREF_SEL                  Fld(6, 0) //[5:0]
    #define SHU4_CA_CMD5_RG_RX_ARCMD_VREF_BYPASS               Fld(1, 6) //[6:6]
    #define SHU4_CA_CMD5_RG_ARPI_FB_CA                         Fld(6, 8) //[13:8]
    #define SHU4_CA_CMD5_RG_RX_ARCLK_DQSIEN_DLY                Fld(3, 16) //[18:16]
    #define SHU4_CA_CMD5_DA_RX_ARCLK_DQSIEN_RB_DLY             Fld(1, 19) //[19:19]
    #define SHU4_CA_CMD5_RG_RX_ARCLK_DVS_DLY                   Fld(3, 20) //[22:20]
    #define SHU4_CA_CMD5_RG_ARPI_MCTL_CA                       Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_CA_CMD6                         (DDRPHY_AO_BASE_ADDRESS + 0x01C18)
    #define SHU4_CA_CMD6_RG_ARPI_OFFSET_CLKIEN                 Fld(6, 0) //[5:0]
    #define SHU4_CA_CMD6_RG_ARPI_RESERVE_CA                    Fld(16, 6) //[21:6]
    #define SHU4_CA_CMD6_RG_ARPI_MIDPI_CAP_SEL_CA              Fld(2, 22) //[23:22]
    #define SHU4_CA_CMD6_RG_ARPI_MIDPI_VTH_SEL_CA              Fld(2, 24) //[25:24]
    #define SHU4_CA_CMD6_RG_ARPI_MIDPI_EN_CA                   Fld(1, 26) //[26:26]
    #define SHU4_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA            Fld(1, 27) //[27:27]
    #define SHU4_CA_CMD6_RG_ARPI_CAP_SEL_CA                    Fld(2, 28) //[29:28]
    #define SHU4_CA_CMD6_RG_ARPI_MIDPI_BYPASS_EN_CA            Fld(1, 31) //[31:31]

#define DDRPHY_SHU4_CA_CMD7                         (DDRPHY_AO_BASE_ADDRESS + 0x01C1C)
    #define SHU4_CA_CMD7_R_DMRANKRXDVS_CA                      Fld(4, 0) //[3:0]
    #define SHU4_CA_CMD7_R_DMRXDVS_PBYTE_FLAG_OPT_CA           Fld(1, 12) //[12:12]
    #define SHU4_CA_CMD7_R_DMRODTEN_CA                         Fld(1, 15) //[15:15]
    #define SHU4_CA_CMD7_R_DMARPI_CG_FB2DLL_DCM_EN_CA          Fld(1, 16) //[16:16]
    #define SHU4_CA_CMD7_R_DMTX_ARPI_CG_CMD_NEW                Fld(1, 17) //[17:17]
    #define SHU4_CA_CMD7_R_DMTX_ARPI_CG_CLK_NEW                Fld(1, 18) //[18:18]
    #define SHU4_CA_CMD7_R_DMTX_ARPI_CG_CS_NEW                 Fld(1, 19) //[19:19]
    #define SHU4_CA_CMD7_R_LP4Y_SDN_MODE_CLK                   Fld(1, 20) //[20:20]
    #define SHU4_CA_CMD7_R_DMRXRANK_CMD_EN                     Fld(1, 24) //[24:24]
    #define SHU4_CA_CMD7_R_DMRXRANK_CMD_LAT                    Fld(3, 25) //[27:25]
    #define SHU4_CA_CMD7_R_DMRXRANK_CLK_EN                     Fld(1, 28) //[28:28]
    #define SHU4_CA_CMD7_R_DMRXRANK_CLK_LAT                    Fld(3, 29) //[31:29]

#define DDRPHY_SHU4_CA_CMD8                         (DDRPHY_AO_BASE_ADDRESS + 0x01C20)
    #define SHU4_CA_CMD8_R_DMRXDVS_UPD_FORCE_CYC_CA            Fld(15, 0) //[14:0]
    #define SHU4_CA_CMD8_R_DMRXDVS_UPD_FORCE_EN_CA             Fld(1, 15) //[15:15]
    #define SHU4_CA_CMD8_R_DMRANK_RXDLY_PIPE_CG_IG_CA          Fld(1, 19) //[19:19]
    #define SHU4_CA_CMD8_R_RMRODTEN_CG_IG_CA                   Fld(1, 20) //[20:20]
    #define SHU4_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA                 Fld(1, 21) //[21:21]
    #define SHU4_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA         Fld(1, 22) //[22:22]
    #define SHU4_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA     Fld(1, 23) //[23:23]
    #define SHU4_CA_CMD8_R_DMRXDLY_CG_IG_CA                    Fld(1, 24) //[24:24]
    #define SHU4_CA_CMD8_R_DMSTBEN_SYNC_CG_IG_CA               Fld(1, 25) //[25:25]
    #define SHU4_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA         Fld(1, 26) //[26:26]
    #define SHU4_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA         Fld(1, 27) //[27:27]
    #define SHU4_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA        Fld(1, 28) //[28:28]
    #define SHU4_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA    Fld(1, 29) //[29:29]
    #define SHU4_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA                Fld(1, 30) //[30:30]
    #define SHU4_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA            Fld(1, 31) //[31:31]

#define DDRPHY_SHU4_CA_CMD9                         (DDRPHY_AO_BASE_ADDRESS + 0x01C24)
    #define SHU4_CA_CMD9_RESERVED_0X1C24                       Fld(32, 0) //[31:0]

#define DDRPHY_SHU4_CA_CMD10                        (DDRPHY_AO_BASE_ADDRESS + 0x01C28)
    #define SHU4_CA_CMD10_RESERVED_0X1C28                      Fld(32, 0) //[31:0]

#define DDRPHY_SHU4_CA_CMD11                        (DDRPHY_AO_BASE_ADDRESS + 0x01C2C)
    #define SHU4_CA_CMD11_RG_RIMP_REV                          Fld(8, 0) //[7:0]
    #define SHU4_CA_CMD11_RG_RIMP_VREF_SEL                     Fld(6, 8) //[13:8]
    #define SHU4_CA_CMD11_RG_TX_ARCKE_DRVP                     Fld(5, 17) //[21:17]
    #define SHU4_CA_CMD11_RG_TX_ARCKE_DRVN                     Fld(5, 22) //[26:22]

#define DDRPHY_SHU4_CA_CMD12                        (DDRPHY_AO_BASE_ADDRESS + 0x01C30)
    #define SHU4_CA_CMD12_RESERVED_0X1C30                      Fld(32, 0) //[31:0]

#define DDRPHY_SHU4_CA_DLL0                         (DDRPHY_AO_BASE_ADDRESS + 0x01C34)
    #define SHU4_CA_DLL0_RG_ARPISM_MCK_SEL_CA_SHU              Fld(1, 0) //[0:0]
    #define SHU4_CA_DLL0_CA_DLL0_RFU                           Fld(1, 3) //[3:3]
    #define SHU4_CA_DLL0_RG_ARDLL_FAST_PSJP_CA                 Fld(1, 4) //[4:4]
    #define SHU4_CA_DLL0_RG_ARDLL_PHDIV_CA                     Fld(1, 9) //[9:9]
    #define SHU4_CA_DLL0_RG_ARDLL_PHJUMP_EN_CA                 Fld(1, 10) //[10:10]
    #define SHU4_CA_DLL0_RG_ARDLL_P_GAIN_CA                    Fld(4, 12) //[15:12]
    #define SHU4_CA_DLL0_RG_ARDLL_IDLECNT_CA                   Fld(4, 16) //[19:16]
    #define SHU4_CA_DLL0_RG_ARDLL_GAIN_CA                      Fld(4, 20) //[23:20]
    #define SHU4_CA_DLL0_RG_ARDLL_PHDET_IN_SWAP_CA             Fld(1, 30) //[30:30]
    #define SHU4_CA_DLL0_RG_ARDLL_PHDET_OUT_SEL_CA             Fld(1, 31) //[31:31]

#define DDRPHY_SHU4_CA_DLL1                         (DDRPHY_AO_BASE_ADDRESS + 0x01C38)
    #define SHU4_CA_DLL1_RG_ARDLL_FASTPJ_CK_SEL_CA             Fld(1, 0) //[0:0]
    #define SHU4_CA_DLL1_RG_ARDLL_PS_EN_CA                     Fld(1, 1) //[1:1]
    #define SHU4_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA                 Fld(1, 2) //[2:2]
    #define SHU4_CA_DLL1_RG_ARCMD_REV                          Fld(24, 8) //[31:8]

#define DDRPHY_SHU4_MISC0                           (DDRPHY_AO_BASE_ADDRESS + 0x01CF0)
    #define SHU4_MISC0_R_RX_PIPE_BYPASS_EN                     Fld(1, 1) //[1:1]
    #define SHU4_MISC0_RG_CMD_TXPIPE_BYPASS_EN                 Fld(1, 2) //[2:2]
    #define SHU4_MISC0_RG_CK_TXPIPE_BYPASS_EN                  Fld(1, 3) //[3:3]
    #define SHU4_MISC0_RG_RVREF_SEL_DQ                         Fld(6, 16) //[21:16]
    #define SHU4_MISC0_RG_RVREF_DDR4_SEL                       Fld(1, 22) //[22:22]
    #define SHU4_MISC0_RG_RVREF_DDR3_SEL                       Fld(1, 23) //[23:23]
    #define SHU4_MISC0_RG_RVREF_SEL_CMD                        Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R0_B0_DQ0                       (DDRPHY_AO_BASE_ADDRESS + 0x01D00)
    #define SHU4_R0_B0_DQ0_RK0_TX_ARDQ0_DLY_B0                 Fld(4, 0) //[3:0]
    #define SHU4_R0_B0_DQ0_RK0_TX_ARDQ1_DLY_B0                 Fld(4, 4) //[7:4]
    #define SHU4_R0_B0_DQ0_RK0_TX_ARDQ2_DLY_B0                 Fld(4, 8) //[11:8]
    #define SHU4_R0_B0_DQ0_RK0_TX_ARDQ3_DLY_B0                 Fld(4, 12) //[15:12]
    #define SHU4_R0_B0_DQ0_RK0_TX_ARDQ4_DLY_B0                 Fld(4, 16) //[19:16]
    #define SHU4_R0_B0_DQ0_RK0_TX_ARDQ5_DLY_B0                 Fld(4, 20) //[23:20]
    #define SHU4_R0_B0_DQ0_RK0_TX_ARDQ6_DLY_B0                 Fld(4, 24) //[27:24]
    #define SHU4_R0_B0_DQ0_RK0_TX_ARDQ7_DLY_B0                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU4_R0_B0_DQ1                       (DDRPHY_AO_BASE_ADDRESS + 0x01D04)
    #define SHU4_R0_B0_DQ1_RK0_TX_ARDQM0_DLY_B0                Fld(4, 0) //[3:0]
    #define SHU4_R0_B0_DQ1_RK0_TX_ARDQS0_DLYB_B0               Fld(4, 16) //[19:16]
    #define SHU4_R0_B0_DQ1_RK0_TX_ARDQS0B_DLYB_B0              Fld(4, 20) //[23:20]
    #define SHU4_R0_B0_DQ1_RK0_TX_ARDQS0_DLY_B0                Fld(4, 24) //[27:24]
    #define SHU4_R0_B0_DQ1_RK0_TX_ARDQS0B_DLY_B0               Fld(4, 28) //[31:28]

#define DDRPHY_SHU4_R0_B0_DQ2                       (DDRPHY_AO_BASE_ADDRESS + 0x01D08)
    #define SHU4_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU4_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU4_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU4_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R0_B0_DQ3                       (DDRPHY_AO_BASE_ADDRESS + 0x01D0C)
    #define SHU4_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU4_R0_B0_DQ3_RK0_RX_ARDQ2_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU4_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU4_R0_B0_DQ3_RK0_RX_ARDQ3_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R0_B0_DQ4                       (DDRPHY_AO_BASE_ADDRESS + 0x01D10)
    #define SHU4_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU4_R0_B0_DQ4_RK0_RX_ARDQ4_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU4_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU4_R0_B0_DQ4_RK0_RX_ARDQ5_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R0_B0_DQ5                       (DDRPHY_AO_BASE_ADDRESS + 0x01D14)
    #define SHU4_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU4_R0_B0_DQ5_RK0_RX_ARDQ6_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU4_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU4_R0_B0_DQ5_RK0_RX_ARDQ7_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R0_B0_DQ6                       (DDRPHY_AO_BASE_ADDRESS + 0x01D18)
    #define SHU4_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0              Fld(6, 0) //[5:0]
    #define SHU4_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0              Fld(6, 8) //[13:8]
    #define SHU4_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0              Fld(7, 16) //[22:16]
    #define SHU4_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0              Fld(7, 24) //[30:24]

#define DDRPHY_SHU4_R0_B0_DQ7                       (DDRPHY_AO_BASE_ADDRESS + 0x01D1C)
    #define SHU4_R0_B0_DQ7_RK0_ARPI_DQ_B0                      Fld(6, 8) //[13:8]
    #define SHU4_R0_B0_DQ7_RK0_ARPI_DQM_B0                     Fld(6, 16) //[21:16]
    #define SHU4_R0_B0_DQ7_RK0_ARPI_PBYTE_B0                   Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X1D20                           (DDRPHY_AO_BASE_ADDRESS + 0x01D20)
    #define RFU_0X1D20_RESERVED_0X1D20                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1D24                           (DDRPHY_AO_BASE_ADDRESS + 0x01D24)
    #define RFU_0X1D24_RESERVED_0X1D24                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1D28                           (DDRPHY_AO_BASE_ADDRESS + 0x01D28)
    #define RFU_0X1D28_RESERVED_0X1D28                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1D2C                           (DDRPHY_AO_BASE_ADDRESS + 0x01D2C)
    #define RFU_0X1D2C_RESERVED_0X1D2C                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU4_R0_B1_DQ0                       (DDRPHY_AO_BASE_ADDRESS + 0x01D50)
    #define SHU4_R0_B1_DQ0_RK0_TX_ARDQ0_DLY_B1                 Fld(4, 0) //[3:0]
    #define SHU4_R0_B1_DQ0_RK0_TX_ARDQ1_DLY_B1                 Fld(4, 4) //[7:4]
    #define SHU4_R0_B1_DQ0_RK0_TX_ARDQ2_DLY_B1                 Fld(4, 8) //[11:8]
    #define SHU4_R0_B1_DQ0_RK0_TX_ARDQ3_DLY_B1                 Fld(4, 12) //[15:12]
    #define SHU4_R0_B1_DQ0_RK0_TX_ARDQ4_DLY_B1                 Fld(4, 16) //[19:16]
    #define SHU4_R0_B1_DQ0_RK0_TX_ARDQ5_DLY_B1                 Fld(4, 20) //[23:20]
    #define SHU4_R0_B1_DQ0_RK0_TX_ARDQ6_DLY_B1                 Fld(4, 24) //[27:24]
    #define SHU4_R0_B1_DQ0_RK0_TX_ARDQ7_DLY_B1                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU4_R0_B1_DQ1                       (DDRPHY_AO_BASE_ADDRESS + 0x01D54)
    #define SHU4_R0_B1_DQ1_RK0_TX_ARDQM0_DLY_B1                Fld(4, 0) //[3:0]
    #define SHU4_R0_B1_DQ1_RK0_TX_ARDQS0_DLYB_B1               Fld(4, 16) //[19:16]
    #define SHU4_R0_B1_DQ1_RK0_TX_ARDQS0B_DLYB_B1              Fld(4, 20) //[23:20]
    #define SHU4_R0_B1_DQ1_RK0_TX_ARDQS0_DLY_B1                Fld(4, 24) //[27:24]
    #define SHU4_R0_B1_DQ1_RK0_TX_ARDQS0B_DLY_B1               Fld(4, 28) //[31:28]

#define DDRPHY_SHU4_R0_B1_DQ2                       (DDRPHY_AO_BASE_ADDRESS + 0x01D58)
    #define SHU4_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU4_R0_B1_DQ2_RK0_RX_ARDQ0_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU4_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU4_R0_B1_DQ2_RK0_RX_ARDQ1_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R0_B1_DQ3                       (DDRPHY_AO_BASE_ADDRESS + 0x01D5C)
    #define SHU4_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU4_R0_B1_DQ3_RK0_RX_ARDQ2_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU4_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU4_R0_B1_DQ3_RK0_RX_ARDQ3_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R0_B1_DQ4                       (DDRPHY_AO_BASE_ADDRESS + 0x01D60)
    #define SHU4_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU4_R0_B1_DQ4_RK0_RX_ARDQ4_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU4_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU4_R0_B1_DQ4_RK0_RX_ARDQ5_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R0_B1_DQ5                       (DDRPHY_AO_BASE_ADDRESS + 0x01D64)
    #define SHU4_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU4_R0_B1_DQ5_RK0_RX_ARDQ6_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU4_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU4_R0_B1_DQ5_RK0_RX_ARDQ7_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R0_B1_DQ6                       (DDRPHY_AO_BASE_ADDRESS + 0x01D68)
    #define SHU4_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1              Fld(6, 0) //[5:0]
    #define SHU4_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1              Fld(6, 8) //[13:8]
    #define SHU4_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1              Fld(7, 16) //[22:16]
    #define SHU4_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1              Fld(7, 24) //[30:24]

#define DDRPHY_SHU4_R0_B1_DQ7                       (DDRPHY_AO_BASE_ADDRESS + 0x01D6C)
    #define SHU4_R0_B1_DQ7_RK0_ARPI_DQ_B1                      Fld(6, 8) //[13:8]
    #define SHU4_R0_B1_DQ7_RK0_ARPI_DQM_B1                     Fld(6, 16) //[21:16]
    #define SHU4_R0_B1_DQ7_RK0_ARPI_PBYTE_B1                   Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X1D70                           (DDRPHY_AO_BASE_ADDRESS + 0x01D70)
    #define RFU_0X1D70_RESERVED_0X1D70                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1D74                           (DDRPHY_AO_BASE_ADDRESS + 0x01D74)
    #define RFU_0X1D74_RESERVED_0X1D74                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1D78                           (DDRPHY_AO_BASE_ADDRESS + 0x01D78)
    #define RFU_0X1D78_RESERVED_0X1D78                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1D7C                           (DDRPHY_AO_BASE_ADDRESS + 0x01D7C)
    #define RFU_0X1D7C_RESERVED_0X1D7C                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU4_R0_CA_CMD0                      (DDRPHY_AO_BASE_ADDRESS + 0x01DA0)
    #define SHU4_R0_CA_CMD0_RK0_TX_ARCA0_DLY                   Fld(4, 0) //[3:0]
    #define SHU4_R0_CA_CMD0_RK0_TX_ARCA1_DLY                   Fld(4, 4) //[7:4]
    #define SHU4_R0_CA_CMD0_RK0_TX_ARCA2_DLY                   Fld(4, 8) //[11:8]
    #define SHU4_R0_CA_CMD0_RK0_TX_ARCA3_DLY                   Fld(4, 12) //[15:12]
    #define SHU4_R0_CA_CMD0_RK0_TX_ARCA4_DLY                   Fld(4, 16) //[19:16]
    #define SHU4_R0_CA_CMD0_RK0_TX_ARCA5_DLY                   Fld(4, 20) //[23:20]
    #define SHU4_R0_CA_CMD0_RK0_TX_ARCLK_DLYB                  Fld(4, 24) //[27:24]
    #define SHU4_R0_CA_CMD0_RK0_TX_ARCLKB_DLYB                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU4_R0_CA_CMD1                      (DDRPHY_AO_BASE_ADDRESS + 0x01DA4)
    #define SHU4_R0_CA_CMD1_RK0_TX_ARCKE0_DLY                  Fld(4, 0) //[3:0]
    #define SHU4_R0_CA_CMD1_RK0_TX_ARCKE1_DLY                  Fld(4, 4) //[7:4]
    #define SHU4_R0_CA_CMD1_RK0_TX_ARCKE2_DLY                  Fld(4, 8) //[11:8]
    #define SHU4_R0_CA_CMD1_RK0_TX_ARCS0_DLY                   Fld(4, 12) //[15:12]
    #define SHU4_R0_CA_CMD1_RK0_TX_ARCS1_DLY                   Fld(4, 16) //[19:16]
    #define SHU4_R0_CA_CMD1_RK0_TX_ARCS2_DLY                   Fld(4, 20) //[23:20]
    #define SHU4_R0_CA_CMD1_RK0_TX_ARCLK_DLY                   Fld(4, 24) //[27:24]
    #define SHU4_R0_CA_CMD1_RK0_TX_ARCLKB_DLY                  Fld(4, 28) //[31:28]

#define DDRPHY_SHU4_R0_CA_CMD2                      (DDRPHY_AO_BASE_ADDRESS + 0x01DA8)
    #define SHU4_R0_CA_CMD2_RG_RK0_RX_ARCA0_R_DLY              Fld(6, 0) //[5:0]
    #define SHU4_R0_CA_CMD2_RG_RK0_RX_ARCA0_F_DLY              Fld(6, 8) //[13:8]
    #define SHU4_R0_CA_CMD2_RG_RK0_RX_ARCA1_R_DLY              Fld(6, 16) //[21:16]
    #define SHU4_R0_CA_CMD2_RG_RK0_RX_ARCA1_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R0_CA_CMD3                      (DDRPHY_AO_BASE_ADDRESS + 0x01DAC)
    #define SHU4_R0_CA_CMD3_RG_RK0_RX_ARCA2_R_DLY              Fld(6, 0) //[5:0]
    #define SHU4_R0_CA_CMD3_RG_RK0_RX_ARCA2_F_DLY              Fld(6, 8) //[13:8]
    #define SHU4_R0_CA_CMD3_RG_RK0_RX_ARCA3_R_DLY              Fld(6, 16) //[21:16]
    #define SHU4_R0_CA_CMD3_RG_RK0_RX_ARCA3_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R0_CA_CMD4                      (DDRPHY_AO_BASE_ADDRESS + 0x01DB0)
    #define SHU4_R0_CA_CMD4_RG_RK0_RX_ARCA4_R_DLY              Fld(6, 0) //[5:0]
    #define SHU4_R0_CA_CMD4_RG_RK0_RX_ARCA4_F_DLY              Fld(6, 8) //[13:8]
    #define SHU4_R0_CA_CMD4_RG_RK0_RX_ARCA5_R_DLY              Fld(6, 16) //[21:16]
    #define SHU4_R0_CA_CMD4_RG_RK0_RX_ARCA5_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R0_CA_CMD5                      (DDRPHY_AO_BASE_ADDRESS + 0x01DB4)
    #define SHU4_R0_CA_CMD5_RG_RK0_RX_ARCKE0_R_DLY             Fld(6, 0) //[5:0]
    #define SHU4_R0_CA_CMD5_RG_RK0_RX_ARCKE0_F_DLY             Fld(6, 8) //[13:8]
    #define SHU4_R0_CA_CMD5_RG_RK0_RX_ARCKE1_R_DLY             Fld(6, 16) //[21:16]
    #define SHU4_R0_CA_CMD5_RG_RK0_RX_ARCKE1_F_DLY             Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R0_CA_CMD6                      (DDRPHY_AO_BASE_ADDRESS + 0x01DB8)
    #define SHU4_R0_CA_CMD6_RG_RK0_RX_ARCKE2_R_DLY             Fld(6, 0) //[5:0]
    #define SHU4_R0_CA_CMD6_RG_RK0_RX_ARCKE2_F_DLY             Fld(6, 8) //[13:8]
    #define SHU4_R0_CA_CMD6_RG_RK0_RX_ARCS0_R_DLY              Fld(6, 16) //[21:16]
    #define SHU4_R0_CA_CMD6_RG_RK0_RX_ARCS0_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R0_CA_CMD7                      (DDRPHY_AO_BASE_ADDRESS + 0x01DBC)
    #define SHU4_R0_CA_CMD7_RG_RK0_RX_ARCS1_R_DLY              Fld(6, 0) //[5:0]
    #define SHU4_R0_CA_CMD7_RG_RK0_RX_ARCS1_F_DLY              Fld(6, 8) //[13:8]
    #define SHU4_R0_CA_CMD7_RG_RK0_RX_ARCS2_R_DLY              Fld(6, 16) //[21:16]
    #define SHU4_R0_CA_CMD7_RG_RK0_RX_ARCS2_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R0_CA_CMD8                      (DDRPHY_AO_BASE_ADDRESS + 0x01DC0)
    #define SHU4_R0_CA_CMD8_RG_RK0_RX_ARCLK_R_DLY              Fld(7, 16) //[22:16]
    #define SHU4_R0_CA_CMD8_RG_RK0_RX_ARCLK_F_DLY              Fld(7, 24) //[30:24]

#define DDRPHY_SHU4_R0_CA_CMD9                      (DDRPHY_AO_BASE_ADDRESS + 0x01DC4)
    #define SHU4_R0_CA_CMD9_RG_RK0_ARPI_CS                     Fld(6, 0) //[5:0]
    #define SHU4_R0_CA_CMD9_RG_RK0_ARPI_CMD                    Fld(6, 8) //[13:8]
    #define SHU4_R0_CA_CMD9_RG_RK0_ARPI_CLK                    Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X1DC8                           (DDRPHY_AO_BASE_ADDRESS + 0x01DC8)
    #define RFU_0X1DC8_RESERVED_0X1DC8                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1DCC                           (DDRPHY_AO_BASE_ADDRESS + 0x01DCC)
    #define RFU_0X1DCC_RESERVED_0X1DCC                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU4_R1_B0_DQ0                       (DDRPHY_AO_BASE_ADDRESS + 0x01E00)
    #define SHU4_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0                 Fld(4, 0) //[3:0]
    #define SHU4_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0                 Fld(4, 4) //[7:4]
    #define SHU4_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0                 Fld(4, 8) //[11:8]
    #define SHU4_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0                 Fld(4, 12) //[15:12]
    #define SHU4_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0                 Fld(4, 16) //[19:16]
    #define SHU4_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0                 Fld(4, 20) //[23:20]
    #define SHU4_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0                 Fld(4, 24) //[27:24]
    #define SHU4_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU4_R1_B0_DQ1                       (DDRPHY_AO_BASE_ADDRESS + 0x01E04)
    #define SHU4_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0                Fld(4, 0) //[3:0]
    #define SHU4_R1_B0_DQ1_RK1_TX_ARDQS0_DLYB_B0               Fld(4, 16) //[19:16]
    #define SHU4_R1_B0_DQ1_RK1_TX_ARDQS0B_DLYB_B0              Fld(4, 20) //[23:20]
    #define SHU4_R1_B0_DQ1_RK1_TX_ARDQS0_DLY_B0                Fld(4, 24) //[27:24]
    #define SHU4_R1_B0_DQ1_RK1_TX_ARDQS0B_DLY_B0               Fld(4, 28) //[31:28]

#define DDRPHY_SHU4_R1_B0_DQ2                       (DDRPHY_AO_BASE_ADDRESS + 0x01E08)
    #define SHU4_R1_B0_DQ2_RK1_RX_ARDQ0_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU4_R1_B0_DQ2_RK1_RX_ARDQ0_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU4_R1_B0_DQ2_RK1_RX_ARDQ1_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU4_R1_B0_DQ2_RK1_RX_ARDQ1_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R1_B0_DQ3                       (DDRPHY_AO_BASE_ADDRESS + 0x01E0C)
    #define SHU4_R1_B0_DQ3_RK1_RX_ARDQ2_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU4_R1_B0_DQ3_RK1_RX_ARDQ2_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU4_R1_B0_DQ3_RK1_RX_ARDQ3_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU4_R1_B0_DQ3_RK1_RX_ARDQ3_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R1_B0_DQ4                       (DDRPHY_AO_BASE_ADDRESS + 0x01E10)
    #define SHU4_R1_B0_DQ4_RK1_RX_ARDQ4_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU4_R1_B0_DQ4_RK1_RX_ARDQ4_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU4_R1_B0_DQ4_RK1_RX_ARDQ5_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU4_R1_B0_DQ4_RK1_RX_ARDQ5_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R1_B0_DQ5                       (DDRPHY_AO_BASE_ADDRESS + 0x01E14)
    #define SHU4_R1_B0_DQ5_RK1_RX_ARDQ6_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU4_R1_B0_DQ5_RK1_RX_ARDQ6_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU4_R1_B0_DQ5_RK1_RX_ARDQ7_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU4_R1_B0_DQ5_RK1_RX_ARDQ7_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R1_B0_DQ6                       (DDRPHY_AO_BASE_ADDRESS + 0x01E18)
    #define SHU4_R1_B0_DQ6_RK1_RX_ARDQM0_R_DLY_B0              Fld(6, 0) //[5:0]
    #define SHU4_R1_B0_DQ6_RK1_RX_ARDQM0_F_DLY_B0              Fld(6, 8) //[13:8]
    #define SHU4_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0              Fld(7, 16) //[22:16]
    #define SHU4_R1_B0_DQ6_RK1_RX_ARDQS0_F_DLY_B0              Fld(7, 24) //[30:24]

#define DDRPHY_SHU4_R1_B0_DQ7                       (DDRPHY_AO_BASE_ADDRESS + 0x01E1C)
    #define SHU4_R1_B0_DQ7_RK1_ARPI_DQ_B0                      Fld(6, 8) //[13:8]
    #define SHU4_R1_B0_DQ7_RK1_ARPI_DQM_B0                     Fld(6, 16) //[21:16]
    #define SHU4_R1_B0_DQ7_RK1_ARPI_PBYTE_B0                   Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X1E20                           (DDRPHY_AO_BASE_ADDRESS + 0x01E20)
    #define RFU_0X1E20_RESERVED_0X1E20                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1E24                           (DDRPHY_AO_BASE_ADDRESS + 0x01E24)
    #define RFU_0X1E24_RESERVED_0X1E24                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1E28                           (DDRPHY_AO_BASE_ADDRESS + 0x01E28)
    #define RFU_0X1E28_RESERVED_0X1E28                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1E2C                           (DDRPHY_AO_BASE_ADDRESS + 0x01E2C)
    #define RFU_0X1E2C_RESERVED_0X1E2C                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU4_R1_B1_DQ0                       (DDRPHY_AO_BASE_ADDRESS + 0x01E50)
    #define SHU4_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1                 Fld(4, 0) //[3:0]
    #define SHU4_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1                 Fld(4, 4) //[7:4]
    #define SHU4_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1                 Fld(4, 8) //[11:8]
    #define SHU4_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1                 Fld(4, 12) //[15:12]
    #define SHU4_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1                 Fld(4, 16) //[19:16]
    #define SHU4_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1                 Fld(4, 20) //[23:20]
    #define SHU4_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1                 Fld(4, 24) //[27:24]
    #define SHU4_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU4_R1_B1_DQ1                       (DDRPHY_AO_BASE_ADDRESS + 0x01E54)
    #define SHU4_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1                Fld(4, 0) //[3:0]
    #define SHU4_R1_B1_DQ1_RK1_TX_ARDQS0_DLYB_B1               Fld(4, 16) //[19:16]
    #define SHU4_R1_B1_DQ1_RK1_TX_ARDQS0B_DLYB_B1              Fld(4, 20) //[23:20]
    #define SHU4_R1_B1_DQ1_RK1_TX_ARDQS0_DLY_B1                Fld(4, 24) //[27:24]
    #define SHU4_R1_B1_DQ1_RK1_TX_ARDQS0B_DLY_B1               Fld(4, 28) //[31:28]

#define DDRPHY_SHU4_R1_B1_DQ2                       (DDRPHY_AO_BASE_ADDRESS + 0x01E58)
    #define SHU4_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU4_R1_B1_DQ2_RK1_RX_ARDQ0_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU4_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU4_R1_B1_DQ2_RK1_RX_ARDQ1_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R1_B1_DQ3                       (DDRPHY_AO_BASE_ADDRESS + 0x01E5C)
    #define SHU4_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU4_R1_B1_DQ3_RK1_RX_ARDQ2_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU4_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU4_R1_B1_DQ3_RK1_RX_ARDQ3_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R1_B1_DQ4                       (DDRPHY_AO_BASE_ADDRESS + 0x01E60)
    #define SHU4_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU4_R1_B1_DQ4_RK1_RX_ARDQ4_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU4_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU4_R1_B1_DQ4_RK1_RX_ARDQ5_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R1_B1_DQ5                       (DDRPHY_AO_BASE_ADDRESS + 0x01E64)
    #define SHU4_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU4_R1_B1_DQ5_RK1_RX_ARDQ6_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU4_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU4_R1_B1_DQ5_RK1_RX_ARDQ7_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R1_B1_DQ6                       (DDRPHY_AO_BASE_ADDRESS + 0x01E68)
    #define SHU4_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1              Fld(6, 0) //[5:0]
    #define SHU4_R1_B1_DQ6_RK1_RX_ARDQM0_F_DLY_B1              Fld(6, 8) //[13:8]
    #define SHU4_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1              Fld(7, 16) //[22:16]
    #define SHU4_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1              Fld(7, 24) //[30:24]

#define DDRPHY_SHU4_R1_B1_DQ7                       (DDRPHY_AO_BASE_ADDRESS + 0x01E6C)
    #define SHU4_R1_B1_DQ7_RK1_ARPI_DQ_B1                      Fld(6, 8) //[13:8]
    #define SHU4_R1_B1_DQ7_RK1_ARPI_DQM_B1                     Fld(6, 16) //[21:16]
    #define SHU4_R1_B1_DQ7_RK1_ARPI_PBYTE_B1                   Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X1E70                           (DDRPHY_AO_BASE_ADDRESS + 0x01E70)
    #define RFU_0X1E70_RESERVED_0X1E70                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1E74                           (DDRPHY_AO_BASE_ADDRESS + 0x01E74)
    #define RFU_0X1E74_RESERVED_0X1E74                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1E78                           (DDRPHY_AO_BASE_ADDRESS + 0x01E78)
    #define RFU_0X1E78_RESERVED_0X1E78                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1E7C                           (DDRPHY_AO_BASE_ADDRESS + 0x01E7C)
    #define RFU_0X1E7C_RESERVED_0X1E7C                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU4_R1_CA_CMD0                      (DDRPHY_AO_BASE_ADDRESS + 0x01EA0)
    #define SHU4_R1_CA_CMD0_RK1_TX_ARCA0_DLY                   Fld(4, 0) //[3:0]
    #define SHU4_R1_CA_CMD0_RK1_TX_ARCA1_DLY                   Fld(4, 4) //[7:4]
    #define SHU4_R1_CA_CMD0_RK1_TX_ARCA2_DLY                   Fld(4, 8) //[11:8]
    #define SHU4_R1_CA_CMD0_RK1_TX_ARCA3_DLY                   Fld(4, 12) //[15:12]
    #define SHU4_R1_CA_CMD0_RK1_TX_ARCA4_DLY                   Fld(4, 16) //[19:16]
    #define SHU4_R1_CA_CMD0_RK1_TX_ARCA5_DLY                   Fld(4, 20) //[23:20]
    #define SHU4_R1_CA_CMD0_RK1_TX_ARCLK_DLYB                  Fld(4, 24) //[27:24]
    #define SHU4_R1_CA_CMD0_RK1_TX_ARCLKB_DLYB                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU4_R1_CA_CMD1                      (DDRPHY_AO_BASE_ADDRESS + 0x01EA4)
    #define SHU4_R1_CA_CMD1_RK1_TX_ARCKE0_DLY                  Fld(4, 0) //[3:0]
    #define SHU4_R1_CA_CMD1_RK1_TX_ARCKE1_DLY                  Fld(4, 4) //[7:4]
    #define SHU4_R1_CA_CMD1_RK1_TX_ARCKE2_DLY                  Fld(4, 8) //[11:8]
    #define SHU4_R1_CA_CMD1_RK1_TX_ARCS0_DLY                   Fld(4, 12) //[15:12]
    #define SHU4_R1_CA_CMD1_RK1_TX_ARCS1_DLY                   Fld(4, 16) //[19:16]
    #define SHU4_R1_CA_CMD1_RK1_TX_ARCS2_DLY                   Fld(4, 20) //[23:20]
    #define SHU4_R1_CA_CMD1_RK1_TX_ARCLK_DLY                   Fld(4, 24) //[27:24]
    #define SHU4_R1_CA_CMD1_RK1_TX_ARCLKB_DLY                  Fld(4, 28) //[31:28]

#define DDRPHY_SHU4_R1_CA_CMD2                      (DDRPHY_AO_BASE_ADDRESS + 0x01EA8)
    #define SHU4_R1_CA_CMD2_RG_RK1_RX_ARCA0_R_DLY              Fld(6, 0) //[5:0]
    #define SHU4_R1_CA_CMD2_RG_RK1_RX_ARCA0_F_DLY              Fld(6, 8) //[13:8]
    #define SHU4_R1_CA_CMD2_RG_RK1_RX_ARCA1_R_DLY              Fld(6, 16) //[21:16]
    #define SHU4_R1_CA_CMD2_RG_RK1_RX_ARCA1_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R1_CA_CMD3                      (DDRPHY_AO_BASE_ADDRESS + 0x01EAC)
    #define SHU4_R1_CA_CMD3_RG_RK1_RX_ARCA2_R_DLY              Fld(6, 0) //[5:0]
    #define SHU4_R1_CA_CMD3_RG_RK1_RX_ARCA2_F_DLY              Fld(6, 8) //[13:8]
    #define SHU4_R1_CA_CMD3_RG_RK1_RX_ARCA3_R_DLY              Fld(6, 16) //[21:16]
    #define SHU4_R1_CA_CMD3_RG_RK1_RX_ARCA3_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R1_CA_CMD4                      (DDRPHY_AO_BASE_ADDRESS + 0x01EB0)
    #define SHU4_R1_CA_CMD4_RG_RK1_RX_ARCA4_R_DLY              Fld(6, 0) //[5:0]
    #define SHU4_R1_CA_CMD4_RG_RK1_RX_ARCA4_F_DLY              Fld(6, 8) //[13:8]
    #define SHU4_R1_CA_CMD4_RG_RK1_RX_ARCA5_R_DLY              Fld(6, 16) //[21:16]
    #define SHU4_R1_CA_CMD4_RG_RK1_RX_ARCA5_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R1_CA_CMD5                      (DDRPHY_AO_BASE_ADDRESS + 0x01EB4)
    #define SHU4_R1_CA_CMD5_RG_RK1_RX_ARCKE0_R_DLY             Fld(6, 0) //[5:0]
    #define SHU4_R1_CA_CMD5_RG_RK1_RX_ARCKE0_F_DLY             Fld(6, 8) //[13:8]
    #define SHU4_R1_CA_CMD5_RG_RK1_RX_ARCKE1_R_DLY             Fld(6, 16) //[21:16]
    #define SHU4_R1_CA_CMD5_RG_RK1_RX_ARCKE1_F_DLY             Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R1_CA_CMD6                      (DDRPHY_AO_BASE_ADDRESS + 0x01EB8)
    #define SHU4_R1_CA_CMD6_RG_RK1_RX_ARCKE2_R_DLY             Fld(6, 0) //[5:0]
    #define SHU4_R1_CA_CMD6_RG_RK1_RX_ARCKE2_F_DLY             Fld(6, 8) //[13:8]
    #define SHU4_R1_CA_CMD6_RG_RK1_RX_ARCS0_R_DLY              Fld(6, 16) //[21:16]
    #define SHU4_R1_CA_CMD6_RG_RK1_RX_ARCS0_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R1_CA_CMD7                      (DDRPHY_AO_BASE_ADDRESS + 0x01EBC)
    #define SHU4_R1_CA_CMD7_RG_RK1_RX_ARCS1_R_DLY              Fld(6, 0) //[5:0]
    #define SHU4_R1_CA_CMD7_RG_RK1_RX_ARCS1_F_DLY              Fld(6, 8) //[13:8]
    #define SHU4_R1_CA_CMD7_RG_RK1_RX_ARCS2_R_DLY              Fld(6, 16) //[21:16]
    #define SHU4_R1_CA_CMD7_RG_RK1_RX_ARCS2_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R1_CA_CMD8                      (DDRPHY_AO_BASE_ADDRESS + 0x01EC0)
    #define SHU4_R1_CA_CMD8_RG_RK1_RX_ARCLK_R_DLY              Fld(7, 16) //[22:16]
    #define SHU4_R1_CA_CMD8_RG_RK1_RX_ARCLK_F_DLY              Fld(7, 24) //[30:24]

#define DDRPHY_SHU4_R1_CA_CMD9                      (DDRPHY_AO_BASE_ADDRESS + 0x01EC4)
    #define SHU4_R1_CA_CMD9_RG_RK1_ARPI_CS                     Fld(6, 0) //[5:0]
    #define SHU4_R1_CA_CMD9_RG_RK1_ARPI_CMD                    Fld(6, 8) //[13:8]
    #define SHU4_R1_CA_CMD9_RG_RK1_ARPI_CLK                    Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X1EC8                           (DDRPHY_AO_BASE_ADDRESS + 0x01EC8)
    #define RFU_0X1EC8_RESERVED_0X1EC8                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1ECC                           (DDRPHY_AO_BASE_ADDRESS + 0x01ECC)
    #define RFU_0X1ECC_RESERVED_0X1ECC                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU4_R2_B0_DQ0                       (DDRPHY_AO_BASE_ADDRESS + 0x01F00)
    #define SHU4_R2_B0_DQ0_RK2_TX_ARDQ0_DLY_B0                 Fld(4, 0) //[3:0]
    #define SHU4_R2_B0_DQ0_RK2_TX_ARDQ1_DLY_B0                 Fld(4, 4) //[7:4]
    #define SHU4_R2_B0_DQ0_RK2_TX_ARDQ2_DLY_B0                 Fld(4, 8) //[11:8]
    #define SHU4_R2_B0_DQ0_RK2_TX_ARDQ3_DLY_B0                 Fld(4, 12) //[15:12]
    #define SHU4_R2_B0_DQ0_RK2_TX_ARDQ4_DLY_B0                 Fld(4, 16) //[19:16]
    #define SHU4_R2_B0_DQ0_RK2_TX_ARDQ5_DLY_B0                 Fld(4, 20) //[23:20]
    #define SHU4_R2_B0_DQ0_RK2_TX_ARDQ6_DLY_B0                 Fld(4, 24) //[27:24]
    #define SHU4_R2_B0_DQ0_RK2_TX_ARDQ7_DLY_B0                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU4_R2_B0_DQ1                       (DDRPHY_AO_BASE_ADDRESS + 0x01F04)
    #define SHU4_R2_B0_DQ1_RK2_TX_ARDQM0_DLY_B0                Fld(4, 0) //[3:0]
    #define SHU4_R2_B0_DQ1_RK2_TX_ARDQS0_DLYB_B0               Fld(4, 16) //[19:16]
    #define SHU4_R2_B0_DQ1_RK2_TX_ARDQS0B_DLYB_B0              Fld(4, 20) //[23:20]
    #define SHU4_R2_B0_DQ1_RK2_TX_ARDQS0_DLY_B0                Fld(4, 24) //[27:24]
    #define SHU4_R2_B0_DQ1_RK2_TX_ARDQS0B_DLY_B0               Fld(4, 28) //[31:28]

#define DDRPHY_SHU4_R2_B0_DQ2                       (DDRPHY_AO_BASE_ADDRESS + 0x01F08)
    #define SHU4_R2_B0_DQ2_RK2_RX_ARDQ0_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU4_R2_B0_DQ2_RK2_RX_ARDQ0_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU4_R2_B0_DQ2_RK2_RX_ARDQ1_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU4_R2_B0_DQ2_RK2_RX_ARDQ1_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R2_B0_DQ3                       (DDRPHY_AO_BASE_ADDRESS + 0x01F0C)
    #define SHU4_R2_B0_DQ3_RK2_RX_ARDQ2_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU4_R2_B0_DQ3_RK2_RX_ARDQ2_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU4_R2_B0_DQ3_RK2_RX_ARDQ3_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU4_R2_B0_DQ3_RK2_RX_ARDQ3_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R2_B0_DQ4                       (DDRPHY_AO_BASE_ADDRESS + 0x01F10)
    #define SHU4_R2_B0_DQ4_RK2_RX_ARDQ4_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU4_R2_B0_DQ4_RK2_RX_ARDQ4_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU4_R2_B0_DQ4_RK2_RX_ARDQ5_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU4_R2_B0_DQ4_RK2_RX_ARDQ5_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R2_B0_DQ5                       (DDRPHY_AO_BASE_ADDRESS + 0x01F14)
    #define SHU4_R2_B0_DQ5_RK2_RX_ARDQ6_R_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU4_R2_B0_DQ5_RK2_RX_ARDQ6_F_DLY_B0               Fld(6, 8) //[13:8]
    #define SHU4_R2_B0_DQ5_RK2_RX_ARDQ7_R_DLY_B0               Fld(6, 16) //[21:16]
    #define SHU4_R2_B0_DQ5_RK2_RX_ARDQ7_F_DLY_B0               Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R2_B0_DQ6                       (DDRPHY_AO_BASE_ADDRESS + 0x01F18)
    #define SHU4_R2_B0_DQ6_RK2_RX_ARDQM0_R_DLY_B0              Fld(6, 0) //[5:0]
    #define SHU4_R2_B0_DQ6_RK2_RX_ARDQM0_F_DLY_B0              Fld(6, 8) //[13:8]
    #define SHU4_R2_B0_DQ6_RK2_RX_ARDQS0_R_DLY_B0              Fld(7, 16) //[22:16]
    #define SHU4_R2_B0_DQ6_RK2_RX_ARDQS0_F_DLY_B0              Fld(7, 24) //[30:24]

#define DDRPHY_SHU4_R2_B0_DQ7                       (DDRPHY_AO_BASE_ADDRESS + 0x01F1C)
    #define SHU4_R2_B0_DQ7_RK2_ARPI_DQ_B0                      Fld(6, 8) //[13:8]
    #define SHU4_R2_B0_DQ7_RK2_ARPI_DQM_B0                     Fld(6, 16) //[21:16]
    #define SHU4_R2_B0_DQ7_RK2_ARPI_PBYTE_B0                   Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X1F20                           (DDRPHY_AO_BASE_ADDRESS + 0x01F20)
    #define RFU_0X1F20_RESERVED_0X1F20                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1F24                           (DDRPHY_AO_BASE_ADDRESS + 0x01F24)
    #define RFU_0X1F24_RESERVED_0X1F24                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1F28                           (DDRPHY_AO_BASE_ADDRESS + 0x01F28)
    #define RFU_0X1F28_RESERVED_0X1F28                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1F2C                           (DDRPHY_AO_BASE_ADDRESS + 0x01F2C)
    #define RFU_0X1F2C_RESERVED_0X1F2C                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU4_R2_B1_DQ0                       (DDRPHY_AO_BASE_ADDRESS + 0x01F50)
    #define SHU4_R2_B1_DQ0_RK2_TX_ARDQ0_DLY_B1                 Fld(4, 0) //[3:0]
    #define SHU4_R2_B1_DQ0_RK2_TX_ARDQ1_DLY_B1                 Fld(4, 4) //[7:4]
    #define SHU4_R2_B1_DQ0_RK2_TX_ARDQ2_DLY_B1                 Fld(4, 8) //[11:8]
    #define SHU4_R2_B1_DQ0_RK2_TX_ARDQ3_DLY_B1                 Fld(4, 12) //[15:12]
    #define SHU4_R2_B1_DQ0_RK2_TX_ARDQ4_DLY_B1                 Fld(4, 16) //[19:16]
    #define SHU4_R2_B1_DQ0_RK2_TX_ARDQ5_DLY_B1                 Fld(4, 20) //[23:20]
    #define SHU4_R2_B1_DQ0_RK2_TX_ARDQ6_DLY_B1                 Fld(4, 24) //[27:24]
    #define SHU4_R2_B1_DQ0_RK2_TX_ARDQ7_DLY_B1                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU4_R2_B1_DQ1                       (DDRPHY_AO_BASE_ADDRESS + 0x01F54)
    #define SHU4_R2_B1_DQ1_RK2_TX_ARDQM0_DLY_B1                Fld(4, 0) //[3:0]
    #define SHU4_R2_B1_DQ1_RK2_TX_ARDQS0_DLYB_B1               Fld(4, 16) //[19:16]
    #define SHU4_R2_B1_DQ1_RK2_TX_ARDQS0B_DLYB_B1              Fld(4, 20) //[23:20]
    #define SHU4_R2_B1_DQ1_RK2_TX_ARDQS0_DLY_B1                Fld(4, 24) //[27:24]
    #define SHU4_R2_B1_DQ1_RK2_TX_ARDQS0B_DLY_B1               Fld(4, 28) //[31:28]

#define DDRPHY_SHU4_R2_B1_DQ2                       (DDRPHY_AO_BASE_ADDRESS + 0x01F58)
    #define SHU4_R2_B1_DQ2_RK2_RX_ARDQ0_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU4_R2_B1_DQ2_RK2_RX_ARDQ0_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU4_R2_B1_DQ2_RK2_RX_ARDQ1_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU4_R2_B1_DQ2_RK2_RX_ARDQ1_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R2_B1_DQ3                       (DDRPHY_AO_BASE_ADDRESS + 0x01F5C)
    #define SHU4_R2_B1_DQ3_RK2_RX_ARDQ2_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU4_R2_B1_DQ3_RK2_RX_ARDQ2_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU4_R2_B1_DQ3_RK2_RX_ARDQ3_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU4_R2_B1_DQ3_RK2_RX_ARDQ3_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R2_B1_DQ4                       (DDRPHY_AO_BASE_ADDRESS + 0x01F60)
    #define SHU4_R2_B1_DQ4_RK2_RX_ARDQ4_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU4_R2_B1_DQ4_RK2_RX_ARDQ4_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU4_R2_B1_DQ4_RK2_RX_ARDQ5_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU4_R2_B1_DQ4_RK2_RX_ARDQ5_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R2_B1_DQ5                       (DDRPHY_AO_BASE_ADDRESS + 0x01F64)
    #define SHU4_R2_B1_DQ5_RK2_RX_ARDQ6_R_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU4_R2_B1_DQ5_RK2_RX_ARDQ6_F_DLY_B1               Fld(6, 8) //[13:8]
    #define SHU4_R2_B1_DQ5_RK2_RX_ARDQ7_R_DLY_B1               Fld(6, 16) //[21:16]
    #define SHU4_R2_B1_DQ5_RK2_RX_ARDQ7_F_DLY_B1               Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R2_B1_DQ6                       (DDRPHY_AO_BASE_ADDRESS + 0x01F68)
    #define SHU4_R2_B1_DQ6_RK2_RX_ARDQM0_R_DLY_B1              Fld(6, 0) //[5:0]
    #define SHU4_R2_B1_DQ6_RK2_RX_ARDQM0_F_DLY_B1              Fld(6, 8) //[13:8]
    #define SHU4_R2_B1_DQ6_RK2_RX_ARDQS0_R_DLY_B1              Fld(7, 16) //[22:16]
    #define SHU4_R2_B1_DQ6_RK2_RX_ARDQS0_F_DLY_B1              Fld(7, 24) //[30:24]

#define DDRPHY_SHU4_R2_B1_DQ7                       (DDRPHY_AO_BASE_ADDRESS + 0x01F6C)
    #define SHU4_R2_B1_DQ7_RK2_ARPI_DQ_B1                      Fld(6, 8) //[13:8]
    #define SHU4_R2_B1_DQ7_RK2_ARPI_DQM_B1                     Fld(6, 16) //[21:16]
    #define SHU4_R2_B1_DQ7_RK2_ARPI_PBYTE_B1                   Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X1F70                           (DDRPHY_AO_BASE_ADDRESS + 0x01F70)
    #define RFU_0X1F70_RESERVED_0X1F70                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1F74                           (DDRPHY_AO_BASE_ADDRESS + 0x01F74)
    #define RFU_0X1F74_RESERVED_0X1F74                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1F78                           (DDRPHY_AO_BASE_ADDRESS + 0x01F78)
    #define RFU_0X1F78_RESERVED_0X1F78                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1F7C                           (DDRPHY_AO_BASE_ADDRESS + 0x01F7C)
    #define RFU_0X1F7C_RESERVED_0X1F7C                         Fld(32, 0) //[31:0]

#define DDRPHY_SHU4_R2_CA_CMD0                      (DDRPHY_AO_BASE_ADDRESS + 0x01FA0)
    #define SHU4_R2_CA_CMD0_RK2_TX_ARCA0_DLY                   Fld(4, 0) //[3:0]
    #define SHU4_R2_CA_CMD0_RK2_TX_ARCA1_DLY                   Fld(4, 4) //[7:4]
    #define SHU4_R2_CA_CMD0_RK2_TX_ARCA2_DLY                   Fld(4, 8) //[11:8]
    #define SHU4_R2_CA_CMD0_RK2_TX_ARCA3_DLY                   Fld(4, 12) //[15:12]
    #define SHU4_R2_CA_CMD0_RK2_TX_ARCA4_DLY                   Fld(4, 16) //[19:16]
    #define SHU4_R2_CA_CMD0_RK2_TX_ARCA5_DLY                   Fld(4, 20) //[23:20]
    #define SHU4_R2_CA_CMD0_RK2_TX_ARCLK_DLYB                  Fld(4, 24) //[27:24]
    #define SHU4_R2_CA_CMD0_RK2_TX_ARCLKB_DLYB                 Fld(4, 28) //[31:28]

#define DDRPHY_SHU4_R2_CA_CMD1                      (DDRPHY_AO_BASE_ADDRESS + 0x01FA4)
    #define SHU4_R2_CA_CMD1_RK2_TX_ARCKE0_DLY                  Fld(4, 0) //[3:0]
    #define SHU4_R2_CA_CMD1_RK2_TX_ARCKE1_DLY                  Fld(4, 4) //[7:4]
    #define SHU4_R2_CA_CMD1_RK2_TX_ARCKE2_DLY                  Fld(4, 8) //[11:8]
    #define SHU4_R2_CA_CMD1_RK2_TX_ARCS0_DLY                   Fld(4, 12) //[15:12]
    #define SHU4_R2_CA_CMD1_RK2_TX_ARCS1_DLY                   Fld(4, 16) //[19:16]
    #define SHU4_R2_CA_CMD1_RK2_TX_ARCS2_DLY                   Fld(4, 20) //[23:20]
    #define SHU4_R2_CA_CMD1_RK2_TX_ARCLK_DLY                   Fld(4, 24) //[27:24]
    #define SHU4_R2_CA_CMD1_RK2_TX_ARCLKB_DLY                  Fld(4, 28) //[31:28]

#define DDRPHY_SHU4_R2_CA_CMD2                      (DDRPHY_AO_BASE_ADDRESS + 0x01FA8)
    #define SHU4_R2_CA_CMD2_RG_RK2_RX_ARCA0_R_DLY              Fld(6, 0) //[5:0]
    #define SHU4_R2_CA_CMD2_RG_RK2_RX_ARCA0_F_DLY              Fld(6, 8) //[13:8]
    #define SHU4_R2_CA_CMD2_RG_RK2_RX_ARCA1_R_DLY              Fld(6, 16) //[21:16]
    #define SHU4_R2_CA_CMD2_RG_RK2_RX_ARCA1_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R2_CA_CMD3                      (DDRPHY_AO_BASE_ADDRESS + 0x01FAC)
    #define SHU4_R2_CA_CMD3_RG_RK2_RX_ARCA2_R_DLY              Fld(6, 0) //[5:0]
    #define SHU4_R2_CA_CMD3_RG_RK2_RX_ARCA2_F_DLY              Fld(6, 8) //[13:8]
    #define SHU4_R2_CA_CMD3_RG_RK2_RX_ARCA3_R_DLY              Fld(6, 16) //[21:16]
    #define SHU4_R2_CA_CMD3_RG_RK2_RX_ARCA3_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R2_CA_CMD4                      (DDRPHY_AO_BASE_ADDRESS + 0x01FB0)
    #define SHU4_R2_CA_CMD4_RG_RK2_RX_ARCA4_R_DLY              Fld(6, 0) //[5:0]
    #define SHU4_R2_CA_CMD4_RG_RK2_RX_ARCA4_F_DLY              Fld(6, 8) //[13:8]
    #define SHU4_R2_CA_CMD4_RG_RK2_RX_ARCA5_R_DLY              Fld(6, 16) //[21:16]
    #define SHU4_R2_CA_CMD4_RG_RK2_RX_ARCA5_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R2_CA_CMD5                      (DDRPHY_AO_BASE_ADDRESS + 0x01FB4)
    #define SHU4_R2_CA_CMD5_RG_RK2_RX_ARCKE0_R_DLY             Fld(6, 0) //[5:0]
    #define SHU4_R2_CA_CMD5_RG_RK2_RX_ARCKE0_F_DLY             Fld(6, 8) //[13:8]
    #define SHU4_R2_CA_CMD5_RG_RK2_RX_ARCKE1_R_DLY             Fld(6, 16) //[21:16]
    #define SHU4_R2_CA_CMD5_RG_RK2_RX_ARCKE1_F_DLY             Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R2_CA_CMD6                      (DDRPHY_AO_BASE_ADDRESS + 0x01FB8)
    #define SHU4_R2_CA_CMD6_RG_RK2_RX_ARCKE2_R_DLY             Fld(6, 0) //[5:0]
    #define SHU4_R2_CA_CMD6_RG_RK2_RX_ARCKE2_F_DLY             Fld(6, 8) //[13:8]
    #define SHU4_R2_CA_CMD6_RG_RK2_RX_ARCS0_R_DLY              Fld(6, 16) //[21:16]
    #define SHU4_R2_CA_CMD6_RG_RK2_RX_ARCS0_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R2_CA_CMD7                      (DDRPHY_AO_BASE_ADDRESS + 0x01FBC)
    #define SHU4_R2_CA_CMD7_RG_RK2_RX_ARCS1_R_DLY              Fld(6, 0) //[5:0]
    #define SHU4_R2_CA_CMD7_RG_RK2_RX_ARCS1_F_DLY              Fld(6, 8) //[13:8]
    #define SHU4_R2_CA_CMD7_RG_RK2_RX_ARCS2_R_DLY              Fld(6, 16) //[21:16]
    #define SHU4_R2_CA_CMD7_RG_RK2_RX_ARCS2_F_DLY              Fld(6, 24) //[29:24]

#define DDRPHY_SHU4_R2_CA_CMD8                      (DDRPHY_AO_BASE_ADDRESS + 0x01FC0)
    #define SHU4_R2_CA_CMD8_RG_RK2_RX_ARCLK_R_DLY              Fld(7, 16) //[22:16]
    #define SHU4_R2_CA_CMD8_RG_RK2_RX_ARCLK_F_DLY              Fld(7, 24) //[30:24]

#define DDRPHY_SHU4_R2_CA_CMD9                      (DDRPHY_AO_BASE_ADDRESS + 0x01FC4)
    #define SHU4_R2_CA_CMD9_RG_RK2_ARPI_CS                     Fld(6, 0) //[5:0]
    #define SHU4_R2_CA_CMD9_RG_RK2_ARPI_CMD                    Fld(6, 8) //[13:8]
    #define SHU4_R2_CA_CMD9_RG_RK2_ARPI_CLK                    Fld(6, 24) //[29:24]

#define DDRPHY_RFU_0X1FC8                           (DDRPHY_AO_BASE_ADDRESS + 0x01FC8)
    #define RFU_0X1FC8_RESERVED_0X1FC8                         Fld(32, 0) //[31:0]

#define DDRPHY_RFU_0X1FCC                           (DDRPHY_AO_BASE_ADDRESS + 0x01FCC)
    #define RFU_0X1FCC_RESERVED_0X1FCC                         Fld(32, 0) //[31:0]

/*--------------------------------------------------
 * Page DDRPHY_NAO
 *--------------------------------------------------
 */
#define DDRPHY_NAO_BASE_ADDRESS Channel_A_PHY_NAO_BASE_VIRTUAL

#define DDRPHY_MISC_STA_EXTLB0                         (DDRPHY_NAO_BASE_ADDRESS + 0x00000)
    #define MISC_STA_EXTLB0_STA_EXTLB_DONE                     Fld(32, 0) //[31:0]

#define DDRPHY_MISC_STA_EXTLB1                         (DDRPHY_NAO_BASE_ADDRESS + 0x00004)
    #define MISC_STA_EXTLB1_STA_EXTLB_FAIL                     Fld(32, 0) //[31:0]

#define DDRPHY_MISC_STA_EXTLB2                         (DDRPHY_NAO_BASE_ADDRESS + 0x00008)
    #define MISC_STA_EXTLB2_STA_EXTLB_DBG_INFO                 Fld(32, 0) //[31:0]

#define DDRPHY_MISC_DQ_RXDLY_TRRO0                     (DDRPHY_NAO_BASE_ADDRESS + 0x00080)
    #define MISC_DQ_RXDLY_TRRO0_DVS_RKX_BX_SW_LAG_CNT_OUT_B0   Fld(8, 0) //[7:0]
    #define MISC_DQ_RXDLY_TRRO0_DVS_RKX_BX_SW_LEAD_CNT_OUT_B0  Fld(8, 8) //[15:8]
    #define MISC_DQ_RXDLY_TRRO0_DVS_RKX_BX_SW_LAG_CNT_OUT_B1   Fld(8, 16) //[23:16]
    #define MISC_DQ_RXDLY_TRRO0_DVS_RKX_BX_SW_LEAD_CNT_OUT_B1  Fld(8, 24) //[31:24]

#define DDRPHY_MISC_DQ_RXDLY_TRRO1                     (DDRPHY_NAO_BASE_ADDRESS + 0x00084)
    #define MISC_DQ_RXDLY_TRRO1_DVS_RKX_BX_SW_LAG_CNT_OUT_B2   Fld(8, 0) //[7:0]
    #define MISC_DQ_RXDLY_TRRO1_DVS_RKX_BX_SW_LEAD_CNT_OUT_B2  Fld(8, 8) //[15:8]
    #define MISC_DQ_RXDLY_TRRO1_DVS_RKX_BX_SW_LAG_CNT_OUT_B3   Fld(8, 16) //[23:16]
    #define MISC_DQ_RXDLY_TRRO1_DVS_RKX_BX_SW_LEAD_CNT_OUT_B3  Fld(8, 24) //[31:24]

#define DDRPHY_MISC_DQ_RXDLY_TRRO2                     (DDRPHY_NAO_BASE_ADDRESS + 0x00088)
    #define MISC_DQ_RXDLY_TRRO2_DVS_RKX_BX_SW_LAG_CNT_OUT_B4   Fld(8, 0) //[7:0]
    #define MISC_DQ_RXDLY_TRRO2_DVS_RKX_BX_SW_LEAD_CNT_OUT_B4  Fld(8, 8) //[15:8]
    #define MISC_DQ_RXDLY_TRRO2_DVS_RKX_BX_SW_LAG_CNT_OUT_B5   Fld(8, 16) //[23:16]
    #define MISC_DQ_RXDLY_TRRO2_DVS_RKX_BX_SW_LEAD_CNT_OUT_B5  Fld(8, 24) //[31:24]

#define DDRPHY_MISC_DQ_RXDLY_TRRO3                     (DDRPHY_NAO_BASE_ADDRESS + 0x0008C)
    #define MISC_DQ_RXDLY_TRRO3_DVS_RKX_BX_SW_LAG_CNT_OUT_B6   Fld(8, 0) //[7:0]
    #define MISC_DQ_RXDLY_TRRO3_DVS_RKX_BX_SW_LEAD_CNT_OUT_B6  Fld(8, 8) //[15:8]
    #define MISC_DQ_RXDLY_TRRO3_DVS_RKX_BX_SW_LAG_CNT_OUT_B7   Fld(8, 16) //[23:16]
    #define MISC_DQ_RXDLY_TRRO3_DVS_RKX_BX_SW_LEAD_CNT_OUT_B7  Fld(8, 24) //[31:24]

#define DDRPHY_MISC_DQ_RXDLY_TRRO4                     (DDRPHY_NAO_BASE_ADDRESS + 0x00090)
    #define MISC_DQ_RXDLY_TRRO4_DVS_RKX_BX_LEAD_LAG_CNT_OUT_B0 Fld(8, 0) //[7:0]
    #define MISC_DQ_RXDLY_TRRO4_DVS_RKX_BX_LEAD_LAG_CNT_OUT_B1 Fld(8, 8) //[15:8]
    #define MISC_DQ_RXDLY_TRRO4_DVS_RKX_BX_LEAD_LAG_CNT_OUT_B2 Fld(8, 16) //[23:16]
    #define MISC_DQ_RXDLY_TRRO4_DVS_RKX_BX_LEAD_LAG_CNT_OUT_B3 Fld(8, 24) //[31:24]

#define DDRPHY_MISC_DQ_RXDLY_TRRO5                     (DDRPHY_NAO_BASE_ADDRESS + 0x00094)
    #define MISC_DQ_RXDLY_TRRO5_DVS_RKX_BX_LEAD_LAG_CNT_OUT_B4 Fld(8, 0) //[7:0]
    #define MISC_DQ_RXDLY_TRRO5_DVS_RKX_BX_LEAD_LAG_CNT_OUT_B5 Fld(8, 8) //[15:8]
    #define MISC_DQ_RXDLY_TRRO5_DVS_RKX_BX_LEAD_LAG_CNT_OUT_B6 Fld(8, 16) //[23:16]
    #define MISC_DQ_RXDLY_TRRO5_DVS_RKX_BX_LEAD_LAG_CNT_OUT_B7 Fld(8, 24) //[31:24]

#define DDRPHY_MISC_DQ_RXDLY_TRRO6                     (DDRPHY_NAO_BASE_ADDRESS + 0x00098)
    #define MISC_DQ_RXDLY_TRRO6_DVS_RKX_BX_SW_LAG_CNT_OUT_DQM0 Fld(8, 0) //[7:0]
    #define MISC_DQ_RXDLY_TRRO6_DVS_RKX_BX_SW_LEAD_CNT_OUT_DQM0 Fld(8, 8) //[15:8]
    #define MISC_DQ_RXDLY_TRRO6_DVS_RKX_BX_LEAD_LAG_CNT_OUT_DQM0 Fld(8, 24) //[31:24]

#define DDRPHY_MISC_DQ_RXDLY_TRRO7                     (DDRPHY_NAO_BASE_ADDRESS + 0x0009C)
    #define MISC_DQ_RXDLY_TRRO7_DVS_RK0_B0_SW_UP_DONE          Fld(1, 0) //[0:0]
    #define MISC_DQ_RXDLY_TRRO7_DVS_RK0_B1_SW_UP_DONE          Fld(1, 4) //[4:4]
    #define MISC_DQ_RXDLY_TRRO7_DVS_RK1_B0_SW_UP_DONE          Fld(1, 8) //[8:8]
    #define MISC_DQ_RXDLY_TRRO7_DVS_RK1_B1_SW_UP_DONE          Fld(1, 12) //[12:12]
    #define MISC_DQ_RXDLY_TRRO7_DVS_RK2_B0_SW_UP_DONE          Fld(1, 16) //[16:16]
    #define MISC_DQ_RXDLY_TRRO7_DVS_RK2_B1_SW_UP_DONE          Fld(1, 20) //[20:20]

#define DDRPHY_MISC_DQ_RXDLY_TRRO8                     (DDRPHY_NAO_BASE_ADDRESS + 0x000A0)
    #define MISC_DQ_RXDLY_TRRO8_DVS_RKX_BX_TH_CNT_OUT_B0       Fld(9, 0) //[8:0]
    #define MISC_DQ_RXDLY_TRRO8_DVS_RKX_BX_TH_CNT_OUT_B1       Fld(9, 16) //[24:16]

#define DDRPHY_MISC_DQ_RXDLY_TRRO9                     (DDRPHY_NAO_BASE_ADDRESS + 0x000A4)
    #define MISC_DQ_RXDLY_TRRO9_DVS_RKX_BX_TH_CNT_OUT_B2       Fld(9, 0) //[8:0]
    #define MISC_DQ_RXDLY_TRRO9_DVS_RKX_BX_TH_CNT_OUT_B3       Fld(9, 16) //[24:16]

#define DDRPHY_MISC_DQ_RXDLY_TRRO10                    (DDRPHY_NAO_BASE_ADDRESS + 0x000A8)
    #define MISC_DQ_RXDLY_TRRO10_DVS_RKX_BX_TH_CNT_OUT_B4      Fld(9, 0) //[8:0]
    #define MISC_DQ_RXDLY_TRRO10_DVS_RKX_BX_TH_CNT_OUT_B5      Fld(9, 16) //[24:16]

#define DDRPHY_MISC_DQ_RXDLY_TRRO11                    (DDRPHY_NAO_BASE_ADDRESS + 0x000AC)
    #define MISC_DQ_RXDLY_TRRO11_DVS_RKX_BX_TH_CNT_OUT_B6      Fld(9, 0) //[8:0]
    #define MISC_DQ_RXDLY_TRRO11_DVS_RKX_BX_TH_CNT_OUT_B7      Fld(9, 16) //[24:16]

#define DDRPHY_MISC_DQ_RXDLY_TRRO12                    (DDRPHY_NAO_BASE_ADDRESS + 0x000B0)
    #define MISC_DQ_RXDLY_TRRO12_DVS_RKX_BX_TH_CNT_OUT_DQM0    Fld(9, 0) //[8:0]

#define DDRPHY_MISC_DQ_RXDLY_TRRO13                    (DDRPHY_NAO_BASE_ADDRESS + 0x000B4)
    #define MISC_DQ_RXDLY_TRRO13_DA_RK0_DQX_B0_R_DLY           Fld(6, 0) //[5:0]
    #define MISC_DQ_RXDLY_TRRO13_DA_RK0_DQS0_R_DLY             Fld(7, 8) //[14:8]
    #define MISC_DQ_RXDLY_TRRO13_DA_RK0_DQX_B1_R_DLY           Fld(6, 16) //[21:16]
    #define MISC_DQ_RXDLY_TRRO13_DA_RK0_DQS1_R_DLY             Fld(7, 24) //[30:24]

#define DDRPHY_MISC_DQ_RXDLY_TRRO14                    (DDRPHY_NAO_BASE_ADDRESS + 0x000B8)
    #define MISC_DQ_RXDLY_TRRO14_DA_RK1_DQX_B0_R_DLY           Fld(6, 0) //[5:0]
    #define MISC_DQ_RXDLY_TRRO14_DA_RK1_DQS0_R_DLY             Fld(7, 8) //[14:8]
    #define MISC_DQ_RXDLY_TRRO14_DA_RK1_DQX_B1_R_DLY           Fld(6, 16) //[21:16]
    #define MISC_DQ_RXDLY_TRRO14_DA_RK1_DQS1_R_DLY             Fld(7, 24) //[30:24]

#define DDRPHY_MISC_DQ_RXDLY_TRRO15                    (DDRPHY_NAO_BASE_ADDRESS + 0x000BC)
    #define MISC_DQ_RXDLY_TRRO15_DA_RK2_DQX_B0_R_DLY           Fld(6, 0) //[5:0]
    #define MISC_DQ_RXDLY_TRRO15_DA_RK2_DQS0_R_DLY             Fld(7, 8) //[14:8]
    #define MISC_DQ_RXDLY_TRRO15_DA_RK2_DQX_B1_R_DLY           Fld(6, 16) //[21:16]
    #define MISC_DQ_RXDLY_TRRO15_DA_RK2_DQS1_R_DLY             Fld(7, 24) //[30:24]

#define DDRPHY_MISC_DQ_RXDLY_TRRO16                    (DDRPHY_NAO_BASE_ADDRESS + 0x000C0)
    #define MISC_DQ_RXDLY_TRRO16_DVS_RXDLY_STS_ERR_CNT_ALL     Fld(32, 0) //[31:0]

#define DDRPHY_MISC_DQ_RXDLY_TRRO17                    (DDRPHY_NAO_BASE_ADDRESS + 0x000C4)
    #define MISC_DQ_RXDLY_TRRO17_DVS_RXDLY_STS_ERR_CNT_ALL_47_32 Fld(16, 0) //[15:0]
    #define MISC_DQ_RXDLY_TRRO17_PBYTE_LEADLAG_STUCK_B0        Fld(1, 16) //[16:16]
    #define MISC_DQ_RXDLY_TRRO17_PBYTE_LEADLAG_STUCK_B1        Fld(1, 24) //[24:24]

#define DDRPHY_MISC_DQ_RXDLY_TRRO18                    (DDRPHY_NAO_BASE_ADDRESS + 0x000C8)
    #define MISC_DQ_RXDLY_TRRO18_RXDLY_DBG_MON_VALID           Fld(1, 0) //[0:0]
    #define MISC_DQ_RXDLY_TRRO18_RXDLY_RK0_FAIL_LAT            Fld(1, 1) //[1:1]
    #define MISC_DQ_RXDLY_TRRO18_RXDLY_RK1_FAIL_LAT            Fld(1, 2) //[2:2]
    #define MISC_DQ_RXDLY_TRRO18_RXDLY_RK2_FAIL_LAT            Fld(1, 3) //[3:3]
    #define MISC_DQ_RXDLY_TRRO18_DFS_SHU_GP_FAIL_LAT           Fld(2, 4) //[5:4]

#define DDRPHY_MISC_DQ_RXDLY_TRRO19                    (DDRPHY_NAO_BASE_ADDRESS + 0x000CC)
    #define MISC_DQ_RXDLY_TRRO19_RESERVED_0X00C                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_DQ_RXDLY_TRRO20                    (DDRPHY_NAO_BASE_ADDRESS + 0x000D0)
    #define MISC_DQ_RXDLY_TRRO20_RESERVED_0X0D0                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_DQ_RXDLY_TRRO21                    (DDRPHY_NAO_BASE_ADDRESS + 0x000D4)
    #define MISC_DQ_RXDLY_TRRO21_RESERVED_0X0D4                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_DQ_RXDLY_TRRO22                    (DDRPHY_NAO_BASE_ADDRESS + 0x000D8)
    #define MISC_DQ_RXDLY_TRRO22_RESERVED_0X0D8                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_DQ_RXDLY_TRRO23                    (DDRPHY_NAO_BASE_ADDRESS + 0x000DC)
    #define MISC_DQ_RXDLY_TRRO23_RESERVED_0X0DC                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_DQ_RXDLY_TRRO24                    (DDRPHY_NAO_BASE_ADDRESS + 0x000E0)
    #define MISC_DQ_RXDLY_TRRO24_RESERVED_0X0E0                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_DQ_RXDLY_TRRO25                    (DDRPHY_NAO_BASE_ADDRESS + 0x000E4)
    #define MISC_DQ_RXDLY_TRRO25_RESERVED_0X0E4                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_DQ_RXDLY_TRRO26                    (DDRPHY_NAO_BASE_ADDRESS + 0x000E8)
    #define MISC_DQ_RXDLY_TRRO26_RESERVED_0X0E8                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_DQ_RXDLY_TRRO27                    (DDRPHY_NAO_BASE_ADDRESS + 0x000EC)
    #define MISC_DQ_RXDLY_TRRO27_RESERVED_0X0EC                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_DQ_RXDLY_TRRO28                    (DDRPHY_NAO_BASE_ADDRESS + 0x000F0)
    #define MISC_DQ_RXDLY_TRRO28_RESERVED_0X0F0                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_DQ_RXDLY_TRRO29                    (DDRPHY_NAO_BASE_ADDRESS + 0x000F4)
    #define MISC_DQ_RXDLY_TRRO29_RESERVED_0X0F4                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_DQ_RXDLY_TRRO30                    (DDRPHY_NAO_BASE_ADDRESS + 0x000F8)
    #define MISC_DQ_RXDLY_TRRO30_RESERVED_0X0F8                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_DQ_RXDLY_TRRO31                    (DDRPHY_NAO_BASE_ADDRESS + 0x000FC)
    #define MISC_DQ_RXDLY_TRRO31_RESERVED_0X0FC                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_CA_RXDLY_TRRO0                     (DDRPHY_NAO_BASE_ADDRESS + 0x00100)
    #define MISC_CA_RXDLY_TRRO0_DVS_RKX_CA_SW_LAG_CNT_OUT_CA0  Fld(8, 0) //[7:0]
    #define MISC_CA_RXDLY_TRRO0_DVS_RKX_CA_SW_LEAD_CNT_OUT_CA0 Fld(8, 8) //[15:8]
    #define MISC_CA_RXDLY_TRRO0_DVS_RKX_CA_SW_LAG_CNT_OUT_CA1  Fld(8, 16) //[23:16]
    #define MISC_CA_RXDLY_TRRO0_DVS_RKX_CA_SW_LEAD_CNT_OUT_CA1 Fld(8, 24) //[31:24]

#define DDRPHY_MISC_CA_RXDLY_TRRO1                     (DDRPHY_NAO_BASE_ADDRESS + 0x00104)
    #define MISC_CA_RXDLY_TRRO1_DVS_RKX_CA_SW_LAG_CNT_OUT_CA2  Fld(8, 0) //[7:0]
    #define MISC_CA_RXDLY_TRRO1_DVS_RKX_CA_SW_LEAD_CNT_OUT_CA2 Fld(8, 8) //[15:8]
    #define MISC_CA_RXDLY_TRRO1_DVS_RKX_CA_SW_LAG_CNT_OUT_CA3  Fld(8, 16) //[23:16]
    #define MISC_CA_RXDLY_TRRO1_DVS_RKX_CA_SW_LEAD_CNT_OUT_CA3 Fld(8, 24) //[31:24]

#define DDRPHY_MISC_CA_RXDLY_TRRO2                     (DDRPHY_NAO_BASE_ADDRESS + 0x00108)
    #define MISC_CA_RXDLY_TRRO2_DVS_RKX_CA_SW_LAG_CNT_OUT_CA4  Fld(8, 0) //[7:0]
    #define MISC_CA_RXDLY_TRRO2_DVS_RKX_CA_SW_LEAD_CNT_OUT_CA4 Fld(8, 8) //[15:8]
    #define MISC_CA_RXDLY_TRRO2_DVS_RKX_CA_SW_LAG_CNT_OUT_CA5  Fld(8, 16) //[23:16]
    #define MISC_CA_RXDLY_TRRO2_DVS_RKX_CA_SW_LEAD_CNT_OUT_CA5 Fld(8, 24) //[31:24]

#define DDRPHY_MISC_CA_RXDLY_TRRO3                     (DDRPHY_NAO_BASE_ADDRESS + 0x0010C)
    #define MISC_CA_RXDLY_TRRO3_DVS_RKX_CA_SW_LAG_CNT_OUT_CKE0 Fld(8, 0) //[7:0]
    #define MISC_CA_RXDLY_TRRO3_DVS_RKX_CA_SW_LEAD_CNT_OUT_CKE0 Fld(8, 8) //[15:8]
    #define MISC_CA_RXDLY_TRRO3_DVS_RKX_CA_SW_LAG_CNT_OUT_CKE1 Fld(8, 16) //[23:16]
    #define MISC_CA_RXDLY_TRRO3_DVS_RKX_CA_SW_LEAD_CNT_OUT_CKE1 Fld(8, 24) //[31:24]

#define DDRPHY_MISC_CA_RXDLY_TRRO4                     (DDRPHY_NAO_BASE_ADDRESS + 0x00110)
    #define MISC_CA_RXDLY_TRRO4_DVS_RKX_CA_SW_LAG_CNT_OUT_CKE2 Fld(8, 0) //[7:0]
    #define MISC_CA_RXDLY_TRRO4_DVS_RKX_CA_SW_LEAD_CNT_OUT_CKE2 Fld(8, 8) //[15:8]
    #define MISC_CA_RXDLY_TRRO4_DVS_RKX_CA_SW_LAG_CNT_OUT_CS0  Fld(8, 16) //[23:16]
    #define MISC_CA_RXDLY_TRRO4_DVS_RKX_CA_SW_LEAD_CNT_OUT_CS0 Fld(8, 24) //[31:24]

#define DDRPHY_MISC_CA_RXDLY_TRRO5                     (DDRPHY_NAO_BASE_ADDRESS + 0x00114)
    #define MISC_CA_RXDLY_TRRO5_DVS_RKX_CA_SW_LAG_CNT_OUT_CS1  Fld(8, 0) //[7:0]
    #define MISC_CA_RXDLY_TRRO5_DVS_RKX_CA_SW_LEAD_CNT_OUT_CS1 Fld(8, 8) //[15:8]
    #define MISC_CA_RXDLY_TRRO5_DVS_RKX_CA_SW_LAG_CNT_OUT_CS2  Fld(8, 16) //[23:16]
    #define MISC_CA_RXDLY_TRRO5_DVS_RKX_CA_SW_LEAD_CNT_OUT_CS2 Fld(8, 24) //[31:24]

#define DDRPHY_MISC_CA_RXDLY_TRRO6                     (DDRPHY_NAO_BASE_ADDRESS + 0x00118)
    #define MISC_CA_RXDLY_TRRO6_DVS_RKX_CA_LEAD_LAG_CNT_OUT_CA0 Fld(8, 0) //[7:0]
    #define MISC_CA_RXDLY_TRRO6_DVS_RKX_CA_LEAD_LAG_CNT_OUT_CA1 Fld(8, 8) //[15:8]
    #define MISC_CA_RXDLY_TRRO6_DVS_RKX_CA_LEAD_LAG_CNT_OUT_CA2 Fld(8, 16) //[23:16]
    #define MISC_CA_RXDLY_TRRO6_DVS_RKX_CA_LEAD_LAG_CNT_OUT_CA3 Fld(8, 24) //[31:24]

#define DDRPHY_MISC_CA_RXDLY_TRRO7                     (DDRPHY_NAO_BASE_ADDRESS + 0x0011C)
    #define MISC_CA_RXDLY_TRRO7_DVS_RKX_CA_LEAD_LAG_CNT_OUT_CA4 Fld(8, 0) //[7:0]
    #define MISC_CA_RXDLY_TRRO7_DVS_RKX_CA_LEAD_LAG_CNT_OUT_CA5 Fld(8, 8) //[15:8]
    #define MISC_CA_RXDLY_TRRO7_DVS_RKX_CA_LEAD_LAG_CNT_OUT_CKE0 Fld(8, 16) //[23:16]
    #define MISC_CA_RXDLY_TRRO7_DVS_RKX_CA_LEAD_LAG_CNT_OUT_CKE1 Fld(8, 24) //[31:24]

#define DDRPHY_MISC_CA_RXDLY_TRRO8                     (DDRPHY_NAO_BASE_ADDRESS + 0x00120)
    #define MISC_CA_RXDLY_TRRO8_DVS_RKX_CA_LEAD_LAG_CNT_OUT_CKE2 Fld(8, 0) //[7:0]
    #define MISC_CA_RXDLY_TRRO8_DVS_RKX_CA_LEAD_LAG_CNT_OUT_CS0 Fld(8, 8) //[15:8]
    #define MISC_CA_RXDLY_TRRO8_DVS_RKX_CA_LEAD_LAG_CNT_OUT_CS1 Fld(8, 16) //[23:16]
    #define MISC_CA_RXDLY_TRRO8_DVS_RKX_CA_LEAD_LAG_CNT_OUT_CS2 Fld(8, 24) //[31:24]

#define DDRPHY_MISC_CA_RXDLY_TRRO9                     (DDRPHY_NAO_BASE_ADDRESS + 0x00124)
    #define MISC_CA_RXDLY_TRRO9_DVS_RK0_CA_SW_UP_DONE          Fld(1, 0) //[0:0]
    #define MISC_CA_RXDLY_TRRO9_DVS_RK1_CA_SW_UP_DONE          Fld(1, 8) //[8:8]
    #define MISC_CA_RXDLY_TRRO9_DVS_RK2_CA_SW_UP_DONE          Fld(1, 16) //[16:16]

#define DDRPHY_MISC_CA_RXDLY_TRRO10                    (DDRPHY_NAO_BASE_ADDRESS + 0x00128)
    #define MISC_CA_RXDLY_TRRO10_DVS_RKX_CA_TH_CNT_OUT_CA0     Fld(9, 0) //[8:0]
    #define MISC_CA_RXDLY_TRRO10_DVS_RKX_CA_TH_CNT_OUT_CA1     Fld(9, 16) //[24:16]

#define DDRPHY_MISC_CA_RXDLY_TRRO11                    (DDRPHY_NAO_BASE_ADDRESS + 0x0012C)
    #define MISC_CA_RXDLY_TRRO11_DVS_RKX_CA_TH_CNT_OUT_CA2     Fld(9, 0) //[8:0]
    #define MISC_CA_RXDLY_TRRO11_DVS_RKX_CA_TH_CNT_OUT_CA3     Fld(9, 16) //[24:16]

#define DDRPHY_MISC_CA_RXDLY_TRRO12                    (DDRPHY_NAO_BASE_ADDRESS + 0x00130)
    #define MISC_CA_RXDLY_TRRO12_DVS_RKX_CA_TH_CNT_OUT_CA4     Fld(9, 0) //[8:0]
    #define MISC_CA_RXDLY_TRRO12_DVS_RKX_CA_TH_CNT_OUT_CA5     Fld(9, 16) //[24:16]

#define DDRPHY_MISC_CA_RXDLY_TRRO13                    (DDRPHY_NAO_BASE_ADDRESS + 0x00134)
    #define MISC_CA_RXDLY_TRRO13_DVS_RKX_CA_TH_CNT_OUT_CKE0    Fld(9, 0) //[8:0]
    #define MISC_CA_RXDLY_TRRO13_DVS_RKX_CA_TH_CNT_OUT_CKE1    Fld(9, 16) //[24:16]

#define DDRPHY_MISC_CA_RXDLY_TRRO14                    (DDRPHY_NAO_BASE_ADDRESS + 0x00138)
    #define MISC_CA_RXDLY_TRRO14_DVS_RKX_CA_TH_CNT_OUT_CKE2    Fld(9, 0) //[8:0]
    #define MISC_CA_RXDLY_TRRO14_DVS_RKX_CA_TH_CNT_OUT_CS0     Fld(9, 16) //[24:16]

#define DDRPHY_MISC_CA_RXDLY_TRRO15                    (DDRPHY_NAO_BASE_ADDRESS + 0x0013C)
    #define MISC_CA_RXDLY_TRRO15_DVS_RKX_CA_TH_CNT_OUT_CS1     Fld(9, 0) //[8:0]
    #define MISC_CA_RXDLY_TRRO15_DVS_RKX_CA_TH_CNT_OUT_CS2     Fld(9, 16) //[24:16]

#define DDRPHY_MISC_CA_RXDLY_TRRO16                    (DDRPHY_NAO_BASE_ADDRESS + 0x00140)
    #define MISC_CA_RXDLY_TRRO16_DA_RK0_CAX_CA_R_DLY           Fld(6, 0) //[5:0]
    #define MISC_CA_RXDLY_TRRO16_DA_RK0_CLK_R_DLY              Fld(8, 8) //[15:8]

#define DDRPHY_MISC_CA_RXDLY_TRRO17                    (DDRPHY_NAO_BASE_ADDRESS + 0x00144)
    #define MISC_CA_RXDLY_TRRO17_DA_RK1_CAX_CA_R_DLY           Fld(6, 0) //[5:0]
    #define MISC_CA_RXDLY_TRRO17_DA_RK1_CLK_R_DLY              Fld(8, 8) //[15:8]

#define DDRPHY_MISC_CA_RXDLY_TRRO18                    (DDRPHY_NAO_BASE_ADDRESS + 0x00148)
    #define MISC_CA_RXDLY_TRRO18_DA_RK2_CAX_CA_R_DLY           Fld(6, 0) //[5:0]
    #define MISC_CA_RXDLY_TRRO18_DA_RK2_CLK_R_DLY              Fld(8, 8) //[15:8]

#define DDRPHY_MISC_CA_RXDLY_TRRO19                    (DDRPHY_NAO_BASE_ADDRESS + 0x0014C)
    #define MISC_CA_RXDLY_TRRO19_DVS_RXDLY_STS_ERR_CNT_ALL_CA  Fld(24, 0) //[23:0]
    #define MISC_CA_RXDLY_TRRO19_PBYTE_LEADLAG_STUCK_CA        Fld(1, 24) //[24:24]

#define DDRPHY_MISC_CA_RXDLY_TRRO20                    (DDRPHY_NAO_BASE_ADDRESS + 0x00150)
    #define MISC_CA_RXDLY_TRRO20_RESERVED_0X150                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_CA_RXDLY_TRRO21                    (DDRPHY_NAO_BASE_ADDRESS + 0x00154)
    #define MISC_CA_RXDLY_TRRO21_RESERVED_0X154                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_CA_RXDLY_TRRO22                    (DDRPHY_NAO_BASE_ADDRESS + 0x00158)
    #define MISC_CA_RXDLY_TRRO22_RESERVED_0X158                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_CA_RXDLY_TRRO23                    (DDRPHY_NAO_BASE_ADDRESS + 0x0015C)
    #define MISC_CA_RXDLY_TRRO23_RESERVED_0X15C                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_CA_RXDLY_TRRO24                    (DDRPHY_NAO_BASE_ADDRESS + 0x00160)
    #define MISC_CA_RXDLY_TRRO24_RESERVED_0X160                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_CA_RXDLY_TRRO25                    (DDRPHY_NAO_BASE_ADDRESS + 0x00164)
    #define MISC_CA_RXDLY_TRRO25_RESERVED_0X164                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_CA_RXDLY_TRRO26                    (DDRPHY_NAO_BASE_ADDRESS + 0x00168)
    #define MISC_CA_RXDLY_TRRO26_RESERVED_0X168                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_CA_RXDLY_TRRO27                    (DDRPHY_NAO_BASE_ADDRESS + 0x0016C)
    #define MISC_CA_RXDLY_TRRO27_RESERVED_0X16C                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_CA_RXDLY_TRRO28                    (DDRPHY_NAO_BASE_ADDRESS + 0x00170)
    #define MISC_CA_RXDLY_TRRO28_RESERVED_0X170                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_CA_RXDLY_TRRO29                    (DDRPHY_NAO_BASE_ADDRESS + 0x00174)
    #define MISC_CA_RXDLY_TRRO29_RESERVED_0X174                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_CA_RXDLY_TRRO30                    (DDRPHY_NAO_BASE_ADDRESS + 0x00178)
    #define MISC_CA_RXDLY_TRRO30_RESERVED_0X178                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_CA_RXDLY_TRRO31                    (DDRPHY_NAO_BASE_ADDRESS + 0x0017C)
    #define MISC_CA_RXDLY_TRRO31_RESERVED_0X17C                Fld(32, 0) //[31:0]

#define DDRPHY_MISC_DQO1                               (DDRPHY_NAO_BASE_ADDRESS + 0x00180)
    #define MISC_DQO1_DQO1_RO                                  Fld(32, 0) //[31:0]

#define DDRPHY_MISC_CAO1                               (DDRPHY_NAO_BASE_ADDRESS + 0x00184)
    #define MISC_CAO1_RA0_O1                                   Fld(1, 0) //[0:0]
    #define MISC_CAO1_RA1_O1                                   Fld(1, 1) //[1:1]
    #define MISC_CAO1_RA2_O1                                   Fld(1, 2) //[2:2]
    #define MISC_CAO1_RA3_O1                                   Fld(1, 3) //[3:3]
    #define MISC_CAO1_RA4_O1                                   Fld(1, 4) //[4:4]
    #define MISC_CAO1_RA5_O1                                   Fld(1, 5) //[5:5]
    #define MISC_CAO1_RA6_O1                                   Fld(1, 6) //[6:6]
    #define MISC_CAO1_RA7_O1                                   Fld(1, 7) //[7:7]
    #define MISC_CAO1_RA8_O1                                   Fld(1, 8) //[8:8]
    #define MISC_CAO1_RA9_O1                                   Fld(1, 9) //[9:9]
    #define MISC_CAO1_CKEO1_RO                                 Fld(1, 10) //[10:10]
    #define MISC_CAO1_CKE1O1_RO                                Fld(1, 11) //[11:11]
    #define MISC_CAO1_CKE2O1_RO                                Fld(1, 12) //[12:12]
    #define MISC_CAO1_CSO1_RO                                  Fld(1, 13) //[13:13]
    #define MISC_CAO1_CS1O1_RO                                 Fld(1, 14) //[14:14]
    #define MISC_CAO1_CS2O1_RO                                 Fld(1, 15) //[15:15]
    #define MISC_CAO1_RESETO1_RO                               Fld(1, 16) //[16:16]
    #define MISC_CAO1_DQM0O1_RO                                Fld(1, 24) //[24:24]
    #define MISC_CAO1_DQM1O1_RO                                Fld(1, 25) //[25:25]
    #define MISC_CAO1_DQM2O1_RO                                Fld(1, 26) //[26:26]
    #define MISC_CAO1_DQM3O1_RO                                Fld(1, 27) //[27:27]

#define DDRPHY_MISC_AD_RX_DQ_O1                        (DDRPHY_NAO_BASE_ADDRESS + 0x00188)
    #define MISC_AD_RX_DQ_O1_AD_RX_ARDQ_O1_B0                  Fld(8, 0) //[7:0]
    #define MISC_AD_RX_DQ_O1_AD_RX_ARDQM0_O1_B0                Fld(1, 8) //[8:8]
    #define MISC_AD_RX_DQ_O1_AD_RX_ARDQ_O1_B1                  Fld(8, 16) //[23:16]
    #define MISC_AD_RX_DQ_O1_AD_RX_ARDQM0_O1_B1                Fld(1, 24) //[24:24]
    #define MISC_AD_RX_DQ_O1_AD_RX_ARDQ_O1_B0_BIT2	Fld(1, 2) //[2:2]	 //francis added
    #define MISC_AD_RX_DQ_O1_AD_RX_ARDQ_O1_B1_BIT2	Fld(1, 18 )//[18:18]    //francis added

#define DDRPHY_MISC_AD_RX_CMD_O1                       (DDRPHY_NAO_BASE_ADDRESS + 0x0018C)
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCA0_O1                   Fld(1, 0) //[0:0]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCA1_O1                   Fld(1, 1) //[1:1]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCA2_O1                   Fld(1, 2) //[2:2]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCA3_O1                   Fld(1, 3) //[3:3]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCA4_O1                   Fld(1, 4) //[4:4]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCA5_O1                   Fld(1, 5) //[5:5]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCA6_O1                   Fld(1, 6) //[6:6]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCA7_O1                   Fld(1, 7) //[7:7]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCA8_O1                   Fld(1, 8) //[8:8]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCA9_O1                   Fld(1, 9) //[9:9]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCKE0_O1                  Fld(1, 10) //[10:10]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCKE1_O1                  Fld(1, 11) //[11:11]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCKE2_O1                  Fld(1, 12) //[12:12]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCS0_O1                   Fld(1, 13) //[13:13]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCS1_O1                   Fld(1, 14) //[14:14]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCS2_O1                   Fld(1, 15) //[15:15]

#define DDRPHY_MISC_PHY_RGS_DQ                         (DDRPHY_NAO_BASE_ADDRESS + 0x00190)
    #define MISC_PHY_RGS_DQ_RGS_ARDQ_OFFSET_FLAG_B0            Fld(8, 0) //[7:0]
    #define MISC_PHY_RGS_DQ_RGS_ARDQM0_OFFSET_FLAG_B0          Fld(1, 8) //[8:8]
    #define MISC_PHY_RGS_DQ_RGS_RX_ARDQS0_RDY_EYE_B0           Fld(1, 9) //[9:9]
    #define MISC_PHY_RGS_DQ_RGS_ARDQ_OFFSET_FLAG_B1            Fld(8, 16) //[23:16]
    #define MISC_PHY_RGS_DQ_RGS_ARDQM0_OFFSET_FLAG_B1          Fld(1, 24) //[24:24]
    #define MISC_PHY_RGS_DQ_RGS_RX_ARDQS0_RDY_EYE_B1           Fld(1, 25) //[25:25]
    #define MISC_PHY_RGS_DQ_DA_RPHYPLLGP_CK_SEL                Fld(1, 31) //[31:31]

#define DDRPHY_MISC_PHY_RGS_CMD                        (DDRPHY_NAO_BASE_ADDRESS + 0x00194)
    #define MISC_PHY_RGS_CMD_RGS_ARCA0_OFFSET_FLAG             Fld(1, 0) //[0:0]
    #define MISC_PHY_RGS_CMD_RGS_ARCA1_OFFSET_FLAG             Fld(1, 1) //[1:1]
    #define MISC_PHY_RGS_CMD_RGS_ARCA2_OFFSET_FLAG             Fld(1, 2) //[2:2]
    #define MISC_PHY_RGS_CMD_RGS_ARCA3_OFFSET_FLAG             Fld(1, 3) //[3:3]
    #define MISC_PHY_RGS_CMD_RGS_ARCA4_OFFSET_FLAG             Fld(1, 4) //[4:4]
    #define MISC_PHY_RGS_CMD_RGS_ARCA5_OFFSET_FLAG             Fld(1, 5) //[5:5]
    #define MISC_PHY_RGS_CMD_RGS_ARCA6_OFFSET_FLAG             Fld(1, 6) //[6:6]
    #define MISC_PHY_RGS_CMD_RGS_ARCA7_OFFSET_FLAG             Fld(1, 7) //[7:7]
    #define MISC_PHY_RGS_CMD_RGS_ARCA8_OFFSET_FLAG             Fld(1, 8) //[8:8]
    #define MISC_PHY_RGS_CMD_RGS_ARCA9_OFFSET_FLAG             Fld(1, 9) //[9:9]
    #define MISC_PHY_RGS_CMD_RGS_ARCKE0_OFFSET_FLAG            Fld(1, 10) //[10:10]
    #define MISC_PHY_RGS_CMD_RGS_ARCKE1_OFFSET_FLAG            Fld(1, 11) //[11:11]
    #define MISC_PHY_RGS_CMD_RGS_ARCKE2_OFFSET_FLAG            Fld(1, 12) //[12:12]
    #define MISC_PHY_RGS_CMD_RGS_ARCS0_OFFSET_FLAG             Fld(1, 13) //[13:13]
    #define MISC_PHY_RGS_CMD_RGS_ARCS1_OFFSET_FLAG             Fld(1, 14) //[14:14]
    #define MISC_PHY_RGS_CMD_RGS_ARCS2_OFFSET_FLAG             Fld(1, 15) //[15:15]
    #define MISC_PHY_RGS_CMD_RGS_RX_ARCLK_RDY_EYE              Fld(1, 16) //[16:16]
    #define MISC_PHY_RGS_CMD_RGS_RIMPCALOUT                    Fld(1, 24) //[24:24]

#define DDRPHY_MISC_PHY_RGS_STBEN_B0                   (DDRPHY_NAO_BASE_ADDRESS + 0x00198)
    #define MISC_PHY_RGS_STBEN_B0_AD_RX_ARDQ0_STBEN_B0         Fld(8, 0) //[7:0]
    #define MISC_PHY_RGS_STBEN_B0_AD_RX_ARDQ4_STBEN_B0         Fld(8, 8) //[15:8]
    #define MISC_PHY_RGS_STBEN_B0_AD_RX_ARDQS0_STBEN_LEAD_B0   Fld(1, 16) //[16:16]
    #define MISC_PHY_RGS_STBEN_B0_AD_RX_ARDQS0_STBEN_LAG_B0    Fld(1, 17) //[17:17]
    #define MISC_PHY_RGS_STBEN_B0_AD_ARDLL_PD_EN_B0            Fld(1, 18) //[18:18]
    #define MISC_PHY_RGS_STBEN_B0_AD_ARDLL_MON_B0              Fld(8, 24) //[31:24]

#define DDRPHY_MISC_PHY_RGS_STBEN_B1                   (DDRPHY_NAO_BASE_ADDRESS + 0x0019C)
    #define MISC_PHY_RGS_STBEN_B1_AD_RX_ARDQ0_STBEN_B1         Fld(8, 0) //[7:0]
    #define MISC_PHY_RGS_STBEN_B1_AD_RX_ARDQ4_STBEN_B1         Fld(8, 8) //[15:8]
    #define MISC_PHY_RGS_STBEN_B1_AD_RX_ARDQS0_STBEN_LEAD_B1   Fld(1, 16) //[16:16]
    #define MISC_PHY_RGS_STBEN_B1_AD_RX_ARDQS0_STBEN_LAG_B1    Fld(1, 17) //[17:17]
    #define MISC_PHY_RGS_STBEN_B1_AD_ARDLL_PD_EN_B1            Fld(1, 18) //[18:18]
    #define MISC_PHY_RGS_STBEN_B1_AD_ARDLL_MON_B1              Fld(8, 24) //[31:24]

#define DDRPHY_MISC_PHY_RGS_STBEN_CMD                  (DDRPHY_NAO_BASE_ADDRESS + 0x001A0)
    #define MISC_PHY_RGS_STBEN_CMD_AD_RX_ARCA0_STBEN           Fld(8, 0) //[7:0]
    #define MISC_PHY_RGS_STBEN_CMD_AD_RX_ARCA4_STBEN           Fld(8, 8) //[15:8]
    #define MISC_PHY_RGS_STBEN_CMD_AD_RX_ARCLK_STBEN_LEAD      Fld(1, 16) //[16:16]
    #define MISC_PHY_RGS_STBEN_CMD_AD_RX_ARCLK_STBEN_LAG       Fld(1, 17) //[17:17]
    #define MISC_PHY_RGS_STBEN_CMD_AD_ARDLL_PD_EN_CA           Fld(1, 18) //[18:18]
    #define MISC_PHY_RGS_STBEN_CMD_AD_ARDLL_MON_CA             Fld(8, 24) //[31:24]

#define DDRPHY_MISC_STA_TOGLB0                         (DDRPHY_NAO_BASE_ADDRESS + 0x001A4)
    #define MISC_STA_TOGLB0_STA_TOGLB_DONE                     Fld(32, 0) //[31:0]

#define DDRPHY_MISC_STA_TOGLB1                         (DDRPHY_NAO_BASE_ADDRESS + 0x001A8)
    #define MISC_STA_TOGLB1_STA_TOGLB_FAIL                     Fld(32, 0) //[31:0]

#define DDRPHY_MISC_STA_TOGLB2                         (DDRPHY_NAO_BASE_ADDRESS + 0x001AC)
    #define MISC_STA_TOGLB2_STA_TOGLB_PUHI_TIMEOUT             Fld(32, 0) //[31:0]

#define DDRPHY_MISC_STA_TOGLB3                         (DDRPHY_NAO_BASE_ADDRESS + 0x001B0)
    #define MISC_STA_TOGLB3_STA_TOGLB_PULO_TIMEOUT             Fld(32, 0) //[31:0]

#define DDRPHY_MISC_FT_STATUS_0                        (DDRPHY_NAO_BASE_ADDRESS + 0x001B4)
    #define MISC_FT_STATUS_0_AD_RX_ARDQ_DVS_R_LAG_B1           Fld(8, 0) //[7:0]
    #define MISC_FT_STATUS_0_AD_RX_ARDQ_DVS_R_LEAD_B1          Fld(8, 8) //[15:8]
    #define MISC_FT_STATUS_0_AD_RX_ARDQ_DVS_R_LAG_B0           Fld(8, 16) //[23:16]
    #define MISC_FT_STATUS_0_AD_RX_ARDQ_DVS_R_LEAD_B0          Fld(8, 24) //[31:24]

#define DDRPHY_MISC_FT_STATUS_1                        (DDRPHY_NAO_BASE_ADDRESS + 0x001B8)
    #define MISC_FT_STATUS_1_AD_RX_ARDQ_DVS_F_LAG_B1           Fld(8, 0) //[7:0]
    #define MISC_FT_STATUS_1_AD_RX_ARDQ_DVS_F_LEAD_B1          Fld(8, 8) //[15:8]
    #define MISC_FT_STATUS_1_AD_RX_ARDQ_DVS_F_LAG_B0           Fld(8, 16) //[23:16]
    #define MISC_FT_STATUS_1_AD_RX_ARDQ_DVS_F_LEAD_B0          Fld(8, 24) //[31:24]

#define DDRPHY_MISC_FT_STATUS_2                        (DDRPHY_NAO_BASE_ADDRESS + 0x001BC)
    #define MISC_FT_STATUS_2_AD_RRESETB_O                      Fld(1, 0) //[0:0]

#define DDRPHY_MISC_FT_STATUS_3                        (DDRPHY_NAO_BASE_ADDRESS + 0x001C0)
    #define MISC_FT_STATUS_3_AD_RX_ARCA0_DVS_R_LAG             Fld(1, 0) //[0:0]
    #define MISC_FT_STATUS_3_AD_RX_ARCA1_DVS_R_LAG             Fld(1, 1) //[1:1]
    #define MISC_FT_STATUS_3_AD_RX_ARCA2_DVS_R_LAG             Fld(1, 2) //[2:2]
    #define MISC_FT_STATUS_3_AD_RX_ARCA3_DVS_R_LAG             Fld(1, 3) //[3:3]
    #define MISC_FT_STATUS_3_AD_RX_ARCA4_DVS_R_LAG             Fld(1, 4) //[4:4]
    #define MISC_FT_STATUS_3_AD_RX_ARCA5_DVS_R_LAG             Fld(1, 5) //[5:5]
    #define MISC_FT_STATUS_3_AD_RX_ARCKE0_DVS_R_LAG            Fld(1, 6) //[6:6]
    #define MISC_FT_STATUS_3_AD_RX_ARCKE1_DVS_R_LAG            Fld(1, 7) //[7:7]
    #define MISC_FT_STATUS_3_AD_RX_ARCS0_DVS_R_LAG             Fld(1, 8) //[8:8]
    #define MISC_FT_STATUS_3_AD_RX_ARCS1_DVS_R_LAG             Fld(1, 9) //[9:9]
    #define MISC_FT_STATUS_3_AD_RX_ARCA0_DVS_R_LEAD            Fld(1, 16) //[16:16]
    #define MISC_FT_STATUS_3_AD_RX_ARCA1_DVS_R_LEAD            Fld(1, 17) //[17:17]
    #define MISC_FT_STATUS_3_AD_RX_ARCA2_DVS_R_LEAD            Fld(1, 18) //[18:18]
    #define MISC_FT_STATUS_3_AD_RX_ARCA3_DVS_R_LEAD            Fld(1, 19) //[19:19]
    #define MISC_FT_STATUS_3_AD_RX_ARCA4_DVS_R_LEAD            Fld(1, 20) //[20:20]
    #define MISC_FT_STATUS_3_AD_RX_ARCA5_DVS_R_LEAD            Fld(1, 21) //[21:21]
    #define MISC_FT_STATUS_3_AD_RX_ARCKE0_DVS_R_LEAD           Fld(1, 22) //[22:22]
    #define MISC_FT_STATUS_3_AD_RX_ARCKE1_DVS_R_LEAD           Fld(1, 23) //[23:23]
    #define MISC_FT_STATUS_3_AD_RX_ARCS0_DVS_R_LEAD            Fld(1, 24) //[24:24]
    #define MISC_FT_STATUS_3_AD_RX_ARCS1_DVS_R_LEAD            Fld(1, 25) //[25:25]

#define DDRPHY_MISC_FT_STATUS_4                        (DDRPHY_NAO_BASE_ADDRESS + 0x001C4)
    #define MISC_FT_STATUS_4_AD_RX_ARCA0_DVS_F_LAG             Fld(1, 0) //[0:0]
    #define MISC_FT_STATUS_4_AD_RX_ARCA1_DVS_F_LAG             Fld(1, 1) //[1:1]
    #define MISC_FT_STATUS_4_AD_RX_ARCA2_DVS_F_LAG             Fld(1, 2) //[2:2]
    #define MISC_FT_STATUS_4_AD_RX_ARCA3_DVS_F_LAG             Fld(1, 3) //[3:3]
    #define MISC_FT_STATUS_4_AD_RX_ARCA4_DVS_F_LAG             Fld(1, 4) //[4:4]
    #define MISC_FT_STATUS_4_AD_RX_ARCA5_DVS_F_LAG             Fld(1, 5) //[5:5]
    #define MISC_FT_STATUS_4_AD_RX_ARCKE0_DVS_F_LAG            Fld(1, 6) //[6:6]
    #define MISC_FT_STATUS_4_AD_RX_ARCKE1_DVS_F_LAG            Fld(1, 7) //[7:7]
    #define MISC_FT_STATUS_4_AD_RX_ARCS0_DVS_F_LAG             Fld(1, 8) //[8:8]
    #define MISC_FT_STATUS_4_AD_RX_ARCS1_DVS_F_LAG             Fld(1, 9) //[9:9]
    #define MISC_FT_STATUS_4_AD_RX_ARCA0_DVS_F_LEAD            Fld(1, 16) //[16:16]
    #define MISC_FT_STATUS_4_AD_RX_ARCA1_DVS_F_LEAD            Fld(1, 17) //[17:17]
    #define MISC_FT_STATUS_4_AD_RX_ARCA2_DVS_F_LEAD            Fld(1, 18) //[18:18]
    #define MISC_FT_STATUS_4_AD_RX_ARCA3_DVS_F_LEAD            Fld(1, 19) //[19:19]
    #define MISC_FT_STATUS_4_AD_RX_ARCA4_DVS_F_LEAD            Fld(1, 20) //[20:20]
    #define MISC_FT_STATUS_4_AD_RX_ARCA5_DVS_F_LEAD            Fld(1, 21) //[21:21]
    #define MISC_FT_STATUS_4_AD_RX_ARCKE0_DVS_F_LEAD           Fld(1, 22) //[22:22]
    #define MISC_FT_STATUS_4_AD_RX_ARCKE1_DVS_F_LEAD           Fld(1, 23) //[23:23]
    #define MISC_FT_STATUS_4_AD_RX_ARCS0_DVS_F_LEAD            Fld(1, 24) //[24:24]
    #define MISC_FT_STATUS_4_AD_RX_ARCS1_DVS_F_LEAD            Fld(1, 25) //[25:25]

#define DDRPHY_MISC_STA_EXTLB_DBG0                     (DDRPHY_NAO_BASE_ADDRESS + 0x001C8)
    #define MISC_STA_EXTLB_DBG0_STA_EXTLB_DVS_LEAD_0TO1        Fld(32, 0) //[31:0]

#define DDRPHY_MISC_STA_EXTLB_DBG1                     (DDRPHY_NAO_BASE_ADDRESS + 0x001CC)
    #define MISC_STA_EXTLB_DBG1_STA_EXTLB_DVS_LEAD_1TO0        Fld(32, 0) //[31:0]

#define DDRPHY_MISC_STA_EXTLB_DBG2                     (DDRPHY_NAO_BASE_ADDRESS + 0x001D0)
    #define MISC_STA_EXTLB_DBG2_STA_EXTLB_DVS_LAG_0TO1         Fld(32, 0) //[31:0]

#define DDRPHY_MISC_STA_EXTLB_DBG3                     (DDRPHY_NAO_BASE_ADDRESS + 0x001D4)
    #define MISC_STA_EXTLB_DBG3_STA_EXTLB_DVS_LAG_1TO0         Fld(32, 0) //[31:0]


/*
 * DDRPHY_SHU1_B0_DLL1
 * DDRPHY_SHU1_B1_DLL1
 */
#define RG_ARDQ_REV_BIT_23_NA Fld(1, 31)/* [31:31] */
#define RG_ARDQ_REV_BIT_2221_DATA_SWAP Fld(2, 29)/* [30:29] */
#define RG_ARDQ_REV_BIT_20_DATA_SWAP_EN Fld(1, 28)/* [28:28] */
#define RG_ARDQ_REV_BIT_19_RX_DQSIEN_FORCE_ON_EN Fld(1, 27)/* [27:27] */
#define RG_ARDQ_REV_BIT_18_RX_LP4Y_EN Fld(1, 26)/* [26:26] */
#define RG_ARDQ_REV_BIT_1716_TX_LP4Y_SEL Fld(2, 24)/* [25:24] */
#define RG_ARDQ_REV_BIT_15_TX_PRE_EN Fld(1, 23)/* [23:23] */
#define RG_ARDQ_REV_BIT_14_TX_PRE_DATA_SEL Fld(1, 22)/* [22:22] */
#define RG_ARDQ_REV_BIT_1312_DRVP_PRE Fld(2, 20)/* [21:20] */
#define RG_ARDQ_REV_BIT_1110_DRVN_PRE Fld(2, 18)/* [19:18] */
#define RG_ARDQ_REV_BIT_09_TX_READ_BASE_EN Fld(1, 17)/* [17:17] */
#define RG_ARDQ_REV_BIT_08_TX_ODT_DISABLE Fld(1, 16)/* [16:16] */
#define RG_ARDQ_REV_BIT_07_MCK4X_SEL_DQ5 Fld(1, 15)/* [15:15] */
#define RG_ARDQ_REV_BIT_06_MCK4X_SEL_DQ1 Fld(1, 14)/* [14:14] */
#define RG_ARDQ_REV_BIT_05_RX_SER_RST_MODE Fld(1, 13)/* [13:13] */
#define RG_ARDQ_REV_BIT_04_RX_DQSIEN_RB_DLY Fld(1, 12)/* [12:12] */
#define RG_ARDQ_REV_BIT_03_RX_DQS_GATE_EN_MODE Fld(1, 11)/* [11:11] */
#define RG_ARDQ_REV_BIT_02_TX_READ_BASE_EN_DQSB Fld(1, 10)/* [10:10] */
#define RG_ARDQ_REV_BIT_01_DQS_MCK4XB_DLY_EN Fld(1, 9)/* [9:9] */
#define RG_ARDQ_REV_BIT_00_DQS_MCK4X_DLY_EN Fld(1, 8)/* [8:8] */

/* DDRPHY_SHU1_CA_DLL1 */
#define RG_ARCMD_REV_BIT_23_NA Fld(1, 31)/* [31:31] */
#define RG_ARCMD_REV_BIT_2221_DATA_SWAP Fld(2, 29)/* [30:29] */
#define RG_ARCMD_REV_BIT_20_DATA_SWAP_EN Fld(1, 28)/* [28:28] */
#define RG_ARCMD_REV_BIT_19_RX_DQSIEN_FORCE_ON_EN Fld(1, 27)/* [27:27] */
#define RG_ARCMD_REV_BIT_18_RX_LP4Y_EN Fld(1, 26)/* [26:26] */
#define RG_ARCMD_REV_BIT_1716_TX_LP4Y_SEL Fld(2, 24)/* [25:24] */
#define RG_ARCMD_REV_BIT_15_TX_DDR4P_CKE_SEL Fld(1, 23)/* [23:23] */
#define RG_ARCMD_REV_BIT_14_TX_DDR4_CKE_SEL Fld(1, 22)/* [22:22] */
#define RG_ARCMD_REV_BIT_13_TX_DDR3_CKE_SEL Fld(1, 21)/* [21:21] */
#define RG_ARCMD_REV_BIT_1208_TX_CKE_DRVN Fld(5, 16)/* [20:16] */
#define RG_ARCMD_REV_BIT_07_MCK4X_SEL_CKE1 Fld(1, 15)/* [15:15] */
#define RG_ARCMD_REV_BIT_06_MCK4X_SEL_CKE0 Fld(1, 14)/* [14:14] */
#define RG_ARCMD_REV_BIT_05_RX_SER_RST_MODE Fld(1, 13)/* [13:13] */
#define RG_ARCMD_REV_BIT_04_RX_DQSIEN_RB_DLY Fld(1, 12)/* [12:12] */
#define RG_ARCMD_REV_BIT_03_RX_DQS_GATE_EN_MODE Fld(1, 11)/* [11:11] */
#define RG_ARCMD_REV_BIT_02_TX_LSH_DQM_CG_EN Fld(1, 10)/* [10:10] */
#define RG_ARCMD_REV_BIT_01_TX_LSH_DQS_CG_EN Fld(1, 9)/* [9:9] */
#define RG_ARCMD_REV_BIT_00_TX_LSH_DQ_CG_EN Fld(1, 8)/* [8:8] */

/*
 * DDRPHY_SHU1_CA_CMD6
 * DDRPHY_SHU1_B0_DQ6
 * DDRPHY_SHU1_B1_DQ6
 */
#define RG_ARPI_RESERVE_BIT_15 Fld(1, 21)/* [21:21] */
#define RG_ARPI_RESERVE_BIT_14_LP3_SEL Fld(1, 20)/* [20:20] */
#define RG_ARPI_RESERVE_BIT_13_CG_SYNC_ENB Fld(1, 19)/* [19:19] */
#define RG_ARPI_RESERVE_BIT_12_BYPASS_SR_DQS Fld(1, 18)/* [18:18] */
#define RG_ARPI_RESERVE_BIT_11_BYPASS_SR Fld(1, 17)/* [17:17] */
#define RG_ARPI_RESERVE_BIT_10_BUFGP_XLATCH_FORCE_DQS Fld(1, 16)/* [16:16] */
#define RG_ARPI_RESERVE_BIT_09_BUFGP_XLATCH_FORCE Fld(1, 15)/* [15:15] */
#define RG_ARPI_RESERVE_BIT_08_SMT_XLATCH_FORCE_DQS Fld(1, 14)/* [14:14] */
#define RG_ARPI_RESERVE_BIT_07_SMT_XLATCH_FORCE Fld(1, 13)/* [13:13] */
#define RG_ARPI_RESERVE_BIT_06_PSMUX_XLATCH_FORCEDQS Fld(1, 12)/* [12:12] */
#define RG_ARPI_RESERVE_BIT_05_PSMUX_XLATCH_FORCE Fld(1, 11)/* [11:11] */
#define RG_ARPI_RESERVE_BIT_04_8PHASE_XLATCH_FORCE Fld(1, 10)/* [10:10] */
#define RG_ARPI_RESERVE_BIT_03_MIDPI_CAP_SEL Fld(1, 9)/* [9:9] */
#define RG_ARPI_RESERVE_BIT_02_HYST_SEL Fld(1, 8)/* [8:8] */
#define RG_ARPI_RESERVE_BIT_01_DLL_FAST_PSJP Fld(1, 7)/* [7:7] */
#define RG_ARPI_RESERVE_BIT_00_TX_CG_EN Fld(1, 6)/* [6:6] */


#endif
/* ========== Page Breaker ================== */

