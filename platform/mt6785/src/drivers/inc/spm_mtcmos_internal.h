#ifndef _SPM_MTCMOS_INTERNAL_
#define _SPM_MTCMOS_INTERNAL_


#define SPM_PROJECT_CODE        0xb16

#define spm_read(addr)                  __raw_readl(addr)
#define spm_write(addr, val)            mt_reg_sync_writel(val, addr)

#endif //#ifndef _SPM_MTCMOS_INTERNAL_