#include <string.h>
#include "dram_buffer.h"
#include "ram_console.h"

#define RAM_CONSOLE_SIG (0x43474244) /* DBGC */
#define MOD "RAM_CONSOLE"

struct ram_console_buffer {
	u32 sig;
	/* for size comptible */
	u32 off_pl;
	u32 off_lpl; /* last preloader */
	u32 sz_pl;
	u32 off_lk;
	u32 off_llk; /* last lk */
	u32 sz_lk;
	u32 padding[4]; /* size = 4 * 16 = 64 byte */
	u32 off_linux;
	u32 off_console;
	u32 padding2[3];
};

struct reboot_reason_pl {
	u32 wdt_status;
	u32 last_func[RAM_CONSOLE_PL_SIZE];
};

struct reboot_reason_kernel {
	u32 fiq_step;
	u32 exp_type; /* 0xaeedeadX: X=1 (HWT), X=2 (KE), X=3 (nested panic) */
	u32 others[0];
};

static struct ram_console_buffer *ram_console = NULL;
static int ram_console_size;

#define ALIGN(x, size) ((x + size - 1) & ~(size - 1))

static void ram_console_fatal(const char *str)
{
	print("%s FATAL:%s\n", MOD, str);
	while(1)
		;
}

#define bootarg g_dram_buf->bootarg
void ram_console_mblock_reserve(void)
{
	u64 addr_res;

	addr_res = mblock_reserve_ext(&bootarg.mblock_info, RAM_CONSOLE_DRAM_SIZE, 0x10000/*align*/, (RAM_CONSOLE_DRAM_ADDR + RAM_CONSOLE_DRAM_SIZE), 0, "ram_console");
	if (addr_res != RAM_CONSOLE_DRAM_ADDR)
		ram_console_fatal("reserve dram memory failed");
	addr_res = mblock_reserve_ext(&bootarg.mblock_info, PSTORE_SIZE, 0x10000/*align*/, (PSTORE_ADDR + PSTORE_SIZE), 0, "pstore");
	if (addr_res != PSTORE_ADDR)
		ram_console_fatal("reserve pstore memory failed");
	addr_res = mblock_reserve_ext(&bootarg.mblock_info, MRDUMP_MINI_HEADER_SIZE, 0x10000/*align*/, (MRDUMP_MINI_HEADER_ADDR + MRDUMP_MINI_HEADER_SIZE), 0, "minirdump");
	if (addr_res != MRDUMP_MINI_HEADER_ADDR)
		ram_console_fatal("reserve minirdump memory failed");
}


void ram_console_init(void)
{
    int i;
    struct reboot_reason_pl *rr_pl;
    ram_console = (struct ram_console_buffer *)RAM_CONSOLE_SRAM_ADDR;
    u32 def_type;

    def_type = ram_console_def_memory();
    if (def_type == RAM_CONSOLE_DEF_SRAM) {
	print("%s using SRAM\n", MOD);
	ram_console = (struct ram_console_buffer *)RAM_CONSOLE_SRAM_ADDR;
	ram_console_size = RAM_CONSOLE_SRAM_SIZE;
    } else if (def_type == RAM_CONSOLE_DEF_DRAM) {
	print("%s using DRAM\n", MOD);
	ram_console = (struct ram_console_buffer *)RAM_CONSOLE_DRAM_ADDR;
	ram_console_size = RAM_CONSOLE_DRAM_SIZE;
    } else {
	    print("%s def type:%d\n", MOD, def_type);
	    ram_console_fatal("[pl] unknown def type");
    }

    print("%s start: 0x%x, size: 0x%x, sig: 0x%x\n", MOD, ram_console, ram_console_size, ram_console->sig);
    if (ram_console->sig == RAM_CONSOLE_SIG && ram_console->sz_pl == sizeof(struct reboot_reason_pl) && ram_console->off_pl + ALIGN(ram_console->sz_pl, 64) == ram_console->off_lpl) {
	print("%s preloader last status: ", MOD);
	rr_pl = (void*)ram_console + ram_console->off_pl;
	for (i = 0; i < RAM_CONSOLE_PL_SIZE; i++) {
	    print("0x%x ", rr_pl->last_func[i]);
	}
	print("\n");
	memcpy((void *)ram_console + ram_console->off_lpl, (void *)ram_console + ram_console->off_pl, ram_console->sz_pl);
    } else {
	memset(ram_console, 0, ram_console_size);
	ram_console->sig = RAM_CONSOLE_SIG;
	ram_console->off_pl = sizeof(struct ram_console_buffer);
	ram_console->sz_pl = sizeof(struct reboot_reason_pl);
	ram_console->off_lpl = ram_console->off_pl + ALIGN(ram_console->sz_pl, 64);
	print("%s init done\n", MOD);
    }
}

void ram_console_reboot_reason_save(u32 rgu_status)
{
    struct reboot_reason_pl *rr_pl;
    if (ram_console) {
	rr_pl = (void*)ram_console + ram_console->off_pl;
	rr_pl->wdt_status = rgu_status;
	print("%s wdt status (0x%x)=0x%x\n", MOD,
	      rr_pl->wdt_status, rgu_status);
    }
}

void ram_console_pl_save(unsigned int val, int index)
{
    struct reboot_reason_pl *rr_pl;
    if (ram_console) {
	rr_pl = (void*)ram_console + ram_console->off_pl;
	if (index < RAM_CONSOLE_PL_SIZE)
	    rr_pl->last_func[index] = val;
    }
}

#define RE_BOOT_BY_WDT_SW 2
#define RE_BOOT_NORMAL_BOOT 0
#define RE_BOOT_FULL_PMIC 0x800
bool ram_console_is_abnormal_boot(void)
{
	unsigned int fiq_step, wdt_status, exp_type;

	if (!ram_console) {
		ram_console_init();
	}
	wdt_status = ((struct reboot_reason_pl*)((void*)ram_console + ram_console->off_pl))->wdt_status;
	if (wdt_status != RE_BOOT_NORMAL_BOOT && wdt_status != RE_BOOT_BY_WDT_SW && wdt_status != RE_BOOT_FULL_PMIC) {
		print("%s detect abnormal boot wdt_status:0x%x ", MOD, wdt_status);
		return true;
	}
	if (ram_console && ram_console->off_linux && ram_console->off_linux < ram_console_size && ram_console->off_pl < ram_console_size) {
		fiq_step = ((struct reboot_reason_kernel*)((void*)ram_console + ram_console->off_linux))->fiq_step;
		exp_type = ((struct reboot_reason_kernel*)((void*)ram_console + ram_console->off_linux))->exp_type;
		print("%s wdt_status 0x%x, fiq_step 0x%x, exp_type 0x%x\n", MOD, wdt_status, fiq_step, (exp_type ^ 0xaeedead0) < 16 ? exp_type ^ 0xaeedead0 : exp_type);
		if (fiq_step != 0 && (exp_type ^ 0xaeedead0) >= 16) {
			fiq_step = 0;
			((struct reboot_reason_kernel*)((void*)ram_console + ram_console->off_linux))->fiq_step = fiq_step;
		}
		if ((fiq_step == 0 && (wdt_status == RE_BOOT_BY_WDT_SW || wdt_status == RE_BOOT_FULL_PMIC)) || (wdt_status == RE_BOOT_NORMAL_BOOT))
			return false;
		else
			return true;
	}
	return false;
}

u32 ram_console_def_memory(void)
{
	if (RAM_CONSOLE_DEF_ADDR == RAM_CONSOLE_SRAM_ADDR)
		return RAM_CONSOLE_DEF_SRAM;
	else if (RAM_CONSOLE_DEF_ADDR == RAM_CONSOLE_DRAM_ADDR)
		return RAM_CONSOLE_DEF_DRAM;
	else
		return RAM_CONSOLE_DEF_UNKNOWN;
}

void ram_console_sram_addr_size(u32 *addr, int *size)
{
	if (addr != NULL)
		*addr = RAM_CONSOLE_SRAM_ADDR;
	if (size != NULL)
		*size = RAM_CONSOLE_SRAM_SIZE;
}

#define SHOW_ARGS(p, a, b, c, d) \
	print("RAM_CONSOLE " #a":0x%x, "#b":0x%x, "#c":0x%x, "#d":0x%x\n", (p)->a, (p)->b, (p)->c, (p)->d)
static void ram_console_memory_info_show(struct ram_console_memory_info *memory_info)
{
	if (memory_info == NULL) {
		print("%s. %s args null\n", MOD, __func__);
		return;
	}

	SHOW_ARGS(memory_info, sram_plat_dbg_info_addr, sram_plat_dbg_info_size, sram_log_store_addr, sram_log_store_size);
	SHOW_ARGS(memory_info, mrdump_addr, mrdump_size, dram_addr, dram_size);
	SHOW_ARGS(memory_info, pstore_addr, pstore_size, pstore_console_size, pstore_pmsg_size);
	SHOW_ARGS(memory_info, mrdump_mini_header_addr, mrdump_mini_header_size, magic1, magic2);
}

u32 ram_console_memory_info_offset(void)
{
#ifdef RAM_CONSOLE_SRAM_SIZE_OVERALL
	struct ram_console_memory_info *memory_info;

	u32 offset = RAM_CONSOLE_MEMORY_INFO_OFFSET;
	print("%s offset:0x%x\n", MOD, offset);

	if (offset <= 0 || offset >= RAM_CONSOLE_SRAM_SIZE_OVERALL) {
		print("%s. illegal offset\n", MOD);
		return 0;
	}

	memory_info = (struct ram_console_memory_info *)(RAM_CONSOLE_SRAM_ADDR + offset);
	memset(memory_info, 0, sizeof(struct ram_console_memory_info));
	memory_info->magic1 = MEM_MAGIC1;
	memory_info->sram_plat_dbg_info_addr = RAM_CONSOLE_PLAT_DBG_INFO_ADDR;
	memory_info->sram_plat_dbg_info_size = RAM_CONSOLE_PLAT_DBG_INFO_SIZE;
	memory_info->sram_log_store_addr = RAM_CONSOLE_LOG_STORE_ADDR;
	memory_info->sram_log_store_size = RAM_CONSOLE_LOG_STORE_SIZE;
	memory_info->mrdump_addr = RAM_CONSOLE_MRDUMP_ADDR;
	memory_info->mrdump_size = RAM_CONSOLE_MRDUMP_SIZE;
	memory_info->dram_addr = RAM_CONSOLE_DRAM_ADDR;
	memory_info->dram_size = RAM_CONSOLE_DRAM_SIZE;
	memory_info->pstore_addr = PSTORE_ADDR;
	memory_info->pstore_size = PSTORE_SIZE;
	memory_info->pstore_console_size = PSTORE_CONSOLE_SIZE;
	memory_info->pstore_pmsg_size = PSTORE_PMSG_SIZE;
	memory_info->mrdump_mini_header_addr = MRDUMP_MINI_HEADER_ADDR;
	memory_info->mrdump_mini_header_size = MRDUMP_MINI_HEADER_SIZE;
	memory_info->magic2 = MEM_MAGIC2;

	ram_console_memory_info_show(memory_info);

	return offset;
#else
	return 0;
#endif
}
