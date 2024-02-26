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
#define spm_mtcmos_cpu_lock(x)
#define spm_mtcmos_cpu_unlock(x)

/* Define MTCMOS Bus Protect Mask */
#define MD1_PROT_MASK                    ((0x1 << 16) \
					  |(0x1 << 17) \
					  |(0x1 << 18) \
					  |(0x1 << 19) \
					  |(0x1 << 20) \
					  |(0x1 << 21))
#define CONN_PROT_MASK                   ((0x1 << 13))
#define DIS_PROT_MASK                    ((0x1 << 1))
#define MFG_PROT_MASK                    ((0x1 << 21))
#define MP0_CPUTOP_PROT_MASK             ((0x1 << 26))
#define MP1_CPUTOP_PROT_MASK             ((0x1 << 30))
#define C2K_PROT_MASK                    ((0x1 << 22) \
					  |(0x1 << 23) \
					  |(0x1 << 24))
#define MDSYS_INTF_INFRA_PROT_MASK       ((0x1 << 3) \
					  |(0x1 << 4))
					  
 /* Define MTCMOS Power Status Mask */

#define MD1_PWR_STA_MASK                 (0x1 << 0)
#define CONN_PWR_STA_MASK                (0x1 << 1)
#define DPY_PWR_STA_MASK                 (0x1 << 2)
#define DIS_PWR_STA_MASK                 (0x1 << 3)
#define MFG_PWR_STA_MASK                 (0x1 << 4)
#define ISP_PWR_STA_MASK                 (0x1 << 5)
#define IFR_PWR_STA_MASK                 (0x1 << 6)
#define VDE_PWR_STA_MASK                 (0x1 << 7)
#define MP0_CPUTOP_PWR_STA_MASK          (0x1 << 8)
#define MP0_CPU0_PWR_STA_MASK            (0x1 << 9)
#define MP0_CPU1_PWR_STA_MASK            (0x1 << 10)
#define MP0_CPU2_PWR_STA_MASK            (0x1 << 11)
#define MP0_CPU3_PWR_STA_MASK            (0x1 << 12)
#define MCU_PWR_STA_MASK                 (0x1 << 14)
#define MP1_CPUTOP_PWR_STA_MASK          (0x1 << 15)
#define MP1_CPU0_PWR_STA_MASK            (0x1 << 16)
#define MP1_CPU1_PWR_STA_MASK            (0x1 << 17)
#define MP1_CPU2_PWR_STA_MASK            (0x1 << 18)
#define MP1_CPU3_PWR_STA_MASK            (0x1 << 19)
#define VEN_PWR_STA_MASK                 (0x1 << 21)
#define MFG_ASYNC_PWR_STA_MASK           (0x1 << 23)
#define AUDIO_PWR_STA_MASK               (0x1 << 24)
#define C2K_PWR_STA_MASK                 (0x1 << 28)
#define MDSYS_INTF_INFRA_PWR_STA_MASK    (0x1 << 29)
/* Define Non-CPU SRAM Mask */
#define MD1_SRAM_PDN                     (0x1 << 8)
#define MD1_SRAM_PDN_ACK                 (0x0 << 12)
#define DIS_SRAM_PDN                     (0x1 << 8)
#define DIS_SRAM_PDN_ACK                 (0x1 << 12)
#define MFG_SRAM_PDN                     (0x1 << 8)
#define MFG_SRAM_PDN_ACK                 (0x1 << 16)
#define ISP_SRAM_PDN                     (0x3 << 8)
#define ISP_SRAM_PDN_ACK                 (0x3 << 12)
#define IFR_SRAM_PDN                     (0xF << 8)
#define IFR_SRAM_PDN_ACK                 (0xF << 12)
#define VDE_SRAM_PDN                     (0x1 << 8)
#define VDE_SRAM_PDN_ACK                 (0x1 << 12)
#define VEN_SRAM_PDN                     (0xF << 8)
#define VEN_SRAM_PDN_ACK                 (0xF << 12)
#define AUDIO_SRAM_PDN                   (0xF << 8)
#define AUDIO_SRAM_PDN_ACK               (0xF << 12)
/* Define CPU SRAM Mask */
#define MP0_CPUTOP_SRAM_PDN              (0x1 << 0)
#define MP0_CPUTOP_SRAM_PDN_ACK          (0x1 << 8)
#define MP0_CPUTOP_SRAM_SLEEP_B          (0x1 << 0)
#define MP0_CPUTOP_SRAM_SLEEP_B_ACK      (0x1 << 8)
#define MP0_CPU0_SRAM_PDN                (0x1 << 0)
#define MP0_CPU0_SRAM_PDN_ACK            (0x1 << 8)
#define MP0_CPU1_SRAM_PDN                (0x1 << 0)
#define MP0_CPU1_SRAM_PDN_ACK            (0x1 << 8)
#define MP0_CPU2_SRAM_PDN                (0x1 << 0)
#define MP0_CPU2_SRAM_PDN_ACK            (0x1 << 8)
#define MP0_CPU3_SRAM_PDN                (0x1 << 0)
#define MP0_CPU3_SRAM_PDN_ACK            (0x1 << 8)
#define MP1_CPUTOP_SRAM_PDN              (0x1 << 0)
#define MP1_CPUTOP_SRAM_PDN_ACK          (0x1 << 8)
#define MP1_CPUTOP_SRAM_SLEEP_B          (0x1 << 0)
#define MP1_CPUTOP_SRAM_SLEEP_B_ACK      (0x1 << 8)
#define MP1_CPU0_SRAM_PDN                (0x1 << 0)
#define MP1_CPU0_SRAM_PDN_ACK            (0x1 << 8)
#define MP1_CPU1_SRAM_PDN                (0x1 << 0)
#define MP1_CPU1_SRAM_PDN_ACK            (0x1 << 8)
#define MP1_CPU2_SRAM_PDN                (0x1 << 0)
#define MP1_CPU2_SRAM_PDN_ACK            (0x1 << 8)
#define MP1_CPU3_SRAM_PDN                (0x1 << 0)
#define MP1_CPU3_SRAM_PDN_ACK            (0x1 << 8)


typedef int (*spm_cpu_mtcmos_ctrl_func)(int state, int chkWfiBeforePdn);
static spm_cpu_mtcmos_ctrl_func spm_cpu_mtcmos_ctrl_funcs[] =
{
    spm_mtcmos_ctrl_cpu0,
    spm_mtcmos_ctrl_cpu1,
    spm_mtcmos_ctrl_cpu2,
    spm_mtcmos_ctrl_cpu3,
    spm_mtcmos_ctrl_cpu4,
    spm_mtcmos_ctrl_cpu5,
    spm_mtcmos_ctrl_cpu6,
    spm_mtcmos_ctrl_cpu7
};
int spm_mtcmos_ctrl_cpu(unsigned int cpu, int state, int chkWfiBeforePdn)
{
    return (*spm_cpu_mtcmos_ctrl_funcs[cpu])(state, chkWfiBeforePdn);
}

int spm_mtcmos_ctrl_cpu0(int state, int chkWfiBeforePdn)
{
    unsigned long flags;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off MP0_CPU0" */
		/* TINFO="Set PWR_ISO = 1" */
        spm_mtcmos_cpu_lock(&flags);
		spm_write(MP0_CPU0_PWR_CON, spm_read(MP0_CPU0_PWR_CON) | PWR_ISO);
		/* TINFO="Set SRAM_CKISO = 1" */
		spm_write(MP0_CPU0_PWR_CON, spm_read(MP0_CPU0_PWR_CON) | SRAM_CKISO);
		/* TINFO="Set SRAM_ISOINT_B = 0" */
		spm_write(MP0_CPU0_PWR_CON, spm_read(MP0_CPU0_PWR_CON) & ~SRAM_ISOINT_B);
		/* TINFO="Set SRAM_PDN = 1" */
		spm_write(MP0_CPU0_L1_PDN, spm_read(MP0_CPU0_L1_PDN) | MP0_CPU0_SRAM_PDN);
		/* TINFO="Wait until MP0_CPU0_SRAM_PDN_ACK = 1" */
		#ifndef CFG_FPGA_PLATFORM
		while (!(spm_read(MP0_CPU0_L1_PDN) & MP0_CPU0_SRAM_PDN_ACK)) {
		}
		#endif //#ifndef CFG_FPGA_PLATFORM
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(MP0_CPU0_PWR_CON, spm_read(MP0_CPU0_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(MP0_CPU0_PWR_CON, spm_read(MP0_CPU0_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(MP0_CPU0_PWR_CON, spm_read(MP0_CPU0_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(MP0_CPU0_PWR_CON, spm_read(MP0_CPU0_PWR_CON) & ~PWR_ON_2ND);

    #ifndef CFG_FPGA_PLATFORM
		/* TINFO="Wait until PWR_STATUS = 0 and PWR_STATUS_2ND = 0" */
		while ((spm_read(PWR_STATUS) & MP0_CPU0_PWR_STA_MASK)
		       || (spm_read(PWR_STATUS_2ND) & MP0_CPU0_PWR_STA_MASK)) {
		}
    #endif //#ifndef CFG_FPGA_PLATFORM
spm_mtcmos_cpu_unlock(&flags);
		/* TINFO="Finish to turn off MP0_CPU0" */
	} else {    /* STA_POWER_ON */
	spm_mtcmos_cpu_lock(&flags);
		/* TINFO="Start to turn on MP0_CPU0" */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(MP0_CPU0_PWR_CON, spm_read(MP0_CPU0_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(MP0_CPU0_PWR_CON, spm_read(MP0_CPU0_PWR_CON) | PWR_ON_2ND);

 #ifndef CFG_FPGA_PLATFORM
		/* TINFO="Wait until PWR_STATUS = 1 and PWR_STATUS_2ND = 1" */
		while (!(spm_read(PWR_STATUS) & MP0_CPU0_PWR_STA_MASK)
		       || !(spm_read(PWR_STATUS_2ND) & MP0_CPU0_PWR_STA_MASK)) {
		}
#endif //#ifndef CFG_FPGA_PLATFORM

		/* TINFO="Set PWR_ISO = 0" */
		spm_write(MP0_CPU0_PWR_CON, spm_read(MP0_CPU0_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set SRAM_PDN = 0" */
		spm_write(MP0_CPU0_L1_PDN, spm_read(MP0_CPU0_L1_PDN) & ~(0x1 << 0));
		/* TINFO="Wait until MP0_CPU0_SRAM_PDN_ACK = 0" */
		#ifndef CFG_FPGA_PLATFORM
		while (spm_read(MP0_CPU0_L1_PDN) & MP0_CPU0_SRAM_PDN_ACK) {
		}
		#endif //#ifndef CFG_FPGA_PLATFORM
		/* TINFO="Delay 1us" */
		udelay(1);
		/* TINFO="Set SRAM_ISOINT_B = 1" */
		spm_write(MP0_CPU0_PWR_CON, spm_read(MP0_CPU0_PWR_CON) | SRAM_ISOINT_B);
		/* TINFO="Set SRAM_CKISO = 0" */
		spm_write(MP0_CPU0_PWR_CON, spm_read(MP0_CPU0_PWR_CON) & ~SRAM_CKISO);
		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(MP0_CPU0_PWR_CON, spm_read(MP0_CPU0_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(MP0_CPU0_PWR_CON, spm_read(MP0_CPU0_PWR_CON) | PWR_RST_B);
		/* TINFO="Finish to turn on MP0_CPU0" */
		spm_mtcmos_cpu_unlock(&flags);
	}
    return 0;
}

int spm_mtcmos_ctrl_cpu1(int state, int chkWfiBeforePdn)
{
    unsigned long flags;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off MP0_CPU1" */
		/* TINFO="Set PWR_ISO = 1" */
        spm_mtcmos_cpu_lock(&flags);
		spm_write(MP0_CPU1_PWR_CON, spm_read(MP0_CPU1_PWR_CON) | PWR_ISO);
		/* TINFO="Set SRAM_CKISO = 1" */
		spm_write(MP0_CPU1_PWR_CON, spm_read(MP0_CPU1_PWR_CON) | SRAM_CKISO);
		/* TINFO="Set SRAM_ISOINT_B = 0" */
		spm_write(MP0_CPU1_PWR_CON, spm_read(MP0_CPU1_PWR_CON) & ~SRAM_ISOINT_B);
		/* TINFO="Set SRAM_PDN = 1" */
		spm_write(MP0_CPU1_L1_PDN, spm_read(MP0_CPU1_L1_PDN) | MP0_CPU1_SRAM_PDN);
		/* TINFO="Wait until MP0_CPU1_SRAM_PDN_ACK = 1" */
		 #ifndef CFG_FPGA_PLATFORM
		while (!(spm_read(MP0_CPU1_L1_PDN) & MP0_CPU1_SRAM_PDN_ACK)) {
		}
		 #endif //#ifndef CFG_FPGA_PLATFORM
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(MP0_CPU1_PWR_CON, spm_read(MP0_CPU1_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(MP0_CPU1_PWR_CON, spm_read(MP0_CPU1_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(MP0_CPU1_PWR_CON, spm_read(MP0_CPU1_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(MP0_CPU1_PWR_CON, spm_read(MP0_CPU1_PWR_CON) & ~PWR_ON_2ND);

    #ifndef CFG_FPGA_PLATFORM
		/* TINFO="Wait until PWR_STATUS = 0 and PWR_STATUS_2ND = 0" */
		while ((spm_read(PWR_STATUS) & MP0_CPU1_PWR_STA_MASK)
		       || (spm_read(PWR_STATUS_2ND) & MP0_CPU1_PWR_STA_MASK)) {
		}
    #endif //#ifndef CFG_FPGA_PLATFORM
	spm_mtcmos_cpu_unlock(&flags);
		/* TINFO="Finish to turn off MP0_CPU1" */
	} else {    /* STA_POWER_ON */
	spm_mtcmos_cpu_lock(&flags);
		/* TINFO="Start to turn on MP0_CPU1" */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(MP0_CPU1_PWR_CON, spm_read(MP0_CPU1_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(MP0_CPU1_PWR_CON, spm_read(MP0_CPU1_PWR_CON) | PWR_ON_2ND);

#ifndef CFG_FPGA_PLATFORM
		/* TINFO="Wait until PWR_STATUS = 1 and PWR_STATUS_2ND = 1" */
		while (!(spm_read(PWR_STATUS) & MP0_CPU1_PWR_STA_MASK)
		       || !(spm_read(PWR_STATUS_2ND) & MP0_CPU1_PWR_STA_MASK)) {
		}
    #endif //#ifndef CFG_FPGA_PLATFORM

		/* TINFO="Set PWR_ISO = 0" */
		spm_write(MP0_CPU1_PWR_CON, spm_read(MP0_CPU1_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set SRAM_PDN = 0" */
		spm_write(MP0_CPU1_L1_PDN, spm_read(MP0_CPU1_L1_PDN) & ~(0x1 << 0));
		/* TINFO="Wait until MP0_CPU1_SRAM_PDN_ACK = 0" */
		#ifndef CFG_FPGA_PLATFORM
		while (spm_read(MP0_CPU1_L1_PDN) & MP0_CPU1_SRAM_PDN_ACK) {
		}
		 #endif //#ifndef CFG_FPGA_PLATFORM
		/* TINFO="Delay 1us" */
		udelay(1);
		/* TINFO="Set SRAM_ISOINT_B = 1" */
		spm_write(MP0_CPU1_PWR_CON, spm_read(MP0_CPU1_PWR_CON) | SRAM_ISOINT_B);
		/* TINFO="Set SRAM_CKISO = 0" */
		spm_write(MP0_CPU1_PWR_CON, spm_read(MP0_CPU1_PWR_CON) & ~SRAM_CKISO);
		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(MP0_CPU1_PWR_CON, spm_read(MP0_CPU1_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(MP0_CPU1_PWR_CON, spm_read(MP0_CPU1_PWR_CON) | PWR_RST_B);
		/* TINFO="Finish to turn on MP0_CPU1" */
        spm_mtcmos_cpu_unlock(&flags);
	}
    return 0;
}

int spm_mtcmos_ctrl_cpu2(int state, int chkWfiBeforePdn)
{
    unsigned long flags;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off MP0_CPU2" */
		/* TINFO="Set PWR_ISO = 1" */
        spm_mtcmos_cpu_lock(&flags);
		spm_write(MP0_CPU2_PWR_CON, spm_read(MP0_CPU2_PWR_CON) | PWR_ISO);
		/* TINFO="Set SRAM_CKISO = 1" */
		spm_write(MP0_CPU2_PWR_CON, spm_read(MP0_CPU2_PWR_CON) | SRAM_CKISO);
		/* TINFO="Set SRAM_ISOINT_B = 0" */
		spm_write(MP0_CPU2_PWR_CON, spm_read(MP0_CPU2_PWR_CON) & ~SRAM_ISOINT_B);
		/* TINFO="Set SRAM_PDN = 1" */
		spm_write(MP0_CPU2_L1_PDN, spm_read(MP0_CPU2_L1_PDN) | MP0_CPU2_SRAM_PDN);
		/* TINFO="Wait until MP0_CPU2_SRAM_PDN_ACK = 1" */
		    #ifndef CFG_FPGA_PLATFORM
		while (!(spm_read(MP0_CPU2_L1_PDN) & MP0_CPU2_SRAM_PDN_ACK)) {
		}
		#endif //#ifndef CFG_FPGA_PLATFORM
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(MP0_CPU2_PWR_CON, spm_read(MP0_CPU2_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(MP0_CPU2_PWR_CON, spm_read(MP0_CPU2_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(MP0_CPU2_PWR_CON, spm_read(MP0_CPU2_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(MP0_CPU2_PWR_CON, spm_read(MP0_CPU2_PWR_CON) & ~PWR_ON_2ND);

    #ifndef CFG_FPGA_PLATFORM
		/* TINFO="Wait until PWR_STATUS = 0 and PWR_STATUS_2ND = 0" */
		while ((spm_read(PWR_STATUS) & MP0_CPU2_PWR_STA_MASK)
		       || (spm_read(PWR_STATUS_2ND) & MP0_CPU2_PWR_STA_MASK)) {
		}
    #endif //#ifndef CFG_FPGA_PLATFORM
	spm_mtcmos_cpu_unlock(&flags);
		/* TINFO="Finish to turn off MP0_CPU2" */
	} else {    /* STA_POWER_ON */
	 	spm_mtcmos_cpu_lock(&flags);
		/* TINFO="Start to turn on MP0_CPU2" */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(MP0_CPU2_PWR_CON, spm_read(MP0_CPU2_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(MP0_CPU2_PWR_CON, spm_read(MP0_CPU2_PWR_CON) | PWR_ON_2ND);

  #ifndef CFG_FPGA_PLATFORM
		/* TINFO="Wait until PWR_STATUS = 1 and PWR_STATUS_2ND = 1" */
		while (!(spm_read(PWR_STATUS) & MP0_CPU2_PWR_STA_MASK)
		       || !(spm_read(PWR_STATUS_2ND) & MP0_CPU2_PWR_STA_MASK)) {
		}
#endif //#ifndef CFG_FPGA_PLATFORM

		/* TINFO="Set PWR_ISO = 0" */
		spm_write(MP0_CPU2_PWR_CON, spm_read(MP0_CPU2_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set SRAM_PDN = 0" */
		spm_write(MP0_CPU2_L1_PDN, spm_read(MP0_CPU2_L1_PDN) & ~(0x1 << 0));
		/* TINFO="Wait until MP0_CPU2_SRAM_PDN_ACK = 0" */
		    #ifndef CFG_FPGA_PLATFORM
		while (spm_read(MP0_CPU2_L1_PDN) & MP0_CPU2_SRAM_PDN_ACK) {
		}
		 #endif //#ifndef CFG_FPGA_PLATFORM
		/* TINFO="Delay 1us" */
		udelay(1);
		/* TINFO="Set SRAM_ISOINT_B = 1" */
		spm_write(MP0_CPU2_PWR_CON, spm_read(MP0_CPU2_PWR_CON) | SRAM_ISOINT_B);
		/* TINFO="Set SRAM_CKISO = 0" */
		spm_write(MP0_CPU2_PWR_CON, spm_read(MP0_CPU2_PWR_CON) & ~SRAM_CKISO);
		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(MP0_CPU2_PWR_CON, spm_read(MP0_CPU2_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(MP0_CPU2_PWR_CON, spm_read(MP0_CPU2_PWR_CON) | PWR_RST_B);
		/* TINFO="Finish to turn on MP0_CPU2" */
        spm_mtcmos_cpu_unlock(&flags);
	}
    return 0;
}

int spm_mtcmos_ctrl_cpu3(int state, int chkWfiBeforePdn)
{
    unsigned long flags;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off MP0_CPU3" */
		/* TINFO="Set PWR_ISO = 1" */
        spm_mtcmos_cpu_lock(&flags);
		spm_write(MP0_CPU3_PWR_CON, spm_read(MP0_CPU3_PWR_CON) | PWR_ISO);
		/* TINFO="Set SRAM_CKISO = 1" */
		spm_write(MP0_CPU3_PWR_CON, spm_read(MP0_CPU3_PWR_CON) | SRAM_CKISO);
		/* TINFO="Set SRAM_ISOINT_B = 0" */
		spm_write(MP0_CPU3_PWR_CON, spm_read(MP0_CPU3_PWR_CON) & ~SRAM_ISOINT_B);
		/* TINFO="Set SRAM_PDN = 1" */
		spm_write(MP0_CPU3_L1_PDN, spm_read(MP0_CPU3_L1_PDN) | MP0_CPU3_SRAM_PDN);
		/* TINFO="Wait until MP0_CPU3_SRAM_PDN_ACK = 1" */
		   #ifndef CFG_FPGA_PLATFORM
		while (!(spm_read(MP0_CPU3_L1_PDN) & MP0_CPU3_SRAM_PDN_ACK)) {
		}
		#endif //#ifndef CFG_FPGA_PLATFORM
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(MP0_CPU3_PWR_CON, spm_read(MP0_CPU3_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(MP0_CPU3_PWR_CON, spm_read(MP0_CPU3_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(MP0_CPU3_PWR_CON, spm_read(MP0_CPU3_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(MP0_CPU3_PWR_CON, spm_read(MP0_CPU3_PWR_CON) & ~PWR_ON_2ND);

    #ifndef CFG_FPGA_PLATFORM
		/* TINFO="Wait until PWR_STATUS = 0 and PWR_STATUS_2ND = 0" */
		while ((spm_read(PWR_STATUS) & MP0_CPU3_PWR_STA_MASK)
		       || (spm_read(PWR_STATUS_2ND) & MP0_CPU3_PWR_STA_MASK)) {
		}
    #endif //#ifndef CFG_FPGA_PLATFORM
    spm_mtcmos_cpu_unlock(&flags);
		/* TINFO="Finish to turn off MP0_CPU3" */
	} else {    /* STA_POWER_ON */
	spm_mtcmos_cpu_lock(&flags);
		/* TINFO="Start to turn on MP0_CPU3" */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(MP0_CPU3_PWR_CON, spm_read(MP0_CPU3_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(MP0_CPU3_PWR_CON, spm_read(MP0_CPU3_PWR_CON) | PWR_ON_2ND);

#ifndef CFG_FPGA_PLATFORM
		/* TINFO="Wait until PWR_STATUS = 1 and PWR_STATUS_2ND = 1" */
		while (!(spm_read(PWR_STATUS) & MP0_CPU3_PWR_STA_MASK)
		       || !(spm_read(PWR_STATUS_2ND) & MP0_CPU3_PWR_STA_MASK)) {
		}
#endif //#ifndef CFG_FPGA_PLATFORM

		/* TINFO="Set PWR_ISO = 0" */
		spm_write(MP0_CPU3_PWR_CON, spm_read(MP0_CPU3_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set SRAM_PDN = 0" */
		spm_write(MP0_CPU3_L1_PDN, spm_read(MP0_CPU3_L1_PDN) & ~(0x1 << 0));
		/* TINFO="Wait until MP0_CPU3_SRAM_PDN_ACK = 0" */
		 #ifndef CFG_FPGA_PLATFORM
		while (spm_read(MP0_CPU3_L1_PDN) & MP0_CPU3_SRAM_PDN_ACK) {
		}
		#endif //#ifndef CFG_FPGA_PLATFORM
		/* TINFO="Delay 1us" */
		udelay(1);
		/* TINFO="Set SRAM_ISOINT_B = 1" */
		spm_write(MP0_CPU3_PWR_CON, spm_read(MP0_CPU3_PWR_CON) | SRAM_ISOINT_B);
		/* TINFO="Set SRAM_CKISO = 0" */
		spm_write(MP0_CPU3_PWR_CON, spm_read(MP0_CPU3_PWR_CON) & ~SRAM_CKISO);
		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(MP0_CPU3_PWR_CON, spm_read(MP0_CPU3_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(MP0_CPU3_PWR_CON, spm_read(MP0_CPU3_PWR_CON) | PWR_RST_B);
		/* TINFO="Finish to turn on MP0_CPU3" */
        spm_mtcmos_cpu_unlock(&flags);
	}
	return 0;
}

int spm_mtcmos_ctrl_cpu4(int state, int chkWfiBeforePdn)
{
    unsigned long flags;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off MP1_CPU0" */
		/* TINFO="Set PWR_ISO = 1" */
        spm_mtcmos_cpu_lock(&flags);
		spm_write(MP1_CPU0_PWR_CON, spm_read(MP1_CPU0_PWR_CON) | PWR_ISO);
		/* TINFO="Set SRAM_CKISO = 1" */
		spm_write(MP1_CPU0_PWR_CON, spm_read(MP1_CPU0_PWR_CON) | SRAM_CKISO);
		/* TINFO="Set SRAM_ISOINT_B = 0" */
		spm_write(MP1_CPU0_PWR_CON, spm_read(MP1_CPU0_PWR_CON) & ~SRAM_ISOINT_B);
		/* TINFO="Set SRAM_PDN = 1" */
		spm_write(MP1_CPU0_L1_PDN, spm_read(MP1_CPU0_L1_PDN) | MP1_CPU0_SRAM_PDN);
		/* TINFO="Wait until MP1_CPU0_SRAM_PDN_ACK = 1" */
		   #ifndef CFG_FPGA_PLATFORM
		while (!(spm_read(MP1_CPU0_L1_PDN) & MP1_CPU0_SRAM_PDN_ACK)) {
		}
		#endif //#ifndef CFG_FPGA_PLATFORM
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(MP1_CPU0_PWR_CON, spm_read(MP1_CPU0_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(MP1_CPU0_PWR_CON, spm_read(MP1_CPU0_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(MP1_CPU0_PWR_CON, spm_read(MP1_CPU0_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(MP1_CPU0_PWR_CON, spm_read(MP1_CPU0_PWR_CON) & ~PWR_ON_2ND);

    #ifndef CFG_FPGA_PLATFORM
		/* TINFO="Wait until PWR_STATUS = 0 and PWR_STATUS_2ND = 0" */
		while ((spm_read(PWR_STATUS) & MP1_CPU0_PWR_STA_MASK)
		       || (spm_read(PWR_STATUS_2ND) & MP1_CPU0_PWR_STA_MASK)) {
		}
    #endif //#ifndef CFG_FPGA_PLATFORM
	spm_mtcmos_cpu_unlock(&flags);

		/* TINFO="Finish to turn off MP1_CPU0" */
	} else {    /* STA_POWER_ON */
	spm_mtcmos_cpu_lock(&flags);
		/* TINFO="Start to turn on MP1_CPU0" */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(MP1_CPU0_PWR_CON, spm_read(MP1_CPU0_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(MP1_CPU0_PWR_CON, spm_read(MP1_CPU0_PWR_CON) | PWR_ON_2ND);

#ifndef CFG_FPGA_PLATFORM
		/* TINFO="Wait until PWR_STATUS = 1 and PWR_STATUS_2ND = 1" */
		while (!(spm_read(PWR_STATUS) & MP1_CPU0_PWR_STA_MASK)
		       || !(spm_read(PWR_STATUS_2ND) & MP1_CPU0_PWR_STA_MASK)) {
		}
    #endif //#ifndef CFG_FPGA_PLATFORM

		/* TINFO="Set PWR_ISO = 0" */
		spm_write(MP1_CPU0_PWR_CON, spm_read(MP1_CPU0_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set SRAM_PDN = 0" */
		spm_write(MP1_CPU0_L1_PDN, spm_read(MP1_CPU0_L1_PDN) & ~(0x1 << 0));
		/* TINFO="Wait until MP1_CPU0_SRAM_PDN_ACK = 0" */
		  #ifndef CFG_FPGA_PLATFORM
		while (spm_read(MP1_CPU0_L1_PDN) & MP1_CPU0_SRAM_PDN_ACK) {
		}
		 #endif //#ifndef CFG_FPGA_PLATFORM
		/* TINFO="Delay 1us" */
		udelay(1);
		/* TINFO="Set SRAM_ISOINT_B = 1" */
		spm_write(MP1_CPU0_PWR_CON, spm_read(MP1_CPU0_PWR_CON) | SRAM_ISOINT_B);
		/* TINFO="Set SRAM_CKISO = 0" */
		spm_write(MP1_CPU0_PWR_CON, spm_read(MP1_CPU0_PWR_CON) & ~SRAM_CKISO);
		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(MP1_CPU0_PWR_CON, spm_read(MP1_CPU0_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(MP1_CPU0_PWR_CON, spm_read(MP1_CPU0_PWR_CON) | PWR_RST_B);
		/* TINFO="Finish to turn on MP1_CPU0" */
        spm_mtcmos_cpu_unlock(&flags);
	}
    return 0;
}

int spm_mtcmos_ctrl_cpu5(int state, int chkWfiBeforePdn)
{
    unsigned long flags;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off MP1_CPU1" */
		/* TINFO="Set PWR_ISO = 1" */
        spm_mtcmos_cpu_lock(&flags);
		spm_write(MP1_CPU1_PWR_CON, spm_read(MP1_CPU1_PWR_CON) | PWR_ISO);
		/* TINFO="Set SRAM_CKISO = 1" */
		spm_write(MP1_CPU1_PWR_CON, spm_read(MP1_CPU1_PWR_CON) | SRAM_CKISO);
		/* TINFO="Set SRAM_ISOINT_B = 0" */
		spm_write(MP1_CPU1_PWR_CON, spm_read(MP1_CPU1_PWR_CON) & ~SRAM_ISOINT_B);
		/* TINFO="Set SRAM_PDN = 1" */
		spm_write(MP1_CPU1_L1_PDN, spm_read(MP1_CPU1_L1_PDN) | MP1_CPU1_SRAM_PDN);
		/* TINFO="Wait until MP1_CPU1_SRAM_PDN_ACK = 1" */
		    #ifndef CFG_FPGA_PLATFORM
		while (!(spm_read(MP1_CPU1_L1_PDN) & MP1_CPU1_SRAM_PDN_ACK)) {
		}
		#endif //#ifndef CFG_FPGA_PLATFORM
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(MP1_CPU1_PWR_CON, spm_read(MP1_CPU1_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(MP1_CPU1_PWR_CON, spm_read(MP1_CPU1_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(MP1_CPU1_PWR_CON, spm_read(MP1_CPU1_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(MP1_CPU1_PWR_CON, spm_read(MP1_CPU1_PWR_CON) & ~PWR_ON_2ND);

    #ifndef CFG_FPGA_PLATFORM
		/* TINFO="Wait until PWR_STATUS = 0 and PWR_STATUS_2ND = 0" */
		while ((spm_read(PWR_STATUS) & MP1_CPU1_PWR_STA_MASK)
		       || (spm_read(PWR_STATUS_2ND) & MP1_CPU1_PWR_STA_MASK)) {
		}
    #endif //#ifndef CFG_FPGA_PLATFORM
spm_mtcmos_cpu_unlock(&flags);
		/* TINFO="Finish to turn off MP1_CPU1" */
	} else {    /* STA_POWER_ON */
	spm_mtcmos_cpu_lock(&flags);
		/* TINFO="Start to turn on MP1_CPU1" */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(MP1_CPU1_PWR_CON, spm_read(MP1_CPU1_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(MP1_CPU1_PWR_CON, spm_read(MP1_CPU1_PWR_CON) | PWR_ON_2ND);

 #ifndef CFG_FPGA_PLATFORM
		/* TINFO="Wait until PWR_STATUS = 1 and PWR_STATUS_2ND = 1" */
		while (!(spm_read(PWR_STATUS) & MP1_CPU1_PWR_STA_MASK)
		       || !(spm_read(PWR_STATUS_2ND) & MP1_CPU1_PWR_STA_MASK)) {
		}
    #endif //#ifndef CFG_FPGA_PLATFORM

		/* TINFO="Set PWR_ISO = 0" */
		spm_write(MP1_CPU1_PWR_CON, spm_read(MP1_CPU1_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set SRAM_PDN = 0" */
		spm_write(MP1_CPU1_L1_PDN, spm_read(MP1_CPU1_L1_PDN) & ~(0x1 << 0));
		/* TINFO="Wait until MP1_CPU1_SRAM_PDN_ACK = 0" */
		    #ifndef CFG_FPGA_PLATFORM
		while (spm_read(MP1_CPU1_L1_PDN) & MP1_CPU1_SRAM_PDN_ACK) {
		}
		#endif //#ifndef CFG_FPGA_PLATFORM
		/* TINFO="Delay 1us" */
		udelay(1);
		/* TINFO="Set SRAM_ISOINT_B = 1" */
		spm_write(MP1_CPU1_PWR_CON, spm_read(MP1_CPU1_PWR_CON) | SRAM_ISOINT_B);
		/* TINFO="Set SRAM_CKISO = 0" */
		spm_write(MP1_CPU1_PWR_CON, spm_read(MP1_CPU1_PWR_CON) & ~SRAM_CKISO);
		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(MP1_CPU1_PWR_CON, spm_read(MP1_CPU1_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(MP1_CPU1_PWR_CON, spm_read(MP1_CPU1_PWR_CON) | PWR_RST_B);
		/* TINFO="Finish to turn on MP1_CPU1" */
        spm_mtcmos_cpu_unlock(&flags);
	}
    return 0;
}

int spm_mtcmos_ctrl_cpu6(int state, int chkWfiBeforePdn)
{
    unsigned long flags;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off MP1_CPU2" */
		/* TINFO="Set PWR_ISO = 1" */
        spm_mtcmos_cpu_lock(&flags);
		spm_write(MP1_CPU2_PWR_CON, spm_read(MP1_CPU2_PWR_CON) | PWR_ISO);
		/* TINFO="Set SRAM_CKISO = 1" */
		spm_write(MP1_CPU2_PWR_CON, spm_read(MP1_CPU2_PWR_CON) | SRAM_CKISO);
		/* TINFO="Set SRAM_ISOINT_B = 0" */
		spm_write(MP1_CPU2_PWR_CON, spm_read(MP1_CPU2_PWR_CON) & ~SRAM_ISOINT_B);
		/* TINFO="Set SRAM_PDN = 1" */
		spm_write(MP1_CPU2_L1_PDN, spm_read(MP1_CPU2_L1_PDN) | MP1_CPU2_SRAM_PDN);
		/* TINFO="Wait until MP1_CPU2_SRAM_PDN_ACK = 1" */
		 #ifndef CFG_FPGA_PLATFORM
		while (!(spm_read(MP1_CPU2_L1_PDN) & MP1_CPU2_SRAM_PDN_ACK)) {
		}
		#endif //#ifndef CFG_FPGA_PLATFORM
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(MP1_CPU2_PWR_CON, spm_read(MP1_CPU2_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(MP1_CPU2_PWR_CON, spm_read(MP1_CPU2_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(MP1_CPU2_PWR_CON, spm_read(MP1_CPU2_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(MP1_CPU2_PWR_CON, spm_read(MP1_CPU2_PWR_CON) & ~PWR_ON_2ND);

    #ifndef CFG_FPGA_PLATFORM
		/* TINFO="Wait until PWR_STATUS = 0 and PWR_STATUS_2ND = 0" */
		while ((spm_read(PWR_STATUS) & MP1_CPU2_PWR_STA_MASK)
		       || (spm_read(PWR_STATUS_2ND) & MP1_CPU2_PWR_STA_MASK)) {
		}
    #endif //#ifndef CFG_FPGA_PLATFORM
		spm_mtcmos_cpu_unlock(&flags);
		/* TINFO="Finish to turn off MP1_CPU2" */
	} else {    /* STA_POWER_ON */
	spm_mtcmos_cpu_lock(&flags);
		/* TINFO="Start to turn on MP1_CPU2" */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(MP1_CPU2_PWR_CON, spm_read(MP1_CPU2_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(MP1_CPU2_PWR_CON, spm_read(MP1_CPU2_PWR_CON) | PWR_ON_2ND);

 #ifndef CFG_FPGA_PLATFORM
		/* TINFO="Wait until PWR_STATUS = 1 and PWR_STATUS_2ND = 1" */
		while (!(spm_read(PWR_STATUS) & MP1_CPU2_PWR_STA_MASK)
		       || !(spm_read(PWR_STATUS_2ND) & MP1_CPU2_PWR_STA_MASK)) {
		}
    #endif //#ifndef CFG_FPGA_PLATFORM

		/* TINFO="Set PWR_ISO = 0" */
		spm_write(MP1_CPU2_PWR_CON, spm_read(MP1_CPU2_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set SRAM_PDN = 0" */
		spm_write(MP1_CPU2_L1_PDN, spm_read(MP1_CPU2_L1_PDN) & ~(0x1 << 0));
		/* TINFO="Wait until MP1_CPU2_SRAM_PDN_ACK = 0" */
		    #ifndef CFG_FPGA_PLATFORM
		while (spm_read(MP1_CPU2_L1_PDN) & MP1_CPU2_SRAM_PDN_ACK) {
		}
		 #endif //#ifndef CFG_FPGA_PLATFORM
		/* TINFO="Delay 1us" */
		udelay(1);
		/* TINFO="Set SRAM_ISOINT_B = 1" */
		spm_write(MP1_CPU2_PWR_CON, spm_read(MP1_CPU2_PWR_CON) | SRAM_ISOINT_B);
		/* TINFO="Set SRAM_CKISO = 0" */
		spm_write(MP1_CPU2_PWR_CON, spm_read(MP1_CPU2_PWR_CON) & ~SRAM_CKISO);
		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(MP1_CPU2_PWR_CON, spm_read(MP1_CPU2_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(MP1_CPU2_PWR_CON, spm_read(MP1_CPU2_PWR_CON) | PWR_RST_B);
		/* TINFO="Finish to turn on MP1_CPU2" */
        spm_mtcmos_cpu_unlock(&flags);
	}
    return 0;
}

int spm_mtcmos_ctrl_cpu7(int state, int chkWfiBeforePdn)
{
    unsigned long flags;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off MP1_CPU3" */
		/* TINFO="Set PWR_ISO = 1" */
        spm_mtcmos_cpu_lock(&flags);
		spm_write(MP1_CPU3_PWR_CON, spm_read(MP1_CPU3_PWR_CON) | PWR_ISO);
		/* TINFO="Set SRAM_CKISO = 1" */
		spm_write(MP1_CPU3_PWR_CON, spm_read(MP1_CPU3_PWR_CON) | SRAM_CKISO);
		/* TINFO="Set SRAM_ISOINT_B = 0" */
		spm_write(MP1_CPU3_PWR_CON, spm_read(MP1_CPU3_PWR_CON) & ~SRAM_ISOINT_B);
		/* TINFO="Set SRAM_PDN = 1" */
		spm_write(MP1_CPU3_L1_PDN, spm_read(MP1_CPU3_L1_PDN) | MP1_CPU3_SRAM_PDN);
		/* TINFO="Wait until MP1_CPU3_SRAM_PDN_ACK = 1" */
		    #ifndef CFG_FPGA_PLATFORM
		while (!(spm_read(MP1_CPU3_L1_PDN) & MP1_CPU3_SRAM_PDN_ACK)) {
		}
		 #endif //#ifndef CFG_FPGA_PLATFORM
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(MP1_CPU3_PWR_CON, spm_read(MP1_CPU3_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(MP1_CPU3_PWR_CON, spm_read(MP1_CPU3_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(MP1_CPU3_PWR_CON, spm_read(MP1_CPU3_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(MP1_CPU3_PWR_CON, spm_read(MP1_CPU3_PWR_CON) & ~PWR_ON_2ND);

    #ifndef CFG_FPGA_PLATFORM
		/* TINFO="Wait until PWR_STATUS = 0 and PWR_STATUS_2ND = 0" */
		while ((spm_read(PWR_STATUS) & MP1_CPU3_PWR_STA_MASK)
		       || (spm_read(PWR_STATUS_2ND) & MP1_CPU3_PWR_STA_MASK)) {
		}
    #endif //#ifndef CFG_FPGA_PLATFORM
spm_mtcmos_cpu_unlock(&flags);
		/* TINFO="Finish to turn off MP1_CPU3" */
	} else {    /* STA_POWER_ON */
	spm_mtcmos_cpu_lock(&flags);
		/* TINFO="Start to turn on MP1_CPU3" */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(MP1_CPU3_PWR_CON, spm_read(MP1_CPU3_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(MP1_CPU3_PWR_CON, spm_read(MP1_CPU3_PWR_CON) | PWR_ON_2ND);

    #ifndef CFG_FPGA_PLATFORM
		/* TINFO="Wait until PWR_STATUS = 1 and PWR_STATUS_2ND = 1" */
		while (!(spm_read(PWR_STATUS) & MP1_CPU3_PWR_STA_MASK)
		       || !(spm_read(PWR_STATUS_2ND) & MP1_CPU3_PWR_STA_MASK)) {
		}
    #endif //#ifndef CFG_FPGA_PLATFORM

		/* TINFO="Set PWR_ISO = 0" */
		spm_write(MP1_CPU3_PWR_CON, spm_read(MP1_CPU3_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set SRAM_PDN = 0" */
		spm_write(MP1_CPU3_L1_PDN, spm_read(MP1_CPU3_L1_PDN) & ~(0x1 << 0));
		/* TINFO="Wait until MP1_CPU3_SRAM_PDN_ACK = 0" */
		 #ifndef CFG_FPGA_PLATFORM		
		while (spm_read(MP1_CPU3_L1_PDN) & MP1_CPU3_SRAM_PDN_ACK) {
		}
		#endif
		/* TINFO="Delay 1us" */
		udelay(1);
		/* TINFO="Set SRAM_ISOINT_B = 1" */
		spm_write(MP1_CPU3_PWR_CON, spm_read(MP1_CPU3_PWR_CON) | SRAM_ISOINT_B);
		/* TINFO="Set SRAM_CKISO = 0" */
		spm_write(MP1_CPU3_PWR_CON, spm_read(MP1_CPU3_PWR_CON) & ~SRAM_CKISO);
		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(MP1_CPU3_PWR_CON, spm_read(MP1_CPU3_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(MP1_CPU3_PWR_CON, spm_read(MP1_CPU3_PWR_CON) | PWR_RST_B);
		/* TINFO="Finish to turn on MP1_CPU3" */
        spm_mtcmos_cpu_unlock(&flags);
	}
    return 0;
}



int spm_mtcmos_ctrl_cpusys0(int state, int chkWfiBeforePdn)/* Shut down/Dormant mode*/
{
    int err = 0;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off MP1_CPUTOP" */
		/* TINFO="Set bus protect" */
		spm_write(INFRA_TOPAXI_PROTECTEN, spm_read(INFRA_TOPAXI_PROTECTEN) | MP1_CPUTOP_PROT_MASK);
		while ((spm_read(INFRA_TOPAXI_PROTECTSTA1) & MP1_CPUTOP_PROT_MASK) != MP1_CPUTOP_PROT_MASK) {
		}
		/* TINFO="Set PWR_ISO = 1" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) | PWR_ISO);
		/* TINFO="Set SRAM_CKISO = 1" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) | SRAM_CKISO);
		/* TINFO="Set SRAM_ISOINT_B = 0" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) & ~SRAM_ISOINT_B);
		/* TINFO="Set SRAM_PDN = 1" */
		spm_write(MP1_CPUTOP_L2_PDN, spm_read(MP1_CPUTOP_L2_PDN) | MP1_CPUTOP_SRAM_PDN);
		/* TINFO="Wait until MP1_CPUTOP_SRAM_PDN_ACK = 1" */
		while (!(spm_read(MP1_CPUTOP_L2_PDN) & MP1_CPUTOP_SRAM_PDN_ACK)) {
		}
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) & ~PWR_ON_2ND);

#ifndef IGNORE_PWR_ACK
		/* TINFO="Wait until PWR_STATUS = 0 and PWR_STATUS_2ND = 0" */
		while ((spm_read(PWR_STATUS) & MP1_CPUTOP_PWR_STA_MASK)
		       || (spm_read(PWR_STATUS_2ND) & MP1_CPUTOP_PWR_STA_MASK)) {
		}
#endif

		/* TINFO="Finish to turn off MP1_CPUTOP" */
	} else {    /* STA_POWER_ON */
		/* TINFO="Start to turn on MP1_CPUTOP" */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) | PWR_ON_2ND);

#ifndef IGNORE_PWR_ACK
		/* TINFO="Wait until PWR_STATUS = 1 and PWR_STATUS_2ND = 1" */
		while (!(spm_read(PWR_STATUS) & MP1_CPUTOP_PWR_STA_MASK)
		       || !(spm_read(PWR_STATUS_2ND) & MP1_CPUTOP_PWR_STA_MASK)) {
		}
#endif

		/* TINFO="Set PWR_ISO = 0" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set SRAM_PDN = 0" */
		spm_write(MP1_CPUTOP_L2_PDN, spm_read(MP1_CPUTOP_L2_PDN) & ~(0x1 << 0));
		/* TINFO="Wait until MP1_CPUTOP_SRAM_PDN_ACK = 0" */
		while (spm_read(MP1_CPUTOP_L2_PDN) & MP1_CPUTOP_SRAM_PDN_ACK) {
		}
		/* TINFO="Delay 1us" */
		udelay(1);
		/* TINFO="Set SRAM_ISOINT_B = 1" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) | SRAM_ISOINT_B);
		/* TINFO="Delay 1us" */
		udelay(1);
		/* TINFO="Set SRAM_CKISO = 0" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) & ~SRAM_CKISO);
		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) | PWR_RST_B);
		/* TINFO="Release bus protect" */
		spm_write(INFRA_TOPAXI_PROTECTEN, spm_read(INFRA_TOPAXI_PROTECTEN) & ~MP1_CPUTOP_PROT_MASK);
		while (spm_read(INFRA_TOPAXI_PROTECTSTA1) & MP1_CPUTOP_PROT_MASK) {
		}
		/* TINFO="Finish to turn on MP1_CPUTOP" */
	}
	return err;
}

int spm_mtcmos_ctrl_cpusys1(int state, int chkWfiBeforePdn)/* Shut down/Dormant mode*/
{
   int err = 0;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off MP1_CPUTOP" */
		/* TINFO="Set bus protect" */
        spm_mtcmos_cpu_lock(&flags);
		spm_write(INFRA_TOPAXI_PROTECTEN, spm_read(INFRA_TOPAXI_PROTECTEN) | MP1_CPUTOP_PROT_MASK);
		while ((spm_read(INFRA_TOPAXI_PROTECTSTA1) & MP1_CPUTOP_PROT_MASK) != MP1_CPUTOP_PROT_MASK) {
		}
		/* TINFO="Set PWR_ISO = 1" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) | PWR_ISO);
		/* TINFO="Set SRAM_CKISO = 1" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) | SRAM_CKISO);
		/* TINFO="Set SRAM_ISOINT_B = 0" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) & ~SRAM_ISOINT_B);
		/* TINFO="Set SRAM_PDN = 1" */
		spm_write(MP1_CPUTOP_L2_PDN, spm_read(MP1_CPUTOP_L2_PDN) | MP1_CPUTOP_SRAM_PDN);
		/* TINFO="Wait until MP1_CPUTOP_SRAM_PDN_ACK = 1" */
    #ifndef CFG_FPGA_PLATFORM
		while (!(spm_read(MP1_CPUTOP_L2_PDN) & MP1_CPUTOP_SRAM_PDN_ACK)) {
		}
		#endif //#ifndef CFG_FPGA_PLATFORM
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) & ~PWR_ON_2ND);

    #ifndef CFG_FPGA_PLATFORM
		/* TINFO="Wait until PWR_STATUS = 0 and PWR_STATUS_2ND = 0" */
		while ((spm_read(PWR_STATUS) & MP1_CPUTOP_PWR_STA_MASK)
		       || (spm_read(PWR_STATUS_2ND) & MP1_CPUTOP_PWR_STA_MASK)) {
		}
#endif //#ifndef CFG_FPGA_PLATFORM

		/* TINFO="Finish to turn off MP1_CPUTOP" */
	} else {    /* STA_POWER_ON */
	spm_mtcmos_cpu_lock(&flags);
		/* TINFO="Start to turn on MP1_CPUTOP" */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) | PWR_ON_2ND);

  #ifndef CFG_FPGA_PLATFORM
		/* TINFO="Wait until PWR_STATUS = 1 and PWR_STATUS_2ND = 1" */
		while (!(spm_read(PWR_STATUS) & MP1_CPUTOP_PWR_STA_MASK)
		       || !(spm_read(PWR_STATUS_2ND) & MP1_CPUTOP_PWR_STA_MASK)) {
		}
#endif //#ifndef CFG_FPGA_PLATFORM

		/* TINFO="Set PWR_ISO = 0" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set SRAM_PDN = 0" */
		spm_write(MP1_CPUTOP_L2_PDN, spm_read(MP1_CPUTOP_L2_PDN) & ~(0x1 << 0));
		/* TINFO="Wait until MP1_CPUTOP_SRAM_PDN_ACK = 0" */
		 #ifndef CFG_FPGA_PLATFORM
		while (spm_read(MP1_CPUTOP_L2_PDN) & MP1_CPUTOP_SRAM_PDN_ACK) {
		}
		#endif //#ifndef CFG_FPGA_PLATFORM
		/* TINFO="Delay 1us" */
		udelay(1);
		/* TINFO="Set SRAM_ISOINT_B = 1" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) | SRAM_ISOINT_B);
		/* TINFO="Delay 1us" */
		udelay(1);
		/* TINFO="Set SRAM_CKISO = 0" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) & ~SRAM_CKISO);
		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(MP1_CPUTOP_PWR_CON, spm_read(MP1_CPUTOP_PWR_CON) | PWR_RST_B);
		/* TINFO="Release bus protect" */
		spm_write(INFRA_TOPAXI_PROTECTEN, spm_read(INFRA_TOPAXI_PROTECTEN) & ~MP1_CPUTOP_PROT_MASK);
		 #ifndef CFG_FPGA_PLATFORM
		while (spm_read(INFRA_TOPAXI_PROTECTSTA1) & MP1_CPUTOP_PROT_MASK) {
		}
		 #endif //#ifndef CFG_FPGA_PLATFORM
		 spm_mtcmos_cpu_unlock(&flags);
		/* TINFO="Finish to turn on MP1_CPUTOP" */
	}
	return 0;
}

void spm_mtcmos_ctrl_cpusys1_init_1st_bring_up(int state)
{

    if (state == STA_POWER_DOWN)
    {
        spm_mtcmos_ctrl_cpu7(STA_POWER_DOWN, 0);
        spm_mtcmos_ctrl_cpu6(STA_POWER_DOWN, 0);
        spm_mtcmos_ctrl_cpu5(STA_POWER_DOWN, 0);
        spm_mtcmos_ctrl_cpu4(STA_POWER_DOWN, 0);
    }
    else /* STA_POWER_ON */
    {
        spm_mtcmos_ctrl_cpu4(STA_POWER_ON, 1);
        spm_mtcmos_ctrl_cpu5(STA_POWER_ON, 1);
        spm_mtcmos_ctrl_cpu6(STA_POWER_ON, 1);
        spm_mtcmos_ctrl_cpu7(STA_POWER_ON, 1);
        //spm_mtcmos_ctrl_dbg1(STA_POWER_ON);
    }

}

bool spm_cpusys0_can_power_down(void)
{
    return !(spm_read(PWR_STATUS) & (CA15_CPU0 | CA15_CPU1 | CA15_CPU2 | CA15_CPU3 | CA15_CPUTOP | CA7_CPU1 | CA7_CPU2 | CA7_CPU3)) &&
           !(spm_read(PWR_STATUS_2ND) & (CA15_CPU0 | CA15_CPU1 | CA15_CPU2 | CA15_CPU3 | CA15_CPUTOP | CA7_CPU1 | CA7_CPU2 | CA7_CPU3));
}

bool spm_cpusys1_can_power_down(void)
{
    return !(spm_read(PWR_STATUS) & (CA7_CPU0 | CA7_CPU1 | CA7_CPU2 | CA7_CPU3 | CA7_CPUTOP | CA15_CPU1 | CA15_CPU2 | CA15_CPU3)) &&
           !(spm_read(PWR_STATUS_2ND) & (CA7_CPU0 | CA7_CPU1 | CA7_CPU2 | CA7_CPU3 | CA7_CPUTOP | CA15_CPU1 | CA15_CPU2 | CA15_CPU3));
}



int spm_mtcmos_ctrl_vdec(int state)
{
 int err = 0;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off VDE" */
		/* TINFO="Set SRAM_PDN = 1" */
		spm_write(VDE_PWR_CON, spm_read(VDE_PWR_CON) | VDE_SRAM_PDN);
		/* TINFO="Wait until VDE_SRAM_PDN_ACK = 1" */
		while (!(spm_read(VDE_PWR_CON) & VDE_SRAM_PDN_ACK)) {
		}
		/* TINFO="Set PWR_ISO = 1" */
		spm_write(VDE_PWR_CON, spm_read(VDE_PWR_CON) | PWR_ISO);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(VDE_PWR_CON, spm_read(VDE_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(VDE_PWR_CON, spm_read(VDE_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(VDE_PWR_CON, spm_read(VDE_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(VDE_PWR_CON, spm_read(VDE_PWR_CON) & ~PWR_ON_2ND);

	 #ifndef CFG_FPGA_PLATFORM
		/* TINFO="Wait until PWR_STATUS = 0 and PWR_STATUS_2ND = 0" */
		while ((spm_read(PWR_STATUS) & VDE_PWR_STA_MASK)
		       || (spm_read(PWR_STATUS_2ND) & VDE_PWR_STA_MASK)) {
		}
#endif

		/* TINFO="Finish to turn off VDE" */
	} else {    /* STA_POWER_ON */
		/* TINFO="Start to turn on VDE" */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(VDE_PWR_CON, spm_read(VDE_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(VDE_PWR_CON, spm_read(VDE_PWR_CON) | PWR_ON_2ND);

#ifndef IGNORE_PWR_ACK
		/* TINFO="Wait until PWR_STATUS = 1 and PWR_STATUS_2ND = 1" */
		while (!(spm_read(PWR_STATUS) & VDE_PWR_STA_MASK)
		       || !(spm_read(PWR_STATUS_2ND) & VDE_PWR_STA_MASK)) {
		}
#endif

		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(VDE_PWR_CON, spm_read(VDE_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_ISO = 0" */
		spm_write(VDE_PWR_CON, spm_read(VDE_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(VDE_PWR_CON, spm_read(VDE_PWR_CON) | PWR_RST_B);
		/* TINFO="Set SRAM_PDN = 0" */
		spm_write(VDE_PWR_CON, spm_read(VDE_PWR_CON) & ~(0x1 << 8));
		/* TINFO="Wait until VDE_SRAM_PDN_ACK = 0" */
		while (spm_read(VDE_PWR_CON) & VDE_SRAM_PDN_ACK) {
		}
		/* TINFO="Finish to turn on VDE" */
	}
	return err;
}

int spm_mtcmos_ctrl_venc(int state)
{
 int err = 0;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off VEN" */
		/* TINFO="Set SRAM_PDN = 1" */
		spm_write(VEN_PWR_CON, spm_read(VEN_PWR_CON) | VEN_SRAM_PDN);
		/* TINFO="Wait until VEN_SRAM_PDN_ACK = 1" */
		while (!(spm_read(VEN_PWR_CON) & VEN_SRAM_PDN_ACK)) {
		}
		/* TINFO="Set PWR_ISO = 1" */
		spm_write(VEN_PWR_CON, spm_read(VEN_PWR_CON) | PWR_ISO);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(VEN_PWR_CON, spm_read(VEN_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(VEN_PWR_CON, spm_read(VEN_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(VEN_PWR_CON, spm_read(VEN_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(VEN_PWR_CON, spm_read(VEN_PWR_CON) & ~PWR_ON_2ND);

		/* TINFO="Wait until PWR_STATUS = 0 and PWR_STATUS_2ND = 0" */
		while ((spm_read(PWR_STATUS) & VEN_PWR_STA_MASK)
		       || (spm_read(PWR_STATUS_2ND) & VEN_PWR_STA_MASK)) {
		}

		/* TINFO="Finish to turn off VEN" */
	} else {    /* STA_POWER_ON */
		/* TINFO="Start to turn on VEN" */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(VEN_PWR_CON, spm_read(VEN_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(VEN_PWR_CON, spm_read(VEN_PWR_CON) | PWR_ON_2ND);

		/* TINFO="Wait until PWR_STATUS = 1 and PWR_STATUS_2ND = 1" */
		while (!(spm_read(PWR_STATUS) & VEN_PWR_STA_MASK)
		       || !(spm_read(PWR_STATUS_2ND) & VEN_PWR_STA_MASK)) {
		}

		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(VEN_PWR_CON, spm_read(VEN_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_ISO = 0" */
		spm_write(VEN_PWR_CON, spm_read(VEN_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(VEN_PWR_CON, spm_read(VEN_PWR_CON) | PWR_RST_B);
		/* TINFO="Set SRAM_PDN = 0" */
		spm_write(VEN_PWR_CON, spm_read(VEN_PWR_CON) & ~(0x1 << 8));
		spm_write(VEN_PWR_CON, spm_read(VEN_PWR_CON) & ~(0x1 << 9));
		spm_write(VEN_PWR_CON, spm_read(VEN_PWR_CON) & ~(0x1 << 10));
		spm_write(VEN_PWR_CON, spm_read(VEN_PWR_CON) & ~(0x1 << 11));
		/* TINFO="Wait until VEN_SRAM_PDN_ACK = 0" */
		while (spm_read(VEN_PWR_CON) & VEN_SRAM_PDN_ACK) {
		}
		/* TINFO="Finish to turn on VEN" */
	}
	return err;
}

int spm_mtcmos_ctrl_isp(int state)
{
 int err = 0;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off ISP" */
		/* TINFO="Set SRAM_PDN = 1" */
		spm_write(ISP_PWR_CON, spm_read(ISP_PWR_CON) | ISP_SRAM_PDN);
		/* TINFO="Wait until ISP_SRAM_PDN_ACK = 1" */
		while (!(spm_read(ISP_PWR_CON) & ISP_SRAM_PDN_ACK)) {
		}
		/* TINFO="Set PWR_ISO = 1" */
		spm_write(ISP_PWR_CON, spm_read(ISP_PWR_CON) | PWR_ISO);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(ISP_PWR_CON, spm_read(ISP_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(ISP_PWR_CON, spm_read(ISP_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(ISP_PWR_CON, spm_read(ISP_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(ISP_PWR_CON, spm_read(ISP_PWR_CON) & ~PWR_ON_2ND);

#ifndef IGNORE_PWR_ACK
		/* TINFO="Wait until PWR_STATUS = 0 and PWR_STATUS_2ND = 0" */
		while ((spm_read(PWR_STATUS) & ISP_PWR_STA_MASK)
		       || (spm_read(PWR_STATUS_2ND) & ISP_PWR_STA_MASK)) {
		}
#endif

		/* TINFO="Finish to turn off ISP" */
	} else {    /* STA_POWER_ON */
		/* TINFO="Start to turn on ISP" */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(ISP_PWR_CON, spm_read(ISP_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(ISP_PWR_CON, spm_read(ISP_PWR_CON) | PWR_ON_2ND);

#ifndef IGNORE_PWR_ACK
		/* TINFO="Wait until PWR_STATUS = 1 and PWR_STATUS_2ND = 1" */
		while (!(spm_read(PWR_STATUS) & ISP_PWR_STA_MASK)
		       || !(spm_read(PWR_STATUS_2ND) & ISP_PWR_STA_MASK)) {
		}
#endif

		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(ISP_PWR_CON, spm_read(ISP_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_ISO = 0" */
		spm_write(ISP_PWR_CON, spm_read(ISP_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(ISP_PWR_CON, spm_read(ISP_PWR_CON) | PWR_RST_B);
		/* TINFO="Set SRAM_PDN = 0" */
		spm_write(ISP_PWR_CON, spm_read(ISP_PWR_CON) & ~(0x1 << 8));
		spm_write(ISP_PWR_CON, spm_read(ISP_PWR_CON) & ~(0x1 << 9));
		/* TINFO="Wait until ISP_SRAM_PDN_ACK = 0" */
		while (spm_read(ISP_PWR_CON) & ISP_SRAM_PDN_ACK) {
		}
		/* TINFO="Finish to turn on ISP" */
	}
	return err;
}


int spm_mtcmos_ctrl_disp(int state)
{
  int err = 0;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off DIS" */
		/* TINFO="Set bus protect" */
		spm_write(INFRA_TOPAXI_PROTECTEN, spm_read(INFRA_TOPAXI_PROTECTEN) | DIS_PROT_MASK);
		while ((spm_read(INFRA_TOPAXI_PROTECTSTA1) & DIS_PROT_MASK) != DIS_PROT_MASK) {
		}
		/* TINFO="Set SRAM_PDN = 1" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) | DIS_SRAM_PDN);
		/* TINFO="Wait until DIS_SRAM_PDN_ACK = 1" */
		while (!(spm_read(DIS_PWR_CON) & DIS_SRAM_PDN_ACK)) {
		}
		/* TINFO="Set PWR_ISO = 1" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) | PWR_ISO);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) & ~PWR_ON_2ND);

#ifndef IGNORE_PWR_ACK
		/* TINFO="Wait until PWR_STATUS = 0 and PWR_STATUS_2ND = 0" */
		while ((spm_read(PWR_STATUS) & DIS_PWR_STA_MASK)
		       || (spm_read(PWR_STATUS_2ND) & DIS_PWR_STA_MASK)) {
		}
#endif

		/* TINFO="Finish to turn off DIS" */
	} else {    /* STA_POWER_ON */
		/* TINFO="Start to turn on DIS" */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) | PWR_ON_2ND);

#ifndef IGNORE_PWR_ACK
		/* TINFO="Wait until PWR_STATUS = 1 and PWR_STATUS_2ND = 1" */
		while (!(spm_read(PWR_STATUS) & DIS_PWR_STA_MASK)
		       || !(spm_read(PWR_STATUS_2ND) & DIS_PWR_STA_MASK)) {
		}
#endif

		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_ISO = 0" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) | PWR_RST_B);
		/* TINFO="Set SRAM_PDN = 0" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) & ~(0x1 << 8));
		/* TINFO="Wait until DIS_SRAM_PDN_ACK = 0" */
		while (spm_read(DIS_PWR_CON) & DIS_SRAM_PDN_ACK) {
		}
		/* TINFO="Release bus protect" */
		spm_write(INFRA_TOPAXI_PROTECTEN, spm_read(INFRA_TOPAXI_PROTECTEN) & ~DIS_PROT_MASK);
		while (spm_read(INFRA_TOPAXI_PROTECTSTA1) & DIS_PROT_MASK) {
		}
		/* TINFO="Finish to turn on DIS" */
	}
	return err;
}


int spm_mtcmos_ctrl_mfg(int state)
{
    int err = 0;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off MFG" */
		/* TINFO="Set bus protect" */
		spm_write(INFRA_TOPAXI_PROTECTEN, spm_read(INFRA_TOPAXI_PROTECTEN) | MFG_PROT_MASK);
		while ((spm_read(INFRA_TOPAXI_PROTECTSTA1) & MFG_PROT_MASK) != MFG_PROT_MASK) {
		}
		/* TINFO="Set SRAM_PDN = 1" */
		spm_write(MFG_PWR_CON, spm_read(MFG_PWR_CON) | MFG_SRAM_PDN);
		/* TINFO="Wait until MFG_SRAM_PDN_ACK = 1" */
		while (!(spm_read(MFG_PWR_CON) & MFG_SRAM_PDN_ACK)) {
		}
		/* TINFO="Set PWR_ISO = 1" */
		spm_write(MFG_PWR_CON, spm_read(MFG_PWR_CON) | PWR_ISO);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(MFG_PWR_CON, spm_read(MFG_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(MFG_PWR_CON, spm_read(MFG_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(MFG_PWR_CON, spm_read(MFG_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(MFG_PWR_CON, spm_read(MFG_PWR_CON) & ~PWR_ON_2ND);

#ifndef IGNORE_PWR_ACK
		/* TINFO="Wait until PWR_STATUS = 0 and PWR_STATUS_2ND = 0" */
		while ((spm_read(PWR_STATUS) & MFG_PWR_STA_MASK)
		       || (spm_read(PWR_STATUS_2ND) & MFG_PWR_STA_MASK)) {
		}
#endif

		/* TINFO="Finish to turn off MFG" */
	} else {    /* STA_POWER_ON */
		/* TINFO="Start to turn on MFG" */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(MFG_PWR_CON, spm_read(MFG_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(MFG_PWR_CON, spm_read(MFG_PWR_CON) | PWR_ON_2ND);

#ifndef IGNORE_PWR_ACK
		/* TINFO="Wait until PWR_STATUS = 1 and PWR_STATUS_2ND = 1" */
		while (!(spm_read(PWR_STATUS) & MFG_PWR_STA_MASK)
		       || !(spm_read(PWR_STATUS_2ND) & MFG_PWR_STA_MASK)) {
		}
#endif

		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(MFG_PWR_CON, spm_read(MFG_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_ISO = 0" */
		spm_write(MFG_PWR_CON, spm_read(MFG_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(MFG_PWR_CON, spm_read(MFG_PWR_CON) | PWR_RST_B);
		/* TINFO="Set SRAM_PDN = 0" */
		spm_write(MFG_PWR_CON, spm_read(MFG_PWR_CON) & ~(0x1 << 8));
		/* TINFO="Wait until MFG_SRAM_PDN_ACK = 0" */
		while (spm_read(MFG_PWR_CON) & MFG_SRAM_PDN_ACK) {
		}
		/* TINFO="Release bus protect" */
		spm_write(INFRA_TOPAXI_PROTECTEN, spm_read(INFRA_TOPAXI_PROTECTEN) & ~MFG_PROT_MASK);
		while (spm_read(INFRA_TOPAXI_PROTECTSTA1) & MFG_PROT_MASK) {
		}
		/* TINFO="Finish to turn on MFG" */
	}
	return err;
}

int spm_mtcmos_ctrl_mfg_ASYNC(int state)
{
   	int err = 0;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off MFG_ASYNC" */
		/* TINFO="Set PWR_ISO = 1" */
		spm_write(MFG_ASYNC_PWR_CON, spm_read(MFG_ASYNC_PWR_CON) | PWR_ISO);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(MFG_ASYNC_PWR_CON, spm_read(MFG_ASYNC_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(MFG_ASYNC_PWR_CON, spm_read(MFG_ASYNC_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(MFG_ASYNC_PWR_CON, spm_read(MFG_ASYNC_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(MFG_ASYNC_PWR_CON, spm_read(MFG_ASYNC_PWR_CON) & ~PWR_ON_2ND);

#ifndef IGNORE_PWR_ACK
		/* TINFO="Wait until PWR_STATUS = 0 and PWR_STATUS_2ND = 0" */
		while ((spm_read(PWR_STATUS) & MFG_ASYNC_PWR_STA_MASK)
		       || (spm_read(PWR_STATUS_2ND) & MFG_ASYNC_PWR_STA_MASK)) {
		}
#endif

		/* TINFO="Finish to turn off MFG_ASYNC" */
	} else {    /* STA_POWER_ON */
		/* TINFO="Start to turn on MFG_ASYNC" */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(MFG_ASYNC_PWR_CON, spm_read(MFG_ASYNC_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(MFG_ASYNC_PWR_CON, spm_read(MFG_ASYNC_PWR_CON) | PWR_ON_2ND);

#ifndef IGNORE_PWR_ACK
		/* TINFO="Wait until PWR_STATUS = 1 and PWR_STATUS_2ND = 1" */
		while (!(spm_read(PWR_STATUS) & MFG_ASYNC_PWR_STA_MASK)
		       || !(spm_read(PWR_STATUS_2ND) & MFG_ASYNC_PWR_STA_MASK)) {
		}
#endif

		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(MFG_ASYNC_PWR_CON, spm_read(MFG_ASYNC_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_ISO = 0" */
		spm_write(MFG_ASYNC_PWR_CON, spm_read(MFG_ASYNC_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(MFG_ASYNC_PWR_CON, spm_read(MFG_ASYNC_PWR_CON) | PWR_RST_B);
		/* TINFO="Finish to turn on MFG_ASYNC" */
	}
	return err;
}


int spm_mtcmos_ctrl_connsys(int state)
{
   int err = 0;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off CONN" */
		/* TINFO="Set bus protect" */
		spm_write(INFRA_TOPAXI_PROTECTEN, spm_read(INFRA_TOPAXI_PROTECTEN) | CONN_PROT_MASK);
		while ((spm_read(INFRA_TOPAXI_PROTECTSTA1) & CONN_PROT_MASK) != CONN_PROT_MASK) {
		}
		/* TINFO="Set PWR_ISO = 1" */
		spm_write(CONN_PWR_CON, spm_read(CONN_PWR_CON) | PWR_ISO);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(CONN_PWR_CON, spm_read(CONN_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(CONN_PWR_CON, spm_read(CONN_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(CONN_PWR_CON, spm_read(CONN_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(CONN_PWR_CON, spm_read(CONN_PWR_CON) & ~PWR_ON_2ND);

#ifndef IGNORE_PWR_ACK
		/* TINFO="Wait until PWR_STATUS = 0 and PWR_STATUS_2ND = 0" */
		while ((spm_read(PWR_STATUS) & CONN_PWR_STA_MASK)
		       || (spm_read(PWR_STATUS_2ND) & CONN_PWR_STA_MASK)) {
		}
#endif

		/* TINFO="Finish to turn off CONN" */
	} else {    /* STA_POWER_ON */
		/* TINFO="Start to turn on CONN" */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(CONN_PWR_CON, spm_read(CONN_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(CONN_PWR_CON, spm_read(CONN_PWR_CON) | PWR_ON_2ND);

#ifndef IGNORE_PWR_ACK
		/* TINFO="Wait until PWR_STATUS = 1 and PWR_STATUS_2ND = 1" */
		while (!(spm_read(PWR_STATUS) & CONN_PWR_STA_MASK)
		       || !(spm_read(PWR_STATUS_2ND) & CONN_PWR_STA_MASK)) {
		}
#endif

		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(CONN_PWR_CON, spm_read(CONN_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_ISO = 0" */
		spm_write(CONN_PWR_CON, spm_read(CONN_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(CONN_PWR_CON, spm_read(CONN_PWR_CON) | PWR_RST_B);
		/* TINFO="Release bus protect" */
		spm_write(INFRA_TOPAXI_PROTECTEN, spm_read(INFRA_TOPAXI_PROTECTEN) & ~CONN_PROT_MASK);
		while (spm_read(INFRA_TOPAXI_PROTECTSTA1) & CONN_PROT_MASK) {
		}
		/* TINFO="Finish to turn on CONN" */
	}
	return err;
}


int spm_mtcmos_ctrl_aud(int state)
{
  int err = 0;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off AUDIO" */
		/* TINFO="Set SRAM_PDN = 1" */
		spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) | AUDIO_SRAM_PDN);
		/* TINFO="Wait until AUDIO_SRAM_PDN_ACK = 1" */
		while (!(spm_read(AUDIO_PWR_CON) & AUDIO_SRAM_PDN_ACK)) {
		}
		/* TINFO="Set PWR_ISO = 1" */
		spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) | PWR_ISO);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) & ~PWR_ON_2ND);

#ifndef IGNORE_PWR_ACK
		/* TINFO="Wait until PWR_STATUS = 0 and PWR_STATUS_2ND = 0" */
		while ((spm_read(PWR_STATUS) & AUDIO_PWR_STA_MASK)
		       || (spm_read(PWR_STATUS_2ND) & AUDIO_PWR_STA_MASK)) {
		}
#endif

		/* TINFO="Finish to turn off AUDIO" */
	} else {    /* STA_POWER_ON */
		/* TINFO="Start to turn on AUDIO" */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) | PWR_ON_2ND);

#ifndef IGNORE_PWR_ACK
		/* TINFO="Wait until PWR_STATUS = 1 and PWR_STATUS_2ND = 1" */
		while (!(spm_read(PWR_STATUS) & AUDIO_PWR_STA_MASK)
		       || !(spm_read(PWR_STATUS_2ND) & AUDIO_PWR_STA_MASK)) {
		}
#endif

		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_ISO = 0" */
		spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) | PWR_RST_B);
		/* TINFO="Set SRAM_PDN = 0" */
		spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) & ~(0x1 << 8));
		spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) & ~(0x1 << 9));
		spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) & ~(0x1 << 10));
		spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) & ~(0x1 << 11));
		/* TINFO="Wait until AUDIO_SRAM_PDN_ACK = 0" */
		while (spm_read(AUDIO_PWR_CON) & AUDIO_SRAM_PDN_ACK) {
		}
		/* TINFO="Finish to turn on AUDIO" */
	}
	return err;
}

int spm_topaxi_prot(int bit, int en)/* Unknown */
{
    unsigned long flags;
    //XXX: only in kernel
    //spm_mtcmos_noncpu_lock(flags);

    if (en == 1) {
        spm_write(TOPAXI_PROT_EN, spm_read(TOPAXI_PROT_EN) | (1<<bit));
        while ((spm_read(TOPAXI_PROT_STA1) & (1<<bit)) != (1<<bit)) {
        }
    } else {
   	    spm_write(TOPAXI_PROT_EN, spm_read(TOPAXI_PROT_EN) & ~(1<<bit));
        while (spm_read(TOPAXI_PROT_STA1) & (1<<bit)) {
        }
    }

    //XXX: only in kernel
    //spm_mtcmos_noncpu_unlock(flags);

    return 0;
}
