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
/*-----------------------------------------------------------------------------
 *
 * $Author: jc.wu $
 * $Date: 2012/6/5 $
 * $RCSfile: pi_basic_api.c,v $
 * $Revision: #5 $
 *
 *---------------------------------------------------------------------------*/

/** @file dramc_pi_basic_api.c
 *  Basic DRAMC API implementation
 */

//=============================================================================
//	Include Files                                                      
//=============================================================================
#include "dramc_common.h"
#include "dramc_register.h"
#include "dramc_pi_api.h"
#if __ETT__
#include "common.h"
#include "custom_emi.h"
#endif

//=============================================================================
//	Definition                                                         
//=============================================================================
#ifdef XTALK_SSO_STRESS
// only for SW worst pattern
const static U32 u4xtalk_pat[76] = { \
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, \
0x01010101, 0x02020202, 0x04040404, 0x08080808, 0x10101010, 0x20202020, 0x40404040, 0x80808080, \
0x00000000, 0x00000000, 0xffffffff, 0xffffffff, \
0xfefefefe, 0xfdfdfdfd, 0xfbfbfbfb, 0xf7f7f7f7, 0xefefefef, 0xdfdfdfdf, 0xbfbfbfbf, 0x7f7f7f7f, \
0xffffffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, \
0xfefefefe, 0x01010101, 0xfdfdfdfd, 0x02020202, 0xfbfbfbfb, 0x04040404, 0xf7f7f7f7, 0x08080808, \
0xefefefef, 0x10101010, 0xdfdfdfdf, 0x20202020, 0xbfbfbfbf, 0x40404040, 0x7f7f7f7f, 0x80808080, \
0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, \
0xfefefefe, 0x01010101, 0xfdfdfdfd, 0x02020202, 0xfbfbfbfb, 0x04040404, 0xf7f7f7f7, 0x08080808, \
0xefefefef, 0x10101010, 0xdfdfdfdf, 0x20202020, 0xbfbfbfbf, 0x40404040, 0x7f7f7f7f, 0x80808080, \
0xffffffff, 0x00000000, 0xffffffff, 0xffffffff, 0xffffffff, \
};

const static U32 u4xtalk_pat_64[76*2] = { \
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, \
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, \
0x01010101, 0x01010101, 0x02020202, 0x02020202, 0x04040404, 0x04040404, 0x08080808, 0x08080808,\
0x10101010, 0x10101010, 0x20202020, 0x20202020, 0x40404040, 0x40404040, 0x80808080, 0x80808080,\
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,\
0xfefefefe, 0xfefefefe, 0xfdfdfdfd, 0xfdfdfdfd, 0xfbfbfbfb, 0xfbfbfbfb, 0xf7f7f7f7, 0xf7f7f7f7,\
0xefefefef, 0xefefefef, 0xdfdfdfdf, 0xdfdfdfdf, 0xbfbfbfbf, 0xbfbfbfbf, 0x7f7f7f7f, 0x7f7f7f7f,\
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000,\
0x00000000, 0x00000000, \
0xfefefefe, 0xfefefefe, 0x01010101, 0x01010101, 0xfdfdfdfd, 0xfdfdfdfd, 0x02020202, 0x02020202,\
0xfbfbfbfb, 0xfbfbfbfb, 0x04040404, 0x04040404, 0xf7f7f7f7, 0xf7f7f7f7, 0x08080808, 0x08080808,\
0xefefefef, 0xefefefef, 0x10101010, 0x10101010, 0xdfdfdfdf, 0xdfdfdfdf, 0x20202020, 0x20202020,\
0xbfbfbfbf, 0xbfbfbfbf, 0x40404040, 0x40404040, 0x7f7f7f7f, 0x7f7f7f7f, 0x80808080, 0x80808080,\
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0x00000000,\
0x00000000, 0x00000000, 0x00000000, 0x00000000, \
0xfefefefe, 0xfefefefe, 0x01010101, 0x01010101, 0xfdfdfdfd, 0xfdfdfdfd, 0x02020202, 0x02020202,\
0xfbfbfbfb, 0xfbfbfbfb, 0x04040404, 0x04040404, 0xf7f7f7f7, 0xf7f7f7f7, 0x08080808, 0x08080808,\
0xefefefef, 0xefefefef, 0x10101010, 0x10101010, 0xdfdfdfdf, 0xdfdfdfdf, 0x20202020, 0x20202020,\
0xbfbfbfbf, 0xbfbfbfbf, 0x40404040, 0x40404040, 0x7f7f7f7f, 0x7f7f7f7f, 0x80808080, 0x80808080,\
0xffffffff, 0xffffffff, 0x00000000, 0x00000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, \
0xffffffff, 0xffffffff,\
};

const static U32 u4spb0x[64] = { \
0x00000000, 0x00000000, 0x00000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, \
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0x00000000, \
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0xffffffff, 0xffffffff, \
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0xffffffff, 0xffffffff, 0xffffffff, \
0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, \
0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, \
0xffffffff, 0x00000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, \
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0xffffffff, 0xffffffff, \
};

const static U32 u4spb0x_64[64*2] = { \
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xffffffff, 0xffffffff,\
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0x00000000,\
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,\
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000,\
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,\
0xffffffff, 0xffffffff, 0x00000000, 0x00000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,\
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,\
0x00000000, 0x00000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,\
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0x00000000,\
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,\
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0x00000000,\
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,\
0xffffffff, 0xffffffff, 0x00000000, 0x00000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,\
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0x00000000,\
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,\
0xffffffff, 0xffffffff, 0x00000000, 0x00000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,\
};
#endif






//=============================================================================
//	Global Variables                                                  
//=============================================================================
// num of "1", num of "0", repeat, 
// num of "1", num of "0" (at the end)
static U8 ucsso_bsx[6][5] = { \
{1, 2, 7, 0, 1}, \
{2, 1, 15,0, 2}, \
{3, 2, 7, 1, 0}, \
{2, 3, 7, 1, 0}, \
{4, 3, 7, 1, 0}, \
{3, 4, 7, 0, 0}\
};

unsigned int uiDualRank = 1;

U8 VCOK_Cha_Mempll2, VCOK_Cha_Mempll3, VCOK_Cha_Mempll4;
U8 VCOK_Chb_Mempll2, VCOK_Chb_Mempll3, VCOK_Chb_Mempll4;
U8 VCOK_05PHY_Mempll2, VCOK_05PHY_Mempll3, VCOK_05PHY_Mempll4;

U32 new_DMYPAD_option = 0;
U32 gENABLE_MEMPLL_CG_TO_SAVE_POWER_val = 0;

//=============================================================================
//	External references                                                
//=============================================================================
extern const U32 uiLPDDR3_PHY_Mapping_POP_CHA[32];


extern U8 opt_gw_coarse_value_R0[2], opt_gw_fine_value_R0[2];
extern U8 opt_gw_coarse_value_R1[2], opt_gw_fine_value_R1[2];

#ifdef SPM_WORKAROUND_CATRAINING
extern U32 DUAL_FREQ_HIGH_reg_00c, DUAL_FREQ_LOW_reg_00c;
#endif

void Reg_Sync_Writel(unsigned int addr,unsigned int val);
unsigned int Reg_Readl(unsigned int addr);







#if fcFOR_CHIP_ID == fcK2 || fcFOR_CHIP_ID == fcJADE

//-------------------------------------------------------------------------
/** ucDramC_Register_Read
 *  DRAMC register read (32-bit).
 *  @param  u4reg_addr    register address in 32-bit.
 *  @param  pu4reg_value  Pointer of register read value.
 *  @retval 0: OK, 1: FAIL 
 */
//-------------------------------------------------------------------------
// This function need to be porting by BU requirement
U8 ucDramC_Register_Read(U32 u4reg_addr, U32 *pu4reg_value)
{
    	U8 ucstatus = 0;

	*pu4reg_value = 	(*(volatile unsigned int *)(CHA_DRAMCAO_BASE + (u4reg_addr)));
	return ucstatus;
}

//-------------------------------------------------------------------------
/** ucDramC_Register_Write
 *  DRAMC register write (32-bit).
 *  @param  u4reg_addr    register address in 32-bit.
 *  @param  u4reg_value   register write value.
 *  @retval 0: OK, 1: FAIL 
 */
//-------------------------------------------------------------------------
// This function need to be porting by BU requirement
U8 ucDramC_Register_Write(U32 u4reg_addr, U32 u4reg_value)
{
	U8 ucstatus = 0;

	(*(volatile unsigned int *)(CHA_DRAMCAO_BASE + (u4reg_addr))) = u4reg_value;
	dsb();
    
	return ucstatus;
}

//-------------------------------------------------------------------------
/** ucDram_Register_Read
 *  DRAM register read (32-bit).
 *  @param  u4reg_addr    register address in 32-bit.
 *  @param  pu4reg_value  Pointer of register read value.
 *  @retval 0: OK, 1: FAIL 
 */
//-------------------------------------------------------------------------
// This function need to be porting by BU requirement
U8 ucDram_Register_Read(U32 u4reg_addr, U32 *pu4reg_value)
{
	U8 ucstatus = 0;

   	*pu4reg_value = 	(*(volatile unsigned int *)(CHA_DRAMCAO_BASE + (u4reg_addr))) |
					(*(volatile unsigned int *)(CHA_DDRPHY_BASE + (u4reg_addr))) |
					(*(volatile unsigned int *)(CHA_DRAMCNAO_BASE + (u4reg_addr)));
	return ucstatus;
}

//-------------------------------------------------------------------------
/** ucDram_Register_Write
 *  DRAM register write (32-bit).
 *  @param  u4reg_addr    register address in 32-bit.
 *  @param  u4reg_value   register write value.
 *  @retval 0: OK, 1: FAIL 
 */
//-------------------------------------------------------------------------
// This function need to be porting by BU requirement
U8 ucDram_Register_Write(U32 u4reg_addr, U32 u4reg_value)
{
	U8 ucstatus = 0;

	(*(volatile unsigned int *)(CHA_DRAMCAO_BASE + (u4reg_addr))) = u4reg_value;
	(*(volatile unsigned int *)(CHA_DDRPHY_BASE + (u4reg_addr))) = u4reg_value;
	(*(volatile unsigned int *)(CHA_DRAMCNAO_BASE + (u4reg_addr))) = u4reg_value;
	dsb();
    
	return ucstatus;
}

#endif

//-------------------------------------------------------------------------
/** ucDram_Read
 *  DRAM memory read (32-bit).
 *  @param  u4mem_addr    memory address in 32-bit.
 *  @param  pu4mem_value  Pointer of memory read value.
 *  @retval 0: OK, 1: FAIL 
 */
//-------------------------------------------------------------------------
// This function need to be porting by BU requirement
U8 ucDram_Read(U32 u4mem_addr, U32 *pu4mem_value)
{
U8 ucstatus = 0;

    *pu4mem_value = 	(*(volatile unsigned int *)(DDR_BASE + (u4mem_addr))) ;

    return ucstatus;
}

//-------------------------------------------------------------------------
/** ucDram_Write
 *  DRAM memory write (32-bit).
 *  @param  u4mem_addr    register address in 32-bit.
 *  @param  u4mem_value   register write value.
 *  @retval 0: OK, 1: FAIL 
 */
//-------------------------------------------------------------------------
// This function need to be porting by BU requirement
U8 ucDram_Write(U32 u4mem_addr, U32 u4mem_value)
{
U8 ucstatus = 0;

    (*(volatile unsigned int *)(DDR_BASE + (u4mem_addr))) = u4mem_value;
    
    return ucstatus;
}

//-------------------------------------------------------------------------
/** Round_Operation
 *  Round operation of A/B
 *  @param  A   
 *  @param  B   
 *  @retval round(A/B) 
 */
//-------------------------------------------------------------------------
U32 Round_Operation(U32 A, U32 B)
{
    U32 temp;

    if (B == 0)
    {
        return 0xffffffff;
    }
    
    temp = A/B;
        
    if ((A-temp*B) >= ((temp+1)*B-A))
    {
        return (temp+1);
    }
    else
    {
        return temp;
    }    
}


#ifdef MRW_WAIT_RESPONSE
void MRW_Wait_Response(void)
{
	U32 u4value;

	ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x3b8), &u4value);
	while((u4value&1)==0)
	{
		ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x3b8), &u4value);	//[0] MRW command response
	}

}
#endif

void Global_Option_Init(void)
{
    U8 ucstatus = 0;

#if fcFOR_CHIP_ID == fcJADE
			// enable new DMYPAD option to prevent IO glitches
			//Reg0x698 bit[14]
#ifdef DDRPHY_BYPASS_NEW_DMYPAD
			new_DMYPAD_option = 0;
#else
			new_DMYPAD_option = 1<<14;
#endif
	
#ifdef ENABLE_MEMPLL_CG_TO_SAVE_POWER
			gENABLE_MEMPLL_CG_TO_SAVE_POWER_val = 1<<16;
#else
			gENABLE_MEMPLL_CG_TO_SAVE_POWER_val = 0;
#endif
#endif


	// RX DQ, DQS input delay
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x210), 0x00000001);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x214), 0x00000300);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x218), 0x00020302);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x21c), 0x01010501);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x220), 0x02030103);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x224), 0x02010200);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x228), 0x01020101);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x22c), 0x02000300);

	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x18), 0x07080806);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1c), 0x07080806);
	
}

#ifdef ONE_PLL_MODE_INIT
DRAM_STATUS_T MemPllInit(DRAMC_CTX_T *p)
{
	U8 ucstatus = 0;
	U32 u4value;
	U32 MEMPLL_N_INFO, MEMPLL_DIV;
	U32 MEMPLLx_FBDIV, MEMPLLx_FBDIV_0, MEMPLLx_BR, MEMPLLx_BC, MEMPLLx_IR, MEMPLLx_IC, MEMPLLx_BP, MEMPLLx_M4PDIV;
    int dram_type;
    EMI_SETTINGS *emi_set;
    
#if fcFOR_CHIP_ID == fcJADE
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x600), 0xd5200000);  // set to default value, bit[31] set to 1 to toggle INFO_CHG
#endif
    
    
    if(emi_setting_index == -1)   
    {
        emi_set = default_emi_setting;
    }
    else
    {
        emi_set = &emi_settings[emi_setting_index];
    }                              


#ifndef LOCK_ON_1PLL_3PLL
//According to T28HPM_LJ_MEM_PLL_SettingWizard_Jade.xls
    if(p->frequency == 933) //DDR_1866
    {
    	MEMPLL_N_INFO = 0x6fa4158d;
    	MEMPLL_DIV = 0x38;
    	MEMPLLx_FBDIV = 0x08;
    	MEMPLLx_BR = 0x01;
    	MEMPLLx_BC = 0x00;
    	MEMPLLx_IR = 0x0d;
    	MEMPLLx_IC = 0x06;
    	MEMPLLx_BP = 0x01;
    	MEMPLLx_M4PDIV = 0x0;
    }
    else if(p->frequency == 890) //DDR_1780	
    {
    	MEMPLL_N_INFO = 0x7163b13b;
    	MEMPLL_DIV = 0x35;
    	MEMPLLx_FBDIV = 0x07;
    	MEMPLLx_BR = 0x01;
    	MEMPLLx_BC = 0x00;
    	MEMPLLx_IR = 0x0c;
    	MEMPLLx_IC = 0x06;
    	MEMPLLx_BP = 0x01;
    	MEMPLLx_M4PDIV = 0x0;
    }
    else if(p->frequency == 800)	//DDR_1600
    {
    	MEMPLL_N_INFO = 0x3d89d89d;
    	MEMPLL_DIV = 0x1c;
    	MEMPLLx_FBDIV = 0x06;
    	MEMPLLx_BR = 0x01;
    	MEMPLLx_BC = 0x01;
    	MEMPLLx_IR = 0x0a;
    	MEMPLLx_IC = 0x07;
    	MEMPLLx_BP = 0x01;
    	MEMPLLx_M4PDIV = 0x0;
    }
    else if(p->frequency == 666) //DDR_1333	
    {
    	MEMPLL_N_INFO = 0x3bd06906;
    	MEMPLL_DIV = 0x1c;
    	MEMPLLx_FBDIV = 0x05;
    	MEMPLLx_BR = 0x01;
    	MEMPLLx_BC = 0x01;
    	MEMPLLx_IR = 0x08;
    	MEMPLLx_IC = 0x06;
    	MEMPLLx_BP = 0x01;
    	MEMPLLx_M4PDIV = 0x0;
    }
    else if(p->frequency == 533)	//DDR_1066
    {
    	MEMPLL_N_INFO = 0x72cccccc;
    	MEMPLL_DIV = 0x38;
    	MEMPLLx_FBDIV = 0x04;
    	MEMPLLx_BR = 0x02;
    	MEMPLLx_BC = 0x02;
    	MEMPLLx_IR = 0x0d;
    	MEMPLLx_IC = 0x07;
    	MEMPLLx_BP = 0x02;
    	MEMPLLx_M4PDIV = 0x0;
    }	
#endif
#ifdef LOCK_ON_1PLL_3PLL	//Francis : for 3pll/1pll all use one setting, and this setting is according to 1pll. 
		//	1300/1800 setting
		MEMPLL_N_INFO = 0x6384ec4e;	//26*0x6384ec4e/2^24=2587.5	1pll:2587.5/4=646.76(1293MHz)	3pll:2587.5/0x2E=56.24(REF_INT), 56.25*2*(0x07+1)=900(1800MHz)
		MEMPLL_DIV = 0x2E;
    	MEMPLLx_FBDIV = 0x07;
    	MEMPLLx_BR = 0x02;
    	MEMPLLx_BC = 0x03;
    	MEMPLLx_IR = 0x0c;
    	MEMPLLx_IC = 0x07;
    	MEMPLLx_BP = 0x02;
    	MEMPLLx_M4PDIV = 0x0;	

#ifdef FIRST_BRING_UP
		//	1146/1066 setting
		MEMPLL_N_INFO = 0x58266666;	//26*0x58266666/2^24=2291.9	1pll:2291.9/4=573(1146MHz)	3pll:2291.9/0x2B=53.3(REF_INT), 53.3*2*(0x04+1)=533(1066MHz)
		MEMPLL_DIV = 0x2B;
    	MEMPLLx_FBDIV = 0x04;
    	MEMPLLx_BR = 0x02;
    	MEMPLLx_BC = 0x03;
    	MEMPLLx_IR = 0x07;
    	MEMPLLx_IC = 0x04;
    	MEMPLLx_BP = 0x02;
    	MEMPLLx_M4PDIV = 0x0;				
#endif		
#endif


#if fcFOR_CHIP_ID == fcJADE
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x000220b1);  //0x1000F640[13] = 1 -> ALL_CLK_ECO_EN = 1, [13]RG_E2_ECO_EN = 1, Mengru Si:reset mode phase error    
#endif


#if 1
//	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x09000000); 		
//	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x09000010); 		

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
	// RG_MEMPLLx_EN = 0, Reg.614h[0], Reg.620h[0], Reg.62ch[0]
	// RG_MEMPLLx_AUTOK_EN=1 [23]
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x614), &u4value);
	mcCLR_BIT(u4value, 0);
	mcSET_BIT(u4value, 23);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), u4value);
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x620), &u4value);
	mcCLR_BIT(u4value, 0);
	mcSET_BIT(u4value, 23);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), u4value);
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x62c), &u4value);
	mcCLR_BIT(u4value, 0);
	mcSET_BIT(u4value, 23);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62c), u4value);

	// RG_MEMPLLx_FB_MCK_SEL = 0, Reg.618h[25], Reg.624h[25], Reg.630h[25]
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x618), &u4value);
	mcCLR_BIT(u4value, 25);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), u4value);
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x624), &u4value);
	mcCLR_BIT(u4value, 25);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), u4value);
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x630), &u4value);
	mcCLR_BIT(u4value, 25);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), u4value);


	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), 0x07800000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set RG_MEMPLL2
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), 0x4c00c000 | (MEMPLLx_M4PDIV << 28)); // Set RG_MEMPLL2_RST_DLY[31:30]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x61C), 0x00020001 | (MEMPLLx_BR << 10) | (MEMPLLx_BC << 8) | (MEMPLLx_BP << 12)); // Set RG_MEMPLL2
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62C), 0x07800000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set RG_MEMPLL4
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), 0x4c00c000 | (MEMPLLx_M4PDIV << 28)); // Set RG_MEMPLL4_RST_DLY[31:30]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x634), 0x00000001 | (MEMPLLx_BR << 10) | (MEMPLLx_BC << 8) | (MEMPLLx_BP << 12)); // Set RG_MEMPLL4
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), 0x07800000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set RG_MEMPLL3
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), 0x4c00c000 | (MEMPLLx_M4PDIV << 28)); // Set RG_MEMPLL3_RST_DLY[31:30]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x628), 0x00020001 | (MEMPLLx_BR << 10) | (MEMPLLx_BC << 8) | (MEMPLLx_BP << 12)); // Set RG_MEMPLL3
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x698), 0x00001e00 | gENABLE_MEMPLL_CG_TO_SAVE_POWER_val); // 4xck = VCO/5, RG_MEMPLL_RSV[2]=1
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x63C), 0x00000000); 
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x688), 0x00000000); // Set RG_MEMPLL_REFCK_EN[0]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x638), 0x00000000); // Set RG_MEMPLL_REFCK_SEL to 1 by PLL1
#endif


	// --------------------------------------------------
	// MEMPLL configuration
	// --------------------------------------------------
	Reg_Sync_Writel((DFS_MEM_DCM_CTRL), (0x01 << 28)); //change clk_mux from 26M to 4X clock


	if (p->dram_type == TYPE_LPDDR3)
	{
		// LPDDR3
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x7c), emi_set->DRAMC_DDR2CTL_VAL); 

#if fcFOR_CHIP_ID == fcK2		
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), 0x00f00ae0);	// 114563ns, new DATLAT for TOP_PIPE, RX pipe [7:5]
#endif		
#if fcFOR_CHIP_ID == fcJADE
#ifndef JADE_FLOW_RUN_ON_K2_EVB
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), 0x006f0be0);	// new DATLAT for TOP_PIPE, RX pipe [7:5]
#else
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), 0x00f00ae0);	// new DATLAT for TOP_PIPE, RX pipe [7:5]
#endif
#endif
	}  

#if fcFOR_CHIP_ID == fcJADE
#ifdef DUAL_FREQ_DIFF_ACTIMING
			if (p->frequency == DUAL_FREQ_LOW)
			{
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xfc), LPDDR3_MISCTL0_VAL_0fc_LOW);	// 114563ns
			}
			else
			{
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xfc), emi_set->DRAMC_MISCTL0_VAL);	// 114563ns
			}
#else
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xfc), emi_set->DRAMC_MISCTL0_VAL);	// 114563ns
#endif

		//	  7: 0 rg_mempll_iso_en_stb_time
		//	 15: 8 rg_dmpll2_ck_en_stb_time
		//	 23:16 rg_mempll_div_en_stb_time
		//	 31:24 rg_mempll_ddsen_stb_time
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x5c4), 0x5096001e);	// r_dmpll2_ck_en_stb_time = 0, change 0x5096961e ==> 0x5096001e
#endif

#if fcFOR_CHIP_ID == fcK2
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x5c8), 0x1111ff11);	// [15:12] bypass delay chain for mempllx enable. Need to check??
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x5cc), 0xd1111111);	// by_dmpll2_ck_en_dly first, manual enable mempll, [12] TBD
#endif
#if fcFOR_CHIP_ID == fcJADE
	// Enable S/W control (not SPM)
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x5c8), 0x01011f01); // [9]:rg_mempll2_pwd_en_sel, [12]:by_mempll_pwd_dly
	// Enable S/W control (not SPM)
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x5cc), 0x41011101); // by_dmpll2_ck_en_dly first, manual enable mempll[12], by_mempll_div_en_dly[20] TBD
#endif

#if fcFOR_CHIP_ID == fcK2
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x000000b1); 		// Sync off.
	mcDELAY_US(1); 	//WAIT 100ns. DA_SDM_ISO_EN=0 -> 100ns -> RG_MEMPLL_BIAS_EN=1
#endif	
	
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x60c), 0xd0000040);	// 46857ns
	mcDELAY_US(2);	// RG_MEMPLL_BIAS_EN=1 -> 1us -> RG_MEMPLL_EN
#if fcFOR_CHIP_ID == fcJADE
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x60c), 0xd00000c0);	// [6]:RG_MEMPLL_BIAS_EN=1, RG_MEMPLL2/3/4_GATE_EN=1(RG_MEMPLL_RESERVE1[2][3][4]=0), [7] BIAS_LPF_EN =1
	mcDELAY_US(2);
#endif

	//ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x604), 0x10080000);	// 50472ns
	//mcDELAY_US(20);	// RG_MEMPLL_EN=1 -> 20us -> RG_MEMPLL_DIV_EN

#if fcFOR_CHIP_ID == fcK2
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x698), 0x00001e02);    // 4xck = VCO/5, RG_MEMPLL_RSV[2]=1
#endif	
#if fcFOR_CHIP_ID == fcJADE
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e0), 0x08000200);	// Rx78[9] = 1 (R_DM05XGEN) 
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x698), 0x000e9e30 | new_DMYPAD_option | gENABLE_MEMPLL_CG_TO_SAVE_POWER_val);    // [3:0]:4XCKDIVSEL=0(4xck = VCO/2), RG_MEMPLL_RSV=0000, [4]:RG_MEMPLL_ONEPLLSEL=0, fpc = 0, 
#endif	

	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x600), MEMPLL_N_INFO); // Set [30:0]:RG_MEMPLL_N_INFO = 102.564, [31]:CHG=0
#if fcFOR_CHIP_ID == fcJADE
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x600), MEMPLL_N_INFO|0x80000000); 	// Set [30:0]:RG_MEMPLL_N_INFO = 102.564, [31]:CHG=1
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x600), MEMPLL_N_INFO); 				// Set [30:0]:RG_MEMPLL_N_INFO = 102.564, [31]:CHG=0
#endif
	
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x610), 0x08000000 | (MEMPLL_DIV << 17)); // Set [23:17]:RG_MEMPLL_DIV = 7'd52, [16]:RG_MEMPLL_DIV_EN = 0, [31:24]:RG_MEMPLL_RESERVE[3]=1																			 						
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x638), 0x10000000); // Set RG_MEMPLL_REFCK_SEL to 1 by PLL1
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x604), 0x10080000); // Set RG_MEMPLL_EN = 1, RG_MEMPLL_SDM_DI_EN = 1
	mcDELAY_US(20);	// RG_MEMPLL_EN=1 -> 20us -> RG_MEMPLL_DIV_EN		
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x610), 0x08010000 | (MEMPLL_DIV << 17)); // Set RG_MEMPLL_DIV = 7'd56, RG_MEMPLL_DIV_EN = 1, RG_MEMPLL_RESERVE[3]=1
	mcDELAY_US(1);		// RG_MEMPLL_DIV_EN = 1 -> 1us -> RG_MEMPLLx_EN=1
	
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), 0x07800000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set RG_MEMPLL2
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), 0x4c00c000 | (MEMPLLx_M4PDIV << 28)); // Set RG_MEMPLL2_RST_DLY[31:30]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x61C), 0x00020001 | (MEMPLLx_BR << 10) | (MEMPLLx_BC << 8) | (MEMPLLx_BP << 12)); // Set RG_MEMPLL2
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62C), 0x07800000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set RG_MEMPLL4
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), 0x4c00c000 | (MEMPLLx_M4PDIV << 28)); // Set RG_MEMPLL4_RST_DLY[31:30]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x634), 0x00000001 | (MEMPLLx_BR << 10) | (MEMPLLx_BC << 8) | (MEMPLLx_BP << 12)); // Set RG_MEMPLL4
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), 0x07800000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set RG_MEMPLL3
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), 0x4c00c000 | (MEMPLLx_M4PDIV << 28)); // Set RG_MEMPLL3_RST_DLY[31:30]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x628), 0x00020001 | (MEMPLLx_BR << 10) | (MEMPLLx_BC << 8) | (MEMPLLx_BP << 12)); // Set RG_MEMPLL3

	// wait 100ns
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x688), 0x00000001); // Set RG_MEMPLL_REFCK_EN[0]

	mcDELAY_US(20);	// RG_MEMPLL_DIV_EN->1us->RG_MEMPLLx_EN

#if fcFOR_CHIP_ID == fcK2
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x60c), 0xd000005d); // Set RG_MEMPLL_BIAS_EN[6], RG_MEMPLL2/3/4_GATE_EN=1(RG_MEMPLL_RESERVE1[2][3][4]=1)
#endif	
#if fcFOR_CHIP_ID == fcJADE
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x60c), 0xd00000dd); // [7]:RG_MEMPLL_BIAS_LPF_EN=1, [6]:RG_MEMPLL_BIAS_EN=1, RG_MEMPLL2/3/4_GATE_EN=1(RG_MEMPLL_RESERVE1[2][3][4]=1)
#endif

	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), 0x07800000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // [23]:MEMPLL2_AUTOK_EN = 1, Clr RG_MEMPLL2_EN[0]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62C), 0x07800000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // [23]:MEMPLL4_AUTOK_EN = 1, Clr RG_MEMPLL4_EN[0]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), 0x07800000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // [23]:MEMPLL3_AUTOK_EN = 1, Clr RG_MEMPLL3_EN[0]

	mcDELAY_US(106);	// RG_MEMPLLx_EN -> wait 100us -> RG_MEMPLL_BIAS_LPF_EN

#if fcFOR_CHIP_ID == fcK2
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x60c), 0xd00000dd); // Set RG_MEMPLL_LPF_BIAS_EN[7], RG_MEMPLL2/3/4_GATE_EN=1(RG_MEMPLL_RESERVE1[2][3][4]=1)
#endif	

	mcDELAY_US(20);	//PLL ready (internal loop)

#ifdef MEMPLL_BAND_INFO
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x3e0), &u4value);
    	VCOK_Cha_Mempll2 = (u4value >> 16) & 0x7f;
	    VCOK_Cha_Mempll3 = (u4value >> 8) & 0x7f;
        mcSHOW_DBG_MSG3(("0x3e0=0x%x, VCOK_Cha_Mempll2=0x%x, VCOK_Cha_Mempll3=0x%x \n", 
        	u4value, VCOK_Cha_Mempll2, VCOK_Cha_Mempll3));

        // RG_MEMPLL_RESERVE[2]=1, to select MEMPLL4 band register
        // RGS_MEMPLL4_AUTOK_BAND[6:0]= RGS_MEMPLL4_AUTOK_BAND[6]+RGS_MEMPLL3_AUTOK_BAND[5:0]
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x610), &u4value);
        mcSET_BIT(u4value, 26);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x610), u4value);

        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x3e0), &u4value);
        VCOK_Cha_Mempll4 = ((u4value >> 8) & 0x3f) | (u4value & 0x40);
        mcSHOW_DBG_MSG3(("0x3e0=0x%x, VCOK_Cha_Mempll4=0x%x\n", u4value, VCOK_Cha_Mempll4));        

        // RG_MEMPLL_RESERVE[2]=0, recover back
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x610), &u4value);
        mcCLR_BIT(u4value, 26);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x610), u4value);        
#endif

#if fcFOR_CHIP_ID == fcK2
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), 0x07800000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set ~RG_MEMPLL2_EN[0] & RG_MEMPLL2_AUTOK_EN[23]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62C), 0x07800000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set ~RG_MEMPLL4_EN[0] & RG_MEMPLL4_AUTOK_EN[23]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), 0x07800000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set ~RG_MEMPLL3_EN[0] & RG_MEMPLL3_AUTOK_EN[23]

	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), 0x07000000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set ~RG_MEMPLL2_EN[0] & ~RG_MEMPLL2_AUTOK_EN[23]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62C), 0x07000000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set ~RG_MEMPLL4_EN[0] & ~RG_MEMPLL4_AUTOK_EN[23]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), 0x07000000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set ~RG_MEMPLL3_EN[0] & ~RG_MEMPLL3_AUTOK_EN[23]
#endif	

	mcDELAY_US(1);	// RG_MEMPLL_BIAS_LPF_EN -> 1us -> RG_MEMPLLx_FB_MCK_SEL

#if fcFOR_CHIP_ID == fcK2
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), 0x4e00c000 | (MEMPLLx_M4PDIV << 28)); // Set RG_MEMPLL2_FB_MCK_SEL[25]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), 0x4e00c000 | (MEMPLLx_M4PDIV << 28)); // Set RG_MEMPLL4_FB_MCK_SEL[25]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), 0x4e00c000 | (MEMPLLx_M4PDIV << 28)); // Set RG_MEMPLL3_FB_MCK_SEL[25]

	mcDELAY_US(1);	// RG_MEMPLLx_FB_MCK_SEL -> 100ns -> RG_MEMPLLx_EN 

	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), 0x07000001 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set RG_MEMPLL2_EN[0]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62C), 0x07000001 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set RG_MEMPLL4_EN[0]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), 0x07000001 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set RG_MEMPLL3_EN[0]	
#endif
#if fcFOR_CHIP_ID == fcJADE
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), 0x4c00c000 | (MEMPLLx_M4PDIV << 28)); // Clear RG_MEMPLL2_FB_MCK_SEL[25]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), 0x4c00c000 | (MEMPLLx_M4PDIV << 28)); // Clear RG_MEMPLL4_FB_MCK_SEL[25]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), 0x4c00c000 | (MEMPLLx_M4PDIV << 28)); // Clear RG_MEMPLL3_FB_MCK_SEL[25]

	mcDELAY_US(1);	// RG_MEMPLLx_FB_MCK_SEL -> 100ns -> RG_MEMPLLx_EN 

	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), 0x07800000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // [23]:MEMPLL2_AUTOK_EN = 1, Clr RG_MEMPLL2_EN[0]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62C), 0x07800000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // [23]:MEMPLL4_AUTOK_EN = 1, Clr RG_MEMPLL4_EN[0]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), 0x07800000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // [23]:MEMPLL3_AUTOK_EN = 1, Clr RG_MEMPLL3_EN[0]
#endif



#ifdef MEMPLL_NEW_POWERON
#if fcFOR_CHIP_ID == fcK2
	*(volatile unsigned int*)(0x10006000) = 0x0b160001; 
	*(volatile unsigned int*)(0x10006010) |= 0x08000000;  //(4) 0x10006010[27]=1  //Request MEMPLL reset/pdn mode 
	mcDELAY_US(2);
	*(volatile unsigned int*)(0x10006010) &= ~(0x08000000);  //(1) 0x10006010[27]=0 //Unrequest MEMPLL reset/pdn mode and wait settle (1us for reset)
	mcDELAY_US(13);
#endif	
#if fcFOR_CHIP_ID == fcJADE
	mcDELAY_US(2);
	mcDELAY_US(13);
#endif
#else
	mcDELAY_US(20);
#endif

	#ifndef DYNAMIC_CLK_OFF
//	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x63C), 0x00000006); // set RG_M_CK_DYN_CG_EN_COMB[1], RG_M_CK_DYN_CG_EN[2]
	#endif
	
	//ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x000000b0); // Set R_DMPLL2_CLK_EN[4]

#if fcFOR_CHIP_ID == fcK2
	//set [19:17] back to 3'b000 for SPM use
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x698), 0x00001e02);    // 4xck = VCO/5, RG_MEMPLL_RSV[2]=1

	mcDELAY_US(20);

	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x000000b0); // Set R_DMPLL2_CLK_EN[4]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x000200b1); // [0] =1, Reg.5cch[12]=1 bypass delay chain. Reg.5cch[8]=1 from Reg.640h[5]=1 --> invert to 0. So sync = 0.
	mcDELAY_US(1);
#endif
#if fcFOR_CHIP_ID == fcJADE
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x63C), 0x00000006); // set RG_M_CK_DYN_CG_EN_COMB[1], RG_M_CK_DYN_CG_EN[2] 

	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x00002090); // Set R_DMPLL2_CLK_EN[4]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x698), 0x000e9e30 | new_DMYPAD_option | gENABLE_MEMPLL_CG_TO_SAVE_POWER_val);    // [3:0]:4XCKDIVSEL=0(4xck = VCO/2), [12:5]RG_MEMPLL_RSV=f0, [4]RG_MEMPLL_ONEPLLSEL=0, [15]fpc = 0


	Reg_Sync_Writel((MEM_DCM_CTRL), (0x1f << 21) | (0x0 << 16) | (0x01 << 9) | (0x01 << 8) | (0x0 << 7) | (0x0 << 6) | (0x1f<<1));
	Reg_Sync_Writel((MEM_DCM_CTRL), Reg_Readl(MEM_DCM_CTRL) | 0x01);
	Reg_Sync_Writel((MEM_DCM_CTRL), Reg_Readl(MEM_DCM_CTRL) & (~0x01));
		
	Reg_Sync_Writel((DFS_MEM_DCM_CTRL), (0x01 << 28) | (0x1f << 21) | (0x0 << 16) | (0x01 << 9) | (0x01 << 8) | (0x0 << 7) | (0x0 << 6) | (0x1f << 1)); // add rg_dfs_mem_dcm_force_on[6]
	Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) | 0x01);
	Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) & (~0x01));

	mcDELAY_US(20);

	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x00022091);	
	mcDELAY_US(1);	
#endif

	#ifndef DYNAMIC_CLK_OFF
	// Need to move after DIV2 sync.
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x63C), 0x00000006); // set RG_M_CK_DYN_CG_EN_COMB[1], RG_M_CK_DYN_CG_EN[2]
	#endif

#ifdef fcMEMPLL_DBG_MONITOR
    // delay 20us
    mcDELAY_US(20);

    // Monitor enable
    // RG_MEMPLL*_MONCK_EN=1; RG_MEMPLL*_MONREF=1
    // PLL2
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x614), &u4value);
    mcSET_BIT(u4value, 3);
    mcSET_BIT(u4value, 1);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), u4value);
    // PLL3
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x620), &u4value);
    mcSET_BIT(u4value, 3);
    mcSET_BIT(u4value, 1);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), u4value);
    // PLL4
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x62c), &u4value);
    mcSET_BIT(u4value, 3);
    mcSET_BIT(u4value, 1);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62c), u4value);    

    // 0x61c[22] RG_MEMPLL2_MON_EN=1, 0x61c[21:19] RG_MEMPLL2_CKSEL_MON=100
    // RG_MEMPLL2_MON_EN -> to enable M5
    // RG_MEMPLL2_CKSEL_MON = 100 (select M5)
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x61c), &u4value);
    mcSET_BIT(u4value, 22);
    mcSET_FIELD(u4value, 4, 0x00380000, 19);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x61c), u4value);

    // 0x628[22] RG_MEMPLL3_MON_EN=1, 0x628[21:19] RG_MEMPLL3_CKSEL_MON=100
    // RG_MEMPLL3_MON_EN -> to enable M5
    // RG_MEMPLL3_CKSEL_MON = 100 (select M5)
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x628), &u4value);
    mcSET_BIT(u4value, 22);
    mcSET_FIELD(u4value, 4, 0x00380000, 19);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x628), u4value);

    // 0x634[22] RG_MEMPLL4_MON_EN=1, 0x634[21:19] RG_MEMPLL4_CKSEL_MON=100 
    // RG_MEMPLL4_MON_EN -> to enable M5
    // RG_MEMPLL4_CKSEL_MON = 100 (select M5)
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x634), &u4value);
    mcSET_BIT(u4value, 22);
    mcSET_FIELD(u4value, 4, 0x00380000, 19);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x634), u4value);

    // 0x60c[23:8] RG_MEMPLL_TEST[15:0]; 
    // RG_MEMPLL_TEST[5:4]=01,RG_MEMPLL_TEST[3]=1;RG_MEMPLL_TEST[1:0]=11
    // RG_MEMPLL_TEST[5:4]=01 -> MEMPLL2
    // RG_MEMPLL_TEST[5:4]=10 -> MEMPLL3
    // RG_MEMPLL_TEST[5:4]=11 -> MEMPLL4
    // RG_MEMPLL_TEST[3,1] -> select CKMUX (measure clock or voltage)
    // RG_MEMPLL_TEST[0] -> RG_A2DCK_EN (for FT)
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x60c), &u4value);
    mcSET_BIT(u4value, 6);
    mcSET_BIT(u4value, 7);
    mcSET_FIELD(u4value, 3, 0x00000300, 8);
    mcSET_BIT(u4value, 11);
    mcSET_FIELD(u4value, 3, 0x00003000, 12);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x60c), u4value);

    (*(volatile unsigned int *)(0x1000F638)) = ((*(volatile unsigned int *)(0x1000F638)) & 0xf8ffffff) | 0x03000000;
    //mcSHOW_DBG_MSG(("mt_get_mem_freq()=%d\n", mt_get_bus_freq()));


/*
    //0x638[26:24] RG_MEMPLL_TEST_DIV2_SEL=011, /8 (for FT)
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x638), &u4value);
    mcSET_FIELD(u4value, 3, 0x07000000, 24);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x638), u4value);*/
#endif

#if 0
    // for debug
    u4value = DramOperateDataRate(p);
    mcSHOW_DBG_MSG(("DRAM Operation Data Rate: DDR-%d\n", u4value));
#endif
            
    if (ucstatus)
    {
        return DRAM_FAIL;
    }
    else
    {
        return DRAM_OK;
    }    
}

#else //ONE_PLL_MODE_INIT
DRAM_STATUS_T MemPllInit(DRAMC_CTX_T *p)
{
	U8 ucstatus = 0;
	U32 u4value;
	U32 MEMPLL_N_INFO, MEMPLL_DIV;
	U32 MEMPLLx_FBDIV, MEMPLLx_BR, MEMPLLx_BC, MEMPLLx_IR, MEMPLLx_IC, MEMPLLx_BP, MEMPLLx_M4PDIV;
    EMI_SETTINGS *emi_set;
    
#if fcFOR_CHIP_ID == fcJADE
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x600), 0xd5200000);  // set to default value, bit[31] set to 1 to toggle INFO_CHG
#endif
    
    
    if(emi_setting_index == -1)   
    {
        emi_set = default_emi_setting;
    }
    else
    {
        emi_set = &emi_settings[emi_setting_index];
    }                              


#ifndef LOCK_ON_1PLL_3PLL
//According to T28HPM_LJ_MEM_PLL_SettingWizard_Jade.xls
    if(p->frequency == 933) //DDR_1866
    {
    	MEMPLL_N_INFO = 0x6fa4158d;
    	MEMPLL_DIV = 0x38;
    	MEMPLLx_FBDIV = 0x08;
    	MEMPLLx_BR = 0x01;
    	MEMPLLx_BC = 0x00;
    	MEMPLLx_IR = 0x0d;
    	MEMPLLx_IC = 0x06;
    	MEMPLLx_BP = 0x01;
    	MEMPLLx_M4PDIV = 0x0;
    }
    else if(p->frequency == 890) //DDR_1780	
    {
    	MEMPLL_N_INFO = 0x7163b13b;
    	MEMPLL_DIV = 0x35;
    	MEMPLLx_FBDIV = 0x07;
    	MEMPLLx_BR = 0x01;
    	MEMPLLx_BC = 0x00;
    	MEMPLLx_IR = 0x0c;
    	MEMPLLx_IC = 0x06;
    	MEMPLLx_BP = 0x01;
    	MEMPLLx_M4PDIV = 0x0;
    }
    else if(p->frequency == 800)	//DDR_1600
    {
    	MEMPLL_N_INFO = 0x3d89d89d;
    	MEMPLL_DIV = 0x1c;
    	MEMPLLx_FBDIV = 0x06;
    	MEMPLLx_BR = 0x01;
    	MEMPLLx_BC = 0x01;
    	MEMPLLx_IR = 0x0a;
    	MEMPLLx_IC = 0x07;
    	MEMPLLx_BP = 0x01;
    	MEMPLLx_M4PDIV = 0x0;
    }
    else if(p->frequency == 666) //DDR_1333	
    {
    	MEMPLL_N_INFO = 0x3bd06906;
    	MEMPLL_DIV = 0x1c;
    	MEMPLLx_FBDIV = 0x05;
    	MEMPLLx_BR = 0x01;
    	MEMPLLx_BC = 0x01;
    	MEMPLLx_IR = 0x08;
    	MEMPLLx_IC = 0x06;
    	MEMPLLx_BP = 0x01;
    	MEMPLLx_M4PDIV = 0x0;
    }
    else if(p->frequency == 533)	//DDR_1066
    {
    	MEMPLL_N_INFO = 0x72cccccc;
    	MEMPLL_DIV = 0x38;
    	MEMPLLx_FBDIV = 0x04;
    	MEMPLLx_BR = 0x02;
    	MEMPLLx_BC = 0x02;
    	MEMPLLx_IR = 0x0d;
    	MEMPLLx_IC = 0x07;
    	MEMPLLx_BP = 0x02;
    	MEMPLLx_M4PDIV = 0x0;
    }	
#if 0
    else if(p->frequency == 580) //DDR_1160
    {
    	MEMPLL_N_INFO = 0x71c4ec4e;
    	MEMPLL_DIV = 0x33;
    	MEMPLLx_FBDIV = 0x04;
    	MEMPLLx_BR = 0x02;
    	MEMPLLx_BC = 0x02;
    	MEMPLLx_IR = 0x0d;
    	MEMPLLx_IC = 0x07;
    	MEMPLLx_BP = 0x02;
    	MEMPLLx_M4PDIV = 0x0;
    }
    else if(p->frequency == 400) //DDR_800
    {
    	MEMPLL_N_INFO = 0x6f89d89d;
    	MEMPLL_DIV = 0x3a;
    	MEMPLLx_FBDIV = 0x07;
    	MEMPLLx_FBDIV_0 = 0x03;
    	MEMPLLx_BR = 0x01;
    	MEMPLLx_BC = 0x01;
    	MEMPLLx_IR = 0x0b;
    	MEMPLLx_IC = 0x0c;
    	MEMPLLx_BP = 0x01;
    	MEMPLLx_M4PDIV = 0x01;
    }
    else if(p->frequency == 333) //DDR_667	
    {
    	MEMPLL_N_INFO = 0x714dee6b;
    	MEMPLL_DIV = 0x35;
    	MEMPLLx_FBDIV = 0x05;
    	MEMPLLx_FBDIV_0 = 0x02;
    	MEMPLLx_BR = 0x01;
    	MEMPLLx_BC = 0x01;
    	MEMPLLx_IR = 0x08;
    	MEMPLLx_IC = 0x06;
    	MEMPLLx_BP = 0x01;
    	MEMPLLx_M4PDIV = 0x01;
    }
#endif
#endif
#ifdef LOCK_ON_1PLL_3PLL	//Francis : for 3pll/1pll all use one setting, and this setting is according to 1pll. 

#if 0
		//	1300/1800 setting
		MEMPLL_N_INFO = 0x6384ec4e;	//26*0x6384ec4e/2^24=2587.5	1pll:2587.5/4=646.76(1293MHz)	3pll:2587.5/0x2E=56.24(REF_INT), 56.25*2*(0x07+1)=900(1800MHz)
		MEMPLL_DIV = 0x2E;
    	MEMPLLx_FBDIV = 0x07;
    	MEMPLLx_BR = 0x02;
    	MEMPLLx_BC = 0x03;
    	MEMPLLx_IR = 0x0c;
    	MEMPLLx_IC = 0x07;
    	MEMPLLx_BP = 0x02;
    	MEMPLLx_M4PDIV = 0x0;	
#endif

#if 0
		//	1250/1739 setting
		MEMPLL_N_INFO = 0x60176276;	//26*0x60276276/2^24=2500	1pll:2500/4=625(1250MHz)	3pll:2500/0x2E=54.34(REF_INT), 54.34*2*(0x07+1)=869.5(1739MHz)
		MEMPLL_DIV = 0x2E;
    	MEMPLLx_FBDIV = 0x07;
    	MEMPLLx_BR = 0x02;
    	MEMPLLx_BC = 0x03;
    	MEMPLLx_IR = 0x0c;
    	MEMPLLx_IC = 0x07;
    	MEMPLLx_BP = 0x02;
    	MEMPLLx_M4PDIV = 0x0;	
#endif	

#if 1
        //  1300/1666 setting
        MEMPLL_N_INFO = 0x641ec4ec; //26*0x641ec4ec/2^24=2603.1	1pll:2603.1/4=650.78(1301MHz)	3pll:2603.1/0x32=52(REF_INT), 52*2*(0x07+1)=833(1666MHz)
        MEMPLL_DIV = 0x32;
        MEMPLLx_FBDIV = 0x07;
        MEMPLLx_BR = 0x02;
        MEMPLLx_BC = 0x03;
        MEMPLLx_IR = 0x0c;
        MEMPLLx_IC = 0x07;
        MEMPLLx_BP = 0x02;
        MEMPLLx_M4PDIV = 0x0;   
#endif 

#if 0	//Darren+
	// 1068/1800 setting
	MEMPLL_N_INFO = 0x52362762; //26*0x52362762/2^24=2137.5 1pll:2137.5/4=534.3(1068MHz) 3pll:2137.5/0x26=56.24(REF_INT), 56.25*2*(0x07+1)=900(1800MHz)
	MEMPLL_DIV = 0x26;
	MEMPLLx_FBDIV = 0x05;
	MEMPLLx_BR = 0x02;
	MEMPLLx_BC = 0x03;
	MEMPLLx_IR = 0x0c;
	MEMPLLx_IC = 0x07;
	MEMPLLx_BP = 0x02;
	MEMPLLx_M4PDIV = 0x0; 
#endif

#ifdef FIRST_BRING_UP
		//	1146/1066 setting
		MEMPLL_N_INFO = 0x58266666;	//26*0x58266666/2^24=2291.9	1pll:2291.9/4=573(1146MHz)	3pll:2291.9/0x2B=53.3(REF_INT), 53.3*2*(0x04+1)=533(1066MHz)
		MEMPLL_DIV = 0x2B;
    	MEMPLLx_FBDIV = 0x04;
    	MEMPLLx_BR = 0x02;
    	MEMPLLx_BC = 0x03;
    	MEMPLLx_IR = 0x07;
    	MEMPLLx_IC = 0x04;
    	MEMPLLx_BP = 0x02;
    	MEMPLLx_M4PDIV = 0x0;				
#endif		
#endif


#if fcFOR_CHIP_ID == fcJADE
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x000220b1);  //0x1000F640[13] = 1 -> ALL_CLK_ECO_EN = 1, [13]RG_E2_ECO_EN = 1, Mengru Si:reset mode phase error    
#endif


#if 1
//	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x09000000); 		
//	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x09000010); 		

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
	// RG_MEMPLLx_EN = 0, Reg.614h[0], Reg.620h[0], Reg.62ch[0]
	// RG_MEMPLLx_AUTOK_EN=1 [23]
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x614), &u4value);
	mcCLR_BIT(u4value, 0);
	mcSET_BIT(u4value, 23);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), u4value);
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x620), &u4value);
	mcCLR_BIT(u4value, 0);
	mcSET_BIT(u4value, 23);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), u4value);
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x62c), &u4value);
	mcCLR_BIT(u4value, 0);
	mcSET_BIT(u4value, 23);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62c), u4value);

	// RG_MEMPLLx_FB_MCK_SEL = 0, Reg.618h[25], Reg.624h[25], Reg.630h[25]
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x618), &u4value);
	mcCLR_BIT(u4value, 25);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), u4value);
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x624), &u4value);
	mcCLR_BIT(u4value, 25);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), u4value);
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x630), &u4value);
	mcCLR_BIT(u4value, 25);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), u4value);


	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), 0x07800000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set RG_MEMPLL2
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), 0x4c00c000 | (MEMPLLx_M4PDIV << 28)); // Set RG_MEMPLL2_RST_DLY[31:30]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x61C), 0x00020001 | (MEMPLLx_BR << 10) | (MEMPLLx_BC << 8) | (MEMPLLx_BP << 12)); // Set RG_MEMPLL2
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62C), 0x07800000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set RG_MEMPLL4
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), 0x4c00c000 | (MEMPLLx_M4PDIV << 28)); // Set RG_MEMPLL4_RST_DLY[31:30]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x634), 0x00000001 | (MEMPLLx_BR << 10) | (MEMPLLx_BC << 8) | (MEMPLLx_BP << 12)); // Set RG_MEMPLL4
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), 0x07800000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set RG_MEMPLL3
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), 0x4c00c000 | (MEMPLLx_M4PDIV << 28)); // Set RG_MEMPLL3_RST_DLY[31:30]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x628), 0x00020001 | (MEMPLLx_BR << 10) | (MEMPLLx_BC << 8) | (MEMPLLx_BP << 12)); // Set RG_MEMPLL3
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x698), 0x00001e00 | gENABLE_MEMPLL_CG_TO_SAVE_POWER_val); // 4xck = VCO/5, RG_MEMPLL_RSV[2]=1
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x63C), 0x00000000); 
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x688), 0x00000000); // Set RG_MEMPLL_REFCK_EN[0]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x638), 0x00000000); // Set RG_MEMPLL_REFCK_SEL to 1 by PLL1
#endif


	// --------------------------------------------------
	// MEMPLL configuration
	// --------------------------------------------------
#if fcFOR_CHIP_ID == fcJADE	
		Reg_Sync_Writel((MEM_DCM_CTRL), (0x1f << 21) | (0x0 << 16) | (0x01 << 9) | (0x01 << 8) | (0x0 << 7) | (0x0 << 6) | (0x1f<<1));
		Reg_Sync_Writel((MEM_DCM_CTRL), Reg_Readl(MEM_DCM_CTRL) | 0x01);
		Reg_Sync_Writel((MEM_DCM_CTRL), Reg_Readl(MEM_DCM_CTRL) & (~0x01)); 
//		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), (0x01 << 28)); //change clk_mux from 26M to 4X clock
	


#if 1
Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) |(0x01 << 31) | (0x01 << 26)); 
Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) | (0x01 << 28)); //change clk_mux from 26M to 4X clock

//Close [5]
ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x698), 0x000e9e10 | new_DMYPAD_option | gENABLE_MEMPLL_CG_TO_SAVE_POWER_val);    // [3:0]:4XCKDIVSEL=0(4xck = VCO/2), RG_MEMPLL_RSV=0000, [4]:RG_MEMPLL_ONEPLLSEL=0, fpc = 0, 

//move open CG laster before pin mux 
//ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x698), 0x000e9e30 | new_DMYPAD_option | gENABLE_MEMPLL_CG_TO_SAVE_POWER_val);    // [3:0]:4XCKDIVSEL=0(4xck = VCO/2), RG_MEMPLL_RSV=0000, [4]:RG_MEMPLL_ONEPLLSEL=0, fpc = 0, 
//mcDELAY_US(10);
//Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) & (~(0x01<<31)));
//mcDELAY_US(10);

//Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) & (~(0x01 << 26)));
Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) | 0x01);
Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) & (~0x01));
#endif





	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x430)), 0x10ff10ff);
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x434)), 0xffffffff);	// R_DMDQ0SEL1XGEN	~ R_DMDQ31SEL1XGEN	
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x438)), 0xffffffff);	// R_DMDQ0SEL05XGEN ~ R_DMDQ31SEL05XGEN
	////////////////////////////////////////////////////////////////////////////////////////////////////////////	
#endif


	if (p->dram_type == TYPE_LPDDR3)
	{
		// LPDDR3
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x7c), emi_set->DRAMC_DDR2CTL_VAL); 

#if fcFOR_CHIP_ID == fcK2		
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), 0x00f00ae0);	// 114563ns, new DATLAT for TOP_PIPE, RX pipe [7:5]
#endif		
#if fcFOR_CHIP_ID == fcJADE
#ifndef JADE_FLOW_RUN_ON_K2_EVB
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), 0x006f0be0);	// new DATLAT for TOP_PIPE, RX pipe [7:5]
#else
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), 0x00f00be0);	// new DATLAT for TOP_PIPE, RX pipe [7:5]
#endif
#endif
	}  

#if fcFOR_CHIP_ID == fcJADE
#ifdef DUAL_FREQ_DIFF_ACTIMING
			if (p->frequency == DUAL_FREQ_LOW)
			{
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xfc), LPDDR3_MISCTL0_VAL_0fc_LOW);	// 114563ns
			}
			else
			{
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xfc), emi_set->DRAMC_MISCTL0_VAL);	// 114563ns
			}
#else
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xfc), emi_set->DRAMC_MISCTL0_VAL);	// 114563ns
#endif

		//	  7: 0 rg_mempll_iso_en_stb_time
		//	 15: 8 rg_dmpll2_ck_en_stb_time
		//	 23:16 rg_mempll_div_en_stb_time
		//	 31:24 rg_mempll_ddsen_stb_time
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x5c4), 0x5096001e);	// r_dmpll2_ck_en_stb_time = 0, change 0x5096961e ==> 0x5096001e
#endif

#if fcFOR_CHIP_ID == fcK2
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x5c8), 0x1111ff11);	// [15:12] bypass delay chain for mempllx enable. Need to check??
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x5cc), 0xd1111911);	// by_dmpll2_ck_en_dly first, manual enable mempll, [12] TBD
#endif	
#if fcFOR_CHIP_ID == fcJADE
	// Enable S/W control (not SPM)
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x5c8), 0x01011f01); // [9]:rg_mempll2_pwd_en_sel, [12]:by_mempll_pwd_dly
	// Enable S/W control (not SPM)
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x5cc), 0x41011101); // by_dmpll2_ck_en_dly first, manual enable mempll[12], by_mempll_div_en_dly[20] TBD
#endif	

	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x000020b1); 		// Sync off.
	mcDELAY_US(1); 	//WAIT 100ns. DA_SDM_ISO_EN=0 -> 100ns -> RG_MEMPLL_BIAS_EN=1
	
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x60c), 0xd0000040);	// 46857ns
	mcDELAY_US(2);	// RG_MEMPLL_BIAS_EN=1 -> 1us -> RG_MEMPLL_EN

	//ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x604), 0x10080000);	// 50472ns
	//mcDELAY_US(20);	// RG_MEMPLL_EN=1 -> 20us -> RG_MEMPLL_DIV_EN

#if fcFOR_CHIP_ID == fcK2
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x698), 0x00001e02);    // 4xck = VCO/5, RG_MEMPLL_RSV[2]=1
#endif	
#if fcFOR_CHIP_ID == fcJADE
//	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x698), 0x000e9e30 | new_DMYPAD_option | gENABLE_MEMPLL_CG_TO_SAVE_POWER_val);    // [3:0]:4XCKDIVSEL=0(4xck = VCO/2), RG_MEMPLL_RSV=0000, [4]:RG_MEMPLL_ONEPLLSEL=0, fpc = 0, 
#endif	

	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x600), MEMPLL_N_INFO); 				// Set [30:0]:RG_MEMPLL_N_INFO = 102.564, [31]:CHG=0
#if fcFOR_CHIP_ID == fcJADE
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x600), MEMPLL_N_INFO|0x80000000); 	// Set [30:0]:RG_MEMPLL_N_INFO = 102.564, [31]:CHG=1
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x600), MEMPLL_N_INFO); 				// Set [30:0]:RG_MEMPLL_N_INFO = 102.564, [31]:CHG=0
#endif
	
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x610), 0x08000000 | (MEMPLL_DIV << 17)); // Set [23:17]:RG_MEMPLL_DIV = 7'd52, [16]:RG_MEMPLL_DIV_EN = 0, [31:24]:RG_MEMPLL_RESERVE[3]=1																			 						
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x638), 0x10000000); // Set RG_MEMPLL_REFCK_SEL to 1 by PLL1
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x604), 0x10080000); // Set RG_MEMPLL_EN = 1, RG_MEMPLL_SDM_DI_EN = 1
	mcDELAY_US(20);	// RG_MEMPLL_EN=1 -> 20us -> RG_MEMPLL_DIV_EN		
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x610), 0x08010000 | (MEMPLL_DIV << 17)); // Set RG_MEMPLL_DIV = 7'd56, RG_MEMPLL_DIV_EN = 1, RG_MEMPLL_RESERVE[3]=1
	mcDELAY_US(1);		// RG_MEMPLL_DIV_EN = 1 -> 1us -> RG_MEMPLLx_EN=1
	
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), 0x07800000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set RG_MEMPLL2
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), 0x4c00c000 | (MEMPLLx_M4PDIV << 28)); // Set RG_MEMPLL2_RST_DLY[31:30]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x61C), 0x00020001 | (MEMPLLx_BR << 10) | (MEMPLLx_BC << 8) | (MEMPLLx_BP << 12)); // Set RG_MEMPLL2
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62C), 0x07800000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set RG_MEMPLL4
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), 0x4c00c000 | (MEMPLLx_M4PDIV << 28)); // Set RG_MEMPLL4_RST_DLY[31:30]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x634), 0x00000001 | (MEMPLLx_BR << 10) | (MEMPLLx_BC << 8) | (MEMPLLx_BP << 12)); // Set RG_MEMPLL4
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), 0x07800000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set RG_MEMPLL3
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), 0x4c00c000 | (MEMPLLx_M4PDIV << 28)); // Set RG_MEMPLL3_RST_DLY[31:30]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x628), 0x00020001 | (MEMPLLx_BR << 10) | (MEMPLLx_BC << 8) | (MEMPLLx_BP << 12)); // Set RG_MEMPLL3

	// wait 100ns
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x688), 0x00000001); // Set RG_MEMPLL_REFCK_EN[0]

	mcDELAY_US(20);	// RG_MEMPLL_DIV_EN->1us->RG_MEMPLLx_EN

	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x60c), 0xd000005d); // Set RG_MEMPLL_BIAS_EN[6], RG_MEMPLL2/3/4_GATE_EN=1(RG_MEMPLL_RESERVE1[2][3][4]=1)

	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), 0x07800001 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set RG_MEMPLL2_EN[0]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62C), 0x07800001 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set RG_MEMPLL4_EN[0]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), 0x07800001 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set RG_MEMPLL3_EN[0]	

	mcDELAY_US(106);	// RG_MEMPLLx_EN -> wait 100us -> RG_MEMPLL_BIAS_LPF_EN

	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x60c), 0xd00000dd); // Set RG_MEMPLL_LPF_BIAS_EN[7], RG_MEMPLL2/3/4_GATE_EN=1(RG_MEMPLL_RESERVE1[2][3][4]=1)

	mcDELAY_US(20);	//PLL ready (internal loop)

#ifdef MEMPLL_BAND_INFO
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x3e0), &u4value);
    	VCOK_Cha_Mempll2 = (u4value >> 16) & 0x7f;
	    VCOK_Cha_Mempll3 = (u4value >> 8) & 0x7f;
        mcSHOW_DBG_MSG3(("0x3e0=0x%x, VCOK_Cha_Mempll2=0x%x, VCOK_Cha_Mempll3=0x%x \n", 
        	u4value, VCOK_Cha_Mempll2, VCOK_Cha_Mempll3));

        // RG_MEMPLL_RESERVE[2]=1, to select MEMPLL4 band register
        // RGS_MEMPLL4_AUTOK_BAND[6:0]= RGS_MEMPLL4_AUTOK_BAND[6]+RGS_MEMPLL3_AUTOK_BAND[5:0]
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x610), &u4value);
        mcSET_BIT(u4value, 26);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x610), u4value);

        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x3e0), &u4value);
        VCOK_Cha_Mempll4 = ((u4value >> 8) & 0x3f) | (u4value & 0x40);
        mcSHOW_DBG_MSG3(("0x3e0=0x%x, VCOK_Cha_Mempll4=0x%x\n", u4value, VCOK_Cha_Mempll4));        

        // RG_MEMPLL_RESERVE[2]=0, recover back
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x610), &u4value);
        mcCLR_BIT(u4value, 26);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x610), u4value);        
#endif

	
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), 0x07800000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set ~RG_MEMPLL2_EN[0] & RG_MEMPLL2_AUTOK_EN[23]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62C), 0x07800000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set ~RG_MEMPLL4_EN[0] & RG_MEMPLL4_AUTOK_EN[23]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), 0x07800000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set ~RG_MEMPLL3_EN[0] & RG_MEMPLL3_AUTOK_EN[23]

	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), 0x07000000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set ~RG_MEMPLL2_EN[0] & ~RG_MEMPLL2_AUTOK_EN[23]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62C), 0x07000000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set ~RG_MEMPLL4_EN[0] & ~RG_MEMPLL4_AUTOK_EN[23]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), 0x07000000 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set ~RG_MEMPLL3_EN[0] & ~RG_MEMPLL3_AUTOK_EN[23]

	mcDELAY_US(1);	// RG_MEMPLL_BIAS_LPF_EN -> 1us -> RG_MEMPLLx_FB_MCK_SEL

	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), 0x4e00c000 | (MEMPLLx_M4PDIV << 28)); // Set RG_MEMPLL2_FB_MCK_SEL[25]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), 0x4e00c000 | (MEMPLLx_M4PDIV << 28)); // Set RG_MEMPLL4_FB_MCK_SEL[25]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), 0x4e00c000 | (MEMPLLx_M4PDIV << 28)); // Set RG_MEMPLL3_FB_MCK_SEL[25]

	mcDELAY_US(1);	// RG_MEMPLLx_FB_MCK_SEL -> 100ns -> RG_MEMPLLx_EN 

	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), 0x07000001 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set RG_MEMPLL2_EN[0]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62C), 0x07000001 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set RG_MEMPLL4_EN[0]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), 0x07000001 | (MEMPLLx_FBDIV << 16) | (MEMPLLx_IR << 28) | (MEMPLLx_IC << 8)); // Set RG_MEMPLL3_EN[0]

#ifdef MEMPLL_NEW_POWERON
#if fcFOR_CHIP_ID == fcK2
	*(volatile unsigned int*)(0x10006000) = 0x0b160001; 
	*(volatile unsigned int*)(0x10006010) |= 0x08000000;  //(4) 0x10006010[27]=1  //Request MEMPLL reset/pdn mode 
	mcDELAY_US(2);
	*(volatile unsigned int*)(0x10006010) &= ~(0x08000000);  //(1) 0x10006010[27]=0 //Unrequest MEMPLL reset/pdn mode and wait settle (1us for reset)
	mcDELAY_US(13);
#endif	
#if fcFOR_CHIP_ID == fcJADE
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), 0x4e01c000 | (MEMPLLx_M4PDIV << 28)); // Set RG_MEMPLL2_RESERVE0(RG_MEMPLL2_RST)[16]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), 0x4e01c000 | (MEMPLLx_M4PDIV << 28)); // Set RG_MEMPLL4_RESERVE0(RG_MEMPLL2_RST)[16]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), 0x4e01c000 | (MEMPLLx_M4PDIV << 28)); // Set RG_MEMPLL3_RESERVE0(RG_MEMPLL2_RST)[16]
	mcDELAY_US(2);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), 0x4e00c000 | (MEMPLLx_M4PDIV << 28)); // Clr RG_MEMPLL2_RESERVE0(RG_MEMPLL2_RST)[16]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), 0x4e00c000 | (MEMPLLx_M4PDIV << 28)); // Clr RG_MEMPLL4_RESERVE0(RG_MEMPLL2_RST)[16]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), 0x4e00c000 | (MEMPLLx_M4PDIV << 28)); // Clr RG_MEMPLL3_RESERVE0(RG_MEMPLL2_RST)[16]
	mcDELAY_US(13);
#endif
#else
	mcDELAY_US(20);
#endif

	#ifndef DYNAMIC_CLK_OFF
//	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x63C), 0x00000006); // set RG_M_CK_DYN_CG_EN_COMB[1], RG_M_CK_DYN_CG_EN[2]
	#endif
	
	//ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x000000b0); // Set R_DMPLL2_CLK_EN[4]

#if fcFOR_CHIP_ID == fcK2
	//set [19:17] back to 3'b000 for SPM use
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x698), 0x00001e02);    // 4xck = VCO/5, RG_MEMPLL_RSV[2]=1

	mcDELAY_US(20);

	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x000000b0); // Set R_DMPLL2_CLK_EN[4]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x000200b1); // [0] =1, Reg.5cch[12]=1 bypass delay chain. Reg.5cch[8]=1 from Reg.640h[5]=1 --> invert to 0. So sync = 0.
	mcDELAY_US(1);
#endif
#if fcFOR_CHIP_ID == fcJADE
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x63C), 0x00000006); // set RG_M_CK_DYN_CG_EN_COMB[1], RG_M_CK_DYN_CG_EN[2] 

	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x000020b0); // Set R_DMPLL2_CLK_EN[4]
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x698), 0x000e9e20 | new_DMYPAD_option | gENABLE_MEMPLL_CG_TO_SAVE_POWER_val);    // [3:0]:4XCKDIVSEL=0(4xck = VCO/2), [12:5]RG_MEMPLL_RSV=f0, [4]RG_MEMPLL_ONEPLLSEL=0, [15]fpc = 0

	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x000220b1);	
	mcDELAY_US(1);	

//1PLL path CG on
//ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x698), 0x000e9e20 | new_DMYPAD_option | gENABLE_MEMPLL_CG_TO_SAVE_POWER_val);    // [3:0]:4XCKDIVSEL=0(4xck = VCO/2), RG_MEMPLL_RSV=0000, [4]:RG_MEMPLL_ONEPLLSEL=0, fpc = 0, 
//mcDELAY_US(10);
Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) & (~(0x01<<31)));
mcDELAY_US(10);

Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) & (~(0x01 << 26)));

///////////////////////////////////////////////////////////////////////////////////////////
	mcDELAY_US(20);

	Reg_Sync_Writel((CLK_CFG_0_CLR), 0x300);
	Reg_Sync_Writel((CLK_CFG_0_SET), 0x100);	

	mcDELAY_US(2);

	Reg_Sync_Writel((CLK_CFG_UPDATE), 0x02);
///////////////////////////////////////////////////////////////////////////////////////////	
#endif

	#ifndef DYNAMIC_CLK_OFF
	// Need to move after DIV2 sync.
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x63C), 0x00000006); // set RG_M_CK_DYN_CG_EN_COMB[1], RG_M_CK_DYN_CG_EN[2]
	#endif

#ifdef fcMEMPLL_DBG_MONITOR
    // delay 20us
    mcDELAY_US(20);

    // Monitor enable
    // RG_MEMPLL*_MONCK_EN=1; RG_MEMPLL*_MONREF=1
    // PLL2
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x614), &u4value);
    mcSET_BIT(u4value, 3);
    mcSET_BIT(u4value, 1);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), u4value);
    // PLL3
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x620), &u4value);
    mcSET_BIT(u4value, 3);
    mcSET_BIT(u4value, 1);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), u4value);
    // PLL4
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x62c), &u4value);
    mcSET_BIT(u4value, 3);
    mcSET_BIT(u4value, 1);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62c), u4value);    

    // 0x61c[22] RG_MEMPLL2_MON_EN=1, 0x61c[21:19] RG_MEMPLL2_CKSEL_MON=100
    // RG_MEMPLL2_MON_EN -> to enable M5
    // RG_MEMPLL2_CKSEL_MON = 100 (select M5)
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x61c), &u4value);
    mcSET_BIT(u4value, 22);
    mcSET_FIELD(u4value, 4, 0x00380000, 19);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x61c), u4value);

    // 0x628[22] RG_MEMPLL3_MON_EN=1, 0x628[21:19] RG_MEMPLL3_CKSEL_MON=100
    // RG_MEMPLL3_MON_EN -> to enable M5
    // RG_MEMPLL3_CKSEL_MON = 100 (select M5)
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x628), &u4value);
    mcSET_BIT(u4value, 22);
    mcSET_FIELD(u4value, 4, 0x00380000, 19);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x628), u4value);

    // 0x634[22] RG_MEMPLL4_MON_EN=1, 0x634[21:19] RG_MEMPLL4_CKSEL_MON=100 
    // RG_MEMPLL4_MON_EN -> to enable M5
    // RG_MEMPLL4_CKSEL_MON = 100 (select M5)
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x634), &u4value);
    mcSET_BIT(u4value, 22);
    mcSET_FIELD(u4value, 4, 0x00380000, 19);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x634), u4value);

    // 0x60c[23:8] RG_MEMPLL_TEST[15:0]; 
    // RG_MEMPLL_TEST[5:4]=01,RG_MEMPLL_TEST[3]=1;RG_MEMPLL_TEST[1:0]=11
    // RG_MEMPLL_TEST[5:4]=01 -> MEMPLL2
    // RG_MEMPLL_TEST[5:4]=10 -> MEMPLL3
    // RG_MEMPLL_TEST[5:4]=11 -> MEMPLL4
    // RG_MEMPLL_TEST[3,1] -> select CKMUX (measure clock or voltage)
    // RG_MEMPLL_TEST[0] -> RG_A2DCK_EN (for FT)
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x60c), &u4value);
    mcSET_BIT(u4value, 6);
    mcSET_BIT(u4value, 7);
    mcSET_FIELD(u4value, 3, 0x00000300, 8);
    mcSET_BIT(u4value, 11);
    mcSET_FIELD(u4value, 3, 0x00003000, 12);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x60c), u4value);

    (*(volatile unsigned int *)(0x1000F638)) = ((*(volatile unsigned int *)(0x1000F638)) & 0xf8ffffff) | 0x03000000;
    //mcSHOW_DBG_MSG(("mt_get_mem_freq()=%d\n", mt_get_bus_freq()));


/*
    //0x638[26:24] RG_MEMPLL_TEST_DIV2_SEL=011, /8 (for FT)
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x638), &u4value);
    mcSET_FIELD(u4value, 3, 0x07000000, 24);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x638), u4value);*/
#endif

#if 0
    // for debug
    u4value = DramOperateDataRate(p);
    mcSHOW_DBG_MSG(("DRAM Operation Data Rate: DDR-%d\n", u4value));
#endif
            
    if (ucstatus)
    {
        return DRAM_FAIL;
    }
    else
    {
        return DRAM_OK;
    }    
}

#endif //ONE_PLL_MODE_INIT


U32 DramOperateDataRate(DRAMC_CTX_T *p)
{
    U32 u4value1;
    U8 ucstatus = 0;
    
	U32 MEMPLL_N_INFO, MEMPLL_DIV;
	U32 MEMPLLx_FBDIV, MEMPLLx_M4PDIV;
	U32 onepll_fout, threepll_fout;

    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x600), &u4value1);
    MEMPLL_N_INFO = mcGET_FIELD(u4value1, 0x7fffffff, 0);	

    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x610), &u4value1);
    MEMPLL_DIV = mcGET_FIELD(u4value1, 0x00fe0000, 17);		

    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x62C), &u4value1);
    MEMPLLx_FBDIV = mcGET_FIELD(u4value1, 0x007f0000, 16);	

    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x630), &u4value1);
    MEMPLLx_M4PDIV = mcGET_FIELD(u4value1, 0x30000000, 28);	
	if (MEMPLLx_M4PDIV == 0) MEMPLLx_M4PDIV = 2;
	else if (MEMPLLx_M4PDIV == 1) MEMPLLx_M4PDIV = 4;
	else if (MEMPLLx_M4PDIV == 2) MEMPLLx_M4PDIV = 8;

	//mcSHOW_DBG_MSG(("[DramOperateDataRate] MEMPLL_N_INFO=%x, MEMPLL_DIV=%x, MEMPLLx_FBDIV=%x, MEMPLLx_M4PDIV=%x\n",MEMPLL_N_INFO,MEMPLL_DIV,MEMPLLx_FBDIV,MEMPLLx_M4PDIV));

	onepll_fout = ((26*(MEMPLL_N_INFO>>24))+(26*(MEMPLL_N_INFO&0x00ffffff)>>24))/4;
	threepll_fout = (onepll_fout*4*MEMPLLx_M4PDIV*(MEMPLLx_FBDIV+1))/MEMPLL_DIV;	

	//mcSHOW_DBG_MSG(("[DramOperateDataRate] onepll_fout = %d, threepll_fout = %d\n",onepll_fout, threepll_fout));

	u4value1 = 	(Reg_Readl(CLK_CFG_0) >> 8) & 3; 	
	if (u4value1 == 1)
	{
		return threepll_fout;
	}
	else
	{
		return onepll_fout;
	}
}

void Hynix_Test_Mode(DRAMC_CTX_T *p, unsigned char rank_num)
{
	Fra_ModeRegWrite(9, 0xb8, rank_num);
	Fra_ModeRegWrite(9, 0xe8, rank_num);
	Fra_ModeRegWrite(9, 0x98, rank_num);
	Fra_ModeRegWrite(9, 0xbf, rank_num);
	Fra_ModeRegWrite(9, 0xef, rank_num);
	Fra_ModeRegWrite(9, 0x9f, rank_num);
	Fra_ModeRegWrite(9, 0xb9, rank_num);
	Fra_ModeRegWrite(9, 0xe9, rank_num);
	Fra_ModeRegWrite(9, 0x99, rank_num);
	Fra_ModeRegWrite(9, 0xd8, rank_num);
	Fra_ModeRegWrite(9, 0x88, rank_num);
	Fra_ModeRegWrite(9, 0xa3, rank_num);
	Fra_ModeRegWrite(9, 0xe0, rank_num);
}

// The following routine should be enabled after bring-up.
// Add dual frequency AC timing, RL/WL and selph for different MR2 (RL/WL)
//-------------------------------------------------------------------------
/** DramcInit
 *  DRAMC Initialization.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
DRAM_STATUS_T DramcInit(DRAMC_CTX_T *p)
{
	U8 ucstatus = 0;
	U32 u4value;
    EMI_SETTINGS *emi_set;
    
    if(emi_setting_index == -1)
        emi_set = default_emi_setting;
    else
        emi_set = &emi_settings[emi_setting_index];   
    
	// error handling
	if (!p)
	{
		mcSHOW_ERR_MSG(("context is NULL\n"));
		return DRAM_FAIL;
	}

#ifdef FRCLK_26M_DIV8
	 *(volatile unsigned *)(0x10001088) |= 0x80000000;	// MODULE_SW_CG_1_SET[31] = 1 ==> dramc_f26m_cg_set
	 *(volatile unsigned *)(0x10001098) |= (0x01 <<7);		// dramc_f26m_sel : 0-div 1, 1-div8
	 *(volatile unsigned *)(0x1000108C) |= 0x80000000;	// MODULE_SW_CG_1_CLR[31] = 1 ==> dramc_f26m_cg_set
#endif

#ifdef DUAL_RANKS
	u4value = *(volatile unsigned *)(EMI_APB_BASE+0x0);
	// EMI_CONA[17]
	if (u4value & 0x00020000)	
	{
		uiDualRank = 1;
	}
	else
#endif
	{
		uiDualRank = 0;
	}		



	if (p->dram_type == TYPE_LPDDR3) 
	{
		// LPDDR3
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x430), 0x10ff10ff);	
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x434), 0xffffffff);	// R_DMDQ0SEL1XGEN  ~ R_DMDQ31SEL1XGEN
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x438), 0xffffffff);	// R_DMDQ0SEL05XGEN ~ R_DMDQ31SEL05XGEN
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x43c), 0x0000001f);	
#ifdef NORMAL_CMD_SELPH
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x400), 0x11111111);	
#else
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x400), 0x00000000);	
#endif

		if (p->frequency == 400)
		{
		  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x404), 0x11312111);	// 112860ns, [28,24,20] TBD, reg_TX_DLY_DQSgated_P1[22:20], reg_TX_DLY_DQSgated[14:12] different from local
		}
		else
		{
#if fcFOR_CHIP_ID == fcK2		
		  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x404), 0x11413111);	// 112860ns, [28,24,20] TBD, reg_TX_DLY_DQSgated_P1[22:20], reg_TX_DLY_DQSgated[14:12] different from local
#endif
#if fcFOR_CHIP_ID == fcJADE
#ifdef NORMAL_CMD_SELPH
		  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x404), 0x11514111);	// 112860ns, [28,24,20] TBD, reg_TX_DLY_DQSgated_P1[22:20], reg_TX_DLY_DQSgated[14:12] different from local
#else
		  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x404), 0x10514000);	// 112860ns, [28,24,20] TBD, reg_TX_DLY_DQSgated_P1[22:20], reg_TX_DLY_DQSgated[14:12] different from local
#endif
#endif

		}
		
#ifdef NORMAL_CMD_SELPH		
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x408), 0x11111111);	// 112890ns
#else
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x408), 0x00000000);	// 112890ns
#endif
#ifdef NORMAL_CMD_SELPH
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x40c), 0x11111111);	// 112920ns
#else
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x40c), 0x00000000);	// 112920ns
#endif

		if(is_TB_setting()==0)
        {
			if (p->frequency == 400)
			{
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x410), 0x55d55555);	// 112950ns, reg_dly_DQSgated_P1[25:24] and reg_dly_DQSgated[23:22] different from local
			}
			else
			{
#if fcFOR_CHIP_ID == fcK2			
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x410), 0x54955555);	// 112950ns, reg_dly_DQSgated_P1[25:24] and reg_dly_DQSgated[23:22] different from local
#endif				
#if fcFOR_CHIP_ID == fcJADE
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x410), 0x55c55555); // 112950ns, reg_dly_DQSgated_P1[25:24] and reg_dly_DQSgated[23:22] different from local
#endif

			}
		}
		else
       	{
#if fcFOR_CHIP_ID == fcK2	       	
	       ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x410), 0x54955555);	// 112950ns, reg_dly_DQSgated_P1[25:24] and reg_dly_DQSgated[23:22] different from local
#endif	       
#if fcFOR_CHIP_ID == fcJADE
		   ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x410), 0x55c55555);	// 112950ns, reg_dly_DQSgated_P1[25:24] and reg_dly_DQSgated[23:22] different from local
#endif

       	}
		
		if (p->frequency == 800)
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x418), 0x00000327);	// reg_TX_DLY_R1DQSgated_P1[10:8], reg_TX_DLY_R1DQSgated[6:4], reg_dly_R1DQSgated_P1[3:2], and reg_dly_R1DQSgated[1:0] different from local
		}
		else
		{
#if fcFOR_CHIP_ID == fcK2		
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x418), 0x00000432);	// reg_TX_DLY_R1DQSgated_P1[10:8], reg_TX_DLY_R1DQSgated[6:4], reg_dly_R1DQSgated_P1[3:2], and reg_dly_R1DQSgated[1:0] different from local
#endif			
#if fcFOR_CHIP_ID == fcJADE
					ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x418), 0x00000542); // reg_TX_DLY_R1DQSgated_P1[10:8], reg_TX_DLY_R1DQSgated[6:4], reg_dly_R1DQSgated_P1[3:2], and reg_dly_R1DQSgated[1:0] different from local
#endif

		}
		

#ifdef DUAL_FREQ_DIFF_RLWL
#if fcFOR_CHIP_ID == fcK2
		if ((p->frequency >= 367) && (p->frequency < 600))	// WL=3	
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x41c), 0x11112222);	
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x420), 0x11112222);	
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x424), 0x11112222);		
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x428), 0xffff5555);	
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x42c), 0x00ff0055);					
		}
		else if ((p->frequency <= 800) && (p->frequency > 600))	// WL=6
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x41c), 0x33333333);	
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x420), 0x33333333);	
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x424), 0x33333333);	
        	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x428), 0x5555ffff);	
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x42c), 0x005500ff);			
		}
		else if ((p->frequency <= 933) && (p->frequency > 800))	// WL=8
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x41c), 0x44444444);	
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x420), 0x44444444);	
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x424), 0x44444444);	
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x428), 0x5555ffff);	
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x42c), 0x005500ff);		
		}
#endif

#if fcFOR_CHIP_ID == fcJADE
		if (p->frequency != DUAL_FREQ_LOW)
		{
#ifdef NORMAL_CMD_SELPH
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x41c), 0x44444444);	
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x420), 0x44444444);	
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x424), 0x44444444);	
#else
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x41c), 0x33333333);	
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x420), 0x33333333);	
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x424), 0x33333333);	
#endif
		}
		else
		{
#ifdef NORMAL_CMD_SELPH
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x41c), 0x33333333);	
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x420), 0x33333333);	
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x424), 0x33333333);	
#else
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x41c), 0x22222222);	
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x420), 0x22222222);	
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x424), 0x22222222);	
#endif
		}
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x428), 0x5555ffff);	
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x42c), 0x005500ff);		
#endif

#else
#if fcFOR_CHIP_ID == fcK2		
		if ((p->frequency <= 933) && (p->frequency > 800))
                {
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x41c), 0x44444444);	// 112980ns
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x420), 0x44444444);	// 113011ns
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x424), 0x44444444);	// 113041ns
                }
		else
                {
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x41c), 0x33333333);	// 112980ns
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x420), 0x33333333);	// 113011ns
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x424), 0x33333333);	// 113041ns
                }
#endif

#if fcFOR_CHIP_ID == fcJADE
#ifdef NORMAL_CMD_SELPH
                    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x41c), 0x44444444); 
                    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x420), 0x44444444); 
                    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x424), 0x44444444); 
#else
                    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x41c), 0x33333333); 
                    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x420), 0x33333333); 
                    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x424), 0x33333333); 
#endif
#endif

		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x428), 0x5555ffff);	// 113071ns
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x42c), 0x005500ff);	// 113101ns
#endif		
		
#ifdef FIRST_BRING_UP
	/* bring use p->frequency = 533 but ACTiming is 1800, so use 1800's setting */
#ifdef NORMAL_CMD_SELPH
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x41c), 0x44444444); 
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x420), 0x44444444); 
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x424), 0x44444444); 
#else
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x41c), 0x33333333); 
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x420), 0x33333333); 
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x424), 0x33333333); 
#endif
#endif
	
		
	
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e0), 0x2201ffff);	// 113131ns

#ifdef DUAL_FREQ_DIFF_ACTIMING
		if (p->frequency == DUAL_FREQ_LOW)
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1f8), LPDDR3_AC_TIME_05T_1f8_LOW);	// new
		}
		else
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1f8), emi_set->DRAMC_ACTIM05T_VAL);	// new
		}
#else
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1f8), emi_set->DRAMC_ACTIM05T_VAL);	// new
#endif		
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x23c), 0x2201ffff);	// 113161ns

#if fcFOR_CHIP_ID == fcK2
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x00020091);	// 113544ns, [0] TBD
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x000200b1);	// 113904ns, [0] TBD
#endif
#if fcFOR_CHIP_ID == fcJADE
#ifndef JADE_FLOW_RUN_ON_K2_EVB
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x00022091);	// 113544ns, [0] TBD
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x000220b1); // 113544ns, [0] TBD
#else
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x00022091);	// 113544ns, [0] TBD
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x000220b1);	// 113904ns, [0] TBD
#endif
#endif

#if fcFOR_CHIP_ID == fcK2
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1dc), 0x90006442);	// enable DDRPHY dynamic clk gating
#endif		
#if fcFOR_CHIP_ID == fcJADE
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1dc), 0x9a655942 & (~(0xc2000002))); // [15:8]=R_DMTXREFCNT=8'h28, [23:16]=R_DMREFCNT_FR_CLK=8'h62, R_DMDCMEN=1, R_DMREFFRERUN=0, close dcm
#endif

		//ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x08), 0x03801600);	  // FR_CLK div 8
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x08), emi_set->DRAMC_CONF2_VAL & 0xfffc00ff);	//R_DMREFBW_FR: 0x08[17:8]=0 for DerPing's suggestion
								
#ifdef DUAL_FREQ_DIFF_ACTIMING
		if (p->frequency == DUAL_FREQ_LOW)
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x48), LPDDR3_TEST2_4_048_LOW);	// 113965ns
		}
		else
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x48), emi_set->DRAMC_TEST2_4_VAL);	// 113965ns
		}
#else
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x48), emi_set->DRAMC_TEST2_4_VAL);	// 113965ns
#endif		

#if fcFOR_CHIP_ID == fcK2
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x8c), 0x00e00001);	// 113988ns, TX pipe [23:21]
#endif
#if fcFOR_CHIP_ID == fcJADE
		// Rx23 
		//	 [21]  ADV_1T_MODE
		//	 [22]  Tx PIEP2_EN
		//	 [23]  Tx PIEP3_EN
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x8c), 0x00c00001);	// 113988ns, TX pipe [23:21]
#endif
		
#ifdef DUAL_FREQ_DIFF_ACTIMING
		if (p->frequency == DUAL_FREQ_LOW)
		{	
			if (uiDualRank) 
			{
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x110), LPDDR3_RKCFG_110_LOW  | 0x00000001);	// dual-rank enable [2:0], rank address [26:24] - cpu_address[27] for rank_1
			}
			else
			{
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x110), LPDDR3_RKCFG_110_LOW & 0xfffffffe);	// dual-rank enable [2:0], rank address [26:24] - cpu_address[27] for rank_1
			}

		}
		else
		{
			if (uiDualRank) 
			{
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x110), emi_set->DRAMC_RKCFG_VAL  | 0x00000001);	// dual-rank enable [2:0], rank address [26:24] - cpu_address[27] for rank_1
			}
			else
			{
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x110), emi_set->DRAMC_RKCFG_VAL & 0xfffffffe);	// dual-rank enable [2:0], rank address [26:24] - cpu_address[27] for rank_1
			}
		}
#else		
		if (uiDualRank) 
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x110), emi_set->DRAMC_RKCFG_VAL | 0x00000001);	// dual-rank enable [2:0], rank address [26:24] - cpu_address[27] for rank_1
		}
		else
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x110), emi_set->DRAMC_RKCFG_VAL & 0xfffffffe);	// dual-rank enable [2:0], rank address [26:24] - cpu_address[27] for rank_1
		}
#endif		

#if fcFOR_CHIP_ID == fcK2
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xd8), 0x00100110);	// 114018ns, [31:30] PINMUX TBD
#endif		
#if fcFOR_CHIP_ID == fcJADE
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xd8), 0x00100510);	// 114018ns, [31:30] PINMUX TBD
#endif
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xe4), 0x00000001);	// 114048ns, no ODT (different from local), DATLAT3[4], DDR3 RESET[1] for toggle rate
		if (p->frequency >= 800)
		{
			// Reg.138[4] tCKEH/tCKEL extend 1T
#ifdef ENABLE_MENGRU_SUGGEST_SETTING			
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x138), 0x82788c10); //[19] PBREFDISBYREFNUM = 1,Mengru Si:refresh is not enough. [11:10] DMSTBLAT = 3, Mengru Si:dqs gating glitch, [23:21]=3 for DerPing's suggestion, [15]=1, [20]=1, [25]=1 for DerPing's suggestion
#else
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x138), 0x80000c10); 
#endif
	        }
		else	
		{
#ifdef ENABLE_MENGRU_SUGGEST_SETTING		
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x138), 0x82788c00); //[19] PBREFDISBYREFNUM = 1,Mengru Si:refresh is not enough. [11:10] DMSTBLAT = 3, Mengru Si:dqs gating glitch, [23:21]=3 for DerPing's suggestion, [15]=1, [20]=1, [25]=1 for DerPing's suggestion
#else
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x138), 0x80000c00); 
#endif
		}
		
#ifdef DUAL_FREQ_K
		// Impedance calibration only do once in high frequency. Set in low frequency will overwrite the settings of calibration value.
        if (p->frequency != DUAL_FREQ_LOW)   
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xb8), 0x99709970);	// 114078ns
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xbc), 0x99709970);	// 114108ns
		}	
#else			
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xb8), 0x99709970);	// 114078ns
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xbc), 0x99709970);	// 114108ns
#endif		
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x90), 0x00000000);	// 114139ns
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x94), 0x80000000);	// 114169ns
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xdc), 0x83200200);	// 114199ns
		
	       	if(is_TB_setting()==0)
		       ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xe0), 0x14200200);	// 114229ns, R_DMDQSINCTL[27:24] different from local, TBD
			else
		       ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xe0), 0x15200200);	// 114229ns, R_DMDQSINCTL[27:24] different from local, 
		

#if fcFOR_CHIP_ID == fcK2			  	
		      if(is_TB_setting()==0)
		      ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x118), 0x00000004);	// 12584724ns, R_DMR1DQSINCTL[3:0] different from local, TBD
		      else
		      ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x118), 0x00000005);	// 12584724ns, R_DMR1DQSINCTL[3:0] different f
#endif
#if fcFOR_CHIP_ID == fcJADE	
				if(is_TB_setting()==0)
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x118), 0x00000084);   // 12584724ns, R_DMR1DQSINCTL[3:0] different from local, TBD
				else
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x118), 0x00000085);   // 12584724ns, R_DMR1DQSINCTL[3:0] different f			
#endif


		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf0), 0x02000000);	// 114259ns, DATLAT4[25]
	
#ifdef DUAL_FREQ_DIFF_ACTIMING
		if (p->frequency == DUAL_FREQ_LOW)
		{	
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf4), LPDDR3_GDDR3CTL1_0F4_LOW);	// 114289ns, R_DMPHYSYNCM
		}
		else
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf4), emi_set->DRAMC_GDDR3CTL1_VAL);	// 114289ns, R_DMPHYSYNCM
		}
#else
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf4), emi_set->DRAMC_GDDR3CTL1_VAL);	// [24]:R_DM8BKEN=1, [21]:R_DMCKE1FIXOFF=0, [20]:R_DMCKE1FIXON=0
#endif		

#if fcFOR_CHIP_ID == fcJADE
		//	   12: R_DMGTDMW_SYNC_MASK
		//	   13: R_DMGT_SYNC_MASK
		//	   14: R_DMDAT_SYNC_MASK
		//	   15: R_DMPHY_SYNC_MASK
		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xf8), &u4value);
		u4value |= 0x0000f000;
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf8), u4value);
#endif

#if fcFOR_CHIP_ID == fcK2
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x168), 0x00000000);	// 114319ns
#endif
#if fcFOR_CHIP_ID == fcJADE
		//	  14:12 R_DMMPENDREFCNT
		//		  9 R_DMWDATACNTDIS
		//		  8 R_DMRDATACNTDIS
		//	   7: 0 R_DMMAXPENDCNT
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x168), 0x00004080); // 114319ns
#endif

#if fcFOR_CHIP_ID == fcK2
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x130), 0x30000000);	// 114349ns
#endif		
#if fcFOR_CHIP_ID == fcJADE
		//	  29: reg_clk_1   DRAMC CLK1 +/-PAD (only used in PC-DDR3)
		//	  28: reg_clk_0   DRAMC CLK0 +/-PAD
		// 27-24: R_DMBANK_ARB_MASK_SEL
		// 23-20: R_DMPBC_ARB_MASK_SEL
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x130), 0x10000000); // reg_clk_1=0, reg_clk_0=1

        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xa8), 0x043f003e );
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xb0), &u4value);
		u4value &= 0x0000ffff; 
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xb0), u4value );
#endif

#if fcFOR_CHIP_ID == fcJADE
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xd8), 0x00300510);	// [31:30] PINMUX TBD
#endif

		mcDELAY_US(1);
	  
#ifdef DUAL_FREQ_DIFF_ACTIMING
		if (p->frequency == DUAL_FREQ_LOW)
		{	
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x04), LPDDR3_CONF1_004_LOW);	// 114439ns
		}
		else
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x04), emi_set->DRAMC_CONF1_VAL);	// 114439ns
		}
#else
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x04), emi_set->DRAMC_CONF1_VAL);	// 114439ns
#endif		

		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x124), 0xc0000011);	// 114469ns, R_DMDQSGDUALP[30], reg_dly_sel_rxdqs_com1[7:4], reg_dly_sel_rxdqs_com0[3:0] different from local, TBD

#if fcFOR_CHIP_ID == fcJADE
#ifdef DDRPHY_BYPASS_DMYPAD
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x124), 0xC0060000);	// 114469ns, R_DMDQSGDUALP[30], reg_dly_sel_rxdqs_com1[7:4], reg_dly_sel_rxdqs_com0[3:0] different from local, TBD
#else
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x124), 0xC0000000);	// 114469ns, R_DMDQSGDUALP[30], reg_dly_sel_rxdqs_com1[7:4], reg_dly_sel_rxdqs_com0[3:0] different from local, TBD
#endif
#endif

#if fcFOR_CHIP_ID == fcJADE
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x200), 0x00000000);
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x204), 0x00000000);
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x208), 0x00000000);
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x20c), 0x00000000);
#endif

		
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x94), 0x40404040);	// 114500ns

#if fcFOR_CHIP_ID == fcK2		
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1c0), 0x00000000);	// 114530ns, disable DQS calibration
#endif
#if fcFOR_CHIP_ID == fcJADE
#ifdef AP_SLEEP_WORK_AROUND
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1c0), 0x80000000);	//DDDD: 0x1c0[15:0]=0 for DerPing's suggestion
#else		
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1c0), 0x00000000);	//DDDD: 0x1c0[15:0]=0 for DerPing's suggestion
#endif		
#endif


#ifdef DUAL_FREQ_DIFF_ACTIMING
		if (p->frequency == DUAL_FREQ_LOW)
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x00), LPDDR3_ACTIM_000_LOW);	// 114563ns
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xfc), LPDDR3_MISCTL0_VAL_0fc_LOW);	// 114563ns
		}
		else
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x00), emi_set->DRAMC_ACTIM_VAL);	// 114563ns
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xfc), emi_set->DRAMC_MISCTL0_VAL);	// 114563ns
		}
#else
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x00), emi_set->DRAMC_ACTIM_VAL);	// 114563ns
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xfc), emi_set->DRAMC_MISCTL0_VAL);	// 114563ns
#endif		

#if fcFOR_CHIP_ID == fcK2
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1ec), 0x00100001);	// 118899ns, dual rank
#endif
#if fcFOR_CHIP_ID == fcJADE
	if (uiDualRank) {
#ifdef AP_SLEEP_WORK_AROUND	 
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1ec), 0x00101000);	// dual rank 0x7B[12]R_DMCS2RANK, [20]R_DMCKE2RANK
#else
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1ec), 0x00101001); // dual rank 0x8B[ 0]R_DMDUALSCHENx
#endif
	}
#endif

#ifdef DUAL_FREQ_DIFF_ACTIMING
		if (p->frequency == DUAL_FREQ_LOW)
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x7c), LPDDR3_DDR2CTL_07C_LOW);	// 114563ns, DATLAT210[6:4]
		}
		else
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x7c), emi_set->DRAMC_DDR2CTL_VAL);	// 114563ns, DATLAT210[6:4]
		}
#else
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x7c), emi_set->DRAMC_DDR2CTL_VAL);	// 114563ns, DATLAT210[6:4]
#endif		

#if fcFOR_CHIP_ID == fcK2
			if(is_TB_setting()==0)
		    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), 0x00f00ae0);	// 114563ns, R_DMDATLAT_DSEL[12:8]
		    else
		     ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), 0x00f20ce0);	// 114563ns, R_DMDATLAT_DSEL[12:8]
#endif
#if fcFOR_CHIP_ID == fcJADE
#ifndef JADE_FLOW_RUN_ON_K2_EVB
		     if(is_TB_setting()==0)
		    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), 0x006f0be0);	// 114563ns, R_DMDATLAT_DSEL[12:8]
		    else
		     ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), 0x00620be0);	// 114563ns, R_DMDATLAT_DSEL[12:8]
#else
		     if(is_TB_setting()==0)
		    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), 0x00f00be0);	// 114563ns, R_DMDATLAT_DSEL[12:8]
		    else
		     ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), 0x00f20be0);	// 114563ns, R_DMDATLAT_DSEL[12:8]
#endif
#endif


#if fcFOR_CHIP_ID == fcK2
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x28), 0xf1200f01);	// 114590ns
#endif		
#if fcFOR_CHIP_ID == fcJADE
		//		31: R_DMCLR_EN
		//		30: R_DMDLLFRZ
		//		29: R_DMREQQUE_DEPTH_UPD
		//		28: R_DMMANUDQS
		//
		//		19: R_DMREFNA_OPT
		//		18: R_DMDVFSMRW_EN
		//		17: R_DMSHU_DRAMCx
		//		16: R_DMSHU_LOWx
		if (p->frequency == DUAL_FREQ_LOW)
		{
#ifdef ENABLE_MENGRU_SUGGEST_SETTING		
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x28), 0xD12c0f01); //[19]REFNA_OPT = 1, Mengru Si:tcke violation.
#else
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x28), 0xD1240f01); 
#endif
		}
		else
		{
#ifdef ENABLE_MENGRU_SUGGEST_SETTING		
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x28), 0xD12c0f01); //[19]REFNA_OPT = 1, Mengru Si:tcke violation. Clr [18]R_DMDVFSMRW_EN=0
#else
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x28), 0xD1200f01); 
#endif
		}
#endif

		//dramc clk gating always no gating
		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1dc), &u4value);
		mcSET_BIT(u4value, 26);
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1dc), u4value);
		mcDELAY_US(1);

#ifdef DUAL_FREQ_DIFF_ACTIMING
		if (p->frequency == DUAL_FREQ_LOW)
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e8), LPDDR3_ACTIM1_1E8_LOW);
		}
		else
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e8), emi_set->DRAMC_ACTIM1_VAL);
		}
#else   
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e8), emi_set->DRAMC_ACTIM1_VAL);
#endif
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x158), 0x00000000);	// 114620ns
#if fcFOR_CHIP_ID == fcK2		
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x54), 0x00000001);	// 114620ns
#endif		

		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xe4), 0x00000005);	// 114650ns, turn on CKE before initial, different from local simulation, TBD

#if fcFOR_CHIP_ID == fcJADE
		// DQ Swap
		// setting MRR read data bit source mapping
		// Bit 3/2/1/0	
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1f4), 0x04010200);
		// Bit 7/6/5/4
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1fc), 0x07060305);
#endif

		
		mcDELAY_US(200);

#if fcFOR_CHIP_ID == fcK2
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1ec), 0x00100000);	// 118899ns, dual rank
#endif		
#if fcFOR_CHIP_ID == fcJADE
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1ec), 0x00100000);	// 118899ns, dual rank
#endif

		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), emi_set->iLPDDR3_MODE_REG_63);	// 116193ns
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);	// 116253ns
#ifdef MRW_WAIT_RESPONSE
		MRW_Wait_Response();
                mcDELAY_US(10);
#else
        //need to wait 10us for spec
		mcDELAY_US(10);
#endif
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);	// 116655ns

		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), emi_set->iLPDDR3_MODE_REG_10);	// 116716ns
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);	// 116776ns
#ifdef MRW_WAIT_RESPONSE
		MRW_Wait_Response();
#else
		mcDELAY_US(1);
#endif
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);	// 117178ns

		#ifdef LPDDR_MAX_DRIVE
		// MR3, driving stregth
		// for testing, set to max
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x00010003);
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);
#ifdef MRW_WAIT_RESPONSE
		MRW_Wait_Response();
#else		
		mcDELAY_US(1);
#endif
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);
		#elif defined(LPDDR_MIN_DRIVE)
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x00030003);
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);
#ifdef MRW_WAIT_RESPONSE
		MRW_Wait_Response();
#else		
		mcDELAY_US(1);
#endif
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);
		#endif

		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), emi_set->iLPDDR3_MODE_REG_1);	// 117239ns
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);	// 117299ns
#ifdef MRW_WAIT_RESPONSE
		MRW_Wait_Response();
#else		
		mcDELAY_US(1);
#endif
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);	// 117701ns

#ifdef DUAL_FREQ_DIFF_RLWL
		if (p->frequency == DUAL_FREQ_LOW)
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), LPDDR3_MODE_REG_2_LOW);	// 117762ns
		}
		else
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), emi_set->iLPDDR3_MODE_REG_2);	// 117762ns
		}
#else
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), emi_set->iLPDDR3_MODE_REG_2);	// 117762ns
#endif		

		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);	// 117822ns
#ifdef MRW_WAIT_RESPONSE
		MRW_Wait_Response();
#else		
		mcDELAY_US(1);
#endif
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);	// 117822ns
		
#if fcFOR_CHIP_ID == fcK2		
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x0000000b);	// 117762ns
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);	// 117822ns
#ifdef MRW_WAIT_RESPONSE
		MRW_Wait_Response();
#else		
		mcDELAY_US(1);
#endif
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);	// 117822ns
#endif
#if fcFOR_CHIP_ID == fcJADE
		//ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x54), 0x0000001c);	// 117762ns
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x54), 0x00000001);	//BBBB: 0x54[7:0]=1 for DerPing's suggestion

		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x0000000b);	// 117762ns
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001); // 117822ns
#ifdef MRW_WAIT_RESPONSE
		MRW_Wait_Response();
#else		
		mcDELAY_US(1);
#endif
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000); // 117822ns

//can't CKE low because now CKE1toRANK0		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xe4), 0x00008001);	// [15]:R_DMSTBENCMPEN	
#endif


		if (uiDualRank) {
#ifdef DUAL_FREQ_DIFF_ACTIMING
			if (p->frequency == DUAL_FREQ_LOW)
			{
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf4), 0x00100000 | LPDDR3_GDDR3CTL1_0F4_LOW);	// 37270775ns
			}
			else
			{
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf4), 0x00100000 | emi_set->DRAMC_GDDR3CTL1_VAL);	// 37270775ns
			}
#else
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf4), 0x00100000 | emi_set->DRAMC_GDDR3CTL1_VAL);	// 37270775ns
#endif

            mcDELAY_US(200);     // tINIT3 > 200us


			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x10000000 | emi_set->iLPDDR3_MODE_REG_63);	// 37372970ns
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);	// 37388346ns
#ifdef MRW_WAIT_RESPONSE
			MRW_Wait_Response();
            mcDELAY_US(10);
#else			
            //need to wait 10us for spec
			mcDELAY_US(10);
#endif
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);	// 37409274ns

			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x10000000 | emi_set->iLPDDR3_MODE_REG_10);	// 37427213ns
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);	// 37444297ns
#ifdef MRW_WAIT_RESPONSE
			MRW_Wait_Response();
#else			
			mcDELAY_US(1);
#endif
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);	// 37465226ns

			#ifdef LPDDR_MAX_DRIVE
			// MR3, driving stregth
			// for testing, set to max
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x10010003);
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);
#ifdef MRW_WAIT_RESPONSE
			MRW_Wait_Response();
#else			
			mcDELAY_US(1);
#endif
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);
			#elif defined(LPDDR_MIN_DRIVE)
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x10030003);
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);
#ifdef MRW_WAIT_RESPONSE
			MRW_Wait_Response();
#else			
			mcDELAY_US(1);
#endif
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);
			#endif

			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x10000000 | emi_set->iLPDDR3_MODE_REG_1);	// 37482737ns
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);	// 37625819ns
#ifdef MRW_WAIT_RESPONSE
			MRW_Wait_Response();
#else			
			mcDELAY_US(1);
#endif
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);	// 37645039ns

#ifdef DUAL_FREQ_DIFF_RLWL
			if (p->frequency == DUAL_FREQ_LOW)
			{
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x10000000 | LPDDR3_MODE_REG_2_LOW);	// 37662978ns
			}
			else
			{
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x10000000 | emi_set->iLPDDR3_MODE_REG_2);	// 37662978ns
			}
#else
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x10000000 | emi_set->iLPDDR3_MODE_REG_2);	// 37662978ns
#endif			
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);	// 37680063ns
#ifdef MRW_WAIT_RESPONSE
			MRW_Wait_Response();
#else			
			mcDELAY_US(1);
#endif
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);	// 37700991ns
			
#if fcFOR_CHIP_ID == fcK2		
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x1000000b);	// 117762ns
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001); // 117822ns
#ifdef MRW_WAIT_RESPONSE
			MRW_Wait_Response();
#else			
			mcDELAY_US(1);
#endif
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000); // 117822ns
#endif
#if fcFOR_CHIP_ID == fcJADE
//			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x54), 0x0000001c);	// 117762ns
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x54), 0x00000001);	//BBBB: 0x54[7:0]=1 for DerPing's suggestion

			
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x1000000b);	// 117762ns
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001); // 117822ns
#ifdef MRW_WAIT_RESPONSE
			MRW_Wait_Response();
#else			
			mcDELAY_US(1);
#endif
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000); // 117822ns
#endif
			

#ifdef DUAL_FREQ_DIFF_ACTIMING
			if (p->frequency == DUAL_FREQ_LOW)
			{
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf4), LPDDR3_GDDR3CTL1_0F4_LOW);	// 37806675ns
			}
			else
			{
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf4), emi_set->DRAMC_GDDR3CTL1_VAL);	// 37806675ns
			}
#else
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf4), emi_set->DRAMC_GDDR3CTL1_VAL);	// 37806675ns
#endif			
		}

#ifdef DUAL_FREQ_DIFF_ACTIMING
		if (p->frequency == DUAL_FREQ_LOW)
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1dc), LPDDR3_PD_CTRL_1DC_LOW & (~(0xc2000002)));	//close dcm
		}
		else
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1dc), emi_set->DRAMC_PD_CTRL_VAL & (~(0xc2000002)));	//close dcm
		}
#else
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1dc), emi_set->DRAMC_PD_CTRL_VAL & (~(0xc2000002)));	//close dcm
#endif


#if fcFOR_CHIP_ID == fcK2
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xe4), 0x00000001);	// 114650ns, turn on CKE before initial, different from local simulation, TBD
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1ec), 0x00100001);	// 118899ns, sync rome's esl setting
#endif
#if fcFOR_CHIP_ID == fcJADE
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xe4), 0x00008001);   // [15]:R_DMSTBENCMPEN
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1ec), 0x0012cf11);	// 0x7B[19]R_DMREORDEREN=0, [20]R_DMCKE2RANK=1 for DerPing's suggestion
#endif


        //dramc clk gating controlled by dramc
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1dc), &u4value);
        mcCLR_BIT(u4value, 26);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1dc), u4value);


		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x84), 0x00000a56);	// 118929ns

#if fcFOR_CHIP_ID == fcJADE
#ifndef JADE_FLOW_RUN_ON_K2_EVB
		if(is_TB_setting()==0)
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), 0x006f0be0);	// 114563ns, R_DMDATLAT_DSEL[12:8]
		else
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), 0x00620be0);	// 114563ns, R_DMDATLAT_DSEL[12:8]
#else
		if(is_TB_setting()==0)
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), 0x00f00be0);	// 114563ns, R_DMDATLAT_DSEL[12:8]
		else
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), 0x00f20be0);	// 114563ns, R_DMDATLAT_DSEL[12:8]
#endif
#endif


#ifdef DUAL_FREQ_K
		// This register has no shuffle registers and is used in calibration. So only K in high frequency and use high frequency settings.
        if (p->frequency != DUAL_FREQ_LOW)   
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x0c), 0x00000000);	// 118993ns
		}
#else		
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x0c), 0x00000000);	// 118993ns
#endif	
#ifdef DUAL_FREQ_DIFF_ACTIMING
		if (p->frequency == DUAL_FREQ_LOW)
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x44), LPDDR3_TEST2_3_044_LOW);	// 119050ns
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x08), LPDDR3_CONF2_008_LOW  & 0xfffc00ff);	//R_DMREFBW_FR: 0x08[17:8]=0 for DerPing's suggestion
		}
		else
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x44), emi_set->DRAMC_TEST2_3_VAL);	// 119050ns
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x08), emi_set->DRAMC_CONF2_VAL  & 0xfffc00ff);	//R_DMREFBW_FR: 0x08[17:8]=0 for DerPing's suggestion
		}
#else
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x44), emi_set->DRAMC_TEST2_3_VAL);	// 119050ns
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x08), emi_set->DRAMC_CONF2_VAL  & 0xfffc00ff);	//R_DMREFBW_FR: 0x08[17:8]=0 for DerPing's suggestion
#endif		

		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x10), 0x00000000);	// 119139ns
#if fcFOR_CHIP_ID == fcJADE
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x14), 0x00000000);	// 119139ns
#endif
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x100), 0x00008110);	// 119169ns
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1c), 0x12121212);	// 119200ns
#if fcFOR_CHIP_ID == fcK2
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf8), 0x00000000);	// 119230ns
#endif		
#if fcFOR_CHIP_ID == fcJADE
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf8), 0x00002000);	//R_DMGT_SYNC_MASK: 0xf8[13]=1 for DerPing's suggestion
#endif


#if fcFOR_CHIP_ID == fcJADE
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xc4), &u4value);
		u4value &= ~0x00000001;  // Clr Rx31[0] to Zero. by SY Cheng's Request
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xc4), u4value);	// 119139ns
#endif

		#if 0
		// Delay default value.
		// DQS RX input delay
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x18), 0x08080908);
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1c), 0x08080908);		// Need to adjust further
		// DQ input delay
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x210), 0x01010300);
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x214), 0x06030002);
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x218), 0x01010201);
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x21c), 0x03020002);
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x220), 0x00010103);
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x224), 0x02010201);
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x228), 0x02040200);
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x22c), 0x02020201);
		#endif
	}
	else
	{
		mcSHOW_ERR_MSG(("unknow dram type  should be lpddr2 or lpddr3 \n"));
		return DRAM_FAIL;
	}

	// K2?? TDN default values?
#ifdef CLKTDN_ENABLE
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xd4), &u4value);
        mcSET_BIT(u4value, 1);	// CLKTDN
        mcSET_BIT(u4value, 31);	// DS3TDN
        mcSET_BIT(u4value, 29);	// DS2TDN
        mcSET_BIT(u4value, 27);	// DS1TDN
        mcSET_BIT(u4value, 25);	// DS0TDN
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xd4), u4value);     
#endif       

	DramcEnterSelfRefresh(p, 1);
#ifdef DUAL_FREQ_K
	// This register has no shuffle registers and is used in calibration. So only K in high frequency and use high frequency settings.
    if (p->frequency != DUAL_FREQ_LOW)   
	{
		// Duty default value.
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x148), 0x10010000);     
	}
#else
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x148), 0x10010000);     
#endif		

	mcDELAY_US(20);	// Wait PLL lock.
	DramcEnterSelfRefresh(p, 0);
	
#ifdef REFTHD_ADJUST_0
	// Clear R_DMREFTHD(dramc AO) 0x8[26:24]=0 for reduce special command (MR4) wait refresh queue time. 
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x08), &u4value);
	u4value &= 0xf8ffffff;
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x08), u4value);     
#endif

#ifdef SEQ_FREERUN
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x138), &u4value);
	u4value |= (0x01 << 7);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x138), u4value);     
#endif

#ifdef DDRCLK_NOGATING
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1dc), &u4value);
	u4value |= (0x01 << 26);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1dc), u4value);     
#endif

#ifdef TWPST_ENLARGE
    if (p->dram_type == TYPE_LPDDR3)
    {     
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1ec), &u4value);
	u4value |= (0x01 << 17);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1ec), u4value);
    }         
#endif

#ifdef MAX_DCMDLYREF
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1dc), &u4value);
	u4value = u4value | 0x00000070;
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1dc), u4value);     
#endif

#ifdef DISABLE_FASTWAKE
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1e8), &u4value);
        mcCLR_BIT(u4value, 31);	
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e8), u4value);     
#endif

#ifdef MAX_SREF_CK_DLY
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x04), &u4value);
       u4value |= 0xC0000000;	// [31:30] SREF_CK_DLY = 11b.	
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x04), u4value);     
#endif

	// Duty default value.
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x148), 0x10010000);     

#if fcFOR_CHIP_ID == fcK2
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x000220b1);  //0x1000F640[13] = 1 -> ALL_CLK_ECO_EN = 1, [13]RG_E2_ECO_EN = 1, Mengru Si:reset mode phase error    
#endif  
  
	if (ucstatus)
	{
		mcSHOW_ERR_MSG(("register access fail!\n"));
		return DRAM_FAIL;
	}
	else
	{
		return DRAM_OK;
	}
}


DRAM_STATUS_T Dram_Reset(DRAMC_CTX_T *p, unsigned char use_Hynix_test_mode)
{
    // This function is implemented based on DE's bring up flow for DRAMC
    
    U8 ucstatus = 0;
    U32 u4value;
    EMI_SETTINGS *emi_set;
    
    if(emi_setting_index == -1)
        emi_set = default_emi_setting;
    else
        emi_set = &emi_settings[emi_setting_index];	

    //dramc clk gating always no gating
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1dc), &u4value);
    mcSET_BIT(u4value, 26);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1dc), u4value);
    mcDELAY_US(1);
    

    // CKEFIXON before MRW
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xe4), &u4value);
    mcSET_BIT(u4value, 2);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xe4), u4value);
    
    mcDELAY_US(200);     // tINIT3 > 200us
                
#if 1
        // MR63 -> Reset
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), emi_set->iLPDDR3_MODE_REG_63);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);
#ifdef MRW_WAIT_RESPONSE		
		MRW_Wait_Response();
        mcDELAY_US(10);    // Wait >=10us if not check DAI.
#else		
        mcDELAY_US(10);    // Wait >=10us if not check DAI.
#endif        
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);        


		/* if dram vendor is Hynix */
		if (use_Hynix_test_mode == 1)
		{	
			Hynix_Test_Mode(p, 0);
		}




        // MR10 -> ZQ Init
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), emi_set->iLPDDR3_MODE_REG_10);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);
#ifdef MRW_WAIT_RESPONSE		
		MRW_Wait_Response();
#else		
        mcDELAY_US(1);                      // tZQINIT>=1us
#endif        
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);

    #ifdef LPDDR_MAX_DRIVE
        // MR3, driving stregth
        // for testing, set to max
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x00010003);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);
#ifdef MRW_WAIT_RESPONSE		
		MRW_Wait_Response();
#else		
        mcDELAY_US(1);
#endif
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);
    #elif defined(LPDDR_MIN_DRIVE)
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x00030003);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);
#ifdef MRW_WAIT_RESPONSE		
		MRW_Wait_Response();
#else		
        mcDELAY_US(1);
#endif
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);
    #endif

        // MR1     
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), emi_set->iLPDDR3_MODE_REG_1);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);
#ifdef MRW_WAIT_RESPONSE		
		MRW_Wait_Response();
#else		
        mcDELAY_US(1);
#endif
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);

        // MR2
#ifdef DUAL_FREQ_DIFF_RLWL
		if (p->frequency == DUAL_FREQ_LOW)
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), LPDDR3_MODE_REG_2_LOW);	// 117762ns
		}
		else
		{
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), emi_set->iLPDDR3_MODE_REG_2); // 117762ns
		}
#else
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), emi_set->iLPDDR3_MODE_REG_2); // 117762ns
#endif		
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);
#ifdef MRW_WAIT_RESPONSE
		MRW_Wait_Response();
#else
        mcDELAY_US(1);
#endif
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);

#if fcFOR_CHIP_ID == fcK2
        // MR11, ODT disable.
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x0000000b);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);
#ifdef MRW_WAIT_RESPONSE
		MRW_Wait_Response();
#else		
        mcDELAY_US(1);
#endif
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);
#endif

#if fcFOR_CHIP_ID == fcJADE
				// MR11, ODT disable.
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x0000000b);
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);
#ifdef MRW_WAIT_RESPONSE
				MRW_Wait_Response();
#else		
				mcDELAY_US(1);
#endif
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);
#endif

#endif



#if 1
#ifdef DUAL_RANKS

                if (uiDualRank)
                {
#ifdef DUAL_FREQ_DIFF_ACTIMING			
						if (p->frequency == DUAL_FREQ_LOW)			
						{				
							ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf4), 0x00100000 | LPDDR3_GDDR3CTL1_0F4_LOW);	// 37270775ns			
						}			
						else			
						{				
							ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf4), 0x00100000 | emi_set->DRAMC_GDDR3CTL1_VAL);	// 37270775ns			
						}
#else			
						ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf4), 0x00100000 | emi_set->DRAMC_GDDR3CTL1_VAL);	// 37270775ns
#endif

                        mcDELAY_US(200);     // tINIT3 > 200us


				
                        // MR63 -> Reset
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x10000000 | emi_set->iLPDDR3_MODE_REG_63);
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);
#ifdef MRW_WAIT_RESPONSE
						MRW_Wait_Response();
                        mcDELAY_US(10);    // Wait >=10us if not check DAI.
#else
                        mcDELAY_US(10);    // Wait >=10us if not check DAI.
#endif                        
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);        


		/* if dram vendor is Hynix */
		if (use_Hynix_test_mode == 1)
		{	
			Hynix_Test_Mode(p, 1);
		}




                        // MR10 -> ZQ Init
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x10000000 | emi_set->iLPDDR3_MODE_REG_10);
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);
#ifdef MRW_WAIT_RESPONSE
						MRW_Wait_Response();
#else						
                        mcDELAY_US(1);                      // tZQINIT>=1us
#endif                        
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);

                    #ifdef LPDDR_MAX_DRIVE
                        // MR3, driving stregth
                        // for testing, set to max
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x10010003);
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);
#ifdef MRW_WAIT_RESPONSE
						MRW_Wait_Response();
#else						
                        mcDELAY_US(1);
#endif
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);
                    #elif defined(LPDDR_MIN_DRIVE)
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x10030003);
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);
#ifdef MRW_WAIT_RESPONSE
						MRW_Wait_Response();
#else						
                        mcDELAY_US(1);
#endif
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);
#endif

                        // MR1 
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x10000000 | emi_set->iLPDDR3_MODE_REG_1);
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);
#ifdef MRW_WAIT_RESPONSE
						MRW_Wait_Response();
#else						
                        mcDELAY_US(1);
#endif
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);

                        // MR2
#ifdef DUAL_FREQ_DIFF_RLWL
						if (p->frequency == DUAL_FREQ_LOW)
						{
							ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x10000000 | LPDDR3_MODE_REG_2_LOW);	// 117762ns
						}
						else
						{
							ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x10000000 | emi_set->iLPDDR3_MODE_REG_2); // 117762ns
						}
#else
						ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x10000000 | emi_set->iLPDDR3_MODE_REG_2); // 117762ns
#endif
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);
#ifdef MRW_WAIT_RESPONSE
						MRW_Wait_Response();
#else
                        mcDELAY_US(1);
#endif
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);
                        
#if fcFOR_CHIP_ID == fcK2					
                        // MR11, ODT disable.
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x1000000b);
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001);
#ifdef MRW_WAIT_RESPONSE
						MRW_Wait_Response();
#else						
                        mcDELAY_US(1);
#endif
                        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000);
#endif

#if fcFOR_CHIP_ID == fcJADE
						ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x1000000b);	// 117762ns			
						ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000001); // 117822ns
#ifdef MRW_WAIT_RESPONSE			
						MRW_Wait_Response();
#else						
						mcDELAY_US(1);
#endif			
						ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), 0x00000000); // 117822ns
#endif			

#ifdef DUAL_FREQ_DIFF_ACTIMING			
						if (p->frequency == DUAL_FREQ_LOW)			
						{				
							ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf4), LPDDR3_GDDR3CTL1_0F4_LOW);	// 37806675ns			
						}			
						else			
						{				
							ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf4), emi_set->DRAMC_GDDR3CTL1_VAL);	// 37806675ns			
						}
#else			
						ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf4), emi_set->DRAMC_GDDR3CTL1_VAL);	// 37806675ns#endif	
#endif						


                }

#endif
#endif

        // release CKEFIXON
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xe4), &u4value);
       mcCLR_BIT(u4value, 2);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xe4), u4value);
    
    //dramc clk gating controlled by dramc
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1dc), &u4value);
    mcCLR_BIT(u4value, 26);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1dc), u4value);



    
    if (ucstatus)
    {
        mcSHOW_ERR_MSG(("register access fail!\n"));
        return DRAM_FAIL;
    }
    else
    {
        return DRAM_OK;
    }
}



void DramcEnterSelfRefresh(DRAMC_CTX_T *p, U8 op)
{
U8 ucstatus = 0;
U32 uiTemp;

    if (op == 1) // enter self refresh
    {
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF1), &uiTemp);
        mcSET_BIT(uiTemp, POS_CONF1_SELFREF);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF1), uiTemp);
        mcDELAY_US(2);
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), &uiTemp);
        while ( (mcTEST_BIT(uiTemp, POS_SPCMDRESP_SREF_STATE))==0)
        {
            mcSHOW_DBG_MSG3(("Still not enter self refresh...\n"));
    	    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), &uiTemp);
        }
    }
    else // exit self refresh
    {
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF1), &uiTemp);
        mcCLR_BIT(uiTemp, POS_CONF1_SELFREF);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF1), uiTemp);
        mcDELAY_US(2);
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), &uiTemp);
        while ( (mcTEST_BIT(uiTemp, POS_SPCMDRESP_SREF_STATE))!=0)
        {
            mcSHOW_DBG_MSG3(("Still not exit self refresh...\n"));
    	    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), &uiTemp);
        }
    }

    // tREFI/4 may cause self refresh fail. Set to tREFI manual first. After SF, switch back.
//    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x114), uiReg114h);         
}

void Reg_Sync_Writel(unsigned int addr,unsigned int val)
{
	(*(volatile unsigned int *)(addr)) = val;
	dsb();
}

unsigned int Reg_Readl(unsigned int addr)
{
	return (*(volatile unsigned int *)(addr));
}


void DramcSwitchFreq(DRAMC_CTX_T *p)
{
	U8 ucstatus = 0;
	U32 u4value;
	U32 read_data;
	U32 bak_data1;
	U32 bak_data2;
	U32 bak_data3;
	U32 bak_data4;

	mcSHOW_DBG_MSG(("Switch frequency to %d...\n", p->frequency));

	DramcEnterSelfRefresh(p, 1); // enter self refresh
	
    if (p->frequency != DUAL_FREQ_LOW)   
	{
		// Shuffle to high


#if fcFOR_CHIP_ID == fcJADE
#if 1
		Reg_Sync_Writel((MEM_DCM_CTRL), (0x1f << 21) | (0x0 << 16) | (0x01 << 9) | (0x01 << 8) | (0x00 << 7) | (0x0 << 6) | (0x1f<<1));
		Reg_Sync_Writel((MEM_DCM_CTRL), Reg_Readl(MEM_DCM_CTRL) |0x01);
		Reg_Sync_Writel((MEM_DCM_CTRL), Reg_Readl(MEM_DCM_CTRL) & (~0x01));	
#endif        

#if 1
		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x614), &u4value);
		mcSET_BIT(u4value, 0);	// set [0]:RG_MEMPLL2_EN, [23]:MEMPLL2_AUTOK_EN	
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), u4value);						

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x62c), &u4value);
		mcSET_BIT(u4value, 0);	// set [0]:RG_MEMPLL4_EN, [23]:MEMPLL4_AUTOK_EN
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62c), u4value);						

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x620), &u4value);
		mcSET_BIT(u4value, 0);	// set [0]:RG_MEMPLL3_EN, [23]:MEMPLL3_AUTOK_EN
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), u4value);												


		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x618), &u4value);
		mcSET_BIT(u4value, 16);	// Set RG_MEMPLL2_RESERVE0(RG_MEMPLL2_RST)[16]
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), u4value);		

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x630), &u4value);
		mcSET_BIT(u4value, 16);	// Set RG_MEMPLL4_RESERVE0(RG_MEMPLL2_RST)[16]
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), u4value);		

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x624), &u4value);
		mcSET_BIT(u4value, 16);	// Set RG_MEMPLL3_RESERVE0(RG_MEMPLL2_RST)[16]
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), u4value);

        mcDELAY_US(2);

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x618), &u4value);
		mcCLR_BIT(u4value, 16);	// Clr RG_MEMPLL2_RESERVE0(RG_MEMPLL2_RST)[16]
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), u4value);		

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x630), &u4value);
		mcCLR_BIT(u4value, 16);	// Clr RG_MEMPLL4_RESERVE0(RG_MEMPLL2_RST)[16]
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), u4value);		

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x624), &u4value);
		mcCLR_BIT(u4value, 16);	// Clr RG_MEMPLL3_RESERVE0(RG_MEMPLL2_RST)[16]
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), u4value);

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x60c), &u4value);
		mcSET_BIT(u4value, 7);	// Set [7]:RG_MEMPLL_BIAS_LPF_EN
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x60c), u4value);				

        mcDELAY_US(13);

#endif		
#endif		

		bak_data1 = Reg_Readl((CHA_DRAMCAO_BASE + (0x1dc)));

#if fcFOR_CHIP_ID == fcJADE
		Reg_Sync_Writel((CHA_DRAMCAO_BASE + (0x1dc)), bak_data1 & ~(0xc2000002));	// clr [31]:R_DMCOMBCLKCTRL/[30]:R_DMPHYCLKDYNGEN to 0, Disable Dynamic Clock Gating
#endif

		Reg_Sync_Writel((CLK_CFG_0_CLR), 0x300);	// /u_top_ap_clk_ctrl/u_cksys_reg 
		// 3PLL_Mode
		Reg_Sync_Writel((CLK_CFG_0_SET), 0x100);	// Set [9:8] to be 2'b01, dmpll_ck_buf  (from PLL4)

		// need to wait 16us here for PLL output stable
		// still allow other master to access DRAM here
		mcDELAY_US(16);

		bak_data2 = Reg_Readl((CHA_DDRPHY_BASE + (0x640)));
#if fcFOR_CHIP_ID == fcJADE		
		bak_data4 = Reg_Readl((SPM_POWER_ON_VAL0));
#endif


		// [3:0]=1 : VCO/4 . [4]=0 : RG_MEMPLL_ONEPLLSEL. [12:5] RG_MEMPLL_RSV. RG_MEMPLL_RSV[1]=1
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x698)), 0x00001e20 | new_DMYPAD_option| gENABLE_MEMPLL_CG_TO_SAVE_POWER_val); 



		Reg_Sync_Writel((CLK_CFG_UPDATE), 0x02);

		// toggle DIV2_SYNC
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x640)), bak_data2 & (~0x01)); 	//sync = 1
    	mcDELAY_US(1);
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x640)), bak_data2 | 0x01);		// sync back to original. Should be 0. Bu in case of SPM control, need to make sure SPM is not toggling.

		Reg_Sync_Writel((CHA_DRAMCAO_BASE + (0x1dc)), bak_data1);


#if 0       
		Reg_Sync_Writel((MEM_DCM_CTRL), (0x1f << 21) | (0x0 << 16) | (0x01 << 9) | (0x01 << 8) | (0x00 << 7) | (0x0 << 6) | (0x1f<<1));
		Reg_Sync_Writel((MEM_DCM_CTRL), Reg_Readl(MEM_DCM_CTRL) |0x01);
		Reg_Sync_Writel((MEM_DCM_CTRL), Reg_Readl(MEM_DCM_CTRL) & (~0x01));
#endif
		
#if 0 
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), (0x01 << 28) | (0x1f << 21) | (0x0 << 16) | (0x01 << 9) | (0x01 << 8) | (0x0 << 7) | (0x0 << 6) | (0x1f << 1));
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) | 0x01);
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) & (~0x01));
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) |(0x01 << 31));
#endif        



#if fcFOR_CHIP_ID == fcJADE
		// Rx1a6: [8:5] RG_MEMPLL_RSV
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x698)), 0x00001e20 | new_DMYPAD_option | gENABLE_MEMPLL_CG_TO_SAVE_POWER_val); 	 // Set CLKSW=0, 4xck = VCO/4, Clear all [12:5] RG_MEMPLL_RSV, [4]:ONEPLLMODE=0 
#endif

	}
	else
	{
	
		
#if fcFOR_CHIP_ID == fcJADE
		// Set CLKSW=0, 4xck = VCO/4, [12:5]RG_MEMPLL_RSV[0]=1, Enabel Base PLL clock output, [4]:ONEPLL=0, [16]=0, [19/18/17]=1
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x698)), 0x000e0020 | new_DMYPAD_option  | gENABLE_MEMPLL_CG_TO_SAVE_POWER_val);
#endif


#if 0
#if fcFOR_CHIP_ID == fcJADE
		Reg_Sync_Writel((MEM_DCM_CTRL), (0x1f << 21) | (0x0 << 16) | (0x01 << 9) | (0x01 << 8) | (0x00 << 7) | (0x0 << 6) | (0x1f<<1));	// 3pll_mode: dcm_en = 0
		Reg_Sync_Writel((MEM_DCM_CTRL), Reg_Readl(MEM_DCM_CTRL) |0x01);
		Reg_Sync_Writel((MEM_DCM_CTRL), Reg_Readl(MEM_DCM_CTRL) & (~0x01));
		
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) | (0x01 << 31)); // gate off the 1pll_mode clock before change the mux sel
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) | (0x01 << 28) | (0x1f << 21) | (0x0 << 16) | (0x01 << 9) | (0x01 << 8) | (0x0 << 7) | (0x0 << 6) | (0x1f << 1));	// 1pll_mode: dcm_en=0
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) &  (~(0x01 << 31)));	// gate on  the 1pll_mode clock before change the mux sel
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) | 0x01);
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) & (~0x01));
#endif		
#endif		
		

	
		
		bak_data1 = Reg_Readl((CHA_DRAMCAO_BASE + (0x1dc)));
		Reg_Sync_Writel((CHA_DRAMCAO_BASE + (0x1dc)), bak_data1 & (~0xc2000002));	// set R_DMCOMBCLKCTRL/R_DMPHYCLKDYNGEN to 0

		// /u_top_ap_clk_ctrl/u_cksys_reg 
		Reg_Sync_Writel((CLK_CFG_0_CLR), 0x300);	// Clr [9:8] to be 2'b00
		// 1PLL Mode
		Reg_Sync_Writel((CLK_CFG_0_SET), 0x200);	// Set [9:8] to be 2'b10, ddr_x1_ck_buf (from Base PLL)

		// need to wait 16us here for PLL output stable
		// still allow other master to access DRAM here
		mcDELAY_US(16);        

		bak_data2 = Reg_Readl((CHA_DDRPHY_BASE + (0x640)));

		Reg_Sync_Writel((CLK_CFG_UPDATE), 0x02);

#if fcFOR_CHIP_ID == fcJADE
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x698)), 0x00008030 | new_DMYPAD_option | gENABLE_MEMPLL_CG_TO_SAVE_POWER_val);  // Set CLKSW=1, 4xck = VCO/4, [12:5]:RG_MEMPLL_RSV[0]=1, [4]:ONEPLL_MODE=1, [15]=[4]
#endif	

		// toggle DIV2_SYNC
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x640)), bak_data2 & (~0x01));	// Clr [5] PHYCLKP0ENB
		mcDELAY_US(1);
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x640)), bak_data2 | 0x01);		// Set [5] PHYCLKP0ENB

#if fcFOR_CHIP_ID == fcJADE
#ifdef DRAMC_DRAMPHY_DCM_ENABLE
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) | ((0x01 << 28) | (0x1f << 21) | (0x0 << 16) | (0x01 << 9) | (0x01 << 8) | (0x01 << 7) | (0x0 << 6) | (0x1f << 1)));	// 1pll_mode: dcm_en=1
#else
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) | ((0x01 << 28) | (0x1f << 21) | (0x0 << 16) | (0x01 << 9) | (0x01 << 8) | (0x00 << 7) | (0x0 << 6) | (0x1f << 1)));	// 1pll_mode: dcm_en=1
#endif		
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) | 0x01);
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) & (~0x01));

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x60c), &u4value);
		mcCLR_BIT(u4value, 7);	// Clr [7]:RG_MEMPLL_BIAS_LPF_EN
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x60c), u4value);				

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x614), &u4value);
		mcCLR_BIT(u4value, 0);	// Clr [0]:RG_MEMPLL2_EN, [23]:MEMPLL2_AUTOK_EN
		mcCLR_BIT(u4value, 23);		
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), u4value);						

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x62c), &u4value);
		mcCLR_BIT(u4value, 0);	// Clr [0]:RG_MEMPLL4_EN, [23]:MEMPLL4_AUTOK_EN
		mcCLR_BIT(u4value, 23);		
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62c), u4value);						

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x620), &u4value);
		mcCLR_BIT(u4value, 0);	// Clr [0]:RG_MEMPLL3_EN, [23]:MEMPLL3_AUTOK_EN
		mcCLR_BIT(u4value, 23);		
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), u4value);										
#endif


		Reg_Sync_Writel((CHA_DRAMCAO_BASE + (0x1dc)), bak_data1);


	}
	DramcEnterSelfRefresh(p, 0); // enter self refresh

	//mcSHOW_DBG_MSG(("mt_pll_post_init: mt_get_mem_freq = %dKhz\n", mt_get_bus_freq()));
}

void DramcShuffle(DRAMC_CTX_T *p)
{
	U8 ucstatus = 0;
    EMI_SETTINGS *emi_set;
    	// Shuffle to high
	U32 read_data;
	U32 bak_data1;
	U32 bak_data2;
	U32 bak_data3;
	U32 bak_data4;
    
    if(emi_setting_index == -1)
        emi_set = default_emi_setting;
    else
        emi_set = &emi_settings[emi_setting_index];  

	mcSHOW_DBG_MSG(("Switch frequency to %d...\n", p->frequency));

    if (p->frequency != DUAL_FREQ_LOW)   
	{
		// MR2 RL/WL set
		#ifdef DUAL_FREQ_DIFF_RLWL
	            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), emi_set->iLPDDR3_MODE_REG_2);
		#else
				ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), emi_set->iLPDDR3_MODE_REG_2);
		#endif

		// Shuffle to high start.
	        // Reg.28h[17]=1 R_DMSHU_DRAMC
	        // Reg.28h[16]=0 R_DMSHU_LOW
		bak_data3 = Reg_Readl((CHA_DRAMCAO_BASE + (0x028)));
		bak_data3 = (bak_data3 & (~0x00030000)) | 0x20000;
		// Block EMI start.
		Reg_Sync_Writel((CHA_DRAMCAO_BASE + (0x028)), bak_data3);
		
		// Wait shuffle_end Reg.16ch[0] == 1
	        read_data = Reg_Readl((CHA_DRAMCAO_BASE + (0x16c)));
	        while ((read_data & 0x01)  != 0x01)
	        {
		        read_data = Reg_Readl((CHA_DRAMCAO_BASE + (0x16c)));
	        }

		//block EMI end.
		Reg_Sync_Writel((CHA_DRAMCAO_BASE + (0x028)), bak_data3 & (~0x20000));
	}
	else
	{

		// MR2 RL/WL set
		#ifdef DUAL_FREQ_DIFF_RLWL
	        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), LPDDR3_MODE_REG_2_LOW);
		#else
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), emi_set->iLPDDR3_MODE_REG_2);
		#endif

		// Shuffle to low. block EMI start.
	        // Reg.28h[17]=1 R_DMSHU_DRAMC
	        // Reg.28h[16]=1 R_DMSHU_LOW
		bak_data3 = Reg_Readl((CHA_DRAMCAO_BASE + (0x028)));
		bak_data3 = bak_data3 | 0x30000;
		Reg_Sync_Writel((CHA_DRAMCAO_BASE + (0x028)), bak_data3),
		
		// Wait shuffle_end Reg.16ch[0] == 1
	        read_data = Reg_Readl((CHA_DRAMCAO_BASE + (0x16c)));
	        while ((read_data & 0x01)  != 0x01)
	        {
		        read_data = Reg_Readl((CHA_DRAMCAO_BASE + (0x16c)));
	        }

		// Block EMI end.
		Reg_Sync_Writel((CHA_DRAMCAO_BASE + (0x028)), bak_data3 & (~0x20000));
	}
	//mcSHOW_DBG_MSG(("mt_pll_post_init: mt_get_mem_freq = %dKhz\n", mt_get_bus_freq()));
}
	        
void DramcDFS(DRAMC_CTX_T *p, U32 Init)
{
	U8 ucstatus = 0;
	U32 u4value, u4HWTrackR0, u4HWTrackR1, u4HWGatingEnable;
	U32 read_data;
	U32 bak_data1;
	U32 bak_data2;
	U32 bak_data3;
	U32 bak_data4;
#ifdef TEST_DFS_DMABUSY
	U32 uiCmpErr;
	static U32 uiOffset;
#endif
    EMI_SETTINGS *emi_set;
    
#ifndef DUAL_FREQ_K
    return;
#endif
    
    if(emi_setting_index == -1)
        emi_set = default_emi_setting;
    else
        emi_set = &emi_settings[emi_setting_index];  

	mcSHOW_DBG_MSG(("Switch frequency to %d...\n", p->frequency));

	//DramcEnterSelfRefresh(p, 1); // enter self refresh
	//mcDELAY_US(1);
	//Read back HW tracking first. After shuffle finish, need to copy this value into SW fine tune.

	if (Init!=0)
	{
		u4HWGatingEnable = 0;

#ifdef FRCLK_26M_DIV8
		if (Init==2)
		{
			//in DDR reserved mode, need to set dramc_f26m_sel because it will be clear in reset
			*(volatile unsigned *)(0x10001088) |= 0x80000000;	// MODULE_SW_CG_1_SET[31] = 1 ==> dramc_f26m_cg_set
			*(volatile unsigned *)(0x10001098) |= (0x01 <<7);		// dramc_f26m_sel : 0-div 1, 1-div8
			*(volatile unsigned *)(0x1000108C) |= 0x80000000;	// MODULE_SW_CG_1_CLR[31] = 1 ==> dramc_f26m_cg_set
		}
#endif		
	}
	else
	{
		if (Reg_Readl((CHA_DRAMCAO_BASE + 0x1c0)) & 0x80000000)
		{
			u4HWGatingEnable = 1;
		}
		else
		{
			u4HWGatingEnable = 0;
		}
	}
	
	if (u4HWGatingEnable)
	{
		Reg_Sync_Writel((CHA_DRAMCAO_BASE + 0x028), Reg_Readl((CHA_DRAMCAO_BASE + 0x028)) & (~(0x01<<30)));     // cha DLLFRZ=0
		u4HWTrackR0 = Reg_Readl((CHA_DRAMCNAO_BASE + 0x374));	// cha r0 
		u4HWTrackR1 = Reg_Readl((CHA_DRAMCNAO_BASE + 0x378));	// cha r1
		Reg_Sync_Writel((CHA_DRAMCAO_BASE + 0x028), Reg_Readl((CHA_DRAMCAO_BASE + 0x028)) |(0x01<<30));     	// cha DLLFRZ=1
	}

    if (p->frequency != DUAL_FREQ_LOW)   
	{


		if (u4HWGatingEnable)
		{
			// Current is low frequency. Save to low frequency fine tune here because shuffle enable will cause HW GW reload.
			Reg_Sync_Writel((CHA_DRAMCAO_BASE + 0x840), u4HWTrackR0);
			Reg_Sync_Writel((CHA_DRAMCAO_BASE + 0x844), u4HWTrackR1);
		}
		
#if fcFOR_CHIP_ID == fcJADE
		Reg_Sync_Writel((MEM_DCM_CTRL), (0x1f << 21) | (0x0 << 16) | (0x01 << 9) | (0x01 << 8) | (0x00 << 7) | (0x0 << 6) | (0x1f<<1));	// 3pll_mode: dcm_en = 0
		Reg_Sync_Writel((MEM_DCM_CTRL), Reg_Readl(MEM_DCM_CTRL) |0x01);
		Reg_Sync_Writel((MEM_DCM_CTRL), Reg_Readl(MEM_DCM_CTRL) & (~0x01));

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x614), &u4value);
		mcSET_BIT(u4value, 0);	// set [0]:RG_MEMPLL2_EN, [23]:MEMPLL2_AUTOK_EN	
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), u4value);

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x62c), &u4value);
		mcSET_BIT(u4value, 0);	// set [0]:RG_MEMPLL4_EN, [23]:MEMPLL4_AUTOK_EN
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62c), u4value);

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x620), &u4value);
		mcSET_BIT(u4value, 0);	// set [0]:RG_MEMPLL3_EN, [23]:MEMPLL3_AUTOK_EN
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), u4value);		        


		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x618), &u4value);
		mcSET_BIT(u4value, 16);	// Set RG_MEMPLL2_RESERVE0(RG_MEMPLL2_RST)[16]
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), u4value);		

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x630), &u4value);
		mcSET_BIT(u4value, 16);	// Set RG_MEMPLL4_RESERVE0(RG_MEMPLL2_RST)[16]
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), u4value);		

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x624), &u4value);
		mcSET_BIT(u4value, 16);	// Set RG_MEMPLL3_RESERVE0(RG_MEMPLL2_RST)[16]
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), u4value);

        mcDELAY_US(2);

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x618), &u4value);
		mcCLR_BIT(u4value, 16);	// Clr RG_MEMPLL2_RESERVE0(RG_MEMPLL2_RST)[16]
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), u4value);		

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x630), &u4value);
		mcCLR_BIT(u4value, 16);	// Clr RG_MEMPLL4_RESERVE0(RG_MEMPLL2_RST)[16]
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), u4value);		

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x624), &u4value);
		mcCLR_BIT(u4value, 16);	// Clr RG_MEMPLL3_RESERVE0(RG_MEMPLL2_RST)[16]
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), u4value);

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x60c), &u4value);
		mcSET_BIT(u4value, 7);	// Set [7]:RG_MEMPLL_BIAS_LPF_EN
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x60c), u4value);				

        mcDELAY_US(13);	

#ifdef FRANCIS_MEM_C_DIDNT_HAS_BUT_JADE_EMI_DFS_SWITCH_FLOW_REFRENCE_HAS
		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x618), &u4value);
		mcSET_BIT(u4value, 16);
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), u4value); // Set RG_MEMPLL2_RESERVE0(RG_MEMPLL2_RST)[16]
		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x630), &u4value);
		mcSET_BIT(u4value, 16);		
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), u4value); // Set RG_MEMPLL4_RESERVE0(RG_MEMPLL4_RST)[16]
		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x624), &u4value);
		mcSET_BIT(u4value, 16);		
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), u4value); // Set RG_MEMPLL3_RESERVE0(RG_MEMPLL3_RST)[16]
		mcDELAY_US(2);
		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x618), &u4value);
		mcCLR_BIT(u4value, 16);
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), u4value); // clr RG_MEMPLL2_RESERVE0(RG_MEMPLL2_RST)[16]
		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x630), &u4value);
		mcCLR_BIT(u4value, 16);		
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), u4value); // clr RG_MEMPLL4_RESERVE0(RG_MEMPLL4_RST)[16]
		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x624), &u4value);
		mcCLR_BIT(u4value, 16);		
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), u4value); // clr RG_MEMPLL3_RESERVE0(RG_MEMPLL3_RST)[16]
		mcDELAY_US(13);		
		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x614), &u4value);
		mcSET_BIT(u4value, 0);
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), u4value);
		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x620), &u4value);
		mcSET_BIT(u4value, 0);
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), u4value);
		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x62c), &u4value);
		mcSET_BIT(u4value, 0);
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62c), u4value);


		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x60c), &u4value);
		mcCLR_BIT(u4value, 7);	// Clr [7]:RG_MEMPLL_BIAS_LPF_EN
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x60c), u4value);	
		mcDELAY_US(13);		
#endif	
#endif				

	
		// MR2 RL/WL set
	            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), emi_set->iLPDDR3_MODE_REG_2);



		bak_data1 = Reg_Readl((CHA_DRAMCAO_BASE + (0x1dc)));
#if fcFOR_CHIP_ID == fcK2
		Reg_Sync_Writel((CHA_DRAMCAO_BASE + (0x1dc)), bak_data1 & ~(0xc0000000));		
#endif
#if fcFOR_CHIP_ID == fcJADE
		Reg_Sync_Writel((CHA_DRAMCAO_BASE + (0x1dc)), bak_data1 & ~(0xc2000002)); // clr [31]:R_DMCOMBCLKCTRL/[30]:R_DMPHYCLKDYNGEN to 0, Disable Dynamic Clock Gating
#endif


		Reg_Sync_Writel((CLK_CFG_0_CLR), 0x300);
		Reg_Sync_Writel((CLK_CFG_0_SET), 0x100);

		bak_data2 = Reg_Readl((CHA_DDRPHY_BASE + (0x640)));
#if fcFOR_CHIP_ID == fcK2		
		bak_data4 = Reg_Readl((PCM_INI_PWRON0_REG));
#endif
#if fcFOR_CHIP_ID == fcJADE		
		bak_data4 = Reg_Readl((SPM_POWER_ON_VAL0));
#endif

		

		
		// Shuffle to high start.
	        // Reg.28h[17]=1 R_DMSHU_DRAMC
	        // Reg.28h[16]=0 R_DMSHU_LOW
		bak_data3 = Reg_Readl((CHA_DRAMCAO_BASE + (0x028)));

		// need to wait 16us here for PLL output stable
		// still allow other master to access DRAM here
		mcDELAY_US(16);
		
		bak_data3 = (bak_data3 & (~0x00030000)) | 0x20000;	// [17]: R_DMSHU_DRAMCx=1, [16]: R_DMSHU_LOWx = 1'b0;
#if (TEST_DFS_DMABUSY==1)
		if (!Init)
		{
			if (uiDualRank) 
			{
				uiCmpErr = DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, 0x50000000+uiOffset, 0x80000004, 0xff000, 8, 0, 1);
			}
			else
			{
				uiCmpErr = DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, 0x50000000+uiOffset, 0x60000004, 0xff000, 8, 0, 1);
			}
		}
#endif

		// Block EMI start.
		Reg_Sync_Writel((CHA_DRAMCAO_BASE + (0x028)), bak_data3);	// GO-High
		
#ifdef SPM_WORKAROUND_CATRAINING
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x0c), DUAL_FREQ_HIGH_reg_00c);
#endif
        
#if (TEST_DFS_DMABUSY==2)
		if (!Init)
		{
			if (uiDualRank) 
			{
				uiCmpErr = DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, 0x50000000+uiOffset, 0x80000004, 0xff000, 8, 0, 1);
			}
			else
			{
				uiCmpErr = DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, 0x50000000+uiOffset, 0x60000004, 0xff000, 8, 0, 1);
			}
		}
#endif

		// Wait shuffle_end Reg.16ch[0] == 1
	        read_data = Reg_Readl((CHA_DRAMCAO_BASE + (0x16c)));
	        while ((read_data & 0x01)  != 0x01)
	        {
		        read_data = Reg_Readl((CHA_DRAMCAO_BASE + (0x16c)));
	        }

		// [3:0]=1 : VCO/4 . [4]=0 : RG_MEMPLL_ONEPLLSEL. [12:5] RG_MEMPLL_RSV. RG_MEMPLL_RSV[1]=1
#if fcFOR_CHIP_ID == fcK2		
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x698)), 0x00001e41); 		
#endif
#if fcFOR_CHIP_ID == fcJADE
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x698)), 0x00001e20 | new_DMYPAD_option | gENABLE_MEMPLL_CG_TO_SAVE_POWER_val);	// Set CLKSW=0, 4xck = VCO/4,[12:5]RG_MEMPLL_RSV=8'h01, ONEPLLMODE=0 
#endif

#if fcFOR_CHIP_ID == fcK2
	        Reg_Sync_Writel((PCM_INI_PWRON0_REG), bak_data4 & (~0x8000000));  // K2?? *(volatile unsigned int*)(0x10006000) = 0x0b160001??		
		mcDELAY_US(1);		// Wait 1us.
#endif		

		Reg_Sync_Writel((CLK_CFG_UPDATE), 0x02);

		// toggle DIV2_SYNC
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x640)), bak_data2 & (~0x01)); 	//sync = 1
		mcDELAY_US(1);
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x640)), bak_data2 | 0x01);		// sync back to original. Should be 0. Bu in case of SPM control, need to make sure SPM is not toggling.
		
		//block EMI end.
		Reg_Sync_Writel((CHA_DRAMCAO_BASE + (0x028)), bak_data3 & (~0x20000));	// [17]: R_DMSHU_DRAMCx=0 (clear)

#ifdef TEST_DFS_DMABUSY
		if (!Init)
		{
			if (uiDualRank) 
			{
				uiCmpErr = DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, 0x50000000+uiOffset, 0x80000004, 0xff000, 8, 0x0f, 1);
			}
			else
			{
				uiCmpErr = DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, 0x50000000+uiOffset, 0x60000004, 0xff000, 8, 0x0f, 1);
			}
			if (uiCmpErr)
			{
				mcSHOW_DBG_MSG(("DMA Fail in DFS... %xh, source=%xh\n", uiCmpErr, 0x50000000+uiOffset));
				while(1);
			}
			else
			{
				mcSHOW_DBG_MSG(("DMA Pass in DFS... %xh, source=%xh\n", uiCmpErr, 0x50000000+uiOffset));
			}	
			if (uiOffset < 0x100)
			{
				uiOffset += 4;
			}
			else
			{
				uiOffset = 0;
			}
		}
#endif	

#if fcFOR_CHIP_ID == fcK2
		Reg_Sync_Writel((CHA_DRAMCAO_BASE + (0x1dc)), bak_data1);
#endif
#if fcFOR_CHIP_ID == fcJADE
#ifdef DRAMC_DRAMPHY_DCM_ENABLE
		Reg_Sync_Writel((CHA_DRAMCAO_BASE + (0x1dc)), bak_data1 | 0xC2000002);	// set [31]:R_DMCOMBCLKCTRL, [30]:R_DMPHYCLKDYNGEN, enable Dynamic Clock Gating
#else
		Reg_Sync_Writel((CHA_DRAMCAO_BASE + (0x1dc)), bak_data1);	// set [31]:R_DMCOMBCLKCTRL, [30]:R_DMPHYCLKDYNGEN, enable Dynamic Clock Gating		
#endif		
#endif


#ifdef DRAMC_DRAMPHY_DCM_ENABLE
		Reg_Sync_Writel((MEM_DCM_CTRL), (0x1f << 21) | (0x0 << 16) | (0x01 << 9) | (0x01 << 8) | (0x01 << 7) | (0x0 << 6) | (0x1f<<1)); // 3pll_mode: dcm_en = 1
#else
		Reg_Sync_Writel((MEM_DCM_CTRL), (0x1f << 21) | (0x0 << 16) | (0x01 << 9) | (0x01 << 8) | (0x00 << 7) | (0x0 << 6) | (0x1f<<1)); // 3pll_mode: dcm_en = 1
#endif		
		Reg_Sync_Writel((MEM_DCM_CTRL), Reg_Readl(MEM_DCM_CTRL) |0x01);
		Reg_Sync_Writel((MEM_DCM_CTRL), Reg_Readl(MEM_DCM_CTRL) & (~0x01));
		
		//Reg_Sync_Writel((DFS_MEM_DCM_CTRL), (0x01 << 28) | (0x1f << 21) | (0x0 << 16) | (0x01 << 9) | (0x01 << 8) | (0x0 << 7) | (0x0 << 6) | (0x1f << 1));	// 1pll_mode: dcm_en=0
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), (0x01 << 28) | (0x1f << 21) | (0x0 << 16) | (0x01 << 9) | (0x01 << 8) | (0x1 << 7) | (0x0 << 6) | (0x1f << 1));	// 1pll_mode: dcm_en=1 always on then SPM no need to set this
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) | 0x01);
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) & (~0x01));
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) |(0x01 << 31));
		

#if fcFOR_CHIP_ID == fcK2		
		// [3:0]=1 : VCO/4 . [4]=0 : RG_MEMPLL_ONEPLLSEL. [12:5] RG_MEMPLL_RSV. RG_MEMPLL_RSV[1]=0 ==> disable output.
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x698)), 0x00001e01);		
#endif

#if fcFOR_CHIP_ID == fcJADE
		// Rx1a6: [8:5] RG_MEMPLL_RSV
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x698)), 0x00001e00 | new_DMYPAD_option | gENABLE_MEMPLL_CG_TO_SAVE_POWER_val);	// Set CLKSW=0, 4xck = VCO/4, Clear all [12:5] RG_MEMPLL_RSV, [4]:ONEPLLMODE=0 
#endif
	}
	else
	{
		// Shuffle to low
		U32 read_data;
		U32 bak_data0;
		U32 bak_data1;
		U32 bak_data2;
		U32 bak_data3;

		if (u4HWGatingEnable)
		{
			// Current is low frequency. Save to high frequency fine tune here because shuffle enable will cause HW GW reload.
			Reg_Sync_Writel((CHA_DRAMCAO_BASE + 0x94), u4HWTrackR0);
			Reg_Sync_Writel((CHA_DRAMCAO_BASE + 0x98), u4HWTrackR1);
		}

#if fcFOR_CHIP_ID == fcJADE
		// Rx1a6: [8:5] RG_MEMPLL_RSV
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x698)), 0x000e0020 | new_DMYPAD_option | gENABLE_MEMPLL_CG_TO_SAVE_POWER_val);	// Set CLKSW=0, 4xck = VCO/4, [12:5]RG_MEMPLL_RSV[0]=1, Enabel Base PLL clock output, [4]:ONEPLL=0, [16]=0, [19/18/17]=1
#ifdef JADE_FLOW_RUN_ON_K2_EVB
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x698)), 0x00001e41);
#endif
#endif


#if fcFOR_CHIP_ID == fcJADE
		Reg_Sync_Writel((MEM_DCM_CTRL), (0x1f << 21) | (0x0 << 16) | (0x01 << 9) | (0x01 << 8) | (0x00 << 7) | (0x0 << 6) | (0x1f<<1));	// 3pll_mode: dcm_en = 0
		Reg_Sync_Writel((MEM_DCM_CTRL), Reg_Readl(MEM_DCM_CTRL) |0x01);
		Reg_Sync_Writel((MEM_DCM_CTRL), Reg_Readl(MEM_DCM_CTRL) & (~0x01));

                Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) | (0x01 << 31)); // gate off the 1pll_mode clock before change the mux sel
#ifdef DRAMC_DRAMPHY_DCM_ENABLE
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) | (0x01 << 28) | (0x1f << 21) | (0x0 << 16) | (0x01 << 9) | (0x01 << 8) | (0x1 << 7) | (0x0 << 6) | (0x1f << 1));	// 1pll_mode: dcm_en=1
#else
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) | (0x01 << 28) | (0x1f << 21) | (0x0 << 16) | (0x01 << 9) | (0x01 << 8) | (0x0 << 7) | (0x0 << 6) | (0x1f << 1));	// 1pll_mode: dcm_en=0
#endif		
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) &  (~(0x01 << 31)));	// gate on  the 1pll_mode clock before change the mux sel
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) | 0x01);
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) & (~0x01));	
#endif				
		
		
		// MR2 RL/WL set
		#ifdef DUAL_FREQ_DIFF_RLWL
	        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), LPDDR3_MODE_REG_2_LOW);
		#else
			ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), emi_set->iLPDDR3_MODE_REG_2);
		#endif



#if fcFOR_CHIP_ID == fcK2
		// [3:0]=1 : VCO/4 . [4]=0 : RG_MEMPLL_ONEPLLSEL. [12:5] RG_MEMPLL_RSV. RG_MEMPLL_RSV[1]=1
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x698)), 0x00001e41);		

		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x618)), Reg_Readl(CHA_DDRPHY_BASE + (0x618)) | 0x10000);	// Switch MEMPLL2 reset mode select 
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x624)), Reg_Readl(CHA_DDRPHY_BASE + (0x624)) | 0x10000);	// Switch MEMPLL3 reset mode select		
		
		Reg_Sync_Writel((MEM_DCM_CTRL), (0x1f << 21) | (0x0 << 16) | (0x01 << 9) | (0x01 << 8) | (0x0 << 7) | (0x0 << 6) | (0x1f << 1));
		Reg_Sync_Writel((MEM_DCM_CTRL), Reg_Readl(MEM_DCM_CTRL) |0x01);
		Reg_Sync_Writel((MEM_DCM_CTRL), Reg_Readl(MEM_DCM_CTRL) & (~0x01));
		
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), (0x01 << 31));
#ifdef DRAMC_DRAMPHY_DCM_ENABLE
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) | ((0x01 << 28) | (0x1f << 21) | (0x0 << 16) | (0x01 << 9) | (0x01 << 8) | (0x01 << 7) | (0x0 << 6) | (0x1f << 1)));
#else
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) | ((0x01 << 28) | (0x1f << 21) | (0x0 << 16) | (0x01 << 9) | (0x01 << 8) | (0x00 << 7) | (0x0 << 6) | (0x1f << 1)));
#endif
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) &  (~(0x01 << 31)));
		
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) | 0x01);
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) & (~0x01));
#endif		

		bak_data1 = Reg_Readl((CHA_DRAMCAO_BASE + (0x828)));
		Reg_Sync_Writel((CHA_DRAMCAO_BASE + (0x828)), bak_data1 & (~0xc0000000));	// set R_DMCOMBCLKCTRL/R_DMPHYCLKDYNGEN to 0
		
		bak_data0 = Reg_Readl((CHA_DRAMCAO_BASE + (0x1dc)));
		Reg_Sync_Writel((CHA_DRAMCAO_BASE + (0x1dc)), bak_data0 & ~(0xc2000002));

		
		// /u_top_ap_clk_ctrl/u_cksys_reg 
		Reg_Sync_Writel((CLK_CFG_0_CLR), 0x300);	// Clr [9:8] to be 2'b00
		Reg_Sync_Writel((CLK_CFG_0_SET), 0x200);	// Set [9:8] to be 2'b10, ddr_x1_ck_buf

		bak_data2 = Reg_Readl((CHA_DDRPHY_BASE + (0x640)));

		// Shuffle to low.
	        // Reg.28h[17]=1 R_DMSHU_DRAMC
	        // Reg.28h[16]=1 R_DMSHU_LOW
		bak_data3 = Reg_Readl((CHA_DRAMCAO_BASE + (0x028)));
		bak_data3 = bak_data3 | 0x30000;	// [17]: R_DMSHU_DRAMCx=1, [16]: R_DMSHU_LOWx = 1'b1;
#if (TEST_DFS_DMABUSY==1)
		if (!Init)
		{
			if (uiDualRank) 
			{
				uiCmpErr = DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, 0x50000000+uiOffset, 0x80000004, 0xff000, 8, 0, 1);
			}
			else
			{
				uiCmpErr = DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, 0x50000000+uiOffset, 0x60000004, 0xff000, 8, 0, 1);
			}
		}
#endif		


#ifdef FRANCIS_MEM_C_DIDNT_HAS_BUT_JADE_EMI_DFS_SWITCH_FLOW_REFRENCE_HAS
		//Reg0x138[5]=1, // Block DRAMC ALE 
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x138), &u4value);
		mcSET_BIT(u4value, 5); 	
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x138), u4value);

		// Wait Reg0x3b8[6] == 1
        read_data = Reg_Readl((CHA_DRAMCAO_BASE + (0x3b8)));
        while ((read_data & (0x01<<6))  != (0x01<<6))	//wait until dramc2spm_request_queue_empty=1 
        {
	        read_data = Reg_Readl((CHA_DRAMCAO_BASE + (0x3b8)));
        }		

		Reg_Sync_Writel((CHA_DRAMCAO_BASE + (0x028)), bak_data3);	// dramc shuffle start, dramc shuffle register to low

		// //polling dramc shuffle status
        read_data = Reg_Readl((CHA_DRAMCAO_BASE + (0x16c)));
        while ((read_data & (0x01<<1))  != (0x01<<1))
        {
	        read_data = Reg_Readl((CHA_DRAMCAO_BASE + (0x16c)));
        }

		//Reg0x138[4]=1, //  1PLL mode switch
        //ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x698), &u4value);
		//mcSET_BIT(u4value, 4); 	
        //ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x698), u4value);

		//Reg_Sync_Writel((CLK_CFG_UPDATE), 0x02);

		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x698)), 0x00008030 | new_DMYPAD_option);  // Set CLKSW=1, 4xck = VCO/4, [12:5]:RG_MEMPLL_RSV[0]=1, [4]:ONEPLL_MODE=1, [15]=[4]
		mcDELAY_US(1);		
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x698)), 0x00000030 | new_DMYPAD_option);  // Set CLKSW=1, 4xck = VCO/4, [12:5]:RG_MEMPLL_RSV[0]=1, [4]:ONEPLL_MODE=1, [15]=[4]

		// toggle DIV2_SYNC
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x640)), bak_data2 & (~0x01));	// Clr [5] PHYCLKP0ENB
		mcDELAY_US(1);
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x640)), bak_data2 | 0x01);		// Set [5] PHYCLKP0ENB
		
		// Block EMI end.
		Reg_Sync_Writel((CHA_DRAMCAO_BASE + (0x028)), bak_data3 & (~0x20000));	// [17]: R_DMSHU_DRAMCx=0

		//Reg0x138[5]=0, // Release DRAMC ALE  
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x138), &u4value);
		mcCLR_BIT(u4value, 5); 	
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x138), u4value);		

#else //FRANCIS_MEM_C_DIDNT_HAS_BUT_JADE_EMI_DFS_SWITCH_FLOW_REFRENCE_HAS
		// Block EMI start.
		Reg_Sync_Writel((CHA_DRAMCAO_BASE + (0x028)), bak_data3);

#ifdef SPM_WORKAROUND_CATRAINING
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x0c), DUAL_FREQ_LOW_reg_00c);
#endif

#if (TEST_DFS_DMABUSY==2)
		if (!Init)
		{
			if (uiDualRank) 
			{
				uiCmpErr = DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, 0x50000000+uiOffset, 0x80000004, 0xff000, 8, 0, 1);
			}
			else
			{
				uiCmpErr = DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, 0x50000000+uiOffset, 0x60000004, 0xff000, 8, 0, 1);
			}
		}
#endif
		
		// Wait shuffle_end Reg.16ch[0] == 1
	        read_data = Reg_Readl((CHA_DRAMCAO_BASE + (0x16c)));
	        while ((read_data & 0x01)  != 0x01)
	        {
		        read_data = Reg_Readl((CHA_DRAMCAO_BASE + (0x16c)));
	        }

#if fcFOR_CHIP_ID == fcK2
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x698)), 0x00069e41);	// Set 4xck = VCO/4, RG_MEMPLL_RSV[1]=1
		                                                                // Also set MEMPLL2/3 enter reset mode
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x698)), 0x00069e51);  // Set CLKSW=1 			
#endif

		Reg_Sync_Writel((CLK_CFG_UPDATE), 0x02);

#if fcFOR_CHIP_ID == fcJADE
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x698)), 0x00008030 | new_DMYPAD_option | gENABLE_MEMPLL_CG_TO_SAVE_POWER_val);  // Set CLKSW=1, 4xck = VCO/4, [12:5]:RG_MEMPLL_RSV[0]=1, [4]:ONEPLL_MODE=1, [15]=[4]
#endif

		// toggle DIV2_SYNC
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x640)), bak_data2 & (~0x01));	// Clr [5] PHYCLKP0ENB
		mcDELAY_US(1);
		Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x640)), bak_data2 | 0x01);		// Set [5] PHYCLKP0ENB
		
		// Block EMI end.
		Reg_Sync_Writel((CHA_DRAMCAO_BASE + (0x028)), bak_data3 & (~0x20000));	// [17]: R_DMSHU_DRAMCx=0
#endif //FRANCIS_MEM_C_DIDNT_HAS_BUT_JADE_EMI_DFS_SWITCH_FLOW_REFRENCE_HAS		

#ifdef TEST_DFS_DMABUSY
		if (!Init)
		{
			if (uiDualRank) 
			{
				uiCmpErr = DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, 0x50000000+uiOffset, 0x80000004, 0xff000, 8, 0x0f, 1);
			}
			else
			{
				uiCmpErr = DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, 0x50000000+uiOffset, 0x60000004, 0xff000, 8, 0x0f, 1);
			}
			if (uiCmpErr)
			{
				mcSHOW_DBG_MSG(("DMA Fail in DFS... %xh, source=%xh\n", uiCmpErr, 0x50000000+uiOffset));
				while(1);
			}
			else
			{
				mcSHOW_DBG_MSG(("DMA Pass in DFS... %xh, source=%xh\n", uiCmpErr, 0x50000000+uiOffset));
			}	
			if (uiOffset < 0x100)
			{
				uiOffset += 4;
			}
			else
			{
				uiOffset = 0;
			}
		}
#endif	

#if fcFOR_CHIP_ID == fcJADE		
#ifdef DRAMC_DRAMPHY_DCM_ENABLE
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) | ((0x01 << 28) | (0x1f << 21) | (0x0 << 16) | (0x01 << 9) | (0x01 << 8) | (0x01 << 7) | (0x0 << 6) | (0x1f << 1)));	// 1pll_mode: dcm_en=1	
#else
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) | ((0x01 << 28) | (0x1f << 21) | (0x0 << 16) | (0x01 << 9) | (0x01 << 8) | (0x00 << 7) | (0x0 << 6) | (0x1f << 1)));	// 1pll_mode: dcm_en=0
#endif		
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) | 0x01);
		Reg_Sync_Writel((DFS_MEM_DCM_CTRL), Reg_Readl(DFS_MEM_DCM_CTRL) & (~0x01));

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x60c), &u4value);
		mcCLR_BIT(u4value, 7);	// Clr [7]:RG_MEMPLL_BIAS_LPF_EN
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x60c), u4value);				

#ifndef JADE_FLOW_RUN_ON_K2_EVB
		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x614), &u4value);
		mcCLR_BIT(u4value, 0);	// Clr [0]:RG_MEMPLL2_EN, [23]:MEMPLL2_AUTOK_EN
		mcCLR_BIT(u4value, 23);		
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), u4value);						

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x62c), &u4value);
		mcCLR_BIT(u4value, 0);	// Clr [0]:RG_MEMPLL4_EN, [23]:MEMPLL4_AUTOK_EN
		mcCLR_BIT(u4value, 23);		
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62c), u4value);						

		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x620), &u4value);
		mcCLR_BIT(u4value, 0);	// Clr [0]:RG_MEMPLL3_EN, [23]:MEMPLL3_AUTOK_EN
		mcCLR_BIT(u4value, 23);		
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), u4value);								
#endif		
#endif		

#if fcFOR_CHIP_ID == fcK2
		Reg_Sync_Writel((CHA_DRAMCAO_BASE + (0x828)), bak_data1);

		Reg_Sync_Writel((PCM_INI_PWRON0_REG), Reg_Readl(PCM_INI_PWRON0_REG) |0x8000000);
#endif
#if fcFOR_CHIP_ID == fcJADE
		Reg_Sync_Writel((CHA_DRAMCAO_BASE + (0x828)), bak_data1 | 0xC2000002);    // release R_DMCOMBCLKCTRL, R_DMPHYCLKDYNGEN
#endif

		Reg_Sync_Writel((CHA_DRAMCAO_BASE + (0x1dc)), bak_data0 | 0xc2000002);


	}

	//mcSHOW_DBG_MSG(("mt_pll_post_init: mt_get_mem_freq = %dKhz\n", mt_get_bus_freq()));

	//DramcEnterSelfRefresh(p, 0); // exit self refresh
}

#define FREQREG_SIZE 52

typedef struct _SHUFFLE_REG
{
	U32 uiShuffleAddr;
	U32 uiSourceAddr;
} SHUFFLE_REG;

const SHUFFLE_REG LowFreq_ShuffleReg[FREQREG_SIZE] =
{
	{CHA_DRAMCAO_BASE + 0x800, CHA_DRAMCAO_BASE + 0x000},
	{CHA_DRAMCAO_BASE + 0x804, CHA_DRAMCAO_BASE + 0x004},
	{CHA_DRAMCAO_BASE + 0x808, CHA_DRAMCAO_BASE + 0x008},
	{CHA_DRAMCAO_BASE + 0x80C, CHA_DRAMCAO_BASE + 0x1F0},
	{CHA_DRAMCAO_BASE + 0x810, CHA_DRAMCAO_BASE + 0x1F8},
	{CHA_DRAMCAO_BASE + 0x814, CHA_DRAMCAO_BASE + 0x110},
	{CHA_DRAMCAO_BASE + 0x818, CHA_DRAMCAO_BASE + 0x018},
	{CHA_DRAMCAO_BASE + 0x81C, CHA_DRAMCAO_BASE + 0x01C},
	{CHA_DRAMCAO_BASE + 0x820, CHA_DRAMCAO_BASE + 0x044},
	{CHA_DRAMCAO_BASE + 0x824, CHA_DRAMCAO_BASE + 0x07C},
	{CHA_DRAMCAO_BASE + 0x828, CHA_DRAMCAO_BASE + 0x1DC},
	{CHA_DRAMCAO_BASE + 0x82C, CHA_DRAMCAO_BASE + 0x1E8},
	{CHA_DRAMCAO_BASE + 0x830, CHA_DRAMCAO_BASE + 0x0FC},
	{CHA_DRAMCAO_BASE + 0x834, CHA_DRAMCAO_BASE + 0x08C},
	{CHA_DRAMCAO_BASE + 0x838, CHA_DRAMCAO_BASE + 0x080},
	{CHA_DRAMCAO_BASE + 0x83C, CHA_DRAMCAO_BASE + 0x0E0},
	{CHA_DRAMCAO_BASE + 0x840, CHA_DRAMCAO_BASE + 0x094},
	{CHA_DRAMCAO_BASE + 0x844, CHA_DRAMCAO_BASE + 0x098},
	{CHA_DRAMCAO_BASE + 0x84C, CHA_DRAMCAO_BASE + 0x118},
	{CHA_DRAMCAO_BASE + 0x850, CHA_DRAMCAO_BASE + 0x1C4},
	{CHA_DRAMCAO_BASE + 0x854, CHA_DRAMCAO_BASE + 0x138},
	{CHA_DRAMCAO_BASE + 0x858, CHA_DRAMCAO_BASE + 0x124},
	{CHA_DRAMCAO_BASE + 0x85C, CHA_DRAMCAO_BASE + 0x048},
	{CHA_DRAMCAO_BASE + 0x860, CHA_DRAMCAO_BASE + 0x400},
	{CHA_DRAMCAO_BASE + 0x864, CHA_DRAMCAO_BASE + 0x404},
	{CHA_DRAMCAO_BASE + 0x868, CHA_DRAMCAO_BASE + 0x408},
	{CHA_DRAMCAO_BASE + 0x86C, CHA_DRAMCAO_BASE + 0x40C},
	{CHA_DRAMCAO_BASE + 0x870, CHA_DRAMCAO_BASE + 0x410},
	{CHA_DRAMCAO_BASE + 0x874, CHA_DRAMCAO_BASE + 0x414},
	{CHA_DRAMCAO_BASE + 0x878, CHA_DRAMCAO_BASE + 0x418},
	{CHA_DRAMCAO_BASE + 0x87C, CHA_DRAMCAO_BASE + 0x41C},
	{CHA_DRAMCAO_BASE + 0x880, CHA_DRAMCAO_BASE + 0x420},
	{CHA_DRAMCAO_BASE + 0x884, CHA_DRAMCAO_BASE + 0x424},
	{CHA_DRAMCAO_BASE + 0x888, CHA_DRAMCAO_BASE + 0x428},
	{CHA_DRAMCAO_BASE + 0x88C, CHA_DRAMCAO_BASE + 0x42C},
	{CHA_DRAMCAO_BASE + 0x890, CHA_DRAMCAO_BASE + 0x1C0},
	{CHA_DRAMCAO_BASE + 0x894, CHA_DRAMCAO_BASE + 0x1C8},
	{CHA_DRAMCAO_BASE + 0x8A0, CHA_DRAMCAO_BASE + 0x210},
	{CHA_DRAMCAO_BASE + 0x8A4, CHA_DRAMCAO_BASE + 0x214},
	{CHA_DRAMCAO_BASE + 0x8A8, CHA_DRAMCAO_BASE + 0x218},
	{CHA_DRAMCAO_BASE + 0x8AC, CHA_DRAMCAO_BASE + 0x21C},
	{CHA_DRAMCAO_BASE + 0x8B0, CHA_DRAMCAO_BASE + 0x220},
	{CHA_DRAMCAO_BASE + 0x8B4, CHA_DRAMCAO_BASE + 0x224},
	{CHA_DRAMCAO_BASE + 0x8B8, CHA_DRAMCAO_BASE + 0x228},
	{CHA_DRAMCAO_BASE + 0x8BC, CHA_DRAMCAO_BASE + 0x22C},
	{CHA_DDRPHY_BASE + 0x8D0, CHA_DDRPHY_BASE + 0x200},
	{CHA_DDRPHY_BASE + 0x8D4, CHA_DDRPHY_BASE + 0x204},
	{CHA_DDRPHY_BASE + 0x8D8, CHA_DDRPHY_BASE + 0x208},
	{CHA_DDRPHY_BASE + 0x8DC, CHA_DDRPHY_BASE + 0x20C},
	{CHA_DDRPHY_BASE + 0x8E0, CHA_DDRPHY_BASE + 0x010},
	{CHA_DDRPHY_BASE + 0x8E4, CHA_DDRPHY_BASE + 0x014},
	{CHA_DDRPHY_BASE + 0x8F0, CHA_DDRPHY_BASE + 0x124},
};

static U32 FreqReg_Backup[FREQREG_SIZE];

void DramcSaveToBackup(DRAMC_CTX_T *p)
{
	U32 Offset;
	
        mcSHOW_DBG_MSG(("Save  frequency registers setting into backup. \n"));
	for (Offset = 0; Offset < FREQREG_SIZE; Offset++)
	{
		FreqReg_Backup[Offset] = (*(volatile unsigned int *)(LowFreq_ShuffleReg[Offset].uiSourceAddr));
	}
}

void DramcRestoreBackup(DRAMC_CTX_T *p)
{
	U32 Offset;
	
        mcSHOW_DBG_MSG(("Restore backup to frequency registers. \n"));
	for (Offset = 0; Offset < FREQREG_SIZE; Offset++)
	{
		(*(volatile unsigned int *)(LowFreq_ShuffleReg[Offset].uiSourceAddr)) = FreqReg_Backup[Offset];
	}
}

void DramcSaveToShuffleReg(DRAMC_CTX_T *p)
{
	U32 Offset;
	
        mcSHOW_DBG_MSG(("Save  frequency registers setting into shuffle register. \n"));
	for (Offset = 0; Offset < FREQREG_SIZE; Offset++)
	{
		(*(volatile unsigned int *)(LowFreq_ShuffleReg[Offset].uiShuffleAddr)) = 
			(*(volatile unsigned int *)(LowFreq_ShuffleReg[Offset].uiSourceAddr));
	}
}

/*
void DramcExchangeFreqSetting(DRAMC_CTX_T *p)
{
	U32 Offset, RegVal;

        mcSHOW_DBG_MSG(("Exchange  frequency registers setting with shuffle register. \n"));
	
	for (Offset = 0; Offset < FREQREG_SIZE; Offset++)
	{
		RegVal = (*(volatile unsigned int *)(LowFreq_ShuffleReg[Offset].uiShuffleAddr));
		(*(volatile unsigned int *)(LowFreq_ShuffleReg[Offset].uiShuffleAddr)) = 
			(*(volatile unsigned int *)(LowFreq_ShuffleReg[Offset].uiSourceAddr));
		(*(volatile unsigned int *)(LowFreq_ShuffleReg[Offset].uiSourceAddr)) = RegVal;
	}	
}*/

void DramcDumpFreqSetting(DRAMC_CTX_T *p)
{
	U32 Offset;

        mcSHOW_DBG_MSG(("Dump shuffle registers...\n"));
        mcSHOW_DBG_MSG(("Shuffle registers number = %d\n", FREQREG_SIZE));
	
	for (Offset = 0; Offset < FREQREG_SIZE; Offset++)
	{
        	mcSHOW_DBG_MSG(("[Source] Addr %xh = %xh [Shuffle] Addr %xh = %xh\n", 
        		LowFreq_ShuffleReg[Offset].uiSourceAddr, (*(volatile unsigned int *)(LowFreq_ShuffleReg[Offset].uiSourceAddr)),
        		LowFreq_ShuffleReg[Offset].uiShuffleAddr, (*(volatile unsigned int *)(LowFreq_ShuffleReg[Offset].uiShuffleAddr))));
	}
}



void DramcLowFreqWrite(DRAMC_CTX_T *p)
{
    U8 ucstatus = 0;
    U16 u2freq_orig;
    U32 u4err_value;

    if (p->fglow_freq_write_en == ENABLE)
    {
        u2freq_orig = p->frequency;
        p->frequency = p->frequency_low;
        mcSHOW_DBG_MSG(("Enable low speed write function...\n"));

        // we will write data in memory on a low frequency,to make sure the data we write is  right
        // then use engine2 read to do the calibration
        // so ,we will do :
        // 1.change freq 
        // 2. use self test engine2 write to write data ,and check the data is right or not
        // 3.change freq to original value        

        // 1. change freq
        p->channel = CHANNEL_A;
        DramcEnterSelfRefresh(p, 1); // enter self refresh
        mcDELAY_US(1);
        MemPllInit(p);
        mcDELAY_US(1);
        DramcEnterSelfRefresh(p, 0); // exit self refresh

        // double check frequency
        //mcSHOW_DBG_MSG(("Low Speed Write: mt_get_mem_freq = %dKhz\n", mt_get_mem_freq()));
        
        // 2. use self test engine2 to write data (only support AUDIO or XTALK pattern)
        if (p->test_pattern== TEST_AUDIO_PATTERN)
        {
            u4err_value = DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 1, 0, 0, 0);            
        }
        else if (p->test_pattern== TEST_XTALK_PATTERN)
        {
            u4err_value = DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 2, 0, 0, 0);
        }
        else
        {
            mcSHOW_ERR_MSG(("ERROR! Only support AUDIO or XTALK in Low Speed Write and High Speed Read calibration!! \n"));
            u4err_value = 0xffffffff;
        }

        // May error due to gating not calibrate @ low speed
        mcSHOW_DBG_MSG(("Low speed write error value: 0x%x\n", u4err_value));

        // do phy reset due to ring counter may be wrong
        DramcPhyReset(p);

        // 3. change to original freq 
        p->frequency = u2freq_orig;
        DramcEnterSelfRefresh(p, 1); // enter self refresh
        mcDELAY_US(1);
        MemPllInit(p);
        mcDELAY_US(1);
        DramcEnterSelfRefresh(p, 0); // exit self refresh

        // double check frequency
        //mcSHOW_DBG_MSG(("High Speed Read: mt_get_mem_freq = %dKhz\n", mt_get_mem_freq()));
    }    
}

void DramcDiv2PhaseSync(DRAMC_CTX_T *p)
{
	U8 ucstatus = 0;
	U32 u4value, u4Backup;
	
    	// Reg.5cch[12]=1 bypass delay chain, Reg.5cch[8]=1 from Reg.640h[5]. 
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x5cc), &u4value);
    	u4Backup = u4value;
	mcSET_BIT(u4value, 8);
	mcSET_BIT(u4value, 12);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x5cc), u4value);
    	
#if fcFOR_CHIP_ID == fcK2
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x00020091);		// Reg.640h[5] = 0 invert 1. Sync =1.
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x000200b1);	 	// Reg.640h[5] = 1 invert 0. Sync =0.

#endif
#if fcFOR_CHIP_ID == fcJADE
#ifndef JADE_FLOW_RUN_ON_K2_EVB
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x00022091); // 113904ns, [0] TBD
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x000220b1); // 113544ns, [0] TBD	
#else
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x00022091);	// 113544ns, [0] TBD
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), 0x000220b1);	// 113904ns, [0] TBD
#endif
#endif	

	// Restore.
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x5cc), u4Backup);
}

void DramcPhyReset(DRAMC_CTX_T *p)
{
    U8 ucstatus = 0;//, ucref_cnt;
    U32 u4value;

    // reset phy
    // 0x0f0[28] = 1 -> 0
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), &u4value);
    mcSET_BIT(u4value, POS_PHYCTL1_PHYRST);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), u4value);

    // read data counter reset
    // 0x0f4[25] = 1 -> 0
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), &u4value);
    mcSET_BIT(u4value, POS_GDDR3CTL1_RDATRST);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), u4value);

    //delay 10ns, need to change when porting
    mcDELAY_US(1);  

    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), &u4value);
    mcCLR_BIT(u4value, POS_PHYCTL1_PHYRST);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), u4value);    

    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), &u4value);
    mcCLR_BIT(u4value, POS_GDDR3CTL1_RDATRST);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), u4value);

}

void DramcHWGWEnable(DRAMC_CTX_T *p)
{
    U8 ucstatus = 0;
    U32 u4value;  
    
#ifdef HW_GATING    
    // 0x1c0[31], DQS strobe calibration enable
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCAL0), &u4value);
    mcSET_BIT(u4value, POS_DQSCAL0_STBCALEN);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCAL0), u4value);

    // Shuffle register 0x890[31], DVFS_SHUFFLE_REG1C0, DQS strobe calibration enable
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x890), &u4value);
    mcSET_BIT(u4value, 31);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x890), u4value);    
#endif   

#ifdef CKE_FIXON
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xe4), &u4value);
	u4value |= (0x01 << 2);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xe4), u4value);     
#endif

#ifdef EMI_OPTIMIZATION
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1ec), &u4value);
    #ifdef RWSPLIT_ENABLE
    u4value |= 0x0000cf11;  //0x7B[19]R_DMREORDEREN=0 for DerPing's suggestion
    #else
    u4value |= 0x00004f11;  //0x7B[19]R_DMREORDEREN=0 for DerPing's suggestion
    #endif
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1ec), u4value);  
#endif    

#ifdef ZQCS_ENABLE
    if (p->dram_type == TYPE_LPDDR3)
    {
        // Set (0x1e4[23:16] ZQCSCNT)
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1e4), &u4value);
#if fcFOR_CHIP_ID == fcK2		
        mcSET_FIELD(u4value, 0x20, 0x00ff0000, 16);
#endif
#if fcFOR_CHIP_ID == fcJADE
		mcSET_FIELD(u4value, 0xff, 0x00ff0000, 16);
#endif
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), u4value);  

#ifdef INCREASE_ZQCSCNT_AND_REFRCNT_RANGE
#if fcFOR_CHIP_ID == fcJADE
        // Set (0x1dc[0] ZQCSCNT) , ZQCSCNT[7:0] -> ZQCSCNT[8:0]
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1dc), &u4value);
        mcSET_BIT(u4value, 0);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1dc), u4value); 		
#endif		
#endif
    }	
#endif    
}

void DramcHWGWDisable(DRAMC_CTX_T *p)
{
    U8 ucstatus = 0;
    U32 u4value;  
    
#ifdef HW_GATING    
    // 0x1c0[31], DQS strobe calibration enable
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCAL0), &u4value);
    mcCLR_BIT(u4value, POS_DQSCAL0_STBCALEN);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCAL0), u4value);

    // Shuffle register 0x890[31], DVFS_SHUFFLE_REG1C0, DQS strobe calibration enable
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x890), &u4value);
    mcCLR_BIT(u4value, 31);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x890), u4value);    
#endif   

#ifdef CKE_FIXON
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xe4), &u4value);
	u4value &= ~(0x01 << 2);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xe4), u4value);     
#endif

#if fcFOR_CHIP_ID == fcJADE
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1ec), 0x0012cf11);	// 0x7B[19]R_DMREORDEREN=0, [20]R_DMCKE2RANK=1 for DerPing's suggestion
#endif   
}


void DramcOtherSettingAfterCalibration(DRAMC_CTX_T *p)
{
    U8 ucstatus = 0;
    U32 u4value;  
    U32 u4CoarseTune_MCK;

#ifdef DUAL_RANKS
    if (uiDualRank) 
    {
    	// RANKINCTL_ROOT1 = DQSINCTL+reg_TX_DLY_DQSGATE (min of RK0 and RK1).
	if (opt_gw_coarse_value_R0[p->channel] < opt_gw_coarse_value_R1[p->channel])
	{
		u4CoarseTune_MCK = opt_gw_coarse_value_R0[p->channel] >> 2;
	}
	else
	{
		u4CoarseTune_MCK = opt_gw_coarse_value_R1[p->channel] >> 2;
	}
	
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x138), &u4value);
        mcSET_FIELD(u4value, u4CoarseTune_MCK, 0x0f, 0);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x138), u4value);         

	// RANKINCTL = RANKINCTL_ROOT1+0
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1c4), &u4value);
	//u4CoarseTune_MCK += 2;
		u4CoarseTune_MCK += 1;
 
        mcSET_FIELD(u4value, u4CoarseTune_MCK, 0x000f0000, 16);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1c4), u4value);         
    }
#endif

    if (p->dram_type == TYPE_LPDDR3)
    {
        // Disable Per-bank Refresh when refresh rate >= 5 (only for LPDDR3)
        // Set (0x110[6])
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_RKCFG), &u4value);
        mcSET_BIT(u4value, POS_RKCFG_PBREF_DISBYRATE);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_RKCFG), u4value);  
    }   

}

void DramcEnableTemperatureSensorMR4(DRAMC_CTX_T *p)
{
    U8 ucstatus = 0;
    U32 u4value;  



#ifdef TEMP_SENSOR_ENABLE
    if ((p->dram_type == TYPE_LPDDR3))
    {
        // Enable rum time refresh rate auto update
        // important!!

	// The following derating AC timings need to be modified according to different parts AC timings + 1.875ns + 1 DRAMC clk
        unsigned int TRRD_DERATE = 0x05 <<28;
	unsigned int TRPAB_DERATE = 0x02 << 24;
	unsigned int  TRP_DERATE = 0x09 << 20;
	unsigned int  TRAS_DERATE = 0x0d <<16;
	unsigned int  TRC_DERATE = 0x17 << 8;
	unsigned int  TRCD_DERATE = 0x09 <<4;

	/* setup derating AC timing & enable */
	 ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1f0), 
	 	TRRD_DERATE | TRPAB_DERATE | TRP_DERATE | TRAS_DERATE | TRC_DERATE | TRCD_DERATE | 0x01);            
	 ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80c), 
	 	TRRD_DERATE | TRPAB_DERATE | TRP_DERATE | TRAS_DERATE | TRC_DERATE | TRCD_DERATE | 0x01);            

        // set DQ bit 0, 1, 2 pinmux
        if (p->channel == CHANNEL_A)
        {
            if(p->dram_type == TYPE_LPDDR3)
            {
                // refer to CA training pinmux array for LPDDR3
                ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_RRRATE_CTL), &u4value);
                mcSET_FIELD(u4value, uiLPDDR3_PHY_Mapping_POP_CHA[0], MASK_RRRATE_CTL_BIT0_SEL, POS_RRRATE_CTL_BIT0_SEL);
                mcSET_FIELD(u4value, uiLPDDR3_PHY_Mapping_POP_CHA[1], MASK_RRRATE_CTL_BIT1_SEL, POS_RRRATE_CTL_BIT1_SEL);
                mcSET_FIELD(u4value, uiLPDDR3_PHY_Mapping_POP_CHA[2], MASK_RRRATE_CTL_BIT2_SEL, POS_RRRATE_CTL_BIT2_SEL);
                mcSET_FIELD(u4value, uiLPDDR3_PHY_Mapping_POP_CHA[3], MASK_RRRATE_CTL_BIT3_SEL, POS_RRRATE_CTL_BIT3_SEL);
                ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_RRRATE_CTL), u4value);         
            }
        }
 

        //Set MRSMA to MR4.
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x04);

        // set refrcnt
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_ACTIM1), &u4value);
#if fcFOR_CHIP_ID == fcK2		
        mcSET_FIELD(u4value, 0x10, MASK_ACTIM1_REFRCNT, POS_ACTIM1_REFRCNT);
#endif
#if fcFOR_CHIP_ID == fcJADE
        mcSET_FIELD(u4value, 0xff, MASK_ACTIM1_REFRCNT, POS_ACTIM1_REFRCNT);
        //may set refresh cnt from 0x1ff to 0x10 to prevent tDQSCK viralate too large

#endif
#ifdef INCREASE_ZQCSCNT_AND_REFRCNT_RANGE		
#if fcFOR_CHIP_ID == fcJADE		
		mcSET_BIT(u4value, 2);	// REFRCNT[7:0] -> REFRCNT[8:0]
#endif
#endif

        mcCLR_BIT(u4value, 26); //0:Enable 1:Disable MR4

        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_ACTIM1), u4value);    

        mcDELAY_MS(1);

        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x3B8), &u4value);

        mcSHOW_ERR_MSG(("MRR(MR4) Reg.3B8h[10:8]=%x\n", (u4value & 0x700)>>8));
    }
#endif
}

void DramcRunTimeConfig(DRAMC_CTX_T *p)
{
    U8 ucstatus = 0;
    U32 u4value;  

#ifdef tREFI_DIV4_MANUAL
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x114), &u4value);
        mcSET_BIT(u4value, 31);	
        mcSET_FIELD(u4value, 5, 0x70000000, 28);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x114), u4value);     
	
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x3B8), &u4value);
        mcSHOW_ERR_MSG(("MRR(MR4) Reg.3B8h[10:8]=%x\n", (u4value & 0x700)>>8));
#endif

#ifdef tREFI_DIV2_MANUAL
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x114), &u4value);
        mcSET_BIT(u4value, 31);	
        mcSET_FIELD(u4value, 4, 0x70000000, 28);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x114), u4value);     
	
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x3B8), &u4value);
        mcSHOW_ERR_MSG(("MRR(MR4) Reg.3B8h[10:8]=%x\n", (u4value & 0x700)>>8));
#endif

}

#ifdef SPM_CONTROL_AFTERK

#if fcFOR_CHIP_ID == fcK2 || fcFOR_CHIP_ID == fcJADE

void TransferToSPMControl(void)
{
	unsigned int read_val;
	U8 ucstatus;

#if fcFOR_CHIP_ID == fcK2
	// K2?? : need to check 5c8h, 5cch detailed definition.
	//(*(volatile unsigned int *)(CHA_DDRPHY_BASE + (0x5c8))) = 0x00001010;
	//(*(volatile unsigned int *)(CHA_DDRPHY_BASE + (0x5cc))) = 0x40000000;
	
	(*(volatile unsigned int *)(CHA_DDRPHY_BASE + (0x5c8))) = 0x1010F030;
	(*(volatile unsigned int *)(CHA_DDRPHY_BASE + (0x5cc))) = 0x50101010;
#endif	

#if fcFOR_CHIP_ID == fcJADE
	// Rlease DFS Control to SPM
	// Rx172   [ 0]  rg_dmss_pwdb_en_sel		  0: Use SPM Control, 1: Use S/W Control
	//		   [ 4]  by_dmss_pwdb_dly			  bypass counter delay chain
	//		   [ 8]  rg_mempll_pwd_en_sel		  0: Use SPM Control, 1: Use S/W Control
	//		   [ 9]  rg_mempll2_pwd_en_sel		  0: Use SPM Control, 1: Use S/W Control
	//		   [10]  rg_mempll3_pwd_en_sel		  0: Use SPM Control, 1: Use S/W Control
	//		   [11]  rg_mempll4_pwd_en_sel		  0: Use SPM Control, 1: Use S/W Control
	//		   [12]  by_mempll_pwd_dly			  bypass counter delay chain
	//		   [13]  by_mempll2_pwd_dly 		  bypass counter delay chain
	//		   [14]  by_mempll3_pwd_dly 		  bypass counter delay chain
	//		   [15]  by_mempll4_pwd_dly 		  bypass counter delay chain
	//		   [16]  rg_mempll_bias_rst_en_sel	  0: Use SPM Control, 1: Use S/W Control
	//		   [20]  by_mempll_bias_rst_dly
	//		   [24]  rg_mempll_bias_pwd_en_sel	  0: Use SPM Control, 1: Use S/W Control
	//		   [28]  by_mempll_bias_pwd_dly 	  bypass counter delay chain
	(*(volatile unsigned int *)(CHA_DDRPHY_BASE + (0x5c8))) = 0x9010f010;

	// spm2dphy_div2_sync = 1
	// r0 = r0 | r1 << PDEF_SC_MEMPLLOUT_OFF_LSB
	read_val = Reg_Readl((SPM_POWER_ON_VAL0));
    Reg_Sync_Writel((SPM_POWER_ON_VAL0), read_val | (0x1 << 13));


	// Rx194
	//	  0  R_SRC_SEL_ASYNC_MODE		1: SPM, 0: S/W
	//	  1  Rsv
	//	  2  Rsv
	//	  3  R_SRC_SEL_DIV2_SYNC		1: SPM, 0: S/W
	//	  4  R_SRC_SEL_1PLL_MODE_SW 	1: SPM, 0: S/W
	(*(volatile unsigned int *)(CHA_DDRPHY_BASE + (0x650))) = 0x00000018; // Enable SPM Control, R_SRC_SEL_ASYNC_MODE = 0
	
	Reg_Sync_Writel((CHA_DDRPHY_BASE + (0x698)), (Reg_Readl(CHA_DDRPHY_BASE + (0x698)) & ~0x000e0000) | new_DMYPAD_option | gENABLE_MEMPLL_CG_TO_SAVE_POWER_val);  // [17,18,19] RESET source selection, 0: SPM, 1: BYPASS, Clr for SPM



	// Rx173
	//			 0	 rg_mempll_iso_en_en_sel	 0: Use SPM Control, 1: Use S/W Control 
	//			 3	 rg_mempll_iso_en_en
	//			 4	 by_mempll_iso_en_dly
	//			 8	 rg_dmpll2_ck_en_en_sel 	 0: Use SPM Control, 1: Use S/W Control
	//			12	 by_dmpll2_ck_en_dly
	//			16	 rg_mempll_div_en_en_sel	 0: Use SPM Control, 1: Use S/W Control
	//			20	 by_mempll_div_en_dly		*bypass counter delay chain  
	//			22	 rg_mempll_div_en_pll_sel
	//			24	 rg_mempll_ddsen_en_sel 	 0: Use SPM Control, 1: Use S/W Control
	//			28	 by_mempll_ddsen_dly		 bypass counter delay chain
	//			30	 by_mempll_chg_dly			 bypass counter delay chain
	(*(volatile unsigned int *)(CHA_DDRPHY_BASE + (0x5cc))) = 0x50101010;	// by_dmpll2_ck_en_dly first, manual enable mempll[12], by_mempll_div_en_dly[20] TBD
#endif	

}

void TransferToRegControl(void)
{
#if fcFOR_CHIP_ID == fcK2
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x5c8), 0x1111ff11);	// [15:12] bypass delay chain for mempllx enable. Need to check??
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x5cc), 0xd1111111);	// by_dmpll2_ck_en_dly first, manual enable mempll, [12] TBD	
#endif	
#if fcFOR_CHIP_ID == fcJADE
	// Enable S/W control (not SPM)
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x5c8), 0x01011f01); // [9]:rg_mempll2_pwd_en_sel, [12]:by_mempll_pwd_dly
	// Enable S/W control (not SPM)
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x5cc), 0x41011101); // by_dmpll2_ck_en_dly first, manual enable mempll[12], by_mempll_div_en_dly[20] TBD
#endif	
}


#endif

#endif

void DisableDramcDramphyDCM(void)
{
	U8 ucstatus = 0;
	U32 u4value;

	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1dc), &u4value);
	mcCLR_BIT(u4value, 31);	
	mcCLR_BIT(u4value, 30);
#if fcFOR_CHIP_ID == fcJADE	
	mcCLR_BIT(u4value, 25);
	mcCLR_BIT(u4value, 1);
#endif
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1dc), u4value);
}

//-------------------------------------------------------------------------
/** DramcEngine1
 *  start the self test engine inside dramc to test dram w/r.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  test2_0         (U32): 16bits,set pattern1 [31:24] and set pattern0 [23:16].
 *  @param  test2_1         (U32): 28bits,base address[27:0].
 *  @param  test2_2         (U32): 28bits,offset address[27:0].
 *  @param  loopforever     (S16):  0 read\write one time ,then exit
 *                                 >0 enable eingie1, after "loopforever" second ,write log and exit
 *                                 -1 loop forever to read\write, every "period" seconds ,check result ,only when we find error,write log and exit
 *                                 -2 loop forever to read\write, every "period" seconds ,write log ,only when we find error,write log and exit
 *                                 -3 just enable loop forever ,then exit
 *  @param period           (U8):  it is valid only when loopforever <0; period should greater than 0
 *  @retval status          (U32): return the value of DM_CMP_ERR  ,0  is ok ,others mean  error
 */
//-------------------------------------------------------------------------
U32 DramcEngine1(DRAMC_CTX_T *p, U32 test2_1, U32 test2_2, S16 loopforever, U8 period)
{
    // This function may not need to be modified unless test engine-1 design has changed

    U8 ucengine_status;
    U8 ucstatus = 0, ucnumber = 0;
    U32 u4value, u4result = 0xffffffff;
    U8 ucloop_count = 0;

    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context is NULL\n"));
        return u4result;;
    }

    // This is TA1 limitation
    // offset must be 0x7ff
    if ((test2_2&0x00ffffff) != 0x000007ff)
    {
        mcSHOW_ERR_MSG(("TA1 offset must be 0x7ff!!\n"));
        mcSET_FIELD(test2_2, 0x7ff, 0x00ffffff, 0);
        mcSHOW_DBG_MSG2(("Force test2_2 to 0x%8x\n", test2_2));
    }
        
    // we get the status 
    // loopforever    period    status    mean 
    //     0             x         1       read\write one time ,then exit ,don't write log 
    //    >0             x         2       read\write in a loop,after "loopforever" seconds ,disable it ,return the R\W status  
    //    -1            >0         3       read\write in a loop,every "period" seconds ,check result ,only when we find error,write log and exit 
    //    -2            >0         4       read\write in a loop,every "period" seconds ,write log ,only when we find error,write log and exit
    //    -3             x         5       just enable loop forever , then exit (so we should disable engine1 outside the function)
    if (loopforever == 0)
    {
        ucengine_status = 1;
    }
    else if (loopforever > 0)
    {
        ucengine_status = 2;
    }
    else if (loopforever == -1)
    {
        if (period > 0)
        {
            ucengine_status = 3;
        }
        else
        {
            mcSHOW_ERR_MSG(("parameter 'status' should be equal or greater than 0\n"));
            return u4result;
        }
    }
    else if (loopforever == -2)
    {
        if (period > 0)
        {
            ucengine_status = 4;
        }
        else
        {
            mcSHOW_ERR_MSG(("parameter 'status' should be equal or greater than 0\n"));
            return u4result;
        }
    }
    else if (loopforever == -3)
    {
        ucengine_status = 5;
    }
    else
    {
        mcSHOW_ERR_MSG(("wrong parameter!\n"));
        mcSHOW_ERR_MSG(("loopforever    period    status    mean \n"));
        mcSHOW_ERR_MSG(("      0                x           1         read/write one time ,then exit ,don't write log\n"));
        mcSHOW_ERR_MSG(("    >0                x           2         read/write in a loop,after [loopforever] seconds ,disable it ,return the R/W status\n"));
        mcSHOW_ERR_MSG(("    -1              >0           3         read/write in a loop,every [period] seconds ,check result ,only when we find error,write log and exit\n"));
        mcSHOW_ERR_MSG(("    -2              >0           4         read/write in a loop,every [period] seconds ,write log ,only when we find error,write log and exit\n"));
        mcSHOW_ERR_MSG(("    -3                x           5         just enable loop forever , then exit (so we should disable engine1 outside the function)\n"));
        return u4result;
    }



    // step
    // 1.select loop forever or not
    // 2.set pattern, base address,offset address
    // 3.enable test1 
    // 4.run different code according status
    // 5.diable test1
    // 6.return DM_CMP_ERR

    if (ucengine_status == 4)
    {
        mcSHOW_DBG_MSG(("============================================\n"));
        mcSHOW_DBG_MSG(("enable test egine1 loop forever\n"));        
        mcSHOW_DBG_MSG(("============================================\n"));
        ucnumber = 1;
    }

    // 1.
    if (loopforever != 0)
    {
        // enable infinite loop
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF1), &u4value);
        mcSET_BIT(u4value, POS_CONF1_TESTLP);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF1), u4value);
    }
    else
    {
        // disable infinite loop
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF1), &u4value);
        mcCLR_BIT(u4value, POS_CONF1_TESTLP);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF1), u4value);
    }
    // 2.
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_1), test2_1);    
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_2), test2_2);    
    // 3.
    // enable test engine 1 (first write and then read???)
    // disable it before enable ,DM_CMP_ERR may not be 0,because may be loopforever and don't disable it before
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), &u4value);
    mcCLR_BIT(u4value, POS_CONF2_TEST1);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4value);

    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), &u4value);
    mcSET_BIT(u4value, POS_CONF2_TEST1);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4value);
    // 4.
    if (ucengine_status == 1)
    {
        // read data compare ready check
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), &u4value);
        // infinite loop??? check DE about the time???
        ucloop_count = 0;
        while(mcCHK_BIT1(u4value, POS_TESTRPT_DM_CMP_CPT) == 0)
        {            
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), &u4value);
            mcDELAY_MS(CMP_CPT_POLLING_PERIOD);
            ucloop_count++;
            if (ucloop_count > MAX_CMP_CPT_WAIT_LOOP)
            {
                mcSHOW_ERR_MSG(("TESTRPT_DM_CMP_CPT polling timeout\n"));
                break;
            }
        }

        // delay 10ns after ready check from DE suggestion (1ms here)
        mcDELAY_MS(1);

        // save  DM_CMP_ERR, 0 is ok ,others are fail,disable test engine 1
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), &u4value);
        u4result = mcCHK_BIT1(u4value, POS_TESTRPT_DM_CMP_ERR);
        mcSHOW_DBG_MSG2(("0x3fc = %d\n", u4value));
    }
    else if (ucengine_status == 2)
    {
        // wait "loopforever" seconds
        mcDELAY_MS(loopforever*1000);
        // get result, no need to check read data compare ready???
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), &u4value);
        u4result = mcCHK_BIT1(u4value, POS_TESTRPT_DM_CMP_ERR);
    }
    else if (ucengine_status == 3)
    {
        while(1)
        {
            // wait "period" seconds
            mcDELAY_MS(period*1000);
            // get result
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), &u4value);
            u4result = mcCHK_BIT1(u4value, POS_TESTRPT_DM_CMP_ERR);
            if (u4result == 0)
            {
                // pass, continue to check
                continue;
            }
            // some bit error
            // write log
            mcSHOW_DBG_MSG(("%d#    CMP_ERR = 0x%8x\n", ucnumber, u4result));
            break;
        }
    }
    else if (ucengine_status == 4)
    {
        while(1)
        {
            // wait "period" seconds
            mcDELAY_MS(period*1000);
            // get result
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), &u4value);
            u4result = mcCHK_BIT1(u4value, POS_TESTRPT_DM_CMP_ERR);

            // write log
            mcSHOW_DBG_MSG(("%d#    CMP_ERR = 0x%8x\n", ucnumber, u4result));
            
            if (u4result == 0)
            {
                // pass, continue to check
                continue;
            }
            // some bit error            
            break;
        }
    }
    else if (ucengine_status == 5)
    {
        // loopforever is  enable ahead ,we just exit this function
        return 0;
    }
    else
    {
    }

    // 5. disable engine1
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), &u4value);
    mcCLR_BIT(u4value, POS_CONF2_TEST1);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4value);
    

    return u4result;
}

//-------------------------------------------------------------------------
/** DramcEngine2
 *  start the self test engine 2 inside dramc to test dram w/r.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  wr              (DRAM_TE_OP_T): TE operation
 *  @param  test2_1         (U32): 28bits,base address[27:0].
 *  @param  test2_2         (U32): 28bits,offset address[27:0]. (unit is 16-byte, i.e: 0x100 is 0x1000).
 *  @param  loopforever     (S16): 0    read\write one time ,then exit
 *                                >0 enable eingie2, after "loopforever" second ,write log and exit
 *                                -1 loop forever to read\write, every "period" seconds ,check result ,only when we find error,write log and exit
 *                                -2 loop forever to read\write, every "period" seconds ,write log ,only when we find error,write log and exit
 *                                -3 just enable loop forever ,then exit
 *  @param period           (U8):  it is valid only when loopforever <0; period should greater than 0
 *  @param log2loopcount    (U8): test loop number of test agent2 loop number =2^(log2loopcount) ,0 one time
 *  @retval status          (U32): return the value of DM_CMP_ERR  ,0  is ok ,others mean  error
 */
//-------------------------------------------------------------------------
U32 DramcEngine2(DRAMC_CTX_T *p, DRAM_TE_OP_T wr, U32 test2_1, U32 test2_2, U8 testaudpat, S16 loopforever, U8 period, U8 log2loopcount)
{
    U8 ucengine_status;
    U8 ucstatus = 0, ucloop_count = 0;
    U32 u4value, u4result = 0xffffffff;
    U32 u4log2loopcount = (U32) log2loopcount;

    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context is NULL\n"));
        return u4result;
    }

    // check loop number validness
//    if ((log2loopcount > 15) || (log2loopcount < 0))		// U8 >=0 always.
    if (log2loopcount > 15)
    {
        mcSHOW_ERR_MSG(("wrong parameter log2loopcount:    log2loopcount just 0 to 15 !\n"));
        return u4result;
    }

    // disable self test engine1 and self test engine2 
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), &u4value);
    mcCLR_MASK(u4value, MASK_CONF2_TE12_ENABLE);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4value);

    // we get the status 
    // loopforever    period    status    mean 
    //     0             x         1       read\write one time ,then exit ,don't write log 
    //    >0             x         2       read\write in a loop,after "loopforever" seconds ,disable it ,return the R\W status  
    //    -1            >0         3       read\write in a loop,every "period" seconds ,check result ,only when we find error,write log and exit 
    //    -2            >0         4       read\write in a loop,every "period" seconds ,write log ,only when we find error,write log and exit
    //    -3             x         5       just enable loop forever , then exit (so we should disable engine1 outside the function)
    if (loopforever == 0)
    {
        ucengine_status = 1;
    }
    else if (loopforever > 0)
    {
        ucengine_status = 2;
    }
    else if (loopforever == -1)
    {
        if (period > 0)
        {
            ucengine_status = 3;
        }
        else
        {
            mcSHOW_ERR_MSG(("parameter 'period' should be equal or greater than 0\n"));
            return u4result;
        }
    }
    else if (loopforever == -2)
    {
        if (period > 0)
        {
            ucengine_status = 4;
        }
        else
        {
            mcSHOW_ERR_MSG(("parameter 'period' should be equal or greater than 0\n"));
            return u4result;
        }
    }
    else if (loopforever == -3)
    {
        if (period > 0)
        {
            ucengine_status = 5;
        }
        else
        {
            mcSHOW_ERR_MSG(("parameter 'period' should be equal or greater than 0\n"));
            return u4result;
        }
    }
    else
    {
        mcSHOW_ERR_MSG(("parameter 'loopforever' should be 0 -1 -2 -3 or greater than 0\n"));        
        return u4result;
    }


    // 1.set pattern ,base address ,offset address
    // 2.select  ISI pattern or audio pattern or xtalk pattern
    // 3.set loop number
    // 4.enable read or write
    // 5.loop to check DM_CMP_CPT
    // 6.return CMP_ERR
    // currently only implement ucengine_status = 1, others are left for future extension    
    /*if (ucengine_status == 4)
    {
        mcSHOW_DBG_MSG(("============================================\n"));
        mcSHOW_DBG_MSG(("enable test egine2 loop forever\n"));        
        mcSHOW_DBG_MSG(("============================================\n"));        
    }*/
    u4result = 0;
    while(1)
    {
        // 1
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_1), test2_1);    
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_2), test2_2);    

        // 2 & 3
        // (TESTXTALKPAT, TESTAUDPAT) = 00 (ISI), 01 (AUD), 10 (XTALK), 11 (UNKNOWN)
        if (testaudpat == 2)   // xtalk
        {
            // select XTALK pattern
            // set addr 0x044 [7] to 0
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), &u4value);
            mcCLR_BIT(u4value, POS_TEST2_3_TESTAUDPAT);
            mcSET_FIELD(u4value, u4log2loopcount, MASK_TEST2_3_TESTCNT, POS_TEST2_3_TESTCNT);
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), u4value);

            // set addr 0x48[16] to 1, TESTXTALKPAT = 1
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), &u4value);
            mcSET_BIT(u4value,POS_TEST2_4_TESTXTALKPAT);
            mcCLR_BIT(u4value, POS_TEST2_4_TESTAUDBITINV);
            mcCLR_BIT(u4value, POS_TEST2_4_TESTAUDMODE);  // for XTALK pattern characteristic, we don' t enable write after read
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), u4value);
        }
        else if (testaudpat == 1)   // audio
        {
            // set AUDINIT=0x11 AUDINC=0x0d AUDBITINV=1 AUDMODE=1
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), &u4value);
            mcSET_FIELD(u4value, 0x00000011, MASK_TEST2_4_TESTAUDINIT, POS_TEST2_4_TESTAUDINIT);
            mcSET_FIELD(u4value, 0x0000000d, MASK_TEST2_4_TESTAUDINC, POS_TEST2_4_TESTAUDINC);
            mcSET_BIT(u4value, POS_TEST2_4_TESTAUDBITINV);
            mcSET_BIT(u4value, POS_TEST2_4_TESTAUDMODE);
            mcCLR_BIT(u4value,POS_TEST2_4_TESTXTALKPAT);     // Edward : This bit needs to be disable in audio. Otherwise will fail.
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), u4value);

            // set addr 0x044 [7] to 1 ,select audio pattern
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), &u4value);
            mcSET_BIT(u4value, POS_TEST2_3_TESTAUDPAT);
            mcSET_FIELD(u4value, u4log2loopcount, MASK_TEST2_3_TESTCNT, POS_TEST2_3_TESTCNT);
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), u4value);
        } 
        else   // ISI
        {
            // select ISI pattern
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), &u4value);
            mcCLR_BIT(u4value, POS_TEST2_3_TESTAUDPAT);
            mcSET_FIELD(u4value, u4log2loopcount, MASK_TEST2_3_TESTCNT, POS_TEST2_3_TESTCNT);
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), u4value);  

            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), &u4value);
            mcCLR_BIT(u4value,POS_TEST2_4_TESTXTALKPAT);
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), u4value);
        }

        // 4
        if (wr == TE_OP_READ_CHECK)
        {
            if ((testaudpat == 1) || (testaudpat == 2))
            {
                //if audio pattern, enable read only (disable write after read), AUDMODE=0x48[15]=0
                ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), &u4value);
                mcCLR_BIT(u4value, POS_TEST2_4_TESTAUDMODE);
                ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), u4value);
            }
            
            // enable read, 0x008[31:29]
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), &u4value);
            mcSET_FIELD(u4value, (U32) 2, MASK_CONF2_TE12_ENABLE, POS_CONF2_TEST1);
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4value);            
        }
        else if (wr == TE_OP_WRITE_READ_CHECK)
        {
            // enable write
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), &u4value);
            mcSET_FIELD(u4value, (U32) 4, MASK_CONF2_TE12_ENABLE, POS_CONF2_TEST1);
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4value);

            // read data compare ready check
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), &u4value);
            ucloop_count = 0;
            while(mcCHK_BIT1(u4value, POS_TESTRPT_DM_CMP_CPT) == 0)
            {
                ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), &u4value);
/*            mcDELAY_US(CMP_CPT_POLLING_PERIOD);
                ucloop_count++;
                if (ucloop_count > MAX_CMP_CPT_WAIT_LOOP)
                {
                    //mcSHOW_ERR_MSG(("TESTRPT_DM_CMP_CPT polling timeout: %d\n", ucloop_count));
                #ifndef fcWAVEFORM_MEASURE // for testing, waveform ,measurement
                    break;
                #endif
                }*/
            }
            
            // disable write
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), &u4value);
            mcSET_FIELD(u4value, (U32) 0, MASK_CONF2_TE12_ENABLE, POS_CONF2_TEST1);
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4value);

            // enable read
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), &u4value);
            mcSET_FIELD(u4value, (U32) 2, MASK_CONF2_TE12_ENABLE, POS_CONF2_TEST1);
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4value);
        }

        // 5
        // read data compare ready check
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), &u4value);        
        ucloop_count = 0;
        while(mcCHK_BIT1(u4value, POS_TESTRPT_DM_CMP_CPT) == 0)
        {
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), &u4value);
/*            mcDELAY_US(CMP_CPT_POLLING_PERIOD);
            ucloop_count++;
            if (ucloop_count > MAX_CMP_CPT_WAIT_LOOP)
            {
                mcSHOW_ERR_MSG(("TESTRPT_DM_CMP_CPT polling timeout\n"));
            #ifndef fcWAVEFORM_MEASURE // for testing, waveform ,measurement
                break;
            #endif
            }*/
        }

        // delay 10ns after ready check from DE suggestion (1ms here)
        mcDELAY_US(1);
        
        // 6
        // return CMP_ERR, 0 is ok ,others are fail,diable test2w or test2r
        // get result
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CMP_ERR), &u4value);
        // or all result
        u4result |= u4value;
        // disable read
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), &u4value);
        mcCLR_MASK(u4value, MASK_CONF2_TE12_ENABLE);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4value);

        // handle status
        if (ucengine_status == 1)
        {
            break;    
        }
        else if (ucengine_status == 2)
        {
            mcSHOW_ERR_MSG(("not support for now\n"));
            break;
        }
        else if (ucengine_status == 3)
        {
            mcSHOW_ERR_MSG(("not support for now\n"));
            break;
        }
        else if (ucengine_status == 4)
        {
            mcSHOW_ERR_MSG(("not support for now\n"));
            break;
        }
        else if (ucengine_status == 5)
        {
            mcSHOW_ERR_MSG(("not support for now\n"));
            break;
        }
        else
        {
            mcSHOW_ERR_MSG(("not support for now\n"));
            break;
        }
    }

    return u4result;    
}


#ifdef XTALK_SSO_STRESS
//-------------------------------------------------------------------------
/** DramcWorstPat_mem
 *  Write HFID worst pattern to memory
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param src_addr         (U32): DRAM src address
 *  @retval status          (U32): return the value of ERR_VALUE, 0 is ok, others mean  error
 */
//-------------------------------------------------------------------------
void DramcWorstPat_mem(DRAMC_CTX_T *p, U32 src_addr)
{
    U8 ii, jj, kk, repeat;

    //mcSHOW_DBG_MSG(("START to write worst pattern to memory...\n"));
    
    // XTALK worst pattern
    for (ii =0 ; ii < 76; ii++)
    {
        ucDram_Write(src_addr, u4xtalk_pat[ii]);
        src_addr +=4;
    }
    // SSO worst pattern
    //  sso_bs1A
    //   sso1x_bs ~ sso6x_bs
    for (ii = 1; ii <= 6; ii++)
    {
        repeat = 7;

        for (jj = 0; jj <= repeat; jj++)
        {
            for (kk = 0; kk < ii; kk++)
            {
                // send "1"
                ucDram_Write(src_addr, 0xffffffff);
                src_addr +=4;
            }
            for (kk = 0; kk < ii; kk++)
            {
                // send "0"
                ucDram_Write(src_addr, 0x00000000);
                src_addr +=4;
            } 
        }

        // step_1x
        ucDram_Write(src_addr, 0x00000000);
        src_addr +=4;
    }

    //   sso6x_bs
    repeat = 7;
    for (jj = 0; jj <= repeat; jj++)
    {
        for (kk = 0; kk < 6; kk++)
        {
            // send "1"
            ucDram_Write(src_addr, 0xffffffff);
            src_addr +=4;
        }
        for (kk = 0; kk < 6; kk++)
        {
            // send "0"
            ucDram_Write(src_addr, 0x00000000);
            src_addr +=4;
        } 
    }

    //   step_1y
    ucDram_Write(src_addr, 0xffffffff);
    src_addr +=4;

    //   spb0x
    for (ii =0 ; ii < 64; ii++)
    {
        ucDram_Write(src_addr, u4spb0x[ii]);
        src_addr +=4;
    }

    //  sso_bs2A
    for (ii = 0; ii < 6; ii++)
    {
        repeat = ucsso_bsx[ii][2];

        for (jj = 0; jj <= repeat; jj++)
        {
            for (kk = 0; kk < ucsso_bsx[ii][0]; kk++)
            {
                // send "1"
                ucDram_Write(src_addr, 0xffffffff);
                src_addr +=4;
            }
            for (kk = 0; kk < ucsso_bsx[ii][1]; kk++)
            {
                // send "0"
                ucDram_Write(src_addr, 0x00000000);
                src_addr +=4;
            } 
        }

        // END; step_xx
        for (kk = 0; kk < ucsso_bsx[ii][3]; kk++)
        {
            // send "1"
            ucDram_Write(src_addr, 0xffffffff);
            src_addr +=4;
        }
        for (kk = 0; kk < ucsso_bsx[ii][4]; kk++)
        {
            // send "0"
            ucDram_Write(src_addr, 0x00000000);
            src_addr +=4;
        }                
    }

//    mcSHOW_DBG_MSG(("END to write worst pattern to memory! addr=%xh %d size=%xh %d\n", 
//    	src_addr, src_addr, src_addr-size, src_addr-size));
}

#ifdef NEW_WORST_PAT_64

void DramcWorstPat_mem_64(DRAMC_CTX_T *p, U32 src_addr)
{
    EMI_SETTINGS *emi_set;
    
    if(emi_setting_index == -1)
        emi_set = default_emi_setting;
    else
        emi_set = &emi_settings[emi_setting_index];  
    
	*(volatile unsigned *)(EMI_APB_BASE+0x00000000) &= (~0x01);	// disable dual channel.

	p->channel = CHANNEL_A;
	*(volatile unsigned *)(EMI_APB_BASE+0x00000000) &= (~(0x01<<18));
	DramcWorstPat_mem((DRAMC_CTX_T *)p, (U32)src_addr>>1);

	p->channel = CHANNEL_B;
	*(volatile unsigned *)(EMI_APB_BASE+0x00000000) |= ((0x01<<18));
	DramcWorstPat_mem((DRAMC_CTX_T *)p, (U32)src_addr>>1);

    *(volatile unsigned *)(EMI_APB_BASE+0x00000000) = emi_set->EMI_CONA_VAL;
}

#else

void DramcWorstPat_mem_64(DRAMC_CTX_T *p, U32 src_addr)
{
    U8 ii, jj, kk, repeat;
    U32 size=src_addr;

    //mcSHOW_DBG_MSG(("START to write worst pattern to memory...\n"));
    
    // XTALK worst pattern
    for (ii =0 ; ii < 76*2; ii++)
    {
        ucDram_Write(src_addr, u4xtalk_pat_64[ii]);
        src_addr +=4;
    }
    // SSO worst pattern
    //  sso_bs1A
    //   sso1x_bs ~ sso6x_bs
    for (ii = 1; ii <= 6; ii++)
    {
        repeat = 7;

        for (jj = 0; jj <= repeat; jj++)
        {
            for (kk = 0; kk < ii; kk++)
            {
                // send "1"
                ucDram_Write(src_addr, 0xffffffff);
                src_addr +=4;
                ucDram_Write(src_addr, 0xffffffff);
                src_addr +=4;
            }
            for (kk = 0; kk < ii; kk++)
            {
                // send "0"
                ucDram_Write(src_addr, 0x00000000);
                src_addr +=4;
                ucDram_Write(src_addr, 0x00000000);
                src_addr +=4;
            } 
        }

        // step_1x
        ucDram_Write(src_addr, 0x00000000);
        src_addr +=4;
        ucDram_Write(src_addr, 0x00000000);
        src_addr +=4;
    }

    //   sso6x_bs
    repeat = 7;
    for (jj = 0; jj <= repeat; jj++)
    {
        for (kk = 0; kk < 6; kk++)
        {
            // send "1"
            ucDram_Write(src_addr, 0xffffffff);
            src_addr +=4;
            ucDram_Write(src_addr, 0xffffffff);
            src_addr +=4;        }
        for (kk = 0; kk < 6; kk++)
        {
            // send "0"
            ucDram_Write(src_addr, 0x00000000);
            src_addr +=4;
            ucDram_Write(src_addr, 0x00000000);
            src_addr +=4;
        } 
    }

    //   step_1y
    ucDram_Write(src_addr, 0xffffffff);
    src_addr +=4;
    ucDram_Write(src_addr, 0xffffffff);
    src_addr +=4;
    
    //   spb0x
    for (ii =0 ; ii < 64*2; ii++)
    {
        ucDram_Write(src_addr, u4spb0x_64[ii]);
        src_addr +=4;
    }

    //  sso_bs2A
    for (ii = 0; ii < 6; ii++)
    {
        repeat = ucsso_bsx[ii][2];

        for (jj = 0; jj <= repeat; jj++)
        {
            for (kk = 0; kk < (ucsso_bsx[ii][0]*2); kk++)
            {
                // send "1"
                ucDram_Write(src_addr, 0xffffffff);
                src_addr +=4;
            }
            for (kk = 0; kk < (ucsso_bsx[ii][1]*2); kk++)
            {
                // send "0"
                ucDram_Write(src_addr, 0x00000000);
                src_addr +=4;
            } 
        }

        // END; step_xx
        for (kk = 0; kk < (ucsso_bsx[ii][3]*2); kk++)
        {
            // send "1"
            ucDram_Write(src_addr, 0xffffffff);
            src_addr +=4;
        }
        for (kk = 0; kk < (ucsso_bsx[ii][4]*2); kk++)
        {
            // send "0"
            ucDram_Write(src_addr, 0x00000000);
            src_addr +=4;
        }                
    }

//    mcSHOW_DBG_MSG(("END to write worst pattern to memory! addr=%xh %d size=%xh %d\n", 
//    	src_addr, src_addr, src_addr-size, src_addr-size));
}

#endif

//U32 uiPatLenB = 3584;  // Xtalk + SSO
U32 uiPatLenB;  // Xtalk + SSO

void DramcWorstPat(DRAMC_CTX_T *p, U32 src_addr, U8 ChannelNum)
{

}

U32 DramcDmaEngine(DRAMC_CTX_T *p, DRAM_DMA_OP_T op, U32 src_addr, U32 dst_addr, U32 trans_len, U8 burst_len, U8 check_result, U8 ChannelNum)
{
	int err =  0;
	unsigned int data, uiPatRange;
	int i;
	unsigned int uiCompErr = 0, len;
	unsigned int *src_buffp1;
	unsigned int *dst_buffp1;

	if (ChannelNum==1)
	{
		uiPatLenB = XTALK_SSO_LEN;
	}
	else
	{
		uiPatLenB = XTALK_SSO_LEN << 1;
	}
	
	if (src_addr) 
	{
		src_buffp1 = (unsigned int *)src_addr;
	} 
	else 
	{
		src_buffp1 = (unsigned int *)(0x40000000);
	}
	
	if (dst_addr) 
	{
		dst_buffp1 = (unsigned int *)dst_addr;
	} 
	else 
	{
		dst_buffp1 = (unsigned int *)0x50000000;
	}

	len = trans_len;
	
	if (len < uiPatLenB) {
		// Should be larger.
		len = uiPatLenB;
	}
	// Have the DMA length to be the multiple of uiPatternLen.
	len = ((len+uiPatLenB-1)/uiPatLenB) * uiPatLenB;


	if (check_result == 0x0f)
	{
		// Only do comparison.
		goto DMA_COMPARE;
	}

	if (check_result != 0)
	{
	for (i = 0 ; i < (len/sizeof(unsigned int)) ; i++) {
	    *((unsigned int *)dst_buffp1+i) = 0;
	}

		if (ChannelNum==1)
		{
			DramcWorstPat_mem((DRAMC_CTX_T *)p, (U32)src_buffp1-DDR_BASE);
		}
		else
		{
			DramcWorstPat_mem_64((DRAMC_CTX_T *)p, (U32)src_buffp1-DDR_BASE);
		}
		
		for (i = 0 ; i < len ; i+=uiPatLenB) {
			memcpy((void *)src_buffp1 + i, src_buffp1, uiPatLenB);
		}
	}

DMA_INIT:	
	 *((volatile unsigned int *)(0x10212c18)) = (DMA_BURST_LEN & 0x07)  << 16; //BURST_LEN:7-8,R/W
	 *((volatile unsigned int *)(0x10212c1c)) = (unsigned int)src_buffp1;
	 *((volatile unsigned int *)(0x10212c20)) = (unsigned int)dst_buffp1;
	 *((volatile unsigned int *)(0x10212c24)) = len;
	 dsb();
	 *((volatile unsigned int *)(0x10212c08)) = 0x1; //start dma

	if (check_result == 0)
	{	
		// Not wait and return.
		return uiCompErr;
	}

DMA_COMPARE:
	
	 while(*((volatile unsigned int *)(0x10212c08))) ;

#ifdef WAVEFORM_MEASURE
	 goto DMA_INIT;
#endif

	for (i = 0 ; i < (len/sizeof(unsigned int)) ; i++)
	{
		#ifdef DUMMY_READ
		unsigned int DummyRead = *((volatile unsigned int *)dst_buffp1+i);
		DummyRead = *((volatile unsigned int *)dst_buffp1+i);
		DummyRead = *((volatile unsigned int *)src_buffp1+i);
		DummyRead = *((volatile unsigned int *)src_buffp1+i);
		#endif
		if (*((unsigned int *)dst_buffp1+i) != *((unsigned int *)src_buffp1+i)) 
		{
			err = 1;
	   		uiCompErr |= ((*((unsigned int *)dst_buffp1+i)) ^ (*((unsigned int *)src_buffp1+i)));
			#ifdef DMA_ERROR_DISPLAY
			mcSHOW_DBG_MSG2(("Source %xh = %xh Destination %xh= %xh diff=%xh\n", 
				(unsigned int)((unsigned int *)src_buffp1+i), (*((unsigned int *)src_buffp1+i)), 
				(unsigned int)((unsigned int *)dst_buffp1+i), (*((unsigned int *)dst_buffp1+i)),
				(*((unsigned int *)src_buffp1+i)) ^ (*((unsigned int *)dst_buffp1+i))));
			#endif      	   	
		}
        }
	
	#ifdef DMA_ERROR_STOP
	    if (uiCompErr) {
		mcSHOW_DBG_MSG2(("Enter to continue...\n"));
		while (1);
		/*
		{
			if ( UART_Get_Command() )  {    
				 break;
			 }
		}*/
	    }
	#endif

    return uiCompErr;	 
}


#endif

#ifdef SUSPEND_TEST

void Suspend_Resume(DRAMC_CTX_T *p)
{
	U8 ucstatus = 0;
	U32 u4value;

	mcSHOW_DBG_MSG(("MEMPLL 1 suspend...\n"));

//Lewis@20150820: ETT will not use TransferToSPMControl() except low power scenario test code
#if !(__ETT__)
	TransferToRegControl();
#endif

#if 1	
	// RG_MEMPLLx_EN = 0, Reg.614h[0], Reg.620h[0], Reg.62ch[0]
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x614), &u4value);
	mcCLR_BIT(u4value, 0);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), u4value);
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x620), &u4value);
	mcCLR_BIT(u4value, 0);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), u4value);
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x62c), &u4value);
	mcCLR_BIT(u4value, 0);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62c), u4value);

	// RG_MEMPLL_EN = 0, Reg.604h[28]
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x604), &u4value);
	mcCLR_BIT(u4value, 28);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x604), u4value);

	mcDELAY_US(20);	

	// RG_MEMPLL_EN = 1, Reg.604h[28]
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x604), &u4value);
	mcSET_BIT(u4value, 28);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x604), u4value);

	mcDELAY_US(20);

	// RG_MEMPLLx_EN = 0, Reg.614h[0], Reg.620h[0], Reg.62ch[0]
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x614), &u4value);
	mcSET_BIT(u4value, 0);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x614), u4value);
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x620), &u4value);
	mcSET_BIT(u4value, 0);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x620), u4value);
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x62c), &u4value);
	mcSET_BIT(u4value, 0);
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x62c), u4value);
	
	mcDELAY_US(20);

	DramcPhyReset(p);
	DramcDiv2PhaseSync(p);
#endif

//Lewis@20150820: ETT will not use TransferToSPMControl() except low power scenario test code
#if !(__ETT__)
	TransferToSPMControl();
#endif

	mcSHOW_DBG_MSG(("MEMPLL 1 resume...\n"));
	
}
#endif

#ifdef DDR_FT_LOAD_BOARD
void LoadBoardGpioInit(void)
{
	U32 u4temp;
	
	//Set GPIO mode registers (3bits each)
	u4temp = (*(volatile unsigned int *)(0x10005000 + (0x600)));
	// GPIO0
	mcSET_FIELD(u4temp, 0x0, 0x00000007, 0);
	// GPIO2
	mcSET_FIELD(u4temp, 0x0, 0x000001c0, 6);
	// GPIO3
	mcSET_FIELD(u4temp, 0x0, 0x00000e00, 9);
	// GPIO4
	mcSET_FIELD(u4temp, 0x0, 0x00007000, 12);	
	(*(volatile unsigned int *)(0x10005000 + (0x600))) = u4temp;

	u4temp = (*(volatile unsigned int *)(0x10005000 + (0x610)));
	// GPIO5
	//mcSET_FIELD(u4temp, 0x0, 0x00000007, 0);
	// GPIO6
	mcSET_FIELD(u4temp, 0x0, 0x00000038, 3);
	// GPIO7
	mcSET_FIELD(u4temp, 0x0, 0x000001c0, 6);
	// GPIO8
	mcSET_FIELD(u4temp, 0x0, 0x00000e00, 9);
	// GPIO9
	mcSET_FIELD(u4temp, 0x0, 0x00007000, 12);
	(*(volatile unsigned int *)(0x10005000 + (0x610))) = u4temp;

	// Set GPIO direction
	u4temp = (*(volatile unsigned int *)(0x10005000 + (0x000)));
	mcSET_BIT(u4temp, 0);
	mcSET_BIT(u4temp, 2);
	mcSET_BIT(u4temp, 3);
	mcSET_BIT(u4temp, 4);
	//mcSET_BIT(u4temp, 5);
	mcSET_BIT(u4temp, 6);
	mcSET_BIT(u4temp, 7);
	mcSET_BIT(u4temp, 8);
	mcSET_BIT(u4temp, 9);
	(*(volatile unsigned int *)(0x10005000 + (0x000))) = u4temp;
}
void GpioOutput(U8 gpio_no, U8 low_high)
{
	U32 u4temp;
	
	if(low_high == 0)
	{
		u4temp = (*(volatile unsigned int *)(0x10005000 + (0x400)));
		mcCLR_BIT(u4temp, gpio_no); // GPIO0~15
		(*(volatile unsigned int *)(0x10005000 + (0x400))) = u4temp;
	}
	else
	{
		u4temp = (*(volatile unsigned int *)(0x10005000 + (0x400)));
		mcSET_BIT(u4temp, gpio_no); // GPIO0~15
		(*(volatile unsigned int *)(0x10005000 + (0x400))) = u4temp;
	}
}
void LoadBoardShowResult(U8 step, U8 error_type, U8 channel, U8 complete)
{
	mcSHOW_DBG_MSG(("result: "));
	switch(complete)
	{
		case FLAG_NOT_COMPLETE_OR_FAIL:
			//GpioOutput(7, 0);
			GpioOutput(9, 0);
			mcSHOW_DBG_MSG(("0"));
			break;
		case FLAG_COMPLETE_AND_PASS:
			//GpioOutput(7, 1);
			GpioOutput(9, 1);
			mcSHOW_DBG_MSG(("1"));
			break;
		default:
			break;
	}

	switch(channel)
	{
		case CHANNEL_A:
			GpioOutput(8, 0);
			mcSHOW_DBG_MSG(("0"));
			break;
		case CHANNEL_B:
			GpioOutput(8, 1);
			mcSHOW_DBG_MSG(("1"));
			break;
		default:
			break;
	}

	switch(error_type)
	{
		case FLAG_CALIBRATION_PASS:
			GpioOutput(7, 0);
			GpioOutput(6, 0);
			mcSHOW_DBG_MSG(("00"));
			break;
		case FLAG_WINDOW_TOO_SMALL:
			GpioOutput(7, 0);
			GpioOutput(6, 1);
			mcSHOW_DBG_MSG(("01"));
			break;
		case FLAG_WINDOW_TOO_BIG:
			GpioOutput(7, 1);
			GpioOutput(6, 0);
			mcSHOW_DBG_MSG(("10"));
			break;
		case FLAG_CALIBRATION_FAIL:
			GpioOutput(7, 1);
			GpioOutput(6, 1);
			mcSHOW_DBG_MSG(("11"));
			break;
		default:
			break;
	}

	switch(step)
	{
		case FLAG_PLLPHASE_CALIBRATION:
			GpioOutput(4, 0);
			GpioOutput(3, 0);
			GpioOutput(2, 0);
			GpioOutput(0, 0);
			mcSHOW_DBG_MSG(("0000"));
			break;
		case FLAG_PLLGPPHASE_CALIBRATION:
			GpioOutput(4, 0);
			GpioOutput(3, 0);
			GpioOutput(2, 0);
			GpioOutput(0, 1);
			mcSHOW_DBG_MSG(("0001"));
			break;
		case FLAG_IMPEDANCE_CALIBRATION:
			GpioOutput(4, 0);
			GpioOutput(3, 0);
			GpioOutput(2, 1);
			GpioOutput(0, 0);
			mcSHOW_DBG_MSG(("0010"));
			break;
		case FLAG_CA_CALIBRATION:
			GpioOutput(4, 0);
			GpioOutput(3, 0);
			GpioOutput(2, 1);
			GpioOutput(0, 1);
			mcSHOW_DBG_MSG(("0011"));
			break;
		case FLAG_WL_CALIBRATION:
			GpioOutput(4, 0);
			GpioOutput(3, 1);
			GpioOutput(2, 0);
			GpioOutput(0, 0);
			mcSHOW_DBG_MSG(("0100"));
			break;
		case FLAG_GATING_CALIBRATION:
			GpioOutput(4, 0);
			GpioOutput(3, 1);
			GpioOutput(2, 0);
			GpioOutput(0, 1);
			mcSHOW_DBG_MSG(("0101"));
			break;
		case FLAG_RX_CALIBRATION:
			GpioOutput(4, 0);
			GpioOutput(3, 1);
			GpioOutput(2, 1);
			GpioOutput(0, 0);
			mcSHOW_DBG_MSG(("0110"));
			break;
		case FLAG_DATLAT_CALIBRATION:
			GpioOutput(4, 0);
			GpioOutput(3, 1);
			GpioOutput(2, 1);
			GpioOutput(0, 1);
			mcSHOW_DBG_MSG(("0111"));
			break;
		case FLAG_TX_CALIBRATION:
			GpioOutput(4, 1);
			GpioOutput(3, 0);
			GpioOutput(2, 0);
			GpioOutput(0, 0);
			mcSHOW_DBG_MSG(("1000"));
			break;
		default:
			break;
	}

	mcSHOW_DBG_MSG(("\n"));
}
#endif


