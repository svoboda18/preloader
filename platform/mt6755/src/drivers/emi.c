// 6595_DDR.cpp : Defines the entry point for the console application.
//

#include <emi.h>
#include <typedefs.h>
#include <stdlib.h>
#include "dramc_common.h"
#include "dramc_register.h"
#include "dramc_pi_api.h"
#include "dram_buffer.h"
#include "emi.h"
#include "platform.h"
#include "emi_hw.h"
#include "wdt.h"
#include "custom_emi.h"
#include "upmu_hw.h"

#define DRAM_BASE 0x40000000ULL
 
#if CFG_BOOT_ARGUMENT
#define bootarg g_dram_buf->bootarg
#endif

int DQS_PRINT = 1;
int emi_setting_index = -1;
extern DRAMC_CTX_T *psCurrDramCtx;
EMI_SETTINGS *default_emi_setting;
unsigned int enable_4GB_flag=0;
static int enable_combo_dis = 0;

#if fcFOR_CHIP_ID == fcK2 || fcFOR_CHIP_ID == fcJADE

#define EMI_CONA		(EMI_APB_BASE + 0x0)
#define EMI_CONB		(EMI_APB_BASE + 0x8)
#if fcFOR_CHIP_ID == fcK2
#define EMI_CONC		(EMI_APB_BASE + 0xc)
#endif
#if fcFOR_CHIP_ID == fcJADE
#define EMI_CONC		(EMI_APB_BASE + 0x10)
#endif
#define EMI_COND		(EMI_APB_BASE + 0x18)
#define EMI_CONE		(EMI_APB_BASE + 0x20)
#define EMI_CONG		(EMI_APB_BASE + 0x30)
#define EMI_CONH		(EMI_APB_BASE + 0x38)
#define EMI_CONI		(EMI_APB_BASE + 0x40)
#define EMI_MDCT		(EMI_APB_BASE + 0x78)
#define EMI_TESTB	(EMI_APB_BASE + 0xe8)
#define EMI_TESTC	(EMI_APB_BASE + 0xf0)
#define EMI_TESTD	(EMI_APB_BASE + 0xf8)
#define EMI_TEST0	(EMI_APB_BASE + 0xd0)
#define EMI_TEST1	(EMI_APB_BASE + 0xd8)
#define EMI_ARBI		(EMI_APB_BASE + 0x140)
#define EMI_ARBI_2ND		(EMI_APB_BASE + 0x144)
#define EMI_ARBJ		(EMI_APB_BASE + 0x148)
#define EMI_ARBJ_2ND		(EMI_APB_BASE + 0x14c)
#define EMI_ARBK		(EMI_APB_BASE + 0x150)
#define EMI_ARBK_2ND	(EMI_APB_BASE + 0x154)
#define EMI_SLCT		(EMI_APB_BASE + 0x158)
#define EMI_BMEN		(EMI_APB_BASE + 0x400)	
#define EMI_ARBA		(EMI_APB_BASE + 0x100)
#define EMI_ARBB		(EMI_APB_BASE + 0x108)
#define EMI_ARBC		(EMI_APB_BASE + 0x110)
#define EMI_ARBD		(EMI_APB_BASE + 0x118)
#define EMI_ARBE		(EMI_APB_BASE + 0x120)
#define EMI_ARBF		(EMI_APB_BASE + 0x128)
#define EMI_ARBH		(EMI_APB_BASE + 0x138)
#define EMI_CONM		(EMI_APB_BASE + 0x60)

void EMI_Init(DRAMC_CTX_T *p)
{
    EMI_SETTINGS *emi_set;
	
    if(emi_setting_index == -1)
        emi_set = default_emi_setting;
    else
        emi_set = &emi_settings[emi_setting_index];

		*((UINT32P)(EMI_APB_BASE+0x00000100))= 0x7f807C46;
		*((UINT32P)(EMI_APB_BASE+0x00000108))= 0xa0a070d5;
		*((UINT32P)(EMI_APB_BASE+0x00000110))= 0x80807043;
		*((UINT32P)(EMI_APB_BASE+0x00000118))= 0x40007a49;
		*((UINT32P)(EMI_APB_BASE+0x00000120))= 0x4040604f;
		*((UINT32P)(EMI_APB_BASE+0x00000128))= 0xa0a0704b;
		*((UINT32P)(EMI_APB_BASE+0x00000134))= 0xa0a0704b; // Please set 0x134=0x128

	
		//#ifdef LP3_1333
		*(volatile unsigned *)(EMI_APB_BASE+0x00000008)=0x0d1e293a;
		*(volatile unsigned *)(EMI_APB_BASE+0x00000010)=0x09190819;
		*(volatile unsigned *)(EMI_APB_BASE+0x00000030)=0x2b2b282e;
		//#endif
		*(volatile unsigned *)(EMI_APB_BASE+0x00000018)=0x3657587a; //SMI threthold
		*(volatile unsigned *)(EMI_APB_BASE+0x00000020)=0xFFFF0848;
		
		*(volatile unsigned *)(EMI_APB_BASE+0x00000038)=emi_set->EMI_CONH_VAL | 0x00000000; //*(volatile unsigned *)(EMI_APB_BASE+0x00000038)=0x00000000;
		*(volatile unsigned *)(EMI_APB_BASE+0x00000060)=0x000006ff;
		
		*(volatile unsigned *)(EMI_APB_BASE+0x00000078)=0x99bb0c3f;// defer ultra excpt MDMCU, R/W reserv_buf = 3
		
		*(volatile unsigned *)(EMI_APB_BASE+0x000000d0)=0xfCCCCCCC;//R/8 W/8 outstanding [31:28]=0xf MDMCU W OSTD=15 
		*(volatile unsigned *)(EMI_APB_BASE+0x000000d8)=0xcccccccc;//R/8 W/8 outstanding
		
		*(volatile unsigned *)(EMI_APB_BASE+0x000000e8)=0x00060027;
		
		*(volatile unsigned *)(EMI_APB_BASE+0x000000f0)=0x38460000; //[16] 1:enable urgent read
		
		*(volatile unsigned *)(EMI_APB_BASE+0x000000f8)=0x00000000;
		
		*(volatile unsigned *)(EMI_APB_BASE+0x00000140)=0x20406188;//0x12202488;   // 83 for low latency
		*(volatile unsigned *)(EMI_APB_BASE+0x00000144)=0x20406188;//0x12202488; //new add
		*(volatile unsigned *)(EMI_APB_BASE+0x00000148)=0x9719595e;//0323 chg, ori :0x00462f2f
		*(volatile unsigned *)(EMI_APB_BASE+0x0000014c)=0x9719595e; // new add
		*(volatile unsigned *)(EMI_APB_BASE+0x00000150)=0x64f3fc79;
		*(volatile unsigned *)(EMI_APB_BASE+0x00000154)=0x64f3fc79;
		*(volatile unsigned *)(EMI_APB_BASE+0x00000158)=0xff01ff00;// ???????????????????????0x08090800; 
		
		//==============Scramble address==========================
		*(volatile unsigned *)(EMI_APB_BASE+0x00000028)=0x04210000;
		
		
		*(volatile unsigned *)(EMI_APB_BASE+0x00000400)=0x00ff0001;
		// check RESP error 
		*((UINT32P)(EMI_APB_BASE+0x000001c0))=0x10000000;
		*((UINT32P)(EMI_APB_BASE+0x000001c8))=0x10000000;
		*((UINT32P)(EMI_APB_BASE+0x000001d0))=0x10000000;
		*((UINT32P)(EMI_APB_BASE+0x00000200))=0x10000000;

  	*(volatile unsigned *)EMI_CONA = emi_set->EMI_CONA_VAL;	

}

#endif

unsigned int is_TB_setting(void)
{
	return 0;
#if 0
   int data;
  
    	//return 1;//TB
   data = Reg_Readl(0x10206040);
   if((data & 0x80000000)==0)
     {
    	return 0; //SP
     }
     else
     {
     	return 1;//TB
     }
 #endif   
}


/* Turbo Dram/GPU B */
#define GPU800MHz_DRAM1866MHz_MASK	0x80000000
int	GPU800MHz_DRAM1866MHz_Flag = 0;
/* Turbo Dram/GPU E */

int mt_get_freq_setting(DRAMC_CTX_T *p)
{
#if 0
    int freq;

    if(is_TB_setting()!=0)
    freq = 890;
  
	/* Turbo Dram/GPU B */
	print("GPU/DRAM EFUSE 02 = 0x%x\n", seclib_get_devinfo_with_index(3));
	GPU800MHz_DRAM1866MHz_Flag = seclib_get_devinfo_with_index(3) & GPU800MHz_DRAM1866MHz_MASK;
	if (GPU800MHz_DRAM1866MHz_Flag)
		freq = 890;
	/* Turbo Dram/GPU E */

    return freq;
#endif
    return DUAL_FREQ_HIGH;


}

void CHA_HWGW_Print(DRAMC_CTX_T *p)
	{
	static U8 LowFreq_Min_R0_DQS[4] = {0xff, 0xff, 0xff, 0xff};
	static U8 LowFreq_Max_R0_DQS[4] = {0x00, 0x00, 0x00, 0x00};
	static U8 HighFreq_Min_R0_DQS[4] = {0xff, 0xff, 0xff, 0xff};
	static U8 HighFreq_Max_R0_DQS[4] = {0x00, 0x00, 0x00, 0x00};
	U8 ucstatus = 0, R0_DQS[4], Count;
	U32 u4value, u4value1, u4value2;

	p->channel = CHANNEL_A;
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x374), &u4value);
	R0_DQS[0] = (u4value >> 0) & 0x7f;
	R0_DQS[1] = (u4value >> 8) & 0x7f;
	R0_DQS[2] = (u4value >> 16) & 0x7f;
	R0_DQS[3] = (u4value >> 24) & 0x7f;
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x94), &u4value1);
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x840), &u4value2);

	mcSHOW_DBG_MSG(("[Channel %d]Clock=%d,Reg.94h=%xh,Reg.840h=%xh,Reg.374h=%xh\n", p->channel, p->frequency, u4value1, u4value2, u4value));
	mcSHOW_DBG_MSG(("mt_pll_post_init: mt_get_mem_freq = %dKhz\n", mt_get_mem_freq()));

#ifdef DUAL_FREQ_TEST
	if (p->frequency == DUAL_FREQ_LOW)
	{
		for (Count=0; Count<4; Count++)
		{
			if (R0_DQS[Count] < LowFreq_Min_R0_DQS[Count])
			{
				LowFreq_Min_R0_DQS[Count] = R0_DQS[Count];
			}
			if (R0_DQS[Count]  > LowFreq_Max_R0_DQS[Count])
			{
				LowFreq_Max_R0_DQS[Count] = R0_DQS[Count];
			}
		}

		mcSHOW_DBG_MSG(("[Channel %d]Clock=%d,DQS0=(%d, %d),DQS1=(%d, %d),DQS2=(%d, %d),DQS3=(%d, %d)\n", 
			p->channel, p->frequency, 
			LowFreq_Min_R0_DQS[0], LowFreq_Max_R0_DQS[0], LowFreq_Min_R0_DQS[1], LowFreq_Max_R0_DQS[1], 
			LowFreq_Min_R0_DQS[2], LowFreq_Max_R0_DQS[2], LowFreq_Min_R0_DQS[3], LowFreq_Max_R0_DQS[3]));
	}
	else
	{
		for (Count=0; Count<4; Count++)
		{
			if (R0_DQS[Count] < HighFreq_Min_R0_DQS[Count])
			{
				HighFreq_Min_R0_DQS[Count] = R0_DQS[Count];
			}
			if (R0_DQS[Count]  > HighFreq_Max_R0_DQS[Count])
			{
				HighFreq_Max_R0_DQS[Count] = R0_DQS[Count];
			}
		}	
		mcSHOW_DBG_MSG(("[Channel %d]Clock=%d,DQS0=(%d, %d),DQS1=(%d, %d),DQS2=(%d, %d),DQS3=(%d, %d)\n", 
			p->channel, p->frequency, 
			HighFreq_Min_R0_DQS[0], HighFreq_Max_R0_DQS[0], HighFreq_Min_R0_DQS[1], HighFreq_Max_R0_DQS[1], 
			HighFreq_Min_R0_DQS[2], HighFreq_Max_R0_DQS[2], HighFreq_Min_R0_DQS[3], HighFreq_Max_R0_DQS[3]));

	}
	#else
	for (Count=0; Count<4; Count++)
	{
		if (R0_DQS[Count] < LowFreq_Min_R0_DQS[Count])
		{
			LowFreq_Min_R0_DQS[Count] = R0_DQS[Count];
		}
		if (R0_DQS[Count]  > LowFreq_Max_R0_DQS[Count])
		{
			LowFreq_Max_R0_DQS[Count] = R0_DQS[Count];
		}
	}
	mcSHOW_DBG_MSG(("[Channel %d]Clock=%d,DQS0=(%d, %d),DQS1=(%d, %d),DQS2=(%d, %d),DQS3=(%d, %d)\n", 
		p->channel, p->frequency, 
		LowFreq_Min_R0_DQS[0], LowFreq_Max_R0_DQS[0], LowFreq_Min_R0_DQS[1], LowFreq_Max_R0_DQS[1], 
		LowFreq_Min_R0_DQS[2], LowFreq_Max_R0_DQS[2], LowFreq_Min_R0_DQS[3], LowFreq_Max_R0_DQS[3]));

#endif

#ifdef TEMP_SENSOR_ENABLE
	p->channel = CHANNEL_A;
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x3B8), &u4value);
        mcSHOW_ERR_MSG(("[CHA] MRR(MR4) Reg.3B8h[10:8]=%x\n", (u4value & 0x700)>>8));
	p->channel = CHANNEL_B;
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x3B8), &u4value);
        mcSHOW_ERR_MSG(("[CHB] MRR(MR4) Reg.3B8h[10:8]=%x\n", (u4value & 0x700)>>8));
#endif	

}

void Dump_EMIRegisters(DRAMC_CTX_T *p)
{
	U8 ucstatus = 0;
	U32 uiAddr;
	U32 u4value;

	for (uiAddr=0; uiAddr<=0x160; uiAddr+=4)
	{
		mcSHOW_DBG_MSG2(("EMI offset:%x, value:%x\n", uiAddr, *(volatile unsigned *)(EMI_APB_BASE+uiAddr)));
	}
}

void print_DBG_info(DRAMC_CTX_T *p)
{
    unsigned int addr = 0x0;
    U32 u4value;

#ifdef DDR_INIT_TIME_PROFILING    
    return;
#endif

    mcSHOW_DBG_MSG2(("EMI_CONA=%x\n",*(volatile unsigned *)(EMI_APB_BASE+0x00000000)));
    mcSHOW_DBG_MSG2(("EMI_CONH=%x\n",*(volatile unsigned *)(EMI_APB_BASE+0x00000038)));

    mcSHOW_DBG_MSG2(("=====================DBG=====================\n"));
    for(addr = 0x0; addr <= 0x900; addr +=4)
    {
        ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(addr), &u4value);
        mcSHOW_DBG_MSG2(("addr:%x, value:%x\n",addr, u4value));
    }

    mcSHOW_DBG_MSG2(("TOP related\n"));
    u4value = Reg_Readl(MEM_DCM_CTRL);
    mcSHOW_DBG_MSG2(("addr:%x, value:%x\n", MEM_DCM_CTRL, u4value));
    u4value = Reg_Readl(DFS_MEM_DCM_CTRL);
    mcSHOW_DBG_MSG2(("addr:%x, value:%x\n", DFS_MEM_DCM_CTRL, u4value));
    u4value = Reg_Readl(CLK_CFG_0_CLR);
    mcSHOW_DBG_MSG2(("addr:%x, value:%x\n", CLK_CFG_0_CLR, u4value));
    u4value = Reg_Readl(CLK_CFG_0_SET);
    mcSHOW_DBG_MSG2(("addr:%x, value:%x\n", CLK_CFG_0_SET, u4value));
    u4value = Reg_Readl(CLK_CFG_UPDATE);
    mcSHOW_DBG_MSG2(("addr:%x, value:%x\n", CLK_CFG_UPDATE, u4value));
    u4value = Reg_Readl(CLK_CFG_UPDATE);
    mcSHOW_DBG_MSG2(("addr:%x, value:%x\n", CLK_CFG_UPDATE, u4value));
    u4value = Reg_Readl(0x1000CF00);
    mcSHOW_DBG_MSG2(("addr:%x, value:%x\n", 0x1000CF00, u4value));
    u4value = Reg_Readl(0x10007040);
    mcSHOW_DBG_MSG2(("addr:%x, value:%x\n", 0x10007040, u4value));
    u4value = Reg_Readl(0x1000602c);
    mcSHOW_DBG_MSG2(("addr:%x, value:%x\n", 0x1000602c, u4value));
    u4value = Reg_Readl(0x10007040);
    mcSHOW_DBG_MSG2(("addr:%x, value:%x\n", 0x10007040, u4value));
    mcSHOW_DBG_MSG2(("=============================================\n"));
}

#ifdef MEM_TEST

void pmic_Vcore1_adjust(int nAdjust)
{
}

void pmic_Vcore2_adjust(int nAdjust)
{
}

void pmic_voltage_read(void)
{
}

void static MemoryTest(DRAMC_CTX_T *p)
{
	int temp;
	int mem_start,len;
	unsigned int err_count = 0;
	unsigned int pass_count = 0;
	unsigned int count;
	unsigned int count2 = 1000;
	unsigned int delay = 1000;
	int reg_val;
	int rank0_col,rank1_col;
	U8 ucstatus = 0;
	U32 u4value;
	unsigned int uiStartAddr = 0x50000000;
	U32 uiCmpErr;
	U32 uiSrcAddr, uiDestAddr, uiLen, uiFixedAddr = 0x4f000000, uiReadCompareOK=1;
	U8 Voltage;

	uiSrcAddr = 0x50000000;
	uiLen = 0xff000;

	*(volatile unsigned int*)(0x10007000) = 0x22000000;
#ifdef READ_COMPARE_TEST
	for (count=uiFixedAddr; count<uiFixedAddr+0x10000; count+=4)
	{
		*(volatile unsigned int   *)(count) = count;
	}
#endif
	do {

#ifdef MEMPLL_RESET_TEST
    dbg_print("MEMPLL Reset\n");

    *(volatile unsigned int*)(0x10006000) = 0x0b160001; 
    *(volatile unsigned int*)(0x10006010) |= 0x08000000;  //(4) 0x10006010[27]=1  //Request MEMPLL reset/pdn mode 
    mcDELAY_US(1);
    *(volatile unsigned int*)(0x10006010) &= ~(0x08000000);  //(1) 0x10006010[27]=0 //Unrequest MEMPLL reset/pdn mode and wait settle (1us for reset)
    mcDELAY_US(1);                
#endif        

		if (uiDualRank) 
		{
			#ifdef SINGLE_RANK_DMA
			uiDestAddr = uiSrcAddr+0x10000000;
			#else
			#ifdef SINGLE_CHANNEL_ENABLE
			uiDestAddr = uiSrcAddr+0x30000000;
			#else
			uiDestAddr = uiSrcAddr+0x60000000;
			#endif
			#endif
			
		}
		else
		{
			uiDestAddr = uiSrcAddr+0x10000000;
		}
		

		CHA_HWGW_Print(p);
#ifdef DUAL_FREQ_TEST			
        if (p->frequency != DUAL_FREQ_LOW)   
		{
			// Switch to low frequency.
			p->frequency = DUAL_FREQ_LOW;
		}
		else
		{
			// Switch into high frequency.
	        p->frequency = mt_get_freq_setting(p);

			// Adjust to HV first.
			// HV 1.125V
			for (Voltage=0x30; Voltage<0x44; Voltage++)
			{
				pmic_Vcore1_adjust(4);
				pmic_Vcore2_adjust(4);
				if (uiDualRank) 
				{
					DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, uiSrcAddr, uiDestAddr, 0x100, 8, 1, 1);
					DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, uiDestAddr, uiSrcAddr, 0x100, 8, 1, 1);
				}
				else
				{
					DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, uiSrcAddr, uiSrcAddr+0x10000000, 0x100, 8, 1, 1);
				}
			}
			//pmic_Vcore1_adjust(6);
			//pmic_Vcore2_adjust(6);	
			pmic_voltage_read();
		}
		RXPERBIT_LOG_PRINT = 0;

		//DramcSwitchFreq(p, 0);
		DramcDFS(p, 0);
		
		RXPERBIT_LOG_PRINT = 1;

		CHA_HWGW_Print(p);
	
        if (p->frequency != DUAL_FREQ_LOW)   
		{
		}
		else
		{
			// Now in low freq. switch into low voltage.
			// LV 1.00V
			for (Voltage=0x44; Voltage>0x30; Voltage--)
			{
				pmic_Vcore1_adjust(5);
				pmic_Vcore2_adjust(5);
				if (uiDualRank) 
				{
					DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, uiSrcAddr, uiDestAddr, 0x100, 8, 1, 1);
					DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, uiDestAddr, uiSrcAddr, 0x100, 8, 1, 1);
				}
				else
				{
					DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, uiSrcAddr, uiDestAddr, 0x100, 8, 1, 1);
				}
			}
			pmic_voltage_read();
		}
		
	
#endif
		
#ifdef XTALK_SSO_STRESS
		{
			
			#ifdef SINGLE_CHANNEL_ENABLE
			uiCmpErr = DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, uiSrcAddr, uiDestAddr, uiLen, 8, 1, 1);
			#else
			uiCmpErr = DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, uiSrcAddr, uiDestAddr, uiLen, 8, 1, 2);
			#endif
			if (uiCmpErr)
			{
				err_count++;
				mcSHOW_DBG_MSG(("Fail %xh : DMA source =%xh Dest = %xh Len=%xh\n", uiCmpErr, uiSrcAddr, uiDestAddr, uiLen));
			}
			else
			{
				pass_count++;
				mcSHOW_DBG_MSG(("Pass %xh : DMA source =%xh Dest = %xh Len=%xh\n", uiCmpErr, uiSrcAddr, uiDestAddr, uiLen));
			}
			mcSHOW_DBG_MSG(("pass_count = %d, err_count = %d \n", pass_count, err_count));

			#ifdef SUSPEND_TEST
			Suspend_Resume(p);
			#endif

			#ifdef SINGLE_CHANNEL_ENABLE
			uiCmpErr = DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, uiDestAddr, uiSrcAddr, uiLen, 8, 1, 1);
			#else
			uiCmpErr = DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, uiDestAddr, uiSrcAddr, uiLen, 8, 1, 2);
			#endif
			if (uiCmpErr)
			{
				err_count++;
				mcSHOW_DBG_MSG(("Fail %xh : DMA source =%xh Dest = %xh Len=%xh\n", uiCmpErr, uiDestAddr, uiSrcAddr, uiLen));
			}
			else
			{
				pass_count++;
				mcSHOW_DBG_MSG(("Pass %xh : DMA source =%xh Dest = %xh Len=%xh\n", uiCmpErr, uiDestAddr, uiSrcAddr, uiLen));
			}
			mcSHOW_DBG_MSG(("pass_count = %d, err_count = %d \n", pass_count, err_count));
		    
			uiSrcAddr+=0xff000;
			if ((uiSrcAddr+0x200000) >= 0x60000000)
			{
				uiSrcAddr = 0x50000000;
			}
		}
#endif		

#ifdef SUSPEND_TEST
		Suspend_Resume(p);
#endif

#ifdef READ_COMPARE_TEST
		uiReadCompareOK = 1;
		for (count=uiFixedAddr; count<uiFixedAddr+0x10000; count+=4)
		{
			if (*(volatile unsigned int   *)(count) != count)
			{
				mcSHOW_DBG_MSG(("[Fail] Addr %xh = %xh\n",count, *(volatile unsigned int   *)(count)));
				uiReadCompareOK = 0;
			}
		}		
		if (uiReadCompareOK)
		{
			pass_count++;
			mcSHOW_DBG_MSG(("Read compare pass..\n"));
		}
		else
		{
			err_count++;
			mcSHOW_DBG_MSG(("Read compare fail..\n"));
		}
#endif

#ifdef SUSPEND_TEST
		Suspend_Resume(p);
#endif
		
#ifdef TA2_STRESS
		p->channel = CHANNEL_A;
		//temp = DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, 0xaaffffff, 2, 0, 0, 0);
		temp = DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, 0xaa7fffff, 2, 0, 0, 0);
		if (temp < 0){
		    err_count++;
		    dbg_print("channel A TA2 failed,pass_cnt:%d,err_cnt:%d\n",pass_count,err_count);
		}
		else
		{
		    pass_count++;
		    dbg_print("channel A TA2 pass,pass_cnt:%d,err_cnt:%d\n",pass_count,err_count);
		}       	
#endif

#ifdef SUSPEND_TEST
		Suspend_Resume(p);
#endif

#ifdef CPUTEST
		temp = Memory_Read_Write_Test(uiStartAddr,0x100000);
		if (temp < 0){
		    err_count++;
		    dbg_print("mem test failed,pass_cnt:%d,err_cnt:%d\n",pass_count,err_count);
		}
		else{
		    pass_count++;
		    dbg_print("mem test pass,pass_cnt:%d,err_cnt:%d\n",pass_count,err_count);
		}
#endif
		uiStartAddr+=0x100000;
		if (uiStartAddr>=0x80000000)
		{
			uiStartAddr = 0x50000000;
		}
	}while(1);
}

#endif

#ifdef DQM_CALIBRATION
extern S32 DQMSkew[DQS_NUMBER];
#endif

unsigned int is_one_pll_mode(void)
{  
   int data;
   
   //data = Reg_Readl(0x10004088);
   data = Reg_Readl(0x10006198);
   if((data & 0x01) == 0x0)
   {
      print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
      print("It's 1-PLL mode (value = 0x%x)\n", data);
      print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
      
      return 1;
   }
   else
   {
      print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
      print("It's 3-PLL mode (value = 0x%x)\n", data);
      print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

      return 0;
   }
}

#ifdef DDR_RESERVE_MODE
extern u32 g_ddr_reserve_enable;
extern u32 g_ddr_reserve_success;
#define TIMEOUT 3
#endif

void Print_Shuffle_Reg(void)
{
    U32 u4value, u4value1;
    u4value = Reg_Readl(0x10004028);
    u4value1 = Reg_Readl(0x1000416c);
    mcSHOW_DBG_MSG2(("### 28[16]:%x 16c[1]:%x\n", (u4value & 0x10000) >> 16, (u4value1 & 0x2) >> 1));
    return;
}

void Block_EMI(void)
{
    U8 ucstatus = 0;
    U32 u4value = 0;

    //Reg0x138[5]=1, // Block DRAMC ALE 
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x138), &u4value);
    mcSET_BIT(u4value, 5); 	
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x138), u4value);    

    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x854), &u4value);
    mcSET_BIT(u4value, 5); 	
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x854), u4value);    

    return;
}

void Unblock_EMI(void)
{
    U8 ucstatus = 0;
    U32 u4value = 0;

    //Reg0x138[5]=0, // Un block DRAMC ALE 
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x138), &u4value);
    mcCLR_BIT(u4value, 5); 	
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x138), u4value);    

    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x854), &u4value);
    mcCLR_BIT(u4value, 5); 	
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x854), u4value);    

    return;
}

#define	CHAN_DRAMC_NAO_MISC_STATUSA(base)	(base + 0x3B8)
#define SREF_STATE				(1 << 16)

static unsigned int is_dramc_exit_slf(void)
{
	unsigned int ret;

	ret = *(volatile unsigned *)CHAN_DRAMC_NAO_MISC_STATUSA(DRAMC_NAO_BASE);
	if ((ret & SREF_STATE) != 0) {
		print("DRAM CHAN-A is in self-refresh (MISC_STATUSA = 0x%x)\n", ret);
		return 0;
	}

	print("ALL DRAM CHAN is not in self-refresh\n");
	return 1;
}

void release_dram(void)
{
#ifdef DDR_RESERVE_MODE  
    int i;
    int counter = TIMEOUT;
    int one_pll_mode = 0;
      
    if(g_ddr_reserve_enable==0)
        return;
    Before_Exit_DDR_Reserved_Mode_setting();//Transfer to SW control
    rgu_release_rg_dramc_conf_iso();//release 0x400{PHY ISO}
    Exit_DDR_Reserved_Mode_setting();//Refer to FORCE_DRAM_HIGH()
    rgu_release_rg_dramc_iso();//release 0x200{DRAMC ISO} 
    rgu_release_rg_dramc_sref();//release 0x100
    one_pll_mode = is_one_pll_mode();

    // setup for EMI
    DRV_WriteReg32(EMI_MPUP, 0x200);
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

    Block_EMI();
    Print_Shuffle_Reg();
    do_DFS(0);//Move to Exit_DDR_Reserved_Mode_setting     
    Print_Shuffle_Reg();
    do_DFS(1);//Move to Exit_DDR_Reserved_Mode_setting
    Print_Shuffle_Reg();
    Unblock_EMI();
        
    After_Exit_DDR_Reserved_Mode_setting();//Transfer to SPM control
#endif    
}

void ddr_reserve_sample_flow(void)
{
    int read_data;
    int i;
    *(volatile unsigned *)(0x10007040) = (0x59000000 | *(volatile unsigned *)(0x10007040)&~0x400);

    if(*(volatile unsigned *)(0x10007040) &0x10000==1)
    {
        print("DDR RESERVE Before RST Fail\n");
    }
#if 0
    while ((*(volatile unsigned *)(0x10007040) & 0x200000)==0x0){
        print("dram is not in self refresh\n");
    }
#endif
    
    read_data= *(unsigned int *)(0x10004000+(0x77<<2));
    *(unsigned int *)(0x10004000+(0x77<<2))= read_data & ~(0xc0000000);

    for (i=0;i<10;i++);

    /* read fifo reset */
    read_data= *(unsigned int *)(0x100040f4);
    *(unsigned int *)(0x100040f4) = read_data | (0x02000000);
    read_data= *(unsigned int *)(0x100040f4);
    *(unsigned int *)(0x100040f4) = read_data & (0xfdffffff);

    /* phase sync reset */
    read_data= *(unsigned int *)(0x1000f5cc);
    *(unsigned int *)(0x1000f5cc) = read_data | (0x00001100);
    *(unsigned int *)(0x1000f640) = 0x00020091;
    *(unsigned int *)(0x1000f640) = 0x000200b1;

    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_PHYCTL1), 0x10000000);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_PHYCTL1), 0x0);

    read_data= *(unsigned int *)(0x1000f640);
    *(unsigned int *)(0x1000f640) = read_data & 0xfffffffe;
    *(unsigned int *)(0x1000f640) = read_data | 0x1;

    for (i=0;i<10;i++);

    read_data= *(unsigned int *)(0x10004000+(0x77<<2));
    *(unsigned int *)(0x10004000+(0x77<<2))= read_data | (0xc0000000);

    *(volatile unsigned *)(0x10007040) = (0x59000000 | *(volatile unsigned *)(0x10007040)&~0x200);
    for (i=0;i<10;i++);
    //exit self refresh control
    *(volatile unsigned *)(0x10007040) = (0x59000000 | *(volatile unsigned *)(0x10007040)&~0x100);

    for (i=0;i<10;i++);    
#if 0
    while (*(volatile unsigned *)(0x10007040) & 0x200000){
        print("dram in self refresh\n");
    }
#endif
   
    for (i=0;i<10;i++);
    
    DRV_WriteReg32(EMI_MPUP, 0x200);
    for (i=0;i<10;i++);
       print("[DDR Reserve] ddr_reserve_sample_flow\n");
    for (i = 0x410; i <= 0x48C; i +=4)
    {
        print("[DDR Reserve] 0x%x:0x%x\n",(0x10200000+i),*(volatile unsigned int *)(0x10200000+i));
    }

#if 0
    for (i = 0x0; i <= 0x1000; i +=4)
    {
        print("[DDR Reserve:DRAMC] 0x%x:0x%x\n",(0x10004000+i),*(volatile unsigned int *)(0x10004000+i));
    }
    for (i = 0x0; i <= 0x1000; i +=4)
    {
        print("[DDR Reserve:DRAMC_NAO] 0x%x:0x%x\n",(0x1020E000+i),*(volatile unsigned int *)(0x1020E000+i));
    }
    for (i = 0x0; i <= 0x1000; i +=4)
    {
        print("[DDR Reserve:DDRPHY] 0x%x:0x%x\n",(0x1000F000+i),*(volatile unsigned int *)(0x1000F000+i));
    }
#endif

    //while(1);
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
      if(rgu_is_reserve_ddr_mode_success())
      {
        while(counter)
        {
          if(rgu_is_dram_slf())
          {
            g_ddr_reserve_success = 1;
            //release_dram();
            break;
          }
          counter--;
        }
        if(counter == 0)
        {
          print("[DDR Reserve] ddr reserve mode success but DRAM not in self-refresh!\n");
          g_ddr_reserve_success = 0;
        }
      }
    else
      {
        print("[DDR Reserve] ddr reserve mode FAIL!\n");
        g_ddr_reserve_success = 0;
      }
    }
    else
    {
      print("[DDR Reserve] ddr reserve mode not be enabled yet\n");
      g_ddr_reserve_enable = 0;
    }
    
    /* release dram, no matter success or failed */
    release_dram();
#endif    
}


EMI_SETTINGS emi_setting_default_lpddr3 =
{
#if 1
		0x0,			/* sub_version */
		0x0003, 		/* TYPE */
		0,				/* EMMC ID/FW ID checking length */
		0,				/* FW length */
		{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},				/* NAND_EMMC_ID */
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},		/* FW_ID */
		0x0002A052, 			/* EMI_CONA_VAL */
	0x00000000, 	/* EMI_CONH_VAL */
	0x77ED484B, 	/* DRAMC_ACTIM_VAL */
		0x11000000, 			/* DRAMC_GDDR3CTL1_VAL */
	0x000484E3, 	/* DRAMC_CONF1_VAL */
	0x000063A1, 	/* DRAMC_DDR2CTL_VAL */
	0xbfc30480, 	/* DRAMC_TEST2_3_VAL */
	0x11C0B06C, 	/* DRAMC_CONF2_VAL */
	0xD3656042, 	/* DRAMC_PD_CTRL_VAL */
	0x95001D51, 	/* DRAMC_ACTIM1_VAL*/
	0x91190000, 	/* DRAMC_MISCTL0_VAL*/
	0x000034FF, 	/* DRAMC_ACTIM05T_VAL*/
	0x04264681, 	/* DRAMC_RKCFG_VAL*/
	0x2700110D, 	/* DRAMC_TEST2_4_VAL*/
	{0x40000000,0x40000000,0,0},		/* DRAM RANK SIZE */
	{0,0,0,0,0,0,0,0,0,0},		/* reserved 10 */
		0x00830001, 			/* LPDDR3_MODE_REG1 */
		0x001C0002, 			/* LPDDR3_MODE_REG2 */
		0x00020003, 			/* LPDDR3_MODE_REG3 */
		0x00000006, 			/* LPDDR3_MODE_REG5 */
		0x00FF000A, 			/* LPDDR3_MODE_REG10 */
		0x0000003F, 			/* LPDDR3_MODE_REG63 */
#endif
};


static int mt_get_dram_type_for_dis(void)
{
    int i;
    int type = 2;
    type = (emi_settings[0].type & 0xF);
    for (i = 0 ; i < num_of_emi_records; i++)
    {
      //print("[EMI][%d] type%d\n",i,type);
      if (type != (emi_settings[0].type & 0xF))
      {
          print("It's not allow to combine two type dram when combo discrete dram enable\n");
          ASSERT(0);
          break;
      }
    }
    return type;
}

extern const U32 uiLPDDR3_PHY_Mapping_POP_CHA[32];

unsigned int DRAM_MRR(int rank,int MRR_num)
{
	unsigned int MRR_value = 0x0;
	unsigned int dram_type, ucstatus = 0 , u4value;
	DRAMC_CTX_T *p = psCurrDramCtx;
	unsigned int backup_reg_0x1e8;


	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_ACTIM1), &u4value);
	backup_reg_0x1e8 = u4value;
	mcSET_BIT(u4value, 26); //0:Enable 1:Disable
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_ACTIM1), u4value); 

	// CKEFIXON before MRW
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xe4), &u4value);
	mcSET_BIT(u4value, 2);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xe4), u4value);

	mcDELAY_US(200); // tINIT3 > 200us

	if ((p->dram_type == TYPE_LPDDR3))
	{
		// set DQ bit 0, 1, 2 pinmux
		if (p->channel == CHANNEL_A)
		{
			if (p->dram_type == TYPE_LPDDR3)
			{
				// refer to CA training pinmux array
				ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_RRRATE_CTL), &u4value);
				mcSET_FIELD(u4value, uiLPDDR3_PHY_Mapping_POP_CHA[0], MASK_RRRATE_CTL_BIT0_SEL, POS_RRRATE_CTL_BIT0_SEL);
				mcSET_FIELD(u4value, uiLPDDR3_PHY_Mapping_POP_CHA[1], MASK_RRRATE_CTL_BIT1_SEL, POS_RRRATE_CTL_BIT1_SEL);
				mcSET_FIELD(u4value, uiLPDDR3_PHY_Mapping_POP_CHA[2], MASK_RRRATE_CTL_BIT2_SEL, POS_RRRATE_CTL_BIT2_SEL);
				mcSET_FIELD(u4value, uiLPDDR3_PHY_Mapping_POP_CHA[3], MASK_RRRATE_CTL_BIT3_SEL, POS_RRRATE_CTL_BIT3_SEL);
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_RRRATE_CTL), u4value);

				ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MRR_CTL), &u4value);
				mcSET_FIELD(u4value, uiLPDDR3_PHY_Mapping_POP_CHA[4], MASK_MRR_CTL_BIT4_SEL, POS_MRR_CTL_BIT4_SEL);
				mcSET_FIELD(u4value, uiLPDDR3_PHY_Mapping_POP_CHA[5], MASK_MRR_CTL_BIT5_SEL, POS_MRR_CTL_BIT5_SEL);
				mcSET_FIELD(u4value, uiLPDDR3_PHY_Mapping_POP_CHA[6], MASK_MRR_CTL_BIT6_SEL, POS_MRR_CTL_BIT6_SEL);
				mcSET_FIELD(u4value, uiLPDDR3_PHY_Mapping_POP_CHA[7], MASK_MRR_CTL_BIT7_SEL, POS_MRR_CTL_BIT7_SEL);
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MRR_CTL), u4value);
			}
		}

		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), MRR_num |(rank<<26));
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000002);
		mcDELAY_US(10);
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);
		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x3B8), &u4value);
		MRR_value = (u4value >> 20) & 0xFF;
	}
	// release CKEFIXON
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xe4), &u4value);
	mcCLR_BIT(u4value, 2);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xe4), u4value);

	//restore backup value
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_ACTIM1), backup_reg_0x1e8); 

	return MRR_value;
}


static int mt_get_dram_density(int rank)
{
    int value, density, io_width;
    long long size;
    
    value = DRAM_MRR(rank,8);
    print("[EMI]MRR(8) = 0x%x,rank is %d\n", value,rank);
    
    io_width = ((value & 0xC0) >> 6)? 2 : 4; //0:32bit(4byte), 1:16bit(2byte)
    //print("[EMI]DRAM IO width = %d bit\n", io_width*8);
        
    density = (value & 0x3C) >> 2;
    switch(density)
    {
        case 0x6:
            size = 0x20000000;  //4Gb
            //print("[EMI]DRAM density = 4Gb\n");
            break;
        case 0xE:
            size = 0x30000000;  //6Gb
            //print("[EMI]DRAM density = 6Gb\n");
            break;
        case 0x7:
            size = 0x40000000;  //8Gb
            //print("[EMI]DRAM density = 8Gb\n");
            break;
        case 0xD:
            size = 0x60000000;  //12Gb
            //print("[EMI]DRAM density = 12Gb\n");
            break;
        case 0x8:
            size = 0x80000000;  //16Gb
            //print("[EMI]DRAM density = 16Gb\n");
            break;
        //case 0x9:
            //size = 0x100000000L; //32Gb
            //print("[EMI]DRAM density = 32Gb\n");
            //break;
        default:
            size = 0; //reserved
     }  
     
     if(io_width == 2)
        size = size << 1;
        
     return size; 
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

    unsigned int mode_reg_5 = 0, dram_density, dram_channel_nr, dram_rank_nr;
    unsigned int dram_type;
    
    if (!(found))
    {
        int result=0;
        platform_get_mcp_id (id, emmc_nand_id_len,&fw_id_len);      
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
                if (dram_type != (emi_settings[i].type & 0x000F))
                {
                    printf("[EMI] Combo discrete dram only support when combo lists are all same dram type.");
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
        printf("[EMI] mcp_dram_num:%d,discrete_dram_num:%d,enable_combo_dis:%d\r\n",mcp_dram_num,discrete_dram_num,enable_combo_dis);
        /*
         *
         * 0. if there is only one discrete dram, use index=0 emi setting and boot it.
         * */
        if ((0 == mcp_dram_num) && (1 == discrete_dram_num))
        {
            mdl_number = 0;
            found = 1;
            return mdl_number;
        }
            

        /* 1.
         * if there is MCP dram in the list, we try to find emi setting by emmc ID
         * */
        if (mcp_dram_num > 0)
        {
        result = platform_get_mcp_id (id, emmc_nand_id_len,&fw_id_len);
    
        print("[EMI] eMMC/NAND ID = %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x\r\n", id[0], id[1], id[2], id[3], id[4], id[5], id[6], id[7], id[8],id[9],id[10],id[11],id[12],id[13],id[14],id[15]);
    
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
#if 1
                                printf("fw id len:%d\n",emi_settings[i].fw_id_length);
                                if (emi_settings[i].fw_id_length > 0)
                                {
                                    char fw_id[6];
                                    memset(fw_id, 0, sizeof(fw_id));
                                    memcpy(fw_id,id+emmc_nand_id_len,fw_id_len);
                                    for (j = 0; j < fw_id_len;j ++){
                                        printf("0x%x, 0x%x ",fw_id[j],emi_settings[i].fw_id[j]); 
                                    }
                                    if(memcmp(fw_id,emi_settings[i].fw_id,fw_id_len) == 0)
                                    {
                                        mdl_number = i;
                                        found = 1;
                                        break; /* found */
                                    }
                                    else
                                    {
                                        printf("[EMI] fw id match failed\n");
                                    }
                                }
                                else
                                {
                                    mdl_number = i;
                                    found = 1;
                                    break; /* found */
                                }
#else
                                    mdl_number = i;
                                    found = 1;
                                    break; /* found */
#endif
                            }
                            else{
                                  printf("[EMI] index(%d) emmc id match failed\n",i);
                            }
                            
                        }
                    }
                }
            }
        }
        }
#if 1
        /* 2. find emi setting by MODE register 5
         * */
        // if we have found the index from by eMMC ID checking, we can boot android by the setting
        // if not, we try by vendor ID
        if ((0 == found) && (1 == enable_combo_dis))
        {
            //print_DBG_info();
            //print("-->%x,%x,%x\n",emi_set->DRAMC_ACTIM_VAL,emi_set->sub_version,emi_set->fw_id_length); 
            //print("-->%x,%x,%x\n",emi_setting_default.DRAMC_ACTIM_VAL,emi_setting_default.sub_version,emi_setting_default.fw_id_length); 
            dram_type = mt_get_dram_type_for_dis();
            if (TYPE_LPDDR3 == dram_type)
            {
                
                default_emi_setting = &emi_setting_default_lpddr3;
                psCurrDramCtx = &DramCtx_LPDDR3;  
                print("[EMI] LPDDR3 discrete dram init\r\n");
            }


                psCurrDramCtx->frequency = mt_get_freq_setting(psCurrDramCtx); 


            Init_DRAM();
	    Switch_To_SWcontrol(); 

            unsigned int manu_id = DRAM_MRR(0,0x5);
            print("[EMI]MR5:%x\n",manu_id);
            //try to find discrete dram by DDR2_MODE_REG5(vendor ID)
            for (i = 0; i < num_of_emi_records; i++)
            {
                if (TYPE_LPDDR3 == dram_type)
                    mode_reg_5 = emi_settings[i].iLPDDR3_MODE_REG_5; 
                print("emi_settings[%d].MODE_REG_5:%x,emi_settings[%d].type:%x\n",i,mode_reg_5,i,emi_settings[i].type);
                //only check discrete dram type
                if ((emi_settings[i].type & 0x0F00) == 0x0000) 
                {
                    //support for compol discrete dram 
                    if ((mode_reg_5 == manu_id) )
                    {
                        dram_density = mt_get_dram_density(0);                       
                        print("emi_settings[%d].DRAM_RANK_SIZE[0]:0xll%x, dram_density:0xll%x\n",i,emi_settings[i].DRAM_RANK_SIZE[0], dram_density);                            
                        if(emi_settings[i].DRAM_RANK_SIZE[0] == dram_density)
                        {  
			        
                            dram_density = mt_get_dram_density(1);                       
                            print("emi_settings[%d].DRAM_RANK_SIZE[1]:0x%x, dram_density:0x%x\n",i,emi_settings[i].DRAM_RANK_SIZE[1], dram_density);                            
                            if(emi_settings[i].DRAM_RANK_SIZE[1] == dram_density)
                            {  
                            mdl_number = i;
                            found = 1;
                            break;
                        } 
                    }
                }
            }
        }
        }
#endif
        printf("found:%d,i:%d\n",found,i);    
    }
/*	
    if(is_TB_setting()!=0)
    	{
    	mdl_number=2;// 1866 settting must be CS_PART_NUMBER[2] !!!
    	printf("found Common 2GB 1866\n");  
    	}
*/
    return mdl_number;
}



int get_dram_rank_nr (void)
{

    int index;
    int emi_cona;
    if (g_ddr_reserve_enable==1 && g_ddr_reserve_success==1) {
          emi_cona = *(volatile unsigned int*)(EMI_CONA);
    } else {	
    	index = mt_get_mdl_number ();
    	if (index < 0 || index >=  num_of_emi_records)
    	{
        	return -1;
    	}
    	emi_cona = emi_settings[index].EMI_CONA_VAL;
    }
    return (emi_cona & 0x20000) ? 2 : 1;

}


int is_combo_dis_enable(void)
{
    int i;
    int discrete_dram_num = 0;
    int mcp_dram_num = 0;
    unsigned int dram_type;
    int combo_dis = 0;   
      
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
     * then combo discrete enable is true
     * */
     
    if ((discrete_dram_num > 0) && (num_of_emi_records >= 2))
    {
        /* if we enable combo discrete dram
         * check all dram are all same type and not DDR3
         * */
        combo_dis = 1;
        dram_type = emi_settings[0].type & 0x000F;
        for (i = 0 ; i < num_of_emi_records; i++)
        {
            if (dram_type != (emi_settings[i].type & 0x000F))
            {
                printf("[EMI] Combo discrete dram only support when combo lists are all same dram type.");
                ASSERT(0);
            }
            if ((emi_settings[i].type & 0x000F) == TYPE_PCDDR3) 
            {
                // has PCDDR3, disable combo discrete drame, no need to check others setting 
                combo_dis = 0; 
                break;
            }
            dram_type = emi_settings[i].type & 0x000F;
        }       
    } 
    return combo_dis;
}


int mt_get_dram_type (void)
{
return TYPE_LPDDR3;
#if 0
    int n;

   /* if combo discrete is enabled, the dram_type is LPDDR2 or LPDDR4, depend on the emi_setting list*/
    if ( is_combo_dis_enable() == 1)
    return mt_get_dram_type_for_dis();

    n = mt_get_mdl_number ();

    if (n < 0  || n >= num_of_emi_records)
    {
        return 0; /* invalid */
    }

    return (emi_settings[n].type & 0xF);
#endif
}
/* 
 * setup block correctly, we should hander both 4GB mode and 
 * non-4GB mode.
 */
void get_orig_dram_rank_info(dram_info_t *orig_dram_info)
{
	int i, j;
	int dram_rank_nr = 0;
	u64 base = DRAM_BASE;
	u64 rank_size[4];

	dram_rank_nr = get_dram_rank_nr();
	if(dram_rank_nr < 0)
	{
		printf("[ERR] get_dram_rank_nr() < 0 !!!");
	}
	orig_dram_info->rank_num = (dram_rank_nr > 0)? dram_rank_nr : 0;
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

void get_dram_rank_size_by_EMI_CONA (u64 dram_rank_size[])
{
    unsigned int col_bit, row_bit ;
    u64 ch0_rank0_size, ch0_rank1_size, ch1_rank0_size, ch1_rank1_size;
    unsigned int shift_for_16bit = 1;

    unsigned emi_cona = *(volatile unsigned int*)(EMI_CONA);
    unsigned emi_conh = *(volatile unsigned int*)(EMI_CONH);

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
            dram_rank_size[0] = ((u64)(1 << (row_bit + col_bit))) * ((u64)(4 >> shift_for_16bit) * 8); // 4 or 2 byte * 8 banks
        }
        else
        {
            dram_rank_size[0] = (ch0_rank0_size * 256 << 20);
        }

        if (0 != (emi_cona &  (1 << 17)))   //rank 1 exist
        {
            if(ch0_rank1_size == 0)
            {
                col_bit = ((emi_cona >> 6) & 0x03) + 9;
                row_bit = (((emi_cona >> 25) & 0x1) << 2) + ((emi_cona >> 14) & 0x03) + 13;
                dram_rank_size[1] = ((u64)(1 << (row_bit + col_bit)) )* ((u64)(4 >> shift_for_16bit) * 8); // 4 or 2 byte * 8 banks
            }
            else
            {
                dram_rank_size[1] = (ch0_rank1_size * 256 << 20);
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
            dram_rank_size[0] += (ch1_rank0_size * 256 << 20);
        }

        if (0 != (emi_cona &  (1 << 16)))   //rank 1 exist
        {
            if(ch1_rank1_size == 0)
            {
                col_bit = ((emi_cona >> 22) & 0x03) + 9;
                row_bit = (((emi_conh >> 5) & 0x1) << 2) + ((emi_cona >> 30) & 0x03) + 13;
                dram_rank_size[1] += ((u64)(1 << (row_bit + col_bit)) * (u64)(4 >> shift_for_16bit) * 8); // 4 or 2 byte * 8 banks
            }
            else
            {
                dram_rank_size[1] += (ch1_rank1_size * 256 << 20);
            }
        }
    }

    printf("DRAM rank0 size:0x%llx,\nDRAM rank1 size=0x%llx\n", dram_rank_size[0], dram_rank_size[1]);
}
void get_dram_rank_size (u64 dram_rank_size[])
{
    int index, rank_nr, i;

  #ifdef DDR_RESERVE_MODE
    if(g_ddr_reserve_enable==1 && g_ddr_reserve_success==1)
    {
        get_dram_rank_size_by_EMI_CONA(dram_rank_size);
    } 
    else
  #endif  
    { 
    index = mt_get_mdl_number ();

    if (index < 0 || index >=  num_of_emi_records)
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
}

DRAMC_CTX_T* mt_get_dramc_context(void)
{
    int index = 0;
    EMI_SETTINGS *emi_set;
if(g_ddr_reserve_enable==1 && g_ddr_reserve_success==1)
{

    return &DramCtx_LPDDR3;

}else
{
    index = emi_setting_index = mt_get_mdl_number ();
    print("[Check]mt_get_mdl_number 0x%x\n", index);
    //print("[EMI] eMMC/NAND ID = %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x\r\n", id[0], id[1], id[2], id[3], id[4], id[5], id[6], id[7], id[8],id[9],id[10],id[11],id[12],id[13],id[14],id[15]);
    if (index < 0 || index >=  num_of_emi_records)
    {
        print("[EMI] setting failed 0x%x\r\n", index);
        ASSERT(0);
    }
  
    print("[EMI] MDL number = %d\r\n", index);
    emi_set = &emi_settings[index];

    print("[EMI] emi_set eMMC/NAND ID = %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x\r\n", emi_set->ID[0], emi_set->ID[1], emi_set->ID[2], emi_set->ID[3], emi_set->ID[4], emi_set->ID[5], emi_set->ID[6], emi_set->ID[7], emi_set->ID[8],emi_set->ID[9],emi_set->ID[10],emi_set->ID[11],emi_set->ID[12],emi_set->ID[13],emi_set->ID[14],emi_set->ID[15]);
    if ((emi_set->type & 0xF) == TYPE_LPDDR3)
    {
          return &DramCtx_LPDDR3;
    }
    else
    {
        print("The DRAM type is not supported");
        ASSERT(0);
        return -1;
    }    
}
}
#if defined(MACH_TYPE_MT6750)
extern U8  gOnly_09V_1PLL_Flow_Flag;
#endif
void do_DFS(high_freq)
{
    DRAMC_CTX_T *p;
     
    p = mt_get_dramc_context();

    if(high_freq == 1)
    {
      p->frequency = mt_get_freq_setting(p);

    }
    else
    {
      p->frequency = DUAL_FREQ_LOW;      
    }
#if defined(MACH_TYPE_MT6750)||defined(PMIC_CHIP_MT6353)	
    if (gOnly_09V_1PLL_Flow_Flag == 1)
    {
        if (high_freq==0){        
            DramcSwitchFreq(p);
    	}
    }
    else
   {
        DramcDFS(p, 2);	
    }
#else
       DramcDFS(p, 2); 
#endif

}

void do_memory_test(void)
{
    DRAMC_CTX_T *p;
     
    p = mt_get_dramc_context();
    MemoryTest(p);
}

#ifdef DRAM_HQA
void dram_HQA_adjust_voltage(void)
{
#ifdef HVcore1	/*Vcore1=1.10V, Vdram=1.30V,  Vio18=1.8*/
	pmic_config_interface(Vcore_REG_SW, Vcore1_HV, 0x7F, 0);
	pmic_config_interface(Vcore_REG_HW, Vcore1_HV, 0x7F, 0);
	pmic_config_interface(Vdram_REG, Vdram_HV, 0x7F, 0);
	print("[HQA]Set HVcore1 setting: Vcore1=1.10V(SW_Ctrl=0x%x, HW_Ctrl=0x%x, should be 0x%x), Vdram=1.30V(0x%x, should be 0x%x)\n",
		upmu_get_reg_value(Vcore_REG_SW), upmu_get_reg_value(Vcore_REG_HW),
		Vcore1_HV, upmu_get_reg_value(Vdram_REG), Vdram_HV);
#endif

#ifdef NV	/*Vcore1=1.00V, Vdram=1.22V,  Vio18=1.8*/
	pmic_config_interface(Vcore_REG_SW, Vcore1_NV, 0x7F, 0);
	pmic_config_interface(Vcore_REG_HW, Vcore1_NV, 0x7F, 0);
	pmic_config_interface(Vdram_REG, Vdram_NV, 0x7F, 0);
	print("[HQA]Set NV setting: Vcore1=1.00V(SW_Ctrl=0x%x, HW_Ctrl=0x%x, should be 0x%x), Vdram=1.22V(0x%x, should be 0x%x)\n",
		upmu_get_reg_value(Vcore_REG_SW), upmu_get_reg_value(Vcore_REG_HW),
		Vcore1_NV, upmu_get_reg_value(Vdram_REG), Vdram_NV);
#endif

#ifdef LVcore1	/*Vcore1=0.90V, Vdram=1.16V,  Vio18=1.8*/
	pmic_config_interface(Vcore_REG_SW, Vcore1_LV, 0x7F, 0);
	pmic_config_interface(Vcore_REG_HW, Vcore1_LV, 0x7F, 0);
	pmic_config_interface(Vdram_REG, Vdram_LV, 0x7F, 0);
	print("[HQA]Set LVcore1 setting: Vcore1=0.90V(SW_Ctrl=0x%x, HW_Ctrl=0x%x, should be 0x%x), Vdram=1.16V(0x%x, should be 0x%x)\n",
		upmu_get_reg_value(Vcore_REG_SW), upmu_get_reg_value(Vcore_REG_HW),
		Vcore1_LV, upmu_get_reg_value(Vdram_REG), Vdram_LV);
#endif

#ifdef HVcore1_LVdram	/*Vcore1=1.10V, Vdram=1.16V,  Vio18=1.8*/
	pmic_config_interface(Vcore_REG_SW, Vcore1_HV, 0x7F, 0);
	pmic_config_interface(Vcore_REG_HW, Vcore1_HV, 0x7F, 0);
	pmic_config_interface(Vdram_REG, Vdram_LV, 0x7F, 0);
	print("[HQA]Set HVcore1_LVdram setting: Vcore1=1.10V(SW_Ctrl=0x%x, HW_Ctrl=0x%x, should be 0x%x), Vdram=1.16V(0x%x, should be 0x%x)\n",
		upmu_get_reg_value(Vcore_REG_SW), upmu_get_reg_value(Vcore_REG_HW),
		Vcore1_HV, upmu_get_reg_value(Vdram_REG), Vdram_LV);
#endif

#ifdef LVcore1_HVdram	/*Vcore1=0.90V, Vdram=1.30V,  Vio18=1.8*/
	pmic_config_interface(Vcore_REG_SW, Vcore1_LV, 0x7F, 0);
	pmic_config_interface(Vcore_REG_HW, Vcore1_LV, 0x7F, 0);
	pmic_config_interface(Vdram_REG, Vdram_HV, 0x7F, 0);
	print("[HQA]Set LVcore1_HVdram setting: Vcore1=0.90V(SW_Ctrl=0x%x, HW_Ctrl=0x%x, should be 0x%x), Vdram=1.30V(0x%x, should be 0x%x)\n",
		upmu_get_reg_value(Vcore_REG_SW), upmu_get_reg_value(Vcore_REG_HW),
		Vcore1_LV, upmu_get_reg_value(Vdram_REG), Vdram_HV);
#endif
}

#else

void dram_HQA_adjust_voltage(void)
{
	pmic_config_interface(Vdram_REG, Vdram_NV_default, 0x7F, 0);

#if 0
	print("GPU/DRAM EFUSE 01 = 0x%x\n", seclib_get_devinfo_with_index(3));
	GPU800MHz_DRAM1866MHz_Flag = seclib_get_devinfo_with_index(3) & GPU800MHz_DRAM1866MHz_MASK;
	if (GPU800MHz_DRAM1866MHz_Flag) {
		pmic_config_interface(0x662, Vcore_NV_Turbo, 0x7F,0); 
		pmic_config_interface(0x664, Vcore_NV_Turbo, 0x7F,0); 
		print("[HQA][Turbo]Set NV setting: Vcore1=1.0V(0x%x, should be 0x%x)\n", 
			  upmu_get_reg_value(0x662),Vcore_NV_Turbo);	
	} else
		print("[HQA][Turbo]GPU/DRAM is not turbo mode\n");
#endif	
}
/* Turbo Dram/GPU E */
#endif

void mt_set_emi(void)
{  
#if defined (PMIC_CHIP_MT6353)
    //VCORE1 force PWM mode
    pmic_config_interface(0x44A,0x1,0x1,11);
#else

     unsigned int pmic_val_0x452;
     unsigned int pmic_val_0x450;
     //limit the PMIC bandwidth and let the PMIC response time slower
     //it could prevent oscillation of  Vcore
     pmic_read_interface(0x0452, &pmic_val_0x452 ,0xffff,0);
     pmic_read_interface(0x0450, &pmic_val_0x450,0xffff ,0);
     pmic_config_interface(0x0452 , 0x5602 ,0xffff ,0);
     pmic_config_interface(0x0450 , 0x7800 ,0xffff ,0);

    //VCORE1 force PWM mode
     pmic_config_interface(0x44E,0x1,0x1,1);
#endif
    psCurrDramCtx = mt_get_dramc_context();


    psCurrDramCtx->frequency = mt_get_freq_setting(psCurrDramCtx); 
       
    Init_DRAM();

#if 0
    int i=0;
    for (i = 0x0; i <= 0x1000; i +=4)
    {
        print("[DDR Reserve:DRAMC] 0x%x:0x%x\n",(0x10004000+i),*(volatile unsigned int *)(0x10004000+i));
    }
    for (i = 0x0; i <= 0x1000; i +=4)
    {
        print("[DDR Reserve:DRAMC_NAO] 0x%x:0x%x\n",(0x1020E000+i),*(volatile unsigned int *)(0x1020E000+i));
    }
    for (i = 0x0; i <= 0x1000; i +=4)
    {
        print("[DDR Reserve:DDRPHY] 0x%x:0x%x\n",(0x1000F000+i),*(volatile unsigned int *)(0x1000F000+i));
    }
#endif


#if defined (PMIC_CHIP_MT6353)
    //VCORE1 change to auto mode
    pmic_config_interface(0x44A,0x0,0x1,11);
     /*jade minus TBD :DRAM disable dummy load */
#else
    //VCORE1 change to auto mode
    pmic_config_interface(0x44E,0x0,0x1,1);

    //restore pmic value
    pmic_config_interface(0x0452 , pmic_val_0x452,0xffff ,0);
    pmic_config_interface(0x0450 , pmic_val_0x450,0xffff ,0);


    //DRAM disable dummy load ,0x0B24: MT6351_VDRAM_ANA_CON1
    pmic_config_interface(0x0B24,0x003E,0xFFFF,0);
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
	}

	if(total_dram_size > 0xC0000000ULL) {
		enable_4GB_flag = 1;
		print("[Enable 4GB Support] Total_dram_size = 0x%llx\n,flag:0x%x", total_dram_size,enable_4GB_flag);
		*(volatile unsigned int *)(0x10003208) |= 1 << 15;
		*(volatile unsigned int *)(0x10001f00) |= 1 << 13;
	}
}

unsigned int Is_enable_4GB(void)
{
	print("[Enable 4GB Support] 4GB_flag 0x%x\n",enable_4GB_flag);
	return enable_4GB_flag;
}
