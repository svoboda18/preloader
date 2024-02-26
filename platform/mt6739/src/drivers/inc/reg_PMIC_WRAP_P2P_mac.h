#ifndef __PMIC_WRAP_P2P_REGS_HMAC__
#define __PMIC_WRAP_P2P_REGS_HMAC__

#define GET_WACS_P2P_EN(x)           ((x>>0)  & 0x00000001)
#define GET_INIT_DONE_P2P(x)         ((x>>0)  & 0x00000001)
#define GET_WACS_P2P_WDATA(x)        ((x>>0)  & 0x0000ffff)
#define GET_WACS_P2P_ADR(x)          ((x>>16) & 0x00007fff)
#define GET_WACS_P2P_WRITE(x)        ((x>>31) & 0x00000001)
#define GET_WACS_P2P_RDATA(x)        ((x>>0)  & 0x0000ffff)
#define GET_WACS_P2P_FSM(x)          ((x>>16) & 0x00000007)
#define GET_WACS_P2P_REQ(x)          ((x>>19) & 0x00000001)
#define GET_WACS_SYNC_IDLE_P2P(x)    ((x>>20) & 0x00000001)
#define GET_WACS_INIT_DONE_P2P(x)    ((x>>21) & 0x00000001)
#define GET_SYS_IDLE_P2P(x)          ((x>>22) & 0x00000001)
#define GET_WACS_P2P_FIFO_FILLCNT(x) ((x>>24) & 0x0000000f)
#define GET_WACS_P2P_FIFO_FREECNT(x) ((x>>28) & 0x0000000f)
#define GET_WACS_P2P_VLDCLR(x)       ((x>>0)  & 0x00000001)

#endif //__PMIC_WRAP_P2P_REGS_HMAC__

