#ifndef DRAM_BUFFER_H 
#define DRAM_BUFFER_H

#include "nand_core.h"
#include "platform.h"
#include "partition.h"
#include "partition.h"
#include "msdc.h"	// for 	gpd_t, bd_t structure
#include "pmt.h"
/*bmt.c*/
#define BMT_BUFFER_SIZE     0x10000
#define MAX_MAIN_SIZE                (0x4000) /*nand_core.h*/
#define MAX_SPAR_SIZE                (0x500) /*nand_core.h*/
#define BMT_DAT_BUFFER_SIZE         (MAX_MAIN_SIZE + MAX_SPAR_SIZE) /*nand_core.h*/
/*nand.c*/
#define NFI_BUF_MAX_SIZE             (0x10000)         /*nand_core.h*/
#define NAND_NFI_BUFFER_SIZE        (NFI_BUF_MAX_SIZE) /*nand_core.h*/

#define NAND_NFI_BUFFER_SIZE        (NFI_BUF_MAX_SIZE) /*nand_core.h*/
#define HYNIX_RR_TABLE_SIZE_ALIGN         (0x800)  //hynix read retry table size :4byte align

/*download.c*/
#define PART_HDR_BUF_SIZE 512

/*efi.c*/
#define GPT_BUFFER_SIZE                 (0x4000)

/*mmc_common_inter.c*/
#define STORAGE_BUFFER_SIZE 0x10000

/*partition.c*/
#define IMG_HDR_BUF_SIZE 512

/*partition.c*/
#define EMMC_PMT_BUFFER_SIZE             (0x20000)

/*print.c*/
#define LOG_BUFFER_MAX_SIZE             (0x10000)

/*sec.c*/
#define SEC_SECRO_BUFFER_LENGTH     (0x3000)   /*buffer.h*/
#define SEC_WORKING_BUFFER_LENGTH   0x2000  
#define SEC_UTIL_BUFFER_LENGTH      0x1000   
#define SEC_LIB_HEAP_LENGTH         0x4000   
#define SEC_IMG_BUFFER_LENGTH       0x3000     
#define SEC_CHUNK_BUFFER_LENGTH     0x100000 

#if CFG_BYPASS_EMI
typedef struct{
#ifndef MTK_EMMC_SUPPORT
    /*bmt.c*/
    u8 bmt_buf[0x10000];
	u8 bmt_dat_buf[BMT_DAT_BUFFER_SIZE];
    /*nand.c*/
    u8 nand_nfi_buf[0x1000];
    u8 nand_hynix_rr_table[HYNIX_RR_TABLE_SIZE_ALIGN];
	u8 Bad_Block_Table[8192];
#endif
    /*download.c*/
    part_hdr_t part_hdr_buf[1];  
    /*efi.c*/
    u32 crc32_table[256];
    u8 gpt_header_buf[512];
    u8 gpt_entries_buf[GPT_BUFFER_SIZE];
    /*mmc_common_inter.c*/
    unsigned char storage_buffer[16];
	/*partition.c*/
	u8 img_hdr_buf[IMG_HDR_BUF_SIZE];
	unsigned int part_num;
    part_hdr_t   part_info[2];
	/*partition.c*/
	u8 emmc_pmt_buf[EMMC_PMT_BUFFER_SIZE];
	pt_resident new_part[PART_MAX_COUNT];
    pt_resident lastest_part[PART_MAX_COUNT];
#ifndef MTK_EMMC_SUPPORT
	u8  pmt_dat_buf[PMT_DAT_BUFFER_SIZE];
	u8  pmt_read_buf[PMT_READ_BUFFER_SIZE];
#endif
    part_t  partition_info[2];    
	
#ifdef MTK_EMMC_SUPPORT
    struct part_meta_info meta_info[1];
#endif
	/*platform.c*/
	boot_arg_t bootarg; 
	/*print.c*/
    u8 log_dram_buf[0x1000];
	/*sec.c*/
	u8  sec_secro_buf[SEC_SECRO_BUFFER_LENGTH];
	u8  sec_working_buf[SEC_WORKING_BUFFER_LENGTH];/*This dram Buffer not used for security concern*/
	u8  sec_util_buf[SEC_UTIL_BUFFER_LENGTH];
	u8  sec_lib_heap_buf[SEC_LIB_HEAP_LENGTH];
	u8  sec_img_buf[SEC_IMG_BUFFER_LENGTH];        /*This dram Buffer not used for security concern*/
    u8  sec_chunk_buf[0x4000];
	/*msdc.c*/
	gpd_t msdc_gpd_pool[MSDC_MAX_NUM][MAX_GPD_POOL_SZ] __attribute__ ((aligned(64)));
	bd_t  msdc_bd_pool[MSDC_MAX_NUM][MAX_BD_POOL_SZ] __attribute__ ((aligned(64)));
    boot_tag *boottag; /*boot args pass to LK by ATAG*/
} dram_buf_t;


#else    //#if CFG_BYPASS_EMI
typedef struct{
    /*bmt.c*/
#ifndef MTK_EMMC_SUPPORT
	u8 bmt_buf[BMT_BUFFER_SIZE];
	u8 bmt_dat_buf[BMT_DAT_BUFFER_SIZE];
    /*nand.c*/
	u8 nand_nfi_buf[NAND_NFI_BUFFER_SIZE];
    u8 nand_hynix_rr_table[HYNIX_RR_TABLE_SIZE_ALIGN];
	u8 Bad_Block_Table[8192];
#endif	
    /*download.c*/
	part_hdr_t part_hdr_buf[PART_HDR_BUF_SIZE];  
    /*efi.c*/
    u32 crc32_table[256];
    u8 gpt_header_buf[4096];
    u8 gpt_entries_buf[GPT_BUFFER_SIZE];
    /*mmc_common_inter.c*/
	unsigned char storage_buffer[STORAGE_BUFFER_SIZE];
	/*partition.c*/
	u8 img_hdr_buf[IMG_HDR_BUF_SIZE];
	unsigned int part_num;
	part_hdr_t   part_info[PART_MAX_NUM];
	/*partition.c*/
	u8 emmc_pmt_buf[EMMC_PMT_BUFFER_SIZE];
	pt_resident new_part[PART_MAX_COUNT];
    pt_resident lastest_part[PART_MAX_COUNT];
#ifndef MTK_EMMC_SUPPORT
	u8  pmt_dat_buf[PMT_DAT_BUFFER_SIZE];
	u8  pmt_read_buf[PMT_READ_BUFFER_SIZE];
#endif
    part_t  partition_info[128];
	
#ifdef MTK_EMMC_SUPPORT
    struct part_meta_info meta_info[128];
#endif
	/*platform.c*/
	boot_arg_t bootarg; 
	/*print.c*/
	u8 log_dram_buf[LOG_BUFFER_MAX_SIZE];
	/*sec.c*/
	u8  sec_secro_buf[SEC_SECRO_BUFFER_LENGTH];
	u8  sec_working_buf[SEC_WORKING_BUFFER_LENGTH];/*This dram Buffer not used for security concern*/
	u8  sec_util_buf[SEC_UTIL_BUFFER_LENGTH];
	u8  sec_lib_heap_buf[SEC_LIB_HEAP_LENGTH];
	u8  sec_img_buf[SEC_IMG_BUFFER_LENGTH];        /*This dram Buffer not used for security concern*/
	u8  sec_chunk_buf[SEC_CHUNK_BUFFER_LENGTH];
	/*msdc.c*/
	gpd_t msdc_gpd_pool[MSDC_MAX_NUM][MAX_GPD_POOL_SZ] __attribute__ ((aligned(64)));
	bd_t  msdc_bd_pool[MSDC_MAX_NUM][MAX_BD_POOL_SZ] __attribute__ ((aligned(64)));
    boot_tag *boottag; /*boot args pass to LK by ATAG*/
} dram_buf_t;
#endif    //#if CFG_BYPASS_EMI

typedef struct{
    u8  sram_sec_working_buf[SEC_WORKING_BUFFER_LENGTH];
    u8  sram_sec_img_buf[SEC_IMG_BUFFER_LENGTH];
} sec_buf_t;
void init_dram_buffer(void);

extern dram_buf_t *g_dram_buf;

extern sec_buf_t  g_sec_buf;
#endif /*DRAM_BUFFER_H*/
