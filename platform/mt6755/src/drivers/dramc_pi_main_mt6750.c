/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/

//=============================================================================
//  Include Files                                                      
//=============================================================================
#if __ETT__
#include <common.h>
#include <ett_common.h>
#include <api.h>
#endif

#include "dramc_common.h"
#include "dramc_register.h"
#include "dramc_pi_api.h"

#ifdef DDR_RESERVE_MODE  
#include "platform.h"
#include "wdt.h"
#endif

#include "emi.h"
#include "upmu_hw.h"
//=============================================================================
//  Definition                                                         
//=============================================================================

//=============================================================================
//  Global Variables                                                  
//=============================================================================
DRAMC_CTX_T *psCurrDramCtx;

DRAMC_CTX_T DramCtx_LPDDR3 =
{
  CHANNEL_A,          // DRAM_CHANNEL
  TYPE_LPDDR3,        // DRAM_DRAM_TYPE_T
#if fcFOR_CHIP_ID == fcK2 || fcFOR_CHIP_ID == fcJADE
  PACKAGE_SBS,        // DRAM_PACKAGE_T
#endif
  DATA_WIDTH_32BIT,     // DRAM_DATA_WIDTH_T
  DEFAULT_TEST2_1_CAL,    // test2_1;
  DEFAULT_TEST2_2_CAL,    // test2_2;
  TEST_XTALK_PATTERN,     // test_pattern;
  DUAL_FREQ_HIGH,
  DUAL_FREQ_LOW,    // frequency_low;
  DISABLE,  // fglow_freq_write_en;
  DISABLE,  // ssc_en;
  DISABLE   // en_4bitMux;
};





//=============================================================================
//  External references                                                
//=============================================================================
extern U8 gIs_ECO_IC;
extern U8 gOnly_09V_1PLL_Flow_Flag;
extern U8 gFAB_IC;

extern void EMI_Init(DRAMC_CTX_T *p);
extern void Dump_EMIRegisters(DRAMC_CTX_T *p);
extern void print_DBG_info(DRAMC_CTX_T *p);
#if 1 //for warning
extern void DramcOtherSettingAfterCalibration(DRAMC_CTX_T *p);
extern void DramcCATraining_4(DRAMC_CTX_T *p, S8 *iFirstClkPass, S8 *iLastClkPass, S8 *iFirstCAPass, S8 *iLastCAPass, S8 *iCAShift, S8 *iFirstClkPass2, S8 *iLastClkPass2, S8 *iFirstCAPass2, S8 *iLastCAPass2, U8 update_CA_flag);
extern void DramcCATraining_2(S8 *iFirstClkPass, S8 *iLastClkPass, S8 *iFirstCAPass, S8 *iLastCAPass, S8 *iFirstClkPass2, S8 *iLastClkPass2, S8 *iFirstCAPass2, S8 *iLastCAPass2, S8 *iMaxCenter);
extern void DramcCATraining_3(DRAMC_CTX_T *p, S8 *iFirstClkPass, S8 *iLastClkPass, S8 *iFirstCAPass, S8 *iLastCAPass, S8 *iCAShift, U8 update_CA_flag);
extern DRAM_STATUS_T Dram_Reset(DRAMC_CTX_T *p, unsigned char use_Hynix_test_mode);
extern DRAM_STATUS_T DramcCATraining_1(DRAMC_CTX_T *p, U32 capattern, S8 *iFirstClkPass, S8 *iLastClkPass, S8 *iFirstCAPass, S8 *iLastCAPass);
extern void DramcSwitchFreq(DRAMC_CTX_T *p);
extern void Global_Option_Init(void);
extern void DramcHWGWDisable(DRAMC_CTX_T *p);
extern void Reg_Sync_Writel(unsigned int addr,unsigned int val);
extern unsigned int Reg_Readl(unsigned int addr);
extern int mt_get_freq_setting(DRAMC_CTX_T *p);
#endif

//=============================================================================
void mt_mempll_init(DRAMC_CTX_T *p)
{
    /*p->channel = CHANNEL_A;
    MemPllInit((DRAMC_CTX_T *) p);

    p->channel = CHANNEL_B;*/

#ifdef DDR_FT_LOAD_BOARD
    LoadBoardGpioInit();
#endif

    MemPllInit((DRAMC_CTX_T *) p);  
    return;
}

void mt_mempll_cali(DRAMC_CTX_T *p)
{
#if fcFOR_CHIP_ID == fcK2 || fcFOR_CHIP_ID == fcJADE
#ifndef ONE_PLL_MODE_INIT
    p->channel = CHANNEL_A;
    DramcPllPhaseCal(p);
#endif
#endif

    return;
}

void mt_mempll_pre(void)
{
#ifdef DDR_RESERVE_MODE  
    unsigned int wdt_mode;
    unsigned int wdt_dbg_ctrl;
#endif
    int dram_type = mt_get_dram_type();
    
    if(dram_type == TYPE_LPDDR3)
    {
        psCurrDramCtx = &DramCtx_LPDDR3;
        default_emi_setting = &emi_setting_default_lpddr3;   
    }
    
    psCurrDramCtx->frequency = mt_get_freq_setting(psCurrDramCtx); 

#ifdef DDR_RESERVE_MODE  
    wdt_mode = READ_REG(MTK_WDT_MODE);
    wdt_dbg_ctrl = READ_REG(MTK_WDT_DEBUG_CTL);
    
    mcSHOW_DBG_MSG(("before mt_mempll_init, wdt_mode = 0x%x, wdt_dbg_ctrl = 0x%x\n", wdt_mode, wdt_dbg_ctrl));     
    if(((wdt_mode & MTK_WDT_MODE_DDR_RESERVE) !=0) && ((wdt_dbg_ctrl & MTK_DDR_RESERVE_RTA) != 0) ) {
        mcSHOW_DBG_MSG(("[PLL] skip mt_mempll_init!!!\n"));
        return;
    }
#endif
    
    mcSHOW_DBG_MSG(("[PLL] mempll_init\n"));
    mt_mempll_init(psCurrDramCtx);
    return;
}

void mt_mempll_post(void)
{
#ifdef DDR_RESERVE_MODE  
    unsigned int wdt_mode;
    unsigned int wdt_dbg_ctrl;
#endif
    int dram_type = mt_get_dram_type();
    
    if(dram_type == TYPE_LPDDR3)
        psCurrDramCtx = &DramCtx_LPDDR3;

#ifdef DDR_RESERVE_MODE  
    wdt_mode = READ_REG(MTK_WDT_MODE);
    wdt_dbg_ctrl = READ_REG(MTK_WDT_DEBUG_CTL);

    mcSHOW_DBG_MSG(("before mt_mempll_cali, wdt_mode = 0x%x, wdt_dbg_ctrl = 0x%x\n", wdt_mode, wdt_dbg_ctrl));     
    if(((wdt_mode & MTK_WDT_MODE_DDR_RESERVE) !=0) && ((wdt_dbg_ctrl & MTK_DDR_RESERVE_RTA) != 0) ) {
        mcSHOW_DBG_MSG(("[PLL] skip mt_mempll_cali!!!\n"));
        return;
    }
#endif
    mcSHOW_DBG_MSG(("[PLL] mempll_cali\n"));

	#ifndef ONE_PLL_MODE_INIT
    mt_mempll_cali(psCurrDramCtx);
	#endif
    return;
}

extern unsigned int g_ddr_reserve_enable;
extern U32 new_DMYPAD_option;
extern U32 gENABLE_MEMPLL_CG_TO_SAVE_POWER_val;

static U32 u4Value_Reg80;
static U32 u4Value_Reg63C;
static U32 u4Value_Reg698;
static U32 u4Value_Reg1DC;
static U32 u4Value_Reg828;
static U32 u4Value_Reg5C8;
static U32 u4Value_Reg5CC;
static U32 u4Value_Reg650;
static U32 u4Value_Reg1C0;
static U32 u4Value_Reg890;
static U32 u4value_mr4;

void Prepare_MEMPLL_init(void) //MEMPLL_EN to 0
{
    U8 ucstatus = 0;
    U32 u4value = 0;

    // Reset PLL initial setting in order to be a correct mempll power on sequence.
    // RG_MEMPLL_BIAS_EN = 0, Reg.60ch[6]
    // RG_MEMPLL_BIAS_LPF_EN = 0, Reg.60ch[7]
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x60c), &u4value);
    mcCLR_BIT(u4value, 6);
    mcCLR_BIT(u4value, 7);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x60c), u4value);
    // RG_MEMPLL_EN = 0, Reg.604h[28]
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x604), &u4value);
    mcCLR_BIT(u4value, 28);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x604), u4value);
    // RG_MEMPLL_DIV_EN = 0, Reg.610h[16]
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x610), &u4value);
    mcCLR_BIT(u4value, 16);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x610), u4value);

    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x614), &u4value);
    mcCLR_BIT(u4value, 0);	// set [0]:RG_MEMPLL2_EN, [23]:MEMPLL2_AUTOK_EN	
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), u4value);

    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x62c), &u4value);
    mcCLR_BIT(u4value, 0);	// set [0]:RG_MEMPLL4_EN, [23]:MEMPLL4_AUTOK_EN
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62c), u4value);

    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x620), &u4value);
    mcCLR_BIT(u4value, 0);	// set [0]:RG_MEMPLL3_EN, [23]:MEMPLL3_AUTOK_EN
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), u4value);		        

    return;
}

void Apply_HW_default_Value(void)//DCM related, DCM off
{
    U32 u4Value = 0;
	
    ucDram_Register_Read(0x1DC, &u4Value);
    u4Value &= ~(0x01 << 25);   
    u4Value &= ~(0x01 << 30);   
    u4Value |= ((0x01 << 31) | (0x01 << 1));
    ucDram_Register_Write(0x1DC, u4Value);
	
    ucDram_Register_Read(0x828, &u4Value);
    u4Value &= ~(0x01 << 1);   
    u4Value &= ~(0x01 << 25);   
    u4Value &= ~(0x01 << 30);   
    u4Value &= ~(0x01 << 31);   
    ucDram_Register_Write(0x828, u4Value);

    ucDram_Register_Read(0x640, &u4Value);
    u4Value &= ~(0x01);   
    ucDram_Register_Write(0x640, u4Value);

    Reg_Sync_Writel((MEM_DCM_CTRL), Reg_Readl(MEM_DCM_CTRL) | (0x1 << 26));
    Reg_Sync_Writel((MEM_DCM_CTRL), Reg_Readl(MEM_DCM_CTRL) | (0x01));
    Reg_Sync_Writel((MEM_DCM_CTRL), Reg_Readl(MEM_DCM_CTRL) & (~0x01)); 
}

void Save_Specific_Regs(void)
{
    ucDram_Register_Read(0x80, &u4Value_Reg80);
    ucDram_Register_Read(0x63C, &u4Value_Reg63C);
    ucDram_Register_Read(0x698, &u4Value_Reg698);
    ucDram_Register_Read(0x828, &u4Value_Reg828);//DCM
    ucDram_Register_Read(0x1dc, &u4Value_Reg1DC);//DCM
//==================================================
    ucDram_Register_Read(0x5c8, &u4Value_Reg5C8);//S/W control
    ucDram_Register_Read(0x5cc, &u4Value_Reg5CC);//S/W control
    ucDram_Register_Read(0x650, &u4Value_Reg650);//S/W control
    ucDram_Register_Read(0x1c0, &u4Value_Reg1C0);//HW gating
    ucDram_Register_Read(0x890, &u4Value_Reg890);//HW gating
    return;
}

void Restore_Specific_Regs(void)//After_Exit_DDR_Reserved_Mode_setting
{
    ucDram_Register_Write(0x80, u4Value_Reg80);//Data latch
    ucDram_Register_Write(0x63C, u4Value_Reg63C);//Dummy pad power down disable->Init DRAM
    ucDram_Register_Write(0x698, u4Value_Reg698);//Save power
    ucDram_Register_Write(0x828, u4Value_Reg828);//DCM
    ucDram_Register_Write(0x1dc, u4Value_Reg1DC);//DCM
    //==========================================================
    ucDram_Register_Write(0x5c8, u4Value_Reg5C8);//S/W control
    ucDram_Register_Write(0x5cc, u4Value_Reg5CC);//S/W control
    ucDram_Register_Write(0x650, u4Value_Reg650);//S/W control
    ucDram_Register_Write(0x1c0, u4Value_Reg1C0);//HW gating
    ucDram_Register_Write(0x890, u4Value_Reg890);//HW gating
    return;
}

void Switch_To_SWcontrol(void)
{
    U8 ucstatus = 0;
    U32 u4value = 0;

    u4value = Reg_Readl(DDRPHY_BASE + 0x650);
    u4value &= ~0x00000018;//Switch to SW mode, fix to 3 PLL mode, 3th and 4th set to 0
    Reg_Sync_Writel((DDRPHY_BASE + 0x650), u4value);

    DramcHWGWDisable(psCurrDramCtx);

	u4value = Reg_Readl((SPM_POWER_ON_VAL0));
    Reg_Sync_Writel((SPM_POWER_ON_VAL0), u4value & (~(0x1 << 13)));

	// Enable S/W control (not SPM)
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x5c8), 0x01011f01);//[9]:rg_mempll2_pwd_en_sel, [12]:by_mempll_pwd_dly
	// Enable S/W control (not SPM)
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x5cc), 0x41011101);//by_dmpll2_ck_en_dly first, manual enable mempll[12], by_mempll_div_en_dly[20] TBD

}
void Before_Exit_DDR_Reserved_Mode_setting(void)//Switch to Sw control
{
    U8 ucstatus = 0;
    U32 u4value = 0;
    
	if(g_ddr_reserve_enable==0) 
        return;

    //MR4 setting
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_ACTIM1), &u4value_mr4);
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_ACTIM1), &u4value);
    mcSET_BIT(u4value, 26);//disable
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_ACTIM1), u4value);    

    mcDELAY_MS(1);
    CHECK_09V_1PLL_FLOW();
    Save_Specific_Regs();//Since exit reserve mode will cover some regs
    Reg_Sync_Writel((SPM_POWER_ON_VAL0), Reg_Readl((SPM_POWER_ON_VAL0)) & ~((0x1 << 12) | (0x1 << 14)));  

    Switch_To_SWcontrol();

	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x698), &u4value);
	u4value = u4value | 0x000e0000;
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x698), u4value);

}

#define PDEF_SPM_RSV_5 0x1000661C
void After_Exit_DDR_Reserved_Mode_setting(void)
{
    EMI_SETTINGS *emi_set;
    U8 ucstatus = 0;
    U32 u4value = 0;
    mcSHOW_DBG_MSG(("@@@@@@@ Final => OK @@@@@@@@\n")); 
    if(g_ddr_reserve_enable==0) 
        return;

    //MR4 setting
    //ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_ACTIM1), &u4value);
    //mcCLR_BIT(u4value, 26);//enable
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_ACTIM1), u4value_mr4);    

    mcDELAY_MS(1);

    if(emi_setting_index == -1)
        emi_set = default_emi_setting;
    else
        emi_set = &emi_settings[emi_setting_index]; 

    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xfc), emi_set->DRAMC_MISCTL0_VAL);

    if (gOnly_09V_1PLL_Flow_Flag == 1)//Lewis@20160112: Fix for Jade minus since it has the chance to run no shuffle
    {
    	//Force to 1PLL mode, [12] spm2dramc_shuffle_switch, [14] spm2dpy_1pll_switch 
        Reg_Sync_Writel((SPM_POWER_ON_VAL0), Reg_Readl((SPM_POWER_ON_VAL0)) | (0x1 << 12) | (0x1 << 14));		
        Reg_Sync_Writel((PDEF_SPM_RSV_5), Reg_Readl((PDEF_SPM_RSV_5)) & ~(0x1));//1pll bit[0]=0 		

    }
    else
    {
    	//Force to 3PLL mode, [12] spm2dramc_shuffle_switch, [14] spm2dpy_1pll_switch 
    Reg_Sync_Writel((SPM_POWER_ON_VAL0), Reg_Readl((SPM_POWER_ON_VAL0)) & ~((0x1 << 12) | (0x1 << 14)));    
        Reg_Sync_Writel((PDEF_SPM_RSV_5), Reg_Readl((PDEF_SPM_RSV_5)) | (0x1));//3pll bit[0]=1 		
    }   
    DramcHWGWEnable(psCurrDramCtx);
    //Ma@20150831: Should set to 1 after exit reserve mode
    Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) | (0x01 << 7));	// 1pll_mode: dcm_en=0
    Restore_Specific_Regs();
#if !(__ETT__)
	TransferToSPMControl();
#endif
    //Restore_Specific_Regs();//Since exit reserve mode will cover some regs
}

//After WDT reset, it will call this function; Use 3 PLL mode to do MEMPLL init and phase calibration
void Exit_DDR_Reserved_Mode_setting(void)
{
    volatile int read_data;
    unsigned int phy_ldo, phy_pwr_con;
    U8 ucstatus = 0;
    U32 u4value;
    int dram_type = mt_get_dram_type();

    if(g_ddr_reserve_enable==0)
        return;

    if(dram_type == TYPE_LPDDR3)
    {
        psCurrDramCtx = &DramCtx_LPDDR3;
        default_emi_setting = &emi_setting_default_lpddr3;   
    }  

    phy_ldo = Reg_Readl((CHA_DDRPHY_BASE + (0x644)));//Enable PHY LDO pwr
    phy_ldo |= (0x100);
    Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x644)), phy_ldo);
  
    phy_pwr_con = Reg_Readl(0x1000631C);//Reset DDR PHY
    phy_pwr_con &= 0xfffffffc;
    Reg_Sync_Writel(0x1000631C, phy_pwr_con);
    mcDELAY_US(1);
    phy_pwr_con |= 0xd;
    Reg_Sync_Writel(0x1000631C, phy_pwr_con);
    
//#ifndef DUAL_FREQ_K
if (gOnly_09V_1PLL_Flow_Flag == 1)
{
#ifdef FRCLK_26M_DIV8
    //in DDR reserved mode, need to set dramc_f26m_sel because it will be clear in reset
    *(volatile unsigned *)(0x10001088) |= 0x80000000;//MODULE_SW_CG_1_SET[31] = 1 ==> dramc_f26m_cg_set
    *(volatile unsigned *)(0x10001098) |= (0x01 <<7);//dramc_f26m_sel : 0-div 1, 1-div8
    *(volatile unsigned *)(0x1000108C) |= 0x80000000;//MODULE_SW_CG_1_CLR[31] = 1 ==> dramc_f26m_cg_set
#endif
//#endif
}
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1dc), &u4value);
    mcCLR_BIT(u4value, 30);
    mcCLR_BIT(u4value, 31);
	mcCLR_BIT(u4value, 25);
	mcCLR_BIT(u4value, 1);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1dc), u4value);    
    mcDELAY_US(1);

    /* read fifo reset */
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xf4), &u4value);
    mcSET_BIT(u4value, 25); //read data counter reset enable
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf4), u4value);    
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xf4), &u4value);
    mcCLR_BIT(u4value, 25); //read data counter reset disable
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf4), u4value);    

    /* phase sync reset */
    read_data = Reg_Readl((CHA_DDRPHY_BASE + 0x5cc));
    Reg_Sync_Writel((CHA_DDRPHY_BASE + 0x5cc), read_data | (0x00001100));

	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x00022091);//113904ns, [0] TBD
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x000220b1);//113544ns, [0] TBD

    Reg_Sync_Writel((CHA_DDRPHY_BASE + 0x5cc), read_data);
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xF0), &u4value);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xF0), u4value | 0x10000000);  //PHY reset enable
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xF0), (u4value & ~0x10000000));   //PHY reset disable

    read_data = Reg_Readl((CHA_DDRPHY_BASE + 0x640));
    Reg_Sync_Writel((CHA_DDRPHY_BASE + 0x640), read_data & 0xfffffffe);
    Reg_Sync_Writel((CHA_DDRPHY_BASE + 0x640), read_data | 0x1);

    mcDELAY_US(1);
	//[20] Asynchronous mode enabling between DRAMC & DDRPHY : 0:sync mode 1:async mode
	//need set async mode at 26Mhz!!
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xfc), &u4value);
	u4value |= 1 << 20;
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xfc), u4value);

#if 1
    psCurrDramCtx->frequency = mt_get_freq_setting(psCurrDramCtx);//Force to High Freq.
    mcSHOW_DBG_MSG(("[DDR Reserved Mode][PLL] mempll_init\n"));
    Apply_HW_default_Value();
    Prepare_MEMPLL_init();
    mt_mempll_init(psCurrDramCtx);
    mcSHOW_DBG_MSG(("[DDR Reserved Mode][PLL] mempll_cali\n"));
    mt_mempll_cali(psCurrDramCtx);
    mcSHOW_DBG_MSG(("\n\n3.After Phase K ...\n\n"));
    print_DBG_info(psCurrDramCtx);
#endif
}

#if 0 //Reserver mode reference code
void reset_pll234(void)
{
 	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), 0x4e01c000 ); 
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), 0x4e01c000 );
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), 0x4e01c000 ); 
    
    mcDELAY_US(1);    
    Reg_Sync_Writel((DDRPHY_BASE + 0x698), 0x000e5e00);
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), 0x4e00c000 );
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), 0x4e00c000 ); 
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), 0x4e00c000 );
}

void FORCE_DRAM_HIGH(void)
{
    U32 u4value = 0;
    U32 u4read_data_16c;

    ///* TINFO="== FORCE_to_HIGH() called ==" */ *MDM_TM_TINFOMSG = 1817;
    //*MEM_DCM_CTRL      =                (0x1f << 21) | (0x0 << 16) | (0x1 << 9) | (0x1 << 8) | (0x0 << 7) | (0x0 << 6) | (0x1f << 1);   // 3pll_mode: dcm_en = 0                                                                   
    //*MEM_DCM_CTRL     |=  0x1;    // toggle enable
    //*MEM_DCM_CTRL     &= ~(0x1);  // toggle enable
    u4value = (0x1f << 21) | (0x0 << 16) | (0x1 << 9) | (0x1 << 8) | (0x0 << 7) | (0x0 << 6) | (0x1f << 1);   // 3pll_mode: dcm_en = 0 
    Reg_Sync_Writel(MEM_DCM_CTRL, u4value);
    u4value = Reg_Readl(MEM_DCM_CTRL);
    u4value |=  0x1;
    Reg_Sync_Writel(MEM_DCM_CTRL, u4value);
    u4value &= ~(0x1);
    Reg_Sync_Writel(MEM_DCM_CTRL, u4value);
  
    //*DFS_MEM_DCM_CTRL  =  (0x1 << 31); // gate off the 1pll_mode clock before change the mux sel
    //*DFS_MEM_DCM_CTRL  =  (0x1 << 28) | (0x1f << 21) | (0x0 << 16) | (0x1 << 9) | (0x1 << 8) | (0x0 << 7) | (0x0 << 6) | (0x1f << 1);   // 1pll_mode: dcm_en=0
    //*DFS_MEM_DCM_CTRL &= ~(0x1 << 31); // gate on  the 1pll_mode clock after  change the mux sel
    //*DFS_MEM_DCM_CTRL |= 0x1;    // toggle enable
    //*DFS_MEM_DCM_CTRL &= ~(0x1); // gate off the 1pll_mode clock before change the mux sel
    Reg_Sync_Writel((DFS_MEM_DCM_CTRL), (0x1 << 31));
	Reg_Sync_Writel((DFS_MEM_DCM_CTRL), (0x1 << 28) | (0x1f << 21) | (0x0 << 16) | (0x1 << 9) | (0x1 << 8) | (0x0 << 7) | (0x0 << 6) | (0x1f << 1));
	Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) & ~(0x01 << 31));
	Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) | 0x01);
	Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) & (~0x01));


    // disable MEMPLL
    //*((UINT32P)(DDRPHY_BASE    + (0x0172 << 2)))|= 0x00000F00; // [8]:rg_mempll_pwd_en_sel, [9]:mempll2, [10]:mempll3, [11]:mempll4
    //*((UINT32P)(DDRPHY_BASE    + (0x0185 << 2)))&=~0x00000001; // Clr [0]:RG_MEMPLL2_EN
    //*((UINT32P)(DDRPHY_BASE    + (0x018b << 2)))&=~0x00000001; // Clr [0]:RG_MEMPLL4_EN
    //*((UINT32P)(DDRPHY_BASE    + (0x0188 << 2)))&=~0x00000001; // Clr [0]:RG_MEMPLL3_EN
    //*((UINT32P)(DDRPHY_BASE    + (0x0181 << 2)))&=~0x10000000; // Rx181[28]: Clr RG_MEMPLL_EN = 0
    u4value = Reg_Readl(DDRPHY_BASE + 0x5C8);
    u4value |= 0x00000F00;   
    Reg_Sync_Writel((DDRPHY_BASE + 0x5C8), u4value);

    u4value = Reg_Readl(DDRPHY_BASE + 0x614);
    u4value &=~0x00000001;   
    Reg_Sync_Writel((DDRPHY_BASE + 0x614), u4value);

    u4value = Reg_Readl(DDRPHY_BASE + 0x62C);
    u4value &=~0x00000001;   
    Reg_Sync_Writel((DDRPHY_BASE + 0x62C), u4value);

    u4value = Reg_Readl(DDRPHY_BASE + 0x620);
    u4value &=~0x00000001;   
    Reg_Sync_Writel((DDRPHY_BASE + 0x620), u4value);

    u4value = Reg_Readl(DDRPHY_BASE + 0x604);
    u4value &=~0x10000000;   
    Reg_Sync_Writel((DDRPHY_BASE + 0x604), u4value);
    //check shuffle_start_low state
    //Rx5b[1]
    //read_data = *((UINT32P)(DRAMC0_BASE + (0x005b << 2)));
    u4read_data_16c = Reg_Readl(DRAMC0_BASE + 0x16C);

    //enable MEMPLL
    //mem_wait_us(1);   // MEMPLL_EN must low > 1us, cannot enable too soon
    mcDELAY_US(1);    

    //*((UINT32P)(DDRPHY_BASE    + (0x0181 << 2)))|= 0x10000000; // Rx181[28]: Set RG_MEMPLL_EN = 1
    //*((UINT32P)(DDRPHY_BASE    + (0x0185 << 2)))|= 0x00000001; // Set [0]:RG_MEMPLL2_EN, [23]:MEMPLL2_AUTOK_EN
    //*((UINT32P)(DDRPHY_BASE    + (0x018b << 2)))|= 0x00000001; // Set [0]:RG_MEMPLL4_EN, [23]:MEMPLL4_AUTOK_EN
    //*((UINT32P)(DDRPHY_BASE    + (0x0188 << 2)))|= 0x00000001; // Set [0]:RG_MEMPLL3_EN, [23]:MEMPLL3_AUTOK_EN
    u4value = Reg_Readl(DDRPHY_BASE + 0x604);
    u4value |= 0x10000000;   
    Reg_Sync_Writel((DDRPHY_BASE + 0x604), u4value);

    u4value = Reg_Readl(DDRPHY_BASE + 0x614);
    u4value |= 0x00000001;   
    Reg_Sync_Writel((DDRPHY_BASE + 0x614), u4value);

    u4value = Reg_Readl(DDRPHY_BASE + 0x62C);
    u4value |= 0x00000001;   
    Reg_Sync_Writel((DDRPHY_BASE + 0x62C), u4value);

    u4value = Reg_Readl(DDRPHY_BASE + 0x620);
    u4value |= 0x00000001;   
    Reg_Sync_Writel((DDRPHY_BASE + 0x620), u4value);

    //if ((read_data & 0x2) == 0x0) 
    if((u4read_data_16c & 0x2) == 0x0) // 3PLL mode
    {                           // polling shuffle_start_low
        ///* TINFO="== SHUFFLE_LOW 0  ==" */ *MDM_TM_TINFOMSG = 1819;
        // 1667

        //clk_on_base_pll();       // to switch the clock, the original clock must be active first
        u4value = Reg_Readl(DDRPHY_BASE + 0x698);
        u4value |= 0x00000020;   
        Reg_Sync_Writel(DDRPHY_BASE + 0x698, u4value);      

        //*CLK_CFG_0_CLR  = 0x300; // Clr [9:8] to be 2'b00
        //*CLK_CFG_0_SET  = 0x100; // Set [9:8] to be 2'b01, switch to 3PLL mode (dmpll_ck_buf)
        //*CLK_CFG_UPDATE = 0x2;
        Reg_Sync_Writel((CLK_CFG_0_CLR), 0x300);
        Reg_Sync_Writel((CLK_CFG_0_SET), 0x100);	
        Reg_Sync_Writel((CLK_CFG_UPDATE), 0x02);
        //*((UINT32P)(DDRPHY_BASE    +(0x01a6 << 2)))&=~0x00008010; // switch to 3PLL mode, [4],[15]
        u4value = Reg_Readl(DDRPHY_BASE + 0x698);
        u4value &=~0x00008010;   
        Reg_Sync_Writel((DDRPHY_BASE + 0x698), u4value);
            
        /* LewisLi:important issues */
        // clear Rx194[3]  
        //*((UINT32P)(DDRPHY_BASE    +(0x0194 << 2)))&=~0x00000008; // Enable S/W Control, R_SRC_SEL_DIV2_SYNC=0
        u4value = Reg_Readl(DDRPHY_BASE + 0x650);
        u4value &=~0x00000008;   
        Reg_Sync_Writel((DDRPHY_BASE + 0x650), u4value);
        //ddr_reserved_release_dramc_iso();
        rgu_release_rg_dramc_iso();
        rgu_release_rg_dramc_sref();
        //reset_pll234();
        ///* TINFO="Release control from SPM to S/W" */ *MDM_TM_TINFOMSG = 1818;
        //*((UINT32P)(DDRPHY_BASE    +(0x0172 << 2))) = 0x00001f10;
        //*((UINT32P)(DDRPHY_BASE    +(0x0194 << 2))) = 0xd2000000;
        //*((UINT32P)(DDRPHY_BASE    +(0x0173 << 2))) = 0x41011101; // clear [0,8,16,24]
        Reg_Sync_Writel((DDRPHY_BASE + 0x5c8), 0x00001f10);
        Reg_Sync_Writel((DDRPHY_BASE + 0x650), 0xd2000000);
        Reg_Sync_Writel((DDRPHY_BASE + 0x5CC), 0x41011101);
        //*((UINT32P)(DDRPHY_BASE    +(0x01a6 << 2)))|= 0x000e0000; //
        //*((UINT32P)(DDRPHY_BASE    +(0x01a6 << 2))) = 0x000e5e00; // switch to 3PLL mode
        u4value = Reg_Readl(DDRPHY_BASE + 0x698);
        u4value |= 0x000e0000;   
        Reg_Sync_Writel((DDRPHY_BASE + 0x698), u4value);
        Reg_Sync_Writel((DDRPHY_BASE + 0x698), 0x000e5e00);
        //DFS_to_LOW();
    } 
    else // one PLL mode
    {
        ///* TINFO="== SHUFFLE_LOW 1  ==" */ *MDM_TM_TINFOMSG = 1820;
        // 1333
        //*CLK_CFG_0_CLR  = 0x300; // Clr [9:8] to be 2'b00
        //*CLK_CFG_0_SET  = 0x200; // Set [9:8] to be 2'b01, switch to 1PLL mode (dmpll_ck_buf)
        //*CLK_CFG_UPDATE = 0x2;
        Reg_Sync_Writel((CLK_CFG_0_CLR), 0x300);
        Reg_Sync_Writel((CLK_CFG_0_SET), 0x200);	
        Reg_Sync_Writel((CLK_CFG_UPDATE), 0x02);
        //*((UINT32P)(DDRPHY_BASE    +(0x01a6 << 2)))|= 0x00008010; // switch to 1PLL mode:[4],[15]
        u4value = Reg_Readl(DDRPHY_BASE + 0x698);
        u4value |= 0x00008010;   
        Reg_Sync_Writel((DDRPHY_BASE + 0x698), u4value);        

        //ddr_reserved_release_dramc_iso();
        rgu_release_rg_dramc_iso();
        rgu_release_rg_dramc_sref();
        //reset_pll234();

        ///* TINFO="Release control from SPM to S/W" */ *MDM_TM_TINFOMSG = 1818;
        //*((UINT32P)(DDRPHY_BASE    +(0x0172 << 2))) = 0x00001f10;
        //*((UINT32P)(DDRPHY_BASE    +(0x0194 << 2))) = 0xd2000000;
        //*((UINT32P)(DDRPHY_BASE    +(0x0173 << 2))) = 0x41011101; // clear [0,8,16,24]
        Reg_Sync_Writel((DDRPHY_BASE + 0x5c8), 0x00001f10);
        Reg_Sync_Writel((DDRPHY_BASE + 0x650), 0xd2000000);
        Reg_Sync_Writel((DDRPHY_BASE + 0x5CC), 0x41011101);
        //*((UINT32P)(DDRPHY_BASE    +(0x01a6 << 2)))|= 0x000e0000; //
        //*((UINT32P)(DDRPHY_BASE    +(0x01a6 << 2))) = 0x000ede30; // switch to 1PLL mode:[4]
        u4value = Reg_Readl(DDRPHY_BASE + 0x698);
        u4value |= 0x000e0000;   
        Reg_Sync_Writel((DDRPHY_BASE + 0x698), u4value);
        Reg_Sync_Writel((DDRPHY_BASE + 0x698), 0x000ede30);

        //*((UINT32P)(DDRPHY_BASE    + (0x0183 << 2))) &= ~0x00000080;  // Clr [7]:RG_MEMPLL_BIAS_LPF_EN
        //*((UINT32P)(DDRPHY_BASE    + (0x0185 << 2))) &= ~0x00800001;  // Clr [0]:RG_MEMPLL2_EN, [23]:MEMPLL2_AUTOK_EN
        //*((UINT32P)(DDRPHY_BASE    + (0x018b << 2))) &= ~0x00800001;  // Clr [0]:RG_MEMPLL4_EN, [23]:MEMPLL4_AUTOK_EN
        //*((UINT32P)(DDRPHY_BASE    + (0x0188 << 2))) &= ~0x00800001;  // Clr [0]:RG_MEMPLL3_EN, [23]:MEMPLL3_AUTOK_EN
        u4value = Reg_Readl(DDRPHY_BASE + 0x60C);
        u4value &= ~0x00000080;   
        Reg_Sync_Writel((DDRPHY_BASE + 0x60C), u4value);
        
        u4value = Reg_Readl(DDRPHY_BASE + 0x614);
        u4value &= ~0x00800001;   
        Reg_Sync_Writel((DDRPHY_BASE + 0x614), u4value);

        u4value = Reg_Readl(DDRPHY_BASE + 0x62C);
        u4value &= ~0x00800001;   
        Reg_Sync_Writel((DDRPHY_BASE + 0x62C), u4value);

        u4value = Reg_Readl(DDRPHY_BASE + 0x620);
        u4value &= ~0x00800001;   
        Reg_Sync_Writel((DDRPHY_BASE + 0x620), u4value);
        
    }
    //DFS_to_HIGH();
    ///* TINFO="== FORCE_to_HIGH() return ==" */ *MDM_TM_TINFOMSG = 1821;
}
#endif
//DDR reserve mode related [End]

unsigned int ModeRegRead(int rank, int MRR_num)
{
    unsigned int MRR_value = 0x0;
    unsigned int dram_type, ucstatus=0, u4value;
    DRAMC_CTX_T *p = psCurrDramCtx; 
    unsigned int backup_reg_0x1e8;


    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_ACTIM1), &u4value);
    backup_reg_0x1e8 = u4value;
    mcSET_BIT(u4value, 26); //0:Enable 1:Disable
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_ACTIM1), u4value);  

    // CKEFIXON before MRW
    if (rank==0)
    	{
		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xe4), &u4value);
		mcSET_BIT(u4value, 2);
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xe4), u4value);
	}
	else
	{
		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xf4), &u4value);
		mcSET_BIT(u4value, 20);
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf4), u4value);		
	}

    mcDELAY_US(200);     // tINIT3 > 200us

        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), MRR_num | (rank<<26));
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000002);
#if 0 //#ifdef MRW_WAIT_RESPONSE
        MRR_Wait_Response();
#else
        mcDELAY_US(10);
#endif 
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x03B8), &u4value);
        MRR_value = (u4value >> 20) & 0xFF;

    // release CKEFIXON
    if (rank==0)
    	{
	    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xe4), &u4value);
	    mcCLR_BIT(u4value, 2);
	    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xe4), u4value);
	}
	else
	{
	    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xf4), &u4value);
	    mcCLR_BIT(u4value, 20);
	    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf4), u4value);		
	}

    //restore backup value
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_ACTIM1), backup_reg_0x1e8);  


    return MRR_value;
}

void Fra_ModeRegWrite(unsigned int op, unsigned int value, unsigned int rank_num)
{
unsigned int u4value, ucstatus;

mcSHOW_DBG_MSG(("Fra_ModeRegWrite Rank%d, MRW%d, write 0x%x\n",rank_num, op, value));

#if 0
	if (rank_num==0)
	{
    		// CKEFIXON before MRW
    		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xe4), &u4value);
    		mcSET_BIT(u4value, 2);
    		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xe4), u4value);
	}
	else
	{
                // CKEFIXON before MRW
                ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xf4), &u4value);
                mcSET_BIT(u4value, 20);
                ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf4), u4value);
	}
#endif

	value = (rank_num<<28) | (value<<16) | op;

                ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), value);        // 116716ns
                ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);     // 116776ns
#if 0 //#ifdef MRW_WAIT_RESPONSE
                MRW_Wait_Response();
#else
                mcDELAY_US(10);
#endif
                ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);     // 117178ns

#if 0
	if (rank_num==0)
	{
        	// release CKEFIXON
        	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xe4), &u4value);
       		mcCLR_BIT(u4value, 2);
        	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xe4), u4value);
	}
	else
	{
                // release CKEFIXON
                ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xf4), &u4value);
                mcCLR_BIT(u4value, 20);
                ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf4), u4value);
	}
#endif
}





int Init_DRAM(void)
{
  DRAMC_CTX_T * p;
  U8 ucstatus = 0;
  U32 u4value, err;
  
	Global_Option_Init();
  
#if fcFOR_CHIP_ID == fcJADE
  //[20] Asynchronous mode enabling between DRAMC & DDRPHY : 0:sync mode 1:async mode
  // need set async mode at 26Mhz!!
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xfc), &u4value);
  u4value |= 1 << 20;
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xfc), u4value);
#endif

  mt_mempll_pre();
  mt_mempll_post(); 

    //set mem_clk
    //CLK_CFG_0[9:8], 0: mem_ck is 26M , 1: mem_ck is 3PLL mode, 2:mem_ck is 1PLL_mode    
	u4value = Reg_Readl(CLK_CFG_0); 	
    u4value = u4value & 0xfffffcff;
    u4value = u4value | (1<<8);
    Reg_Sync_Writel((CLK_CFG_0), u4value);	//mem_ck =mempll    
    Reg_Sync_Writel((CLK_CFG_UPDATE), 0x02);	

  p = psCurrDramCtx;  

    
  //According to Benson & MH,  K2 with LPDDR3/2 VBianN is connected to GND directly. So no need to set.
  // Reg.648h[15:11] INTREF2_REFTUNE=01111b      [10:9]INTREF2_DS=10b      [8] EN_INTREF2=1b
  // Reg.648h[7:3] INTREF1_REFTUNE=01111b      [2:1]INTREF1_DS_DQ=10b      [0] EN_INTREF1_DQ=1b
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x648), &u4value);
  u4value &= 0xffff0000;
  u4value |= 0x00007d7d;
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x648), u4value);        

  // Reg.644h[7:3] INTREF0_REFTUNE=01111b      [2:1]INTREF0_DS_DQ=10b      [0] EN_INTREF0_DQ=1b
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x644), &u4value);
  u4value &= 0xffffff00;
  u4value |= 0x0000007d;
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x644), u4value);        
        
  mcDELAY_MS(50); // According to ACD spec, need to delay 50ms in normal operation (DS1,DS0)=(1,0).

  EMI_Init(p);    

  mcSHOW_DBG_MSG(("\n\n[JADE] ETT version 0.0.9.7\n\n"));

    p->channel = CHANNEL_A;
    if (gOnly_09V_1PLL_Flow_Flag == 1)
    {
        p->frequency = DUAL_FREQ_LOW;
        DramcSwitchFreq(p);
    }
    DramcInit((DRAMC_CTX_T *) p);   

#ifdef FTTEST_ZQONLY
  while (1)
  {
      ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), LPDDR3_MODE_REG_10);
#ifdef MRW_WAIT_RESPONSE
      MRW_Wait_Response();
#else	  
      ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);
      mcDELAY_US(1);    // tZQINIT>=1us
      ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);
#endif	  
  }
#endif

#if defined(DDR_INIT_TIME_PROFILING)
  /* enable ARM CPU PMU */
  asm volatile(
      "MRC p15, 0, %0, c9, c12, 0\n"
      "BIC %0, %0, #1 << 0\n"   /* disable */
      "ORR %0, %0, #1 << 2\n"   /* reset cycle count */
      "BIC %0, %0, #1 << 3\n"   /* count every clock cycle */
      "MCR p15, 0, %0, c9, c12, 0\n"
      : "+r"(temp)
      :
      : "cc"
  );
  asm volatile(
      "MRC p15, 0, %0, c9, c12, 0\n"
      "ORR %0, %0, #1 << 0\n"   /* enable */
      "MCR p15, 0, %0, c9, c12, 0\n"
      "MRC p15, 0, %0, c9, c12, 1\n"
      "ORR %0, %0, #1 << 31\n"
      "MCR p15, 0, %0, c9, c12, 1\n"
      : "+r"(temp)
      :
      : "cc"
  );

  mcDELAY_US(100);

  /* get CPU cycle count from the ARM CPU PMU */
  asm volatile(
      "MRC p15, 0, %0, c9, c12, 0\n"
      "BIC %0, %0, #1 << 0\n"   /* disable */
      "MCR p15, 0, %0, c9, c12, 0\n"
      "MRC p15, 0, %0, c9, c13, 0\n"
      : "+r"(temp)
      :
      : "cc"
  );
  mcSHOW_DBG_MSG((" mcDELAY_US(100) takes %d CPU cycles\n\r", temp));
#endif

#if defined(DDR_INIT_TIME_PROFILING)
  /* enable ARM CPU PMU */
  asm volatile(
      "MRC p15, 0, %0, c9, c12, 0\n"
      "BIC %0, %0, #1 << 0\n"   /* disable */
      "ORR %0, %0, #1 << 2\n"   /* reset cycle count */
      "BIC %0, %0, #1 << 3\n"   /* count every clock cycle */
      "MCR p15, 0, %0, c9, c12, 0\n"
      : "+r"(temp)
      :
      : "cc"
  );
  asm volatile(
      "MRC p15, 0, %0, c9, c12, 0\n"
      "ORR %0, %0, #1 << 0\n"   /* enable */
      "MCR p15, 0, %0, c9, c12, 0\n"
      "MRC p15, 0, %0, c9, c12, 1\n"
      "ORR %0, %0, #1 << 31\n"
      "MCR p15, 0, %0, c9, c12, 1\n"
      : "+r"(temp)
      :
      : "cc"
  );
#endif

        
#ifdef LOOPBACK_TEST
  mcSHOW_DBG_MSG(("\n\nChannel A loop-back test...n\n"));
  DramcLoopbackTest(p);
  print_DBG_info(p);
  while (1);
#endif
  if ((gOnly_09V_1PLL_Flow_Flag == 0) && (gFAB_IC == 1))
  {	//switch to 0.95V
    pmic_config_interface(Vcore_REG_SW, Vcore1_Before_ImpedanceK ,0x7F, 0);
    pmic_config_interface(Vcore_REG_HW, Vcore1_Before_ImpedanceK ,0x7F, 0);
  }

  p->channel = CHANNEL_A;
  DramcSwImpedanceCal((DRAMC_CTX_T *) p, 1);  //BringUp : Maybe bypass.
    
  //mcSHOW_DBG_MSG(("mt_get_mem_freq=%d\n", mt_get_bus_freq()));
  if ((gOnly_09V_1PLL_Flow_Flag == 0) && (gFAB_IC == 1))
  {//switch to 1.0V 
    pmic_config_interface(Vcore_REG_SW, Vcore1_After_ImpedanceK ,0x7F, 0);
    pmic_config_interface(Vcore_REG_HW, Vcore1_After_ImpedanceK ,0x7F, 0);
  }	
#if 0 
// Run again here for different voltage. For preloader, if the following code is executed after voltage change, no need.
    DramcEnterSelfRefresh(p, 1);
    mcSHOW_DBG_MSG(("mempll init again\n"));
  MemPllInit(p);    
  mcDELAY_US(1);
  mt_mempll_cali(p);              //BringUp : Maybe bypass.
    mcSHOW_DBG_MSG(("Dramc clock=%d\n", mt_get_bus_freq()));
    DramcEnterSelfRefresh(p, 0);
#endif
  
//#ifdef DUAL_FREQ_K
DDR_CALI_START: 
  // Disable MR4 for GW calibration.
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_ACTIM1), &u4value);
  mcSET_BIT(u4value, 26); //0:Enable 1:Disable  
  mcSET_FIELD(u4value, 0x0, MASK_ACTIM1_REFRCNT, POS_ACTIM1_REFRCNT);
#ifdef INCREASE_ZQCSCNT_AND_REFRCNT_RANGE		
#if fcFOR_CHIP_ID == fcJADE		
  mcCLR_BIT(u4value, 2);	// REFRCNT[7:0] -> REFRCNT[8:0]
#endif
#endif  
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_ACTIM1), u4value);    
//#endif

    
#ifdef MATYPE_ADAPTATION  
    // Backup here because Reg.04h may be modified based on different column address of different die or channel.
    // Default value should be the smallest number.
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x04), &u4Backup_Reg_04);
#endif


  p->channel = CHANNEL_A;


   if (p->dram_type == TYPE_LPDDR3)
    {  
        S8 iFirstClkPass[CATRAINING_NUM]={0}, iLastClkPass[CATRAINING_NUM]={0}, iFirstCAPass[CATRAINING_NUM]={0}, iLastCAPass[CATRAINING_NUM]={0};
        S8 iFirstClkPass2[CATRAINING_NUM]={0}, iLastClkPass2[CATRAINING_NUM]={0}, iFirstCAPass2[CATRAINING_NUM]={0}, iLastCAPass2[CATRAINING_NUM]={0}; 
        S8 iCAShift[CATRAINING_NUM]={0};
        S8 iMaxCenter = 0;

#ifdef CA_TRAINING_NEW_FLOW
        S8 iFirstClkPass_1PLL[CATRAINING_NUM]={0}, iLastClkPass_1PLL[CATRAINING_NUM]={0}, iFirstCAPass_1PLL[CATRAINING_NUM]={0}, iLastCAPass_1PLL[CATRAINING_NUM]={0};
        S8 iFirstClkPass_3PLL[CATRAINING_NUM]={0}, iLastClkPass_3PLL[CATRAINING_NUM]={0}, iFirstCAPass_3PLL[CATRAINING_NUM]={0}, iLastCAPass_3PLL[CATRAINING_NUM]={0};
        U32 u4prv_register_698;
        U32 backup_reg_0xb0, backup_reg_0xb8, backup_reg_0xbc;
#endif        
        
//#ifdef DUAL_FREQ_K

if (gOnly_09V_1PLL_Flow_Flag == 0)
{
#ifdef SPM_WORKAROUND_CATRAINING
            DramcCATraining_1((DRAMC_CTX_T *) p, 0x55555555, iFirstClkPass, iLastClkPass, iFirstCAPass, iLastCAPass);
            Dram_Reset((DRAMC_CTX_T *) p);

            DramcCATraining_3((DRAMC_CTX_T *) p, iFirstClkPass, iLastClkPass, iFirstCAPass, iLastCAPass, iCAShift, 1);
#endif

        
                if (p->frequency != DUAL_FREQ_LOW)   
              {
                // CA, clock output delay regsiters only have one set which could not be shuffled. So only do calibration in high frequency.
#ifndef CA_TRAINING_NEW_FLOW                
#ifndef SPM_WORKAROUND_CATRAINING
            DramcCATraining_1((DRAMC_CTX_T *) p, 0x55555555, iFirstClkPass_1PLL, iLastClkPass_1PLL, iFirstCAPass_1PLL, iLastCAPass_1PLL);
            Dram_Reset((DRAMC_CTX_T *) p);
            
            DramcCATraining_3((DRAMC_CTX_T *) p, iFirstClkPass_1PLL, iLastClkPass_1PLL, iFirstCAPass_1PLL, iLastCAPass_1PLL, iCAShift, 1);                
#endif
#endif

#ifdef CA_TRAINING_NEW_FLOW

if (gIs_ECO_IC == 0)
{
        ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xb0), &backup_reg_0xb0);
        ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xb8), &backup_reg_0xb8);
        ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xbc), &backup_reg_0xbc);

        p->frequency = DUAL_FREQ_LOW;
        DramcSwitchFreq(p);
        DramcInit((DRAMC_CTX_T *) p);   

        DramcCATraining_1((DRAMC_CTX_T *) p, 0x55555555, iFirstClkPass_1PLL, iLastClkPass_1PLL, iFirstCAPass_1PLL, iLastCAPass_1PLL);
        Dram_Reset((DRAMC_CTX_T *) p, 0);
        DramcCATraining_1((DRAMC_CTX_T *) p, 0x99999999, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2);
        Dram_Reset((DRAMC_CTX_T *) p, 0);        
        DramcCATraining_2(iFirstClkPass_1PLL, iLastClkPass_1PLL, iFirstCAPass_1PLL, iLastCAPass_1PLL, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2, &iMaxCenter);
        DramcCATraining_1((DRAMC_CTX_T *) p, 0xaaaaaaaa, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2);
        Dram_Reset((DRAMC_CTX_T *) p, 0);        
        DramcCATraining_2(iFirstClkPass_1PLL, iLastClkPass_1PLL, iFirstCAPass_1PLL, iLastCAPass_1PLL, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2, &iMaxCenter);                
        DramcCATraining_1((DRAMC_CTX_T *) p, 0x66666666, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2);
        Dram_Reset((DRAMC_CTX_T *) p, 0);        
        DramcCATraining_2(iFirstClkPass_1PLL, iLastClkPass_1PLL, iFirstCAPass_1PLL, iLastCAPass_1PLL, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2, &iMaxCenter);        

        
        DramcCATraining_3((DRAMC_CTX_T *) p, iFirstClkPass_1PLL, iLastClkPass_1PLL, iFirstCAPass_1PLL, iLastCAPass_1PLL, iCAShift, 0);

        p->frequency = DUAL_FREQ_HIGH;
        DramcSwitchFreq(p);
        DramcInit((DRAMC_CTX_T *) p);

        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xb0), backup_reg_0xb0);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xb8), backup_reg_0xb8);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xbc), backup_reg_0xbc);  


        DramcCATraining_1((DRAMC_CTX_T *) p, 0x55555555, iFirstClkPass_3PLL, iLastClkPass_3PLL, iFirstCAPass_3PLL, iLastCAPass_3PLL);
        Dram_Reset((DRAMC_CTX_T *) p, 0);
        DramcCATraining_1((DRAMC_CTX_T *) p, 0x99999999, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2);
        Dram_Reset((DRAMC_CTX_T *) p, 0);        
        DramcCATraining_2(iFirstClkPass_3PLL, iLastClkPass_3PLL, iFirstCAPass_3PLL, iLastCAPass_3PLL, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2, &iMaxCenter);
        DramcCATraining_1((DRAMC_CTX_T *) p, 0xaaaaaaaa, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2);
        Dram_Reset((DRAMC_CTX_T *) p, 0);        
        DramcCATraining_2(iFirstClkPass_3PLL, iLastClkPass_3PLL, iFirstCAPass_3PLL, iLastCAPass_3PLL, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2, &iMaxCenter);                
        DramcCATraining_1((DRAMC_CTX_T *) p, 0x66666666, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2);
        Dram_Reset((DRAMC_CTX_T *) p, 0);        
        DramcCATraining_2(iFirstClkPass_3PLL, iLastClkPass_3PLL, iFirstCAPass_3PLL, iLastCAPass_3PLL, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2, &iMaxCenter);                


        DramcCATraining_4((DRAMC_CTX_T *) p, iFirstClkPass_1PLL, iLastClkPass_1PLL, iFirstCAPass_1PLL, iLastCAPass_1PLL, iCAShift, iFirstClkPass_3PLL, iLastClkPass_3PLL, iFirstCAPass_3PLL, iLastCAPass_3PLL, 1);


}
else
{
    DramcCATraining_1((DRAMC_CTX_T *) p, 0x55555555, iFirstClkPass_3PLL, iLastClkPass_3PLL, iFirstCAPass_3PLL, iLastCAPass_3PLL);
    Dram_Reset((DRAMC_CTX_T *) p, 0);
    DramcCATraining_1((DRAMC_CTX_T *) p, 0x99999999, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2);
    Dram_Reset((DRAMC_CTX_T *) p, 0);        
    DramcCATraining_2(iFirstClkPass_3PLL, iLastClkPass_3PLL, iFirstCAPass_3PLL, iLastCAPass_3PLL, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2, &iMaxCenter);
    DramcCATraining_1((DRAMC_CTX_T *) p, 0xaaaaaaaa, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2);
    Dram_Reset((DRAMC_CTX_T *) p, 0);        
    DramcCATraining_2(iFirstClkPass_3PLL, iLastClkPass_3PLL, iFirstCAPass_3PLL, iLastCAPass_3PLL, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2, &iMaxCenter);                
    DramcCATraining_1((DRAMC_CTX_T *) p, 0x66666666, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2);
    Dram_Reset((DRAMC_CTX_T *) p, 0);        
    DramcCATraining_2(iFirstClkPass_3PLL, iLastClkPass_3PLL, iFirstCAPass_3PLL, iLastCAPass_3PLL, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2, &iMaxCenter);                

    DramcCATraining_3((DRAMC_CTX_T *) p, iFirstClkPass_3PLL, iLastClkPass_3PLL, iFirstCAPass_3PLL, iLastCAPass_3PLL, iCAShift, 1);

}
#else
    DramcCATraining_1((DRAMC_CTX_T *) p, 0x55555555, iFirstClkPass_3PLL, iLastClkPass_3PLL, iFirstCAPass_3PLL, iLastCAPass_3PLL);
    Dram_Reset((DRAMC_CTX_T *) p, 0);
    DramcCATraining_1((DRAMC_CTX_T *) p, 0x99999999, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2);
    Dram_Reset((DRAMC_CTX_T *) p, 0);        
    DramcCATraining_2(iFirstClkPass_3PLL, iLastClkPass_3PLL, iFirstCAPass_3PLL, iLastCAPass_3PLL, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2, &iMaxCenter);
    DramcCATraining_1((DRAMC_CTX_T *) p, 0xaaaaaaaa, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2);
    Dram_Reset((DRAMC_CTX_T *) p, 0);        
    DramcCATraining_2(iFirstClkPass_3PLL, iLastClkPass_3PLL, iFirstCAPass_3PLL, iLastCAPass_3PLL, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2, &iMaxCenter);                
    DramcCATraining_1((DRAMC_CTX_T *) p, 0x66666666, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2);
    Dram_Reset((DRAMC_CTX_T *) p, 0);        
    DramcCATraining_2(iFirstClkPass_3PLL, iLastClkPass_3PLL, iFirstCAPass_3PLL, iLastCAPass_3PLL, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2, &iMaxCenter);        


    DramcCATraining_3((DRAMC_CTX_T *) p, iFirstClkPass_3PLL, iLastClkPass_3PLL, iFirstCAPass_3PLL, iLastCAPass_3PLL, iCAShift, 1);    
#endif

                DramcWriteLeveling((DRAMC_CTX_T *) p);
				Dram_Reset((DRAMC_CTX_T *) p, 0);
              }
              else
              {
                if (gIs_ECO_IC == 1)
                {
                    DramcCATraining_1((DRAMC_CTX_T *) p, 0x55555555, iFirstClkPass_1PLL, iLastClkPass_1PLL, iFirstCAPass_1PLL, iLastCAPass_1PLL);
                    Dram_Reset((DRAMC_CTX_T *) p, 0);
                    DramcCATraining_1((DRAMC_CTX_T *) p, 0x99999999, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2);
                    Dram_Reset((DRAMC_CTX_T *) p, 0);        
                    DramcCATraining_2(iFirstClkPass_1PLL, iLastClkPass_1PLL, iFirstCAPass_1PLL, iLastCAPass_1PLL, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2, &iMaxCenter);
                    DramcCATraining_1((DRAMC_CTX_T *) p, 0xaaaaaaaa, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2);
                    Dram_Reset((DRAMC_CTX_T *) p, 0);        
                    DramcCATraining_2(iFirstClkPass_1PLL, iLastClkPass_1PLL, iFirstCAPass_1PLL, iLastCAPass_1PLL, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2, &iMaxCenter);                
                    DramcCATraining_1((DRAMC_CTX_T *) p, 0x66666666, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2);
                    Dram_Reset((DRAMC_CTX_T *) p, 0);        
                    DramcCATraining_2(iFirstClkPass_1PLL, iLastClkPass_1PLL, iFirstCAPass_1PLL, iLastCAPass_1PLL, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2, &iMaxCenter);        

                    
                    DramcCATraining_3((DRAMC_CTX_T *) p, iFirstClkPass_1PLL, iLastClkPass_1PLL, iFirstCAPass_1PLL, iLastCAPass_1PLL, iCAShift, 1);                
                }
              }
} 
else 
{
//#else   //DUAL_FREQ_K
            DramcCATraining_1((DRAMC_CTX_T *) p, 0x55555555, iFirstClkPass_1PLL, iLastClkPass_1PLL, iFirstCAPass_1PLL, iLastCAPass_1PLL);
            Dram_Reset((DRAMC_CTX_T *) p, 0);
            DramcCATraining_1((DRAMC_CTX_T *) p, 0x99999999, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2);
            Dram_Reset((DRAMC_CTX_T *) p, 0);        
            DramcCATraining_2(iFirstClkPass_1PLL, iLastClkPass_1PLL, iFirstCAPass_1PLL, iLastCAPass_1PLL, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2, &iMaxCenter);
            DramcCATraining_1((DRAMC_CTX_T *) p, 0xaaaaaaaa, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2);
            Dram_Reset((DRAMC_CTX_T *) p, 0);        
            DramcCATraining_2(iFirstClkPass_1PLL, iLastClkPass_1PLL, iFirstCAPass_1PLL, iLastCAPass_1PLL, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2, &iMaxCenter);                
            DramcCATraining_1((DRAMC_CTX_T *) p, 0x66666666, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2);
            Dram_Reset((DRAMC_CTX_T *) p, 0);        
            DramcCATraining_2(iFirstClkPass_1PLL, iLastClkPass_1PLL, iFirstCAPass_1PLL, iLastCAPass_1PLL, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2, &iMaxCenter);        
            
            
            DramcCATraining_3((DRAMC_CTX_T *) p, iFirstClkPass_1PLL, iLastClkPass_1PLL, iFirstCAPass_1PLL, iLastCAPass_1PLL, iCAShift, 1);                
            
            DramcWriteLeveling((DRAMC_CTX_T *) p);
            Dram_Reset((DRAMC_CTX_T *) p, 0);
//#endif  // DUAL_FREQ_K
}
    }   

  
#ifdef DUAL_RANKS
  if (uiDualRank) 
  {
    DualRankDramcRxdqsGatingCal((DRAMC_CTX_T *) p);
  }
  else
  {
#ifdef USE_COARSE_TUNE_ALIGN_UPDATE
    DramcRxdqsGatingCal((DRAMC_CTX_T *) p, 0xff); 
#else
  DramcRxdqsGatingCal((DRAMC_CTX_T *) p); 
#endif
  }
#else
#ifdef USE_COARSE_TUNE_ALIGN_UPDATE
  DramcRxdqsGatingCal((DRAMC_CTX_T *) p, 0xff); 
#else
  DramcRxdqsGatingCal((DRAMC_CTX_T *) p); 
#endif
#endif

  
  if (((DRAMC_CTX_T *) p)->fglow_freq_write_en==ENABLE)
  {
    mcSHOW_DBG_MSG(("**********************NOTICE*************************\n"));
    mcSHOW_DBG_MSG(("Low speed write and high speed read calibration...\n"));
    mcSHOW_DBG_MSG(("*****************************************************\n"));
    // change low frequency and use test engine2 to write data, after write, recover back to the original frequency   
    CurrentRank = 0;
    DramcLowFreqWrite((DRAMC_CTX_T *) p);
    if (uiDualRank) 
    {
      CurrentRank = 1;
      // Swap CS0 and CS1.
      ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x110), &u4value);
      u4value = u4value |0x08;
      ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x110), u4value);

      // do channel A & B low frequency write simultaneously
      DramcLowFreqWrite((DRAMC_CTX_T *) p); 

      // Swap CS back.
      ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x110), &u4value);
      u4value = u4value & (~0x08);
      ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x110), u4value);
      CurrentRank = 0;
    }
  }   

  if (uiDualRank) 
  {
    DramcDualRankRxdatlatCal((DRAMC_CTX_T *) p);
  }
  else
  {

    if (DramcRxdatlatCal((DRAMC_CTX_T *) p, fcDATLAT_USE_TXRX_DEFAULT))
    {
        if (DramcRxdatlatCal((DRAMC_CTX_T *) p, fcDATLAT_USE_RX_SCAN))
        {
            DramcRxdatlatCal((DRAMC_CTX_T *) p, fcDATLAT_USE_TXRX_SCAN);
        }
    }
  }
  
#ifdef RX_DUTY_CALIBRATION
  //#ifdef DUAL_FREQ_K
  if (gOnly_09V_1PLL_Flow_Flag == 0)
  {
  if (p->frequency != DUAL_FREQ_LOW)   
  {
    DramcClkDutyCal(p);
  }
  }
  else
  {
  //#else
  DramcClkDutyCal(p);
  //#endif
  }
#endif

#ifdef ENABLE_DQSI_PER_RANK
  if (uiDualRank) 
  {
    // Rank 1
    mcSHOW_DBG_MSG(("Rank 1 RX calibration\n"));
    DramcRxWindowPerbitCal_RK1  ((DRAMC_CTX_T *) p);  
    //DramcRxRegPrint((DRAMC_CTX_T *) p);
  }
#endif
  // Rank 0
  mcSHOW_DBG_MSG(("Rank 0 RX calibration\n"));
  DramcRxWindowPerbitCal((DRAMC_CTX_T *) p);

#ifdef DQM_CALIBRATION
  DQMSkew[0] = DQMSkew[1] = DQMSkew[2] = DQMSkew[3] = 0;
  DramcTxWindowPerbitCal((DRAMC_CTX_T *) p);
  DramcDQMCal(p);
  DramcTxWindowPerbitCal((DRAMC_CTX_T *) p);  
#else
  mcSHOW_DBG_MSG(("TX calibration\n"));
  DramcTxWindowPerbitCal((DRAMC_CTX_T *) p);
#endif

#ifdef TDP_TDN_CAL
  DramcClkTDNTDPCal(p);
  DramcDQSTDNTDPCal(p);
#endif

  //SW workaround for DFS hang issue on single rank memory device
  ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x110), &u4value);
  ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x110), u4value | 0x1);

  //Put RANKINCTL setting
  DramcOtherSettingAfterCalibration(p);


//#ifdef DUAL_FREQ_K
if (gOnly_09V_1PLL_Flow_Flag == 0)
{
  print_DBG_info(p);
        
  if (p->frequency != DUAL_FREQ_LOW)   
  {
    DramcSaveToBackup(p);

#ifdef ONEPLL_TEST
#ifdef Vcore_ADJ_FOR_DVT
        /* change vcore to 0.9V */ 
        pmic_config_interface(0x608,0x30,0x7f,0); 
        pmic_config_interface(0x60A,0x30,0x7f,0); 
#endif
#endif

    p->frequency = DUAL_FREQ_LOW;
    DramcSwitchFreq(p);

// #if defined(DUAL_FREQ_DIFF_ACTIMING) || defined(DUAL_FREQ_DIFF_RLWL)
//    #if fcFOR_CHIP_ID == fcK2 || fcFOR_CHIP_ID == fcJADE
    DramcInit((DRAMC_CTX_T *) p);   
//    #endif
// #endif
    goto DDR_CALI_START;
  } 
  else
  {
#if fcFOR_CHIP_ID == fcK2 || fcFOR_CHIP_ID == fcJADE
    DramcSaveToShuffleReg(p); // Now is in low frequency with normal registers. Save to shuffle.
    p->frequency = DUAL_FREQ_LOW; // Switch to low frequency with shuffle registers.
    //DramcDFS(p, 1);
    DramcShuffle(p);
  
#ifdef SPM_WORKAROUND_CATRAINING
    DramcRestoreDual_Freq_High_CATraining_Reg();
#endif

    DramcRestoreBackup(p);      // Restore back high frequency registers to normal.

#ifndef ONEPLL_TEST
#if 0
#ifdef Vcore_ADJ_FOR_DVT
        /* change vcore to 1.0V */ 
        pmic_config_interface(0x608,0x40,0x7f,0); 
        pmic_config_interface(0x60A,0x40,0x7f,0); 
#endif
#endif
        p->frequency = mt_get_freq_setting(p);  // Switch to high frequency with normal registers
        DramcDFS(p, 1);
#endif    
#endif
  }
   
//#endif
}

//read MR5 and MR6
	{
		unsigned int Vendor_ID, Revision_ID;
		Vendor_ID = ModeRegRead(0, 5);
		mcSHOW_DBG_MSG(("Vendor ID = 0x%x\n",Vendor_ID));
		Revision_ID = ModeRegRead(0, 6);
		mcSHOW_DBG_MSG(("Revision ID = 0x%x\n",Revision_ID));
		
		if (Vendor_ID == 0x6 && Revision_ID== 0x4)
		{
				//Reset DRAM and set Hynix test mode to set MR4 refresh cnt
				Dram_Reset((DRAMC_CTX_T *) p, 1);		
		}
	}

//#ifdef DUAL_FREQ_K
    if (gOnly_09V_1PLL_Flow_Flag == 0) DramcDumpFreqSetting(p);
//#endif

#if defined(DDR_INIT_TIME_PROFILING)
  /* get CPU cycle count from the ARM CPU PMU */
  asm volatile(
      "MRC p15, 0, %0, c9, c12, 0\n"
      "BIC %0, %0, #1 << 0\n"   /* disable */
      "MCR p15, 0, %0, c9, c12, 0\n"
      "MRC p15, 0, %0, c9, c13, 0\n"
      : "+r"(temp)
      :
      : "cc"
  );
  mcSHOW_DBG_MSG(("DRAMC calibration takes %d CPU cycles\n\r", temp));
#endif

  DramcHWGWEnable((DRAMC_CTX_T *) p);
  DramcRunTimeConfig((DRAMC_CTX_T *) p);

#if 0
#if fcFOR_CHIP_ID == fcJADE
    //Disable this reduce power feature of JADE because has problem in 3PLL per-bank refresh
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xc4), &u4value);
    mcCLR_BIT(u4value, 4);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xc4), u4value);	
#endif
#endif
#ifdef DISABLE_AUTO_SAVE
ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xf8), &u4value);
mcCLR_BIT(u4value, 12);
mcSET_BIT(u4value, 13);
mcSET_BIT(u4value, 14);
mcSET_BIT(u4value, 15);
ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf8), u4value);
#endif

#ifdef DRAMC_DRAMPHY_DCM_ENABLE
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1dc), &u4value);
		mcSET_BIT(u4value, 30);
		mcSET_BIT(u4value, 31);
#if fcFOR_CHIP_ID == fcJADE
		mcSET_BIT(u4value, 25);
		mcSET_BIT(u4value, 1);
#endif
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1dc), u4value);
#endif

#ifdef DRAMC_DRAMPHY_DCM_ENABLE
	  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x828), &u4value);
		  mcSET_BIT(u4value, 30);
		  mcSET_BIT(u4value, 31);
#if fcFOR_CHIP_ID == fcJADE
		  mcSET_BIT(u4value, 25);
		  mcSET_BIT(u4value, 1);
#endif
	  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x828), u4value);
#endif
	  
//#if defined(ONEPLL_TEST) || (SINGLE_FREQ_MODE == 1)
if (gOnly_09V_1PLL_Flow_Flag == 1)
{
	//  0x1000f644[8], 1'b0: off; 1'b1: on 
	  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x644), &u4value);
	mcCLR_BIT(u4value, 8);
	  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x644), u4value);	  

	// mempll234_en close, power down
		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x614), &u4value);
		mcCLR_BIT(u4value, 0);	// Clr [0]:RG_MEMPLL2_EN, [23]:MEMPLL2_AUTOK_EN
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), u4value);						

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x62c), &u4value);
		mcCLR_BIT(u4value, 0);	// Clr [0]:RG_MEMPLL4_EN, [23]:MEMPLL4_AUTOK_EN
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62c), u4value);						

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x620), &u4value);
		mcCLR_BIT(u4value, 0);	// Clr [0]:RG_MEMPLL3_EN, [23]:MEMPLL3_AUTOK_EN
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), u4value);		

#if !(__ETT__)  
	// [12] spm2dramc_shuffle_switch, [14] spm2dpy_1pll_switch 
    Reg_Sync_Writel((SPM_POWER_ON_VAL0), Reg_Readl((SPM_POWER_ON_VAL0)) | (0x1 << 12) | (0x1 << 14)|(0x00410000));		
#endif
}
else
{
//#else
#if !(__ETT__)
	// [12] spm2dramc_shuffle_switch, [14] spm2dpy_1pll_switch 
	Reg_Sync_Writel((SPM_POWER_ON_VAL0), Reg_Readl((SPM_POWER_ON_VAL0)) & ~((0x1 << 12) | (0x1 << 14)));	  
#endif
//#endif
}

#ifdef SPM_WORKAROUND_CATRAINING   
    Reg_Sync_Writel((0x10003204), Reg_Readl(0x10003204) & (~(1<<6)));   //Force SPM early write response command
#endif

	//Dummy Pad Power Down Disable
ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x63c), &u4value);
mcSET_BIT(u4value, 3);
ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x63c), u4value);


//Lewis@20150820: ETT will not use TransferToSPMControl() except low power scenario test code
#if !(__ETT__)
	TransferToSPMControl();
#endif
  
#ifdef RANK1_TEST
	// Swap CS0 and CS1.
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x110), &u4value);
	u4value = u4value |0x08;
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x110), u4value);
#endif
#ifdef DR_QUICKENDTEST
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xd8), &u4value);
	mcSET_BIT(u4value, 10);
	mcSET_BIT(u4value, 11);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xd8), u4value);
#endif  


  mcSHOW_DBG_MSG(("\n\nSettings after calibration ...\n\n"));
        print_DBG_info(p);
        Dump_EMIRegisters(p); 
        
#ifdef EYE_SCAN
  mcSHOW_DBG_MSG(("Channel A eye scan.\n\n"));
  DramcEyeScan(p);
        while(1);
#endif

#ifdef TUNE_RX_TX_FACTORS  
  { 
      int i;
      for(i=1; i<5; i++)
          mcSHOW_DBG_MSG(("\n\nETT Done ...\n\n"));
  }
       while(1);
#endif

    //enable MR4 here to avoid tDQSCK viralate too large
    DramcEnableTemperatureSensorMR4(p);
    return 0;
}
