#ifndef SSPM_H
#define SSPM_H

#define SSPM_DBG_MODE 0

#if SSPM_DBG_MODE
#define sspm_dbg(f, ...)    pal_log_info("[SSPM] " f, ##__VA_ARGS__)
#else
#define sspm_dbg(f, ...)    do{}while(0)
#endif

#define SSPM_MEM_SIZE       0x00100000  //  1M (Should be large than SSPM_IMG_SZ + SSPM_TCM_SZ + SSPM_CPM_SZ )
#define SSPM_MEM_ALIGN      0x00010000  // 64K (minimal size for EMI MPU)
#define SSPM_MEM_LIMIT      0x80000000
#define SSPM_IMG_SZ         0x000C0000  // 768K
#define SSPM_TCM_SZ         0x00028000  // 160K
#define SSPM_CPM_SZ         0x00000000  //   0K

#define SSPM_DDR_SIZE       0x00002000  //   8K
#define SSPM_DDR_ALIGN      0x00040000  // 256K

#define PT_ID_SSPM_DM     0
#define PT_ID_SSPM_PM     1
#define PT_ID_SSPM_CDM    2  // un-used
#define PT_ID_SSPM_CPM    3
#define PT_ID_SSPM_XFILE 99

#define SSPM_SW_RSTN        0x10440000
#define SSPM_CFGREG_GPR0    SSPM_SW_RSTN + 0x20
#define SSPM_CFGREG_GPR1    SSPM_SW_RSTN + 0x24
#define SSPM_DDR_RMAP0      SSPM_SW_RSTN + 0x94

#define ROUNDUP(a,b)    (((a) + ((b)-1)) & ~((b)-1))

struct sspm_info_t {
    unsigned int sspm_dm_ofs; /* for sspm data memory */
    unsigned int sspm_dm_sz;
    unsigned int rd_ofs;      /* for sspm ramdump */
    unsigned int rd_sz;
    unsigned int xfile_addr;  /* for LKdump */
    unsigned int xfile_sz;
};

#endif /* SSPM_H */
