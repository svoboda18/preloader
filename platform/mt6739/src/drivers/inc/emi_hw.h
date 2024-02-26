#ifndef __EMI_HW_H__
#define __EMI_HW_H__

#include "mt6739.h"

typedef volatile unsigned int *      P_U32;
#define         EMI_CONA                (( P_U32)(EMI_BASE+0x000))
#define         EMI_CONB                (( P_U32)(EMI_BASE+0x008))
#define         EMI_CONC                (( P_U32)(EMI_BASE+0x010))
#define         EMI_COND                (( P_U32)(EMI_BASE+0x018))
#define         EMI_CONE                (( P_U32)(EMI_BASE+0x020))
#define         EMI_CONF                (( P_U32)(EMI_BASE+0x028))
#define         EMI_CONG                (( P_U32)(EMI_BASE+0x030))
#define         EMI_CONH                (( P_U32)(EMI_BASE+0x038))
#define         EMI_CONH_2ND            (( P_U32)(EMI_BASE+0x03C))
#define         EMI_CONI                (( P_U32)(EMI_BASE+0x040))
#define         EMI_CONJ                (( P_U32)(EMI_BASE+0x048))
#define         EMI_CONM                (( P_U32)(EMI_BASE+0x060))
#define         EMI_CONN                (( P_U32)(EMI_BASE+0x068))
#define         EMI_CONO                (( P_U32)(EMI_BASE+0x070))
#define         EMI_MDCT                (( P_U32)(EMI_BASE+0x078))
#define         EMI_MDCT_2ND            (( P_U32)(EMI_BASE+0x07C))
#define         EMI_LPCT0               (( P_U32)(EMI_BASE+0x0A0))
#define         EMI_LPCT1               (( P_U32)(EMI_BASE+0x0A4))
#define         EMI_LPCT2               (( P_U32)(EMI_BASE+0x0A8))
#define         EMI_LPCT5               (( P_U32)(EMI_BASE+0x0B4))
#define         EMI_LPCT6               (( P_U32)(EMI_BASE+0x0B8))
#define         EMI_IOCL                (( P_U32)(EMI_BASE+0x0D0))
#define         EMI_IOCL_2ND            (( P_U32)(EMI_BASE+0x0D4))
#define         EMI_IOCM                (( P_U32)(EMI_BASE+0x0D8))
#define         EMI_IOCM_2ND            (( P_U32)(EMI_BASE+0x0DC))
#define         EMI_TESTB               (( P_U32)(EMI_BASE+0x0E8))
#define         EMI_TESTC               (( P_U32)(EMI_BASE+0x0F0))
#define         EMI_TESTD               (( P_U32)(EMI_BASE+0x0F8))
#define         EMI_ARBA                (( P_U32)(EMI_BASE+0x100))
#define         EMI_ARBB                (( P_U32)(EMI_BASE+0x108))
#define         EMI_ARBC                (( P_U32)(EMI_BASE+0x110))
#define         EMI_ARBD                (( P_U32)(EMI_BASE+0x118))
#define         EMI_ARBE                (( P_U32)(EMI_BASE+0x120))
#define         EMI_ARBF                (( P_U32)(EMI_BASE+0x128))
#define         EMI_ARBG                (( P_U32)(EMI_BASE+0x130))
#define         EMI_ARBH                (( P_U32)(EMI_BASE+0x138))
#define         EMI_ARBI                (( P_U32)(EMI_BASE+0x140))
#define         EMI_ARBI_2ND            (( P_U32)(EMI_BASE+0x144))
#define         EMI_ARBK                (( P_U32)(EMI_BASE+0x150))
#define         EMI_ARBK_2ND            (( P_U32)(EMI_BASE+0x154))
#define         EMI_SLCT                (( P_U32)(EMI_BASE+0x158))
#define         EMI_MPUD0_ST            (( P_U32)(EMI_BASE+0x160))
#define         EMI_MPUD1_ST            (( P_U32)(EMI_BASE+0x164))
#define         EMI_MPUD2_ST            (( P_U32)(EMI_BASE+0x168))
#define         EMI_MPUD3_ST            (( P_U32)(EMI_BASE+0x16C))
#define         EMI_MPUD4_ST            (( P_U32)(EMI_BASE+0x170))
#define         EMI_MPUD5_ST            (( P_U32)(EMI_BASE+0x174))
#define         EMI_MPUD6_ST            (( P_U32)(EMI_BASE+0x178))
#define         EMI_MPUD7_ST            (( P_U32)(EMI_BASE+0x17C))
#define         EMI_MPUD8_ST            (( P_U32)(EMI_BASE+0x180))
#define         EMI_MPUD9_ST            (( P_U32)(EMI_BASE+0x184))
#define         EMI_MPUD10_ST           (( P_U32)(EMI_BASE+0x188))
#define         EMI_MPUD11_ST           (( P_U32)(EMI_BASE+0x18C))
#define         EMI_MPUD12_ST           (( P_U32)(EMI_BASE+0x190))
#define         EMI_MPUD13_ST           (( P_U32)(EMI_BASE+0x194))
#define         EMI_MPUD14_ST           (( P_U32)(EMI_BASE+0x198))
#define         EMI_MPUD15_ST           (( P_U32)(EMI_BASE+0x19C))
#define         EMI_MPUD16_ST           (( P_U32)(EMI_BASE+0x1A0))
#define         EMI_MPUD17_ST           (( P_U32)(EMI_BASE+0x1A4))
#define         EMI_MPUD18_ST           (( P_U32)(EMI_BASE+0x1A8))
#define         EMI_MPUD19_ST           (( P_U32)(EMI_BASE+0x1AC))
#define         EMI_MPUD20_ST           (( P_U32)(EMI_BASE+0x1B0))
#define         EMI_MPUD21_ST           (( P_U32)(EMI_BASE+0x1B4))
#define         EMI_MPUD22_ST           (( P_U32)(EMI_BASE+0x1B8))
#define         EMI_MPUD23_ST           (( P_U32)(EMI_BASE+0x1BC))
#define         EMI_MPUD24_ST           (( P_U32)(EMI_BASE+0x1C0))
#define         EMI_MPUD25_ST           (( P_U32)(EMI_BASE+0x1C4))
#define         EMI_MPUD26_ST           (( P_U32)(EMI_BASE+0x1C8))
#define         EMI_MPUD27_ST           (( P_U32)(EMI_BASE+0x1CC))
#define         EMI_MPUD28_ST           (( P_U32)(EMI_BASE+0x1D0))
#define         EMI_MPUD29_ST           (( P_U32)(EMI_BASE+0x1D4))
#define         EMI_MPUD30_ST           (( P_U32)(EMI_BASE+0x1D8))
#define         EMI_MPUD31_ST           (( P_U32)(EMI_BASE+0x1DC))
#define         EMI_MPUS                (( P_U32)(EMI_BASE+0x1F0))
#define         EMI_MPUT                (( P_U32)(EMI_BASE+0x1F8))
#define         EMI_MPUT_2ND            (( P_U32)(EMI_BASE+0x1FC))
#define         EMI_D0_ST2              (( P_U32)(EMI_BASE+0x200))
#define         EMI_D1_ST2              (( P_U32)(EMI_BASE+0x204))
#define         EMI_D2_ST2              (( P_U32)(EMI_BASE+0x208))
#define         EMI_D3_ST2              (( P_U32)(EMI_BASE+0x20C))
#define         EMI_D4_ST2              (( P_U32)(EMI_BASE+0x210))
#define         EMI_D5_ST2              (( P_U32)(EMI_BASE+0x214))
#define         EMI_D6_ST2              (( P_U32)(EMI_BASE+0x218))
#define         EMI_D7_ST2              (( P_U32)(EMI_BASE+0x21C))
#define         EMI_D8_ST2              (( P_U32)(EMI_BASE+0x220))
#define         EMI_D9_ST2              (( P_U32)(EMI_BASE+0x224))
#define         EMI_D10_ST2             (( P_U32)(EMI_BASE+0x228))
#define         EMI_D11_ST2             (( P_U32)(EMI_BASE+0x22C))
#define         EMI_D12_ST2             (( P_U32)(EMI_BASE+0x230))
#define         EMI_D13_ST2             (( P_U32)(EMI_BASE+0x234))
#define         EMI_D14_ST2             (( P_U32)(EMI_BASE+0x238))
#define         EMI_D15_ST2             (( P_U32)(EMI_BASE+0x23C))
#define         EMI_D16_ST2             (( P_U32)(EMI_BASE+0x240))
#define         EMI_D17_ST2             (( P_U32)(EMI_BASE+0x244))
#define         EMI_D18_ST2             (( P_U32)(EMI_BASE+0x248))
#define         EMI_D19_ST2             (( P_U32)(EMI_BASE+0x24C))
#define         EMI_D20_ST2             (( P_U32)(EMI_BASE+0x250))
#define         EMI_D21_ST2             (( P_U32)(EMI_BASE+0x254))
#define         EMI_D22_ST2             (( P_U32)(EMI_BASE+0x258))
#define         EMI_D23_ST2             (( P_U32)(EMI_BASE+0x25C))
#define         EMI_D24_ST2             (( P_U32)(EMI_BASE+0x260))
#define         EMI_D25_ST2             (( P_U32)(EMI_BASE+0x264))
#define         EMI_D26_ST2             (( P_U32)(EMI_BASE+0x268))
#define         EMI_D27_ST2             (( P_U32)(EMI_BASE+0x26C))
#define         EMI_D28_ST2             (( P_U32)(EMI_BASE+0x270))
#define         EMI_D29_ST2             (( P_U32)(EMI_BASE+0x274))
#define         EMI_D30_ST2             (( P_U32)(EMI_BASE+0x278))
#define         EMI_D31_ST2             (( P_U32)(EMI_BASE+0x27C))
#define         EMI_BMEN                (( P_U32)(EMI_BASE+0x400))
#define         EMI_BSTP                (( P_U32)(EMI_BASE+0x404))
#define         EMI_BCNT                (( P_U32)(EMI_BASE+0x408))
#define         EMI_TACT                (( P_U32)(EMI_BASE+0x410))
#define         EMI_TSCT                (( P_U32)(EMI_BASE+0x418))
#define         EMI_WACT                (( P_U32)(EMI_BASE+0x420))
#define         EMI_WSCT                (( P_U32)(EMI_BASE+0x428))
#define         EMI_BACT                (( P_U32)(EMI_BASE+0x430))
#define         EMI_BSCT                (( P_U32)(EMI_BASE+0x438))
#define         EMI_MSEL                (( P_U32)(EMI_BASE+0x440))
#define         EMI_TSCT2               (( P_U32)(EMI_BASE+0x448))
#define         EMI_TSCT3               (( P_U32)(EMI_BASE+0x450))
#define         EMI_WSCT2               (( P_U32)(EMI_BASE+0x458))
#define         EMI_WSCT3               (( P_U32)(EMI_BASE+0x460))
#define         EMI_WSCT4               (( P_U32)(EMI_BASE+0x464))
#define         EMI_MSEL2               (( P_U32)(EMI_BASE+0x468))
#define         EMI_MSEL3               (( P_U32)(EMI_BASE+0x470))
#define         EMI_MSEL4               (( P_U32)(EMI_BASE+0x478))
#define         EMI_MSEL5               (( P_U32)(EMI_BASE+0x480))
#define         EMI_MSEL6               (( P_U32)(EMI_BASE+0x488))
#define         EMI_MSEL7               (( P_U32)(EMI_BASE+0x490))
#define         EMI_MSEL8               (( P_U32)(EMI_BASE+0x498))
#define         EMI_MSEL9               (( P_U32)(EMI_BASE+0x4A0))
#define         EMI_MSEL10              (( P_U32)(EMI_BASE+0x4A8))
#define         EMI_BMID0               (( P_U32)(EMI_BASE+0x4B0))
#define         EMI_BMID1               (( P_U32)(EMI_BASE+0x4B4))
#define         EMI_BMID2               (( P_U32)(EMI_BASE+0x4B8))
#define         EMI_BMID3               (( P_U32)(EMI_BASE+0x4BC))
#define         EMI_BMID4               (( P_U32)(EMI_BASE+0x4C0))
#define         EMI_BMID5               (( P_U32)(EMI_BASE+0x4C4))
#define         EMI_BMID6               (( P_U32)(EMI_BASE+0x4C8))
#define         EMI_BMID7               (( P_U32)(EMI_BASE+0x4CC))
#define         EMI_BMID8               (( P_U32)(EMI_BASE+0x4D0))
#define         EMI_BMID9               (( P_U32)(EMI_BASE+0x4D4))
#define         EMI_BMID10              (( P_U32)(EMI_BASE+0x4D8))
#define         EMI_BMEN1               (( P_U32)(EMI_BASE+0x4E0))
#define         EMI_BMEN2               (( P_U32)(EMI_BASE+0x4E8))
#define         EMI_BMRW0               (( P_U32)(EMI_BASE+0x4F8))
#define         EMI_BMRW1               (( P_U32)(EMI_BASE+0x4FC))
#define         EMI_TTYPE1              (( P_U32)(EMI_BASE+0x500))
#define         EMI_TTYPE2              (( P_U32)(EMI_BASE+0x508))
#define         EMI_TTYPE3              (( P_U32)(EMI_BASE+0x510))
#define         EMI_TTYPE4              (( P_U32)(EMI_BASE+0x518))
#define         EMI_TTYPE5              (( P_U32)(EMI_BASE+0x520))
#define         EMI_TTYPE6              (( P_U32)(EMI_BASE+0x528))
#define         EMI_TTYPE7              (( P_U32)(EMI_BASE+0x530))
#define         EMI_TTYPE8              (( P_U32)(EMI_BASE+0x538))
#define         EMI_TTYPE9              (( P_U32)(EMI_BASE+0x540))
#define         EMI_TTYPE10             (( P_U32)(EMI_BASE+0x548))
#define         EMI_TTYPE11             (( P_U32)(EMI_BASE+0x550))
#define         EMI_TTYPE12             (( P_U32)(EMI_BASE+0x558))
#define         EMI_TTYPE13             (( P_U32)(EMI_BASE+0x560))
#define         EMI_TTYPE14             (( P_U32)(EMI_BASE+0x568))
#define         EMI_TTYPE15             (( P_U32)(EMI_BASE+0x570))
#define         EMI_TTYPE16             (( P_U32)(EMI_BASE+0x578))
#define         EMI_TTYPE17             (( P_U32)(EMI_BASE+0x580))
#define         EMI_TTYPE18             (( P_U32)(EMI_BASE+0x588))
#define         EMI_TTYPE19             (( P_U32)(EMI_BASE+0x590))
#define         EMI_TTYPE20             (( P_U32)(EMI_BASE+0x598))
#define         EMI_TTYPE21             (( P_U32)(EMI_BASE+0x5A0))
#define         EMI_BWCT0               (( P_U32)(EMI_BASE+0x5B0))
#define         EMI_BWCT1               (( P_U32)(EMI_BASE+0x5B4))
#define         EMI_BWCT2               (( P_U32)(EMI_BASE+0x5B8))
#define         EMI_BWCT3               (( P_U32)(EMI_BASE+0x5BC))
#define         EMI_BWCT4               (( P_U32)(EMI_BASE+0x5C0))
#define         EMI_BWST0               (( P_U32)(EMI_BASE+0x5C4))
#define         EMI_BWST1               (( P_U32)(EMI_BASE+0x5C8))
#define         EMI_EX_CON              (( P_U32)(EMI_BASE+0x5D0))
#define         EMI_EX_ST0              (( P_U32)(EMI_BASE+0x5D4))
#define         EMI_EX_ST1              (( P_U32)(EMI_BASE+0x5D8))
#define         EMI_EX_ST2              (( P_U32)(EMI_BASE+0x5DC))
#define         EMI_WP_ADR              (( P_U32)(EMI_BASE+0x5E0))
#define         EMI_WP_ADR_2ND          (( P_U32)(EMI_BASE+0x5E4))
#define         EMI_WP_CTRL             (( P_U32)(EMI_BASE+0x5E8))
#define         EMI_CHKER               (( P_U32)(EMI_BASE+0x5F0))
#define         EMI_CHKER_TYPE          (( P_U32)(EMI_BASE+0x5F4))
#define         EMI_CHKER_ADR           (( P_U32)(EMI_BASE+0x5F8))
#define         EMI_CHKER_ADR_2ND       (( P_U32)(EMI_BASE+0x5FC))
#define         EMI_SHF1                (( P_U32)(EMI_BASE+0x718))
#define         EMI_SLVA                (( P_U32)(EMI_BASE+0x800))

#define         EMI_MPU_CTRL                 (EMI_MPU_BASE+0x000)
#define         EMI_MPU_DBG                  (EMI_MPU_BASE+0x004)
#define         EMI_MPU_SA0                  (EMI_MPU_BASE+0x100)
#define         EMI_MPU_EA0                  (EMI_MPU_BASE+0x200)
#define         EMI_MPU_SA(region)           (EMI_MPU_SA0 + (region*4))
#define         EMI_MPU_EA(region)           (EMI_MPU_EA0 + (region*4))
#define         EMI_MPU_APC0                 (EMI_MPU_BASE+0x300)
#define         EMI_MPU_APC(region, domain)  (EMI_MPU_APC0 + (region*4) + ((domain/8)*0x100))
#define         EMI_MPU_CTRL_D0              (EMI_MPU_BASE+0x800)
#define         EMI_MPU_CTRL_D(domain)       (EMI_MPU_CTRL_D0 + (domain*4))
#define         EMI_RG_MASK_D0               (EMI_MPU_BASE+0x900)
#define         EMI_RG_MASK_D(domain)        (EMI_RG_MASK_D0 + (domain*4))

#define         EMI_CONA_CH0                (( P_U32)(CHN0_EMI_BASE+0x000))
#define         EMI_CONC_CH0                (( P_U32)(CHN0_EMI_BASE+0x010))


#define EMI_MPU_REGION_NUMBER	(24)
#define EMI_MPU_DOMAIN_NUMBER	(8)



#define         DRAMC_ACTIM0            (0x0)           /* DRAM AC TIMING SETTING 0 */
#define         DRAMC_CONF1             (0x4)           /* DRAM CONFIGURATION 1 */
#define         DRAMC_CONF2             (0x8)           /* DRAM CONFIGURATION 2 */
#define         DRAMC_PADCTL1           (0xc)           /* DRAM PAD CONTROL 1 */
#define         DRAMC_PADCTL2           (0x10)          /* DRAM PAD CONTROL 2 */
#define         DRAMC_PADCTL3           (0x14)          /* DRAM PAD CONTROL 3 */
#define         DRAMC_R0DELDLY          (0x18)          /* RANK0 DQS INPUT DELAY CHAIN SETTING */
#define         DRAMC_R1DELDLY          (0x1c)          /* RANK1 DQS INPUT DELAY CHAIN SETTING */
#define         DRAMC_R0DIFDLY          (0x20)          /* RANK0 DQS INPUT DELAY CHAIN OFFSET SETTING */
#define         DRAMC_R1DIFDLY          (0x24)          /* RANK1 DQS INPUT DELAY CHAIN OFFSET SETTING */
#define         DRAMC_DLLCONF           (0x28)          /* DLL CONFIGURATION */
#define         DRAMC_TESTMODE          (0x2c)          /* TEST MODE CONFIGURATION 1 */
#define         DRAMC_TEST2_1           (0x3c)          /* TEST AGENT 2 CONFIGURATION 1 */
#define         DRAMC_TEST2_2           (0x40)          /* TEST AGENT 2 CONFIGURATION 2 */
#define         DRAMC_TEST2_3           (0x44)          /* TEST AGENT 2 CONFIGURATION 3 */
#define         DRAMC_TEST2_4           (0x48)          /* TEST AGENT 2 CONFIGURATION 4 */
#define         DRAMC_DDR2CTL           (0x7c)          /* DDR2 CONTROL REGISTER */
#define         DRAMC_MRS               (0x88)          /* MRS value setting */
#define         DRAMC_CLK1DELAY         (0x8c)          /* Clock 1 output delay CONTROL */
#define         DRAMC_IOCTL             (0x90)          /* IO CONTROL */
#define         DRAMC_R0DQSIEN          (0x94)          /* RANK0 DQS INPUT RANGE FINE TUNER */
#define         DRAMC_R1DQSIEN          (0x98)          /* RANK1 DQS INPUT RANGE FINE TUNER */
#define         DRAMC_DRVCTL00          (0xb4)          /* PAD DRIVING CONTROL SETTING 00 */
#define         DRAMC_DRVCTL0           (0xb8)          /* PAD DRIVING CONTROL SETTING 0 */
#define         DRAMC_DRVCTL1           (0xbc)          /* PAD DRIVING CONTROL SETTING 1 */
#define         DRAMC_DLLSEL            (0xc0)          /* DLL SELECTION SETTING */
#define         DRAMC_TDSEL0            (0xcc)          /* IO OUTPUT DUTY CONTROL 0 */
#define         DRAMC_TDSEL1            (0xd0)          /* IO OUTPUT DUTY CONTROL 1 */
#define         DRAMC_MCKDLY            (0xd8)          /* MEMORY CLOCK DELAY CHAIN SETTING */
#define         DRAMC_DQSCTL0           (0xdc)          /* DQS INPUT RANGE CONTROL 0 */
#define         DRAMC_DQSCTL1           (0xe0)          /* DQS INPUT RANGE CONTROL 1 */
#define         DRAMC_PADCTL4           (0xe4)          /* PAD CONTROL 1 */
#define         DRAMC_PADCTL5           (0xe8)          /* PAD CONTROL 2 */
#define         DRAMC_PADCTL6           (0xec)          /* PAD CONTROL 3 */
#define         DRAMC_PHYCTL1           (0xf0)          /* DDR PHY CONTROL 1 */
#define         DRAMC_GDDR3CTL1         (0xf4)          /* GDDR3 CONTROL 1 */
#define         DRAMC_PADCTL7           (0xf8)          /* PAD CONTROL 4 */
#define         DRAMC_MISCTL0           (0xfc)          /* MISC CONTROL 0 */
#define         DRAMC_OCDK              (0x100)         /* OCD CALIBRATION CONTROL */
#define         DRAMC_LBWDAT0           (0x104)         /* LOOP BACK DATA 0 */
#define         DRAMC_LBWDAT1           (0x108)         /* LOOP BACK DATA 1 */
#define         DRAMC_LBWDAT2           (0x10c)         /* LOOP BACK DATA 2 */
#define         DRAMC_RKCFG             (0x110)         /* RANK CONFIGURATION */
#define         DRAMC_CKPHDET           (0x114)         /* CLOCK PHASE DETECTION SETTING */
#define         DRAMC_DQSGCTL           (0x124)         /* INPUT DQS GATING CONTROL */
#define         DRAMC_CLKENCTL          (0x130)         /* DRAM CLOCK ENABLE CONTROL */
#define         DRAMC_DQSGCTL1          (0x140)         /* DQS gating delay control 1 */
#define         DRAMC_DQSGCTL2          (0x144)         /* DQS gating delay control 2 */
#define         DRAMC_ARBCTL0           (0x168)         /* ARBITRATION CONTROL 0 */
#define         DRAMC_CMDDLY0           (0x1a8)         /* Command Delay CTL0 */
#define         DRAMC_CMDDLY1           (0x1ac)         /* Command Delay CTL1 */
#define         DRAMC_CMDDLY2           (0x1b0)         /* Command Delay CTL2 */
#define         DRAMC_CMDDLY3           (0x1b4)         /* Command Delay CTL3 */
#define         DRAMC_CMDDLY4           (0x1b8)         /* Command Delay CTL4 */
#define         DRAMC_CMDDLY5           (0x1bc)         /* Command Delay CTL5 */
#define         DRAMC_DQSCAL0           (0x1c0)         /* DQS CAL CONTROL 0 */
#define         DRAMC_DQSCAL1           (0x1c4)         /* DQS CAL CONTROL 1 */
#define         DRAMC_DMMonitor         (0x1d8)         /* Monitor parameter */
#define         DRAMC_DRAMC_PD_CTRL             (0x1dc)         /* PD mode parameter */
#define         DRAMC_LPDDR2            (0x1e0)         /* LPDDR2 setting */
#define         DRAMC_SPCMD             (0x1e4)         /* Special command mode */
#define         DRAMC_ACTIM1            (0x1e8)         /* DRAM AC TIMING SETTING 1 */
#define         DRAMC_PERFCTL0          (0x1ec)         /* PERFORMANCE CONTROL 0 */
#define         DRAMC_AC_DERATING               (0x1f0)         /* AC TIME DERATING CONTROL */
#define         DRAMC_RRRATE_CTL                (0x1f4)         /* REFRESH RATE CONTROL */
#define         DRAMC_DQODLY1           (0x200)         /* DQ output DELAY1 CHAIN setting  */
#define         DRAMC_DQODLY2           (0x204)         /* DQ output DELAY2 CHAIN setting  */
#define         DRAMC_DQODLY3           (0x208)         /* DQ output DELAY3 CHAIN setting  */
#define         DRAMC_DQODLY4           (0x20c)         /* DQ output DELAY4 CHAIN setting  */
#define         DRAMC_DQIDLY1           (0x210)         /* DQ input DELAY1 CHAIN setting  */
#define         DRAMC_DQIDLY2           (0x214)         /* DQ input DELAY2 CHAIN setting  */
#define         DRAMC_DQIDLY3           (0x218)         /* DQ input DELAY3 CHAIN setting  */
#define         DRAMC_DQIDLY4           (0x21c)         /* DQ input DELAY4 CHAIN setting  */
#define         DRAMC_DQIDLY5           (0x220)         /* DQ input DELAY5 CHAIN setting  */
#define         DRAMC_DQIDLY6           (0x224)         /* DQ input DELAY6 CHAIN setting  */
#define         DRAMC_DQIDLY7           (0x228)         /* DQ input DELAY7 CHAIN setting  */
#define         DRAMC_DQIDLY8           (0x22c)         /* DQ input DELAY8 CHAIN setting  */
#define         DRAMC_R2R_page_hit_counter              (0x280)         /* R2R_page_hit_counter */
#define         DRAMC_R2R_page_miss_counter             (0x284)         /* R2R_page_miss_counter */
#define         DRAMC_R2R_interbank_counter             (0x288)         /* R2R_interbank_counter */
#define         DRAMC_R2W_page_hit_counter              (0x28c)         /* R2W_page_hit_counter */
#define         DRAMC_R2W_page_miss_counter             (0x290)         /* R2W_page_miss_counter */
#define         DRAMC_R2W_interbank_counter             (0x294)         /* R2W_interbank_counter */
#define         DRAMC_W2R_page_hit_counter              (0x298)         /* W2R_page_hit_counter */
#define         DRAMC_W2R_page_miss_counter             (0x29c)         /* W2R_page_miss_counter */
#define         DRAMC_W2R_interbank_counter             (0x2a0)         /* W2R_interbank_counter */
#define         DRAMC_W2W_page_hit_counter              (0x2a4)         /* W2W_page_hit_counter */
#define         DRAMC_W2W_page_miss_counter             (0x2a8)         /* W2W_page_miss_counter */
#define         DRAMC_W2W_interbank_counter             (0x2ac)         /* W2W_interbank_counter */
#define         DRAMC_dramc_idle_counter                (0x2b0)         /* dramc_idle_counter */
#define         DRAMC_freerun_26m_counter               (0x2b4)         /* freerun_26m_counter */
#define         DRAMC_refresh_pop_counter               (0x2b8)         /* refresh_pop_counter */
#define         DRAMC_JMETER_ST         (0x2bc)         /* Jitter Meter Status */
#define         DRAMC_DQ_CAL_MAX_0              (0x2c0)         /* DQ INPUT CALIBRATION per bit 3-0 */
#define         DRAMC_DQ_CAL_MAX_1              (0x2c4)         /* DQ INPUT CALIBRATION per bit 7-4 */
#define         DRAMC_DQ_CAL_MAX_2              (0x2c8)         /* DQ INPUT CALIBRATION per bit 11-8 */
#define         DRAMC_DQ_CAL_MAX_3              (0x2cc)         /* DQ INPUT CALIBRATION per bit 15-12 */
#define         DRAMC_DQ_CAL_MAX_4              (0x2d0)         /* DQ INPUT CALIBRATION per bit 19-16 */
#define         DRAMC_DQ_CAL_MAX_5              (0x2d4)         /* DQ INPUT CALIBRATION per bit 23-20 */
#define         DRAMC_DQ_CAL_MAX_6              (0x2d8)         /* DQ INPUT CALIBRATION per bit 27-34 */
#define         DRAMC_DQ_CAL_MAX_7              (0x2dc)         /* DQ INPUT CALIBRATION per bit 31-28 */
#define         DRAMC_DQS_CAL_MIN_0             (0x2e0)         /* DQS INPUT CALIBRATION per bit 3-0 */
#define         DRAMC_DQS_CAL_MIN_1             (0x2e4)         /* DQS INPUT CALIBRATION per bit 7-4 */
#define         DRAMC_DQS_CAL_MIN_2             (0x2e8)         /* DQS INPUT CALIBRATION per bit 11-8 */
#define         DRAMC_DQS_CAL_MIN_3             (0x2ec)         /* DQS INPUT CALIBRATION per bit 15-12 */
#define         DRAMC_DQS_CAL_MIN_4             (0x2f0)         /* DQS INPUT CALIBRATION per bit 19-16 */
#define         DRAMC_DQS_CAL_MIN_5             (0x2f4)         /* DQS INPUT CALIBRATION per bit 23-20 */
#define         DRAMC_DQS_CAL_MIN_6             (0x2f8)         /* DQS INPUT CALIBRATION per bit 27-34 */
#define         DRAMC_DQS_CAL_MIN_7             (0x2fc)         /* DQS INPUT CALIBRATION per bit 31-28 */
#define         DRAMC_DQS_CAL_MAX_0             (0x300)         /* DQS INPUT CALIBRATION per bit 3-0 */
#define         DRAMC_DQS_CAL_MAX_1             (0x304)         /* DQS INPUT CALIBRATION per bit 7-4 */
#define         DRAMC_DQS_CAL_MAX_2             (0x308)         /* DQS INPUT CALIBRATION per bit 11-8 */
#define         DRAMC_DQS_CAL_MAX_3             (0x30c)         /* DQS INPUT CALIBRATION per bit 15-12 */
#define         DRAMC_DQS_CAL_MAX_4             (0x310)         /* DQS INPUT CALIBRATION per bit 19-16 */
#define         DRAMC_DQS_CAL_MAX_5             (0x314)         /* DQS INPUT CALIBRATION per bit 23-20 */
#define         DRAMC_DQS_CAL_MAX_6             (0x318)         /* DQS INPUT CALIBRATION per bit 27-34 */
#define         DRAMC_DQS_CAL_MAX_7             (0x31c)         /* DQS INPUT CALIBRATION per bit 31-28 */
#define         DRAMC_DQICAL0           (0x350)         /* DQS INPUT CALIBRATION 0 */
#define         DRAMC_DQICAL1           (0x354)         /* DQS INPUT CALIBRATION 1 */
#define         DRAMC_DQICAL2           (0x358)         /* DQS INPUT CALIBRATION 2 */
#define         DRAMC_DQICAL3           (0x35c)         /* DQS INPUT CALIBRATION 3 */
#define         DRAMC_CMP_ERR           (0x370)         /* CMP ERROR */
#define         DRAMC_R0DQSIENDLY               (0x374)         /* RANK0 DQS INPUT GATING DELAY VALUE */
#define         DRAMC_R1DQSIENDLY               (0x378)         /* RANK1 DQS INPUT GATING DELAY VALUE */
#define         DRAMC_STBEN0            (0x38c)         /* DQS RING COUNTER 0 */
#define         DRAMC_STBEN1            (0x390)         /* DQS RING COUNTER 1 */
#define         DRAMC_STBEN2            (0x394)         /* DQS RING COUNTER 2 */
#define         DRAMC_STBEN3            (0x398)         /* DQS RING COUNTER 3 */
#define         DRAMC_DQSDLY0           (0x3a0)         /* DQS INPUT DELAY SETTING 0 */
#define         DRAMC_SPCMDRESP         (0x3b8)         /* SPECIAL COMMAND RESPONSE */
#define         DRAMC_IORGCNT           (0x3bc)         /* IO RING COUNTER */
#define         DRAMC_DQSGNWCNT0                (0x3c0)         /* DQS GATING WINODW COUNTER 0 */
#define         DRAMC_DQSGNWCNT1                (0x3c4)         /* DQS GATING WINODW COUNTER 1 */
#define         DRAMC_DQSGNWCNT2                (0x3c8)         /* DQS GATING WINODW COUNTER 2 */
#define         DRAMC_DQSGNWCNT3                (0x3cc)         /* DQS GATING WINODW COUNTER 3 */
#define         DRAMC_DQSGNWCNT4                (0x3d0)         /* DQS GATING WINODW COUNTER 4 */
#define         DRAMC_DQSGNWCNT5                (0x3d4)         /* DQS GATING WINODW COUNTER 5 */
#define         DRAMC_DQSSAMPLEV                (0x3d8)         /* DQS SAMPLE VALUE */
#define         DRAMC_DLLCNT0           (0x3dc)         /* DLL STATUS 0 */
#define         DRAMC_CKPHCNT           (0x3e8)         /* CLOCK PHASE DETECTION RESULT */
#define         DRAMC_TESTRPT           (0x3fc)         /* TEST AGENT STATUS */
#define         DRAMC_MEMPLL0           (0x600)         /* MEMPLL REGISTER SETTING 0 */
#define         DRAMC_MEMPLL1           (0x604)         /* MEMPLL REGISTER SETTING 1 */
#define         DRAMC_MEMPLL2           (0x608)         /* MEMPLL REGISTER SETTING 2 */
#define         DRAMC_MEMPLL3           (0x60c)         /* MEMPLL REGISTER SETTING 3 */
#define         DRAMC_MEMPLL4           (0x610)         /* MEMPLL REGISTER SETTING 4 */
#define         DRAMC_MEMPLL5           (0x614)         /* MEMPLL REGISTER SETTING 5 */
#define         DRAMC_MEMPLL6           (0x618)         /* MEMPLL REGISTER SETTING 6 */
#define         DRAMC_MEMPLL7           (0x61c)         /* MEMPLL REGISTER SETTING 7 */
#define         DRAMC_MEMPLL8           (0x620)         /* MEMPLL REGISTER SETTING 8 */
#define         DRAMC_MEMPLL9           (0x624)         /* MEMPLL REGISTER SETTING 9 */
#define         DRAMC_MEMPLL10          (0x628)         /* MEMPLL REGISTER SETTING 10 */
#define         DRAMC_MEMPLL11          (0x62c)         /* MEMPLL REGISTER SETTING 11 */
#define         DRAMC_MEMPLL12          (0x630)         /* MEMPLL REGISTER SETTING 12 */
#define         DRAMC_MEMPLL_DIVIDER            (0x640)         /* MEMPLL DIVIDER REGISTER CONTROL */
#define         DRAMC_VREFCTL0          (0x644)         /* VREF REGISTER SETTING 0 */


#define         DRAMC_ACTIM0_DEFAULT            0x22564154
#define         DRAMC_CONF1_DEFAULT             0x00000000
#define         DRAMC_CONF2_DEFAULT             0x00000000
#define         DRAMC_PADCTL1_DEFAULT           0x00000000
#define         DRAMC_PADCTL2_DEFAULT           0x00000000
#define         DRAMC_PADCTL3_DEFAULT           0x00000000
#define         DRAMC_R0DELDLY_DEFAULT          0x00000000
#define         DRAMC_R1DELDLY_DEFAULT          0x00000000
#define         DRAMC_R0DIFDLY_DEFAULT          0x00000000
#define         DRAMC_R1DIFDLY_DEFAULT          0x00000000
#define         DRAMC_DLLCONF_DEFAULT           0x8000FF01
#define         DRAMC_TESTMODE_DEFAULT          0x55010000
#define         DRAMC_TEST2_1_DEFAULT           0x01200000
#define         DRAMC_TEST2_2_DEFAULT           0x00010000
#define         DRAMC_TEST2_3_DEFAULT           0x00000000
#define         DRAMC_TEST2_4_DEFAULT           0x0000110D
#define         DRAMC_DDR2CTL_DEFAULT           0x00000000
#define         DRAMC_MRS_DEFAULT               0x00000000
#define         DRAMC_CLK1DELAY_DEFAULT         0x00000000
#define         DRAMC_IOCTL_DEFAULT             0x00000000
#define         DRAMC_R0DQSIEN_DEFAULT          0x00000000
#define         DRAMC_R1DQSIEN_DEFAULT          0x00000000
#define         DRAMC_DRVCTL00_DEFAULT          0xAA22AA22
#define         DRAMC_DRVCTL0_DEFAULT           0xAA22AA22
#define         DRAMC_DRVCTL1_DEFAULT           0xAA22AA22
#define         DRAMC_DLLSEL_DEFAULT            0x00000000
#define         DRAMC_TDSEL0_DEFAULT            0x00000000
#define         DRAMC_TDSEL1_DEFAULT            0x00000000
#define         DRAMC_MCKDLY_DEFAULT            0x00000900
#define         DRAMC_DQSCTL0_DEFAULT           0x00000000
#define         DRAMC_DQSCTL1_DEFAULT           0x00000000
#define         DRAMC_PADCTL4_DEFAULT           0x00000000
#define         DRAMC_PADCTL5_DEFAULT           0x00000000
#define         DRAMC_PADCTL6_DEFAULT           0x00000000
#define         DRAMC_PHYCTL1_DEFAULT           0x00000000
#define         DRAMC_GDDR3CTL1_DEFAULT         0x00000000
#define         DRAMC_PADCTL7_DEFAULT           0xEDCB0000
#define         DRAMC_MISCTL0_DEFAULT           0x00000000
#define         DRAMC_OCDK_DEFAULT              0x00000000
#define         DRAMC_LBWDAT0_DEFAULT           0x00000000
#define         DRAMC_LBWDAT1_DEFAULT           0x00000000
#define         DRAMC_LBWDAT2_DEFAULT           0x00000000
#define         DRAMC_RKCFG_DEFAULT             0x0B051100
#define         DRAMC_CKPHDET_DEFAULT           0x00000000
#define         DRAMC_DQSGCTL_DEFAULT           0xAA080088
#define         DRAMC_CLKENCTL_DEFAULT          0x50000000
#define         DRAMC_DQSGCTL1_DEFAULT          0x00000000
#define         DRAMC_DQSGCTL2_DEFAULT          0x00000000
#define         DRAMC_ARBCTL0_DEFAULT           0x00000000
#define         DRAMC_CMDDLY0_DEFAULT           0x00000000
#define         DRAMC_CMDDLY1_DEFAULT           0x00000000
#define         DRAMC_CMDDLY2_DEFAULT           0x00000000
#define         DRAMC_CMDDLY3_DEFAULT           0x00000000
#define         DRAMC_CMDDLY4_DEFAULT           0x00000000
#define         DRAMC_CMDDLY5_DEFAULT           0x00000000
#define         DRAMC_DQSCAL0_DEFAULT           0x00000000
#define         DRAMC_DQSCAL1_DEFAULT           0x00000000
#define         DRAMC_DMMonitor_DEFAULT         0x00C80000
#define         DRAMC_DRAMC_PD_CTRL_DEFAULT             0x10622842
#define         DRAMC_LPDDR2_DEFAULT            0x00000000
#define         DRAMC_SPCMD_DEFAULT             0x00000000
#define         DRAMC_ACTIM1_DEFAULT            0x00000000
#define         DRAMC_PERFCTL0_DEFAULT          0x00000000
#define         DRAMC_AC_DERATING_DEFAULT               0x00000000
#define         DRAMC_RRRATE_CTL_DEFAULT                0x00020100
#define         DRAMC_DQODLY1_DEFAULT           0x00000000
#define         DRAMC_DQODLY2_DEFAULT           0x00000000
#define         DRAMC_DQODLY3_DEFAULT           0x00000000
#define         DRAMC_DQODLY4_DEFAULT           0x00000000
#define         DRAMC_DQIDLY1_DEFAULT           0x00000000
#define         DRAMC_DQIDLY2_DEFAULT           0x00000000
#define         DRAMC_DQIDLY3_DEFAULT           0x00000000
#define         DRAMC_DQIDLY4_DEFAULT           0x00000000
#define         DRAMC_DQIDLY5_DEFAULT           0x00000000
#define         DRAMC_DQIDLY6_DEFAULT           0x00000000
#define         DRAMC_DQIDLY7_DEFAULT           0x00000000
#define         DRAMC_DQIDLY8_DEFAULT           0x00000000
#define         DRAMC_R2R_page_hit_counter_DEFAULT              0x00000000
#define         DRAMC_R2R_page_miss_counter_DEFAULT             0x00000000
#define         DRAMC_R2R_interbank_counter_DEFAULT             0x00000000
#define         DRAMC_R2W_page_hit_counter_DEFAULT              0x00000000
#define         DRAMC_R2W_page_miss_counter_DEFAULT             0x00000000
#define         DRAMC_R2W_interbank_counter_DEFAULT             0x00000000
#define         DRAMC_W2R_page_hit_counter_DEFAULT              0x00000000
#define         DRAMC_W2R_page_miss_counter_DEFAULT             0x00000000
#define         DRAMC_W2R_interbank_counter_DEFAULT             0x00000000
#define         DRAMC_W2W_page_hit_counter_DEFAULT              0x00000000
#define         DRAMC_W2W_page_miss_counter_DEFAULT             0x00000000
#define         DRAMC_W2W_interbank_counter_DEFAULT             0x00000000
#define         DRAMC_dramc_idle_counter_DEFAULT                0x00000000
#define         DRAMC_freerun_26m_counter_DEFAULT               0x00000000
#define         DRAMC_refresh_pop_counter_DEFAULT               0x00000000
#define         DRAMC_JMETER_ST_DEFAULT         0x00000000
#define         DRAMC_DQ_CAL_MAX_0_DEFAULT              0x00000000
#define         DRAMC_DQ_CAL_MAX_1_DEFAULT              0x00000000
#define         DRAMC_DQ_CAL_MAX_2_DEFAULT              0x00000000
#define         DRAMC_DQ_CAL_MAX_3_DEFAULT              0x00000000
#define         DRAMC_DQ_CAL_MAX_4_DEFAULT              0x00000000
#define         DRAMC_DQ_CAL_MAX_5_DEFAULT              0x00000000
#define         DRAMC_DQ_CAL_MAX_6_DEFAULT              0x00000000
#define         DRAMC_DQ_CAL_MAX_7_DEFAULT              0x00000000
#define         DRAMC_DQS_CAL_MIN_0_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MIN_1_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MIN_2_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MIN_3_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MIN_4_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MIN_5_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MIN_6_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MIN_7_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MAX_0_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MAX_1_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MAX_2_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MAX_3_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MAX_4_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MAX_5_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MAX_6_DEFAULT             0x00000000
#define         DRAMC_DQS_CAL_MAX_7_DEFAULT             0x00000000
#define         DRAMC_DQICAL0_DEFAULT           0x00000000
#define         DRAMC_DQICAL1_DEFAULT           0x00000000
#define         DRAMC_DQICAL2_DEFAULT           0x00000000
#define         DRAMC_DQICAL3_DEFAULT           0x00000000
#define         DRAMC_CMP_ERR_DEFAULT           0x00000000
#define         DRAMC_R0DQSIENDLY_DEFAULT               0x00000000
#define         DRAMC_R1DQSIENDLY_DEFAULT               0x00000000
#define         DRAMC_STBEN0_DEFAULT            0x00000003
#define         DRAMC_STBEN1_DEFAULT            0x00000003
#define         DRAMC_STBEN2_DEFAULT            0x00000003
#define         DRAMC_STBEN3_DEFAULT            0x00000003
#define         DRAMC_DQSDLY0_DEFAULT           0x0F0F0F0F
#define         DRAMC_SPCMDRESP_DEFAULT         0x00010300
#define         DRAMC_IORGCNT_DEFAULT           0x00000000
#define         DRAMC_DQSGNWCNT0_DEFAULT                0x00000000
#define         DRAMC_DQSGNWCNT1_DEFAULT                0x00000000
#define         DRAMC_DQSGNWCNT2_DEFAULT                0x00000000
#define         DRAMC_DQSGNWCNT3_DEFAULT                0x00000000
#define         DRAMC_DQSGNWCNT4_DEFAULT                0x00000000
#define         DRAMC_DQSGNWCNT5_DEFAULT                0x00000000
#define         DRAMC_DQSSAMPLEV_DEFAULT                0x00000000
#define         DRAMC_DLLCNT0_DEFAULT           0x00000000
#define         DRAMC_CKPHCNT_DEFAULT           0x00000000
#define         DRAMC_TESTRPT_DEFAULT           0x00000000
#define         DRAMC_MEMPLL0_DEFAULT           0x044C6000
#define         DRAMC_MEMPLL1_DEFAULT           0x00002000
#define         DRAMC_MEMPLL2_DEFAULT           0x0000044C
#define         DRAMC_MEMPLL3_DEFAULT           0x60000000
#define         DRAMC_MEMPLL4_DEFAULT           0x0000044C
#define         DRAMC_MEMPLL5_DEFAULT           0x60000000
#define         DRAMC_MEMPLL6_DEFAULT           0x0000044C
#define         DRAMC_MEMPLL7_DEFAULT           0x60000000
#define         DRAMC_MEMPLL8_DEFAULT           0x00000000
#define         DRAMC_MEMPLL9_DEFAULT           0x00000000
#define         DRAMC_MEMPLL10_DEFAULT          0x00000000
#define         DRAMC_MEMPLL11_DEFAULT          0x00000000
#define         DRAMC_MEMPLL12_DEFAULT          0x00000000
#define         DRAMC_MEMPLL_DIVIDER_DEFAULT            0x00000003
#define         DRAMC_VREFCTL0_DEFAULT          0x00000000


#endif	// __EMI_HW_H__
