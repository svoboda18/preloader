/* Include header files */
#include "typedefs.h"
#include "tz_mem.h"
#include "uart.h"
#include "dram_buffer.h"
#include "platform.h"

#include <rtc.h>

#include "tz_tkcore.h"

#define MOD "[TZ_TKCORE]"

#define DBG_MSG(str, ...) do { print(str, ##__VA_ARGS__); } while(0)

#if CFG_BOOT_ARGUMENT_BY_ATAG
extern unsigned int g_uart;
#elif CFG_BOOT_ARGUMENT && !CFG_BOOT_ARGUMENT_BY_ATAG
#define bootarg g_dram_buf->bootarg
#endif

extern unsigned int seclib_get_soc_id(unsigned char *soc_id,
                                      unsigned int size);

#if CFG_TRUSTKERNEL_TEE_SDRPMB_SUPPORT

struct sdrpmb_info {
    int failed; int part_id;
    u32 sdrpmb_part_start;

    u32 sdrpmb_partaddr;
    u32 sdrpmb_partsize;
    u32 sdrpmb_starting_sector;
    u32 sdrpmb_nr_sectors;
} sdrpmb_info = { 0, -1, 0U, 0U, 0U, 0U, 0U };

static u64 mblock_reserve_dryrun(mblock_info_t *mblock_info, u64 reserved_size)
{
    int i, max_rank, target = -1;
    u64 start, end, sz, max_addr = 0;
    u64 reserved_addr = 0, align, limit;
    mblock_t mblock;

    align = 1ULL << 20;
    /* address cannot go beyond 64bit */
    limit = 0x100000000ULL;
    /* always allocate from the larger rank */
    max_rank = mblock_info->mblock_num - 1;

    for (i = 0; i < mblock_info->mblock_num; i++) {
        start = mblock_info->mblock[i].start;
        sz = mblock_info->mblock[i].size;
        end = limit < (start + sz)? limit: (start + sz);
        reserved_addr = (end - reserved_size);
        reserved_addr &= ~(align - 1);
        if ((reserved_addr + reserved_size <= start + sz) &&
                (reserved_addr >= start) &&
                (mblock_info->mblock[i].rank <= max_rank) &&
                (start + sz > max_addr) &&
                (reserved_addr + reserved_size <= limit)) {
            max_addr = start + sz;
            target = i;
        }
    }

    if (target < 0) {
        print("mblock_reserve error\n");
        return 0;
    }

    start = mblock_info->mblock[target].start;
    sz = mblock_info->mblock[target].size;
    end = limit < (start + sz)? limit: (start + sz);
    reserved_addr = (end - reserved_size);
    reserved_addr &= ~(align - 1);

    return reserved_addr;
}

/* note that memory is not really reserved */
static int reserve_tmpmem(mblock_info_t *mblock_info, u32 *addr, u32 size)
{
    u64 _addr = mblock_reserve_dryrun(mblock_info, size);
    if (_addr == 0ULL) {
        return -1;
    }

    /* we only reserve memory lower than 32-bit address, thus
       we can safely convert variable to u32 */
    *addr = (u32) _addr;
    return 0;
}

void sdrpmb_init_set_failed(void)
{
    sdrpmb_info.failed = 1;
    sdrpmb_info.sdrpmb_partaddr = SDRPMB_FAILURE_MAGIC;
    sdrpmb_info.sdrpmb_partsize = 0;
}

void tkcore_boot_param_prepare_sdrpmb_region(part_t *part)
{
    if (sdrpmb_info.failed || part == NULL) {
        return;
    }

    sdrpmb_info.part_id = part->part_id;

    u32 sect = part->start_sect + part->nr_sects;

    if (sect < SDRPMB_REGION_SIZE / 512) {
        print("%s: unexpected MMC size: %u sectors\n", MOD, sect);
        goto err;
    }
    sect -= SDRPMB_REGION_SIZE / 512;
    /* sect % N must be smaller than sect */
    sect -= sect % (SDRPMB_REGION_ALIGNMENT / 512);

    if (sect < part->start_sect) {
        print("%s: unexpected sdrpmb partition start: %u size: %u\n", MOD, part->start_sect, part->nr_sects);
        goto err;
    }

    sdrpmb_info.sdrpmb_part_start = part->start_sect;

    sdrpmb_info.sdrpmb_starting_sector = sect;
    sdrpmb_info.sdrpmb_nr_sectors = SDRPMB_REGION_SIZE / 512;

    return;

err:
    sdrpmb_init_set_failed();
}

void tkcore_boot_param_prepare_sdrpmb_data(mblock_info_t *mblock, blkdev_t *bootdev)
{
    int ret = 0;
    u64 start_byte;

    if (sdrpmb_info.failed)
        return;

    /* check if sdrpmb region is not reserved */
    if (sdrpmb_info.part_id < 0)
        return;

    if (mblock == NULL || bootdev == NULL) {
        ret = -1;
        goto out;
    }

    sdrpmb_info.sdrpmb_partsize = SDRPMB_DATA_SIZE << 1;
    ret = reserve_tmpmem(mblock, &(sdrpmb_info.sdrpmb_partaddr), SDRPMB_DATA_SIZE << 1);
    if (ret) {
        print("%s: reserve memory failed\n", MOD);
        goto out;
    }

    start_byte = ((u64) sdrpmb_info.sdrpmb_starting_sector) * 512;

    if ((ret = blkdev_read(bootdev, start_byte, SDRPMB_DATA_SIZE,
        (u8 *) (sdrpmb_info.sdrpmb_partaddr), sdrpmb_info.part_id))) {
        print("%s: read SDRPMB.0 failed", MOD);
        goto out;
    }

    if ((ret = blkdev_read(bootdev, start_byte + (SDRPMB_REGION_SIZE >> 1),
        SDRPMB_DATA_SIZE, (u8 *) (sdrpmb_info.sdrpmb_partaddr + SDRPMB_DATA_SIZE),
        sdrpmb_info.part_id))) {
        print("%s: read SDRPMB.1 failed", MOD);
        goto out;
    }

out:
    if (ret)
        sdrpmb_init_set_failed();
    return;
}

void tkcore_boot_sdrpmb_init_finish(u32 param_addr)
{
    int ret = 0;
    tee_arg_t_ptr teearg = (tee_arg_t_ptr) param_addr;

    if (teearg == NULL)
        return;

    if (sdrpmb_info.failed || sdrpmb_info.part_id < 0)
        return;

    teearg->sdrpmb_partaddr = sdrpmb_info.sdrpmb_partaddr;
    teearg->sdrpmb_partsize = sdrpmb_info.sdrpmb_partsize;
    teearg->sdrpmb_starting_sector = sdrpmb_info.sdrpmb_starting_sector - sdrpmb_info.sdrpmb_part_start;
    teearg->sdrpmb_nr_sectors = sdrpmb_info.sdrpmb_nr_sectors;

    return;
}
#endif

static u32 rtc_mktime(int yea, int mth, int dom, int hou, int min, int sec)

{
    unsigned long d1, d2, d3;

    mth -= 2;
    if (mth <= 0) {
        mth += 12;
        yea -= 1;
    }

    d1 = (yea - 1) * 365 + (yea / 4 - yea / 100 + yea / 400);
    d2 = (367 * mth / 12 - 30) + 59;
    d3 = d1 + d2 + (dom - 1) - 719162;

    return ((d3 * 24 + hou) * 60 + min) * 60 + sec;
}

void tkcore_boot_param_prepare(u32 param_addr, u32 tee_entry,
    u64 sec_dram_size, u64 dram_base, u64 dram_size, int meta_uart)
{
    tee_arg_t_ptr teearg = (tee_arg_t_ptr) param_addr;

    if (teearg == NULL) {
        return;
    }

    /* Prepare TEE boot parameters */
    teearg->magic = TKCORE_BOOTCFG_MAGIC;
    teearg->length = sizeof(tee_arg_t);
    teearg->version = (u64) TEE_ARGUMENT_VERSION;
    teearg->dRamBase = dram_base;
    teearg->dRamSize = dram_size;
    teearg->secDRamBase = tee_entry;
    teearg->secDRamSize = sec_dram_size;
    teearg->secIRamBase = TEE_SECURE_ISRAM_ADDR;
    teearg->secIRamSize = TEE_SECURE_ISRAM_SIZE;

    /* GIC parameters */
    teearg->total_number_spi = 256 + 32;
    /* Does not reserve SSIQ */
    teearg->ssiq_number = 0;

    teearg->flags = 0;

    if (meta_uart == 0) {
#if CFG_BOOT_ARGUMENT_BY_ATAG
        teearg->uart_base = g_uart;
#elif CFG_BOOT_ARGUMENT && !CFG_BOOT_ARGUMENT_BY_ATAG
        teearg->uart_base = bootarg.log_port;
#else
        teearg->uart_base = CFG_UART_LOG;
#endif
    } else {
        teearg->uart_base = 0U;
    }
}

void tkcore_dump_param(u32 param_addr)
{
    tee_arg_t_ptr teearg = (tee_arg_t_ptr) param_addr;

    if (teearg->version >= TEE_ARGUMENT_VERSION_V1_0) {
        DBG_MSG("%s teearg.rpmb_key_programmed : %d\n", MOD, teearg->rpmb_key_programmed);
    }

    if (teearg->version >= TEE_ARGUMENT_VERSION_V1_1) {
        DBG_MSG("%s teearg.nw_bootargs: 0x%x\n",
            MOD, teearg->nw_bootargs);

        DBG_MSG("%s teearg.nw_bootargs_size: 0x%x\n",
            MOD, teearg->nw_bootargs_size);
    }

    if (teearg->version >= TEE_ARGUMENT_VERSION_V1_2) {
        DBG_MSG("%s teearg.sdrpmb_partaddr: 0x%x\n",
            MOD, teearg->sdrpmb_partaddr);

        DBG_MSG("%s teearg.sdrpmb_partsize: 0x%x\n",
            MOD, teearg->sdrpmb_partsize);

        DBG_MSG("%s teearg.sdrpmb_starting_sector: 0x%x\n",
            MOD, teearg->sdrpmb_starting_sector);

        DBG_MSG("%s teearg.sdrpmb_nr_sectors: 0x%x\n",
            MOD, teearg->sdrpmb_nr_sectors);
    }
}

void tkcore_boot_param_prepare_nwbootargs(u32 param_addr, u32 addr, u32 size)
{
    tee_arg_t_ptr teearg = (tee_arg_t_ptr) param_addr;

    if (teearg == NULL)
        return;

    teearg->nw_bootargs = addr;
    teearg->nw_bootargs_size = size;
}

void tkcore_boot_param_prepare_rpmbkey(u32 param_addr,
                                       u8 *rpmb_key,
                                       u32 rpmb_keylen)
{
    tee_arg_t_ptr teearg = (tee_arg_t_ptr) param_addr;

    if (teearg == NULL) {
        return ;
    }

    if (rpmb_keylen != RPMB_KEY_SIZE) {
        return ;
    }

    memcpy(teearg->rpmb_key, rpmb_key, RPMB_KEY_SIZE);
    teearg->rpmb_key_programmed = 1;
}

void tkcore_boot_param_prepare_soc_id(u32 param_addr)
{
    unsigned int r;
    tee_arg_t_ptr teearg = (tee_arg_t_ptr) param_addr;

    if (teearg == NULL)
        return;

    r = seclib_get_soc_id(teearg->soc_id, sizeof(teearg->soc_id));
    if (r) {
        print("Failed to get soc_id");
        memset(teearg->soc_id, 0, sizeof(teearg->soc_id));
    }

    teearg->soc_id_programmed = 1;
}

void tkcore_boot_param_prepare_rtctime(u32 param_addr)
{
    struct rtc_time tm;
    tee_arg_t_ptr teearg = (tee_arg_t_ptr) param_addr;

    if (teearg == NULL)
        return;

    rtc_get_time(&tm);

    teearg->rtc_time = rtc_mktime(
    tm.tm_year, tm.tm_mon, tm.tm_mday,
    tm.tm_hour, tm.tm_min, tm.tm_sec);
}
