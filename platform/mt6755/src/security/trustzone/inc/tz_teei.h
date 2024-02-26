/*
 * Copyright (c) 2015-2016 MICROTRUST Incorporated
 * All rights reserved
 *
 * This file and software is confidential and proprietary to MICROTRUST Inc.
 * Unauthorized copying of this file and software is strictly prohibited.
 * You MUST NOT disclose this file and software unless you get a license
 * agreement from MICROTRUST Incorporated.
 */

#ifndef TZ_TEEI_H
#define TZ_TEEI_H

#include <typedefs.h>
#include "tz_platform_teei.h"

#define KEY_LEN 32

//#define CFG_MICROTRUST_FP_SUPPORT

enum device_type {
    MT_UNUSED = 0,
    MT_UART16550 =1,    /* secure uart */
    MT_SEC_GPT,         /* secure gp timer */
    MT_SEC_WDT,         /* secure watch dog */
    MT_SEC_CRYPTO,
    MT_SEC_SPI,       /* secure spi */
};

enum isee_flags {
    /* default value */
    ISEE_FLAGS_DEFAULT_VALUE            = 0x0000000000000000,

    /* bit 0~1 */
    ISEE_FLAGS_LOG_PATH_OFFSET          = 0,
    ISEE_FLAGS_LOG_PATH_BUFFER          = 0b00,
    ISEE_FLAGS_LOG_PATH_UART            = 0b01,
    ISEE_FLAGS_LOG_PATH_LOG_SERVER      = 0b10,
    ISEE_FLAGS_LOG_PATH_RESERVED        = 0b11,
    ISEE_FLAGS_LOG_PATH_MASK            = 0x0000000000000003,

    /* bit 2~4 */
    ISEE_FLAGS_LOG_LEVEL_OFFSET         = 2,
    ISEE_FLAGS_LOG_LEVEL_ALL            = 0b000,
    ISEE_FLAGS_LOG_LEVEL_TRACE          = 0b001,
    ISEE_FLAGS_LOG_LEVEL_DEBUG          = 0b010,
    ISEE_FLAGS_LOG_LEVEL_INFO           = 0b011,
    ISEE_FLAGS_LOG_LEVEL_WARNING        = 0b100,
    ISEE_FLAGS_LOG_LEVEL_ERROR          = 0b101,
    ISEE_FLAGS_LOG_LEVEL_NO_LOG         = 0b110,
    ISEE_FLAGS_LOG_LEVEL_RESERVED2      = 0b111,
    ISEE_FLAGS_LOG_LEVEL_MASK           = 0x000000000000001c,

    /* bit 5 */
    ISEE_FLAGS_SOTER_TYPE_OFFSET        = 5,
    ISEE_FLAGS_SOTER_TYPE_MINIMAL       = 0b0,
    ISEE_FLAGS_SOTER_TYPE_NORMAL        = 0b1,
    ISEE_FLAGS_SOTER_TYPE_MASK          = 0x0000000000000020,

    /* bit 6~7 */
    ISEE_FLAGS_VERIFY_MODE_OFFSET       = 6,
    ISEE_FLAGS_VERIFY_MODE_DISABLE_COUNTER = 0b00,
    ISEE_FLAGS_VERIFY_MODE_ENABLE_COUNTER = 0b01,
    ISEE_FLAGS_VERIFY_MODE_RESERVED0   = 0b10,
    ISEE_FLAGS_VERIFY_MODE_RESERVED1   = 0b11,
    ISEE_FLAGS_VERIFY_MODE_MASK        = 0x00000000000000C0,

    /* bit 8 */
    ISEE_FLAGS_SECURE_STORAGE_OFFSET    = 8,
    ISEE_FLAGS_SECURE_STORAGE_RPMB      = 0b0,
    ISEE_FLAGS_SECURE_STORAGE_FS        = 0b1,
    ISEE_FLAGS_SECURE_STORAGE_MASK      = 0x0000000000000100,

    /* bit 9~24 */
    ISEE_FLAGS_SOTER_FEATURE_OFFSET     = 9,
    ISEE_FLAGS_SOTER_FEATURE_RESET      = 0b0000000000000000,
    ISEE_FLAGS_SOTER_FEATURE_TUI        = 0b0000000000000001,
    ISEE_FLAGS_SOTER_FEATURE_GP         = 0b0000000000000010,
    ISEE_FLAGS_SOTER_FEATURE_DRM        = 0b0000000000000100,
    ISEE_FLAGS_SOTER_FEATURE_AARCH64    = 0b0000000000001000,
    ISEE_FLAGS_SOTER_FEATURE_MULTI_TA   = 0b0000000000010000,
    ISEE_FLAGS_SOTER_FEATURE_EFUSEDISABLE = 0b0000000000100000,
    ISEE_FLAGS_SOTER_FEATURE_RESERVED4  = 0b0000000001000000,
    ISEE_FLAGS_SOTER_FEATURE_RESERVED5  = 0b0000000010000000,
    ISEE_FLAGS_SOTER_FEATURE_RESERVED6  = 0b0000000100000000,
    ISEE_FLAGS_SOTER_FEATURE_RESERVED7  = 0b0000001000000000,
    ISEE_FLAGS_SOTER_FEATURE_RESERVED8  = 0b0000010000000000,
    ISEE_FLAGS_SOTER_FEATURE_RESERVED9  = 0b0000100000000000,
    ISEE_FLAGS_SOTER_FEATURE_RESERVED10 = 0b0001000000000000,
    ISEE_FLAGS_SOTER_FEATURE_RESERVED11 = 0b0010000000000000,
    ISEE_FLAGS_SOTER_FEATURE_RESERVED12 = 0b0100000000000000,
    ISEE_FLAGS_SOTER_FEATURE_RESERVED13 = 0b1000000000000000,
    ISEE_FLAGS_SOTER_FEATURE_MASK       = 0x0000000000FFFE00,

    /* bit 25 */
    ISEE_FLAGS_SOTER_SYSTEM_DEBUG_OFFSET = 25,
    ISEE_FLAGS_SOTER_SYSTEM_DEBUG_CLOSE  = 0b0,
    ISEE_FLAGS_SOTER_SYSTEM_DEBUG_OPEN   = 0b1,
    ISEE_FLAGS_SOTER_SYSTEM_DEBUG_MASK   = 0x0000000002000000,

    /* bit 26 */
    ISEE_FLAGS_RPMB_KEY_PROGRAM_OFFSET	= 26,
    ISEE_FLAGS_RPMB_KEY_IS_PROGRAMMED	= 0b0,
    ISEE_FLAGS_RPMB_KEY_NOT_PROGRAMMED	= 0b1,
    ISEE_FLAGS_RPMB_KEY_PROGRAM_MASK	= 0x0000000004000000,

    /* bit 27 */
    ISEE_FLAGS_BOOT_MODE_OFFSET        = 27,
    ISEE_FLAGS_BOOT_RELEASE_MODE       = 0b0,
    ISEE_FLAGS_BOOT_DEBUG_MODE         = 0b1,
    ISEE_FLAGS_BOOT_MODE_MASK          = 0x0000000008000000,

    /* bit 28 */
    ISEE_FLAGS_SYSTRACE_MODE_OFFSET    = 28,
    ISEE_FLAGS_SYSTRACE_DISABLE_MODE   = 0b0,
    ISEE_FLAGS_SYSTRACE_ENABLE_MODE    = 0b1,
    ISEE_FLAGS_SYSTRACE_MODE_MASK      = 0x0000000010000000,
};

/*
 * because different 32/64 system or different align rules ,
 * we use attribute packed  ,only init once so not speed probelm,
 */
typedef struct {
    u32 dev_type;       /* secure device type ,enum device_type */
    u64 base_addr;      /* secure deivice base address */
    u32 intr_num;       /* irq number for device */
    u32 apc_num;        /* secure  device apc (secure attribute) */
    u32 param[3];       /* others paramenter ,baudrate,speed,etc */
} __attribute__((packed)) tee_dev_t, *tee_dev_t_ptr;

typedef struct {
    unsigned int magic;                 /* magic value from information */
    unsigned int length;                /* size of struct in bytes. */
    unsigned long long version;         /* Version of structure */
    unsigned long long secDRamBase;     /* Secure DRAM start address */
    unsigned long long secDRamSize;     /* Secure DRAM size */
    unsigned long long gic_distributor_base;
#if GIC_VERSION == 0x02
    unsigned long long gic_cpuinterface_base;
#else
    unsigned long long gic_redistributor_base;
#endif
    unsigned int gic_version;
    unsigned int total_number_spi;
    unsigned int ssiq_number[5];
    tee_dev_t tee_dev[5];
    unsigned long long flags;
    unsigned int chip_hw_code;
} __attribute__((packed)) tee_arg_t, *tee_arg_t_ptr;

typedef struct {
    u32 magic;
    u32 version;
    u8 rpmb_key[KEY_LEN];	/* rpmb key */
    u8 hw_id[16];			/* modify hwid is 16byte */
    u8 hr_id[KEY_LEN];		/* modify hrid is 32byte */
    u32 hrid_size;			/* modify hrid is size 4byte */
    u32 hwid_size;			/* hwid size */
} tee_keys_t, *tee_keys_t_ptr;

#ifdef CFG_MICROTRUST_FP_SUPPORT
/*before use spi on TEE, set right SPI_INDEX for current platform */
#define SPI_INDEX   0

//#define DEBUG_ENABLE
#ifdef DEBUG_ENABLE
    #define DEBUG (1 << 3)
#else
    #define DEBUG 0
#endif

//#define LOOPBACK_ENABLE
#ifdef LOOPBACK_ENABLE
    #define LOOPBACK (1 << 4)
#else
    #define LOOPBACK 0
#endif
#endif
/**************************************************************************
 * EXPORTED FUNCTIONS
 **************************************************************************/
void teei_boot_param_prepare(u32 param_addr, u32 tee_entry, u64 teei_sec_dram_size, u64 dram_base,
                             u64 dram_size, u32 chip_hw_code);
void teei_key_param_prepare(u32 param_addr,u8 * hwuid, u32 hwid_size, u8 * hrid, u32 hrid_size, u8 * rpmb_key);
void teei_rtctime_param_prepare(u32 param_addr);
#endif /* TZ_TEEI_H */

