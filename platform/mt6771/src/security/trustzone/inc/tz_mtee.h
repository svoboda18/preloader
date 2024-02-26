
#ifndef TZ_MTEE_H
#define TZ_MTEE_H

#include <typedefs.h>

#define KEY_LEN 32

enum device_type {
    MT_UNUSED = 0,
    MT_UART16550 =1,    /* secure uart */
    MT_SEC_GPT,         /* secure gp timer */
    MT_SEC_WDT,         /* secure watch dog */
    MT_SEC_CRYPTO,
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

/**************************************************************************
 * EXPORTED FUNCTIONS
 **************************************************************************/
void mtee_boot_param_prepare(u32 param_addr, u32 tee_entry, u32 mtee_sec_dram_size, u64 dram_base,
                             u64 dram_size);
void mtee_key_param_prepare(u32 param_addr, u8 *hwuid, u32 hwid_size, u8 *hrid, u32 hrid_size, u8 *rpmb_key);
void mtee_rtctime_param_prepare(u32 param_addr);
#endif /* TZ_MTEE_H */

