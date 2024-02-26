
#ifndef _SEC_REG_H_
#define _SEC_REG_H_

#include "platform.h"

#define BOOTROM_PWR_CTRL        (INFRACFG_AO_BASE + 0x804)
#define SRAMROM_SEC_ADDR        (INFRACFG_AO_BASE + 0x808)

#define SRAMROM_SEC_ADDR_MASK        (0x0003FC00)

#define SRAMROM_SEC_SEC0_DOM0_SHIFT  (0)
#define SRAMROM_SEC_SEC0_DOM1_SHIFT  (3)
#define SRAMROM_SEC_SEC0_DOM2_SHIFT  (6)
#define SRAMROM_SEC_SEC0_DOM3_SHIFT  (9)
#define SRAMROM_SEC_SEC1_DOM0_SHIFT  (16)
#define SRAMROM_SEC_SEC1_DOM1_SHIFT  (19)
#define SRAMROM_SEC_SEC1_DOM2_SHIFT  (22)
#define SRAMROM_SEC_SEC1_DOM3_SHIFT  (25)
#define SRAMROM_SEC_SEC1_EN_SHIFT    (28)

#define SRAMROM_SEC_SEC0_DOM0_MASK   (0x7 << SRAMROM_SEC_SEC0_DOM0_SHIFT)
#define SRAMROM_SEC_SEC0_DOM1_MASK   (0x7 << SRAMROM_SEC_SEC0_DOM1_SHIFT)
#define SRAMROM_SEC_SEC0_DOM2_MASK   (0x7 << SRAMROM_SEC_SEC0_DOM2_SHIFT)
#define SRAMROM_SEC_SEC0_DOM3_MASK   (0x7 << SRAMROM_SEC_SEC0_DOM3_SHIFT)
#define SRAMROM_SEC_SEC1_DOM0_MASK   (0x7 << SRAMROM_SEC_SEC1_DOM0_SHIFT)
#define SRAMROM_SEC_SEC1_DOM1_MASK   (0x7 << SRAMROM_SEC_SEC1_DOM1_SHIFT)
#define SRAMROM_SEC_SEC1_DOM2_MASK   (0x7 << SRAMROM_SEC_SEC1_DOM2_SHIFT)
#define SRAMROM_SEC_SEC1_DOM3_MASK   (0x7 << SRAMROM_SEC_SEC1_DOM3_SHIFT)
#define SRAMROM_SEC_SEC1_EN_MASK     (0x1 << SRAMROM_SEC_SEC1_EN_SHIFT)

#define PERMIT_S_RW_NS_RW       (0x0)
#define PERMIT_S_RW_NS_BLOCK    (0x1)
#define PERMIT_S_RW_NS_RO       (0x2)
#define PERMIT_S_RW_NS_WO       (0x3)
#define PERMIT_S_RO_NS_RO       (0x4)
#define PERMIT_S_BLOCK_NS_BLOCK (0x5)

#define DISABLE_SEC_SEC1_PROTECTION (0x0)
#define ENABLE_SEC_SEC1_PROTECTION  (0x1)

#define GIC_CPU_CTRL            0x00
#define GIC_CPU_PRIMASK         0x04
#define GIC_CPU_BINPOINT        0x08
#define GIC_CPU_INTACK          0x0c
#define GIC_CPU_EOI             0x10
#define GIC_CPU_RUNNINGPRI      0x14
#define GIC_CPU_HIGHPRI         0x18

#define CCI400_SEC_ACCESS       (CCI400_BASE + 0x0008)

/**************************************
 * spm cpu mtcmos
 **************************************/
#define SPM_PROJECT_CODE        0xb16
/*
 * regiser address
 */
#define SPM_POWERON_CONFIG_SET  (SPM_BASE + 0x0000)
#define SPM_SLEEP_TIMER_STA     (SPM_BASE + 0x0720)

#define SPM_MP0_FC1_PWR_CON     (SPM_BASE + 0x0218)
#define SPM_MP0_FC2_PWR_CON     (SPM_BASE + 0x021c)
#define SPM_MP0_FC3_PWR_CON     (SPM_BASE + 0x0220)
#define SPM_MP1_FC0_PWR_CON     (SPM_BASE + 0x02A0)
#define SPM_MP1_FC1_PWR_CON     (SPM_BASE + 0x02A4)
#define SPM_MP1_FC2_PWR_CON     (SPM_BASE + 0x02A8)
#define SPM_MP1_FC3_PWR_CON     (SPM_BASE + 0x02AC)
#define SPM_MP1_CPU_PWR_CON     (SPM_BASE + 0x02B0)
#define SPM_MP1_DBG_PWR_CON     (SPM_BASE + 0x02B4)

#define SPM_MP0_FC0_L1_PDN      (SPM_BASE + 0x025c)
#define SPM_MP0_FC1_L1_PDN      (SPM_BASE + 0x0264)
#define SPM_MP0_FC2_L1_PDN      (SPM_BASE + 0x026c)
#define SPM_MP0_FC3_L1_PDN      (SPM_BASE + 0x0274)
#define SPM_MP1_FC0_L1_PDN      (SPM_BASE + 0x02C0)
#define SPM_MP1_FC1_L1_PDN      (SPM_BASE + 0x02C4)
#define SPM_MP1_FC2_L1_PDN      (SPM_BASE + 0x02C8)
#define SPM_MP1_FC3_L1_PDN      (SPM_BASE + 0x02CC)
#define SPM_MP1_L2_DAT_PDN      (SPM_BASE + 0x02D0)
#define SPM_MP1_L2_DAT_SLEEP_B  (SPM_BASE + 0x02D4)

#define SPM_PWR_STATUS          (SPM_BASE + 0x060c)
#define SPM_PWR_STATUS_S        (SPM_BASE + 0x0610)

/*
 * regiser bit difinition
 */
/* SPM_MP0_FC1_PWR_CON */
/* SPM_MP0_FC2_PWR_CON */
/* SPM_MP0_FC3_PWR_CON */
/* SPM_MP0_DBG_PWR_CON */
/* SPM_MP1_FC0_PWR_CON */
/* SPM_MP1_FC1_PWR_CON */
/* SPM_MP1_FC2_PWR_CON */
/* SPM_MP1_FC3_PWR_CON */
/* SPM_MP1_DBG_PWR_CON */
/* SPM_MP1_CPU_PWR_CON */
#define SRAM_ISOINT_B           (1U << 6)
#define SRAM_CKISO              (1U << 5)
#define PWR_CLK_DIS             (1U << 4)
#define PWR_ON_S                (1U << 3)
#define PWR_ON                  (1U << 2)
#define PWR_ISO                 (1U << 1)
#define PWR_RST_B               (1U << 0)

/* SPM_MP0_FC1_L1_PDN */
/* SPM_MP0_FC2_L1_PDN */
/* SPM_MP0_FC3_L1_PDN */
/* SPM_MP1_FC0_L1_PDN */
/* SPM_MP1_FC1_L1_PDN */
/* SPM_MP1_FC2_L1_PDN */
/* SPM_MP1_FC3_L1_PDN */
#define L1_PDN_ACK              (1U << 8)
#define L1_PDN                  (1U << 0)
/* SPM_MP1_L2_DAT_PDN */
#define L2_SRAM_PDN_ACK         (1U << 8)
#define L2_SRAM_PDN             (1U << 0)
/* SPM_MP1_L2_DAT_SLEEP_B */
#define L2_SRAM_SLEEP_B_ACK     (1U << 8)
#define L2_SRAM_SLEEP_B         (1U << 0)

/* SPM_PWR_STATUS */
/* SPM_PWR_STATUS_S */
#define MP1_DBG                 (1U << 20)
#define MP1_FC0                 (1U << 19)
#define MP1_FC1                 (1U << 18)
#define MP1_FC2                 (1U << 17)
#define MP1_FC3                 (1U << 16)
#define MP1_CPU                 (1U << 15)
#define MP0_DBG                 (1U << 14)
#define MP0_FC1                 (1U << 11)
#define MP0_FC2                 (1U << 10)
#define MP0_FC3                 (1U <<  9)

/* SPM_SLEEP_TIMER_STA */
#define MP1_CPU3_STANDBYWFI     (1U << 23)
#define MP1_CPU2_STANDBYWFI     (1U << 22)
#define MP1_CPU1_STANDBYWFI     (1U << 21)
#define MP1_CPU0_STANDBYWFI     (1U << 20)
#define MP0_CPU3_STANDBYWFI     (1U << 19)
#define MP0_CPU2_STANDBYWFI     (1U << 18)
#define MP0_CPU1_STANDBYWFI     (1U << 17)

/**************************************
 * bootrom slave cores migic reg and num
 **************************************/
#define SLAVE_JUMP_REG          (SRAMROM_BASE + 0x34)
#define SLAVE1_MAGIC_REG        (SRAMROM_BASE + 0x38)
#define SLAVE2_MAGIC_REG        (SRAMROM_BASE + 0x38)
#define SLAVE3_MAGIC_REG        (SRAMROM_BASE + 0x38)

#define SLAVE1_MAGIC_NUM        0x534C4131
#define SLAVE2_MAGIC_NUM        0x4C415332
#define SLAVE3_MAGIC_NUM        0x41534C33

#endif
