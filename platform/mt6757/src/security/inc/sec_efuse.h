#ifndef SEC_EFUSE_H
#define SEC_EFUSE_H

#define EFUSE_BLOW_KEY1			0x90b1e2f6
#define EFUSE_BLOW_KEY2			0x73d5a8c9

struct efuse_param {
	U32 gfh_addr;
	U32 magic_key1;
	U32 magic_key2;
	bool  clear_part_dis;
	bool  reserved1;
	bool  reserved2;
	bool  reserved3;
};
/**************************************************************************
 * EXPORTED FUNCTIONS
 **************************************************************************/
/* partition */
extern int efuse_part_get_base(unsigned long *base);

/* Storage */
extern int efuse_storage_init(void);
extern int efuse_storage_read(unsigned long blknr, U32 blkcnt, unsigned long *dst);
extern int efuse_storage_write(unsigned long blknr, U32 blkcnt, unsigned long *src);

/* WDT */
extern void efuse_wdt_init(void);
extern void efuse_wdt_disable(void);
extern void efuse_wdt_sw_reset(void);

/* DDR reserved mode */
extern int efuse_dram_reserved(int enable);

/* PLL */
extern void efuse_pll_set(void);

/* Vbat */
extern int efuse_check_lowbat(void);

/* Fsource */
extern U32 efuse_fsource_set(void);
extern U32 efuse_fsource_close(void);
extern U32 efuse_fsource_is_enabled(void);

/* Vcore */
extern U32 efuse_vcore_blow(void);
extern U32 efuse_vcore_high(void);
extern U32 efuse_vcore_low(void);

/* Others */
extern int efuse_module_reinit(void);

#endif /* SEC_EFUSE_H */

