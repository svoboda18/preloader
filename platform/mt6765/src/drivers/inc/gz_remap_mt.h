#ifndef __GZ_REMAP_MT_H__
#define __GZ_REMAP_MT_H__

#include <mt6765.h>

/* HW registers */
#define INFRA_AO_SEC_HYP                (uintptr_t)(INFRACFG_AO_BASE + 0xFB0)
#define RG_DDR_HYP_REMAP_EN             ((0x1) << 8)
#define RG_IO_HYP_REMAP_EN              ((0x1) << 9)
#define RG_DDR_HYP_ADDR_MASK_OFFSET     (0)
#define RG_DDR_HYP_ADDR_MASK_MASK       (0x7 << RG_DDR_HYP_ADDR_MASK_OFFSET)
#define RG_DDR_HYP_ADDR_MATCH_OFFSET    (4)
#define RG_DDR_HYP_ADDR_MATCH_MASK      (0x7 << RG_DDR_HYP_ADDR_MATCH_OFFSET)

/* HW config (EMI 8G settings) */
#define GZ_REMAP_DDR_OFFSET 0x200000000ULL
#define GZ_REMAP_IO_OFFSET  0x20000000ULL
#define GZ_REMAP_DDR_HYP_ADDR_MASK   0b100
#define GZ_REMAP_DDR_HYP_ADDR_MATCH  0b100

/* VM used/max numbfer */
#define GZ_REMAP_VM_NUM     2
#define GZ_REMAP_VM_MAX     2

/* VM domain mapping */
#define GZ_REMAP_VMDOMAIN_AP 0
#define GZ_REMAP_VMDOMAIN_GZ 11

#endif /* end of __GZ_REMAP_MT_H__ */
