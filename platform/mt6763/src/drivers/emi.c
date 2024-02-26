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
#include "dramc_common.h"

#if !__ETT__
#if (FOR_DV_SIMULATION_USED==0)
#include "dram_buffer.h"
#endif
#endif

   #include "rt5738.h"
#if !__FLASH_TOOL_DA__ && !__ETT__ 
#if (FOR_DV_SIMULATION_USED==0)
   #include "platform.h"
   #include "upmu_hw.h"
   #include "pmic.h"
   #include "emi_hw.h"
#if defined(DEF_LAST_DRAMC) || defined(DEF_LAST_LPDMA)
   #include "plat_dbg_info.h"
#endif
#endif
#endif

#include "emi.h"
#include <regulator/mtk_regulator.h>

#if !__FLASH_TOOL_DA__ && !__ETT__
#include "custom_emi.h"   // fix build error: emi_settings
#endif

#if __ETT__
#include "upmu_common.h"
#include "upmu_hw.h"
#endif

#if CFG_BOOT_ARGUMENT
#define bootarg g_dram_buf->bootarg
#endif

#ifdef DRAM_CALIB_LOG
#include <mt_rtc_hw.h>
#include <rtc.h>

static U16 read_rtc(U16 unit);
static void dram_klog_init(void);
static int dram_klog_check(void);
static void dram_klog_print(void);
static void dram_klog_utest(void);
#endif //end #ifdef DRAM_CALIB_LOG
//=============================================================================
//  Definition                                                                                                                                                                                   
//=============================================================================

//=============================================================================
//  Global Variables                                                                                                                                                                         
//=============================================================================
int emi_setting_index = -1;
static int enable_combo_dis = 0;
extern DRAMC_CTX_T *psCurrDramCtx;
extern EMI_SETTINGS default_emi_setting;
#if USE_PMIC_CHIP_MT6356
#include <regulator/mtk_regulator.h>
struct mtk_regulator reg_vdram, reg_vcore, reg_vsram;
unsigned int pmic_trap_ddr_type = 0;
#endif

#ifdef LAST_DRAMC
static LAST_DRAMC_INFO_T* last_dramc_info_ptr;
#endif

#ifdef LAST_EMI
static LAST_EMI_INFO_T* last_emi_info_ptr;
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
extern u64 get_part_addr(const char *name);
static u64 part_dram_data_addr = 0;
#endif
//=============================================================================
//  External references                                                                                                                                                                   
//=============================================================================
extern DRAMC_CTX_T *psCurrDramCtx;
extern char* opt_dle_value;

void print_DBG_info(DRAMC_CTX_T *p);
void Dump_EMIRegisters(DRAMC_CTX_T *p);

#define EMI_APB_BASE    0x10219000
#define EMI_CHANNEL_APB_BASE 0x1022D000
#define INFRA_DRAMC_REG_CONFIG  0x100010b4
#if __ETT__
#define CHN0_EMI_BASE  0x1022D000
#define CHN1_EMI_BASE  0x10235000
#define EMI_CONA               (EMI_APB_BASE + 0x000)
#define EMI_CONF               (EMI_APB_BASE + 0x028)
#define EMI_CONH               (EMI_APB_BASE + 0x038)
#endif

#define CHN_EMI_CONA(base)	(base + 0x000)
#define CHN_EMI_CONC(base)	(base + 0x010)

void EMI_ESL_Setting1(void)
{
// sagy: from emi_config_3200_v6.c
	
	dsb();
  *(volatile unsigned int*)INFRA_DRAMC_REG_CONFIG |= 0xf; // (ch0,ch1 same setting ;  ch2,ch3 same setting)	
	dsb();
	
	
//C:-------------------BEGIN))= EMI Setting--------------------
//))= Row = 15-bit
#ifndef ONE_CH
  #ifdef RANK_512MB  // = > 2channel = dual rank = total= 2G
    *((UINT32P)(EMI_APB_BASE+0x00000000))= 0xa053a154; //10:Row= 15bits= 11:Row= 16bits
  #else  //RANK_1G  = > 2channel = dual rank = total= 4G
    *((UINT32P)(EMI_APB_BASE+0x00000000))= 0xf053f154; //2CH
  #endif  
#else
  #ifdef RANK_512MB  
    *((UINT32P)(EMI_APB_BASE+0x00000000))= 0xa053a054; //1CH
  #else  
    *((UINT32P)(EMI_APB_BASE+0x00000000))= 0xf053f054; //1CH
  #endif  
#endif
//))= Row = 14-bit;
//*((UINT32P)(EMI_APB_BASE+0x00000000))= 0x50505154;
*((UINT32P)(EMI_APB_BASE+0x00000008))= 0x17283544;
*((UINT32P)(EMI_APB_BASE+0x00000010))= 0x0a1a0b1a;
*((UINT32P)(EMI_APB_BASE+0x00000018))= 0x3657587a;
*((UINT32P)(EMI_APB_BASE+0x00000020))= 0xffff0848;
*((UINT32P)(EMI_APB_BASE+0x00000028))= 0x00000000;
*((UINT32P)(EMI_APB_BASE+0x00000030))= 0x2b2b2a38;
*((UINT32P)(EMI_APB_BASE+0x00000038))= 0x00000000;
#ifdef BANK_INTERLEAVE_ON
*((UINT32P)(EMI_APB_BASE+0x00000038))= 0x00000004; // enable bank interleaving
#endif
*((UINT32P)(EMI_APB_BASE+0x00000040))= 0x00008803;
*((UINT32P)(EMI_APB_BASE+0x00000060))= 0x000001ff; // reserved buffer to normal read/write no limit
#ifdef ICFP
*((UINT32P)(EMI_APB_BASE+0x00000060))= 0x009d01ff; // reserved buffer to normal read/write = 4/4
#endif
#ifdef VPWFD 
*((UINT32P)(EMI_APB_BASE+0x00000060))= 0x009d01ff; // reserved buffer to normal read/write = 4/4
#endif
#ifdef VR4K 
*((UINT32P)(EMI_APB_BASE+0x00000060))= 0x009d01ff; // reserved buffer to normal read/write = 4/4
#endif
*((UINT32P)(EMI_APB_BASE+0x00000068))= 0x00000000;
*((UINT32P)(EMI_APB_BASE+0x00000078))= 0x11128c17; //reserve buffer to ap_w[30:28]= ap_r[26:24]= u_w[22:20]= u_r[28:16]
*((UINT32P)(EMI_APB_BASE+0x0000007c))= 0x00001123; //reserve buffer to hi_w[15:12]= hi_r[10:8]= md_w[6:4]= md_r[2:0]
*((UINT32P)(EMI_APB_BASE+0x000000d0))= 0xa8a8a8a8;
*((UINT32P)(EMI_APB_BASE+0x000000d4))= 0x25252525;
*((UINT32P)(EMI_APB_BASE+0x000000d8))= 0xa8a8a8a8;
*((UINT32P)(EMI_APB_BASE+0x000000dc))= 0x25252525;
// *((UINT32P)(EMI_APB_BASE+0x000000e8))= 0x00060027;
*((UINT32P)(EMI_APB_BASE+0x000000e8))= 0x00060037; // initial starvation counter div2= [4]= 1
*((UINT32P)(EMI_APB_BASE+0x000000f0))= 0x38460000;
*((UINT32P)(EMI_APB_BASE+0x000000f8))= 0x00000000;
/////  EMI  bandwidth limit  ////////// default UI
//#ifdef UI
*((UINT32P)(EMI_APB_BASE+0x00000100))= 0x80407244;
*((UINT32P)(EMI_APB_BASE+0x00000108))= 0x80407044;
*((UINT32P)(EMI_APB_BASE+0x00000110))= 0xa0a050c6;
*((UINT32P)(EMI_APB_BASE+0x00000118))= 0x000070cc;
*((UINT32P)(EMI_APB_BASE+0x00000120))= 0x40406045;
*((UINT32P)(EMI_APB_BASE+0x00000128))= 0xa0a050d5;
*((UINT32P)(EMI_APB_BASE+0x00000130))= 0xb0b07046;
*((UINT32P)(EMI_APB_BASE+0x00000138))= 0xb0b07046;
//#endif
// scenario setting from Alaska begin
#ifdef VPWFD
*((UINT32P)(EMI_APB_BASE+0x00000100))= 0x8fb07241;
*((UINT32P)(EMI_APB_BASE+0x00000108))= 0x8fb07041;
*((UINT32P)(EMI_APB_BASE+0x00000110))= 0xa0a050d8;
*((UINT32P)(EMI_APB_BASE+0x00000118))= 0x000070c3;
*((UINT32P)(EMI_APB_BASE+0x00000120))= 0x40406048;
*((UINT32P)(EMI_APB_BASE+0x00000128))= 0xa0a050d7;
*((UINT32P)(EMI_APB_BASE+0x00000130))= 0xb0b07042;
*((UINT32P)(EMI_APB_BASE+0x00000138))= 0xb0b07042;
#endif
#ifdef VR4K
*((UINT32P)(EMI_APB_BASE+0x00000100))= 0xc0c07241;
*((UINT32P)(EMI_APB_BASE+0x00000108))= 0xc0c07041;
*((UINT32P)(EMI_APB_BASE+0x00000110))= 0xa0a050da;
*((UINT32P)(EMI_APB_BASE+0x00000118))= 0x000070c4;
*((UINT32P)(EMI_APB_BASE+0x00000120))= 0x40406049;
*((UINT32P)(EMI_APB_BASE+0x00000128))= 0xa0a050d3;
*((UINT32P)(EMI_APB_BASE+0x00000130))= 0xc0c07042;
*((UINT32P)(EMI_APB_BASE+0x00000138))= 0xc0c07042;
#endif
//scenario setting from Alaska end
#ifdef ICFP
*((UINT32P)(EMI_APB_BASE+0x00000100))= 0xafb07245;
*((UINT32P)(EMI_APB_BASE+0x00000108))= 0xafb07045;
*((UINT32P)(EMI_APB_BASE+0x00000110))= 0xa0a050cb;
*((UINT32P)(EMI_APB_BASE+0x00000118))= 0x000070cc;
*((UINT32P)(EMI_APB_BASE+0x00000120))= 0x40406046;
*((UINT32P)(EMI_APB_BASE+0x00000128))= 0xa0a050d6;
*((UINT32P)(EMI_APB_BASE+0x00000130))= 0xb0b07041;
*((UINT32P)(EMI_APB_BASE+0x00000138))= 0xb0b07042;
#endif
////////////////////////////////////////////////////

*((UINT32P)(EMI_APB_BASE+0x00000140))= 0x00007108;
*((UINT32P)(EMI_APB_BASE+0x00000144))= 0x00007108;
*((UINT32P)(EMI_APB_BASE+0x00000158))= 0x0001ff00;
//*((UINT32P)(EMI_APB_BASE+0x00000400))= 0x00ff0001; //[27:20] enable monitor
//C:-------------------END))= EMI Setting--------------------;
//C:-------------------BEGIN))= CHANNEL EMI Setting--------------------;
#ifdef RANK_512MB  // = > 2channel = dual rank = total= 2G
  *((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000000))= 0x0400a051;
#else  //RANK_1G  = > 2channel = dual rank = total= 4G
  *((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000000))= 0x0400f051;
#endif  
#ifdef BANK_INTERLEAVE_ON
*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000000))= 0x2400f051; // enable bank interleaving
#endif
*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000008))= 0x00ff2048; // over BW limit= starvation slow down= [13:12]= 2
*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000010))= 0x00000000;
*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000018))= 0x88008c17;
*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000048))= 0x00030027; //RD_INORDER_THR[20:16]= 3
*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000050))= 0x38460002; // [1] : MD_RD_AFT_WR_EN
#ifdef MD_FAVOR
//*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000050))= 0x38470000;
//*((UINT32P)(EMI_APB_BASE+0x00000078))= 0x9a99fc3f; // [15:12] = 0xf : SBR
//*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000018))= 0x0000cc7f; // [15:14] = 0x3 : SBR
//*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000158))= (*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000158)) & 0xff0808ff); // [23:8] = 0x0808 : SBR
//*((UINT32P)(EMI_APB_BASE+0x0001200c))= (*((UINT32P)(EMI_APB_BASE+0x0001200c)) | 0x00c08000); // [23] = 0x1= [22] = 0x1= [15] = 0x1 : SBR enable= SBR aggressive enable= RWLS
#endif
//*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000158))= (*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000158)) | 0x00101000); // enable LLAT to M4
*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000058))= 0x00000000;
*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000140))= 0x20406188;
*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000144))= 0x20406188;
*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000148))= 0x3719595e;
*((UINT32P)(EMI_CHANNEL_APB_BASE+0x0000014c))= 0x3719595e;
*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000150))= 0x64f3fc79;
*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000154))= 0x64f3fc79;
#ifdef ICFP
//*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000158))= ((*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000158)) & 0xffffff0f)| 0x00000060); // reserve buffer to MDMCU 1
*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000158))= 0x00080868;
*((UINT32P)(EMI_CHANNEL_APB_BASE+0x0000015c))= 0x88410222; //STRICT_MSK_ULTRA_EN [5]= 1
#else
*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000158))= 0x00080888;
*((UINT32P)(EMI_CHANNEL_APB_BASE+0x0000015c))= 0x82410222; //STRICT_MSK_ULTRA_EN [5]= 1
#endif
//*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000160))= 0x0a000013; // rank0 grouping for CKE // rank total>10= expire>0,ultra>0 stop grouping
//*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000164))= 0x0a000211; // rank1 grouping for CKE // rank total>10= expire>0,ultra>2 stop grouping 
//*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000168))= 0x20f26055; // rank1 DRS
//*((UINT32P)(EMI_CHANNEL_APB_BASE+0x0000016c))= 0x0000f801;
//*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000170))= 0x40000000;
*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000710))= 0x8a228c17; // [24:20] = 0x2 : bank throttling (default= 0x01f00000)

*((UINT32P)(EMI_CHANNEL_APB_BASE+0x000001b0))= 0x0006002f; // Rank-Aware arbitration
*((UINT32P)(EMI_CHANNEL_APB_BASE+0x000001b4))= 0x01010101; // Rank-Aware arbitration
*((UINT32P)(EMI_CHANNEL_APB_BASE+0x000001b8))= 0x20201840; // Rank-Aware arbitration
*((UINT32P)(EMI_CHANNEL_APB_BASE+0x000003fc))= 0x00000000; // Write M17_toggle_mask =0!!!!
//#ifdef MD_BANK_PRE_MASK
	#ifdef ICFP
		*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000080))= 0xb00;
		*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000088))= 0xb00;
	#else
		*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000080))= 0xf00;
		*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000088))= 0xb00;
	#endif
//#endif  
//C:-------------------END))= CHANNEL EMI Setting--------------------;
}

void EMI_ESL_Setting2(void)
{
unsigned int  rd_tmp;

	dsb();
  *(volatile unsigned int*)INFRA_DRAMC_REG_CONFIG |= 0xf; // sagy, ch0,ch1 same setting ;  ch2,ch3 same setting)	
  dsb();
//////////////////////////////////////////// 
//
// Bianco Patch for single channel bug
//
////////////////////////////////////////////

if ((*((UINT32P)(EMI_APB_BASE+0x0)) & 0x300) == 0x0){  
*((UINT32P)(EMI_CHANNEL_APB_BASE+0x00000048))&= ~(0x1f<<16); //RD_INORDER_THR[20:16]= 0
}


// CHN_EMI enable after DRAMC setting done
 // CHN_EMI enable after DRAMC setting done
rd_tmp = *((volatile unsigned *)(EMI_CHANNEL_APB_BASE+0x000010));
*((volatile unsigned *)(EMI_CHANNEL_APB_BASE+0x000010))=rd_tmp | 0x1;		// CHN_EMI_CONC :	[31: 1] --> EMI APB Configuration & Address Mapping tool

// CEN_EMI enable after CHN_EMI setting done
rd_tmp = *((volatile unsigned *)(EMI_APB_BASE+0x00000060));
*((volatile unsigned *)(EMI_APB_BASE+0x00000060))      =rd_tmp | 0x400;		// EMI_CONM :		[10]	--> EMI enable, 
										// 			[ 9: 8]	--> Normail age speed, 
										//			[ 7: 0]	--> Page hit high priority enable
										//			[0]     --> MUST be 1
  dsb();
  *(volatile unsigned int*)INFRA_DRAMC_REG_CONFIG &= ~0xf; // sagy, ch0,ch1 same setting ;  ch2,ch3 same setting)	
  dsb();
}


void EMI_Patch(void)
{
	unsigned int emi_conm;

	//The following is EMI patch

#if !__ETT__
	// Enable MPU violation interrupt to MD for D1 and D7
	*((volatile unsigned int *)EMI_MPU_CTRL_D(1)) |= 0x10;
	*((volatile unsigned int *)EMI_MPU_CTRL_D(7)) |= 0x10;

	// EMI setting for MBW
	if (get_dram_channel_nr() == 1) 
		*((volatile unsigned int *)EMI_BWCT0) = 0x05000407;
	else if (get_dram_channel_nr() == 2)
		*((volatile unsigned int *)EMI_BWCT0) = 0x0a000707;
	else
		print("[EMI MBW] unsupported channel number\n");

	// EMI setting for ELM
	emi_conm = *((volatile unsigned int *)EMI_CONM);
	*((volatile unsigned int *)EMI_CONM) = (emi_conm & 0x00ffffff) | 0x40000000;
	*((volatile unsigned int *)EMI_CONM) &= 0xfffffffc;
	*((volatile unsigned int *)EMI_BMEN) = 0x00ff0000;
	*((volatile unsigned int *)EMI_MSEL) = 0x00240003;
	*((volatile unsigned int *)EMI_MSEL2) = 0x00000018;
	*((volatile unsigned int *)EMI_BMEN2) = 0x02000000;
	*((volatile unsigned int *)EMI_BMRW0) = 0xffffffff;
	*((volatile unsigned int *)EMI_CONM) = emi_conm;

	*((volatile unsigned int *)EMI_LTCT0_2ND) = 0x0000cc08;
	if (pmic_trap_ddr_type == TYPE_LPDDR3) {
		*((volatile unsigned int *)EMI_LTCT1_2ND) = 0x09c40b5b;
		*((volatile unsigned int *)EMI_LTCT2_2ND) = 0x00000754;
		*((volatile unsigned int *)EMI_LTCT3_2ND) = 0x007194ad;
	} else {
		*((volatile unsigned int *)EMI_LTCT1_2ND) = 0x04e209c4;
		*((volatile unsigned int *)EMI_LTCT2_2ND) = 0x00000754;
		*((volatile unsigned int *)EMI_LTCT3_2ND) = 0x00714a94;
	}

	// EMI QoS0.5
	*((volatile unsigned int *) 0x10219400) = 0x00FF0000;
	*((volatile unsigned int *) 0x10219440) = 0x00C00003;
	*((volatile unsigned int *) 0x10219468) = 0x00000024;

#ifdef LAST_EMI
	last_emi_info_ptr = (LAST_EMI_INFO_T *) get_dbg_info_base(KEY_LAST_EMI);
	last_emi_info_ptr->decs_magic = LAST_EMI_MAGIC_PATTERN;
	last_emi_info_ptr->decs_ctrl = 0xDEC8DEC8;
	last_emi_info_ptr->decs_dram_type = 0;
	last_emi_info_ptr->decs_diff_us = 0;
#endif
#endif
}

void EMI_Init(DRAMC_CTX_T *p)
{
	EMI_SETTINGS *emi_set;
	
	if(emi_setting_index == -1)
		emi_set = &default_emi_setting;
#if (FOR_DV_SIMULATION_USED==0)
	else
		emi_set = &emi_settings[emi_setting_index];
#endif

	EMI_ESL_Setting1(); //Copy Paste from DE
	
	//The following is MDL settings
	*(volatile unsigned *)EMI_CONA = emi_set->EMI_CONA_VAL;
	*(volatile unsigned *)EMI_CONH = emi_set->EMI_CONH_VAL;

	// CHNA and CHNB uses the same CH0 setting
	*(volatile unsigned *)CHN_EMI_CONA(CHN0_EMI_BASE) = emi_set->CHN0_EMI_CONA_VAL;

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

    p->vendor_id = emi_set->iLPDDR3_MODE_REG_5;
}

void EMI_Init2(void)
{
	EMI_ESL_Setting2(); //Copy Paste from DE
	
	EMI_Patch(); //Please add the EMI patch here
}

static unsigned int emi_conh = 0;
static unsigned int chn_emi_cona[4] = {0, 0, 0, 0};

#define MIN_PER_RANK_SZ_PER_CHAN	0x10000000 // 256MB

/* return the start address of rank1 */
unsigned int set_emi_before_rank1_mem_test(void)
{
	unsigned int channels;

	emi_conh = *(volatile unsigned int*)EMI_CONH;
	chn_emi_cona[0] = *(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_BASE);
	chn_emi_cona[1] = *(volatile unsigned int*)CHN_EMI_CONA(CHN1_EMI_BASE);

	channels = *(volatile unsigned int*)EMI_CONA;
	channels = (channels >> 8) & 0x3;
	channels = 1 << channels;

	/* set all the rank size of all the channels to the minimum value */
	*(volatile unsigned int*)EMI_CONH = ((*(volatile unsigned int*)EMI_CONH & 0x0000ffff) | (0x11110000));

	/* set all the rank size of all channel-0 to the minimum value */
	*(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_BASE)
		= ((*(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_BASE) & 0xff00ffff ) | (0x00110000));

	/* set all the rank size of all channel-1 to the minimum value */
	*(volatile unsigned int*)CHN_EMI_CONA(CHN1_EMI_BASE)
		= ((*(volatile unsigned int*)CHN_EMI_CONA(CHN1_EMI_BASE) & 0xff00ffff ) | (0x00110000));

	dsb();
	return MIN_PER_RANK_SZ_PER_CHAN * channels;
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
  return u4IO32ReadFldAlign(DRAMC_REG_ARBCTL, ARBCTL_RSV_DRAM_TYPE);
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

#include "pmic.h"
#define	CHAN_DRAMC_NAO_MISC_STATUSA(base)	(base + 0x80)
#define SREF_STATE				(1 << 16)

static unsigned int is_dramc_exit_slf(void)
{
	unsigned int ret;
	U32 u4DramType = 0;

	u4DramType = (*((volatile unsigned int *)(0x1022A010)) >> 10 & 0x7);
	ret = *(volatile unsigned *)CHAN_DRAMC_NAO_MISC_STATUSA(Channel_A_DRAMC_NAO_BASE_ADDRESS);
	if ((ret & SREF_STATE) != 0) {
		print("DRAM CHAN-A is in self-refresh (MISC_STATUSA = 0x%x)\n", ret);
		return 0;
	}

if(u4DramType >=2) //LP3:1, LP4:2, LP4X:3
{
	ret = *(volatile unsigned *)CHAN_DRAMC_NAO_MISC_STATUSA(Channel_B_DRAMC_NAO_BASE_ADDRESS);
	if ((ret & SREF_STATE) != 0) {
		print("DRAM CHAN-B is in self-refresh (MISC_STATUSA = 0x%x)\n", ret);
		return 0;
	}
}
	print("ALL DRAM CHAN is not in self-refresh\n");
	return 1;
}

#endif

static void restore_vcore_setting(void)
{
#if (FOR_DV_SIMULATION_USED==0)
#if USE_PMIC_CHIP_MT6356
	int ret;

	ret = mtk_regulator_get("vcore", &reg_vcore);
	if (ret)
		printf("mtk_regulator_get vcore fail\n");

#if __FLASH_TOOL_DA__
	mtk_regulator_set_voltage(&reg_vcore, 700000, MAX_VCORE);
#else
#if defined(SLT) && defined(SLT_VCORE_LP4)
	mtk_regulator_set_voltage(&reg_vcore, SLT_VCORE_LP4, MAX_VCORE);
#else	
	mtk_regulator_set_voltage(&reg_vcore, SEL_PREFIX_VCORE(LP4, KOPP0), MAX_VCORE);
#endif
#endif
	printf("Vcore = %d\n", mtk_regulator_get_voltage(&reg_vcore));

#endif
#endif
}

static unsigned int lp3_vdram_base[] = { 1100000, 1200000 };
static unsigned int lp3_vdram_ofs[] = { 20000, 40000 };

void dramc_set_lp3_vdram(unsigned int voltage)
{
	int ret;

	ret = mtk_regulator_get("vdram", &reg_vdram);

	if (ret) {
		printf("mtk_regulator_get vdram fail\n");
		return ;
	}

	if (voltage < lp3_vdram_base[0]) {
		voltage = lp3_vdram_base[0];
		printf("voltage too low, reset to lowest=%u\n", voltage);
	}

	for (ret = 0;(unsigned int)ret < sizeof(lp3_vdram_base) / sizeof(*lp3_vdram_base);ret++) {
		if (voltage < lp3_vdram_base[ret]) {
			if (ret > 0)
				ret -= 1;
			break;
		}
	}

	if (ret == sizeof(lp3_vdram_base) / sizeof(*lp3_vdram_base))
		ret -= 1;

	voltage -= lp3_vdram_base[ret];
	if (voltage > 100000) {
		voltage = 100000;
		printf("voltage offset too high, reset to highest=%u\n", voltage);
	}

	voltage /= 10000;

	mtk_regulator_set_voltage(&reg_vdram, lp3_vdram_base[ret] + lp3_vdram_ofs[ret], 1300000);
	pmic_config_interface(PMIC_RG_VDRAM_VOCAL_ADDR, voltage, PMIC_RG_VDRAM_VOCAL_MASK, PMIC_RG_VDRAM_VOCAL_SHIFT);
}

unsigned int dramc_get_lp3_vdram(void)
{
	unsigned int sum;
	int ret;

	ret = mtk_regulator_get("vdram", &reg_vdram);

	if (ret) {
		printf("mtk_regulator_get vdram fail\n");
		return 0;
	}

	sum = mtk_regulator_get_voltage(&reg_vdram);

	for (ret = 0;(unsigned int)ret < sizeof(lp3_vdram_base) / sizeof(*lp3_vdram_base);ret++) {
		if (sum == lp3_vdram_base[ret] + lp3_vdram_ofs[ret]) {
			break;
		}
	}

	pmic_read_interface(PMIC_RG_VDRAM_VOCAL_ADDR, &sum, PMIC_RG_VDRAM_VOCAL_MASK, PMIC_RG_VDRAM_VOCAL_SHIFT);

	if (ret < 2)
		return lp3_vdram_base[ret] + (sum * 10000);
	else
		return 0;
}

static void restore_pmic_setting(void)
{
#if (FOR_DV_SIMULATION_USED==0)
#if USE_PMIC_CHIP_MT6356
	int ret;

	restore_vcore_setting();

	if (pmic_trap_ddr_type == TYPE_LPDDR3) {
		/* LPDDR3 */
		dramc_set_lp3_vdram(SEL_PREFIX_VDRAM);
	} else {
		/* LPDDR4 */
		rt5738_set_voltage(RT5738_VDD2, SEL_PREFIX_VDD2);
		rt5738_set_voltage(RT5738_VDDQ, SEL_PREFIX_VDDQ);
	}

	if (pmic_trap_ddr_type == TYPE_LPDDR3) {
		/* LPDDR3 */
		printf("Vdram = %d\n", dramc_get_lp3_vdram());
	} else {
		/* LPDDR4 */
		printf("Vdram = %d\n", rt5738_get_voltage(RT5738_VDD2));
		printf("Vddq = %d\n", rt5738_get_voltage(RT5738_VDDQ));
	}
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
    print("[DDR reserve] EMI CONA: %x\n", *(volatile unsigned int*)EMI_CONA);
    print("[DDR reserve] EMI CHA CONA: %x\n", *(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_BASE));
    print("[DDR reserve] EMI CHB CONA: %x\n", *(volatile unsigned int*)CHN_EMI_CONA(CHN1_EMI_BASE));
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
            print("[DDR Reserve] release dram from self-refresh FAIL!\n");
            g_ddr_reserve_success = 0;
        }
    }
    else
    {
         print("[DDR Reserve] release dram from self-refresh PASS!\n");
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
          print("[DDR Reserve] ddr reserve mode success but DRAM not in self-refresh!\n");
          g_ddr_reserve_success = 0;
#ifdef LAST_DRAMC
	  dram_fatal_set_ddr_rsv_mode_err();
#endif
        }
      }
      else
      {
        print("[DDR Reserve] ddr reserve mode FAIL!\n");
        g_ddr_reserve_success = 0;
#ifdef LAST_DRAMC
	  dram_fatal_set_ddr_rsv_mode_err();
#endif
      }
	/* Disable DDR-reserve mode in pre-loader stage then enable it again in kernel stage */
	rgu_dram_reserved(0);
	
	/* overwrite g_ddr_reserve_success if some of dcs/dvfsrc/drs failed */
	/* TODO: check DRS status */
	if ((dcs_en == 1 && dcs_success == 0) || (dvfsrc_en == 1 && dvfsrc_success == 0)) {
		print("[DDR Reserve] DRAM content might be corrupted -> clear g_ddr_reserve_success\n");
		g_ddr_reserve_success = 0;

		if (dvfsrc_en == 1 && dvfsrc_success == 0) {
			print("[DDR Reserve] DVFSRC fail!\n");
#if 0//def LAST_DRAMC
			dram_fatal_set_dvfsrc_err();
#endif
		}

		if (dcs_en == 1 && dcs_success == 0) {
			print("[DDR Reserve] DCS fail!\n");
#if 0 //def LAST_DRAMC
			dram_fatal_set_emi_dcs_err();
#endif
		}
	} else {
		print("[DDR Reserve] DCS/DVFSRC success! (dcs_en=%d, dvfsrc_en=%d)\n", dcs_en, dvfsrc_en);
	}
	/* release dram, no matter success or failed */
	release_dram();
    }
    else
    {
      print("[DDR Reserve] ddr reserve mode not be enabled yet\n");
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
    return pmic_trap_ddr_type;
}

#ifdef COMBO_MCP
static int mt_get_mdl_number(void);
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
		printf("orig_dram_info[%d] start: 0x%llx, size: 0x%llx\n",
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
			print("[dramc] reduce rank size = %u\n", (unsigned int) orig_dram_info->rank_num);
			break;
		}
	}
#endif
}

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

    unsigned int dram_type;
    DRAM_INFO_BY_MRR_T DramInfo;
    DRAM_DRAM_TYPE_T Dis_DramType;

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
        if ((discrete_dram_num > 0) && (num_of_emi_records >= 2)) {
            enable_combo_dis = 1;
#if 0 // Alaska will have only LPDDR4 and LPDDR4X
            /* if we enable combo discrete dram
             * check all dram are all same type
             * */
            dram_type = emi_settings[0].type & 0x000F;
            for (i = 0 ; i < num_of_emi_records; i++) {
                if ((dram_type != (emi_settings[i].type & 0x000F)) && (dram_type != TYPE_LPDDR4) && (dram_type != TYPE_LPDDR4X)) { //Darren
                    print("[EMI] Combo discrete dram only support when combo lists are all same dram type.");
                    ASSERT(0);
                }
                dram_type = emi_settings[i].type & 0x000F;
            }
#endif
        }
        print("[EMI] mcp_dram_num:%d,discrete_dram_num:%d,enable_combo_dis:%d\r\n",mcp_dram_num,discrete_dram_num,enable_combo_dis);

        /*
         *
         * 0. if there is only one discrete dram, use index=0 emi setting and boot it.
         * */
        if ((0 == mcp_dram_num) && (1 == discrete_dram_num)) {
            mdl_number = 0;
            found = 1;
            
#if ENABLE_MRR_AFTER_FIRST_K 
            //K first frequency (1066MHz)
            Dis_DramType = mt_get_dram_type_for_dis();
#if 0 // Alaska will only use LPDDR4X
            if ((Dis_DramType == TYPE_LPDDR4) || (Dis_DramType == TYPE_LPDDR4X)) {
                if ((is_fan53528buc08x_exist() == 1) && (is_fan53526_exist() == 1))
                    Dis_DramType = TYPE_LPDDR4X;
                else if ((is_fan53528buc08x_exist() == 0) && (is_fan53526_exist() == 1))
                    Dis_DramType = TYPE_LPDDR4;
                else {
                    print("[DRAMC] Discrete DRAM buck check fails !!!\n");
                    ASSERT(0);
                }
            }
#endif
            Init_DRAM(Dis_DramType, CBT_NORMAL_MODE, &DramInfo, GET_MDL_USED);
#endif			  
            return mdl_number;
        }

        /* 1.
         * if there is MCP dram in the list, we try to find emi setting by emmc ID
         * */
        if (mcp_dram_num > 0) {
            result = platform_get_mcp_id(id, emmc_nand_id_len,&fw_id_len);

            for (i = 0; i < num_of_emi_records; i++) {
                if (emi_settings[i].type != 0) {
                    if ((emi_settings[i].type & 0xF00) != 0x000) {
                        /* valid ID */
                        if (result == 0) {
                            if ((emi_settings[i].type & 0xF00) == 0x200) {
                                /* eMMC */
                                if (memcmp(id, emi_settings[i].ID, emi_settings[i].id_length) == 0) {
                                    mdl_number = i;
                                    found = 1;
                                    break;
                                } else {
                                    print("[MDL] index(%d) emmc id match failed\n",i);
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
                                    print("[MDL] index(%d) ufs id match failed\n",i);
                                }
                            }
#if 0 // Alaska will not use NAND
                            else if ((emi_settings[i].type & 0xF00) == 0x100) {
                                /* NAND */
                                if (memcmp(id, emi_settings[i].ID, emi_settings[i].id_length) == 0){
                                    memset(id + emi_settings[i].id_length, 0, sizeof(id) - emi_settings[i].id_length);                                
                                    mdl_number = i;
                                    found = 1;
                                    break;
                                } else {
                                    print("[MDL] index(%d) ufs id match failed\n",i);
                                }
                            }
#endif
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
            u64 rank0_size=0, rank1_size=0;

            Dis_DramType = mt_get_dram_type_for_dis();
#if 0 // Alaska will only support LPDDR4X
            if ((Dis_DramType == TYPE_LPDDR4) || (Dis_DramType == TYPE_LPDDR4X)) {
                if ((is_fan53528buc08x_exist() == 1) && (is_fan53526_exist() == 1))
                    Dis_DramType = TYPE_LPDDR4X;
                else if ((is_fan53528buc08x_exist() == 0) && (is_fan53526_exist() == 1))
                    Dis_DramType = TYPE_LPDDR4;
                else {
                    print("[DRAMC] Discrete DRAM buck check fails !!!\n");
                    ASSERT(0);
                }
            }
#endif

            Init_DRAM(Dis_DramType, CBT_NORMAL_MODE, &DramInfo, GET_MDL_USED);
  	 
            // try to find discrete dram by MR5 (vendor ID)
            for (i = 0; i < num_of_emi_records; i++) {
                print("[MDL]index:%d, MR5:%x, type:%x, vender_id:%x\n", i, emi_settings[i].iLPDDR3_MODE_REG_5, emi_settings[i].type, DramInfo.u2MR5VendorID);
                // only check discrete dram type
                if ((emi_settings[i].type & 0xF) == Dis_DramType && (emi_settings[i].type & 0x0F00) == 0x0000) {
                    // support for combo discrete dram 
                    if (emi_settings[i].iLPDDR3_MODE_REG_5 == DramInfo.u2MR5VendorID) {
                        //rank0_size = (u64)DramInfo.u8MR8Density[0][0] + (u64)DramInfo.u8MR8Density[1][0];
                        //rank1_size = (u64)DramInfo.u8MR8Density[0][1] + (u64)DramInfo.u8MR8Density[1][1];
                        rank0_size = (u64) DramInfo.u8MR8Density[0][0]; //now only K CHA to save time
                        rank1_size = (u64) DramInfo.u8MR8Density[0][1]; //now only K CHA to save time

                        printf("[MDL] index:%d, rank0_size:0x%llx(0x%llx), rank1_size:0x%llx(0x%llx)\n", i,
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

        print("found:%d,i:%d\n",found,i);
    }

    return mdl_number;
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
		    print("invalid CHN_EN field in EMI_CONA (0x%x)\n", emi_cona);
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

    printf("DRAM rank0 size:0x%llx,\nDRAM rank1 size=0x%llx\n", dram_rank_size[0], dram_rank_size[1]);
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

            printf("%d:dram_rank_size:%llx\n",i,dram_rank_size[i]);
        }
    }
    return;
#else
    get_dram_rank_size_by_EMI_CONA(dram_rank_size);
    return;        
#endif
}
#endif //#if !__FLASH_TOOL_DA__ && !__ETT__ 
#endif


#include "pmic.h"

#if USE_PMIC_CHIP_MT6356
unsigned int mt_get_dram_type_from_hw_trap(void)
{
	unsigned int hw_trap;
	hw_trap = get_dram_type();

	switch (hw_trap) {
	case 0:
		return TYPE_LPDDR3;
	case 1:
		return TYPE_LPDDR4;
	case 2:
		return TYPE_LPDDR4X;
	case 3:
		return TYPE_LPDDR4P;
	default:
		printf("[dramc] Wrong HW TRAP\n");
		ASSERT(0);
		break;
	}
	return 0;
}
#endif

void setup_dramc_voltage_by_pmic(void)
{
#if (FOR_DV_SIMULATION_USED==0)
#if USE_PMIC_CHIP_MT6356
	int ret;

	pmic_trap_ddr_type = mt_get_dram_type_from_hw_trap();
#if !__ETT__	
	if (((seclib_get_devinfo_with_index(25) & 0x200) >> 9) && (pmic_trap_ddr_type != TYPE_LPDDR3)) {
		print("[dramc] LP3 IC + LP4 DRAM, Wrong Combination. \n");
		ASSERT(0);
	}
#endif
	printf("PMIC TRAP GET DDR TYPE: 0x%x\n", pmic_trap_ddr_type);

	ret = mtk_regulator_get("vcore", &reg_vcore);
	if (ret)
		printf("mtk_regulator_get vcore fail\n");

	ret = mtk_regulator_get("vdram", &reg_vdram);
	if (ret)
		printf("mtk_regulator_get vdram fail\n");

	if (pmic_trap_ddr_type == TYPE_LPDDR3) {
		/* LPDDR3 */
	} else {
		/* LPDDR4 */
		ret = mtk_regulator_enable(&reg_vdram, 0);
		if (ret)
			printf("disable reg_vdram failed\n");

		ret = rt5738_enable(RT5738_VDD2, 1);
		if (ret == 0) {
			printf("rt5738_enable RT5738_VDD2 fail (ret = %d)\n", ret);
			return;
		}

		ret = rt5738_enable(RT5738_VDDQ, 1);
		if (ret == 0) {
			printf("rt5738_enable RT5738_VDDQ fail (ret = %d)\n", ret);
			return;
		}
	}

	mtk_regulator_set_mode(&reg_vcore, 0x1);
#if __FLASH_TOOL_DA__
	mtk_regulator_set_voltage(&reg_vcore, 700000, MAX_VCORE);
#else
	mtk_regulator_set_voltage(&reg_vcore, SEL_PREFIX_VCORE(LP4, KOPP0), MAX_VCORE);
#endif

	if (pmic_trap_ddr_type == TYPE_LPDDR3) {
		/* LPDDR3 */
// 6356 vdram no PWM mode
//		mtk_regulator_set_mode(&reg_vdram, 0x1);

		dramc_set_lp3_vdram(SEL_PREFIX_VDRAM);
	} else {
		/* LPDDR4 */
		rt5738_set_mode(RT5738_VDD2, 1);
		rt5738_set_mode(RT5738_VDDQ, 1);

		rt5738_set_voltage(RT5738_VDD2, SEL_PREFIX_VDD2);
		rt5738_set_voltage(RT5738_VDDQ, SEL_PREFIX_VDDQ);
	}

	printf("Vcore = %d\n", mtk_regulator_get_voltage(&reg_vcore));

	if (pmic_trap_ddr_type == TYPE_LPDDR3) {
		/* LPDDR3 */
		printf("Vdram = %d\n", dramc_get_lp3_vdram());
	} else {
		/* LPDDR4 */
		printf("Vdram = %d\n", rt5738_get_voltage(RT5738_VDD2));
		printf("Vddq = %d\n", rt5738_get_voltage(RT5738_VDDQ));
	}
#endif
#endif
}

void switch_dramc_voltage_to_auto_mode(void)
{
#if (FOR_DV_SIMULATION_USED==0)
#if USE_PMIC_CHIP_MT6356

	mtk_regulator_set_mode(&reg_vcore, 0x0);

	if (pmic_trap_ddr_type == TYPE_LPDDR3) {
		/* LPDDR3 */
// 6356 vdram no PWM mode
//		mtk_regulator_set_mode(&reg_vdram, 0x0);
	} else {
		/* LPDDR4 */
		rt5738_set_mode(RT5738_VDD2, 0);
		rt5738_set_mode(RT5738_VDDQ, 0);
	}
#endif
#endif
}

#if (FOR_DV_SIMULATION_USED==0)
#if !__ETT__
void mt_set_emi(void)
{
    int index;
    /*unsigned int SW_CTRL_VC, HW_CTRL_VC;*/
    EMI_SETTINGS *emi_set;
#if ENABLE_MRR_AFTER_FIRST_K
    DRAM_INFO_BY_MRR_T DramInfo;
#endif

#ifdef DRAM_CALIB_LOG
    dram_klog_init();
//    dram_klog_utest();
#endif

	// set voltage and hw trapping before mdl
    setup_dramc_voltage_by_pmic();

#ifdef COMBO_MCP
    index = mt_get_mdl_number();
    print("[EMI] MDL number = %d\r\n", index);
    if (index < 0 || index >=  num_of_emi_records)
    {
        print("[EMI] setting failed 0x%x\r\n", index);
        ASSERT(0);
    }
    else
    {
        emi_setting_index = index;
        emi_set = &emi_settings[emi_setting_index];
    }
#else
	print("[EMI] ComboMCP not ready, using default setting\n");
	emi_setting_index = -1;
	emi_set = &default_emi_setting;
#endif
  
#ifdef DDR_RESERVE_MODE
    if(g_ddr_reserve_enable==1 && g_ddr_reserve_success==0)
        Before_Init_DRAM_While_Reserve_Mode_fail(emi_set->type & 0xF);
#endif
#if ENABLE_MRR_AFTER_FIRST_K
    Init_DRAM((emi_set->type & 0xF), emi_set->dram_cbt_mode_extern, &DramInfo, FIRST_USED);
#endif

    Init_DRAM((emi_set->type & 0xF), emi_set->dram_cbt_mode_extern, NULL, NORMAL_USED);
    switch_dramc_voltage_to_auto_mode();
    restore_vcore_setting();
        
#if 0  
	{
		DRAMC_CTX_T * p = psCurrDramCtx;
		DramcRegDump(p);
	}
#endif
}

#if 0 //remove unused function to save space of log string
uint32 mt_set_emis(uint8* emi, uint32 len, bool use_default) //array of emi setting.
{
    EMI_SETTINGS *v_emi_settings = (EMI_SETTINGS*)emi;

    mcSHOW_DBG_MSG(("v_emi_settings->sub_version = 0x%x\n",v_emi_settings->sub_version));
    if(use_default)
    {
        if(v_emi_settings->sub_version == 0x1)
        {
            mcSHOW_DBG_MSG(("sub_version == 01, Using preloader tag\n"));
            memcpy(emi_settings,emi,len);
            num_of_emi_records = len/sizeof(EMI_SETTINGS);
            mcSHOW_DBG_MSG(("num_of_emi_records = %d\n",num_of_emi_records));
        }
        else
        {
            mcSHOW_DBG_MSG(("Use default EMI.\n"));
        }
        mt_set_emi();
    }
    else
    {
        mcSHOW_DBG_MSG(("EMI_TAG_Version == 25, Using preloader tag\n"));
        memcpy(emi_settings,emi,len);
        num_of_emi_records = len/sizeof(EMI_SETTINGS);
        mcSHOW_DBG_MSG(("num_of_emi_records = %d\n",num_of_emi_records));
        mt_set_emi();
    }
    mcSHOW_DBG_MSG(("EMI Setting OK.\n"));
    return 0;
}
#endif //remove unused function to save space of log string
#endif
#endif

#ifdef DRAM_CALIB_LOG
static U16 read_rtc(U16 unit)
{
    U32 time=0;
    pwrap_read((U32)unit, &time);
    return (U16)time;
}

void dram_klog_clean(void)
{
    print("[DRAM Klog] clean klog space in SRAM\n");
    memset((void*)CALIB_LOG_BASE, 0, CALIB_LOG_SIZE);
}

static DRAM_KLOG_HEAD *klog_head;
static DRAM_KLOG_TAIL *klog_tail;
static unsigned int *klog_data;

static void dram_klog_init(void)
{
    U16 sec, min, hou, dom, mth, yea;

    klog_head = (DRAM_KLOG_HEAD*) CALIB_LOG_BASE;
    klog_data = (unsigned int*)(CALIB_LOG_BASE + sizeof(DRAM_KLOG_HEAD));
    klog_tail = (DRAM_KLOG_TAIL*)(CALIB_LOG_BASE + CALIB_LOG_SIZE - sizeof(DRAM_KLOG_TAIL));

    print("[DRAM Klog] head: 0x%x, data: 0x%x, tail: 0x%x\n", klog_head, klog_data, klog_tail);

    sec = read_rtc(RTC_TC_SEC);
    min = read_rtc(RTC_TC_MIN);
    hou = read_rtc(RTC_TC_HOU);
    dom = read_rtc(RTC_TC_DOM);
    mth = read_rtc(RTC_TC_MTH);
    yea = read_rtc(RTC_TC_YEA) + RTC_MIN_YEAR;

    klog_head->rtc_yea_mth = (yea << 16) | mth;
    klog_head->rtc_dom_hou = (dom << 16) | hou;
    klog_head->rtc_min_sec = (min << 16) | sec;
    klog_tail->guard = 0x5A5A5A5A;
    klog_tail->check = (klog_head->rtc_yea_mth) ^ (klog_head->rtc_dom_hou) ^ (klog_head->rtc_min_sec) ^ (klog_tail->guard);

    print("[DRAM Klog] init SRAM space for Klog at %d/%d/%d %d:%d:%d\n", yea, mth, dom, hou, min, sec);

    return;
}

static int dram_klog_check(void)
{
    unsigned int i;
    unsigned int check = 0;

    for(i=0; i<CALIB_LOG_SIZE/4; i++)
        check ^= *((unsigned int*)CALIB_LOG_BASE+i);

    if(check != 0) {
        print("[DRAM Klog] wrong checksum\n");
        return -1;
    }

    print("[DRAM Klog] correct checksum\n");
    return 0;
}

int i4WriteSramLog(unsigned int u4Offset, unsigned int *pu4Src, unsigned int u4WordCnt)
{
    unsigned int i;

    i = (unsigned int) pu4Src;

    if ((i & 0x3) != 0) {
        print("[DRAM Klog] misalignment\n");
        i &= ~0x3;

		pu4Src = (unsigned int *) i;
    }

    if((u4Offset+u4WordCnt) > ((CALIB_LOG_SIZE-sizeof(DRAM_KLOG_HEAD)-sizeof(DRAM_KLOG_TAIL))/4)) {
        print("[DRAM Klog] fail to write Klog due to crossing the boundary\n");
        return -1;
    }
    else if(((unsigned int)0xffffffff-u4Offset) < u4WordCnt) {
        print("[DRAM Klog] fail to write Klog due to overflow\n");
        return -1;
    }

        for(i=0; i<u4WordCnt; i++) {
            klog_tail->check ^= *(klog_data+u4Offset+i) ^ *(pu4Src+i);
            *(klog_data+u4Offset+i) = *(pu4Src+i);
        }

    if(klog_head->data_count < (u4Offset+u4WordCnt)) {
        klog_tail->check ^= klog_head->data_count ^ (u4Offset+u4WordCnt);
        klog_head->data_count = u4Offset+u4WordCnt;
    }

    return 0;
}

static void dram_klog_print(void)
{
    unsigned int *curr;

    for(curr=(unsigned int*)CALIB_LOG_BASE; curr<(unsigned int*)(CALIB_LOG_BASE+CALIB_LOG_SIZE); curr++) {
        print("[DRAM Klog] 0x%x: 0x%x\n", curr, *curr);
    }

    return;
}

static void dram_klog_utest(void)
{
    int result;
    unsigned int *curr, value;
    unsigned int pattern[5] = {0x11112222, 0x33334444, 0x55556666, 0x77778888, 0x9999aaaa};

    //test 1: normal write
    dram_klog_clean();
    dram_klog_init();
    result = i4WriteSramLog(0, pattern, 1);
    //dram_klog_print();
    result |= i4WriteSramLog(2037, pattern, 5);
    dram_klog_print();
    result |= dram_klog_check();
    if(result != 0) print("[DRAM Klog] test 1 fail\n");
    else print("[DRAM Klog] test 1 success\n");
    /*//test 2: out of boundary
    dram_klog_clean();
    dram_klog_init();
    result = i4WriteSramLog(2038, pattern, 5);
    dram_klog_print();
    result |= dram_klog_check();
    if(result == 0) print("[DRAM Klog] test 2 fail\n");
    else print("[DRAM Klog] test 2 success\n");
    //test 3: out of boundary
    dram_klog_clean();
    dram_klog_init();
    result = i4WriteSramLog(4096, pattern, 5);
    dram_klog_print();
    result |= dram_klog_check();
    if(result == 0) print("[DRAM Klog] test 3 fail\n");
    else print("[DRAM Klog] test 3 success\n");
    //test 4: negative offset or word count
    result = i4WriteSramLog(-1, pattern, 1);
    result |= i4WriteSramLog(0, pattern, -1);
    dram_klog_print();
    result |= dram_klog_check();
    if(result == 0) print("[DRAM Klog] test 4 fail\n");
    else print("[DRAM Klog] test 4 success\n");
    //test 5: violate check sum
    *klog_data = 0xdeadbeef;
    dram_klog_print();
    result = dram_klog_check();
    if(result == 0) print("[DRAM Klog] test 5 fail\n");
    else print("[DRAM Klog] test 5 success\n");*/

}
#endif //DRAM_CALIB_LOG

#ifdef LAST_DRAMC

#define DRAMC_ADDR_SHIFT_CHN(addr, channel) (addr + (channel * 0x10000))

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
		//print("[LastDRAMC] latch result before RST: %x\n", u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_WDT_DBG_SIGNAL, chn)));
		latch_result = (latch_result << 16) | u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_WDT_DBG_SIGNAL, chn)) & 0xFFFF;
		temp = u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_DDRCONF0, chn));
		vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_DDRCONF0, chn), temp | 0x00000004);
		vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_DDRCONF0, chn), temp & 0xFFFFFFFB);
		//print("[LastDRAMC] latch result after RST: %x\n", u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_WDT_DBG_SIGNAL, chn)));
	}

	last_dramc_info_ptr->ta2_result_checksum ^= last_dramc_info_ptr->ta2_result_past ^ latch_result;
	last_dramc_info_ptr->ta2_result_past = last_dramc_info_ptr->ta2_result_last;
	last_dramc_info_ptr->ta2_result_last = latch_result;
	for (temp = 0; temp < sizeof(LAST_DRAMC_INFO_T) / sizeof(temp); temp++) {
		curr = (unsigned int *)last_dramc_info_ptr + temp;
		print("[LastDRAMC] 0x%x: 0x%x\n", curr, *curr);
	}

	return;
}

void init_ta2_all_channel(void)
{
	unsigned int chn;

	update_last_dramc_info();

	// TODO: consider DCS
	for (chn = 0; chn < CHANNEL_NUM; ++chn)
		init_ta2_single_channel(chn);
}

void init_ta2_single_channel(unsigned int channel)
{
	unsigned int temp;
	unsigned int matype;

	// mt6759: CHN0_EMI for CHN-A and CHN-B;  CHN1_EMI for CHN-C and CHN-D
	// TODO: consider remapping of EMI_CONJ
	if(channel < 2)
		matype = *(volatile unsigned *)EMI_CONA >> 4;
	else
		matype = *(volatile unsigned *)EMI_CONA >> 20;
	matype = ((matype & 0x3) + 1) << 30;

	// disable self test engine1 and self test engine2
	temp = u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_3, channel)) & 0x1FFFFFFF;
	vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_3, channel), temp);

	// set MATYPE
	temp = (u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_SHU_CONF0, channel))&0x7FFFFFFF) | matype;
	vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_SHU_CONF0, channel), temp);
	temp = (u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_SHU2_CONF0, channel))&0x7FFFFFFF) | matype;
	vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_SHU2_CONF0, channel), temp);
	temp = (u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_SHU3_CONF0, channel))&0x7FFFFFFF) | matype;
	vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_SHU3_CONF0, channel), temp);

	// set rank address for test agent to auto
	temp = u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_4, channel)) & 0x8FFFFFFF;
	temp |= (0x4 << 28);
	vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_4, channel), temp);

	// set test for both rank0 and rank1
	temp = u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_3, channel)) & 0xFFFFFFF0;
	vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_3, channel), temp | 0x1);

	// set base address for test agent to 0x0
	temp = (u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_1, channel))&0x0000000F) | (0x0<<4);
	vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_1, channel), temp);
	temp = (u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_5, channel))&0x0000000F) | (0x0<<4);
	vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_5, channel), temp);

	// set test length (offset) to 0x20
	temp = (u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_2, channel))&0x0000000F) | (0x20<<4);
	vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_2, channel), temp);

	// set pattern for XTALK (default XTALK)
	//vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(0, TEST2_4_TEST_REQ_LEN1));
	//vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0), P_Fld(1, PERFCTL0_RWOFOEN));
	//vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TESTAUDPAT)|P_Fld(u4log2loopcount,TEST2_3_TESTCNT));
	//vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(1, TEST2_4_TESTXTALKPAT)|P_Fld(0,TEST2_4_TESTAUDMODE)|P_Fld(0,TEST2_4_TESTAUDBITINV));
	//vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(0, TEST2_4_TESTSSOPAT)|P_Fld(0,TEST2_4_TESTSSOXTALKPAT));

	return;
}

unsigned int check_gating_err_in_dramc_latch(void)
{
	unsigned int chn, ret = 0;

	if ((g_boot_reason == BR_POWER_KEY) || (g_boot_reason == BR_USB)
			|| mtk_wdt_is_pmic_full_reset() || (is_last_dramc_initialized() == 0)){
		print("for cold boot, always return 0\n");
		return 0;
	}

	for (chn = 0; chn <= 3; ++chn) {
		if (u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_WDT_DBG_SIGNAL, chn)) & 0x80) {
			print("[dramc] found gating error in channel %d (0x%x)\n",
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
	part_dram_data_addr = get_part_addr("boot_para"); // addr = 0x8000
	if (part_dram_data_addr != 0x0)
		print("[dramc] init partition address is 0x%llx\n", part_dram_data_addr);
	else {
		print("[dramc] init partition address is incorrect !!!\n");
	}
#endif

	if ((g_boot_reason == BR_POWER_KEY) || (g_boot_reason == BR_USB)
			|| mtk_wdt_is_pmic_full_reset() || (is_last_dramc_initialized() == 0)){
		/* cold boot: initialize last_dram_fatal_err_flag and dram_fatal_err_flag */
		print("[dramc] init SRAM region for DRAM exception detection\n");
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
	last_dramc_info_ptr->fatal_err_flag &= (1 << OFFSET_DDR_RSV_MODE_FLOW);
	dsb();
}

unsigned int check_dram_fatal_exception(void)
{
	print("[dramc] DRAM_FATAL_ERR_FLAG = 0x%x\n", last_dramc_info_ptr->fatal_err_flag);

	return ((last_dramc_info_ptr->fatal_err_flag & ~((1 << OFFSET_DRAM_FATAL_ERR)|DDR_RSV_MODE_ERR_MASK)) != 0x0) ? 1 : 0;
}

unsigned int check_last_dram_fatal_exception(void)
{
	print("[dramc] LAST_DRAM_FATAL_ERR_FLAG = 0x%x\n", last_dramc_info_ptr->last_fatal_err_flag);

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

static void assign_checksum_for_dram_data(DRAM_CALIBRATION_DATA_T *data)
{
	/* need to initialize checksum to 0 before calculation */
	data->checksum = 0;
	data->checksum = crc16((u8*)data, sizeof(DRAM_CALIBRATION_DATA_T));
}

static int check_checksum_for_dram_data(DRAM_CALIBRATION_DATA_T *data)
{
	u16 checksum_in_storage = data->checksum;

	/* need to initialize checksum to 0 before calculation */
	data->checksum = 0;
	data->checksum = crc16((u8*)data, sizeof(DRAM_CALIBRATION_DATA_T));

	return (data->checksum == checksum_in_storage) ? 1 : 0;
}

#if !__ETT__
int read_offline_dram_calibration_data(SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData)
{
	int ret;
	blkdev_t *bootdev = NULL;
	DRAM_CALIBRATION_DATA_T data;

	if (offLine_SaveData == NULL) {
		print("[dramc] offLine_SaveData == NULL, skip\n");
		SET_DRAM_STORAGE_API_ERR(ERR_NULL_POINTER, DRAM_STORAGE_API_READ);
		return -ERR_NULL_POINTER;
	}

	bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		print("[dramc] can't find boot device(%d)\n", CFG_BOOT_DEV);
		SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_NOT_FOUND, DRAM_STORAGE_API_READ);
		return -ERR_BLKDEV_NOT_FOUND;
	}

	if (!part_dram_data_addr) {
		return -ERR_BLKDEV_NO_PART;
	}

	ret = blkdev_read(bootdev, part_dram_data_addr, sizeof(DRAM_CALIBRATION_DATA_T), (u8*)&data, PART_ID_DRAM_DATA);
	if (ret != 0) {
		print("[dramc] blkdev_read failed\n");
		SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_READ_FAIL, DRAM_STORAGE_API_READ);
		return -ERR_BLKDEV_READ_FAIL;
	}

	/* check preloader version */
	if (data.pl_version != PL_VERSION) {
		/* current preloader version does not match the calibration data in storage -> erase the partition */
		print("[dramc] PL_VERSION is updated, erase the DRAM data\n");
		memset(&data, 0, sizeof(DRAM_CALIBRATION_DATA_T));
		ret = blkdev_write(bootdev, part_dram_data_addr, sizeof(DRAM_CALIBRATION_DATA_T), (u8*)&data, PART_ID_DRAM_DATA);
		if (ret != 0) {
			print("[dramc] blkdev_write failed\n");
			SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_WRITE_FAIL, DRAM_STORAGE_API_READ);
			return -ERR_BLKDEV_WRITE_FAIL;
		}
		SET_DRAM_STORAGE_API_ERR(ERR_PL_UPDATED, DRAM_STORAGE_API_READ);
		return -ERR_PL_UPDATED;
	}

	/* check magic number */
	if (data.magic_number != DRAM_CALIBRATION_DATA_MAGIC) {
		print("[dramc] magic number mismatch\n");
		SET_DRAM_STORAGE_API_ERR(ERR_MAGIC_NUMBER, DRAM_STORAGE_API_READ);
		return -ERR_MAGIC_NUMBER;
	}

	/* check checksum */
	if (check_checksum_for_dram_data(&data) != 1) {
		print("[dramc] checksum failed\n");
		SET_DRAM_STORAGE_API_ERR(ERR_CHECKSUM, DRAM_STORAGE_API_READ);
		return -ERR_CHECKSUM;
	}

	/* copy the data stored in storage to the data structure for calibration */
	memcpy(offLine_SaveData, &(data.calibration_data), sizeof(SAVE_TIME_FOR_CALIBRATION_T));

	return 0;
}

int write_offline_dram_calibration_data(SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData)
{
	int ret;
	blkdev_t *bootdev = NULL;
	DRAM_CALIBRATION_DATA_T data;

	if (offLine_SaveData == NULL) {
		print("[dramc] offLine_SaveData == NULL, skip\n");
		SET_DRAM_STORAGE_API_ERR(ERR_NULL_POINTER, DRAM_STORAGE_API_WRITE);
		return -ERR_NULL_POINTER;
	}

	bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		print("[dramc] can't find boot device(%d)\n", CFG_BOOT_DEV);
		SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_NOT_FOUND, DRAM_STORAGE_API_WRITE);
		return -ERR_BLKDEV_NOT_FOUND;
	}

	if (!part_dram_data_addr) {
		return -ERR_BLKDEV_NO_PART;
	}

	memcpy(&(data.calibration_data), offLine_SaveData, sizeof(SAVE_TIME_FOR_CALIBRATION_T));

	/* assign PL version */
	data.pl_version = PL_VERSION;

	/* assign magic number */
	data.magic_number = DRAM_CALIBRATION_DATA_MAGIC;

	/* assign api error code */
	data.calib_err_code = g_dram_storage_api_err_code;

	/* calculate and assign checksum */
	assign_checksum_for_dram_data(&data);

	ret = blkdev_write(bootdev, part_dram_data_addr, sizeof(DRAM_CALIBRATION_DATA_T), (u8*)&data, PART_ID_DRAM_DATA);
	if (ret != 0) {
		print("[dramc] blkdev_write failed\n");
		SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_WRITE_FAIL, DRAM_STORAGE_API_WRITE);
		return -ERR_BLKDEV_WRITE_FAIL;
	}

	return 0;
}

int clean_dram_calibration_data(void)
{
	int ret;
	blkdev_t *bootdev = NULL;
	DRAM_CALIBRATION_DATA_T data;

	bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		print("[dramc] can't find block device(%d)\n", CFG_BOOT_DEV);
		SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_NOT_FOUND, DRAM_STORAGE_API_CLEAN);
		return -ERR_BLKDEV_NOT_FOUND;
	}

	if (!part_dram_data_addr) {
		return -ERR_BLKDEV_NO_PART;
	}

	memset(&data, 0, sizeof(DRAM_CALIBRATION_DATA_T));

	ret = blkdev_write(bootdev, part_dram_data_addr, sizeof(DRAM_CALIBRATION_DATA_T), (u8*)&data, PART_ID_DRAM_DATA);
	if (ret != 0) {
		print("[dramc] blkdev_write failed\n");
		SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_WRITE_FAIL, DRAM_STORAGE_API_CLEAN);
		return -ERR_BLKDEV_WRITE_FAIL;
	}

	return 0;
}

#else

DRAM_CALIBRATION_DATA_T dram_data; // using global variable to avoid stack overflow

int read_offline_dram_calibration_data(SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData)
{
#if !EMMC_READY
	return 0;
#else
	int ret;

	if (offLine_SaveData == NULL) {
		print("[dramc] offLine_SaveData == NULL, skip\n");
		SET_DRAM_STORAGE_API_ERR_AND_RETURN(ERR_NULL_POINTER, DRAM_STORAGE_API_READ);
		return -DRAM_STORAGE_API_READ;
	}

	ret = storage_read(part_dram_data_addr, BLK_NR_FOR_DRAM_DATA, (u8*)&dram_data);
	if (ret != 0) {
		print("[dramc] storage_read failed\n");
		ASSERT(0);
	}

	/* check magic number */
	if (dram_data.magic_number != DRAM_CALIBRATION_DATA_MAGIC) {
		print("[dramc] magic number mismatch\n");
		SET_DRAM_STORAGE_API_ERR(ERR_MAGIC_NUMBER, DRAM_STORAGE_API_READ);
		return -ERR_MAGIC_NUMBER;
	}

	/* check checksum */
	if (check_checksum_for_dram_data(&dram_data) != 1) {
		print("[dramc] checksum failed\n");
		SET_DRAM_STORAGE_API_ERR(ERR_CHECKSUM, DRAM_STORAGE_API_READ);
		return -ERR_CHECKSUM;
	}

	/* copy the data stored in storage to the data structure for calibration */
	memcpy(offLine_SaveData, &(dram_data.calibration_data), sizeof(SAVE_TIME_FOR_CALIBRATION_T));

	return 0;
#endif
}

int write_offline_dram_calibration_data(SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData)
{
#if !EMMC_READY
	return 0;
#else
	int ret;

	if (offLine_SaveData == NULL) {
		print("[dramc] offLine_SaveData == NULL, skip\n");
		SET_DRAM_STORAGE_API_ERR_AND_RETURN(ERR_NULL_POINTER, DRAM_STORAGE_API_WRITE);
		return -DRAM_STORAGE_API_WRITE;
	}

	memcpy(&(dram_data.calibration_data), offLine_SaveData, sizeof(SAVE_TIME_FOR_CALIBRATION_T));

	/* assign PL version */
	dram_data.pl_version = 0;

	/* assign magic number */
	dram_data.magic_number = DRAM_CALIBRATION_DATA_MAGIC;

	/* assign api error code */
	dram_data.calib_err_code = g_dram_storage_api_err_code;

	/* calculate and assign checksum */
	assign_checksum_for_dram_data(&dram_data);

	ret = storage_write(part_dram_data_addr, BLK_NR_FOR_DRAM_DATA, (u8*)&dram_data);
	if (ret != 0) {
		print("[dramc] storage_write failed\n");
		ASSERT(0);
	}

	return 0;
#endif
}

int clean_dram_calibration_data(void)
{
#if !EMMC_READY
	return;
#else
	int ret;

	memset(&dram_data, 0, sizeof(DRAM_CALIBRATION_DATA_T));

	ret = storage_write(part_dram_data_addr, BLK_NR_FOR_DRAM_DATA, (u8*)&dram_data);
	if (ret != 0) {
		print("[dramc] storage_write failed\n");
		ASSERT(0);
	}

	return 0;
#endif
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

void set_emi_drs(void)
{
#if ENABLE_DRS
        *((UINT32P)(CHN0_EMI_BASE + 0x00000160)) = 0x0a000012; // Disable rank0 grouping
        *((UINT32P)(CHN1_EMI_BASE + 0x00000160)) = 0x0a000012; // Disable rank0 grouping

        *((UINT32P)(CHN0_EMI_BASE + 0x00000164)) = 0x00000014; // Disable rank1 grouping
        *((UINT32P)(CHN1_EMI_BASE + 0x00000164)) = 0x00000014; // Disable rank1 grouping

        *((UINT32P)(CHN0_EMI_BASE + 0x00000168)) = 0x20f26044; // rank1 DRS disable & disable ASF
        *((UINT32P)(CHN1_EMI_BASE + 0x00000168)) = 0x20f26044; // rank1 DRS disable & disable ASF
#endif  // ENABLE_DRS

#if ENABLE_DRS_MONITOR
        *((UINT32P)(CHN0_EMI_BASE + 0x0000016c)) = 0x0000f801;
        *((UINT32P)(CHN1_EMI_BASE + 0x0000016c)) = 0x0000f801;

        *((UINT32P)(CHN0_EMI_BASE + 0x00000170)) = 0x40000000;
        *((UINT32P)(CHN1_EMI_BASE + 0x00000170)) = 0x40000000;
#endif  // ENABLE_DRS_MONITOR
}
