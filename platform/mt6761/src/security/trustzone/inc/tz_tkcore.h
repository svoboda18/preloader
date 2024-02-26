#ifndef TZ_TKCORE_H
#define TZ_TKCORE_H

#include "typedefs.h"

#define TKCORE_BOOTCFG_MAGIC    (0x54534958)

#define TEE_ARGUMENT_VERSION_LEGACY (0x00010000U)
#define TEE_ARGUMENT_VERSION_V1_0   (0x00010001U)
#define TEE_ARGUMENT_VERSION_V1_1   (0x00010002U)
#define TEE_ARGUMENT_VERSION_V1_2   (0x00010003U)
#define TEE_ARGUMENT_VERSION_V2_0   (0x00020001U)

#define TEE_ARGUMENT_VERSION        TEE_ARGUMENT_VERSION_V2_0

#define TKCORE_SHM_SIZE_LIMIT   (0x200000U)

#define RPMB_KEY_SIZE    32

typedef struct {
    u32 magic;        // magic value from information
    u32 length;       // size of struct in bytes.
    u64 version;      // Version of structure
    u64 dRamBase;     // NonSecure DRAM start address
    u64 dRamSize;     // NonSecure DRAM size
    u64 secDRamBase;  // Secure DRAM start address
    u64 secDRamSize;  // Secure DRAM size
    u64 secIRamBase;  // Secure IRAM base
    u64 secIRamSize;  // Secure IRam size
    u64 gic_distributor_base;
    u64 gic_cpuinterface_base;
    u32 gic_version;
    u32    uart_base;
    u32 total_number_spi;
    u32 ssiq_number;
    u64 flags;
    u8  rpmb_key[RPMB_KEY_SIZE];

    /* for TEE_ARGUMENT_VERSION_1 */
    u8 rpmb_key_programmed;

    /* for TEE_ARGUMENT_VERSION_2 (mt6580 specific) */
    u32 nw_bootargs;
    u32 nw_bootargs_size;

    /* for TEE_ARGUMENT_VERSION_3 */
    u32 sdrpmb_partaddr;
    u32 sdrpmb_partsize;
    u32 sdrpmb_starting_sector;
    u32 sdrpmb_nr_sectors;

    u32 rtc_time;
    u8 soc_id_programmed;
    u8 __reserved[2];

    /* for TEE_ARGUMENT_VERSION_4 */
    u8 soc_id[SOC_ID_LEN];
} __attribute__((packed)) tee_arg_t, *tee_arg_t_ptr;

void tkcore_boot_param_prepare(u32 param_addr, u32 tee_entry,
    u64 sec_dram_size, u64 dram_base, u64 dram_size, int meta_uart);

void tkcore_boot_param_prepare_rpmbkey(u32 param_addr,
    u8 *rpmb_key, u32 rpmb_keylen);

void tkcore_boot_param_prepare_nwbootargs(u32 param_addr,
    u32 addr, u32 size);

void tkcore_boot_param_prepare_soc_id(
    u32 param_addr);

void tkcore_boot_param_prepare_rtctime(u32 param_addr);

void tkcore_dump_param(u32 param_addr);

#endif
