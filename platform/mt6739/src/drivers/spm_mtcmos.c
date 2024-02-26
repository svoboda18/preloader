/*
//XXX: only in kernel
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/delay.h>    //udelay

#include <mach/mt_typedefs.h>
#include <mach/mt_spm.h>
#include <mach/mt_spm_mtcmos.h>
#include <mach/mt_spm_mtcmos_internal.h>
#include <mach/hotplug.h>
#include <mach/mt_clkmgr.h>
*/
#include <platform.h>
#include <spm.h>
#include <spm_mtcmos.h>
#include <spm_mtcmos_internal.h>
#include <timer.h>  //udelay
#include <pll.h>
#if CFG_FPGA_PLATFORM
#define IGNORE_MTCMOS_CHECK	1 //for FPGA
#endif
/**************************************
 * for CPU MTCMOS
 **************************************/
 /*
//XXX: only in kernel
static DEFINE_SPINLOCK(spm_cpu_lock);

void spm_mtcmos_cpu_lock(unsigned long *flags)
{
    spin_lock_irqsave(&spm_cpu_lock, *flags);
}

void spm_mtcmos_cpu_unlock(unsigned long *flags)
{
    spin_unlock_irqrestore(&spm_cpu_lock, *flags);
}
*/


/**************************************
 * for non-CPU MTCMOS
 **************************************/
#define SPM_REG(offset)		(0x10b00000 + offset)
#define MM0_PWR_CON	SPM_REG(0x0338)
#define PWR_STATUS	SPM_REG(0x0190)
#define PWR_STATUS_2ND	SPM_REG(0x0194)


#define INFRACFG_REG(offset)		(0x10000000 + offset)
#define INFRA_TOPAXI_PROTECTEN_SET	INFRACFG_REG(0x02A0)
#define INFRA_TOPAXI_PROTECTSTA1	INFRACFG_REG(0x0228)
#define INFRA_TOPAXI_PROTECTEN_CLR	INFRACFG_REG(0x02A4)

#define INFRA_TOPAXI_PROTECTEN_1_SET	INFRACFG_REG(0x02A8)
#define INFRA_TOPAXI_PROTECTSTA1_1	INFRACFG_REG(0x0258)
#define INFRA_TOPAXI_PROTECTEN_1_CLR	INFRACFG_REG(0x02AC)

#define INFRA_TOPAXI_PROTECTEN_2_SET	INFRACFG_REG(0x02C8)
#define INFRA_TOPAXI_PROTECTSTA1_2	INFRACFG_REG(0x02D4)
#define INFRA_TOPAXI_PROTECTEN_2_CLR	INFRACFG_REG(0x02CC)

#define MM0_SRAM_PDN                     (0x1 << 8)
#define MM0_SRAM_PDN_ACK                 (0x1 << 24)
#define MM0_PWR_STA_MASK                 (0x1 << 15)
#define MM0_SRAM_PDN_ACK_BIT0            (0x1 << 24)

#define MM0_PROT_BIT_MASK                ((0x1 << 8) \
					  |(0x1 << 9))
#define MM0_PROT_BIT_ACK_MASK            ((0x1 << 8) \
					  |(0x1 << 9))
#define MM0_PROT_BIT_2ND_MASK            ((0x1 << 4) \
					  |(0x1 << 6) \
					  |(0x1 << 8) \
					  |(0x1 << 10) \
					  |(0x1 << 12) \
					  |(0x1 << 14))
#define MM0_PROT_BIT_ACK_2ND_MASK        ((0x1 << 4) \
					  |(0x1 << 6) \
					  |(0x1 << 8) \
					  |(0x1 << 10) \
					  |(0x1 << 12) \
					  |(0x1 << 14))
					  					  
int spm_mtcmos_ctrl_mm0(int state)
{
	int err = 0;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off MM0" */
		/* TINFO="Set way_en = 0" */
		*((UINT32P)(0x10000200)) = *((UINT32P)(0x10000200)) & (~(0x1 << 6));
#ifndef IGNORE_MTCMOS_CHECK
		while ((*((UINT32P)(0x10000224)) & (0x1 << 18)) != (0x1 << 18)){
		}
#endif		
		/* TINFO="Set bus protect" */
		spm_write(INFRA_TOPAXI_PROTECTEN_SET, MM0_PROT_BIT_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		while ((spm_read(INFRA_TOPAXI_PROTECTSTA1) & MM0_PROT_BIT_ACK_MASK) != MM0_PROT_BIT_ACK_MASK) {
		}
#endif
		/* TINFO="Set bus protect" */
		spm_write(INFRA_TOPAXI_PROTECTEN_2_SET, MM0_PROT_BIT_2ND_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		while ((spm_read(INFRA_TOPAXI_PROTECTSTA1_2) & MM0_PROT_BIT_ACK_2ND_MASK) != MM0_PROT_BIT_ACK_2ND_MASK) {
		}
#endif
		/* TINFO="Set SRAM_PDN = 1" */
		spm_write(MM0_PWR_CON, spm_read(MM0_PWR_CON) | MM0_SRAM_PDN);
#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until MM0_SRAM_PDN_ACK = 1" */
		while ((spm_read(MM0_PWR_CON) & MM0_SRAM_PDN_ACK) != MM0_SRAM_PDN_ACK) {
				/* Need f_fmm_ck for SRAM PDN delay IP. */
		}
#endif
		/* TINFO="Set PWR_ISO = 1" */
		spm_write(MM0_PWR_CON, spm_read(MM0_PWR_CON) | PWR_ISO);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(MM0_PWR_CON, spm_read(MM0_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(MM0_PWR_CON, spm_read(MM0_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(MM0_PWR_CON, spm_read(MM0_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(MM0_PWR_CON, spm_read(MM0_PWR_CON) & ~PWR_ON_2ND);
#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until PWR_STATUS = 0 and PWR_STATUS_2ND = 0" */
		while ((spm_read(PWR_STATUS) & MM0_PWR_STA_MASK)
		       || (spm_read(PWR_STATUS_2ND) & MM0_PWR_STA_MASK)) {
				/* No logic between pwr_on and pwr_ack. Print SRAM / MTCMOS control and PWR_ACK for debug. */
		}
#endif
		/* TINFO="Finish to turn off MM0" */
	} else {    /* STA_POWER_ON */
		/* TINFO="Start to turn on MM0" */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(MM0_PWR_CON, spm_read(MM0_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(MM0_PWR_CON, spm_read(MM0_PWR_CON) | PWR_ON_2ND);
#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until PWR_STATUS = 1 and PWR_STATUS_2ND = 1" */
		while (((spm_read(PWR_STATUS) & MM0_PWR_STA_MASK) != MM0_PWR_STA_MASK)
		       || ((spm_read(PWR_STATUS_2ND) & MM0_PWR_STA_MASK) != MM0_PWR_STA_MASK)) {
				/* No logic between pwr_on and pwr_ack. Print SRAM / MTCMOS control and PWR_ACK for debug. */
		}
#endif
		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(MM0_PWR_CON, spm_read(MM0_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_ISO = 0" */
		spm_write(MM0_PWR_CON, spm_read(MM0_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(MM0_PWR_CON, spm_read(MM0_PWR_CON) | PWR_RST_B);
		/* TINFO="Set SRAM_PDN = 0" */
		spm_write(MM0_PWR_CON, spm_read(MM0_PWR_CON) & ~(0x1 << 8));
#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until MM0_SRAM_PDN_ACK_BIT0 = 0" */
		while (spm_read(MM0_PWR_CON) & MM0_SRAM_PDN_ACK_BIT0) {
				/* Need f_fmm_ck for SRAM PDN delay IP. */
		}
#endif
		/* TINFO="Release bus protect" */
		spm_write(INFRA_TOPAXI_PROTECTEN_2_CLR, MM0_PROT_BIT_2ND_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		/* Note that this protect ack check after releasing protect has been ignored */
#endif
		/* TINFO="Release bus protect" */
		spm_write(INFRA_TOPAXI_PROTECTEN_CLR, MM0_PROT_BIT_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		/* Note that this protect ack check after releasing protect has been ignored */
#endif
		/* TINFO=Set way_en = 1" */
		*((UINT32P)(0x10000200)) = *((UINT32P)(0x10000200)) | (0x1 << 6);
		/* TINFO="Finish to turn on MM0" */
	}
	return err;
}



