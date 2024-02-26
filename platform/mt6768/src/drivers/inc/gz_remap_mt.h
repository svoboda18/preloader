#ifndef __GZ_REMAP_MT_H__
#define __GZ_REMAP_MT_H__

#include <mt6768.h>

/* HW registers */
#define INFRA_AO_SEC_HYP            (uintptr_t)(INFRACFG_AO_BASE + 0xFB0)
#define RG_VMID0_SECURE_EN          (0x1 << 0)
#define RG_VMID1_SECURE_EN          (0x1 << 1)
#define RG_VMID2_SECURE_EN          (0x1 << 2)
#define RG_VMID3_SECURE_EN          (0x1 << 3)
#define RG_IO_VMID_SEC_REMAP_EN     (0x1 << 4)
#define RG_DDR_HYP_REMAP_EN         (0x1 << 8)
#define RG_IO_HYP_REMAP_EN          (0x1 << 9)
#define RG_ACP_HYP_REMAP_EN         (0x1 << 10)

#define MM_IOMMU_DOMAIN_NUM         (uintptr_t)(SECURITY_AO + 0x614)
#define VPU_IOMMU_DOMAIN_NUM        (uintptr_t)(SECURITY_AO + 0x61C)

/**
 * VM used/max number and per-VM offset
 *
 * The whole VM offset depends on overall addressing bits
 * and the secure IO remap is activated by accessing peripherals
 * through Bank 3 instead of Bank 1 addresses
 */
#define GZ_REMAP_VM_NUM             2
#define GZ_REMAP_VM_MAX             4
#define GZ_REMAP_VM_OFFSET          (0x1ULL << 34)
#define GZ_REMAP_VM_SEC_IO_OFFSET   0x20000000

/* VM domain mapping */
#define GZ_REMAP_VMDOMAIN_AP 0
#define GZ_REMAP_VMDOMAIN_GZ 11
#define GZ_REMAP_VMDOMAIN_MAP_BITS  0x5

#endif /* end of __GZ_REMAP_MT_H__ */
