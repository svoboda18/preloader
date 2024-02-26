/* Include header files */
#include "typedefs.h"
#include "tz_mem.h"
#include <mt_uart.h>
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
