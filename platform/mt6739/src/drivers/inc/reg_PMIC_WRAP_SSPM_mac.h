#ifndef __PMIC_WRAP_SSPM_REGS_HMAC__
#define __PMIC_WRAP_SSPM_REGS_HMAC__

#define GET_WACS_SSPM_EN(x)          ((x>>0)  & 0x00000001)
#define GET_INIT_DONE_SSPM(x)        ((x>>0)  & 0x00000001)
#define GET_WACS_SSPM_WDATA(x)       ((x>>0)  & 0x0000ffff)
#define GET_WACS_SSPM_ADR(x)         ((x>>16) & 0x00007fff)
#define GET_WACS_SSPM_WRITE(x)       ((x>>31) & 0x00000001)
#define GET_WACS_SSPM_RDATA(x)       ((x>>0)  & 0x0000ffff)
#define GET_WACS_SSPM_FSM(x)         ((x>>16) & 0x00000007)
#define GET_WACS_SSPM_REQ(x)         ((x>>19) & 0x00000001)
#define GET_WACS_SYNC_IDLE_SSPM(x)   ((x>>20) & 0x00000001)
#define GET_WACS_INIT_DONE_SSPM(x)   ((x>>21) & 0x00000001)
#define GET_SYS_IDLE_SSPM(x)         ((x>>22) & 0x00000001)
#define GET_WACS_SSPM_FIFO_FILLCNT(x) ((x>>24) & 0x0000000f)
#define GET_WACS_SSPM_FIFO_FREECNT(x) ((x>>28) & 0x0000000f)
#define GET_WACS_SSPM_VLDCLR(x)      ((x>>0)  & 0x00000001)
#define GET_PWRAP_PWRMCU_RDATA(x)    ((x>>0)  & 0x0000ffff)

#endif //__PMIC_WRAP_SSPM_REGS_HMAC__

