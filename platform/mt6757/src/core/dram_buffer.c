#include "dram_buffer.h"
#include "typedefs.h"
#include "emi.h"
#include "memory_layout.h"

#define MOD "[Dram_Buffer]"

dram_buf_t* g_dram_buf = 0;

void init_dram_buffer(){
	u32 structure_size = sizeof(dram_buf_t);

	printf("%s dram_buf_t size: 0x%x \n" ,MOD, structure_size);
	printf("%s part_hdr_t size: 0x%x \n" ,MOD, sizeof(part_hdr_t));
	printf("%s sizeof(boot_arg_t): 0x%x \n" ,MOD, sizeof(boot_arg_t));
	/*allocate dram_buf*/
#if defined(SLT)
  if ((u64)CFG_DRAM_ADDR + platform_memory_size() > (u64)0x10000000)
    g_dram_buf = 0x100000000- (5*1024*1024);
  else
    g_dram_buf = CFG_DRAM_ADDR + platform_memory_size() - (5*1024*1024); // Max DRAM size - 5MB
#elif defined(DUMMY_AP)
  g_dram_buf = CFG_DRAM_ADDR;
#else
  g_dram_buf = PL_BOOTARG_BASE; //0x4480_0000 ~ 0x44A0_0000
#endif
    // init boot argument
    memset((void *)&(g_dram_buf->bootarg), 0, sizeof(boot_arg_t));

	//make sure dram_buffer if move to the head of memory
	print("%s g_dram_buf start addr: 0x%x \n" ,MOD, g_dram_buf);
	//make sure msdc_gpd_pool and msdc_bd_pool is 64 bytes alignment
	print("%s g_dram_buf->msdc_gpd_pool start addr: 0x%x \n" ,MOD, g_dram_buf->msdc_gpd_pool);
	print("%s g_dram_buf->msdc_bd_pool start addr: 0x%x \n" ,MOD, g_dram_buf->msdc_bd_pool);
}

