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
int spm_mtcmos_ctrl_md1(int state);
int spm_mtcmos_ctrl_conn(int state);
int spm_mtcmos_ctrl_dpy(int state);
int spm_mtcmos_ctrl_dis(int state);
int spm_mtcmos_ctrl_mfg(int state);
int spm_mtcmos_ctrl_isp(int state);
int spm_mtcmos_ctrl_ifr(int state);
int spm_mtcmos_ctrl_vde(int state);
int spm_mtcmos_ctrl_ven(int state);
int spm_mtcmos_ctrl_mfg_async(int state);
int spm_mtcmos_ctrl_audio(int state);
int spm_mtcmos_ctrl_cam(int state);
int spm_mtcmos_ctrl_c2k(int state);
int spm_mtcmos_ctrl_mdsys_intf_infra(int state);
int spm_mtcmos_ctrl_mfg_core1(int state);
int spm_mtcmos_ctrl_mfg_core0(int state);


extern int spm_topaxi_prot(int bit, int en);

#endif
