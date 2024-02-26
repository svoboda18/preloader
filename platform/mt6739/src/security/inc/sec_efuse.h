#ifndef SEC_EFUSE_H
#define SEC_EFUSE_H

#define EFUSE_BLOW_KEY1			0x90b1e2f6
#define EFUSE_BLOW_KEY2			0x73d5a8c9

struct efuse_param {
	U32 magic_key1;
	U32 magic_key2;
	U32 reserved;
	bool  clear_part_dis;
	bool  enable_self_blow;
	bool  reserved2;
	bool  reserved3;
};

typedef struct efuse_part_info {
	U64 part_base;
	U32 part_id;
	blkdev_t *bootdev;
	bool  initialized;
	bool  reserved1;
	bool  reserved2;
	bool  reserved3;
} EFUSE_PART_INFO;

/**************************************************************************
 * EXPORTED FUNCTIONS
 **************************************************************************/
/* Partition & Storage */
extern int efuse_storage_init(void);
extern int efuse_storage_read(U64 efuse_part_start_offset, U32 size, U8 *dst);
extern int efuse_storage_write(U64 efuse_part_start_offset, U32 size, U8 *src);

/* WDT */
extern void efuse_wdt_init(void);
extern void efuse_wdt_restart(void);
extern void efuse_wdt_sw_reset(void);

/* DDR reserved mode */
extern int efuse_dram_reserved(int enable);

/* PLL */
extern void efuse_pll_set(void);

/* Vbat */
extern int efuse_check_lowbat(void);

/* Fsource */
extern U32 efuse_fsource_set(void);
extern U32 efuse_fsource_is_enabled(void);
extern U32 efuse_fsource_close(void);

/* Vcore */
extern U32 efuse_vcore_blow(void);
extern U32 efuse_vcore_high(void);
extern U32 efuse_vcore_low(void);

/* Others */
extern int efuse_module_reinit(void);

#endif /* SEC_EFUSE_H */

