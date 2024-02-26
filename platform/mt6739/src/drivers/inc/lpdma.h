#ifndef __LPDMA_H__
#define __LPDMA_H__

#include "mt6739.h"

#define DCS_ENABLE          (1)

#define LPDMA_CONA          (EMI_DCS_BASE+0x0)		/* Tigger */
#define LPDMA_CONB          (EMI_DCS_BASE+0x8)		/* Mode */
#define LPDMA_CONC          (EMI_DCS_BASE+0x10)		/* Cascad region */
#define LPDMA_COND          (EMI_DCS_BASE+0x18)		/* Interleave region */
#define LPDMA_CONE          (EMI_DCS_BASE+0x20)		/* All channels region */
#define LPDMA_CONF          (EMI_DCS_BASE+0x28)		/* DMA region */
#define LPDMA_CONG          (EMI_DCS_BASE+0x30)		/* DMA destination address */
#define LPDMA_CONH          (EMI_DCS_BASE+0x38)		/* Channel one start address for swap mode */
#define LPDMA_CONI          (EMI_DCS_BASE+0x40)		/* Channel two start address for swap mode */
#define LPDMA_CONJ          (EMI_DCS_BASE+0x48)		/* Channel three start address for swap mode */
#define LPDMA_CONK          (EMI_DCS_BASE+0x50)		/* Address range for issue ultra DMA command */
#define LPDMA_CONL          (EMI_DCS_BASE+0x58)		/* Address range for issue preultra DMA command */
#define LPDMA_CONM          (EMI_DCS_BASE+0x60)		/* DMA migration status interrupt */
#define LPDMA_CONN          (EMI_DCS_BASE+0x68)		/* Memory protection region one in copy mode */
#define LPDMA_CONO          (EMI_DCS_BASE+0x70)		/* Memory protection region two in copy mode */
#define LPDMA_CONP          (EMI_DCS_BASE+0x78)		/* MMU coherence check */
#define LPDMA_CONQ          (EMI_DCS_BASE+0x80)		/* MISC */
#define LPDMA_CONR          (EMI_DCS_BASE+0x88)		/* Dummy value */

/* memory remap registers */
#define LPDMA_MMU1          (EMI_DCS_BASE+0x400)
#define LPDMA_MMU2          (EMI_DCS_BASE+0x408)
#define LPDMA_MMU3          (EMI_DCS_BASE+0x410)
#define LPDMA_MMU4          (EMI_DCS_BASE+0x418)
#define LPDMA_MMU5          (EMI_DCS_BASE+0x420)
#define LPDMA_MMU6          (EMI_DCS_BASE+0x428)
#define LPDMA_MMU7          (EMI_DCS_BASE+0x430)
#define LPDMA_MMU8          (EMI_DCS_BASE+0x438)
#define LPDMA_MMU9          (EMI_DCS_BASE+0x440)
#define LPDMA_MMU10         (EMI_DCS_BASE+0x448)
#define LPDMA_MMU11         (EMI_DCS_BASE+0x450)
#define LPDMA_MMU12         (EMI_DCS_BASE+0x458)
#define LPDMA_MMU13         (EMI_DCS_BASE+0x460)
#define LPDMA_MMU14         (EMI_DCS_BASE+0x468)
#define LPDMA_MMU15         (EMI_DCS_BASE+0x470)
#define LPDMA_MMU16         (EMI_DCS_BASE+0x478)
#define LPDMA_MMU17         (EMI_DCS_BASE+0x480)
#define LPDMA_MMU18         (EMI_DCS_BASE+0x488)
#define LPDMA_MMU19         (EMI_DCS_BASE+0x490)
#define LPDMA_MMU20         (EMI_DCS_BASE+0x498)
#define LPDMA_MMU21         (EMI_DCS_BASE+0x4A0)
#define LPDMA_MMU22         (EMI_DCS_BASE+0x4A8)
#define LPDMA_MMU23         (EMI_DCS_BASE+0x4B0)
#define LPDMA_MMU24         (EMI_DCS_BASE+0x4B8)
#define LPDMA_MMU25         (EMI_DCS_BASE+0x4C0)
#define LPDMA_MMU26         (EMI_DCS_BASE+0x4C8)
#define LPDMA_MMU27         (EMI_DCS_BASE+0x4D0)
#define LPDMA_MMU28         (EMI_DCS_BASE+0x4D8)
#define LPDMA_MMU29         (EMI_DCS_BASE+0x4E0)
#define LPDMA_MMU30         (EMI_DCS_BASE+0x4E8)
#define LPDMA_MMU31         (EMI_DCS_BASE+0x4F0)
#define LPDMA_MMU32         (EMI_DCS_BASE+0x4F8)

/* status */
#define LPDMA_INTLV         (EMI_DCS_BASE+0x808)
#define LPDMA_CASD          (EMI_DCS_BASE+0x810)
#define LPDMA_MASK          (EMI_DCS_BASE+0x818)
#define LPDMA_STA           (EMI_DCS_BASE+0x820)
#define LPDMA_DBG1ST        (EMI_DCS_BASE+0x830)
#define LPDMA_DBG2ND        (EMI_DCS_BASE+0x838)
#define LPDMA_DBG3RD        (EMI_DCS_BASE+0x840)
#define LPDMA_DBG4TH        (EMI_DCS_BASE+0x848)
#define LPDMA_DBG5TH        (EMI_DCS_BASE+0x850)
#define LPDMA_DBG6TH        (EMI_DCS_BASE+0x858)
#define LPDMA_DBG7TH        (EMI_DCS_BASE+0x860)
#define LPDMA_DBG8TH        (EMI_DCS_BASE+0x868)
#define LPDMA_DBG9TH        (EMI_DCS_BASE+0x870)
#define LPDMA_DBG10TH       (EMI_DCS_BASE+0x878)
#define LPDMA_DBG11TH       (EMI_DCS_BASE+0x880)
#define LPDMA_DBG12TH       (EMI_DCS_BASE+0x888)
#define LPDMA_DBG13TH       (EMI_DCS_BASE+0x890)
#define LPDMA_DBG14TH       (EMI_DCS_BASE+0x898)
#define LPDMA_DBG15TH       (EMI_DCS_BASE+0x8A0)
#define LPDMA_DBG16TH       (EMI_DCS_BASE+0x8A8)
#define LPDMA_DBG17TH       (EMI_DCS_BASE+0x8B0)
#define LPDMA_DBG18TH       (EMI_DCS_BASE+0x8B8)
#define LPDMA_DBG19TH       (EMI_DCS_BASE+0x8C0)
#define LPDMA_DBG20TH       (EMI_DCS_BASE+0x8C8)

#define INFRA_SLEEP_PRO          (0x10000250)


typedef struct {
	unsigned int channel_num;
	unsigned long long dram_size;
	unsigned long long dram_start;
	unsigned long long interleave_region;
	unsigned long long all_channel_region;
} lpdma_dram_spec_t;

enum migrate_dir {
	NORMAL,
	LOWPWR,
};

/*
PAGE_32M	MMU Page size
1: For DRAM size is less than 4GB
0: For DRAM size is large than 4GB
*/
//#define PAGE_32M  1


#define DGB_CLK_EN 1

#define SWAP_MODE 1
#define COPY_MODE 0

enum migration_mode {
	MODE_CH_4to1=0,
	MODE_CH_4to2,
	MODE_CH_2to1,
};

enum BW_LIMIT {
	BW_LIMIT_0=0,
	BW_LIMIT_1_64,
	BW_LIMIT_2_64,
	BW_LIMIT_3_64
};

/*
Force access low address as high address power down and master access high
0: Normal map access addess
1: Force address map to low
*/
#define FORCE_ACC_LOW  1
#define BW_LIMIT 0x3f
#define BW_FILTER_LEN 0x3
#define STOP_EN  1
#define STOP_DIS 0
#define INT_EN   1
#define INT_DIS  0

enum initial_status {
	CASCADE_NORMAL_INTERLEAVE_NORMAL,
	CASCADE_LOWPWR_INTERLEAVE_NORMAL,
	CASCADE_NORMAL_INTERLEAVE_LOWPWR,
	CASCADE_LOWPWR_INTERLEAVE_LOWPWR,
};

/* function prototypes */
extern int lpdma_init(lpdma_dram_spec_t *spec);
extern void lpdma_set_initial_status(enum initial_status status);
extern void lpdma_reload_initial_status(void);
extern void lpdma_set_security_dummy_write_pattern(unsigned int pattern);
extern void lpdma_dump_debug_info(void);

#endif	// __LPDMA_H__
