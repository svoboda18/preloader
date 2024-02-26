//=============================================================================
//  Include Files
//=============================================================================
//#include <common.h>
//#include <ett_common.h>
//#include <test_case_controller.h>
//#include <api.h>
//#include "gpio.h"
//#include "ett_cust.h"
//#include "emi_setting.h"
//#include "pll.h"
//#include "dramc_pi_api.h"
#include "emi_hw.h"
#include "dramc_common.h"

#if !__FLASH_TOOL_DA__ && !__ETT__ 
#include <blkdev.h>
#include <partition.h>
#include <storage_api.h>
#endif

#if !__ETT__
#if (FOR_DV_SIMULATION_USED==0)
#include "dram_buffer.h"
#include  "gpio.h"
#define GPIO_110        (0x80000000 | 110)
#endif
#else
#include <barriers.h>
#endif

#if !__FLASH_TOOL_DA__ && !__ETT__
#if (FOR_DV_SIMULATION_USED==0)
#include "platform.h"
#include "emi_hw.h"
#if defined(DEF_LAST_DRAMC)
#include "plat_dbg_info.h"
#endif
#endif
#endif

#include "emi.h"
#include "voltage.h"

#if !__FLASH_TOOL_DA__ && !__ETT__
#include "custom_emi.h"   // fix build error: emi_settings
#endif

#if CFG_BOOT_ARGUMENT
#define bootarg g_dram_buf->bootarg
#endif

#if (FOR_DV_SIMULATION_USED==0)
#include <pmic.h>
#define MTK_PMIC_CHIP_MT6358
#endif

#if !__ETT__
#define mt_reg_sync_write(x,y) mt_reg_sync_writel(y,x)
#endif



#if !__ETT__
#define CQ_DMA_BASE (0x10212000)
#endif

//=============================================================================
//  Definition
//=============================================================================

static unsigned int get_dramc_addr(dram_addr_t *dram_addr, unsigned int offset);

//=============================================================================
//  Global Variables
//=============================================================================
int emi_setting_index = -1;
static int enable_combo_dis = 0;
static unsigned int mr5;
static unsigned int rank_swap;
extern DRAMC_CTX_T *psCurrDramCtx;
extern EMI_SETTINGS default_emi_setting;
#ifdef MTK_PMIC_CHIP_MT6358
#include <regulator/mtk_regulator.h>
static struct mtk_regulator reg_vdram, reg_vcore, reg_vddq, reg_vio18;
unsigned int pmic_trap_ddr_type = 0;
unsigned int mt_get_dram_type_from_hw_trap(void);
#endif

#ifdef LAST_DRAMC
static LAST_DRAMC_INFO_T* last_dramc_info_ptr;
#endif

#ifdef LAST_EMI
static LAST_EMI_INFO_T* last_emi_info_ptr;
#endif

#ifdef VOLTAGE_SEL
static VOLTAGE_SEL_INFO_T voltage_sel_info_ptr;
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
extern u64 get_part_addr(const char *name);
static int read_offline_dram_mdl_data(DRAM_INFO_BY_MRR_T *DramInfo);
static int write_offline_dram_mdl_data(DRAM_INFO_BY_MRR_T *DramInfo);
static u64 part_dram_data_addr = 0;
static unsigned int dram_offline_data_flags = 0;
#endif

/*Global Variables for LOG to storage EYE scan*/
#if !__FLASH_TOOL_DA__ && !__ETT__ 
extern u64 get_part_addr(const char *name);
static u64 part_dram_data_addr_uart = 0;
static u32 log_start = 0;
u32 dram_log_to_storage_flag = 0;
#endif

//=============================================================================
//  External references
//=============================================================================
extern DRAMC_CTX_T *psCurrDramCtx;
extern char* opt_dle_value;

void print_DBG_info(DRAMC_CTX_T *p);
void Dump_EMIRegisters(DRAMC_CTX_T *p);

#if 0// __ETT__
#define EMI_CONA               (EMI_APB_BASE + 0x000)
#define EMI_CONF               (EMI_APB_BASE + 0x028)
#define EMI_CONH               (EMI_APB_BASE + 0x038)
#endif

#define CHN_EMI_CONA(base)	(base + 0x000)
#define CHN_EMI_CONC(base)	(base + 0x010)

void emi_cen_config(unsigned cfg_base_addr)  {
  unsigned emi_apb_base = cfg_base_addr;
  unsigned int  rd_tmp;
#ifndef ONE_CH
  #ifdef RANK_512MB  // => 2channel , dual rank , total=2G
    mt_reg_sync_write(emi_apb_base+0x00000000,0xa053a154);
  #else  //RANK_1G  => 2channel , dual rank , total=4G
    mt_reg_sync_write(emi_apb_base+0x00000000,0xf053f154);
  #endif
#else
  #ifdef RANK_512MB
    mt_reg_sync_write(emi_apb_base+0x00000000,0xa053a054);
  #else
    mt_reg_sync_write(emi_apb_base+0x00000000,0xf053f054);
  #endif
#endif
//=Row = 14-bit;
  mt_reg_sync_write(emi_apb_base+0x00000008,0x17283544);
  mt_reg_sync_write(emi_apb_base+0x00000010,0x0a1a0b1a);
  mt_reg_sync_write(emi_apb_base+0x00000018,0x3657587a);
  mt_reg_sync_write(emi_apb_base+0x00000020,0x0000c042);
  mt_reg_sync_write(emi_apb_base+0x00000028,0x08421000);
  mt_reg_sync_write(emi_apb_base+0x00000030,0x2b2b2a38);
  mt_reg_sync_write(emi_apb_base+0x00000038,0x00001783);
  mt_reg_sync_write(emi_apb_base+0x0000003c,0x00000010);
  mt_reg_sync_write(emi_apb_base+0x00000040,0x00008802);
  mt_reg_sync_write(emi_apb_base+0x00000060,0x007811ff); // reserved buffer to normal read/write :8/7
  mt_reg_sync_write(emi_apb_base+0x00000068,0x00000000);
  mt_reg_sync_write(emi_apb_base+0x00000078,0x11120c1f);  //22:20=ultra_w=1
  mt_reg_sync_write(emi_apb_base+0x00000710,0x11120c1f);  //22:20=ultra_w=1
  mt_reg_sync_write(emi_apb_base+0x0000007c,0x00001123);
  mt_reg_sync_write(emi_apb_base+0x00000718,0x00001123);
  mt_reg_sync_write(emi_apb_base+0x000000d0,0xa8a8a8a8);
  mt_reg_sync_write(emi_apb_base+0x000000d4,0x25252525);
  mt_reg_sync_write(emi_apb_base+0x000000d8,0xa8a8a8a8);
  mt_reg_sync_write(emi_apb_base+0x000000dc,0x25252525);
  mt_reg_sync_write(emi_apb_base+0x000000e8,0x00060037); // initial starvation counter div2, [4]=1
  mt_reg_sync_write(emi_apb_base+0x000000f0,0x38420014);
  mt_reg_sync_write(emi_apb_base+0x000000f8,0xa0000000);
/////  EMI  bandwidth limit  ////////// default UI
#ifdef SCN_ICFP
  mt_reg_sync_write(emi_apb_base+0x00000100,0x10107244);
  mt_reg_sync_write(emi_apb_base+0x00000108,0x10107044);
  mt_reg_sync_write(emi_apb_base+0x00000110,0x040a50df);
  mt_reg_sync_write(emi_apb_base+0x00000118,0x0000f0d0);
  mt_reg_sync_write(emi_apb_base+0x00000120,0x08086048);
  mt_reg_sync_write(emi_apb_base+0x00000124,0x00000034);  
  mt_reg_sync_write(emi_apb_base+0x00000128,0x0a0a50df);
  mt_reg_sync_write(emi_apb_base+0x00000130,0x20207044);
  mt_reg_sync_write(emi_apb_base+0x00000138,0x20207044);  
#elif SCN_VPWFD
  mt_reg_sync_write(emi_apb_base+0x00000100,0x10107244);
  mt_reg_sync_write(emi_apb_base+0x00000108,0x10107044);
  mt_reg_sync_write(emi_apb_base+0x00000110,0x040a50df);
  mt_reg_sync_write(emi_apb_base+0x00000118,0x0000f0d0);
  mt_reg_sync_write(emi_apb_base+0x00000120,0x08086048);
  mt_reg_sync_write(emi_apb_base+0x00000124,0x00000034);  
  mt_reg_sync_write(emi_apb_base+0x00000128,0x0a0a50df);
  mt_reg_sync_write(emi_apb_base+0x00000130,0x20207044);
  mt_reg_sync_write(emi_apb_base+0x00000138,0x20207044);  
#elif SCN_VR4K
  mt_reg_sync_write(emi_apb_base+0x00000100,0x10107244);
  mt_reg_sync_write(emi_apb_base+0x00000108,0x10107044);
  mt_reg_sync_write(emi_apb_base+0x00000110,0x040a50df);
  mt_reg_sync_write(emi_apb_base+0x00000118,0x0000f0d0);
  mt_reg_sync_write(emi_apb_base+0x00000120,0x08086048);
  mt_reg_sync_write(emi_apb_base+0x00000124,0x00000034);  
  mt_reg_sync_write(emi_apb_base+0x00000128,0x0a0a50df);
  mt_reg_sync_write(emi_apb_base+0x00000130,0x20207044);
  mt_reg_sync_write(emi_apb_base+0x00000138,0x20207044);   
#else //SCN_UI
  mt_reg_sync_write(emi_apb_base+0x00000100,0x08047250);
  mt_reg_sync_write(emi_apb_base+0x00000108,0x10107050);
  mt_reg_sync_write(emi_apb_base+0x00000110,0x0a0a50df);
  mt_reg_sync_write(emi_apb_base+0x00000118,0x0000f0d0);
  mt_reg_sync_write(emi_apb_base+0x00000120,0x08086050);
  mt_reg_sync_write(emi_apb_base+0x00000124,0x00000034);
  mt_reg_sync_write(emi_apb_base+0x00000128,0x0a0a50df);
  mt_reg_sync_write(emi_apb_base+0x00000130,0x20207048);
  mt_reg_sync_write(emi_apb_base+0x00000138,0x20207048);
#endif
////////////////////////////////////////////////////
  mt_reg_sync_write(emi_apb_base+0x00000140,0x00007108);
  mt_reg_sync_write(emi_apb_base+0x00000144,0x00007108);
  mt_reg_sync_write(emi_apb_base+0x00000150,0x00000000);
  mt_reg_sync_write(emi_apb_base+0x00000158,0xff01ff00);
  mt_reg_sync_write(emi_apb_base+0x00000400,0x00ff0001); //[27:20] enable monitor
  mt_reg_sync_write(emi_apb_base+0x0000071c,0x10000008);
  mt_reg_sync_write(emi_apb_base+0x00000800,0xffffffff);
  mt_reg_sync_write(emi_apb_base+0x00000808,0x0a0a0a0a);
  mt_reg_sync_write(emi_apb_base+0x0000080c,0x0a0a0a0a);
  mt_reg_sync_write(emi_apb_base+0x00000820,0x24240101);
  mt_reg_sync_write(emi_apb_base+0x00000824,0x01012424);
  mt_reg_sync_write(emi_apb_base+0x00000828,0x50500101);
  mt_reg_sync_write(emi_apb_base+0x0000082c,0x01015050);
  mt_reg_sync_write(emi_apb_base+0x00000830,0x0fc39a70);
  mt_reg_sync_write(emi_apb_base+0x00000834,0x05050003);
  mt_reg_sync_write(emi_apb_base+0x00000838,0x254dffff);
  mt_reg_sync_write(emi_apb_base+0x0000083c,0x32644b32);
  mt_reg_sync_write(emi_apb_base+0x00000840,0x00000000);
  mt_reg_sync_write(emi_apb_base+0x00000844,0x00000000);
  mt_reg_sync_write(emi_apb_base+0x00000848,0x00000000);
  mt_reg_sync_write(emi_apb_base+0x0000084c,0x00000000);
  mt_reg_sync_write(emi_apb_base+0x00000850,0x00000000);
  mt_reg_sync_write(emi_apb_base+0x00000854,0x00000000);
  mt_reg_sync_write(emi_apb_base+0x00000858,0x02533cff);
  mt_reg_sync_write(emi_apb_base+0x0000085c,0x00002785);
  mt_reg_sync_write(emi_apb_base+0x00000874,0x00006c98);
  mt_reg_sync_write(emi_apb_base+0x00000878,0x008b0000);
//C:-------------------END))=EMI Setting--------------------;
}//void emi_cen_config()

void emi_chn_config(unsigned cfg_base_addr)  {
  unsigned emi_chn_base = cfg_base_addr;
  unsigned int  rd_tmp;
  //C:-------------------BEGIN))=CHANNEL EMI Setting--------------------;
#ifdef RANK_512MB  // => 2channel , dual rank , total=2G
  mt_reg_sync_write(emi_chn_base+0x00000000,0x0400a051);
#else  //RANK_1G  => 2channel , dual rank , total=4G
  mt_reg_sync_write(emi_chn_base+0x00000000,0x0400f051);
#endif
  mt_reg_sync_write(emi_chn_base+0x00000008,0x00ff5048);
  mt_reg_sync_write(emi_chn_base+0x00000010,0x00000004);
  mt_reg_sync_write(emi_chn_base+0x00000018,0x99f08c03);
  mt_reg_sync_write(emi_chn_base+0x00000710,0x9a508c17); // [24:20] = 0x2 : bank throttling (default=0x01f00000)
  mt_reg_sync_write(emi_chn_base+0x00000048,0x00038037); //RD_INORDER_THR[20:16]=2
  mt_reg_sync_write(emi_chn_base+0x00000050,0x38460002); // [1] : MD_RD_AFT_WR_EN
  mt_reg_sync_write(emi_chn_base+0x00000058,0x00000000);
  mt_reg_sync_write(emi_chn_base+0x00000090,0x000002ff);
  mt_reg_sync_write(emi_chn_base+0x00000098,0x00003101);
  mt_reg_sync_write(emi_chn_base+0x00000140,0x20407188);
  mt_reg_sync_write(emi_chn_base+0x00000144,0x20407188);
  mt_reg_sync_write(emi_chn_base+0x00000148,0x0719595e);
  mt_reg_sync_write(emi_chn_base+0x0000014c,0x0719595e);
  mt_reg_sync_write(emi_chn_base+0x00000150,0x64f3fc79);
  mt_reg_sync_write(emi_chn_base+0x00000154,0x64f3fc79);
  mt_reg_sync_write(emi_chn_base+0x00000158,0x02080868);
#ifdef SCN_ICFP
  mt_reg_sync_write(emi_chn_base+0x0000015c,0x88410222); // Stop urgent read first when write command buffer remain < 8
#elif SCN_VPWFD
  mt_reg_sync_write(emi_chn_base+0x0000015c,0x88410222); // Stop urgent read first when write command buffer remain < 8
#elif SCN_VR4K
  mt_reg_sync_write(emi_chn_base+0x0000015c,0x88410222); // Stop urgent read first when write command buffer remain < 8  
#else //SCN_UI
  mt_reg_sync_write(emi_chn_base+0x0000015c,0x82410222); // Stop urgent read first when write command buffer remain < 2
#endif
  mt_reg_sync_write(emi_chn_base+0x0000016c,0x0000f801);
  mt_reg_sync_write(emi_chn_base+0x00000170,0x40000000);
  mt_reg_sync_write(emi_chn_base+0x000001b0,0x0006002f); // Rank-Aware arbitration
  mt_reg_sync_write(emi_chn_base+0x000001b4,0x09090909); // Rank-Aware arbitration
  mt_reg_sync_write(emi_chn_base+0x000001b8,0x10001820); // Rank-Aware arbitration
  mt_reg_sync_write(emi_chn_base+0x00000080,0x5a482f00);
  mt_reg_sync_write(emi_chn_base+0x00000088,0x5a482f00);
}//void emi_chn_config()

//This is for Chip, Please don't remove it.
//#define EMI_APB_BASE 0x10219000
//#define EMI_CHANNEL_APB_BASE 0x10235000
#define INFRA_DRAMC_REG_CONFIG 0x100010b4

void emi_init()
{
  //Broadcast on
  mt_reg_sync_write(INFRA_DRAMC_REG_CONFIG,0x0000007f);

  emi_cen_config(EMI_BASE);
  emi_chn_config(CHN0_EMI_BASE);

  //Broadcast off
  mt_reg_sync_write(INFRA_DRAMC_REG_CONFIG,0x00000000);
}
void emi_init2()
{
  //Broadcast on
  mt_reg_sync_write(INFRA_DRAMC_REG_CONFIG, 0x0000007f);

  mt_reg_sync_write(CHN0_EMI_BASE+0x00000010, 0x00000005);
  mt_reg_sync_write(EMI_BASE+0x00000060, 0x007815ff);

  //Broadcast off
  mt_reg_sync_write(INFRA_DRAMC_REG_CONFIG, 0x00000000);
}

void EMI_ESL_Setting1(void)
{
	emi_init();
}

void EMI_ESL_Setting2(void)
{
	emi_init2();
}


void EMI_Patch(void)
{
	//The following is EMI patch

#if !__ETT__
	// Enable MPU violation interrupt to MD for D1 and D7
	*((volatile unsigned int *)EMI_MPU_CTRL_D(1)) |= 0x10;
	*((volatile unsigned int *)EMI_MPU_CTRL_D(7)) |= 0x10;

        // DVFS threshold
        if (u1IsLP4Family(mt_get_dram_type_from_hw_trap())){
	/*LP4*/
                *((volatile unsigned int *)EMI_BWCT0) = 0x0a000705;
        } else {
                *((volatile unsigned int *)EMI_BWCT0) = 0x07000505; //total BW setting for VcoreDVFS
        }

	// EMI QoS0.5
	*((volatile unsigned int *) EMI_BWCT0_2ND) = 0x00030023; // monitor AP
	*((volatile unsigned int *) EMI_BWCT0_3RD) = 0x00ff0023; // monitor ALL
	*((volatile unsigned int *) EMI_BWCT0_4TH) = 0x00C00023; // monitor GPU
	*((volatile unsigned int *) EMI_BWCT0_5TH) = 0x00240023; // monitor MM

#ifdef LAST_EMI
	last_emi_info_ptr = (LAST_EMI_INFO_T *) get_dbg_info_base(KEY_LAST_EMI);
	last_emi_info_ptr->decs_magic = LAST_EMI_MAGIC_PATTERN;
#if CFG_LAST_EMI_BW_DUMP
	last_emi_info_ptr->decs_ctrl = 0xDECDDECD;
#else
	last_emi_info_ptr->decs_ctrl = 0xDEC8DEC8;
#endif
	last_emi_info_ptr->isu_version = 0xFFFFFFFF;
	last_emi_info_ptr->decs_dram_type = 0;
	last_emi_info_ptr->decs_diff_us = 0;
#endif
#endif
}

#if !__ETT__
void reserve_emi_mbw_buf(void)
{
	unsigned long long rsv_start;
	dram_addr_t dram_addr;

	dram_addr.ch = 0;
	dram_addr.rk = 0;
	get_dramc_addr(&dram_addr, 0x0);

	if (dram_addr.full_sys_addr > 0xFFFFFFFF)
		rsv_start = mblock_reserve_ext(&bootarg.mblock_info, 0x800000, 0x800000, 0x100000000, 0, "emi_mbw_buf");
	else
		rsv_start = mblock_reserve_ext(&bootarg.mblock_info, 0x800000, 0x800000, dram_addr.full_sys_addr, 0, "emi_mbw_buf");

#ifdef LAST_EMI
	last_emi_info_ptr->mbw_buf_l = (unsigned int) (rsv_start & 0xFFFFFFFF);
	last_emi_info_ptr->mbw_buf_h = (unsigned int) (rsv_start >> 32);
#endif
}
#endif

static void EMI_rank_swap_emi_setting(EMI_SETTINGS *emi_set)
{
	static unsigned int temp;

	if (emi_set->EMI_CONA_VAL & 0x20000) {
		temp = emi_set->EMI_CONA_VAL;
		emi_set->EMI_CONA_VAL &= ~(0xF3F0F0F0);
		emi_set->EMI_CONA_VAL |= (temp & 0xC0C0C0C0) >> 2;
		emi_set->EMI_CONA_VAL |= (temp & 0x30303030) << 2;
		emi_set->EMI_CONA_VAL |= (temp & 0x02000000) >> 1;
		emi_set->EMI_CONA_VAL |= (temp & 0x01000000) << 1;

		temp = emi_set->EMI_CONH_VAL;
		emi_set->EMI_CONH_VAL &= ~(0xFFFF0030);
		emi_set->EMI_CONH_VAL |= (temp & 0xF0F00000) >> 4;
		emi_set->EMI_CONH_VAL |= (temp & 0x0F0F0000) << 4;
		emi_set->EMI_CONH_VAL |= (temp & 0x00000020) >> 1;
		emi_set->EMI_CONH_VAL |= (temp & 0x00000010) << 1;

		temp = emi_set->CHN0_EMI_CONA_VAL;
		emi_set->CHN0_EMI_CONA_VAL &= ~(0x00FFF0FC);
		emi_set->CHN0_EMI_CONA_VAL |= (temp & 0x00F00000) >> 4;
		emi_set->CHN0_EMI_CONA_VAL |= (temp & 0x000F0000) << 4;
		emi_set->CHN0_EMI_CONA_VAL |= (temp & 0x0000C0C0) >> 2;
		emi_set->CHN0_EMI_CONA_VAL |= (temp & 0x00003030) << 2;
		emi_set->CHN0_EMI_CONA_VAL |= (temp & 0x00000008) >> 1;
		emi_set->CHN0_EMI_CONA_VAL |= (temp & 0x00000004) << 1;

		temp = emi_set->CHN1_EMI_CONA_VAL;
		emi_set->CHN1_EMI_CONA_VAL &= ~(0x00FFF0FC);
		emi_set->CHN1_EMI_CONA_VAL |= (temp & 0x00F00000) >> 4;
		emi_set->CHN1_EMI_CONA_VAL |= (temp & 0x000F0000) << 4;
		emi_set->CHN1_EMI_CONA_VAL |= (temp & 0x0000C0C0) >> 2;
		emi_set->CHN1_EMI_CONA_VAL |= (temp & 0x00003030) << 2;
		emi_set->CHN1_EMI_CONA_VAL |= (temp & 0x00000008) >> 1;
		emi_set->CHN1_EMI_CONA_VAL |= (temp & 0x00000004) << 1;

		emi_set->CHN0_EMI_CONA_VAL |= 0x80000000;
		emi_set->CHN1_EMI_CONA_VAL |= 0x80000000;

		temp = emi_set->DRAM_RANK_SIZE[0];
		emi_set->DRAM_RANK_SIZE[0] = emi_set->DRAM_RANK_SIZE[1];
		emi_set->DRAM_RANK_SIZE[1] = temp;

		if (emi_set->dram_cbt_mode_extern == CBT_R0_NORMAL_R1_BYTE)
			emi_set->dram_cbt_mode_extern = CBT_R0_BYTE_R1_NORMAL;
		else if (emi_set->dram_cbt_mode_extern == CBT_R0_BYTE_R1_NORMAL)
			emi_set->dram_cbt_mode_extern = CBT_R0_NORMAL_R1_BYTE;
	}
}

static void EMI_rank_swap_handle(void)
{
	static unsigned int handled = 0;
	int i;

	if (!handled) {
		EMI_rank_swap_emi_setting(&default_emi_setting);

		for (i = 0 ; i < num_of_emi_records; i++)
			EMI_rank_swap_emi_setting(&emi_settings[i]);

		handled = 1;
	}
}

void EMI_set_rank_swap(unsigned int enable)
{
	rank_swap = (enable) ? 1 : 0;
}

void EMI_Init(DRAMC_CTX_T *p)
{
	EMI_SETTINGS *emi_set;

	emi_set = get_emi_setting();

	if (rank_swap)
		EMI_rank_swap_handle();

	EMI_ESL_Setting1(); //Copy Paste from DE

	//The following is MDL settings
	*(volatile unsigned *)EMI_CONA = emi_set->EMI_CONA_VAL;
	*(volatile unsigned *)EMI_CONH &= ~0xFFFF0030;
	*(volatile unsigned *)EMI_CONH |= (emi_set->EMI_CONH_VAL & 0xFFFF0030);

	// CHNA and CHNB uses the same CH0 setting
	*(volatile unsigned *)CHN_EMI_CONA(CHN0_EMI_BASE) = emi_set->CHN0_EMI_CONA_VAL;
	*(volatile unsigned *)CHN_EMI_CONA(CHN1_EMI_BASE) = emi_set->CHN1_EMI_CONA_VAL;

//#if CHANNEL_NUM == 1
//	*(volatile unsigned *)EMI_CONF = 0x0;
//#else
	*(volatile unsigned *)EMI_CONF = emi_set->EMI_CONF_VAL;
	if (u1IsLP4Family(p->dram_type))
	{
		*(volatile unsigned *)CHN_EMI_CONC(CHN0_EMI_BASE) = 0x0;
		*(volatile unsigned *)CHN_EMI_CONC(CHN1_EMI_BASE) = 0x0;
	} else {
		*(volatile unsigned *)CHN_EMI_CONC(CHN0_EMI_BASE) = emi_set->EMI_CONF_VAL;
		*(volatile unsigned *)CHN_EMI_CONC(CHN1_EMI_BASE) = emi_set->EMI_CONF_VAL;
	}
//#endif

#if 0
#if __ETT__ && !defined(__LPAE__)
	/*
	 * special emi mapping w/o LPAE support
	 * RANK0_MAX_SIZE_ETT = 0x40000000 --> RANK0 @0x4000_0000~0x7fff_ffff
	 * RANK1_MAX_SIZE_ETT = 0x40000000 --> RANK1 @0x8000_0000~0xbfff_ffff
	 */
	/* set all the rank size of all the channels to the minimum value */
	*(volatile unsigned int*)EMI_CONH = ((*(volatile unsigned int*)EMI_CONH & 0x0000ffff) | (0x11110000));

	/* set all the rank size of all channel-0 to the minimum value */
	*(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_BASE)
		= ((*(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_BASE) & 0xff00ffff ) | (0x00110000));

	/* set all the rank size of all channel-1 to the minimum value */
	*(volatile unsigned int*)CHN_EMI_CONA(CHN1_EMI_BASE)
		= ((*(volatile unsigned int*)CHN_EMI_CONA(CHN1_EMI_BASE) & 0xff00ffff ) | (0x00110000));

#endif
#endif

	dsb();
}

void EMI_Init2(void)
{
	EMI_ESL_Setting2(); //Copy Paste from DE

	EMI_Patch(); //Please add the EMI patch here
}

static unsigned int emi_conh = 0;
static unsigned int chn_emi_cona[2] = {0, 0};

/* return the start address of rank1 */
unsigned int set_emi_before_rank1_mem_test(void)
{
	unsigned int emi_cona;

	emi_cona = *(volatile unsigned int*)EMI_CONA;
	emi_conh = *(volatile unsigned int*)EMI_CONH;
	chn_emi_cona[0] = *(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_BASE);
	chn_emi_cona[1] = *(volatile unsigned int*)CHN_EMI_CONA(CHN1_EMI_BASE);

	if (emi_cona & 0x100) {
		/* set all the rank size of all the channels to the minimum value */
		*(volatile unsigned int*)EMI_CONH = ((*(volatile unsigned int*)EMI_CONH & 0x0000ffff) | (0x22220000));

		/* set all the rank size of all channel-0 to the minimum value */
		*(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_BASE)
			= ((*(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_BASE) & 0xff00ffff ) | (0x00220000));

		/* set all the rank size of all channel-1 to the minimum value */
		*(volatile unsigned int*)CHN_EMI_CONA(CHN1_EMI_BASE)
			= ((*(volatile unsigned int*)CHN_EMI_CONA(CHN1_EMI_BASE) & 0xff00ffff ) | (0x00220000));
	} else {
		/* set all the rank size of all the channels to the minimum value */
		*(volatile unsigned int*)EMI_CONH = ((*(volatile unsigned int*)EMI_CONH & 0x0000ffff) | (0x44440000));

		/* set all the rank size of all channel-0 to the minimum value */
		*(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_BASE)
			= ((*(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_BASE) & 0xff00ffff ) | (0x00440000));

		/* set all the rank size of all channel-1 to the minimum value */
		*(volatile unsigned int*)CHN_EMI_CONA(CHN1_EMI_BASE)
			= ((*(volatile unsigned int*)CHN_EMI_CONA(CHN1_EMI_BASE) & 0xff00ffff ) | (0x00440000));
	}

	dsb();
	return 0x40000000;
}

void restore_emi_after_rank1_mem_test(void)
{
	*(volatile unsigned int*)EMI_CONH = emi_conh;
	*(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_BASE) = chn_emi_cona[0];
	*(volatile unsigned int*)CHN_EMI_CONA(CHN1_EMI_BASE) = chn_emi_cona[1];
	dsb();
}

unsigned int check_gating_error(void)
{
	unsigned int ret = 0, i, phy_base, err_code = 0;
	unsigned int misc_stberr_rk0_r, misc_stberr_rk0_f, misc_stberr_rk1_r, misc_stberr_rk1_f;

	phy_base = Channel_A_PHY_AO_BASE_ADDRESS;

	for (i = 0; i < CHANNEL_NUM; ++i, phy_base += 0x8000, err_code = 0) {
		misc_stberr_rk0_r = *(volatile unsigned int*)(phy_base + 0x510);
		misc_stberr_rk0_f = *(volatile unsigned int*)(phy_base + 0x514);
		misc_stberr_rk1_r = *(volatile unsigned int*)(phy_base + 0x518);
		misc_stberr_rk1_f = *(volatile unsigned int*)(phy_base + 0x51c);
		if (misc_stberr_rk0_r & (1 << 16)) {
			ret |= (1 << i);
#ifdef LAST_DRAMC
			if ((misc_stberr_rk0_r & 0xffff) != 0) {
				err_code |= ERR_DRAM_GATING_RK0_R;
			}
			if ((misc_stberr_rk0_f & 0xffff) != 0) {
				err_code |= ERR_DRAM_GATING_RK0_F;
			}
			if ((misc_stberr_rk1_r & 0xffff) != 0) {
				err_code |= ERR_DRAM_GATING_RK1_R;
			}
			if ((misc_stberr_rk1_f & 0xffff) != 0) {
				err_code |= ERR_DRAM_GATING_RK1_F;
			}
			dram_fatal_set_gating_err(i, err_code);
			dram_fatal_set_stberr(i, 0, (misc_stberr_rk0_r & 0xffff) | ((misc_stberr_rk0_f & 0xffff) << 16));
			dram_fatal_set_stberr(i, 1, (misc_stberr_rk1_r & 0xffff) | ((misc_stberr_rk1_f & 0xffff) << 16));
		} else {
			dram_fatal_set_gating_err(i, 0);
			dram_fatal_set_stberr(i, 0, 0);
			dram_fatal_set_stberr(i, 1, 0);
#endif
		}
	}

	return ret;
}

#if 0
void CHA_HWGW_Print(DRAMC_CTX_T *p)
{
    U8 u1RefreshRate;
    U32 backup_channel, chIdx;

    backup_channel = p->channel;

#if (FOR_DV_SIMULATION_USED==0)
    // Read HW gating tracking
#ifdef HW_GATING
    for(chIdx=0; chIdx<p->support_channel_num; chIdx++)
    {
        DramcPrintHWGatingStatus(p, chIdx);
    }
#endif

#if ENABLE_RX_TRACKING_LP4
    for(chIdx=0; chIdx<p->support_channel_num; chIdx++)
    {
        DramcPrintRXDQDQSStatus(p, chIdx);
    }
#endif

#ifdef IMPEDANCE_TRACKING_ENABLE
        if(u1IsLP4Family(p->dram_type))
        {
            DramcPrintIMPTrackingStatus(p, CHANNEL_A);
//CH_B HW Fail, can't use            DramcPrintIMPTrackingStatus(p, CHANNEL_B);
        }
#endif

#ifdef TEMP_SENSOR_ENABLE
        for(chIdx=0; chIdx<p->support_channel_num; chIdx++)
        {
            u1RefreshRate = u1GetMR4RefreshRate(p, chIdx);
            mcSHOW_ERR_MSG(("[CH%d] MRR(MR4) [10:8]=%x\n", chIdx, u1RefreshRate));
        }
#endif
#endif

    vSetPHY2ChannelMapping(p, backup_channel);
}
#endif

void Dump_EMIRegisters(DRAMC_CTX_T *p)
{
#ifndef OLYMPUS_TO_BE_PORTING

  U8 ucstatus = 0;
  U32 uiAddr;
  U32 u4value;

  for (uiAddr=0; uiAddr<0x160; uiAddr+=4)
  {
    mcSHOW_DBG_MSG(("EMI offset:%x, value:%x\n", uiAddr, *(volatile unsigned *)(EMI_APB_BASE+uiAddr)));
  }
#endif
}

void print_DBG_info(DRAMC_CTX_T *p)
{
#ifndef OLYMPUS_TO_BE_PORTING

    unsigned int addr = 0x0;
    U32 u4value;

#ifdef DDR_INIT_TIME_PROFILING
    return;
#endif

    mcSHOW_DBG_MSG(("EMI_CONA=%x\n",*(volatile unsigned *)(EMI_APB_BASE+0x00000000)));
    mcSHOW_DBG_MSG(("EMI_CONH=%x\n",*(volatile unsigned *)(EMI_APB_BASE+0x00000038)));

    //RISCReadAll();
#endif
}

int mt_get_dram_type(void)
{
#if (fcFOR_CHIP_ID == fcTalbot)
    return (*((volatile unsigned *)(Channel_A_DRAMC_AO_BASE_ADDRESS+0x10)) >> 10) & 0x7;
#else
    #error No defined mt_get_dram_type for your chip !!!
#endif
}

int mt_get_freq_setting(DRAMC_CTX_T *p)
{
    return p->frequency;
}

#ifdef DDR_RESERVE_MODE
extern u32 g_ddr_reserve_enable;
extern u32 g_ddr_reserve_success;
#define TIMEOUT 3
extern void before_Dramc_DDR_Reserved_Mode_setting(void);
#endif

#ifdef DDR_RESERVE_MODE

#define	CHAN_DRAMC_NAO_MISC_STATUSA(base)	(base + 0x80)
#define SREF_STATE				(1 << 16)

static unsigned int is_dramc_exit_slf(void)
{
	unsigned int ret;
	U32 u4DramType = 0;

	u4DramType = (*((volatile unsigned int *)(0x1022A010)) >> 10 & 0x7);
	ret = *(volatile unsigned *)CHAN_DRAMC_NAO_MISC_STATUSA(Channel_A_DRAMC_NAO_BASE_ADDRESS);
	if ((ret & SREF_STATE) != 0) {
		dramc_crit("DRAM CHAN-A is in self-refresh (MISC_STATUSA = 0x%x)\n", ret);
		return 0;
	}

if(u4DramType >=2) //LP3:1, LP4:2, LP4X:3
{
	ret = *(volatile unsigned *)CHAN_DRAMC_NAO_MISC_STATUSA(Channel_B_DRAMC_NAO_BASE_ADDRESS);
	if ((ret & SREF_STATE) != 0) {
		dramc_crit("DRAM CHAN-B is in self-refresh (MISC_STATUSA = 0x%x)\n", ret);
		return 0;
	}
}
	dramc_crit("ALL DRAM CHAN is not in self-refresh\n");
	return 1;
}

#endif

unsigned int dramc_set_vcore_voltage(unsigned int vcore)
{
#ifdef MTK_PMIC_CHIP_MT6358
	return mtk_regulator_set_voltage(&reg_vcore, vcore, MAX_VCORE);
#endif
	return 0;
}

unsigned int dramc_get_vcore_voltage(void)
{
#ifdef MTK_PMIC_CHIP_MT6358
	return mtk_regulator_get_voltage(&reg_vcore);
#else
	return 0;
#endif
}

unsigned int dramc_set_vdd1_voltage(unsigned int ddr_type, unsigned int vdd1)
{
	unsigned int tmp = 0;

#if 0
	if (u1IsLP4Family(ddr_type))
		return 0;
#endif

	if (vdd1) {
		if (vdd1 > STD_VIO18) {
			tmp = vdd1 - STD_VIO18;
			if (tmp > 80000) {
				tmp = tmp % 100000;
				pmic_config_interface( PMIC_RG_VIO18_VOCAL_ADDR, (vdd1 - tmp - STD_VIO18) / 10000, PMIC_RG_VIO18_VOCAL_MASK, PMIC_RG_VIO18_VOCAL_SHIFT);
			}
			mtk_regulator_set_votrim(&reg_vio18, tmp);
		} else if (vdd1 < STD_VIO18) {
			mtk_regulator_set_votrim(&reg_vio18, vdd1 - STD_VIO18);
		}
	}

	return 0;
}

unsigned int dramc_get_vdd1_voltage(void)
{
	unsigned int vio18_vocal=0;
	unsigned int vio18_votrim=0;
	unsigned int vio18=0;


	vio18_vocal  = mtk_regulator_get_voltage(&reg_vio18);
	vio18_votrim = mtk_regulator_get_votrim(&reg_vio18);
	vio18 = vio18_vocal + vio18_votrim;

	return vio18;
}

static unsigned int lp3_vdram_base[] = { 1100000, 1200000 };
static unsigned int lp3_vdram_ofs[] = { 0, 0 } ;//20000, 40000 };

unsigned int dramc_set_vdd2_voltage(unsigned int ddr_type, unsigned int vdd2)
{
	unsigned int ret = 0;

	ret = mtk_regulator_set_voltage(&reg_vdram, vdd2, MAX_VDRAM);
	return 0;
}

unsigned int dramc_get_vdd2_voltage(void)
{
	unsigned int ret = 0;

	ret = mtk_regulator_get_voltage(&reg_vdram);

	return ret;
}

unsigned int dramc_set_vddq_voltage(unsigned int ddr_type, unsigned int vddq)
{
	if (ddr_type == TYPE_LPDDR4X)
	{
	//	mtk_regulator_set_voltage(&reg_vddq, STD_VDDQ, MAX_VDDQ);
		mtk_regulator_set_votrim(&reg_vddq, vddq - STD_VDDQ);
	}

	return 0;
}

unsigned int dramc_get_vddq_voltage(unsigned int ddr_type)
{
	unsigned int sum = 0;

	if (ddr_type == TYPE_LPDDR4)
		sum = dramc_get_vdd2_voltage();
	else
		sum = mtk_regulator_get_voltage(&reg_vddq) + mtk_regulator_get_votrim(&reg_vddq);


	return sum;
}

/* cc move here to use reg_vcore */
#if ENABLE_DVS
void SetDVSProgram(DRAMC_CTX_T *p)
{
    unsigned int vio18, vcore, vdram, vddq;
    int ret;

#if defined(DRAM_HQA) && defined(__ETT__)
	hqa_set_voltage_by_freq(p, &vio18, &vcore, &vdram, &vddq);
#if USE_PMIC_CHIP_MT6358
	ret = mtk_regulator_get("vcore", &reg_vcore);
	if (ret)
		mcSHOW_DBG_MSG(("mtk_regulator_get vcore fail\n"));

	if (vcore)
	{
		mtk_regulator_set_voltage(&reg_vcore, vcore, MAX_VCORE);
		mcSHOW_DBG_MSG(("set to Vcore = %d\n", vcore));
	}
	mcSHOW_DBG_MSG(("SetDVSProgram Vcore = %d\n", mtk_regulator_get_voltage(&reg_vcore)));
#endif
#endif
}
#endif

unsigned int mt_get_dram_type_from_hw_trap(void)
{
	static unsigned int ddr_type_detected = 0;
	static unsigned int pmic_trap_ddr_type = TYPE_LPDDR4X;

	if (!ddr_type_detected) {
	#if ( !__ETT__ && (FOR_DV_SIMULATION_USED==0))
	EMI_SETTINGS *emi_set;
	emi_set = &default_emi_setting;
	emi_set->PIN_MUX_TYPE = 0;
	#endif
	#ifdef MTK_PMIC_CHIP_MT6358
		unsigned int hw_trap;
		hw_trap = get_dram_type();

		switch (hw_trap) {
		case 0:
			return TYPE_LPDDR4X;
		case 1:
			return TYPE_LPDDR4X;
		case 2:
			#if ( !__ETT__ && (FOR_DV_SIMULATION_USED==0))
			if (mt_get_gpio_in(GPIO_110) == 1){
				emi_set->PIN_MUX_TYPE = 1;
				dramc_crit("pin mux type is 1 \n");
			}else{	
				dramc_crit("pin mux type is 0 \n");
			}
			#endif
			return TYPE_LPDDR3;
		case 3:
			return TYPE_LPDDR4X;
		default:
			printf("[dramc] Wrong HW TRAP\n");
			ASSERT(0);
			break;
		}

		dramc_debug("PMIC TRAP GET DDR TYPE: 0x%x\n", pmic_trap_ddr_type);
	#endif
		ddr_type_detected = 1;
	}

	return pmic_trap_ddr_type;
}

void setup_dramc_voltage_by_pmic(void)
{
#if (FOR_DV_SIMULATION_USED==0)
#ifdef MTK_PMIC_CHIP_MT6358
	unsigned int ddr_type = mt_get_dram_type_from_hw_trap();
	int ret=0;

	ret = mtk_regulator_get("vcore", &reg_vcore);
	if (ret)
		dramc_crit("mtk_regulator_get vcore fail\n");

	ret = mtk_regulator_get("vdram1", &reg_vdram);
	if (ret)
		dramc_crit("mtk_regulator_get vdram fail\n");

	if (ddr_type == TYPE_LPDDR4X) {
		ret = mtk_regulator_get("vdram2", &reg_vddq);
		if (ret)
			dramc_crit("mtk_regulator_get vddq fail\n");
	}
	ret = mtk_regulator_get("vio18", &reg_vio18);
	if (ret)
		 dramc_crit("mtk_regulator_get vio18 fail\n");
	mtk_regulator_set_mode(&reg_vcore, 0x1);
	mtk_regulator_set_mode(&reg_vdram, 0x1);

	
	if (ddr_type == TYPE_LPDDR3) {
		#ifdef VOLTAGE_SEL
		dramc_set_vcore_voltage(vcore_voltage_select(TYPE_LPDDR3, KOPP0));
		dramc_set_vdd2_voltage(TYPE_LPDDR3,vdram_voltage_select(TYPE_LPDDR3));
		#else
		dramc_set_vcore_voltage(SEL_PREFIX_VCORE(LP3, KOPP0));
		dramc_set_vdd2_voltage(ddr_type, SEL_PREFIX_VDRAM(LP3));
		#endif
	} else {
		/* LPDDR4 */
		mtk_regulator_set_mode(&reg_vddq, 0x1);
		#ifdef VOLTAGE_SEL
		dramc_set_vcore_voltage(vcore_voltage_select(TYPE_LPDDR4, KOPP0));
		dramc_set_vdd2_voltage(TYPE_LPDDR4,vdram_voltage_select(TYPE_LPDDR4));
		dramc_set_vddq_voltage(TYPE_LPDDR4X, vddq_voltage_select());
		#else
		dramc_set_vcore_voltage(SEL_PREFIX_VCORE(LP4, KOPP0));
		dramc_set_vdd2_voltage(ddr_type, SEL_PREFIX_VDRAM(LP4));
		dramc_set_vddq_voltage(ddr_type, SEL_PREFIX_VDDQ);
		#endif
	}
	#ifdef VOLTAGE_SEL
	dramc_set_vdd1_voltage(0,vio18_voltage_select());
	#endif
	
	dramc_crit("Vcore = %d\n", dramc_get_vcore_voltage());
	dramc_crit("Vdram = %d\n", dramc_get_vdd2_voltage());

	if (ddr_type == TYPE_LPDDR4X)
		dramc_crit("Vddq = %d\n", dramc_get_vddq_voltage(ddr_type));

#endif
#endif
}

static void restore_vcore_setting(void)
{
#if (FOR_DV_SIMULATION_USED==0)
#ifdef MTK_PMIC_CHIP_MT6358
	int ret = 0;
	#ifdef VOLTAGE_SEL
	int vcore = 0;
	#endif
	ret = mtk_regulator_get("vcore", &reg_vcore);
	if (ret)
		dramc_crit("mtk_regulator_get vcore fail\n");

	#ifdef VOLTAGE_SEL
	if (u1IsLP4Family(mt_get_dram_type_from_hw_trap()))
	vcore = vcore_voltage_select(TYPE_LPDDR4, KOPP0);
	else
	vcore = vcore_voltage_select(TYPE_LPDDR3, KOPP0);

	if (doe_get_config("dram_fix_2400_0800") || doe_get_config("all_shu_2400_0800")
		|| doe_get_config("dram_dvs_fix_1534"))
	    dramc_set_vcore_voltage(800000);
	else if (doe_get_config("dram_fix_1534_0700") || doe_get_config("dram_fix_2400_0700")
				|| doe_get_config("all_shu_2400_0700") || doe_get_config("all_shu_1534_0700"))
	    dramc_set_vcore_voltage(700000);
	else if ((doe_get_config("dram_fix_1534_0625")))
	    dramc_set_vcore_voltage(625000);
	else if (vcore)
	    dramc_set_vcore_voltage(vcore);
	else
	#endif
 	{	
	if (u1IsLP4Family(mt_get_dram_type_from_hw_trap()))
		dramc_set_vcore_voltage(SEL_PREFIX_VCORE(LP4, KOPP0));
	else
		dramc_set_vcore_voltage(SEL_PREFIX_VCORE(LP3, KOPP0));
	}
	dramc_crit("Vcore = %d\n", dramc_get_vcore_voltage());
#endif
#endif
}

static void restore_pmic_setting(void)
{
#if (FOR_DV_SIMULATION_USED==0)
#ifdef MTK_PMIC_CHIP_MT6358
	unsigned int ddr_type = mt_get_dram_type_from_hw_trap();
	int ret = 0 ;

	restore_vcore_setting();


	ret = mtk_regulator_get("vdram1", &reg_vdram);
	if (ret) {
		dramc_crit("mtk_regulator_get vdram fail\n");
		return;
	}


	if (u1IsLP4Family(ddr_type)) {
		/* LPDDR4 */

		ret = mtk_regulator_get("vdram2", &reg_vddq);
		if (ret){
			dramc_crit("mtk_regulator_get vdram failed\n");
			return;
		}
		dramc_set_vdd2_voltage(ddr_type,SEL_PREFIX_VDRAM(LP4));
		dramc_set_vddq_voltage(ddr_type,SEL_PREFIX_VDDQ);
	}else
		dramc_set_vdd2_voltage(ddr_type, SEL_PREFIX_VDRAM(LP3));

	dramc_crit("Vdram = %d\n", dramc_get_vdd2_voltage());
	if (ddr_type == TYPE_LPDDR4X)
		dramc_crit("Vddq = %d\n", dramc_get_vddq_voltage(ddr_type));
#endif
#endif
}

void switch_dramc_voltage_to_auto_mode(void)
{
#if (FOR_DV_SIMULATION_USED==0)
#ifdef MTK_PMIC_CHIP_MT6358
    if (doe_get_config("vcore_fix_mode") == 1)
        mtk_regulator_set_mode(&reg_vcore, 0x1);
    else
        mtk_regulator_set_mode(&reg_vcore, 0x0);

    if (doe_get_config("vdd2_fix_mode") == 1)
        mtk_regulator_set_mode(&reg_vdram, 0x1);
    else
        mtk_regulator_set_mode(&reg_vdram, 0x0);

	if (pmic_trap_ddr_type == TYPE_LPDDR4X)
		mtk_regulator_set_mode(&reg_vddq, 0x0);
#endif
#endif
}

void release_dram(void)
{
#ifdef DDR_RESERVE_MODE
    int i;
    int counter = TIMEOUT;

    // scy: restore pmic setting (VCORE, VDRAM, VSRAM, VDDQ)
    restore_pmic_setting();
    rgu_release_rg_dramc_conf_iso();//Release DRAMC/PHY conf ISO
    Dramc_DDR_Reserved_Mode_setting();
    rgu_release_rg_dramc_iso();//Release PHY IO ISO
    rgu_release_rg_dramc_sref();//Let DRAM Leave SR

    // setup for EMI: touch center EMI and channel EMI to enable CLK
    dramc_crit("[DDR reserve] EMI CONA: %x\n", *(volatile unsigned int*)EMI_CONA);
    dramc_crit("[DDR reserve] EMI CHA CONA: %x\n", *(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_BASE));
    dramc_crit("[DDR reserve] EMI CHB CONA: %x\n", *(volatile unsigned int*)CHN_EMI_CONA(CHN1_EMI_BASE));
    for (i=0;i<10;i++);

    while(counter)
    {
        if(is_dramc_exit_slf() == 1) /* expect to exit dram-self-refresh */
            break;
        counter--;
    }

    if(counter == 0)
    {
        if(g_ddr_reserve_enable==1 && g_ddr_reserve_success==1)
        {
            dramc_crit("[DDR Reserve] release dram from self-refresh FAIL!\n");
            g_ddr_reserve_success = 0;
        }
    }
    else
    {
         dramc_crit("[DDR Reserve] release dram from self-refresh PASS!\n");
    }
    Dramc_DDR_Reserved_Mode_AfterSR();
    //Expect to Use LPDDR3200 and PHYPLL as output, so no need to handle
    //shuffle status since the status will be reset by system reset
    //There is an PLLL_SHU_GP in SPM which will reset by system reset
    return;
#endif
}

void check_ddr_reserve_status(void)
{
	/* get status of DCS and DVFSRC */
    int dcs_success = rgu_is_emi_dcs_success(), dvfsrc_success = rgu_is_dvfsrc_success();
    int dcs_en = rgu_is_emi_dcs_enable(), dvfsrc_en = rgu_is_dvfsrc_enable();
    /* EMI SPM workaround for Bianco only: toggle mask */
    *(volatile unsigned int*) 0x1022D3FC &= ~0x1;
    *(volatile unsigned int*) 0x102353FC &= ~0x1;

#ifdef DDR_RESERVE_MODE
    int counter = TIMEOUT;
    if(rgu_is_reserve_ddr_enabled())
    {
      g_ddr_reserve_enable = 1;
#ifdef LAST_DRAMC
      dram_fatal_set_ddr_rsv_mode_flow();
#endif

      if(rgu_is_reserve_ddr_mode_success())
      {
        while(counter)
        {
          if(rgu_is_dram_slf())
          {
            g_ddr_reserve_success = 1;
            break;
          }
          counter--;
        }
        if(counter == 0)
        {
          dramc_crit("[DDR Reserve] ddr reserve mode success but DRAM not in self-refresh!\n");
          g_ddr_reserve_success = 0;
#ifdef LAST_DRAMC
	  dram_fatal_set_ddr_rsv_mode_err();
#endif
        }
      }
      else
      {
        dramc_crit("[DDR Reserve] ddr reserve mode FAIL!\n");
        g_ddr_reserve_success = 0;
#ifdef LAST_DRAMC
	  dram_fatal_set_ddr_rsv_mode_err();
#endif
      }
	/* Disable DDR-reserve mode in pre-loader stage then enable it again in kernel stage */
	//rgu_dram_reserved(0);

	/* overwrite g_ddr_reserve_success if some of dcs/dvfsrc/drs failed */
	/* TODO: check DRS status */
	if ((dcs_en == 1 && dcs_success == 0) || (dvfsrc_en == 1 && dvfsrc_success == 0)) {
		dramc_crit("[DDR Reserve] DRAM content might be corrupted -> clear g_ddr_reserve_success\n");
		g_ddr_reserve_success = 0;

		if (dvfsrc_en == 1 && dvfsrc_success == 0) {
			dramc_crit("[DDR Reserve] DVFSRC fail!\n");
#if 0//def LAST_DRAMC
			dram_fatal_set_dvfsrc_err();
#endif
		}

		if (dcs_en == 1 && dcs_success == 0) {
			dramc_crit("[DDR Reserve] DCS fail!\n");
#if 0 //def LAST_DRAMC
			dram_fatal_set_emi_dcs_err();
#endif
		}
	} else {
		dramc_crit("[DDR Reserve] DCS/DVFSRC success! (dcs_en=%d, dvfsrc_en=%d)\n", dcs_en, dvfsrc_en);
	}
	/* release dram, no matter success or failed */
	release_dram();
    }
    else
    {
      dramc_crit("[DDR Reserve] ddr reserve mode not be enabled yet\n");
      g_ddr_reserve_enable = 0;
    }
#endif
}

unsigned int DRAM_MRR(int MRR_num)
{
    u16 MRR_value = 0x0;
    DRAMC_CTX_T *p = psCurrDramCtx;

    DramcModeRegRead(p, MRR_num, &MRR_value);
    return MRR_value;
}

static int mt_get_dram_type_for_dis(void)
{
    return mt_get_dram_type_from_hw_trap();
}

#if defined(SLT) && (!__ETT__)
#include <pl_version.h>
static u64 part_dram_data_addr_slt = 0;
#endif

#ifdef COMBO_MCP
static char id[22];
static int emmc_nand_id_len=16;
static int fw_id_len;
static int mt_get_mdl_number(void)
{
    static int found = 0;
    static int mdl_number = -1;
    int i;
    int j;
    int has_emmc_nand = 0;
    int discrete_dram_num = 0;
    int mcp_dram_num = 0;
    u64 rank0_size=0, rank1_size=0;

    unsigned int dram_type;
    DRAM_INFO_BY_MRR_T DramInfo;
    DRAM_DRAM_TYPE_T Dis_DramType;
    DRAM_CBT_MODE_EXTERN_T DramMode;

    if (!(found)) {
        int result=0;

        for (i = 0 ; i < num_of_emi_records; i++) {
            if ((emi_settings[i].type & 0x0F00) == 0x0000)
                discrete_dram_num ++;
            else
                mcp_dram_num ++;
        }

        /*If the number >=2  &&
         * one of them is discrete DRAM
         * enable combo discrete dram parse flow
         * */
        if ((discrete_dram_num > 0) && (num_of_emi_records >= 2))
            enable_combo_dis = 1;

        dramc_crit("[EMI] mcp_dram_num:%d,discrete_dram_num:%d,enable_combo_dis:%d\r\n",mcp_dram_num,discrete_dram_num,enable_combo_dis);

        Dis_DramType = mt_get_dram_type_for_dis();
#if DUAL_FREQ_K || __FLASH_TOOL_DA__
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if (!u1IsLP4Family(Dis_DramType) || read_offline_dram_mdl_data(&DramInfo)<0) {
#endif
        if (u1IsLP4Family(Dis_DramType))
            DramMode = CBT_BYTE_MODE1;
        else
            DramMode = CBT_NORMAL_MODE;

        Init_DRAM(Dis_DramType, DramMode, &DramInfo, GET_MDL_USED);
        DramInfo.u2MR5VendorID &= 0xFF;

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if (u1IsLP4Family(Dis_DramType))
            write_offline_dram_mdl_data(&DramInfo);
        }
#endif

        rank0_size = (u64) DramInfo.u8MR8RankSize[0]; //now only K CHA to save time
        rank1_size = (u64) DramInfo.u8MR8RankSize[1]; //now only K CHA to save time
#endif
        /*
         *
         * 0. if there is only one discrete dram, use index=0 emi setting and boot it.
         * */
        if ((0 == mcp_dram_num) && (1 == discrete_dram_num)) {
            mdl_number = 0;
            found = 1;

            //K first frequency (1066MHz)

            return mdl_number;
        }

        /* 1.
         * if there is MCP dram in the list, we try to find emi setting by emmc ID
         * */
        if (mcp_dram_num > 0) {
            result = platform_get_mcp_id(id, emmc_nand_id_len,&fw_id_len);
            for (i = 0; i < num_of_emi_records; i++) {
                if ((emi_settings[i].type & 0xF) != Dis_DramType)
                    continue;

                if (emi_settings[i].type != 0) {
                    if ((emi_settings[i].type & 0xF00) != 0x000) {
                        /* valid ID */
                        if (result == 0
#if DUAL_FREQ_K || __FLASH_TOOL_DA__
                                && (emi_settings[i].DRAM_RANK_SIZE[0] == rank0_size) &&
                                (emi_settings[i].DRAM_RANK_SIZE[1] == rank1_size)
#endif
                           ) {
                            if ((emi_settings[i].type & 0xF00) == 0x200) {
                                /* eMMC */
                                if (memcmp(id, emi_settings[i].ID, emi_settings[i].id_length) == 0) {
                                    mdl_number = i;
                                    found = 1;
                                    break;
                                } else {
                                    dramc_crit("[MDL] index(%d) emmc id match failed\n",i);
                                }
                            }
                            else if ((emi_settings[i].type & 0xF00) == 0x300)
                            {
                                /* UFS */
                                if (memcmp(id, emi_settings[i].ID, emi_settings[i].id_length) == 0){
                                    mdl_number = i;
                                    found = 1;
                                    break;
                                } else {
                                    dramc_crit("[MDL] index(%d) ufs id match failed\n",i);
                                }
                            }
                        }
                    }
                }
            }
        }

        /* 2. find emi setting by MODE register 5
         * */
        // if we have found the index from by eMMC ID checking, we can boot android by the setting
        // if not, we try by vendor ID
        if ((0 == found) && (1 == enable_combo_dis))
        {
            EMI_SETTINGS *emi_set;

            // try to find discrete dram by MR5 (vendor ID)
            for (i = 0; i < num_of_emi_records; i++) {
                dramc_crit("[MDL]index:%d, MR5:%x, type:%x, vender_id:%x\n", i, emi_settings[i].iLPDDR3_MODE_REG_5, emi_settings[i].type, DramInfo.u2MR5VendorID);
                // only check discrete dram type
                if ((emi_settings[i].type & 0xF) == Dis_DramType && (emi_settings[i].type & 0x0F00) == 0x0000) {
                    // support for combo discrete dram
                    if (emi_settings[i].iLPDDR3_MODE_REG_5 == DramInfo.u2MR5VendorID) {
                        dramc_debug("[MDL] index:%d, rank0_size:0x%llx(0x%llx), rank1_size:0x%llx(0x%llx)\n", i,
                            rank0_size, emi_settings[i].DRAM_RANK_SIZE[0],
                            rank1_size, emi_settings[i].DRAM_RANK_SIZE[1]);
                        if((emi_settings[i].DRAM_RANK_SIZE[0] == rank0_size) && (emi_settings[i].DRAM_RANK_SIZE[1] == rank1_size)) {
                            mdl_number = i;
                            found = 1;
                            break;
                        }
                    }
                }
            }
        }

        dramc_crit("found:%d,i:%d\n",found,i);
    }

    return mdl_number;
}
#endif

int get_dram_channel_nr(void)
{
    int channel_nr;

    channel_nr = (*((volatile unsigned int*)(EMI_CONA)) >> 8) & 0x3;

    return (0x1 << channel_nr);
}

int get_dram_rank_nr(void)
{

    int index;
    int emi_cona;

#ifdef COMBO_MCP
  #ifdef DDR_RESERVE_MODE
    if(g_ddr_reserve_enable==1 && g_ddr_reserve_success==1)
    {
      emi_cona = *(volatile unsigned int*)(EMI_CONA);
    }
    else
  #endif
    {
      index = mt_get_mdl_number();
      if (index < 0 || index >=  num_of_emi_records)
      {
          return -1;
      }

      emi_cona = emi_settings[index].EMI_CONA_VAL;
    }
#else
    emi_cona = default_emi_setting.EMI_CONA_VAL;
#if CFG_FPGA_PLATFORM
    return 1;
#endif
#endif

    if ((emi_cona & (1 << 17)) != 0 || //for channel 0
        (emi_cona & (1 << 16)) != 0 )  //for channel 1
        return 2;
    else
        return 1;
}

unsigned int get_dram_mr(unsigned int index)
{
	switch (index) {
	case 5:
		return mr5;
	default:
		return 0;
	}
}

void get_dram_rank_size_by_EMI_CONA (u64 dram_rank_size[])
{
    unsigned col_bit, row_bit;
    u64 ch0_rank0_size, ch0_rank1_size, ch1_rank0_size, ch1_rank1_size;
#ifndef COMBO_MCP
    unsigned emi_cona = default_emi_setting.EMI_CONA_VAL;
    unsigned emi_conh = default_emi_setting.EMI_CONH_VAL;
#else
    unsigned emi_cona = *(volatile unsigned int*)(EMI_CONA);
    unsigned emi_conh = *(volatile unsigned int*)(EMI_CONH);
#endif
    unsigned nr_chan_enabled = 1;
    u64 per_chan_rank0_size = 0, per_chan_rank1_size = 0;
    unsigned shift_for_16bit = 1;	// data width = 2 bytes

    if (emi_cona & 0x2)
        shift_for_16bit = 0;		// data width = 4 bytes

    dram_rank_size[0] = 0;
    dram_rank_size[1] = 0;

    ch0_rank0_size = (emi_conh >> 16) & 0xf;
    ch0_rank1_size = (emi_conh >> 20) & 0xf;
    ch1_rank0_size = (emi_conh >> 24) & 0xf;
    ch1_rank1_size = (emi_conh >> 28) & 0xf;

    switch ((emi_cona >> 8) & 0x3) {
	    case 0:
		    nr_chan_enabled = 1;
		    break;
	    case 1:
		    nr_chan_enabled = 2;
		    break;
	    case 2:
		    nr_chan_enabled = 4;
		    break;
	    case 3:
	    default:
		    dramc_crit("invalid CHN_EN field in EMI_CONA (0x%x)\n", emi_cona);
		    // assume 4 channel by default
		    nr_chan_enabled = 2;
		    break;
    }

    // CH0 EMI
    {
        if(ch0_rank0_size == 0)
        {
            //rank 0 setting
            col_bit = ((emi_cona >> 4) & 0x03) + 9;
            row_bit = ((((emi_cona >> 24) & 0x01) << 2) + ((emi_cona >> 12) & 0x03)) + 13;
            per_chan_rank0_size = ((u64)(1 << (row_bit + col_bit))) * ((u64)(4 >> shift_for_16bit) * 8); // data width (bytes) * 8 banks
        }
        else
        {
            per_chan_rank0_size = (ch0_rank0_size * 256 << 20);
        }

        if (0 != (emi_cona &  (1 << 17)))   //rank 1 exist
        {
            if(ch0_rank1_size == 0)
            {
                col_bit = ((emi_cona >> 6) & 0x03) + 9;
                row_bit = ((((emi_cona >> 25) & 0x01) << 2) + ((emi_cona >> 14) & 0x03)) + 13;
                per_chan_rank1_size = ((u64)(1 << (row_bit + col_bit))) * ((u64)(4 >> shift_for_16bit) * 8); // data width (bytes) * 8 banks
            }
            else
            {
                per_chan_rank1_size = (ch0_rank1_size * 256 << 20);
            }
        }

	if (nr_chan_enabled > 2) {
		// CH0 EMI have CHA+CHB
		dram_rank_size[0] = per_chan_rank0_size * 2;
		dram_rank_size[1] = per_chan_rank1_size * 2;
	} else {
		// CH0 EMI is CHA
		dram_rank_size[0] = per_chan_rank0_size;
		dram_rank_size[1] = per_chan_rank1_size;
	}
    }

    // CH1 EMI
    if(nr_chan_enabled >= 2)
    {
        if(ch1_rank0_size == 0)
        {
            //rank0 setting
            col_bit = ((emi_cona >> 20) & 0x03) + 9;
            row_bit = ((((emi_conh >> 4) & 0x01) << 2) + ((emi_cona >> 28) & 0x03)) + 13;
            per_chan_rank0_size = ((u64)(1 << (row_bit + col_bit))) * ((u64)(4 >> shift_for_16bit) * 8); // data width (bytes) * 8 banks
        }
        else
        {
            per_chan_rank0_size = (ch1_rank0_size * 256 << 20);
        }

        if (0 != (emi_cona &  (1 << 16)))   //rank 1 exist
        {
            if(ch1_rank1_size == 0)
            {
                col_bit = ((emi_cona >> 22) & 0x03) + 9;
                row_bit = ((((emi_conh >> 5) & 0x01) << 2) + ((emi_cona >> 30) & 0x03)) + 13;
                per_chan_rank1_size = ((u64)(1 << (row_bit + col_bit))) * ((u64)(4 >> shift_for_16bit) * 8); // data width (bytes) * 8 banks
            }
            else
            {
                per_chan_rank1_size = (ch1_rank1_size * 256 << 20);
            }
        }
	if (nr_chan_enabled > 2) {
		// CH1 EMI have CHC+CHD
		dram_rank_size[0] += per_chan_rank0_size * 2;
		dram_rank_size[1] += per_chan_rank1_size * 2;
	} else {
		// CH1 EMI is CHB
		dram_rank_size[0] += per_chan_rank0_size;
		dram_rank_size[1] += per_chan_rank1_size;
	}
    }

    //dramc_debug("DRAM rank0 size:0x%llx,\nDRAM rank1 size=0x%llx\n", dram_rank_size[0], dram_rank_size[1]);
}

#if (FOR_DV_SIMULATION_USED==0)
#if !__FLASH_TOOL_DA__ && !__ETT__
/*
 * setup block
 */
void get_orig_dram_rank_info(dram_info_t *orig_dram_info)
{
	int i, j;
	u64 base = DRAM_BASE;
	u64 rank_size[4];

	i = get_dram_rank_nr();

	orig_dram_info->rank_num = (i > 0) ? i : 0;
	get_dram_rank_size(rank_size);

	orig_dram_info->rank_info[0].start = base;
	for (i = 0; i < orig_dram_info->rank_num; i++) {

		orig_dram_info->rank_info[i].size = (u64)rank_size[i];

		if (i > 0) {
			orig_dram_info->rank_info[i].start =
				orig_dram_info->rank_info[i - 1].start +
				orig_dram_info->rank_info[i - 1].size;
		}
		dramc_debug("orig_dram_info[%d] start: 0x%llx, size: 0x%llx\n",
				i, orig_dram_info->rank_info[i].start,
				orig_dram_info->rank_info[i].size);
	}

	for(j=i; j<4; j++)
	{
	  		orig_dram_info->rank_info[j].start = 0;
	  		orig_dram_info->rank_info[j].size = 0;
	}
#ifdef CUSTOM_CONFIG_MAX_DRAM_SIZE
	for (i = 1; i < orig_dram_info->rank_num; i++) {
		if (orig_dram_info->rank_info[i].start >= (CUSTOM_CONFIG_MAX_DRAM_SIZE + DRAM_BASE)) {
			orig_dram_info->rank_num = i;
			dramc_crit("[dramc] reduce rank size = %u\n", (unsigned int) orig_dram_info->rank_num);
			break;
		}
	}
#endif
}

void get_dram_rank_size (u64 dram_rank_size[])
{
#ifdef COMBO_MCP
    int index, rank_nr, i;

  #ifdef DDR_RESERVE_MODE
    if(g_ddr_reserve_enable==1 && g_ddr_reserve_success==1)
    {
        get_dram_rank_size_by_EMI_CONA(dram_rank_size);
    }
    else
  #endif
    {
        index = mt_get_mdl_number();

        if (index < 0 || index >= num_of_emi_records)
        {
            return;
        }

        rank_nr = get_dram_rank_nr();

        for(i = 0; i < rank_nr; i++){
            dram_rank_size[i] = emi_settings[index].DRAM_RANK_SIZE[i];

            dramc_debug("%d:dram_rank_size:%llx\n",i,dram_rank_size[i]);
        }
    }
    return;
#else
    get_dram_rank_size_by_EMI_CONA(dram_rank_size);
    return;
#endif
}

void reserve_dramc_dummy_read(void)
{
	unsigned long long reserve_start;
	char *reserve_name[4] = {"dramc-rk0", "dramc-rk1", "dramc-rk2", "dramc-rk3"};
	unsigned int i;
	int rank_nr;
	dram_addr_t dram_addr;

	dram_addr.ch = 0;

	rank_nr = get_dram_rank_nr();
	if (rank_nr <= 0) {
		dramc_crit("[DRAMC] reserve dummy read fail\n");
		ASSERT(0);
	}

	for (i = 0; i < (unsigned int)rank_nr; i++) {
		dram_addr.rk = i;
		get_dramc_addr(&dram_addr, 0x0);
#ifdef CUSTOM_CONFIG_MAX_DRAM_SIZE
		if (dram_addr.full_sys_addr > (unsigned long long)CUSTOM_CONFIG_MAX_DRAM_SIZE + 0x40000000LL)
			break;
#endif
		reserve_start = mblock_reserve_ext(&bootarg.mblock_info, 0x1000, 0x1000, dram_addr.full_sys_addr, 0, reserve_name[i]);
		if (reserve_start != (dram_addr.full_sys_addr - 0x1000)) {
			dramc_crit("[DRAMC] dummy read fail (0x%llx)\n", reserve_start);
			ASSERT(0);
		}
	}
}
#endif //#if !__FLASH_TOOL_DA__ && !__ETT__
#endif

#ifdef VOLTAGE_SEL
void update_voltage_select_info(void)
{
	voltage_sel_info_ptr.vcore = doe_get_config("vcore");
	voltage_sel_info_ptr.vdram = doe_get_config("vdram");
	voltage_sel_info_ptr.vdram_cust = doe_get_config("vdram_cust");
	voltage_sel_info_ptr.vddq = doe_get_config("vddq");
	voltage_sel_info_ptr.vddq_cust = doe_get_config("vddq_cust");
	voltage_sel_info_ptr.vio18 = doe_get_config("vio18");
	voltage_sel_info_ptr.vio18_cust = doe_get_config("vio18_cust");

	print("DOE setting: vcore %d, vdram %d, vddq %d, vio18 %d \n",
		voltage_sel_info_ptr.vcore, voltage_sel_info_ptr.vdram,
		voltage_sel_info_ptr.vddq, voltage_sel_info_ptr.vio18);
}

int vio18_voltage_select()
{
	if (voltage_sel_info_ptr.vio18_cust)
		return voltage_sel_info_ptr.vio18_cust;

	if (voltage_sel_info_ptr.vio18 == LEVEL_LV) {
		return HQA_VIO18_LV;	
	} else if (voltage_sel_info_ptr.vio18 == LEVEL_HV) {
		return HQA_VIO18_HV;
	} else if (voltage_sel_info_ptr.vio18 == LEVEL_NV){
		return HQA_VIO18_NV;
	} else {
		return 0; //HQA_VIO18_NV will set pmic to 1.85V,so keep default ,not set.
	}
}

int vddq_voltage_select()
{
	if (voltage_sel_info_ptr.vddq_cust)
		return voltage_sel_info_ptr.vddq_cust;

	if (voltage_sel_info_ptr.vddq == LEVEL_LV) {
		return HQA_VDDQ_LV_LP4;	
	} else if (voltage_sel_info_ptr.vddq == LEVEL_HV) {
		return HQA_VDDQ_HV_LP4;
	} else {
		return HQA_VDDQ_NV_LP4;
	}
}

int vdram_voltage_select(unsigned int dtype)
{
	if (voltage_sel_info_ptr.vdram_cust)
		return voltage_sel_info_ptr.vdram_cust;

	if (voltage_sel_info_ptr.vdram == LEVEL_LV) {
                if (dtype == TYPE_LPDDR3) {
			return HQA_VDRAM_LV_LP3;
                } else {
			return HQA_VDRAM_LV_LP4;
                }
	} else if (voltage_sel_info_ptr.vdram == LEVEL_HV) {
                if (dtype == TYPE_LPDDR3) {
			return HQA_VDRAM_HV_LP3;
                } else {
			return HQA_VDRAM_HV_LP4;
                }
	} else {
		if (dtype == TYPE_LPDDR3) {
			return HQA_VDRAM_NV_LP3;
		} else {
			return HQA_VDRAM_NV_LP4;
		}
	}
}

int vcore_voltage_select(unsigned int dtype, DRAM_KSHU kshu)
{
	int ret = 0;
	if (voltage_sel_info_ptr.vcore == LEVEL_LV) {
		if (dtype == TYPE_LPDDR3) {
			switch(kshu) {
				case KOPP0:
					ret = HQA_VCORE_LV_LP3_KOPP0;
					break;
				case KOPP1:
					ret = HQA_VCORE_LV_LP3_KOPP1;
					break;
				case KOPP2:
					ret = HQA_VCORE_LV_LP3_KOPP2;
					break;
				case KOPP3:
					ret = HQA_VCORE_LV_LP3_KOPP3;
					break;
				case KOPP4:
					ret = HQA_VCORE_LV_LP3_KOPP4;
					break;
			};
		} else {
			switch(kshu) {
				case KOPP0:
					ret = HQA_VCORE_LV_LP4_KOPP0;
					break;
				case KOPP1:
					ret = HQA_VCORE_LV_LP4_KOPP1;
					break;
				case KOPP2:
					ret = HQA_VCORE_LV_LP4_KOPP2;
					break;
				case KOPP3:
					ret = HQA_VCORE_LV_LP4_KOPP3;
					break;
				case KOPP4:
					ret = HQA_VCORE_LV_LP4_KOPP4;
					break;
			};
		}
	} else if (voltage_sel_info_ptr.vcore == LEVEL_HV) {
                if (dtype == TYPE_LPDDR3) {
                        switch(kshu) {
                                case KOPP0:
                                        ret = HQA_VCORE_HV_LP3_KOPP0;
                                        break;
                                case KOPP1:
                                        ret = HQA_VCORE_HV_LP3_KOPP1;
                                        break;
                                case KOPP2:
                                        ret = HQA_VCORE_HV_LP3_KOPP2;
                                        break;
                                case KOPP3:
                                        ret = HQA_VCORE_HV_LP3_KOPP3;
                                        break;
                                case KOPP4:
                                        ret = HQA_VCORE_HV_LP3_KOPP4;
                                        break;
                        };
                } else {
                        switch(kshu) {
                                case KOPP0:
                                        ret = HQA_VCORE_HV_LP4_KOPP0;
                                        break;
                                case KOPP1:
                                        ret = HQA_VCORE_HV_LP4_KOPP1;
                                        break;
                                case KOPP2:
                                        ret = HQA_VCORE_HV_LP4_KOPP2;
                                        break;
                                case KOPP3:
                                        ret = HQA_VCORE_HV_LP4_KOPP3;
                                        break;
                                case KOPP4:
                                        ret = HQA_VCORE_HV_LP4_KOPP4;
                                        break;
                        };
			}
	} else {
		if (dtype == TYPE_LPDDR3) {
                        switch(kshu) {
                                case KOPP0:
                                        ret = HQA_VCORE_NV_LP3_KOPP0;
                                        break;
                                case KOPP1:
                                        ret = HQA_VCORE_NV_LP3_KOPP1;
                                        break;
                                case KOPP2:
                                        ret = HQA_VCORE_NV_LP3_KOPP2;
                                        break;
                                case KOPP3:
                                        ret = HQA_VCORE_NV_LP3_KOPP3;
                                        break;
                                case KOPP4:
                                        ret = HQA_VCORE_NV_LP3_KOPP4;
                                        break;
                        };
                } else {
                        switch(kshu) {
                                case KOPP0:
                                        ret = HQA_VCORE_NV_LP4_KOPP0;
                                        break;
                                case KOPP1:
                                        ret = HQA_VCORE_NV_LP4_KOPP1;
                                        break;
                                case KOPP2:
                                        ret = HQA_VCORE_NV_LP4_KOPP2;
                                        break;
                                case KOPP3:
                                        ret = HQA_VCORE_NV_LP4_KOPP3;
                                        break;
                                case KOPP4:
                                        ret = HQA_VCORE_NV_LP4_KOPP4;
                                        break;
                        };
                }

	}

	return ret;
}

#endif

#if (FOR_DV_SIMULATION_USED==0)
#if !__ETT__
void mt_set_emi(void)
{
    int index;
    /*unsigned int SW_CTRL_VC, HW_CTRL_VC;*/
    EMI_SETTINGS *emi_set;

    #ifdef VOLTAGE_SEL
    update_voltage_select_info();
    #endif
    // set voltage and hw trapping before mdl
    setup_dramc_voltage_by_pmic();

    /* log to storage flag init */
    log_to_storage_init();

#ifdef VCORE_BIN
    dvfsrc_opp_level_mapping();
#endif

#ifdef COMBO_MCP
    index = mt_get_mdl_number();
    dramc_crit("[EMI] MDL number = %d\r\n", index);
    if (index < 0 || index >=  num_of_emi_records)
    {
        dramc_crit("[EMI] setting failed 0x%x\r\n", index);
        ASSERT(0);
    }
    else
    {
        emi_setting_index = index;
        emi_set = &emi_settings[emi_setting_index];
    }
#else
    dramc_crit("[EMI] ComboMCP not ready, using default setting\n");
    emi_setting_index = -1;
    emi_set = &default_emi_setting;
#endif
    mr5 = emi_set->iLPDDR3_MODE_REG_5;

#ifdef DDR_RESERVE_MODE
    if(g_ddr_reserve_enable==1 && g_ddr_reserve_success==0)
        Before_Init_DRAM_While_Reserve_Mode_fail(emi_set->type & 0xF);
#endif

    if (dram_log_to_storage_flag)
    {
        log_start = 1;
        print("log_start=0x%x part_dram_data_addr_uart=0x%llx \n",log_start,part_dram_data_addr_uart);
    }
#if defined(SLT) && (!__ETT__)
	part_dram_data_addr_slt = get_part_addr("boot_para");
	if (part_dram_data_addr_slt != 0x0)
		dramc_crit("[dramc] SLT partition address is 0x%llx\n", part_dram_data_addr_slt);
	else
		dramc_crit("[dramc] SLT partition address is incorrect !!!\n");
	SLT_Test_Main_Flow((emi_set->type & 0xF), emi_set->dram_cbt_mode_extern, NULL, SLT_USED);
#endif
    Init_DRAM((emi_set->type & 0xF), emi_set->dram_cbt_mode_extern, NULL, NORMAL_USED);
    switch_dramc_voltage_to_auto_mode();
    restore_vcore_setting();

    if (dram_log_to_storage_flag)
    {
        log_start = 0;
        print("log_start=0x%x part_dram_data_addr_uart=0x%llx \n",log_start,part_dram_data_addr_uart);
    }
#if 0
	{
		DRAMC_CTX_T * p = psCurrDramCtx;
		DramcRegDump(p);
	}
#endif

	if (doe_get_config("emi_dcm_opt") == 1) {
		*(volatile unsigned int *) EMI_CONM |= 0xff000000;
		*(volatile unsigned int *) EMI_CONN |= 0xff000000;
		*(volatile unsigned int *) CHN_EMI_CONB(CHN0_EMI_BASE) |= 0xff000000;
		*(volatile unsigned int *) CHN_EMI_CONB(CHN1_EMI_BASE) |= 0xff000000;
		print("EMI DCM DISABLED\n");
	} else if (doe_get_config("emi_dcm_opt") == 2) {
		*(volatile unsigned int *) EMI_CONM &= ~0xff000000;
		*(volatile unsigned int *) EMI_CONN &= ~0xff000000;
		*(volatile unsigned int *) CHN_EMI_CONB(CHN0_EMI_BASE) &= ~0xff000000;
		*(volatile unsigned int *) CHN_EMI_CONB(CHN1_EMI_BASE) &= ~0xff000000;
		print("EMI DCM ENABLED\n");
	}
}
#endif
#endif

#define DRAMC_ADDR_SHIFT_CHN(addr, channel) (addr + (channel * 0x10000))

void phy_addr_to_dram_addr(dram_addr_t *dram_addr, unsigned long long phy_addr)
{
	unsigned int emi_cona, emi_conf;
	unsigned long long rank_size[4];
	unsigned int channel_num, rank_num;
	unsigned int bit_scramble, bit_xor, bit_shift, channel_pos, channel_width;
	unsigned int temp;
	unsigned int index;
	unsigned int ddr_type = mt_get_dram_type_from_hw_trap();

	emi_cona = *((volatile unsigned int *)EMI_CONA);
	emi_conf = *((volatile unsigned int *)EMI_CONF) >> 8;
	get_dram_rank_size_by_EMI_CONA(rank_size);
	rank_num = (unsigned int) get_dram_rank_nr();
	channel_num = (unsigned int) get_dram_channel_nr();

	if (rank_num >= sizeof(rank_size) / sizeof(*rank_size)) {
		dramc_crit("[Dramc] Wrong rank_num: %u\n", rank_num);
		ASSERT(0);
	}

	phy_addr -= 0x40000000;
	for (index = 0; index < rank_num; index++) {
		if (phy_addr >= rank_size[index])
			phy_addr -= rank_size[index];
		else
			break;
	}

	for (bit_scramble = 11; bit_scramble < 17; bit_scramble++) {
		bit_xor = (emi_conf >> (4 * (bit_scramble - 11))) & 0xf;
		bit_xor &= phy_addr >> 16;
		for (bit_shift = 0; bit_shift < 4; bit_shift++)
			phy_addr ^= ((bit_xor>>bit_shift)&0x1) << bit_scramble;
	}

	if (channel_num > 1) {
		channel_pos = ((emi_cona >> 2) & 0x3) + 7;

		for (channel_width = bit_shift = 0; bit_shift < 4; bit_shift++) {
			if ((unsigned int)(1 << bit_shift) >= channel_num)
				break;
			channel_width++;
		}

		switch (channel_width) {
			case 2:
				dram_addr->addr = ((phy_addr & ~(((0x1 << 2) << channel_pos) - 1)) >> 2);
				break;
			default:
				dram_addr->addr = ((phy_addr & ~(((0x1 << 1) << channel_pos) - 1)) >> 1);
				break;
		}
		dram_addr->addr |= (phy_addr & ((0x1 << channel_pos) - 1));
	} else {
		dram_addr->addr = phy_addr;
	}

	if (u1IsLP4Family(ddr_type))
		dram_addr->addr >>= 1;
	else
		dram_addr->addr >>= 2;

	temp = dram_addr->addr;
	switch ((emi_cona >> 4) & 0x3) {
		case 0:
			dram_addr->col = temp & 0x1FF;
			temp = temp >> 9;
			break;
		case 1:
			dram_addr->col = temp & 0x3FF;
			temp = temp >> 10;
			break;
		case 2:
		default:
			dram_addr->col = temp & 0x7FF;
			temp = temp >> 11;
			break;
	}
	dram_addr->bk = temp & 0x7;
	temp = temp >> 3;

	dram_addr->row = temp;

	//mcSHOW_DBG_MSG(("ch%d, rk%d, dram addr: %x\n", dram_addr->ch, dram_addr->rk, dram_addr->addr));
	//mcSHOW_DBG_MSG(("bk%x, row%x, col%x\n", dram_addr->bk, dram_addr->row, dram_addr->col));
}

void put_dummy_read_pattern(unsigned long long dst_pa, unsigned int src_pa, unsigned int len)
{
	*((volatile unsigned int *)(CQ_DMA_BASE + 0x018)) = 7 << 16;

	*((volatile unsigned int *)(CQ_DMA_BASE + 0x01c)) = src_pa;
	*((volatile unsigned int *)(CQ_DMA_BASE + 0x060)) = 0;

	*((volatile unsigned int *)(CQ_DMA_BASE + 0x020)) = dst_pa & 0xffffffff;
	*((volatile unsigned int *)(CQ_DMA_BASE + 0x064)) = dst_pa >> 32;

	*((volatile unsigned int *)(CQ_DMA_BASE + 0x024)) = len;
	dsb();
	*((volatile unsigned int *)(CQ_DMA_BASE + 0x008)) = 0x1;

	while(*((volatile unsigned int *)(CQ_DMA_BASE + 0x008)));

	*((volatile unsigned int *)(CQ_DMA_BASE + 0x064)) = 0;
}

static unsigned int get_dramc_addr(dram_addr_t *dram_addr, unsigned int offset)
{
	unsigned int channel_num, rank_num;
	unsigned long long dummy_read_addr;
	unsigned long long rank_size[4];
	unsigned int index;
	unsigned int *src_addr;

	channel_num = (unsigned int) get_dram_channel_nr();
	rank_num = (unsigned int) get_dram_rank_nr();
	get_dram_rank_size_by_EMI_CONA(rank_size);
	dummy_read_addr = 0x40000000;
	src_addr = (unsigned int *) 0x40000000;

	if (dram_addr->ch >= channel_num) {
		mcSHOW_DBG_MSG(("[DRAMC] invalid channel: %d\n", dram_addr->ch));
		return 0;
	}

	if (dram_addr->rk >= rank_num) {
		mcSHOW_DBG_MSG(("[DRAMC] invalid rank: %d\n", dram_addr->rk));
		return 0;
	}

	for (index = 0; index <= dram_addr->rk; index++)
		dummy_read_addr += rank_size[index];
	dummy_read_addr -= offset;
	if (dram_addr->ch == 0)
		dummy_read_addr &= ~(0x100);

	if (offset == 0x20) {
		for (index = 0; index < 4; index++)
			*(src_addr + index) = 0xAAAA5555;
		put_dummy_read_pattern(dummy_read_addr, (unsigned int) src_addr, 16);
	}

	dram_addr->full_sys_addr = dummy_read_addr;
	phy_addr_to_dram_addr(dram_addr, dummy_read_addr);

	return dram_addr->addr;
}

unsigned int get_dummy_read_addr(dram_addr_t *dram_addr)
{
	return get_dramc_addr(dram_addr, 0x20); // 32-byte align for dummy RW pattern
}

unsigned int get_ta2_addr(dram_addr_t *dram_addr)
{
	unsigned int addr = get_dramc_addr(dram_addr, 0x1000);

	if (!u1IsLP4Family(mt_get_dram_type_from_hw_trap()))
		addr <<= 2;

	return addr & 0xFFFFFFF0;
}

void init_ta2_single_channel(unsigned int channel)
{
	unsigned int temp;
	unsigned int matype[2];
	unsigned int col_shf[2] = {0, 0};
	dram_addr_t dram_addr;
	DRAMC_CTX_T *p = psCurrDramCtx;
	int rank_nr;

	// mt6771: CHN0_EMI for CHN-A;  CHN1_EMI for CHN-B
	if(channel < 2) {
		matype[0] = (*(volatile unsigned *)EMI_CONA >> 4) & 0x3;
		matype[1] = (*(volatile unsigned *)EMI_CONA >> 6) & 0x3;
	} else {
		matype[0] = (*(volatile unsigned *)EMI_CONA >> 20) & 0x3;
		matype[1] = (*(volatile unsigned *)EMI_CONA >> 22) & 0x3;
	}

	rank_nr = get_dram_rank_nr();
	if (rank_nr > 1) {
		if (matype[0] < matype[1]) {
			col_shf[0] = matype[1] - matype[0];
			matype[0] = matype[1];
		} else if (matype[1] < matype[0])
			col_shf[1] = matype[0] - matype[1];
	}

	matype[0] = (matype[0] + 1) << 30;

	// disable self test engine1 and self test engine2
	temp = u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_3, channel)) & 0x1FFFFFFF;
	vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_3, channel), temp);

	// set MATYPE
	temp = (u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_SHU_CONF0, channel)) & 0x3FFFFFFF) | matype[0];
	vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_SHU_CONF0, channel), temp);
	temp = (u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_SHU2_CONF0, channel)) & 0x3FFFFFFF) | matype[0];
	vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_SHU2_CONF0, channel), temp);
	temp = (u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_SHU3_CONF0, channel)) & 0x3FFFFFFF) | matype[0];
	vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_SHU3_CONF0, channel), temp);

	if (rank_nr > 1) {
		// set rank address for test agent to auto
		temp = u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_4, channel)) & 0x8FFFFFFF;
		temp |= (0x4 << 28);
		vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_4, channel), temp);

		// set test for both rank0 and rank1
		temp = u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_3, channel)) & 0xFFFFFFF0;
		vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_3, channel), temp | 0x1);
	}

	// set base address for test agent to reserved space
	dram_addr.ch = channel;
	dram_addr.rk = 0;
	temp = (u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_1, channel)) & 0x0000000F);
	vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_1, channel), temp | (get_ta2_addr(&dram_addr) << col_shf[0]));
	dram_addr.rk = 1;
	temp = (u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_5, channel)) & 0x0000000F);
	vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_5, channel), temp | (get_ta2_addr(&dram_addr) << col_shf[1]));

	// set test length (offset) to 0x20
	temp = (u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_2, channel)) & 0x0000000F) | (0x20 << 4);
	vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_2, channel), temp);

	return;
}

#ifdef LAST_DRAMC

static unsigned int is_last_dramc_initialized(void)
{
	if(last_dramc_info_ptr->ta2_result_magic != LAST_DRAMC_MAGIC_PATTERN) {
		return 0;
	} else {
		return 1;
	}
}

void update_last_dramc_info(void)
{
	unsigned int chn;
	unsigned int latch_result = 0;
	unsigned int temp;
	unsigned int *curr;
	DRAMC_CTX_T *p = psCurrDramCtx;

	// init checksum and magic pattern
	if(last_dramc_info_ptr->ta2_result_magic != LAST_DRAMC_MAGIC_PATTERN) {
		last_dramc_info_ptr->ta2_result_magic = LAST_DRAMC_MAGIC_PATTERN;
		last_dramc_info_ptr->ta2_result_last = 0;
		last_dramc_info_ptr->ta2_result_past = 0;
		last_dramc_info_ptr->ta2_result_checksum = LAST_DRAMC_MAGIC_PATTERN;
		last_dramc_info_ptr->reboot_count = 0;
	} else {
		last_dramc_info_ptr->ta2_result_checksum ^= last_dramc_info_ptr->reboot_count;
		last_dramc_info_ptr->reboot_count++;
		last_dramc_info_ptr->ta2_result_checksum ^= last_dramc_info_ptr->reboot_count;
	}

	// TODO: check DCS status

	// read data from latch register and reset
	for (chn = 0; chn < CHANNEL_NUM; ++chn) {
		//dramc_crit("[LastDRAMC] latch result before RST: %x\n", u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_WDT_DBG_SIGNAL, chn)));
		latch_result = (latch_result << 16) | u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_WDT_DBG_SIGNAL, chn)) & 0xFFFF;
		temp = u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_DDRCONF0, chn));
		vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_DDRCONF0, chn), temp | 0x00000004);
		vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_DDRCONF0, chn), temp & 0xFFFFFFFB);
		//dramc_crit("[LastDRAMC] latch result after RST: %x\n", u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_WDT_DBG_SIGNAL, chn)));
	}

	last_dramc_info_ptr->ta2_result_checksum ^= last_dramc_info_ptr->ta2_result_past ^ latch_result;
	last_dramc_info_ptr->ta2_result_past = last_dramc_info_ptr->ta2_result_last;
	last_dramc_info_ptr->ta2_result_last = latch_result;
	for (temp = 0; temp < sizeof(LAST_DRAMC_INFO_T) / sizeof(temp); temp++) {
		curr = (unsigned int *)last_dramc_info_ptr + temp;
		dramc_crit("[LastDRAMC] 0x%x: 0x%x\n", curr, *curr);
	}

	return;
}

void update_last_dramc_k_voltage(DRAMC_CTX_T *p, unsigned int voltage)
{
	unsigned int shu_type;

	shu_type = get_shuffleIndex_by_Freq(p);

	last_dramc_info_ptr->k_voltage[shu_type] = voltage;
}

void init_ta2_all_channel(void)
{
	unsigned int chn;

	update_last_dramc_info();

	// TODO: consider DCS
	for (chn = 0; chn < CHANNEL_NUM; ++chn)
		init_ta2_single_channel(chn);
}


unsigned int check_gating_err_in_dramc_latch(void)
{
	unsigned int chn, ret = 0;
	DRAMC_CTX_T *p = psCurrDramCtx;

	if ((g_boot_reason == BR_POWER_KEY) || (g_boot_reason == BR_USB)
			|| mtk_wdt_is_pmic_full_reset() || (is_last_dramc_initialized() == 0)){
		dramc_crit("for cold boot, always return 0\n");
		return 0;
	}

	for (chn = 0; chn <= 3; ++chn) {
		if (u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_WDT_DBG_SIGNAL, chn)) & 0x4000) {
			dramc_crit("[dramc] found gating error in channel %d (0x%x)\n",
					chn, u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_WDT_DBG_SIGNAL, chn)));
			ret |= (1 << chn);
		}
	}

	return ret;
}

void dram_fatal_exception_detection_start(void)
{
	last_dramc_info_ptr = (LAST_DRAMC_INFO_T *) get_dbg_info_base(KEY_LAST_DRAMC);

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
	part_dram_data_addr = get_part_addr("boot_para")+0x100000; // addr = 0x8000
	if (part_dram_data_addr != 0x0)
		dramc_crit("[dramc] init partition address is 0x%llx\n", part_dram_data_addr);
	else {
		dramc_crit("[dramc] init partition address is incorrect !!!\n");
	}
#endif

	if ((g_boot_reason == BR_POWER_KEY) || (g_boot_reason == BR_USB)
			|| mtk_wdt_is_pmic_full_reset() || (is_last_dramc_initialized() == 0)){
		/* cold boot: initialize last_dram_fatal_err_flag and dram_fatal_err_flag */
		dramc_crit("[dramc] init SRAM region for DRAM exception detection\n");
		last_dramc_info_ptr->last_fatal_err_flag = 0x0;
		last_dramc_info_ptr->storage_api_err_flag = 0x0;
		dram_fatal_init_stberr();
	} else {
		last_dramc_info_ptr->last_fatal_err_flag = last_dramc_info_ptr->fatal_err_flag;
		last_dramc_info_ptr->storage_api_err_flag = 0x0;
		dram_fatal_backup_stberr();
		dram_fatal_init_stberr();
	}

	last_dramc_info_ptr->fatal_err_flag = 1 << OFFSET_DRAM_FATAL_ERR;
	dsb();
}

void dram_fatal_exception_detection_end(void)
{
	last_dramc_info_ptr->fatal_err_flag = 0x0;
	dsb();
}

unsigned int check_dram_fatal_exception(void)
{
	dramc_crit("[dramc] DRAM_FATAL_ERR_FLAG = 0x%x\n", last_dramc_info_ptr->fatal_err_flag);

	return ((last_dramc_info_ptr->fatal_err_flag & ~((1 << OFFSET_DRAM_FATAL_ERR)|DDR_RSV_MODE_ERR_MASK)) != 0x0) ? 1 : 0;
}

unsigned int check_last_dram_fatal_exception(void)
{
	dramc_crit("[dramc] LAST_DRAM_FATAL_ERR_FLAG = 0x%x\n", last_dramc_info_ptr->last_fatal_err_flag);

	return ((last_dramc_info_ptr->last_fatal_err_flag & ~(DDR_RSV_MODE_ERR_MASK)) != 0x0) ? 1 : 0;
}

void dram_fatal_set_ta2_err(unsigned int chn, unsigned int err_code)
{
	unsigned int shift = OFFSET_DRAM_TA2_ERR + 2 * chn, ret;

	if (chn > 3)
		return;

	ret = last_dramc_info_ptr->fatal_err_flag & ~(0x7 << shift);
	last_dramc_info_ptr->fatal_err_flag = ret | ((err_code & 0x7) << shift);
	dsb();
}

void dram_fatal_set_gating_err(unsigned int chn, unsigned int err_code)
{
	unsigned int shift = OFFSET_DRAM_GATING_ERR + 4 * chn, ret;

	if (chn > 3)
		return;

	ret = last_dramc_info_ptr->fatal_err_flag & ~(0xf << shift);
	last_dramc_info_ptr->fatal_err_flag = ret | ((err_code & 0xf) << shift);
	dsb();
}

void dram_fatal_init_stberr(void)
{
	last_dramc_info_ptr->gating_err[0][0] = 0x0;
	last_dramc_info_ptr->gating_err[0][1] = 0x0;
	last_dramc_info_ptr->gating_err[1][0] = 0x0;
	last_dramc_info_ptr->gating_err[1][1] = 0x0;

	dsb();
}

void dram_fatal_backup_stberr(void)
{
	last_dramc_info_ptr->last_gating_err[0][0] = last_dramc_info_ptr->gating_err[0][0];
	last_dramc_info_ptr->last_gating_err[0][1] = last_dramc_info_ptr->gating_err[0][1];
	last_dramc_info_ptr->last_gating_err[1][0] = last_dramc_info_ptr->gating_err[1][0];
	last_dramc_info_ptr->last_gating_err[1][1] = last_dramc_info_ptr->gating_err[1][1];

	dsb();
}

void dram_fatal_set_stberr(unsigned int chn, unsigned int rk, unsigned int err_code)
{
	if ((chn > 1) || (rk > 1))
		return;

	last_dramc_info_ptr->gating_err[chn][rk] = err_code;

	dsb();
}

void dram_fatal_set_err(unsigned int err_code, unsigned int mask, unsigned int offset)
{
	unsigned int ret;

	ret = last_dramc_info_ptr->fatal_err_flag & ~(mask << offset);
	last_dramc_info_ptr->fatal_err_flag = ret | ((err_code & mask) << offset);
	dsb();
}

#endif

int doe_get_config(char* feature)
{
#if defined(ENABLE_DOE)
	char *doe_feature = dconfig_getenv(feature);
	int doe_result = atoi(doe_feature);
	dramc_crit("DOE force setting %s=%d\n", feature, doe_result);
	return doe_result;
#else
	return 0;
#endif
}

#if defined(TARGET_BUILD_VARIANT_ENG) || defined(TARGET_BUILD_VARIANT_USERDEBUG)
extern U8 gHQALog_flag;
static void log_to_storage_init(void)
{
    if (doe_get_config("log_to_storage"))
    {
        dram_log_to_storage_flag = 1;
        gHQALog_flag = 1;
        gEye_Scan_color_flag = 0;
        gCBT_EYE_Scan_flag = 0;
        gCBT_EYE_Scan_only_higheset_freq_flag = 1;
        gRX_EYE_Scan_flag = 1;
        gRX_EYE_Scan_only_higheset_freq_flag = 1;
        gTX_EYE_Scan_flag = 1;
        gTX_EYE_Scan_only_higheset_freq_flag = 1;
    }
}

void log_to_storage(const char c)
{
    int ret, clr_count;
    blkdev_t *bootdev = NULL;
    static u8 logen = 0;
    static char logbuf[1024];
    static int logcount;

    bootdev = blkdev_get(CFG_BOOT_DEV);

    if (log_start && (!logen)) {
        logen = 1;
        logcount = 0;
        part_dram_data_addr_uart = get_part_addr("boot_para") + 0x100000; // addr = 0x1f300000, the first 1MB for debug
        memset(&logbuf, 0, sizeof(logbuf));
        for (clr_count = 1300; clr_count < 1320 ; clr_count++) //clean after 1.3M, every project not the same
            ret = blkdev_write(bootdev, (part_dram_data_addr_uart + (1024 * clr_count)), 1024, (u8*)&logbuf, storage_get_part_id(STORAGE_PHYS_PART_USER));
    }

    if (log_start) {
        if (((((char) c >> 4) & 0x7) > 1) & ((((char) c >> 4) & 0x7) < 7))
            logbuf[logcount] = ((char) c & 0xF0) | (((char) c >> 2) & 0x03) | (((char) c << 2) & 0x0C);
        else
            logbuf[logcount] = (char) c;
        logcount = logcount + 1;
        //write to storage
        if (logcount==1024) {
            logcount = 0;
            ret = blkdev_write(bootdev, part_dram_data_addr_uart, 1024, (u8*)&logbuf, storage_get_part_id(STORAGE_PHYS_PART_USER));
            part_dram_data_addr_uart = part_dram_data_addr_uart + 1024;
        }
    }
}
#else
static void log_to_storage_init(void)
{
}
void log_to_storage(const char c)
{
}
#endif
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION

#if !__ETT__
#include <blkdev.h>
#include <partition.h>
#include <pl_version.h>
#else
#include "ett_common.h"
#include "emi.h"
#endif

u32 g_dram_storage_api_err_code;

static u16 crc16(const u8* data, u32 length){
	u8 x;
	u16 crc = 0xFFFF;

	while (length--) {
		x = crc >> 8 ^ *data++;
		x ^= x >> 4;
		crc = (crc << 8) ^ ((u8)(x << 12)) ^ ((u8)(x <<5)) ^ ((u8)x);
	}
	return crc;
}

static void assign_checksum_for_dram_data(DRAM_CALIBRATION_SHU_DATA_T *shu_data)
{
	/* need to initialize checksum to 0 before calculation */
	shu_data->checksum = 0;
	shu_data->checksum = crc16((u8*)shu_data, sizeof(*shu_data));
}

static int check_checksum_for_dram_data(DRAM_CALIBRATION_SHU_DATA_T *shu_data)
{
	u16 checksum_in_storage = shu_data->checksum;

	assign_checksum_for_dram_data(shu_data);

	return (shu_data->checksum == checksum_in_storage) ? 1 : 0;
}

#ifdef COMBO_MCP
static void assign_checksum_for_mdl_data(DRAM_CALIBRATION_MRR_DATA_T *mrr_info)
{
	/* need to initialize checksum to 0 before calculation */
	mrr_info->checksum = 0;
	mrr_info->checksum = crc16((u8*)mrr_info, sizeof(*mrr_info));
}

static int check_checksum_for_mdl_data(DRAM_CALIBRATION_MRR_DATA_T *mrr_info)
{
	u16 checksum_in_storage = mrr_info->checksum;

	assign_checksum_for_mdl_data(mrr_info);

	return (mrr_info->checksum == checksum_in_storage) ? 1 : 0;
}
#endif

#if !__ETT__
#ifdef COMBO_MCP
static int read_offline_dram_mdl_data(DRAM_INFO_BY_MRR_T *DramInfo)
{
	int i, ret;
	u16 emi_checksum;
	blkdev_t *bootdev = NULL;
	DRAM_CALIBRATION_HEADER_T hdr;
	DRAM_CALIBRATION_MRR_DATA_T mrr_info;
	DRAM_CALIBRATION_SHU_DATA_T shu_data;
	DRAM_CALIBRATION_DATA_T *datap = NULL;

	if (dram_offline_data_flags)
		goto exit;

	if (DramInfo == NULL) {
		dramc_crit("[dramc] DramInfo == NULL, skip\n");
		dram_offline_data_flags = ERR_NULL_POINTER;
		goto exit;
	}

	bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		dramc_crit("[dramc] can't find boot device(%d)\n", CFG_BOOT_DEV);
		dram_offline_data_flags = ERR_BLKDEV_NOT_FOUND;
		goto exit;
	}

	if (!part_dram_data_addr) {
		dram_offline_data_flags = ERR_BLKDEV_NO_PART;
		goto exit;
	}

	ret = blkdev_read(bootdev, part_dram_data_addr, sizeof(hdr), (u8*)&hdr, PART_ID_DRAM_DATA);
	if (ret != 0) {
		dramc_crit("[dramc] blkdev_read %s failed\n", "hdr");
		dram_offline_data_flags = ERR_BLKDEV_READ_FAIL;
		goto exit;
	}

	if (hdr.pl_version != PL_VERSION) {
		/* current preloader version does not match the calibration hdr in storage -> erase the partition */
		dramc_crit("[dramc] PL_VERSION is updated, erase the DRAM shu_data\n");

		shu_data.checksum = 0;

		/* clear each shuffle */
		for (i = 0; i < DRAM_DFS_SHUFFLE_MAX; i++) {
			ret = blkdev_write(bootdev, part_dram_data_addr + ((unsigned long) &datap->data[i]), sizeof(shu_data), (u8*)&shu_data, PART_ID_DRAM_DATA);
			if (ret != 0) {
				dramc_crit("[dramc] blkdev_write failed\n");
				dram_offline_data_flags = ERR_BLKDEV_WRITE_FAIL;
				goto exit;
			}
		}

		dram_offline_data_flags = ERR_PL_UPDATED;
		goto exit;
	}

	/* check magic number */
	if (hdr.magic_number != DRAM_CALIBRATION_DATA_MAGIC) {
		dramc_crit("[dramc] magic number mismatch\n");
		dram_offline_data_flags = ERR_MAGIC_NUMBER;
		goto exit;
	}

	ret = blkdev_read(bootdev, part_dram_data_addr + ((unsigned long) &datap->mrr_info), sizeof(mrr_info), (u8*)&mrr_info, PART_ID_DRAM_DATA);
	if (ret != 0) {
		dramc_crit("[dramc] blkdev_read %s failed\n", "data");
		dram_offline_data_flags = ERR_BLKDEV_READ_FAIL;
		goto exit;
	}

	/* check checksum */
	if (check_checksum_for_mdl_data(&mrr_info) != 1) {
		dramc_crit("[dramc] checksum failed\n");
		dram_offline_data_flags = ERR_CHECKSUM;

		goto exit;
	}

	emi_checksum = crc16((u8*)emi_settings, sizeof(emi_settings));

	if (emi_checksum != mrr_info.emi_checksum) {
		dramc_crit("[dramc] emi checksum failed\n");
		dram_offline_data_flags = ERR_CHECKSUM;

		goto exit;
	}

	/* copy the data stored in storage to the data structure for calibration */
	memcpy(DramInfo, &(mrr_info.DramInfo), sizeof(*DramInfo));

exit:
	if (dram_offline_data_flags)
		SET_DRAM_STORAGE_API_ERR(dram_offline_data_flags, DRAM_STORAGE_API_READ);

	return 0 - dram_offline_data_flags;
}

static int write_offline_dram_mdl_data(DRAM_INFO_BY_MRR_T *DramInfo)
{
	int ret;
	blkdev_t *bootdev = NULL;
	DRAM_CALIBRATION_HEADER_T hdr;
	DRAM_CALIBRATION_MRR_DATA_T mrr_info;
	DRAM_CALIBRATION_DATA_T *datap = NULL;

	if (DramInfo == NULL) {
		dramc_crit("[dramc] DramInfo == NULL, skip\n");
		SET_DRAM_STORAGE_API_ERR(ERR_NULL_POINTER, DRAM_STORAGE_API_WRITE);
		return -ERR_NULL_POINTER;
	}

	bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		dramc_crit("[dramc] can't find boot device(%d)\n", CFG_BOOT_DEV);
		SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_NOT_FOUND, DRAM_STORAGE_API_WRITE);
		return -ERR_BLKDEV_NOT_FOUND;
	}

	if (!part_dram_data_addr) {
		return -ERR_BLKDEV_NO_PART;
	}

	memcpy(&(mrr_info.DramInfo), DramInfo, sizeof(*DramInfo));

#if 0
    /* assign PL version */
    hdr.pl_version = PL_VERSION;

    /* assign magic number */
    hdr.magic_number = DRAM_CALIBRATION_DATA_MAGIC;

    /* assign api error code */
    hdr.calib_err_code = g_dram_storage_api_err_code;

    ret = blkdev_write(bootdev, part_dram_data_addr, sizeof(hdr), (u8*)&hdr, PART_ID_DRAM_DATA);
    if (ret != 0) {
        dramc_crit("[dramc] blkdev_write failed\n");
        SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_WRITE_FAIL, DRAM_STORAGE_API_WRITE);
        return -ERR_BLKDEV_WRITE_FAIL;
    }
#endif

    /* calculate and assign checksum */
	mrr_info.emi_checksum = crc16((u8*)emi_settings, sizeof(emi_settings));
    assign_checksum_for_mdl_data(&mrr_info);

    ret = blkdev_write(bootdev, part_dram_data_addr + ((unsigned long) &datap->mrr_info), sizeof(mrr_info), (u8*)&mrr_info, PART_ID_DRAM_DATA);
    if (ret != 0) {
        dramc_crit("[dramc] blkdev_write failed\n");
        SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_WRITE_FAIL, DRAM_STORAGE_API_WRITE);
        return -ERR_BLKDEV_WRITE_FAIL;
    }

    return 0;
}
#endif

int read_offline_dram_calibration_data(DRAM_DFS_SHUFFLE_TYPE_T shuffle, SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData)
{
	int i, ret;
	blkdev_t *bootdev = NULL;
	DRAM_CALIBRATION_HEADER_T hdr;
	DRAM_CALIBRATION_SHU_DATA_T shu_data;
	DRAM_CALIBRATION_DATA_T *datap = NULL;
	unsigned char *ch;

	if (dram_offline_data_flags)
		goto exit;

	if (offLine_SaveData == NULL) {
		dramc_crit("[dramc] offLine_SaveData == NULL, skip\n");
		dram_offline_data_flags = ERR_NULL_POINTER;
		goto exit;
	}

	bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		dramc_crit("[dramc] can't find boot device(%d)\n", CFG_BOOT_DEV);
		dram_offline_data_flags = ERR_BLKDEV_NOT_FOUND;
		goto exit;
	}

	if (!part_dram_data_addr) {
		dram_offline_data_flags = ERR_BLKDEV_NO_PART;
		goto exit;
	}

	ret = blkdev_read(bootdev, part_dram_data_addr, sizeof(hdr), (u8*)&hdr, PART_ID_DRAM_DATA);
	if (ret != 0) {
		dramc_crit("[dramc] blkdev_read %s failed\n", "hdr");
		dram_offline_data_flags = ERR_BLKDEV_READ_FAIL;
		goto exit;
	}

    	/* check preloader version */
	if (hdr.pl_version != PL_VERSION) {
		/* current preloader version does not match the calibration hdr in storage -> erase the partition */
		dramc_crit("[dramc] PL_VERSION is updated, erase the DRAM shu_data\n");

		shu_data.checksum = 0;

		/* clear each shuffle */
		for (i = 0; i < DRAM_DFS_SHUFFLE_MAX; i++) {
			ret = blkdev_write(bootdev, part_dram_data_addr + ((unsigned long) &datap->data[i]), sizeof(shu_data), (u8*)&shu_data, PART_ID_DRAM_DATA);
			if (ret != 0) {
				dramc_crit("[dramc] blkdev_write failed\n");
				dram_offline_data_flags = ERR_BLKDEV_WRITE_FAIL;
				goto exit;
			}
		}

		dram_offline_data_flags = ERR_PL_UPDATED;
		goto exit;
	}

	/* check magic number */
	if (hdr.magic_number != DRAM_CALIBRATION_DATA_MAGIC) {
		dramc_crit("[dramc] magic number mismatch\n");
		dram_offline_data_flags = ERR_MAGIC_NUMBER;
		goto exit;
	}

	ret = blkdev_read(bootdev, part_dram_data_addr + ((unsigned long) &datap->data[shuffle]), sizeof(shu_data), (u8*)&shu_data, PART_ID_DRAM_DATA);
	if (ret != 0) {
		dramc_crit("[dramc] blkdev_read %s failed\n", "data");
		dram_offline_data_flags = ERR_BLKDEV_READ_FAIL;
		goto exit;
	}

	/* check checksum */
	if (check_checksum_for_dram_data(&shu_data) != 1) {
		dramc_crit("[dramc] checksum failed\n");
		dram_offline_data_flags = ERR_CHECKSUM;
		goto exit;
	}

	/* copy the data stored in storage to the data structure for calibration */
	memcpy(offLine_SaveData, &(shu_data.calibration_data), sizeof(*offLine_SaveData));

exit:
	if (dram_offline_data_flags)
		SET_DRAM_STORAGE_API_ERR(dram_offline_data_flags, DRAM_STORAGE_API_READ);

	return 0 - dram_offline_data_flags;
}

int write_offline_dram_calibration_data(DRAM_DFS_SHUFFLE_TYPE_T shuffle, SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData)
{
	int ret;
	blkdev_t *bootdev = NULL;
	DRAM_CALIBRATION_HEADER_T hdr;
	DRAM_CALIBRATION_SHU_DATA_T shu_data;
	DRAM_CALIBRATION_DATA_T *datap = NULL;

	if (offLine_SaveData == NULL) {
		dramc_crit("[dramc] offLine_SaveData == NULL, skip\n");
		SET_DRAM_STORAGE_API_ERR(ERR_NULL_POINTER, DRAM_STORAGE_API_WRITE);
		return -ERR_NULL_POINTER;
	}

	bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		dramc_crit("[dramc] can't find boot device(%d)\n", CFG_BOOT_DEV);
		SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_NOT_FOUND, DRAM_STORAGE_API_WRITE);
		return -ERR_BLKDEV_NOT_FOUND;
	}

	if (!part_dram_data_addr) {
		return -ERR_BLKDEV_NO_PART;
	}

	memcpy(&(shu_data.calibration_data), offLine_SaveData, sizeof(*offLine_SaveData));

    /* assign PL version */
    hdr.pl_version = PL_VERSION;

    /* assign magic number */
    hdr.magic_number = DRAM_CALIBRATION_DATA_MAGIC;

    /* assign api error code */
    hdr.calib_err_code = g_dram_storage_api_err_code;

    ret = blkdev_write(bootdev, part_dram_data_addr, sizeof(hdr), (u8*)&hdr, PART_ID_DRAM_DATA);
    if (ret != 0) {
        dramc_crit("[dramc] blkdev_write failed\n");
        SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_WRITE_FAIL, DRAM_STORAGE_API_WRITE);
        return -ERR_BLKDEV_WRITE_FAIL;
    }

    /* calculate and assign checksum */
    assign_checksum_for_dram_data(&shu_data);

    ret = blkdev_write(bootdev, part_dram_data_addr + ((unsigned long) &datap->data[shuffle]), sizeof(shu_data), (u8*)&shu_data, PART_ID_DRAM_DATA);
    if (ret != 0) {
        dramc_crit("[dramc] blkdev_write failed\n");
        SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_WRITE_FAIL, DRAM_STORAGE_API_WRITE);
        return -ERR_BLKDEV_WRITE_FAIL;
    }

    return 0;
}

int clean_dram_calibration_data(void)
{
	int i, ret;
	blkdev_t *bootdev = NULL;
	DRAM_CALIBRATION_HEADER_T hdr;
	DRAM_CALIBRATION_SHU_DATA_T shu_data;
	DRAM_CALIBRATION_DATA_T *datap = NULL;

	bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		dramc_crit("[dramc] can't find block device(%d)\n", CFG_BOOT_DEV);
		SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_NOT_FOUND, DRAM_STORAGE_API_CLEAN);
		return -ERR_BLKDEV_NOT_FOUND;
	}

	if (!part_dram_data_addr) {
		return -ERR_BLKDEV_NO_PART;
	}

	memset(&hdr, 0, sizeof(hdr));

	ret = blkdev_write(bootdev, part_dram_data_addr, sizeof(hdr), (u8*)&hdr, PART_ID_DRAM_DATA);
	if (ret != 0) {
		dramc_crit("[dramc] blkdev_write failed\n");
		SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_WRITE_FAIL, DRAM_STORAGE_API_CLEAN);
		return -ERR_BLKDEV_WRITE_FAIL;
	}

    shu_data.checksum = 0;

    /* clear each shuffle */
    for (i = 0; i < DRAM_DFS_SHUFFLE_MAX; i++) {
        ret = blkdev_write(bootdev, part_dram_data_addr + ((unsigned long) &datap->data[i]), sizeof(shu_data), (u8*)&shu_data, PART_ID_DRAM_DATA);
        if (ret != 0) {
            dramc_crit("[dramc] blkdev_write failed\n");
            SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_WRITE_FAIL, DRAM_STORAGE_API_WRITE);
            return -ERR_BLKDEV_WRITE_FAIL;
        }
    }

    return 0;
}

#else

DRAM_CALIBRATION_DATA_T dram_data; // using global variable to avoid stack overflow

int read_offline_dram_calibration_data(DRAM_DFS_SHUFFLE_TYPE_T shuffle, SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData)
{
	return 0;
}

int write_offline_dram_calibration_data(DRAM_DFS_SHUFFLE_TYPE_T shuffle, SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData)
{
	return 0;
}

int clean_dram_calibration_data(void)
{
	return;
}
#endif


void set_err_code_for_storage_api(void)
{
#ifdef LAST_DRAMC
	last_dramc_info_ptr->storage_api_err_flag = g_dram_storage_api_err_code;
	dsb();
#endif
}

#endif

#if __ETT__
#define FHCTL_HP_EN	(FHCTL_BASE + 0x004)
#define MPLL_FHCTL_CFG	(FHCTL_BASE + 0x0A0)
#define MPLL_FHCTL_LMT	(FHCTL_BASE + 0x0A4)
#define MPLL_FHCTL_DDS	(FHCTL_BASE + 0x0A8)
#define MPLL_CON1	(APMIXED_BASE + 0x2A4)
#define MPLL_ID		5
#define SETTING_DF	9
#define SETTING_DT	0
#define SETTING_LOWBND	1 // ssc ratio
#define PERCENT_TO_DDSLMT(dDS, pERCENT_M10) (((dDS * pERCENT_M10) >> 5) / 100)

void en_mpll_ssc(void)
{
	unsigned int reg_val;

	*((volatile unsigned int *)0x1000CE08) = 0x1FF;
	*((volatile unsigned int *)0x1000CE0C) = 0x1FF;

	reg_val = *((volatile unsigned int *)MPLL_FHCTL_CFG);
	reg_val &= ~0xFF0000;
	*((volatile unsigned int *)MPLL_FHCTL_CFG) = reg_val | (SETTING_DF << 20) | (SETTING_DT << 16);

	reg_val = (*((volatile unsigned int *)MPLL_CON1) & 0x3FFFFF) | 0x80000000;
	*((volatile unsigned int *)MPLL_FHCTL_DDS) = reg_val;

	reg_val &= 0x3FFFFF;
	reg_val = PERCENT_TO_DDSLMT(reg_val, SETTING_LOWBND) << 16;
	*((volatile unsigned int *)MPLL_FHCTL_LMT) = reg_val;

	*((volatile unsigned int *)FHCTL_HP_EN) |= (1 << MPLL_ID);
	dsb();
	*((volatile unsigned int *)MPLL_FHCTL_CFG) |= 0x2;
	dsb();
	*((volatile unsigned int *)MPLL_FHCTL_CFG) |= 0x1;
	dsb();
}

void dis_mpll_ssc(void)
{
	*((volatile unsigned int *)MPLL_FHCTL_CFG) &= ~0x2;
	dsb();
	*((volatile unsigned int *)MPLL_FHCTL_CFG) &= ~0x1;
	dsb();
	*((volatile unsigned int *)FHCTL_HP_EN) &= ~(1 << MPLL_ID);
	dsb();
}
#endif

#if defined(SLT) && (!__ETT__)
#include <storage_api.h>
#include <emi.h>
int clean_slt_data(void)
{
	int n;
	DRAM_SLT_DATA_T data;
	data.header.stage_status = -1;
	data.header.pl_version = PL_VERSION;

	for (n = 0; n < SLT_TESTDATA_ENTRY; n++)
		data.test_result[n] = 0;

	return write_slt_data(&data);
}

int read_slt_data(DRAM_SLT_DATA_T *data)
{
	int i, ret;
	blkdev_t *bootdev = NULL;

	if (data == NULL) {
		dramc_crit("[dramc_slt] SLT data == NULL, skip\n");
	}

	bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		dramc_crit("[dramc_slt] can't find boot device(%d)\n", CFG_BOOT_DEV);
		return SLT_ERR_NO_DEV;
	}

	if (!part_dram_data_addr_slt) {
		return SLT_ERR_NO_ADDR;
	}

	ret = blkdev_read(bootdev, part_dram_data_addr_slt, sizeof(DRAM_SLT_DATA_T), (u8*)data, storage_get_part_id(STORAGE_PHYS_PART_USER));

	if (ret != 0) {
		return SLT_ERR_READ_FAIL;
	}

	if (data->header.pl_version != PL_VERSION) {
		dramc_crit("[dramc_slt] PL_VERSION mismatch\n");
		clean_slt_data();
		blkdev_read(bootdev, part_dram_data_addr_slt, sizeof(DRAM_SLT_DATA_T), (u8*)data, storage_get_part_id(STORAGE_PHYS_PART_USER));
	}

	return 0;
}

int write_slt_data(DRAM_SLT_DATA_T *data)
{
	int ret;
	blkdev_t *bootdev = NULL;

	if (data == NULL) {
		dramc_crit("[dramc_slt] data == NULL, skip\n");
		return SLT_ERR_NO_DATA;
	}

	bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		dramc_crit("[dramc_slt] can't find boot device(%d)\n", CFG_BOOT_DEV);
		return SLT_ERR_NO_DEV;
	}

	if (!part_dram_data_addr_slt) {
		return SLT_ERR_NO_ADDR;
	}

	data->header.pl_version = PL_VERSION;

	ret = blkdev_write(bootdev, part_dram_data_addr_slt, sizeof(DRAM_SLT_DATA_T), (u8*)data, storage_get_part_id(STORAGE_PHYS_PART_USER));
	if (ret != 0) {
		dramc_crit("[dramc_slt] blkdev_write failed\n");
		return SLT_ERR_WRITE_FAIL;
	}

	return 0;
}
#endif

