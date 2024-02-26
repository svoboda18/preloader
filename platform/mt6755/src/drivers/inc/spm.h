#ifndef _SPM_
#define _SPM_

/* 
//XXX: only in kernel
#include <linux/kernel.h>
#include <linux/io.h>

#include <mach/mt_reg_base.h>
#include <mach/mt_irq.h>
#include <mach/sync_write.h>
*/
#include <platform.h>
#include <typedefs.h>
#include <sync_write.h>

/**************************************
 * Config and Parameter
 **************************************/
//#define SPM_BASE		SLEEP_BASE
#define SPM_IRQ0_ID		SLEEP_IRQ_BIT0_ID
#define SPM_IRQ1_ID		SLEEP_IRQ_BIT1_ID
#define SPM_IRQ2_ID		SLEEP_IRQ_BIT2_ID
#define SPM_IRQ3_ID		SLEEP_IRQ_BIT3_ID
#define SPM_IRQ4_ID		SLEEP_IRQ_BIT4_ID
#define SPM_IRQ5_ID		SLEEP_IRQ_BIT5_ID
#define SPM_IRQ6_ID		SLEEP_IRQ_BIT6_ID
#define SPM_IRQ7_ID		SLEEP_IRQ_BIT7_ID


/************************************************************************************
* SPM_CODA2RTL.pl
* Revision Hisotry:
*   -- 2014/05/20 -- (v1.0) First release for Rocky SPM integration. (Kev.Lin)
*   -- 2014/12/04 -- (v1.1) Modify to support second SPM to generate different register
*                           name with the same CODA. To avoid conflicting on dsim.
*   -- 2015/01/12 -- (v1.2) Fix SPM-PWRAP DVFS interface bug.
*   -- 2015/03/12 -- (v1.3) Modify to meet Rocky's requirment. (for previous project please use v1.2)
*                           Change address size to 18 bits.
*   -- 2015/04/09 -- (v1.2.1) Generate SPM register header file for SW. <based on v1.2>
*                    (v1.3.1) Generate SPM register header file for SW. <based on v1.3>
*************************************************************************************/

#define SLEEP_BASE                     0x10006000
#define POWERON_CONFIG_EN              (SLEEP_BASE + 0x000)
#define SPM_POWER_ON_VAL0              (SLEEP_BASE + 0x004)
#define SPM_POWER_ON_VAL1              (SLEEP_BASE + 0x008)
#define SPM_CLK_CON                    (SLEEP_BASE + 0x00C)
#define SPM_CLK_SETTLE                 (SLEEP_BASE + 0x010)
#define SPM_AP_STANDBY_CON             (SLEEP_BASE + 0x014)
#define PCM_CON0                       (SLEEP_BASE + 0x018)
#define PCM_CON1                       (SLEEP_BASE + 0x01C)
#define PCM_IM_PTR                     (SLEEP_BASE + 0x020)
#define PCM_IM_LEN                     (SLEEP_BASE + 0x024)
#define PCM_REG_DATA_INI               (SLEEP_BASE + 0x028)
#define PCM_PWR_IO_EN                  (SLEEP_BASE + 0x02C)
#define PCM_TIMER_VAL                  (SLEEP_BASE + 0x030)
#define PCM_WDT_VAL                    (SLEEP_BASE + 0x034)
#define PCM_IM_HOST_RW_PTR             (SLEEP_BASE + 0x038)
#define PCM_IM_HOST_RW_DAT             (SLEEP_BASE + 0x03C)
#define PCM_EVENT_VECTOR0              (SLEEP_BASE + 0x040)
#define PCM_EVENT_VECTOR1              (SLEEP_BASE + 0x044)
#define PCM_EVENT_VECTOR2              (SLEEP_BASE + 0x048)
#define PCM_EVENT_VECTOR3              (SLEEP_BASE + 0x04C)
#define PCM_EVENT_VECTOR4              (SLEEP_BASE + 0x050)
#define PCM_EVENT_VECTOR5              (SLEEP_BASE + 0x054)
#define PCM_EVENT_VECTOR6              (SLEEP_BASE + 0x058)
#define PCM_EVENT_VECTOR7              (SLEEP_BASE + 0x05C)
#define PCM_EVENT_VECTOR8              (SLEEP_BASE + 0x060)
#define PCM_EVENT_VECTOR9              (SLEEP_BASE + 0x064)
#define PCM_EVENT_VECTOR10             (SLEEP_BASE + 0x068)
#define PCM_EVENT_VECTOR11             (SLEEP_BASE + 0x06C)
#define PCM_EVENT_VECTOR12             (SLEEP_BASE + 0x070)
#define PCM_EVENT_VECTOR13             (SLEEP_BASE + 0x074)
#define PCM_EVENT_VECTOR14             (SLEEP_BASE + 0x078)
#define PCM_EVENT_VECTOR15             (SLEEP_BASE + 0x07C)
#define PCM_EVENT_VECTOR_EN            (SLEEP_BASE + 0x080)
#define SPM_SW_INT_SET                 (SLEEP_BASE + 0x090)
#define SPM_SW_INT_CLEAR               (SLEEP_BASE + 0x094)
#define SPM_SCP_MAILBOX                (SLEEP_BASE + 0x098)
#define SPM_SCP_IRQ                    (SLEEP_BASE + 0x09C)
#define SPM_TWAM_CON                   (SLEEP_BASE + 0x0A0)
#define SPM_TWAM_WINDOW_LEN            (SLEEP_BASE + 0x0A4)
#define SPM_TWAM_IDLE_SEL              (SLEEP_BASE + 0x0A8)
#define SPM_CPU_WAKEUP_EVENT           (SLEEP_BASE + 0x0B0)
#define SPM_IRQ_MASK                   (SLEEP_BASE + 0x0B4)
#define SPM_SRC_REQ                    (SLEEP_BASE + 0x0B8)
#define SPM_SRC_MASK                   (SLEEP_BASE + 0x0BC)
#define SPM_SRC2_MASK                  (SLEEP_BASE + 0x0C0)
#define SPM_WAKEUP_EVENT_MASK          (SLEEP_BASE + 0x0C4)
#define SPM_WAKEUP_EVENT_EXT_MASK      (SLEEP_BASE + 0x0C8)
#define SCP_CLK_CON                    (SLEEP_BASE + 0x0D0)
#define PCM_DEBUG_CON                  (SLEEP_BASE + 0x0D4)
#define PCM_REG0_DATA                  (SLEEP_BASE + 0x100)
#define PCM_REG1_DATA                  (SLEEP_BASE + 0x104)
#define PCM_REG2_DATA                  (SLEEP_BASE + 0x108)
#define PCM_REG3_DATA                  (SLEEP_BASE + 0x10C)
#define PCM_REG4_DATA                  (SLEEP_BASE + 0x110)
#define PCM_REG5_DATA                  (SLEEP_BASE + 0x114)
#define PCM_REG6_DATA                  (SLEEP_BASE + 0x118)
#define PCM_REG7_DATA                  (SLEEP_BASE + 0x11C)
#define PCM_REG8_DATA                  (SLEEP_BASE + 0x120)
#define PCM_REG9_DATA                  (SLEEP_BASE + 0x124)
#define PCM_REG10_DATA                 (SLEEP_BASE + 0x128)
#define PCM_REG11_DATA                 (SLEEP_BASE + 0x12C)
#define PCM_REG12_DATA                 (SLEEP_BASE + 0x130)
#define PCM_REG13_DATA                 (SLEEP_BASE + 0x134)
#define PCM_REG14_DATA                 (SLEEP_BASE + 0x138)
#define PCM_REG15_DATA                 (SLEEP_BASE + 0x13C)
#define PCM_REG12_MASK_B_STA           (SLEEP_BASE + 0x140)
#define PCM_REG12_EXT_DATA             (SLEEP_BASE + 0x144)
#define PCM_REG12_EXT_MASK_B_STA       (SLEEP_BASE + 0x148)
#define PCM_EVENT_REG_STA              (SLEEP_BASE + 0x14C)
#define PCM_TIMER_OUT                  (SLEEP_BASE + 0x150)
#define PCM_WDT_OUT                    (SLEEP_BASE + 0x154)
#define SPM_IRQ_STA                    (SLEEP_BASE + 0x158)
#define SPM_WAKEUP_STA                 (SLEEP_BASE + 0x15C)
#define SPM_WAKEUP_EXT_STA             (SLEEP_BASE + 0x160)
#define SPM_WAKEUP_MISC                (SLEEP_BASE + 0x164)
#define BUS_PROTECT_RDY                (SLEEP_BASE + 0x168)
#define BUS_PROTECT2_RDY               (SLEEP_BASE + 0x16C)
#define SUBSYS_IDLE_STA                (SLEEP_BASE + 0x170)
#define CPU_IDLE_STA                   (SLEEP_BASE + 0x174)
#define PCM_FSM_STA                    (SLEEP_BASE + 0x178)
#define PWR_STATUS                     (SLEEP_BASE + 0x180)
#define PWR_STATUS_2ND                 (SLEEP_BASE + 0x184)
#define CPU_PWR_STATUS                 (SLEEP_BASE + 0x188)
#define CPU_PWR_STATUS_2ND             (SLEEP_BASE + 0x18C)
#define PCM_WDT_LATCH_0                (SLEEP_BASE + 0x190)
#define PCM_WDT_LATCH_1                (SLEEP_BASE + 0x194)
#define PCM_WDT_LATCH_2                (SLEEP_BASE + 0x198)
#define DRAMC_DBG_LATCH                (SLEEP_BASE + 0x19C)
#define SPM_TWAM_LAST_STA0             (SLEEP_BASE + 0x1A0)
#define SPM_TWAM_LAST_STA1             (SLEEP_BASE + 0x1A4)
#define SPM_TWAM_LAST_STA2             (SLEEP_BASE + 0x1A8)
#define SPM_TWAM_LAST_STA3             (SLEEP_BASE + 0x1AC)
#define SPM_TWAM_CURR_STA0             (SLEEP_BASE + 0x1B0)
#define SPM_TWAM_CURR_STA1             (SLEEP_BASE + 0x1B4)
#define SPM_TWAM_CURR_STA2             (SLEEP_BASE + 0x1B8)
#define SPM_TWAM_CURR_STA3             (SLEEP_BASE + 0x1BC)
#define SPM_TWAM_TIMER_OUT             (SLEEP_BASE + 0x1C0)
#define PCM_WDT_LATCH_3                (SLEEP_BASE + 0x1C4)
#define SPM_SRC_RDY_STA                (SLEEP_BASE + 0x1D0)
#define MCU_PWR_CON                    (SLEEP_BASE + 0x200)
#define MP0_CPUTOP_PWR_CON             (SLEEP_BASE + 0x204)
#define MP0_CPU0_PWR_CON               (SLEEP_BASE + 0x208)
#define MP0_CPU1_PWR_CON               (SLEEP_BASE + 0x20C)
#define MP0_CPU2_PWR_CON               (SLEEP_BASE + 0x210)
#define MP0_CPU3_PWR_CON               (SLEEP_BASE + 0x214)
#define MP1_CPUTOP_PWR_CON             (SLEEP_BASE + 0x218)
#define MP1_CPU0_PWR_CON               (SLEEP_BASE + 0x21C)
#define MP1_CPU1_PWR_CON               (SLEEP_BASE + 0x220)
#define MP1_CPU2_PWR_CON               (SLEEP_BASE + 0x224)
#define MP1_CPU3_PWR_CON               (SLEEP_BASE + 0x228)
#define MP0_CPUTOP_L2_PDN              (SLEEP_BASE + 0x240)
#define MP0_CPUTOP_L2_SLEEP_B          (SLEEP_BASE + 0x244)
#define MP0_CPU0_L1_PDN                (SLEEP_BASE + 0x248)
#define MP0_CPU1_L1_PDN                (SLEEP_BASE + 0x24C)
#define MP0_CPU2_L1_PDN                (SLEEP_BASE + 0x250)
#define MP0_CPU3_L1_PDN                (SLEEP_BASE + 0x254)
#define MP1_CPUTOP_L2_PDN              (SLEEP_BASE + 0x258)
#define MP1_CPUTOP_L2_SLEEP_B          (SLEEP_BASE + 0x25C)
#define MP1_CPU0_L1_PDN                (SLEEP_BASE + 0x260)
#define MP1_CPU1_L1_PDN                (SLEEP_BASE + 0x264)
#define MP1_CPU2_L1_PDN                (SLEEP_BASE + 0x268)
#define MP1_CPU3_L1_PDN                (SLEEP_BASE + 0x26C)
#define CPU_EXT_BUCK_ISO               (SLEEP_BASE + 0x290)
#define DUMMY1_PWR_CON                 (SLEEP_BASE + 0x2B0)
#define VDE_PWR_CON                    (SLEEP_BASE + 0x300)
#define VEN_PWR_CON                    (SLEEP_BASE + 0x304)
#define ISP_PWR_CON                    (SLEEP_BASE + 0x308)
#define DIS_PWR_CON                    (SLEEP_BASE + 0x30C)
#define MJC_PWR_CON                    (SLEEP_BASE + 0x310)
#define AUDIO_PWR_CON                  (SLEEP_BASE + 0x314)
#define IFR_PWR_CON                    (SLEEP_BASE + 0x318)
#define DPY_PWR_CON                    (SLEEP_BASE + 0x31C)
#define MD1_PWR_CON                    (SLEEP_BASE + 0x320)
#define MD2_PWR_CON                    (SLEEP_BASE + 0x324)
#define C2K_PWR_CON                    (SLEEP_BASE + 0x328)
#define CONN_PWR_CON                   (SLEEP_BASE + 0x32C)
#define VCOREPDN_PWR_CON               (SLEEP_BASE + 0x330)
#define MFG_ASYNC_PWR_CON              (SLEEP_BASE + 0x334)
#define MFG_PWR_CON                    (SLEEP_BASE + 0x338)
#define SYSRAM_CON                     (SLEEP_BASE + 0x350)
#define SYSROM_CON                     (SLEEP_BASE + 0x354)
#define SCP_SRAM_CON                   (SLEEP_BASE + 0x358)
#define GCPU_SRAM_CON                  (SLEEP_BASE + 0x35C)
#define MDSYS_INTF_INFRA_PWR_CON       (SLEEP_BASE + 0x360)
#define MDSYS_INTF_MD1_PWR_CON         (SLEEP_BASE + 0x364)
#define MDSYS_INTF_C2K_PWR_CON         (SLEEP_BASE + 0x368)
#define BSI_TOP_SRAM_CON               (SLEEP_BASE + 0x370)
#define DVFSP_SRAM_CON                 (SLEEP_BASE + 0x374)
#define MD_EXT_BUCK_ISO                (SLEEP_BASE + 0x390)
#define DUMMY2_PWR_CON                 (SLEEP_BASE + 0x3B0)
#define SPM_DVFS_CON                   (SLEEP_BASE + 0x400)
#define SPM_MDBSI_CON                  (SLEEP_BASE + 0x404)
#define SPM_MAS_PAUSE_MASK_B           (SLEEP_BASE + 0x408)
#define SPM_MAS_PAUSE2_MASK_B          (SLEEP_BASE + 0x40C)
#define SPM_BSI_GEN                    (SLEEP_BASE + 0x410)
#define SPM_BSI_EN_SR                  (SLEEP_BASE + 0x414)
#define SPM_BSI_CLK_SR                 (SLEEP_BASE + 0x418)
#define SPM_BSI_D0_SR                  (SLEEP_BASE + 0x41C)
#define SPM_BSI_D1_SR                  (SLEEP_BASE + 0x420)
#define SPM_BSI_D2_SR                  (SLEEP_BASE + 0x424)
#define SPM_AP_SEMA                    (SLEEP_BASE + 0x428)
#define SPM_SPM_SEMA                   (SLEEP_BASE + 0x42C)
#define AP2MD_CROSS_TRIGGER            (SLEEP_BASE + 0x430)
#define AP_MDSRC_REQ                   (SLEEP_BASE + 0x434)
#define SPM2MD_DVFS_CON                (SLEEP_BASE + 0x438)
#define MD2SPM_DVFS_CON                (SLEEP_BASE + 0x43C)
#define DRAMC_DPY_CLK_SW_CON           (SLEEP_BASE + 0x440)
#define DPY_LP_CON                     (SLEEP_BASE + 0x444)
#define CPU_DVFS_REQ                   (SLEEP_BASE + 0x448)
#define SPM_PLL_CON                    (SLEEP_BASE + 0x44C)
#define SPM_EMI_BW_MODE                (SLEEP_BASE + 0x450)
#define AP2MD_PEER_WAKEUP              (SLEEP_BASE + 0x454)
#define ULPOSC_CON                     (SLEEP_BASE + 0x458)
#define MP0_CPU0_IRQ_MASK              (SLEEP_BASE + 0x500)
#define MP0_CPU1_IRQ_MASK              (SLEEP_BASE + 0x504)
#define MP0_CPU2_IRQ_MASK              (SLEEP_BASE + 0x508)
#define MP0_CPU3_IRQ_MASK              (SLEEP_BASE + 0x50C)
#define MP1_CPU0_IRQ_MASK              (SLEEP_BASE + 0x510)
#define MP1_CPU1_IRQ_MASK              (SLEEP_BASE + 0x514)
#define MP1_CPU2_IRQ_MASK              (SLEEP_BASE + 0x518)
#define MP1_CPU3_IRQ_MASK              (SLEEP_BASE + 0x51C)
#define MP0_CPU0_WFI_EN                (SLEEP_BASE + 0x530)
#define MP0_CPU1_WFI_EN                (SLEEP_BASE + 0x534)
#define MP0_CPU2_WFI_EN                (SLEEP_BASE + 0x538)
#define MP0_CPU3_WFI_EN                (SLEEP_BASE + 0x53C)
#define MP1_CPU0_WFI_EN                (SLEEP_BASE + 0x540)
#define MP1_CPU1_WFI_EN                (SLEEP_BASE + 0x544)
#define MP1_CPU2_WFI_EN                (SLEEP_BASE + 0x548)
#define MP1_CPU3_WFI_EN                (SLEEP_BASE + 0x54C)
#define CPU_PTPOD2_CON                 (SLEEP_BASE + 0x560)
#define SPM_SW_FLAG                    (SLEEP_BASE + 0x600)
#define SPM_SW_DEBUG                   (SLEEP_BASE + 0x604)
#define SPM_SW_RSV_0                   (SLEEP_BASE + 0x608)
#define SPM_SW_RSV_1                   (SLEEP_BASE + 0x60C)
#define SPM_SW_RSV_2                   (SLEEP_BASE + 0x610)
#define SPM_SW_RSV_3                   (SLEEP_BASE + 0x614)
#define SPM_SW_RSV_4                   (SLEEP_BASE + 0x618)
#define SPM_SW_RSV_5                   (SLEEP_BASE + 0x61C)
#define SPM_RSV_CON                    (SLEEP_BASE + 0x620)
#define SPM_RSV_STA                    (SLEEP_BASE + 0x624)
#define SPM_PASR_DPD_0                 (SLEEP_BASE + 0x630)
#define SPM_PASR_DPD_1                 (SLEEP_BASE + 0x634)
#define SPM_PASR_DPD_2                 (SLEEP_BASE + 0x638)
#define SPM_PASR_DPD_3                 (SLEEP_BASE + 0x63C)





#define SPM_PROJECT_CODE	0xb16

#define SPM_REGWR_EN		(1U << 0)
#define SPM_REGWR_CFG_KEY	(SPM_PROJECT_CODE << 16)

#define SPM_CPU_PDN_DIS		(1U << 0)
#define SPM_INFRA_PDN_DIS	(1U << 1)
#define SPM_DDRPHY_PDN_DIS	(1U << 2)
#define SPM_DUALVCORE_PDN_DIS	(1U << 3)
#define SPM_PASR_DIS		(1U << 4)
#define SPM_DPD_DIS		(1U << 5)
#define SPM_SODI_DIS		(1U << 6)
#define SPM_MEMPLL_RESET	(1U << 7)
#define SPM_MAINPLL_PDN_DIS	(1U << 8)
#define SPM_CPU_DVS_DIS		(1U << 9)
#define SPM_CPU_DORMANT		(1U << 10)


/* for power management init */
extern int spm_module_init(void);



/**************************************
 * Macro and Inline
 **************************************/
/* 
//XXX: only in kernel
#define spm_read(addr)			__raw_readl(IOMEM(addr))
*/
#define spm_read(addr)			__raw_readl(addr)
#define spm_write(addr, val)		mt_reg_sync_writel(val, addr)

#define is_cpu_pdn(flags)		(!((flags) & SPM_CPU_PDN_DIS))
#define is_infra_pdn(flags)		(!((flags) & SPM_INFRA_PDN_DIS))
#define is_ddrphy_pdn(flags)		(!((flags) & SPM_DDRPHY_PDN_DIS))
#define is_dualvcore_pdn(flags)		(!((flags) & SPM_DUALVCORE_PDN_DIS))

#define get_high_cnt(sigsta)		((sigsta) & 0x3ff)
#define get_high_percent(sigsta)	(get_high_cnt(sigsta) * 100 / 1023)

#endif
