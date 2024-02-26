#ifndef _SPM_MTCMOS_
#define _SPM_MTCMOS_

/* 
//XXX: only in kernel
#include <linux/kernel.h>
*/

#define STA_POWER_DOWN  0
#define STA_POWER_ON    1


/*
 * 1. for non-CPU MTCMOS: VDEC, VENC, ISP, DISP, MFG, INFRA, DDRPHY, MDSYS1, MDSYS2
 * 2. call spm_mtcmos_noncpu_lock/unlock() before/after any operations
 */
extern int spm_mtcmos_ctrl_mm0(int state);


#endif
