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
#if !__FLASH_TOOL_DA__ && !__ETT__ 
#if (FOR_DV_SIMULATION_USED==0)
   #include "platform.h"
   #include "upmu_hw.h"
   #include "pmic.h"
   #include "emi_hw.h"
   #include "fan53528buc08x.h"
   #include "fan53526.h"
#endif
#endif
#include "emi.h"

#if !__FLASH_TOOL_DA__ && !__ETT__
#include "custom_emi.h"   // fix build error: emi_settings
#endif

#if CFG_BOOT_ARGUMENT
#define bootarg g_dram_buf->bootarg
#endif

#ifdef DRAM_CALIB_LOG
static U16 read_rtc(U16 unit);
void dram_klog_clean(void);
void dram_klog_init(void);
//int dram_klog_check(void);
//void dram_klog_print(void);
static DRAM_KLOG_HEAD *klog_head;
static DRAM_KLOG_TAIL *klog_tail;
static unsigned int *klog_data;
#endif //end #ifdef DRAM_CALIB_LOG
unsigned int enable_4GB_flag=0;
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
#if defined(PMIC_CHIP_MT6355)
struct regulator *reg_vdram1, *reg_vdram2, *reg_vcore;
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

#if fcFOR_CHIP_ID == fcOlympus
#define CHN0_EMI_BASE	0x1022D000
#define CHN1_EMI_BASE	0x10235000

#define CHN_EMI_CONA(base)	(base + 0x000)
#define CHN_EMI_CONB(base)	(base + 0x008)
#define CHN_EMI_CONC(base)	(base + 0x010)
#define CHN_EMI_MDCT(base)	(base + 0x018)
#define CHN_EMI_TESTB(base)	(base + 0x048)
#define CHN_EMI_TESTC(base)	(base + 0x050)
#define CHN_EMI_TESTD(base)	(base + 0x058)
#define CHN_EMI_ARBI(base)	(base + 0x140)
#define CHN_EMI_ARBI_2ND(base)	(base + 0x144)
#define CHN_EMI_ARBJ(base)	(base + 0x148)
#define CHN_EMI_ARBJ_2ND(base)	(base + 0x14c)
#define CHN_EMI_ARBK(base)	(base + 0x150)
#define CHN_EMI_ARBK_2ND(base)	(base + 0x154)
#define CHN_EMI_ARB_REF(base)	(base + 0x15c)
#define CHN_EMI_RKx_GP(base) (base + 0x164)
#define CHN_EMI_RKARB0(base)	(base + 0x1B0)
#define CHN_EMI_RKARB1(base)	(base + 0x1B4)
#define CHN_EMI_RKARB2(base)	(base + 0x1B8)
#define CHN_EMI_SHF0(base)	(base + 0x710)

void EMI_Init(DRAMC_CTX_T *p)
{
#if !__ETT__
    EMI_SETTINGS *emi_set;
    int dram_type;

    if(emi_setting_index == -1)
        emi_set = &default_emi_setting;
#if (FOR_DV_SIMULATION_USED==0)
    else
        emi_set = &emi_settings[emi_setting_index];
#endif

  dram_type = emi_set->type & 0x000F;

  *(volatile unsigned *)EMI_CONA = emi_set->EMI_CONA_VAL;
  *(volatile unsigned *)EMI_CONF = emi_set->EMI_CONF_VAL;
  *(volatile unsigned *)EMI_CONH = emi_set->EMI_CONH_VAL;
  *(volatile unsigned *)CHN_EMI_CONA(CHN0_EMI_BASE) = emi_set->CHN0_EMI_CONA_VAL;
  *(volatile unsigned *)CHN_EMI_CONA(CHN1_EMI_BASE) = emi_set->CHN1_EMI_CONA_VAL;
  *(volatile unsigned *)CHN_EMI_CONC(CHN0_EMI_BASE) = emi_set->EMI_CONF_VAL;
  *(volatile unsigned *)CHN_EMI_CONC(CHN1_EMI_BASE) = emi_set->EMI_CONF_VAL; 
  
  *(volatile unsigned *)EMI_CONB	 = 0x15243242;
  *(volatile unsigned *)EMI_CONC	 = 0x0c1e0b1a;
  *(volatile unsigned *)EMI_COND	 = 0x3657587a;
  *(volatile unsigned *)EMI_CONE	 = 0xffff0848;
  *(volatile unsigned *)EMI_CONG	 = 0x372f2a35;
  *(volatile unsigned *)EMI_CONI	 = 0x00008806;
  *(volatile unsigned *)EMI_CONN	 = 0x000000f0;
  *(volatile unsigned *)EMI_MDCT	 = 0x11118c37;
  *(volatile unsigned *)EMI_MDCT_2ND	 = 0x00001111;
  *(volatile unsigned *)EMI_IOCL	 = 0xa8a8a8a8;
  *(volatile unsigned *)EMI_IOCL_2ND	 = 0x25252525;
  *(volatile unsigned *)EMI_IOCM	 = 0xa8a8a8a8;
  *(volatile unsigned *)EMI_IOCM_2ND	 = 0x25252525;
  *(volatile unsigned *)EMI_TESTB	 = 0x00060027;
  *(volatile unsigned *)EMI_TESTC	 = 0x78460000;
  *(volatile unsigned *)EMI_TESTD	 = 0x00000000;
  *(volatile unsigned *)EMI_ARBI	 = 0x00006108;
  *(volatile unsigned *)EMI_SLCT	 = 0x0001ff00;
  *(volatile unsigned *)EMI_SLVA	 = 0x33331311;

  if (dram_type == TYPE_LPDDR3) {
  	*(volatile unsigned *)EMI_ARBA	 = 0xffff7044;
  	*(volatile unsigned *)EMI_ARBB	 = 0xffff7044;
  	*(volatile unsigned *)EMI_ARBC	 = 0xffff50c0;
  	*(volatile unsigned *)EMI_ARBD	 = 0x5f5f70cb;
  	*(volatile unsigned *)EMI_ARBE	 = 0xffff604c;
  	*(volatile unsigned *)EMI_ARBF	 = 0xffff50d7;
  	*(volatile unsigned *)EMI_ARBG	 = 0xffff7046;
  	*(volatile unsigned *)EMI_ARBH	 = 0xffff7043;
  } else if ((dram_type == TYPE_LPDDR4) || (dram_type == TYPE_LPDDR4X)) {
  	*(volatile unsigned *)EMI_ARBA	 = 0xffff7044;
  	*(volatile unsigned *)EMI_ARBB	 = 0xffff7044;
  	*(volatile unsigned *)EMI_ARBC	 = 0xffff70d0;
  	*(volatile unsigned *)EMI_ARBD	 = 0x5f5f70c6;
  	*(volatile unsigned *)EMI_ARBE	 = 0xffff6046;
  	*(volatile unsigned *)EMI_ARBF	 = 0xffff50d0;
  	*(volatile unsigned *)EMI_ARBG	 = 0xffff7046;
  	*(volatile unsigned *)EMI_ARBH	 = 0xffff7046;
  }

  *(volatile unsigned *)EMI_CONM	 = 0xff0006b0;

  //CHN0_EMI
  *(volatile unsigned *)CHN_EMI_CONB(CHN0_EMI_BASE)	= 0xffff2048;
  *(volatile unsigned *)CHN_EMI_MDCT(CHN0_EMI_BASE)	= 0x00008cd7;
  *(volatile unsigned *)CHN_EMI_TESTB(CHN0_EMI_BASE)	= 0x00028027;
  *(volatile unsigned *)CHN_EMI_TESTC(CHN0_EMI_BASE)	= 0x78460000;
  *(volatile unsigned *)CHN_EMI_TESTD(CHN0_EMI_BASE)	= 0x00000000;
  *(volatile unsigned *)CHN_EMI_ARBI(CHN0_EMI_BASE)	= 0x20407188;
  *(volatile unsigned *)CHN_EMI_ARBI_2ND(CHN0_EMI_BASE)	= 0x20406188;
  *(volatile unsigned *)CHN_EMI_ARBJ(CHN0_EMI_BASE)	= 0x9819595e;
  *(volatile unsigned *)CHN_EMI_ARBJ_2ND(CHN0_EMI_BASE)	= 0x9819595e;
  *(volatile unsigned *)CHN_EMI_ARBK(CHN0_EMI_BASE)	= 0x64f3fc79;
  *(volatile unsigned *)CHN_EMI_ARBK_2ND(CHN0_EMI_BASE)	= 0x64f3fc79;
  *(volatile unsigned *)CHN_EMI_ARB_REF(CHN0_EMI_BASE)	= 0x0002032e;
  *(volatile unsigned *)CHN_EMI_RKx_GP(CHN0_EMI_BASE)	= 0x0a000210;
  *(volatile unsigned *)CHN_EMI_RKARB0(CHN0_EMI_BASE)	= 0x0006002f;
  *(volatile unsigned *)CHN_EMI_RKARB1(CHN0_EMI_BASE)	= 0xff01ff01;
  *(volatile unsigned *)CHN_EMI_RKARB2(CHN0_EMI_BASE)	= 0x20001840;
  *(volatile unsigned *)CHN_EMI_SHF0(CHN0_EMI_BASE)	= 0x88228cd7;

  //CHN1_EMI
  *(volatile unsigned *)CHN_EMI_CONB(CHN1_EMI_BASE)	= 0xffff2048;
  *(volatile unsigned *)CHN_EMI_MDCT(CHN1_EMI_BASE)	= 0x00008cd7;
  *(volatile unsigned *)CHN_EMI_TESTB(CHN1_EMI_BASE)	= 0x00028027;
  *(volatile unsigned *)CHN_EMI_TESTC(CHN1_EMI_BASE)	= 0x78460000;
  *(volatile unsigned *)CHN_EMI_TESTD(CHN1_EMI_BASE)	= 0x00000000;
  *(volatile unsigned *)CHN_EMI_ARBI(CHN1_EMI_BASE)	= 0x20407188;
  *(volatile unsigned *)CHN_EMI_ARBI_2ND(CHN1_EMI_BASE)	= 0x20406188;
  *(volatile unsigned *)CHN_EMI_ARBJ(CHN1_EMI_BASE)	= 0x9819595e;
  *(volatile unsigned *)CHN_EMI_ARBJ_2ND(CHN1_EMI_BASE)	= 0x9819595e;
  *(volatile unsigned *)CHN_EMI_ARBK(CHN1_EMI_BASE)	= 0x64f3fc79;
  *(volatile unsigned *)CHN_EMI_ARBK_2ND(CHN1_EMI_BASE)	= 0x64f3fc79;
  *(volatile unsigned *)CHN_EMI_ARB_REF(CHN1_EMI_BASE)	= 0x0002032e;
  *(volatile unsigned *)CHN_EMI_RKx_GP(CHN1_EMI_BASE)	= 0x0a000210;
  *(volatile unsigned *)CHN_EMI_RKARB0(CHN1_EMI_BASE)	= 0x0006002f;
  *(volatile unsigned *)CHN_EMI_RKARB1(CHN1_EMI_BASE)	= 0xff01ff01;
  *(volatile unsigned *)CHN_EMI_RKARB2(CHN1_EMI_BASE)	= 0x20001840;
  *(volatile unsigned *)CHN_EMI_SHF0(CHN1_EMI_BASE)	= 0x88228cd7;

  *(volatile unsigned *)CHN_EMI_CONC(CHN0_EMI_BASE)	|= 0x00000001;
  *(volatile unsigned *)CHN_EMI_CONC(CHN1_EMI_BASE)	|= 0x00000001;
  *(volatile unsigned *)EMI_CONM	                |= 0x00000400;

  p->vendor_id = emi_set->iLPDDR3_MODE_REG_5;
#endif
}
#elif fcFOR_CHIP_ID == fcElbrus
#define EMI_APB_BASE    0x10230000
#define CHN0_EMI_BASE	0x10335000
#define CHN1_EMI_BASE	0x1033D000

#define EMI_CONA		(EMI_APB_BASE + 0x000)
#define EMI_CONB		(EMI_APB_BASE + 0x008)
#define EMI_CONC		(EMI_APB_BASE + 0x010)
#define EMI_COND		(EMI_APB_BASE + 0x018)
#define EMI_CONE		(EMI_APB_BASE + 0x020)
#define EMI_CONF		(EMI_APB_BASE + 0x028)
#define EMI_CONG		(EMI_APB_BASE + 0x030)
#define EMI_CONH		(EMI_APB_BASE + 0x038)
#define EMI_CONI		(EMI_APB_BASE + 0x040)
#define EMI_CONM		(EMI_APB_BASE + 0x060)
#define EMI_CONN		(EMI_APB_BASE + 0x068)
#define EMI_MDCT		(EMI_APB_BASE + 0x078)
#define EMI_MDCT_2ND		(EMI_APB_BASE + 0x07C)
#define EMI_IOCL		(EMI_APB_BASE + 0x0d0)
#define EMI_IOCL_2ND		(EMI_APB_BASE + 0x0d4)
#define EMI_IOCM		(EMI_APB_BASE + 0x0d8)
#define EMI_IOCM_2ND		(EMI_APB_BASE + 0x0dc)
#define EMI_TESTB		(EMI_APB_BASE + 0x0e8)
#define EMI_TESTC		(EMI_APB_BASE + 0x0f0)
#define EMI_TESTD		(EMI_APB_BASE + 0x0f8)
#define EMI_SLVA		(EMI_APB_BASE + 0x800)
#define EMI_ARBA		(EMI_APB_BASE + 0x100)
#define EMI_ARBC		(EMI_APB_BASE + 0x110)
#define EMI_ARBD		(EMI_APB_BASE + 0x118)
#define EMI_ARBE		(EMI_APB_BASE + 0x120)
#define EMI_ARBF		(EMI_APB_BASE + 0x128)
#define EMI_ARBG		(EMI_APB_BASE + 0x130)
#define EMI_ARBH		(EMI_APB_BASE + 0x138)
#define EMI_ARBI		(EMI_APB_BASE + 0x140)
#define EMI_SLCT		(EMI_APB_BASE + 0x158)
#define EMI_BMEN		(EMI_APB_BASE + 0x400)

#define CHN_EMI_CONA(base)	(base + 0x000)
#define CHN_EMI_CONB(base)	(base + 0x008)
#define CHN_EMI_CONC(base)	(base + 0x010)
#define CHN_EMI_MDCT(base)	(base + 0x018)
#define CHN_EMI_TESTB(base)	(base + 0x048)
#define CHN_EMI_TESTC(base)	(base + 0x050)
#define CHN_EMI_TESTD(base)	(base + 0x058)
#define CHN_EMI_ARBI(base)	(base + 0x140)
#define CHN_EMI_ARBI_2ND(base)	(base + 0x144)
#define CHN_EMI_ARBJ(base)	(base + 0x148)
#define CHN_EMI_ARBJ_2ND(base)	(base + 0x14c)
#define CHN_EMI_ARBK(base)	(base + 0x150)
#define CHN_EMI_ARBK_2ND(base)	(base + 0x154)
#define CHN_EMI_ARB_REF(base)	(base + 0x15c)


void EMI_Init(DRAMC_CTX_T *p)
{
    EMI_SETTINGS *emi_set;
    
    if(emi_setting_index == -1)
        emi_set = &default_emi_setting;
#if (FOR_DV_SIMULATION_USED==0)    
    else
        emi_set = &emi_settings[emi_setting_index];
#endif

  *(volatile unsigned *)EMI_CONA = emi_set->EMI_CONA_VAL;
  *(volatile unsigned *)EMI_CONF = emi_set->EMI_CONF_VAL;
  *(volatile unsigned *)EMI_CONH = emi_set->EMI_CONH_VAL;
  *(volatile unsigned *)CHN_EMI_CONA(CHN0_EMI_BASE) = emi_set->CHN0_EMI_CONA_VAL;
  *(volatile unsigned *)CHN_EMI_CONA(CHN1_EMI_BASE) = emi_set->CHN1_EMI_CONA_VAL;
  *(volatile unsigned *)CHN_EMI_CONC(CHN0_EMI_BASE) = emi_set->EMI_CONF_VAL;
  *(volatile unsigned *)CHN_EMI_CONC(CHN1_EMI_BASE) = emi_set->EMI_CONF_VAL; 
  
  *(volatile unsigned *)EMI_CONB	 = 0x17283544;
  *(volatile unsigned *)EMI_CONC	 = 0x0a1a0b1a;
  *(volatile unsigned *)EMI_COND	 = 0x3657587a;
  *(volatile unsigned *)EMI_CONE	 = 0xffff0848;
  *(volatile unsigned *)EMI_CONG	 = 0x2b2b2a38;
  *(volatile unsigned *)EMI_CONI	 = 0x00008803;
  *(volatile unsigned *)EMI_CONM	 = 0x000006ff;
  *(volatile unsigned *)EMI_CONN	 = 0x00000000;
  *(volatile unsigned *)EMI_MDCT	 = 0x9a998c17;
  *(volatile unsigned *)EMI_MDCT_2ND	 = 0x00001224;
  *(volatile unsigned *)EMI_IOCL	 = 0xa8a8a8a8;
  *(volatile unsigned *)EMI_IOCL_2ND	 = 0x25252525;
  *(volatile unsigned *)EMI_IOCM	 = 0xa8a8a8a8;
  *(volatile unsigned *)EMI_IOCM_2ND	 = 0x25252525;
  *(volatile unsigned *)EMI_TESTB	 = 0x00060027;
  *(volatile unsigned *)EMI_TESTC	 = 0x38460000;
  *(volatile unsigned *)EMI_TESTD	 = 0x00000000;
  *(volatile unsigned *)EMI_ARBA	 = 0x8fa07241;
  *(volatile unsigned *)EMI_ARBC	 = 0xa0a070cf;
  *(volatile unsigned *)EMI_ARBD	 = 0x000070c6;
  *(volatile unsigned *)EMI_ARBE	 = 0x40406048;
  *(volatile unsigned *)EMI_ARBF	 = 0xa0a070d8;
  *(volatile unsigned *)EMI_ARBG	 = 0xa0a07045;
  *(volatile unsigned *)EMI_ARBH	 = 0xb0b07044;
  *(volatile unsigned *)EMI_ARBI	 = 0x00006108;
  *(volatile unsigned *)EMI_SLCT	 = 0x0001ff00;
  *(volatile unsigned *)EMI_BMEN	 = 0x00000001;
  
  //CHN0_EMI
  *(volatile unsigned *)CHN_EMI_CONB(CHN0_EMI_BASE)	= 0xffff2048;
  *(volatile unsigned *)CHN_EMI_CONC(CHN0_EMI_BASE)	= 0x00000001;
  *(volatile unsigned *)CHN_EMI_MDCT(CHN0_EMI_BASE)	= 0x88028c17;
  *(volatile unsigned *)CHN_EMI_TESTB(CHN0_EMI_BASE)	= 0x00040027;
  *(volatile unsigned *)CHN_EMI_TESTC(CHN0_EMI_BASE)	= 0x38460000;
  *(volatile unsigned *)CHN_EMI_TESTD(CHN0_EMI_BASE)	= 0x00000000;
  *(volatile unsigned *)CHN_EMI_ARBI(CHN0_EMI_BASE)	= 0x20406188;
  *(volatile unsigned *)CHN_EMI_ARBI_2ND(CHN0_EMI_BASE)	= 0x20406188;
  *(volatile unsigned *)CHN_EMI_ARBJ(CHN0_EMI_BASE)	= 0x3719595e;
  *(volatile unsigned *)CHN_EMI_ARBJ_2ND(CHN0_EMI_BASE)	= 0x3719595e;   
  *(volatile unsigned *)CHN_EMI_ARBK(CHN0_EMI_BASE)	= 0x64f3fc79;
  *(volatile unsigned *)CHN_EMI_ARBK_2ND(CHN0_EMI_BASE)	= 0x64f3fc79; 
  *(volatile unsigned *)CHN_EMI_ARB_REF(CHN0_EMI_BASE)	= 0x82410222;          

  //CHN1_EMI
  *(volatile unsigned *)CHN_EMI_CONB(CHN1_EMI_BASE)	= 0xffff2048;
  *(volatile unsigned *)CHN_EMI_CONC(CHN1_EMI_BASE)	= 0x00000001;
  *(volatile unsigned *)CHN_EMI_MDCT(CHN1_EMI_BASE)	= 0x88028c17;
  *(volatile unsigned *)CHN_EMI_TESTB(CHN1_EMI_BASE)	= 0x00040027;
  *(volatile unsigned *)CHN_EMI_TESTC(CHN1_EMI_BASE)	= 0x38460000;
  *(volatile unsigned *)CHN_EMI_TESTD(CHN1_EMI_BASE)	= 0x00000000;
  *(volatile unsigned *)CHN_EMI_ARBI(CHN1_EMI_BASE)	= 0x20406188;
  *(volatile unsigned *)CHN_EMI_ARBI_2ND(CHN1_EMI_BASE)	= 0x20406188;
  *(volatile unsigned *)CHN_EMI_ARBJ(CHN1_EMI_BASE)	= 0x3719595e;
  *(volatile unsigned *)CHN_EMI_ARBJ_2ND(CHN1_EMI_BASE)	= 0x3719595e;   
  *(volatile unsigned *)CHN_EMI_ARBK(CHN1_EMI_BASE)	= 0x64f3fc79;
  *(volatile unsigned *)CHN_EMI_ARBK_2ND(CHN1_EMI_BASE)	= 0x64f3fc79; 
  *(volatile unsigned *)CHN_EMI_ARB_REF(CHN1_EMI_BASE)	= 0x82410222;          
    
}

void EMI_Init2(void)
{
	*(volatile unsigned *)CHN_EMI_CONC(CHN0_EMI_BASE)	|= 0x00000001;
  *(volatile unsigned *)CHN_EMI_CONC(CHN1_EMI_BASE)	|= 0x00000001;
  *(volatile unsigned *)EMI_CONM	                |= 0x00000400;
  *(volatile unsigned *)EMI_CONN	                &= 0x0;
}
#endif

static unsigned int emi_conh = 0;
static unsigned int chn_emi_cona[2] = {0, 0};

#define MIN_PER_RANK_SZ_PER_CHAN	0x10000000 // 256MB
#define CHANNEL_NUMBER			2

#if !__ETT__
/* return the start address of rank1 */
unsigned int set_emi_before_rank1_mem_test(void)
{
	EMI_SETTINGS *emi_set;
	emi_set = &emi_settings[emi_setting_index];

	emi_conh = *(volatile unsigned int*)EMI_CONH;
	chn_emi_cona[0] = *(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_BASE);
	chn_emi_cona[1] = *(volatile unsigned int*)CHN_EMI_CONA(CHN1_EMI_BASE);

	if ((emi_set->type & 0xF) != TYPE_LPDDR3) {
		/* set all the rank size of all the channels to the minimum value */
		*(volatile unsigned int*)EMI_CONH = ((*(volatile unsigned int*)EMI_CONH & 0x0000ffff) | (0x11110000));

		/* set all the rank size of all channel-0 to the minimum value */
		*(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_BASE)
			= ((*(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_BASE) & 0xff00ffff ) | (0x00110000));

		/* set all the rank size of all channel-1 to the minimum value */
		*(volatile unsigned int*)CHN_EMI_CONA(CHN1_EMI_BASE)
			= ((*(volatile unsigned int*)CHN_EMI_CONA(CHN1_EMI_BASE) & 0xff00ffff ) | (0x00110000));

		dsb();
		return MIN_PER_RANK_SZ_PER_CHAN * CHANNEL_NUMBER;	/* rank1 start address */
	} else {
		/* set all the rank size of all the channels to the minimum value */
		*(volatile unsigned int*)EMI_CONH = ((*(volatile unsigned int*)EMI_CONH & 0x0000ffff) | (0x00110000));

		/* set all the rank size of all channel-0 to the minimum value */
		*(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_BASE)
			= ((*(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_BASE) & 0xff00ffff ) | (0x00110000));

		dsb();
		return MIN_PER_RANK_SZ_PER_CHAN;	/* rank1 start address */
	}
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

	phy_base = Channel_A_PHY_BASE_ADDRESS;

	for (i = 0; i <= 3; ++i, phy_base += 0x8000, err_code = 0) {
		misc_stberr_rk0_r = *(volatile unsigned int*)(phy_base + 0x510);
		misc_stberr_rk0_f = *(volatile unsigned int*)(phy_base + 0x514);
		misc_stberr_rk1_r = *(volatile unsigned int*)(phy_base + 0x518);
		misc_stberr_rk1_f = *(volatile unsigned int*)(phy_base + 0x51c);
		if (misc_stberr_rk0_r & (1 << 16)) {
			ret |= (1 << i);
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
		}
	}

	return ret;
}
#endif

#if 0
void CHA_HWGW_Print(DRAMC_CTX_T *p)
{
    U8 u1RefreshRate;
    U32 backup_channel;

    backup_channel = p->channel;    


#if (FOR_DV_SIMULATION_USED==0)
    // Read HW gating tracking
#ifdef HW_GATING
        DramcPrintHWGatingStatus(p, CHANNEL_A);
        
        if(u1IsLP4Family(p->dram_type))
         DramcPrintHWGatingStatus(p, CHANNEL_B);
#endif
    
#if ENABLE_RX_TRACKING_LP4
        DramcPrintRXDQDQSStatus(p, CHANNEL_A);        
        
        if(u1IsLP4Family(p->dram_type))
         DramcPrintRXDQDQSStatus(p, CHANNEL_B);        
#endif
    
#ifdef IMPEDENCE_TRACKING_ENABLE
        if(u1IsLP4Family(p->dram_type))
        {
            DramcPrintIMPTrackingStatus(p, CHANNEL_A);
//CH_B HW Fail, can't use            DramcPrintIMPTrackingStatus(p, CHANNEL_B);
        }
#endif

#ifdef TEMP_SENSOR_ENABLE
        u1RefreshRate = u1GetMR4RefreshRate(p, CHANNEL_A);
        mcSHOW_ERR_MSG(("[CHA] MRR(MR4) Reg.3B8h[10:8]=%x\n", u1RefreshRate));
        u1RefreshRate = u1GetMR4RefreshRate(p, CHANNEL_B);
        mcSHOW_ERR_MSG(("[CHB] MRR(MR4) Reg.3B8h[10:8]=%x\n", u1RefreshRate));
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
    mcSHOW_DBG_MSG(("=============================================\n"));
#endif    
}

int mt_get_dram_type(void)
{
  return TYPE_LPDDR3;
}

int mt_get_freq_setting(DRAMC_CTX_T *p)
{
    return DUAL_FREQ_HIGH;
}

#ifdef DDR_RESERVE_MODE
extern u32 g_ddr_reserve_enable;
extern u32 g_ddr_reserve_success;
#define TIMEOUT 1000 //about 1us
extern void before_Dramc_DDR_Reserved_Mode_setting(void);

#endif

#define	CHAN_DRAMC_NAO_MISC_STATUSA(base)	(base + 0x80)
#define	CHAN_DRAMC_AO_ARBCTL(base)			(base + 0x10) //[12:10] for dram type Darren+
#define SREF_STATE							(1 << 16)

static unsigned int is_dramc_exit_slf(void)
{
	unsigned int dram_type = 0, statusa = 0;

	statusa = *(volatile unsigned *)CHAN_DRAMC_NAO_MISC_STATUSA(Channel_A_DRAMC_NAO_BASE_ADDRESS);
	if ((statusa & SREF_STATE) != 0) {
		//print("DRAM CHAN-A is in self-refresh (MISC_STATUSA = 0x%x)\n", statusa);
		return 0;
	}

	dram_type = *(volatile unsigned *)CHAN_DRAMC_AO_ARBCTL(Channel_A_DRAMC_AO_BASE_ADDRESS);
	dram_type = (dram_type >> 10) & 0x7;
	//print("dram_type = %d\n", dram_type);

	statusa = *(volatile unsigned *)CHAN_DRAMC_NAO_MISC_STATUSA(Channel_B_DRAMC_NAO_BASE_ADDRESS);
	if (((statusa & SREF_STATE) != 0) && (dram_type != 1)) { // LPDDR3 ignore
		//print("DRAM CHAN-B is in self-refresh (MISC_STATUSA = 0x%x)\n", statusa);
		return 0;
	}

	print("ALL DRAM CHAN is not in self-refresh\n");
	return 1;
}

void release_dram(void)
{   
#ifdef DDR_RESERVE_MODE  
    int i;
    //int counter = TIMEOUT;
 
#if defined(PMIC_CHIP_MT6355) //Darren+
    regulator_set_voltage(reg_vcore, 800000, MAX_VCORE);
#else
    pmic_config_interface(MT6351_BUCK_VCORE_CON5, 0x20, 0x7F, 0); 
#endif
    rgu_release_rg_dramc_conf_iso();//Release DRAMC/PHY conf ISO      
    Dramc_DDR_Reserved_Mode_setting();	
    rgu_release_rg_dramc_iso();//Release PHY IO ISO    
    rgu_release_rg_dramc_sref();//Let DRAM Leave SR
    
    // setup for EMI: touch center EMI and channel EMI to enable CLK
    print("[DDR reserve] EMI CONA: %x\n", *(volatile unsigned int*)EMI_CONA);
    print("[DDR reserve] EMI CHA CONA: %x\n", *(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_BASE));
    print("[DDR reserve] EMI CHB CONA: %x\n", *(volatile unsigned int*)CHN_EMI_CONA(CHN1_EMI_BASE));
    for (i=0;i<10;i++);
    
    while(1/*counter*/)
    {
        if(is_dramc_exit_slf() == 1) /* expect to exit dram-self-refresh */
            break;
        //counter--;
    }
  
    /*if(counter == 0)
    {
        if(g_ddr_reserve_enable==1 && g_ddr_reserve_success==1)
        {
            print("[DDR Reserve] release dram from self-refresh FAIL!\n");
            g_ddr_reserve_success = 0;
        }
    }
    else
    {*/
    print("[DDR Reserve] release dram from self-refresh PASS!\n");
    //}
    Dramc_DDR_Reserved_Mode_AfterSR();
    //Expect to Use LPDDR3200 and PHYPLL as output, so no need to handle 
    //shuffle status since the status will be reset by system reset
    //There is an PLLL_SHU_GP in SPM which will reset by system reset
    return;
#endif    
}

void check_ddr_reserve_status(void)
{
    //unsigned int wdt_mode;
    //unsigned int wdt_dbg_ctrl;
    //wdt_mode = READ_REG(MTK_WDT_MODE);
    //wdt_dbg_ctrl = READ_REG(MTK_WDT_DEBUG_CTL);

    //print("before test, wdt_mode = 0x%x, wdt_dbg_ctrl = 0x%x\n", wdt_mode, wdt_dbg_ctrl);     
    //if(((wdt_mode & MTK_WDT_MODE_DDR_RESERVE) !=0) && ((wdt_dbg_ctrl & MTK_DDR_RESERVE_RTA) != 0) )
    //{
    //    print("go sample reserved flow\n");
    //    print("[DDR Reserve_sp:DRAMC] 0x%x:0x%x\n",(0x10004080),*(volatile unsigned int *)(0x10004080));
    //    ddr_reserve_sample_flow();
    //    print("[DDR Reserve_sp:DRAMC] 0x%x:0x%x\n",(0x10004080),*(volatile unsigned int *)(0x10004080));
    //}

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
    int i;
    int type = 2;
    type = (emi_settings[0].type & 0xF);
    for (i = 0 ; i < num_of_emi_records; i++)
    {
      //print("[EMI][%d] type%d\n",i,type);
      if ((type != (emi_settings[i].type & 0xF)) && (type != TYPE_LPDDR4) && (type != TYPE_LPDDR4X))
    {
#if (FOR_DV_SIMULATION_USED==0)    
          print("It's not allow to combine two type dram when combo discrete dram enable\n");
          ASSERT(0);
#endif    
            break;
     }  
    }
    return type;
}

#if (FOR_DV_SIMULATION_USED==0)
#if !__FLASH_TOOL_DA__ && !__ETT__ 

/* 
 * setup block correctly, we should hander both 4GB mode and 
 * non-4GB mode.
 */
void get_orig_dram_rank_info(dram_info_t *orig_dram_info)
{
	int i, j;
	u64 base = DRAM_BASE;
	u64 rank_size[4];

	orig_dram_info->rank_num = get_dram_rank_nr();
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
}

static char id[22];
static int emmc_nand_id_len=16;
static int fw_id_len;
static int mt_get_mdl_number (void)
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

    if (!(found))
    {
        int result=0;
        platform_get_mcp_id (id, emmc_nand_id_len,&fw_id_len);
        print("[EMI] eMMC/NAND ID = %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x\r\n", id[0], id[1], id[2], id[3], id[4], id[5], id[6], id[7], id[8],id[9],id[10],id[11],id[12],id[13     ],id[14],id[15]);
        for (i = 0 ; i < num_of_emi_records; i++)
        {
            if ((emi_settings[i].type & 0x0F00) == 0x0000) 
            {
                discrete_dram_num ++; 
            }
            else
            {
                mcp_dram_num ++; 
            }
        }

        /*If the number >=2  &&
         * one of them is discrete DRAM
         * enable combo discrete dram parse flow
         * */
        if ((discrete_dram_num > 0) && (num_of_emi_records >= 2))
        {
            /* if we enable combo discrete dram
             * check all dram are all same type and not DDR3
             * */
            enable_combo_dis = 1;
            dram_type = emi_settings[0].type & 0x000F;
            for (i = 0 ; i < num_of_emi_records; i++)
            {
                if ((dram_type != (emi_settings[i].type & 0x000F)) && (dram_type != TYPE_LPDDR4) && (dram_type != TYPE_LPDDR4X)) //Darren
                {
                    print("[EMI] Combo discrete dram only support when combo lists are all same dram type.");
                    ASSERT(0);
                }
                if ((emi_settings[i].type & 0x000F) == TYPE_PCDDR3) 
                {
                    // has PCDDR3, disable combo discrete drame, no need to check others setting 
                    enable_combo_dis = 0; 
                    break;
                }
                dram_type = emi_settings[i].type & 0x000F;
            }
            
        } 
        print("[EMI] mcp_dram_num:%d,discrete_dram_num:%d,enable_combo_dis:%d\r\n",mcp_dram_num,discrete_dram_num,enable_combo_dis);
        /*
         *
         * 0. if there is only one discrete dram, use index=0 emi setting and boot it.
         * */
        if ((0 == mcp_dram_num) && (1 == discrete_dram_num))
        {
            mdl_number = 0;
            found = 1;
#if 0 //Darren-
		#if ENABLE_MRR_AFTER_FIRST_K 
			//K first frequency (1066MHz)
			Dis_DramType = mt_get_dram_type_for_dis();
			if ((Dis_DramType == TYPE_LPDDR4) || (Dis_DramType == TYPE_LPDDR4X)) {
#if defined(PMIC_CHIP_MT6355)
				// TODO: FIXME
				Dis_DramType = TYPE_LPDDR4X;
#else
				if ((is_fan53528buc08x_exist() == 1) && (is_fan53526_exist() == 1))
					Dis_DramType = TYPE_LPDDR4X;
				else if ((is_fan53528buc08x_exist() == 0) && (is_fan53526_exist() == 1))
					Dis_DramType = TYPE_LPDDR4;
				else {
					print("[DRAMC] Discrete DRAM buck check fails !!!\n");
					ASSERT(0);
				}
#endif
			}
			Init_DRAM(Dis_DramType, CBT_NORMAL_MODE, &DramInfo);         
		#endif
#endif		 
            return mdl_number;
        }

#if 1
        /* 1.
         * if there is MCP dram in the list, we try to find emi setting by emmc ID
         * */
        if (mcp_dram_num > 0)
        {
            result = platform_get_mcp_id (id, emmc_nand_id_len,&fw_id_len);
            
            for (i = 0; i < num_of_emi_records; i++)
            {
                if (emi_settings[i].type != 0)
                {
                    if ((emi_settings[i].type & 0xF00) != 0x000)
                    {
                        if (result == 0)
                        {   /* valid ID */
            
                            if ((emi_settings[i].type & 0xF00) == 0x100)
                            {
                                /* NAND */
                                if (memcmp(id, emi_settings[i].ID, emi_settings[i].id_length) == 0){
                                    memset(id + emi_settings[i].id_length, 0, sizeof(id) - emi_settings[i].id_length);                                
                                    mdl_number = i;
                                    found = 1;
                                    break; /* found */
                                }
                            }
                            else
                            {
                                
                                /* eMMC */
                                if (memcmp(id, emi_settings[i].ID, emi_settings[i].id_length) == 0)
                                {
                                    print("fw id len:%d\n",emi_settings[i].fw_id_length);
                                    if (emi_settings[i].fw_id_length > 0)
                                    {
                                        char fw_id[6];
                                        memset(fw_id, 0, sizeof(fw_id));
                                        memcpy(fw_id,id+emmc_nand_id_len,fw_id_len);
                                        for (j = 0; j < fw_id_len;j ++){
                                            print("0x%x, 0x%x ",fw_id[j],emi_settings[i].fw_id[j]); 
                                        }
                                        if(memcmp(fw_id,emi_settings[i].fw_id,fw_id_len) == 0)
                                        {
                                            mdl_number = i;
                                            found = 1;
                                            break; /* found */
                                        }
                                        else
                                        {
                                            print("[EMI] fw id match failed\n");
                                        }
                                    }
                                    else
                                    {
                                        mdl_number = i;
                                        found = 1;
                                        break; /* found */
                                    }    
                                }
                                else{
                                      print("[EMI] index(%d) timing table id match failed\n",i);
                                }
                                
                            }
                        }
                    }
                }
            }
        }
#endif

#if 1
        /* 2. find emi setting by MODE register 5
         * */
        // if we have found the index from by eMMC ID checking, we can boot android by the setting
        // if not, we try by vendor ID            
        if ((0 == found) && (1 == enable_combo_dis))
        {
            EMI_SETTINGS *emi_set;              
            u64 rank0_size=0, rank1_size=0;
#if defined(PMIC_CHIP_MT6355)
            U8 vm_mode;
            pmic_read_interface(PMIC_VM_MODE_ADDR, &vm_mode, PMIC_VM_MODE_ADDR, PMIC_VM_MODE_SHIFT); 
            vm_mode &= 0x3;
	    switch (vm_mode)
	    {
	        case 0:
                    Dis_DramType = TYPE_LPDDR4X;
                    break;
	        case 1:
                    Dis_DramType = TYPE_LPDDR4;
                    break;
	        case 2:
                    Dis_DramType = TYPE_LPDDR4P;
                    break;
	        case 3:
                    Dis_DramType = TYPE_LPDDR3;
                    break;
                default:
                    print("[DRAMC] Discrete DRAM buck check fails !!!\n");
                    ASSERT(0);
            }
#else
            Dis_DramType = mt_get_dram_type_for_dis();
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
            Init_DRAM(Dis_DramType, CBT_NORMAL_MODE, &DramInfo);
  	 
            //try to find discrete dram by DDR2_MODE_REG5(vendor ID)
            for (i = 0; i < num_of_emi_records; i++)
            {
                print("emi_settings[%d].MODE_REG_5:%x,emi_settings[%d].type:%x, vender_id=%x\n", i, emi_settings[i].iLPDDR3_MODE_REG_5, i, emi_settings[i].type, DramInfo.u2MR5VendorID);
                //only check discrete dram type
                if ((emi_settings[i].type & 0x0F00) == 0x0000) 
                {
                    //support for compol discrete dram 
                    if (emi_settings[i].iLPDDR3_MODE_REG_5 == DramInfo.u2MR5VendorID)
                    //if (emi_settings[i].iLPDDR3_MODE_REG_5 == (DramInfo.u2MR5VendorID & 0x000F))
                    {
                        rank0_size = (u64)DramInfo.u4MR8Density[0][0] + (u64)DramInfo.u4MR8Density[1][0];
                        rank1_size = (u64)DramInfo.u4MR8Density[0][1] + (u64)DramInfo.u4MR8Density[1][1];

                        print("emi_settings[%d].DRAM_RANK_SIZE[0]:0x%llx, DRAM_RANK_SIZE[1]:0x%llx, rank0_size:0x%llx, rankl_size:0x%llx\n",i,emi_settings[i].DRAM_RANK_SIZE[0], emi_settings[i].DRAM_RANK_SIZE[1], rank0_size, rank1_size);                            
                        if((emi_settings[i].DRAM_RANK_SIZE[0] == rank0_size) && (emi_settings[i].DRAM_RANK_SIZE[1] == rank1_size))
                        {  
                            mdl_number = i;
                            found = 1;
                            break;
                        } 
                    }
                }
            }
        }
#endif
        print("found:%d,i:%d\n",found,i);   
        //while(1);
    }
    return mdl_number;
}

int get_dram_rank_nr (void)
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
      index = mt_get_mdl_number ();
      if (index < 0 || index >=  num_of_emi_records)
      {
          return 0;
      }

      emi_cona = emi_settings[index].EMI_CONA_VAL;
    } 
#else
    emi_cona = default_emi_setting->EMI_CONA_VAL;
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

void get_dram_rank_size_by_EMI_CONA (u64 dram_rank_size[])
{
    unsigned col_bit, row_bit;
	u64 ch0_rank0_size, ch0_rank1_size, ch1_rank0_size, ch1_rank1_size;
    unsigned int shift_for_16bit = 1;
#ifndef COMBO_MCP   
    unsigned emi_cona = default_emi_setting->EMI_CONA_VAL;
    unsigned emi_conh = default_emi_setting->EMI_CONH_VAL;
#else
    unsigned emi_cona = *(volatile unsigned int*)(EMI_CONA);
    unsigned emi_conh = *(volatile unsigned int*)(EMI_CONH);
#endif

    /* Is it 32-bit or 16-bit I/O */
    if (emi_cona & 0x2)
        shift_for_16bit = 0;

    dram_rank_size[0] = 0;
    dram_rank_size[1] = 0;

    ch0_rank0_size = (emi_conh >> 16) & 0xf;
    ch0_rank1_size = (emi_conh >> 20) & 0xf;
    ch1_rank0_size = (emi_conh >> 24) & 0xf;
    ch1_rank1_size = (emi_conh >> 28) & 0xf;

    //Channel 0
    {   
        if(ch0_rank0_size == 0)
        {
            //rank 0 setting
            col_bit = ((emi_cona >> 4) & 0x03) + 9;
            row_bit = (((emi_cona >> 24) & 0x1) << 2) + ((emi_cona >> 12) & 0x03) + 13;
            dram_rank_size[0] = (u64)(1 << (row_bit + col_bit)) * (u64)(4 >> shift_for_16bit) * 8; // 4 or 2 byte * 8 banks
        }
        else
        {
            dram_rank_size[0] = (u64)(ch0_rank0_size * 256 << 20);
        }

        if (0 != (emi_cona &  (1 << 17)))   //rank 1 exist
        {
            if(ch0_rank1_size == 0)
            {
                col_bit = ((emi_cona >> 6) & 0x03) + 9;
                row_bit = (((emi_cona >> 25) & 0x1) << 2) + ((emi_cona >> 14) & 0x03) + 13;
                dram_rank_size[1] = ((u64)(1 << (row_bit + col_bit)) * (u64)((4 >> shift_for_16bit) * 8)); // 4 or 2 byte * 8 banks
            }
            else
            {
                dram_rank_size[1] = (u64)(ch0_rank1_size * 256 << 20);
            }
        }
    }

    if(0 != ((emi_cona >> 8) & 0x01))     //channel 1 exist
    {
        if(ch1_rank0_size == 0)
        {
            //rank0 setting
            col_bit = ((emi_cona >> 20) & 0x03) + 9;
            row_bit = (((emi_conh >> 4) & 0x1) << 2) + ((emi_cona >> 28) & 0x03) + 13;
            dram_rank_size[0] += ((u64)(1 << (row_bit + col_bit)) * (u64)(4 >> shift_for_16bit) * 8); // 4 or 2 byte * 8 banks
        }
        else
        {
            dram_rank_size[0] += (u64)(ch1_rank0_size * 256 << 20);
        }

        if (0 != (emi_cona &  (1 << 16)))   //rank 1 exist
        {
            if(ch1_rank1_size == 0)
            {
                col_bit = ((emi_cona >> 22) & 0x03) + 9;
                row_bit = (((emi_conh >> 5) & 0x1) << 2) + ((emi_cona >> 30) & 0x03) + 13;
                dram_rank_size[1] += ((u64)(1 << (row_bit + col_bit)) * (u64)((4 >> shift_for_16bit) * 8)); // 4 or 2 byte * 8 banks
            }
            else
            {
                dram_rank_size[1] += (u64)(ch1_rank1_size * 256 << 20);
            }
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

#if defined(PMIC_CHIP_MT6355)
static void set_dramc_vol_by_pmic(void)
{
    reg_vdram1 = regulator_get(NULL,"vdram1");
    if (!reg_vdram1)
        print("regulator_get of reg_vdram1 fail !!!\n");

    reg_vdram2 = regulator_get(NULL,"vdram2");
    if (!reg_vdram2)
        print("regulator_get of reg_vdram2 fail !!!\n");

    reg_vcore = regulator_get(NULL,"vcore");
    if (!reg_vcore)
        print("regulator_get of reg_vcore fail !!!\n");
}
#endif

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
#endif

    index = mt_get_mdl_number ();
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

#if defined(PMIC_CHIP_MT6355)
    regulator_set_mode(reg_vcore, 0x1);
    regulator_set_mode(reg_vdram1, 0x1);
    regulator_set_mode(reg_vdram2, 0x1);
#else
    if ((emi_set->type & 0xF) != TYPE_LPDDR3)
        pmic_disable_vdram();

    //VCORE1 force PWM mode
    pmic_config_interface(MT6351_VCORE_ANA_CON1,0x7800,0xFFFF,0);
    //pmic_config_interface(MT6351_VCORE_ANA_CON2,0x5602,0xFFFF,0);
    pmic_config_interface(MT6351_VCORE_ANA_CON0,0x1,0x1,1);

    if ((emi_set->type & 0xF) == TYPE_LPDDR4)
        fan53526_set_mode(0x3);
    else if ((emi_set->type & 0xF) == TYPE_LPDDR4X)
        fan53528buc08x_set_mode(0x3);
#endif

    if(g_ddr_reserve_enable==1 && g_ddr_reserve_success==0)
        Before_Init_DRAM_While_Reserve_Mode_fail(emi_set->type & 0xF);

#ifndef PMIC_CHIP_MT6355
    /* scy: report error for mix mode dram support for chips with MT6351 */
    if (emi_set->dram_cbt_mode_extern >= 2) {
	print("[dramc] fatal error: report error for mix mode dram support for chips with MT6351\n");
	while(1);
    }
#endif

#if ENABLE_MRR_AFTER_FIRST_K
    Init_DRAM((emi_set->type & 0xF), emi_set->dram_cbt_mode_extern, &DramInfo);
#endif
    
    /*pmic_settings_before_K();*/
    Init_DRAM((emi_set->type & 0xF), emi_set->dram_cbt_mode_extern, NULL);
    /*pmic_settings_after_K();*/

#if defined(PMIC_CHIP_MT6355)
    regulator_set_mode(reg_vcore, 0x0);
    regulator_set_mode(reg_vdram1, 0x0);
    regulator_set_mode(reg_vdram2, 0x0);
#else
    //VCORE1 change to auto mode
    pmic_config_interface(MT6351_VCORE_ANA_CON0,0x0,0x1,1);
    pmic_config_interface(MT6351_VCORE_ANA_CON1,0x7804,0xFFFF,0);
    //pmic_config_interface(MT6351_VCORE_ANA_CON2,0x560A,0xFFFF,0);

    if ((emi_set->type & 0xF) == TYPE_LPDDR4)
        fan53526_set_mode(0x0);
    else if ((emi_set->type & 0xF) == TYPE_LPDDR4X)
        fan53528buc08x_set_mode(0x0);
#endif

#ifdef DRAM_CALIB_LOG
    klog_tail->dram_type = emi_set->type & 0xF;
    klog_tail->check ^= klog_tail->dram_type;
#endif
  
#if 0  
	{
		DRAMC_CTX_T * p = psCurrDramCtx;
		DramcRegDump(p);
	}
#endif
}

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


#ifdef DRAM_HQA
unsigned int hqa_vcore_lpm = HQA_VCORE_LPM;
unsigned int hqa_vcore_hpm = HQA_VCORE_HPM;
unsigned int hqa_vcore_kulpm = HQA_VCORE_KULPM;
unsigned int hqa_vcore_klpm = HQA_VCORE_KLPM;
unsigned int hqa_vcore_khpm = HQA_VCORE_KHPM;

void set_vcore_by_freq(DRAMC_CTX_T *p)
{
    if(u1IsLP4Family(p->dram_type))
    {
        if(p->frequency>=1600)
        {
#if defined(PMIC_CHIP_MT6355)
            regulator_set_voltage(reg_vcore, hqa_vcore_khpm, MAX_VCORE);
            print("[HQA] set vcore to %d mV for HPM\r\n", (hqa_vcore_khpm/1000));
#else
            pmic_config_interface(MT6351_BUCK_VCORE_CON4,hqa_vcore_khpm,0x7F,0); // 0.8V
            pmic_config_interface(MT6351_BUCK_VCORE_CON5,hqa_vcore_khpm,0x7F,0); // 0.8V
            print("[HQA] set vcore to %d mV for HPM\r\n", (600+((625*hqa_vcore_khpm)/100)));
#endif
        }
        else if(p->frequency>=1333)
        {
#if defined(PMIC_CHIP_MT6355)
            //hqa_vcore_klpm -= REDUCE_3_STEPS;
            regulator_set_voltage(reg_vcore, hqa_vcore_klpm, MAX_VCORE);
            print("[HQA] set vcore to %d mV for LPM\r\n", (hqa_vcore_klpm/1000));
#else
            pmic_config_interface(MT6351_BUCK_VCORE_CON4,hqa_vcore_klpm,0x7F,0); // 0.75V
            pmic_config_interface(MT6351_BUCK_VCORE_CON5,hqa_vcore_klpm,0x7F,0); // 0.75V
            print("[HQA] set vcore to %d mV for LPM\r\n", (600+((625*hqa_vcore_klpm)/100)));
#endif
        }
        else if(p->frequency>=800)
        {
#if defined(PMIC_CHIP_MT6355)
            //hqa_vcore_kulpm -= REDUCE_3_STEPS;
            regulator_set_voltage(reg_vcore, hqa_vcore_kulpm, MAX_VCORE);
            print("[HQA] set vcore to %d mV for ULPM\r\n", (hqa_vcore_kulpm/1000));
#else
            pmic_config_interface(MT6351_BUCK_VCORE_CON4,hqa_vcore_kulpm,0x7F,0); // 0.7V
            pmic_config_interface(MT6351_BUCK_VCORE_CON5,hqa_vcore_kulpm,0x7F,0); // 0.7V
            print("[HQA] set vcore to %d mV for ULPM\r\n", (600+((625*hqa_vcore_kulpm)/100)));
#endif
        }
    }
    else if (p->dram_type == TYPE_LPDDR3)
    {
        if(p->frequency>=933)
        {
#if defined(PMIC_CHIP_MT6355)
            regulator_set_voltage(reg_vcore, hqa_vcore_khpm, MAX_VCORE);
            print("[HQA] set vcore to %d mV for HPM\r\n", (hqa_vcore_khpm/1000));
#else
            pmic_config_interface(MT6351_BUCK_VCORE_CON4,hqa_vcore_khpm,0x7F,0); // 0.8V
            pmic_config_interface(MT6351_BUCK_VCORE_CON5,hqa_vcore_khpm,0x7F,0); // 0.8V
            print("[HQA] set vcore to %d mV for HPM\r\n", (600+((625*hqa_vcore_khpm)/100)));
#endif
        }
        else if(p->frequency>=667)
        {
#if defined(PMIC_CHIP_MT6355)
            regulator_set_voltage(reg_vcore, hqa_vcore_khpm, MAX_VCORE);
            print("[HQA] set vcore to %d mV for LPM\r\n", (hqa_vcore_khpm/1000));
#else
            pmic_config_interface(MT6351_BUCK_VCORE_CON4,hqa_vcore_khpm,0x7F,0); // 0.8V
            pmic_config_interface(MT6351_BUCK_VCORE_CON5,hqa_vcore_khpm,0x7F,0); // 0.8V
            print("[HQA] set vcore to %d mV for LPM\r\n", (600+((625*hqa_vcore_khpm)/100)));
#endif
        }
        else if(p->frequency>=467)
        {
#if defined(PMIC_CHIP_MT6355)
            //hqa_vcore_kulpm -= REDUCE_3_STEPS;
            regulator_set_voltage(reg_vcore, hqa_vcore_kulpm, MAX_VCORE);
            print("[HQA] set vcore to %d mV for ULPM\r\n", (hqa_vcore_kulpm/1000));
#else
            pmic_config_interface(MT6351_BUCK_VCORE_CON4,hqa_vcore_kulpm,0x7F,0); // 0.7V
            pmic_config_interface(MT6351_BUCK_VCORE_CON5,hqa_vcore_kulpm,0x7F,0); // 0.7V
            print("[HQA] set vcore to %d mV for ULPM\r\n", (600+((625*hqa_vcore_kulpm)/100)));
#endif
        }
    }
}

int calculate_voltage(unsigned int x)
{
    return (600+((625*x)/100));
}

static void set_vdram(unsigned int vdram)
{
#if defined(PMIC_CHIP_MT6355)
    regulator_set_voltage(reg_vdram1, vdram, MAX_VDRAM1);
#else
#if defined(HQA_LPDDR3)
    pmic_config_interface(MT6351_VDRAM_ANA_CON0, vdram, 0x71f,0);
#elif defined(HQA_LPDDR4) || defined(HQA_LPDDR4X)
    fan53526_set_voltage((unsigned long)vdram);
#endif
#endif
}

static void set_vddq(unsigned int vddq)
{
#if defined(PMIC_CHIP_MT6355)
    regulator_set_voltage(reg_vdram2, vddq, MAX_VDRAM2);
#else
#ifdef HQA_LPDDR4X
    fan53528buc08x_set_voltage((unsigned long)vddq);
#endif
#endif
}

static unsigned int get_vdram(void)
{
    unsigned int vdram;

#if defined(PMIC_CHIP_MT6355)
    vdram = regulator_get_voltage(reg_vdram1);
#else
#if defined(HQA_LPDDR3)
    pmic_read_interface(MT6351_VDRAM_ANA_CON0, &vdram, 0x71f, 0);
#elif defined(HQA_LPDDR4) || defined(HQA_LPDDR4X)
    vdram = (unsigned int)fan53526_get_voltage();
#endif
#endif

    return vdram;
}

static unsigned int get_vddq(void)
{
    unsigned int vddq = 0;

#if defined(PMIC_CHIP_MT6355)
    vddq = regulator_get_voltage(reg_vdram2);
#else
#ifdef HQA_LPDDR4X
    vddq = (unsigned int)fan53528buc08x_get_voltage();
#endif
#endif
    return vddq;
}

static void print_HQA_voltage(void)
{
#if defined(HVCORE_HVDRAM)
    print("[HQA] Vcore HV, Vdram HV\n");
#elif defined(NVCORE_NVDRAM)
    print("[HQA] Vcore NV, Vdram NV\n");
#elif defined(LVCORE_LVDRAM)
    print("[HQA] Vcore LV, Vdram LV\n");
#elif defined(HVCORE_LVDRAM)
    print("[HQA] Vcore HV, Vdram LV\n");
#elif defined(LVCORE_HVDRAM)
    print("[HQA] Vcore LV, Vdram HV\n");
#else
    print("[HQA] Undefined HQA condition\n");
#endif

#if defined(PMIC_CHIP_MT6355)
    print("[HQA] Vcore = %d uV(should be %d uV)\n",
           regulator_get_voltage(reg_vcore), HQA_VCORE_LPM);
    print("[HQA] Vdram = %d uV (should be %d uV)\n",
           get_vdram(), HQA_VDRAM);
    print("[HQA] vddq = %d uV (should be %d uV)\n",
           get_vddq(), HQA_VDDQ);
#else
    print("[HQA] Vcore = %d mV(should be %d mV)\n",
           calculate_voltage(upmu_get_reg_value(MT6351_BUCK_VCORE_CON4)), calculate_voltage(HQA_VCORE_LPM));
    print("[HQA] Vdram = 0x%x (should be 0x%x)\n",
           get_vdram(), HQA_VDRAM);
#ifdef HQA_LPDDR4X
    print("[HQA] vddq = 0x%x (should be 0x%x)\n",
           get_vddq(), HQA_VDDQ);
#endif
#endif
}
#endif //DRAM_HQA

void dram_HQA_adjust_voltage(void)
{
#if defined(PMIC_CHIP_MT6355)
    set_dramc_vol_by_pmic();
#endif
#ifdef DRAM_HQA
#if defined(PMIC_CHIP_MT6355)
    regulator_set_voltage(reg_vcore, HQA_VCORE_LPM, MAX_VCORE);
#else
    pmic_config_interface(MT6351_BUCK_VCORE_CON4, HQA_VCORE_LPM, 0x7F, 0);
    pmic_config_interface(MT6351_BUCK_VCORE_CON5, HQA_VCORE_LPM, 0x7F, 0);
#endif
    set_vdram(HQA_VDRAM);
    set_vddq(HQA_VDDQ);

    print_HQA_voltage();
#endif
}

void enable_4GB_mode(void)
{
        int i;
        u64 dram_rank_size[4] = {0,0,0,0};
        u64 total_dram_size = 0;
        get_dram_rank_size(dram_rank_size);
	enable_4GB_flag = 0;
        for(i=0; i<4; i++){
		total_dram_size += dram_rank_size[i];
		print("dram_rank_size[%d] = 0x%x\n", i, dram_rank_size[i]);
        }
#if 1
        if(total_dram_size > 0xC0000000ULL) {
		enable_4GB_flag = 1;
		print("[Enable 4GB Support] Total_dram_size = 0x%llx\n,flag:0x%x", total_dram_size,enable_4GB_flag);
		*(volatile unsigned int *)(0x10003208) |= 1 << 15;
		*(volatile unsigned int *)(0x10001f00) |= 1 << 13;
        }
#endif
}

unsigned int Is_enable_4GB(void)
{
	print("[Enable 4GB Support] 4GB_flag 0x%x\n",enable_4GB_flag);
	return enable_4GB_flag;
}
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

void dram_klog_init(void)
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

    klog_head->guard = CALIB_LOG_MAGIC;
    klog_head->rtc_yea_mth = (yea << 16) | mth;
    klog_head->rtc_dom_hou = (dom << 16) | hou;
    klog_head->rtc_min_sec = (min << 16) | sec;
    klog_tail->dram_type = 0;
    klog_tail->guard = CALIB_LOG_MAGIC;
    klog_tail->check = (klog_head->rtc_yea_mth) ^ (klog_head->rtc_dom_hou) ^ (klog_head->rtc_min_sec) ^
                       (klog_head->guard) ^ (klog_tail->guard);

    print("[DRAM Klog] init SRAM space for Klog at %d/%d/%d %d:%d:%d\n", yea, mth, dom, hou, min, sec);

    return;
}

int dram_klog_check(void)
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

    if((u4Offset+u4WordCnt) > ((CALIB_LOG_SIZE-sizeof(DRAM_KLOG_HEAD)-sizeof(DRAM_KLOG_TAIL))/4)) {
        print("[DRAM Klog] fail to write Klog due to crossing the boundary\n");
        return -1;
    }
    else if(((unsigned int)0xffffffff-u4Offset) < u4WordCnt) {
        print("[DRAM Klog] fail to write Klog due to overflow\n");
        return -1;
    }
    else {
        for(i=0; i<u4WordCnt; i++) {
            klog_tail->check ^= *(klog_data+u4Offset+i) ^ *(pu4Src+i);
            *(klog_data+u4Offset+i) = *(pu4Src+i);
        }
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
#endif //DRAM_CALIB_LOG

#ifdef LAST_DRAMC
static unsigned int is_last_dramc_initialized(void)
{
	LAST_DRAMC_INFO_T* last_dramc_info_ptr;

	last_dramc_info_ptr = (LAST_DRAMC_INFO_T*) DRAM_DEBUG_SRAM_BASE;
	if(last_dramc_info_ptr->ta2_result_magic != LAST_DRAMC_MAGIC_PATTERN) {
		return 0;
	} else {
		return 1;
	}
}

void update_last_dramc_info(void)
{
    LAST_DRAMC_INFO_T* last_dramc_info_ptr;
    unsigned int latch_result;
    unsigned int temp;
    unsigned int *curr;

    last_dramc_info_ptr = (LAST_DRAMC_INFO_T*) DRAM_DEBUG_SRAM_BASE;

    // init checksum and magic pattern
    if(last_dramc_info_ptr->ta2_result_magic != LAST_DRAMC_MAGIC_PATTERN) {
        memset((void*)DRAM_DEBUG_SRAM_BASE, 0, LAST_DRAMC_SRAM_SIZE);
        print("[LastDRAMC] init SRAM space\n");
	last_dramc_info_ptr->ta2_result_magic = LAST_DRAMC_MAGIC_PATTERN;
        last_dramc_info_ptr->ta2_result_checksum = LAST_DRAMC_MAGIC_PATTERN;
        last_dramc_info_ptr->reboot_count = 0;
    } else {
        last_dramc_info_ptr->ta2_result_checksum ^= last_dramc_info_ptr->reboot_count;
        last_dramc_info_ptr->reboot_count++;
        last_dramc_info_ptr->ta2_result_checksum ^= last_dramc_info_ptr->reboot_count;
    }

    // read data from latch register and reset
    //print("[LastDRAMC] latch result before RST: %x, %x\n",
    //    u4IO32Read4B(DRAMC_REG_WDT_DBG_SIGNAL),
    //    u4IO32Read4B(DRAMC_REG_WDT_DBG_SIGNAL+(1<<16)));
    latch_result = u4IO32Read4B(DRAMC_REG_WDT_DBG_SIGNAL) & 0xFF;
    temp = u4IO32Read4B(DRAMC_REG_DDRCONF0);
    vIO32Write4B(DRAMC_REG_DDRCONF0, temp | 0x00000004);
    vIO32Write4B(DRAMC_REG_DDRCONF0, temp & 0xFFFFFFFB);
    if(*(volatile unsigned *)EMI_CONA & 0x300) {
        latch_result = (latch_result << 8) | (u4IO32Read4B(DRAMC_REG_WDT_DBG_SIGNAL+(1<<16)) & 0xFF);
        temp = u4IO32Read4B(DRAMC_REG_DDRCONF0+(1<<16));
        vIO32Write4B(DRAMC_REG_DDRCONF0+(1<<16), temp | 0x00000004);
        vIO32Write4B(DRAMC_REG_DDRCONF0+(1<<16), temp & 0xFFFFFFFB);
    }
    //print("[LastDRAMC] latch result after RST: %x, %x\n",
    //    u4IO32Read4B(DRAMC_REG_WDT_DBG_SIGNAL),
    //    u4IO32Read4B(DRAMC_REG_WDT_DBG_SIGNAL+(1<<16)));

    last_dramc_info_ptr->ta2_result_checksum ^= last_dramc_info_ptr->ta2_result_past ^ latch_result;
    last_dramc_info_ptr->ta2_result_past = last_dramc_info_ptr->ta2_result_last;
    last_dramc_info_ptr->ta2_result_last = latch_result;

    for(curr=(unsigned int*)DRAM_DEBUG_SRAM_BASE; curr<(unsigned int*)(DRAM_DEBUG_SRAM_BASE+LAST_DRAMC_SRAM_SIZE); curr++) {
        print("[LastDRAMC] 0x%x: 0x%x\n", curr, *curr);
    }

    return;
}

void init_ta2_all_channel(void)
{
    update_last_dramc_info();

    init_ta2_single_channel(0);
    if(*(volatile unsigned *)EMI_CONA & 0x300) {
        print("[LastDRAMC] two channels\n");
        init_ta2_single_channel(1);
    }
}

void init_ta2_single_channel(unsigned int channel)
{
    unsigned int temp;
    unsigned int matype;

    if(channel == 0)
        matype = *(volatile unsigned *)EMI_CONA >> 4;
    else
        matype = *(volatile unsigned *)EMI_CONA >> 20;
    matype = ((matype & 0x3) + 1) << 30;

    // disable self test engine1 and self test engine2
    temp = u4IO32Read4B(DRAMC_REG_TEST2_3+(channel<<16)) & 0x1FFFFFFF;
    vIO32Write4B(DRAMC_REG_TEST2_3+(channel<<16), temp);

    // set MATYPE
    temp = (u4IO32Read4B(DRAMC_REG_SHU_CONF0+(channel<<16))&0x7FFFFFFF) | matype;
    vIO32Write4B(DRAMC_REG_SHU_CONF0+(channel<<16), temp);
    temp = (u4IO32Read4B(DRAMC_REG_SHU2_CONF0+(channel<<16))&0x7FFFFFFF) | matype;
    vIO32Write4B(DRAMC_REG_SHU2_CONF0+(channel<<16), temp);
    temp = (u4IO32Read4B(DRAMC_REG_SHU3_CONF0+(channel<<16))&0x7FFFFFFF) | matype;
    vIO32Write4B(DRAMC_REG_SHU3_CONF0+(channel<<16), temp);
    temp = (u4IO32Read4B(DRAMC_REG_SHU4_CONF0+(channel<<16))&0x7FFFFFFF) | matype;
    vIO32Write4B(DRAMC_REG_SHU4_CONF0+(channel<<16), temp);

    // set rank address for test agent to rank0
    temp = u4IO32Read4B(DRAMC_REG_TEST2_4+(channel<<16)) & 0x8CFFFFFF;
    temp |= (0x00 << 24);
    vIO32Write4B(DRAMC_REG_TEST2_4+(channel<<16), temp);

    // set base address for test agent to 0x0
    temp = (u4IO32Read4B(DRAMC_REG_TEST2_1+(channel<<16))&0x0000000F) | (0x0<<4);
    vIO32Write4B(DRAMC_REG_TEST2_1+(channel<<16), temp);

    // set test length (offset) to 0x20
    temp = (u4IO32Read4B(DRAMC_REG_TEST2_2+(channel<<16))&0x0000000F) | (0x20<<4);
    vIO32Write4B(DRAMC_REG_TEST2_2+(channel<<16), temp);

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

	for (chn = 0; chn < 2; ++chn) {
		if (u4IO32Read4B(DRAMC_REG_WDT_DBG_SIGNAL+(chn << POS_BANK_NUM)) & 0x80) {
			print("[dramc] found gating error in channel %d (0x%x)\n",
					chn, u4IO32Read4B(DRAMC_REG_WDT_DBG_SIGNAL+(chn << POS_BANK_NUM)));
			ret |= (1 << chn);
		}
	}

	return ret;
}

void dram_fatal_exception_detection_start(void)
{
	u32 segment = (seclib_get_devinfo_with_index(30) & 0x000000E0) >> 5;
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
	part_dram_data_addr = get_part_addr("boot_para"); // addr = 0x8000
	if (part_dram_data_addr != 0x0)
		print("[dramc] init partition address is 0x%llx\n", part_dram_data_addr);
	else {
		print("[dramc] init partition address is incorrect !!!\n");
		ASSERT(0);
	}
#endif

	if ((g_boot_reason == BR_POWER_KEY) || (g_boot_reason == BR_USB)
			|| mtk_wdt_is_pmic_full_reset() || (is_last_dramc_initialized() == 0)){
		/* cold boot: initialize last_dram_fatal_err_flag and dram_fatal_err_flag */
		print("[dramc] init SRAM region for DRAM exception detection\n");
		DRV_WriteReg32(LAST_DRAM_FATAL_ERR_FLAG, 0x0);
		DRV_WriteReg32(STORAGE_API_ERR_FLAG, 0x0);
		dram_fatal_init_stberr();
	} else {
		DRV_WriteReg32(LAST_DRAM_FATAL_ERR_FLAG, DRV_Reg32(DRAM_FATAL_ERR_FLAG));
		DRV_WriteReg32(STORAGE_API_ERR_FLAG, 0x0);
		dram_fatal_backup_stberr();
		dram_fatal_init_stberr();
	}

	/* Kibo Plus Only for GAT Tools*/
	if ((segment == 0x3) || (segment == 0x7))
		dram_fatal_set_kibo_plus_only();

	DRV_WriteReg32(DRAM_FATAL_ERR_FLAG, 1 << OFFSET_DRAM_FATAL_ERR);
	dsb();
}

void dram_fatal_exception_detection_end(void)
{
	/* not to clear OFFSET_DDR_RSV_MODE_FLOW for the indication of DDR reserve mode */
	DRV_WriteReg32(DRAM_FATAL_ERR_FLAG, DRV_Reg32(DRAM_FATAL_ERR_FLAG) & (1 << OFFSET_DDR_RSV_MODE_FLOW));
	dsb();
}

unsigned int check_dram_fatal_exception(void)
{
	print("[dramc] DRAM_FATAL_ERR_FLAG = 0x%x\n", DRV_Reg32(DRAM_FATAL_ERR_FLAG));
	return ((DRV_Reg32(DRAM_FATAL_ERR_FLAG) & ~((1 << OFFSET_DRAM_FATAL_ERR)|(1 << OFFSET_DDR_RSV_MODE_FLOW))) != 0x0) ? 1 : 0;
}

unsigned int check_last_dram_fatal_exception(void)
{
	print("[dramc] LAST_DRAM_FATAL_ERR_FLAG = 0x%x\n", DRV_Reg32(LAST_DRAM_FATAL_ERR_FLAG));
	return ((DRV_Reg32(LAST_DRAM_FATAL_ERR_FLAG) & ~(DDR_RSV_MODE_ERR_MASK)) != 0x0) ? 1 : 0;
}

void dram_fatal_set_ta2_err(unsigned int chn, unsigned int err_code)
{
	unsigned int shift = OFFSET_DRAM_TA2_ERR + 2 * chn, ret;

	if (chn > 1)
		return;

	ret = DRV_Reg32(DRAM_FATAL_ERR_FLAG) & ~(0x7 << shift);
	DRV_WriteReg32(DRAM_FATAL_ERR_FLAG, ret | ((err_code & 0x7) << shift));
	dsb();
}

void dram_fatal_set_gating_err(unsigned int chn, unsigned int err_code)
{
	unsigned int shift = OFFSET_DRAM_GATING_ERR + 4 * chn, ret;

	if (chn > 1)
		return;

	ret = DRV_Reg32(DRAM_FATAL_ERR_FLAG) & ~(0xf << shift);
	DRV_WriteReg32(DRAM_FATAL_ERR_FLAG, ret | ((err_code & 0xf) << shift));
	dsb();
}

void dram_fatal_init_stberr(void)
{
	DRV_WriteReg32(DRAM_GATING_ERR_CHA_RK0, 0x0);
	DRV_WriteReg32(DRAM_GATING_ERR_CHA_RK1, 0x0);
	DRV_WriteReg32(DRAM_GATING_ERR_CHB_RK0, 0x0);
	DRV_WriteReg32(DRAM_GATING_ERR_CHB_RK1, 0x0);

	dsb();
}

void dram_fatal_backup_stberr(void)
{
	DRV_WriteReg32(LAST_DRAM_GATING_ERR_CHA_RK0, DRV_Reg32(DRAM_GATING_ERR_CHA_RK0));
	DRV_WriteReg32(LAST_DRAM_GATING_ERR_CHA_RK1, DRV_Reg32(DRAM_GATING_ERR_CHA_RK1));
	DRV_WriteReg32(LAST_DRAM_GATING_ERR_CHB_RK0, DRV_Reg32(DRAM_GATING_ERR_CHB_RK0));
	DRV_WriteReg32(LAST_DRAM_GATING_ERR_CHB_RK1, DRV_Reg32(DRAM_GATING_ERR_CHB_RK1));

	dsb();
}

void dram_fatal_set_stberr(unsigned int chn, unsigned int rk, unsigned int err_code)
{
	if (chn > 1)
		return;

	if (rk == 0) {
		DRV_WriteReg32(DRAM_GATING_ERR_CHA_RK0 + (8 * chn), err_code);
	} else if (rk == 1) {
		DRV_WriteReg32(DRAM_GATING_ERR_CHA_RK1 + (8 * chn), err_code);
	}
	dsb();
}

void dram_fatal_set_cpu_rw_err(unsigned int err_code)
{
	unsigned int ret;

	ret = DRV_Reg32(DRAM_FATAL_ERR_FLAG) & ~(0x3 << OFFSET_CPU_RW_ERR);
	DRV_WriteReg32(DRAM_FATAL_ERR_FLAG, ret | ((err_code & 0x3) << OFFSET_CPU_RW_ERR));
	dsb();
}

void dram_fatal_set_err(unsigned int err_code, unsigned int mask, unsigned int offset)
{
	unsigned int ret;

	ret = DRV_Reg32(DRAM_FATAL_ERR_FLAG) & ~(mask << offset);
	DRV_WriteReg32(DRAM_FATAL_ERR_FLAG, ret | ((err_code & mask) << offset));
	dsb();
}
#endif //LAST_DRAMC


#if SUPPORT_SAVE_TIME_FOR_CALIBRATION

#if !__ETT__
#include <blkdev.h>
#include <partition.h>
//Darren-#include <pl_version.h>
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
	DRV_WriteReg32(STORAGE_API_ERR_FLAG, g_dram_storage_api_err_code);
	dsb();
#endif
}

#endif

