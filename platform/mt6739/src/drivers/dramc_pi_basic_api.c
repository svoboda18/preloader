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

/** @file pi_basic_api.c
 *  Basic DRAMC API implementation
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
//#include "..\Common\pd_common.h"
//#include "Register.h"
#include "dramc_common.h"
#include "x_hal_io.h"
#if !__ETT__
#include "emi.h"
#endif

//#include "DramC_reg.h"
//#include "System_reg.h"
//#include "string.h"
//#define mcFPRINTF(_x_)            do{}while(0)




//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
#if REG_ACCESS_PORTING_DGB
extern U8 RegLogEnable;
#endif
U8 u1PrintModeRegWrite=0;

U32 PLL_FBKDIV_6_0, PLL_IC_3_0, PLL_BR_3_0, PLL_BC_3_0, PLL_BC_4;
U32 PLL_BP_3_0, PLL_BP_4, PLL_BAND_6_0;

U32 PLL_FBKSEL_0;


const U32 uiLPDDR4_MRR_Mapping_POP[16] =
{
    0, 1, 2, 3, 4, 5, 6, 7,
    8, 9, 10, 11, 12, 13, 14, 15
};

const U32 uiLPDDR3_MRR_Mapping_POP[16] =
{
    24, 29, 25, 30, 28, 26, 27, 31,
    6, 5, 7, 4, 0, 2, 1, 3
};

#if DUAL_FREQ_K
U8 gu1PhyPLLEn=TRUE;
#endif

#if 0
//{runtime, MR23, MR1819}
const U16 TX3200_Runtime_tDQS2DQ[3][31] =
{
    { 40,  4, 100},
    { 50,  5, 114},
    { 60,  6, 125},
    { 70,  7, 135},
    { 80,  8, 143},
    { 90,  9, 150},
    {100, 10, 156},
    {120, 12, 176},
    {130, 13, 181},
    {150, 15, 197},
    {160, 16, 200},
    {180, 18, 214},
    {200, 20, 227},
    {220, 22, 239},
    {240, 24, 250},
    {260, 26, 260},
    {280, 28, 269},
    {300, 30, 278},
    {320, 32, 286},
    {340, 34, 293},
    {370, 37, 308},
    {390, 39, 315},
    {420, 42, 328},
    {450, 45, 341},
    {470, 47, 346},
    {500, 50, 357},
    {530, 53, 368},
    {560, 56, 378},
    {590, 59, 388},
    {620, 62, 397},
    {660, 66, 413}
};

const U16 TX2667_Runtime_tDQS2DQ[3][31] =
{
    { 36,  3,  90},
    { 36,  3,  82},
    { 48,  4, 100},
    { 60,  5, 115},
    { 72,  6, 129},
    { 72,  6, 120},
    { 84,  7, 131},
    { 96,  8, 141},
    {108,  9, 150},
    {120, 10, 158},
    {132, 11, 165},
    {156, 13, 186},
    {168, 14, 191},
    {180, 15, 196},
    {192, 16, 200},
    {216, 18, 216},
    {228, 19, 219},
    {252, 21, 233},
    {264, 22, 236},
    {288, 24, 248},
    {312, 26, 260},
    {324, 27, 261},
    {348, 29, 272},
    {372, 31, 282},
    {396, 33, 291},
    {420, 35, 300},
    {444, 37, 308},
    {468, 39, 316},
    {492, 41, 324},
    {516, 43, 331},
    {552, 46, 345}
};

const U16 TX1600_Runtime_tDQS2DQ[3][31] =
{
    { 20,  1,  50},
    { 20,  1,  45},
    { 20,  1,  42},
    { 40,  2,  77},
    { 40,  2,  71},
    { 40,  2,  67},
    { 60,  3,  94},
    { 60,  3,  88},
    { 60,  3,  83},
    { 80,  4, 105},
    { 80,  4, 100},
    {100,  5, 119},
    {100,  5, 114},
    {100,  5, 109},
    {120,  6, 125},
    {120,  6, 120},
    {140,  7, 135},
    {140,  7, 130},
    {160,  8, 143},
    {180,  9, 155},
    {180,  9, 150},
    {200, 10, 161},
    {200, 10, 156},
    {220, 11, 167},
    {240, 12, 176},
    {260, 13, 186},
    {260, 13, 181},
    {280, 14, 189},
    {300, 15, 197},
    {320, 16, 205},
    {320, 16, 200}
};
#endif

void DramcPrintRXDQDQSStatus(DRAMC_CTX_T *p, U8 u1Channel);
void DramcPrintIMPTrackingStatus(DRAMC_CTX_T *p, U8 u1Channel);

#if __ETT__ //Darren+
unsigned int mt_get_chip_hw_ver(void)
{
    U32 tmp;

    tmp = *((UINT32P)(APHW_VER));

    return tmp;
}
#elif(FOR_DV_SIMULATION_USED==1 || SW_CHANGE_FOR_SIMULATION==1)
unsigned int mt_get_chip_hw_ver(void)
{
    return 0;
}
CHIP_SW_VER mt_get_chip_sw_ver(void)
{
    return 0;
}
#endif


#if 0//__ETT__ //Darren+
CHIP_SW_VER mt_get_chip_sw_ver(void)
{
    CHIP_SW_VER sw_ver;
    U32 ver;
    //    unsigned int hw_subcode = DRV_Reg32(APHW_SUBCODE);

    ver = *((UINT32P)(APSW_VER));
    if ( 0x0 == ver )
        sw_ver = CHIP_SW_VER_01;
    else
        sw_ver = CHIP_SW_VER_02;

    return sw_ver;
}
#endif

void DramcBroadcastOnOff(U32 bOnOff)
{
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    *((volatile unsigned int *)(DRAMC_WBR)) = bOnOff;
#endif
}

U8 u1IsLP4Family(DRAM_DRAM_TYPE_T dram_type)
{
    if (dram_type == TYPE_LPDDR4 || dram_type == TYPE_LPDDR4X || dram_type == TYPE_LPDDR4P)
        return 1;
    else
        return 0;
}


void Set_MRR_Pinmux_Mapping(DRAMC_CTX_T *p)
{
    U32 *uiLPDDR_MRR_Mapping;

#if (FOR_DV_SIMULATION_USED==0)
    if (p->dram_type == TYPE_LPDDR4 || p->dram_type == TYPE_LPDDR4X || p->dram_type == TYPE_LPDDR4P)
    {
        uiLPDDR_MRR_Mapping = (U32 *)uiLPDDR4_MRR_Mapping_POP;
    }
    else
    {
        uiLPDDR_MRR_Mapping = (U32 *)uiLPDDR3_MRR_Mapping_POP;
    }

    //Set MRR pin mux
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_MRR_BIT_MUX1), P_Fld( uiLPDDR_MRR_Mapping[0], MRR_BIT_MUX1_MRR_BIT0_SEL) | P_Fld( uiLPDDR_MRR_Mapping[1], MRR_BIT_MUX1_MRR_BIT1_SEL) |
                                                               P_Fld( uiLPDDR_MRR_Mapping[2], MRR_BIT_MUX1_MRR_BIT2_SEL) | P_Fld( uiLPDDR_MRR_Mapping[3], MRR_BIT_MUX1_MRR_BIT3_SEL));
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_MRR_BIT_MUX2), P_Fld( uiLPDDR_MRR_Mapping[4], MRR_BIT_MUX2_MRR_BIT4_SEL) | P_Fld( uiLPDDR_MRR_Mapping[5], MRR_BIT_MUX2_MRR_BIT5_SEL) |
                                                               P_Fld( uiLPDDR_MRR_Mapping[6], MRR_BIT_MUX2_MRR_BIT6_SEL) | P_Fld( uiLPDDR_MRR_Mapping[7], MRR_BIT_MUX2_MRR_BIT7_SEL));
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_MRR_BIT_MUX3), P_Fld( uiLPDDR_MRR_Mapping[8], MRR_BIT_MUX3_MRR_BIT8_SEL) | P_Fld( uiLPDDR_MRR_Mapping[9], MRR_BIT_MUX3_MRR_BIT9_SEL) |
                                                               P_Fld( uiLPDDR_MRR_Mapping[10], MRR_BIT_MUX3_MRR_BIT10_SEL) | P_Fld(uiLPDDR_MRR_Mapping[11], MRR_BIT_MUX3_MRR_BIT11_SEL));
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_MRR_BIT_MUX4), P_Fld( uiLPDDR_MRR_Mapping[12], MRR_BIT_MUX4_MRR_BIT12_SEL) | P_Fld(uiLPDDR_MRR_Mapping[13], MRR_BIT_MUX4_MRR_BIT13_SEL) |
                                                               P_Fld( uiLPDDR_MRR_Mapping[14], MRR_BIT_MUX4_MRR_BIT14_SEL) | P_Fld(uiLPDDR_MRR_Mapping[15], MRR_BIT_MUX4_MRR_BIT15_SEL));
#endif

}

void Global_Option_Init(DRAMC_CTX_T *p)
{
    SaveCurrDramCtx(p);
    vSetChannelNumber(p);
    vSetRankNumber(p);
    Set_MRR_Pinmux_Mapping(p);
}

void RISCWriteDRAM(unsigned int UI_offset_address, unsigned int UI_content_value)
{
    *((volatile unsigned int *)(Channel_A_DRAMC_AO_BASE_ADDRESS + UI_offset_address)) = UI_content_value;
    *((volatile unsigned int *)(Channel_A_DRAMC_NAO_BASE_ADDRESS + UI_offset_address)) = UI_content_value;
} // end of RISCWriteDRAM

void RISCWriteDDRPHY(unsigned int UI_offset_address, unsigned int UI_content_value)
{
    *((volatile unsigned int *)(Channel_A_PHY_BASE_VIRTUAL + UI_offset_address)) = UI_content_value;
} // end of RISCWriteDDRPHY

void RISCWrite(unsigned int UI_offset_address, unsigned int UI_content_value)
{
    *((volatile unsigned int *)(UI_offset_address)) = UI_content_value;
} // end of RISCWrite

unsigned int RISCRead(unsigned int UI_offset_address)
{
    return *((volatile unsigned int *)(UI_offset_address));
} // end of RISCWrite

#if 0//__ETT__
void RISCReadAll(void)
{
    unsigned int address = DRAMC0_BASE;
    unsigned int value = 0;

    /*TINFO=">>>>>DRAMC0<<<<<\n"*/
    mcSHOW_DBG_MSG((">>>>>DRAMC0<<<<<\n"));
    for(address=DRAMC0_BASE; address<=DRAMC0_BASE+0xD54; address+=4)
    {
        value = *((volatile unsigned int *)(address));
        #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("Addr. 0x%X = 0x%X\n", address, value));
        #else
        mcSHOW_DBG_MSG(("Addr. 0x%8x = 0x%8x\n", address, value));
        #endif
        /*TINFO="[REG_ACCESS_PORTING_DBG] Register_Dump Reg(%h) = %h\n", address, value*/
    }

    /*TINFO=">>>>>DRAMC1<<<<<\n"*/
    mcSHOW_DBG_MSG((">>>>>DRAMC1<<<<<\n"));
    for(address=DRAMC1_BASE; address<=DRAMC1_BASE+0xD54; address+=4)
    {
        value = *((volatile unsigned int *)(address));
        #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("Addr. 0x%X = 0x%X\n", address, value));
        #else
        mcSHOW_DBG_MSG(("Addr. 0x%8x = 0x%8x\n", address, value));
        #endif
        /*TINFO="[REG_ACCESS_PORTING_DBG] Register_Dump Reg(%h) = %h\n", address, value*/
    }

    /*TINFO=">>>>>DRAMC0_NAO<<<<<\n"*/
    mcSHOW_DBG_MSG((">>>>>DRAMC0_NAO<<<<<\n"));
    for(address=DRAMC0_NAO_BASE; address<=DRAMC0_NAO_BASE+0x59C; address+=4)
    {
        value = *((volatile unsigned int *)(address));
        #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("Addr. 0x%X = 0x%X\n", address, value));
        #else
        mcSHOW_DBG_MSG(("Addr. 0x%8x = 0x%8x\n", address, value));
        #endif
        /*TINFO="[REG_ACCESS_PORTING_DBG] Register_Dump Reg(%h) = %h\n", address, value*/
    }

    /*TINFO=">>>>>DRAMC1_NAO<<<<<\n"*/
    mcSHOW_DBG_MSG((">>>>>DRAMC1_NAO<<<<<\n"));
    for(address=DRAMC1_NAO_BASE; address<=DRAMC1_NAO_BASE+0x59C; address+=4)
    {
        value = *((volatile unsigned int *)(address));
        #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("Addr. 0x%X = 0x%X\n", address, value));
        #else
        mcSHOW_DBG_MSG(("Addr. 0x%8x = 0x%8x\n", address, value));
        #endif
        /*TINFO="[REG_ACCESS_PORTING_DBG] Register_Dump Reg(%h) = %h\n", address, value*/
    }

    /*TINFO=">>>>>DDRPHY_A<<<<<\n"*/
    mcSHOW_DBG_MSG((">>>>>DDRPHY_A<<<<<\n"));
    for(address=DDRPHY_BASE; address<=DDRPHY_BASE+0xFD4; address+=4)
    {
        value = *((volatile unsigned int *)(address));
        #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("Addr. 0x%X = 0x%X\n", address, value));
        #else
        mcSHOW_DBG_MSG(("Addr. 0x%8x = 0x%8x\n", address, value));
        #endif
        /*TINFO="[REG_ACCESS_PORTING_DBG] Register_Dump Reg(%h) = %h\n", address, value*/
    }

    /*TINFO=">>>>>DDRPHY_B<<<<<\n"*/
    mcSHOW_DBG_MSG((">>>>>DDRPHY_B<<<<<\n"));
    for(address=DDRPHY1_BASE; address<=DDRPHY1_BASE+0xFD4; address+=4)
    {
        value = *((volatile unsigned int *)(address));
        #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("Addr. 0x%X = 0x%X\n", address, value));
        #else
        mcSHOW_DBG_MSG(("Addr. 0x%8x = 0x%8x\n", address, value));
        #endif
        /*TINFO="[REG_ACCESS_PORTING_DBG] Register_Dump Reg(%h) = %h\n", address, value*/
    }

    /*TINFO=">>>>>DDRPHY_C<<<<<\n"*/
    mcSHOW_DBG_MSG((">>>>>DDRPHY_C<<<<<\n"));
    for(address=DDRPHY2_BASE; address<=DDRPHY2_BASE+0xFD4; address+=4)
    {
        value = *((volatile unsigned int *)(address));
        #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("Addr. 0x%X = 0x%X\n", address, value));
        #else
        mcSHOW_DBG_MSG(("Addr. 0x%8x = 0x%8x\n", address, value));
        #endif
        /*TINFO="[REG_ACCESS_PORTING_DBG] Register_Dump Reg(%h) = %h\n", address, value*/
    }
    /*TINFO=">>>>>DDRPHY_D<<<<<\n"*/
    mcSHOW_DBG_MSG((">>>>>DDRPHY_D<<<<<\n"));
    for(address=DDRPHY3_BASE; address<=DDRPHY3_BASE+0xFD4; address+=4)
    {
        value = *((volatile unsigned int *)(address));
        #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("Addr. 0x%X = 0x%X\n", address, value));
        #else
        mcSHOW_DBG_MSG(("Addr. 0x%8x = 0x%8x\n", address, value));
        #endif
        /*TINFO="[REG_ACCESS_PORTING_DBG] Register_Dump Reg(%h) = %h\n", address, value*/
    }
}
#endif


void DDRPhyPLLSetting(DRAMC_CTX_T *p)
{
    U8 u1CAP_SEL;
    U8 u1MIDPICAP_SEL;
    U8 u1VTH_SEL;

#if (fcFOR_CHIP_ID == fcWhitney)
    u1VTH_SEL = 0x0;
#endif

#if (fcFOR_CHIP_ID == fcWhitney)
    if(p->frequency<=400)//DDR800
    {
        u1CAP_SEL = 0x2;
    }
    else if(p->frequency<=800)//DDR1600
    {
        u1CAP_SEL = 0x1;
    }
    else//DDR2667,3200,3733
    {
        u1CAP_SEL = 0x0;
    }
#endif

#if (fcFOR_CHIP_ID == fcWhitney)
    if(p->frequency<=1333)
    {
        u1MIDPICAP_SEL = 0x2;
    }
    else if(p->frequency<=1600)
    {
        u1MIDPICAP_SEL = 0x0;
    }
    else//3733
    {
        u1MIDPICAP_SEL = 0x0;
    }
#endif

    //26M
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_CLK_MEM_SEL);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_W_CHG_MEM);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_W_CHG_MEM);
    #ifdef USE_CLK26M
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_RG_DA_RREF_CK_SEL);
    #endif

    //MIDPI
    vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ6, P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0)
                | P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0));
    vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ6, P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1)
                | P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1));
    vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD6, P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA)
                | P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA));

    vIO32WriteFldMulti_All(DDRPHY_PLL4, P_Fld(0x0, PLL4_RG_RPHYPLL_ADA_MCK8X_EN)
                | P_Fld(0x0, PLL4_RG_RPHYPLL_RESETB));

    //PLL
    vIO32WriteFldAlign_All(DDRPHY_PLL1, 0x0, PLL1_RG_RPHYPLL_EN);
    vIO32WriteFldAlign_All(DDRPHY_PLL2, 0x0, PLL2_RG_RCLRPLL_EN);

    //DLL
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI5, 0x0, CA_DLL_ARPI5_RG_ARDLL_PHDET_EN_CA);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI5, 0x0, B0_DLL_ARPI5_RG_ARDLL_PHDET_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI5, 0x0, B1_DLL_ARPI5_RG_ARDLL_PHDET_EN_B1);

    vIO32WriteFldMulti_All(DDRPHY_B0_DLL_ARPI2, P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
    vIO32WriteFldMulti_All(DDRPHY_B1_DLL_ARPI2, P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
    vIO32WriteFldMulti_All(DDRPHY_CA_DLL_ARPI2, P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARRPI_CG_FB_CA)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_CLK)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_CMD)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));

    //RESETB
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 0x0, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 0x0, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 0x0, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);

    mcDELAY_US(1);

    ///TODO: PLL/MIDPI Settings
    //Ref clock should be 20M~30M, if MPLL=52M, Pre-divider should be set to 1
    #ifdef USE_CLK26M
    vIO32WriteFldMulti_All(DDRPHY_SHU1_PLL8, P_Fld(0x0, SHU1_PLL8_RG_RPHYPLL_POSDIV) | P_Fld(0x0, SHU1_PLL8_RG_RPHYPLL_PREDIV));
    vIO32WriteFldMulti_All(DDRPHY_SHU1_PLL10, P_Fld(0x0, SHU1_PLL10_RG_RCLRPLL_POSDIV) | P_Fld(0x0, SHU1_PLL10_RG_RCLRPLL_PREDIV));
    #else //MPLL 52M
    vIO32WriteFldMulti_All(DDRPHY_SHU1_PLL8, P_Fld(0x0, SHU1_PLL8_RG_RPHYPLL_POSDIV) | P_Fld(0x1, SHU1_PLL8_RG_RPHYPLL_PREDIV));
    vIO32WriteFldMulti_All(DDRPHY_SHU1_PLL10, P_Fld(0x0, SHU1_PLL10_RG_RCLRPLL_POSDIV) | P_Fld(0x1, SHU1_PLL10_RG_RCLRPLL_PREDIV));
    #endif
    if(p->frequency==1866)
    {   // 2722/26=143(0x8f)
        vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL5, 0x8f00, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
        vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL7, 0x8f00, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);    
    }
    else if(p->frequency==1600 || p->frequency==800 || p->frequency==400)
    {
        vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL5, 0x7b00, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
        vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL7, 0x7b00, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
        if(p->frequency==400)
        {
            vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL8, 0x1, SHU1_PLL8_RG_RPHYPLL_POSDIV);
            vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL10, 0x1, SHU1_PLL10_RG_RCLRPLL_POSDIV);
        }
    }
    else if(p->frequency==1333 || p->frequency==667)
    {
        vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL5, 0x6600, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
        vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL7, 0x6600, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
    }
    else if(p->frequency==1200)
    {
    }
    //LP3
    else if(p->frequency==933)
    {
        vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL5, 0x8f00, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
        vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL7, 0x8f00, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
    }
    else if(p->frequency==467)
    {
        vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL5, 0x4700, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
        vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL7, 0x4700, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
    }
    vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ6, P_Fld(u1VTH_SEL, SHU1_B0_DQ6_RG_ARPI_MIDPI_VTH_SEL_B0)
                | P_Fld(u1CAP_SEL, SHU1_B0_DQ6_RG_ARPI_CAP_SEL_B0)
                | P_Fld(u1MIDPICAP_SEL, SHU1_B0_DQ6_RG_ARPI_MIDPI_CAP_SEL_B0));
    vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ6, P_Fld(u1VTH_SEL, SHU1_B1_DQ6_RG_ARPI_MIDPI_VTH_SEL_B1)
                | P_Fld(u1CAP_SEL, SHU1_B1_DQ6_RG_ARPI_CAP_SEL_B1)
                | P_Fld(u1MIDPICAP_SEL, SHU1_B1_DQ6_RG_ARPI_MIDPI_CAP_SEL_B1));
    vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD6, P_Fld(u1VTH_SEL, SHU1_CA_CMD6_RG_ARPI_MIDPI_VTH_SEL_CA)
                | P_Fld(u1CAP_SEL, SHU1_CA_CMD6_RG_ARPI_CAP_SEL_CA)
                | P_Fld(u1MIDPICAP_SEL, SHU1_CA_CMD6_RG_ARPI_MIDPI_CAP_SEL_CA));


    ///TODO: RESETB
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 0x1, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 0x1, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 0x1, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
    mcDELAY_US(1);

    ///TODO: PLL EN
    vIO32WriteFldAlign_All(DDRPHY_PLL1, 0x1, PLL1_RG_RPHYPLL_EN);
    vIO32WriteFldAlign_All(DDRPHY_PLL2, 0x1, PLL2_RG_RCLRPLL_EN);
    mcDELAY_US(100);

    ///TODO: MIDPI Init 1
    vIO32WriteFldMulti_All(DDRPHY_PLL4, P_Fld(0x1, PLL4_RG_RPHYPLL_ADA_MCK8X_EN)
                | P_Fld(0x1, PLL4_RG_RPHYPLL_RESETB));

    mcDELAY_US(1);


    ///TODO: MIDPI Init 2
    if(p->frequency>=1200)//DDR-2400, DDR-2667, DDR-3200, DDR3667
    {
        vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ6, P_Fld(0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0)
                    | P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0));
        vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ6, P_Fld(0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1)
                    | P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1));
        vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD6, P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA)
                    | P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA));
    }
    else
    {
        vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ6, P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0)
                    | P_Fld(0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0));
        vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ6, P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1)
                    | P_Fld(0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1));
        vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD6, P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA)
                    | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA));
    }
    mcDELAY_US(1);

    vIO32WriteFldMulti_All(DDRPHY_CA_DLL_ARPI2, P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA)
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA)
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARRPI_CG_FB_CA)
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CLK)
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CMD)
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));
    vIO32WriteFldMulti_All(DDRPHY_B0_DLL_ARPI2, P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
    vIO32WriteFldMulti_All(DDRPHY_B1_DLL_ARPI2, P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
    mcDELAY_US(2);

    //DLL
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI5, 0x1, CA_DLL_ARPI5_RG_ARDLL_PHDET_EN_CA);
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI5, 0x1, B0_DLL_ARPI5_RG_ARDLL_PHDET_EN_B0);
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI5, 0x1, B1_DLL_ARPI5_RG_ARDLL_PHDET_EN_B1);
    mcDELAY_US(1);

    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_CLK_MEM_SEL);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_W_CHG_MEM);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_W_CHG_MEM);

    DDRPhyFreqMeter();
}

#if 1
void DramcPowerOnSequence(DRAMC_CTX_T *p)
{
    #if APPLY_LP4_POWER_INIT_SEQUENCE
    //static U8 u1PowerOn=0;
    //if(u1PowerOn ==0)
    {
    //reset dram = low
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DDRCONF0), 0, DDRCONF0_GDDR3RST);

    //vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0, RKCFG_CKE2RANK_OPT2);

    //CKE low
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL),
                                            P_Fld(0, CKECTRL_CKEFIXON) | P_Fld(0, CKECTRL_CKE1FIXON)|
                                            P_Fld(1, CKECTRL_CKEFIXOFF) | P_Fld(1, CKECTRL_CKE1FIXOFF));
    // delay tINIT1=200us(min) & tINIT2=10ns(min)
    mcDELAY_US(200);

    //reset dram = low
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DDRCONF0), 1, DDRCONF0_GDDR3RST);

    // Disable HW MIOCK control to make CLK always on
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 1, DRAMC_PD_CTRL_MIOCKCTRLOFF);

    //tINIT3=2ms(min)
    mcDELAY_MS(2);

    //CKE high
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL),
                                            P_Fld(1, CKECTRL_CKEFIXON) | P_Fld(1, CKECTRL_CKE1FIXON)|
                                            P_Fld(0, CKECTRL_CKEFIXOFF) | P_Fld(0, CKECTRL_CKE1FIXOFF));
    // tINIT5=2us(min)
    mcDELAY_US(2);
    //u1PowerOn=1;
    mcSHOW_DBG_MSG(("APPLY_LP4_POWER_INIT_SEQUENCE\n"));
    }
    #endif
}


DRAM_STATUS_T DramcModeRegInit_LP4(DRAMC_CTX_T *p, U8 init_type)
{
    U32 u4RankIdx;//, u4CKE0Bak, u4CKE1Bak, u4MIOCKBak, u4AutoRefreshBak;
    U16 u2MR3Value;
    U8 u1MRFsp= FSP_0;
    U8 u1ChannelIdx, support_channel_num;
    U8 backup_channel;
    U8 operating_fsp = p->dram_fsp;

#if SET_CKE_2_RANK_INDEPENDENT
    // set CKE1 is the same as CKE0
    vIO32WriteFldMulti_All(DRAMC_REG_RKCFG, P_Fld(1, RKCFG_CKE2RANK)
                    | P_Fld(0, RKCFG_CKE2RANK_OPT)
                    | P_Fld(0, RKCFG_CKE2RANK_OPT2));
#endif

    //Francis : pin mux issue, need to set CHD
    DramcBroadcastOnOff(BROADCAST_4CHANNELS);
    DramcPowerOnSequence(p);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    u1PrintModeRegWrite=1;
    backup_channel = p->channel;

    if (init_type)
    {
        support_channel_num = p->support_channel_num;
    }
    else
    {
        support_channel_num = 1;
    }

#if VENDER_JV_LOG
    vPrintCalibrationBasicInfo_ForJV(p);
#endif

    for(u1ChannelIdx=0; u1ChannelIdx<(support_channel_num); u1ChannelIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChannelIdx);

        for(u4RankIdx =0; u4RankIdx < (U32)(p->support_rank_num); u4RankIdx++)
        {
            mcSHOW_DBG_MSG(("DramcModeRegInit_LP4 for Rank%d\n", u4RankIdx));
            mcFPRINTF((fp_A60501, "DramcModeRegInit_LP4 for Rank%d\n", u4RankIdx));
            #if VENDER_JV_LOG
            mcSHOW_DBG_MSG5(("\nDramcModeRegInit_LP4 for Rank%d\n", u4RankIdx));
            #endif

            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u4RankIdx, MRS_MRSRK);

            // Note : MR37 for LP4P should be set before any Mode register.
            // MR37 is not shadow register, just need to set by channel and rank. No need to set by FSP
            if(p->dram_type == TYPE_LPDDR4P)
            {
                //temp solution, need remove later
                #ifndef MT6739_FPGA
                #if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
                rt5738_set_voltage(600000);
                #endif
                #endif

                DramcModeRegWrite(p, 37, 0x1);

                //temp solution, need remove later
                #ifndef MT6739_FPGA
                #if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
                rt5738_set_voltage(400000);
                #endif
                #endif
            }

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
           if (operating_fsp == FSP_1)
           {
               //switch to low freq
               CBT_Switch_Freq(p, CBT_LOW_FREQ);
           }
#endif


           // if(p->frequency<=1200)
            {
                u1MRFsp = FSP_0;
                mcSHOW_DBG_MSG(("--Fsp%d --\n", u1MRFsp));
                mcFPRINTF((fp_A60501, "--Fsp%d --\n", u1MRFsp));
                #if VENDER_JV_LOG
                mcSHOW_DBG_MSG5(("--Fsp%d --\n", u1MRFsp));
                #endif

                u1MR13Value[u1MRFsp] = 0x0 | (MR13_RRO<<4) | (1<<3);
                DramcModeRegWrite(p, 0xd, u1MR13Value[u1MRFsp]); //FSP-0
                //MR12 use previous value
                DramcModeRegWrite(p, 0xc, u1MR12Value[u1ChannelIdx][u4RankIdx][u1MRFsp]); //MR12 VREF-CA
                u1MR01Value[u1MRFsp] = 0x56;
                DramcModeRegWrite(p, 0x1, u1MR01Value[u1MRFsp]);

                //if(p->frequency==1200)
                //{
                //}
                //else if(p->frequency==800)
                {
                    u1MR2Value[u1MRFsp] = 0x12;
                }
                DramcModeRegWrite(p, 0x2, u1MR2Value[u1MRFsp]);
                //if(p->odt_onoff)
                    DramcModeRegWrite(p, 0xb, 0x0);     //ODT

                if(p->dram_type == TYPE_LPDDR4)
                {
                    DramcModeRegWrite(p, 0x16, 0x20);    //SOC-ODT, ODTE-CK, ODTE-CS, Disable ODTD-CA
                }
                else  //TYPE_LPDDR4x, LP4P
                {
                    DramcModeRegWrite(p, 0x16, 0x38);    //SOC-ODT, ODTE-CK, ODTE-CS, Disable ODTD-CA
                }

                //MR14 use previous value
                DramcModeRegWrite(p, 0xe, u1MR14Value[u1ChannelIdx][u4RankIdx][u1MRFsp]);    //MR14 VREF-DQ      
                
                u2MR3Value = 0x31;
                if(p->dram_fsp==FSP_0)
                {
                    u2MR3Value |= ((p->DBI_W_onoff)<<7 | (p->DBI_R_onoff)<<6);
                }
                if(p->dram_type == TYPE_LPDDR4X || p->dram_type == TYPE_LPDDR4P)
                        u2MR3Value &= 0xff;  // MR3[0] =0, PU-Cal= VEED/2.5

                DramcModeRegWrite(p, 0x3, u2MR3Value);//Enable DBI
            }
            //else
            {
                u1MRFsp = FSP_1;
                mcSHOW_DBG_MSG(("--Fsp%d --\n", u1MRFsp));
                mcFPRINTF((fp_A60501, "--Fsp%d --\n", u1MRFsp));
                #if VENDER_JV_LOG
                mcSHOW_DBG_MSG5(("--Fsp%d --\n", u1MRFsp));
                #endif

                u1MR13Value[u1MRFsp] = 0x40 | (MR13_RRO<<4) | (1<<3);
                DramcModeRegWrite(p, 0xd, u1MR13Value[u1MRFsp]); //FSP-1
                //MR12 use previous value
                DramcModeRegWrite(p, 0xc, u1MR12Value[u1ChannelIdx][u4RankIdx][u1MRFsp]); //MR12 VREF-CA
                DramcModeRegWrite(p, 0x1, 0x56);
                if(p->frequency==1866)
                {
                    u1MR2Value[u1MRFsp] = 0x36;
                }                
                if(p->frequency==1600)
                {
                    u1MR2Value[u1MRFsp] = 0x2d;
                }
                else if(p->frequency==1333)
                {
                    u1MR2Value[u1MRFsp] = 0x24;
                }
                DramcModeRegWrite(p, 0x2, u1MR2Value[u1MRFsp]);

                if(p->dram_type == TYPE_LPDDR4P || p->odt_onoff==ODT_OFF)  //(p->odt_onoff)
                    DramcModeRegWrite(p, 0xb, 0x0);     //ODT
                else
                    DramcModeRegWrite(p, 0xb, 0x04);     //ODT


                if(p->odt_onoff==ODT_OFF)
                {
                    if(p->dram_type == TYPE_LPDDR4)
                    {
                        DramcModeRegWrite(p, 0x16, 0x20);    //SOC-ODT, ODTE-CK, ODTE-CS, Disable ODTD-CA
                    }
                    else  //TYPE_LPDDR4x, LP4P
                    {
                        DramcModeRegWrite(p, 0x16, 0x38);    //SOC-ODT, ODTE-CK, ODTE-CS, Disable ODTD-CA
                    }
                }
                else
                {
                    if(p->dram_type == TYPE_LPDDR4)
                    {
                        DramcModeRegWrite(p, 0x16, 0x24);    //SOC-ODT, ODTE-CK, ODTE-CS, Disable ODTD-CA
                    }
                    else  //TYPE_LPDDR4x, LP4P
                    {
                        DramcModeRegWrite(p, 0x16, 0x3c);    //SOC-ODT, ODTE-CK, ODTE-CS, Disable ODTD-CA
                    }
                }

                //MR14 use previous value
                if(p->dram_type == TYPE_LPDDR4P || p->odt_onoff==ODT_OFF)  //(p->odt_onoff)
                    DramcModeRegWrite(p, 0xe, u1MR14Value[u1ChannelIdx][u4RankIdx][0]);    //MR14 VREF-DQ
                else
                    DramcModeRegWrite(p, 0xe, u1MR14Value[u1ChannelIdx][u4RankIdx][u1MRFsp]);    //MR14 VREF-DQ

                u2MR3Value = 0xf1;
                if(p->dram_fsp==FSP_1)
                {
                    //u2MR3Value |= ((p->DBI_W_onoff)<<7 | (p->DBI_R_onoff)<<6);
                    u2MR3Value = (u2MR3Value & (0x3f)) | ((p->DBI_W_onoff)<<7 | (p->DBI_R_onoff)<<6);
                    
                }
                if(p->dram_type == TYPE_LPDDR4X || p->dram_type == TYPE_LPDDR4P)
                        u2MR3Value &= 0xff;  // MR3[0] =0, PU-Cal= VEED/2.5

                DramcModeRegWrite(p, 0x3, u2MR3Value);//Enable DBI
            }

            if(p->frequency<=1200)
            {
                u1MRFsp = FSP_0;
                u1MR13Value[u1MRFsp] = 0x0 | (MR13_RRO<<4) | (1<<3);
                DramcModeRegWrite(p, 0xd, u1MR13Value[u1MRFsp]); //FSP-0
            }
            else
            {
                u1MRFsp = FSP_1;
                u1MR13Value[u1MRFsp] = 0xc0 | (MR13_RRO<<4) | (1<<3);
                DramcModeRegWrite(p, 0xd, u1MR13Value[u1MRFsp]); //FSP-1
            }

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
            if (operating_fsp == FSP_1)
            {
                //switch to low freq
                CBT_Switch_Freq(p, CBT_HIGH_FREQ);
            }
#endif

        }

        //u1MRFsp = FSP_0; // for fix warning, Uninitialized array index read
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR2), P_Fld(u1MR2Value[u1MRFsp], SHU_HWSET_MR2_HWSET_MR2_OP)
                                                                    | P_Fld(2, SHU_HWSET_MR2_HWSET_MR2_MRSMA));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR13), P_Fld(u1MR13Value[u1MRFsp]|0x1<<3, SHU_HWSET_MR13_HWSET_MR13_OP)
                                                                    | P_Fld(13, SHU_HWSET_MR13_HWSET_MR13_MRSMA));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG), P_Fld(u1MR13Value[u1MRFsp]&0xf7, SHU_HWSET_VRCG_HWSET_VRCG_OP)
                                                                    | P_Fld(13, SHU_HWSET_VRCG_HWSET_VRCG_MRSMA));
    }

    vSetPHY2ChannelMapping(p, backup_channel);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), RANK_0, MRS_MRSRK);

    u1PrintModeRegWrite=0;
    return DRAM_OK;
}
#else

DRAM_STATUS_T DramcModeRegInit_LP4(DRAMC_CTX_T *p, U8 init_type)
{
    U32 u4RankIdx;//, u4CKE0Bak, u4CKE1Bak, u4MIOCKBak, u4AutoRefreshBak;
    U16 u2MR3Value;

    vPrintCalibrationBasicInfo(p);

    #if APPLY_LP4_POWER_INIT_SEQUENCE
    static U8 u1PowerOn=0;
    if(u1PowerOn ==0)
    {
        //reset dram = low
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DDRCONF0), 0, DDRCONF0_GDDR3RST);

        //CKE low
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL),
                                                P_Fld(0, CKECTRL_CKEFIXON) | P_Fld(0, CKECTRL_CKE1FIXON)|
                                                P_Fld(1, CKECTRL_CKEFIXOFF) | P_Fld(1, CKECTRL_CKE1FIXOFF));
        // delay tINIT1=200us(min) & tINIT2=10ns(min)
        mcDELAY_US(200);

        //reset dram = low
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DDRCONF0), 1, DDRCONF0_GDDR3RST);

        // Disable HW MIOCK control to make CLK always on
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 1, DRAMC_PD_CTRL_MIOCKCTRLOFF);

        //tINIT3=2ms(min)
        mcDELAY_MS(2);

        //CKE high
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL),
                                                P_Fld(1, CKECTRL_CKEFIXON) | P_Fld(1, CKECTRL_CKE1FIXON)|
                                                P_Fld(0, CKECTRL_CKEFIXOFF) | P_Fld(0, CKECTRL_CKE1FIXOFF));
        // tINIT5=2us(min)
        mcDELAY_US(2);
        u1PowerOn=1;
        mcSHOW_DBG_MSG(("APPLY_LP4_POWER_INIT_SEQUENCE\n"));
    }
    #endif

    for(u4RankIdx =0; u4RankIdx < (U32)(p->support_rank_num); u4RankIdx++)
    {
        mcSHOW_DBG_MSG(("DramcModeRegInit_LP4 for Rank%d, dram_fsp %d\n", u4RankIdx, p->dram_fsp));
        mcFPRINTF((fp_A60501, "DramcModeRegInit_LP4 for Rank%d dram_fsp %d\n", u4RankIdx, p->dram_fsp));

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u4RankIdx, MRS_MRSRK);

        // Note : MR37 for LP4P should be set before any Mode register.
        // MR37 is not shadow register, just need to set by channel and rank. No need to set by FSP
        if(p->dram_type == TYPE_LPDDR4P)
        {
            //temp solution, need remove later
            #ifndef MT6739_FPGA
            fan53528buc08x_set_voltage((unsigned long)600000);
            #endif

            DramcModeRegWrite(p, 37, 0x1);

            //temp solution, need remove later
            #ifndef MT6739_FPGA
            fan53528buc08x_set_voltage((unsigned long)400000);
            #endif
        }

        //if(p->frequency<=1200)
        if(p->odt_onoff == ODT_OFF)
        {
            u1MR13Value[p->dram_fsp] = 0 | (MR13_RRO<<4) | (1<<3);
            DramcModeRegWrite(p, 0xd, u1MR13Value[p->dram_fsp]); //FSP-0
//            u1MR12Value[p->channel][p->dram_fsp] = 0x4d;
            DramcModeRegWrite(p, 0xc, u1MR12Value[p->channel][p->dram_fsp]); //MR12 VREF-CA
            u1MR01Value[p->dram_fsp] = 0x56;
            DramcModeRegWrite(p, 0x1, u1MR01Value[p->dram_fsp]);

            if(p->frequency==1600)
            {
                u1MR2Value[p->dram_fsp] = 0x2d;
            }
            else if(p->frequency==1333)
            {
                u1MR2Value[p->dram_fsp] = 0x24;
            }
            else if(p->frequency==1200)
            {
            }
            else if(p->frequency==800)
            {
                u1MR2Value[p->dram_fsp] = 0x12;
            }
            DramcModeRegWrite(p, 0x2, u1MR2Value[p->dram_fsp]);
            DramcModeRegWrite(p, 0xb, 0x0);     //ODT
            if(p->dram_type == TYPE_LPDDR4)
            {
                DramcModeRegWrite(p, 0x16, 0x20);    //SOC-ODT, ODTE-CK, ODTE-CS, disable ODTD-CA
            }
            else  //TYPE_LPDDR4x & TYPE_LPDDR4P
            {
                DramcModeRegWrite(p, 0x16, 0x38);    //SOC-ODT, ODTE-CK, ODTE-CS, disable ODTD-CA
            }
  //          u1MR14Value[p->channel][p->dram_fsp] = 0x4d;
            DramcModeRegWrite(p, 0xe, u1MR14Value[p->channel][p->dram_fsp]);    //MR14 VREF-DQ
        }
        else
        {
            #if 0//CA-Term
            u1MR13Value[p->dram_fsp] = 0xc0 | (MR13_RRO<<4);
            DramcModeRegWrite(p, 0xd, u1MR13Value[p->dram_fsp]); //FSP-1
            u1MR12Value[p->dram_fsp] = 0x10;
            DramcModeRegWrite(p, 0xc, u1MR12Value[p->dram_fsp]); //MR12 VREF-CA
            u1MR01Value[p->dram_fsp] = 0x56;
            DramcModeRegWrite(p, 0x1, u1MR01Value[p->dram_fsp]);

            if(p->frequency==1600)
            {
                u1MR2Value[p->dram_fsp] = 0x2d;
            }
            else if(p->frequency==1333)
            {
                u1MR2Value[p->dram_fsp] = 0x24;
            }
            DramcModeRegWrite(p, 0x2, u1MR2Value[p->dram_fsp]);
            DramcModeRegWrite(p, 0xb, 0x44);     //ODT
            DramcModeRegWrite(p, 0x16, 0xC4);    //SOC-ODT, ODTE-CK, ODTE-CS, ODTD-CA
            u1MR14Value[p->dram_fsp] = 0x10;
            DramcModeRegWrite(p, 0xe, u1MR14Value);    //MR14 VREF-DQ
            #else
            u1MR13Value[p->dram_fsp] = 0xc0 | (MR13_RRO<<4) | (1<<3);
            DramcModeRegWrite(p, 0xd, u1MR13Value[p->dram_fsp]); //FSP-1
//            u1MR12Value[p->channel][p->dram_fsp] = 0x4d;
            DramcModeRegWrite(p, 0xc, u1MR12Value[p->channel][p->dram_fsp]); //MR12 VREF-CA
            DramcModeRegWrite(p, 0x1, 0x56);
            if(p->frequency==1600)
            {
                u1MR2Value[p->dram_fsp] = 0x2d;
            }
            else if(p->frequency==1333)
            {
                u1MR2Value[p->dram_fsp] = 0x24;
            }
            DramcModeRegWrite(p, 0x2, u1MR2Value[p->dram_fsp]);
            DramcModeRegWrite(p, 0xb, 0x04);     //ODT
            if(p->dram_type == TYPE_LPDDR4)
            {
                DramcModeRegWrite(p, 0x16, 0x24);    //SOC-ODT, ODTE-CK, ODTE-CS, disable ODTD-CA
            }
            else  //TYPE_LPDDR4x
            {
                DramcModeRegWrite(p, 0x16, 0x3c);    //SOC-ODT, ODTE-CK, ODTE-CS, disable ODTD-CA
            }

//            u1MR14Value[p->channel][p->dram_fsp] = 0x10;
            DramcModeRegWrite(p, 0xe, u1MR14Value[p->channel][p->dram_fsp]);    //MR14 VREF-DQ
            #endif
        }

        u2MR3Value = 0x31;
        u2MR3Value |= ((p->DBI_W_onoff)<<7 | (p->DBI_R_onoff)<<6);
        if(p->dram_type == TYPE_LPDDR4X || p->dram_type == TYPE_LPDDR4P)
                u2MR3Value &= 0xfe;  // MR3[0] =0, PU-Cal= VEED/2.5

        DramcModeRegWrite(p, 0x3, u2MR3Value);//Enable DBI
    }


    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), RANK_0, MRS_MRSRK);

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR2), P_Fld(u1MR2Value[p->dram_fsp], SHU_HWSET_MR2_HWSET_MR2_OP)
                                                                | P_Fld(2, SHU_HWSET_MR2_HWSET_MR2_MRSMA));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR13), P_Fld(u1MR13Value[p->dram_fsp]|(1<<3), SHU_HWSET_MR13_HWSET_MR13_OP)
                                                                | P_Fld(13, SHU_HWSET_MR13_HWSET_MR13_MRSMA));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG), P_Fld(u1MR13Value[p->dram_fsp]&0xf7, SHU_HWSET_VRCG_HWSET_VRCG_OP)
                                                                | P_Fld(13, SHU_HWSET_VRCG_HWSET_VRCG_MRSMA));

    return DRAM_OK;
}

#endif

#if (__ETT__ || ENABLE_LP3_SW) //remove unused function to save space of log string
DRAM_STATUS_T DramcModeRegInit_LP3(DRAMC_CTX_T *p)
{
    U32 u4RankIdx, u4AutoRefreshBak;

    #if 0
    U32 u4CKE0Bak, u4CKE1Bak, u4MIOCKBak;
    u4MIOCKBak =u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), DRAMC_PD_CTRL_MIOCKCTRLOFF);
    u4CKE0Bak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), CKECTRL_CKEFIXON);
    u4CKE1Bak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), CKECTRL_CKE1FIXON);
    #endif
    u4AutoRefreshBak= u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), REFCTRL0_REFDIS);

    // Disable HW MIOCK control to make CLK always on
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 1, DRAMC_PD_CTRL_MIOCKCTRLOFF);
    mcDELAY_US(1);

    //if CKE2RANK=1, only need to set CKEFIXON, it will apply to both rank.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), 1, CKECTRL_CKEFIXON);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), 1, CKECTRL_CKE1FIXON);

    // disable auto refresh command during Dram reset (MR63)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0) ,1, REFCTRL0_REFDIS);

    //wait cke to reset timing constraint 'tinit3'
    //for (i=0; i < 10 ; ++i);
    mcDELAY_US(200);

    for(u4RankIdx =0; u4RankIdx < (U32)(p->support_rank_num); u4RankIdx++)
    {
        mcSHOW_DBG_MSG2(("DramcModeRegInit_LP3 for Rank%d\n", u4RankIdx));
        mcFPRINTF((fp_A60501, "DramcModeRegInit_LP3 for Rank%d\n", u4RankIdx));

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u4RankIdx, MRS_MRSRK);

        DramcModeRegWrite(p, 0x3f, 0);       //MR63=0   -> Reset
        mcDELAY_US(10);  //wait 10us for dram reset

        DramcModeRegWrite(p, 0xa, 0xff);   //MR10=0xff  -> ZQ Init

        DramcModeRegWrite(p, 1, 0x83);      //MR1=0x83 nWR: OP[7:5] (14 -> LP3-1866),   A60817 is 0x43  nWR: OP[7:5] (12 -> LP3-1600)

        // MR2
        /*
        if(p->frequency <= 533)
        {
            u1MR2Value[p->dram_fsp] = 0x16;
        }
        else
        */
        if(p->frequency <= 667)
        {
            u1MR2Value[p->dram_fsp] = 0x18;
        }
        else if(p->frequency <= 800)
        {
            u1MR2Value[p->dram_fsp] = 0x1a;
        }
        else
        {
            u1MR2Value[p->dram_fsp] = 0x1c;
        }
        DramcModeRegWrite(p, 2, u1MR2Value[p->dram_fsp]);

        DramcModeRegWrite(p, 0xb, 0x3);      //MR11=0x3 , ODT disable.
    }

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), 0, MRS_MRSRK);

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR2), P_Fld(0, SHU_HWSET_MR2_HWSET_MR2_OP)
                                                                | P_Fld(0, SHU_HWSET_MR2_HWSET_MR2_MRSMA));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR13), P_Fld(u1MR2Value[p->dram_fsp], SHU_HWSET_MR13_HWSET_MR13_OP)
                                                                | P_Fld(2, SHU_HWSET_MR13_HWSET_MR13_MRSMA));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG), P_Fld(0, SHU_HWSET_VRCG_HWSET_VRCG_OP)
                                                                | P_Fld(0, SHU_HWSET_VRCG_HWSET_VRCG_MRSMA));

    // Restore auto refresh command after Dram reset (MR63)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0) ,u4AutoRefreshBak, REFCTRL0_REFDIS);

#if 0//Set to dynamic in ApplyAfterCalibration();
    // Restore CKE fix on setting
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), u4CKE0Bak, CKECTRL_CKEFIXON);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), u4CKE1Bak, CKECTRL_CKE1FIXON);

    // Restore HW MIOCK control setting
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), u4MIOCKBak, DRAMC_PD_CTRL_MIOCKCTRLOFF);
#endif

    /*TINFO="===  dram initial end (DramcModeRegInit_LP3)==="*/
    return DRAM_OK;
}
#endif //remove unused function to save space of log string

void MPLLInit()
{
    *((volatile unsigned int *)(0x1021222C)) = 0x00000003;	// [0]: MPLL_PWR_ON=1
    mcDELAY_US(30);
    *((volatile unsigned int *)(0x1021222C)) = 0x00000001;	// [1]: MPLL_ISO_EN=0
    mcDELAY_US(1);
    // 52MHz for DRAM <--> 208MHz MPLL
    *((volatile unsigned int *)(0x10212220))  = 0xFD000180;	// [31]: MPLL_SDM_PCW_CHG=1; [30:28]: MPLL_POSDIV; [21:0]: MPLL_SDM_PCW
    mcDELAY_US(20);
    *((volatile unsigned int *)(0x10212220))  = 0xFD000181;	// [24]: MPLL_DIV_RSTB=1
}

void DDRPhyFreqSel(DRAMC_CTX_T *p, DRAM_PLL_FREQ_SEL_T sel)
{
    p->freq_sel = sel;
    switch(p->freq_sel)
    {
        case LP4_DDR3733:
            p->frequency=1866;
            p->dram_fsp = FSP_1;
            p->odt_onoff = ODT_ON;
            p->DBI_R_onoff= DBI_ON;
            p->DBI_W_onoff = DBI_ON;
            break;
        case LP4_DDR3200:
            p->frequency=1600;
            p->dram_fsp = FSP_1;
            p->odt_onoff = ODT_ON;
            p->DBI_R_onoff= DBI_ON;
            p->DBI_W_onoff = DBI_ON;
            break;
        case LP4_DDR2667:
            p->frequency=1333;
            p->dram_fsp = FSP_1;
            p->odt_onoff = ODT_ON;
            p->DBI_R_onoff= DBI_ON;
            p->DBI_W_onoff = DBI_ON;
            break;
        case LP4_DDR2400:
            p->frequency=1200;
            p->dram_fsp = FSP_0;
            p->odt_onoff = ODT_OFF;
            p->DBI_R_onoff= DBI_OFF;
            p->DBI_W_onoff = DBI_OFF;
            break;
        case LP4_DDR1600:
            p->frequency=800;
            p->dram_fsp = FSP_0;
            p->odt_onoff = ODT_OFF;
            p->DBI_R_onoff= DBI_OFF;
            p->DBI_W_onoff = DBI_OFF;
            break;
        case LP4_DDR800:
            p->frequency=400;
            p->dram_fsp = FSP_0;
            p->odt_onoff = ODT_OFF;
            p->DBI_R_onoff= DBI_OFF;
            p->DBI_W_onoff = DBI_OFF;
            break;
#if (__ETT__ || ENABLE_LP3_SW)
	    // scy: reduce code size by removing unused LPDDR3 opp
        case LP3_DDR1866:
            p->frequency=933;
            p->dram_fsp = FSP_0;
            p->odt_onoff = ODT_OFF;
            break;
        case LP3_DDR1333:
            p->frequency=667;
            p->dram_fsp = FSP_0;
            p->odt_onoff = ODT_OFF;
            break;
        case LP3_DDR933:
            p->frequency=467;
            p->dram_fsp = FSP_0;
            p->odt_onoff = ODT_OFF;
            break;
#endif
        default:
            p->frequency=800;
            p->dram_fsp = FSP_0;
            p->odt_onoff = ODT_OFF;
            break;
    }

    if(p->dram_type == TYPE_LPDDR4P)
        p->odt_onoff = ODT_OFF;

    ///TODO: add DBI_onoff by condition
    //p->DBI_onoff = p->odt_onoff;
}

static DRAM_STATUS_T UpdateInitialSettings_LP4(DRAMC_CTX_T *p)
{
    U16 u2RXVrefDefault;

    if(p->odt_onoff==ODT_ON)
    {
        vIO32WriteFldAlign(DRAMC_REG_SHU_ODTCTRL, 1, SHU_ODTCTRL_ROEN);
    }
    else
    {
        vIO32WriteFldAlign(DRAMC_REG_SHU_ODTCTRL, 0, SHU_ODTCTRL_ROEN);
    }

    //close RX DQ/DQS tracking to save power
    vIO32WriteFldMulti(DDRPHY_R0_B0_RXDVS2, P_Fld(0x0, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0)
                | P_Fld(0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0)
                | P_Fld(0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0));
    vIO32WriteFldMulti(DDRPHY_R1_B0_RXDVS2, P_Fld(0x0, R1_B0_RXDVS2_R_RK1_DVS_MODE_B0)
                | P_Fld(0x0, R1_B0_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B0)
                | P_Fld(0x0, R1_B0_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B0));
    vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS2, P_Fld(0x0, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1)
                | P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                | P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1));
    vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS2, P_Fld(0x0, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1)
                | P_Fld(0x0, R1_B1_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                | P_Fld(0x0, R1_B1_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B1));


    //DDRPhyTxRxInitialSettings_LP4
    vIO32WriteFldAlign(DDRPHY_CA_CMD3, 0x1, CA_CMD3_RG_RX_ARCMD_STBENCMP_EN);

    vIO32WriteFldAlign(DDRPHY_CA_CMD3, 0x1, CA_CMD3_RG_RX_ARCLK_DQSIENMODE);

    vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_BIAS_VREF_SEL);

    vIO32WriteFldMulti(DDRPHY_B0_DQ3, P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B0)
                        | P_Fld(0x1, B0_DQ3_RG_RX_ARDQM0_IN_BUFF_EN)
                        | P_Fld(0x1, B0_DQ3_RG_RX_ARDQS0_IN_BUFF_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DQ3, P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B1)
                        | P_Fld(0x1, B1_DQ3_RG_RX_ARDQM1_IN_BUFF_EN)
                        | P_Fld(0x1, B1_DQ3_RG_RX_ARDQS1_IN_BUFF_EN));
    vIO32WriteFldMulti(DDRPHY_CA_CMD3, P_Fld(0x1, CA_CMD3_RG_RX_ARCMD_IN_BUFF_EN)
                            | P_Fld(0x1, CA_CMD3_RG_RX_ARCLK_IN_BUFF_EN));

    vIO32WriteFldAlign(DDRPHY_B0_DQ3, 0x0, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ3, 0x0, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1);

    vIO32WriteFldAlign(DDRPHY_B0_DQ5, 0x1, B0_DQ5_RG_RX_ARDQS0_DVS_EN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ5, 0x1, B1_DQ5_RG_RX_ARDQS0_DVS_EN_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD5, 0x1, CA_CMD5_RG_RX_ARCLK_DVS_EN);

    //LP4 no need, follow LP3 first.
    //vIO32WriteFldAlign(DDRPHY_MISC_VREF_CTRL, P_Fld(0x1, MISC_VREF_CTRL_RG_RVREF_DDR3_SEL)
    //                                        | P_Fld(0x0, MISC_VREF_CTRL_RG_RVREF_DDR4_SEL));


    /*
    vIO32WriteFldMulti(DDRPHY_CA_CMD6, P_Fld(0x0, CA_CMD6_RG_TX_ARCMD_DDR3_SEL)
                                    | P_Fld(0x1, CA_CMD6_RG_TX_ARCMD_DDR4_SEL)
                                    | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_DDR3_SEL)
                                    | P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_DDR4_SEL));
    */
    vIO32WriteFldMulti(DDRPHY_CA_CMD6, P_Fld(0x0, CA_CMD6_RG_TX_ARCMD_DDR3_SEL)
                                    | P_Fld(0x1, CA_CMD6_RG_TX_ARCMD_DDR4_SEL)
                                    | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_DDR3_SEL)
                                    | P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_DDR4_SEL));
    //WHITNEY
    vIO32WriteFldMulti(DDRPHY_B0_DQ6, P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_DDR3_SEL_B0)
                                | P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_DDR4_SEL_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DQ6, P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_DDR3_SEL_B1)
                                | P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_DDR4_SEL_B1));
    vIO32WriteFldMulti(DDRPHY_CA_CMD6, P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_DDR3_SEL)
                                | P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_DDR4_SEL));

    vIO32WriteFldMulti(DDRPHY_MISC_IMP_CTRL0, P_Fld(0x0, MISC_IMP_CTRL0_RG_RIMP_DDR3_SEL)
                                    | P_Fld(0x1, MISC_IMP_CTRL0_RG_RIMP_DDR4_SEL));

    vIO32WriteFldAlign(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_O1_SEL_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_O1_SEL_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_O1_SEL);

    vIO32WriteFldAlign(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_BIAS_PS_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_BIAS_PS_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_BIAS_PS);

    vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN);

    vIO32WriteFldAlign(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_TX_ARCMD_ODTEN_EXT_DIS);

    vIO32WriteFldAlign(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_RPRE_TOG_EN);


    if (p->dram_type == TYPE_LPDDR4)
    {
        if(p->odt_onoff==ODT_ON)
        {
            u2RXVrefDefault = 0x8;//143mv
        }
        else
        {
            u2RXVrefDefault = 0x32;
        }
    }
    else if(p->dram_type == TYPE_LPDDR4X)
    {
        if(p->odt_onoff==ODT_ON)
        {
            u2RXVrefDefault = 0x8;//143mv
        }
        else
        {
            u2RXVrefDefault = 0x2a;//300mv
        }
    }
    else if(p->dram_type == TYPE_LPDDR4P)
    {
        u2RXVrefDefault = 0x10;//180mv
    }


    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5, u2RXVrefDefault, SHU1_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, u2RXVrefDefault, SHU1_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1);
    //WHITNEY
    vIO32WriteFldAlign(DDRPHY_B0_DQ5, u2RXVrefDefault, B0_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ5, u2RXVrefDefault, B1_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B1);

    //WHITNEY
    if((p->dram_type == TYPE_LPDDR4X) ||(p->dram_type == TYPE_LPDDR4P))
    {
        // LP4x eye fine-tune
        vIO32WriteFldMulti_All(DDRPHY_B0_DQ8, P_Fld(0x1, B0_DQ8_RG_TX_ARDQ_EN_LP4P_B0)
                                            | P_Fld(0x1, B0_DQ8_RG_TX_ARDQ_EN_CAP_LP4P_B0)
                                            | P_Fld(0x1, B0_DQ8_RG_TX_ARDQ_CAP_DET_B0));
        vIO32WriteFldMulti_All(DDRPHY_B1_DQ8, P_Fld(0x1, B1_DQ8_RG_TX_ARDQ_EN_LP4P_B1)
                                            | P_Fld(0x1, B1_DQ8_RG_TX_ARDQ_EN_CAP_LP4P_B1)
                                            | P_Fld(0x1, B1_DQ8_RG_TX_ARDQ_CAP_DET_B1));
        vIO32WriteFldMulti_All(DDRPHY_CA_CMD9, P_Fld(0x1, CA_CMD9_RG_TX_ARCMD_EN_LP4P)
                                            | P_Fld(0x1, CA_CMD9_RG_TX_ARCMD_EN_CAP_LP4P)
                                            | P_Fld(0x1, CA_CMD9_RG_TX_ARCMD_CAP_DET));
    }
    else
    {
        vIO32WriteFldMulti_All(DDRPHY_B0_DQ8, P_Fld(0x0, B0_DQ8_RG_TX_ARDQ_EN_LP4P_B0)
                                            | P_Fld(0x0, B0_DQ8_RG_TX_ARDQ_EN_CAP_LP4P_B0)
                                            | P_Fld(0x0, B0_DQ8_RG_TX_ARDQ_CAP_DET_B0));
        vIO32WriteFldMulti_All(DDRPHY_B1_DQ8, P_Fld(0x0, B1_DQ8_RG_TX_ARDQ_EN_LP4P_B1)
                                            | P_Fld(0x0, B1_DQ8_RG_TX_ARDQ_EN_CAP_LP4P_B1)
                                            | P_Fld(0x0, B1_DQ8_RG_TX_ARDQ_CAP_DET_B1));
        vIO32WriteFldMulti_All(DDRPHY_CA_CMD9, P_Fld(0x0, CA_CMD9_RG_TX_ARCMD_EN_LP4P)
                                            | P_Fld(0x0, CA_CMD9_RG_TX_ARCMD_EN_CAP_LP4P)
                                            | P_Fld(0x0, CA_CMD9_RG_TX_ARCMD_CAP_DET));
    }


#if (fcFOR_CHIP_ID == fcWhitney)
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0x22, SHU1_B0_DQ7_RG_ARDQ_REV_B0); //RG_*ARDQ_REV_B*[1] =1  (RX SER PBYTE4 RESET timing enhancement)
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x22, SHU1_B1_DQ7_RG_ARDQ_REV_B1); //RG_*ARDQ_REV_B*[1] =1  (RX SER PBYTE4 RESET timing enhancement)
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD7, 0x09B2, SHU1_CA_CMD7_RG_ARCMD_REV);//WHITNEY
#else
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0x20, SHU1_B0_DQ7_RG_ARDQ_REV_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x20, SHU1_B1_DQ7_RG_ARDQ_REV_B1);
#endif

    DramcBroadcastOnOff(BROADCAST_4CHANNELS);     //DDR3_SEL & DDR4_SEL need to set CHD
    vIO32WriteFldAlign(DDRPHY_CA_CMD8, 0x1, CA_CMD8_RG_TX_RRESETB_DDR3_SEL);
    vIO32WriteFldAlign(DDRPHY_CA_CMD8, 0x0, CA_CMD8_RG_TX_RRESETB_DDR4_SEL);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);     
    //End of DDRPhyTxRxInitialSettings_LP4

    //DFS workaround
    vIO32WriteFldAlign(DRAMC_REG_SHU_MISC, 0x2, SHU_MISC_REQQUE_MAXCNT);

    //should set 0x1f, otherwise AC-timing violation
    vIO32WriteFldAlign(DRAMC_REG_SHU_SCINTV, 0x1f, SHU_SCINTV_SCINTV);


    //Update setting for TXWindowPerBit calibration according to Justin@2016/05/17 RE:[Olympus] DQ6 too small
    vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ5, 0x4, SHU1_B0_DQ5_RG_ARPI_FB_B0);
    vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ5, 0x4, SHU1_B1_DQ5_RG_ARPI_FB_B1);
    //Justin@20160701 =4(Risk@Tx), =6(Risk@Rx). In LP4, CA is not used as RX, so we keep 0x6 setting
    vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD5, 0x6, SHU1_CA_CMD5_RG_ARPI_FB_CA);

    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI1, 0x0, B0_DLL_ARPI1_RG_ARPI_OFFSET_DQSIEN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI1, 0x0, B1_DLL_ARPI1_RG_ARPI_OFFSET_DQSIEN_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI1, 0x0, CA_DLL_ARPI1_RG_ARPI_OFFSET_CLKIEN);


    //IMP Tracking Init Settings
    //Write (DRAMC _BASE+ 0x219) [31:0] = 32'h80080020//DDR3200 default
    vIO32WriteFldMulti(DRAMC_REG_SHU_IMPCAL1, P_Fld(8, SHU_IMPCAL1_IMPCAL_CALICNT) | P_Fld(0x10, SHU_IMPCAL1_IMPCALCNT) | P_Fld(4, SHU_IMPCAL1_IMPCAL_CALEN_CYCLE) | P_Fld(4, SHU_IMPCAL1_IMPCAL_CHKCYCLE));

#if SUPPORT_HYNIX_RX_DQS_WEAK_PULL
    #if 1//DQS pull down, DQSB pull up
    if (p->vendor_id == VENDOR_HYNIX)
    {
        vIO32WriteFldMulti(DDRPHY_B0_DQ7,
                                           P_Fld(0x0, B0_DQ7_RG_TX_ARDQS0_PULL_UP_B0)
                                        | P_Fld(0x1, B0_DQ7_RG_TX_ARDQS0_PULL_DN_B0)
                                        | P_Fld(0x1, B0_DQ7_RG_TX_ARDQS0B_PULL_UP_B0)
                                        | P_Fld(0x0, B0_DQ7_RG_TX_ARDQS0B_PULL_DN_B0));

        vIO32WriteFldMulti(DDRPHY_B1_DQ7,
                                           P_Fld(0x0, B1_DQ7_RG_TX_ARDQS0_PULL_UP_B1)
                                        | P_Fld(0x1, B1_DQ7_RG_TX_ARDQS0_PULL_DN_B1)
                                        | P_Fld(0x1, B1_DQ7_RG_TX_ARDQS0B_PULL_UP_B1)
                                        | P_Fld(0x0, B1_DQ7_RG_TX_ARDQS0B_PULL_DN_B1));
    }
    #endif
#endif

    //for _K_
    vIO32WriteFldMulti(DRAMC_REG_SREFCTRL, P_Fld(0x1, SREFCTRL_SCSM_CGAR)
                                            | P_Fld(0x1, SREFCTRL_SCARB_SM_CGAR)
                                            | P_Fld(0x1, SREFCTRL_RDDQSOSC_CGAR)
                                            | P_Fld(0x1, SREFCTRL_HMRRSEL_CGAR));
    vIO32WriteFldAlign(DRAMC_REG_PRE_TDQSCK1, 0x1, PRE_TDQSCK1_TXUIPI_CAL_CGAR);
    vIO32WriteFldAlign(DRAMC_REG_SHU_MISC, 0x1f, SHU_MISC_PREA_INTV);
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ8, P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDLY_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0)
                                        | P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDVS_UPD_FORCE_EN_B0)
                                        | P_Fld(0x7fff, SHU1_B0_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ8, P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMSTBEN_SYNC_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDLY_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1)
                                        | P_Fld(0x0, SHU1_B1_DQ8_R_DMRXDVS_UPD_FORCE_EN_B1)
                                        | P_Fld(0x7fff, SHU1_B1_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD8, P_Fld(0x1, SHU1_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMSTBEN_SYNC_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDLY_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA)
                                        | P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_UPD_FORCE_EN_CA)
                                        | P_Fld(0x7fff, SHU1_CA_CMD8_R_DMRXDVS_UPD_FORCE_CYC_CA));
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL3, 0x1, MISC_CTRL3_R_DDRPHY_COMB_CG_IG);
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
                                        | P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0)
                                        | P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_DQM_FLAGSEL_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
                                        | P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1)
                                        | P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_DQM_FLAGSEL_B1));

    vIO32WriteFldMulti(DRAMC_REG_CLKAR, P_Fld(0x1, CLKAR_SELPH_CMD_CG_DIS) | P_Fld(0x7FFF, CLKAR_REQQUE_PACG_DIS));


    vIO32WriteFldAlign(DRAMC_REG_SHU_DQSG_RETRY, 0x1, SHU_DQSG_RETRY_R_RETRY_PA_DSIABLE);
    vIO32WriteFldAlign(DRAMC_REG_WRITE_LEV, 0x0, WRITE_LEV_DDRPHY_COMB_CG_SEL);
    vIO32WriteFldAlign(DRAMC_REG_DUMMY_RD, 0x1, DUMMY_RD_DUMMY_RD_PA_OPT);
    vIO32WriteFldMulti(DRAMC_REG_STBCAL_CG, P_Fld(0x1, STBCAL_CG_STB_UIDLYCG_IG)
                                            | P_Fld(0x1, STBCAL_CG_STB_PIDLYCG_IG));
    vIO32WriteFldMulti(DRAMC_REG_EYESCAN, P_Fld(0x1, EYESCAN_EYESCAN_DQS_SYNC_EN)
                                            | P_Fld(0x1, EYESCAN_EYESCAN_NEW_DQ_SYNC_EN)
                                            | P_Fld(0x1, EYESCAN_EYESCAN_DQ_SYNC_EN));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x1, SHU_ODTCTRL_RODTENSTB_SELPH_CG_IG)
                                            | P_Fld(0x1, SHU_ODTCTRL_RODTEN_SELPH_CG_IG));
#ifdef WHITNEY_TO_BE_PORTING
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ0, 0x0, SHU1_B0_DQ0_RG_ARPISM_MCK_SEL_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ0, 0x0, SHU1_B1_DQ0_RG_ARPISM_MCK_SEL_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD0, 0x1, SHU1_CA_CMD0_RG_ARPISM_MCK_SEL_CA);
#endif
    //end _K_

    //DE_UPDATE
    vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_WRFIFO_OPT)
                                        | P_Fld(0x1, PERFCTL0_REORDEREN)
                                        | P_Fld(0x0, PERFCTL0_RWSPLIT));
    vIO32WriteFldAlign(DRAMC_REG_SREFCTRL, 0x0, SREFCTRL_SREF2_OPTION);
    vIO32WriteFldAlign(DRAMC_REG_SHUCTRL1, 0x4d, SHUCTRL1_FC_PRDCNT);
    vIO32WriteFldMulti(DDRPHY_B0_DQ6, P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0)
                                | P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DQ6, P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1)
                                | P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1));
    vIO32WriteFldMulti(DDRPHY_CA_CMD6, P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_OP_BIAS_SW_EN)
                                    | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_BIAS_EN));
    //end DE UPDATE


    //Rx Gating tracking settings
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DQSG), \
                                                P_Fld(9, SHU1_DQSG_STB_UPDMASKCYC) | \
                                                P_Fld(1, SHU1_DQSG_STB_UPDMASK_EN));
    vIO32WriteFldMulti_All(DRAMC_REG_SHURK0_DQSCAL, P_Fld(0, SHURK0_DQSCAL_R0DQSIENLLMTEN) | P_Fld(0, SHURK0_DQSCAL_R0DQSIENHLMTEN));
    vIO32WriteFldMulti_All(DRAMC_REG_SHURK1_DQSCAL, P_Fld(0, SHURK1_DQSCAL_R1DQSIENLLMTEN) | P_Fld(0, SHURK1_DQSCAL_R1DQSIENHLMTEN));
    vIO32WriteFldMulti_All(DRAMC_REG_SHU_RANKCTL, P_Fld(1, SHU_RANKCTL_DQSG_MODE) | P_Fld(1, SHU_RANKCTL_PICGLAT));

    //Modify for corner IC failed at HQA test XTLV
    vIO32WriteFldAlign(DDRPHY_MISC_RXDVS1, 0x7, MISC_RXDVS1_R_IN_GATE_EN_LOW_OPT);

    #if (fcFOR_CHIP_ID == fcWhitney)
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x1, SHU1_B0_DQ7_DQ_REV_B0_BIT_01) | P_Fld(0x0, SHU1_B0_DQ7_DQ_REV_B0_BIT_03) | P_Fld(0x0, SHU1_B0_DQ7_DQ_REV_B0_BIT_04) | P_Fld(0x0, SHU1_B0_DQ7_DQ_REV_B0_BIT_05));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x1, SHU1_B1_DQ7_DQ_REV_B1_BIT_01) | P_Fld(0x0, SHU1_B1_DQ7_DQ_REV_B1_BIT_03) | P_Fld(0x0, SHU1_B1_DQ7_DQ_REV_B1_BIT_04) | P_Fld(0x0, SHU1_B1_DQ7_DQ_REV_B1_BIT_05));
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD7, P_Fld(0x1, SHU1_CA_CMD7_ARCMD_REV_BIT_01) | P_Fld(0x0, SHU1_CA_CMD7_ARCMD_REV_BIT_03) | P_Fld(0x1, SHU1_CA_CMD7_ARCMD_REV_BIT_04) | P_Fld(0x1, SHU1_CA_CMD7_ARCMD_REV_BIT_05) | P_Fld(0x0, SHU1_CA_CMD7_ARCMD_REV_BIT_06));
    #endif

    vIO32WriteFldAlign(DDRPHY_B0_RXDVS0, 0x1, B0_RXDVS0_R_DMRXDLY_CG_IG_B0);
    vIO32WriteFldAlign(DDRPHY_B1_RXDVS0, 0x1, B1_RXDVS0_R_DMRXDLY_CG_IG_B1);

    return DRAM_OK;
}


#if ENABLE_LP3_SW
static DRAM_STATUS_T UpdateInitialSettings_LP3(DRAMC_CTX_T *p)
{

    //close RX DQ/DQS tracking to save power
    vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS2, P_Fld(0x0, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1)
                | P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                | P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1));
    vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS2, P_Fld(0x0, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1)
                | P_Fld(0x0, R1_B1_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                | P_Fld(0x0, R1_B1_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B1));
    vIO32WriteFldMulti(DDRPHY_R0_B0_RXDVS2+(1<<POS_BANK_NUM), P_Fld(0x0, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0)
                | P_Fld(0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0)
                | P_Fld(0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0));
    vIO32WriteFldMulti(DDRPHY_R1_B0_RXDVS2+(1<<POS_BANK_NUM), P_Fld(0x0, R1_B0_RXDVS2_R_RK1_DVS_MODE_B0)
                | P_Fld(0x0, R1_B0_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B0)
                | P_Fld(0x0, R1_B0_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B0));
    vIO32WriteFldMulti(DDRPHY_R0_CA_RXDVS2+(1<<POS_BANK_NUM), P_Fld(0x0, R0_CA_RXDVS2_R_RK0_DVS_MODE_CA)
                | P_Fld(0x0, R0_CA_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_CA)
                | P_Fld(0x0, R0_CA_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_CA));
    vIO32WriteFldMulti(DDRPHY_R1_CA_RXDVS2+(1<<POS_BANK_NUM), P_Fld(0x0, R1_CA_RXDVS2_R_RK1_DVS_MODE_CA)
                | P_Fld(0x0, R1_CA_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_CA)
                | P_Fld(0x0, R1_CA_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_CA));
    vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS2+(1<<POS_BANK_NUM), P_Fld(0x0, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1)
                | P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                | P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1));
    vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS2+(1<<POS_BANK_NUM), P_Fld(0x0, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1)
                | P_Fld(0x0, R1_B1_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                | P_Fld(0x0, R1_B1_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B1));


    //DDRPhyTxRxInitialSettings_LP3
    vIO32WriteFldMulti_All(DDRPHY_B0_DQ6, P_Fld(0x1, B0_DQ6_RG_TX_ARDQ_DDR3_SEL_B0)
                                        | P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_DDR3_SEL_B0)
                                        | P_Fld(0x0, B0_DQ6_RG_TX_ARDQ_DDR4_SEL_B0)
                                        | P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_DDR4_SEL_B0));
    vIO32WriteFldMulti_All(DDRPHY_B1_DQ6, P_Fld(0x1, B1_DQ6_RG_TX_ARDQ_DDR3_SEL_B1)
                                        | P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_DDR3_SEL_B1)
                                        | P_Fld(0x0, B1_DQ6_RG_TX_ARDQ_DDR4_SEL_B1)
                                        | P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_DDR4_SEL_B1));
    vIO32WriteFldMulti_All(DDRPHY_CA_CMD6, P_Fld(0x1, CA_CMD6_RG_TX_ARCMD_DDR3_SEL)
                                        | P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_DDR3_SEL)
                                        | P_Fld(0x0, CA_CMD6_RG_TX_ARCMD_DDR4_SEL)
                                        | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_DDR4_SEL));


    vIO32WriteFldAlign_All(DDRPHY_B0_DQ8, 0x0, B0_DQ8_RG_TX_ARDQ_EN_LP4P_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ8, 0x0, B1_DQ8_RG_TX_ARDQ_EN_LP4P_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD9, 0x0, CA_CMD9_RG_TX_ARCMD_EN_LP4P);

    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 1, B0_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B0);
    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 1, B1_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B1);
    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD5), 1, CA_CMD5_RG_RX_ARCMD_EYE_VREF_EN);

    vIO32WriteFldMulti_All(DDRPHY_CA_CMD8, P_Fld(0x1, CA_CMD8_RG_TX_RRESETB_DDR3_SEL)
                                         | P_Fld(0x0, CA_CMD8_RG_TX_RRESETB_DDR4_SEL));

    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 0xb, B0_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B0);
    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 0xb, B1_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B1);
    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD5), 0xb, CA_CMD5_RG_RX_ARCMD_EYE_VREF_SEL);

    vIO32WriteFldAlign_All(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN);




    vIO32WriteFldAlign_Phy_All(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_O1_SEL_B0);
    vIO32WriteFldAlign_Phy_All(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_O1_SEL_B1);
    vIO32WriteFldAlign_Phy_All(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_O1_SEL);

    vIO32WriteFldAlign_Phy_All(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B0);
    vIO32WriteFldAlign_Phy_All(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B1);
    vIO32WriteFldAlign_Phy_All(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_TX_ARCMD_ODTEN_EXT_DIS);

    //CH_A B0/CA = 0
    vIO32WriteFldMulti(DDRPHY_B0_DQ3, P_Fld(0x0, B0_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B0)
                                        | P_Fld(0x0, B0_DQ3_RG_RX_ARDQM0_IN_BUFF_EN)
                                        | P_Fld(0x0, B0_DQ3_RG_RX_ARDQS0_IN_BUFF_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B0_DQ3+(1<<POS_BANK_NUM), P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B0)
                                        | P_Fld(0x1, B0_DQ3_RG_RX_ARDQM0_IN_BUFF_EN)
                                        | P_Fld(0x1, B0_DQ3_RG_RX_ARDQS0_IN_BUFF_EN_B0));

    vIO32WriteFldMulti_All(DDRPHY_B1_DQ3, P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B1)
                                        | P_Fld(0x1, B1_DQ3_RG_RX_ARDQM1_IN_BUFF_EN)
                                        | P_Fld(0x1, B1_DQ3_RG_RX_ARDQS1_IN_BUFF_EN));

    vIO32WriteFldMulti(DDRPHY_CA_CMD3, P_Fld(0x0, CA_CMD3_RG_RX_ARCMD_IN_BUFF_EN)
                                         | P_Fld(0x0, CA_CMD3_RG_RX_ARCLK_IN_BUFF_EN));
    vIO32WriteFldMulti(DDRPHY_CA_CMD3+(1<<POS_BANK_NUM), P_Fld(0x1, CA_CMD3_RG_RX_ARCMD_IN_BUFF_EN)
                                         | P_Fld(0x1, CA_CMD3_RG_RX_ARCLK_IN_BUFF_EN));

    vIO32WriteFldAlign(DDRPHY_MISC_RXDVS1, 0x8000000e, MISC_RXDVS1_R_IN_GATE_EN_LOW_OPT);
    vIO32WriteFldAlign(DDRPHY_MISC_RXDVS1+(1<<POS_BANK_NUM), 0x8000000e, MISC_RXDVS1_R_IN_GATE_EN_LOW_OPT);
    //End of DDRPhyTxRxInitialSettings_LP3

    //DFS workaround
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_MISC, 0x2, SHU_MISC_REQQUE_MAXCNT);

    //should set 0x1f, otherwise AC-timing violation
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_SCINTV, 0x1f, SHU_SCINTV_SCINTV);


    //for _K_
    vIO32WriteFldMulti(DRAMC_REG_SREFCTRL, P_Fld(0x1, SREFCTRL_K_SCSM_CGAR)
                                            | P_Fld(0x1, SREFCTRL_K_SCARB_SM_CGAR)
                                            | P_Fld(0x1, SREFCTRL_K_RDDQSOSC_CGAR)
                                            | P_Fld(0x1, SREFCTRL_K_HMRRSEL_CGAR));
    vIO32WriteFldAlign(DRAMC_REG_PRE_TDQSCK1, 0x1, PRE_TDQSCK1_K_TXUIPI_CAL_CGAR);
    vIO32WriteFldAlign(DRAMC_REG_SHU_MISC, 0x1f, SHU_MISC_K_PREA_INTV);
    vIO32WriteFldAlign(DRAMC_REG_DVFSDLL, 0x0, DVFSDLL_K_R_BYPASS_1ST_DLL);
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ8, P_Fld(0x1, SHU1_B0_DQ8_K_R_DMRANK_CHG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_K_R_DMRANK_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_K_R_DMRDSEL_TOG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_K_R_DMRDSEL_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_K_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_K_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_K_R_DMSTBEN_SYNC_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_K_R_DMRXDLY_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_K_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_K_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0)
                                        | P_Fld(0x0, SHU1_B0_DQ8_K_R_DMRXDVS_UPD_FORCE_EN_B0)
                                        | P_Fld(0x7fff, SHU1_B0_DQ8_K_R_DMRXDVS_UPD_FORCE_CYC_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ8+(1<<POS_BANK_NUM), P_Fld(0x1, SHU1_B0_DQ8_K_R_DMRANK_CHG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_K_R_DMRANK_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_K_R_DMRDSEL_TOG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_K_R_DMRDSEL_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_K_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_K_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_K_R_DMSTBEN_SYNC_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_K_R_DMRXDLY_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_K_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_K_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0)
                                        | P_Fld(0x0, SHU1_B0_DQ8_K_R_DMRXDVS_UPD_FORCE_EN_B0)
                                        | P_Fld(0x7fff, SHU1_B0_DQ8_K_R_DMRXDVS_UPD_FORCE_CYC_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ8, P_Fld(0x1, SHU1_B1_DQ8_K_R_DMRANK_CHG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_K_R_DMRANK_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_K_R_DMRDSEL_TOG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_K_R_DMRDSEL_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_K_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_K_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_K_R_DMSTBEN_SYNC_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_K_R_DMRXDLY_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_K_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_K_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1)
                                        | P_Fld(0x0, SHU1_B1_DQ8_K_R_DMRXDVS_UPD_FORCE_EN_B1)
                                        | P_Fld(0x7fff, SHU1_B1_DQ8_K_R_DMRXDVS_UPD_FORCE_CYC_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ8+(1<<POS_BANK_NUM), P_Fld(0x1, SHU1_B1_DQ8_K_R_DMRANK_CHG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_K_R_DMRANK_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_K_R_DMRDSEL_TOG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_K_R_DMRDSEL_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_K_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_K_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_K_R_DMSTBEN_SYNC_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_K_R_DMRXDLY_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_K_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_K_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1)
                                        | P_Fld(0x0, SHU1_B1_DQ8_K_R_DMRXDVS_UPD_FORCE_EN_B1)
                                        | P_Fld(0x7fff, SHU1_B1_DQ8_K_R_DMRXDVS_UPD_FORCE_CYC_B1));
    vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD8, P_Fld(0x1, SHU1_CA_CMD8_K_R_DMRANK_CHG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_K_R_DMRANK_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_K_R_DMRDSEL_TOG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_K_R_DMRDSEL_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_K_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_K_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_K_R_DMSTBEN_SYNC_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_K_R_DMRXDLY_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_K_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_K_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA)
                                        | P_Fld(0x0, SHU1_CA_CMD8_K_R_DMRXDVS_UPD_FORCE_EN_CA)
                                        | P_Fld(0x7fff, SHU1_CA_CMD8_K_R_DMRXDVS_UPD_FORCE_CYC_CA));
    vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD8+(1<<POS_BANK_NUM), P_Fld(0x1, SHU1_CA_CMD8_K_R_DMRANK_CHG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_K_R_DMRANK_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_K_R_DMRDSEL_TOG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_K_R_DMRDSEL_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_K_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_K_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_K_R_DMSTBEN_SYNC_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_K_R_DMRXDLY_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_K_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_K_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA)
                                        | P_Fld(0x0, SHU1_CA_CMD8_K_R_DMRXDVS_UPD_FORCE_EN_CA)
                                        | P_Fld(0x7fff, SHU1_CA_CMD8_K_R_DMRXDVS_UPD_FORCE_CYC_CA));
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL3, 0x1, MISC_CTRL3_K_R_DDRPHY_COMB_CG_IG);
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL3+(1<<POS_BANK_NUM), 0x1, MISC_CTRL3_K_R_DDRPHY_COMB_CG_IG);
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL0, 0x0, MISC_CTRL0_K_R_DMSHU_PHYDCM_FORCEOFF);
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL0+(1<<POS_BANK_NUM), 0x0, MISC_CTRL0_K_R_DMSHU_PHYDCM_FORCEOFF);
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
                                        | P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0)
                                        | P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_DQM_FLAGSEL_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7+(1<<POS_BANK_NUM), P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
                                        | P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0)
                                        | P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_DQM_FLAGSEL_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
                                        | P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1)
                                        | P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_DQM_FLAGSEL_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7+(1<<POS_BANK_NUM), P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
                                            | P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1)
                                            | P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_DQM_FLAGSEL_B1));

    if(mt_get_chip_sw_ver()==CHIP_SW_VER_02)
    {
        vIO32WriteFldMulti(DRAMC_REG_CLKAR, P_Fld(0x1, CLKAR_K_SELPH_CMD_CG_DIS) | P_Fld(0x7FFF, CLKAR_K_REQQUE_PACG_DIS));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_CLKAR, P_Fld(0x0, CLKAR_SEQCLKRUN1) | P_Fld(0x0, CLKAR_SEQCLKRUN2));
    }
    vIO32WriteFldAlign(DRAMC_REG_SHU_DQSG_RETRY, 0x1, SHU_DQSG_RETRY_K_R_RETRY_PA_DSIABLE);
    vIO32WriteFldAlign(DRAMC_REG_WRITE_LEV, 0x0, WRITE_LEV_K_DDRPHY_COMB_CG_SEL);
    vIO32WriteFldAlign(DRAMC_REG_DUMMY_RD, 0x1, DUMMY_RD_K_DUMMY_RD_PA_OPT);
    vIO32WriteFldMulti(DRAMC_REG_STBCAL_CG, P_Fld(0x1, STBCAL_CG_K_STB_UIDLYCG_IG)
                                            | P_Fld(0x1, STBCAL_CG_K_STB_PIDLYCG_IG));
    vIO32WriteFldMulti(DRAMC_REG_EYESCAN, P_Fld(0x1, EYESCAN_EYESCAN_DQS_SYNC_EN)
                                            | P_Fld(0x1, EYESCAN_EYESCAN_NEW_DQ_SYNC_EN)
                                            | P_Fld(0x1, EYESCAN_EYESCAN_DQ_SYNC_EN));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x1, SHU_ODTCTRL_K_RODTENSTB_SELPH_CG_IG)
                                            | P_Fld(0x1, SHU_ODTCTRL_K_RODTEN_SELPH_CG_IG));

    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ0, 0x1, SHU1_B0_DQ0_K_RG_ARPISM_MCK_SEL_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ0, 0x0, SHU1_B1_DQ0_K_RG_ARPISM_MCK_SEL_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD0, 0x1, SHU1_CA_CMD0_K_RG_ARPISM_MCK_SEL_CA);
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ0+(1<<POS_BANK_NUM), 0x0, SHU1_B0_DQ0_K_RG_ARPISM_MCK_SEL_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ0+(1<<POS_BANK_NUM), 0x0, SHU1_B1_DQ0_K_RG_ARPISM_MCK_SEL_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD0+(1<<POS_BANK_NUM), 0x0, SHU1_CA_CMD0_K_RG_ARPISM_MCK_SEL_CA);
    //end _K_

    //Rx Gating tracking settings
    vIO32WriteFldMulti_All(DRAMC_REG_SHURK0_DQSCAL, P_Fld(0, SHURK0_DQSCAL_R0DQSIENLLMTEN) | P_Fld(0, SHURK0_DQSCAL_R0DQSIENHLMTEN));
    vIO32WriteFldMulti_All(DRAMC_REG_SHURK1_DQSCAL, P_Fld(0, SHURK1_DQSCAL_R1DQSIENLLMTEN) | P_Fld(0, SHURK1_DQSCAL_R1DQSIENHLMTEN));
    vIO32WriteFldMulti_All(DRAMC_REG_SHU_RANKCTL, P_Fld(1, SHU_RANKCTL_DQSG_MODE) | P_Fld(1, SHU_RANKCTL_PICGLAT));

    //Jade-like ACTiming
    #if DramcHWDQSGatingTracking_DVT_JADE_TRACKING_MODE
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_PIPE, 0xFC000000,PHY_FLD_FULL);
    vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI1, 0x38, B1_DLL_ARPI1_RG_ARPI_OFFSET_DQSIEN_B1);
    vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI1+(1<<POS_BANK_NUM), 0x38, B0_DLL_ARPI1_RG_ARPI_OFFSET_DQSIEN_B0);
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI1+(1<<POS_BANK_NUM), 0x38, CA_DLL_ARPI1_RG_ARPI_OFFSET_CLKIEN);
    vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI1+(1<<POS_BANK_NUM), 0x38, B1_DLL_ARPI1_RG_ARPI_OFFSET_DQSIEN_B1);

    //if(p->frequency==933)//test code
    {
        vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0, SHU1_B0_DQ7_DQ_REV_B0_BIT_05);
        vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD7, 0, SHU1_CA_CMD7_ARCMD_REV_BIT_05);
        vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0, SHU1_B1_DQ7_DQ_REV_B1_BIT_05);
        vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7+(1<<POS_BANK_NUM), 0, SHU1_B0_DQ7_DQ_REV_B0_BIT_05);
        vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD7+(1<<POS_BANK_NUM), 0, SHU1_CA_CMD7_ARCMD_REV_BIT_05);
        vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7+(1<<POS_BANK_NUM), 0, SHU1_B1_DQ7_DQ_REV_B1_BIT_05);
    }

    vIO32WriteFldAlign_All(DRAMC_REG_SHU1_DQSG, 0x0, SHU1_DQSG_STB_UPDMASKCYC);
    vIO32WriteFldAlign_All(DRAMC_REG_SHU1_DQSG, 0x0, SHU1_DQSG_STB_UPDMASK_EN);
    if(p->frequency==933)
    {
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_RANKCTL, 3, SHU_RANKCTL_DMSTBLAT);
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM2, 0x7, SHU_ACTIM2_TR2W);
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_AC_TIME_05T, 0, SHU_AC_TIME_05T_TR2W_05T);
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM_XRT, 0x5, SHU_ACTIM_XRT_XRTW2R);
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM_XRT, 0x6, SHU_ACTIM_XRT_XRTR2W);
    }
    else if(p->frequency==667)
    {
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_RANKCTL, 2, SHU_RANKCTL_DMSTBLAT);
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM2, 0x5, SHU_ACTIM2_TR2W);
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_AC_TIME_05T, 0, SHU_AC_TIME_05T_TR2W_05T);
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM_XRT, 0x5, SHU_ACTIM_XRT_XRTW2R);
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM_XRT, 0x6, SHU_ACTIM_XRT_XRTR2W);
    }
    else if(p->frequency==467)
    {
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_RANKCTL, 1, SHU_RANKCTL_DMSTBLAT);
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM2, 0x4, SHU_ACTIM2_TR2W);
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_AC_TIME_05T, 0, SHU_AC_TIME_05T_TR2W_05T);
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM_XRT, 0x5, SHU_ACTIM_XRT_XRTW2R);
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM_XRT, 0x6, SHU_ACTIM_XRT_XRTR2W);
    }
    #else
    //Valid delay mode debug : monitor window
    //vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5+(1<<POS_BANK_NUM), 7, SHU1_B0_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5+(1<<POS_BANK_NUM), 7, SHU1_B1_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B1);
    //vIO32WriteFldAlign_All(DRAMC_REG_SHU_PIPE, 0xF0000000,PHY_FLD_FULL);
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DQSG), \
                                            P_Fld(9, SHU1_DQSG_STB_UPDMASKCYC) | \
                                            P_Fld(1, SHU1_DQSG_STB_UPDMASK_EN));
    #endif

    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5, 0x4, SHU1_B0_DQ5_RG_ARPI_FB_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, 0x4, SHU1_B1_DQ5_RG_ARPI_FB_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD5, 0x4, SHU1_CA_CMD5_RG_ARPI_FB_CA);
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5+(1<<POS_BANK_NUM), 0x4, SHU1_B0_DQ5_RG_ARPI_FB_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5+(1<<POS_BANK_NUM), 0x4, SHU1_B1_DQ5_RG_ARPI_FB_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD5+(1<<POS_BANK_NUM), 0x4, SHU1_CA_CMD5_RG_ARPI_FB_CA);

    //DE_UPDATE
    vIO32WriteFldMulti(DRAMC_REG_DRAMCTRL, P_Fld(0, DRAMCTRL_FW2R)
                                        | P_Fld(0x0, DRAMCTRL_DYNMWREN)
                                        | P_Fld(0x0, DRAMCTRL_CLKWITRFC)
                                        | P_Fld(0x1, DRAMCTRL_TMRR2WDIS)
                                        | P_Fld(0x0, DRAMCTRL_ADRBIT3DEC)
                                        | P_Fld(0x0, DRAMCTRL_CTOREQ_HPRI_OPT));
    vIO32WriteFldAlign(DRAMC_REG_MISCTL0, 0x1, MISCTL0_PBC_ARB_EN); //Darren+ for LPDDR3
    vIO32WriteFldAlign(DRAMC_REG_PERFCTL0, 0x1, PERFCTL0_WRFIFO_OPT);
    vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_REORDEREN)
                    | P_Fld(0x0, PERFCTL0_RWHPRICTL)
                    | P_Fld(0x1, PERFCTL0_EMILLATEN)
                    | P_Fld(0x1, PERFCTL0_RWAGEEN)
                    | P_Fld(0x1, PERFCTL0_RWHPRIEN)); //Darren+ by Derping confirm
    vIO32WriteFldAlign(DRAMC_REG_SREFCTRL, 0x8, SREFCTRL_SREFDLY);
    vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_REFR_BLOCKEN);
    vIO32WriteFldAlign(DRAMC_REG_HW_MRR_FUN, 0x0, HW_MRR_FUN_TMRR_ENA);
    vIO32WriteFldAlign(DRAMC_REG_SHUCTRL, 0x0, SHUCTRL_VRCG_EN);
    vIO32WriteFldMulti(DRAMC_REG_REFCTRL0, P_Fld(0x1, REFCTRL0_REFNA_OPT)
                | P_Fld(0x3, REFCTRL0_DISBYREFNUM)
                | P_Fld(0x1, REFCTRL0_UPDBYWR)); //Darren+ for LPDDR3
    vIO32WriteFldMulti(DDRPHY_B0_DQ6, P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0)
                                | P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B0_DQ6+(1<<POS_BANK_NUM), P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0)
                                | P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DQ6, P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1)
                                | P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1));
    vIO32WriteFldMulti(DDRPHY_B1_DQ6+(1<<POS_BANK_NUM), P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1)
                                | P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1));
    vIO32WriteFldMulti(DDRPHY_CA_CMD6, P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_OP_BIAS_SW_EN)
                                    | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_BIAS_EN));
    vIO32WriteFldMulti(DDRPHY_CA_CMD6+(1<<POS_BANK_NUM), P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_OP_BIAS_SW_EN)
                                    | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_BIAS_EN));
    //end DE UPDATE

    return DRAM_OK;
}
#endif

#if ENABLE_LP3_SW
static void DramcSetting_Olympus_LP3_DDR1333(DRAMC_CTX_T *p)
{
    ////DRAMC0-SHU2
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM0, P_Fld(0x7, SHU_ACTIM0_TRCD)
                | P_Fld(0x1, SHU_ACTIM0_TRRD)
                | P_Fld(0x9, SHU_ACTIM0_TWR)
                | P_Fld(0x6, SHU_ACTIM0_TWTR));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM1, P_Fld(0xe, SHU_ACTIM1_TRC)
                | P_Fld(0x5, SHU_ACTIM1_TRAS)
                | P_Fld(0x6, SHU_ACTIM1_TRP)
                | P_Fld(0x1, SHU_ACTIM1_TRPAB));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM2, P_Fld(0x8, SHU_ACTIM2_TFAW)
                | P_Fld(0x4, SHU_ACTIM2_TR2W)
                | P_Fld(0x1, SHU_ACTIM2_TRTP));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM3, P_Fld(0x3a, SHU_ACTIM3_TRFC)
                | P_Fld(0x13, SHU_ACTIM3_TRFCPB));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM4, P_Fld(0x1d, SHU_ACTIM4_TZQCS)
                | P_Fld(0x50, SHU_ACTIM4_REFCNT)
                | P_Fld(0x48, SHU_ACTIM4_TXREFCNT));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM5, P_Fld(0x8, SHU_ACTIM5_TWTPD)
                | P_Fld(0x7, SHU_ACTIM5_TR2PD));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x2, SHU_ACTIM_XRT_XRTR2W)
                | P_Fld(0x9, SHU_ACTIM_XRT_XRTR2R));
    vIO32WriteFldMulti(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(0x0, SHU_AC_TIME_05T_TFAW_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TRRD_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TWR_M05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TRAS_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TRPAB_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TRP_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TRCD_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TRTP_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TXP_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TRC_05T));
    vIO32WriteFldMulti(DRAMC_REG_SHU_CONF1, P_Fld(0xb, SHU_CONF1_DATLAT_DSEL_PHY)
                | P_Fld(0xc, SHU_CONF1_DATLAT_DSEL)
                | P_Fld(0xd, SHU_CONF1_DATLAT));
    vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x3, SHU_RANKCTL_RANKINCTL_PHY)
                | P_Fld(0x1, SHU_RANKCTL_RANKINCTL_ROOT1)
                | P_Fld(0x1, SHU_RANKCTL_RANKINCTL));
    vIO32WriteFldAlign(DRAMC_REG_SHU_CKECTRL, 0x2, SHU_CKECTRL_CKEPRD);
    vIO32WriteFldAlign(DRAMC_REG_SHU_ODTCTRL, 0x5, SHU_ODTCTRL_RODT);
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS0, P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_OEN_DQS3)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_OEN_DQS2)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_OEN_DQS1)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_OEN_DQS0)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS3)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS2)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS1)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS1, P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS3)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS2)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS1)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS0)
                | P_Fld(0x7, SHU_SELPH_DQS1_DLY_DQS3)
                | P_Fld(0x7, SHU_SELPH_DQS1_DLY_DQS2)
                | P_Fld(0x7, SHU_SELPH_DQS1_DLY_DQS1)
                | P_Fld(0x7, SHU_SELPH_DQS1_DLY_DQS0));
    vIO32WriteFldAlign(DRAMC_REG_SHU_SCINTV, 0x1f, SHU_SCINTV_SCINTV);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR13, 0x18, SHU_HWSET_MR13_HWSET_MR13_OP);
    vIO32WriteFldAlign(DRAMC_REG_SHURK0_DQSCTL, 0x4, SHURK0_DQSCTL_DQSINCTL);
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x0, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1)
                | P_Fld(0x0, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED)
                | P_Fld(0x0, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1)
                | P_Fld(0x0, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED)
                | P_Fld(0x0, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                | P_Fld(0x0, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                | P_Fld(0x0, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                | P_Fld(0x0, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED_P1)
                | P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED)
                | P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED_P1)
                | P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED)
                | P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                | P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                | P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                | P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ2)
                | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM2)
                | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
                | P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
                | P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
                | P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_OEN_DQ0)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_DQ3)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_DQ2)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_DQ1)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
                | P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
                | P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
                | P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_OEN_DQM0)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_DQM3)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_DQM2)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_DQM1)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_DQM0));
    vIO32WriteFldAlign(DRAMC_REG_SHURK1_DQSCTL, 0x4, SHURK1_DQSCTL_R1DQSINCTL);
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x2, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED_P1)
                | P_Fld(0x0, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED)
                | P_Fld(0x2, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED_P1)
                | P_Fld(0x0, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED)
                | P_Fld(0x2, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                | P_Fld(0x0, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                | P_Fld(0x2, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                | P_Fld(0x0, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
                | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
                | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
                | P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
                | P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
                | P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1DQ3)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1DQ2)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1DQ1)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
                | P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
                | P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
                | P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1DQM3)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1DQM2)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1DQM1)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1DQM0));
    vIO32WriteFldAlign(DRAMC_REG_SHURK2_DQSCTL, 0x4, SHURK2_DQSCTL_R2DQSINCTL);
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ0, P_Fld(0x2, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ3)
                | P_Fld(0x2, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ2)
                | P_Fld(0x2, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ1)
                | P_Fld(0x2, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ0)
                | P_Fld(0x2, SHURK2_SELPH_DQ0_TX_DLY_R2DQ3)
                | P_Fld(0x2, SHURK2_SELPH_DQ0_TX_DLY_R2DQ2)
                | P_Fld(0x2, SHURK2_SELPH_DQ0_TX_DLY_R2DQ1)
                | P_Fld(0x2, SHURK2_SELPH_DQ0_TX_DLY_R2DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ1, P_Fld(0x2, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM3)
                | P_Fld(0x2, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM2)
                | P_Fld(0x2, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM1)
                | P_Fld(0x2, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM0)
                | P_Fld(0x2, SHURK2_SELPH_DQ1_TX_DLY_R2DQM3)
                | P_Fld(0x2, SHURK2_SELPH_DQ1_TX_DLY_R2DQM2)
                | P_Fld(0x2, SHURK2_SELPH_DQ1_TX_DLY_R2DQM1)
                | P_Fld(0x2, SHURK2_SELPH_DQ1_TX_DLY_R2DQM0));
    ////DRAMC1-SHU2
    ////DDRPHY0-SHU2
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, 0x6, SHU1_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, 0x3300, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, 0x3300, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6, P_Fld(0x16, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0xe, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0, P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1, 0x4, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6, P_Fld(0x17, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0xf, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1));
    ////DDRPHY1-SHU2
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5+(1<<POS_BANK_NUM), 0x6, SHU1_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5+(1<<POS_BANK_NUM), 0x6, SHU1_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD5+(1<<POS_BANK_NUM), 0x6, SHU1_CA_CMD5_RG_RX_ARCLK_DVS_DLY);
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ2+(1<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_R0_B0_DQ3+(1<<POS_BANK_NUM), 0x12, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_F_DLY_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_R0_B0_DQ4+(1<<POS_BANK_NUM), 0x12, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_F_DLY_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_R0_B0_DQ5+(1<<POS_BANK_NUM), 0x12, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_F_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ6+(1<<POS_BANK_NUM), P_Fld(0x16, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0xe, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6+(1<<POS_BANK_NUM), P_Fld(0x16, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0xe, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD8+(1<<POS_BANK_NUM), P_Fld(0x16, SHU1_R0_CA_CMD8_RG_RK0_RX_ARCLK_F_DLY)
                | P_Fld(0xe, SHU1_R0_CA_CMD8_RG_RK0_RX_ARCLK_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ0+(1<<POS_BANK_NUM), P_Fld(0x4, SHU1_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0)
                | P_Fld(0x4, SHU1_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0)
                | P_Fld(0x4, SHU1_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0)
                | P_Fld(0x4, SHU1_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0)
                | P_Fld(0x4, SHU1_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0)
                | P_Fld(0x4, SHU1_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0)
                | P_Fld(0x4, SHU1_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0)
                | P_Fld(0x4, SHU1_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B0_DQ1+(1<<POS_BANK_NUM), 0x4, SHU1_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ6+(1<<POS_BANK_NUM), P_Fld(0x17, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0xf, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0+(1<<POS_BANK_NUM), P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1+(1<<POS_BANK_NUM), 0x4, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6+(1<<POS_BANK_NUM), P_Fld(0x17, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0xf, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD0+(1<<POS_BANK_NUM), P_Fld(0x4, SHU1_R1_CA_CMD0_RK1_TX_ARCA5_DLY)
                | P_Fld(0x4, SHU1_R1_CA_CMD0_RK1_TX_ARCA4_DLY)
                | P_Fld(0x4, SHU1_R1_CA_CMD0_RK1_TX_ARCA3_DLY)
                | P_Fld(0x4, SHU1_R1_CA_CMD0_RK1_TX_ARCA2_DLY)
                | P_Fld(0x4, SHU1_R1_CA_CMD0_RK1_TX_ARCA1_DLY)
                | P_Fld(0x4, SHU1_R1_CA_CMD0_RK1_TX_ARCA0_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD1+(1<<POS_BANK_NUM), P_Fld(0x4, SHU1_R1_CA_CMD1_RK1_TX_ARCKE2_DLY)
                | P_Fld(0x4, SHU1_R1_CA_CMD1_RK1_TX_ARCKE1_DLY)
                | P_Fld(0x4, SHU1_R1_CA_CMD1_RK1_TX_ARCKE0_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD8+(1<<POS_BANK_NUM), P_Fld(0x17, SHU1_R1_CA_CMD8_RG_RK1_RX_ARCLK_F_DLY)
                | P_Fld(0xf, SHU1_R1_CA_CMD8_RG_RK1_RX_ARCLK_R_DLY));
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B0_DQ1+(1<<POS_BANK_NUM), 0xb, SHU1_R2_B0_DQ1_RK2_TX_ARDQM0_DLY_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B1_DQ1+(1<<POS_BANK_NUM), 0xb, SHU1_R2_B1_DQ1_RK2_TX_ARDQM0_DLY_B1);

}

#if 0
static void DramcSetting_Olympus_LP3_DDR933(DRAMC_CTX_T *p)
{
    ////DRAMC0-SHU3
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM0, P_Fld(0x4, SHU_ACTIM0_TRCD)
                | P_Fld(0x0, SHU_ACTIM0_TRRD)
                | P_Fld(0x6, SHU_ACTIM0_TWR)
                | P_Fld(0x3, SHU_ACTIM0_TWTR));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM1, P_Fld(0x5, SHU_ACTIM1_TRC)
                | P_Fld(0x0, SHU_ACTIM1_TRAS)
                | P_Fld(0x3, SHU_ACTIM1_TRP)
                | P_Fld(0x0, SHU_ACTIM1_TRPAB));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM2, P_Fld(0x1, SHU_ACTIM2_TFAW)
                | P_Fld(0x3, SHU_ACTIM2_TR2W)
                | P_Fld(0x0, SHU_ACTIM2_TRTP));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM3, P_Fld(0x1e, SHU_ACTIM3_TRFC)
                | P_Fld(0x7, SHU_ACTIM3_TRFCPB));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM4, P_Fld(0x11, SHU_ACTIM4_TZQCS)
                | P_Fld(0x2f, SHU_ACTIM4_REFCNT)
                | P_Fld(0x2a, SHU_ACTIM4_TXREFCNT));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM5, P_Fld(0x5, SHU_ACTIM5_TWTPD)
                | P_Fld(0x5, SHU_ACTIM5_TR2PD));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x1, SHU_ACTIM_XRT_XRTR2W)
                | P_Fld(0x9, SHU_ACTIM_XRT_XRTR2R));
    vIO32WriteFldMulti(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(0x0, SHU_AC_TIME_05T_TFAW_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TRRD_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TWR_M05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TRAS_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TRPAB_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TRP_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TRCD_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TRTP_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TXP_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TRC_05T));
    vIO32WriteFldMulti(DRAMC_REG_SHU_CONF1, P_Fld(0xa, SHU_CONF1_DATLAT_DSEL_PHY)
                | P_Fld(0xb, SHU_CONF1_DATLAT_DSEL)
                | P_Fld(0xc, SHU_CONF1_DATLAT));
    vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x1, SHU_RANKCTL_RANKINCTL_PHY)
                | P_Fld(0x0, SHU_RANKCTL_RANKINCTL_ROOT1)
                | P_Fld(0x0, SHU_RANKCTL_RANKINCTL));
    vIO32WriteFldAlign(DRAMC_REG_SHU_CKECTRL, 0x2, SHU_CKECTRL_CKEPRD);
    vIO32WriteFldAlign(DRAMC_REG_SHU_ODTCTRL, 0x3, SHU_ODTCTRL_RODT);
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS0, P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS3)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS2)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS1)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS0)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_DQS3)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_DQS2)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_DQS1)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_DQS0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS1, P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS3)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS2)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS1)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS0)
                | P_Fld(0x7, SHU_SELPH_DQS1_DLY_DQS3)
                | P_Fld(0x7, SHU_SELPH_DQS1_DLY_DQS2)
                | P_Fld(0x7, SHU_SELPH_DQS1_DLY_DQS1)
                | P_Fld(0x7, SHU_SELPH_DQS1_DLY_DQS0));
    vIO32WriteFldAlign(DRAMC_REG_SHU_SCINTV, 0xf, SHU_SCINTV_SCINTV);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR13, 0x16, SHU_HWSET_MR13_HWSET_MR13_OP);
    vIO32WriteFldAlign(DRAMC_REG_SHURK0_DQSCTL, 0x2, SHURK0_DQSCTL_DQSINCTL);
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1)
                | P_Fld(0x0, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1)
                | P_Fld(0x0, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                | P_Fld(0x0, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                | P_Fld(0x0, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED_P1)
                | P_Fld(0x7, SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED)
                | P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED_P1)
                | P_Fld(0x7, SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED)
                | P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                | P_Fld(0x7, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                | P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                | P_Fld(0x7, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_DQ2)
                | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_DQM2)
                | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
                | P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
                | P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
                | P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_OEN_DQ0)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_DQ3)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_DQ2)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_DQ1)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
                | P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
                | P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
                | P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_OEN_DQM0)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_DQM3)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_DQM2)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_DQM1)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_DQM0));
    vIO32WriteFldAlign(DRAMC_REG_SHURK1_DQSCTL, 0x2, SHURK1_DQSCTL_R1DQSINCTL);
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED)
                | P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED)
                | P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                | P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
                | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
                | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
                | P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
                | P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
                | P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1DQ3)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1DQ2)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1DQ1)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
                | P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
                | P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
                | P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1DQM3)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1DQM2)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1DQM1)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1DQM0));
    vIO32WriteFldAlign(DRAMC_REG_SHURK2_DQSCTL, 0x2, SHURK2_DQSCTL_R2DQSINCTL);
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ0, P_Fld(0x1, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ3)
                | P_Fld(0x1, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ2)
                | P_Fld(0x1, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ1)
                | P_Fld(0x1, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ0)
                | P_Fld(0x1, SHURK2_SELPH_DQ0_TX_DLY_R2DQ3)
                | P_Fld(0x1, SHURK2_SELPH_DQ0_TX_DLY_R2DQ2)
                | P_Fld(0x1, SHURK2_SELPH_DQ0_TX_DLY_R2DQ1)
                | P_Fld(0x1, SHURK2_SELPH_DQ0_TX_DLY_R2DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ1, P_Fld(0x1, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM3)
                | P_Fld(0x1, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM2)
                | P_Fld(0x1, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM1)
                | P_Fld(0x1, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM0)
                | P_Fld(0x1, SHURK2_SELPH_DQ1_TX_DLY_R2DQM3)
                | P_Fld(0x1, SHURK2_SELPH_DQ1_TX_DLY_R2DQM2)
                | P_Fld(0x1, SHURK2_SELPH_DQ1_TX_DLY_R2DQM1)
                | P_Fld(0x1, SHURK2_SELPH_DQ1_TX_DLY_R2DQM0));
    ////DRAMC1-SHU3
    ////DDRPHY0-SHU3
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, 0x7, SHU1_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, 0x1e00, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, 0x1e00, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6, P_Fld(0x26, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0x1e, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0, P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1, 0x0, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6, P_Fld(0x27, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0x1f, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1));
    ////DDRPHY1-SHU3
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5+(1<<POS_BANK_NUM), 0x7, SHU1_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5+(1<<POS_BANK_NUM), 0x7, SHU1_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD5+(1<<POS_BANK_NUM), 0x7, SHU1_CA_CMD5_RG_RX_ARCLK_DVS_DLY);
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ2+(1<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_R0_B0_DQ3+(1<<POS_BANK_NUM), 0xa, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_F_DLY_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_R0_B0_DQ4+(1<<POS_BANK_NUM), 0xa, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_F_DLY_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_R0_B0_DQ5+(1<<POS_BANK_NUM), 0xa, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_F_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ6+(1<<POS_BANK_NUM), P_Fld(0x26, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0x1e, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6+(1<<POS_BANK_NUM), P_Fld(0x26, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0x1e, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD8+(1<<POS_BANK_NUM), P_Fld(0x26, SHU1_R0_CA_CMD8_RG_RK0_RX_ARCLK_F_DLY)
                | P_Fld(0x1e, SHU1_R0_CA_CMD8_RG_RK0_RX_ARCLK_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ0+(1<<POS_BANK_NUM), P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B0_DQ1+(1<<POS_BANK_NUM), 0x0, SHU1_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ6+(1<<POS_BANK_NUM), P_Fld(0x27, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0x1f, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0+(1<<POS_BANK_NUM), P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1+(1<<POS_BANK_NUM), 0x0, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6+(1<<POS_BANK_NUM), P_Fld(0x27, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0x1f, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD0+(1<<POS_BANK_NUM), P_Fld(0xb, SHU1_R1_CA_CMD0_RK1_TX_ARCA5_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD0_RK1_TX_ARCA4_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD0_RK1_TX_ARCA3_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD0_RK1_TX_ARCA2_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD0_RK1_TX_ARCA1_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD0_RK1_TX_ARCA0_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD1+(1<<POS_BANK_NUM), P_Fld(0xb, SHU1_R1_CA_CMD1_RK1_TX_ARCKE2_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD1_RK1_TX_ARCKE1_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD1_RK1_TX_ARCKE0_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD8+(1<<POS_BANK_NUM), P_Fld(0x27, SHU1_R1_CA_CMD8_RG_RK1_RX_ARCLK_F_DLY)
                | P_Fld(0x1f, SHU1_R1_CA_CMD8_RG_RK1_RX_ARCLK_R_DLY));
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B0_DQ1+(1<<POS_BANK_NUM), 0x0, SHU1_R2_B0_DQ1_RK2_TX_ARDQM0_DLY_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B1_DQ1+(1<<POS_BANK_NUM), 0x0, SHU1_R2_B1_DQ1_RK2_TX_ARDQM0_DLY_B1);

}
#endif

static void DramcSetting_Olympus_LP3(DRAMC_CTX_T *p)
{
    //26M
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_CLK_MEM_SEL);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_W_CHG_MEM);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_W_CHG_MEM);

    //                   0 ===LP3_1866_intial_setting_shu1 begin===
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0x0, MISC_SPM_CTRL1_PHY_SPM_CTL1);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL0, 0xffffffff, MISC_SPM_CTRL0_PHY_SPM_CTL0);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1+(1<<POS_BANK_NUM), 0x0, MISC_SPM_CTRL1_PHY_SPM_CTL1);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL0+(1<<POS_BANK_NUM), 0xffffffff, MISC_SPM_CTRL0_PHY_SPM_CTL0);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2, 0xffffffff, MISC_SPM_CTRL2_PHY_SPM_CTL2);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2+(1<<POS_BANK_NUM), 0xffffffff, MISC_SPM_CTRL2_PHY_SPM_CTL2);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL2, 0x6003bf, MISC_CG_CTRL2_RG_MEM_DCM_CTL);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL4, 0x13300000, MISC_CG_CTRL4_R_PHY_MCK_CG_CTRL);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL4+(1<<POS_BANK_NUM), 0x222a2a00, MISC_CG_CTRL4_R_PHY_MCK_CG_CTRL);
    vIO32WriteFldMulti(DDRPHY_SHU1_PLL1, P_Fld(0x1, SHU1_PLL1_R_SHU_AUTO_PLL_MUX)
                | P_Fld(0x7, SHU1_PLL1_SHU1_PLL1_RFU));
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x1, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0)
                | P_Fld(0x1, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
                | P_Fld(0x0, SHU1_B0_DQ7_R_DMRANKRXDVS_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x1, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1)
                | P_Fld(0x1, SHU1_B1_DQ7_R_DMRANKRXDVS_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7+(1<<POS_BANK_NUM), P_Fld(0x1, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0)
                | P_Fld(0x1, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
                | P_Fld(0x1, SHU1_B0_DQ7_R_DMRANKRXDVS_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7+(1<<POS_BANK_NUM), P_Fld(0x1, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1)
                | P_Fld(0x1, SHU1_B1_DQ7_R_DMRANKRXDVS_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD7+(1<<POS_BANK_NUM), P_Fld(0x1, SHU1_CA_CMD7_R_DMRXDVS_PBYTE_FLAG_OPT_CA)
                | P_Fld(0x1, SHU1_CA_CMD7_R_DMRANKRXDVS_CA));
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL4, 0xfe, SHU1_PLL4_RG_RPHYPLL_RESERVED);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL6, 0xfe, SHU1_PLL6_RG_RCLRPLL_RESERVED);
    vIO32WriteFldMulti(DDRPHY_CA_CMD2, P_Fld(0x0, CA_CMD2_RG_TX_ARCMD_OE_DIS)
                | P_Fld(0x0, CA_CMD2_RG_TX_ARCMD_ODTEN_DIS)
                | P_Fld(0x0, CA_CMD2_RG_TX_ARCLK_OE_DIS)
                | P_Fld(0x0, CA_CMD2_RG_TX_ARCLK_ODTEN_DIS));
    vIO32WriteFldMulti(DDRPHY_B0_DQ2, P_Fld(0x0, B0_DQ2_RG_TX_ARDQ_OE_DIS_B0)
                | P_Fld(0x0, B0_DQ2_RG_TX_ARDQ_ODTEN_DIS_B0)
                | P_Fld(0x0, B0_DQ2_RG_TX_ARDQS0_OE_DIS)
                | P_Fld(0x0, B0_DQ2_RG_TX_ARDQS0_ODTEN_DIS));
    vIO32WriteFldMulti(DDRPHY_B1_DQ2, P_Fld(0x0, B1_DQ2_RG_TX_ARDQ_OE_DIS_B1)
                | P_Fld(0x0, B1_DQ2_RG_TX_ARDQ_ODTEN_DIS_B1)
                | P_Fld(0x0, B1_DQ2_RG_TX_ARDQS1_OE_DIS)
                | P_Fld(0x0, B1_DQ2_RG_TX_ARDQS1_ODTEN_DIS));
    vIO32WriteFldAlign(DDRPHY_MISC_RXDVS1, 0x7, MISC_RXDVS1_R_IN_GATE_EN_LOW_OPT);
    vIO32WriteFldMulti(DDRPHY_CA_CMD2+(1<<POS_BANK_NUM), P_Fld(0x0, CA_CMD2_RG_TX_ARCMD_OE_DIS)
                | P_Fld(0x0, CA_CMD2_RG_TX_ARCMD_ODTEN_DIS)
                | P_Fld(0x0, CA_CMD2_RG_TX_ARCLK_OE_DIS)
                | P_Fld(0x0, CA_CMD2_RG_TX_ARCLK_ODTEN_DIS));
    vIO32WriteFldMulti(DDRPHY_B0_DQ2+(1<<POS_BANK_NUM), P_Fld(0x0, B0_DQ2_RG_TX_ARDQ_OE_DIS_B0)
                | P_Fld(0x0, B0_DQ2_RG_TX_ARDQ_ODTEN_DIS_B0)
                | P_Fld(0x0, B0_DQ2_RG_TX_ARDQS0_OE_DIS)
                | P_Fld(0x0, B0_DQ2_RG_TX_ARDQS0_ODTEN_DIS));
    vIO32WriteFldMulti(DDRPHY_B1_DQ2+(1<<POS_BANK_NUM), P_Fld(0x0, B1_DQ2_RG_TX_ARDQ_OE_DIS_B1)
                | P_Fld(0x0, B1_DQ2_RG_TX_ARDQ_ODTEN_DIS_B1)
                | P_Fld(0x0, B1_DQ2_RG_TX_ARDQS1_OE_DIS)
                | P_Fld(0x0, B1_DQ2_RG_TX_ARDQS1_ODTEN_DIS));
    vIO32WriteFldAlign(DDRPHY_MISC_RXDVS1+(1<<POS_BANK_NUM), 0x7, MISC_RXDVS1_R_IN_GATE_EN_LOW_OPT);
    vIO32WriteFldMulti(DDRPHY_MISC_CTRL1, P_Fld(0x1, MISC_CTRL1_R_DMPINMUX)
                | P_Fld(0x0, MISC_CTRL1_R_DMARPIDQ_SW)
                | P_Fld(0x1, MISC_CTRL1_R_DMMCTLPLL_CKSEL));
    vIO32WriteFldMulti(DDRPHY_MISC_CTRL1+(1<<POS_BANK_NUM), P_Fld(0x1, MISC_CTRL1_R_DMPINMUX)
                | P_Fld(0x0, MISC_CTRL1_R_DMARPIDQ_SW)
                | P_Fld(0x1, MISC_CTRL1_R_DMMCTLPLL_CKSEL));

    vIO32WriteFldAlign(DDRPHY_PLL3, 0x0, PLL3_RG_RPHYPLL_TSTOP_EN);
    vIO32WriteFldMulti(DDRPHY_MISC_VREF_CTRL, P_Fld(0x1, MISC_VREF_CTRL_RG_RVREF_VREF_EN)
                | P_Fld(0x0, MISC_VREF_CTRL_RG_RVREF_SEL_CMD)
                | P_Fld(0x1, MISC_VREF_CTRL_RG_RVREF_DDR3_SEL)
                | P_Fld(0x0, MISC_VREF_CTRL_RG_RVREF_DDR4_SEL)
                | P_Fld(0xb, MISC_VREF_CTRL_RG_RVREF_SEL_DQ));
    vIO32WriteFldAlign(DDRPHY_MISC_IMP_CTRL0, 0x1, MISC_IMP_CTRL0_RG_RIMP_DDR3_SEL);
    vIO32WriteFldMulti(DDRPHY_MISC_VREF_CTRL+(1<<POS_BANK_NUM), P_Fld(0x1, MISC_VREF_CTRL_RG_RVREF_VREF_EN)
                | P_Fld(0x0, MISC_VREF_CTRL_RG_RVREF_SEL_CMD)
                | P_Fld(0x1, MISC_VREF_CTRL_RG_RVREF_DDR3_SEL)
                | P_Fld(0x0, MISC_VREF_CTRL_RG_RVREF_DDR4_SEL)
                | P_Fld(0xb, MISC_VREF_CTRL_RG_RVREF_SEL_DQ));
    vIO32WriteFldAlign(DDRPHY_MISC_IMP_CTRL0+(1<<POS_BANK_NUM), 0x1, MISC_IMP_CTRL0_RG_RIMP_DDR3_SEL);
    vIO32WriteFldAlign(DDRPHY_B1_RXDVS0, 0x1, B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1);
    vIO32WriteFldMulti(DDRPHY_B0_RXDVS0+(1<<POS_BANK_NUM), P_Fld(0x1, B0_RXDVS0_R_RX_DLY_TRACK_CG_EN_B0)
                | P_Fld(0x0, B0_RXDVS0_R_RX_RANKINCTL_B0));
    vIO32WriteFldMulti(DDRPHY_CA_RXDVS0+(1<<POS_BANK_NUM), P_Fld(0x1, CA_RXDVS0_R_RX_DLY_TRACK_CG_EN_CA)
                | P_Fld(0x0, CA_RXDVS0_R_RX_RANKINCTL_CA));
    vIO32WriteFldMulti(DDRPHY_B1_RXDVS0+(1<<POS_BANK_NUM), P_Fld(0x1, B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1)
                | P_Fld(0x0, B1_RXDVS0_R_RX_RANKINCTL_B1));
    vIO32WriteFldMulti(DDRPHY_B1_RXDVS0, P_Fld(0x1, B1_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B1)
                | P_Fld(0x1, B1_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B1));
    vIO32WriteFldMulti(DDRPHY_B0_RXDVS0+(1<<POS_BANK_NUM), P_Fld(0x1, B0_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B0)
                | P_Fld(0x1, B0_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B0));
    vIO32WriteFldMulti(DDRPHY_B1_RXDVS0+(1<<POS_BANK_NUM), P_Fld(0x1, B1_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B1)
                | P_Fld(0x1, B1_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B1));
    vIO32WriteFldMulti(DDRPHY_CA_RXDVS0+(1<<POS_BANK_NUM), P_Fld(0x1, CA_RXDVS0_R_DMRXDVS_CNTCMP_OPT_CA)
                | P_Fld(0x1, CA_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_CA));
    vIO32WriteFldAlign(DDRPHY_R0_B1_RXDVS2, 0x1, R0_B1_RXDVS2_R_RK0_DVS_FDLY_MODE_B1);
    vIO32WriteFldAlign(DDRPHY_R1_B1_RXDVS2, 0x1, R1_B1_RXDVS2_R_RK1_DVS_FDLY_MODE_B1);
    vIO32WriteFldAlign(DDRPHY_R0_B0_RXDVS2+(1<<POS_BANK_NUM), 0x1, R0_B0_RXDVS2_R_RK0_DVS_FDLY_MODE_B0);
    vIO32WriteFldAlign(DDRPHY_R1_B0_RXDVS2+(1<<POS_BANK_NUM), 0x1, R1_B0_RXDVS2_R_RK1_DVS_FDLY_MODE_B0);
    vIO32WriteFldAlign(DDRPHY_R0_CA_RXDVS2+(1<<POS_BANK_NUM), 0x1, R0_CA_RXDVS2_R_RK0_DVS_FDLY_MODE_CA);
    vIO32WriteFldAlign(DDRPHY_R1_CA_RXDVS2+(1<<POS_BANK_NUM), 0x1, R1_CA_RXDVS2_R_RK1_DVS_FDLY_MODE_CA);
    vIO32WriteFldAlign(DDRPHY_R0_B1_RXDVS2+(1<<POS_BANK_NUM), 0x1, R0_B1_RXDVS2_R_RK0_DVS_FDLY_MODE_B1);
    vIO32WriteFldAlign(DDRPHY_R1_B1_RXDVS2+(1<<POS_BANK_NUM), 0x1, R1_B1_RXDVS2_R_RK1_DVS_FDLY_MODE_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, 0x4, SHU1_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ5+(1<<POS_BANK_NUM), P_Fld(0x4, SHU1_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0)
                | P_Fld(0x0, SHU1_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD5+(1<<POS_BANK_NUM), P_Fld(0x4, SHU1_CA_CMD5_RG_RX_ARCLK_DVS_DLY)
                | P_Fld(0x0, SHU1_CA_CMD5_RG_RX_ARCMD_VREF_SEL));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ5+(1<<POS_BANK_NUM), P_Fld(0x4, SHU1_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1)
                | P_Fld(0x0, SHU1_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ2, P_Fld(0x12, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ3, P_Fld(0x12, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ4, P_Fld(0x12, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ5, P_Fld(0x12, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6, P_Fld(0x12, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ2, P_Fld(0x13, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ3, P_Fld(0x13, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ4, P_Fld(0x13, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ5, P_Fld(0x13, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6, P_Fld(0x13, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ2+(1<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ3+(1<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ4+(1<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ5+(1<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ6+(1<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ2+(1<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ3+(1<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ4+(1<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ5+(1<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ6+(1<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD2+(1<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA1_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA1_R_DLY)
                | P_Fld(0x12, SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA0_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA0_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD3+(1<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA3_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA3_R_DLY)
                | P_Fld(0x12, SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA2_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA2_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD4+(1<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA5_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA5_R_DLY)
                | P_Fld(0x12, SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA4_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA4_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD5+(1<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE1_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE1_R_DLY)
                | P_Fld(0x12, SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE0_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE0_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD6+(1<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_CA_CMD6_RG_RK0_RX_ARCS0_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD6_RG_RK0_RX_ARCS0_R_DLY)
                | P_Fld(0x12, SHU1_R0_CA_CMD6_RG_RK0_RX_ARCKE2_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD6_RG_RK0_RX_ARCKE2_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD7+(1<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS2_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS2_R_DLY)
                | P_Fld(0x12, SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS1_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS1_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD8+(1<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_CA_CMD8_RG_RK0_RX_ARCLK_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD8_RG_RK0_RX_ARCLK_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD2+(1<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_CA_CMD2_RG_RK1_RX_ARCA1_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD2_RG_RK1_RX_ARCA1_R_DLY)
                | P_Fld(0x13, SHU1_R1_CA_CMD2_RG_RK1_RX_ARCA0_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD2_RG_RK1_RX_ARCA0_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD3+(1<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_CA_CMD3_RG_RK1_RX_ARCA3_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD3_RG_RK1_RX_ARCA3_R_DLY)
                | P_Fld(0x13, SHU1_R1_CA_CMD3_RG_RK1_RX_ARCA2_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD3_RG_RK1_RX_ARCA2_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD4+(1<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_CA_CMD4_RG_RK1_RX_ARCA5_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD4_RG_RK1_RX_ARCA5_R_DLY)
                | P_Fld(0x13, SHU1_R1_CA_CMD4_RG_RK1_RX_ARCA4_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD4_RG_RK1_RX_ARCA4_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD5+(1<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_CA_CMD5_RG_RK1_RX_ARCKE1_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD5_RG_RK1_RX_ARCKE1_R_DLY)
                | P_Fld(0x13, SHU1_R1_CA_CMD5_RG_RK1_RX_ARCKE0_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD5_RG_RK1_RX_ARCKE0_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD6+(1<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_CA_CMD6_RG_RK1_RX_ARCS0_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD6_RG_RK1_RX_ARCS0_R_DLY)
                | P_Fld(0x13, SHU1_R1_CA_CMD6_RG_RK1_RX_ARCKE2_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD6_RG_RK1_RX_ARCKE2_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD7+(1<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_CA_CMD7_RG_RK1_RX_ARCS2_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD7_RG_RK1_RX_ARCS2_R_DLY)
                | P_Fld(0x13, SHU1_R1_CA_CMD7_RG_RK1_RX_ARCS1_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD7_RG_RK1_RX_ARCS1_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD8+(1<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_CA_CMD8_RG_RK1_RX_ARCLK_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD8_RG_RK1_RX_ARCLK_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ2+(1<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ3+(1<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ4+(1<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ5+(1<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6+(1<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ2+(1<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ3+(1<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ4+(1<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ5+(1<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6+(1<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS1, P_Fld(0x4, R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LEAD)
                | P_Fld(0x4, R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LAG));
    vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS1, P_Fld(0x4, R1_B1_RXDVS1_R_RK1_B1_DVS_TH_LEAD)
                | P_Fld(0x4, R1_B1_RXDVS1_R_RK1_B1_DVS_TH_LAG));
    vIO32WriteFldMulti(DDRPHY_R0_B0_RXDVS1+(1<<POS_BANK_NUM), P_Fld(0x4, R0_B0_RXDVS1_R_RK0_B0_DVS_TH_LEAD)
                | P_Fld(0x4, R0_B0_RXDVS1_R_RK0_B0_DVS_TH_LAG));
    vIO32WriteFldMulti(DDRPHY_R1_B0_RXDVS1+(1<<POS_BANK_NUM), P_Fld(0x4, R1_B0_RXDVS1_R_RK1_B0_DVS_TH_LEAD)
                | P_Fld(0x4, R1_B0_RXDVS1_R_RK1_B0_DVS_TH_LAG));
    vIO32WriteFldMulti(DDRPHY_R0_CA_RXDVS1+(1<<POS_BANK_NUM), P_Fld(0x4, R0_CA_RXDVS1_R_RK0_CA_DVS_TH_LEAD)
                | P_Fld(0x4, R0_CA_RXDVS1_R_RK0_CA_DVS_TH_LAG));
    vIO32WriteFldMulti(DDRPHY_R1_CA_RXDVS1+(1<<POS_BANK_NUM), P_Fld(0x4, R1_CA_RXDVS1_R_RK1_CA_DVS_TH_LEAD)
                | P_Fld(0x4, R1_CA_RXDVS1_R_RK1_CA_DVS_TH_LAG));
    vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS1+(1<<POS_BANK_NUM), P_Fld(0x4, R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LEAD)
                | P_Fld(0x4, R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LAG));
    vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS1+(1<<POS_BANK_NUM), P_Fld(0x4, R1_B1_RXDVS1_R_RK1_B1_DVS_TH_LEAD)
                | P_Fld(0x4, R1_B1_RXDVS1_R_RK1_B1_DVS_TH_LAG));

    vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS2, P_Fld(0x2, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1)
                | P_Fld(0x1, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                | P_Fld(0x1, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1));
    vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS2, P_Fld(0x2, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1)
                | P_Fld(0x1, R1_B1_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                | P_Fld(0x1, R1_B1_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B1));
    vIO32WriteFldMulti(DDRPHY_R0_B0_RXDVS2+(1<<POS_BANK_NUM), P_Fld(0x2, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0)
                | P_Fld(0x1, R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0)
                | P_Fld(0x1, R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0));
    vIO32WriteFldMulti(DDRPHY_R1_B0_RXDVS2+(1<<POS_BANK_NUM), P_Fld(0x2, R1_B0_RXDVS2_R_RK1_DVS_MODE_B0)
                | P_Fld(0x1, R1_B0_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B0)
                | P_Fld(0x1, R1_B0_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B0));
    vIO32WriteFldMulti(DDRPHY_R0_CA_RXDVS2+(1<<POS_BANK_NUM), P_Fld(0x2, R0_CA_RXDVS2_R_RK0_DVS_MODE_CA)
                | P_Fld(0x1, R0_CA_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_CA)
                | P_Fld(0x1, R0_CA_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_CA));
    vIO32WriteFldMulti(DDRPHY_R1_CA_RXDVS2+(1<<POS_BANK_NUM), P_Fld(0x2, R1_CA_RXDVS2_R_RK1_DVS_MODE_CA)
                | P_Fld(0x1, R1_CA_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_CA)
                | P_Fld(0x1, R1_CA_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_CA));
    vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS2+(1<<POS_BANK_NUM), P_Fld(0x2, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1)
                | P_Fld(0x1, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                | P_Fld(0x1, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1));
    vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS2+(1<<POS_BANK_NUM), P_Fld(0x2, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1)
                | P_Fld(0x1, R1_B1_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                | P_Fld(0x1, R1_B1_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B1));

    vIO32WriteFldAlign(DDRPHY_B1_RXDVS0, 0x0, B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1);
    vIO32WriteFldAlign(DDRPHY_B0_RXDVS0+(1<<POS_BANK_NUM), 0x0, B0_RXDVS0_R_RX_DLY_TRACK_CG_EN_B0);
    vIO32WriteFldAlign(DDRPHY_CA_RXDVS0+(1<<POS_BANK_NUM), 0x0, CA_RXDVS0_R_RX_DLY_TRACK_CG_EN_CA);
    vIO32WriteFldAlign(DDRPHY_B1_RXDVS0+(1<<POS_BANK_NUM), 0x0, B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1);
    vIO32WriteFldAlign(DDRPHY_B1_DQ3, 0x1, B1_DQ3_RG_RX_ARDQ_STBEN_RESETB_B1);
    vIO32WriteFldAlign(DDRPHY_B0_DQ3+(1<<POS_BANK_NUM), 0x1, B0_DQ3_RG_RX_ARDQ_STBEN_RESETB_B0);
    vIO32WriteFldAlign(DDRPHY_CA_CMD3+(1<<POS_BANK_NUM), 0x1, CA_CMD3_RG_RX_ARCMD_STBEN_RESETB);
    vIO32WriteFldAlign(DDRPHY_B1_DQ3+(1<<POS_BANK_NUM), 0x1, B1_DQ3_RG_RX_ARDQ_STBEN_RESETB_B1);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL1, 0x3f600, MISC_CG_CTRL1_R_DVS_DIV4_CG_CTRL);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL1+(1<<POS_BANK_NUM), 0xff7befdf, MISC_CG_CTRL1_R_DVS_DIV4_CG_CTRL);
    vIO32WriteFldAlign(DDRPHY_SHU1_R0_CA_CMD9, 0xc, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD);
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_CA_CMD9, 0xc, SHU1_R1_CA_CMD9_RG_RK1_ARPI_CMD);
    //vIO32WriteFldAlign(DDRPHY_SHU2_R0_CA_CMD9, 0xc, SHU2_R0_CA_CMD9_RG_RK0_ARPI_CMD);
    //vIO32WriteFldAlign(DDRPHY_SHU2_R1_CA_CMD9, 0xc, SHU2_R1_CA_CMD9_RG_RK1_ARPI_CMD);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0, P_Fld(0x6, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
                | P_Fld(0x6, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
                | P_Fld(0x6, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
                | P_Fld(0x6, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
                | P_Fld(0x6, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
                | P_Fld(0x6, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
                | P_Fld(0x6, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
                | P_Fld(0x6, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B1_DQ0, P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ7_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ6_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ5_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ4_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ3_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ2_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ1_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1, 0x6, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B1_DQ1, 0xb, SHU1_R2_B1_DQ1_RK2_TX_ARDQM0_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD0+(1<<POS_BANK_NUM), P_Fld(0x6, SHU1_R1_CA_CMD0_RK1_TX_ARCA5_DLY)
                | P_Fld(0x6, SHU1_R1_CA_CMD0_RK1_TX_ARCA4_DLY)
                | P_Fld(0x6, SHU1_R1_CA_CMD0_RK1_TX_ARCA3_DLY)
                | P_Fld(0x6, SHU1_R1_CA_CMD0_RK1_TX_ARCA2_DLY)
                | P_Fld(0x6, SHU1_R1_CA_CMD0_RK1_TX_ARCA1_DLY)
                | P_Fld(0x6, SHU1_R1_CA_CMD0_RK1_TX_ARCA0_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_CA_CMD0+(1<<POS_BANK_NUM), P_Fld(0xb, SHU1_R2_CA_CMD0_RK2_TX_ARCA5_DLY)
                | P_Fld(0xb, SHU1_R2_CA_CMD0_RK2_TX_ARCA4_DLY)
                | P_Fld(0xb, SHU1_R2_CA_CMD0_RK2_TX_ARCA3_DLY)
                | P_Fld(0xb, SHU1_R2_CA_CMD0_RK2_TX_ARCA2_DLY)
                | P_Fld(0xb, SHU1_R2_CA_CMD0_RK2_TX_ARCA1_DLY)
                | P_Fld(0xb, SHU1_R2_CA_CMD0_RK2_TX_ARCA0_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD1+(1<<POS_BANK_NUM), P_Fld(0x6, SHU1_R1_CA_CMD1_RK1_TX_ARCKE2_DLY)
                | P_Fld(0x6, SHU1_R1_CA_CMD1_RK1_TX_ARCKE1_DLY)
                | P_Fld(0x6, SHU1_R1_CA_CMD1_RK1_TX_ARCKE0_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_CA_CMD1+(1<<POS_BANK_NUM), P_Fld(0xb, SHU1_R2_CA_CMD1_RK2_TX_ARCKE2_DLY)
                | P_Fld(0xb, SHU1_R2_CA_CMD1_RK2_TX_ARCKE1_DLY)
                | P_Fld(0xb, SHU1_R2_CA_CMD1_RK2_TX_ARCKE0_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ0+(1<<POS_BANK_NUM), P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B0_DQ0+(1<<POS_BANK_NUM), P_Fld(0xb, SHU1_R2_B0_DQ0_RK2_TX_ARDQ7_DLY_B0)
                | P_Fld(0xb, SHU1_R2_B0_DQ0_RK2_TX_ARDQ6_DLY_B0)
                | P_Fld(0xb, SHU1_R2_B0_DQ0_RK2_TX_ARDQ5_DLY_B0)
                | P_Fld(0xb, SHU1_R2_B0_DQ0_RK2_TX_ARDQ4_DLY_B0)
                | P_Fld(0xb, SHU1_R2_B0_DQ0_RK2_TX_ARDQ3_DLY_B0)
                | P_Fld(0xb, SHU1_R2_B0_DQ0_RK2_TX_ARDQ2_DLY_B0)
                | P_Fld(0xb, SHU1_R2_B0_DQ0_RK2_TX_ARDQ1_DLY_B0)
                | P_Fld(0xb, SHU1_R2_B0_DQ0_RK2_TX_ARDQ0_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B0_DQ1+(1<<POS_BANK_NUM), 0x6, SHU1_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B0_DQ1+(1<<POS_BANK_NUM), 0xb, SHU1_R2_B0_DQ1_RK2_TX_ARDQM0_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0+(1<<POS_BANK_NUM), P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B1_DQ0+(1<<POS_BANK_NUM), P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ7_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ6_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ5_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ4_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ3_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ2_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ1_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1+(1<<POS_BANK_NUM), 0x6, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B1_DQ1+(1<<POS_BANK_NUM), 0xb, SHU1_R2_B1_DQ1_RK2_TX_ARDQM0_DLY_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_R0_B0_DQ7, 0xc, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B0_DQ7, 0xc, SHU1_R1_B0_DQ7_RK1_ARPI_DQ_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B0_DQ7, 0xc, SHU1_R2_B0_DQ7_RK2_ARPI_DQ_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ7, P_Fld(0xf, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1)
                | P_Fld(0xf, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ7, P_Fld(0xf, SHU1_R1_B1_DQ7_RK1_ARPI_DQM_B1)
                | P_Fld(0xf, SHU1_R1_B1_DQ7_RK1_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B1_DQ7, P_Fld(0xf, SHU1_R2_B1_DQ7_RK2_ARPI_DQM_B1)
                | P_Fld(0xf, SHU1_R2_B1_DQ7_RK2_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD9+(1<<POS_BANK_NUM), P_Fld(0xf, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD)
                | P_Fld(0xf, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD9+(1<<POS_BANK_NUM), P_Fld(0xf, SHU1_R1_CA_CMD9_RG_RK1_ARPI_CMD)
                | P_Fld(0xf, SHU1_R1_CA_CMD9_RG_RK1_ARPI_CS));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_CA_CMD9+(1<<POS_BANK_NUM), P_Fld(0xf, SHU1_R2_CA_CMD9_RG_RK2_ARPI_CMD)
                | P_Fld(0xf, SHU1_R2_CA_CMD9_RG_RK2_ARPI_CS));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ7+(1<<POS_BANK_NUM), P_Fld(0xf, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0)
                | P_Fld(0xf, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ7+(1<<POS_BANK_NUM), P_Fld(0xf, SHU1_R1_B0_DQ7_RK1_ARPI_DQM_B0)
                | P_Fld(0xf, SHU1_R1_B0_DQ7_RK1_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B0_DQ7+(1<<POS_BANK_NUM), P_Fld(0xf, SHU1_R2_B0_DQ7_RK2_ARPI_DQM_B0)
                | P_Fld(0xf, SHU1_R2_B0_DQ7_RK2_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ7+(1<<POS_BANK_NUM), P_Fld(0xf, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1)
                | P_Fld(0xf, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ7+(1<<POS_BANK_NUM), P_Fld(0xf, SHU1_R1_B1_DQ7_RK1_ARPI_DQM_B1)
                | P_Fld(0xf, SHU1_R1_B1_DQ7_RK1_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B1_DQ7+(1<<POS_BANK_NUM), P_Fld(0xf, SHU1_R2_B1_DQ7_RK2_ARPI_DQM_B1)
                | P_Fld(0xf, SHU1_R2_B1_DQ7_RK2_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_B1_DQ4, P_Fld(0x10, B1_DQ4_RG_RX_ARDQS_EYE_R_DLY_B1)
                | P_Fld(0x10, B1_DQ4_RG_RX_ARDQS_EYE_F_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_B1_DQ5, P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_EN_B1)
                | P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_SEL_B1)
                | P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1)
                | P_Fld(0x10, B1_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B1)
                | P_Fld(0x10, B1_DQ5_B1_DQ5_RFU));
    vIO32WriteFldMulti(DDRPHY_B1_DQ6, P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B1)
                | P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1)
                | P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1)
                | P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1));
    vIO32WriteFldMulti(DDRPHY_B1_DQ5, P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B1)
                | P_Fld(0x0, B1_DQ5_B1_DQ5_RFU));
    vIO32WriteFldMulti(DDRPHY_CA_CMD3, P_Fld(0x1, CA_CMD3_RG_RX_ARCMD_IN_BUFF_EN)
                | P_Fld(0x1, CA_CMD3_RG_ARCMD_RESETB)
                | P_Fld(0x1, CA_CMD3_RG_TX_ARCMD_EN));
    vIO32WriteFldMulti(DDRPHY_CA_CMD6, P_Fld(0x1, CA_CMD6_RG_TX_ARCMD_DDR3_SEL)
                | P_Fld(0x0, CA_CMD6_RG_TX_ARCMD_DDR4_SEL)
                | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_DDR4_SEL)
                | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_BIAS_VREF_SEL)
                | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN)
                | P_Fld(0x1, CA_CMD6_RG_TX_ARCMD_SER_MODE));
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD1, P_Fld(0x1, SHU1_CA_CMD1_RG_TX_ARCMD_DRVN)
                | P_Fld(0x1, SHU1_CA_CMD1_RG_TX_ARCMD_DRVP));
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD2, P_Fld(0x1, SHU1_CA_CMD2_RG_TX_ARCLK_DRVN)
                | P_Fld(0x1, SHU1_CA_CMD2_RG_TX_ARCLK_DRVP));
    //vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD1, P_Fld(0x1, SHU2_CA_CMD1_RG_TX_ARCMD_DRVN)
    //            | P_Fld(0x1, SHU2_CA_CMD1_RG_TX_ARCMD_DRVP));
    //vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD2, P_Fld(0x1, SHU2_CA_CMD2_RG_TX_ARCLK_DRVN)
    //            | P_Fld(0x1, SHU2_CA_CMD2_RG_TX_ARCLK_DRVP));
    vIO32WriteFldMulti(DDRPHY_B0_DQ3, P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B0)
                | P_Fld(0x1, B0_DQ3_RG_ARDQ_RESETB_B0)
                | P_Fld(0x1, B0_DQ3_RG_TX_ARDQ_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B0_DQ6, P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_DDR3_SEL_B0)
                | P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0)
                | P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ1, P_Fld(0x1, SHU1_B0_DQ1_RG_TX_ARDQ_DRVN_B0)
                | P_Fld(0x1, SHU1_B0_DQ1_RG_TX_ARDQ_DRVP_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ2, P_Fld(0x1, SHU1_B0_DQ2_RG_TX_ARDQS0_DRVN_B0)
                | P_Fld(0x1, SHU1_B0_DQ2_RG_TX_ARDQS0_DRVP_B0));
    //vIO32WriteFldMulti(DDRPHY_SHU2_B0_DQ1, P_Fld(0x1, SHU2_B0_DQ1_RG_TX_ARDQ_DRVN_B0)
    //           | P_Fld(0x1, SHU2_B0_DQ1_RG_TX_ARDQ_DRVP_B0));
    //vIO32WriteFldMulti(DDRPHY_SHU2_B0_DQ2, P_Fld(0x1, SHU2_B0_DQ2_RG_TX_ARDQS0_DRVN_B0)
    //            | P_Fld(0x1, SHU2_B0_DQ2_RG_TX_ARDQS0_DRVP_B0));
    vIO32WriteFldMulti(DDRPHY_CA_CMD4+(1<<POS_BANK_NUM), P_Fld(0x10, CA_CMD4_RG_RX_ARCLK_EYE_R_DLY)
                | P_Fld(0x10, CA_CMD4_RG_RX_ARCLK_EYE_F_DLY));
    vIO32WriteFldMulti(DDRPHY_CA_CMD5+(1<<POS_BANK_NUM), P_Fld(0x1, CA_CMD5_RG_RX_ARCMD_EYE_EN)
                | P_Fld(0x1, CA_CMD5_RG_RX_ARCMD_EYE_SEL)
                | P_Fld(0x1, CA_CMD5_RG_RX_ARCMD_VREF_EN)
                | P_Fld(0x10, CA_CMD5_RG_RX_ARCMD_EYE_VREF_SEL)
                | P_Fld(0x10, CA_CMD5_CA_CMD5_RFU));
    vIO32WriteFldMulti(DDRPHY_CA_CMD6+(1<<POS_BANK_NUM), P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_EYE_DLY_DQS_BYPASS)
                | P_Fld(0x1, CA_CMD6_RG_TX_ARCMD_DDR3_SEL)
                | P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_DDR3_SEL)
                | P_Fld(0x0, CA_CMD6_RG_TX_ARCMD_DDR4_SEL)
                | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_DDR4_SEL)
                | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_BIAS_VREF_SEL)
                | P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_BIAS_EN)
                | P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_OP_BIAS_SW_EN)
                | P_Fld(0x1, CA_CMD6_RG_TX_ARCMD_SER_MODE));
    vIO32WriteFldMulti(DDRPHY_CA_CMD5+(1<<POS_BANK_NUM), P_Fld(0x1, CA_CMD5_RG_RX_ARCMD_EYE_STBEN_RESETB)
                | P_Fld(0x0, CA_CMD5_CA_CMD5_RFU));
    vIO32WriteFldMulti(DDRPHY_B0_DQ4+(1<<POS_BANK_NUM), P_Fld(0x10, B0_DQ4_RG_RX_ARDQS_EYE_R_DLY_B0)
                | P_Fld(0x10, B0_DQ4_RG_RX_ARDQS_EYE_F_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_B0_DQ5+(1<<POS_BANK_NUM), P_Fld(0x1, B0_DQ5_RG_RX_ARDQ_EYE_EN_B0)
                | P_Fld(0x1, B0_DQ5_RG_RX_ARDQ_EYE_SEL_B0)
                | P_Fld(0x1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0)
                | P_Fld(0x10, B0_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B0)
                | P_Fld(0x10, B0_DQ5_B0_DQ5_RFU));
    vIO32WriteFldMulti(DDRPHY_B0_DQ6+(1<<POS_BANK_NUM), P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B0)
                | P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0)
                | P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0)
                | P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B0_DQ5+(1<<POS_BANK_NUM), P_Fld(0x1, B0_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B0)
                | P_Fld(0x0, B0_DQ5_B0_DQ5_RFU));
    vIO32WriteFldMulti(DDRPHY_B1_DQ4+(1<<POS_BANK_NUM), P_Fld(0x10, B1_DQ4_RG_RX_ARDQS_EYE_R_DLY_B1)
                | P_Fld(0x10, B1_DQ4_RG_RX_ARDQS_EYE_F_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_B1_DQ5+(1<<POS_BANK_NUM), P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_EN_B1)
                | P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_SEL_B1)
                | P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1)
                | P_Fld(0x10, B1_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B1)
                | P_Fld(0x10, B1_DQ5_B1_DQ5_RFU));
    vIO32WriteFldMulti(DDRPHY_B1_DQ6+(1<<POS_BANK_NUM), P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B1)
                | P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1)
                | P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1)
                | P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1));
    vIO32WriteFldMulti(DDRPHY_B1_DQ5+(1<<POS_BANK_NUM), P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B1)
                | P_Fld(0x0, B1_DQ5_B1_DQ5_RFU));
    if(p->frequency==933)
    {
        vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, 0x4700, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
        vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, 0x4700, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
    }
    else if(p->frequency==667)
    {
        vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, 0x3300, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
        vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, 0x3300, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
    }
    else if(p->frequency==467)
    {
        vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, 0x4700>>1, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
        vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, 0x4700>>1, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
    }
    vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI2, P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
    vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI2+(1<<POS_BANK_NUM), P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DLL_ARPI2, P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
    vIO32WriteFldMulti(DDRPHY_B1_DLL_ARPI2+(1<<POS_BANK_NUM), P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
    vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI2, P_Fld(0x1, CA_DLL_ARPI2_RG_ARRPI_CG_FB_CA)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));
    vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI2+(1<<POS_BANK_NUM), P_Fld(0x1, CA_DLL_ARPI2_RG_ARRPI_CG_FB_CA)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));

    mcDELAY_US(1);

    vIO32WriteFldMulti(DDRPHY_PLL4, P_Fld(0x0, PLL4_RG_RPHYPLL_AD_MCK8X_EN)
                | P_Fld(0x1, PLL4_PLL4_RFU)
                | P_Fld(0x1, PLL4_RG_RPHYPLL_MCK8X_SEL));


#if 0//ORIGINAL_PLL_INIT
    vIO32WriteFldAlign(DDRPHY_PLL1, 0x1, PLL1_RG_RPHYPLL_EN);
    mcDELAY_US(20);

    ///TODO: MIDPI
    vIO32WriteFldMulti(DDRPHY_PLL4, P_Fld(0x1, PLL4_RG_RPHYPLL_ADA_MCK8X_EN)
                | P_Fld(0x1, PLL4_RG_RPHYPLL_RESETB));
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI0, 0x1, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI0, 0x1, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI0, 0x1, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI0+(1<<POS_BANK_NUM), 0x1, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI0+(1<<POS_BANK_NUM), 0x1, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI0+(1<<POS_BANK_NUM), 0x1, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);

    mcDELAY_US(1);

    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ6, 0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU2_B0_DQ6, 0x1, SHU2_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ6, 0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU2_B1_DQ6, 0x1, SHU2_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6, 0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA);
    //vIO32WriteFldAlign(DDRPHY_SHU2_CA_CMD6, 0x1, SHU2_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA);
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ6+(1<<POS_BANK_NUM), 0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU2_B0_DQ6+(1<<POS_BANK_NUM), 0x1, SHU2_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ6+(1<<POS_BANK_NUM), 0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU2_B1_DQ6+(1<<POS_BANK_NUM), 0x1, SHU2_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6+(1<<POS_BANK_NUM), 0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA);
    //vIO32WriteFldAlign(DDRPHY_SHU2_CA_CMD6+(1<<POS_BANK_NUM), 0x1, SHU2_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL8, 0x0, SHU1_PLL8_RG_RPHYPLL_POSDIV);
    //vIO32WriteFldAlign(DDRPHY_SHU2_PLL8, 0x0, SHU2_PLL8_RG_RPHYPLL_POSDIV);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL10, 0x0, SHU1_PLL10_RG_RCLRPLL_POSDIV);
    //vIO32WriteFldAlign(DDRPHY_SHU2_PLL10, 0x0, SHU2_PLL10_RG_RCLRPLL_POSDIV);
#endif
    vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI1, P_Fld(0x1, CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA)
                | P_Fld(0x1, CA_DLL_ARPI1_RG_ARPI_CS_JUMP_EN)
                | P_Fld(0x1, CA_DLL_ARPI1_RG_ARPI_CLK_JUMP_EN));
    vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI1, P_Fld(0x1, B0_DLL_ARPI1_RG_ARPISM_MCK_SEL_B0)
                | P_Fld(0x1, B0_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B0));
    vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI1, 0x1, B1_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B1);
    vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI2, P_Fld(0x0, CA_DLL_ARPI2_RG_ARRPI_CG_FB_CA)
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));
    vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI2, P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DLL_ARPI2, P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
    vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI3, P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA)
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_FB_EN_CA)
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CS_EN)
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CLK_EN)
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CMD_EN));
    vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI3, P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_FB_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQS_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQM_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQ_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DLL_ARPI3, P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_FB_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQS_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQM_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQ_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B1));
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI1+(1<<POS_BANK_NUM), 0x1, CA_DLL_ARPI1_RG_ARPI_CMD_JUMP_EN);
    vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI1+(1<<POS_BANK_NUM), 0x1, B0_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI1+(1<<POS_BANK_NUM), 0x1, B1_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B1);
    vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI2+(1<<POS_BANK_NUM), P_Fld(0x0, CA_DLL_ARPI2_RG_ARRPI_CG_FB_CA)
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));
    vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI2+(1<<POS_BANK_NUM), P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DLL_ARPI2+(1<<POS_BANK_NUM), P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
    vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI3+(1<<POS_BANK_NUM), P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_FB_EN_CA)
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CS_EN)
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CLK_EN)
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CMD_EN)
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CLKIEN_EN));
    vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI3+(1<<POS_BANK_NUM), P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_FB_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQS_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQM_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQ_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DLL_ARPI3+(1<<POS_BANK_NUM), P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_FB_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQS_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQM_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQ_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B1));
    vIO32WriteFldMulti(DDRPHY_B1_DQ3, P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_STBENCMP_EN_B1)
                | P_Fld(0x1, B1_DQ3_RG_ARDQ_RESETB_B1)
                | P_Fld(0x1, B1_DQ3_RG_TX_ARDQ_EN_B1)
                | P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1));
    vIO32WriteFldMulti(DDRPHY_CA_CMD3+(1<<POS_BANK_NUM), P_Fld(0x1, CA_CMD3_RG_RX_ARCMD_STBENCMP_EN)
                | P_Fld(0x1, CA_CMD3_RG_ARCMD_RESETB)
                | P_Fld(0x1, CA_CMD3_RG_TX_ARCMD_EN)
                | P_Fld(0x1, CA_CMD3_RG_RX_ARCMD_SMT_EN));
    vIO32WriteFldMulti(DDRPHY_B0_DQ3+(1<<POS_BANK_NUM), P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_STBENCMP_EN_B0)
                | P_Fld(0x1, B0_DQ3_RG_ARDQ_RESETB_B0)
                | P_Fld(0x1, B0_DQ3_RG_TX_ARDQ_EN_B0)
                | P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DQ3+(1<<POS_BANK_NUM), P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_STBENCMP_EN_B1)
                | P_Fld(0x1, B1_DQ3_RG_ARDQ_RESETB_B1)
                | P_Fld(0x1, B1_DQ3_RG_TX_ARDQ_EN_B1)
                | P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1));
    vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI5, P_Fld(0x9, CA_DLL_ARPI5_RG_ARDLL_IDLECNT_CA)
                | P_Fld(0x8, CA_DLL_ARPI5_RG_ARDLL_P_GAIN_CA)
                | P_Fld(0x1, CA_DLL_ARPI5_RG_ARDLL_PHJUMP_EN_CA)
                | P_Fld(0x1, CA_DLL_ARPI5_RG_ARDLL_PHDIV_CA)
                | P_Fld(0x0, CA_DLL_ARPI5_RG_ARDLL_DIV_MCTL_CA));
    vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI5, P_Fld(0x1, B0_DLL_ARPI5_RG_ARDLL_PHDET_IN_SWAP_B0)
                | P_Fld(0x7, B0_DLL_ARPI5_RG_ARDLL_GAIN_B0)
                | P_Fld(0x7, B0_DLL_ARPI5_RG_ARDLL_IDLECNT_B0)
                | P_Fld(0x8, B0_DLL_ARPI5_RG_ARDLL_P_GAIN_B0)
                | P_Fld(0x1, B0_DLL_ARPI5_RG_ARDLL_PHJUMP_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI5_RG_ARDLL_PHDIV_B0)
                | P_Fld(0x0, B0_DLL_ARPI5_RG_ARDLL_DIV_MCTL_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DLL_ARPI5, P_Fld(0x1, B1_DLL_ARPI5_RG_ARDLL_PHDET_IN_SWAP_B1)
                | P_Fld(0x7, B1_DLL_ARPI5_RG_ARDLL_GAIN_B1)
                | P_Fld(0x7, B1_DLL_ARPI5_RG_ARDLL_IDLECNT_B1)
                | P_Fld(0x8, B1_DLL_ARPI5_RG_ARDLL_P_GAIN_B1)
                | P_Fld(0x1, B1_DLL_ARPI5_RG_ARDLL_PHJUMP_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI5_RG_ARDLL_PHDIV_B1)
                | P_Fld(0x0, B1_DLL_ARPI5_RG_ARDLL_DIV_MCTL_B1));
    vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI5+(1<<POS_BANK_NUM), P_Fld(0x1, CA_DLL_ARPI5_RG_ARDLL_PHDET_OUT_SEL_CA)
                | P_Fld(0x1, CA_DLL_ARPI5_RG_ARDLL_PHDET_IN_SWAP_CA)
                | P_Fld(0x7, CA_DLL_ARPI5_RG_ARDLL_GAIN_CA)
                | P_Fld(0x7, CA_DLL_ARPI5_RG_ARDLL_IDLECNT_CA)
                | P_Fld(0x8, CA_DLL_ARPI5_RG_ARDLL_P_GAIN_CA)
                | P_Fld(0x1, CA_DLL_ARPI5_RG_ARDLL_PHJUMP_EN_CA)
                | P_Fld(0x1, CA_DLL_ARPI5_RG_ARDLL_PHDIV_CA)
                | P_Fld(0x0, CA_DLL_ARPI5_RG_ARDLL_DIV_MCTL_CA));
    vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI5+(1<<POS_BANK_NUM), P_Fld(0x1, B0_DLL_ARPI5_RG_ARDLL_PHDET_IN_SWAP_B0)
                | P_Fld(0x7, B0_DLL_ARPI5_RG_ARDLL_GAIN_B0)
                | P_Fld(0x7, B0_DLL_ARPI5_RG_ARDLL_IDLECNT_B0)
                | P_Fld(0x8, B0_DLL_ARPI5_RG_ARDLL_P_GAIN_B0)
                | P_Fld(0x1, B0_DLL_ARPI5_RG_ARDLL_PHJUMP_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI5_RG_ARDLL_PHDIV_B0)
                | P_Fld(0x0, B0_DLL_ARPI5_RG_ARDLL_DIV_MCTL_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DLL_ARPI5+(1<<POS_BANK_NUM), P_Fld(0x1, B1_DLL_ARPI5_RG_ARDLL_PHDET_IN_SWAP_B1)
                | P_Fld(0x7, B1_DLL_ARPI5_RG_ARDLL_GAIN_B1)
                | P_Fld(0x7, B1_DLL_ARPI5_RG_ARDLL_IDLECNT_B1)
                | P_Fld(0x8, B1_DLL_ARPI5_RG_ARDLL_P_GAIN_B1)
                | P_Fld(0x1, B1_DLL_ARPI5_RG_ARDLL_PHJUMP_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI5_RG_ARDLL_PHDIV_B1)
                | P_Fld(0x0, B1_DLL_ARPI5_RG_ARDLL_DIV_MCTL_B1));
    vIO32WriteFldMulti(DDRPHY_CA_CMD8, P_Fld(0x1, CA_CMD8_RG_TX_RRESETB_DDR3_SEL)
                | P_Fld(0x1, CA_CMD8_RG_RRESETB_DRVN)
                | P_Fld(0x1, CA_CMD8_RG_RRESETB_DRVP));
    vIO32WriteFldMulti(DDRPHY_CA_CMD8+(1<<POS_BANK_NUM), P_Fld(0x1, CA_CMD8_RG_TX_RRESETB_DDR3_SEL)
                | P_Fld(0x1, CA_CMD8_RG_RRESETB_DRVN)
                | P_Fld(0x1, CA_CMD8_RG_RRESETB_DRVP));

    mcDELAY_US(1);

    vIO32WriteFldAlign(DDRPHY_SHU1_PLL0, 0x3, SHU1_PLL0_RG_RPHYPLL_TOP_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU2_PLL0, 0x3, SHU2_PLL0_RG_RPHYPLL_TOP_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU3_PLL0, 0x3, SHU3_PLL0_RG_RPHYPLL_TOP_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU4_PLL0, 0x3, SHU4_PLL0_RG_RPHYPLL_TOP_REV);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL0+(1<<POS_BANK_NUM), 0x3, SHU1_PLL0_RG_RPHYPLL_TOP_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU2_PLL0+(1<<POS_BANK_NUM), 0x3, SHU2_PLL0_RG_RPHYPLL_TOP_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU3_PLL0+(1<<POS_BANK_NUM), 0x3, SHU3_PLL0_RG_RPHYPLL_TOP_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU4_PLL0+(1<<POS_BANK_NUM), 0x3, SHU4_PLL0_RG_RPHYPLL_TOP_REV);
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD0, P_Fld(0x1, SHU1_CA_CMD0_RG_TX_ARCMD_PRE_EN)
                | P_Fld(0x7, SHU1_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
                | P_Fld(0x1, SHU1_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    //vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD0, P_Fld(0x1, SHU2_CA_CMD0_RG_TX_ARCMD_PRE_EN)
    //            | P_Fld(0x7, SHU2_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
    //            | P_Fld(0x1, SHU2_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    //vIO32WriteFldMulti(DDRPHY_SHU3_CA_CMD0, P_Fld(0x1, SHU3_CA_CMD0_RG_TX_ARCMD_PRE_EN)
    //            | P_Fld(0x7, SHU3_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
    //            | P_Fld(0x1, SHU3_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    //vIO32WriteFldMulti(DDRPHY_SHU4_CA_CMD0, P_Fld(0x1, SHU4_CA_CMD0_RG_TX_ARCMD_PRE_EN)
    //            | P_Fld(0x7, SHU4_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
    //            | P_Fld(0x1, SHU4_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD0+(1<<POS_BANK_NUM), P_Fld(0x1, SHU1_CA_CMD0_RG_TX_ARCMD_PRE_EN)
                | P_Fld(0x7, SHU1_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
                | P_Fld(0x1, SHU1_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    //vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD0+(1<<POS_BANK_NUM), P_Fld(0x1, SHU2_CA_CMD0_RG_TX_ARCMD_PRE_EN)
    //            | P_Fld(0x7, SHU2_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
    //            | P_Fld(0x1, SHU2_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    //vIO32WriteFldMulti(DDRPHY_SHU3_CA_CMD0+(1<<POS_BANK_NUM), P_Fld(0x1, SHU3_CA_CMD0_RG_TX_ARCMD_PRE_EN)
    //            | P_Fld(0x7, SHU3_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
    //            | P_Fld(0x1, SHU3_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    //vIO32WriteFldMulti(DDRPHY_SHU4_CA_CMD0+(1<<POS_BANK_NUM), P_Fld(0x1, SHU4_CA_CMD0_RG_TX_ARCMD_PRE_EN)
    //            | P_Fld(0x7, SHU4_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
    //            | P_Fld(0x1, SHU4_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6, 0x3, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA);
    //vIO32WriteFldAlign(DDRPHY_SHU2_CA_CMD6, 0x3, SHU2_CA_CMD6_RG_ARPI_RESERVE_CA);
    //vIO32WriteFldAlign(DDRPHY_SHU3_CA_CMD6, 0x3, SHU3_CA_CMD6_RG_ARPI_RESERVE_CA);
    //vIO32WriteFldAlign(DDRPHY_SHU4_CA_CMD6, 0x3, SHU4_CA_CMD6_RG_ARPI_RESERVE_CA);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6+(1<<POS_BANK_NUM), 0x1, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA);
    //vIO32WriteFldAlign(DDRPHY_SHU2_CA_CMD6+(1<<POS_BANK_NUM), 0x1, SHU2_CA_CMD6_RG_ARPI_RESERVE_CA);
    //vIO32WriteFldAlign(DDRPHY_SHU3_CA_CMD6+(1<<POS_BANK_NUM), 0x1, SHU3_CA_CMD6_RG_ARPI_RESERVE_CA);
    //vIO32WriteFldAlign(DDRPHY_SHU4_CA_CMD6+(1<<POS_BANK_NUM), 0x1, SHU4_CA_CMD6_RG_ARPI_RESERVE_CA);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD7, 0x7e2, SHU1_CA_CMD7_RG_ARCMD_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU2_CA_CMD7, 0x7e2, SHU2_CA_CMD7_RG_ARCMD_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU3_CA_CMD7, 0x7e2, SHU3_CA_CMD7_RG_ARCMD_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU4_CA_CMD7, 0x7e2, SHU4_CA_CMD7_RG_ARCMD_REV);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD7+(1<<POS_BANK_NUM), 0x720, SHU1_CA_CMD7_RG_ARCMD_REV);
    //vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD7+(1<<POS_BANK_NUM), P_Fld(0x720, SHU2_CA_CMD7_RG_ARCMD_REV)
    //            | P_Fld(0x0, SHU2_CA_CMD7_R_DMRANKRXDVS_CA));
    //vIO32WriteFldMulti(DDRPHY_SHU3_CA_CMD7+(1<<POS_BANK_NUM), P_Fld(0x720, SHU3_CA_CMD7_RG_ARCMD_REV)
    //            | P_Fld(0x0, SHU3_CA_CMD7_R_DMRANKRXDVS_CA));
    //vIO32WriteFldMulti(DDRPHY_SHU4_CA_CMD7+(1<<POS_BANK_NUM), P_Fld(0x720, SHU4_CA_CMD7_RG_ARCMD_REV)
    //            | P_Fld(0x0, SHU4_CA_CMD7_R_DMRANKRXDVS_CA));
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x20, SHU1_B0_DQ7_RG_ARDQ_REV_B0)
                | P_Fld(0x1, SHU1_B0_DQ7_DQ_REV_B0_BIT_05));
    //vIO32WriteFldAlign(DDRPHY_SHU2_B0_DQ7, 0x20, SHU2_B0_DQ7_RG_ARDQ_REV_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU3_B0_DQ7, 0x20, SHU3_B0_DQ7_RG_ARDQ_REV_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU4_B0_DQ7, 0x20, SHU4_B0_DQ7_RG_ARDQ_REV_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7+(1<<POS_BANK_NUM), P_Fld(0x20, SHU1_B0_DQ7_RG_ARDQ_REV_B0)
                | P_Fld(0x1, SHU1_B0_DQ7_DQ_REV_B0_BIT_05));
    //vIO32WriteFldMulti(DDRPHY_SHU2_B0_DQ7+(1<<POS_BANK_NUM), P_Fld(0x20, SHU2_B0_DQ7_RG_ARDQ_REV_B0)
    //            | P_Fld(0x0, SHU2_B0_DQ7_R_DMRANKRXDVS_B0));
    //vIO32WriteFldMulti(DDRPHY_SHU3_B0_DQ7+(1<<POS_BANK_NUM), P_Fld(0x20, SHU3_B0_DQ7_RG_ARDQ_REV_B0)
    //            | P_Fld(0x0, SHU3_B0_DQ7_R_DMRANKRXDVS_B0));
    //vIO32WriteFldMulti(DDRPHY_SHU4_B0_DQ7+(1<<POS_BANK_NUM), P_Fld(0x20, SHU4_B0_DQ7_RG_ARDQ_REV_B0)
    //            | P_Fld(0x0, SHU4_B0_DQ7_R_DMRANKRXDVS_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x20, SHU1_B1_DQ7_RG_ARDQ_REV_B1)
                | P_Fld(0x1, SHU1_B1_DQ7_DQ_REV_B1_BIT_05));
    //vIO32WriteFldAlign(DDRPHY_SHU2_B1_DQ7, 0x20, SHU2_B1_DQ7_RG_ARDQ_REV_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU3_B1_DQ7, 0x20, SHU3_B1_DQ7_RG_ARDQ_REV_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU4_B1_DQ7, 0x20, SHU4_B1_DQ7_RG_ARDQ_REV_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7+(1<<POS_BANK_NUM), P_Fld(0x20, SHU1_B1_DQ7_RG_ARDQ_REV_B1)
                | P_Fld(0x1, SHU1_B1_DQ7_DQ_REV_B1_BIT_05));
    //vIO32WriteFldMulti(DDRPHY_SHU2_B1_DQ7+(1<<POS_BANK_NUM), P_Fld(0x20, SHU2_B1_DQ7_RG_ARDQ_REV_B1)
    //            | P_Fld(0x0, SHU2_B1_DQ7_R_DMRANKRXDVS_B1));
    //vIO32WriteFldMulti(DDRPHY_SHU3_B1_DQ7+(1<<POS_BANK_NUM), P_Fld(0x20, SHU3_B1_DQ7_RG_ARDQ_REV_B1)
    //            | P_Fld(0x0, SHU3_B1_DQ7_R_DMRANKRXDVS_B1));
    //vIO32WriteFldMulti(DDRPHY_SHU4_B1_DQ7+(1<<POS_BANK_NUM), P_Fld(0x20, SHU4_B1_DQ7_RG_ARDQ_REV_B1)
    //            | P_Fld(0x0, SHU4_B1_DQ7_R_DMRANKRXDVS_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ6, 0x1, SHU1_B0_DQ6_RG_ARPI_RESERVE_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU2_B0_DQ6, 0x1, SHU2_B0_DQ6_RG_ARPI_RESERVE_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU3_B0_DQ6, 0x1, SHU3_B0_DQ6_RG_ARPI_RESERVE_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU4_B0_DQ6, 0x1, SHU4_B0_DQ6_RG_ARPI_RESERVE_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ6+(1<<POS_BANK_NUM), 0x1, SHU1_B0_DQ6_RG_ARPI_RESERVE_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU2_B0_DQ6+(1<<POS_BANK_NUM), 0x1, SHU2_B0_DQ6_RG_ARPI_RESERVE_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU3_B0_DQ6+(1<<POS_BANK_NUM), 0x1, SHU3_B0_DQ6_RG_ARPI_RESERVE_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU4_B0_DQ6+(1<<POS_BANK_NUM), 0x1, SHU4_B0_DQ6_RG_ARPI_RESERVE_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ6, 0x1, SHU1_B1_DQ6_RG_ARPI_RESERVE_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU2_B1_DQ6, 0x1, SHU2_B1_DQ6_RG_ARPI_RESERVE_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU3_B1_DQ6, 0x1, SHU3_B1_DQ6_RG_ARPI_RESERVE_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU4_B1_DQ6, 0x1, SHU4_B1_DQ6_RG_ARPI_RESERVE_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ6+(1<<POS_BANK_NUM), 0x1, SHU1_B1_DQ6_RG_ARPI_RESERVE_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU2_B1_DQ6+(1<<POS_BANK_NUM), 0x1, SHU2_B1_DQ6_RG_ARPI_RESERVE_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU3_B1_DQ6+(1<<POS_BANK_NUM), 0x1, SHU3_B1_DQ6_RG_ARPI_RESERVE_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU4_B1_DQ6+(1<<POS_BANK_NUM), 0x1, SHU4_B1_DQ6_RG_ARPI_RESERVE_B1);
    vIO32WriteFldMulti(DDRPHY_MISC_SHU_OPT, P_Fld(0x1, MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN)
                | P_Fld(0x1, MISC_SHU_OPT_R_CA_SHU_PHY_GATING_RESETB_SPM_EN)
                | P_Fld(0x2, MISC_SHU_OPT_R_DQB1_SHU_PHDET_SPM_EN)
                | P_Fld(0x1, MISC_SHU_OPT_R_DQB1_SHU_PHY_GATING_RESETB_SPM_EN)
                | P_Fld(0x2, MISC_SHU_OPT_R_DQB0_SHU_PHDET_SPM_EN)
                | P_Fld(0x1, MISC_SHU_OPT_R_DQB0_SHU_PHY_GATING_RESETB_SPM_EN));
    vIO32WriteFldMulti(DDRPHY_MISC_SHU_OPT+(1<<POS_BANK_NUM), P_Fld(0x2, MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN)
                | P_Fld(0x1, MISC_SHU_OPT_R_CA_SHU_PHY_GATING_RESETB_SPM_EN)
                | P_Fld(0x2, MISC_SHU_OPT_R_DQB1_SHU_PHDET_SPM_EN)
                | P_Fld(0x1, MISC_SHU_OPT_R_DQB1_SHU_PHY_GATING_RESETB_SPM_EN)
                | P_Fld(0x2, MISC_SHU_OPT_R_DQB0_SHU_PHDET_SPM_EN)
                | P_Fld(0x1, MISC_SHU_OPT_R_DQB0_SHU_PHY_GATING_RESETB_SPM_EN));

    mcDELAY_US(1);

    vIO32WriteFldMulti(DDRPHY_SHU1_PLL9, P_Fld(0x1, SHU1_PLL9_RG_RPHYPLL_MONCK_EN)
                | P_Fld(0x2, SHU1_PLL9_RG_RPHYPLL_MONVC_EN)
                | P_Fld(0x1, SHU1_PLL9_RG_RPHYPLL_LVROD_EN)
                | P_Fld(0x2, SHU1_PLL9_RG_RPHYPLL_RST_DLY));

    mcDELAY_US(1);


#if 0//ORIGINAL_PLL_INIT
    ///TODO: PLL_EN
    vIO32WriteFldAlign(DDRPHY_PLL1, 0x1, PLL1_RG_RPHYPLL_EN);
    vIO32WriteFldAlign(DDRPHY_PLL2, 0x1, PLL2_RG_RCLRPLL_EN);

    mcDELAY_US(100);
#endif

    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI0, 0x1, CA_DLL_ARPI0_RG_ARDLL_PD_CK_SEL_CA);
    vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI0, 0x1, B0_DLL_ARPI0_RG_ARDLL_FASTPJ_CK_SEL_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI0, 0x1, B1_DLL_ARPI0_RG_ARDLL_FASTPJ_CK_SEL_B1);
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI0+(1<<POS_BANK_NUM), 0x1, CA_DLL_ARPI0_RG_ARDLL_FASTPJ_CK_SEL_CA);
    vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI0+(1<<POS_BANK_NUM), 0x1, B0_DLL_ARPI0_RG_ARDLL_FASTPJ_CK_SEL_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI0+(1<<POS_BANK_NUM), 0x1, B1_DLL_ARPI0_RG_ARDLL_FASTPJ_CK_SEL_B1);

    mcDELAY_US(1);


    //vIO32WriteFldAlign(DDRPHY_PLL2, 0x0, PLL2_RG_RCLRPLL_EN);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL4, 0xff, SHU1_PLL4_RG_RPHYPLL_RESERVED);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL6, 0xff, SHU1_PLL6_RG_RCLRPLL_RESERVED);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0+(1<<POS_BANK_NUM), 0x100000, MISC_CG_CTRL0_CLK_MEM_DFS_CFG);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0xf, MISC_CG_CTRL0_CLK_MEM_DFS_CFG);
#if 0//ORIGINAL_PLL_INIT
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_CLK_MEM_SEL);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_W_CHG_MEM);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_W_CHG_MEM);
#endif

    mcDELAY_US(1);

    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI5, 0x1, CA_DLL_ARPI5_RG_ARDLL_PHDET_EN_CA);

    mcDELAY_US(1);

#if 0//ORIGINAL_PLL_INIT
    ///TODO: DLL_EN
    vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI5, 0x1, B0_DLL_ARPI5_RG_ARDLL_PHDET_EN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI5, 0x1, B1_DLL_ARPI5_RG_ARDLL_PHDET_EN_B1);
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI5+(1<<POS_BANK_NUM), 0x1, CA_DLL_ARPI5_RG_ARDLL_PHDET_EN_CA);
    vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI5+(1<<POS_BANK_NUM), 0x1, B0_DLL_ARPI5_RG_ARDLL_PHDET_EN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI5+(1<<POS_BANK_NUM), 0x1, B1_DLL_ARPI5_RG_ARDLL_PHDET_EN_B1);

    mcDELAY_US(2);
#endif

#if !ORIGINAL_PLL_INIT
    DDRPhyPLLSetting(p);
#endif


    vIO32WriteFldAlign(DDRPHY_B1_DQ3, 0x1, B1_DQ3_RG_RX_ARDQS1_STBEN_RESETB);
    vIO32WriteFldAlign(DDRPHY_CA_CMD3+(1<<POS_BANK_NUM), 0x1, CA_CMD3_RG_RX_ARCLK_STBEN_RESETB);
    vIO32WriteFldAlign(DDRPHY_B0_DQ3+(1<<POS_BANK_NUM), 0x1, B0_DQ3_RG_RX_ARDQS0_STBEN_RESETB);
    vIO32WriteFldAlign(DDRPHY_B1_DQ3+(1<<POS_BANK_NUM), 0x1, B1_DQ3_RG_RX_ARDQS1_STBEN_RESETB);
    vIO32WriteFldMulti(DDRPHY_MISC_CTRL1, P_Fld(0x1, MISC_CTRL1_R_DMDA_RRESETB_E)
                | P_Fld(0x1, MISC_CTRL1_R_HWSAVE_MODE_ENA)
                | P_Fld(0x1, MISC_CTRL1_R_DMDQSIENCG_EN)
                | P_Fld(0x1, MISC_CTRL1_R_DMARPIDQ_SW)
                | P_Fld(0x1, MISC_CTRL1_R_DM_TX_ARCMD_OE)
                | P_Fld(0x1, MISC_CTRL1_R_DM_TX_ARCLK_OE)
                | P_Fld(0x1, MISC_CTRL1_R_DMDQMDBI));
    vIO32WriteFldMulti(DDRPHY_MISC_CTRL1+(1<<POS_BANK_NUM), P_Fld(0x1, MISC_CTRL1_R_DMDA_RRESETB_E)
                | P_Fld(0x1, MISC_CTRL1_R_HWSAVE_MODE_ENA)
                | P_Fld(0x1, MISC_CTRL1_R_DMDQSIENCG_EN)
                | P_Fld(0x1, MISC_CTRL1_R_DMARPIDQ_SW)
                | P_Fld(0x1, MISC_CTRL1_R_DM_TX_ARCMD_OE)
                | P_Fld(0x1, MISC_CTRL1_R_DM_TX_ARCLK_OE)
                | P_Fld(0x1, MISC_CTRL1_R_DMDQMDBI));
    vIO32WriteFldMulti(DDRPHY_CA_CMD8, P_Fld(0x0, CA_CMD8_RG_TX_RRESETB_PULL_DN)
                | P_Fld(0x0, CA_CMD8_RG_TX_RRESETB_DDR3_SEL)
                | P_Fld(0x1, CA_CMD8_RG_TX_RRESETB_DDR4_SEL));
    vIO32WriteFldAlign(DDRPHY_CA_CMD7, 0x0, CA_CMD7_RG_TX_ARCMD_PULL_DN);
    vIO32WriteFldAlign(DDRPHY_B0_DQ7, 0x0, B0_DQ7_RG_TX_ARDQ_PULL_DN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ7, 0x0, B1_DQ7_RG_TX_ARDQ_PULL_DN_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD7+(1<<POS_BANK_NUM), 0x0, CA_CMD7_RG_TX_ARCMD_PULL_DN);
    vIO32WriteFldAlign(DDRPHY_B0_DQ7+(1<<POS_BANK_NUM), 0x0, B0_DQ7_RG_TX_ARDQ_PULL_DN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ7+(1<<POS_BANK_NUM), 0x0, B1_DQ7_RG_TX_ARDQ_PULL_DN_B1);
    vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x1, SHU_DQSG_RETRY_R_RDY_SEL_DLE)
                | P_Fld(0x6, SHU_DQSG_RETRY_R_DQSIENLAT)
                | P_Fld(0x1, SHU_DQSG_RETRY_R_RETRY_ONCE));
    vIO32WriteFldMulti(DRAMC_REG_SHUCTRL2, P_Fld(0x0, SHUCTRL2_HWSET_WLRL)
                | P_Fld(0x1, SHUCTRL2_SHU_PERIOD_GO_ZERO_CNT)
                | P_Fld(0x1, SHUCTRL2_R_DVFS_OPTION)
                | P_Fld(0x1, SHUCTRL2_R_DVFS_PARK_N)
                | P_Fld(0x1, SHUCTRL2_R_DVFS_DLL_CHA)
                | P_Fld(0xa, SHUCTRL2_R_DLL_IDLE));
    vIO32WriteFldMulti(DRAMC_REG_SHUCTRL2+(1<<POS_BANK_NUM), P_Fld(0x0, SHUCTRL2_HWSET_WLRL)
                | P_Fld(0x1, SHUCTRL2_SHU_PERIOD_GO_ZERO_CNT)
                | P_Fld(0x1, SHUCTRL2_R_DVFS_OPTION)
                | P_Fld(0x1, SHUCTRL2_R_DVFS_PARK_N)
                | P_Fld(0xa, SHUCTRL2_R_DLL_IDLE));
    vIO32WriteFldMulti(DRAMC_REG_DVFSDLL, P_Fld(0x12, DVFSDLL_DLL_IDLE_SHU4)
                | P_Fld(0xc, DVFSDLL_DLL_IDLE_SHU3)
                | P_Fld(0x9, DVFSDLL_DLL_IDLE_SHU2)
                | P_Fld(0x1, DVFSDLL_DLL_LOCK_SHU_EN));
    vIO32WriteFldMulti(DRAMC_REG_DDRCONF0, P_Fld(0x1, DDRCONF0_LPDDR3EN)
                | P_Fld(0x1, DDRCONF0_DM64BITEN)
                | P_Fld(0x1, DDRCONF0_BK8EN));
    vIO32WriteFldMulti(DRAMC_REG_EYESCAN, P_Fld(0x1, EYESCAN_STB_GERR_B01)
                | P_Fld(0x1, EYESCAN_STB_GERRSTOP));
    vIO32WriteFldAlign(DRAMC_REG_EYESCAN, 0x1, EYESCAN_STB_GERR_RST);
    vIO32WriteFldAlign(DRAMC_REG_EYESCAN, 0x0, EYESCAN_STB_GERR_RST);
    vIO32WriteFldAlign(DRAMC_REG_SHU1_WODT, 0x0, SHU1_WODT_WPST2T);
    vIO32WriteFldAlign(DRAMC_REG_CLKAR, 0x1, CLKAR_PSELAR);
    vIO32WriteFldMulti(DDRPHY_MISC_CTRL0, P_Fld(0x0, MISC_CTRL0_R_DQS0IEN_DIV4_CK_CG_CTRL)
                | P_Fld(0x1, MISC_CTRL0_R_DMDQSIEN_FIFO_EN)
                | P_Fld(0x1, MISC_CTRL0_R_DMSTBEN_OUTSEL)
                | P_Fld(0xf, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));
    vIO32WriteFldMulti(DDRPHY_MISC_CTRL0+(1<<POS_BANK_NUM), P_Fld(0x1, MISC_CTRL0_R_CLKIEN_DIV4_CK_CG_CTRL)
                | P_Fld(0x1, MISC_CTRL0_R_DMDQSIEN_FIFO_EN)
                | P_Fld(0x1, MISC_CTRL0_R_DMSTBEN_OUTSEL)
                | P_Fld(0xf, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));
    vIO32WriteFldMulti(DRAMC_REG_SHU_CONF0, P_Fld(0x2, SHU_CONF0_MATYPE)
                | P_Fld(0x1, SHU_CONF0_BL4)
                | P_Fld(0x1, SHU_CONF0_FDIV2)
                | P_Fld(0x1, SHU_CONF0_REFTHD)
                | P_Fld(0x1, SHU_CONF0_ADVPREEN)
                | P_Fld(0x3f, SHU_CONF0_DMPGTIM));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x3, SHU_ODTCTRL_TWODT)
                | P_Fld(0x5, SHU_ODTCTRL_RODT)
                | P_Fld(0x1, SHU_ODTCTRL_WOEN));
    vIO32WriteFldAlign(DRAMC_REG_REFCTRL0, 0x5, REFCTRL0_REF_PREGATE_CNT);
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA1, P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CS1)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_RAS)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CAS)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_WE)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_RESET)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_ODT)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CKE)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CS));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA2, P_Fld(0x0, SHU_SELPH_CA2_TXDLY_CKE1)
                | P_Fld(0x0, SHU_SELPH_CA2_TXDLY_CMD)
                | P_Fld(0x0, SHU_SELPH_CA2_TXDLY_BA2)
                | P_Fld(0x0, SHU_SELPH_CA2_TXDLY_BA1)
                | P_Fld(0x0, SHU_SELPH_CA2_TXDLY_BA0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA3, P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA7)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA6)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA5)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA4)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA3)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA2)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA1)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA4, P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA15)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA14)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA13)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA12)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA11)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA10)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA9)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA8));
    vIO32WriteFldAlign(DRAMC_REG_SHU_SELPH_CA5, 0x0, SHU_SELPH_CA5_DLY_ODT);
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS0, P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS3)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS2)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS1)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS0)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS3)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS2)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS1)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS1, P_Fld(0x2, SHU_SELPH_DQS1_DLY_OEN_DQS3)
                | P_Fld(0x2, SHU_SELPH_DQS1_DLY_OEN_DQS2)
                | P_Fld(0x2, SHU_SELPH_DQS1_DLY_OEN_DQS1)
                | P_Fld(0x2, SHU_SELPH_DQS1_DLY_OEN_DQS0)
                | P_Fld(0x3, SHU_SELPH_DQS1_DLY_DQS3)
                | P_Fld(0x3, SHU_SELPH_DQS1_DLY_DQS2)
                | P_Fld(0x3, SHU_SELPH_DQS1_DLY_DQS1)
                | P_Fld(0x3, SHU_SELPH_DQS1_DLY_DQS0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ2)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM2)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
                | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
                | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
                | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_OEN_DQ0)
                | P_Fld(0x3, SHURK0_SELPH_DQ2_DLY_DQ3)
                | P_Fld(0x3, SHURK0_SELPH_DQ2_DLY_DQ2)
                | P_Fld(0x3, SHURK0_SELPH_DQ2_DLY_DQ1)
                | P_Fld(0x3, SHURK0_SELPH_DQ2_DLY_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
                | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
                | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
                | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_OEN_DQM0)
                | P_Fld(0x3, SHURK0_SELPH_DQ3_DLY_DQM3)
                | P_Fld(0x3, SHURK0_SELPH_DQ3_DLY_DQM2)
                | P_Fld(0x3, SHURK0_SELPH_DQ3_DLY_DQM1)
                | P_Fld(0x3, SHURK0_SELPH_DQ3_DLY_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
                | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
                | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
                | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0)
                | P_Fld(0x3, SHURK1_SELPH_DQ2_DLY_R1DQ3)
                | P_Fld(0x3, SHURK1_SELPH_DQ2_DLY_R1DQ2)
                | P_Fld(0x3, SHURK1_SELPH_DQ2_DLY_R1DQ1)
                | P_Fld(0x3, SHURK1_SELPH_DQ2_DLY_R1DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
                | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
                | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
                | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0)
                | P_Fld(0x3, SHURK1_SELPH_DQ3_DLY_R1DQM3)
                | P_Fld(0x3, SHURK1_SELPH_DQ3_DLY_R1DQM2)
                | P_Fld(0x3, SHURK1_SELPH_DQ3_DLY_R1DQM1)
                | P_Fld(0x3, SHURK1_SELPH_DQ3_DLY_R1DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ0, P_Fld(0x2, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ3)
                | P_Fld(0x2, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ2)
                | P_Fld(0x2, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ1)
                | P_Fld(0x2, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ0)
                | P_Fld(0x2, SHURK2_SELPH_DQ0_TX_DLY_R2DQ3)
                | P_Fld(0x2, SHURK2_SELPH_DQ0_TX_DLY_R2DQ2)
                | P_Fld(0x2, SHURK2_SELPH_DQ0_TX_DLY_R2DQ1)
                | P_Fld(0x2, SHURK2_SELPH_DQ0_TX_DLY_R2DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ1, P_Fld(0x2, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM3)
                | P_Fld(0x2, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM2)
                | P_Fld(0x2, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM1)
                | P_Fld(0x2, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM0)
                | P_Fld(0x2, SHURK2_SELPH_DQ1_TX_DLY_R2DQM3)
                | P_Fld(0x2, SHURK2_SELPH_DQ1_TX_DLY_R2DQM2)
                | P_Fld(0x2, SHURK2_SELPH_DQ1_TX_DLY_R2DQM1)
                | P_Fld(0x2, SHURK2_SELPH_DQ1_TX_DLY_R2DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ2, P_Fld(0x6, SHURK2_SELPH_DQ2_DLY_R2OEN_DQ3)
                | P_Fld(0x6, SHURK2_SELPH_DQ2_DLY_R2OEN_DQ2)
                | P_Fld(0x6, SHURK2_SELPH_DQ2_DLY_R2OEN_DQ1)
                | P_Fld(0x6, SHURK2_SELPH_DQ2_DLY_R2OEN_DQ0)
                | P_Fld(0x7, SHURK2_SELPH_DQ2_DLY_R2DQ3)
                | P_Fld(0x7, SHURK2_SELPH_DQ2_DLY_R2DQ2)
                | P_Fld(0x7, SHURK2_SELPH_DQ2_DLY_R2DQ1)
                | P_Fld(0x7, SHURK2_SELPH_DQ2_DLY_R2DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ3, P_Fld(0x6, SHURK2_SELPH_DQ3_DLY_R2OEN_DQM3)
                | P_Fld(0x6, SHURK2_SELPH_DQ3_DLY_R2OEN_DQM2)
                | P_Fld(0x6, SHURK2_SELPH_DQ3_DLY_R2OEN_DQM1)
                | P_Fld(0x6, SHURK2_SELPH_DQ3_DLY_R2OEN_DQM0)
                | P_Fld(0x7, SHURK2_SELPH_DQ3_DLY_R2DQM3)
                | P_Fld(0x7, SHURK2_SELPH_DQ3_DLY_R2DQM2)
                | P_Fld(0x7, SHURK2_SELPH_DQ3_DLY_R2DQM1)
                | P_Fld(0x7, SHURK2_SELPH_DQ3_DLY_R2DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN0, P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B3_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B3_RODTEN)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B2_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B2_RODTEN)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x4, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1)
                | P_Fld(0x4, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN)
                | P_Fld(0x4, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1)
                | P_Fld(0x4, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN)
                | P_Fld(0x4, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
                | P_Fld(0x4, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
                | P_Fld(0x4, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
                | P_Fld(0x4, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN)
                | P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN_P1)
                | P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN)
                | P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
                | P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
                | P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
                | P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_ODTEN0, P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B3_R2RODTEN_P1)
                | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B3_R2RODTEN)
                | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B2_R2RODTEN_P1)
                | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B2_R2RODTEN)
                | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B1_R2RODTEN_P1)
                | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B1_R2RODTEN)
                | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B0_R2RODTEN_P1)
                | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B0_R2RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_ODTEN1, P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B3_R2RODTEN_P1)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B3_R2RODTEN)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B2_R2RODTEN_P1)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B2_R2RODTEN)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B1_R2RODTEN_P1)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B1_R2RODTEN)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B0_R2RODTEN_P1)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B0_R2RODTEN));

    mcDELAY_US(1);

    vIO32WriteFldMulti(DRAMC_REG_STBCAL, P_Fld(0x0, STBCAL_DQSIENMODE_SELPH)
                | P_Fld(0x1, STBCAL_STB_DQIEN_IG)
                | P_Fld(0x1, STBCAL_PICHGBLOCK_NORD)
                | P_Fld(0x1, STBCAL_PIMASK_RKCHG_OPT));
    vIO32WriteFldMulti(DRAMC_REG_SHU1_DQSG, P_Fld(0x9, SHU1_DQSG_STB_UPDMASKCYC)
                | P_Fld(0x1, SHU1_DQSG_STB_UPDMASK_EN));
    vIO32WriteFldAlign(DRAMC_REG_STBCAL, 0x1, STBCAL_DQSIENMODE);
    vIO32WriteFldMulti(DRAMC_REG_SREFCTRL, P_Fld(0x0, SREFCTRL_SREF_HW_EN)
                | P_Fld(0x8, SREFCTRL_SREFDLY)
                | P_Fld(0x0, SREFCTRL_SREF2_OPTION));
    vIO32WriteFldMulti(DRAMC_REG_SHU_PIPE, P_Fld(0x1, SHU_PIPE_READ_START_EXTEND1)
                | P_Fld(0x1, SHU_PIPE_DLE_LAST_EXTEND1));
    vIO32WriteFldMulti(DRAMC_REG_CKECTRL, P_Fld(0x1, CKECTRL_CKEON)
                | P_Fld(0x1, CKECTRL_CKETIMER_SEL));
    vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0x1, RKCFG_CKE2RANK_OPT2);
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM0, P_Fld(0xa, SHU_ACTIM0_TRCD)
                | P_Fld(0x3, SHU_ACTIM0_TRRD)
                | P_Fld(0xd, SHU_ACTIM0_TWR)
                | P_Fld(0x7, SHU_ACTIM0_TWTR));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM1, P_Fld(0x17, SHU_ACTIM1_TRC)
                | P_Fld(0xb, SHU_ACTIM1_TRAS)
                | P_Fld(0x9, SHU_ACTIM1_TRP)
                | P_Fld(0x1, SHU_ACTIM1_TRPAB));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM2, P_Fld(0xe, SHU_ACTIM2_TFAW)
                | P_Fld(0x6, SHU_ACTIM2_TR2W)
                | P_Fld(0x2, SHU_ACTIM2_TRTP));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM3, P_Fld(0x56, SHU_ACTIM3_TRFC)
                | P_Fld(0x1f, SHU_ACTIM3_TRFCPB));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM5, P_Fld(0x1, SHU_ACTIM5_TMRR2W)
                | P_Fld(0xb, SHU_ACTIM5_TWTPD)
                | P_Fld(0xa, SHU_ACTIM5_TR2PD));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x8, SHU_ACTIM_XRT_XRTW2W)
                | P_Fld(0x4, SHU_ACTIM_XRT_XRTR2W)
                | P_Fld(0xa, SHU_ACTIM_XRT_XRTR2R));
    vIO32WriteFldMulti(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(0x1, SHU_AC_TIME_05T_TFAW_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TRPAB_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TRP_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TRCD_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TRTP_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TXP_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TRC_05T));
    vIO32WriteFldMulti(DRAMC_REG_SHU_CONF2, P_Fld(0x7, SHU_CONF2_DCMDLYREF)
                | P_Fld(0x32, SHU_CONF2_FSPCHG_PRDCNT));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SCINTV, P_Fld(0x0, SHU_SCINTV_DQS2DQ_SHU_PITHRD)
                | P_Fld(0x15, SHU_SCINTV_MRW_INTV)
                | P_Fld(0x0, SHU_SCINTV_RDDQC_INTV)
                | P_Fld(0x0, SHU_SCINTV_TZQLAT));
    vIO32WriteFldMulti(DRAMC_REG_SHU_CONF1, P_Fld(0xc, SHU_CONF1_DATLAT_DSEL_PHY)
                | P_Fld(0xd, SHU_CONF1_DATLAT_DSEL)
                | P_Fld(0xe, SHU_CONF1_DATLAT));
    vIO32WriteFldAlign(DRAMC_REG_SHUCTRL, 0x1, SHUCTRL_LPSM_BYPASS_B);
    vIO32WriteFldAlign(DRAMC_REG_REFCTRL1, 0x0, REFCTRL1_SREF_PRD_OPT);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL4, 0xfe, SHU1_PLL4_RG_RPHYPLL_RESERVED);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL6, 0xfe, SHU1_PLL6_RG_RCLRPLL_RESERVED);
    vIO32WriteFldAlign(DRAMC_REG_REFRATRE_FILTER, 0x1, REFRATRE_FILTER_PB2AB_OPT);
    vIO32WriteFldAlign(DRAMC_REG_DDRCONF0, 0x1, DDRCONF0_GDDR3RST);
    vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x0, DRAMCTRL_CLKWITRFC);
    vIO32WriteFldMulti(DRAMC_REG_MISCTL0, P_Fld(0x1, MISCTL0_REFP_ARB_EN2)
                | P_Fld(0x1, MISCTL0_PBC_ARB_EN)
                | P_Fld(0x1, MISCTL0_REFA_ARB_EN2));
    vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_MWHPRIEN)
                | P_Fld(0x1, PERFCTL0_RWSPLIT)
                | P_Fld(0x1, PERFCTL0_WFLUSHEN)
                | P_Fld(0x1, PERFCTL0_EMILLATEN)
                | P_Fld(0x1, PERFCTL0_RWAGEEN)
                | P_Fld(0x1, PERFCTL0_RWLLATEN)
                | P_Fld(0x1, PERFCTL0_RWHPRIEN)
                | P_Fld(0x1, PERFCTL0_RWOFOEN)
                | P_Fld(0x1, PERFCTL0_DISRDPHASE1)
                | P_Fld(0x1, PERFCTL0_DUALSCHEN));
    vIO32WriteFldAlign(DRAMC_REG_ARBCTL, 0x80, ARBCTL_MAXPENDCNT);
    vIO32WriteFldMulti(DRAMC_REG_PADCTRL, P_Fld(0x1, PADCTRL_DQIENLATEBEGIN)
                | P_Fld(0x1, PADCTRL_DQIENQKEND));
    vIO32WriteFldAlign(DRAMC_REG_DRAMC_PD_CTRL, 0x1, DRAMC_PD_CTRL_DCMREF_OPT);
    vIO32WriteFldAlign(DRAMC_REG_CLKCTRL, 0x1, CLKCTRL_CLK_EN_1);
    vIO32WriteFldMulti(DRAMC_REG_REFCTRL0, P_Fld(0x4, REFCTRL0_DISBYREFNUM)
                | P_Fld(0x1, REFCTRL0_DLLFRZ));
    vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_CLR_EN);
    vIO32WriteFldMulti(DRAMC_REG_CATRAINING1, P_Fld(0x4, CATRAINING1_CATRAIN_INTV)
                | P_Fld(0x3, CATRAINING1_CATRAINLAT));
    vIO32WriteFldAlign(DRAMC_REG_STBCAL, 0x1, STBCAL_REFUICHG);
    vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x5, SHU_RANKCTL_RANKINCTL_PHY)
                | P_Fld(0x2, SHU_RANKCTL_RANKINCTL_ROOT1)
                | P_Fld(0x2, SHU_RANKCTL_RANKINCTL)
                | P_Fld(0x1, SHU_RANKCTL_DMSTBLAT));
    vIO32WriteFldAlign(DRAMC_REG_SHURK0_DQSCTL, 0x4, SHURK0_DQSCTL_DQSINCTL);
    vIO32WriteFldAlign(DRAMC_REG_SHURK1_DQSCTL, 0x4, SHURK1_DQSCTL_R1DQSINCTL);
    vIO32WriteFldAlign(DRAMC_REG_SHURK2_DQSCTL, 0x4, SHURK2_DQSCTL_R2DQSINCTL);

    mcDELAY_US(2);

    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED)
                | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED)
                | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED)
                | P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED)
                | P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                | P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED_P1)
                | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED)
                | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED_P1)
                | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED)
                | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x5, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED_P1)
                | P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED)
                | P_Fld(0x5, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED_P1)
                | P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED)
                | P_Fld(0x5, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                | P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                | P_Fld(0x5, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                | P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQSG0, P_Fld(0x3, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS3_GATED_P1)
                | P_Fld(0x2, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS3_GATED)
                | P_Fld(0x3, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS2_GATED_P1)
                | P_Fld(0x2, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS2_GATED)
                | P_Fld(0x3, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED_P1)
                | P_Fld(0x2, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED)
                | P_Fld(0x3, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED_P1)
                | P_Fld(0x2, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQSG1, P_Fld(0x6, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS3_GATED)
                | P_Fld(0x6, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS2_GATED)
                | P_Fld(0x6, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS1_GATED)
                | P_Fld(0x6, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0+(1<<POS_BANK_NUM), P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED)
                | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED)
                | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1+(1<<POS_BANK_NUM), P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED)
                | P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED)
                | P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                | P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0+(1<<POS_BANK_NUM), P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED_P1)
                | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED)
                | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED_P1)
                | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED)
                | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1+(1<<POS_BANK_NUM), P_Fld(0x5, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED_P1)
                | P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED)
                | P_Fld(0x5, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED_P1)
                | P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED)
                | P_Fld(0x5, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                | P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                | P_Fld(0x5, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                | P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQSG0+(1<<POS_BANK_NUM), P_Fld(0x3, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS3_GATED_P1)
                | P_Fld(0x2, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS3_GATED)
                | P_Fld(0x3, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS2_GATED_P1)
                | P_Fld(0x2, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS2_GATED)
                | P_Fld(0x3, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED_P1)
                | P_Fld(0x2, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED)
                | P_Fld(0x3, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED_P1)
                | P_Fld(0x2, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQSG1+(1<<POS_BANK_NUM), P_Fld(0x6, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS3_GATED)
                | P_Fld(0x6, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS2_GATED)
                | P_Fld(0x6, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS1_GATED)
                | P_Fld(0x6, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS0_GATED));
    //               70127 === over_write_setting_begin ===
    //               70127 === over_write_setting_end ===


    ///TODO: overwrite 1866 setting START
    vIO32WriteFldMulti(DRAMC_REG_SHU1_WODT, P_Fld(0x1, SHU1_WODT_DISWODTE)
                    | P_Fld(0x1, SHU1_WODT_WODTFIXOFF)
                    | P_Fld(0x4, SHU1_WODT_DISWODT));

    //vIO32WriteFldAlign(DRAMC_REG_SHU_CONF3, 0x5, SHU_CONF3_ZQCSCNT);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_CONF3, 0xff, SHU_CONF3_REFRCNT);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_CONF1, 0xb0, SHU_CONF1_REFBW_FR);
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM4, P_Fld(0x29, SHU_ACTIM4_TZQCS)
                | P_Fld(0x65, SHU_ACTIM4_REFCNT_FR_CLK)
                | P_Fld(0x70, SHU_ACTIM4_REFCNT)
                | P_Fld(0x65, SHU_ACTIM4_TXREFCNT));
    vIO32WriteFldAlign(DRAMC_REG_SHU_RANKCTL, 0x1, SHU_RANKCTL_DQSG_MODE);
    vIO32WriteFldMulti(DRAMC_REG_SHU_CONF0, P_Fld(0x5, SHU_CONF0_REFTHD)
                | P_Fld(0x15, SHU_CONF0_DMPGTIM));
    //vIO32WriteFldAlign(DRAMC_REG_SHU_CONF1, 0xae, SHU_CONF1_REFBW_FR);
    //vIO32WriteFldMulti(DRAMC_REG_SHU_CONF3, P_Fld(0x8d, SHU_CONF3_REFRCNT)
    //            | P_Fld(0x1f8, SHU_CONF3_ZQCSCNT));
    vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x1, SHU_RANKCTL_PICGLAT)
                | P_Fld(0x0, SHU_RANKCTL_DMSTBLAT));
    //vIO32WriteFldAlign(DRAMC_REG_SHU1_WODT, 0x1, SHU1_WODT_TWPSTEXT);

    //if(p->frequency==933)
    {
        //               91423 ===dramc_shu1_lp3_1866 begin===
        vIO32WriteFldMulti(DRAMC_REG_SHU_CONF0, P_Fld(0x1, SHU_CONF0_REFTHD)
                    | P_Fld(0x3f, SHU_CONF0_DMPGTIM));
        vIO32WriteFldAlign(DRAMC_REG_SHU_CONF1, 0xb0, SHU_CONF1_REFBW_FR);
        vIO32WriteFldMulti(DRAMC_REG_SHU_CONF3, P_Fld(0xff, SHU_CONF3_REFRCNT)
                    | P_Fld(0x5, SHU_CONF3_ZQCSCNT));
        vIO32WriteFldAlign(DRAMC_REG_SHU_RANKCTL, 0x3, SHU_RANKCTL_RANKINCTL_PHY);
        vIO32WriteFldAlign(DRAMC_REG_SHU_CKECTRL, 0x3, SHU_CKECTRL_CKEPRD);
        vIO32WriteFldAlign(DRAMC_REG_SHU1_WODT, 0x0, SHU1_WODT_TWPSTEXT);
        vIO32WriteFldAlign(DRAMC_REG_SHU_SCINTV, 0xf, SHU_SCINTV_SCINTV);
        //vIO32WriteFldMulti(DRAMC_REG_SHU_HWSET_MR2, P_Fld(0x0, SHU_HWSET_MR2_HWSET_MR2_OP)
        //            | P_Fld(0x0, SHU_HWSET_MR2_HWSET_MR2_MRSMA));
        //vIO32WriteFldMulti(DRAMC_REG_SHU_HWSET_MR13, P_Fld(0x1c, SHU_HWSET_MR13_HWSET_MR13_OP)
        //            | P_Fld(0x2, SHU_HWSET_MR13_HWSET_MR13_MRSMA));
        //vIO32WriteFldMulti(DRAMC_REG_SHU_HWSET_VRCG, P_Fld(0x0, SHU_HWSET_VRCG_HWSET_VRCG_OP)
        //            | P_Fld(0x0, SHU_HWSET_VRCG_HWSET_VRCG_MRSMA));
        vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x1, SHU_DQSG_RETRY_R_RETRY_USE_BURST_MDOE)
                    | P_Fld(0x1, SHU_DQSG_RETRY_R_RETRY_ROUND_NUM)
                    | P_Fld(0x1, SHU_DQSG_RETRY_R_XSR_RETRY_SPM_MODE)
                    | P_Fld(0x3, SHU_DQSG_RETRY_R_DQSIENLAT)
                    | P_Fld(0x1, SHU_DQSG_RETRY_R_DM4BYTE));
        //               92288 ===dramc_shu1_lp3_1866 end===

        //               93153 ===ddrphy_shu1_lp3_1866_CHA begin===
        vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5, 0x4, SHU1_B0_DQ5_RG_ARPI_FB_B0);
        vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, 0x4, SHU1_B1_DQ5_RG_ARPI_FB_B1);
        vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD5, 0x4, SHU1_CA_CMD5_RG_ARPI_FB_CA);
        //               93918 ===ddrphy_shu1_lp3_1866_CHA end===
        //               93918 ===ddrphy_shu1_lp3_1866_CHB begin===
        vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5+(1<<POS_BANK_NUM), 0x4, SHU1_B0_DQ5_RG_ARPI_FB_B0);
        vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5+(1<<POS_BANK_NUM), 0x4, SHU1_B1_DQ5_RG_ARPI_FB_B1);
        vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD5+(1<<POS_BANK_NUM), 0x4, SHU1_CA_CMD5_RG_ARPI_FB_CA);
        vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6+(1<<POS_BANK_NUM), 0x3, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA);
        vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ2+(1<<POS_BANK_NUM), P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0)
                    | P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0));
        //               94591 ===ddrphy_shu1_lp3_1866_CHB end===
    }
    //else
    ///TODO: overwrite 1866 setting END
    if(p->frequency==667)
    {
        DramcSetting_Olympus_LP3_DDR1333(p);
    }
    else if(p->frequency==467)
    {
        //DramcSetting_Olympus_LP3_DDR933(p);
        DramcSetting_Olympus_LP3_DDR1333(p);
    }

    ///TODO:
    vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0x1, RKCFG_RKMODE);

    UpdateInitialSettings_LP3(p);

#if SIMULATION_SW_IMPED
    DramcSwImpedanceSaveRegister(p, 1, DRAM_DFS_SHUFFLE_1);
#endif

#ifndef LOOPBACK_TEST
    DDRPhyFreqMeter();
#endif

    DramcModeRegInit_LP3(p);


#if 0
    vIO32WriteFldMulti(DRAMC_REG_MRS, P_Fld(0x0, MRS_MRSRK)
                | P_Fld(0x4, MRS_MRSMA)
                | P_Fld(0x0, MRS_MRSOP));

    mcDELAY_US(1);

    vIO32WriteFldAlign(DRAMC_REG_SPCMD, 0x1, SPCMD_MRREN);

    mcDELAY_US(1);
#endif

    vIO32WriteFldMulti(DRAMC_REG_REFCTRL0, P_Fld(0x0, REFCTRL0_PBREFEN)
                | P_Fld(0x1, REFCTRL0_PBREF_DISBYRATE));
    vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0x1, RKCFG_DQSOSC2RK);
    vIO32WriteFldAlign(DRAMC_REG_CKECTRL, 0x0, CKECTRL_CKEFIXON);
    //vIO32WriteFldAlign(DRAMC_REG_HW_MRR_FUN, 0x1, HW_MRR_FUN_TMRR_ENA);
    vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x1, DRAMCTRL_PREALL_OPTION);
    vIO32WriteFldAlign(DRAMC_REG_ZQCS, 0x56, ZQCS_ZQCSOP);

    mcDELAY_US(1);


    //vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x0, SPCMDCTRL_REFRDIS);//Lewis@20160613: Fix refresh rate is wrong while diable MR4
    vIO32WriteFldMulti(DRAMC_REG_REFCTRL0, P_Fld(0x1, REFCTRL0_REFFRERUN)
                | P_Fld(0x0, REFCTRL0_REFDIS));
    vIO32WriteFldAlign(DRAMC_REG_SREFCTRL, 0x1, SREFCTRL_SREF_HW_EN);
    vIO32WriteFldAlign(DRAMC_REG_MRS, 0x1, MRS_MPCRKEN);
    vIO32WriteFldMulti(DRAMC_REG_DRAMC_PD_CTRL, P_Fld(0x1, DRAMC_PD_CTRL_PHYCLKDYNGEN)
                | P_Fld(0x1, DRAMC_PD_CTRL_DCMEN));
    vIO32WriteFldMulti(DRAMC_REG_DRAMC_PD_CTRL+(1<<POS_BANK_NUM), P_Fld(0x0, DRAMC_PD_CTRL_COMBCLKCTRL)
                | P_Fld(0x0, DRAMC_PD_CTRL_DCMENNOTRFC)
                | P_Fld(0x1, DRAMC_PD_CTRL_DCMEN));
    vIO32WriteFldMulti(DRAMC_REG_EYESCAN, P_Fld(0x1, EYESCAN_RX_DQ_EYE_SEL)
                | P_Fld(0x1, EYESCAN_RG_RX_EYE_SCAN_EN));
    vIO32WriteFldMulti(DRAMC_REG_STBCAL1, P_Fld(0x1, STBCAL1_STBCNT_LATCH_EN)
                | P_Fld(0x1, STBCAL1_STBENCMPEN));
    vIO32WriteFldAlign(DRAMC_REG_TEST2_1, 0x10000, TEST2_1_TEST2_BASE);
    vIO32WriteFldAlign(DRAMC_REG_TEST2_2, 0x400, TEST2_2_TEST2_OFF);
    vIO32WriteFldMulti(DRAMC_REG_TEST2_3, P_Fld(0x1, TEST2_3_TEST2WREN2_HW_EN)
                | P_Fld(0x4, TEST2_3_DQSICALSTP)
                | P_Fld(0x1, TEST2_3_TESTAUDPAT));
    vIO32WriteFldAlign(DRAMC_REG_SHUCTRL2, 0x1, SHUCTRL2_MR13_SHU_EN);
    vIO32WriteFldMulti(DRAMC_REG_DRAMCTRL, P_Fld(0x1, DRAMCTRL_REQQUE_THD_EN)
                | P_Fld(0x1, DRAMCTRL_DPDRK_OPT));
    vIO32WriteFldAlign(DRAMC_REG_SHU_CKECTRL, 0x3, SHU_CKECTRL_SREF_CK_DLY);
    vIO32WriteFldAlign(DRAMC_REG_DUMMY_RD, 0x2, DUMMY_RD_RANK_NUM);
    vIO32WriteFldAlign(DRAMC_REG_TEST2_4, 0x4, TEST2_4_TESTAGENTRKSEL);
    vIO32WriteFldMulti(DRAMC_REG_REFCTRL1, P_Fld(0x1, REFCTRL1_REF_QUE_AUTOSAVE_EN)
                | P_Fld(0x1, REFCTRL1_SLEFREF_AUTOSAVE_EN));
    vIO32WriteFldMulti(DRAMC_REG_RSTMASK, P_Fld(0x0, RSTMASK_GT_SYNC_MASK)
                | P_Fld(0x0, RSTMASK_DVFS_SYNC_MASK)
                | P_Fld(0x0, RSTMASK_GT_SYNC_MASK_FOR_PHY)
                | P_Fld(0x0, RSTMASK_DVFS_SYNC_MASK_FOR_PHY));
    //vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x0, DRAMCTRL_CTOREQ_HPRI_OPT);
    //               91074 === DE initial sequence done ===
    //#ifdef DVFS_Enable
    vIO32WriteFldAlign(DRAMC_REG_STBCAL1, 0x8, STBCAL1_STBCAL_FILTER);
    vIO32WriteFldMulti(DRAMC_REG_STBCAL, P_Fld(0x0, STBCAL_STBCALEN)
                | P_Fld(0x0, STBCAL_STB_SELPHYCALEN)
                | P_Fld(0x0, STBCAL_REFUICHG)
                | P_Fld(0x1, STBCAL_SREF_DQSGUPD));

    mcDELAY_US(1);

      //*((UINT32P)(DDRPHY0AO_BASE + 0x0000)) = 0x00000000; // trun off when SPM need
    vIO32WriteFldAlign(DRAMC_REG_DDRCONF0, 0x1, DDRCONF0_AG0MWR);
    //vIO32WriteFldMulti(DRAMC_REG_DRAMCTRL, P_Fld(0x1, DRAMCTRL_FW2R)
    //            | P_Fld(0x0, DRAMCTRL_DYNMWREN)
    //            | P_Fld(0x1, DRAMCTRL_CLKWITRFC)
    //            | P_Fld(0x1, DRAMCTRL_ADRBIT3DEC)
    //            | P_Fld(0x1, DRAMCTRL_CTOREQ_HPRI_OPT));
#if 0 // Darren- by Jeremy confirm
    vIO32WriteFldAlign(DRAMC_REG_MISCTL0, 0x0, MISCTL0_PBC_ARB_EN);
    vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_REORDEREN)
                | P_Fld(0x1, PERFCTL0_RWHPRICTL)
                | P_Fld(0x0, PERFCTL0_EMILLATEN)
                | P_Fld(0x0, PERFCTL0_RWAGEEN)
                | P_Fld(0x0, PERFCTL0_RWHPRIEN));
#endif
    vIO32WriteFldMulti(DRAMC_REG_RSTMASK, P_Fld(0x0, RSTMASK_PHY_SYNC_MASK)
                | P_Fld(0x0, RSTMASK_DAT_SYNC_MASK));
    //vIO32WriteFldAlign(DRAMC_REG_SREFCTRL, 0xa, SREFCTRL_SREFDLY);
#if 0 // Darren- by Jeremy confirm
    vIO32WriteFldMulti(DRAMC_REG_REFCTRL0, P_Fld(0x1, REFCTRL0_REFNA_OPT)
                | P_Fld(0x2, REFCTRL0_DISBYREFNUM)
                | P_Fld(0x1, REFCTRL0_UPDBYWR));
#endif
    vIO32WriteFldMulti(DRAMC_REG_REFRATRE_FILTER, P_Fld(0x0, REFRATRE_FILTER_REFRATE_FIL7)
                | P_Fld(0x1, REFRATRE_FILTER_REFRATE_FIL6)
                | P_Fld(0x7, REFRATRE_FILTER_REFRATE_FIL5)
                | P_Fld(0x5, REFRATRE_FILTER_REFRATE_FIL4)
                | P_Fld(0x6, REFRATRE_FILTER_REFRATE_FIL2)
                | P_Fld(0x4, REFRATRE_FILTER_REFRATE_FIL1)
                | P_Fld(0x6, REFRATRE_FILTER_REFRATE_FIL0));
    vIO32WriteFldMulti(DRAMC_REG_ZQCS, P_Fld(0x0, ZQCS_ZQCSDUAL)
                | P_Fld(0x0, ZQCS_ZQCSMASK));
    vIO32WriteFldAlign(DRAMC_REG_HW_MRR_FUN, 0x1, HW_MRR_FUN_MRR_HW_HIPRI);
    vIO32WriteFldAlign(DRAMC_REG_DUMMY_RD, 0x1, DUMMY_RD_SREF_DMYRD_EN);
    vIO32WriteFldAlign(DRAMC_REG_STBCAL1, 0x2, STBCAL1_STBCAL_FILTER);
    //               91423 ===LP3_1866_intial_setting_shu1 end===

}
#endif


static void DramcSetting_Olympus_LP4_ByteMode_DDR3733(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS0, P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS3)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS2)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS1)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS0)
                | P_Fld(0x4, SHU_SELPH_DQS0_TXDLY_DQS3)
                | P_Fld(0x4, SHU_SELPH_DQS0_TXDLY_DQS2)
                | P_Fld(0x4, SHU_SELPH_DQS0_TXDLY_DQS1)
                | P_Fld(0x4, SHU_SELPH_DQS0_TXDLY_DQS0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS1, P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS3)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS2)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS1)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS0)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS3)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS2)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS1)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS0));
}


static void DramcSetting_Olympus_LP4_ByteMode_DDR2667(DRAMC_CTX_T *p)
{
    ////DRAMC0-SHU2
    vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK2, P_Fld(0x1a, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO0)
                | P_Fld(0x10, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO1)
                | P_Fld(0x0, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO2)
                | P_Fld(0x26, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO3));
    vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK3, P_Fld(0x13, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO4)
                | P_Fld(0x0, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO5)
                | P_Fld(0x40, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO6)
                | P_Fld(0x35, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO7));
    vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK4, P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO8)
                | P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO9)
                | P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO10)
                | P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO11));
    vIO32WriteFldAlign(DRAMC_REG_RK0_PRE_TDQSCK1, 0xa, RK0_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R0);
    vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK2, P_Fld(0x0, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R0)
                | P_Fld(0x0, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R0)
                | P_Fld(0xb, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R0)
                | P_Fld(0xd, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R0));
    vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK3, P_Fld(0x0, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R0)
                | P_Fld(0x11, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R0)
                | P_Fld(0xe, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R0));
    vIO32WriteFldAlign(DRAMC_REG_RK0_PRE_TDQSCK4, 0xa, RK0_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R0);
    vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK5, P_Fld(0x0, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R0)
                | P_Fld(0x0, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R0)
                | P_Fld(0xb, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R0)
                | P_Fld(0xd, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R0));
    vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK6, P_Fld(0x0, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R0)
                | P_Fld(0x11, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R0)
                | P_Fld(0xe, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R0));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK1, P_Fld(0x8, RK1_PRE_TDQSCK1_TDQSCK_PIFREQ2_B0R1)
                | P_Fld(0x10, RK1_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK2, P_Fld(0x0, RK1_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R1)
                | P_Fld(0x0, RK1_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R1)
                | P_Fld(0xe, RK1_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R1)
                | P_Fld(0x10, RK1_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK3, P_Fld(0x0, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R1)
                | P_Fld(0x14, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R1)
                | P_Fld(0x14, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK4, P_Fld(0x8, RK1_PRE_TDQSCK4_TDQSCK_PIFREQ2_B1R1)
                | P_Fld(0x10, RK1_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK5, P_Fld(0x0, RK1_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R1)
                | P_Fld(0x0, RK1_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R1)
                | P_Fld(0xe, RK1_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R1)
                | P_Fld(0x10, RK1_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK6, P_Fld(0x0, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R1)
                | P_Fld(0x14, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R1)
                | P_Fld(0x14, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R1));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM0, P_Fld(0x5, SHU_ACTIM0_TRCD)
                | P_Fld(0x1, SHU_ACTIM0_TRRD)
                | P_Fld(0xa, SHU_ACTIM0_TWR)
                | P_Fld(0x6, SHU_ACTIM0_TWTR));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM1, P_Fld(0xc, SHU_ACTIM1_TRC)
                | P_Fld(0x5, SHU_ACTIM1_TRAS)
                | P_Fld(0x4, SHU_ACTIM1_TRP)
                | P_Fld(0x1, SHU_ACTIM1_TRPAB));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM2, P_Fld(0x4, SHU_ACTIM2_TFAW)
                    | P_Fld(0x4, SHU_ACTIM2_TR2W)
                    | P_Fld(0x1, SHU_ACTIM2_TRTP)
                    | P_Fld(0x1, SHU_ACTIM2_TXP));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM3, P_Fld(0x51, SHU_ACTIM3_TRFC)
                    | P_Fld(0x23, SHU_ACTIM3_TRFCPB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM4, P_Fld(0x1c, SHU_ACTIM4_TZQCS)
                    | P_Fld(0x51, SHU_ACTIM4_REFCNT)
                    | P_Fld(0x63, SHU_ACTIM4_TXREFCNT));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM5, P_Fld(0x6, SHU_ACTIM5_TMRR2W)
                    | P_Fld(0xa, SHU_ACTIM5_TWTPD)
                    | P_Fld(0x9, SHU_ACTIM5_TR2PD));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x8, SHU_ACTIM_XRT_XRTW2W)
                    | P_Fld(0x4, SHU_ACTIM_XRT_XRTR2W));
        vIO32WriteFldMulti(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(0x0, SHU_AC_TIME_05T_TWTR_M05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TR2W_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TWTPD_M05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TR2PD_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TFAW_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRRD_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TWR_M05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRAS_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRPAB_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRCD_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRTP_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TXP_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRFC_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRC_05T));
        vIO32WriteFldMulti(DRAMC_REG_SHU_CONF1, P_Fld(0xb, SHU_CONF1_DATLAT_DSEL_PHY)
                    | P_Fld(0xc, SHU_CONF1_DATLAT_DSEL)
                    | P_Fld(0xd, SHU_CONF1_DATLAT));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM2, P_Fld(0x4, SHU_ACTIM2_TFAW)
                    | P_Fld(0x7, SHU_ACTIM2_TR2W)
                    | P_Fld(0x1, SHU_ACTIM2_TRTP)
                    | P_Fld(0x0, SHU_ACTIM2_TXP));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM3, P_Fld(0x51, SHU_ACTIM3_TRFC)
                    | P_Fld(0x23, SHU_ACTIM3_TRFCPB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM4, P_Fld(0x1c, SHU_ACTIM4_TZQCS)
                    | P_Fld(0x51, SHU_ACTIM4_REFCNT)
                    | P_Fld(0x63, SHU_ACTIM4_TXREFCNT));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM5, P_Fld(0x6, SHU_ACTIM5_TMRR2W)
                    | P_Fld(0xa, SHU_ACTIM5_TWTPD)
                    | P_Fld(0xa, SHU_ACTIM5_TR2PD));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x8, SHU_ACTIM_XRT_XRTW2W)
                    | P_Fld(0x6, SHU_ACTIM_XRT_XRTR2W));
        vIO32WriteFldMulti(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(0x0, SHU_AC_TIME_05T_TWTR_M05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TWTPD_M05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TFAW_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRRD_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TWR_M05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRAS_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRPAB_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRCD_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRTP_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TXP_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRFC_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRC_05T));
        vIO32WriteFldMulti(DRAMC_REG_SHU_CONF1, P_Fld(0xc, SHU_CONF1_DATLAT_DSEL_PHY)
                    | P_Fld(0xd, SHU_CONF1_DATLAT_DSEL)
                    | P_Fld(0xe, SHU_CONF1_DATLAT));
    }
    vIO32WriteFldAlign(DRAMC_REG_SHU_CONF2, 0x54, SHU_CONF2_FSPCHG_PRDCNT);
    vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x4, SHU_RANKCTL_RANKINCTL_PHY)
                | P_Fld(0x2, SHU_RANKCTL_RANKINCTL_ROOT1)
                | P_Fld(0x2, SHU_RANKCTL_RANKINCTL));
    vIO32WriteFldMulti(DRAMC_REG_SHU_CKECTRL, P_Fld(0x3, SHU_CKECTRL_TCKESRX)
                | P_Fld(0x3, SHU_CKECTRL_CKEPRD));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x1, SHU_ODTCTRL_RODTE)
                | P_Fld(0x1, SHU_ODTCTRL_RODTE2)
                | P_Fld(0x4, SHU_ODTCTRL_RODT)
                | P_Fld(0x1, SHU_ODTCTRL_ROEN));
    vIO32WriteFldMulti(DRAMC_REG_SHU1_DQSOSC_PRD, P_Fld(0xc, SHU1_DQSOSC_PRD_DQSOSCTHRD_DEC)
                | P_Fld(0xc, SHU1_DQSOSC_PRD_DQSOSCTHRD_INC)
                | P_Fld(0x10, SHU1_DQSOSC_PRD_DQSOSC_PRDCNT));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS0, P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_OEN_DQS3)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_OEN_DQS2)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_OEN_DQS1)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_OEN_DQS0)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS3)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS2)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS1)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS1, P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS3)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS2)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS1)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS0)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS3)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS2)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS1)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS0));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldAlign(DRAMC_REG_SHU1_WODT, 0x0, SHU1_WODT_WPST2T);
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU1_WODT, P_Fld(0x0, SHU1_WODT_WPST2T)
                    | P_Fld(0x1, SHU1_WODT_DBIWR));
    }
    vIO32WriteFldAlign(DRAMC_REG_SHU_DQS2DQ_TX, 0x7, SHU_DQS2DQ_TX_OE2DQ_OFFSET);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR2, 0x24, SHU_HWSET_MR2_HWSET_MR2_OP);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR13, 0xc8, SHU_HWSET_MR13_HWSET_MR13_OP);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_VRCG, 0xc0, SHU_HWSET_VRCG_HWSET_VRCG_OP);
    vIO32WriteFldAlign(DRAMC_REG_SHURK0_DQSCTL, 0x4, SHURK0_DQSCTL_DQSINCTL);
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSIEN, P_Fld(0x19, SHURK0_DQSIEN_R0DQS1IEN)
                | P_Fld(0x19, SHURK0_DQSIEN_R0DQS0IEN));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_PI, P_Fld(0x14, SHU1RK0_PI_RK0_ARPI_DQM_B1)
                | P_Fld(0x14, SHU1RK0_PI_RK0_ARPI_DQM_B0)
                | P_Fld(0x14, SHU1RK0_PI_RK0_ARPI_DQ_B1)
                | P_Fld(0x14, SHU1RK0_PI_RK0_ARPI_DQ_B0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQSOSC, P_Fld(0x1ae, SHU1RK0_DQSOSC_DQSOSC_BASE_RK0_B1)
                | P_Fld(0x1ae, SHU1RK0_DQSOSC_DQSOSC_BASE_RK0));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
       vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN0, P_Fld(0x1, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1)
                    | P_Fld(0x1, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN)
                    | P_Fld(0x1, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1)
                    | P_Fld(0x1, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x1, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
                    | P_Fld(0x1, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
                    | P_Fld(0x1, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
                    | P_Fld(0x1, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                    | P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ2)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM2)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN0, P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1)
                    | P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN)
                    | P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1)
                    | P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x1, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
                    | P_Fld(0x1, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
                    | P_Fld(0x1, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
                    | P_Fld(0x1, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                    | P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ2));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM2));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(0x0, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
                | P_Fld(0x0, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
                | P_Fld(0x0, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
                | P_Fld(0x0, SHURK0_SELPH_DQ2_DLY_OEN_DQ0)
                | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_DQ3)
                | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_DQ2)
                | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_DQ1)
                | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(0x0, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
                | P_Fld(0x0, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
                | P_Fld(0x0, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
                | P_Fld(0x0, SHURK0_SELPH_DQ3_DLY_OEN_DQM0)
                | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_DQM3)
                | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_DQM2)
                | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_DQM1)
                | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_DQM0));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL1, P_Fld(0x354, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1)
                    | P_Fld(0x354, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL2, P_Fld(0x354, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1)
                    | P_Fld(0x354, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL1, P_Fld(0x254, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1)
                    | P_Fld(0x254, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL2, P_Fld(0x254, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1)
                    | P_Fld(0x254, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL3, P_Fld(0x14, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1_B4TO0)
                | P_Fld(0x14, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0_B4TO0)
                | P_Fld(0x18, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1)
                | P_Fld(0x18, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL4, P_Fld(0x14, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1_B4TO0)
                | P_Fld(0x14, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0_B4TO0)
                | P_Fld(0x18, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1)
                | P_Fld(0x18, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL5, P_Fld(0x354, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1)
                    | P_Fld(0x354, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL5, P_Fld(0x254, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1)
                    | P_Fld(0x254, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));
    }
    vIO32WriteFldAlign(DRAMC_REG_SHURK1_DQSCTL, 0x4, SHURK1_DQSCTL_R1DQSINCTL);
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSIEN, P_Fld(0x1f, SHURK1_DQSIEN_R1DQS1IEN)
                | P_Fld(0x1f, SHURK1_DQSIEN_R1DQS0IEN));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_PI, P_Fld(0x14, SHU1RK1_PI_RK1_ARPI_DQM_B1)
                | P_Fld(0x14, SHU1RK1_PI_RK1_ARPI_DQM_B0)
                | P_Fld(0x14, SHU1RK1_PI_RK1_ARPI_DQ_B1)
                | P_Fld(0x14, SHU1RK1_PI_RK1_ARPI_DQ_B0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQSOSC, P_Fld(0x160, SHU1RK1_DQSOSC_DQSOSC_BASE_RK1_B1)
                | P_Fld(0x160, SHU1RK1_DQSOSC_DQSOSC_BASE_RK1));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN_P1)
                    | P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN)
                    | P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN_P1)
                    | P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x6, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
                    | P_Fld(0x6, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
                    | P_Fld(0x6, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
                    | P_Fld(0x6, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN_P1)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN_P1)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x6, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
                    | P_Fld(0x6, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
                    | P_Fld(0x6, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
                    | P_Fld(0x6, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x4, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                | P_Fld(0x0, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                | P_Fld(0x4, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                | P_Fld(0x0, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x0, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
                | P_Fld(0x0, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
                | P_Fld(0x0, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
                | P_Fld(0x0, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0)
                | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1DQ3)
                | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1DQ2)
                | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1DQ1)
                | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x0, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
                | P_Fld(0x0, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
                | P_Fld(0x0, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
                | P_Fld(0x0, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0)
                | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1DQM3)
                | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1DQM2)
                | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1DQM1)
                | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1DQM0));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL1, P_Fld(0x354, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1)
                    | P_Fld(0x354, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL2, P_Fld(0x354, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1)
                    | P_Fld(0x354, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL1, P_Fld(0x254, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1)
                    | P_Fld(0x254, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL2, P_Fld(0x254, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1)
                    | P_Fld(0x254, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL3, P_Fld(0x14, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1_B4TO0)
                | P_Fld(0x14, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0_B4TO0)
                | P_Fld(0x18, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1)
                | P_Fld(0x18, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL4, P_Fld(0x14, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1_B4TO0)
                | P_Fld(0x14, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0_B4TO0)
                | P_Fld(0x18, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1)
                | P_Fld(0x18, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL5, P_Fld(0x354, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1)
                    | P_Fld(0x354, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL5, P_Fld(0x254, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1)
                    | P_Fld(0x254, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0));
    }
    vIO32WriteFldAlign(DRAMC_REG_SHURK2_DQSCTL, 0x4, SHURK2_DQSCTL_R2DQSINCTL);
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_DQSIEN, P_Fld(0x0, SHURK2_DQSIEN_R2DQS3IEN)
                | P_Fld(0x0, SHURK2_DQSIEN_R2DQS2IEN)
                | P_Fld(0x1f, SHURK2_DQSIEN_R2DQS1IEN)
                | P_Fld(0x1f, SHURK2_DQSIEN_R2DQS0IEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_ODTEN0, P_Fld(0x2, SHURK2_SELPH_ODTEN0_TXDLY_B3_R2RODTEN_P1)
                | P_Fld(0x2, SHURK2_SELPH_ODTEN0_TXDLY_B3_R2RODTEN)
                | P_Fld(0x2, SHURK2_SELPH_ODTEN0_TXDLY_B2_R2RODTEN_P1)
                | P_Fld(0x2, SHURK2_SELPH_ODTEN0_TXDLY_B2_R2RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_ODTEN1, P_Fld(0x4, SHURK2_SELPH_ODTEN1_DLY_B3_R2RODTEN_P1)
                | P_Fld(0x4, SHURK2_SELPH_ODTEN1_DLY_B3_R2RODTEN)
                | P_Fld(0x4, SHURK2_SELPH_ODTEN1_DLY_B2_R2RODTEN_P1)
                | P_Fld(0x4, SHURK2_SELPH_ODTEN1_DLY_B2_R2RODTEN)
                | P_Fld(0x7, SHURK2_SELPH_ODTEN1_DLY_B1_R2RODTEN_P1)
                | P_Fld(0x7, SHURK2_SELPH_ODTEN1_DLY_B1_R2RODTEN)
                | P_Fld(0x7, SHURK2_SELPH_ODTEN1_DLY_B0_R2RODTEN_P1)
                | P_Fld(0x7, SHURK2_SELPH_ODTEN1_DLY_B0_R2RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQSG0, P_Fld(0x2, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED_P1)
                | P_Fld(0x1, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED)
                | P_Fld(0x2, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED_P1)
                | P_Fld(0x1, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQSG1, P_Fld(0x3, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS1_GATED_P1)
                | P_Fld(0x7, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS1_GATED)
                | P_Fld(0x3, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS0_GATED_P1)
                | P_Fld(0x7, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ0, P_Fld(0x3, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ3)
                | P_Fld(0x3, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ2)
                | P_Fld(0x3, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ1)
                | P_Fld(0x3, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ0)
                | P_Fld(0x3, SHURK2_SELPH_DQ0_TX_DLY_R2DQ3)
                | P_Fld(0x3, SHURK2_SELPH_DQ0_TX_DLY_R2DQ2)
                | P_Fld(0x3, SHURK2_SELPH_DQ0_TX_DLY_R2DQ1)
                | P_Fld(0x3, SHURK2_SELPH_DQ0_TX_DLY_R2DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ1, P_Fld(0x3, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM3)
                | P_Fld(0x3, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM2)
                | P_Fld(0x3, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM1)
                | P_Fld(0x3, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM0)
                | P_Fld(0x3, SHURK2_SELPH_DQ1_TX_DLY_R2DQM3)
                | P_Fld(0x3, SHURK2_SELPH_DQ1_TX_DLY_R2DQM2)
                | P_Fld(0x3, SHURK2_SELPH_DQ1_TX_DLY_R2DQM1)
                | P_Fld(0x3, SHURK2_SELPH_DQ1_TX_DLY_R2DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ2, P_Fld(0x0, SHURK2_SELPH_DQ2_DLY_R2OEN_DQ3)
                | P_Fld(0x0, SHURK2_SELPH_DQ2_DLY_R2OEN_DQ2)
                | P_Fld(0x0, SHURK2_SELPH_DQ2_DLY_R2OEN_DQ1)
                | P_Fld(0x0, SHURK2_SELPH_DQ2_DLY_R2OEN_DQ0)
                | P_Fld(0x2, SHURK2_SELPH_DQ2_DLY_R2DQ3)
                | P_Fld(0x2, SHURK2_SELPH_DQ2_DLY_R2DQ2)
                | P_Fld(0x2, SHURK2_SELPH_DQ2_DLY_R2DQ1)
                | P_Fld(0x2, SHURK2_SELPH_DQ2_DLY_R2DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ3, P_Fld(0x0, SHURK2_SELPH_DQ3_DLY_R2OEN_DQM3)
                | P_Fld(0x0, SHURK2_SELPH_DQ3_DLY_R2OEN_DQM2)
                | P_Fld(0x0, SHURK2_SELPH_DQ3_DLY_R2OEN_DQM1)
                | P_Fld(0x0, SHURK2_SELPH_DQ3_DLY_R2OEN_DQM0)
                | P_Fld(0x2, SHURK2_SELPH_DQ3_DLY_R2DQM3)
                | P_Fld(0x2, SHURK2_SELPH_DQ3_DLY_R2DQM2)
                | P_Fld(0x2, SHURK2_SELPH_DQ3_DLY_R2DQM1)
                | P_Fld(0x2, SHURK2_SELPH_DQ3_DLY_R2DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x4, SHU_DQSG_RETRY_R_DQSIENLAT)
                | P_Fld(0x1, SHU_DQSG_RETRY_R_DDR1866_PLUS));

    ////DDRPHY0-SHU2
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5, 0x3, SHU1_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0);
    //vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ6, P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0)
    //            | P_Fld(0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x0, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
                    | P_Fld(0x1, SHU1_B0_DQ7_MIDPI_ENABLE));
    }
    else
    {
        vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x1, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
                    | P_Fld(0x1, SHU1_B0_DQ7_R_DMDQMDBI_SHU_B0)
                    | P_Fld(0x0, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
                    | P_Fld(0x1, SHU1_B0_DQ7_MIDPI_ENABLE));
    }
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, 0x3, SHU1_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1);
    //vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ6, P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1)
    //            | P_Fld(0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
    }
    else
    {
        vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x1, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
                    | P_Fld(0x1, SHU1_B1_DQ7_R_DMDQMDBI_SHU_B1));
    }
    //DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    //vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6, P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA)
    //            | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA));
    //vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6+(1<<POS_BANK_NUM), P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA)
    //            | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA));
    //DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, 0x3300, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, 0x3300, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ2, P_Fld(0x12, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ3, P_Fld(0x12, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ4, P_Fld(0x12, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ5, P_Fld(0x12, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ6, P_Fld(0x1d, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0x15, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ7, P_Fld(0x14, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0)
                | P_Fld(0x14, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ2, P_Fld(0x12, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ3, P_Fld(0x12, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ4, P_Fld(0x12, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ5, P_Fld(0x12, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6, P_Fld(0x1d, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0x15, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ7, P_Fld(0x14, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1)
                | P_Fld(0x14, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ0, P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B0_DQ1, 0xa, SHU1_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ2, P_Fld(0x13, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ3, P_Fld(0x13, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ4, P_Fld(0x13, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ5, P_Fld(0x13, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ6, P_Fld(0x1e, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0x16, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ7, P_Fld(0x14, SHU1_R1_B0_DQ7_RK1_ARPI_DQM_B0)
                | P_Fld(0x14, SHU1_R1_B0_DQ7_RK1_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0, P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1, 0xa, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ2, P_Fld(0x13, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ3, P_Fld(0x13, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ4, P_Fld(0x13, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ5, P_Fld(0x13, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6, P_Fld(0x1e, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0x16, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ7, P_Fld(0x14, SHU1_R1_B1_DQ7_RK1_ARPI_DQM_B1)
                | P_Fld(0x14, SHU1_R1_B1_DQ7_RK1_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B0_DQ0, P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ7_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ6_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ5_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ4_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ3_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ2_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ1_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ0_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B0_DQ1, 0xa, SHU1_R2_B0_DQ1_RK2_TX_ARDQM0_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B0_DQ7, P_Fld(0x1f, SHU1_R2_B0_DQ7_RK2_ARPI_DQM_B0)
                | P_Fld(0x1f, SHU1_R2_B0_DQ7_RK2_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B1_DQ0, P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ7_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ6_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ5_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ4_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ3_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ2_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ1_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B1_DQ1, 0xa, SHU1_R2_B1_DQ1_RK2_TX_ARDQM0_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B1_DQ7, P_Fld(0x1f, SHU1_R2_B1_DQ7_RK2_ARPI_DQM_B1)
                | P_Fld(0x1f, SHU1_R2_B1_DQ7_RK2_ARPI_DQ_B1));

}

static void DramcSetting_Olympus_LP4_ByteMode_DDR2400(DRAMC_CTX_T *p)
{}

static void DramcSetting_Olympus_LP4_ByteMode_DDR1600(DRAMC_CTX_T *p)
{
    ////DRAMC0-SHU3
    vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK2, P_Fld(0x1a, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO0)
                | P_Fld(0x10, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO1)
                | P_Fld(0x0, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO2)
                | P_Fld(0x26, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO3));
    vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK3, P_Fld(0x13, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO4)
                | P_Fld(0x0, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO5)
                | P_Fld(0x40, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO6)
                | P_Fld(0x35, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO7));
    vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK4, P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO8)
                | P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO9)
                | P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO10)
                | P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO11));
    vIO32WriteFldAlign(DRAMC_REG_RK0_PRE_TDQSCK1, 0xa, RK0_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R0);
    vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK2, P_Fld(0x0, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R0)
                | P_Fld(0x0, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R0)
                | P_Fld(0xb, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R0)
                | P_Fld(0xd, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R0));
    vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK3, P_Fld(0x0, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R0)
                | P_Fld(0x11, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R0)
                | P_Fld(0xe, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R0));
    vIO32WriteFldAlign(DRAMC_REG_RK0_PRE_TDQSCK4, 0xa, RK0_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R0);
    vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK5, P_Fld(0x0, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R0)
                | P_Fld(0x0, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R0)
                | P_Fld(0xb, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R0)
                | P_Fld(0xd, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R0));
    vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK6, P_Fld(0x0, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R0)
                | P_Fld(0x11, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R0)
                | P_Fld(0xe, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R0));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK1, P_Fld(0x8, RK1_PRE_TDQSCK1_TDQSCK_PIFREQ2_B0R1)
                | P_Fld(0x10, RK1_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK2, P_Fld(0x0, RK1_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R1)
                | P_Fld(0x0, RK1_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R1)
                | P_Fld(0xe, RK1_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R1)
                | P_Fld(0x10, RK1_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK3, P_Fld(0x0, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R1)
                | P_Fld(0x14, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R1)
                | P_Fld(0x14, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK4, P_Fld(0x8, RK1_PRE_TDQSCK4_TDQSCK_PIFREQ2_B1R1)
                | P_Fld(0x10, RK1_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK5, P_Fld(0x0, RK1_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R1)
                | P_Fld(0x0, RK1_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R1)
                | P_Fld(0xe, RK1_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R1)
                | P_Fld(0x10, RK1_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK6, P_Fld(0x0, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R1)
                | P_Fld(0x14, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R1)
                | P_Fld(0x14, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R1));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM0, P_Fld(0x3, SHU_ACTIM0_TRCD)
                    | P_Fld(0x0, SHU_ACTIM0_TRRD)
                    | P_Fld(0x8, SHU_ACTIM0_TWR)
                    | P_Fld(0x5, SHU_ACTIM0_TWTR));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM1, P_Fld(0x4, SHU_ACTIM1_TRC)
                    | P_Fld(0x0, SHU_ACTIM1_TRAS)
                    | P_Fld(0x2, SHU_ACTIM1_TRP)
                    | P_Fld(0x0, SHU_ACTIM1_TRPAB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM2, P_Fld(0x0, SHU_ACTIM2_TFAW)
                    | P_Fld(0x3, SHU_ACTIM2_TR2W)
                    | P_Fld(0x1, SHU_ACTIM2_TRTP)
                    | P_Fld(0x0, SHU_ACTIM2_TXP));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM3, P_Fld(0x2c, SHU_ACTIM3_TRFC)
                    | P_Fld(0x10, SHU_ACTIM3_TRFCPB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM4, P_Fld(0x10, SHU_ACTIM4_TZQCS)
                    | P_Fld(0x30, SHU_ACTIM4_REFCNT)
                    | P_Fld(0x3d, SHU_ACTIM4_TXREFCNT));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM5, P_Fld(0x3, SHU_ACTIM5_TMRR2W)
                    | P_Fld(0x7, SHU_ACTIM5_TWTPD)
                    | P_Fld(0x4, SHU_ACTIM5_TR2PD));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x7, SHU_ACTIM_XRT_XRTW2W)
                    | P_Fld(0x3, SHU_ACTIM_XRT_XRTR2W));
        vIO32WriteFldMulti(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(0x1, SHU_AC_TIME_05T_TWTR_M05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TR2W_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TWTPD_M05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TR2PD_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TFAW_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRRD_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TWR_M05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRAS_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRPAB_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRCD_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRTP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TXP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRFC_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRC_05T));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM0, P_Fld(0x3, SHU_ACTIM0_TRCD)
                    | P_Fld(0x0, SHU_ACTIM0_TRRD)
                    | P_Fld(0x7, SHU_ACTIM0_TWR)
                    | P_Fld(0x4, SHU_ACTIM0_TWTR));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM1, P_Fld(0x4, SHU_ACTIM1_TRC)
                    | P_Fld(0x0, SHU_ACTIM1_TRAS)
                    | P_Fld(0x2, SHU_ACTIM1_TRP)
                    | P_Fld(0x0, SHU_ACTIM1_TRPAB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM2, P_Fld(0x0, SHU_ACTIM2_TFAW)
                    | P_Fld(0x2, SHU_ACTIM2_TR2W)
                    | P_Fld(0x1, SHU_ACTIM2_TRTP)
                    | P_Fld(0x0, SHU_ACTIM2_TXP));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM3, P_Fld(0x2c, SHU_ACTIM3_TRFC)
                    | P_Fld(0x10, SHU_ACTIM3_TRFCPB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM4, P_Fld(0x10, SHU_ACTIM4_TZQCS)
                    | P_Fld(0x30, SHU_ACTIM4_REFCNT)
                    | P_Fld(0x3d, SHU_ACTIM4_TXREFCNT));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM5, P_Fld(0x3, SHU_ACTIM5_TMRR2W)
                    | P_Fld(0x6, SHU_ACTIM5_TWTPD)
                    | P_Fld(0x6, SHU_ACTIM5_TR2PD));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x7, SHU_ACTIM_XRT_XRTW2W)
                    | P_Fld(0x3, SHU_ACTIM_XRT_XRTR2W));
        vIO32WriteFldMulti(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(0x1, SHU_AC_TIME_05T_TWTR_M05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TWTPD_M05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TFAW_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRRD_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TWR_M05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRAS_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRPAB_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRCD_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRTP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TXP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRFC_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRC_05T));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHU_CONF1, P_Fld(0xa, SHU_CONF1_DATLAT_DSEL_PHY)
                | P_Fld(0xb, SHU_CONF1_DATLAT_DSEL)
                | P_Fld(0xc, SHU_CONF1_DATLAT));
    vIO32WriteFldAlign(DRAMC_REG_SHU_CONF2, 0x32, SHU_CONF2_FSPCHG_PRDCNT);
    vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x2, SHU_RANKCTL_RANKINCTL_PHY)
                | P_Fld(0x0, SHU_RANKCTL_RANKINCTL_ROOT1)
                | P_Fld(0x0, SHU_RANKCTL_RANKINCTL));
    vIO32WriteFldMulti(DRAMC_REG_SHU_CKECTRL, P_Fld(0x0, SHU_CKECTRL_TCKESRX)
                | P_Fld(0x2, SHU_CKECTRL_CKEPRD));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x0, SHU_ODTCTRL_RODTE)
                | P_Fld(0x0, SHU_ODTCTRL_RODTE2)
                | P_Fld(0x2, SHU_ODTCTRL_RODT)
#ifdef LOOPBACK_TEST
                | P_Fld(0x1, SHU_ODTCTRL_ROEN));
#else
                | P_Fld(0x0, SHU_ODTCTRL_ROEN));
#endif
    vIO32WriteFldMulti(DRAMC_REG_SHU1_DQSOSC_PRD, P_Fld(0x14, SHU1_DQSOSC_PRD_DQSOSCTHRD_DEC)
                | P_Fld(0x14, SHU1_DQSOSC_PRD_DQSOSCTHRD_INC)
                | P_Fld(0xf, SHU1_DQSOSC_PRD_DQSOSC_PRDCNT));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS0, P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS3)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS2)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS1)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS0)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS3)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS2)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS1)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS1, P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS3)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS2)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS1)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS0)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS3)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS2)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS1)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS0));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldAlign(DRAMC_REG_SHU1_WODT, 0x1, SHU1_WODT_WPST2T);
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU1_WODT, P_Fld(0x1, SHU1_WODT_WPST2T)
                    | P_Fld(0x0, SHU1_WODT_DBIWR));
    }
    vIO32WriteFldAlign(DRAMC_REG_SHU_DQS2DQ_TX, 0x4, SHU_DQS2DQ_TX_OE2DQ_OFFSET);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR2, 0x12, SHU_HWSET_MR2_HWSET_MR2_OP);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR13, 0x8, SHU_HWSET_MR13_HWSET_MR13_OP);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_VRCG, 0x0, SHU_HWSET_VRCG_HWSET_VRCG_OP);
    vIO32WriteFldAlign(DRAMC_REG_SHURK0_DQSCTL, 0x2, SHURK0_DQSCTL_DQSINCTL);
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSIEN, P_Fld(0x0, SHURK0_DQSIEN_R0DQS1IEN)
                | P_Fld(0x0, SHURK0_DQSIEN_R0DQS0IEN));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_PI, P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQM_B1)
                | P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQM_B0)
                | P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQ_B1)
                | P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQ_B0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQSOSC, P_Fld(0x2d0, SHU1RK0_DQSOSC_DQSOSC_BASE_RK0_B1)
                | P_Fld(0x2d0, SHU1RK0_DQSOSC_DQSOSC_BASE_RK0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN0, P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x5, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
                | P_Fld(0x5, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
                | P_Fld(0x5, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
                | P_Fld(0x5, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                    | P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                    | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                    | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                    | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ2)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                    | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                    | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                    | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM2)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x0, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                    | P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                    | P_Fld(0x0, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                    | P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                    | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                    | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                    | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ2));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                    | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                    | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                    | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM2));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_OEN_DQ0)
                | P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ3)
                | P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ2)
                | P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ1)
                | P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_OEN_DQM0)
                | P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM3)
                | P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM2)
                | P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM1)
                | P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL1, P_Fld(0x23a, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1)
                | P_Fld(0x23a, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL2, P_Fld(0x23a, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1)
                | P_Fld(0x23a, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL3, P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1_B4TO0)
                | P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0_B4TO0)
                | P_Fld(0xf, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1)
                | P_Fld(0xf, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL4, P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1_B4TO0)
                | P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0_B4TO0)
                | P_Fld(0xf, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1)
                | P_Fld(0xf, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL5, P_Fld(0x23e, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1)
                | P_Fld(0x23e, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));
    vIO32WriteFldAlign(DRAMC_REG_SHURK1_DQSCTL, 0x2, SHURK1_DQSCTL_R1DQSINCTL);
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSIEN, P_Fld(0x0, SHURK1_DQSIEN_R1DQS1IEN)
                | P_Fld(0x0, SHURK1_DQSIEN_R1DQS0IEN));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_PI, P_Fld(0x1e, SHU1RK1_PI_RK1_ARPI_DQM_B1)
                | P_Fld(0x1e, SHU1RK1_PI_RK1_ARPI_DQM_B0)
                | P_Fld(0x1e, SHU1RK1_PI_RK1_ARPI_DQ_B1)
                | P_Fld(0x1e, SHU1RK1_PI_RK1_ARPI_DQ_B0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQSOSC, P_Fld(0x24e, SHU1RK1_DQSOSC_DQSOSC_BASE_RK1_B1)
                | P_Fld(0x24e, SHU1RK1_DQSOSC_DQSOSC_BASE_RK1));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN_P1)
                | P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN)
                | P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN_P1)
                | P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x0, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                    | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x5, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                    | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                    | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                    | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                    | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                    | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                    | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                    | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                    | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                    | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                    | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                    | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                    | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0)
                | P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1DQ3)
                | P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1DQ2)
                | P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1DQ1)
                | P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0)
                | P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1DQM3)
                | P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1DQM2)
                | P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1DQM1)
                | P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL1, P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1)
                | P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL2, P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1)
                | P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL3, P_Fld(0x1e, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1_B4TO0)
                | P_Fld(0x1e, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0_B4TO0)
                | P_Fld(0xf, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1)
                | P_Fld(0xf, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL4, P_Fld(0x1e, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1_B4TO0)
                | P_Fld(0x1e, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0_B4TO0)
                | P_Fld(0xf, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1)
                | P_Fld(0xf, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL5, P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1)
                | P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0));
    vIO32WriteFldAlign(DRAMC_REG_SHURK2_DQSCTL, 0x2, SHURK2_DQSCTL_R2DQSINCTL);
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_DQSIEN, P_Fld(0x0, SHURK2_DQSIEN_R2DQS3IEN)
                | P_Fld(0x0, SHURK2_DQSIEN_R2DQS2IEN)
                | P_Fld(0x0, SHURK2_DQSIEN_R2DQS1IEN)
                | P_Fld(0x0, SHURK2_DQSIEN_R2DQS0IEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_ODTEN0, P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B3_R2RODTEN_P1)
                | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B3_R2RODTEN)
                | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B2_R2RODTEN_P1)
                | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B2_R2RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_ODTEN1, P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B3_R2RODTEN_P1)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B3_R2RODTEN)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B2_R2RODTEN_P1)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B2_R2RODTEN)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B1_R2RODTEN_P1)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B1_R2RODTEN)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B0_R2RODTEN_P1)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B0_R2RODTEN));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQSG0, P_Fld(0x3, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED_P1)
                    | P_Fld(0x3, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED)
                    | P_Fld(0x3, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED_P1)
                    | P_Fld(0x3, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQSG0, P_Fld(0x2, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED)
                    | P_Fld(0x2, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQSG1, P_Fld(0x4, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS1_GATED_P1)
                | P_Fld(0x0, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS1_GATED)
                | P_Fld(0x4, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS0_GATED_P1)
                | P_Fld(0x0, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ0, P_Fld(0x1, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ3)
                | P_Fld(0x1, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ2)
                | P_Fld(0x1, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ1)
                | P_Fld(0x1, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ0)
                | P_Fld(0x2, SHURK2_SELPH_DQ0_TX_DLY_R2DQ3)
                | P_Fld(0x2, SHURK2_SELPH_DQ0_TX_DLY_R2DQ2)
                | P_Fld(0x2, SHURK2_SELPH_DQ0_TX_DLY_R2DQ1)
                | P_Fld(0x2, SHURK2_SELPH_DQ0_TX_DLY_R2DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ1, P_Fld(0x1, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM3)
                | P_Fld(0x1, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM2)
                | P_Fld(0x1, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM1)
                | P_Fld(0x1, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM0)
                | P_Fld(0x2, SHURK2_SELPH_DQ1_TX_DLY_R2DQM3)
                | P_Fld(0x2, SHURK2_SELPH_DQ1_TX_DLY_R2DQM2)
                | P_Fld(0x2, SHURK2_SELPH_DQ1_TX_DLY_R2DQM1)
                | P_Fld(0x2, SHURK2_SELPH_DQ1_TX_DLY_R2DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ2, P_Fld(0x7, SHURK2_SELPH_DQ2_DLY_R2OEN_DQ3)
                | P_Fld(0x7, SHURK2_SELPH_DQ2_DLY_R2OEN_DQ2)
                | P_Fld(0x7, SHURK2_SELPH_DQ2_DLY_R2OEN_DQ1)
                | P_Fld(0x7, SHURK2_SELPH_DQ2_DLY_R2OEN_DQ0)
                | P_Fld(0x1, SHURK2_SELPH_DQ2_DLY_R2DQ3)
                | P_Fld(0x1, SHURK2_SELPH_DQ2_DLY_R2DQ2)
                | P_Fld(0x1, SHURK2_SELPH_DQ2_DLY_R2DQ1)
                | P_Fld(0x1, SHURK2_SELPH_DQ2_DLY_R2DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ3, P_Fld(0x7, SHURK2_SELPH_DQ3_DLY_R2OEN_DQM3)
                | P_Fld(0x7, SHURK2_SELPH_DQ3_DLY_R2OEN_DQM2)
                | P_Fld(0x7, SHURK2_SELPH_DQ3_DLY_R2OEN_DQM1)
                | P_Fld(0x7, SHURK2_SELPH_DQ3_DLY_R2OEN_DQM0)
                | P_Fld(0x1, SHURK2_SELPH_DQ3_DLY_R2DQM3)
                | P_Fld(0x1, SHURK2_SELPH_DQ3_DLY_R2DQM2)
                | P_Fld(0x1, SHURK2_SELPH_DQ3_DLY_R2DQM1)
                | P_Fld(0x1, SHURK2_SELPH_DQ3_DLY_R2DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x3, SHU_DQSG_RETRY_R_DQSIENLAT)
                | P_Fld(0x0, SHU_DQSG_RETRY_R_DDR1866_PLUS));

    ////DDRPHY0-SHU3
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5, 0x4, SHU1_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0);
    //vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ6, P_Fld(0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0)
    //            | P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x1, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
                    | P_Fld(0x0, SHU1_B0_DQ7_MIDPI_ENABLE));
    }
    else
    {
        vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
                    | P_Fld(0x0, SHU1_B0_DQ7_R_DMDQMDBI_SHU_B0)
                    | P_Fld(0x1, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
                    | P_Fld(0x0, SHU1_B0_DQ7_MIDPI_ENABLE));
    }
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, 0x4, SHU1_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1);
    //vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ6, P_Fld(0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1)
    //            | P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {}
    else
    {
        vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
                    | P_Fld(0x0, SHU1_B1_DQ7_R_DMDQMDBI_SHU_B1));
    }
    //DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    //vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6, P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA)
    //            | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA));
    //vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6+(1<<POS_BANK_NUM), P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA)
    //            | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA));
    //DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, 0x3d00, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, 0x3d00, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ2, P_Fld(0xc, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0)
                | P_Fld(0xc, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ3, P_Fld(0xc, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0)
                | P_Fld(0xc, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ4, P_Fld(0xc, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0)
                | P_Fld(0xc, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ5, P_Fld(0xc, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0)
                | P_Fld(0xc, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ6, P_Fld(0x1d, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0x15, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0xc, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ7, P_Fld(0x1a, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0)
                | P_Fld(0x1a, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ2, P_Fld(0xc, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0xc, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ3, P_Fld(0xc, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0xc, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ4, P_Fld(0xc, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0xc, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ5, P_Fld(0xc, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0xc, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6, P_Fld(0x1d, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0x15, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0xc, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ7, P_Fld(0x1a, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1)
                | P_Fld(0x1a, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ0, P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B0_DQ1, 0xd, SHU1_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ2, P_Fld(0xd, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_R_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ3, P_Fld(0xd, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_R_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ4, P_Fld(0xd, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_R_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ5, P_Fld(0xd, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_R_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ6, P_Fld(0x1e, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0x16, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ7, P_Fld(0x1e, SHU1_R1_B0_DQ7_RK1_ARPI_DQM_B0)
                | P_Fld(0x1e, SHU1_R1_B0_DQ7_RK1_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0, P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1, 0xd, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ2, P_Fld(0xd, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ3, P_Fld(0xd, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ4, P_Fld(0xd, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ5, P_Fld(0xd, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6, P_Fld(0x1e, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0x16, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ7, P_Fld(0x1e, SHU1_R1_B1_DQ7_RK1_ARPI_DQM_B1)
                | P_Fld(0x1e, SHU1_R1_B1_DQ7_RK1_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B0_DQ0, P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ7_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ6_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ5_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ4_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ3_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ2_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ1_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ0_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B0_DQ1, 0xd, SHU1_R2_B0_DQ1_RK2_TX_ARDQM0_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B0_DQ7, P_Fld(0x20, SHU1_R2_B0_DQ7_RK2_ARPI_DQM_B0)
                | P_Fld(0x20, SHU1_R2_B0_DQ7_RK2_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B1_DQ0, P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ7_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ6_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ5_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ4_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ3_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ2_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ1_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B1_DQ1, 0xd, SHU1_R2_B1_DQ1_RK2_TX_ARDQM0_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B1_DQ7, P_Fld(0x20, SHU1_R2_B1_DQ7_RK2_ARPI_DQM_B1)
                | P_Fld(0x20, SHU1_R2_B1_DQ7_RK2_ARPI_DQ_B1));

}

static void DramcSetting_Olympus_LP4_ByteMode_DDR800(DRAMC_CTX_T *p)
{
    ////DRAMC0-SHU3
    vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK2, P_Fld(0x1a, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO0)
                | P_Fld(0x10, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO1)
                | P_Fld(0x0, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO2)
                | P_Fld(0x26, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO3));
    vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK3, P_Fld(0x13, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO4)
                | P_Fld(0x0, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO5)
                | P_Fld(0x40, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO6)
                | P_Fld(0x35, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO7));
    vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK4, P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO8)
                | P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO9)
                | P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO10)
                | P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO11));
    vIO32WriteFldAlign(DRAMC_REG_RK0_PRE_TDQSCK1, 0xa, RK0_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R0);
    vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK2, P_Fld(0x0, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R0)
                | P_Fld(0x0, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R0)
                | P_Fld(0xb, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R0)
                | P_Fld(0xd, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R0));
    vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK3, P_Fld(0x0, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R0)
                | P_Fld(0x11, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R0)
                | P_Fld(0xe, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R0));
    vIO32WriteFldAlign(DRAMC_REG_RK0_PRE_TDQSCK4, 0xa, RK0_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R0);
    vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK5, P_Fld(0x0, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R0)
                | P_Fld(0x0, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R0)
                | P_Fld(0xb, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R0)
                | P_Fld(0xd, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R0));
    vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK6, P_Fld(0x0, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R0)
                | P_Fld(0x11, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R0)
                | P_Fld(0xe, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R0));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK1, P_Fld(0x8, RK1_PRE_TDQSCK1_TDQSCK_PIFREQ2_B0R1)
                | P_Fld(0x10, RK1_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK2, P_Fld(0x0, RK1_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R1)
                | P_Fld(0x0, RK1_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R1)
                | P_Fld(0xe, RK1_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R1)
                | P_Fld(0x10, RK1_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK3, P_Fld(0x0, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R1)
                | P_Fld(0x14, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R1)
                | P_Fld(0x14, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK4, P_Fld(0x8, RK1_PRE_TDQSCK4_TDQSCK_PIFREQ2_B1R1)
                | P_Fld(0x10, RK1_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK5, P_Fld(0x0, RK1_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R1)
                | P_Fld(0x0, RK1_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R1)
                | P_Fld(0xe, RK1_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R1)
                | P_Fld(0x10, RK1_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK6, P_Fld(0x0, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R1)
                | P_Fld(0x14, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R1)
                | P_Fld(0x14, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R1));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM0, P_Fld(0x3, SHU_ACTIM0_TRCD)
                    | P_Fld(0x0, SHU_ACTIM0_TRRD)
                    | P_Fld(0x8, SHU_ACTIM0_TWR)
                    | P_Fld(0x5, SHU_ACTIM0_TWTR));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM1, P_Fld(0x4, SHU_ACTIM1_TRC)
                    | P_Fld(0x0, SHU_ACTIM1_TRAS)
                    | P_Fld(0x2, SHU_ACTIM1_TRP)
                    | P_Fld(0x0, SHU_ACTIM1_TRPAB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM2, P_Fld(0x0, SHU_ACTIM2_TFAW)
                    | P_Fld(0x3, SHU_ACTIM2_TR2W)
                    | P_Fld(0x1, SHU_ACTIM2_TRTP)
                    | P_Fld(0x0, SHU_ACTIM2_TXP));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM3, P_Fld(0x2c, SHU_ACTIM3_TRFC)
                    | P_Fld(0x10, SHU_ACTIM3_TRFCPB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM4, P_Fld(0x10, SHU_ACTIM4_TZQCS)
                    | P_Fld(0x30, SHU_ACTIM4_REFCNT)
                    | P_Fld(0x3d, SHU_ACTIM4_TXREFCNT));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM5, P_Fld(0x3, SHU_ACTIM5_TMRR2W)
                    | P_Fld(0x7, SHU_ACTIM5_TWTPD)
                    | P_Fld(0x4, SHU_ACTIM5_TR2PD));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x7, SHU_ACTIM_XRT_XRTW2W)
                    | P_Fld(0x3, SHU_ACTIM_XRT_XRTR2W));
        vIO32WriteFldMulti(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(0x1, SHU_AC_TIME_05T_TWTR_M05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TR2W_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TWTPD_M05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TR2PD_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TFAW_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRRD_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TWR_M05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRAS_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRPAB_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRCD_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRTP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TXP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRFC_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRC_05T));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM0, P_Fld(0x3, SHU_ACTIM0_TRCD)
                    | P_Fld(0x0, SHU_ACTIM0_TRRD)
                    | P_Fld(0x7, SHU_ACTIM0_TWR)
                    | P_Fld(0x4, SHU_ACTIM0_TWTR));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM1, P_Fld(0x4, SHU_ACTIM1_TRC)
                    | P_Fld(0x0, SHU_ACTIM1_TRAS)
                    | P_Fld(0x2, SHU_ACTIM1_TRP)
                    | P_Fld(0x0, SHU_ACTIM1_TRPAB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM2, P_Fld(0x0, SHU_ACTIM2_TFAW)
                    | P_Fld(0x2, SHU_ACTIM2_TR2W)
                    | P_Fld(0x1, SHU_ACTIM2_TRTP)
                    | P_Fld(0x0, SHU_ACTIM2_TXP));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM3, P_Fld(0x2c, SHU_ACTIM3_TRFC)
                    | P_Fld(0x10, SHU_ACTIM3_TRFCPB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM4, P_Fld(0x10, SHU_ACTIM4_TZQCS)
                    | P_Fld(0x30, SHU_ACTIM4_REFCNT)
                    | P_Fld(0x3d, SHU_ACTIM4_TXREFCNT));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM5, P_Fld(0x3, SHU_ACTIM5_TMRR2W)
                    | P_Fld(0x6, SHU_ACTIM5_TWTPD)
                    | P_Fld(0x6, SHU_ACTIM5_TR2PD));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x7, SHU_ACTIM_XRT_XRTW2W)
                    | P_Fld(0x3, SHU_ACTIM_XRT_XRTR2W));
        vIO32WriteFldMulti(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(0x1, SHU_AC_TIME_05T_TWTR_M05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TWTPD_M05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TFAW_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRRD_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TWR_M05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRAS_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRPAB_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRCD_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRTP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TXP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRFC_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRC_05T));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHU_CONF1, P_Fld(0xa, SHU_CONF1_DATLAT_DSEL_PHY)
                | P_Fld(0xb, SHU_CONF1_DATLAT_DSEL)
                | P_Fld(0xc, SHU_CONF1_DATLAT));
    vIO32WriteFldAlign(DRAMC_REG_SHU_CONF2, 0x32, SHU_CONF2_FSPCHG_PRDCNT);
    vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x2, SHU_RANKCTL_RANKINCTL_PHY)
                | P_Fld(0x0, SHU_RANKCTL_RANKINCTL_ROOT1)
                | P_Fld(0x0, SHU_RANKCTL_RANKINCTL));
    vIO32WriteFldMulti(DRAMC_REG_SHU_CKECTRL, P_Fld(0x0, SHU_CKECTRL_TCKESRX)
                | P_Fld(0x2, SHU_CKECTRL_CKEPRD));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x0, SHU_ODTCTRL_RODTE)
                | P_Fld(0x0, SHU_ODTCTRL_RODTE2)
                | P_Fld(0x2, SHU_ODTCTRL_RODT)
#ifdef LOOPBACK_TEST
                | P_Fld(0x1, SHU_ODTCTRL_ROEN));
#else
                | P_Fld(0x0, SHU_ODTCTRL_ROEN));
#endif
    vIO32WriteFldMulti(DRAMC_REG_SHU1_DQSOSC_PRD, P_Fld(0x14, SHU1_DQSOSC_PRD_DQSOSCTHRD_DEC)
                | P_Fld(0x14, SHU1_DQSOSC_PRD_DQSOSCTHRD_INC)
                | P_Fld(0xf, SHU1_DQSOSC_PRD_DQSOSC_PRDCNT));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS0, P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS3)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS2)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS1)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS0)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS3)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS2)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS1)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS1, P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS3)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS2)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS1)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS0)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS3)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS2)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS1)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS0));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldAlign(DRAMC_REG_SHU1_WODT, 0x1, SHU1_WODT_WPST2T);
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU1_WODT, P_Fld(0x1, SHU1_WODT_WPST2T)
                    | P_Fld(0x0, SHU1_WODT_DBIWR));
    }
    vIO32WriteFldAlign(DRAMC_REG_SHU_DQS2DQ_TX, 0x4, SHU_DQS2DQ_TX_OE2DQ_OFFSET);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR2, 0x12, SHU_HWSET_MR2_HWSET_MR2_OP);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR13, 0x8, SHU_HWSET_MR13_HWSET_MR13_OP);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_VRCG, 0x0, SHU_HWSET_VRCG_HWSET_VRCG_OP);
    vIO32WriteFldAlign(DRAMC_REG_SHURK0_DQSCTL, 0x2, SHURK0_DQSCTL_DQSINCTL);
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSIEN, P_Fld(0x0, SHURK0_DQSIEN_R0DQS1IEN)
                | P_Fld(0x0, SHURK0_DQSIEN_R0DQS0IEN));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_PI, P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQM_B1)
                | P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQM_B0)
                | P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQ_B1)
                | P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQ_B0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQSOSC, P_Fld(0x2d0, SHU1RK0_DQSOSC_DQSOSC_BASE_RK0_B1)
                | P_Fld(0x2d0, SHU1RK0_DQSOSC_DQSOSC_BASE_RK0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN0, P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x5, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
                | P_Fld(0x5, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
                | P_Fld(0x5, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
                | P_Fld(0x5, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                    | P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                    | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                    | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                    | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ2)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                    | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                    | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                    | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM2)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x0, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                    | P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                    | P_Fld(0x0, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                    | P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                    | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                    | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                    | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ2));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                    | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                    | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                    | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM2));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_OEN_DQ0)
                | P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ3)
                | P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ2)
                | P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ1)
                | P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_OEN_DQM0)
                | P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM3)
                | P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM2)
                | P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM1)
                | P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL1, P_Fld(0x23a, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1)
                | P_Fld(0x23a, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL2, P_Fld(0x23a, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1)
                | P_Fld(0x23a, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL3, P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1_B4TO0)
                | P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0_B4TO0)
                | P_Fld(0xf, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1)
                | P_Fld(0xf, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL4, P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1_B4TO0)
                | P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0_B4TO0)
                | P_Fld(0xf, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1)
                | P_Fld(0xf, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL5, P_Fld(0x23e, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1)
                | P_Fld(0x23e, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));
    vIO32WriteFldAlign(DRAMC_REG_SHURK1_DQSCTL, 0x2, SHURK1_DQSCTL_R1DQSINCTL);
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSIEN, P_Fld(0x0, SHURK1_DQSIEN_R1DQS1IEN)
                | P_Fld(0x0, SHURK1_DQSIEN_R1DQS0IEN));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_PI, P_Fld(0x1e, SHU1RK1_PI_RK1_ARPI_DQM_B1)
                | P_Fld(0x1e, SHU1RK1_PI_RK1_ARPI_DQM_B0)
                | P_Fld(0x1e, SHU1RK1_PI_RK1_ARPI_DQ_B1)
                | P_Fld(0x1e, SHU1RK1_PI_RK1_ARPI_DQ_B0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQSOSC, P_Fld(0x24e, SHU1RK1_DQSOSC_DQSOSC_BASE_RK1_B1)
                | P_Fld(0x24e, SHU1RK1_DQSOSC_DQSOSC_BASE_RK1));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN_P1)
                | P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN)
                | P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN_P1)
                | P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x0, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                    | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x5, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                    | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                    | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                    | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                    | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                    | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                    | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                    | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                    | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                    | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                    | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                    | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                    | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0)
                | P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1DQ3)
                | P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1DQ2)
                | P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1DQ1)
                | P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0)
                | P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1DQM3)
                | P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1DQM2)
                | P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1DQM1)
                | P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL1, P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1)
                | P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL2, P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1)
                | P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL3, P_Fld(0x1e, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1_B4TO0)
                | P_Fld(0x1e, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0_B4TO0)
                | P_Fld(0xf, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1)
                | P_Fld(0xf, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL4, P_Fld(0x1e, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1_B4TO0)
                | P_Fld(0x1e, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0_B4TO0)
                | P_Fld(0xf, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1)
                | P_Fld(0xf, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL5, P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1)
                | P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0));
    vIO32WriteFldAlign(DRAMC_REG_SHURK2_DQSCTL, 0x2, SHURK2_DQSCTL_R2DQSINCTL);
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_DQSIEN, P_Fld(0x0, SHURK2_DQSIEN_R2DQS3IEN)
                | P_Fld(0x0, SHURK2_DQSIEN_R2DQS2IEN)
                | P_Fld(0x0, SHURK2_DQSIEN_R2DQS1IEN)
                | P_Fld(0x0, SHURK2_DQSIEN_R2DQS0IEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_ODTEN0, P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B3_R2RODTEN_P1)
                | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B3_R2RODTEN)
                | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B2_R2RODTEN_P1)
                | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B2_R2RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_ODTEN1, P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B3_R2RODTEN_P1)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B3_R2RODTEN)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B2_R2RODTEN_P1)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B2_R2RODTEN)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B1_R2RODTEN_P1)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B1_R2RODTEN)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B0_R2RODTEN_P1)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B0_R2RODTEN));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQSG0, P_Fld(0x3, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED_P1)
                    | P_Fld(0x3, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED)
                    | P_Fld(0x3, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED_P1)
                    | P_Fld(0x3, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQSG0, P_Fld(0x2, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED)
                    | P_Fld(0x2, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQSG1, P_Fld(0x4, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS1_GATED_P1)
                | P_Fld(0x0, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS1_GATED)
                | P_Fld(0x4, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS0_GATED_P1)
                | P_Fld(0x0, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ0, P_Fld(0x1, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ3)
                | P_Fld(0x1, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ2)
                | P_Fld(0x1, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ1)
                | P_Fld(0x1, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ0)
                | P_Fld(0x2, SHURK2_SELPH_DQ0_TX_DLY_R2DQ3)
                | P_Fld(0x2, SHURK2_SELPH_DQ0_TX_DLY_R2DQ2)
                | P_Fld(0x2, SHURK2_SELPH_DQ0_TX_DLY_R2DQ1)
                | P_Fld(0x2, SHURK2_SELPH_DQ0_TX_DLY_R2DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ1, P_Fld(0x1, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM3)
                | P_Fld(0x1, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM2)
                | P_Fld(0x1, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM1)
                | P_Fld(0x1, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM0)
                | P_Fld(0x2, SHURK2_SELPH_DQ1_TX_DLY_R2DQM3)
                | P_Fld(0x2, SHURK2_SELPH_DQ1_TX_DLY_R2DQM2)
                | P_Fld(0x2, SHURK2_SELPH_DQ1_TX_DLY_R2DQM1)
                | P_Fld(0x2, SHURK2_SELPH_DQ1_TX_DLY_R2DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ2, P_Fld(0x7, SHURK2_SELPH_DQ2_DLY_R2OEN_DQ3)
                | P_Fld(0x7, SHURK2_SELPH_DQ2_DLY_R2OEN_DQ2)
                | P_Fld(0x7, SHURK2_SELPH_DQ2_DLY_R2OEN_DQ1)
                | P_Fld(0x7, SHURK2_SELPH_DQ2_DLY_R2OEN_DQ0)
                | P_Fld(0x1, SHURK2_SELPH_DQ2_DLY_R2DQ3)
                | P_Fld(0x1, SHURK2_SELPH_DQ2_DLY_R2DQ2)
                | P_Fld(0x1, SHURK2_SELPH_DQ2_DLY_R2DQ1)
                | P_Fld(0x1, SHURK2_SELPH_DQ2_DLY_R2DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ3, P_Fld(0x7, SHURK2_SELPH_DQ3_DLY_R2OEN_DQM3)
                | P_Fld(0x7, SHURK2_SELPH_DQ3_DLY_R2OEN_DQM2)
                | P_Fld(0x7, SHURK2_SELPH_DQ3_DLY_R2OEN_DQM1)
                | P_Fld(0x7, SHURK2_SELPH_DQ3_DLY_R2OEN_DQM0)
                | P_Fld(0x1, SHURK2_SELPH_DQ3_DLY_R2DQM3)
                | P_Fld(0x1, SHURK2_SELPH_DQ3_DLY_R2DQM2)
                | P_Fld(0x1, SHURK2_SELPH_DQ3_DLY_R2DQM1)
                | P_Fld(0x1, SHURK2_SELPH_DQ3_DLY_R2DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x3, SHU_DQSG_RETRY_R_DQSIENLAT)
                | P_Fld(0x0, SHU_DQSG_RETRY_R_DDR1866_PLUS));

    ////DDRPHY0-SHU3
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5, 0x4, SHU1_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0);
    //vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ6, P_Fld(0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0)
    //            | P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x1, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
                    | P_Fld(0x0, SHU1_B0_DQ7_MIDPI_ENABLE));
    }
    else
    {
        vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
                    | P_Fld(0x0, SHU1_B0_DQ7_R_DMDQMDBI_SHU_B0)
                    | P_Fld(0x1, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
                    | P_Fld(0x0, SHU1_B0_DQ7_MIDPI_ENABLE));
    }
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, 0x4, SHU1_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1);
    //vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ6, P_Fld(0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1)
    //            | P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {}
    else
    {
        vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
                    | P_Fld(0x0, SHU1_B1_DQ7_R_DMDQMDBI_SHU_B1));
    }
    //DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    //vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6, P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA)
    //            | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA));
    //vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6+(1<<POS_BANK_NUM), P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA)
    //            | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA));
    //DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, 0x3d00, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, 0x3d00, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ2, P_Fld(0xc, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0)
                | P_Fld(0xc, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ3, P_Fld(0xc, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0)
                | P_Fld(0xc, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ4, P_Fld(0xc, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0)
                | P_Fld(0xc, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ5, P_Fld(0xc, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0)
                | P_Fld(0xc, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ6, P_Fld(0x1d, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0x15, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0xc, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ7, P_Fld(0x1a, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0)
                | P_Fld(0x1a, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ2, P_Fld(0xc, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0xc, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ3, P_Fld(0xc, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0xc, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ4, P_Fld(0xc, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0xc, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ5, P_Fld(0xc, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0xc, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6, P_Fld(0x1d, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0x15, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0xc, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ7, P_Fld(0x1a, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1)
                | P_Fld(0x1a, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ0, P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B0_DQ1, 0xd, SHU1_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ2, P_Fld(0xd, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_R_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ3, P_Fld(0xd, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_R_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ4, P_Fld(0xd, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_R_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ5, P_Fld(0xd, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_R_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ6, P_Fld(0x1e, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0x16, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ7, P_Fld(0x1e, SHU1_R1_B0_DQ7_RK1_ARPI_DQM_B0)
                | P_Fld(0x1e, SHU1_R1_B0_DQ7_RK1_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0, P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1, 0xd, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ2, P_Fld(0xd, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ3, P_Fld(0xd, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ4, P_Fld(0xd, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ5, P_Fld(0xd, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6, P_Fld(0x1e, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0x16, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ7, P_Fld(0x1e, SHU1_R1_B1_DQ7_RK1_ARPI_DQM_B1)
                | P_Fld(0x1e, SHU1_R1_B1_DQ7_RK1_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B0_DQ0, P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ7_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ6_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ5_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ4_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ3_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ2_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ1_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ0_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B0_DQ1, 0xd, SHU1_R2_B0_DQ1_RK2_TX_ARDQM0_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B0_DQ7, P_Fld(0x20, SHU1_R2_B0_DQ7_RK2_ARPI_DQM_B0)
                | P_Fld(0x20, SHU1_R2_B0_DQ7_RK2_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B1_DQ0, P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ7_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ6_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ5_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ4_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ3_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ2_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ1_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B1_DQ1, 0xd, SHU1_R2_B1_DQ1_RK2_TX_ARDQM0_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B1_DQ7, P_Fld(0x20, SHU1_R2_B1_DQ7_RK2_ARPI_DQM_B1)
                | P_Fld(0x20, SHU1_R2_B1_DQ7_RK2_ARPI_DQ_B1));

}


/*
CANNOT use DRAMC_WBR :
DDRPHY_CA_DLL_ARPI0
DDRPHY_CA_DLL_ARPI1
DDRPHY_CA_DLL_ARPI3
DDRPHY_CA_DLL_ARPI5
DDRPHY_SHU1_CA_CMD6
*/
static void DramcSetting_Olympus_LP4_ByteMode(DRAMC_CTX_T *p)
{
    U8 u1CAP_SEL;
    U8 u1MIDPICAP_SEL;

    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    //26M
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_CLK_MEM_SEL);
    //chg_mem_en = 1
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_W_CHG_MEM);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_W_CHG_MEM);


    //                   0 ===LP4_3200_intial_setting_shu1 begin===
    //Francis : pin mux issue, need to set CHD
    DramcBroadcastOnOff(BROADCAST_4CHANNELS);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0x0, MISC_SPM_CTRL1_PHY_SPM_CTL1);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2, 0x0, MISC_SPM_CTRL2_PHY_SPM_CTL2);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL0, 0x0, MISC_SPM_CTRL0_PHY_SPM_CTL0);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL2, 0x6003bf, MISC_CG_CTRL2_RG_MEM_DCM_CTL);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL4, 0x11400000, MISC_CG_CTRL4_R_PHY_MCK_CG_CTRL);
    vIO32WriteFldMulti(DDRPHY_SHU1_PLL1, P_Fld(0x1, SHU1_PLL1_R_SHU_AUTO_PLL_MUX)
                | P_Fld(0x7, SHU1_PLL1_SHU1_PLL1_RFU));
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x1, SHU1_B0_DQ7_MIDPI_ENABLE)
                | P_Fld(0x0, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
                | P_Fld(0x1, SHU1_B0_DQ7_R_DMRANKRXDVS_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x1, SHU1_B1_DQ7_R_DMRANKRXDVS_B1);
    //WHITNEY
    vIO32WriteFldMulti(DDRPHY_SHU1_PLL4, P_Fld(0xfe, SHU1_PLL4_RG_RPHYPLL_RESERVED)
                                       | P_Fld(0x0, SHU1_PLL4_RG_RPHYPLL_FS)
                                       | P_Fld(0x0, SHU1_PLL4_RG_RPHYPLL_IBIAS)
                                       | P_Fld(0x0, SHU1_PLL4_RG_RPHYPLL_ICHP));
    vIO32WriteFldMulti(DDRPHY_SHU1_PLL6, P_Fld(0xfe, SHU1_PLL6_RG_RCLRPLL_RESERVED)
                                       | P_Fld(0x0, SHU1_PLL6_RG_RCLRPLL_FS)
                                       | P_Fld(0x0, SHU1_PLL6_RG_RCLRPLL_IBIAS)
                                       | P_Fld(0x0, SHU1_PLL6_RG_RCLRPLL_ICHP));
    vIO32WriteFldAlign(DDRPHY_SSC_SHU1_PLL5, 0x1, SSC_SHU1_PLL5_RG_RPHYPLL_SDM_SSC_PH_INIT);
    vIO32WriteFldAlign(DDRPHY_SSC_SHU1_PLL7, 0x1, SSC_SHU1_PLL7_RG_RCLRPLL_SDM_SSC_PH_INIT);
    vIO32WriteFldMulti(DDRPHY_CA_CMD2, P_Fld(0x0, CA_CMD2_RG_TX_ARCMD_OE_DIS)
                | P_Fld(0x0, CA_CMD2_RG_TX_ARCMD_ODTEN_DIS)
                | P_Fld(0x0, CA_CMD2_RG_TX_ARCLK_OE_DIS)
                | P_Fld(0x0, CA_CMD2_RG_TX_ARCLK_ODTEN_DIS));
    vIO32WriteFldMulti(DDRPHY_B0_DQ2, P_Fld(0x0, B0_DQ2_RG_TX_ARDQ_OE_DIS_B0)
                | P_Fld(0x0, B0_DQ2_RG_TX_ARDQ_ODTEN_DIS_B0)
                | P_Fld(0x0, B0_DQ2_RG_TX_ARDQS0_OE_DIS)
                | P_Fld(0x0, B0_DQ2_RG_TX_ARDQS0_ODTEN_DIS));
    vIO32WriteFldMulti(DDRPHY_B1_DQ2, P_Fld(0x0, B1_DQ2_RG_TX_ARDQ_OE_DIS_B1)
                | P_Fld(0x0, B1_DQ2_RG_TX_ARDQ_ODTEN_DIS_B1)
                | P_Fld(0x0, B1_DQ2_RG_TX_ARDQS1_OE_DIS)
                | P_Fld(0x0, B1_DQ2_RG_TX_ARDQS1_ODTEN_DIS));
    vIO32WriteFldAlign(DDRPHY_MISC_RXDVS1, 0x8000000e, MISC_RXDVS1_R_IN_GATE_EN_LOW_OPT);
    vIO32WriteFldAlign(DDRPHY_MISC_RXDVS2, 0x1, MISC_RXDVS2_R_RXDVS_RDSEL_BUS_LAT);
    vIO32WriteFldAlign(DDRPHY_B0_RXDVS0, 0x1, B0_RXDVS0_R_RX_DLY_TRACK_CG_EN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_RXDVS0, 0x1, B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1);
    vIO32WriteFldAlign(DDRPHY_B0_RXDVS0, 0x1, B0_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B0);
    vIO32WriteFldAlign(DDRPHY_B1_RXDVS0, 0x1, B1_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B1);
    vIO32WriteFldAlign(DDRPHY_R0_B0_RXDVS2, 0x1, R0_B0_RXDVS2_R_RK0_DVS_FDLY_MODE_B0);
    vIO32WriteFldAlign(DDRPHY_R1_B0_RXDVS2, 0x1, R1_B0_RXDVS2_R_RK1_DVS_FDLY_MODE_B0);
    vIO32WriteFldAlign(DDRPHY_R0_B1_RXDVS2, 0x1, R0_B1_RXDVS2_R_RK0_DVS_FDLY_MODE_B1);
    vIO32WriteFldAlign(DDRPHY_R1_B1_RXDVS2, 0x1, R1_B1_RXDVS2_R_RK1_DVS_FDLY_MODE_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5, 0x3, SHU1_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, 0x3, SHU1_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ2, P_Fld(0x12, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ3, P_Fld(0x12, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ4, P_Fld(0x12, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ5, P_Fld(0x12, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ6, P_Fld(0x14, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0xc, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ2, P_Fld(0x13, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ3, P_Fld(0x13, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ4, P_Fld(0x13, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ5, P_Fld(0x13, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ6, P_Fld(0x15, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ2, P_Fld(0x12, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ3, P_Fld(0x12, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ4, P_Fld(0x12, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ5, P_Fld(0x12, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6, P_Fld(0x14, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0xc, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ2, P_Fld(0x13, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ3, P_Fld(0x13, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ4, P_Fld(0x13, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ5, P_Fld(0x13, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6, P_Fld(0x15, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_R0_B0_RXDVS1, P_Fld(0x2, R0_B0_RXDVS1_R_RK0_B0_DVS_TH_LEAD)
                | P_Fld(0x2, R0_B0_RXDVS1_R_RK0_B0_DVS_TH_LAG));
    vIO32WriteFldMulti(DDRPHY_R1_B0_RXDVS1, P_Fld(0x2, R1_B0_RXDVS1_R_RK1_B0_DVS_TH_LEAD)
                | P_Fld(0x2, R1_B0_RXDVS1_R_RK1_B0_DVS_TH_LAG));
    vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS1, P_Fld(0x2, R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LEAD)
                | P_Fld(0x2, R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LAG));
    vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS1, P_Fld(0x2, R1_B1_RXDVS1_R_RK1_B1_DVS_TH_LEAD)
                | P_Fld(0x2, R1_B1_RXDVS1_R_RK1_B1_DVS_TH_LAG));

    vIO32WriteFldMulti(DDRPHY_R0_B0_RXDVS2, P_Fld(0x2, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0)
                | P_Fld(0x1, R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0)
                | P_Fld(0x1, R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0));
    vIO32WriteFldMulti(DDRPHY_R1_B0_RXDVS2, P_Fld(0x2, R1_B0_RXDVS2_R_RK1_DVS_MODE_B0)
                | P_Fld(0x1, R1_B0_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B0)
                | P_Fld(0x1, R1_B0_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B0));
    vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS2, P_Fld(0x2, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1)
                | P_Fld(0x1, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                | P_Fld(0x1, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1));
    vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS2, P_Fld(0x2, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1)
                | P_Fld(0x1, R1_B1_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                | P_Fld(0x1, R1_B1_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B1));

    vIO32WriteFldAlign(DDRPHY_B0_RXDVS0, 0x0, B0_RXDVS0_R_RX_DLY_TRACK_CG_EN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_RXDVS0, 0x0, B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1);
    vIO32WriteFldAlign(DDRPHY_B0_DQ3, 0x1, B0_DQ3_RG_RX_ARDQ_STBEN_RESETB_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ3, 0x1, B1_DQ3_RG_RX_ARDQ_STBEN_RESETB_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ0, P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B0_DQ0, P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ7_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ6_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ5_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ4_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ3_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ2_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ1_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ0_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B0_DQ1, 0xa, SHU1_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B0_DQ1, 0xa, SHU1_R2_B0_DQ1_RK2_TX_ARDQM0_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0, P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B1_DQ0, P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ7_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ6_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ5_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ4_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ3_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ2_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ1_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1, 0xa, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B1_DQ1, 0xa, SHU1_R2_B1_DQ1_RK2_TX_ARDQM0_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ7, P_Fld(0x1f, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0)
                | P_Fld(0x1f, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ7, P_Fld(0x1f, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1)
                | P_Fld(0x1f, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ7, P_Fld(0x1f, SHU1_R1_B0_DQ7_RK1_ARPI_DQM_B0)
                | P_Fld(0x1f, SHU1_R1_B0_DQ7_RK1_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ7, P_Fld(0x1f, SHU1_R1_B1_DQ7_RK1_ARPI_DQM_B1)
                | P_Fld(0x1f, SHU1_R1_B1_DQ7_RK1_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B0_DQ7, P_Fld(0x1f, SHU1_R2_B0_DQ7_RK2_ARPI_DQM_B0)
                | P_Fld(0x1f, SHU1_R2_B0_DQ7_RK2_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B1_DQ7, P_Fld(0x1f, SHU1_R2_B1_DQ7_RK2_ARPI_DQM_B1)
                | P_Fld(0x1f, SHU1_R2_B1_DQ7_RK2_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_B0_DQ4, P_Fld(0x10, B0_DQ4_RG_RX_ARDQS_EYE_R_DLY_B0)
                | P_Fld(0x10, B0_DQ4_RG_RX_ARDQS_EYE_F_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_B0_DQ5, P_Fld(0x1, B0_DQ5_RG_RX_ARDQ_EYE_EN_B0)
                | P_Fld(0x1, B0_DQ5_RG_RX_ARDQ_EYE_SEL_B0)
                | P_Fld(0x1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0)
                | P_Fld(0xe, B0_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B0)
                | P_Fld(0x10, B0_DQ5_B0_DQ5_RFU));
    vIO32WriteFldMulti(DDRPHY_B0_DQ6, P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B0)
                | P_Fld(0x0, B0_DQ6_RG_TX_ARDQ_DDR3_SEL_B0)
                | P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_DDR3_SEL_B0)
                | P_Fld(0x1, B0_DQ6_RG_TX_ARDQ_DDR4_SEL_B0)
                | P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_DDR4_SEL_B0)
                | P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0)
                | P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0)
                | P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0)
                | P_Fld(0x0, B0_DQ6_RG_TX_ARDQ_SER_MODE_B0));
    vIO32WriteFldMulti(DDRPHY_B0_DQ5, P_Fld(0x1, B0_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B0)
                | P_Fld(0x0, B0_DQ5_B0_DQ5_RFU));
    vIO32WriteFldMulti(DDRPHY_B1_DQ4, P_Fld(0x10, B1_DQ4_RG_RX_ARDQS_EYE_R_DLY_B1)
                | P_Fld(0x10, B1_DQ4_RG_RX_ARDQS_EYE_F_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_B1_DQ5, P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_EN_B1)
                | P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_SEL_B1)
                | P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1)
                | P_Fld(0xe, B1_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B1)
                | P_Fld(0x10, B1_DQ5_B1_DQ5_RFU));
    vIO32WriteFldMulti(DDRPHY_B1_DQ6, P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B1)
                | P_Fld(0x0, B1_DQ6_RG_TX_ARDQ_DDR3_SEL_B1)
                | P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_DDR3_SEL_B1)
                | P_Fld(0x1, B1_DQ6_RG_TX_ARDQ_DDR4_SEL_B1)
                | P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_DDR4_SEL_B1)
                | P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1)
                | P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1)
                | P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1)
                | P_Fld(0x0, B1_DQ6_RG_TX_ARDQ_SER_MODE_B1));
    vIO32WriteFldMulti(DDRPHY_B1_DQ5, P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B1)
                | P_Fld(0x0, B1_DQ5_B1_DQ5_RFU));
    vIO32WriteFldMulti(DDRPHY_CA_CMD3, P_Fld(0x1, CA_CMD3_RG_RX_ARCMD_IN_BUFF_EN)
                | P_Fld(0x1, CA_CMD3_RG_ARCMD_RESETB)
                | P_Fld(0x1, CA_CMD3_RG_TX_ARCMD_EN));
    vIO32WriteFldMulti(DDRPHY_CA_CMD6, P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_DDR4_SEL)
                | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_BIAS_VREF_SEL)
                | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN));
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD1, P_Fld(0x1, SHU1_CA_CMD1_RG_TX_ARCMD_DRVN)
                | P_Fld(0x1, SHU1_CA_CMD1_RG_TX_ARCMD_DRVP));
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD2, P_Fld(0x1, SHU1_CA_CMD2_RG_TX_ARCLK_DRVN)
                | P_Fld(0x1, SHU1_CA_CMD2_RG_TX_ARCLK_DRVP));
    //vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD1, P_Fld(0x1, SHU2_CA_CMD1_RG_TX_ARCMD_DRVN)
    //            | P_Fld(0x1, SHU2_CA_CMD1_RG_TX_ARCMD_DRVP));
    //vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD2, P_Fld(0x1, SHU2_CA_CMD2_RG_TX_ARCLK_DRVN)
    //            | P_Fld(0x1, SHU2_CA_CMD2_RG_TX_ARCLK_DRVP));
    vIO32WriteFldAlign(DDRPHY_PLL3, 0x0, PLL3_RG_RPHYPLL_TSTOP_EN);

    vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, 0x0, SHU1_PLL5_RG_RPHYPLL_SDM_FRA_EN);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, 0x0, SHU1_PLL5_RG_RPHYPLL_SDM_FRA_EN);
    if(p->frequency==1600 || p->frequency==800)
    {
        vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, 0x7b00, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
        vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, 0x7b00, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
    }
    else if(p->frequency==1333)
    {
        vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, 0x6600, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
        vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, 0x6600, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
    }
    else if(p->frequency==1200)
    {

    }

#if 0//ORIGINAL_PLL_INIT
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI0, 0x1, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI0+(1<<POS_BANK_NUM), 0x1, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI0, 0x1, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI0, 0x1, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
#endif
    vIO32WriteFldAlign(DDRPHY_B0_DQ3, 0x1, B0_DQ3_RG_ARDQ_RESETB_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ3, 0x1, B1_DQ3_RG_ARDQ_RESETB_B1);

    mcDELAY_US(1);

    //Ref clock should be 20M~30M, if MPLL=52M, Pre-divider should be set to 1
    vIO32WriteFldMulti(DDRPHY_SHU1_PLL8, P_Fld(0x0, SHU1_PLL8_RG_RPHYPLL_POSDIV) | P_Fld(0x1, SHU1_PLL8_RG_RPHYPLL_PREDIV));
    //vIO32WriteFldAlign(DDRPHY_SHU2_PLL8, 0x0, SHU2_PLL8_RG_RPHYPLL_POSDIV);
    //vIO32WriteFldAlign(DDRPHY_SHU3_PLL8, 0x0, SHU3_PLL8_RG_RPHYPLL_POSDIV);
    //vIO32WriteFldAlign(DDRPHY_SHU4_PLL8, 0x0, SHU4_PLL8_RG_RPHYPLL_POSDIV);

    mcDELAY_US(1);

    vIO32WriteFldMulti(DDRPHY_SHU1_PLL9, P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_MONCK_EN)
                | P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_MONVC_EN)
                | P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_LVROD_EN)
                | P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_RST_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_PLL11, P_Fld(0x0, SHU1_PLL11_RG_RCLRPLL_MONCK_EN)
                | P_Fld(0x0, SHU1_PLL11_RG_RCLRPLL_MONVC_EN)
                | P_Fld(0x0, SHU1_PLL11_RG_RCLRPLL_LVROD_EN)
                | P_Fld(0x0, SHU1_PLL11_RG_RCLRPLL_RST_DLY));

    mcDELAY_US(1);

    //Ref clock should be 20M~30M, if MPLL=52M, Pre-divider should be set to 1
    vIO32WriteFldMulti(DDRPHY_SHU1_PLL10, P_Fld(0x0, SHU1_PLL10_RG_RCLRPLL_POSDIV) | P_Fld(0x1, SHU1_PLL10_RG_RCLRPLL_PREDIV));
    //vIO32WriteFldAlign(DDRPHY_SHU2_PLL10, 0x0, SHU2_PLL10_RG_RCLRPLL_POSDIV);
    //vIO32WriteFldAlign(DDRPHY_SHU3_PLL10, 0x0, SHU3_PLL10_RG_RCLRPLL_POSDIV);
    //vIO32WriteFldAlign(DDRPHY_SHU4_PLL10, 0x0, SHU4_PLL10_RG_RCLRPLL_POSDIV);

    mcDELAY_US(1);

    ///TODO: PLL EN
#if 0//ORIGINAL_PLL_INIT
    vIO32WriteFldAlign(DDRPHY_PLL1, 0x1, PLL1_RG_RPHYPLL_EN);
    vIO32WriteFldAlign(DDRPHY_PLL2, 0x1, PLL2_RG_RCLRPLL_EN);
    mcDELAY_US(100);
#endif


    ///TODO: MIDPI Init 1
    vIO32WriteFldMulti(DDRPHY_PLL4, P_Fld(0x0, PLL4_RG_RPHYPLL_AD_MCK8X_EN)
                | P_Fld(0x1, PLL4_PLL4_RFU)
                | P_Fld(0x1, PLL4_RG_RPHYPLL_MCK8X_SEL));

#if 0//ORIGINAL_PLL_INIT
    vIO32WriteFldMulti(DDRPHY_PLL4, P_Fld(0x1, PLL4_RG_RPHYPLL_ADA_MCK8X_EN)
                | P_Fld(0x1, PLL4_RG_RPHYPLL_RESETB));
#endif


    mcDELAY_US(1);

    vIO32WriteFldAlign(DDRPHY_SHU1_PLL0, 0x3, SHU1_PLL0_RG_RPHYPLL_TOP_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU2_PLL0, 0x3, SHU2_PLL0_RG_RPHYPLL_TOP_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU3_PLL0, 0x3, SHU3_PLL0_RG_RPHYPLL_TOP_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU4_PLL0, 0x3, SHU4_PLL0_RG_RPHYPLL_TOP_REV);

    mcDELAY_US(1);

#if 0//move to DDRPhyPLLSetting()
#if (fcFOR_CHIP_ID == fcWhitney)
    if(p->frequency<=400)//DDR800
    {
        u1CAP_SEL = 0x2;
    }
    else if(p->frequency<=800)//DDR1600
    {
        u1CAP_SEL = 0x1;
    }
    else//DDR2667,3200,3733
    {
        u1CAP_SEL = 0x0;
    }
#endif


#if (fcFOR_CHIP_ID == fcWhitney)
    if(p->frequency<=1333)
    {
        u1MIDPICAP_SEL = 0x2;
    }
    else if(p->frequency<=1600)
    {
        u1MIDPICAP_SEL = 0x1;
    }
    else//3733
    {
        u1MIDPICAP_SEL = 0x0;
    }
#endif


    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ6, P_Fld(0x2, SHU1_B0_DQ6_RG_ARPI_MIDPI_VTH_SEL_B0)
                | P_Fld(u1CAP_SEL, SHU1_B0_DQ6_RG_ARPI_CAP_SEL_B0)
                | P_Fld(u1MIDPICAP_SEL, SHU1_B0_DQ6_RG_ARPI_MIDPI_CAP_SEL_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ6, P_Fld(0x2, SHU1_B1_DQ6_RG_ARPI_MIDPI_VTH_SEL_B1)
                | P_Fld(u1CAP_SEL, SHU1_B1_DQ6_RG_ARPI_CAP_SEL_B1)
                | P_Fld(u1MIDPICAP_SEL, SHU1_B1_DQ6_RG_ARPI_MIDPI_CAP_SEL_B1));

    //DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6, P_Fld(0x2, SHU1_CA_CMD6_RG_ARPI_MIDPI_VTH_SEL_CA)
                | P_Fld(u1CAP_SEL, SHU1_CA_CMD6_RG_ARPI_CAP_SEL_CA)
                | P_Fld(u1MIDPICAP_SEL, SHU1_CA_CMD6_RG_ARPI_MIDPI_CAP_SEL_CA));
    //vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6+(1<<POS_BANK_NUM), P_Fld(0x2, SHU1_CA_CMD6_RG_ARPI_MIDPI_VTH_SEL_CA)
    //            | P_Fld(u1CAP_SEL, SHU1_CA_CMD6_RG_ARPI_CAP_SEL_CA)
    //            | P_Fld(u1MIDPICAP_SEL, SHU1_CA_CMD6_RG_ARPI_MIDPI_CAP_SEL_CA));
    //DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    mcDELAY_US(1);
#endif

    ///TODO: MIDPI Init 2
#if 0//ORIGINAL_PLL_INIT
    if(p->frequency>=1200)//DDR-2400, DDR-2667, DDR-3200, DDR3667
    {
        vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ6, P_Fld(0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0)
                    | P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0));
        vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ6, P_Fld(0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1)
                    | P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1));

        DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
            vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6, P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA)
                        | P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA));
            vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6+(1<<POS_BANK_NUM), P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA)
                        | P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA));
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    }
    else
    {
        vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ6, P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0)
                    | P_Fld(0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0));
        vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ6, P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1)
                    | P_Fld(0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1));

        DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
        vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6, P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA)
                    | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA));
        vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6+(1<<POS_BANK_NUM), P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA)
                    | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA));
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    }
#endif

    mcDELAY_US(1);

    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI1, 0x1, CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA);


    vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI3, P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA)
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_FB_EN_CA)
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CS_EN)
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CLK_EN)
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CMD_EN));
    vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI3, P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_FB_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQS_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQM_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQ_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DLL_ARPI3, P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_FB_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQS_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQM_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQ_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B1));
    vIO32WriteFldMulti(DDRPHY_B0_DQ3, P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_STBENCMP_EN_B0)
                | P_Fld(0x1, B0_DQ3_RG_TX_ARDQ_EN_B0)
                | P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DQ3, P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_STBENCMP_EN_B1)
                | P_Fld(0x1, B1_DQ3_RG_TX_ARDQ_EN_B1)
                | P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1));

    vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI5, P_Fld(0x0, CA_DLL_ARPI5_RG_ARDLL_PHDET_OUT_SEL_CA)
                | P_Fld(0x0, CA_DLL_ARPI5_RG_ARDLL_PHDET_IN_SWAP_CA)
                | P_Fld(0x6, CA_DLL_ARPI5_RG_ARDLL_GAIN_CA)
                | P_Fld(0x9, CA_DLL_ARPI5_RG_ARDLL_IDLECNT_CA)
                | P_Fld(0x8, CA_DLL_ARPI5_RG_ARDLL_P_GAIN_CA)
                | P_Fld(0x1, CA_DLL_ARPI5_RG_ARDLL_PHJUMP_EN_CA)
                | P_Fld(0x1, CA_DLL_ARPI5_RG_ARDLL_PHDIV_CA));

    vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI5, P_Fld(0x1, B0_DLL_ARPI5_RG_ARDLL_PHDET_IN_SWAP_B0)
                | P_Fld(0x7, B0_DLL_ARPI5_RG_ARDLL_GAIN_B0)
                | P_Fld(0x7, B0_DLL_ARPI5_RG_ARDLL_IDLECNT_B0)
                | P_Fld(0x8, B0_DLL_ARPI5_RG_ARDLL_P_GAIN_B0)
                | P_Fld(0x1, B0_DLL_ARPI5_RG_ARDLL_PHJUMP_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI5_RG_ARDLL_PHDIV_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DLL_ARPI5, P_Fld(0x1, B1_DLL_ARPI5_RG_ARDLL_PHDET_IN_SWAP_B1)
                | P_Fld(0x7, B1_DLL_ARPI5_RG_ARDLL_GAIN_B1)
                | P_Fld(0x7, B1_DLL_ARPI5_RG_ARDLL_IDLECNT_B1)
                | P_Fld(0x8, B1_DLL_ARPI5_RG_ARDLL_P_GAIN_B1)
                | P_Fld(0x1, B1_DLL_ARPI5_RG_ARDLL_PHJUMP_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI5_RG_ARDLL_PHDIV_B1));
    vIO32WriteFldMulti(DDRPHY_CA_CMD8, P_Fld(0x1, CA_CMD8_RG_TX_RRESETB_DDR4_SEL)
                | P_Fld(0xF, CA_CMD8_RG_RRESETB_DRVN)
                | P_Fld(0xF, CA_CMD8_RG_RRESETB_DRVP)); // Darren
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD5, 0x0, SHU1_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
    //vIO32WriteFldAlign(DDRPHY_SHU2_CA_CMD5, 0x0, SHU2_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
    //vIO32WriteFldAlign(DDRPHY_SHU3_CA_CMD5, 0x0, SHU3_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
    //vIO32WriteFldAlign(DDRPHY_SHU4_CA_CMD5, 0x0, SHU4_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD0, P_Fld(0x1, SHU1_CA_CMD0_RG_TX_ARCMD_PRE_EN)
                | P_Fld(0x4, SHU1_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
                | P_Fld(0x1, SHU1_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    //vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD0, P_Fld(0x1, SHU2_CA_CMD0_RG_TX_ARCMD_PRE_EN)
    //            | P_Fld(0x4, SHU2_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
    //            | P_Fld(0x1, SHU2_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    //vIO32WriteFldMulti(DDRPHY_SHU3_CA_CMD0, P_Fld(0x1, SHU3_CA_CMD0_RG_TX_ARCMD_PRE_EN)
    //            | P_Fld(0x4, SHU3_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
    //            | P_Fld(0x1, SHU3_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    //vIO32WriteFldMulti(DDRPHY_SHU4_CA_CMD0, P_Fld(0x1, SHU4_CA_CMD0_RG_TX_ARCMD_PRE_EN)
    //            | P_Fld(0x4, SHU4_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
    //            | P_Fld(0x1, SHU4_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6, 0xB, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA);//WHITNEY
    //vIO32WriteFldAlign(DDRPHY_SHU2_CA_CMD6, 0x3, SHU2_CA_CMD6_RG_ARPI_RESERVE_CA);
    //vIO32WriteFldAlign(DDRPHY_SHU3_CA_CMD6, 0x3, SHU3_CA_CMD6_RG_ARPI_RESERVE_CA);
    //vIO32WriteFldAlign(DDRPHY_SHU4_CA_CMD6, 0x3, SHU4_CA_CMD6_RG_ARPI_RESERVE_CA);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD7, 0x4e1, SHU1_CA_CMD7_RG_ARCMD_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU2_CA_CMD7, 0x4e1, SHU2_CA_CMD7_RG_ARCMD_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU3_CA_CMD7, 0x4e1, SHU3_CA_CMD7_RG_ARCMD_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU4_CA_CMD7, 0x4e1, SHU4_CA_CMD7_RG_ARCMD_REV);
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x20, SHU1_B0_DQ7_RG_ARDQ_REV_B0)
                | P_Fld(0x1, SHU1_B0_DQ7_DQ_REV_B0_BIT_05));
    //vIO32WriteFldAlign(DDRPHY_SHU2_B0_DQ7, 0x20, SHU2_B0_DQ7_RG_ARDQ_REV_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU3_B0_DQ7, 0x20, SHU3_B0_DQ7_RG_ARDQ_REV_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU4_B0_DQ7, 0x20, SHU4_B0_DQ7_RG_ARDQ_REV_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x20, SHU1_B1_DQ7_RG_ARDQ_REV_B1)
                | P_Fld(0x1, SHU1_B1_DQ7_DQ_REV_B1_BIT_05));
    //vIO32WriteFldAlign(DDRPHY_SHU2_B1_DQ7, 0x20, SHU2_B1_DQ7_RG_ARDQ_REV_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU3_B1_DQ7, 0x20, SHU3_B1_DQ7_RG_ARDQ_REV_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU4_B1_DQ7, 0x20, SHU4_B1_DQ7_RG_ARDQ_REV_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ6, 0x9, SHU1_B0_DQ6_RG_ARPI_RESERVE_B0);//WHITNEY
    //vIO32WriteFldAlign(DDRPHY_SHU2_B0_DQ6, 0x1, SHU2_B0_DQ6_RG_ARPI_RESERVE_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU3_B0_DQ6, 0x1, SHU3_B0_DQ6_RG_ARPI_RESERVE_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU4_B0_DQ6, 0x1, SHU4_B0_DQ6_RG_ARPI_RESERVE_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ6, 0x9, SHU1_B1_DQ6_RG_ARPI_RESERVE_B1);//WHITNEY
    //vIO32WriteFldAlign(DDRPHY_SHU2_B1_DQ6, 0x1, SHU2_B1_DQ6_RG_ARPI_RESERVE_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU3_B1_DQ6, 0x1, SHU3_B1_DQ6_RG_ARPI_RESERVE_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU4_B1_DQ6, 0x1, SHU4_B1_DQ6_RG_ARPI_RESERVE_B1);
    vIO32WriteFldMulti(DDRPHY_MISC_SHU_OPT, P_Fld(0x1, MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN)
                | P_Fld(0x1, MISC_SHU_OPT_R_CA_SHU_PHY_GATING_RESETB_SPM_EN)
                | P_Fld(0x2, MISC_SHU_OPT_R_DQB1_SHU_PHDET_SPM_EN)
                | P_Fld(0x1, MISC_SHU_OPT_R_DQB1_SHU_PHY_GATING_RESETB_SPM_EN)
                | P_Fld(0x2, MISC_SHU_OPT_R_DQB0_SHU_PHDET_SPM_EN)
                | P_Fld(0x1, MISC_SHU_OPT_R_DQB0_SHU_PHY_GATING_RESETB_SPM_EN));

    mcDELAY_US(9);

    vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI0, P_Fld(0x1, CA_DLL_ARPI0_RG_ARDLL_PD_CK_SEL_CA) | P_Fld(0x0, CA_DLL_ARPI0_RG_ARDLL_FASTPJ_CK_SEL_CA));
    vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI0, 0x1, B0_DLL_ARPI0_RG_ARDLL_FASTPJ_CK_SEL_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI0, 0x1, B1_DLL_ARPI0_RG_ARDLL_FASTPJ_CK_SEL_B1);

    mcDELAY_US(1);

    vIO32WriteFldAlign(DDRPHY_PLL2, 0x0, PLL2_RG_RCLRPLL_EN);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL4, 0xff, SHU1_PLL4_RG_RPHYPLL_RESERVED);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL6, 0xff, SHU1_PLL6_RG_RCLRPLL_RESERVED);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0xf, MISC_CG_CTRL0_CLK_MEM_DFS_CFG);
#if 0//ORIGINAL_PLL_INIT
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_CLK_MEM_SEL);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_W_CHG_MEM);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_W_CHG_MEM);
#endif

    mcDELAY_US(1);


    ///TODO: DLL EN
#if 0//ORIGINAL_PLL_INIT
    //*((UINT32P)(DDRPHY0AO_BASE + 0x0194)) = 0x00698e08;
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI5, 0x1, CA_DLL_ARPI5_RG_ARDLL_PHDET_EN_CA);
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI5+(1<<POS_BANK_NUM), 0x1, CA_DLL_ARPI5_RG_ARDLL_PHDET_EN_CA);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    mcDELAY_US(1);

    vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI5, 0x1, B0_DLL_ARPI5_RG_ARDLL_PHDET_EN_B0);
    mcDELAY_US(1);
    vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI5, 0x1, B1_DLL_ARPI5_RG_ARDLL_PHDET_EN_B1);
    mcDELAY_US(1);
#endif

#if !ORIGINAL_PLL_INIT
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    DDRPhyPLLSetting(p);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
#endif

    vIO32WriteFldAlign(DDRPHY_B0_DQ3, 0x1, B0_DQ3_RG_RX_ARDQS0_STBEN_RESETB);
    vIO32WriteFldAlign(DDRPHY_B1_DQ3, 0x1, B1_DQ3_RG_RX_ARDQS1_STBEN_RESETB);
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSIEN, P_Fld(0xf, SHURK1_DQSIEN_R1DQS3IEN)
                | P_Fld(0xf, SHURK1_DQSIEN_R1DQS2IEN)
                | P_Fld(0xf, SHURK1_DQSIEN_R1DQS1IEN)
                | P_Fld(0xf, SHURK1_DQSIEN_R1DQS0IEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_DQSIEN, P_Fld(0xf, SHURK2_DQSIEN_R2DQS3IEN)
                | P_Fld(0xf, SHURK2_DQSIEN_R2DQS2IEN)
                | P_Fld(0xf, SHURK2_DQSIEN_R2DQS1IEN)
                | P_Fld(0xf, SHURK2_DQSIEN_R2DQS0IEN));
    vIO32WriteFldMulti(DRAMC_REG_STBCAL1, P_Fld(0x1, STBCAL1_DLLFRZ_MON_PBREF_OPT)
                | P_Fld(0x1, STBCAL1_STB_FLAGCLR)
                | P_Fld(0x1, STBCAL1_STBCNT_MODESEL));
    vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x1, SHU_DQSG_RETRY_R_RETRY_USE_BURST_MDOE)
                | P_Fld(0x1, SHU_DQSG_RETRY_R_RDY_SEL_DLE)
                | P_Fld(0x6, SHU_DQSG_RETRY_R_DQSIENLAT)
                | P_Fld(0x1, SHU_DQSG_RETRY_R_RETRY_ONCE));
    vIO32WriteFldMulti(DRAMC_REG_SHU1_DRVING1, P_Fld(0x14a, SHU1_DRVING1_DQSDRV2)
                | P_Fld(0x14a, SHU1_DRVING1_DQSDRV1)
                | P_Fld(0x14a, SHU1_DRVING1_DQDRV2));
    vIO32WriteFldMulti(DRAMC_REG_SHU1_DRVING2, P_Fld(0x14a, SHU1_DRVING2_DQDRV1)
                | P_Fld(0x14a, SHU1_DRVING2_CMDDRV2)
                | P_Fld(0x14a, SHU1_DRVING2_CMDDRV1));
    vIO32WriteFldMulti(DRAMC_REG_SHU1_DRVING3, P_Fld(0x14a, SHU1_DRVING3_DQSODT2)
                | P_Fld(0x14a, SHU1_DRVING3_DQSODT1)
                | P_Fld(0x14a, SHU1_DRVING3_DQODT2));
    vIO32WriteFldMulti(DRAMC_REG_SHU1_DRVING4, P_Fld(0x14a, SHU1_DRVING4_DQODT1)
                | P_Fld(0x14a, SHU1_DRVING4_CMDODT2)
                | P_Fld(0x14a, SHU1_DRVING4_CMDODT1));
    /*
    vIO32WriteFldMulti(DRAMC_REG_SHU2_DRVING1, P_Fld(0x14a, SHU2_DRVING1_DQSDRV2)
                | P_Fld(0x14a, SHU2_DRVING1_DQSDRV1)
                | P_Fld(0x14a, SHU2_DRVING1_DQDRV2));
    vIO32WriteFldMulti(DRAMC_REG_SHU2_DRVING2, P_Fld(0x14a, SHU2_DRVING2_DQDRV1)
                | P_Fld(0x14a, SHU2_DRVING2_CMDDRV2)
                | P_Fld(0x14a, SHU2_DRVING2_CMDDRV1));
    vIO32WriteFldMulti(DRAMC_REG_SHU2_DRVING3, P_Fld(0x14a, SHU2_DRVING3_DQSODT2)
                | P_Fld(0x14a, SHU2_DRVING3_DQSODT1)
                | P_Fld(0x14a, SHU2_DRVING3_DQODT2));
    vIO32WriteFldMulti(DRAMC_REG_SHU2_DRVING4, P_Fld(0x14a, SHU2_DRVING4_DQODT1)
                | P_Fld(0x14a, SHU2_DRVING4_CMDODT2)
                | P_Fld(0x14a, SHU2_DRVING4_CMDODT1));
    vIO32WriteFldMulti(DRAMC_REG_SHU3_DRVING1, P_Fld(0x14a, SHU3_DRVING1_DQSDRV2)
                | P_Fld(0x14a, SHU3_DRVING1_DQSDRV1)
                | P_Fld(0x14a, SHU3_DRVING1_DQDRV2));
    vIO32WriteFldMulti(DRAMC_REG_SHU3_DRVING2, P_Fld(0x14a, SHU3_DRVING2_DQDRV1)
                | P_Fld(0x14a, SHU3_DRVING2_CMDDRV2)
                | P_Fld(0x14a, SHU3_DRVING2_CMDDRV1));
    vIO32WriteFldMulti(DRAMC_REG_SHU3_DRVING3, P_Fld(0x14a, SHU3_DRVING3_DQSODT2)
                | P_Fld(0x14a, SHU3_DRVING3_DQSODT1)
                | P_Fld(0x14a, SHU3_DRVING3_DQODT2));
    vIO32WriteFldMulti(DRAMC_REG_SHU3_DRVING4, P_Fld(0x14a, SHU3_DRVING4_DQODT1)
                | P_Fld(0x14a, SHU3_DRVING4_CMDODT2)
                | P_Fld(0x14a, SHU3_DRVING4_CMDODT1));
    vIO32WriteFldMulti(DRAMC_REG_SHU4_DRVING1, P_Fld(0x14a, SHU4_DRVING1_DQSDRV2)
                | P_Fld(0x14a, SHU4_DRVING1_DQSDRV1)
                | P_Fld(0x14a, SHU4_DRVING1_DQDRV2));
    vIO32WriteFldMulti(DRAMC_REG_SHU4_DRVING2, P_Fld(0x14a, SHU4_DRVING2_DQDRV1)
                | P_Fld(0x14a, SHU4_DRVING2_CMDDRV2)
                | P_Fld(0x14a, SHU4_DRVING2_CMDDRV1));
    */
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x08a8)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x08ac)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x08b0)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x08b4)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x0ea8)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x0eac)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x0eb0)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x0eb4)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x14a8)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x14ac)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x14b0)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x14b4)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x1aa8)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x1aac)) = 0x14a5294a;
    vIO32WriteFldMulti(DRAMC_REG_SHUCTRL2, P_Fld(0x0, SHUCTRL2_HWSET_WLRL)
                | P_Fld(0x1, SHUCTRL2_SHU_PERIOD_GO_ZERO_CNT)
                | P_Fld(0x1, SHUCTRL2_R_DVFS_OPTION)
                | P_Fld(0x1, SHUCTRL2_R_DVFS_PARK_N)
                | P_Fld(0x1, SHUCTRL2_R_DVFS_DLL_CHA)
                | P_Fld(0xa, SHUCTRL2_R_DLL_IDLE));
    vIO32WriteFldMulti(DRAMC_REG_DVFSDLL, P_Fld(0x18, DVFSDLL_DLL_IDLE_SHU4)
                | P_Fld(0x18, DVFSDLL_DLL_IDLE_SHU3)
                | P_Fld(0x18, DVFSDLL_DLL_IDLE_SHU2)
                | P_Fld(0x1, DVFSDLL_DLL_LOCK_SHU_EN));
    vIO32WriteFldMulti(DRAMC_REG_DDRCONF0, P_Fld(0x1, DDRCONF0_LPDDR4EN)
                | P_Fld(0x1, DDRCONF0_DM64BITEN)
                | P_Fld(0x1, DDRCONF0_BC4OTF)
                | P_Fld(0x1, DDRCONF0_BK8EN));
    vIO32WriteFldMulti(DRAMC_REG_EYESCAN, P_Fld(0x1, EYESCAN_STB_GERR_B01)
                | P_Fld(0x1, EYESCAN_STB_GERRSTOP)
                | P_Fld(0x1, EYESCAN_EYESCAN_RD_SEL_OPT));
    vIO32WriteFldAlign(DRAMC_REG_EYESCAN, 0x1, EYESCAN_STB_GERR_RST);
    vIO32WriteFldAlign(DRAMC_REG_EYESCAN, 0x0, EYESCAN_STB_GERR_RST);
    vIO32WriteFldAlign(DRAMC_REG_CLKAR, 0x1, CLKAR_PSELAR);
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DDRPHY_MISC_CTRL0, P_Fld(0x0, MISC_CTRL0_R_STBENCMP_DIV4CK_EN)
                    | P_Fld(0x1, MISC_CTRL0_R_DMDQSIEN_FIFO_EN)
                    | P_Fld(0x1, MISC_CTRL0_R_DMDQSIEN_RDSEL_LAT)
                    | P_Fld(0x1, MISC_CTRL0_R_DMSTBEN_OUTSEL)
                    | P_Fld(0xf, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));
    }
    else
    {
        vIO32WriteFldMulti(DDRPHY_MISC_CTRL0, P_Fld(0x0, MISC_CTRL0_R_STBENCMP_DIV4CK_EN)
                    | P_Fld(0x1, MISC_CTRL0_R_DMDQSIEN_FIFO_EN)
                    | P_Fld(0x1, MISC_CTRL0_R_DMDQSIEN_RDSEL_LAT)
                    | P_Fld(0x1, MISC_CTRL0_R_DMSTBEN_OUTSEL)
                    | P_Fld(0xf, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));
    }
    //Francis : pin mux issue, need to set CHD
    DramcBroadcastOnOff(BROADCAST_4CHANNELS);
    vIO32WriteFldMulti(DDRPHY_MISC_CTRL1, P_Fld(0x1, MISC_CTRL1_R_DMDA_RRESETB_E)
                | P_Fld(0x1, MISC_CTRL1_R_HWSAVE_MODE_ENA)
                | P_Fld(0x1, MISC_CTRL1_R_DMDQSIENCG_EN)
                | P_Fld(0x1, MISC_CTRL1_R_DM_TX_ARCMD_OE)
                | P_Fld(0x1, MISC_CTRL1_R_DM_TX_ARCLK_OE));
    vIO32WriteFldAlign(DDRPHY_CA_CMD7, 0x0, CA_CMD7_RG_TX_ARCMD_PULL_DN);
    vIO32WriteFldAlign(DDRPHY_B0_DQ7, 0x0, B0_DQ7_RG_TX_ARDQ_PULL_DN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ7, 0x0, B1_DQ7_RG_TX_ARDQ_PULL_DN_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD8, 0x0, CA_CMD8_RG_TX_RRESETB_PULL_DN);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    vIO32WriteFldMulti(DRAMC_REG_SHU_CONF0, P_Fld(0x2, SHU_CONF0_MATYPE)
                | P_Fld(0x1, SHU_CONF0_BL4)
                | P_Fld(0x1, SHU_CONF0_FREQDIV4)
                | P_Fld(0x1, SHU_CONF0_REFTHD)
                | P_Fld(0x1, SHU_CONF0_ADVPREEN)
                | P_Fld(0x3f, SHU_CONF0_DMPGTIM));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x1, SHU_ODTCTRL_RODTE)
                | P_Fld(0x1, SHU_ODTCTRL_RODTE2)
                | P_Fld(0x1, SHU_ODTCTRL_TWODT)
                | P_Fld(0x5, SHU_ODTCTRL_RODT)
                | P_Fld(0x1, SHU_ODTCTRL_WOEN)
                | P_Fld(0x1, SHU_ODTCTRL_ROEN));
    vIO32WriteFldAlign(DRAMC_REG_REFCTRL0, 0x5, REFCTRL0_REF_PREGATE_CNT);
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA1, P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CS1)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_RAS)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CAS)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_WE)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_RESET)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_ODT)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CKE)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CS));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA2, P_Fld(0x0, SHU_SELPH_CA2_TXDLY_CKE1)
                | P_Fld(0x0, SHU_SELPH_CA2_TXDLY_CMD)
                | P_Fld(0x0, SHU_SELPH_CA2_TXDLY_BA2)
                | P_Fld(0x0, SHU_SELPH_CA2_TXDLY_BA1)
                | P_Fld(0x0, SHU_SELPH_CA2_TXDLY_BA0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA3, P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA7)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA6)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA5)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA4)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA3)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA2)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA1)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA4, P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA15)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA14)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA13)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA12)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA11)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA10)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA9)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA8));
    vIO32WriteFldAlign(DRAMC_REG_SHU_SELPH_CA5, 0x0, SHU_SELPH_CA5_DLY_ODT);
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS0, P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS3)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS2)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS1)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS0)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS3)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS2)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS1)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS1, P_Fld(0x2, SHU_SELPH_DQS1_DLY_OEN_DQS3)
                | P_Fld(0x2, SHU_SELPH_DQS1_DLY_OEN_DQS2)
                | P_Fld(0x2, SHU_SELPH_DQS1_DLY_OEN_DQS1)
                | P_Fld(0x2, SHU_SELPH_DQS1_DLY_OEN_DQS0)
                | P_Fld(0x5, SHU_SELPH_DQS1_DLY_DQS3)
                | P_Fld(0x5, SHU_SELPH_DQS1_DLY_DQS2)
                | P_Fld(0x5, SHU_SELPH_DQS1_DLY_DQS1)
                | P_Fld(0x5, SHU_SELPH_DQS1_DLY_DQS0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ2)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM2)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
                | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
                | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
                | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_OEN_DQ0)
                | P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_DQ3)
                | P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_DQ2)
                | P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_DQ1)
                | P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
                | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
                | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
                | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_OEN_DQM0)
                | P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_DQM3)
                | P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_DQM2)
                | P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_DQM1)
                | P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
                | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
                | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
                | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0)
                | P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1DQ3)
                | P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1DQ2)
                | P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1DQ1)
                | P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
                | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
                | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
                | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0)
                | P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1DQM3)
                | P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1DQM2)
                | P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1DQM1)
                | P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ0, P_Fld(0x3, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ3)
                | P_Fld(0x3, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ2)
                | P_Fld(0x3, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ1)
                | P_Fld(0x3, SHURK2_SELPH_DQ0_TX_DLY_R2OEN_DQ0)
                | P_Fld(0x3, SHURK2_SELPH_DQ0_TX_DLY_R2DQ3)
                | P_Fld(0x3, SHURK2_SELPH_DQ0_TX_DLY_R2DQ2)
                | P_Fld(0x3, SHURK2_SELPH_DQ0_TX_DLY_R2DQ1)
                | P_Fld(0x3, SHURK2_SELPH_DQ0_TX_DLY_R2DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ1, P_Fld(0x3, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM3)
                | P_Fld(0x3, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM2)
                | P_Fld(0x3, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM1)
                | P_Fld(0x3, SHURK2_SELPH_DQ1_TX_DLY_R2OEN_DQM0)
                | P_Fld(0x3, SHURK2_SELPH_DQ1_TX_DLY_R2DQM3)
                | P_Fld(0x3, SHURK2_SELPH_DQ1_TX_DLY_R2DQM2)
                | P_Fld(0x3, SHURK2_SELPH_DQ1_TX_DLY_R2DQM1)
                | P_Fld(0x3, SHURK2_SELPH_DQ1_TX_DLY_R2DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ2, P_Fld(0x2, SHURK2_SELPH_DQ2_DLY_R2OEN_DQ3)
                | P_Fld(0x2, SHURK2_SELPH_DQ2_DLY_R2OEN_DQ2)
                | P_Fld(0x2, SHURK2_SELPH_DQ2_DLY_R2OEN_DQ1)
                | P_Fld(0x2, SHURK2_SELPH_DQ2_DLY_R2OEN_DQ0)
                | P_Fld(0x6, SHURK2_SELPH_DQ2_DLY_R2DQ3)
                | P_Fld(0x6, SHURK2_SELPH_DQ2_DLY_R2DQ2)
                | P_Fld(0x6, SHURK2_SELPH_DQ2_DLY_R2DQ1)
                | P_Fld(0x6, SHURK2_SELPH_DQ2_DLY_R2DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ3, P_Fld(0x2, SHURK2_SELPH_DQ3_DLY_R2OEN_DQM3)
                | P_Fld(0x2, SHURK2_SELPH_DQ3_DLY_R2OEN_DQM2)
                | P_Fld(0x2, SHURK2_SELPH_DQ3_DLY_R2OEN_DQM1)
                | P_Fld(0x2, SHURK2_SELPH_DQ3_DLY_R2OEN_DQM0)
                | P_Fld(0x6, SHURK2_SELPH_DQ3_DLY_R2DQM3)
                | P_Fld(0x6, SHURK2_SELPH_DQ3_DLY_R2DQM2)
                | P_Fld(0x6, SHURK2_SELPH_DQ3_DLY_R2DQM1)
                | P_Fld(0x6, SHURK2_SELPH_DQ3_DLY_R2DQM0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ7, P_Fld(0x1a, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1)
                | P_Fld(0x1a, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ7, P_Fld(0x1a, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0)
                | P_Fld(0x1a, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ7, P_Fld(0x14, SHU1_R1_B1_DQ7_RK1_ARPI_DQM_B1)
                | P_Fld(0x14, SHU1_R1_B1_DQ7_RK1_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ7, P_Fld(0x14, SHU1_R1_B0_DQ7_RK1_ARPI_DQM_B0)
                | P_Fld(0x14, SHU1_R1_B0_DQ7_RK1_ARPI_DQ_B0));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x2, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1)
                    | P_Fld(0x2, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN)
                    | P_Fld(0x2, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1)
                    | P_Fld(0x2, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN)
                    | P_Fld(0x6, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
                    | P_Fld(0x6, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
                    | P_Fld(0x6, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
                    | P_Fld(0x6, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN_P1)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN_P1)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN_P1)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN_P1)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN_P1)
                    | P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN)
                    | P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN_P1)
                    | P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_ODTEN0, P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B3_R2RODTEN_P1)
                    | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B3_R2RODTEN)
                    | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B2_R2RODTEN_P1)
                    | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B2_R2RODTEN));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x2, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1)
                    | P_Fld(0x2, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN)
                    | P_Fld(0x2, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1)
                    | P_Fld(0x2, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN_P1)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN_P1)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN_P1)
                    | P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN)
                    | P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN_P1)
                    | P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN)
                    | P_Fld(0x7, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
                    | P_Fld(0x7, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
                    | P_Fld(0x7, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
                    | P_Fld(0x7, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_ODTEN0, P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B3_R2RODTEN_P1)
                    | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B3_R2RODTEN)
                    | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B2_R2RODTEN_P1)
                    | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B2_R2RODTEN)
                    | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B1_R2RODTEN_P1)
                    | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B1_R2RODTEN)
                    | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B0_R2RODTEN_P1)
                    | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B0_R2RODTEN));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_ODTEN1, P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B3_R2RODTEN_P1)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B3_R2RODTEN)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B2_R2RODTEN_P1)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B2_R2RODTEN)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B1_R2RODTEN_P1)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B1_R2RODTEN)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B0_R2RODTEN_P1)
                | P_Fld(0x6, SHURK2_SELPH_ODTEN1_DLY_B0_R2RODTEN));

    mcDELAY_US(1);

    vIO32WriteFldAlign(DDRPHY_B1_DQ3, 0x1, B1_DQ3_RG_RX_ARDQS1_DQSIENMODE);
    vIO32WriteFldAlign(DDRPHY_B0_DQ3, 0x1, B0_DQ3_RG_RX_ARDQS0_DQSIENMODE);
    vIO32WriteFldAlign(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1);
    vIO32WriteFldAlign(DRAMC_REG_STBCAL, 0x1, STBCAL_DQSIENMODE);
    vIO32WriteFldMulti(DRAMC_REG_SREFCTRL, P_Fld(0x0, SREFCTRL_SREF_HW_EN)
                | P_Fld(0x8, SREFCTRL_SREFDLY));
    vIO32WriteFldMulti(DRAMC_REG_SHU_CKECTRL, P_Fld(0x3, SHU_CKECTRL_SREF_CK_DLY)
                | P_Fld(0x3, SHU_CKECTRL_TCKESRX)
                | P_Fld(0x3, SHU_CKECTRL_CKEPRD));
    vIO32WriteFldMulti(DRAMC_REG_SHU_PIPE, P_Fld(0x1, SHU_PIPE_READ_START_EXTEND1)
                | P_Fld(0x1, SHU_PIPE_DLE_LAST_EXTEND1));
    vIO32WriteFldMulti(DRAMC_REG_CKECTRL, P_Fld(0x1, CKECTRL_CKEON)
                | P_Fld(0x1, CKECTRL_CKETIMER_SEL));
    vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0x1, RKCFG_CKE2RANK_OPT2);
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM0, P_Fld(0x6, SHU_ACTIM0_TRCD)
                    | P_Fld(0x2, SHU_ACTIM0_TRRD)
                    | P_Fld(0xd, SHU_ACTIM0_TWR)
                    | P_Fld(0x8, SHU_ACTIM0_TWTR));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM1, P_Fld(0x10, SHU_ACTIM1_TRC)
                    | P_Fld(0x8, SHU_ACTIM1_TRAS)
                    | P_Fld(0x5, SHU_ACTIM1_TRP)
                    | P_Fld(0x1, SHU_ACTIM1_TRPAB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM2, P_Fld(0x7, SHU_ACTIM2_TFAW)
                    | P_Fld(0x9, SHU_ACTIM2_TR2W)
                    | P_Fld(0x2, SHU_ACTIM2_TRTP)
                    | P_Fld(0x1, SHU_ACTIM2_TXP));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM3, P_Fld(0x64, SHU_ACTIM3_TRFC)
                    | P_Fld(0x2c, SHU_ACTIM3_TRFCPB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM5, P_Fld(0x8, SHU_ACTIM5_TMRR2W)
                    | P_Fld(0xc, SHU_ACTIM5_TWTPD)
                    | P_Fld(0xd, SHU_ACTIM5_TR2PD));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x8, SHU_ACTIM_XRT_XRTW2W)
                    | P_Fld(0x3, SHU_ACTIM_XRT_XRTW2R)
                    | P_Fld(0x7, SHU_ACTIM_XRT_XRTR2W)
                    | P_Fld(0x9, SHU_ACTIM_XRT_XRTR2R));
        vIO32WriteFldMulti(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(0x1, SHU_AC_TIME_05T_TWR_M05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRAS_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRP_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRCD_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRTP_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRC_05T));
        vIO32WriteFldMulti(DRAMC_REG_SHU_CONF2, P_Fld(0x1, SHU_CONF2_WPRE2T)
                    | P_Fld(0x7, SHU_CONF2_DCMDLYREF)
                    | P_Fld(0x64, SHU_CONF2_FSPCHG_PRDCNT));
        vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_CLR_EN);
        vIO32WriteFldAlign(DRAMC_REG_SHU_SCINTV, 0xf, SHU_SCINTV_MRW_INTV);
        vIO32WriteFldAlign(DRAMC_REG_SHUCTRL1, 0x40, SHUCTRL1_FC_PRDCNT);
        vIO32WriteFldMulti(DRAMC_REG_SHU_CONF1, P_Fld(0xc, SHU_CONF1_DATLAT_DSEL_PHY)
                    | P_Fld(0xd, SHU_CONF1_DATLAT_DSEL)
                    | P_Fld(0xe, SHU_CONF1_DATLAT));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM0, P_Fld(0x6, SHU_ACTIM0_TRCD)
                    | P_Fld(0x2, SHU_ACTIM0_TRRD)
                    | P_Fld(0xc, SHU_ACTIM0_TWR)
                    | P_Fld(0x7, SHU_ACTIM0_TWTR));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM1, P_Fld(0x10, SHU_ACTIM1_TRC)
                    | P_Fld(0x8, SHU_ACTIM1_TRAS)
                    | P_Fld(0x5, SHU_ACTIM1_TRP)
                    | P_Fld(0x1, SHU_ACTIM1_TRPAB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM2, P_Fld(0x7, SHU_ACTIM2_TFAW)
                    | P_Fld(0x7, SHU_ACTIM2_TR2W)
                    | P_Fld(0x2, SHU_ACTIM2_TRTP)
                    | P_Fld(0x1, SHU_ACTIM2_TXP));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM3, P_Fld(0x64, SHU_ACTIM3_TRFC)
                    | P_Fld(0x2c, SHU_ACTIM3_TRFCPB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM5, P_Fld(0x8, SHU_ACTIM5_TMRR2W)
                    | P_Fld(0xb, SHU_ACTIM5_TWTPD)
                    | P_Fld(0xa, SHU_ACTIM5_TR2PD));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x8, SHU_ACTIM_XRT_XRTW2W)
                    | P_Fld(0x3, SHU_ACTIM_XRT_XRTW2R)
                    | P_Fld(0x7, SHU_ACTIM_XRT_XRTR2W)
                    | P_Fld(0x9, SHU_ACTIM_XRT_XRTR2R));
        vIO32WriteFldMulti(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(0x1, SHU_AC_TIME_05T_TR2PD_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TWR_M05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRAS_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRP_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRCD_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRTP_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRC_05T));
        vIO32WriteFldMulti(DRAMC_REG_SHU_CONF2, P_Fld(0x1, SHU_CONF2_WPRE2T)
                    | P_Fld(0x7, SHU_CONF2_DCMDLYREF)
                    | P_Fld(0x64, SHU_CONF2_FSPCHG_PRDCNT));
        vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_CLR_EN);
        vIO32WriteFldAlign(DRAMC_REG_SHU_SCINTV, 0xf, SHU_SCINTV_MRW_INTV);
        vIO32WriteFldAlign(DRAMC_REG_SHUCTRL1, 0x40, SHUCTRL1_FC_PRDCNT);
        vIO32WriteFldMulti(DRAMC_REG_SHU_CONF1, P_Fld(0xb, SHU_CONF1_DATLAT_DSEL_PHY)
                    | P_Fld(0xc, SHU_CONF1_DATLAT_DSEL)
                    | P_Fld(0xd, SHU_CONF1_DATLAT));
    }
    vIO32WriteFldAlign(DRAMC_REG_SHUCTRL, 0x1, SHUCTRL_LPSM_BYPASS_B);
    vIO32WriteFldMulti(DRAMC_REG_REFCTRL1, P_Fld(0x0, REFCTRL1_SREF_PRD_OPT) | P_Fld(0x1, REFCTRL1_PSEL_OPT1) | P_Fld(0x0, REFCTRL1_PSEL_OPT2) | P_Fld(0x0, REFCTRL1_PSEL_OPT3));
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL4, 0xfe, SHU1_PLL4_RG_RPHYPLL_RESERVED);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL6, 0xfe, SHU1_PLL6_RG_RCLRPLL_RESERVED);
    vIO32WriteFldMulti(DRAMC_REG_REFRATRE_FILTER, P_Fld(0x1, REFRATRE_FILTER_PB2AB_OPT) | P_Fld(0x0, REFRATRE_FILTER_PB2AB_OPT1));

#if !APPLY_LP4_POWER_INIT_SEQUENCE
    vIO32WriteFldAlign(DRAMC_REG_DDRCONF0, 0x1, DDRCONF0_GDDR3RST);
#endif
    vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x0, DRAMCTRL_CLKWITRFC);
    vIO32WriteFldMulti(DRAMC_REG_MISCTL0, P_Fld(0x1, MISCTL0_REFP_ARB_EN2)
                | P_Fld(0x1, MISCTL0_PBC_ARB_EN)
                | P_Fld(0x1, MISCTL0_REFA_ARB_EN2));
    vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_MWHPRIEN)
                | P_Fld(0x1, PERFCTL0_RWSPLIT)
                | P_Fld(0x1, PERFCTL0_WFLUSHEN)
                | P_Fld(0x1, PERFCTL0_EMILLATEN)
                | P_Fld(0x1, PERFCTL0_RWAGEEN)
                | P_Fld(0x1, PERFCTL0_RWLLATEN)
                | P_Fld(0x1, PERFCTL0_RWHPRIEN)
                | P_Fld(0x1, PERFCTL0_RWOFOEN)
                | P_Fld(0x1, PERFCTL0_DISRDPHASE1)
                | P_Fld(0x1, PERFCTL0_DUALSCHEN));
    vIO32WriteFldAlign(DRAMC_REG_ARBCTL, 0x80, ARBCTL_MAXPENDCNT);
    vIO32WriteFldMulti(DRAMC_REG_PADCTRL, P_Fld(0x1, PADCTRL_DQIENLATEBEGIN)
                | P_Fld(0x1, PADCTRL_DQIENQKEND));
    vIO32WriteFldAlign(DRAMC_REG_DRAMC_PD_CTRL, 0x1, DRAMC_PD_CTRL_DCMREF_OPT);
    vIO32WriteFldAlign(DRAMC_REG_CLKCTRL, 0x1, CLKCTRL_CLK_EN_1);
    vIO32WriteFldMulti(DRAMC_REG_REFCTRL0, P_Fld(0x4, REFCTRL0_DISBYREFNUM)
                | P_Fld(0x1, REFCTRL0_DLLFRZ));
    vIO32WriteFldMulti(DRAMC_REG_CATRAINING1, P_Fld(0xff, CATRAINING1_CATRAIN_INTV)
                | P_Fld(0x0, CATRAINING1_CATRAINLAT));
    vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x6, SHU_RANKCTL_RANKINCTL_PHY)
                | P_Fld(0x4, SHU_RANKCTL_RANKINCTL_ROOT1)
                | P_Fld(0x4, SHU_RANKCTL_RANKINCTL)
                | P_Fld(0x1, SHU_RANKCTL_DMSTBLAT));
    vIO32WriteFldAlign(DRAMC_REG_SHURK0_DQSCTL, 0x5, SHURK0_DQSCTL_DQSINCTL);
    vIO32WriteFldAlign(DRAMC_REG_SHURK1_DQSCTL, 0x5, SHURK1_DQSCTL_R1DQSINCTL);
    vIO32WriteFldAlign(DRAMC_REG_SHURK2_DQSCTL, 0x5, SHURK2_DQSCTL_R2DQSINCTL);

    mcDELAY_US(2);

    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x4, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                    | P_Fld(0x7, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                    | P_Fld(0x5, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQSG0, P_Fld(0x4, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED_P1)
                    | P_Fld(0x4, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED)
                    | P_Fld(0x4, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED_P1)
                    | P_Fld(0x4, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                    | P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSIEN, P_Fld(0x19, SHURK0_DQSIEN_R0DQS1IEN)
                    | P_Fld(0x19, SHURK0_DQSIEN_R0DQS0IEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x4, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSIEN, P_Fld(0x0, SHURK1_DQSIEN_R1DQS3IEN)
                    | P_Fld(0x0, SHURK1_DQSIEN_R1DQS2IEN)
                    | P_Fld(0x1b, SHURK1_DQSIEN_R1DQS1IEN)
                    | P_Fld(0x1b, SHURK1_DQSIEN_R1DQS0IEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQSG0, P_Fld(0x3, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED_P1)
                    | P_Fld(0x3, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS1_GATED)
                    | P_Fld(0x3, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED_P1)
                    | P_Fld(0x3, SHURK2_SELPH_DQSG0_TX_DLY_R2DQS0_GATED));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQSG1, P_Fld(0x4, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS1_GATED_P1)
                | P_Fld(0x4, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS0_GATED_P1));
    //               41536 === over_write_setting_begin ===
    //               41536 === over_write_setting_end ===
    vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x1, DRAMCTRL_PREALL_OPTION);
    vIO32WriteFldAlign(DRAMC_REG_ZQCS, 0x56, ZQCS_ZQCSOP);

    mcDELAY_US(1);

    vIO32WriteFldAlign(DRAMC_REG_SHU_CONF3, 0xff, SHU_CONF3_REFRCNT);
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM4, P_Fld(0x22, SHU_ACTIM4_TZQCS)
                | P_Fld(0x65, SHU_ACTIM4_REFCNT_FR_CLK)
                | P_Fld(0x61, SHU_ACTIM4_REFCNT)
                | P_Fld(0x76, SHU_ACTIM4_TXREFCNT));
    vIO32WriteFldAlign(DRAMC_REG_REFCTRL0, 0x1, REFCTRL0_REFFRERUN);
    vIO32WriteFldAlign(DRAMC_REG_SREFCTRL, 0x1, SREFCTRL_SREF_HW_EN);
    vIO32WriteFldAlign(DRAMC_REG_MRS, 0x1, MRS_MPCRKEN);
    vIO32WriteFldAlign(DRAMC_REG_DRAMC_PD_CTRL, 0x1, DRAMC_PD_CTRL_PHYCLKDYNGEN);
    vIO32WriteFldAlign(DRAMC_REG_DRAMC_PD_CTRL, 0x1, DRAMC_PD_CTRL_DCMEN);
    vIO32WriteFldMulti(DRAMC_REG_EYESCAN, P_Fld(0x1, EYESCAN_RX_DQ_EYE_SEL)
                | P_Fld(0x1, EYESCAN_RG_RX_EYE_SCAN_EN));
    vIO32WriteFldMulti(DRAMC_REG_STBCAL1, P_Fld(0x1, STBCAL1_STBCNT_LATCH_EN)
                | P_Fld(0x1, STBCAL1_STBENCMPEN));
    vIO32WriteFldAlign(DRAMC_REG_TEST2_1, 0x10000, TEST2_1_TEST2_BASE);
#if (FOR_DV_SIMULATION_USED==1 || SW_CHANGE_FOR_SIMULATION==1)
        //because cmd_len=1 has bug with byte mode, so need to set cmd_len=0, then it will cost more time to do a pattern test
        //workaround: reduce TEST2_OFF to make less test agent cmd. make lpddr4-1600 can finish in 60us
        vIO32WriteFldAlign(DRAMC_REG_TEST2_2, 0x20, TEST2_2_TEST2_OFF);
#else
        vIO32WriteFldAlign(DRAMC_REG_TEST2_2, 0x400, TEST2_2_TEST2_OFF);
#endif
    vIO32WriteFldMulti(DRAMC_REG_TEST2_3, P_Fld(0x1, TEST2_3_TEST2WREN2_HW_EN)
                | P_Fld(0x4, TEST2_3_DQSICALSTP)
                | P_Fld(0x1, TEST2_3_TESTAUDPAT));
    vIO32WriteFldAlign(DRAMC_REG_RSTMASK, 0x0, RSTMASK_DAT_SYNC_MASK);
    vIO32WriteFldAlign(DRAMC_REG_RSTMASK, 0x0, RSTMASK_PHY_SYNC_MASK);

    mcDELAY_US(1);

    vIO32WriteFldMulti(DRAMC_REG_HW_MRR_FUN, P_Fld(0x0, HW_MRR_FUN_TRPMRR_EN)
                    | P_Fld(0x0, HW_MRR_FUN_TRCDMRR_EN) | P_Fld(0x0, HW_MRR_FUN_TMRR_ENA));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_WRFIO_MODE2)
                    | P_Fld(0x0, PERFCTL0_RWSPLIT));
        vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_REORDEREN)
                    | P_Fld(0x0, PERFCTL0_REORDER_MODE));
        vIO32WriteFldAlign(DRAMC_REG_RSTMASK, 0x0, RSTMASK_GT_SYNC_MASK);
        vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0x1, RKCFG_DQSOSC2RK);
        vIO32WriteFldAlign(DRAMC_REG_MRS, 0x0, MRS_MPCRK);
        vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_REFR_BLOCKEN);
        vIO32WriteFldAlign(DRAMC_REG_EYESCAN, 0x1, EYESCAN_RG_RX_MIOCK_JIT_EN);
    }
    else
    {
        vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x0, DRAMCTRL_CTOREQ_HPRI_OPT);
        vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_REORDEREN)
                    | P_Fld(0x0, PERFCTL0_REORDER_MODE));
        vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_REFR_BLOCKEN);
        vIO32WriteFldAlign(DRAMC_REG_RSTMASK, 0x0, RSTMASK_GT_SYNC_MASK);
        vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0x1, RKCFG_DQSOSC2RK);
        //vIO32WriteFldAlign(DRAMC_REG_MRS, 0x1, MRS_MPCRK);
        vIO32WriteFldMulti(DRAMC_REG_MRS, P_Fld(1, MRS_MPCRKEN) | P_Fld(0, MRS_MPCRK));
        vIO32WriteFldAlign(DRAMC_REG_EYESCAN, 0x1, EYESCAN_RG_RX_MIOCK_JIT_EN);
        vIO32WriteFldAlign(DRAMC_REG_SHU1_WODT, 0x1, SHU1_WODT_DBIWR);
        vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0x1, SHU1_B0_DQ7_R_DMDQMDBI_SHU_B0);
        vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x1, SHU1_B1_DQ7_R_DMDQMDBI_SHU_B1);
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
        vIO32WriteFldAlign(DRAMC_REG_SHU_RANKCTL, 0x4, SHU_RANKCTL_RANKINCTL);
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN0, P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN)
                    | P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
    }
    mcDELAY_US(5);

    vIO32WriteFldAlign(DRAMC_REG_STBCAL1, 0x3, STBCAL1_STBCAL_FILTER);
    vIO32WriteFldAlign(DRAMC_REG_STBCAL1, 0x1, STBCAL1_STBCAL_FILTER);
    vIO32WriteFldMulti(DRAMC_REG_STBCAL, P_Fld(0x1, STBCAL_STB_DQIEN_IG)
                | P_Fld(0x1, STBCAL_PICHGBLOCK_NORD)
                | P_Fld(0x0, STBCAL_STBCALEN)
                | P_Fld(0x0, STBCAL_STB_SELPHYCALEN)
                | P_Fld(0x1, STBCAL_PIMASK_RKCHG_OPT));
    vIO32WriteFldAlign(DRAMC_REG_STBCAL1, 0x1, STBCAL1_STB_SHIFT_DTCOUT_IG);
    vIO32WriteFldMulti(DRAMC_REG_SHU1_DQSG, P_Fld(0x9, SHU1_DQSG_STB_UPDMASKCYC)
                | P_Fld(0x1, SHU1_DQSG_STB_UPDMASK_EN));
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL0, 0x0, MISC_CTRL0_R_DMDQSIEN_SYNCOPT);
    vIO32WriteFldAlign(DRAMC_REG_SHU_RANKCTL, 0x1, SHU_RANKCTL_DQSG_MODE);
    vIO32WriteFldAlign(DRAMC_REG_STBCAL, 0x1, STBCAL_SREF_DQSGUPD);
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL1, 0x0, MISC_CTRL1_R_DMDQMDBI);
    vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x1, SHU_RANKCTL_PICGLAT)
                | P_Fld(0x0, SHU_RANKCTL_DMSTBLAT));
    vIO32WriteFldMulti(DRAMC_REG_REFCTRL1, P_Fld(0x1, REFCTRL1_REF_QUE_AUTOSAVE_EN)
                | P_Fld(0x1, REFCTRL1_SLEFREF_AUTOSAVE_EN));
    vIO32WriteFldMulti(DRAMC_REG_DQSOSCR, P_Fld(0x1, DQSOSCR_SREF_TXPI_RELOAD_OPT)
                | P_Fld(0x1, DQSOSCR_SREF_TXUI_RELOAD_OPT));
    vIO32WriteFldMulti(DRAMC_REG_RSTMASK, P_Fld(0x0, RSTMASK_DVFS_SYNC_MASK)
                | P_Fld(0x0, RSTMASK_GT_SYNC_MASK_FOR_PHY)
                | P_Fld(0x0, RSTMASK_DVFS_SYNC_MASK_FOR_PHY));
    vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0x1, RKCFG_RKMODE);

#if !APPLY_LP4_POWER_INIT_SEQUENCE
    vIO32WriteFldMulti(DRAMC_REG_CKECTRL, P_Fld(0x1, CKECTRL_CKEFIXON)
                | P_Fld(0x1, CKECTRL_CKE1FIXON));
#endif

    mcDELAY_US(12);

    ///TODO: DDR3200
    //if(p->frequency==1600)
    {
    //               60826 ===dramc_shu1_lp4_3200 begin===
        if(p->dram_cbt_mode==CBT_BYTE_MODE1)
        {
                vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM2, P_Fld(0x8, SHU_ACTIM2_TR2W)
                            | P_Fld(0x0, SHU_ACTIM2_TXP));
                vIO32WriteFldAlign(DRAMC_REG_SHU_ACTIM5, 0x9, SHU_ACTIM5_TR2PD);
                vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x9, SHU_ACTIM_XRT_XRTW2W)
                            | P_Fld(0x8, SHU_ACTIM_XRT_XRTR2W));
                vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x5, SHU_RANKCTL_RANKINCTL_PHY)
                            | P_Fld(0x3, SHU_RANKCTL_RANKINCTL_ROOT1)
                            | P_Fld(0x3, SHU_RANKCTL_RANKINCTL));
        }
        else
        {
               vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM5, P_Fld(0x9, SHU_ACTIM5_TMRR2W)
                            | P_Fld(0xc, SHU_ACTIM5_TR2PD));
                vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x9, SHU_ACTIM_XRT_XRTW2W)
                            | P_Fld(0x8, SHU_ACTIM_XRT_XRTR2W));
                vIO32WriteFldAlign(DRAMC_REG_SHU_AC_TIME_05T, 0x0, SHU_AC_TIME_05T_TR2PD_05T);
                vIO32WriteFldMulti(DRAMC_REG_SHU_CONF1, P_Fld(0xc, SHU_CONF1_DATLAT_DSEL_PHY)
                            | P_Fld(0xd, SHU_CONF1_DATLAT_DSEL)
                            | P_Fld(0xe, SHU_CONF1_DATLAT));
                vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x6, SHU_RANKCTL_RANKINCTL_PHY)
                            | P_Fld(0x4, SHU_RANKCTL_RANKINCTL_ROOT1));
        }
        vIO32WriteFldMulti(DRAMC_REG_SHU1_DQSOSC_PRD, P_Fld(0xa, SHU1_DQSOSC_PRD_DQSOSCTHRD_DEC)
                    | P_Fld(0xa, SHU1_DQSOSC_PRD_DQSOSCTHRD_INC)
                    | P_Fld(0x10, SHU1_DQSOSC_PRD_DQSOSC_PRDCNT));
        vIO32WriteFldAlign(DRAMC_REG_SHU_DQSOSCR, 0x10, SHU_DQSOSCR_DQSOSCRCNT);
        vIO32WriteFldAlign(DRAMC_REG_SHU1_WODT, 0x0, SHU1_WODT_WPST2T);
        //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR2, 0x2d, SHU_HWSET_MR2_HWSET_MR2_OP);
        if(p->dram_cbt_mode==CBT_BYTE_MODE1)
        {
                vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSIEN, P_Fld(0x19, SHURK0_DQSIEN_R0DQS1IEN)
                            | P_Fld(0x19, SHURK0_DQSIEN_R0DQS0IEN));
        }
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_PI, P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQM_B1)
                    | P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQM_B0)
                    | P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQ_B1)
                    | P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQ_B0));
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQSOSC, P_Fld(0x168, SHU1RK0_DQSOSC_DQSOSC_BASE_RK0_B1)
                    | P_Fld(0x168, SHU1RK0_DQSOSC_DQSOSC_BASE_RK0));
        if(p->dram_cbt_mode==CBT_BYTE_MODE1)
        {
                vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN0, P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B3_RODTEN_P1)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B3_RODTEN)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B2_RODTEN_P1)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B2_RODTEN)
                            | P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1)
                            | P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN)
                            | P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1)
                            | P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN));
                vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
                vIO32WriteFldAlign(DRAMC_REG_SHURK0_SELPH_DQSG0, 0x4, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1);
                vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                            | P_Fld(0x5, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                            | P_Fld(0x0, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                            | P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
        }
        else
        {
                vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN0, P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B3_RODTEN_P1)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B3_RODTEN)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B2_RODTEN_P1)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B2_RODTEN)
                            | P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1)
                            | P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1));
                vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
                vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x7, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                            | P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                            | P_Fld(0x7, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                            | P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));

        }
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(0x4, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
                    | P_Fld(0x4, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
                    | P_Fld(0x4, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
                    | P_Fld(0x4, SHURK0_SELPH_DQ2_DLY_OEN_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(0x4, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
                    | P_Fld(0x4, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
                    | P_Fld(0x4, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
                    | P_Fld(0x4, SHURK0_SELPH_DQ3_DLY_OEN_DQM0));
        if(p->dram_cbt_mode==CBT_BYTE_MODE1)
        {
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL1, P_Fld(0x3da, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1)
                            | P_Fld(0x3da, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL2, P_Fld(0x3da, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1)
                            | P_Fld(0x3da, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
        }
        else
        {
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL1, P_Fld(0x2da, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1)
                            | P_Fld(0x2da, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL2, P_Fld(0x2da, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1)
                            | P_Fld(0x2da, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
        }
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL3, P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1_B4TO0)
                    | P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0_B4TO0)
                    | P_Fld(0x1c, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1)
                    | P_Fld(0x1c, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL4, P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1_B4TO0)
                    | P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0_B4TO0)
                    | P_Fld(0x1c, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1)
                    | P_Fld(0x1c, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0));
        if(p->dram_cbt_mode==CBT_BYTE_MODE1)
        {
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL5, P_Fld(0x3da, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1)
                            | P_Fld(0x3da, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));
                vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSIEN, P_Fld(0x0, SHURK1_DQSIEN_R1DQS3IEN)
                            | P_Fld(0x0, SHURK1_DQSIEN_R1DQS2IEN)
                            | P_Fld(0x1b, SHURK1_DQSIEN_R1DQS1IEN)
                            | P_Fld(0x1b, SHURK1_DQSIEN_R1DQS0IEN));
        }
        else
        {
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL5, P_Fld(0x2da, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1)
                            | P_Fld(0x2da, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));
        }
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_PI, P_Fld(0x14, SHU1RK1_PI_RK1_ARPI_DQM_B1)
                    | P_Fld(0x14, SHU1RK1_PI_RK1_ARPI_DQM_B0)
                    | P_Fld(0x14, SHU1RK1_PI_RK1_ARPI_DQ_B1)
                    | P_Fld(0x14, SHU1RK1_PI_RK1_ARPI_DQ_B0));
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQSOSC, P_Fld(0x127, SHU1RK1_DQSOSC_DQSOSC_BASE_RK1_B1)
                    | P_Fld(0x127, SHU1RK1_DQSOSC_DQSOSC_BASE_RK1));
        if(p->dram_cbt_mode==CBT_BYTE_MODE1)
        {
                vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN_P1)
                            | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN)
                            | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN_P1)
                            | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN));
        }
        else
        {
                vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN_P1)
                            | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN)
                            | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN_P1)
                            | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN)
                            | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN_P1)
                            | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN_P1));
        }
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN_P1)
                    | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN)
                    | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN_P1)
                    | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN)
                    | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
                    | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
                    | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
                    | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
        if(p->dram_cbt_mode==CBT_BYTE_MODE1)
        {
                vIO32WriteFldAlign(DRAMC_REG_SHURK1_SELPH_DQSG0, 0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED);
                vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x4, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                            | P_Fld(0x0, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                            | P_Fld(0x6, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                            | P_Fld(0x2, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
        }
        else
        {
                vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x5, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                            | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                            | P_Fld(0x5, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                            | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
        }
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x4, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
                    | P_Fld(0x4, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
                    | P_Fld(0x4, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
                    | P_Fld(0x4, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x4, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
                    | P_Fld(0x4, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
                    | P_Fld(0x4, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
                    | P_Fld(0x4, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0));
        if(p->dram_cbt_mode==CBT_BYTE_MODE1)
        {
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL1, P_Fld(0x3d4, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1)
                            | P_Fld(0x3d4, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0));
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL2, P_Fld(0x3d4, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1)
                            | P_Fld(0x3d4, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0));
        }
        else
        {
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL1, P_Fld(0x2d4, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1)
                            | P_Fld(0x2d4, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0));
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL2, P_Fld(0x2d4, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1)
                            | P_Fld(0x2d4, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0));
        }
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL3, P_Fld(0x14, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1_B4TO0)
                    | P_Fld(0x14, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0_B4TO0)
                    | P_Fld(0x1c, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1)
                    | P_Fld(0x1c, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL4, P_Fld(0x14, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1_B4TO0)
                    | P_Fld(0x14, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0_B4TO0)
                    | P_Fld(0x1c, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1)
                    | P_Fld(0x1c, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0));
        if(p->dram_cbt_mode==CBT_BYTE_MODE1)
        {
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL5, P_Fld(0x3d4, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1)
                            | P_Fld(0x3d4, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0));
                vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_ODTEN0, P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B1_R2RODTEN_P1)
                            | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B1_R2RODTEN)
                            | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B0_R2RODTEN_P1)
                            | P_Fld(0x0, SHURK2_SELPH_ODTEN0_TXDLY_B0_R2RODTEN));
        }
        else
        {
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL5, P_Fld(0x2d4, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1)
                            | P_Fld(0x2d4, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0));
                vIO32WriteFldMulti(DRAMC_REG_SHURK2_DQSIEN, P_Fld(0xf, SHURK2_DQSIEN_R2DQS3IEN)
                            | P_Fld(0xf, SHURK2_DQSIEN_R2DQS2IEN)
                            | P_Fld(0xf, SHURK2_DQSIEN_R2DQS1IEN)
                            | P_Fld(0xf, SHURK2_DQSIEN_R2DQS0IEN));
        }
        vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ2, P_Fld(0x4, SHURK2_SELPH_DQ2_DLY_R2OEN_DQ3)
                    | P_Fld(0x4, SHURK2_SELPH_DQ2_DLY_R2OEN_DQ2)
                    | P_Fld(0x4, SHURK2_SELPH_DQ2_DLY_R2OEN_DQ1)
                    | P_Fld(0x4, SHURK2_SELPH_DQ2_DLY_R2OEN_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQ3, P_Fld(0x4, SHURK2_SELPH_DQ3_DLY_R2OEN_DQM3)
                    | P_Fld(0x4, SHURK2_SELPH_DQ3_DLY_R2OEN_DQM2)
                    | P_Fld(0x4, SHURK2_SELPH_DQ3_DLY_R2OEN_DQM1)
                    | P_Fld(0x4, SHURK2_SELPH_DQ3_DLY_R2OEN_DQM0));
        vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x1, SHU_DQSG_RETRY_R_RETRY_ROUND_NUM)
                    | P_Fld(0x0, SHU_DQSG_RETRY_R_XSR_RETRY_SPM_MODE)
                    | P_Fld(0x5, SHU_DQSG_RETRY_R_DQSIENLAT)
                    | P_Fld(0x1, SHU_DQSG_RETRY_R_DDR1866_PLUS));
        vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK2, P_Fld(0x25, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO0)
                    | P_Fld(0x18, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO1)
                    | P_Fld(0x10, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO2)
                    | P_Fld(0x1b, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO3));
        vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK3, P_Fld(0x14, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO4)
                    | P_Fld(0xd, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO5)
                    | P_Fld(0x2a, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO6)
                    | P_Fld(0x31, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO7));
        vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK4, P_Fld(0x15, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO8)
                    | P_Fld(0x40, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO9)
                    | P_Fld(0x4a, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO10)
                    | P_Fld(0x30, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO11));
        vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK1, P_Fld(0x1a, RK0_PRE_TDQSCK1_TDQSCK_PIFREQ2_B0R0)
                    | P_Fld(0x1c, RK0_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R0)
                    | P_Fld(0x14, RK0_PRE_TDQSCK1_TDQSCK_UIFREQ1_B0R0));
        vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK2, P_Fld(0xb, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R0)
                    | P_Fld(0xd, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R0)
                    | P_Fld(0x10, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R0)
                    | P_Fld(0xa, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R0));
        vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK3, P_Fld(0x11, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R0)
                    | P_Fld(0xe, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R0)
                    | P_Fld(0x20, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R0)
                    | P_Fld(0x18, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ1_P1_B0R0));
        vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK4, P_Fld(0x1a, RK0_PRE_TDQSCK4_TDQSCK_PIFREQ2_B1R0)
                    | P_Fld(0x1c, RK0_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R0)
                    | P_Fld(0x14, RK0_PRE_TDQSCK4_TDQSCK_UIFREQ1_B1R0));
        vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK5, P_Fld(0xb, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R0)
                    | P_Fld(0xd, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R0)
                    | P_Fld(0x10, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R0)
                    | P_Fld(0xa, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R0));
        vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK6, P_Fld(0x11, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R0)
                    | P_Fld(0xe, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R0)
                    | P_Fld(0x20, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R0)
                    | P_Fld(0x18, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ1_P1_B1R0));
        vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK1, P_Fld(0xb, RK1_PRE_TDQSCK1_TDQSCK_PIFREQ2_B0R1)
                    | P_Fld(0x23, RK1_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R1)
                    | P_Fld(0xf, RK1_PRE_TDQSCK1_TDQSCK_PIFREQ1_B0R1)
                    | P_Fld(0x19, RK1_PRE_TDQSCK1_TDQSCK_UIFREQ1_B0R1));
        vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK2, P_Fld(0xe, RK1_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R1)
                    | P_Fld(0x10, RK1_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R1)
                    | P_Fld(0x1f, RK1_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R1)
                    | P_Fld(0xe, RK1_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R1));
        vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK3, P_Fld(0x14, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R1)
                    | P_Fld(0x12, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R1)
                    | P_Fld(0x27, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R1)
                    | P_Fld(0x1d, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ1_P1_B0R1));
        vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK4, P_Fld(0xb, RK1_PRE_TDQSCK4_TDQSCK_PIFREQ2_B1R1)
                    | P_Fld(0x23, RK1_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R1)
                    | P_Fld(0xf, RK1_PRE_TDQSCK4_TDQSCK_PIFREQ1_B1R1)
                    | P_Fld(0x19, RK1_PRE_TDQSCK4_TDQSCK_UIFREQ1_B1R1));
        vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK5, P_Fld(0xe, RK1_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R1)
                    | P_Fld(0x10, RK1_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R1)
                    | P_Fld(0x1f, RK1_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R1)
                    | P_Fld(0xe, RK1_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R1));
        vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK6, P_Fld(0x14, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R1)
                    | P_Fld(0x12, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R1)
                    | P_Fld(0x27, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R1)
                    | P_Fld(0x1d, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ1_P1_B1R1));
        //               61832 ===dramc_shu1_lp4_3200 end===


        //               66870 ===ddrphy_shu1_lp4_3200_CHA begin===
        if(p->dram_cbt_mode==CBT_BYTE_MODE1)
        {
                vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0x1, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0);
                vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x1, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1);
        }
        else
        {
                vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x1, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
                            | P_Fld(0x1, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0));
                vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x1, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
                            | P_Fld(0x1, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1));
//francis remove : it will make CLRPLL frequency wrong!
//francis remove                vIO32WriteFldMulti(DDRPHY_SHU1_PLL7, P_Fld(0x3d00, SHU1_PLL7_RG_RCLRPLL_SDM_PCW)
//francis remove                            | P_Fld(0x1, SHU1_PLL7_RG_RCLRPLL_SDM_PCW_CHG));
        }
        //               67761 ===ddrphy_shu1_lp4_3200_CHA end===

        //NOT included in parsing tool
        vIO32WriteFldMulti(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(0x0, SHU_AC_TIME_05T_TWTR_M05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TR2W_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TWTPD_M05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TR2PD_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TFAW_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TRRD_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TWR_M05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TRAS_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TRPAB_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TRP_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TRCD_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TRTP_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TXP_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TRFC_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TRC_05T));
        vIO32WriteFldAlign(DRAMC_REG_SHU_DQS2DQ_TX, 0x0, SHU_DQS2DQ_TX_OE2DQ_OFFSET);
        vIO32WriteFldMulti(DRAMC_REG_SHURK2_SELPH_DQSG1, P_Fld(0x4, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS1_GATED_P1)
                | P_Fld(0x0, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS1_GATED)
                | P_Fld(0x4, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS0_GATED_P1)
                | P_Fld(0x0, SHURK2_SELPH_DQSG1_REG_DLY_R2DQS0_GATED));
    }
    ///TODO: DDR3733
    if(p->frequency==1866)
    {
        DramcSetting_Olympus_LP4_ByteMode_DDR3733(p);
    }
    ///TODO: DDR2667
    if(p->frequency==1333)
    {
        DramcSetting_Olympus_LP4_ByteMode_DDR2667(p);
    }
    ///TODO: DDR1600
    if(p->frequency==800)
    {
        DramcSetting_Olympus_LP4_ByteMode_DDR1600(p);
    }
    ///TODO: DDR1600
    if(p->frequency==400)
    {
        DramcSetting_Olympus_LP4_ByteMode_DDR800(p);
    }

    UpdateInitialSettings_LP4(p);
#if SIMULATION_SW_IMPED
    if (p->dram_type == TYPE_LPDDR4)
        DramcSwImpedanceSaveRegister(p, ODT_ON, DRAM_DFS_SHUFFLE_1);
    else if (p->dram_type == TYPE_LPDDR4X)
        DramcSwImpedanceSaveRegister(p, p->odt_onoff, DRAM_DFS_SHUFFLE_1);
    else if (p->dram_type == TYPE_LPDDR4P)
        DramcSwImpedanceSaveRegister(p, ODT_OFF, DRAM_DFS_SHUFFLE_1);
#endif

#ifndef LOOPBACK_TEST
    DDRPhyFreqMeter();
#endif

//    DramcModeRegInit_LP4(p, 0);

    mcDELAY_US(1);
#if 0
    vIO32WriteFldMulti(DRAMC_REG_MRS, P_Fld(0x0, MRS_MRSRK)
                | P_Fld(0x4, MRS_MRSMA)
                | P_Fld(0x0, MRS_MRSOP));
    mcDELAY_US(1);
    vIO32WriteFldAlign(DRAMC_REG_SPCMD, 0x1, SPCMD_MRREN);
    vIO32WriteFldAlign(DRAMC_REG_SPCMD, 0x0, SPCMD_MRREN);
#endif

    vIO32WriteFldAlign(DRAMC_REG_TEST2_4, 0x0, TEST2_4_TEST_REQ_LEN1);

    vIO32WriteFldAlign(DRAMC_REG_SHU_CONF3, 0x5, SHU_CONF3_ZQCSCNT);

    mcDELAY_US(1);

#if !APPLY_LP4_POWER_INIT_SEQUENCE
    vIO32WriteFldMulti(DRAMC_REG_CKECTRL, P_Fld(0x0, CKECTRL_CKEFIXON)
                | P_Fld(0x0, CKECTRL_CKE1FIXON));
#endif
    vIO32WriteFldMulti(DRAMC_REG_REFCTRL0, P_Fld(0x1, REFCTRL0_PBREFEN)
                | P_Fld(0x1, REFCTRL0_PBREF_DISBYRATE));
    if(p->dram_cbt_mode==CBT_BYTE_MODE1)
    {
        vIO32WriteFldAlign(DRAMC_REG_DQSOSCR, 0x1, DQSOSCR_RK0_BYTE_MODE);
        vIO32WriteFldAlign(DRAMC_REG_DQSOSCR, 0x1, DQSOSCR_RK1_BYTE_MODE);
    }
    vIO32WriteFldMulti(DRAMC_REG_SHUCTRL2, P_Fld(0x1, SHUCTRL2_MR13_SHU_EN)
                | P_Fld(0x1, SHUCTRL2_HWSET_WLRL));
    vIO32WriteFldAlign(DRAMC_REG_REFCTRL0, 0x0, REFCTRL0_REFDIS);
    //vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x0, SPCMDCTRL_REFRDIS);//Lewis@20160613: Fix refresh rate is wrong while diable MR4
    vIO32WriteFldMulti(DRAMC_REG_DRAMCTRL, P_Fld(0x1, DRAMCTRL_REQQUE_THD_EN)
                | P_Fld(0x1, DRAMCTRL_DPDRK_OPT));
    vIO32WriteFldMulti(DRAMC_REG_DUMMY_RD, P_Fld(0x1, DUMMY_RD_DQSG_DMYRD_EN)
                | P_Fld(0x2, DUMMY_RD_RANK_NUM)
                | P_Fld(0x1, DUMMY_RD_DUMMY_RD_CNT6)
                | P_Fld(0x1, DUMMY_RD_DUMMY_RD_CNT5)
                | P_Fld(0x1, DUMMY_RD_DUMMY_RD_CNT3)
                | P_Fld(0x1, DUMMY_RD_DUMMY_RD_SW));
    vIO32WriteFldAlign(DRAMC_REG_TEST2_4, 0x4, TEST2_4_TESTAGENTRKSEL);
    vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x0, DRAMCTRL_CTOREQ_HPRI_OPT);
    //               60759 === DE initial sequence done ===


///TODO: DVFS_Enable

    mcDELAY_US(1);

      //*((UINT32P)(DDRPHY0AO_BASE + 0x0000)) = 0x00000000;

    mcDELAY_US(1);


#if (fcFOR_CHIP_ID == fcWhitney)
    vIO32WriteFldMulti(DRAMC_REG_SHUCTRL, P_Fld(0x0, SHUCTRL_R_DRAMC_CHA) | P_Fld(0x0, SHUCTRL_SHU_PHYRST_SEL));
    vIO32WriteFldMulti(DRAMC_REG_SHUCTRL2, P_Fld(0x1, SHUCTRL2_R_DVFS_DLL_CHA));
#endif
    //               60826 ===LP4_3200_intial_setting_shu1 end===


    DramcModeRegInit_LP4(p, 0);
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

}

enum
{
    AC_TIMING_DRAM_TYPE=0,
    AC_TIMING_FREQUENCY,
    AC_TIMING_DBI_ONOFF,// Olympus new
    AC_TIMING_BYTE_MODE,// Olympus new

    AC_TIMING_TRAS,
    AC_TIMING_TRP,
    AC_TIMING_TRPAB,
    AC_TIMING_TRC,
    AC_TIMING_TRFC,
    AC_TIMING_TRFCPB,
    AC_TIMING_TXP,
    AC_TIMING_TRTP,
    AC_TIMING_TRCD,
    AC_TIMING_TWR,
    AC_TIMING_TWTR,
    AC_TIMING_TRRD,
    AC_TIMING_TFAW,
    AC_TIMING_TRTW_ODT_OFF,// Olympus new (External)
    AC_TIMING_TRTW_ODT_ON,// Olympus new (External)
    AC_TIMING_REFCNT, //(REFFRERUN = 0)
    AC_TIMING_REFCNT_FR_CLK, //(REFFRERUN = 1)
    AC_TIMING_TXREFCNT,
    AC_TIMING_TZQCS,
    // Olympus new (External)
    AC_TIMING_TRTPD,                // LP4/LP3
    AC_TIMING_TWTPD,                // LP4/LP3
    AC_TIMING_TMRR2W_ODT_OFF,  // LP4
    AC_TIMING_TMRR2W_ODT_ON,   // LP4
    //AC_TIMING_CKEPRD,               // LP3
    // End of Olympus new

    AC_TIMING_TRAS_05T,
    AC_TIMING_TRP_05T,
    AC_TIMING_TRPAB_05T,
    AC_TIMING_TRC_05T,
    AC_TIMING_TRFC_05T,
    AC_TIMING_TRFCPB_05T,
    AC_TIMING_TXP_05T,
    AC_TIMING_TRTP_05T,
    AC_TIMING_TRCD_05T,
    AC_TIMING_TWR_05T,
    AC_TIMING_TWTR_05T,
    AC_TIMING_TRRD_05T,
    AC_TIMING_TFAW_05T,
    AC_TIMING_TRTW_05T_ODT_OFF,
    AC_TIMING_TRTW_05T_ODT_ON,

     // Remove by Yulia, no 0.5T.
    //AC_TIMING_REFCNT_05T, //(REFFRERUN = 0)
    //AC_TIMING_REFCNT_FR_CLK_05T, //(REFFRERUN = 1)
    //AC_TIMING_TXREFCNT_05T,
    //AC_TIMING_TZQCS_05T,

    // Olympus new (External)
    AC_TIMING_TRTPD_05T,                // LP4/LP3
    AC_TIMING_TWTPD_05T,                // LP4/LP3
    //AC_TIMING_TMRR2W_05T_ODT_OFF,  // LP4 no 0.5T
    //AC_TIMING_TMRR2W_05T_ODT_ON,   // LP4  no 0.5T
    //AC_TIMING_CKEPRD,               // LP3
    // End of Olympus new

    AC_TIMING_XRTW2W,
    AC_TIMING_XRTW2R,
    AC_TIMING_XRTR2W,
    AC_TIMING_XRTR2R,

    AC_TIMING_DMCATRAIN_INTV,   // Olympus new, LP4 internal
    ////AC_TIMING_DMMRW_INTV,           // LP4 internal, move to initial setting
    ////AC_TIMING_DMFSPCHG_PRDCNT,  //LP4 internal, move to initial setting
    #if 0
    AC_TIMING_WRITE_LATENCY_0x41c,
    AC_TIMING_WRITE_LATENCY_0x420,
    AC_TIMING_WRITE_LATENCY_0x424,
    AC_TIMING_WRITE_LATENCY_0x428,
    AC_TIMING_WRITE_LATENCY_0x42c,
    #endif

    AC_TIMING_DQSINCTL_FOR_GATING,
    AC_TIMING_DATLAT,
    AC_TIMING_MODE_REG_WL,
    AC_TIMING_MODE_REG_RL,

    AC_TIMING_ITEM_NUM
};


#define TOTAL_AC_TIMING_NUMBER  12

const U32 ACTimingTable[TOTAL_AC_TIMING_NUMBER][AC_TIMING_ITEM_NUM] =
{
    //--------LP4 byte mode ---------------------------------------------
       // LP4-3200, 1600MHz
         {
              TYPE_LPDDR4, // Dram type
              1600 , // Dram frequency
              DBI_ON,
              CBT_BYTE_MODE1,

              //====  AC timing ====
              9  , // 0 : TRAS
              6   , // 1 : TRP
              1   , // 2: TPRAB
              17  , // 3:TRC
              101 , // 4:TFRC
              45  , // 5:TRFCPB
              1   , //6:TXP
              3   , //7:TRTP
              6   , //8:TRCD
              14  , //9:TWR
              8   , //10: TWTR
              3   , //11:TRRD
              8   , //12:TFAW
              7,  //6   , //13:TRTW_ODT_OFF
              10, //9   , //13:TRTW_ODT_ON
              97  , //14:REFCNT
              101 , //15:REFCNT_FR_CLK
              118 , //16:TXREFCNT
              34  , //17:TZQCS

              13, //12 , //TRTPD
              12 , // TWTPD
              10, //9 ,   // TMRR2W
              12, //11 , // TMRR2W

                  // ====  AC timing 0.5T====
              1   ,// TRAS_0.5T
              1   ,// TRP_0.5T
              0   ,// TRPAB_0.5T
              1   ,// TRC_0.5T
              0   ,// TRFC_0.5T
              0   ,// TRFCPB_0.5T
              0   ,// TXP_0.5T
              1   ,// TRTP_0.5T
              1   ,// TRCD_0.5T
              1   ,// TWR_0.5T
              0   ,// TWTR_0.5T
              0   ,// TRRD_0.5T
              0   ,// TFAW_0.5T
              0   ,// TRTW_0.5T_ODT_OFF
              0   ,// TRTW_0.5T_ODT_ON

              0,         //AC_TIMING_TRTPD_05T
              0,          //AC_TIMING_TWTPD_05T

                  // Cross rank setting
              9    ,// XRTW2W
              3    , // XRTW2R
              9    ,// XRTR2W
              10  , // XRTR2R

              8       , // DMCATRAIN_INTV

              6   ,//DQSINCTL = 5+1 for DBI ON
              16 , //Datlat
              14,  // TODO: WL
              36   // TODO: RL
          },

         // LP4-2667, 1333MHz
        {
             TYPE_LPDDR4, // Dram type
             1333 , // Dram frequency
             DBI_OFF,       // LP4-2667 byte mode can apply to both DBI_ON and DBI_Off
             CBT_BYTE_MODE1,

             //====  AC timing ====
             6  , // 0 : TRAS
             5   , // 1 : TRP
             1   , // 2: TPRAB
             13  , // 3:TRC
             82  , // 4:TFRC
             36  , // 5:TRFCPB
             0   , //6:TXP
             2   , //7:TRTP
             5   , //8:TRCD
             12  , //9:TWR
             7   , //10: TWTR
             2   , //11:TRRD
             5   , //12:TFAW
             5   , //13:TRTW_ODT_OFF
             8   , //13:TRTW_ODT_ON
             81  , //14:REFCNT
             101 , //15:REFCNT_FR_CLK
             99  , //16:TXREFCNT
             28  , //17:TZQCS

             10 , //TRTPD
             11 , // TWTPD
             7 ,   // TMRR2W
             9  , // TMRR2W

                 // ====  AC timing 0.5T====
             1   ,// TRAS_0.5T
             0   ,// TRP_0.5T
             0   ,// TRPAB_0.5T
             0   ,// TRC_0.5T
             1   ,// TRFC_0.5T
             0   ,// TRFCPB_0.5T
             1   ,// TXP_0.5T
             0   ,// TRTP_0.5T
             0   ,// TRCD_0.5T
             1   ,// TWR_0.5T
             1   ,// TWTR_0.5T
             1   ,// TRRD_0.5T
             1   ,// TFAW_0.5T
             0   ,// TRTW_0.5T_ODT_OFF
             0   ,// TRTW_0.5T_ODT_ON

             0,         //AC_TIMING_TRTPD_05T
             1,          //AC_TIMING_TWTPD_05T

                 // Cross rank setting
             8     , // XRTW2W
             3     , // XRTW2R
             7     , // XRTR2W
             9     , // XRTR2R
             8       , // DMCATRAIN_INTV, Jeremy: no shuffle, set same value as highest freq.

             4   ,//DQSINCTL
             14 , //Datlat
             12,  // TODO: WL
             28   // TODO: RL
         },

        {
            TYPE_LPDDR4, // Dram type
            800 , // Dram frequency
            DBI_OFF,       // LP4-1600 byte mode can apply to both DBI_ON and DBI_Off
            CBT_BYTE_MODE1,

            //====  AC timing ====
            1  , // 0 : TRAS
            3   , // 1 : TRP
            0   , // 2: TPRAB
            5   , // 3:TRC
            45  , // 4:TFRC
            17  , // 5:TRFCPB
            0   , //6:TXP
            2   , //7:TRTP
            3   , //8:TRCD
            8   , //9:TWR
            4   , //10: TWTR
            1   , //11:TRRD
            0   , //12:TFAW
            3   , //13:TRTW_ODT_OFF
            5   , //13:TRTW_ODT_ON
            48  , //14:REFCNT
            101 , //15:REFCNT_FR_CLK
            61  , //16:TXREFCNT
            16  , //17:TZQCS

            7 , //TRTPD
            7 , // TWTPD
            4,   // TMRR2W
            6 , // TMRR2W

            // ====  AC timing 0.5T====
            0   ,// TRAS_0.5T
            0   ,// TRP_0.5T
            1   ,// TRPAB_0.5T
            0   ,// TRC_0.5T
            0   ,// TRFC_0.5T
            0   ,// TRFCPB_0.5T
            0   ,// TXP_0.5T
            1   ,// TRTP_0.5T
            0   ,// TRCD_0.5T
            0   ,// TWR_0.5T
            0   ,// TWTR_0.5T
            0   ,// TRRD_0.5T
            0   ,// TFAW_0.5T
            0   ,// TRTW_0.5T_ODT_OFF
            0   ,// TRTW_0.5T_ODT_ON

            0,         //AC_TIMING_TRTPD_05T
            1,          //AC_TIMING_TWTPD_05T

            // Cross rank setting
            7     , // XRTW2W
            3     , // XRTW2R
            4     , // XRTR2W
            8     , // XRTR2R

            8       , // DMCATRAIN_INTV, Jeremy: no shuffle, set same value as highest freq.

            2   ,//DQSINCTL
            11, //Datlat = normal mode +1
            14,  // TODO: WL
            32   // TODO: RL
        },


    //--------LP4 Normal mode ---------------------------------------------
    // LP4-3733, 1866MHz
        {
             TYPE_LPDDR4, // Dram type
             1866 , // Dram frequency
             DBI_ON,
             CBT_NORMAL_MODE,

             //====  AC timing ====
             11  , // 0 : TRAS                            
             6  , // 1 : TRP                             
             1   , // 2: TPRAB
             20  , // 3:TRC                              
             118 , // 4:TFRC                             
             53  , // 5:TRFCPB                           
             1   , //6:TXP
             2   , //7:TRTP                              
             7   , //8:TRCD
             14  , //9:TWR                               
             8   , //10: TWTR
             3   , //11:TRRD                             
             1   , //12:TFAW                             
             7  , //13:TRTW_ODT_OFF
             10  , //13:TRTW_ODT_ON
             113  , //14:REFCNT
             101 , //15:REFCNT_FR_CLK
             138 , //16:TXREFCNT                         
             40  , //17:TZQCS

             13    , //TRTPD
             13    , // TWTPD
             10     , // TMRR2W
             12    , // TMRR2W

                 // ====  AC timing 0.5T====
             1   ,// TRAS_0.5T
             1   ,// TRP_0.5T
             1   ,// TRPAB_0.5T
             1   ,// TRC_0.5T
             1   ,// TRFC_0.5T
             1   ,// TRFCPB_0.5T
             1   ,// TXP_0.5T
             0   ,// TRTP_0.5T
             1   ,// TRCD_0.5T
             1   ,// TWR_0.5T
             0   ,// TWTR_0.5T
             0   ,// TRRD_0.5T
             0   ,// TFAW_0.5T
             0   ,// TRTW_0.5T_ODT_OFF
             0   ,// TRTW_0.5T_ODT_ON

             0     ,//AC_TIMING_TRTPD_05T
             0     ,//AC_TIMING_TWTPD_05T

                 // Cross rank setting
             6    ,// XRTW2W                             
             3    , // XRTW2R
             9    ,// XRTR2W
             11   , // XRTR2R                             

             10     , // DMCATRAIN_INTV

             5   ,//DQSINCTL = 4+1 for DBI ON
             18, //13, //17, //21, //18, //19 //15 , //Datlat
             16,  // TODO: WL
             36   // TODO: RL
         },
         
   // LP4-3200, 1600MHz
        {
             TYPE_LPDDR4, // Dram type
             1600 , // Dram frequency
             DBI_ON,
             CBT_NORMAL_MODE,

             //====  AC timing ====
             8  , // 0 : TRAS                            
             5  , // 1 : TRP                             
             1   , // 2: TPRAB
             16  , // 3:TRC                              
             100 , // 4:TFRC                             
             44  , // 5:TRFCPB                           
             1   , //6:TXP
             2   , //7:TRTP                              
             6   , //8:TRCD
             12  , //9:TWR                               
             7   , //10: TWTR
             2   , //11:TRRD                             
             7   , //12:TFAW                             
             6  , //13:TRTW_ODT_OFF
             9  , //13:TRTW_ODT_ON
             97  , //14:REFCNT
             101 , //15:REFCNT_FR_CLK
             119 , //16:TXREFCNT                         
             34  , //17:TZQCS

             12    , //TRTPD
             12    , // TWTPD
             9     , // TMRR2W
             11    , // TMRR2W

                 // ====  AC timing 0.5T====
             1   ,// TRAS_0.5T
             1   ,// TRP_0.5T
             0   ,// TRPAB_0.5T
             1   ,// TRC_0.5T
             0   ,// TRFC_0.5T
             0   ,// TRFCPB_0.5T
             0   ,// TXP_0.5T
             1   ,// TRTP_0.5T
             1   ,// TRCD_0.5T
             1   ,// TWR_0.5T
             0   ,// TWTR_0.5T
             0   ,// TRRD_0.5T
             0   ,// TFAW_0.5T
             0   ,// TRTW_0.5T_ODT_OFF
             0   ,// TRTW_0.5T_ODT_ON

             0     ,//AC_TIMING_TRTPD_05T
             1     ,//AC_TIMING_TWTPD_05T

                 // Cross rank setting
             6    ,// XRTW2W                             
             3    , // XRTW2R
             8    ,// XRTR2W
             11   , // XRTR2R                             

             8     , // DMCATRAIN_INTV

             5   ,//DQSINCTL = 4+1 for DBI ON
             15 , //Datlat
             14,  // TODO: WL
             32   // TODO: RL
         },


         // LP4-2667, 1333MHz
         {
              TYPE_LPDDR4, // Dram type
              1333 , // Dram frequency
              DBI_ON,
              CBT_NORMAL_MODE,

              //====  AC timing ====
              5  , // 0 : TRAS                              
              4  , // 1 : TRP                               
              1   , // 2: TPRAB
              12  , // 3:TRC                                
              81  , // 4:TFRC                               
              35  , // 5:TRFCPB                             
              0   , //6:TXP
              1   , //7:TRTP                                
              5   , //8:TRCD
              10  , //9:TWR                                 
              6   , //10: TWTR
              1   , //11:TRRD                               
              4   , //12:TFAW                               
              5  , //13:TRTW_ODT_OFF
              8  , //13:TRTW_ODT_ON
              81  , //14:REFCNT
              101 , //15:REFCNT_FR_CLK
              100 , //16:TXREFCNT                           
              28  , //17:TZQCS

              10    , //TRTPD
              10    , // TWTPD
              7     , // TMRR2W
              9     , // TMRR2W

              // ====  AC timing 0.5T====
              1   ,// TRAS_0.5T
              0   ,// TRP_0.5T
              0   ,// TRPAB_0.5T
              0   ,// TRC_0.5T
              1   ,// TRFC_0.5T
              0   ,// TRFCPB_0.5T
              1   ,// TXP_0.5T
              0   ,// TRTP_0.5T
              0   ,// TRCD_0.5T
              0   ,// TWR_0.5T
              0   ,// TWTR_0.5T
              1   ,// TRRD_0.5T
              1   ,// TFAW_0.5T
              0   ,// TRTW_0.5T_ODT_OFF
              0   ,// TRTW_0.5T_ODT_ON

              0     ,//AC_TIMING_TRTPD_05T
              1     ,//AC_TIMING_TWTPD_05T

              // Cross rank setting
              6         ,// XRTW2W                              
              3         , // XRTW2R
              7         ,// XRTR2W
              11        , // XRTR2R                             

              8     , // DMCATRAIN_INTV, Jeremy: no shuffle, set same value as highest freq.

               4         ,//DQSINCTL = 3+1 for DBI ON
              13        ,//Datlat
              12        ,// TODO: WL
              28   // TODO: RL
          },

      //--------LP4 Normal mode ---------------------------------------------
      // LP4-3733, 1866MHz
          {
               TYPE_LPDDR4, // Dram type
               1866 , // Dram frequency
               DBI_OFF,
               CBT_NORMAL_MODE,
      
               //====  AC timing ====
               11  , // 0 : TRAS                            
               6  , // 1 : TRP                             
               1   , // 2: TPRAB
               20  , // 3:TRC                              
               118 , // 4:TFRC                             
               53  , // 5:TRFCPB                           
               1   , //6:TXP
               2   , //7:TRTP                              
               7   , //8:TRCD
               14  , //9:TWR                               
               8   , //10: TWTR
               3   , //11:TRRD                             
               1   , //12:TFAW                             
               6  , //13:TRTW_ODT_OFF
               9  , //13:TRTW_ODT_ON
               113  , //14:REFCNT
               101 , //15:REFCNT_FR_CLK
               138 , //16:TXREFCNT                         
               40  , //17:TZQCS
      
               12    , //TRTPD
               13    , // TWTPD
               9     , // TMRR2W
               11    , // TMRR2W
      
                   // ====  AC timing 0.5T====
               1   ,// TRAS_0.5T
               1   ,// TRP_0.5T
               1   ,// TRPAB_0.5T
               1   ,// TRC_0.5T
               1   ,// TRFC_0.5T
               1   ,// TRFCPB_0.5T
               1   ,// TXP_0.5T
               0   ,// TRTP_0.5T
               1   ,// TRCD_0.5T
               1   ,// TWR_0.5T
               0   ,// TWTR_0.5T
               0   ,// TRRD_0.5T
               0   ,// TFAW_0.5T
               0   ,// TRTW_0.5T_ODT_OFF
               0   ,// TRTW_0.5T_ODT_ON
      
               0     ,//AC_TIMING_TRTPD_05T
               0     ,//AC_TIMING_TWTPD_05T
      
                   // Cross rank setting
               6    ,// XRTW2W                             
               3    , // XRTW2R
               8    ,// XRTR2W
               11   , // XRTR2R                             
      
               10     , // DMCATRAIN_INTV
      
               4   ,//DQSINCTL
               18, //13, //17, //21, //18, //19 //15 , //Datlat
               16,  // TODO: WL
               32   // TODO: RL
           },


        // LP4-3200, 1600MHz
           {                                                
                TYPE_LPDDR4, // Dram type                   
                1600 , // Dram frequency                    
                DBI_OFF,                                     
                CBT_NORMAL_MODE,                            
                                                            
                //====  AC timing ====                      
                8  , // 0 : TRAS                            
                5  , // 1 : TRP                             
                1   , // 2: TPRAB                           
                16  , // 3:TRC                              
                100 , // 4:TFRC                             
                44  , // 5:TRFCPB                           
                1   , //6:TXP                               
                2   , //7:TRTP                              
                6   , //8:TRCD                              
                12  , //9:TWR                               
                7   , //10: TWTR                            
                2   , //11:TRRD                             
                7   , //12:TFAW                             
                5  , //13:TRTW_ODT_OFF                      
                8  , //13:TRTW_ODT_ON                       
                97  , //14:REFCNT                           
                101 , //15:REFCNT_FR_CLK                    
                119 , //16:TXREFCNT                         
                34  , //17:TZQCS                            
                                                            
                11    , //TRTPD                               
                12    , // TWTPD                              
                8     , // TMRR2W                             
                10    , // TMRR2W                             
                                                            
                    // ====  AC timing 0.5T====             
                1   ,// TRAS_0.5T                           
                1   ,// TRP_0.5T                            
                0   ,// TRPAB_0.5T                          
                1   ,// TRC_0.5T                            
                0   ,// TRFC_0.5T                           
                0   ,// TRFCPB_0.5T                         
                0   ,// TXP_0.5T                            
                1   ,// TRTP_0.5T                           
                1   ,// TRCD_0.5T                           
                1   ,// TWR_0.5T                            
                0   ,// TWTR_0.5T                           
                0   ,// TRRD_0.5T                           
                0   ,// TFAW_0.5T                           
                0   ,// TRTW_0.5T_ODT_OFF                   
                0   ,// TRTW_0.5T_ODT_ON                    
                                                            
                0     ,//AC_TIMING_TRTPD_05T                  
                1     ,//AC_TIMING_TWTPD_05T                  
                                                            
                    // Cross rank setting                   
                6    ,// XRTW2W                             
                3    , // XRTW2R                            
                7    ,// XRTR2W                             
                11   , // XRTR2R                             
                                                            
                8     , // DMCATRAIN_INTV                     
                                                                                                                     
                4   ,//DQSINCTL                             
                15 , //Datlat                               
                14,  // TODO: WL                            
                28   // TODO: RL                            
            },                                              
      
      
            // LP4-2667, 1333MHz                    
            {                                                  
                 TYPE_LPDDR4, // Dram type                     
                 1333 , // Dram frequency                       
                 DBI_OFF,                                       
                 CBT_NORMAL_MODE,                              
                                                               
                 //====  AC timing ====                        
                 5  , // 0 : TRAS                              
                 4  , // 1 : TRP                               
                 1   , // 2: TPRAB                             
                 12  , // 3:TRC                                
                 81  , // 4:TFRC                               
                 35  , // 5:TRFCPB                             
                 0   , //6:TXP                                 
                 1   , //7:TRTP                                
                 5   , //8:TRCD                                
                 10  , //9:TWR                                 
                 6   , //10: TWTR                              
                 1   , //11:TRRD                               
                 4   , //12:TFAW                               
                 4  , //13:TRTW_ODT_OFF                        
                 7  , //13:TRTW_ODT_ON                         
                 81  , //14:REFCNT                             
                 101 , //15:REFCNT_FR_CLK                      
                 100 , //16:TXREFCNT                           
                 28  , //17:TZQCS                              
                                                               
                 9    , //TRTPD                                 
                 10    , // TWTPD                                
                 6     , // TMRR2W                               
                 8     , // TMRR2W                               
                                                               
                 // ====  AC timing 0.5T====                   
                 1   ,// TRAS_0.5T                             
                 0   ,// TRP_0.5T                              
                 0   ,// TRPAB_0.5T                            
                 0   ,// TRC_0.5T                              
                 1   ,// TRFC_0.5T                             
                 0   ,// TRFCPB_0.5T                           
                 1   ,// TXP_0.5T                              
                 0   ,// TRTP_0.5T                             
                 0   ,// TRCD_0.5T                             
                 0   ,// TWR_0.5T                              
                 0   ,// TWTR_0.5T                             
                 1   ,// TRRD_0.5T                             
                 1   ,// TFAW_0.5T                             
                 0   ,// TRTW_0.5T_ODT_OFF                     
                 0   ,// TRTW_0.5T_ODT_ON                      
                                                               
                 0     ,//AC_TIMING_TRTPD_05T                    
                 1     ,//AC_TIMING_TWTPD_05T                    
                                                               
                 // Cross rank setting                         
                 6         ,// XRTW2W                              
                 3         , // XRTW2R                             
                 6         ,// XRTR2W                              
                 11        , // XRTR2R                             
                                                               
                 8     , // DMCATRAIN_INTV, Jeremy: no shuffle, set same value as highest freq.                       
                                                                                                                           
                  3         ,//DQSINCTL                             
                 13        ,//Datlat                               
                 12        ,// TODO: WL                            
                 24   // TODO: RL                              
             },                                                
                                 
            // LP4-1600, 800MHz             
          {
                TYPE_LPDDR4, // Dram type
                800 , // Dram frequency
                DBI_OFF,                                        
                CBT_NORMAL_MODE,

                //====  AC timing ====
                0  , // 0 : TRAS                               
                2   , // 1 : TRP                               
                0   , // 2: TPRAB
                4   , // 3:TRC                                 
                44  , // 4:TFRC                                
                16  , // 5:TRFCPB                              
                0   , //6:TXP
                1   , //7:TRTP                                 
                3   , //8:TRCD
                7   , //9:TWR                                  
                4   , //10: TWTR
                0   , //11:TRRD                                
                0   , //12:TFAW   //Jeremy check                             
                2   , //13:TRTW_ODT_OFF                        
                5   , //13:TRTW_ODT_ON
                48  , //14:REFCNT
                101 , //15:REFCNT_FR_CLK
                62  , //16:TXREFCNT                            
                16  , //17:TZQCS

                6 , //TRTPD                                    
                6 , // TWTPD
                3,   // TMRR2W                                 
                5 , // TMRR2W                                  

                // ====  AC timing 0.5T====
                0   ,// TRAS_0.5T
                0   ,// TRP_0.5T
                1   ,// TRPAB_0.5T
                0   ,// TRC_0.5T
                0   ,// TRFC_0.5T
                0   ,// TRFCPB_0.5T
                0   ,// TXP_0.5T
                1   ,// TRTP_0.5T
                0   ,// TRCD_0.5T
                1   ,// TWR_0.5T
                1   ,// TWTR_0.5T
                0   ,// TRRD_0.5T
                0   ,// TFAW_0.5T
                0   ,// TRTW_0.5T_ODT_OFF
                0   ,// TRTW_0.5T_ODT_ON

                0,         //AC_TIMING_TRTPD_05T
                0,          //AC_TIMING_TWTPD_05T

                // Cross rank setting
                6     , // XRTW2W                              
                3     , // XRTW2R
                4     , // XRTR2W
                11     , // XRTR2R                              

                8       , // DMCATRAIN_INTV, Jeremy: no shuffle, set same value as highest freq.

                1   ,//DQSINCTL
                10, //Datlat = normal mode +1
                8,  // TODO: WL
                14   // TODO: RL                               
          },
              
              // LP4-800, 400MHz
          ///TODO: copy from DDR1600, need to update to DDR800
          {
              TYPE_LPDDR4, // Dram type
              400 , // Dram frequency
              DBI_OFF,                                        
              CBT_NORMAL_MODE,

              //====  AC timing ====
              0  , // 0 : TRAS
              1   , // 1 : TRP
              0   , // 2: TPRAB
              0   , // 3:TRC
              16  , // 4:TFRC
              2  , // 5:TRFCPB
              0   , //6:TXP
              1   , //7:TRTP
              1   , //8:TRCD
              5   , //9:TWR
              4   , //10: TWTR
              0   , //11:TRRD
              0   , //12:TFAW
              2   , //13:TRTW_ODT_OFF
              5   , //13:TRTW_ODT_ON
              24  , //14:REFCNT
              101 , //15:REFCNT_FR_CLK
              33  , //16:TXREFCNT
              7  , //17:TZQCS

              6 , //TRTPD
              5 , // TWTPD
              3,   // TMRR2W
              5 , // TMRR2W

              // ====  AC timing 0.5T====
              0   ,// TRAS_0.5T
              0   ,// TRP_0.5T
              1   ,// TRPAB_0.5T
              0   ,// TRC_0.5T
              0   ,// TRFC_0.5T
              0   ,// TRFCPB_0.5T
              0   ,// TXP_0.5T
              1   ,// TRTP_0.5T
              0   ,// TRCD_0.5T
              0   ,// TWR_0.5T
              1   ,// TWTR_0.5T
              0   ,// TRRD_0.5T
              0   ,// TFAW_0.5T
              0   ,// TRTW_0.5T_ODT_OFF
              0   ,// TRTW_0.5T_ODT_ON

              0,         //AC_TIMING_TRTPD_05T
              0,          //AC_TIMING_TWTPD_05T

              // Cross rank setting
              7     , // XRTW2W
              3     , // XRTW2R
              4     , // XRTR2W
              11     , // XRTR2R

              8       , // DMCATRAIN_INTV

              1   ,//DQSINCTL
              10, //Datlat = normal mode +1
              8,  // TODO: WL
              14   // TODO: RL
          },
              
    //--------LP3 ---------------------------------------------
       // LP3-1866, 933MHz
          {
              TYPE_LPDDR3, // Dram type
              933 , // Dram frequency
              DBI_OFF,
              CBT_NORMAL_MODE,

              //====  AC timing ====
              12 , // 0 : TRAS
              10  , // 1 : TRP
              1   , // 2: TPRAB
              24  , // 3:TRC
              87  , // 4:TFRC
              31  , // 5:TRFCPB
              1   , //6:TXP
              3   , //7:TRTP
              10  , //8:TRCD
              13  , //9:TWR
              7   , //10: TWTR
              4   , //11:TRRD
              15  , //12:TFAW
#if DramcHWDQSGatingTracking_DVT_JADE_TRACKING_MODE
              7   , //13:TRTW_ODT_OFF
              7   , //13:TRTW_ODT_ON
#else
              6   , //13:TRTW_ODT_OFF
              6   , //13:TRTW_ODT_ON
#endif
              112 , //14:REFCNT
              101 , //15:REFCNT_FR_CLK
              101  , //16:TXREFCNT
              41  , //17:TZQCS

              10 , //TRTPD
              11 , // TWTPD
              0,   // TMRR2W
              0 , // TMRR2W

              // ====  AC timing 0.5T====
              0   ,// TRAS_0.5T
              1   ,// TRP_0.5T
              1   ,// TRPAB_0.5T
              1   ,// TRC_0.5T
              0   ,// TRFC_0.5T
              0   ,// TRFCPB_0.5T
              1   ,// TXP_0.5T
              1   ,// TRTP_0.5T
              1   ,// TRCD_0.5T
              1   ,// TWR_0.5T
              0   ,// TWTR_0.5T
              0   ,// TRRD_0.5T
              1   ,// TFAW_0.5T
              1   ,// TRTW_0.5T_ODT_OFF
              1   ,// TRTW_0.5T_ODT_ON

              0,         //AC_TIMING_TRTPD_05T
              0,          //AC_TIMING_TWTPD_05T

              // Cross rank setting
              8     , // XRTW2W
#if DramcHWDQSGatingTracking_DVT_JADE_TRACKING_MODE
              5     , // XRTW2R
#else
              2     , // XRTW2R
#endif
              6     , // XRTR2W
              10     , // XRTR2R

              0       , // DMCATRAIN_INTV

              5   ,//DQSINCTL
              17, //Datlat
              8,  // TODO: WL
              14   // TODO: RL
          },


         // LP3-1333, 667MHz
          {
              TYPE_LPDDR3, // Dram type
              667 , // Dram frequency
              DBI_OFF,
              CBT_NORMAL_MODE,

              //====  AC timing ====
              6  , // 0 : TRAS
              7   , // 1 : TRP
              1   , // 2: TPRAB
              15  , // 3:TRC
              59  , // 4:TFRC
              19  , // 5:TRFCPB
              0   , //6:TXP
              2   , //7:TRTP
              7   , //8:TRCD
              10  , //9:TWR
              5   , //10: TWTR
              2   , //11:TRRD
              9   , //12:TFAW
#if DramcHWDQSGatingTracking_DVT_JADE_TRACKING_MODE
              5   , //13:TRTW_ODT_OFF
              5   , //13:TRTW_ODT_ON
#else
              4   , //13:TRTW_ODT_OFF
              4   , //13:TRTW_ODT_ON
#endif
              80  , //14:REFCNT
              101 , //15:REFCNT_FR_CLK
              72   , //16:TXREFCNT
              29  , //17:TZQCS

              7 , //TRTPD
              8 , // TWTPD
              0,   // TMRR2W
              0 , // TMRR2W

              // ====  AC timing 0.5T====
              0   ,// TRAS_0.5T
              0   ,// TRP_0.5T
              0   ,// TRPAB_0.5T
              0   ,// TRC_0.5T
              0   ,// TRFC_0.5T
              0   ,// TRFCPB_0.5T
              1   ,// TXP_0.5T
              1   ,// TRTP_0.5T
              0   ,// TRCD_0.5T
              1   ,// TWR_0.5T
              0   ,// TWTR_0.5T
              1   ,// TRRD_0.5T
              0   ,// TFAW_0.5T
              1   ,// TRTW_0.5T_ODT_OFF
              1   ,// TRTW_0.5T_ODT_ON

              0,         //AC_TIMING_TRTPD_05T
              0,          //AC_TIMING_TWTPD_05T

              // Cross rank setting
              7     , // XRTW2W
#if DramcHWDQSGatingTracking_DVT_JADE_TRACKING_MODE
              5     , // XRTW2R
#else
              2     , // XRTW2R
#endif
              6     , // XRTR2W
              8     , // XRTR2R

              0       , // DMCATRAIN_INTV

              3   ,//DQSINCTL
              14, //Datlat
              6,  // TODO: WL
              10   // TODO: RL
          },

#if 0// LP3-933 is removed due to gating PI hole.
        // LP3-933, 467MHz
          {
              TYPE_LPDDR3, // Dram type
              467 , // Dram frequency
              DBI_OFF,
              CBT_NORMAL_MODE,

              //====  AC timing ====
              2  , // 0 : TRAS
              5   , // 1 : TRP
              0   , // 2: TPRAB
              8   , // 3:TRC
              38  , // 4:TFRC
              10  , // 5:TRFCPB
              0   , //6:TXP
              1   , //7:TRTP
              5   , //8:TRCD
              7   , //9:TWR
              4   , //10: TWTR
              1   , //11:TRRD
              4   , //12:TFAW
#if DramcHWDQSGatingTracking_DVT_JADE_TRACKING_MODE
              4   , //13:TRTW_ODT_OFF
              4   , //13:TRTW_ODT_ON
#else
              3   , //13:TRTW_ODT_OFF
              3   , //13:TRTW_ODT_ON
#endif
              55  , //14:REFCNT
              101 , //15:REFCNT_FR_CLK
              50   , //16:TXREFCNT
              20  , //17:TZQCS

              5 , //TRTPD
              5 , // TWTPD
              0,   // TMRR2W
              0 , // TMRR2W

              // ====  AC timing 0.5T====
              0   ,// TRAS_0.5T
              0   ,// TRP_0.5T
              1   ,// TRPAB_0.5T
              1   ,// TRC_0.5T
              0   ,// TRFC_0.5T
              0   ,// TRFCPB_0.5T
              0   ,// TXP_0.5T
              0   ,// TRTP_0.5T
              0   ,// TRCD_0.5T
              0   ,// TWR_0.5T
              1   ,// TWTR_0.5T
              1   ,// TRRD_0.5T
              0   ,// TFAW_0.5T
              1   ,// TRTW_0.5T_ODT_OFF
              1   ,// TRTW_0.5T_ODT_ON

              0,         //AC_TIMING_TRTPD_05T
              0,          //AC_TIMING_TWTPD_05T

              // Cross rank setting
              7     , // XRTW2W
#if DramcHWDQSGatingTracking_DVT_JADE_TRACKING_MODE
              5     , // XRTW2R
#else
              2     , // XRTW2R
#endif
              6     , // XRTR2W
              8     , // XRTR2R

              0       , // DMCATRAIN_INTV

              2   ,//DQSINCTL
              12, //Datlat
              4,  // TODO: WL
              8   // TODO: RL
          },
#endif
};

#if ENABLE_MRR_AFTER_FIRST_K
void vDramcACTimingOptimize(DRAMC_CTX_T *p)
{
    U8 u1TRFC=101, u1TRFC_05T=0, u1TXREFCNT=118;

    if(u1IsLP4Family(p->dram_type))  //already read MR8 for density update
    {
        if (p->frequency == 1600)
    {
        switch(p->density)
        {
            case 0x0://4Gb per die (2Gb per channel), tRFCab=130
                u1TRFC = 41;
                u1TRFC_05T=0;
                u1TXREFCNT = 58;
                break;

            case 0x1://6Gb, tRFCab=180
            case 0x2://8Gb, tRFCab=180
                u1TRFC = 61;
                u1TRFC_05T=0;
                u1TXREFCNT = 78;
                break;
            case 0x3://12Gb, tRFCab=280
            case 0x4://16Gb, tRFCab=280
            default:
                u1TRFC = 101;
                u1TRFC_05T=0;
                u1TXREFCNT = 118;
        }
	}
        else
        if (p->frequency == 1333)
        {
            switch(p->density)
            {
                case 0x0://4Gb per die (2Gb per channel), tRFCab=130
                    u1TRFC = 33;
                    u1TRFC_05T=1;
                    u1TXREFCNT = 50;
                    break;

                case 0x1://6Gb, tRFCab=180
                case 0x2://8Gb, tRFCab=180
                    u1TRFC = 50;
                    u1TRFC_05T=0;
                    u1TXREFCNT = 67;
                    break;
                case 0x3://12Gb, tRFCab=280
                case 0x4://16Gb, tRFCab=280
                default:
                    u1TRFC = 81;
                    u1TRFC_05T=1;
                    u1TXREFCNT = 100;
            }
        }        
        else
        if (p->frequency == 1866)
        {
            switch(p->density)
            {
                case 0x0://4Gb per die (2Gb per channel), tRFCab=130
                    u1TRFC = 48;
                    u1TRFC_05T=1;
                    u1TXREFCNT = 68;
                    break;

                case 0x1://6Gb, tRFCab=180
                case 0x2://8Gb, tRFCab=180
                    u1TRFC = 72;
                    u1TRFC_05T=0;
                    u1TXREFCNT = 92;
                    break;
                case 0x3://12Gb, tRFCab=280
                case 0x4://16Gb, tRFCab=280
                default:
                    u1TRFC = 118;
                    u1TRFC_05T=1;
                    u1TXREFCNT = 138;
            }
        } 
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM3), u1TRFC, SHU_ACTIM3_TRFC);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_AC_TIME_05T), u1TRFC_05T, SHU_AC_TIME_05T_TRFC_05T);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM4), u1TXREFCNT, SHU_ACTIM4_TXREFCNT);
    }

    mcSHOW_DBG_MSG(("[vDramcACTimingOptimize] Density %d, u1TRFC %d, u1TRFC_05T %d, u1TXREFCNT %d\n", p->density, u1TRFC, u1TRFC_05T, u1TXREFCNT));
    mcFPRINTF((fp_A60501, "[vDramcACTimingOptimize] Density %d, u1TRFC %d, u1TRFC_05T %d, u1TXREFCNT %d\n", p->density, u1TRFC, u1TRFC_05T, u1TXREFCNT));
}
#endif

DRAM_STATUS_T DdrUpdateACTimingReg(DRAMC_CTX_T *p, const U32* ACTable)
{
    U32 RODT_TRACKING_SAVEING_UI=0;

    if(ACTable == NULL)
        return DRAM_FAIL;


#if ENABLE_RODT_TRACKING
        if (p->odt_onoff == ODT_ON) RODT_TRACKING_SAVEING_UI= 1; //RODT_TRACKING eanble can save r2w 1ui
        else RODT_TRACKING_SAVEING_UI = 0;
#endif   

    

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM0),
                                    P_Fld( ACTable[AC_TIMING_TRCD], SHU_ACTIM0_TRCD)   |
                                    P_Fld( ACTable[AC_TIMING_TRRD], SHU_ACTIM0_TRRD)    |
                                    P_Fld( ACTable[AC_TIMING_TWR], SHU_ACTIM0_TWR)      |
                                    P_Fld( ACTable[AC_TIMING_TWTR], SHU_ACTIM0_TWTR));

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM1),
                                    P_Fld( ACTable[AC_TIMING_TRC], SHU_ACTIM1_TRC)         |
                                    P_Fld( ACTable[AC_TIMING_TRAS], SHU_ACTIM1_TRAS)     |
                                    P_Fld( ACTable[AC_TIMING_TRP] ,SHU_ACTIM1_TRP)         |
                                    P_Fld( ACTable[AC_TIMING_TRPAB], SHU_ACTIM1_TRPAB));

    #if 1//!SUPPORT_HYNIX_RX_DQS_WEAK_PULL
    if((p->dram_type == TYPE_LPDDR4P) && (p->vendor_id == VENDOR_HYNIX)) //temp solution, need to discuss with SY
    {
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM2),
                                    P_Fld( ACTable[AC_TIMING_TFAW], SHU_ACTIM2_TFAW)         |
                                    P_Fld( ACTable[AC_TIMING_TRTW_ODT_OFF + 1] - RODT_TRACKING_SAVEING_UI, SHU_ACTIM2_TR2W)               |
                                    P_Fld( ACTable[AC_TIMING_TRTP], SHU_ACTIM2_TRTP)         |
                                    P_Fld( ACTable[AC_TIMING_TXP], SHU_ACTIM2_TXP));
    }
    else
    #endif
    {
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM2),
                                    P_Fld( ACTable[AC_TIMING_TFAW], SHU_ACTIM2_TFAW)         |
                                    P_Fld( ACTable[AC_TIMING_TRTW_ODT_OFF + p->odt_onoff] - RODT_TRACKING_SAVEING_UI, SHU_ACTIM2_TR2W)               |
                                    P_Fld( ACTable[AC_TIMING_TRTP], SHU_ACTIM2_TRTP)         |
                                    P_Fld( ACTable[AC_TIMING_TXP], SHU_ACTIM2_TXP));
    }

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM3),
                                    P_Fld( ACTable[AC_TIMING_TRFC], SHU_ACTIM3_TRFC)         |
                                    P_Fld( ACTable[AC_TIMING_TRFCPB], SHU_ACTIM3_TRFCPB));

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM4),
                                    P_Fld( ACTable[AC_TIMING_TZQCS], SHU_ACTIM4_TZQCS)         |
                                    P_Fld( ACTable[AC_TIMING_REFCNT_FR_CLK], SHU_ACTIM4_REFCNT_FR_CLK)               |
                                    P_Fld( ACTable[AC_TIMING_REFCNT], SHU_ACTIM4_REFCNT)         |
                                    P_Fld( ACTable[AC_TIMING_TXREFCNT], SHU_ACTIM4_TXREFCNT));

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM5),
                                    P_Fld( ACTable[AC_TIMING_TMRR2W_ODT_OFF+ p->odt_onoff], SHU_ACTIM5_TMRR2W) |
                                    P_Fld( ACTable[AC_TIMING_TWTPD], SHU_ACTIM5_TWTPD) |
                                    P_Fld( ACTable[AC_TIMING_TRTPD], SHU_ACTIM5_TR2PD));

    // AC timing 0.5T
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_AC_TIME_05T),
                                    P_Fld(ACTable[AC_TIMING_TWTR_05T], SHU_AC_TIME_05T_TWTR_M05T)  |
                                    P_Fld(ACTable[AC_TIMING_TRTW_05T_ODT_OFF+ p->odt_onoff],        SHU_AC_TIME_05T_TR2W_05T)           |
                                    P_Fld(ACTable[AC_TIMING_TWTPD_05T],       SHU_AC_TIME_05T_TWTPD_M05T ) |
                                    P_Fld(ACTable[AC_TIMING_TRTPD_05T],     SHU_AC_TIME_05T_TR2PD_05T)         |
                                    P_Fld(ACTable[AC_TIMING_TFAW_05T],        SHU_AC_TIME_05T_TFAW_05T)               |
                                    P_Fld(ACTable[AC_TIMING_TRRD_05T],       SHU_AC_TIME_05T_TRRD_05T)         |
                                    P_Fld(ACTable[AC_TIMING_TWR_05T],   SHU_AC_TIME_05T_TWR_M05T)           |
                                    P_Fld(ACTable[AC_TIMING_TRAS_05T] ,    SHU_AC_TIME_05T_TRAS_05T ) |
                                    P_Fld(ACTable[AC_TIMING_TRPAB_05T],   SHU_AC_TIME_05T_TRPAB_05T)         |
                                    P_Fld(ACTable[AC_TIMING_TRP_05T],   SHU_AC_TIME_05T_TRP_05T)               |
                                    P_Fld(ACTable[AC_TIMING_TRCD_05T] ,  SHU_AC_TIME_05T_TRCD_05T)         |
                                    P_Fld(ACTable[AC_TIMING_TRTP_05T],   SHU_AC_TIME_05T_TRTP_05T)        |
                                    P_Fld(ACTable[AC_TIMING_TXP_05T],   SHU_AC_TIME_05T_TXP_05T)         |
                                    P_Fld(ACTable[AC_TIMING_TRFC_05T],   SHU_AC_TIME_05T_TRFC_05T)               |
                                    P_Fld(ACTable[AC_TIMING_TRFCPB_05T] ,  SHU_AC_TIME_05T_TRFCPB_05T)         |
                                    P_Fld(ACTable[AC_TIMING_TRC_05T],   SHU_AC_TIME_05T_TRC_05T)
                                    );
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM_XRT),
                                    P_Fld(ACTable[AC_TIMING_XRTW2W], SHU_ACTIM_XRT_XRTW2W) |
                                    P_Fld(ACTable[AC_TIMING_XRTW2R], SHU_ACTIM_XRT_XRTW2R) |
                                    P_Fld(ACTable[AC_TIMING_XRTR2W] - RODT_TRACKING_SAVEING_UI, SHU_ACTIM_XRT_XRTR2W) |
                                    P_Fld(ACTable[AC_TIMING_XRTR2R], SHU_ACTIM_XRT_XRTR2R));

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_CATRAINING1), ACTable[AC_TIMING_DMCATRAIN_INTV], CATRAINING1_CATRAIN_INTV);

    // Write latency
#if 0//ndef OLYMPUS_TO_BE_PORTING
    vIO32Write4B_All(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), ACTable[AC_TIMING_WRITE_LATENCY_0x41c]);
    vIO32Write4B_All(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), ACTable[AC_TIMING_WRITE_LATENCY_0x420]);
    vIO32Write4B_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_DQS0), ACTable[AC_TIMING_WRITE_LATENCY_0x424]);
    vIO32Write4B_All(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), ACTable[AC_TIMING_WRITE_LATENCY_0x428]);
    vIO32Write4B_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_DQS1), ACTable[AC_TIMING_WRITE_LATENCY_0x42c]);
#endif

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSCTL), ACTable[AC_TIMING_DQSINCTL_FOR_GATING], SHURK0_DQSCTL_DQSINCTL);// Rank 0 DQSINCTL
 
    // AC timing table only need to apply to rank0, because of CSSwap for calibration will use only rank0
    //vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DQSCTL2), ACTable[AC_TIMING_DQSINCTL_FOR_GATING], DQSCTL2_R1DQSINCTL); //Rank1 DQSINCTL, no use in A-PHY.
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_CONF1),
                                        P_Fld(ACTable[AC_TIMING_DATLAT], SHU_CONF1_DATLAT) |
                                        P_Fld(ACTable[AC_TIMING_DATLAT] -1, SHU_CONF1_DATLAT_DSEL) |
                                        P_Fld(ACTable[AC_TIMING_DATLAT] -2, SHU_CONF1_DATLAT_DSEL_PHY));

    return DRAM_OK;
}


DRAM_STATUS_T DdrUpdateACTiming(DRAMC_CTX_T *p)
{
    U8 u1TimingIdx=0xff, u1TmpIdx;
    U8 u1TmpDramType = p->dram_type;

    // LP4 and LP4x use same table.
    if(u1TmpDramType== TYPE_LPDDR4X || u1TmpDramType == TYPE_LPDDR4P)
        u1TmpDramType = TYPE_LPDDR4;

    for(u1TmpIdx=0; u1TmpIdx<TOTAL_AC_TIMING_NUMBER; u1TmpIdx++)
    {
        if((ACTimingTable[u1TmpIdx][AC_TIMING_DRAM_TYPE] == u1TmpDramType) &&
            (ACTimingTable[u1TmpIdx][AC_TIMING_FREQUENCY] == p->frequency) &&
            (ACTimingTable[u1TmpIdx][AC_TIMING_DBI_ONOFF] == p->DBI_R_onoff) &&
            (ACTimingTable[u1TmpIdx][AC_TIMING_BYTE_MODE] == p->dram_cbt_mode))
        {
            u1TimingIdx = u1TmpIdx;
            mcSHOW_DBG_MSG(("[DdrUpdateACTiming] match AC timing %d\n", u1TimingIdx));
            mcFPRINTF((fp_A60501, "[DdrUpdateACTiming] match AC timing %d\n", u1TimingIdx));
            break;
        }
    }

    if(u1TimingIdx == 0xff)
    {
        #if 0
        if(u1TmpDramType = TYPE_LPDDR4)
            u1TimingIdx =0;
        else // LPDDR3
            u1TimingIdx =6;
        mcSHOW_ERR_MSG(("[DdrUpdateACTiming] Error, no match AC timing, use default timing %d\n", u1TimingIdx));
        mcFPRINTF((fp_A60501, "[DdrUpdateACTiming] Error, no match AC timing, use default timing %d\n", u1TimingIdx));
        #else
        mcSHOW_ERR_MSG(("[DdrUpdateACTiming] Error, no match AC timing, not apply table\n"));
        mcFPRINTF((fp_A60501, "[DdrUpdateACTiming] Error, no match AC timing, not apply table\n"));
        return DRAM_FAIL;
        #endif
    }

    DdrUpdateACTimingReg(p, ACTimingTable[u1TimingIdx]);

    return DRAM_OK;
}

#if ((!SW_CHANGE_FOR_SIMULATION) && (!FOR_DV_SIMULATION_USED && SW_CHANGE_FOR_SIMULATION==0) && (!__ETT__))
DRAM_STATUS_T DdrUpdateACTiming_EMI(DRAMC_CTX_T *p, AC_TIMING_EXTERNAL_T *ACRegFromEmi)
{
    U8 u1TimingIdx=0xff, u1TmpIdx;
    U32 ACTiming[AC_TIMING_ITEM_NUM];
    U8 u1TmpDramType = p->dram_type;

   if(ACRegFromEmi == NULL)
        return DRAM_FAIL;

    //Get AC timing from emi setting
    ACTiming[AC_TIMING_DRAM_TYPE] =  p->dram_type;
    ACTiming[AC_TIMING_FREQUENCY] =  ACRegFromEmi->AC_TIME_EMI_FREQUENCY ;
    ACTiming[AC_TIMING_TRAS] =  ACRegFromEmi->AC_TIME_EMI_TRAS ;
    ACTiming[AC_TIMING_TRP] =  ACRegFromEmi->AC_TIME_EMI_TRP ;

    ACTiming[AC_TIMING_TRPAB] =  ACRegFromEmi->AC_TIME_EMI_TRPAB ;
    ACTiming[AC_TIMING_TRC] =  ACRegFromEmi->AC_TIME_EMI_TRC ;
    ACTiming[AC_TIMING_TRFC] =  ACRegFromEmi->AC_TIME_EMI_TRFC ;
    ACTiming[AC_TIMING_TRFCPB] =  ACRegFromEmi->AC_TIME_EMI_TRFCPB ;

    ACTiming[AC_TIMING_TXP] =  ACRegFromEmi->AC_TIME_EMI_TXP ;
    ACTiming[AC_TIMING_TRTP] =  ACRegFromEmi->AC_TIME_EMI_TRTP ;
    ACTiming[AC_TIMING_TRCD] =  ACRegFromEmi->AC_TIME_EMI_TRCD ;
    ACTiming[AC_TIMING_TWR] =  ACRegFromEmi->AC_TIME_EMI_TWR ;
    ACTiming[AC_TIMING_TWTR] =  ACRegFromEmi->AC_TIME_EMI_TWTR ;
    ACTiming[AC_TIMING_TRRD] =  ACRegFromEmi->AC_TIME_EMI_TRRD ;
    ACTiming[AC_TIMING_TFAW] =  ACRegFromEmi->AC_TIME_EMI_TFAW ;
    ACTiming[AC_TIMING_TRTW_ODT_OFF] =  ACRegFromEmi->AC_TIME_EMI_TRTW_ODT_OFF ;
    ACTiming[AC_TIMING_TRTW_ODT_ON] =  ACRegFromEmi->AC_TIME_EMI_TRTW_ODT_ON ;
    ACTiming[AC_TIMING_REFCNT] =  ACRegFromEmi->AC_TIME_EMI_REFCNT ; //(REFFRERUN = 0)
    ACTiming[AC_TIMING_REFCNT_FR_CLK] =  ACRegFromEmi->AC_TIME_EMI_REFCNT_FR_CLK ; //(REFFRERUN = 1)
    ACTiming[AC_TIMING_TXREFCNT] =  ACRegFromEmi->AC_TIME_EMI_TXREFCNT ;
    ACTiming[AC_TIMING_TZQCS] =  ACRegFromEmi->AC_TIME_EMI_TZQCS ;

    ACTiming[AC_TIMING_TRTPD] =  ACRegFromEmi->AC_TIME_EMI_TRTPD ;// oylmpus new
    ACTiming[AC_TIMING_TWTPD] =  ACRegFromEmi->AC_TIME_EMI_TWTPD ;// oylmpus new
    ACTiming[AC_TIMING_TMRR2W_ODT_OFF] =  ACRegFromEmi->AC_TIME_EMI_TMRR2W_ODT_OFF ;// oylmpus new
    ACTiming[AC_TIMING_TMRR2W_ODT_ON] =  ACRegFromEmi->AC_TIME_EMI_TMRR2W_ODT_ON;// oylmpus new

    ACTiming[AC_TIMING_TRAS_05T] =  ACRegFromEmi->AC_TIME_EMI_TRAS_05T ;
    ACTiming[AC_TIMING_TRP_05T] =  ACRegFromEmi->AC_TIME_EMI_TRP_05T ;
    ACTiming[AC_TIMING_TRPAB_05T] =  ACRegFromEmi->AC_TIME_EMI_TRPAB_05T ;
    ACTiming[AC_TIMING_TRC_05T] =  ACRegFromEmi->AC_TIME_EMI_TRC_05T ;
    ACTiming[AC_TIMING_TRFC_05T] =  ACRegFromEmi->AC_TIME_EMI_TRFC_05T ;
    ACTiming[AC_TIMING_TRFCPB_05T] =  ACRegFromEmi->AC_TIME_EMI_TRFCPB_05T ;
    ACTiming[AC_TIMING_TXP_05T] =  ACRegFromEmi->AC_TIME_EMI_TXP_05T ;
    ACTiming[AC_TIMING_TRTP_05T] =  ACRegFromEmi->AC_TIME_EMI_TRTP_05T ;
    ACTiming[AC_TIMING_TRCD_05T] =  ACRegFromEmi->AC_TIME_EMI_TRCD_05T ;
    ACTiming[AC_TIMING_TWR_05T] =  ACRegFromEmi->AC_TIME_EMI_TWR_05T ;
    ACTiming[AC_TIMING_TWTR_05T] =  ACRegFromEmi->AC_TIME_EMI_TWTR_05T ;

    ACTiming[AC_TIMING_TRRD_05T] =  ACRegFromEmi->AC_TIME_EMI_TRRD_05T ;
    ACTiming[AC_TIMING_TFAW_05T] =  ACRegFromEmi->AC_TIME_EMI_TFAW_05T ;
    ACTiming[AC_TIMING_TRTW_05T_ODT_OFF] =  ACRegFromEmi->AC_TIME_EMI_TRTW_ODT_OFF_05T ;
    ACTiming[AC_TIMING_TRTW_05T_ODT_ON] =  ACRegFromEmi->AC_TIME_EMI_TRTW_ODT_ON_05T ;

    ACTiming[AC_TIMING_TRTPD_05T] =  ACRegFromEmi->AC_TIME_EMI_TRTPD_05T ;
    ACTiming[AC_TIMING_TWTPD_05T] =  ACRegFromEmi->AC_TIME_EMI_TWTPD_05T ;

    //ACTiming[AC_TIMING_REFCNT_05T] =  ACRegFromEmi->AC_TIME_EMI_REFCNT_05T ; //(REFFRERUN = 0)
    //ACTiming[AC_TIMING_REFCNT_FR_CLK_05T] =  ACRegFromEmi->AC_TIME_EMI_REFCNT_FR_CLK_05T ; //(REFFRERUN = 1)
    //ACTiming[AC_TIMING_TXREFCNT_05T] =  ACRegFromEmi->AC_TIME_EMI_TXREFCNT_05T ;
    //ACTiming[AC_TIMING_TZQCS_05T] =  ACRegFromEmi->AC_TIME_EMI_TZQCS_05T ;

    // LP4 and LP4x use same table.
    if(u1TmpDramType== TYPE_LPDDR4X || u1TmpDramType == TYPE_LPDDR4P)
        u1TmpDramType = TYPE_LPDDR4;

    //Get AC timing from internal ACTimingTable
    for(u1TmpIdx=0; u1TmpIdx<TOTAL_AC_TIMING_NUMBER; u1TmpIdx++)
    {
        if((ACTimingTable[u1TmpIdx][AC_TIMING_DRAM_TYPE] == u1TmpDramType) &&
            (ACTimingTable[u1TmpIdx][AC_TIMING_FREQUENCY] == p->frequency) &&
            //(ACTimingTable[u1TmpIdx][AC_TIMING_DBI_ONOFF] == p->DBI_R_onoff) &&
            (ACTimingTable[u1TmpIdx][AC_TIMING_BYTE_MODE] == p->dram_cbt_mode))
        {
            u1TimingIdx = u1TmpIdx;
            mcSHOW_DBG_MSG(("[DdrUpdateACTiming_EMI] match AC timing %d\n", u1TimingIdx));
            mcFPRINTF((fp_A60501, "[DdrUpdateACTiming_EMI] match AC timing %d\n", u1TimingIdx));
            break;
        }
    }

    if(u1TimingIdx == 0xff)
    {
        u1TimingIdx =0;
        mcSHOW_DBG_MSG(("[DdrUpdateACTiming_EMI] Error, no match AC timing, use default timing 0\n"));
        mcFPRINTF((fp_A60501, "[DdrUpdateACTiming_EMI] Error, no match AC timing, use default timing 0\n"));
        //return DRAM_FAIL;
    }

    ACTiming[AC_TIMING_XRTW2W] =  ACTimingTable[u1TimingIdx][AC_TIMING_XRTW2W];
    ACTiming[AC_TIMING_XRTW2R] =  ACTimingTable[u1TimingIdx][AC_TIMING_XRTW2R];
    ACTiming[AC_TIMING_XRTR2W] =  ACTimingTable[u1TimingIdx][AC_TIMING_XRTR2W];
    ACTiming[AC_TIMING_XRTR2R] =  ACTimingTable[u1TimingIdx][AC_TIMING_XRTR2R];

    ACTiming[AC_TIMING_DMCATRAIN_INTV] =  ACTimingTable[u1TimingIdx][AC_TIMING_DMCATRAIN_INTV];

#if 0
    ACTiming[AC_TIMING_WRITE_LATENCY_0x41c] =  ACTimingTable[u1TimingIdx][AC_TIMING_WRITE_LATENCY_0x41c];
    ACTiming[AC_TIMING_WRITE_LATENCY_0x420] =  ACTimingTable[u1TimingIdx][AC_TIMING_WRITE_LATENCY_0x420];
    ACTiming[AC_TIMING_WRITE_LATENCY_0x424] =  ACTimingTable[u1TimingIdx][AC_TIMING_WRITE_LATENCY_0x424];
    ACTiming[AC_TIMING_WRITE_LATENCY_0x428] =  ACTimingTable[u1TimingIdx][AC_TIMING_WRITE_LATENCY_0x428];
    ACTiming[AC_TIMING_WRITE_LATENCY_0x42c] =  ACTimingTable[u1TimingIdx][AC_TIMING_WRITE_LATENCY_0x42c];
#endif
    ACTiming[AC_TIMING_DQSINCTL_FOR_GATING] =  ACTimingTable[u1TimingIdx][AC_TIMING_DQSINCTL_FOR_GATING];
    ACTiming[AC_TIMING_DATLAT] =  ACTimingTable[u1TimingIdx][AC_TIMING_DATLAT];
    ACTiming[AC_TIMING_MODE_REG_WL] =  ACTimingTable[u1TimingIdx][AC_TIMING_MODE_REG_WL];
    ACTiming[AC_TIMING_MODE_REG_RL] =  ACTimingTable[u1TimingIdx][AC_TIMING_MODE_REG_RL];

    DdrUpdateACTimingReg(p, ACTiming);

    return DRAM_OK;
}
#endif

DRAM_STATUS_T DramcInit(DRAMC_CTX_T *p)
{
    U32 save_ch, dram_t, highest_freq=0; //Darren
    #if (!__ETT__ && !FOR_DV_SIMULATION_USED && SW_CHANGE_FOR_SIMULATION==0)
    EMI_SETTINGS *emi_set; //Darren
    #endif

    mcSHOW_DBG_MSG(("[DramcInit] ====Begin====\n"));
    mcFPRINTF((fp_A60501, "[DramcInit] ====Begin====\n"));

    vSetPHY2ChannelMapping(p, CHANNEL_A);

#if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK    
    memset(p->arfgWriteLevelingInitShif, FALSE, sizeof(p->arfgWriteLevelingInitShif));
#endif
#if TX_PERBIT_INIT_FLOW_CONTROL
    memset(p->fgTXPerbifInit, FALSE, sizeof(p->fgTXPerbifInit));
#endif

    vIO32Write4B_All(DRAMC_REG_ADDR(DDRPHY_MISC_SPM_CTRL3), 0); //SPM control with to SW control

    if(u1IsLP4Family(p->dram_type))
    {
        highest_freq = 1600; //Darren
        DramcSetting_Olympus_LP4_ByteMode(p);
    }
#if ENABLE_LP3_SW
    else if(p->dram_type == TYPE_LPDDR3)
    {
        highest_freq = 933; //Darren
        DramcSetting_Olympus_LP3(p);
    }
#endif

#if (!__ETT__ && !FOR_DV_SIMULATION_USED && SW_CHANGE_FOR_SIMULATION==0)
    if(p->frequency == highest_freq)
    {
        if(emi_setting_index == -1)
            emi_set = &default_emi_setting;
        else
            emi_set = &emi_settings[emi_setting_index];
        if(emi_set->AcTimeEMI.AC_TIME_EMI_FREQUENCY == p->frequency)
        {
            DdrUpdateACTiming_EMI(p, &(emi_set->AcTimeEMI));
        }
        else
        {
            mcSHOW_DBG_MSG(("[DdrUpdateACTiming_EMI] frequency not match (MDL=%d, target=%d), use inner table setting\n", emi_set->AcTimeEMI.AC_TIME_EMI_FREQUENCY, p->frequency));
            DdrUpdateACTiming(p);
        }
    }
    else
#endif
    {
        //if(u1IsLP4Family(p->dram_type) && (p->dram_cbt_mode==CBT_BYTE_MODE1))  // tmp solution, need to take care normal mode and DBI
            DdrUpdateACTiming(p);
    }

#if 0  //update refresh rate
    // for free-run clk 26MHz, 0x62 * (1/26) = 3.8ns
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 0x62, DRAMC_PD_CTRL_REFCNT_FR_CLK);
    // for non-fre-run clk,  reg = 3.8 ns * f / 4 / 16;
    u4RefreshRate = 38*p->frequency/640;
    vIO32WriteFldAlign_All(DRAMC_REG_CONF2, u4RefreshRate, CONF2_REFCNT);
#endif

//WHITNEY_TO_BE_PORTING
#if (fcFOR_CHIP_ID == fcWhitney)
    // For kernel api for check LPDDR3/4/4X (Darren), only for fcOlympus and fcElbrus.
    // For Other chip, please confirm the register is free for SW use.
    save_ch = vGetPHY2ChannelMapping(p);
    vSetPHY2ChannelMapping(p, CHANNEL_A);

    switch (p->dram_type)
    {
        case TYPE_LPDDR3:
            dram_t = 1;
            break;
        case TYPE_LPDDR4:
            dram_t = 2;
            break;
        case TYPE_LPDDR4X:
            dram_t = 3;
            break;
        case TYPE_LPDDR4P:
            dram_t = 4;
            break;
        default:
            dram_t = 0;
            mcSHOW_ERR_MSG(("[DramcInit] Incorrect DRAM Type !!!\n"));
            break;
    }
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_ARBCTL), dram_t, ARBCTL_RSV_DRAM_TYPE);

    // For DRAM normal/byte mode, used by SW TX tracking
    if(p->dram_cbt_mode == CBT_BYTE_MODE1)
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_ARBCTL), 1, ARBCTL_RSV_DRAM_CBT);
    else
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_ARBCTL), 0, ARBCTL_RSV_DRAM_CBT);

    vSetPHY2ChannelMapping(p, save_ch);
#endif

    mcSHOW_DBG_MSG(("[DramcInit] ====Done====\n"));
    mcFPRINTF((fp_A60501, "[DramcInit] ====Done====\n"));

    return DRAM_OK;
}



#if 0
DRAM_STATUS_T ApplySSCSettingReg(U8 u1Percent, U8 u1Slope, U8 u1Dir)
{
    U32 u4PRD, u4Delta, u4Delta1;

    u4PRD = 13000/u1Slope;
    u4Delta = ((76 << 18) *u1Percent)/ (u4PRD *100);
    u4Delta1 = u4Delta;

    #if EVEREST_PORTING_TODO
    vIO32WriteFldAlign(SYS_REG_ADDR(PLLGP_RG_02), u4PRD, RG_02_MEMPLL_SDM_SSC_PRD);
    vIO32WriteFldAlign(SYS_REG_ADDR(PLLGP_RG_03), u4Delta1, RG_03_MEMPLL_SDM_SSC_DELTA1);
    vIO32WriteFldAlign(SYS_REG_ADDR(PLLGP_RG_04), u4Delta, RG_04_MEMPLL_SDM_SSC_DELTA);
#endif
    mcSHOW_DBG_MSG2(("[ApplySSCSettingReg] Percent %d, Slope %d kHz, Dir %d (Down)\n", \
    	u1Percent, u1Slope, u1Dir));

    mcSHOW_DBG_MSG2(("[ApplySSCSettingReg] u4PRD =(0x%x) , u4Delta (0x%x),  u4Delta1 (0x%x)\n", u4PRD, u4Delta, u4Delta1));
    return DRAM_OK;
}


DRAM_STATUS_T SscEnable(DRAMC_CTX_T *p)
{
    if (p->ssc_en == ENABLE)
    {
        mcSHOW_DBG_MSG(("Enable SSC...\n"));
        mcFPRINTF((fp_A60501, "Enable SSC...\n"));
        // RG_SYSPLL_SDM_SSC_EN = 1 (0x14[26])

#if EVEREST_PORTING_TODO
        vIO32WriteFldAlign(SYS_REG_ADDR(PLLGP_RG_11), 1, RG_11_SYSPLL_SDM_SSC_EN);
#endif
    }
    return DRAM_OK;
}
#endif

#if __ETT__ //remove unused function to save space of log string
void DramcEnterSelfRefresh(DRAMC_CTX_T *p, U8 op)
{
    U8 ucstatus = 0;
    U32 uiTemp;
    U32 u4TimeCnt;

    u4TimeCnt = TIME_OUT_CNT;

    mcSHOW_DBG_MSG(("[DramcEnterSelfRefresh]  op:%d (0:exit, 1:enter)\n", op));
    mcFPRINTF((fp_A60501, "[DramcEnterSelfRefresh] op:%d(0:exit, 1:enter)\n", op));

    if (op == 1) // enter self refresh
    {
        if(u1IsLP4Family(p->dram_type))
        {
            // ONLY work for LP4, not LP3
            // MISCA_SRFPD_DIS =1, self-refresh
            // MISCA_SRFPD_DIS =0, self-refresh power down
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SREFCTRL), 1, SREFCTRL_SRFPD_DIS);
        }

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SREFCTRL), 1, SREFCTRL_SELFREF);
        mcDELAY_US(2);
        uiTemp = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_SREF_STATE);
        while((uiTemp==0) &&(u4TimeCnt>0))
        {
            mcSHOW_DBG_MSG2(("Still not enter self refresh(%d)\n",u4TimeCnt));
            mcDELAY_US(1);
            uiTemp = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_SREF_STATE);
            u4TimeCnt --;
        }
    }
    else // exit self refresh
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SREFCTRL), 0, SREFCTRL_SELFREF);

        mcDELAY_US(2);
        uiTemp = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_SREF_STATE);
        while ((uiTemp!=0) &&(u4TimeCnt>0))
        {
            mcSHOW_DBG_MSG2(("Still not exit self refresh(%d)\n", u4TimeCnt));
            mcDELAY_US(1);
            uiTemp = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_SREF_STATE);
            u4TimeCnt--;
        }
    }

   if(u4TimeCnt ==0)
   {
        mcSHOW_DBG_MSG(("[DramcEnterSelfRefresh]  Self Rresh operation fail\n"));
        mcFPRINTF((fp_A60501, "[DramcEnterSelfRefresh] Self Rresh operation fail\n"));
   }
   else
   {
        mcSHOW_DBG_MSG(("[DramcEnterSelfRefresh]  Self Rresh operation done\n"));
        mcFPRINTF((fp_A60501, "[DramcEnterSelfRefresh] Self Rresh operation done\n"));
   }
}
#endif

void DramcEnterSelfRefresh_Everest(DRAMC_CTX_T *p, U8 op)
{
    DRAM_CHANNEL_T channel_bak = p->channel;
    p->channel = CHANNEL_A;
    DramcEnterSelfRefresh(p, op);
    p->channel = CHANNEL_B;
    DramcEnterSelfRefresh(p, op);
    p->channel = channel_bak;
}

#ifdef DUMMY_READ_FOR_TRACKING
void DramcDummyReadForTrackingEnable(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign_All(DRAMC_REG_DUMMY_RD, p->support_rank_num, DUMMY_RD_RANK_NUM);
    
    vIO32Write4B_All(DRAMC_REG_RK0_DUMMY_RD_WDATA0, 0xaa55aa55);
    vIO32Write4B_All(DRAMC_REG_RK0_DUMMY_RD_WDATA1, 0xaa55aa55);
    vIO32Write4B_All(DRAMC_REG_RK0_DUMMY_RD_WDATA2, 0xaa55aa55);
    vIO32Write4B_All(DRAMC_REG_RK0_DUMMY_RD_WDATA3, 0xaa55aa55);    
    
    vIO32WriteFldAlign_All(DRAMC_REG_TEST2_4, 4, TEST2_4_TESTAGENTRKSEL);//Dummy Read rank selection is controlled by Test Agent
    vIO32WriteFldAlign_All(DRAMC_REG_RK1_DUMMY_RD_ADR, 0, RK1_DUMMY_RD_ADR_DMY_RD_RK1_LEN);
    vIO32WriteFldAlign_All(DRAMC_REG_RK0_DUMMY_RD_ADR, 0, RK0_DUMMY_RD_ADR_DMY_RD_RK0_LEN);
    vIO32WriteFldMulti_All(DRAMC_REG_DUMMY_RD, P_Fld(1, DUMMY_RD_DMY_RD_RX_TRACK) | P_Fld(1, DUMMY_RD_DUMMY_RD_EN));
}
#endif

#ifdef IMPEDENCE_TRACKING_ENABLE
void DramcImpedenceHWSaving(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign_All(DRAMC_REG_IMPCAL, 1, IMPCAL_IMPCAL_HW_SAVE_ENABLE);
    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL0, 1, REFCTRL0_IMP_FREERUN_CLK);
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_IMPCAL1, 1, SHU_IMPCAL1_IMPCALCNT);  
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_SELPH_CA2, 0x7, SHU_SELPH_CA2_TXDLY_CMD);
}
void DramcImpedenceTrackingEnable(DRAMC_CTX_T *p)
{
    /* !!!! Olympus's Channel B IMP Tracking is bad, only enable Channel A's IMP Tracking !!!! */

    //Write (DRAMC _BASE+ 0x8B) [31:0] = 32'he4000000//enable impedance tracking
    vIO32WriteFldMulti_All(DRAMC_REG_IMPCAL, P_Fld(1, IMPCAL_IMPCAL_HW) | P_Fld(0, IMPCAL_IMPCAL_EN) | P_Fld(1, IMPCAL_IMPCAL_SWVALUE_EN) | P_Fld(1, IMPCAL_IMPCAL_NEW_OLD_SL));

    // no update imp CA, because CA is unterm now
    vIO32WriteFldAlign_All(DRAMC_REG_IMPCAL, 1, IMPCAL_IMPCAL_BYPASS_UPDATE_CA);   
}
#endif

#ifdef TDQSCK_PRECALCULATION_FOR_DVFS
void DramcDQSPrecalculation_preset(DRAMC_CTX_T *p)//Test tDQSCK_temp Pre-calculation
{
    DRAM_CHANNEL_T eOriChannel = p->channel;
    DRAM_RANK_T eOriRank = p->rank;
    int irank_num = 0, ichannel_num = 0;
    U32 u4value = 0;
    U32 u4value1 = 0;

    
    
    for(ichannel_num=0; ichannel_num<4; ichannel_num++)
    {
        vSetPHY2ChannelMapping(p, ichannel_num);  
        
        //mcSHOW_DBG_MSG(("Step1: Set DVFS HW enable\n"));
        //Step1: Set DVFS HW enable
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK1), 1, PRE_TDQSCK1_TDQSCK_PRECAL_HW);

        //mcSHOW_DBG_MSG(("Step2: Set jump ratio\n"));
        //Step2: Set jump ratio
/*
    F1:3200
    F2:2667
    F3:2667
    F4:800
    
    Name            DVFS setting        Location    Value
    Jump_RATIO_0    00 to 01(F1 to F2)  0x87[31:24] 01(Freq)/00(freq)  * 0x20---1A
    Jump_RATIO_1    00 to 10(F1 to F3)  0x87[23:16] 10(Freq)/00(freq)  * 0x20---1A
    Jump_RATIO_2    00 to 11(F1 to F4)  0x87[15:8]  11(Freq)/00(freq)  * 0x20---8
    Jump_RATIO_3    01 to 00(F2 to F1)  0x87[7:0]   00(Freq)/01(freq)  * 0x20---26
    Jump_RATIO_4    01 to 10(F2 to F3)  0x88[31:24] 10(Freq)/01(freq)  * 0x20---20
    Jump_RATIO_5    01 to 11(F2 to F4)  0x88[23:16] 11(Freq)/01(freq)  * 0x20---9
    Jump_RATIO_6    10 to 00(F3 to F1)  0x88[15:8]  00(Freq)/10(freq)  * 0x20---26
    Jump_RATIO_7    10 to 01(F3 to F2)  0x88[7:0]   01(Freq)/10(freq)  * 0x20---20
    Jump_RATIO_8    10 to 11(F3 to F4)  0x89[31:24] 11(Freq)/10(freq)  * 0x20---9
    Jump_RATIO_9    11 to 00(F4 to F1)  0x89[23:16] 00(Freq)/11(freq)  * 0x20---80
    Jump_RATIO_10   11 to 01(F4 to F2)  0x89[15:8]  01(Freq)/11(freq)  * 0x20---6A
    Jump_RATIO_11   11 to 10(F4 to F3)  0x89[7:0]   10(Freq)/11(freq)  * 0x20---6A
*/    
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK2),   P_Fld(0x1a ,PRE_TDQSCK2_TDDQSCK_JUMP_RATIO0) | \ 
                                                                    P_Fld(0x1a ,PRE_TDQSCK2_TDDQSCK_JUMP_RATIO1) | \
                                                                    P_Fld(0x08 ,PRE_TDQSCK2_TDDQSCK_JUMP_RATIO2) | \
                                                                    P_Fld(0x26 ,PRE_TDQSCK2_TDDQSCK_JUMP_RATIO3));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK3),   P_Fld(0x20 ,PRE_TDQSCK3_TDDQSCK_JUMP_RATIO4) | \ 
                                                                    P_Fld(0x09 ,PRE_TDQSCK3_TDDQSCK_JUMP_RATIO5) | \
                                                                    P_Fld(0x26 ,PRE_TDQSCK3_TDDQSCK_JUMP_RATIO6) | \
                                                                    P_Fld(0x20 ,PRE_TDQSCK3_TDDQSCK_JUMP_RATIO7));    
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK4),   P_Fld(0x09 ,PRE_TDQSCK4_TDDQSCK_JUMP_RATIO8) | \ 
                                                                    P_Fld(0x80 ,PRE_TDQSCK4_TDDQSCK_JUMP_RATIO9) | \
                                                                    P_Fld(0x6a ,PRE_TDQSCK4_TDDQSCK_JUMP_RATIO10) | \
                                                                    P_Fld(0x6a ,PRE_TDQSCK4_TDDQSCK_JUMP_RATIO11));         

        for(irank_num=0; irank_num<2; irank_num++)
        {
            vSetRank(p,irank_num);
        
            //mcSHOW_DBG_MSG(("Step3: Write UI/PI info to RG\n"));
            //Step3: Write UI/PI info to RG
//B0 ==========================            
            //Shuffle 0 =========================================
            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK1), (u4value << 3) | u4value1, RK0_PRE_TDQSCK1_TDQSCK_UIFREQ1_B0R0);//0xcc[5:0]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSIEN), SHURK0_DQSIEN_R0DQS0IEN);   
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK1), u4value, RK0_PRE_TDQSCK1_TDQSCK_PIFREQ1_B0R0);//0xcc[12:6]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK3), (u4value << 3) | u4value1, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ1_P1_B0R0);//0xce[5:0]
            
            //Shuffle 1 =========================================
            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK1), (u4value << 3) | u4value1, RK0_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R0);//0xcc[18:13] 

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_DQSIEN), SHURK0_DQSIEN_R0DQS0IEN);   
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK1), u4value, RK0_PRE_TDQSCK1_TDQSCK_PIFREQ2_B0R0);//0xcc[25:19]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK3), (u4value << 3) | u4value1, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R0);//0xce[11:6]            
            
            //Shuffle 2 =========================================
            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK2), (u4value << 3) | u4value1, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R0);//0xcd[5:0] 

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_DQSIEN), SHURK0_DQSIEN_R0DQS0IEN);   
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK2), u4value, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R0);//0xcd[12:6]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK3), (u4value << 3) | u4value1, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R0);//0xce[17:12]                        
            
            //Shuffle 3 =========================================
            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU4RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU4RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK2), (u4value << 3) | u4value1, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R0);//0xcd[18:13] 

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU4RK0_DQSIEN), SHURK0_DQSIEN_R0DQS0IEN);   
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK2), u4value, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R0);//0xcd[25:19]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU4RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU4RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK3), (u4value << 3) | u4value1, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R0);//0xce[23:18]                        

//B1 ==========================            
            //Shuffle 0 =========================================
            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK4), (u4value << 3) | u4value1, RK0_PRE_TDQSCK4_TDQSCK_UIFREQ1_B1R0);//0xcf[5:0]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSIEN), SHURK0_DQSIEN_R0DQS1IEN);   
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK4), u4value, RK0_PRE_TDQSCK4_TDQSCK_PIFREQ1_B1R0);//0xcf[12:6]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK6), (u4value << 3) | u4value1, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ1_P1_B1R0);//0xd1[5:0]
            
            //Shuffle 1 =========================================
            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK4), (u4value << 3) | u4value1, RK0_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R0);//0xcf[18:13] 

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_DQSIEN), SHURK0_DQSIEN_R0DQS1IEN);   
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK4), u4value, RK0_PRE_TDQSCK4_TDQSCK_PIFREQ2_B1R0);//0xcf[25:19]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK6), (u4value << 3) | u4value1, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R0);//0xd1[11:6]            
            
            //Shuffle 2 =========================================
            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK5), (u4value << 3) | u4value1, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R0);//0xd0[5:0] 

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_DQSIEN), SHURK0_DQSIEN_R0DQS1IEN);   
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK5), u4value, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R0);//0xd0[12:6]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK6), (u4value << 3) | u4value1, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R0);//0xd1[17:12]                        
            
            //Shuffle 3 =========================================
            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU4RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU4RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK5), (u4value << 3) | u4value1, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R0);//0xd0[18:13] 

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU4RK0_DQSIEN), SHURK0_DQSIEN_R0DQS1IEN);   
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK5), u4value, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R0);//0xd0[25:19]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU4RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU4RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK6), (u4value << 3) | u4value1, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R0);//0xd1[23:18]                        

         }    
        //mcSHOW_DBG_MSG((" Select HW flow\n"));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK1), P_Fld(0x1, PRE_TDQSCK1_TDQSCK_REG_DVFS));
        //Step4: Set Auto save to RG
        //mcSHOW_DBG_MSG(("Step 4: Select HW flow\n"));
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK1), 1, PRE_TDQSCK1_TDQSCK_HW_SW_UP_SEL);

    }
    vSetPHY2ChannelMapping(p, eOriChannel);
    vSetRank(p, eOriRank);
    return;
}
#endif

#if GATING_ONLY_FOR_DEBUG
void DramcGatingDebugRankSel(DRAMC_CTX_T *p, U8 u1Rank)
{
    #if DUAL_RANK_ENABLE
    if (p->support_rank_num==RANK_SINGLE)
    #endif
    {
        u1Rank =0;
    }

    vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL1, u1Rank, MISC_CTRL1_R_DMSTBENCMP_RK_OPT);
}

void DramcGatingDebugInit(DRAMC_CTX_T *p)
{
    U32 backupReg0x64[CHANNEL_NUMBER], backupReg0xC8[CHANNEL_NUMBER], backupReg0xD0[CHANNEL_NUMBER];
    U32 channel_idx, backup_channel;

    backup_channel = vGetPHY2ChannelMapping(p);
    for(channel_idx=CHANNEL_A; channel_idx<CHANNEL_NUMBER; channel_idx++)
    {
        vSetPHY2ChannelMapping(p, channel_idx);
        backupReg0x64[channel_idx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL));
        backupReg0xC8[channel_idx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR));
        backupReg0xD0[channel_idx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD));
    }
    vSetPHY2ChannelMapping(p, backup_channel);

    //Disable MR4 MR18/MR19, TxHWTracking, Dummy RD before reset
    vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_REFRDIS);    //MR4 Disable
    vIO32WriteFldMulti_All(DRAMC_REG_DQSOSCR, P_Fld(0x1, DQSOSCR_DQSOSCRDIS)|P_Fld(0x1, DQSOSCR_DQSOSCENDIS));  //MR18, MR19 Disable
    vIO32WriteFldMulti_All(DRAMC_REG_DUMMY_RD, P_Fld(0x0, DUMMY_RD_DUMMY_RD_EN)
                                            | P_Fld(0x0, DUMMY_RD_SREF_DMYRD_EN)
                                            | P_Fld(0x0, DUMMY_RD_DQSG_DMYRD_EN)
                                            | P_Fld(0x0, DUMMY_RD_DMY_RD_DBG));
    mcDELAY_US(4);

    DramPhyReset(p);

    //Restore backup regs
    for(channel_idx=CHANNEL_A; channel_idx<CHANNEL_NUMBER; channel_idx++)
    {
        vSetPHY2ChannelMapping(p, channel_idx);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), backupReg0x64[channel_idx]);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), backupReg0xC8[channel_idx]);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), backupReg0xD0[channel_idx]);
    }
    vSetPHY2ChannelMapping(p, backup_channel);


    //enable &reset DQS counter
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_DQSGCNTEN);
    mcDELAY_US(4);//wait 1 auto refresh after DQS Counter enable

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_DQSGCNTRST);
    mcDELAY_US(1);//delay 2T
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_DQSGCNTRST);
    //mcSHOW_DBG_MSG(("DramcGatingDebugInit done\n" ));
}

void DramcGatingDebugExit(DRAMC_CTX_T *p)
{
    //enable &reset DQS counter
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_DQSGCNTEN);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL1, 0, MISC_CTRL1_R_DMSTBENCMP_RK_OPT);
}

void DramcGatingDebug(DRAMC_CTX_T *p, U8 u1Channel)
{
    U32 LP3_DataPerByte[DQS_NUMBER];
    U32 u4DebugCnt[DQS_NUMBER];
    U16 u2DebugCntPerByte;

    U32 u4value, u4all_result_R, u4all_result_F, u4err_value;
    U8 backup_channel;

    backup_channel = p->channel;
    vSetPHY2ChannelMapping(p, u1Channel);

    mcDELAY_MS(10);

    LP3_DataPerByte[0] = (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_STBERR_RK0_R), MISC_STBERR_RK0_R_STBERR_RK0_R));
    LP3_DataPerByte[2] = (LP3_DataPerByte[0] >>8) & 0xff;
    LP3_DataPerByte[0] &= 0xff;

    u4all_result_R = LP3_DataPerByte[0] | (LP3_DataPerByte[2] <<8);

    // falling
    LP3_DataPerByte[0] = (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_STBERR_RK0_F), MISC_STBERR_RK0_F_STBERR_RK0_F));
    LP3_DataPerByte[2] = (LP3_DataPerByte[0] >>8) & 0xff;
    LP3_DataPerByte[0] &= 0xff;

    u4all_result_F = LP3_DataPerByte[0] | (LP3_DataPerByte[2] <<8);

     //read DQS counter
    u4DebugCnt[0] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DQSGNWCNT0));
    u4DebugCnt[1] = (u4DebugCnt[0] >> 16) & 0xffff;
    u4DebugCnt[0] &= 0xffff;

    u4DebugCnt[2] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DQSGNWCNT1));
    u4DebugCnt[3] = (u4DebugCnt[2] >> 16) & 0xffff;
    u4DebugCnt[2] &= 0xffff;

    mcSHOW_DBG_MSG(("\n[DramcGatingDebug] Channel %d , DQS count (B3->B0) 0x%H, 0x%H, 0x%H, 0x%H \nError flag Rank0 (B3->B0) %B %B  %B %B  %B %B  %B %B\n", \
                                   u1Channel, u4DebugCnt[3], u4DebugCnt[2], u4DebugCnt[1], u4DebugCnt[0], \
                                   (u4all_result_F>>24)&0xff, (u4all_result_R>>24)&0xff, \
                                   (u4all_result_F>>16)&0xff, (u4all_result_R>>16)&0xff, \
                                   (u4all_result_F>>8)&0xff,   (u4all_result_R>>8)&0xff, \
                                   (u4all_result_F)&0xff,         (u4all_result_R)&0xff));

    #if DUAL_RANK_ENABLE
    if (p->support_rank_num==RANK_DUAL)
    {
        LP3_DataPerByte[0] = (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_STBERR_RK1_R), MISC_STBERR_RK1_R_STBERR_RK1_R));//PHY_B
        LP3_DataPerByte[2] = (LP3_DataPerByte[0] >>8) & 0xff;
        LP3_DataPerByte[0] &= 0xff;

        u4all_result_R = LP3_DataPerByte[0] | (LP3_DataPerByte[2] <<8);

        // falling
        LP3_DataPerByte[0] = (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_STBERR_RK1_F), MISC_STBERR_RK1_F_STBERR_RK1_F));//PHY_B
        LP3_DataPerByte[2] = (LP3_DataPerByte[0] >>8) & 0xff;
        LP3_DataPerByte[0] &= 0xff;

        u4all_result_F = LP3_DataPerByte[0];

        mcSHOW_DBG_MSG(("Error flag Rank1 (B3->B0) %B %B  %B %B  %B %B  %B %B\n", \
                                       (u4all_result_F>>24)&0xff, (u4all_result_R>>24)&0xff, \
                                       (u4all_result_F>>16)&0xff, (u4all_result_R>>16)&0xff, \
                                       (u4all_result_F>>8)&0xff,   (u4all_result_R>>8)&0xff, \
                                       (u4all_result_F)&0xff,         (u4all_result_R)&0xff));
    }
    #else
    mcSHOW_DBG_MSG((" \n" ));
    #endif

    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_DQSGCNTRST);
    //mcDELAY_US(1);//delay 2T
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_DQSGCNTRST);

    vSetPHY2ChannelMapping(p, backup_channel);
}
#endif

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#if (__ETT__ || CPU_RW_TEST_AFTER_K)
void DramcDumpDebugInfo(DRAMC_CTX_T *p)
{
    U8 mpdivInSel, cali_shu_sel, mpdiv_shu_sel, u1RefreshRate;

    mpdivInSel= u4IO32ReadFldAlign(DDRPHY_MISC_CTRL2, MISC_CTRL2_PLL_SHU_GP);
    cali_shu_sel= u4IO32ReadFldAlign(DRAMC_REG_SHUCTRL, SHUCTRL_R_OTHER_SHU_GP);
    mpdiv_shu_sel= u4IO32ReadFldAlign(DRAMC_REG_SHUCTRL, SHUCTRL_R_MPDIV_SHU_GP);

    mcSHOW_DBG_MSG(("=========================\n"));

    // Read shuffle selection
    mcSHOW_ERR_MSG(("[DramcDumpDebugInfo] mpdivInSel %d, cali_shu_sel %d, mpdiv_shu_sel %d\n", mpdivInSel, cali_shu_sel, mpdiv_shu_sel));

    #if GATING_ONLY_FOR_DEBUG
    // Read gating error flag
    //DramcGatingDebugInit(p);
    DramcGatingDebug(p, CHANNEL_A);
    #if CHANNEL_NUMBER==2
    if(p->support_channel_num == CHANNEL_DUAL)
        DramcGatingDebug(p, CHANNEL_B);
    #endif
    #if CHANNEL_NUMBER==4
    if(p->support_channel_num == CHANNEL_FOURTH)
    {
        DramcGatingDebug(p, CHANNEL_B);
        DramcGatingDebug(p, CHANNEL_C);
        DramcGatingDebug(p, CHANNEL_D);
    }
    #endif
    #endif

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    // Read HW gating tracking
#ifdef HW_GATING
        DramcPrintHWGatingStatus(p, CHANNEL_A);
        if(u1IsLP4Family(p->dram_type))
        {
            DramcPrintHWGatingStatus(p, CHANNEL_B);
#if CHANNEL_NUMBER==4        
            DramcPrintHWGatingStatus(p, CHANNEL_C);
            DramcPrintHWGatingStatus(p, CHANNEL_D);        
#endif             
        }
#endif

#if ENABLE_RX_TRACKING_LP4
        DramcPrintRXDQDQSStatus(p, CHANNEL_A);
        if(u1IsLP4Family(p->dram_type))
        {
            DramcPrintRXDQDQSStatus(p, CHANNEL_B);        
#if CHANNEL_NUMBER==4        
            DramcPrintRXDQDQSStatus(p, CHANNEL_C);
            DramcPrintRXDQDQSStatus(p, CHANNEL_D);        
#endif                
        }
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
        mcSHOW_ERR_MSG(("[CHA] MRR(MR4) [10:8]=%x\n", u1RefreshRate));
        u1RefreshRate = u1GetMR4RefreshRate(p, CHANNEL_B);
        mcSHOW_ERR_MSG(("[CHB] MRR(MR4) [10:8]=%x\n", u1RefreshRate));
#if CHANNEL_NUMBER==4  
        u1RefreshRate = u1GetMR4RefreshRate(p, CHANNEL_C);
        mcSHOW_ERR_MSG(("[CHC] MRR(MR4) [10:8]=%x\n", u1RefreshRate));
        u1RefreshRate = u1GetMR4RefreshRate(p, CHANNEL_D);
        mcSHOW_ERR_MSG(("[CHD] MRR(MR4) [10:8]=%x\n", u1RefreshRate));
#endif        
#endif
#endif

    mcSHOW_DBG_MSG(("=========================\n"));
}
#endif
#endif

void vAutoRefreshSwitch(DRAMC_CTX_T *p, U8 option)
{
    if (option == ENABLE)
    {
        //enable autorefresh
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), 0, REFCTRL0_REFDIS);      //REFDIS=0, enable auto refresh        
    }
    else
    {
        //disable autorefresh
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), 1, REFCTRL0_REFDIS);      //REFDIS=1, disable auto refresh    

        //because HW will actually disable autorefresh after refresh_queue empty, so we need to wait quene empty.
        mcDELAY_US(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_REFRESH_QUEUE_CNT)*4);   //wait refresh_queue_cnt * 3.9us
    }
}

void DramcDebugTest(DRAMC_CTX_T *p)
{
#if 0
    U32 u4ErrorValue;

    // TestEngine2 R/W test
    p->test2_1 = DEFAULT_TEST2_1_CAL;
    u4ErrorValue = TestEngineCompare(p);
    mcSHOW_ERR_MSG(("[DramcDebugTest] First R/W test . Rank0: u4ErrorValue 0x%X\n", u4ErrorValue));
    #if DUAL_RANK_ENABLE
    if (p->support_rank_num==RANK_DUAL)
    {
        p->test2_1 = 0x55a00000;
        u4ErrorValue = TestEngineCompare(p);
        mcSHOW_ERR_MSG(("[DramcDebugTest] First R/W test . Rank1: u4ErrorValue 0x%X\n", u4ErrorValue));
    }
    #endif

    // Reset PHY
    DramPhyReset(p);

    // TestEngine2 R/W test
    p->test2_1 = DEFAULT_TEST2_1_CAL;
    u4ErrorValue = TestEngineCompare(p);
    mcSHOW_ERR_MSG(("[DramcDebugTest] Reset PHY R/W test. Rank0: u4ErrorValue 0x%X\n", u4ErrorValue));

    #if DUAL_RANK_ENABLE
    if (p->support_rank_num==RANK_DUAL)
    {
        p->test2_1 = 0x55a00000;
        u4ErrorValue = TestEngineCompare(p);
        mcSHOW_ERR_MSG(("[DramcDebugTest] Reset PHY R/W test . Rank1:u4ErrorValue 0x%X\n", u4ErrorValue));
    }
    #endif

    // Reset Dram
    Dram_Reset(p);

    // TestEngine2 R/W test
    p->test2_1 = DEFAULT_TEST2_1_CAL;
    u4ErrorValue = TestEngineCompare(p);
    mcSHOW_ERR_MSG(("[DramcDebugTest] Reset Dram R/W test. Rank0: u4ErrorValue 0x%X\n", u4ErrorValue));

    #if DUAL_RANK_ENABLE
    if (p->support_rank_num==RANK_DUAL)
    {
        p->test2_1 = 0x55a00000;
        u4ErrorValue = TestEngineCompare(p);
        mcSHOW_ERR_MSG(("[DramcDebugTest] Reset Dram R/W test . Rank1:u4ErrorValue 0x%X\n", u4ErrorValue));
    }
    #endif

    p->test2_1 = DEFAULT_TEST2_1_CAL;
    #endif
}


void TransferToSPMControl(DRAMC_CTX_T *p)
{
    U32 u4WbrBackup = 0;

    vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL0, 0xfffffe7f, MISC_SPM_CTRL0_PHY_SPM_CTL0);//Lewis@20160505: PLL control is by SW RG no matter Low power
    vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL2, 0xffffffff, MISC_SPM_CTRL2_PHY_SPM_CTL2);

    return;
}

void TransferPLLToSPMControl(DRAMC_CTX_T *p)
{
    /*TINFO="DRAM : enter SW DVFS"*/

    /*TINFO="DRAM : SPM presetting for pinmux"*/
    //! set SPM project code and enable clock enable
    *((volatile unsigned int *)(SPM_POWERON_CONFIG_EN)) = 0xB16 << 16 | 0x1;

    //! set SPM pinmux
    *((volatile unsigned int *)(SPM_PCM_PWR_IO_EN))     = 0;

    #if DUAL_FREQ_K
    if(gu1PhyPLLEn)
    {
        *((volatile unsigned int *)(SPM_SW_RSV_5)) |= (0x1 << 0);    }
    else
    {
        *((volatile unsigned int *)(SPM_SW_RSV_5)) &= ~(0x1 << 0);
    }
    #endif

    *((volatile unsigned int *)(SPM_DRAMC_DPY_CLK_SW_CON_SEL))  = 0xffffffff;
    *((volatile unsigned int *)(SPM_DRAMC_DPY_CLK_SW_CON_SEL2)) = 0xffffffff;
    *((volatile unsigned int *)(SPM_DRAMC_DPY_CLK_SW_CON_SEL3)) = 0xffffffff;
    *((volatile unsigned int *)(SPM_DRAMC_DPY_CLK_SW_CON_SEL4)) = 0xffffffff;

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    U8 shu_level ;
    shu_level = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHUSTATUS), SHUSTATUS_SHUFFLE_LEVEL);
    if(shu_level==1)
    {
        mcSHOW_DBG_MSG(("TransferPLLToSPMControl - CLRPLL MODE SW\n"));
        *((volatile unsigned int *)(SPM_POWER_ON_VAL0)) &= ~(0x1<<28);
        *((volatile unsigned int *)(SPM_POWER_ON_VAL0)) |= (0x1<<29);
        *((volatile unsigned int *)(SPM_DRAMC_DPY_CLK_SW_CON3)) |= (0x1<<3);
    }
    else
    {
        mcSHOW_DBG_MSG(("TransferPLLToSPMControl - PHYPLL MODE SW\n"));
        *((volatile unsigned int *)(SPM_POWER_ON_VAL0)) &= ~(0x1<<29);
        *((volatile unsigned int *)(SPM_POWER_ON_VAL0)) |= (0x1<<28);
        *((volatile unsigned int *)(SPM_DRAMC_DPY_CLK_SW_CON3)) |= (0x1<<2);
    }
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_PLL1, 0x0, PLL1_RG_RPHYPLL_EN);
    vIO32WriteFldAlign_All(DDRPHY_PLL2, 0x0, PLL2_RG_RCLRPLL_EN);
#endif
    return;
}


void TransferToRegControl(void)
{
    /* Chen-Hsiang@20160323: After leave preloader and low power scenario, conf is controller by RG*/
}

void Switch26MHzDisableDummyReadRefreshAllBank(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_CLK_MEM_SEL);//Switch clk to 26MHz
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);

    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL0, 0, REFCTRL0_PBREFEN);//Switch to all bank refresh

    vIO32WriteFldMulti_All(DRAMC_REG_DUMMY_RD, P_Fld(0x0, DUMMY_RD_DQSG_DMYWR_EN)//Disable Dummy Read
                | P_Fld(0x0, DUMMY_RD_DQSG_DMYRD_EN) | P_Fld(0x0, DUMMY_RD_SREF_DMYRD_EN)
                | P_Fld(0x0, DUMMY_RD_DUMMY_RD_EN) | P_Fld(0x0, DUMMY_RD_DMY_RD_DBG)
                | P_Fld(0x0, DUMMY_RD_DMY_WR_DBG));
    return;
}

#if (SW_CHANGE_FOR_SIMULATION==0)
void EnableDramcPhyDCM(DRAMC_CTX_T *p, BOOL bEn)
{
    U32 u4WbrBackup = 0;

    if(bEn)
    {
        vIO32WriteFldMulti_All( DRAMC_REG_DRAMC_PD_CTRL, P_Fld(0x1, DRAMC_PD_CTRL_DCMENNOTRFC)
                | P_Fld(0x1, DRAMC_PD_CTRL_COMBCLKCTRL)
                | P_Fld(0x1, DRAMC_PD_CTRL_PHYCLKDYNGEN)
                | P_Fld(0x1, DRAMC_PD_CTRL_DCMEN) | P_Fld(0x0, DRAMC_PD_CTRL_COMBPHY_CLKENSAME)
                | P_Fld(0x1, DRAMC_PD_CTRL_DCMEN2));

        vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL0, P_Fld(0x0, MISC_CG_CTRL0_RG_CG_INFRA_OFF_DISABLE)
                | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_COMB1_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_COMB0_OFF_DISABLE)
                | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_CMD_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_COMB1_OFF_DISABLE)
                | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_COMB0_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE)
                | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_COMB_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE)
                | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_DRAMC_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_EMI_OFF_DISABLE));
        //CHANNEL_EMI free run for MD latency issue

        //mem_dcm
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x83e003BE, MISC_CG_CTRL2_RG_MEM_DCM_CTL);
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x83e003BF, MISC_CG_CTRL2_RG_MEM_DCM_CTL);
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x83e003BE, MISC_CG_CTRL2_RG_MEM_DCM_CTL);
    }
    else
    {
        vIO32WriteFldMulti_All( DRAMC_REG_DRAMC_PD_CTRL, P_Fld(0x0, DRAMC_PD_CTRL_DCMENNOTRFC)
                | P_Fld(0x0, DRAMC_PD_CTRL_COMBCLKCTRL)
                | P_Fld(0x0, DRAMC_PD_CTRL_PHYCLKDYNGEN)
                | P_Fld(0x0, DRAMC_PD_CTRL_DCMEN) | P_Fld(0x1, DRAMC_PD_CTRL_COMBPHY_CLKENSAME)
                | P_Fld(0x0, DRAMC_PD_CTRL_DCMEN2));

        vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL0, P_Fld(0x1, MISC_CG_CTRL0_RG_CG_INFRA_OFF_DISABLE)
                | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_RX_COMB1_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_RX_COMB0_OFF_DISABLE)
                | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_RX_CMD_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_COMB1_OFF_DISABLE)
                | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_COMB0_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE)
                | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_COMB_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE)
                | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_DRAMC_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_EMI_OFF_DISABLE));
        //CHANNEL_EMI free run for MD latency issue

        //mem_dcm
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x83e0037E, MISC_CG_CTRL2_RG_MEM_DCM_CTL);
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x83e0037F, MISC_CG_CTRL2_RG_MEM_DCM_CTL);
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x83e0037E, MISC_CG_CTRL2_RG_MEM_DCM_CTL);
    }
    return;
}
#endif

void DDRPhyLowPowerEnable(DRAMC_CTX_T *p)//Wait for Li-Ken
{
    vIO32Write4B_All(DDRPHY_B0_DLL_ARPI2, 0x0);
    vIO32Write4B_All(DDRPHY_B0_DLL_ARPI3, 0x2E800);
    vIO32Write4B_All(DDRPHY_B1_DLL_ARPI2, 0x0);
    vIO32Write4B_All(DDRPHY_B1_DLL_ARPI3, 0x2E800);
    vIO32Write4B_All(DDRPHY_CA_DLL_ARPI2, 0x800);
    vIO32Write4B_All(DDRPHY_CA_DLL_ARPI3, 0xBA000);

    return;
}

void DummyReadForDqsGatingRetry(DRAMC_CTX_T *p, bool bEn)
{
    if(bEn == 1)
    {
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_DQSG_RETRY, 0, SHU_DQSG_RETRY_R_RETRY_ROUND_NUM);//Retry once
        vIO32WriteFldAlign_All(DRAMC_REG_TEST2_4, 4, TEST2_4_TESTAGENTRKSEL);//Dummy Read rank selection is controlled by Test Agent
        vIO32WriteFldMulti_All(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(1, SHU_DQSG_RETRY_R_XSR_RETRY_SPM_MODE)
                                         | P_Fld(0, SHU_DQSG_RETRY_R_XSR_DQSG_RETRY_EN)
                                         | P_Fld(0, SHU_DQSG_RETRY_R_DQSG_RETRY_SW_EN));
        vIO32WriteFldMulti_All(DRAMC_REG_DUMMY_RD, P_Fld(1, DUMMY_RD_DQSG_DMYRD_EN)
                                         | P_Fld(p->support_rank_num, DUMMY_RD_RANK_NUM)
                                         | P_Fld(1, DUMMY_RD_DUMMY_RD_SW));
        vIO32WriteFldAlign_All(DRAMC_REG_RK1_DUMMY_RD_ADR, 0, RK1_DUMMY_RD_ADR_DMY_RD_RK1_LEN);
        vIO32WriteFldAlign_All(DRAMC_REG_RK0_DUMMY_RD_ADR, 0, RK0_DUMMY_RD_ADR_DMY_RD_RK0_LEN);
        vIO32WriteFldMulti_All(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(1, SHU_DQSG_RETRY_R_RETRY_USE_BURST_MDOE)
                                         | P_Fld(1, SHU_DQSG_RETRY_R_RDY_SEL_DLE)
                                         | P_Fld(1, SHU_DQSG_RETRY_R_DDR1866_PLUS));
    }
    else
    {
        vIO32WriteFldMulti_All(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0, SHU_DQSG_RETRY_R_XSR_RETRY_SPM_MODE)
                                         | P_Fld(0, SHU_DQSG_RETRY_R_XSR_DQSG_RETRY_EN)
                                         | P_Fld(0, SHU_DQSG_RETRY_R_DQSG_RETRY_SW_EN));
    }
}

void HwSaveForSR(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti_All(DRAMC_REG_RSTMASK, P_Fld(0, RSTMASK_GT_SYNC_MASK)
                | P_Fld(0, RSTMASK_GT_SYNC_MASK_FOR_PHY));
    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL1, 1, REFCTRL1_SLEFREF_AUTOSAVE_EN);
    vIO32WriteFldMulti_All(DRAMC_REG_SREFCTRL, P_Fld(1, SREFCTRL_SREF2_OPTION)
                | P_Fld(0, SREFCTRL_SREF3_OPTION));
}

#if SET_CKE_2_RANK_INDEPENDENT
void SetCKE2RankIndependent(DRAMC_CTX_T *p)
{
    mcSHOW_DBG_MSG(("=== [SetCKE2RankIndependent] ===\n"));
    vIO32WriteFldMulti_All(DRAMC_REG_RKCFG, P_Fld(0, RKCFG_CKE2RANK)
                    | P_Fld(0, RKCFG_CKE2RANK_OPT)
                    | P_Fld(1, RKCFG_CKE2RANK_OPT2));

    vIO32WriteFldMulti_All(DRAMC_REG_CKECTRL, P_Fld(0, CKECTRL_CKE2RANK_OPT3)
                    | P_Fld(0, CKECTRL_CKE2RANK_OPT5)
                    | P_Fld(0, CKECTRL_CKE2RANK_OPT6)
                    | P_Fld(1, CKECTRL_CKE2RANK_OPT7)
                    | P_Fld(1, CKECTRL_CKE2RANK_OPT8)//Whiteny is different from KIBO+
                    | P_Fld(0, CKECTRL_CKETIMER_SEL)
                    | P_Fld(1, CKECTRL_FASTWAKE_SEL)
                    | P_Fld(0, CKECTRL_CKEWAKE_SEL));

    vIO32WriteFldMulti_All(DRAMC_REG_SHU_CONF2, P_Fld(1,SHU_CONF2_FASTWAKE) | P_Fld(1,SHU_CONF2_FASTWAKE2));
    vIO32WriteFldMulti_All(DRAMC_REG_SHU2_CONF2, P_Fld(1,SHU_CONF2_FASTWAKE) | P_Fld(1,SHU_CONF2_FASTWAKE2));
    vIO32WriteFldMulti_All(DRAMC_REG_SHU3_CONF2, P_Fld(1,SHU_CONF2_FASTWAKE) | P_Fld(1,SHU_CONF2_FASTWAKE2));
    vIO32WriteFldMulti_All(DRAMC_REG_SHU4_CONF2, P_Fld(1,SHU_CONF2_FASTWAKE) | P_Fld(1,SHU_CONF2_FASTWAKE2));

    vIO32WriteFldAlign_All(DRAMC_REG_DRAMCTRL, 0,DRAMCTRL_CLKWITRFC);
}
#endif

#ifdef CLK_FREE_FUN_FOR_DRAMC_PSEL
//If dramc enter SREF and power down, all configure need to sync 2T again after exit SREF.
//If Psel is 1, clock will be free run at the periof of 2T to let conf be applied. 
//If Psel is 0, Clock will be gated
void ClkFreeRunForDramcPsel(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti_All(DRAMC_REG_REFCTRL1, P_Fld(0, REFCTRL1_PSEL_OPT1)
            | P_Fld(0, REFCTRL1_PSEL_OPT2)
            | P_Fld(0, REFCTRL1_PSEL_OPT3));
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_CLKAR), 0, CLKAR_PSELAR);
}
#endif

#ifdef PA_IMPROVEMENT_FOR_DRAMC_ACTIVE_POWER
void DramcPAImprove(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti_All(DRAMC_REG_CLKAR, P_Fld(0, CLKAR_REQQUE_PACG_DIS)
            | P_Fld(0, CLKAR_SELPH_CMD_CG_DIS));
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), 0, DUMMY_RD_DUMMY_RD_PA_OPT);
    vIO32WriteFldMulti_All(DRAMC_REG_SREFCTRL, P_Fld(0, SREFCTRL_SCSM_CGAR)
            | P_Fld(0, SREFCTRL_SCARB_SM_CGAR)
            | P_Fld(0, SREFCTRL_RDDQSOSC_CGAR)
            | P_Fld(0, SREFCTRL_HMRRSEL_CGAR));
}
#endif

void DramcRunTimeConfig(DRAMC_CTX_T *p)
{
    mcSHOW_DBG_MSG(("=== [DramcRunTimeConfig] ===\n"));

    if(p->frequency>=1600)
        vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SPM_CON, 1, DRAMC_DPY_CLK_SPM_CON_PCM_DPY_RX_TRACKING_EN);

    //#ifdef SPM_CONTROL_AFTERK
    TransferPLLToSPMControl(p);
    //#endif

#if ENABLE_TX_TRACKING
    if(u1IsLP4Family(p->dram_type))
    {
        U8 backup_channel;
        U8 channelIdx = CHANNEL_A;
        backup_channel = p->channel;
        for(channelIdx=CHANNEL_A; channelIdx<CHANNEL_NUMBER; channelIdx++)
        {
            vSetPHY2ChannelMapping(p, channelIdx);
            //DramcDFSUpdateFreqRatio(p);
            DramcHwDQSOSC(p);
        }
        
        vSetPHY2ChannelMapping(p, backup_channel);
        mcSHOW_DBG_MSG(("ENABLE_TX_TRACKING: ON\n"));
    }
    else
    mcSHOW_DBG_MSG(("ENABLE_TX_TRACKING: OFF\n"));
#else
    mcSHOW_DBG_MSG(("ENABLE_TX_TRACKING: OFF\n"));
#endif

#if ENABLE_RX_TRACKING_LP4
    if(u1IsLP4Family(p->dram_type))
    {
        DramcRxInputDelayTrackingInit_Common(p);
        DramcRxInputDelayTrackingHW(p);
        mcSHOW_DBG_MSG(("ENABLE_RX_TRACKING_LP4: ON\n"));
    }
#else
    mcSHOW_DBG_MSG(("ENABLE_RX_TRACKING_LP4: OFF\n"));
#endif

#ifdef HW_GATING
    if(p->dram_type == TYPE_LPDDR3|| u1IsLP4Family(p->dram_type))
    {
        DramcHWGatingInit(p);                                   // HW gating initial before RunTime config.
        DramcHWGatingOnOff((DRAMC_CTX_T *) p, 1); // Enable HW gating tracking
        mcSHOW_DBG_MSG(("HW_GATING: ON\n"));
    }
    else
    {
        DramcHWGatingOnOff((DRAMC_CTX_T *) p, 0);
        mcSHOW_DBG_MSG(("HW_GATING: OFF\n"));
    }
#else
        DramcHWGatingOnOff((DRAMC_CTX_T *) p, 0);
        mcSHOW_DBG_MSG(("HW_GATING: OFF\n"));
#endif

#ifdef DUMMY_READ_FOR_TRACKING
    if (u1IsLP4Family(p->dram_type))
    {
//WHITNEY_TO_BE_PORTING
#if (fcFOR_CHIP_ID == fcWhitney)
        if(p->freq_sel == LP4_DDR3200)
        {
            DramcDummyReadForTrackingEnable(p);
            mcSHOW_DBG_MSG(("DUMMY_READ_FOR_TRACKING: ON\n"));
        }
        else
        {
            mcSHOW_DBG_MSG(("DUMMY_READ_FOR_TRACKING: OFF\n"));
        }
#endif
    }
    else
    {
        mcSHOW_DBG_MSG(("DUMMY_READ_FOR_TRACKING: OFF\n"));
    }
#else
    mcSHOW_DBG_MSG(("DUMMY_READ_FOR_TRACKING: OFF\n"));
#endif

//HW sync gating tracking
#if DFS_HW_SYNC_GATING_TRACKING
    vIO32WriteFldAlign_All(DRAMC_REG_RSTMASK, 0, RSTMASK_DVFS_SYNC_MASK);
    mcSHOW_DBG_MSG(("DFS_HW_SYNC_GATING_TRACKING: ON\n"));
#else
    mcSHOW_DBG_MSG(("DFS_HW_SYNC_GATING_TRACKING: OFF\n"));
#endif
    if(u1IsLP4Family(p->dram_type))
    {
#ifdef ZQCS_ENABLE_LP4
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 1, SPCMDCTRL_ZQCALDISB);// LP3 and LP4 are different, be careful.
        mcSHOW_DBG_MSG(("ZQCS_ENABLE_LP4: ON\n"));
#else
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 0, SPCMDCTRL_ZQCALDISB);// LP3 and LP4 are different, be careful.
        mcSHOW_DBG_MSG(("ZQCS_ENABLE_LP4: OFF\n"));
#endif
    }
    else
    {
#ifdef ZQCS_ENABLE_LP3
        vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 1, SPCMDCTRL_ZQCSDISB);
        mcSHOW_DBG_MSG(("ZQCS_ENABLE_LP3: ON\n"));
#else
        vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 0, SPCMDCTRL_ZQCSDISB);
        mcSHOW_DBG_MSG(("ZQCS_ENABLE_LP3: OFF\n"));
#endif
    }
#if APPLY_LOWPOWER_GOLDEN_SETTINGS
    DDRPhyLowPowerEnable(p);
#if (SW_CHANGE_FOR_SIMULATION==0)
    EnableDramcPhyDCM(p, 1);
#endif
    mcSHOW_DBG_MSG(("LOWPOWER_GOLDEN_SETTINGS(DCM): ON\n"));
#else
#if (SW_CHANGE_FOR_SIMULATION==0)
    EnableDramcPhyDCM(p, 0);
#endif
    mcSHOW_DBG_MSG(("LOWPOWER_GOLDEN_SETTINGS(DCM): OFF\n"));
#endif

#ifdef DUMMY_READ_FOR_DQS_GATING_RETRY
    if(u1IsLP4Family(p->dram_type))
    {
        DummyReadForDqsGatingRetry(p, 1);
        mcSHOW_DBG_MSG(("DUMMY_READ_FOR_DQS_GATING_RETRY: ON\n"));
    }
    else
    {
        DummyReadForDqsGatingRetry(p, 0);
        mcSHOW_DBG_MSG(("DUMMY_READ_FOR_DQS_GATING_RETRY: OFF\n"));
    }
#else
    mcSHOW_DBG_MSG(("DUMMY_READ_FOR_DQS_GATING_RETRY: OFF\n"));
#endif

#ifdef SPM_CONTROL_AFTERK
      TransferToSPMControl(p);  //don't enable in ETT
      mcSHOW_DBG_MSG(("SPM_CONTROL_AFTERK: ON\n"));
  #else
      mcSHOW_DBG_MSG(("SPM_CONTROL_AFTERK: OFF\n"));
#endif

#ifdef IMPEDENCE_TRACKING_ENABLE
    if(p->dram_type == TYPE_LPDDR4 || p->dram_type == TYPE_LPDDR4X)
    {
        DramcImpedenceTrackingEnable(p);
        mcSHOW_DBG_MSG(("IMPENDENCE_TRACKING: ON\n"));

#ifdef IMPEDENCE_HW_SAVING
        DramcImpedenceHWSaving(p);
#endif
    }
#else
    mcSHOW_DBG_MSG(("IMPENDENCE_TRACKING: OFF\n"));
#endif

    // 0x1c0[31]
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSCAL0), 0, DQSCAL0_STBCALEN);

#ifdef TEMP_SENSOR_ENABLE
    // enable MR4 refresh rate reference, interval = 0x10
    //vIO32WriteFldAlign_All(DRAMC_REG_SHU_CONF3, 0x10, SHU_CONF3_REFRCNT);
    if(u1IsLP4Family(p->dram_type))
    {
        vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 0, SPCMDCTRL_REFRDIS);
    }
    else
    {
        //lp3 only has CHA
        vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0, SPCMDCTRL_REFRDIS);
    }
    mcSHOW_DBG_MSG(("TEMP_SENSOR_ENABLE: ON\n"));
#else
    vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 1, SPCMDCTRL_REFRDIS);
    mcSHOW_DBG_MSG(("TEMP_SENSOR_ENABLE: OFF\n"));
#endif

#if ENABLE_PER_BANK_REFRESH
    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL0, 1, REFCTRL0_PBREFEN);
    mcSHOW_DBG_MSG(("ENABLE_PER_BANK_REFRESH: ON\n"));
#else
    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL0, 0, REFCTRL0_PBREFEN);
    mcSHOW_DBG_MSG(("ENABLE_PER_BANK_REFRESH: OFF\n"));
#endif
    //Lewis@20160506:[DDR reserve mode] LPDDR3 Let CHB enter SR, let SR ACK always 1 to avoid WDT conf could not enter SR
    if (p->dram_type == TYPE_LPDDR3)
    {
        DRAM_CHANNEL_T backChannel = p->channel;
        p->channel = CHANNEL_B;
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SREFCTRL), 1, SREFCTRL_SELFREF);//Let CHB enter SR
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 1, RKCFG_RK0DPD);//Set to let CHB SREF_STATE always 1, fix(a)LP3 WDT TO to let DRAM into SR fail at second time (b) LP3 could not enter SR in S0
        p->channel = backChannel;
    }

#ifdef HW_SAVE_FOR_SR
    HwSaveForSR(p);
    mcSHOW_DBG_MSG(("HW_SAVE_FOR_SR: ON\n"));
#else
    mcSHOW_DBG_MSG(("HW_SAVE_FOR_SR: OFF\n"));
#endif

#ifdef CLK_FREE_FUN_FOR_DRAMC_PSEL
    ClkFreeRunForDramcPsel(p);
    mcSHOW_DBG_MSG(("CLK_FREE_FUN_FOR_DRAMC_PSEL: ON\n"));
#else
    mcSHOW_DBG_MSG(("CLK_FREE_FUN_FOR_DRAMC_PSEL: OFF\n"));
#endif

#ifdef PA_IMPROVEMENT_FOR_DRAMC_ACTIVE_POWER
    DramcPAImprove(p);
    mcSHOW_DBG_MSG(("PA_IMPROVEMENT_FOR_DRAMC_ACTIVE_POWER: ON\n"));
#else
    mcSHOW_DBG_MSG(("PA_IMPROVEMENT_FOR_DRAMC_ACTIVE_POWER: OFF\n"));
#endif

#if ENABLE_RODT_TRACKING
    mcSHOW_DBG_MSG(("Read ODT Tracking: ON\n"));
#else
    mcSHOW_DBG_MSG(("Read ODT Tracking: OFF\n"));
#endif

#ifdef TDQSCK_PRECALCULATION_FOR_DVFS
        DramcDQSPrecalculation_preset(p);
        mcSHOW_DBG_MSG(("DQS Precalculation for DVFS: ON\n"));
#else
        mcSHOW_DBG_MSG(("DQS Precalculation for DVFS: OFF\n"));
#endif

#if APHY_DDR800_FB_MCK_CG_ON
    vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD0, P_Fld(0x0, SHU1_CA_CMD0_RG_DLL_CG_EN_CA) //DDR3200
        | P_Fld(0x0, SHU1_CA_CMD0_RG_DLL_CG_EN_B0)
        | P_Fld(0x0, SHU1_CA_CMD0_RG_DLL_CG_EN_B1));

    vIO32WriteFldMulti_All(DDRPHY_SHU2_CA_CMD0, P_Fld(0x0, SHU2_CA_CMD0_RG_DLL_CG_EN_CA) //DDR2667
        | P_Fld(0x0, SHU2_CA_CMD0_RG_DLL_CG_EN_B0)
        | P_Fld(0x0, SHU2_CA_CMD0_RG_DLL_CG_EN_B1));

    vIO32WriteFldMulti_All(DDRPHY_SHU3_CA_CMD0, P_Fld(0x0, SHU3_CA_CMD0_RG_DLL_CG_EN_CA) //DDR2667
        | P_Fld(0x0, SHU3_CA_CMD0_RG_DLL_CG_EN_B0)
        | P_Fld(0x0, SHU3_CA_CMD0_RG_DLL_CG_EN_B1));

    vIO32WriteFldMulti_All(DDRPHY_SHU4_CA_CMD0, P_Fld(0x0, SHU4_CA_CMD0_RG_DLL_CG_EN_CA) //DDR800
        | P_Fld(0x1, SHU4_CA_CMD0_RG_DLL_CG_EN_B0)
        | P_Fld(0x1, SHU4_CA_CMD0_RG_DLL_CG_EN_B1));
    mcSHOW_DBG_MSG(("E2 DDR800 FB MCK CG: ON\n"));
#else
    mcSHOW_DBG_MSG(("E2 DDR800 FB MCK CG: OFF\n"));
#endif
    mcSHOW_DBG_MSG(("=========================\n"));
}


void DramcSetRankEngine2(DRAMC_CTX_T *p, U8 u1RankSel)
{
    //LPDDR2_3_ADRDECEN_TARKMODE =0, always rank0
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMCTRL), 1, DRAMCTRL_ADRDECEN_TARKMODE);

    // DUMMY_TESTAGENTRKSEL =0, select rank according to CATRAIN_TESTAGENTRK
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), 0, TEST2_4_TESTAGENTRKSEL);

    //CATRAIN_TESTAGENTRK = u1RankSel
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), u1RankSel, TEST2_4_TESTAGENTRK);
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
static U32 uiReg0D0h;
DRAM_STATUS_T DramcEngine2Init(DRAMC_CTX_T *p, U32 test2_1, U32 test2_2, U8 testaudpat, U8 log2loopcount)
{
    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context is NULL\n"));
        return DRAM_FAIL;
    }

    // check loop number validness
//    if ((log2loopcount > 15) || (log2loopcount < 0))		// U8 >=0 always.
    if (log2loopcount > 15)
    {
        mcSHOW_ERR_MSG(("wrong parameter log2loopcount:    log2loopcount just 0 to 15 !\n"));
        return DRAM_FAIL;
    }

    DramcSetRankEngine2(p, p->rank);

    uiReg0D0h=u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), P_Fld(0, DUMMY_RD_DQSG_DMYRD_EN) | P_Fld(0, DUMMY_RD_DQSG_DMYWR_EN) | P_Fld(0, DUMMY_RD_DUMMY_RD_EN) | P_Fld(0, DUMMY_RD_SREF_DMYRD_EN) | P_Fld(0, DUMMY_RD_DMY_RD_DBG) | P_Fld(0, DUMMY_RD_DMY_WR_DBG));  //must close dummy read when do test agent

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTCHIP_DMA1), 0, TESTCHIP_DMA1_DMA_LP4MATAB_OPT);

    // disable self test engine1 and self test engine2
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TEST2W) | P_Fld(0, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));


    // 1.set pattern ,base address ,offset address
    // 2.select  ISI pattern or audio pattern or xtalk pattern
    // 3.set loop number
    // 4.enable read or write
    // 5.loop to check DM_CMP_CPT
    // 6.return CMP_ERR
    // currently only implement ucengine_status = 1, others are left for future extension

    // 1
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_0), P_Fld((test2_1>>24)&0xff,TEST2_0_TEST2_PAT0)|P_Fld((test2_2>>24)&0xff,TEST2_0_TEST2_PAT1));
#if (FOR_DV_SIMULATION_USED==1 || SW_CHANGE_FOR_SIMULATION==1)
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_TEST2_1), ((test2_1+0x100)<<4)&0x00ffffff);    //DV sim memory 0~0x100 has values, can't used
#else
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_TEST2_1), (test2_1<<4)&0x00ffffff);
#endif
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_TEST2_2), (test2_2<<4)&0x00ffffff);

    // 2 & 3
    // (TESTXTALKPAT, TESTAUDPAT) = 00 (ISI), 01 (AUD), 10 (XTALK), 11 (UNKNOWN)
    if (testaudpat == 2)   // xtalk
    {
        //TEST_REQ_LEN1=1 is new feature, hope to make dq bus continously.
        //but DV simulation will got problem of compare err
        //so go back to use old way
        //TEST_REQ_LEN1=0, R_DMRWOFOEN=1
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(0, TEST2_4_TEST_REQ_LEN1));   //test agent 2 with cmd length = 0
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0), P_Fld(1, PERFCTL0_RWOFOEN));

        // select XTALK pattern
        // set addr 0x044 [7] to 0
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TESTAUDPAT)|P_Fld(log2loopcount,TEST2_3_TESTCNT)); //dont use audio pattern

        // set addr 0x48[16] to 1, TESTXTALKPAT = 1
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(1, TEST2_4_TESTXTALKPAT)|P_Fld(0,TEST2_4_TESTAUDMODE)|P_Fld(0,TEST2_4_TESTAUDBITINV));  //use XTALK pattern, dont use audio pattern

        // R_DMTESTSSOPAT=0, R_DMTESTSSOXTALKPAT=0
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(0, TEST2_4_TESTSSOPAT)|P_Fld(0,TEST2_4_TESTSSOXTALKPAT));   //dont use sso, sso+xtalk pattern
    }
    else if (testaudpat == 1)   // audio
    {
        // set AUDINIT=0x11 AUDINC=0x0d AUDBITINV=1 AUDMODE=1
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), \
            P_Fld(0x00000011, TEST2_4_TESTAUDINIT)|P_Fld(0x0000000d, TEST2_4_TESTAUDINC)| \
            P_Fld(0, TEST2_4_TESTXTALKPAT)|P_Fld(1,TEST2_4_TESTAUDMODE)|P_Fld(1,TEST2_4_TESTAUDBITINV));

        // set addr 0x044 [7] to 1 ,select audio pattern
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(1, TEST2_3_TESTAUDPAT)|P_Fld(log2loopcount,TEST2_3_TESTCNT));
    }
    else   // ISI
    {
        // select ISI pattern
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TESTAUDPAT)|P_Fld(log2loopcount,TEST2_3_TESTCNT));
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), 0, TEST2_4_TESTXTALKPAT);
    }
    return DRAM_OK;
}


U32 DramcEngine2Run(DRAMC_CTX_T *p, DRAM_TE_OP_T wr, U8 testaudpat)
{
    U32 u4loop_count = 0;
    U32 u4result = 0xffffffff;

    // 4
    if (wr == TE_OP_READ_CHECK)
    {
        if ((testaudpat == 1) || (testaudpat == 2))
        {
            //if audio pattern, enable read only (disable write after read), AUDMODE=0x48[15]=0
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), 0, TEST2_4_TESTAUDMODE);
        }

        // enable read, 0x008[31:29]
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TEST2W) | P_Fld(1, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));
    }
    else if (wr == TE_OP_WRITE_READ_CHECK)
    {
        // enable write
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(1, TEST2_3_TEST2W) | P_Fld(0, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));

        // read data compare ready check
        u4loop_count = 0;
        while((u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), TESTRPT_DM_CMP_CPT))==0)
        {
            //ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), &u4value);
            mcDELAY_US(CMP_CPT_POLLING_PERIOD);
            u4loop_count++;
            if ((u4loop_count > 3) &&(u4loop_count <= MAX_CMP_CPT_WAIT_LOOP))
            {
                //mcSHOW_ERR_MSG(("TESTRPT_DM_CMP_CPT (Write): %d\n", u4loop_count));
            }
            else if(u4loop_count > MAX_CMP_CPT_WAIT_LOOP)
            {
               /*TINFO="fcWAVEFORM_MEASURE_A %d: time out\n", u4loop_count*/
               mcSHOW_DBG_MSG(("fcWAVEFORM_MEASURE_A %d :time out\n", u4loop_count));
               mcFPRINTF((fp_A60501, "fcWAVEFORM_MEASURE_A %d: time out\n", u4loop_count));
                break;
            }
        }

        // disable write
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TEST2W) | P_Fld(0, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));

        mcDELAY_US(1);

        // enable read
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TEST2W) | P_Fld(1, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));
    }

    // 5
    // read data compare ready check
    u4loop_count = 0;
    while((u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), TESTRPT_DM_CMP_CPT))==0)
    {
            mcDELAY_US(CMP_CPT_POLLING_PERIOD);
            u4loop_count++;
            if ((u4loop_count > 3)&&(u4loop_count <= MAX_CMP_CPT_WAIT_LOOP))
            {
                //mcSHOW_ERR_MSG(("TESTRPT_DM_CMP_CPT (Read): %d\n", u4loop_count));
            }
            else if(u4loop_count > MAX_CMP_CPT_WAIT_LOOP)
            {
               /*TINFO="fcWAVEFORM_MEASURE_B %d: time out\n", u4loop_count*/
               mcSHOW_DBG_MSG(("fcWAVEFORM_MEASURE_B %d: time out\n", u4loop_count));
               mcFPRINTF((fp_A60501, "fcWAVEFORM_MEASURE_B %d: time out\n", u4loop_count));
               break;
            }
    }

    // delay 10ns after ready check from DE suggestion (1ms here)
    mcDELAY_US(1);

    // 6
    // return CMP_ERR, 0 is ok ,others are fail,diable test2w or test2r
    // get result
    // or all result
    u4result = (u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CMP_ERR)));
    // disable read
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TEST2W) | P_Fld(0, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));

    return u4result;
}

void DramcEngine2End(DRAMC_CTX_T *p)
{
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), uiReg0D0h);
}

// Full set of usage test engine 2, including of DramcEngine2Init->DramcEngine2Run->DramcEngine2End
// if you don't care the performance, and just for convinent, you may use this API (TestEngineCompare)
U32 TestEngineCompare(DRAMC_CTX_T *p)
{
    U8 jj;
    U32 u4err_value;

    if(p->test_pattern <= TEST_XTALK_PATTERN)
    {
        DramcEngine2Init(p, p->test2_1, p->test2_2, p->test_pattern, 0);
        u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, p->test_pattern);
        DramcEngine2End(p);
    }
    else if (p->test_pattern == TEST_MIX_PATTERN)
    {
        DramcEngine2Init(p, p->test2_1, p->test2_2, TEST_AUDIO_PATTERN, 0);
        u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_AUDIO_PATTERN);
        DramcEngine2End(p);

        DramcEngine2Init(p, p->test2_1, p->test2_2, TEST_XTALK_PATTERN, 0);
        u4err_value |= DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_XTALK_PATTERN);
        DramcEngine2End(p);
    }
    else
    {
        mcSHOW_ERR_MSG(("Not support test pattern!! Use xtalk pattern by default.\n"));
        DramcEngine2Init(p, p->test2_1, p->test2_2, TEST_XTALK_PATTERN, 0);
        u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_XTALK_PATTERN);
        DramcEngine2End(p);
    }
    return u4err_value;
}


#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#if __ETT__
//-------------------------------------------------------------------------
/** DramcRegDump
 *  Dump all registers (DDRPHY and DRAMC)
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL
 */
//-------------------------------------------------------------------------
void DramcRegDumpRange(U32 u4StartAddr, U32 u4EndAddr)
{
    U32 ii;

    for(ii=u4StartAddr; ii<u4EndAddr; ii+=4)
    {
        mcSHOW_DBG_MSG(("Reg(0x%xh) Address 0x%X = 0x%X\n", (ii&0xfff)>>2, ii, (*(volatile unsigned int *)(ii))));
    }
}

DRAM_STATUS_T DramcRegDump(DRAMC_CTX_T *p)
{
    mcSHOW_DBG_MSG(("\n=================CHA_DRAMC_NAO_BASE================================\n"));
    DramcRegDumpRange(Channel_A_DRAMC_NAO_BASE_ADDRESS, Channel_A_DRAMC_NAO_BASE_ADDRESS+0x880);
    mcSHOW_DBG_MSG(("\n=================CHB_DRAMC_NAO_BASE================================\n"));
    DramcRegDumpRange(Channel_B_DRAMC_NAO_BASE_ADDRESS, Channel_B_DRAMC_NAO_BASE_ADDRESS+0x880);
    mcSHOW_DBG_MSG(("\n=================CHC_DRAMC_NAO_BASE================================\n"));
    DramcRegDumpRange(Channel_C_DRAMC_NAO_BASE_ADDRESS, Channel_C_DRAMC_NAO_BASE_ADDRESS+0x880);
    mcSHOW_DBG_MSG(("\n=================CHD_DRAMC_NAO_BASE================================\n"));
    DramcRegDumpRange(Channel_D_DRAMC_NAO_BASE_ADDRESS, Channel_D_DRAMC_NAO_BASE_ADDRESS+0x880);

    mcSHOW_DBG_MSG(("\n=================CHA_DRAMC_AO_BASE================================\n"));
    DramcRegDumpRange(Channel_A_DRAMC_AO_BASE_ADDRESS, Channel_A_DRAMC_AO_BASE_ADDRESS+0x1E58);
    mcSHOW_DBG_MSG(("\n=================CHB_DRAMC_AO_BASE================================\n"));
    DramcRegDumpRange(Channel_B_DRAMC_AO_BASE_ADDRESS, Channel_B_DRAMC_AO_BASE_ADDRESS+0x1E58);
    mcSHOW_DBG_MSG(("\n=================CHC_DRAMC_AO_BASE================================\n"));
    DramcRegDumpRange(Channel_C_DRAMC_AO_BASE_ADDRESS, Channel_C_DRAMC_AO_BASE_ADDRESS+0x1E58);
    mcSHOW_DBG_MSG(("\n=================CHD_DRAMC_AO_BASE================================\n"));
    DramcRegDumpRange(Channel_D_DRAMC_AO_BASE_ADDRESS, Channel_D_DRAMC_AO_BASE_ADDRESS+0x1E58);

    mcSHOW_DBG_MSG(("\n=================PHY_A_BASE================================\n"));
    DramcRegDumpRange(Channel_A_PHY_BASE_ADDRESS, Channel_A_PHY_BASE_ADDRESS+0x1FC8);
    mcSHOW_DBG_MSG(("\n=================PHY_B_BASE================================\n"));
    DramcRegDumpRange(Channel_B_PHY_BASE_ADDRESS, Channel_B_PHY_BASE_ADDRESS+0x1FC8);
    mcSHOW_DBG_MSG(("\n=================PHY_C_BASE================================\n"));
    DramcRegDumpRange(Channel_C_PHY_BASE_ADDRESS, Channel_C_PHY_BASE_ADDRESS+0x1FC8);
    mcSHOW_DBG_MSG(("\n=================PHY_D_BASE================================\n"));
    DramcRegDumpRange(Channel_D_PHY_BASE_ADDRESS, Channel_D_PHY_BASE_ADDRESS+0x1FC8);

    return DRAM_OK;
}


#if ETT_NO_DRAM
void NoDramDramcRegDumpRange(U32 u4StartAddr, U32 u4EndAddr)
{
    U32 ii;

    for(ii=u4StartAddr; ii<u4EndAddr; ii+=4)
    {
        mcSHOW_DBG_MSG(("*(volatile unsigned int *)0x%X = 0x%X;\n", ii, (*(volatile unsigned int *)(ii))));
        //mcSHOW_DBG_MSG(("mcSHOW_DBG_MSG((\"0x%X\"));\n", ii));
    }
}

DRAM_STATUS_T NoDramDramcRegDump(DRAMC_CTX_T *p)
{
    mcSHOW_DBG_MSG(("\n//=================CHA_DRAMC_AO_BASE================================\n"));
    NoDramDramcRegDumpRange(Channel_A_DRAMC_AO_BASE_ADDRESS, Channel_A_DRAMC_AO_BASE_ADDRESS+0x1E58);
    mcSHOW_DBG_MSG(("\n//=================CHB_DRAMC_AO_BASE================================\n"));
    NoDramDramcRegDumpRange(Channel_B_DRAMC_AO_BASE_ADDRESS, Channel_B_DRAMC_AO_BASE_ADDRESS+0x1E58);
    mcSHOW_DBG_MSG(("\n//=================PHY_A_BASE================================\n"));
    NoDramDramcRegDumpRange(Channel_A_PHY_BASE_ADDRESS, Channel_A_PHY_BASE_ADDRESS+0x1FC8);
    mcSHOW_DBG_MSG(("\n//=================PHY_B_BASE================================\n"));
    NoDramDramcRegDumpRange(Channel_B_PHY_BASE_ADDRESS, Channel_B_PHY_BASE_ADDRESS+0x1FC8);

    return DRAM_OK;
}

DRAM_STATUS_T NoDramRegFill(void)
{
}
#endif
#endif
#endif




void DramcTest_DualSch_stress(DRAMC_CTX_T *p)
{
    U32 count = 0;
    U16 u2Value = 0;

    vIO32WriteFldAlign_All(DRAMC_REG_PERFCTL0, 1, PERFCTL0_DUALSCHEN);
    while(count<10)
    {
        count++;

        u1MR12Value[p->channel][p->rank][p->dram_fsp] = 0x14;
        DramcModeRegWrite(p, 12, u1MR12Value[p->channel][p->rank][p->dram_fsp]);
        DramcModeRegRead(p, 12, &u2Value);
        //mcSHOW_DBG_MSG(("MR12 = 0x%0X\n", u1Value));
    }
}


void DramcModeRegRead(DRAMC_CTX_T *p, U8 u1MRIdx, U16 *u2pValue)
{
    U32 u4MRValue;
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), p->rank, MRS_MRRRK);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1MRIdx, MRS_MRSMA);

    // MRR command will be fired when MRREN 0->1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_MRREN);

    // wait MRR command fired.
    while(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_MRR_RESPONSE) ==0)
    {
        mcDELAY_US(1);
    }

    // Read out mode register value
    u4MRValue = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRR_STATUS), MRR_STATUS_MRR_REG);
    *u2pValue = (U16)u4MRValue;

    // Set MRREN =0 for next time MRR.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_MRREN);

    mcSHOW_DBG_MSG3(("Read MR%d =0x%x\n", u1MRIdx, u4MRValue));
}


void DramcModeRegReadByRank(DRAMC_CTX_T *p, U8 u1Rank, U8 u1MRIdx, U16 *u2pValue)
{
    U16 u2Value = 0;
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1Rank, MRS_MRRRK);
    DramcModeRegRead(p,u1MRIdx, &u2Value);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), 0, MRS_MRRRK);
    *u2pValue = u2Value;
}

void DramcModeRegWrite(DRAMC_CTX_T *p, U8 u1MRIdx, U8 u1Value)
{
    U32 u4Rank = 0;
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), p->rank, MRS_MRSRK);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1MRIdx, MRS_MRSMA);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1Value, MRS_MRSOP);

    // MRW command will be fired when MRWEN 0->1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_MRWEN);

    // wait MRW command fired.
    while(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_MRW_RESPONSE) ==0)
    {
        mcSHOW_DBG_MSG2(("wait MRW command fired\n"));
        mcDELAY_US(1);
    }

    // Set MRWEN =0 for next time MRW.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_MRWEN);

    u4Rank = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), MRS_MRSRK);
    if(u1PrintModeRegWrite)
    {
        #if VENDER_JV_LOG
        mcSHOW_DBG_MSG5(("Write Rank%d MR%d =0x%x\n", u4Rank, u1MRIdx, u1Value));
        #endif
        mcSHOW_DBG_MSG2(("Write Rank%d MR%d =0x%x\n", u4Rank, u1MRIdx, u1Value));
        mcFPRINTF((fp_A60501, "Write Rank%d MR%d =0x%x\n", u4Rank, u1MRIdx, u1Value));
    }
}

void DramcModeRegWriteByRank(DRAMC_CTX_T *p, U8 u1Rank, U8 u1MRIdx, U8 u1Value)
{
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1Rank, MRS_MRSRK);
    DramcModeRegWrite(p,u1MRIdx, u1Value);
}





#define SHUFFLE_GROUP   4               //SHU1~4

#if ENABLE_TX_TRACKING
void DramcDFSUpdateFreqRatio(DRAMC_CTX_T *p)
{
    U8 u1FreqRatioTX[SHUFFLE_GROUP];
    DRAM_RANK_T rank_bak;
    int rankIdx=0;
    U32 DQ_CAL_PI[2], DQM_CAL_PI[2];
    U32 shu_level = 0;

    if(p->freq_sel == LP4_DDR3200)
    {
        u1FreqRatioTX[0] = 0x8;
        u1FreqRatioTX[1] = 0x7;
        u1FreqRatioTX[2] = 0x4;
        shu_level = 0;
    }
    else if(p->freq_sel == LP4_DDR2667)
    {
        u1FreqRatioTX[0] = 0xa;
        u1FreqRatioTX[1] = 0x8;
        u1FreqRatioTX[2] = 0x5;
        shu_level = 1;
    }
    else //if(p->freq_sel == LP4_DDR1600)
    {
        u1FreqRatioTX[0] = 0x10;
        u1FreqRatioTX[1] = 0xd;
        u1FreqRatioTX[2] = 0x8;
        shu_level = 2;
    }

    //francis todo
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQS2DQ_TX), u1FreqRatioTX[0], SHU_DQS2DQ_TX_OE2DQ_OFFSET);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2_DQS2DQ_TX), u1FreqRatioTX[1], SHU2_DQS2DQ_TX_FREQ_RATIO_TX);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3_DQS2DQ_TX), u1FreqRatioTX[2], SHU3_DQS2DQ_TX_FREQ_RATIO_TX);
    mcSHOW_DBG_MSG3(("\n[DramcDFSUpdateFreqRatio]  (3200/2667/1600) = (0x%x, 0x%x, 0x%x)\n",u1FreqRatioTX[0], u1FreqRatioTX[1], u1FreqRatioTX[2]));

    rank_bak = u1GetRank(p);
    //for(rankIdx=0; rankIdx<RANK_MAX; rankIdx)
    {
        vSetRank(p,RANK_0);
        DQ_CAL_PI[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PI_DQ_CAL), RK0_PI_DQ_CAL_RK0_ARPI_DQ_B0_CAL);
        DQ_CAL_PI[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PI_DQ_CAL), RK0_PI_DQ_CAL_RK0_ARPI_DQ_B1_CAL);
        DQM_CAL_PI[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PI_DQM_CAL), RK0_PI_DQM_CAL_RK0_ARPI_DQM_B0_CAL);
        DQM_CAL_PI[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PI_DQM_CAL), RK0_PI_DQM_CAL_RK0_ARPI_DQM_B1_CAL);
        mcSHOW_DBG_MSG3(("[DramcDFSUpdateFreqRatio]  RK0 DQ  = (0x%x, 0x%x)\n", DQ_CAL_PI[1], DQ_CAL_PI[0]));
        mcSHOW_DBG_MSG3(("[DramcDFSUpdateFreqRatio]  RK0 DQM = (0x%x, 0x%x)\n", DQM_CAL_PI[1], DQM_CAL_PI[0]));
        #if 0
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1RK0_PI)+0x600*shu_level, P_Fld(DQ_CAL_PI[0], SHU1RK0_PI_RK0_ARPI_DQ_B0)
                                                                | P_Fld(DQ_CAL_PI[1], SHU1RK0_PI_RK0_ARPI_DQ_B1)
                                                                | P_Fld(DQM_CAL_PI[0], SHU1RK0_PI_RK0_ARPI_DQM_B0)
                                                                | P_Fld(DQM_CAL_PI[1], SHU1RK0_PI_RK0_ARPI_DQM_B1));
        #endif
        DQ_CAL_PI[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK1_PI_DQ_CAL), RK1_PI_DQ_CAL_RK1_ARPI_DQ_B0_CAL);
        DQ_CAL_PI[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK1_PI_DQ_CAL), RK1_PI_DQ_CAL_RK1_ARPI_DQ_B1_CAL);
        DQM_CAL_PI[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK1_PI_DQM_CAL), RK1_PI_DQM_CAL_RK1_ARPI_DQM_B0_CAL);
        DQM_CAL_PI[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK1_PI_DQM_CAL), RK1_PI_DQM_CAL_RK1_ARPI_DQM_B1_CAL);
        mcSHOW_DBG_MSG3(("[DramcDFSUpdateFreqRatio]  RK1 DQ  = (0x%x, 0x%x)\n", DQ_CAL_PI[1], DQ_CAL_PI[0]));
        mcSHOW_DBG_MSG3(("[DramcDFSUpdateFreqRatio]  RK1 DQM = (0x%x, 0x%x)\n", DQM_CAL_PI[1], DQM_CAL_PI[0]));
        #if 0
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1RK1_PI)+0x600*shu_level, P_Fld(DQ_CAL_PI[0], SHU1RK1_PI_RK1_ARPI_DQ_B0)
                                                                | P_Fld(DQ_CAL_PI[1], SHU1RK1_PI_RK1_ARPI_DQ_B1)
                                                                | P_Fld(DQM_CAL_PI[0], SHU1RK1_PI_RK1_ARPI_DQM_B0)
                                                                | P_Fld(DQM_CAL_PI[1], SHU1RK1_PI_RK1_ARPI_DQM_B1))
        #endif
    }
    vSetRank(p, rank_bak);
}
#endif


#if DUAL_FREQ_K
#define SHUFFLE_ADDR_NUM_DRAMC          4
#define SHUFFLE_ADDR_NUM_DDRPHY         13
#define SHUFFLE_GROPU_OFFSET_DRAMC      0x600
#define SHUFFLE_GROPU_OFFSET_DDRPHY     0x500

typedef struct _SHUFFLE_REG_ADDR
{
    U32 u4StartAddr;
    U32 u4EndAddr;
} SHUFFLE_REG_ADDR;

const SHUFFLE_REG_ADDR ShuffleRegTableDramc[SHUFFLE_ADDR_NUM_DRAMC] =
{
    {DRAMC_AO_BASE_ADDRESS+(0x0800), DRAMC_AO_BASE_ADDRESS+(0x08DC) },
    {DRAMC_AO_BASE_ADDRESS+(0x0A00), DRAMC_AO_BASE_ADDRESS+(0x0A50) },
    {DRAMC_AO_BASE_ADDRESS+(0x0B00), DRAMC_AO_BASE_ADDRESS+(0x0B50) },
    {DRAMC_AO_BASE_ADDRESS+(0x0C00), DRAMC_AO_BASE_ADDRESS+(0x0C54) },
};
const SHUFFLE_REG_ADDR ShuffleRegTableDDRPhy[SHUFFLE_ADDR_NUM_DDRPHY] =
{
    {DDRPHY_BASE_ADDR+(0x0C00), DDRPHY_BASE_ADDR+(0x0C20) },
    {DDRPHY_BASE_ADDR+(0x0C80), DDRPHY_BASE_ADDR+(0x0CA0) },
    {DDRPHY_BASE_ADDR+(0x0D00), DDRPHY_BASE_ADDR+(0x0D20) },
    {DDRPHY_BASE_ADDR+(0x0D80), DDRPHY_BASE_ADDR+(0x0DB4) },//PLL
    {DDRPHY_BASE_ADDR+(0x0E00), DDRPHY_BASE_ADDR+(0x0E1C) },//R0
    {DDRPHY_BASE_ADDR+(0x0E50), DDRPHY_BASE_ADDR+(0x0E6C) },
    {DDRPHY_BASE_ADDR+(0x0EA0), DDRPHY_BASE_ADDR+(0x0EC4) },
    {DDRPHY_BASE_ADDR+(0x0F00), DDRPHY_BASE_ADDR+(0x0F1C) },//R1
    {DDRPHY_BASE_ADDR+(0x0F50), DDRPHY_BASE_ADDR+(0x0F6C) },
    {DDRPHY_BASE_ADDR+(0x0FA0), DDRPHY_BASE_ADDR+(0x0FC4) },
    {DDRPHY_BASE_ADDR+(0x1000), DDRPHY_BASE_ADDR+(0x101C) },//R2
    {DDRPHY_BASE_ADDR+(0x1050), DDRPHY_BASE_ADDR+(0x106C) },
    {DDRPHY_BASE_ADDR+(0x10A0), DDRPHY_BASE_ADDR+(0x10C4) },
};

#if REG_SHUFFLE_REG_CHECK
void ShuffleRegCheckProgram(U32 u4Addr)
{
#ifndef OLYMPUS_TO_BE_PORTING

    U32 Offset, TmpAddr;

    if((u4Addr >= DRAMC_AO_BASE_ADDRESS)  && (u4Addr < DDRPHY_BASE_ADDR))
    {
        TmpAddr = (DRAMC_AO_BASE_ADDRESS | (u4Addr &0xffff));
    }
    else
    {
        TmpAddr = (DDRPHY_BASE_ADDR | (u4Addr &0xffff));
    }

        for (Offset = 0; Offset < FREQREG_SIZE; Offset++)
        {
            if(TmpAddr ==LowFreq_ShuffleReg[Offset].uiSourceAddr)
            {
                mcSHOW_DBG_MSG(("\n[ShuffleRegCheck]  OK 0x%x \n",u4Addr));
                mcFPRINTF((fp_A60501, "\n[ShuffleRegCheck]  OK 0x%x\n",u4Addr));
                break;
            }
        }

        if(Offset ==FREQREG_SIZE)
        {
            mcSHOW_DBG_MSG(("\n[ShuffleRegCheck]  Not in shuffle 0x%x \n",u4Addr));
            mcFPRINTF((fp_A60501, "\n[ShuffleRegCheck]  Not in shuffle 0x%x\n",u4Addr));
        }
#endif
}
#endif


#if __ETT__
void DramcDumpFreqSetting(DRAMC_CTX_T *p)
{
    U32 Offset;
    U32 u4RegValue;
    U32 i,j;
    U32 tbIdx;

    mcSHOW_DBG_MSG(("Dump shuffle registers...\n"));
    for(i=0; i<CHANNEL_NUMBER; i++)
    {
        mcSHOW_DBG_MSG(("DRAMC CH_%d\n", i));
        for(tbIdx=0; tbIdx<SHUFFLE_ADDR_NUM_DRAMC; tbIdx++)
        {
            U32 normalAddr=ShuffleRegTableDramc[tbIdx].u4StartAddr+((U32)i << POS_BANK_NUM);
            while(normalAddr<=ShuffleRegTableDramc[tbIdx].u4EndAddr+((U32)i << POS_BANK_NUM))
            {
                for(j=0; j<SHUFFLE_GROUP; j++)
                {
                    U32 shuffleAddr = normalAddr + SHUFFLE_GROPU_OFFSET_DRAMC*j;
                    U32 regValue = u4IO32Read4B(shuffleAddr);

                    #ifdef ETT_PRINT_FORMAT
                    mcSHOW_DBG_MSG(("[SHU%d]Addr 0x%X=0x%X ", j,  shuffleAddr, regValue));
                    #else
                    mcSHOW_DBG_MSG(("[SHU%2d] Addr 0x%8xh=0x%8xh  ", j,  shuffleAddr, regValue));
                    #endif
                }
                mcSHOW_DBG_MSG(("\n"));
                normalAddr+=4;
            }
        }
    }

    //PHY
    for(i=0; i<CHANNEL_NUMBER; i++)
    {
        mcSHOW_DBG_MSG(("DDRPHY CH_%d\n", i));
        for(tbIdx=0; tbIdx<SHUFFLE_ADDR_NUM_DDRPHY; tbIdx++)
        {
            U32 normalAddr=ShuffleRegTableDDRPhy[tbIdx].u4StartAddr+((U32)i << POS_BANK_NUM);
            while(normalAddr<=ShuffleRegTableDDRPhy[tbIdx].u4EndAddr+((U32)i << POS_BANK_NUM))
            {
                for(j=0; j<SHUFFLE_GROUP; j++)
                {
                    U32 shuffleAddr = normalAddr + SHUFFLE_GROPU_OFFSET_DDRPHY*j;
                    U32 regValue = u4IO32Read4B(shuffleAddr);

                    #ifdef ETT_PRINT_FORMAT
                    mcSHOW_DBG_MSG(("[SHU%d]Addr 0x%X=0x%X ", j,  shuffleAddr, regValue));
                    #else
                    mcSHOW_DBG_MSG(("[SHU%2d]Addr 0x%8xh=0x%8xh  ", j,  shuffleAddr, regValue));
                    #endif
                }
                mcSHOW_DBG_MSG(("\n"));
                normalAddr+=4;
            }
        }
    }
}

void DramcDFSTestCode(DRAMC_CTX_T *p)
{
#ifdef HW_GATING
    if(p->dram_type == TYPE_LPDDR3|| (u1IsLP4Family(p->dram_type)))
    {
        vIO32WriteFldMulti_All(DRAMC_REG_SHUCTRL2, P_Fld(0x1, SHUCTRL2_R_DVFS_OPTION)
                | P_Fld(0x1, SHUCTRL2_R_DVFS_PARK_N));
    }
    else
    {
        vIO32WriteFldMulti_All(DRAMC_REG_SHUCTRL2, P_Fld(0x0, SHUCTRL2_R_DVFS_OPTION)
                    | P_Fld(0x0, SHUCTRL2_R_DVFS_PARK_N));
    }
#else
    vIO32WriteFldMulti_All(DRAMC_REG_SHUCTRL2, P_Fld(0x0, SHUCTRL2_R_DVFS_OPTION)
                    | P_Fld(0x0, SHUCTRL2_R_DVFS_PARK_N));
#endif
//    vIO32WriteFldMulti_All(DRAMC_REG_SHUCTRL2, P_Fld(0x3F, SHUCTRL2_R_DLL_IDLE));
//    vIO32WriteFldAlign_All(DRAMC_REG_DVFSDLL, 0xFFFFFFFF, Fld(32,0,AC_FULLDW));
//    vIO32WriteFldAlign_All(DRAMC_REG_SHU_MISC, 0x0, SHU_MISC_REQQUE_MAXCNT);
//    vIO32WriteFldMulti_All(DRAMC_REG_SHUCTRL2,  P_Fld(0xFF, SHUCTRL2_SHU_PERIOD_GO_ZERO_CNT));
}

#endif  //__ETT__

void DramcSaveToShuffleReg(DRAMC_CTX_T *p, DRAM_DFS_SHUFFLE_TYPE_T srcRG, DRAM_DFS_SHUFFLE_TYPE_T dstRG)
{
    U32 Offset;
    U32 u4RegValue;
    U32 i;
    U32 tbIdx;

    mcSHOW_DBG_MSG(("Save  frequency registers setting into shuffle register. \n"));

    //DRAMC
    for(i=0; i<CHANNEL_NUMBER; i++)
    {
        for(tbIdx=0; tbIdx<SHUFFLE_ADDR_NUM_DRAMC; tbIdx++)
        {
            U32 normalAddr=ShuffleRegTableDramc[tbIdx].u4StartAddr+((U32)i << POS_BANK_NUM);
            while(normalAddr<=ShuffleRegTableDramc[tbIdx].u4EndAddr+((U32)i << POS_BANK_NUM))
            {
                U32 srcAddr = normalAddr + SHUFFLE_GROPU_OFFSET_DRAMC*srcRG;
                U32 shuffleAddr = normalAddr + SHUFFLE_GROPU_OFFSET_DRAMC*dstRG;
                U32 regValue = u4IO32Read4B(srcAddr);
                vIO32Write4B(shuffleAddr, regValue);
                //mcSHOW_DBG_MSG(("0x%X -> 0x%X\n", normalAddr, shuffleAddr));
                normalAddr+=4;
            }
        }
    }

    //PHY
    for(i=0; i<CHANNEL_NUMBER; i++)
    {
        for(tbIdx=0; tbIdx<SHUFFLE_ADDR_NUM_DDRPHY; tbIdx++)
        {
            U32 normalAddr=ShuffleRegTableDDRPhy[tbIdx].u4StartAddr+((U32)i << POS_BANK_NUM);
            while(normalAddr<=ShuffleRegTableDDRPhy[tbIdx].u4EndAddr+((U32)i << POS_BANK_NUM))
            {
                U32 srcAddr = normalAddr + SHUFFLE_GROPU_OFFSET_DDRPHY*srcRG;
                U32 shuffleAddr = normalAddr + SHUFFLE_GROPU_OFFSET_DDRPHY*dstRG;
                U32 regValue = u4IO32Read4B(srcAddr);
                vIO32Write4B(shuffleAddr, regValue);
                //mcSHOW_DBG_MSG(("0x%X -> 0x%X\n", normalAddr, shuffleAddr));
                normalAddr+=4;
            }
        }
    }

    //PHY-exception
    for(i=0; i<CHANNEL_NUMBER; i++)
    {
        U32 normalAddr=DDRPHY_SSC_SHU1_PLL5+((U32)i << POS_BANK_NUM);
        while(normalAddr<=DDRPHY_SSC_SHU1_PLL8+((U32)i << POS_BANK_NUM))
        {
            U32 srcOffset = (srcRG==0)? 0 : 0x20;
            U32 dstOffset = (dstRG==0)? 0 : 0x20;
            U32 srcAddr = normalAddr + 0x10*srcRG + srcOffset;
            U32 shuffleAddr = normalAddr + 0x10*dstRG + dstOffset;
            U32 regValue = u4IO32Read4B(srcAddr);
            vIO32Write4B(shuffleAddr, regValue);
            //mcSHOW_DBG_MSG(("0x%X -> 0x%X\n", normalAddr, shuffleAddr));
            normalAddr+=4;
        }
    }
}


#if 1
#if __ETT__
void DramcDFSDirectJump(DRAMC_CTX_T *p, U8 shu_level)
{
    U8 u1ShuAck = 0;
    U8 i = 0;
    for(i=0;i<p->support_channel_num; i++)
    {
        u1ShuAck |= (0x1<<i);
    }

    if(gu1PhyPLLEn)
    {
        mcSHOW_DBG_MSG(("DramcDFSDirectJump to CLRPLL, SHU_LEVEL=%d, ACK=%x\n", shu_level, u1ShuAck));
    }
    else
    {
        mcSHOW_DBG_MSG(("DramcDFSDirectJump to PHYPLL, SHU_LEVEL=%d, ACK=%x\n", shu_level, u1ShuAck));
    }


    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL_SHU_EN_PCM);
    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL2_SHU_EN_PCM);

    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL_PCM);
    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, shu_level, SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL_PCM);

    if(gu1PhyPLLEn)
    {
        vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL2_SHU_EN_PCM);
        mcDELAY_US(1);
        vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL2_MODE_SW_PCM);
        mcSHOW_DBG_MSG(("Enable CLRPLL\n"));
    }
    else
    {
        vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL_SHU_EN_PCM);
        mcDELAY_US(1);
        vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW_PCM);
        mcSHOW_DBG_MSG(("Enable PHYPLL\n"));
    }
    mcDELAY_US(20);

    mcSHOW_DBG_MSG(("Enable SHORT-QUEUE\n"));
    vIO32WriteFldAlign(SPM_POWER_ON_VAL1, 1, SPM_POWER_ON_VAL1_SC_DR_SHORT_QUEUE_PCM);
    while ((u4IO32ReadFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, DRAMC_DPY_CLK_SW_CON2_SC_DR_SHORT_QUEUE_ACK) & u1ShuAck ) != u1ShuAck)
    {
        mcSHOW_DBG_MSG(("    wait short queue ack...\n"));
    }


    /*TINFO="DRAM : set ddrphy_fb_ck_en=1"*/
    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN_PCM);


    mcSHOW_DBG_MSG(("Disable RX-Tracking\n"));
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SPM_CON, 0, DRAMC_DPY_CLK_SPM_CON_PCM_DPY_RX_TRACKING_EN);


    mcSHOW_DBG_MSG(("SHUFFLE Start\n"));
    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1,  SPM_POWER_ON_VAL0_SC_DR_SHU_EN_PCM);
    while ((u4IO32ReadFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, DRAMC_DPY_CLK_SW_CON2_SC_DR_SHU_EN_ACK) & u1ShuAck ) != u1ShuAck)
    {
        mcSHOW_DBG_MSG(("    wait shu_en ack...\n"));
    }

    vIO32WriteFldAlign(SPM_POWER_ON_VAL1, 0, SPM_POWER_ON_VAL1_SC_DR_SHORT_QUEUE_PCM);
    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0,  SPM_POWER_ON_VAL0_SC_DR_SHU_EN_PCM);
    mcSHOW_DBG_MSG(("SHUFFLE End\n"));

    if(shu_level == 0)
    {
        mcSHOW_DBG_MSG(("Enable RX-Tracking for shuffle-0\n"));
        vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SPM_CON, 1, DRAMC_DPY_CLK_SPM_CON_PCM_DPY_RX_TRACKING_EN);
    }

    /*TINFO="DRAM : set ddrphy_fb_ck_en=0"*/
    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN_PCM);

    if(gu1PhyPLLEn)
    {
        vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW_PCM);

        /*TINFO="DRAM : record PLL usage information for low power scenaio"*/
        //! SPM_SW_RSV_5[0] = 1 : using PHYPLL ,  = 0 : using CLRPLL
        *((volatile unsigned int *)(SPM_SW_RSV_5)) &= ~(0x1 << 0);
    }
    else
    {
        /*TINFO="DRAM : set sc_phypll_mode_sw=0"*/
        vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL2_MODE_SW_PCM);

        /*TINFO="DRAM : record PLL usage information for low power scenaio"*/
        //! SPM_SW_RSV_5[0] = 1 : using PHYPLL ,  = 0 : using CLRPLL
        *((volatile unsigned int *)(SPM_SW_RSV_5)) |= (0x1 << 0);
    }

    mcSHOW_DBG_MSG(("Shuffle flow complete\n"));

    #if 0//ENABLE_TX_TRACKING
    {
        DRAM_CHANNEL_T channel_bak;
        channel_bak = p->channel;

        vSetPHY2ChannelMapping(p, CHANNEL_A);
        DramcDFSUpdateFreqRatio(p);

        vSetPHY2ChannelMapping(p, CHANNEL_B);
        DramcDFSUpdateFreqRatio(p);

        vSetPHY2ChannelMapping(p, channel_bak);
    }
    //vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 0, DQSOSCR_DQSOSCRDIS);
    //vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 0, DQSOSCR_DQSOSCENDIS);
    //vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), 0, SLP4_TESTMODE_K_DQSOSCENDIS);
    #endif

    gu1PhyPLLEn = !gu1PhyPLLEn;
    return;
}
#endif

#else
void DramcDFSDirectJump(DRAMC_CTX_T *p, U8 shu_level)
{
    U8 u1ShuAck = 0;
    U8 i = 0;
    for(i=0;i<p->support_channel_num; i++)
    {
        u1ShuAck |= (0x1<<i);
    }

    if(gu1PhyPLLEn)
    {
        mcSHOW_DBG_MSG(("DramcDFSDirectJump to CLRPLL, SHU_LEVEL=%d, ACK=%x\n", shu_level, u1ShuAck));
    }
    else
    {
        mcSHOW_DBG_MSG(("DramcDFSDirectJump to PHYPLL, SHU_LEVEL=%d, ACK=%x\n", shu_level, u1ShuAck));
    }

    /*TINFO="DRAM : start shuffle from PHYPLL to CLRPLL by SPM SW"*/
    //! start shuffle to level
    /*TINFO="DRAM : set sc_phypll_shu_en=0"*/
    *((volatile unsigned int *)(SPM_POWER_ON_VAL0))         &=  ~(0x1 << 26);

    /*TINFO="DRAM : set sc_phypll2_shu_en=0"*/
    *((volatile unsigned int *)(SPM_POWER_ON_VAL0))         &=  ~(0x1 << 27);

    /*TINFO="DRAM : set sc_dr_shu_level = test case"*/
    *((volatile unsigned int *)(SPM_POWER_ON_VAL0))         &=  ~(0x3 << 30);
    *((volatile unsigned int *)(SPM_POWER_ON_VAL0))         |=   (shu_level << 30);

    if(gu1PhyPLLEn)
    {
        /*TINFO="DRAM : set sc_phypll2_shu_en=1"*/
        *((volatile unsigned int *)(SPM_POWER_ON_VAL0))         |=   (0x1 << 27);

        /*TINFO="DRAM : wait 1000 ns"*/
        mcDELAY_US(1);

        /*TINFO="DRAM : set sc_phypll2_mode_sw=1"*/
        *((volatile unsigned int *)(SPM_POWER_ON_VAL0))         |=   (0x1 << 29);

        mcSHOW_DBG_MSG(("Enable CLRPLL\n"));
    }
    else
    {
        /*TINFO="DRAM : set sc_phypll_shu_en=1"*/
        *((volatile unsigned int *)(SPM_POWER_ON_VAL0))         |=   (0x1 << 26);

        /*TINFO="DRAM : wait 1000 ns"*/
        mcDELAY_US(1);

        /*TINFO="DRAM : set sc_phypll_mode_sw=1"*/
        *((volatile unsigned int *)(SPM_POWER_ON_VAL0))         |=   (0x1 << 28);

        mcSHOW_DBG_MSG(("Enable PHYPLL\n"));
    }

    /*TINFO="DRAM : wait 20us"*/
    mcDELAY_US(20);


    /*TINFO="DRAM : set sc_short_queue=1"*/
    *((volatile unsigned int *)(SPM_POWER_ON_VAL1))         |=   (0x1 << 3);

    /*TINFO="DRAM : wait short queue ack"*/
    while (((*((volatile unsigned int *)(SPM_DRAMC_DPY_CLK_SW_CON2))>>20) & u1ShuAck ) != u1ShuAck)
    {
        mcSHOW_DBG_MSG(("wait short queue ack...\n"));
    }


    /*TINFO="DRAM : set ddrphy_fb_ck_en=1"*/
    *((volatile unsigned int *)(SPM_POWER_ON_VAL0))         |=   (0x1 << 24);


    /*TINFO="DRAM : set rx dly enable = 0"*/
    *((volatile unsigned int *)(SPM_DRAMC_DPY_CLK_SPM_CON)) &=   ~(0x1 << 3);


    /*TINFO="DRAM : set sc_dr_shu_en = 1"*/
    *((volatile unsigned int *)(SPM_POWER_ON_VAL0))         |=   (0x1 << 25);

    /*TINFO="DRAM : wait shu_en ack*/
    while (((*((volatile unsigned int *)(SPM_DRAMC_DPY_CLK_SW_CON2)) >> 12 ) & u1ShuAck ) != u1ShuAck)
    {
        mcSHOW_DBG_MSG(("wait shu_en ack...\n"));
    }


    /*TINFO="DRAM : set sc_short_queue=0"*/
    *((volatile unsigned int *)(SPM_POWER_ON_VAL1))         &=  ~(0x1 << 3);

    /*TINFO="DRAM : set sc_dr_shu_en = 0"*/
    *((volatile unsigned int *)(SPM_POWER_ON_VAL0))         &=   ~(0x1 << 25);


    if(shu_level == 0)
    {
      /*TINFO="DRAM : set rx dly enable = 1 due to shu to level 0"*/
      *((volatile unsigned int *)(SPM_DRAMC_DPY_CLK_SPM_CON))   |=   (0x1 << 3);
    }

    /*TINFO="DRAM : set ddrphy_fb_ck_en=0"*/
    *((volatile unsigned int *)(SPM_POWER_ON_VAL0))         &=   ~(0x1 << 24);

    if(gu1PhyPLLEn)
    {
        /*TINFO="DRAM : set sc_phypll_mode_sw=0"*/
        *((volatile unsigned int *)(SPM_POWER_ON_VAL0))         &=   ~(0x1 << 28);

        /*TINFO="DRAM : record PLL usage information for low power scenaio"*/
        //! SPM_SW_RSV_5[0] = 1 : using PHYPLL ,  = 0 : using CLRPLL
        *((volatile unsigned int *)(SPM_SW_RSV_5))              &= ~(0x1 << 0);
    }
    else
    {
        /*TINFO="DRAM : set sc_phypll_mode_sw=0"*/
        *((volatile unsigned int *)(SPM_POWER_ON_VAL0))         &=   ~(0x1 << 29);

        /*TINFO="DRAM : record PLL usage information for low power scenaio"*/
        //! SPM_SW_RSV_5[0] = 1 : using PHYPLL ,  = 0 : using CLRPLL
        *((volatile unsigned int *)(SPM_SW_RSV_5))              |=    (0x1 << 0);
    }

    mcSHOW_DBG_MSG(("Shuffle flow complete\n"));
    /*TINFO="DRAM : PHYPLL shuffle to CLRPLL SPM sw flow done "*/


    #if ENABLE_TX_TRACKING
    {
        DRAM_CHANNEL_T channel_bak;
        channel_bak = p->channel;

        vSetPHY2ChannelMapping(p, CHANNEL_A);
        DramcDFSUpdateFreqRatio(p);

        vSetPHY2ChannelMapping(p, CHANNEL_B);
        DramcDFSUpdateFreqRatio(p);

        vSetPHY2ChannelMapping(p, channel_bak);
    }
    //vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 0, DQSOSCR_DQSOSCRDIS);
    //vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 0, DQSOSCR_DQSOSCENDIS);
    //vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), 0, SLP4_TESTMODE_K_DQSOSCENDIS);
    #endif

    gu1PhyPLLEn = !gu1PhyPLLEn;
    return;
}
#endif

void CBT_DramcDFSDirectJump(DRAMC_CTX_T *p, U8 shu_level)
{
#if 0
    U8 ShortQueue_ACK_Value;

    //enable another PLL
    if(gu1PhyPLLEn)
    {
        mcSHOW_DBG_MSG3(("Disable CLRPLL\n"));
        //vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 1, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL2_MODE_SW);
        vIO32WriteFldAlign(DDRPHY_PLL2, 0, PLL2_RG_RCLRPLL_EN);
    }
    else
    {
        mcSHOW_DBG_MSG3(("Disable PHYPLL\n"));
        //vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 1, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL_MODE_SW);
        vIO32WriteFldAlign(DDRPHY_PLL1, 0, PLL1_RG_RPHYPLL_EN);
    }

    if(u1IsLP4Family(p->dram_type))
        ShortQueue_ACK_Value = 3;
    else
        ShortQueue_ACK_Value = 1;

    //release PLL shuffle enable
    vIO32WriteFldMulti(SPM_DRAMC_DPY_CLK_SW_CON2, P_Fld(0, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL_SHU_EN)
                                               | P_Fld(0, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL2_SHU_EN));

    //enable shuffle for another PLL related RG
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, shu_level, DRAMC_DPY_CLK_SW_CON2_SW_DR_SHU_LEVEL);
    if(gu1PhyPLLEn)
    {
        vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 1, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL2_SHU_EN);
    }
    else
    {
        vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 1, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL_SHU_EN);
    }
    mcDELAY_US(1);

    //enable another PLL
    if(gu1PhyPLLEn)
    {
        mcSHOW_DBG_MSG3(("Enable CLRPLL\n"));
        //vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 1, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL2_MODE_SW);
        vIO32WriteFldAlign(DDRPHY_PLL2, 1, PLL2_RG_RCLRPLL_EN);
    }
    else
    {
        mcSHOW_DBG_MSG3(("Enable PHYPLL\n"));
        //vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 1, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL_MODE_SW);
        vIO32WriteFldAlign(DDRPHY_PLL1, 1, PLL1_RG_RPHYPLL_EN);
    }
    //wait for PLL stable
    mcDELAY_US(20);

    //change request queue depth to 2 if current frequency is DDR-1600
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 1, DRAMC_DPY_CLK_SW_CON2_SW_DR_SHORT_QUEUE);
    //wait request queue depth change to low ack

    while((u4IO32ReadFldAlign(SPM_DRAMC_DPY_CLK_SW_CON, DRAMC_DPY_CLK_SW_CON_SC_DR_SHORT_QUEUE_ACK)&ShortQueue_ACK_Value)!=ShortQueue_ACK_Value)
    {
        mcSHOW_DBG_MSG(("Wait request queue depth change to low ack\n"));
        mcDELAY_US(1);
    }

    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON, 0x3, DRAMC_DPY_CLK_SW_CON_SW_DDRPHY_FB_CK_EN); //set TOP feedback MCK to full frequency
    vIO32WriteFldAlign(SPM_DRAMC_SPM_SW_RSV_8, 0x0, Fld(2,6,AC_MSKB0)); //RX input delay HW tracking disable

    #if 0//ENABLE_TX_TRACKING
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), 1, SLP4_TESTMODE_K_DQSOSCENDIS);
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 1, DQSOSCR_DQSOSCENDIS);
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 1, DQSOSCR_DQSOSCRDIS);
    #endif

    if(*((volatile unsigned int *)(0x10212c08)))
    {
        mcSHOW_DBG_MSG3(("DMA ongoing\n"));
    }
    else
    {
        mcSHOW_DBG_MSG3(("DMA is done\n"));
    }

    //shuffle enable
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 1, DRAMC_DPY_CLK_SW_CON2_SW_DR_SHU_EN);
    //Shuffle done, release ALE blocking
    while((u4IO32ReadFldAlign(SPM_DRAMC_DPY_CLK_SW_CON, DRAMC_DPY_CLK_SW_CON_SC_DR_SHU_EN_ACK)&ShortQueue_ACK_Value)!=ShortQueue_ACK_Value)
    {
        mcSHOW_DBG_MSG(("Wait shuffle done, release ALE blocking\n"));
        mcDELAY_US(1);
    }

    //change request queue depth to 8
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 0, DRAMC_DPY_CLK_SW_CON2_SW_DR_SHORT_QUEUE);
    //clear R_DMDRAMCSHU & accept new requests
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 0, DRAMC_DPY_CLK_SW_CON2_SW_DR_SHU_EN);

    vIO32WriteFldAlign(SPM_DRAMC_SPM_SW_RSV_8, 0x0, Fld(2,6,AC_MSKB0)); //RX input delay HW tracking enable
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON, 0x0, DRAMC_DPY_CLK_SW_CON_SW_DDRPHY_FB_CK_EN); //set TOP feedback MCK to divided frequency

    //disable current PLL
    if(gu1PhyPLLEn)
    {
        //vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 0, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL_MODE_SW);
        vIO32WriteFldAlign(DDRPHY_PLL1, 0, PLL1_RG_RPHYPLL_EN);
    }
    else
    {
        //vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 0, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL2_MODE_SW);
        vIO32WriteFldAlign(DDRPHY_PLL2, 0, PLL2_RG_RCLRPLL_EN);
    }

    #if 0//ENABLE_TX_TRACKING
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 0, DQSOSCR_DQSOSCRDIS);
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 0, DQSOSCR_DQSOSCENDIS);
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), 0, SLP4_TESTMODE_K_DQSOSCENDIS);
    {
        U8 backup_channel;
        backup_channel = p->channel;
        vSetPHY2ChannelMapping(p, CHANNEL_A);
        DramcDFSUpdateFreqRatio(p);

        vSetPHY2ChannelMapping(p, CHANNEL_B);
        DramcDFSUpdateFreqRatio(p);

        vSetPHY2ChannelMapping(p, backup_channel);
    }
    #endif

    gu1PhyPLLEn = !gu1PhyPLLEn;
#endif
    return;
}

void No_Parking_On_CLRPLL(DRAMC_CTX_T *p)
{
    U8 shu_level;

    if (gu1PhyPLLEn) return; /* already parking on PHYPLL */

    shu_level = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHUSTATUS), SHUSTATUS_SHUFFLE_LEVEL);

    CBT_DramcDFSDirectJump(p, shu_level); /* parking on PHYPLL */
}

void DFSSwitchFreq(DRAMC_CTX_T *p)
{
    DramcEnterSelfRefresh_Everest(p, 1);

    ///TODO: PLL init, DLL init

    DramcEnterSelfRefresh_Everest(p, 0);
}
#endif  // DUAL_FREQ_K


U16 u2DFSGetHighestFreq(DRAMC_CTX_T * p)
{
    if(u1IsLP4Family(p->dram_type))
    {
        return 1600;
    }
    else
    {
        return 933;
    }
}


// both "ENABLE_RANK_NUMBER_AUTO_DETECTION" and "DUAL_FREQ_K" use this API
void DFSInitForCalibration(DRAMC_CTX_T *p)
{
    DramcInit(p);
    vApplyConfigBeforeCalibration(p);
}


void DramcHWDQSGatingTracking_DVT(DRAMC_CTX_T *p)
{
#ifdef HW_GATING
if(u1IsLP4Family(p->dram_type))
{
#if DramcHWDQSGatingTracking_DVT_FIFO_MODE
    //REFUICHG=0, STB_SHIFT_DTCOUT_IG=0, DQSG_MODE=1, NARROW_IG=0
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), P_Fld(1, STBCAL_STB_DQIEN_IG) | P_Fld(1, STBCAL_PICHGBLOCK_NORD) | \
                                            P_Fld(0, STBCAL_REFUICHG) | P_Fld(0, STBCAL_PHYVALID_IG) | \
                                            P_Fld(0, STBCAL_STBSTATE_OPT)| P_Fld(0, STBCAL_STBDLELAST_FILTER) | \
                                            P_Fld(0, STBCAL_STBDLELAST_PULSE) | \
                                            P_Fld(0, STBCAL_STBDLELAST_OPT)|  \
                                            P_Fld(1, STBCAL_PIMASK_RKCHG_OPT));

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), \
                                            P_Fld(1, STBCAL1_STBCAL_FILTER)| \
                                            P_Fld(1, STBCAL1_STB_FLAGCLR) | \
                                            P_Fld(0, STBCAL1_STB_SHIFT_DTCOUT_IG));

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL0), \
                                            P_Fld(1, MISC_CTRL0_R_DMDQSIEN_FIFO_EN) | \
                                            P_Fld(0, MISC_CTRL0_R_DMVALID_DLY)|  \
                                            P_Fld(0, MISC_CTRL0_R_DMVALID_DLY_OPT) | \
                                            P_Fld(0, MISC_CTRL0_R_DMVALID_NARROW_IG) | \
                                            P_Fld(0, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 0, B0_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 0, B1_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B1);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 0, CA_CMD6_RG_RX_ARCMD_DMRANK_OUTSEL);

    #else
    //REFUICHG=0, STB_SHIFT_DTCOUT_IG=0, DQSG_MODE=1, NARROW_IG=0
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), P_Fld(1, STBCAL_STB_DQIEN_IG) | P_Fld(1, STBCAL_PICHGBLOCK_NORD) | \
                                            P_Fld(0, STBCAL_REFUICHG) | P_Fld(0, STBCAL_PHYVALID_IG) | \
                                            P_Fld(0, STBCAL_STBSTATE_OPT)| P_Fld(0, STBCAL_STBDLELAST_FILTER) | \
                                            P_Fld(0, STBCAL_STBDLELAST_PULSE) | \
                                            P_Fld(0, STBCAL_STBDLELAST_OPT)|  \
                                            P_Fld(1, STBCAL_PIMASK_RKCHG_OPT));

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), \
                                            P_Fld(1, STBCAL1_STBCAL_FILTER)| \
                                            P_Fld(1, STBCAL1_STB_FLAGCLR) | \
                                            P_Fld(0, STBCAL1_STB_SHIFT_DTCOUT_IG));


    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL0), \
                                            P_Fld(0, MISC_CTRL0_R_DMDQSIEN_FIFO_EN) | \
                                            P_Fld(3, MISC_CTRL0_R_DMVALID_DLY)|  \
                                            P_Fld(1, MISC_CTRL0_R_DMVALID_DLY_OPT) | \
                                            P_Fld(0, MISC_CTRL0_R_DMVALID_NARROW_IG) | \
                                            P_Fld(0xf, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 1, B0_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 1, B1_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B1);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 1, CA_CMD6_RG_RX_ARCMD_DMRANK_OUTSEL);
    #endif

}
else
{
#if DramcHWDQSGatingTracking_DVT_JADE_TRACKING_MODE
        //UPDBYWR=0, REFUICHG=1, STB_SHIFT_DTCOUT_IG=0, DQSG_MODE=1, NARROW_IG=1
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), 0, REFCTRL0_UPDBYWR);
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), P_Fld(0, STBCAL_STB_DQIEN_IG) | P_Fld(0, STBCAL_PICHGBLOCK_NORD) | \
                                                P_Fld(1, STBCAL_REFUICHG) | P_Fld(0, STBCAL_PHYVALID_IG) | \
                                                P_Fld(1, STBCAL_STBSTATE_OPT)| P_Fld(0, STBCAL_STBDLELAST_FILTER) | \
                                                P_Fld(0, STBCAL_STBDLELAST_PULSE) | \
                                                P_Fld(0, STBCAL_STBDLELAST_OPT)|  \
                                                P_Fld(0, STBCAL_PIMASK_RKCHG_OPT));

        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), \
                                                P_Fld(0, STBCAL1_STBCAL_FILTER)| \
                                                P_Fld(1, STBCAL1_STB_FLAGCLR) | \
                                                P_Fld(0, STBCAL1_STB_SHIFT_DTCOUT_IG));

        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL0), \
                                                P_Fld(0, MISC_CTRL0_R_DMDQSIEN_FIFO_EN) | \
                                                P_Fld(2, MISC_CTRL0_R_DMVALID_DLY)|  \
                                                P_Fld(1, MISC_CTRL0_R_DMVALID_DLY_OPT) | \
                                                P_Fld(1, MISC_CTRL0_R_DMVALID_NARROW_IG) | \
                                                P_Fld(0xf, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));

#else
        //REFUICHG=0, STB_SHIFT_DTCOUT_IG=0, DQSG_MODE=1, NARROW_IG=0
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), P_Fld(1, STBCAL_STB_DQIEN_IG) | P_Fld(1, STBCAL_PICHGBLOCK_NORD) | \
                                                P_Fld(0, STBCAL_REFUICHG) | P_Fld(0, STBCAL_PHYVALID_IG) | \
                                                P_Fld(0, STBCAL_STBSTATE_OPT)| P_Fld(0, STBCAL_STBDLELAST_FILTER) | \
                                                P_Fld(0, STBCAL_STBDLELAST_PULSE) | \
                                                P_Fld(0, STBCAL_STBDLELAST_OPT)|  \
                                                P_Fld(1, STBCAL_PIMASK_RKCHG_OPT));

        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), \
                                                P_Fld(1, STBCAL1_STBCAL_FILTER)| \
                                                P_Fld(1, STBCAL1_STB_FLAGCLR) | \
                                                P_Fld(0, STBCAL1_STB_SHIFT_DTCOUT_IG));

        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL0), \
                                                P_Fld(0, MISC_CTRL0_R_DMDQSIEN_FIFO_EN) | \
                                                P_Fld(5, MISC_CTRL0_R_DMVALID_DLY)|  \
                                                P_Fld(1, MISC_CTRL0_R_DMVALID_DLY_OPT) | \
                                                P_Fld(0, MISC_CTRL0_R_DMVALID_NARROW_IG) | \
                                                P_Fld(0xf, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));

#endif

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 1, B0_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 1, B1_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B1);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 1, CA_CMD6_RG_RX_ARCMD_DMRANK_OUTSEL);


}
#endif
}


void DramcHWGatingInit(DRAMC_CTX_T *p)
{
#ifdef HW_GATING
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), P_Fld(0, STBCAL_STBCALEN) | \
                                            P_Fld(0, STBCAL_STBCAL2R)| P_Fld(0, STBCAL_STB_SELPHYCALEN) | \
                                            P_Fld(0, STBCAL_STBSTATE_OPT) | \
                                            P_Fld(0, STBCAL_RKCHGMASKDIS)| P_Fld(0, STBCAL_REFUICHG) | \
                                            P_Fld(1, STBCAL_PICGEN));

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), P_Fld(1, STBCAL_DQSIENCG_CHG_EN));
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 0, STBCAL_CG_RKEN);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 1, STBCAL_DQSIENCG_NORMAL_EN);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 1, MISC_CTRL1_R_DMDQSIENCG_EN);

    DramcHWDQSGatingTracking_DVT(p);
#endif

#ifdef GATING_7UI_FOR_TRACKING
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), 0x1, STBCAL1_DQSIEN_7UI_EN);		
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 0x2, B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0);		
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 0x2, B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1);

	mcSHOW_DBG_MSG(("GATING_7UI_FOR_TRACKING: ON\n"));
#else
	mcSHOW_DBG_MSG(("GATING_7UI_FOR_TRACKING: OFF\n"));
#endif

}


void DramcHWGatingOnOff(DRAMC_CTX_T *p, U8 u1OnOff)
{
    #ifdef HW_GATING
    vIO32WriteFldMulti_All(DRAMC_REG_SHUCTRL2, P_Fld(u1OnOff, SHUCTRL2_R_DVFS_OPTION)
                | P_Fld(u1OnOff, SHUCTRL2_R_DVFS_PARK_N));
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), u1OnOff, EYESCAN_STB_GERRSTOP);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), u1OnOff, STBCAL_STBCALEN);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), u1OnOff, STBCAL_STB_SELPHYCALEN);
    #else
    vIO32WriteFldMulti_All(DRAMC_REG_SHUCTRL2, P_Fld(0x0, SHUCTRL2_R_DVFS_OPTION)
                    | P_Fld(0x0, SHUCTRL2_R_DVFS_PARK_N));
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 0, EYESCAN_STB_GERRSTOP);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 0, STBCAL_STBCALEN);   // PI tracking off = HW gating tracking off
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 0, STBCAL_STB_SELPHYCALEN);
    #endif
}

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#if (__ETT__ || CPU_RW_TEST_AFTER_K)

U16 u2MaxGatingPos[CHANNEL_MAX][RANK_MAX][DQS_NUMBER]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
U16 u2MinGatingPos[CHANNEL_MAX][RANK_MAX][DQS_NUMBER]={0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

void DramcPrintHWGatingStatus(DRAMC_CTX_T *p, U8 u1Channel)
{
#ifdef HW_GATING
    U8 u1RankIdx, u1RankMax, u1ChannelBak, u1ByteIdx, ii;
    U32 u4ResultDQS_PI, u4ResultDQS_UI, u4ResultDQS_UI_P1;
    U8 u1Dqs_pi[DQS_BIT_NUMBER], u1Dqs_ui[DQS_BIT_NUMBER],u1Dqs_ui_P1[DQS_BIT_NUMBER];
    U16 u2TmpValue, u2TmpUI[DQS_NUMBER], u2TmpPI[DQS_NUMBER];
    U32 MANUDLLFRZ_bak, STBSTATE_OPT_bak;
    U32 backup_rank;

    u1ChannelBak= p->channel;
    vSetPHY2ChannelMapping(p, u1Channel);
    backup_rank = u1GetRank(p);

    #if DUAL_RANK_ENABLE
    if (p->support_rank_num==RANK_DUAL)
        u1RankMax = RANK_MAX;
    else
    #endif
         u1RankMax =RANK_1;

    MANUDLLFRZ_bak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), TEST2_3_MANUDLLFRZ);
    STBSTATE_OPT_bak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), STBCAL_STBSTATE_OPT);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), 1, TEST2_3_MANUDLLFRZ);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 1, STBCAL_STBSTATE_OPT);

    for(u1RankIdx=0; u1RankIdx<u1RankMax; u1RankIdx++)
    {
        vSetRank(p, u1RankIdx);
        mcSHOW_DBG_MSG(("[DramcHWGatingStatus] Channel=%d, Rank=%d\n", p->channel, u1RankIdx));
        u4ResultDQS_PI = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK0_DQSIENDLY));
        u1Dqs_pi[0]  = u4ResultDQS_PI & 0xff;
        u1Dqs_pi[1]  = (u4ResultDQS_PI >>8) & 0xff;
        u1Dqs_pi[2]  = (u4ResultDQS_PI >>16) & 0xff;
        u1Dqs_pi[3]  = (u4ResultDQS_PI >>24) & 0xff;
        //mcSHOW_DBG_MSG(("[DramcHWGatingStatus] PI status (DQS0, DQS1, DQS2, DQS3) =(%d, %d, %d, %d)\n", u1Dqs_pi[0], u1Dqs_pi[1], u1Dqs_pi[2], u1Dqs_pi[3]));

        u4ResultDQS_UI = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK0_DQSIENUIDLY));
        u1Dqs_ui[0]  = u4ResultDQS_UI & 0xff;
        u1Dqs_ui[1]  = (u4ResultDQS_UI >>8) & 0xff;
        u1Dqs_ui[2]  = (u4ResultDQS_UI >>16) & 0xff;
        u1Dqs_ui[3]  = (u4ResultDQS_UI >>24) & 0xff;
        //mcSHOW_DBG_MSG(("[DramcHWGatingStatus] UI status (DQS0, DQS1, DQS2, DQS3) =(%d, %d, %d, %d)\n", u1Dqs_ui[0], u1Dqs_ui[1], u1Dqs_ui[2], u1Dqs_ui[3]));
        mcSHOW_DBG_MSG((" Byte0(2T, 0.5T, PI) =(%d, %d, %d)\n Byte1(2T, 0.5T, PI) =(%d, %d, %d)\n Byte2(2T, 0.5T, PI) =(%d, %d, %d)\n Byte3(2T, 0.5T, PI) =(%d, %d, %d)\n", \
                                        u1Dqs_ui[0]/8, u1Dqs_ui[0]%8, u1Dqs_pi[0], u1Dqs_ui[1]/8, u1Dqs_ui[1]%8, u1Dqs_pi[1], \
                                        u1Dqs_ui[2]/8, u1Dqs_ui[2]%8, u1Dqs_pi[2], u1Dqs_ui[3]/8, u1Dqs_ui[3]%8, u1Dqs_pi[3]));

        u4ResultDQS_UI_P1 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK0_DQSIENUIDLY_P1));
        u1Dqs_ui_P1[0]  = u4ResultDQS_UI_P1 & 0xff;
        u1Dqs_ui_P1[1]  = (u4ResultDQS_UI_P1 >>8) & 0xff;
        u1Dqs_ui_P1[2]  = (u4ResultDQS_UI_P1 >>16) & 0xff;
        u1Dqs_ui_P1[3]  = (u4ResultDQS_UI_P1 >>24) & 0xff;
        mcSHOW_DBG_MSG((" UI_Phase1 (DQS0~3) =(%d, %d, %d, %d)\n\n", u1Dqs_ui_P1[0], u1Dqs_ui_P1[1], u1Dqs_ui_P1[2], u1Dqs_ui_P1[3]));
    }
    vSetRank(p, backup_rank);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), STBSTATE_OPT_bak, STBCAL_STBSTATE_OPT);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), MANUDLLFRZ_bak, TEST2_3_MANUDLLFRZ);

    p->channel = u1ChannelBak;
    vSetPHY2ChannelMapping(p, u1ChannelBak);
#endif
}


void DramcPrintRXDQDQSStatus(DRAMC_CTX_T *p, U8 u1Channel)
{
    U8 u1RankIdx, u1RankMax, u1ChannelBak, u1ByteIdx, ii;
    U32 u4ResultDQS_PI, u4ResultDQS_UI, u4ResultDQS_UI_P1;
    U8 u1Dqs_pi[DQS_BIT_NUMBER], u1Dqs_ui[DQS_BIT_NUMBER],u1Dqs_ui_P1[DQS_BIT_NUMBER];
    U16 u2TmpValue, u2TmpUI[DQS_NUMBER], u2TmpPI[DQS_NUMBER];
    U32 MANUDLLFRZ_bak, STBSTATE_OPT_bak;
    U32 backup_rank;
    U8 u1DQX_B0, u1DQS0, u1DQX_B1, u1DQS1;

    u1ChannelBak= p->channel;
    vSetPHY2ChannelMapping(p, u1Channel);
    backup_rank = u1GetRank(p);

#if DUAL_RANK_ENABLE
    if (p->support_rank_num==RANK_DUAL)
        u1RankMax = RANK_MAX;
    else
#endif
         u1RankMax =RANK_1;

    for(u1RankIdx=0; u1RankIdx<u1RankMax; u1RankIdx++)
    {
        vSetRank(p, u1RankIdx);
        mcSHOW_DBG_MSG(("[DramcPrintRXDQDQSStatus] Channel=%d, Rank=%d\n", p->channel, u1RankIdx));
        if (u1RankIdx==0)
        u4ResultDQS_PI = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_DQ_RXDLY_TRRO22));
        if (u1RankIdx==1)
        u4ResultDQS_PI = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_DQ_RXDLY_TRRO23));

        u1DQX_B0  = u4ResultDQS_PI & 0xff;
        u1DQS0  = (u4ResultDQS_PI >>8) & 0xff;
        u1DQX_B1  = (u4ResultDQS_PI >>16) & 0xff;
        u1DQS1  = (u4ResultDQS_PI >>24) & 0xff;

        mcSHOW_DBG_MSG((" DQX_B0, DQS0, DQX_B1, DQS1 =(%d, %d, %d, %d)\n\n", u1DQX_B0, u1DQS0, u1DQX_B1, u1DQS1));

        }
    vSetRank(p, backup_rank);

    p->channel = u1ChannelBak;
    vSetPHY2ChannelMapping(p, u1ChannelBak);
}


void DramcPrintIMPTrackingStatus(DRAMC_CTX_T *p, U8 u1Channel)
{
#ifdef IMPEDENCE_TRACKING_ENABLE

    U8 u1RankIdx, u1RankMax, u1ChannelBak, u1ByteIdx, ii;
    U8  DQS_DRVP_2,DQS_ODTN_2,DQS_DRVP,DQS_ODTN;
    U8  DQ_DRVP_2,DQ_ODTN_2,DQ_DRVP,DQ_ODTN;
    U8  CMD_DRVP_2,CMD_ODTN_2,CMD_DRVP,CMD_ODTN;

    u1ChannelBak= p->channel;
    vSetPHY2ChannelMapping(p, u1Channel);

    mcSHOW_DBG_MSG(("[DramcPrintIMPTrackingStatus] Channel=%d ================\n", p->channel));

//    if (u1Channel == CHANNEL_A)
    {
        DQS_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS1), IMPCAL_STATUS1_DRVPDQS_SAVE2);
        DQS_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS1), IMPCAL_STATUS1_DRVNDQS_SAVE2);
        DQS_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS1), IMPCAL_STATUS1_DRVPDQS_SAVE1);
        DQS_ODTN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS1), IMPCAL_STATUS1_DRVNDQS_SAVE1);

        DQ_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS1), IMPCAL_STATUS1_DRVPDQ_SAVE2);
        DQ_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS1), IMPCAL_STATUS1_DRVNDQ_SAVE2);
        DQ_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS2), IMPCAL_STATUS2_DRVPDQ_SAVE1);
        DQ_ODTN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS2), IMPCAL_STATUS2_DRVNDQ_SAVE1);

        CMD_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS2), IMPCAL_STATUS2_DRVPCMD_SAVE2);
        CMD_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS2), IMPCAL_STATUS2_DRVNCMD_SAVE2);
        CMD_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS2), IMPCAL_STATUS2_DRVPCMD_SAVE1);
        CMD_ODTN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS2), IMPCAL_STATUS2_DRVNCMD_SAVE1);
    }
#if 0
    else
    {
        U8 shu_level;

        //Channel B is workaround
        shu_level = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHUSTATUS), SHUSTATUS_SHUFFLE_LEVEL);
        mcSHOW_DBG_MSG(("francis [DramcPrintIMPTrackingStatus] shu_level=%d ========\n",shu_level));

        //DQ
        DQ_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING1 + shu_level*0x600), SHU1_DRVING1_DQDRV2_DRVP);
        DQ_DRVP   = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING2 + shu_level*0x600), SHU1_DRVING2_DQDRV1_DRVP);
        DQ_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING3 + shu_level*0x600), SHU1_DRVING3_DQODT2_ODTN);
        DQ_ODTN   = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING4 + shu_level*0x600), SHU1_DRVING4_DQODT1_ODTN);

        //DQS
        DQS_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING1 + shu_level*0x600), SHU1_DRVING1_DQSDRV2_DRVP);
        DQS_DRVP   = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING1 + shu_level*0x600), SHU1_DRVING1_DQSDRV1_DRVP);
        DQS_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING3 + shu_level*0x600), SHU1_DRVING3_DQSODT2_ODTN);
        DQS_ODTN   = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING3 + shu_level*0x600), SHU1_DRVING3_DQSODT1_ODTN);

        //CMD
        CMD_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING2 + shu_level*0x600), SHU1_DRVING2_CMDDRV2_DRVP);
        CMD_DRVP   = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING2 + shu_level*0x600), SHU1_DRVING2_CMDDRV1_DRVP);
        CMD_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING4 + shu_level*0x600), SHU1_DRVING4_CMDODT2_ODTN);
        CMD_ODTN   = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING4 + shu_level*0x600), SHU1_DRVING4_CMDODT1_ODTN);
    }
#endif

    mcSHOW_DBG_MSG((" DQS (DRVP_2,ODTN_2,DRVP,ODTN) = (%d, %d, %d, %d)\n",DQS_DRVP_2,DQS_ODTN_2,DQS_DRVP,DQS_ODTN));
    mcSHOW_DBG_MSG((" DQ  (DRVP_2,ODTN_2,DRVP,ODTN) = (%d, %d, %d, %d)\n",DQ_DRVP_2,DQ_ODTN_2,DQ_DRVP,DQ_ODTN));
    mcSHOW_DBG_MSG((" CMD (DRVP_2,ODTN_2,DRVP,ODTN) = (%d, %d, %d, %d)\n",CMD_DRVP_2,CMD_ODTN_2,CMD_DRVP,CMD_ODTN));

    p->channel = u1ChannelBak;
    vSetPHY2ChannelMapping(p, u1ChannelBak);
#endif
}
#endif  //#if __ETT__
#endif


U8 u1GetMR4RefreshRate(DRAMC_CTX_T *p, DRAM_CHANNEL_T channel)
{
    U8 u1Dummy, u1RefreshRate;

    vSetPHY2ChannelMapping(p, channel);

    //DramcModeRegRead(p, 4, &u1Dummy);
    //mcSHOW_DBG_MSG(("[u2GetRefreshRate] MR4 0x%x,  u1RefreshRate= 0x%x\n", u1Dummy, u1RefreshRate));
    u1RefreshRate = (U8)u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_REFRESH_RATE);
    //mcSHOW_DBG_MSG(("[u2GetRefreshRate] channel = %d, u1RefreshRate= 0x%x\n", channel, u1RefreshRate));

    return u1RefreshRate;
}

#if RX_DLY_TRACKING_VERIFY
/***
1. set DQS*_DVS_DLY=0, then log calibration result
2. set DQS*_DVS_DLY=4b'1xxxx, then log calibration result
3. compare 1 & 2 Rx delay
***/
void DramcRxInputDlyTrackingRG(DRAMC_CTX_T *p)
{
    DRAM_CHANNEL_T channel_bak = p->channel;
    U8 ii, backup_rank;

    vSetPHY2ChannelMapping(p, CHANNEL_A);
    backup_rank = u1GetRank(p);

        //Enable RX_FIFO macro DIV4 clock CG
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CG_CTRL1), 0xffffffff, MISC_CG_CTRL1_R_DVS_DIV4_CG_CTRL);

        //DVS mode to RG mode
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2),0x0, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2),0x0, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1);

        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R1_B0_RXDVS2),0x0, R1_B0_RXDVS2_R_RK1_DVS_MODE_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R1_B1_RXDVS2),0x0, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1);

        //Tracking lead/lag counter >> Rx DLY adjustment fixed to 1 (Olympus added)
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_RXDVS0),0x0, B0_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_RXDVS0),0x0, B1_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B1);

        //DQIEN pre-state option to block update for RX ASVA  1-2
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_RXDVS0),0x1, B0_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_RXDVS0),0x1, B1_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B1);

        //Turn off F_DLY individual calibration option (CTO_AGENT_RDAT cannot separate DR/DF error)
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2),0x0, R0_B0_RXDVS2_R_RK0_DVS_FDLY_MODE_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2),0x0, R0_B1_RXDVS2_R_RK0_DVS_FDLY_MODE_B1);

        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R1_B0_RXDVS2),0x0, R1_B0_RXDVS2_R_RK1_DVS_FDLY_MODE_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R1_B1_RXDVS2),0x0, R1_B1_RXDVS2_R_RK1_DVS_FDLY_MODE_B1);

        for(ii=RANK_0; ii<RANK_MAX; ii++)
        {
            vSetRank(p, ii);

            //DQ/DQM/DQS DLY MAX/MIN value under Tracking mode
            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS3), P_Fld(0x0, R0_B0_RXDVS3_RG_RK0_ARDQ0_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS3_RG_RK0_ARDQ0_MAX_DLY_B0));
            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS3), P_Fld(0x0, R0_B0_RXDVS3_RG_RK0_ARDQ1_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS3_RG_RK0_ARDQ1_MAX_DLY_B0));
            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS4), P_Fld(0x0, R0_B0_RXDVS4_RG_RK0_ARDQ2_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS4_RG_RK0_ARDQ2_MAX_DLY_B0));
            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS4), P_Fld(0x0, R0_B0_RXDVS4_RG_RK0_ARDQ3_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS4_RG_RK0_ARDQ3_MAX_DLY_B0));
            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS5), P_Fld(0x0, R0_B0_RXDVS5_RG_RK0_ARDQ4_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS5_RG_RK0_ARDQ4_MAX_DLY_B0));
            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS5), P_Fld(0x0, R0_B0_RXDVS5_RG_RK0_ARDQ5_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS5_RG_RK0_ARDQ5_MAX_DLY_B0));
            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS6), P_Fld(0x0, R0_B0_RXDVS6_RG_RK0_ARDQ6_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS6_RG_RK0_ARDQ6_MAX_DLY_B0));
            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS6), P_Fld(0x0, R0_B0_RXDVS6_RG_RK0_ARDQ7_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS6_RG_RK0_ARDQ7_MAX_DLY_B0));

            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS3), P_Fld(0x0, R0_B1_RXDVS3_RG_RK0_ARDQ0_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS3_RG_RK0_ARDQ0_MAX_DLY_B1));
            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS3), P_Fld(0x0, R0_B1_RXDVS3_RG_RK0_ARDQ1_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS3_RG_RK0_ARDQ1_MAX_DLY_B1));
            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS4), P_Fld(0x0, R0_B1_RXDVS4_RG_RK0_ARDQ2_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS4_RG_RK0_ARDQ2_MAX_DLY_B1));
            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS4), P_Fld(0x0, R0_B1_RXDVS4_RG_RK0_ARDQ3_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS4_RG_RK0_ARDQ3_MAX_DLY_B1));
            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS5), P_Fld(0x0, R0_B1_RXDVS5_RG_RK0_ARDQ4_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS5_RG_RK0_ARDQ4_MAX_DLY_B1));
            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS5), P_Fld(0x0, R0_B1_RXDVS5_RG_RK0_ARDQ5_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS5_RG_RK0_ARDQ5_MAX_DLY_B1));
            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS6), P_Fld(0x0, R0_B1_RXDVS6_RG_RK0_ARDQ6_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS6_RG_RK0_ARDQ6_MAX_DLY_B1));
            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS6), P_Fld(0x0, R0_B1_RXDVS6_RG_RK0_ARDQ7_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS6_RG_RK0_ARDQ7_MAX_DLY_B1));

            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS7), P_Fld(0x0, R0_B0_RXDVS7_RG_RK0_ARDQM0_MIN_DLY_B0) | P_Fld(0x0, R0_B0_RXDVS7_RG_RK0_ARDQS0_MIN_DLY_B0));
            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS7), P_Fld(0x0, R0_B1_RXDVS7_RG_RK0_ARDQM0_MIN_DLY_B1) | P_Fld(0x0, R0_B1_RXDVS7_RG_RK0_ARDQS0_MIN_DLY_B1));

            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS7), P_Fld(0x3f, R0_B0_RXDVS7_RG_RK0_ARDQM0_MAX_DLY_B0) | P_Fld(0x7f, R0_B0_RXDVS7_RG_RK0_ARDQS0_MAX_DLY_B0));
            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS7), P_Fld(0x3f, R0_B1_RXDVS7_RG_RK0_ARDQM0_MAX_DLY_B1) | P_Fld(0x7f, R0_B1_RXDVS7_RG_RK0_ARDQS0_MAX_DLY_B1));

            //Threshold for LEAD/LAG filter
            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS1), P_Fld(0x2, R0_B0_RXDVS1_R_RK0_B0_DVS_TH_LEAD) | P_Fld(0x2, R0_B0_RXDVS1_R_RK0_B0_DVS_TH_LAG));
            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS1), P_Fld(0x2, R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LEAD) | P_Fld(0x2, R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LAG));

            //DQ/DQS Rx DLY adjustment for tracking mode
            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS1), P_Fld(0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_DQ_SCALE_B0) | P_Fld(0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_DQS_SCALE_B0));
            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS1), P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_DQ_SCALE_B1) | P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_DQS_SCALE_B1));

            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS1), P_Fld(0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_DQ_SCALE_B0) | P_Fld(0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_DQS_SCALE_B0));
            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS1), P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_DQ_SCALE_B1) | P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_DQS_SCALE_B1));
        }
        vSetRank(p, backup_rank);

        //Monitor window size setting
        //DDRPHY.SHU*_B*_DQ5.RG_RX_ARDQS0_DVS_DLY_B* (suggested value from A-PHY owner)
//WHITNEY_TO_BE_PORTING
#if (fcFOR_CHIP_ID == fcWhitney)
        //Olympus   Speed   Voltage     DVS_DLY
        //======================================
        //SHU1      3200    0.8V        3
        //SHU2      2667    0.8V-0.7V   4
        //SHU3      1600    0.7V-0.65V  5
        if (p->frequency == 1600)
        {
            vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ5),0x3, SHU1_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0);
            vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ5),0x3, SHU1_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1);
        }
        else if (p->frequency == 1333)
        {
            vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ5),0x4, SHU1_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0);
            vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ5),0x4, SHU1_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1);
        }
        else
        {
            vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ5),0x5, SHU1_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0);
            vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ5),0x5, SHU1_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1);
        }
#endif


    vSetPHY2ChannelMapping(p, channel_bak);
}

void DramcPrintRxInputDlyTrackingFlag(DRAMC_CTX_T *p)
{
    UINT8 dqsDly=0;
    UINT8 u1ByteIdx=0;
    U8 ii, backup_rank;

    backup_rank = u1GetRank(p);

    for(ii=RANK_0; ii<RANK_MAX; ii++)
    {
        vSetRank(p, ii);

        for(u1ByteIdx = 0; u1ByteIdx < (p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
        {
            INT8 u1BitIdx=0;

            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_RXDVS0), (ii<<1)|(u1ByteIdx),MISC_RXDVS0_R_RX_DLY_TRACK_RO_SEL);
            mcSHOW_DBG_MSG(("  Rank%d Byte%d ro_sel=%d lead count bit(7~0) = ( ", ii, u1ByteIdx, (ii<<1)|(u1ByteIdx)));
            for(u1BitIdx=7; u1BitIdx>=0; u1BitIdx--)
            {
                mcSHOW_DBG_MSG(("%H ", u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_DQ_RXDLY_TRRO0 + (u1BitIdx<<2)), MISC_DQ_RXDLY_TRRO0_DVS_RKX_BX_SW_LEAD_CNT_OUT_B0)));
            }
            mcSHOW_DBG_MSG((")\n"));
        }
    }
    vSetRank(p, backup_rank);

    for(ii=RANK_0; ii<RANK_MAX; ii++)
    {
        vSetRank(p, ii);

        for(u1ByteIdx = 0; u1ByteIdx < (p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
        {
            INT8 u1BitIdx=0;

            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_RXDVS0), (ii<<1)|(u1ByteIdx),MISC_RXDVS0_R_RX_DLY_TRACK_RO_SEL);
            mcSHOW_DBG_MSG(("  Rank%d Byte%d ro_sel=%d lag count bit(7~0) =  ( ", ii, u1ByteIdx, (ii<<1)|(u1ByteIdx>>1)));
            for(u1BitIdx=7; u1BitIdx>=0; u1BitIdx--)
            {
                mcSHOW_DBG_MSG(("%H ", u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_DQ_RXDLY_TRRO0 + (u1BitIdx<<2)), MISC_DQ_RXDLY_TRRO0_DVS_RKX_BX_SW_LAG_CNT_OUT_B0)));
            }
            mcSHOW_DBG_MSG((")\n"));
        }
    }
    vSetRank(p, backup_rank);

    for(ii=RANK_0; ii<RANK_MAX; ii++)
    {
        vSetRank(p, ii);

        for(u1ByteIdx = 0; u1ByteIdx < (p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
        {
            INT8 u1BitIdx=0;

            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_RXDVS0), (ii<<1)|(u1ByteIdx),MISC_RXDVS0_R_RX_DLY_TRACK_RO_SEL);
            mcSHOW_DBG_MSG(("  Rank%d Byte%d ro_sel=%d both count bit(7~0) =  ( ", ii, u1ByteIdx, (ii<<1)|(u1ByteIdx)));
            for(u1BitIdx=7; u1BitIdx>=0; u1BitIdx-=2)
            {
                mcSHOW_DBG_MSG(("%H ", u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_DQ_RXDLY_TRRO9 + (u1BitIdx/2)*4), MISC_DQ_RXDLY_TRRO9_DVS_RKX_BX_LEAD_LAG_CNT_OUT_B1)));
                mcSHOW_DBG_MSG(("%H ", u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_DQ_RXDLY_TRRO9 + (u1BitIdx/2)*4), MISC_DQ_RXDLY_TRRO9_DVS_RKX_BX_LEAD_LAG_CNT_OUT_B0)));
            }
            mcSHOW_DBG_MSG((")\n"));
        }
    }
    vSetRank(p, backup_rank);

    for(ii=RANK_0; ii<RANK_MAX; ii++)
    {
        vSetRank(p, ii);

        for(u1ByteIdx = 0; u1ByteIdx < (p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
        {
            INT8 u1BitIdx=0;

            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_RXDVS0), (ii<<1)|(u1ByteIdx>>1),MISC_RXDVS0_R_RX_DLY_TRACK_RO_SEL);
            mcSHOW_DBG_MSG(("  Rank%d Byte%d ro_sel=%d th_count_out bit(7~0) =  ( ", ii, u1ByteIdx, (ii<<1)|(u1ByteIdx>>1)));
            for(u1BitIdx=7; u1BitIdx>=0; u1BitIdx-=2)
            {
                mcSHOW_DBG_MSG(("%H ", u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_DQ_RXDLY_TRRO16 + (u1BitIdx/2)*4), MISC_DQ_RXDLY_TRRO16_DVS_RKX_BX_TH_CNT_OUT_B1)));
                mcSHOW_DBG_MSG(("%H ", u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_DQ_RXDLY_TRRO16 + (u1BitIdx/2)*4), MISC_DQ_RXDLY_TRRO16_DVS_RKX_BX_TH_CNT_OUT_B0)));
            }
            mcSHOW_DBG_MSG((")\n"));
        }
    }
    vSetRank(p, backup_rank);

    vSetPHY2ChannelMapping(p, CHANNEL_A);

}

void DramcPrintRxInputDly(DRAMC_CTX_T *p)
{
    DRAM_CHANNEL_T channel_bak = p->channel;
    UINT8 dqsDly=0;
    UINT8 u1ByteIdx=0;
    U8 ii, backup_rank;

    vSetPHY2ChannelMapping(p, CHANNEL_A);
    backup_rank = u1GetRank(p);

    mcSHOW_DBG_MSG(("RK%d DQS0 R(%d, %d) F(%d, %d)\n", u1GetRank(p),
        u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6), SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0),
        u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ6), SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1),
        u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6), SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0),
        u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ6), SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)));

    for(ii=RANK_0; ii<RANK_MAX; ii++)
    {
        vSetRank(p, ii);
        for(u1ByteIdx = 0; u1ByteIdx < (p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
        {
            U32 DQ_DLY[4];
            DQ_DLY[0] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ2 + u1ByteIdx*0x50));
            DQ_DLY[1] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ3 + u1ByteIdx*0x50));
            DQ_DLY[2] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ4 + u1ByteIdx*0x50));
            DQ_DLY[3] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ5 + u1ByteIdx*0x50));
            mcSHOW_DBG_MSG(("RK%d B%d R(7~0) (%d, %d, %d, %d, %d, %d, %d, %d)\n", u1GetRank(p), u1ByteIdx,
                (DQ_DLY[3]>>8)&0xff, (DQ_DLY[3]>>24)&0xff, (DQ_DLY[2]>>8)&0xff, (DQ_DLY[2]>>24)&0xff,
                (DQ_DLY[1]>>8)&0xff, (DQ_DLY[1]>>24)&0xff, (DQ_DLY[0]>>8)&0xff, (DQ_DLY[0]>>24)&0xff));
            mcSHOW_DBG_MSG(("RK%d B%d F(7~0) (%d, %d, %d, %d, %d, %d, %d, %d)\n", u1GetRank(p), u1ByteIdx,
                (DQ_DLY[3]>>0)&0xff, (DQ_DLY[3]>>16)&0xff, (DQ_DLY[2]>>0)&0xff, (DQ_DLY[2]>>16)&0xff,
                (DQ_DLY[1]>>0)&0xff, (DQ_DLY[1]>>16)&0xff, (DQ_DLY[0]>>0)&0xff, (DQ_DLY[0]>>16)&0xff));
        }
    }
    vSetRank(p, backup_rank);
    vSetPHY2ChannelMapping(p, channel_bak);
}

/***
1. set DQS*_DVS_DLY=4b'1xxxx, then log calibration result
2. move DQS delay from 0 to N, then check lead_count and lag_count result
3. move DQ delay from 0 to N, then check lead_count and lag_count result
***/
void DramcRxInputDlyTrackingSW(DRAMC_CTX_T *p)
{
#ifndef OLYMPUS_TO_BE_PORTING

    DRAM_CHANNEL_T channel_bak = p->channel;
    UINT8 dqsDly=0, dqDly=0;
    UINT8 u1ByteIdx=0, u1BitIdx=0;
    UINT8 updateDone=0;

    vSetPHY2ChannelMapping(p, CHANNEL_A);

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_RXDLY_TR1),0x1, RXDLY_TR1_R_RK0_DVS_MODE);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_RXDLY_TR11),0x1, RXDLY_TR11_R_RK1_DVS_MODE);

    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_EYE2),0xF, EYE2_RG_RX_ARDQS0_DVS_DLY);
    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_EYEB1_2),0xF, EYEB1_2_RG_RX_ARDQS1_DVS_DLY);

    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_RXDLY_TR7),0x1, RXDLY_TR7_R_RK0_B0_DVS_SW_CNT_ENA);
    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_RXDLY_TR9),0x1, RXDLY_TR9_R_RK0_B1_DVS_SW_CNT_ENA);
    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_RXDLY_TR17),0x1, RXDLY_TR17_R_RK1_B0_DVS_SW_CNT_ENA);
    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_RXDLY_TR19),0x1, RXDLY_TR19_R_RK1_B1_DVS_SW_CNT_ENA);

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_RXDLY_TR0),0x1, RXDLY_TR0_R_RX_DLY_RK_OPT);

#if 1//adjust DQS delay and monitor Lead/Lag count
    for(dqsDly=0; dqsDly<40; dqsDly++)
    {
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_RXDQ5), dqsDly, RXDQ5_RK0_RX_ARDQS0_R_DLY);
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_RXDQ5), dqsDly, RXDQ5_RK0_RX_ARDQS0_F_DLY);
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_RXB1_5), dqsDly, RXB1_5_RK0_RX_ARDQS1_R_DLY);
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_RXB1_5), dqsDly, RXB1_5_RK0_RX_ARDQS1_F_DLY);

        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_RXDQ10), dqsDly, RXDQ10_RK1_RX_ARDQS0_R_DLY);
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_RXDQ10), dqsDly, RXDQ10_RK1_RX_ARDQS0_F_DLY);
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_RXRK1_B1_4), dqsDly, RXRK1_B1_4_RK1_RX_ARDQS1_R_DLY);
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_RXRK1_B1_4), dqsDly, RXRK1_B1_4_RK1_RX_ARDQS1_R_DLY);

        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_RXDLY_TR0), 1, RXDLY_TR0_R_DVS_SW_UP);
        while(!updateDone)
        {
            mcSHOW_DBG_MSG(("wait for DVS_SW_UP DONE=%H\n", updateDone));
            updateDone = u4IO32ReadFldAlign(DDRPHY_RXDLY_TRRO14+(aru1PhyMap2Channel[0]<< POS_BANK_NUM), RXDLY_TRRO14_DVS_RK0_B0_SW_UP_DONE); updateDone<<1;
            updateDone &= u4IO32ReadFldAlign(DDRPHY_RXDLY_TRRO14+(aru1PhyMap2Channel[0]<< POS_BANK_NUM), RXDLY_TRRO14_DVS_RK0_B1_SW_UP_DONE);updateDone<<1;
            updateDone &= u4IO32ReadFldAlign(DDRPHY_RXDLY_TRRO14+(aru1PhyMap2Channel[1]<< POS_BANK_NUM), RXDLY_TRRO14_DVS_RK0_B0_SW_UP_DONE);updateDone<<1;
            updateDone &= u4IO32ReadFldAlign(DDRPHY_RXDLY_TRRO14+(aru1PhyMap2Channel[1]<< POS_BANK_NUM), RXDLY_TRRO14_DVS_RK0_B1_SW_UP_DONE);updateDone<<1;

            updateDone &= u4IO32ReadFldAlign(DDRPHY_RXDLY_TRRO14+(aru1PhyMap2Channel[0]<< POS_BANK_NUM), RXDLY_TRRO14_DVS_RK1_B0_SW_UP_DONE);updateDone<<1;
            updateDone &= u4IO32ReadFldAlign(DDRPHY_RXDLY_TRRO14+(aru1PhyMap2Channel[0]<< POS_BANK_NUM), RXDLY_TRRO14_DVS_RK1_B1_SW_UP_DONE);updateDone<<1;
            updateDone &= u4IO32ReadFldAlign(DDRPHY_RXDLY_TRRO14+(aru1PhyMap2Channel[1]<< POS_BANK_NUM), RXDLY_TRRO14_DVS_RK1_B0_SW_UP_DONE);updateDone<<1;
            updateDone &= u4IO32ReadFldAlign(DDRPHY_RXDLY_TRRO14+(aru1PhyMap2Channel[1]<< POS_BANK_NUM), RXDLY_TRRO14_DVS_RK1_B1_SW_UP_DONE);
        }
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_RXDLY_TR0), 0, RXDLY_TR0_R_DVS_SW_UP);
        mcSHOW_DBG_MSG(("DQS Delay=%d update done\n", dqsDly));

        DramPhyReset(p);

        DramcDFSTriggerDMA();
        while(*((volatile unsigned int *)(0x10212c08))) ;
        DramcPrintRxInputDlyTrackingFlag(p);

    }
#elif 0//ajdust DQ delay and monitor Lead/Lag count
    for(u1BitIdx=0; u1BitIdx<8; u1BitIdx++)
    {
        for(dqDly=12; dqDly<45; dqDly++)
        {
            if(u1BitIdx%2==0)
            {
                //RK0
                vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_RXDQ1+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ0_R_DLY);
                vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_RXDQ1+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ0_R_DLY);
                vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_RXDQ1+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ0_R_DLY);
                vIO32WriteFldAlign_Phy_Byte(3, DRAMC_REG_ADDR(DDRPHY_RXDQ1+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ0_R_DLY);
                vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_RXDQ1+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ0_F_DLY);
                vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_RXDQ1+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ0_F_DLY);
                vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_RXDQ1+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ0_F_DLY);
                vIO32WriteFldAlign_Phy_Byte(3, DRAMC_REG_ADDR(DDRPHY_RXDQ1+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ0_F_DLY);
                //RK1
                vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_RXDQ6+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ0_R_DLY);
                vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_RXDQ6+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ0_R_DLY);
                vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_RXDQ6+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ0_R_DLY);
                vIO32WriteFldAlign_Phy_Byte(3, DRAMC_REG_ADDR(DDRPHY_RXDQ6+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ0_R_DLY);
                vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_RXDQ6+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ0_F_DLY);
                vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_RXDQ6+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ0_F_DLY);
                vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_RXDQ6+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ0_F_DLY);
                vIO32WriteFldAlign_Phy_Byte(3, DRAMC_REG_ADDR(DDRPHY_RXDQ6+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ0_F_DLY);

            }
            else
            {
                //RK0
                vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_RXDQ1+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ1_R_DLY);
                vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_RXDQ1+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ1_R_DLY);
                vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_RXDQ1+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ1_R_DLY);
                vIO32WriteFldAlign_Phy_Byte(3, DRAMC_REG_ADDR(DDRPHY_RXDQ1+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ1_R_DLY);
                vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_RXDQ1+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ1_F_DLY);
                vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_RXDQ1+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ1_F_DLY);
                vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_RXDQ1+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ1_F_DLY);
                vIO32WriteFldAlign_Phy_Byte(3, DRAMC_REG_ADDR(DDRPHY_RXDQ1+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ1_F_DLY);
                //RK1
                vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_RXDQ6+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ1_R_DLY);
                vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_RXDQ6+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ1_R_DLY);
                vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_RXDQ6+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ1_R_DLY);
                vIO32WriteFldAlign_Phy_Byte(3, DRAMC_REG_ADDR(DDRPHY_RXDQ6+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ1_R_DLY);
                vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_RXDQ6+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ1_F_DLY);
                vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_RXDQ6+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ1_F_DLY);
                vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_RXDQ6+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ1_F_DLY);
                vIO32WriteFldAlign_Phy_Byte(3, DRAMC_REG_ADDR(DDRPHY_RXDQ6+(u1BitIdx/2)*4), dqDly, RXDQ1_RK0_RX_ARDQ1_F_DLY);
            }

            vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_RXDLY_TR0), 1, RXDLY_TR0_R_DVS_SW_UP);
            while(!updateDone)
            {
                mcSHOW_DBG_MSG(("wait for DVS_SW_UP DONE=%H\n", updateDone));
                updateDone = u4IO32ReadFldAlign(DDRPHY_RXDLY_TRRO14+(aru1PhyMap2Channel[0]<< POS_BANK_NUM), RXDLY_TRRO14_DVS_RK0_B0_SW_UP_DONE); updateDone<<1;
                updateDone &= u4IO32ReadFldAlign(DDRPHY_RXDLY_TRRO14+(aru1PhyMap2Channel[0]<< POS_BANK_NUM), RXDLY_TRRO14_DVS_RK0_B1_SW_UP_DONE);updateDone<<1;
                updateDone &= u4IO32ReadFldAlign(DDRPHY_RXDLY_TRRO14+(aru1PhyMap2Channel[1]<< POS_BANK_NUM), RXDLY_TRRO14_DVS_RK0_B0_SW_UP_DONE);updateDone<<1;
                updateDone &= u4IO32ReadFldAlign(DDRPHY_RXDLY_TRRO14+(aru1PhyMap2Channel[1]<< POS_BANK_NUM), RXDLY_TRRO14_DVS_RK0_B1_SW_UP_DONE);updateDone<<1;

                updateDone &= u4IO32ReadFldAlign(DDRPHY_RXDLY_TRRO14+(aru1PhyMap2Channel[0]<< POS_BANK_NUM), RXDLY_TRRO14_DVS_RK1_B0_SW_UP_DONE);updateDone<<1;
                updateDone &= u4IO32ReadFldAlign(DDRPHY_RXDLY_TRRO14+(aru1PhyMap2Channel[0]<< POS_BANK_NUM), RXDLY_TRRO14_DVS_RK1_B1_SW_UP_DONE);updateDone<<1;
                updateDone &= u4IO32ReadFldAlign(DDRPHY_RXDLY_TRRO14+(aru1PhyMap2Channel[1]<< POS_BANK_NUM), RXDLY_TRRO14_DVS_RK1_B0_SW_UP_DONE);updateDone<<1;
                updateDone &= u4IO32ReadFldAlign(DDRPHY_RXDLY_TRRO14+(aru1PhyMap2Channel[1]<< POS_BANK_NUM), RXDLY_TRRO14_DVS_RK1_B1_SW_UP_DONE);
            }
            vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_RXDLY_TR0), 0, RXDLY_TR0_R_DVS_SW_UP);
            mcSHOW_DBG_MSG(("DQ%d Delay=%d update done\n", u1BitIdx, dqDly));

            DramPhyReset(p);

            DramcDFSTriggerDMA();
            while(*((volatile unsigned int *)(0x10212c08))) ;
            DramcPrintRxInputDlyTrackingFlag(p);

        }
    }
#endif

    vSetPHY2ChannelMapping(p, channel_bak);
#endif
}

/***
1. set DQS*_DVS_DLY=4b'1xxxx, then log calibration result
LAG side:
2. change V/T and check lead/lag count
   ex. V from 1000->900mv (lag will be detected)
3. DFS (directly jump to itself) and check Rx dly
LEAD side
4. manually increase DQ_DLY(+10) after cali
5. when enabling HW tracking, lead will be detected.
6. DFS (directly jump to itself) and check Rx dly
***/
U32 testCnt=0;
void DramcRxInputDlyTrackingHW(DRAMC_CTX_T *p)
{
    DRAM_CHANNEL_T channel_bak = p->channel;
    UINT8 updateDone=0;
    U16 u2DVS_TH=0x0;
    U16 u2MinDly=0x14;
    U16 u2MaxDly=0x30;
    U8 ii, backup_rank;

    vSetPHY2ChannelMapping(p, CHANNEL_A);
    backup_rank = u1GetRank(p);

    //Rx DLY tracking setting
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_RXDVS0), 0x1, B0_RXDVS0_R_RX_DLY_TRACK_SPM_CTRL_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_RXDVS0), 0x1, B1_RXDVS0_R_RX_DLY_TRACK_SPM_CTRL_B1);

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_RXDVS0), 0x0, B0_RXDVS0_R_RX_RANKINCTL_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_RXDVS0), 0x0, B1_RXDVS0_R_RX_RANKINCTL_B1);

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_RXDVS0), 0x1, B0_RXDVS0_R_RX_RANKINSEL_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_RXDVS0), 0x1, B1_RXDVS0_R_RX_RANKINSEL_B1);

    //Enable A-PHY DVS LEAD/LAG
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 0x1, B0_DQ5_RG_RX_ARDQS0_DVS_EN_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 0x1, B1_DQ5_RG_RX_ARDQS0_DVS_EN_B1);

    //Rx DLY tracking function CG enable
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_RXDVS0), 0x1, B0_RXDVS0_R_RX_DLY_TRACK_CG_EN_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_RXDVS0), 0x1, B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1);

    //Rx DLY tracking lead/lag counter enable
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_RXDVS0), 0x1, B0_RXDVS0_R_RX_DLY_TRACK_ENA_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_RXDVS0), 0x1, B1_RXDVS0_R_RX_DLY_TRACK_ENA_B1);

    for(ii=RANK_0; ii<RANK_MAX; ii++)
    {
        vSetRank(p, ii);

        //Rx DLY tracking update enable
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), 0x2, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), 0x2, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1);

        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), 0x1, R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), 0x1, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1);

        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), 0x1, R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), 0x1, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1);
    }
    vSetRank(p, backup_rank);

#if 0
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_RXDVS0),0x1, B0_RXDVS0_R_RX_DLY_RK_OPT_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_RXDVS0),0x1, B1_RXDVS0_R_RX_DLY_RK_OPT_B1);

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 1, MISC_CTRL1_R_HWSAVE_MODE_ENA);
#endif


        #if DUAL_FREQ_K
        DramcDFSDirectJump(p, 0, 0, 0);
        #endif

        DramcPrintRxInputDly(p);
        DramcPrintRxInputDlyTrackingFlag(p);


    vSetPHY2ChannelMapping(p, channel_bak);
}

#endif

void vGetImpedanceResult(DRAMC_CTX_T *p, U8 *drvp, U8* drvn)
{
    *drvp = (U8)u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ1), SHU1_B0_DQ1_RG_TX_ARDQ_DRVP_B0);
    *drvn = (U8)u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ1), SHU1_B0_DQ1_RG_TX_ARDQ_DRVN_B0);
}


#if ENABLE_DDRPHY_FREQ_METER
/*
1. Select meter clock input: CLK_DBG_CFG[1:0] = 0x1
2. Select clock source from below table: CLK_DBG_CFG[13:8]
3. Setup meter div: CLK_MISC_CFG_0[31:24] = 0x0
4. Enable frequency meter: CLK26CALI_0[12] = 0x1
5. Trigger frequency meter: CLK26CALI_0[4] = 0x1
6. Wait until CLK26CALI_0[4] = 0x0
7. Read meter count: CLK26CALI_1[15:0]
8. Calculate measured frequency: freq. = (26 * cal_cnt) / 1024
CLK_DBG_CFG = 0x1000010c
CLK_MISC_CFG_0 = 0x10000104
CLK26CALI_0 = 0x10000220
CLK26CALI_1 = 0x10000224
*/

/*
1. Select fqmeter input to ckgen_dbgmux. CLK_CFG_20[14] = 0 or 1 (0: abist_clk_01, 1: abist_clk_02)(addr: 0x10210210)
2. Select meter clock input. CLK_CFG_20[13:8] for abist_clk_01 (addr:  0x10210210)
                                             CLK_CFG_20[12:8] for abist_clk_02
3. setup meter div. CLK_MISC_CFG_1[7:0] = 0 (addr: 0x10210414)
4. Enable freq meter. CLK26CALI_0[7] = 1(addr: 0x10210520)
5. Trigger freq meter. CLK26CALI_0[0] = 1(addr: 0x10210520)
6. Read meter count CLK26CALI_1[15:0](addr: 0x10210524)
7. Calculate measured freq. freq = (26 * cal_cnt) / 1024;

NOTE
MEMPLL: abist_meter=abist_clk_01, abist_src=abist_clk39 -> CLK_CFG_20[14]=0, CLK_CFG_20[13:8]=39

DE: Mas Lin
*/
void DDRPhyFreqMeter()
{
    U32 u4CLK_CFG_20 = *((volatile unsigned int *)(0x10210210));
    U32 u4CLK_CFG_21 = *((volatile unsigned int *)(0x10210214));
    U32 u4CLK_CFG_11 = *((volatile unsigned int *)(0x102101B0));
    U32 u4CLK26CALI_0 = *((volatile unsigned int *)(0x10210520));
    U8  u1CLKMeterSel;	//CLK_CFG_20[14]
    U8  u1CLKMuxSel;	//CLK_CFG_20[13:8] or CLK_CFG_20[12:8]
    U32 u4CalCnt;
    U32 u4Freq;

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)

    mcSHOW_DBG_MSG(("DDRPhyFreqMeter...\n"));

    //AD_MPLL_208M_CK
    u1CLKMeterSel = 0x1;
    u1CLKMuxSel = 39;
    *((volatile unsigned int *)(0x10210210)) = (u4CLK_CFG_20 & 0xFFFF80FF) | (0x01 << 31) | (u1CLKMuxSel << 8);
    *((volatile unsigned int *)(0x10210414)) = 0x0;
    *((volatile unsigned int *)(0x10210520)) = 0x00000081;
    while((*((volatile unsigned int *)(0x10210520)) & 0x00000001)!=0) { mcDELAY_US(1); }
    u4CalCnt = *((volatile unsigned int *)(0x10210524)) & 0x0000FFFF;
    u4Freq = (u4CalCnt * 26) / 1024;
    mcSHOW_DBG_MSG(("AD_MPLL_CK FREQUENCY=%d\n", u4Freq));

    u1CLKMeterSel = 0x0;
    u1CLKMuxSel = 46;
    *((volatile unsigned int *)(0x102101b0)) = (u4CLK_CFG_11 & 0xFFFFF0FF) | (0x2 << 8);
    *((volatile unsigned int *)(0x10210214)) = (u4CLK_CFG_21 & 0x7F80FFFF) | (u1CLKMeterSel << 22) | (u1CLKMuxSel << 16);
    *((volatile unsigned int *)(0x10210414)) = 0x0;
    *((volatile unsigned int *)(0x10210520)) = 0x00000090;
    while((*((volatile unsigned int *)(0x10210520)) & 0x00000010)!=0) { mcDELAY_US(1); }
    u4CalCnt = *((volatile unsigned int *)(0x10210528)) & 0x0000FFFF;
    u4Freq = u4CalCnt*26/1024;
    mcSHOW_DBG_MSG(("CH-A_hf_femi_ck FREQUENCY=%d\n", u4Freq));

    #if 0
    *((volatile unsigned int *)(0x102101b0)) = (u4CLK_CFG_11 & 0xFFFFF0FF) | (0x3 << 8);
    *((volatile unsigned int *)(0x10210214)) = (u4CLK_CFG_21 & 0x7F80FFFF) | (u1CLKMeterSel << 22) | (u1CLKMuxSel << 16);
    *((volatile unsigned int *)(0x10210414)) = 0x0;
    *((volatile unsigned int *)(0x10210520)) = 0x00000090;
    while((*((volatile unsigned int *)(0x10210520)) & 0x00000010)!=0) { mcDELAY_US(1); }
    u4CalCnt = *((volatile unsigned int *)(0x10210528)) & 0x0000FFFF;
    u4Freq = u4CalCnt*26/1024;
    mcSHOW_DBG_MSG(("CH-C hf_femi_ck FREQUENCY=%d\n", u4Freq));
    #endif
#endif

}
#endif



#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#if __ETT__
void GetPhyPllFrequency(DRAMC_CTX_T *p)
{
#if 1
    U8 u1ShuLevel = u4IO32ReadFldAlign(DRAMC_REG_SHUSTATUS, SHUSTATUS_SHUFFLE_LEVEL);
    U32 u4PLL5_ADDR = DDRPHY_SHU1_PLL5+0x500*u1ShuLevel;
    U32 u4PLL8_ADDR = DDRPHY_SHU1_PLL8+0x500*u1ShuLevel;
    U32 u4CA_CMD6 = DDRPHY_SHU1_CA_CMD6+0x500*u1ShuLevel;

    U32 u4SDM_PCW = u4IO32ReadFldAlign(u4PLL5_ADDR, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
    U32 u4PREDIV = u4IO32ReadFldAlign(u4PLL8_ADDR, SHU1_PLL8_RG_RPHYPLL_PREDIV);
    U32 u4POSDIV = u4IO32ReadFldAlign(u4PLL8_ADDR, SHU1_PLL8_RG_RPHYPLL_POSDIV);
    U32 u4CKDIV4 = u4IO32ReadFldAlign(u4CA_CMD6, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA);

    U32 u4VCOFreq = ((52>>u4PREDIV)*(u4SDM_PCW>>8))>>u4POSDIV;
    U32 u4DataRate = u4VCOFreq>>u4CKDIV4;
#else
    U8 u1ShuLevel = (*((volatile unsigned int *)(0x10006474))&0x000000c0)>>6;
    U32 u4PLL5_ADDR = 0x10228d94+0x500*u1ShuLevel;
    U32 u4PLL8_ADDR = 0x10228da0+0x500*u1ShuLevel;
    U32 u4CA_CMD6   = 0x10228d18+0x500*u1ShuLevel;

    U32 u4SDM_PCW = *((volatile unsigned int *)(u4PLL5_ADDR))>>16;
    U32 u4PREDIV = (*((volatile unsigned int *)(u4PLL8_ADDR))&0x000c0000)>>18;
    U32 u4POSDIV = (*((volatile unsigned int *)(u4PLL8_ADDR))&0x00000007);
    U32 u4CKDIV4 = (*((volatile unsigned int *)(u4CA_CMD6))&0x08000000)>>27;


    U32 u4VCOFreq = ((52>>u4PREDIV)*(u4SDM_PCW>>8))>>u4POSDIV;
    U32 u4DataRate = u4VCOFreq>>u4CKDIV4;
#endif
    mcSHOW_DBG_MSG(("PCW=0x%X, u4PREDIV=%d, u4POSDIV=%d, CKDIV4=%d, DataRate=%d\n", u4SDM_PCW, u4PREDIV, u4POSDIV, u4CKDIV4, u4DataRate));
}
#endif
#endif

///TODO: EVEREST_PORTING_TODO
#if 0 //#ifdef LOOPBACK_TEST
#define DDRPHY0AO_BASE          0x10228000
#define DDRPHY1AO_BASE          0x10230000
#define DRAMC0_AO_BASE          0x1022a000
#define DRAMC1_AO_BASE          0x10232000
#ifdef LPBK_INTERNAL_EN
void ddrphy_int_lpbk_test_seq (DRAMC_CTX_T *p)
{
    int           lpbk_test_result = 0 ;
    unsigned int  reg_cha_r_data = 0 ;
    unsigned int  reg_chb_r_data = 0 ;
    int           DDRPHY_TX_BASE = 0x0 ;
    int           DDRPHY_RX_BASE = 0x0 ;

    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    /*TINFO="[LPBK] Starting to DDRPHY internal loopback test ..."*/

    //*GPIO_DOUT1 = 0x00000100; //! EINT8 - trigger lpbk test

    //! ==============================
    //! - TX setting
    //! ==============================
    //! [21:16]: R_EXTLB_OE_*_ON
    *((UINT32P)(DDRPHY0AO_BASE + 0x0240)) = 0x003fb401;

    //! [5]: CA_TX_MODE; [4]:DQ_TX_MODE; [3]: DQ_MODE for CA
    //! [9]: CA_RX_MODE; [8]:DQ_RX_MODE; [3]: DQ_MODE for CA
    *((UINT32P)(DDRPHY0AO_BASE + 0x0248)) = 0x00000338;

    //! ==============================
    //! - APHY setting
    //! ==============================
    //! B0
    *((UINT32P)(DDRPHY0AO_BASE + 0x00a4)) = 0x000005bc;
    *((UINT32P)(DDRPHY0AO_BASE + 0x00b0)) = 0x010311c1;
    //! B1
    *((UINT32P)(DDRPHY0AO_BASE + 0x0124)) = 0x000005bc;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0130)) = 0x010311c1;
    //! CA
    *((UINT32P)(DDRPHY0AO_BASE + 0x01a4)) = 0x000005bc;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01b0)) = 0x010311c1;

    //! ==============================
    //! - LPBK setting
    //! ==============================
    //! step-1 : trigger RX SWRST
    *((UINT32P)(DDRPHY0AO_BASE + 0x0244)) = 0x0000cc83;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0244)) = 0x0000cc8b;

    //! step-2 : release RX SWRST
    *((UINT32P)(DDRPHY0AO_BASE + 0x0244)) = 0x0000cc81;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0244)) = 0x0000cc89;

    /*TINFO="[LPBK] Trigger TX Enable ..."*/

    //! step-3 : trigger LPBK TX_EN
    *((UINT32P)(DDRPHY0AO_BASE + 0x0244)) = 0x0000cc85;

    mcDELAY_US(1);

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

    //DramcRegDump(p);

    //! step-4 : wait LPBK done
    do {
    reg_cha_r_data = *((UINT32P)(DDRPHY0AO_BASE + 0x0580)) ;
    reg_chb_r_data = *((UINT32P)(DDRPHY1AO_BASE + 0x0580)) ;
    /*TINFO="[LPBK] Waiting for CHA RX test done ; current RX status is [%x] ...", reg_cha_r_data*/
    /*TINFO="[LPBK] Waiting for CHB RX test done ; current RX status is [%x] ...", reg_chb_r_data*/
    }while( (reg_cha_r_data != 0x0fffffff) || (reg_chb_r_data != 0x0fffffff) ) ;  //! B0-9bits ; B1-9bits ; CA-10bits


    *((UINT32P)(DDRPHY0AO_BASE + 0x0244)) = 0x0000cc81;
    *((UINT32P)(DDRPHY1AO_BASE + 0x0244)) = 0x0000cc81;

    //*GPIO_DOUT1 = 0x00000300; //! EINT9 - trigger lpbk done

    //! step-5 : check fail
    reg_cha_r_data = *((UINT32P)(DDRPHY0AO_BASE + 0x0584)) ;
    reg_chb_r_data = *((UINT32P)(DDRPHY1AO_BASE + 0x0584)) ;

    mcSHOW_DBG_MSG(("[ddrphy_int_lpbk] CH_A=0x%x\n", reg_cha_r_data));
    mcSHOW_DBG_MSG(("[ddrphy_int_lpbk] CH_B=0x%x\n", reg_chb_r_data));


    if( (reg_cha_r_data != 0) || (reg_chb_r_data != 0) ){
    /*TINFO="[LPBK] DDRPHY loopback test FAIL"*/
    /*TINFO="[LPBK] LPBK CHA fail status = %x", reg_cha_r_data*/
    /*TINFO="[LPBK] LPBK CHB fail status = %x", reg_chb_r_data*/
    lpbk_test_result = 0 ;
    } else {
    /*TINFO="[LPBK] DDRPHY loopback test PASS !!!"*/
    lpbk_test_result = 1 ;
    //*GPIO_DOUT1 = 0x00000700; //! EINT10 - trigger lpbk pass
    }

    //*MDM_TM_ENDSIM = 0x0;
    //while(1);


} //! end of ddrphy_int_lpbk_test_seq
#else
//! ----------------------------------------------
//! ddrphy external loopback test sequence control
//! ----------------------------------------------
void ddrphy_ext_lpbk_test_seq ( DRAMC_CTX_T *p, unsigned int LPBK_TX )
{
  int           lpbk_test_result = 0 ;
  unsigned int  reg_r_data = 0 ;
  int           DDRPHY_TX_BASE = 0x0 ;
  int           DDRPHY_RX_BASE = 0x0 ;

  /*TINFO="[LPBK] Starting to DDRPHY external loopback test ..."*/

  //*GPIO_DOUT1 = 0x00000100; //! EINT8 - trigger lpbk test

  if( LPBK_TX==0 ){
    //! CHN-A is TX
    /*TINFO="[LPBK] CHN-A is TX ; CHN-B is RX ..."*/
    DDRPHY_TX_BASE = DDRPHY0AO_BASE ;
    DDRPHY_RX_BASE = DDRPHY1AO_BASE ;
  } else {
    //! CHN-B is TX
    /*TINFO="[LPBK] CHN-B is TX ; CHN-A is RX ..."*/
    DDRPHY_TX_BASE = DDRPHY1AO_BASE ;
    DDRPHY_RX_BASE = DDRPHY0AO_BASE ;
  }

  //! step-1 : APHY TX/RX setting

  //! ==============================
  //! - TX setting
  //! ==============================
  //! [21:16]: R_EXTLB_OE_*_ON
  *((UINT32P)(DDRPHY_TX_BASE + 0x0240)) = 0x003fb401;

  //! [5]: CA_TX_MODE; [4]:DQ_TX_MODE; [3]: DQ_MODE for CA
  *((UINT32P)(DDRPHY_TX_BASE + 0x0248)) = 0x00000038;

  //! ==============================
  //! - RX setting
  //! ==============================
  //! DDRPHY_GLUE_MISC_CHB
  //! [9]: CA_RX_MODE; [8]:DQ_RX_MODE; [3]: DQ_MODE for CA
  *((UINT32P)(DDRPHY_RX_BASE + 0x0248)) = 0x00000308;

  *((UINT32P)(DDRPHY_RX_BASE + 0x0C14)) = 0x00000010;
  *((UINT32P)(DDRPHY_RX_BASE + 0x0C94)) = 0x00000010;
  *((UINT32P)(DDRPHY_RX_BASE + 0x0D14)) = 0x00000010;
  //! APHY-B0
  *((UINT32P)(DDRPHY_RX_BASE + 0x00A4)) = 0x000005b8;
  *((UINT32P)(DDRPHY_RX_BASE + 0x00AC)) = 0x00010000;
  *((UINT32P)(DDRPHY_RX_BASE + 0x00B0)) = 0x00001041;
  //! APHY-B1
  *((UINT32P)(DDRPHY_RX_BASE + 0x0124)) = 0x000005b8;
  *((UINT32P)(DDRPHY_RX_BASE + 0x012C)) = 0x00010000;
  *((UINT32P)(DDRPHY_RX_BASE + 0x0130)) = 0x00001041;
  //! APHY-CA
  *((UINT32P)(DDRPHY_RX_BASE + 0x01A4)) = 0x000005b8;
  *((UINT32P)(DDRPHY_RX_BASE + 0x01AC)) = 0x00010000;
  *((UINT32P)(DDRPHY_RX_BASE + 0x01B0)) = 0x00001041;

  //! step-2 : trigger RX SWRST
  *((UINT32P)(DDRPHY_TX_BASE + 0x0244)) = 0x0000cc83;
  *((UINT32P)(DDRPHY_TX_BASE + 0x0244)) = 0x0000cc8b;
  *((UINT32P)(DDRPHY_RX_BASE + 0x0244)) = 0x0000cc83;
  *((UINT32P)(DDRPHY_RX_BASE + 0x0244)) = 0x0000cc8b;

  //! step-3 : release RX SWRST
  *((UINT32P)(DDRPHY_TX_BASE + 0x0244)) = 0x0000cc81;
  *((UINT32P)(DDRPHY_TX_BASE + 0x0244)) = 0x0000cc89;
  *((UINT32P)(DDRPHY_RX_BASE + 0x0244)) = 0x0000cc81;
  *((UINT32P)(DDRPHY_RX_BASE + 0x0244)) = 0x0000cc89;

  /*TINFO="[LPBK] Trigger TX Enable ..."*/

  //! step-4 : trigger LPBK TX_EN
  *((UINT32P)(DDRPHY_TX_BASE + 0x0244)) = 0x0000cc85;

  mcDELAY_US(1);

  reg_r_data = *((UINT32P)(DDRPHY_RX_BASE + 0x0580)) ;
  mcSHOW_DBG_MSG(("Reg 0x580=%x\n", reg_r_data));
  //! step-5 : wait LPBK done
  do {
    reg_r_data = *((UINT32P)(DDRPHY_RX_BASE + 0x0580)) ;

    reg_r_data = *((UINT32P)(DDRPHY_RX_BASE + 0x0580)) ;
    mcSHOW_DBG_MSG(("Reg 0x580=%x\n", reg_r_data));


    /*TINFO="[LPBK] Waiting for RX test done ; current RX status is [%x] ...", reg_r_data*/
    //!temply mark : waiting for SH update ECO RTL files }while( reg_r_data != 0x0fffffff ) ;  //! B0-9bits ; B1-9bits ; CA-10bits
  }while( reg_r_data != 0xfffffff ) ;  //! B0-9bits ; B1-9bits ; CA-10bits

  //*GPIO_DOUT1 = 0x00000300; //! EINT-9 : trigger lpbk done

  *((UINT32P)(DDRPHY0AO_BASE + 0x0244)) = 0x0000cc81;
  *((UINT32P)(DDRPHY1AO_BASE + 0x0244)) = 0x0000cc81;

  //! check fail
  reg_r_data = *((UINT32P)(DDRPHY_RX_BASE + 0x0584)) ;
  mcSHOW_DBG_MSG(("Reg 0x584=%x\n", reg_r_data));
  if( reg_r_data != 0 ){
    /*TINFO="[LPBK] DDRPHY loopback test FAIL"*/
    /*TINFO="[LPBK] LPBK fail status = %x", reg_r_data*/
    lpbk_test_result = 0 ;
  } else {
    /*TINFO="[LPBK] DDRPHY loopback test PASS !!!"*/
    lpbk_test_result = 1 ;
    //*GPIO_DOUT1 = 0x00000700; //! EINT-10 : trigger lpbk pass
  }

  reg_r_data = *((UINT32P)(DDRPHY_RX_BASE + 0x0588)) ;
  mcSHOW_DBG_MSG(("Reg 0x588=%x\n", reg_r_data));

  //*MDM_TM_ENDSIM = 0x0;
  //while(1);

}//! end of ddrphy_ext_lpbk_test_seq

#endif

//#define DDR1600_LPBK
DRAM_STATUS_T DramcLoopbackTest(DRAMC_CTX_T *p)
{
    int           lpbk_tx     = 0 ;
    int           fpc_result  = 0 ;
    unsigned int  ddrphy_drvp = 0 ;
    unsigned int  ddrphy_drvn = 0 ;
    unsigned int  ddrphy_odtn = 0 ;

#if 0//mem.c
    /*TINFO="[LPBK] start to do ddrphy initial setting ..."*/
    /*TINFO="[LPBK] 2CH simultaneous configuration"*/
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    *((UINT32P)(DDRPHY0AO_BASE + 0x0040)) = 0x00000001;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0278)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x027c)) = 0xfe3fffff;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0274)) = 0xffffffff;
    *((UINT32P)(DDRPHY0AO_BASE + 0x028c)) = 0x006003bf;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0294)) = 0x11300000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d84)) = 0x0000001f;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0c1c)) = 0x00000011;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0c9c)) = 0x00000001;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d90)) = 0xe57800fe;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d98)) = 0xe57800fe;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01a0)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x00a0)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0120)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x05e4)) = 0x8000000e;
    *((UINT32P)(DDRPHY0AO_BASE + 0x05e8)) = 0x00000005;
    *((UINT32P)(DDRPHY0AO_BASE + 0x05f0)) = 0x10000020;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0670)) = 0x10000020;
    *((UINT32P)(DDRPHY0AO_BASE + 0x05f0)) = 0x10000220;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0670)) = 0x10000220;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0608)) = 0x20000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0808)) = 0x20000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0688)) = 0x20000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0888)) = 0x20000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0c14)) = 0x0030000e;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0c94)) = 0x0030000e;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e08)) = 0x120a120a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e0c)) = 0x120a120a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e10)) = 0x120a120a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e14)) = 0x120a120a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e18)) = 0x140c120a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f08)) = 0x130b130b;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f0c)) = 0x130b130b;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f10)) = 0x130b130b;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f14)) = 0x130b130b;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f18)) = 0x150d130b;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e58)) = 0x120a120a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e5c)) = 0x120a120a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e60)) = 0x120a120a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e64)) = 0x120a120a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e68)) = 0x140c120a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f58)) = 0x130b130b;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f5c)) = 0x130b130b;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f60)) = 0x130b130b;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f64)) = 0x130b130b;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f68)) = 0x150d130b;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0604)) = 0x00020002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0804)) = 0x00020002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0684)) = 0x00020002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0884)) = 0x00020002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0608)) = 0xb0800000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0808)) = 0xb0800000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0688)) = 0xb0800000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0888)) = 0xb0800000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x05f0)) = 0x00000220;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0670)) = 0x00000220;
    *((UINT32P)(DDRPHY0AO_BASE + 0x00a4)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0124)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x00a4)) = 0x00000010;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0124)) = 0x00000010;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0288)) = 0xffffffff;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e00)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f00)) = 0xaaaaaaaa;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1000)) = 0xaaaaaaaa;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e04)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f04)) = 0x0000000a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1004)) = 0x0000000a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e50)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f50)) = 0xaaaaaaaa;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1050)) = 0xaaaaaaaa;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e54)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f54)) = 0x0000000a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1054)) = 0x0000000a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e1c)) = 0x001f1f00;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e6c)) = 0x001f1f00;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f1c)) = 0x001f1f00;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f6c)) = 0x001f1f00;
    *((UINT32P)(DDRPHY0AO_BASE + 0x101c)) = 0x001f1f00;
    *((UINT32P)(DDRPHY0AO_BASE + 0x106c)) = 0x001f1f00;
    *((UINT32P)(DDRPHY0AO_BASE + 0x00a8)) = 0x00001010;
    *((UINT32P)(DDRPHY0AO_BASE + 0x00ac)) = 0x01111010;
    *((UINT32P)(DDRPHY0AO_BASE + 0x00b0)) = 0x010310c0;
    *((UINT32P)(DDRPHY0AO_BASE + 0x00ac)) = 0x03111000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0128)) = 0x00001010;
    *((UINT32P)(DDRPHY0AO_BASE + 0x012c)) = 0x01111010;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0130)) = 0x010310c0;
    *((UINT32P)(DDRPHY0AO_BASE + 0x012c)) = 0x03111000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01a4)) = 0x0000008c;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01b0)) = 0x00020000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d04)) = 0x00000101;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d08)) = 0x00000101;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1204)) = 0x00000101;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1208)) = 0x00000101;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0000)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0004)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0008)) = 0x00000010;
#ifdef O_0P7V_SDF
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d94)) = 0x33000003;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d9c)) = 0x33000003;
#else
#ifdef DDR1600_LPBK
    //! 1600
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d94)) = 0x1e400003;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d9c)) = 0x0f200001;
#else
    //! 3200
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d94)) = 0x3d000003;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d9c)) = 0x1e400001;
#endif
#endif
    *((UINT32P)(DDRPHY0AO_BASE + 0x0088)) = 0x880aec00;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0108)) = 0x880aec00;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0188)) = 0x880aec00;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0180)) = 0x00000008;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0080)) = 0x00000008;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0100)) = 0x00000008;
    *((UINT32P)(DDRPHY0AO_BASE + 0x00a4)) = 0x00000018;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0124)) = 0x00000018;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01a4)) = 0x0000008c;

    mcDELAY_US(1);

    *((UINT32P)(DDRPHY0AO_BASE + 0x0da0)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x12a0)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x17a0)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1ca0)) = 0x00000000;

    mcDELAY_US(1);

    *((UINT32P)(DDRPHY0AO_BASE + 0x0da4)) = 0x00019200;
    *((UINT32P)(DDRPHY0AO_BASE + 0x12a4)) = 0x00019200;
    *((UINT32P)(DDRPHY0AO_BASE + 0x17a4)) = 0x00019200;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1ca4)) = 0x00019200;

    mcDELAY_US(1);

    *((UINT32P)(DDRPHY0AO_BASE + 0x0da8)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x12a8)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x17a8)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1ca8)) = 0x00000000;

    mcDELAY_US(1);

    *((UINT32P)(DDRPHY0AO_BASE + 0x0000)) = 0x80000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0004)) = 0x80000000;

    mcDELAY_US(2);

    *((UINT32P)(DDRPHY0AO_BASE + 0x000c)) = 0x006d0000;

    mcDELAY_US(1);

    *((UINT32P)(DDRPHY0AO_BASE + 0x0d80)) = 0x00000003;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1280)) = 0x00000003;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1780)) = 0x00000003;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1c80)) = 0x00000003;

    mcDELAY_US(1);

    *((UINT32P)(DDRPHY0AO_BASE + 0x0c18)) = 0x04000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1118)) = 0x04000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1618)) = 0x04000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1b18)) = 0x04000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0c98)) = 0x04000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1198)) = 0x04000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1698)) = 0x04000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1b98)) = 0x04000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d18)) = 0x04000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1218)) = 0x04000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1718)) = 0x04000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1c18)) = 0x04000000;

    mcDELAY_US(1);

    *((UINT32P)(DDRPHY0AO_BASE + 0x0184)) = 0x00200000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0084)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0104)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0188)) = 0x00000800;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0088)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0108)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x018c)) = 0x000ba000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x008c)) = 0x0002e800;
    *((UINT32P)(DDRPHY0AO_BASE + 0x010c)) = 0x0002e800;
    *((UINT32P)(DDRPHY0AO_BASE + 0x00a4)) = 0x0000041e;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0124)) = 0x0000041e;
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    *((UINT32P)(DDRPHY0AO_BASE + 0x0194)) = 0x00698608;
    *((UINT32P)(DDRPHY1AO_BASE + 0x0194)) = 0xc0778608;// change chb ca dll to slave mode
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    *((UINT32P)(DDRPHY0AO_BASE + 0x0094)) = 0xc0778608;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0114)) = 0xc0778608;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01b8)) = 0x00140101;
    *((UINT32P)(DDRPHY0AO_BASE + 0x026c)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d14)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1214)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1714)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1c14)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d00)) = 0x00104010;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1200)) = 0x00104010;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1700)) = 0x00104010;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1c00)) = 0x00104010;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d18)) = 0x04030000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1218)) = 0x04030000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1718)) = 0x04030000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1c18)) = 0x04030000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d1c)) = 0x04e10002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x121c)) = 0x04e10002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x171c)) = 0x04e10002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1c1c)) = 0x04e10002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0c1c)) = 0x00200011;
    *((UINT32P)(DDRPHY0AO_BASE + 0x111c)) = 0x00200002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x161c)) = 0x00200002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1b1c)) = 0x00200002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0c9c)) = 0x00200001;
    *((UINT32P)(DDRPHY0AO_BASE + 0x119c)) = 0x00200002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x169c)) = 0x00200002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1b9c)) = 0x00200002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0c18)) = 0x04010000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1118)) = 0x04010000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1618)) = 0x04010000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1b18)) = 0x04010000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0c98)) = 0x04010000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1198)) = 0x04010000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1698)) = 0x04010000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1b98)) = 0x04010000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0294)) = 0x11400000;
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    *((UINT32P)(DDRPHY0AO_BASE + 0x0270)) = 0xfff7fbfb;
    *((UINT32P)(DDRPHY1AO_BASE + 0x0270)) = 0xfffbfbfb;// change chb ca dll to slave mode
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    mcDELAY_US(10);

    *((UINT32P)(DDRPHY0AO_BASE + 0x0180)) = 0x0000000c;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0080)) = 0x00000009;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0100)) = 0x00000009;

    mcDELAY_US(1);

    *((UINT32P)(DDRPHY0AO_BASE + 0x0004)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d90)) = 0xe57800ff;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d98)) = 0xe57800ff;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0284)) = 0x0000001f;

    mcDELAY_US(1);

    //*((UINT32P)(DDRPHY0AO_BASE + 0x0194)) = 0x00698e08;
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    *((UINT32P)(DDRPHY0AO_BASE + 0x0194)) = 0x00698e08;
    *((UINT32P)(DDRPHY1AO_BASE + 0x0194)) = 0xc0778e08; // change chb ca dll to slave mode
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    mcDELAY_US(1);

    *((UINT32P)(DDRPHY0AO_BASE + 0x0094)) = 0xc0778e08;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0114)) = 0xc0778e08;

    mcDELAY_US(2);

    *((UINT32P)(DDRPHY0AO_BASE + 0x00a4)) = 0x0001051e;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0124)) = 0x0001051e;
    *((UINT32P)(DRAMC0_AO_BASE + 0x0b04)) = 0x0f0f0f0f;
    *((UINT32P)(DRAMC0_AO_BASE + 0x0c04)) = 0x0f0f0f0f;
    *((UINT32P)(DRAMC0_AO_BASE + 0x0204)) = 0x00014310;

    //! ==============================
    //! - DDRPHY lpbk common setting
    //! ==============================
    /*TINFO="[LPBK] start to do loopback test CHA/B common configuration ..."*/

    /*TINFO="[LPBK] DRVP, DRVN and ODT setting ..."*/
    //! 2016.04.28, from SA's result, SW calibration ODT turn-on
    ddrphy_drvp = 5   ;
    ddrphy_drvn = 9   ;
    ddrphy_odtn = 4   ;
    *((UINT32P)(DRAMC0_AO_BASE + 0x08a8)) = (ddrphy_drvp << 25) |
                  (ddrphy_drvn << 20) |
                  (ddrphy_drvp << 15) |
                  (ddrphy_drvn << 10) |
                  (ddrphy_drvp <<  5) |
                  (ddrphy_drvn <<  0) ;

    *((UINT32P)(DRAMC0_AO_BASE + 0x08ac)) = (ddrphy_drvp << 25) |
                  (ddrphy_drvn << 20) |
                  (ddrphy_drvp << 15) |
                  (ddrphy_drvn << 10) |
                  (ddrphy_drvp <<  5) |
                  (ddrphy_drvn <<  0) ;

    *((UINT32P)(DRAMC0_AO_BASE + 0x08b0)) = (ddrphy_odtn << 20) |
                  (ddrphy_odtn << 10) |
                  (ddrphy_odtn <<  0) ;

    *((UINT32P)(DRAMC0_AO_BASE + 0x08b4)) = (ddrphy_odtn << 20) |
                  (ddrphy_odtn << 10) |
                  (ddrphy_odtn <<  0) ;
#endif
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    //close all dcm
#if (SW_CHANGE_FOR_SIMULATION==0)
    EnableDramcPhyDCM(p, 0);
#endif

    /*TINFO="[LPBK] RX input delay setting ..."*/
    //! for Byte-0
    *((UINT32P)(DDRPHY0AO_BASE + 0x0608)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e08)) = 0x00000000; //! [29:24]: RG_RK0_RX_ARDQ1_F_DLY / [21:16]: RG_RK0_RX_ARDQ1_R_DLY
                              //! [13: 8]: RG_RK0_RX_ARDQ0_F_DLY / [ 5: 0]: RG_RK0_RX_ARDQ0_R_DLY
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e0c)) = 0x00000000; //! [29:24]: RG_RK0_RX_ARDQ3_F_DLY / [21:16]: RG_RK0_RX_ARDQ3_R_DLY
                              //! [13: 8]: RG_RK0_RX_ARDQ2_F_DLY / [ 5: 0]: RG_RK0_RX_ARDQ3_R_DLY
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e10)) = 0x00000000; //! [29:24]: RG_RK0_RX_ARDQ5_F_DLY / [21:16]: RG_RK0_RX_ARDQ5_R_DLY
                              //! [13: 8]: RG_RK0_RX_ARDQ4_F_DLY / [ 5: 0]: RG_RK0_RX_ARDQ4_R_DLY
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e14)) = 0x00000000; //! [29:24]: RG_RK0_RX_ARDQ7_F_DLY / [21:16]: RG_RK0_RX_ARDQ7_R_DLY
                              //! [13: 8]: RG_RK0_RX_ARDQ6_F_DLY / [ 5: 0]: RG_RK0_RX_ARDQ6_R_DLY
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e18)) = 0x00000000; //! [29:24]: RG_RK0_RX_ARDQS_F_DLY / [21:16]: RG_RK0_RX_ARDQS_R_DLY
                              //! [13: 8]: RG_RK0_RX_ARDQM_F_DLY / [ 5: 0]: RG_RK0_RX_ARDQM_R_DLY
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e1c)) = 0x00000000; //! it dosn't need to set TX delay

    //! for Byte-1
    *((UINT32P)(DDRPHY0AO_BASE + 0x0688)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e58)) = 0x00000000; //! [29:24]: RG_RK0_RX_ARDQ1_F_DLY / [21:16]: RG_RK0_RX_ARDQ1_R_DLY
                              //! [13: 8]: RG_RK0_RX_ARDQ0_F_DLY / [ 5: 0]: RG_RK0_RX_ARDQ0_R_DLY
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e5c)) = 0x00000000; //! [29:24]: RG_RK0_RX_ARDQ3_F_DLY / [21:16]: RG_RK0_RX_ARDQ3_R_DLY
                              //! [13: 8]: RG_RK0_RX_ARDQ2_F_DLY / [ 5: 0]: RG_RK0_RX_ARDQ3_R_DLY
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e60)) = 0x00000000; //! [29:24]: RG_RK0_RX_ARDQ5_F_DLY / [21:16]: RG_RK0_RX_ARDQ5_R_DLY
                              //! [13: 8]: RG_RK0_RX_ARDQ4_F_DLY / [ 5: 0]: RG_RK0_RX_ARDQ4_R_DLY
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e64)) = 0x00000000; //! [29:24]: RG_RK0_RX_ARDQ7_F_DLY / [21:16]: RG_RK0_RX_ARDQ7_R_DLY
                              //! [13: 8]: RG_RK0_RX_ARDQ6_F_DLY / [ 5: 0]: RG_RK0_RX_ARDQ6_R_DLY
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e68)) = 0x00000000; //! [29:24]: RG_RK0_RX_ARDQS_F_DLY / [21:16]: RG_RK0_RX_ARDQS_R_DLY
                              //! [13: 8]: RG_RK0_RX_ARDQM_F_DLY / [ 5: 0]: RG_RK0_RX_ARDQM_R_DLY
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e6c)) = 0x00000000; //! it dosn't need to set TX delay

    //! enable lpbk CG control
    *((UINT32P)(DDRPHY0AO_BASE + 0x0284)) = 0x0000801f;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0290)) = 0xffffffff;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0188)) = 0x00000000;

    //! set RG_TX_ARDQ_PULL_DN_B0/B1 to low
    *((UINT32P)(DDRPHY0AO_BASE + 0x00b4)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0134)) = 0x00000000;

    //! enable clock and TX/RX setting of ARCMD/BRCMD
    *((UINT32P)(DDRPHY0AO_BASE + 0x018C)) = 0x000ba800;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0198)) = 0x0f000060;
    *((UINT32P)(DDRPHY0AO_BASE + 0x019C)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01A0)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01A4)) = 0x0001071e;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01A8)) = 0x00001010;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01AC)) = 0x03111010;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01B0)) = 0x010310c0;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01B4)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01B8)) = 0x00000000;

    //! [0]: LFSR_EN; [1]: SSO_EN; [2]: XTALK_EN
#if 0
    *((UINT32P)(DDRPHY0AO_BASE + 0x024C)) = 0x00000007;
#else
    *((UINT32P)(DDRPHY0AO_BASE + 0x024C)) = 0x00000004;
#endif

    /*TINFO="[LPBK] Ignore pinmux  ..."*/
    //! xtalk TX bit toggle setting
    //! x-talk TX
    *((UINT32P)(DDRPHY0AO_BASE + 0x0250)) = 0x77777777  ; //! 0x76543210;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0254)) = 0x77777777  ; //! 0x65432100;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0258)) = 0x77777777  ; //! 0x54321007;
    *((UINT32P)(DDRPHY0AO_BASE + 0x025C)) = 0x77777777  ; //! 0x54321076;
    //! x-talk RX
    *((UINT32P)(DDRPHY0AO_BASE + 0x02F8)) = 0x77777777  ; //! 0x76543210;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02FC)) = 0x77777777  ; //! 0x65432100;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0300)) = 0x77777777  ; //! 0x54321007;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0304)) = 0x77777777  ; //! 0x54321076;

    //! TX LFSR
    *((UINT32P)(DDRPHY0AO_BASE + 0x0200)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0204)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0208)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x020c)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0210)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0214)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0218)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x021c)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0220)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0224)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0228)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x022c)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0230)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0234)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0238)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x023c)) = 0x4c044c04  ;

    //! RX LFSR
    *((UINT32P)(DDRPHY0AO_BASE + 0x02b4)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02b8)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02bc)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02c0)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02c4)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02c8)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02cc)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02d0)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02d4)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02d8)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02dc)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02e0)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02e4)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02e8)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02ec)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02f0)) = 0x4c044c04  ;

    //! ==============================
    //! - DDRPHY lpbk test flow
    //! ==============================
    //! step-0 : trigger loopback prepare on
    *((UINT32P)(DDRPHY0AO_BASE + 0x0244)) = 0x0000cc81;

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

#ifdef LPBK_MODE_0
    lpbk_tx = 0 ; //! CHN-A is TX
#else
    lpbk_tx = 1 ; //! CHN-B is TX
#endif

#if 0
    //! initial debug output
    *GPIO_MODE1 = 0x0;
    *GPIO_DIR1  = 0x00000700;
    *GPIO_DOUT1 = 0x00000000; //PAD_EINT8/9/10
#endif


    #if 0//Rx DQ Delay
    {
        UINT8 iDelay = 0;
        UINT8 ch, ii = 0;
        for(iDelay=1; iDelay<64; iDelay++)
        {
            U32 u4value = ((U32) iDelay) | (((U32)iDelay)<<8) | (((U32)iDelay)<<16) | (((U32)iDelay)<<24);
            for(ch=CHANNEL_A; ch<CHANNEL_NUMBER; ch++)
            {
                p->channel = ch;
                for (ii=0; ii<4; ii++)
                {
                    if(u1IsLP4Family(p->dram_type))
                    {
                        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ2+ ii*4), u4value);//DQ0~DQ7
                        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ2+ ii*4), u4value);//DQ8~DQ15
                    }
                }
                if(p->dram_type == TYPE_LPDDR4 || p->dram_type == TYPE_LPDDR4X || p->dram_type == TYPE_LPDDR4P)
                {
                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6), \
                                            P_Fld(iDelay,SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0) |P_Fld(iDelay,SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0));
                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ6), \
                                    P_Fld(iDelay,SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1) |P_Fld(iDelay,SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1));
                }
                DramPhyReset(p);
            }
            mcSHOW_DBG_MSG(("u1RxDqDly=%d\n", iDelay));
#ifdef LPBK_INTERNAL_EN
            ddrphy_int_lpbk_test_seq ( p ) ;
#else
            ddrphy_ext_lpbk_test_seq ( p, 1 ) ;
#endif

            //mcDELAY_MS(1000);
        }
    }
    #elif 0//RxDQS Delay
    {
        UINT8 iDelay = 0;
        UINT8 ch, ii = 0;
        for(iDelay=0; iDelay<64; iDelay++)
        {
            U32 u4value = ((U32) iDelay) | (((U32)iDelay)<<8) | (((U32)iDelay)<<16) | (((U32)iDelay)<<24);
            for(ch=CHANNEL_A; ch<CHANNEL_NUMBER; ch++)
            {
                p->channel = ch;
                //for (ii=0; ii<4; ii++)
                {
                    // Set DQS delay
                    if(u1IsLP4Family(p->dram_type))
                    {
                        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6), \
                                                P_Fld((iDelay),SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0) |P_Fld((iDelay),SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0));
                        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ6), \
                                        P_Fld(iDelay,SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1) |P_Fld(iDelay,SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1));
                    }
                }
                DramPhyReset(p);
            }
            mcSHOW_DBG_MSG(("u1RxDqsDly=%d\n", iDelay));
#ifdef LPBK_INTERNAL_EN
            ddrphy_int_lpbk_test_seq ( p ) ;
#else
            ddrphy_ext_lpbk_test_seq ( p, 1 ) ;
#endif

            //mcDELAY_MS(1000);
        }
    }
    #elif 1//Rx CA Delay
    {
        UINT8 iDelay = 0;
        UINT8 ch, ii = 0;
        for(iDelay=20; iDelay<=20; iDelay++)
        {
            U32 u4value = ((U32) iDelay) | (((U32)iDelay)<<8) | (((U32)iDelay)<<16) | (((U32)iDelay)<<24);
            for(ch=CHANNEL_A; ch<CHANNEL_NUMBER; ch++)
            {
                p->channel = ch;
                for (ii=0; ii<4; ii++)
                {
                    if(u1IsLP4Family(p->dram_type))
                    {
                        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ2+ ii*4), u4value);//DQ0~DQ7
                        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ2+ ii*4), u4value);//DQ8~DQ15
                    }
                }
                if(p->dram_type == TYPE_LPDDR4 || p->dram_type == TYPE_LPDDR4X || p->dram_type == TYPE_LPDDR4P)
                {
                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6), \
                                            P_Fld(iDelay,SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0) |P_Fld(iDelay,SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0));
                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ6), \
                                    P_Fld(iDelay,SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1) |P_Fld(iDelay,SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1));
                }
            }
            mcSHOW_DBG_MSG(("u1RxDqDly=%d\n", iDelay));
            //mcDELAY_MS(1000);
        }
        for(iDelay=0; iDelay<64; iDelay++)
        {
            U32 u4value = ((U32) iDelay) | (((U32)iDelay)<<8) | (((U32)iDelay)<<16) | (((U32)iDelay)<<24);
            for(ch=CHANNEL_A; ch<CHANNEL_NUMBER; ch++)
            {
                p->channel = ch;
                if(u1IsLP4Family(p->dram_type))
                {
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD2), u4value);//CA0, CA1
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD3), u4value);//CA2, CA3
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD4), u4value);//CA4, CA5
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD5), u4value);//CKE0, CKE1
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD6), u4value);//CKE2, CS0
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD7), u4value);//CS1, CS2
                }
                DramPhyReset(p);
            }
            mcSHOW_DBG_MSG(("u1RxCADly=%d\n", iDelay));
#ifdef LPBK_INTERNAL_EN
            ddrphy_int_lpbk_test_seq(p) ;
#else
            ddrphy_ext_lpbk_test_seq ( p, 1 ) ;
#endif

            //mcDELAY_MS(1000);
        }
        DramPhyReset(p);

#ifdef LPBK_INTERNAL_EN
        ddrphy_int_lpbk_test_seq(p) ;
#else
        ddrphy_ext_lpbk_test_seq ( p, 0 ) ;
#endif
    }
    #endif
    //ddrphy_ext_lpbk_test_seq ( lpbk_tx ) ;
}//! end of fpc_lpbk_mem_init
#endif


#ifdef DDR_INIT_TIME_PROFILING
//Kevin-CW Chen
//M0 ARMPLL div 1  = 897Mhz
//L1.mt6797.dev for smt ? 1.3Ghz
UINT32 temp;

void TimeProfileBegin(void)
{
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
}

UINT32 TimeProfileEnd(void)
{
    UINT32 div = 897*1024;
    asm volatile(
        "MRC p15, 0, %0, c9, c12, 0\n"
        "BIC %0, %0, #1 << 0\n"   /* disable */
        "MCR p15, 0, %0, c9, c12, 0\n"
        "MRC p15, 0, %0, c9, c13, 0\n"
        : "+r"(temp)
        :
        : "cc"
    );

    //mcSHOW_ERR_MSG((" TimeProfileEnd: %d CPU cycles(%d ms)\n\r", temp, temp/div));
    return (temp/div);
}
#endif


