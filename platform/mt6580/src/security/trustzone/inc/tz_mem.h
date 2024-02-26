#include <stddef.h>

#ifndef _TZ_MEM_H_
#define _TZ_MEM_H_

#define SRAM_BASE_ADDRESS   0x00100000
#define SRAM_START_ADDR     0x00102140
#define VECTOR_START        (SRAM_START_ADDR + 0xBAC0)

typedef struct tz_memory_t {
    short next, previous;
} tz_memory_t;

#define FREE            ((short)(0x0001))
#define IS_FREE(x)      ((x)->next & FREE)
#define CLEAR_FREE(x)   ((x)->next &= ~FREE)
#define SET_FREE(x)     ((x)->next |= FREE)
#define FROM_ADDR(x)    ((short)(ptrdiff_t)(x))
#define TO_ADDR(x)      ((tz_memory_t *)(SRAM_BASE_ADDRESS + ((x) & ~FREE)))

/* SEC MEM magic */
#define SEC_MEM_MAGIC                   (0x3C562817U)
/* SEC MEM version */
#define SEC_MEM_VERSION                 (0x00010000U)
/* Tplay Table Size */
#define SEC_MEM_TPLAY_TABLE_SIZE        (0x1000)//4KB by default
#define SEC_MEM_TPLAY_MEMORY_SIZE       (0x000000)//0MB by default

#define ATF_BOOT_ARG_ADDR               (0x100000)  //no use 
#define TEE_PARAMETER_BASE              (ATF_BOOT_ARG_ADDR)

#define TEE_BOOT_ARG_ADDR               (ATF_BOOT_ARG_ADDR + 0x100)
#define TEE_PARAMETER_ADDR              (TEE_BOOT_ARG_ADDR + 0x100)

#define TEE_SECURE_ISRAM_ADDR           (0x120000)
#define TEE_SECURE_ISRAM_SIZE           (0xC000)

#if CFG_ATF_LOG_SUPPORT
#define ATF_LOG_BUFFER_SIZE             (0x200000)//2MB
#else
#define ATF_LOG_BUFFER_SIZE             (0x0)//don't support ATF log
#endif

typedef struct {
    unsigned int magic;           // Magic number
    unsigned int version;         // version
    unsigned int svp_mem_start;   // MM sec mem pool start addr.
    unsigned int svp_mem_end;     // MM sec mem pool end addr.
    unsigned int tplay_table_start; //tplay handle-to-physical table start
    unsigned int tplay_table_size;  //tplay handle-to-physical table size
    unsigned int tplay_mem_start;   //tplay physcial memory start address for crypto operation
    unsigned int tplay_mem_size;    //tplay phsycial memory size for crypto operation
    unsigned int secmem_obfuscation;//MM sec mem obfuscation or not
    unsigned int msg_auth_key[8];
    unsigned int rpmb_size;       /* size of rpmb partition */
    unsigned int emmc_rel_wr_sec_c;  //emmc ext_csd[222]

} sec_mem_arg_t;

#endif
