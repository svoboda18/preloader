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
 * Description:
 *
 *---------------------------------------------------------------------------*/

#ifndef X_HAL_IO_H
#define X_HAL_IO_H


//#include "..\Common\pd_def.h"
#include "dramc_pi_api.h"
//===========================================================================

// field access macro-----------------------------------------------------------

/* field macros */
#define Fld(wid,shft,ac)    (((U32)wid<<16)|(shft<<8)|ac)
#define Fld_wid(fld)    ((UINT8)((fld)>>16))
#define Fld_shft(fld)   ((UINT8)((fld)>>8))
#define Fld_ac(fld)     ((UINT8)(fld))

/* access method*/
#define AC_FULLB0       1
#define AC_FULLB1       2
#define AC_FULLB2       3
#define AC_FULLB3       4
#define AC_FULLW10      5
#define AC_FULLW21      6
#define AC_FULLW32      7
#define AC_FULLDW       8
#define AC_MSKB0        11
#define AC_MSKB1        12
#define AC_MSKB2        13
#define AC_MSKB3        14
#define AC_MSKW10       15
#define AC_MSKW21       16
#define AC_MSKW32       17
#define AC_MSKDW        18

#define Fld2Msk32(fld)  /*lint -save -e504 */ (((U32)0xffffffff>>(32-Fld_wid(fld)))<<Fld_shft(fld)) /*lint -restore */
#define P_Fld(val,fld) ((sizeof(upk)>1)?Fld2Msk32(fld):(((UINT32)(val)&((1<<Fld_wid(fld))-1))<<Fld_shft(fld)))

extern U32 u4Dram_Register_Read(DRAMC_CTX_T *p, U32 u4reg_addr); 
extern U8 ucDram_Register_Write(DRAMC_CTX_T *p, U32 u4reg_addr, U32 u4reg_value);

extern void vIO32Write4BMsk2(DRAMC_CTX_T *p, U32 reg32, U32 val32, U32 msk32);
extern void vIO32Write4BMsk_All2(DRAMC_CTX_T *p, U32 reg32, U32 val32, U32 msk32);
extern void vIO32Write4B_All2(DRAMC_CTX_T *p, U32 reg32, U32 val32);

// =========================
// public Macro for general use. 
//==========================
#define u4IO32Read4B(reg32) u4Dram_Register_Read(p,reg32)
#define vIO32Write4B(reg32,val32) ucDram_Register_Write(p,reg32,val32)
#define vIO32Write4B_All(reg32, val32) vIO32Write4B_All2(p, reg32, val32)
#define vIO32Write4BMsk(reg32, val32, msk32) vIO32Write4BMsk2(p, reg32, val32, msk32)
#define vIO32Write4BMsk_All(reg32, val32, msk32) vIO32Write4BMsk_All2(p, reg32, val32, msk32)

#if SW_CHANGE_FOR_SIMULATION
extern U32 u4IO32ReadFldAlign2(DRAMC_CTX_T *p, U32 reg32, U32 fld);
extern void vIO32WriteFldAlign2(DRAMC_CTX_T *p, U32 reg32, U32 val, U32 fld);
extern void vIO32WriteFldAlign_All2(DRAMC_CTX_T *p, U32 reg32, U32 val, U32 fld);
#define u4IO32ReadFldAlign(reg32, fld)	u4IO32ReadFldAlign2(p, reg32, fld)
#define vIO32WriteFldAlign(reg32, val, fld)	vIO32WriteFldAlign2(p, reg32, val, fld)
#define vIO32WriteFldAlign_All(reg32, val, fld)	vIO32WriteFldAlign_All2(p, reg32, val, fld)

#define vIO32WriteFldMulti(reg32,list) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
{ \
	UINT16 upk; \
	INT32 msk=(INT32)(list); \
	{UINT8 upk; \
	if ((U32)msk==0xffffffff) \
            (vIO32Write4B(reg32,(list))); \
        else if((U32)msk) \
              vIO32Write4BMsk(reg32,(list),((U32)msk)); \
	} \
}/*lint -restore */

#define vIO32WriteFldMulti_All(reg32,list) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
{ \
	UINT16 upk; \
	INT32 msk=(INT32)(list); \
	{UINT8 upk; \
	if ((U32)msk==0xffffffff) \
            (vIO32Write4B_All(reg32,(list))); \
        else if((U32)msk) \
              vIO32Write4BMsk_All(reg32,(list),((U32)msk)); \
	} \
}/*lint -restore */

#else

#define u4IO32ReadFldAlign(reg32,fld) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
	((Fld_ac(fld)==AC_FULLDW)? u4IO32Read4B(reg32):((u4IO32Read4B(reg32)&Fld2Msk32(fld))>>Fld_shft(fld)) )

#define vIO32WriteFldAlign(reg32,val,fld) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
	((Fld_ac(fld)==AC_FULLDW)?(vIO32Write4B((reg32),(val))) : (vIO32Write4BMsk((reg32),((U32)(val)<<Fld_shft(fld)),Fld2Msk32(fld))) )

#define vIO32WriteFldMulti(reg32,list) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
{ \
	UINT16 upk; \
	INT32 msk=(INT32)(list); \
	{UINT8 upk; \
	((U32)msk==0xffffffff)?(vIO32Write4B(reg32,(list))):(((U32)msk)?vIO32Write4BMsk(reg32,(list),((U32)msk)):0); \
	} \
}/*lint -restore */

//=========================
// Public Macro for write all-dramC or all-PHY registers
//=========================
#define vIO32WriteFldAlign_All(reg32,val,fld) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
	((Fld_ac(fld)==AC_FULLDW)?(vIO32Write4B_All((reg32),(val))) : (vIO32Write4BMsk_All((reg32),((U32)(val)<<Fld_shft(fld)),Fld2Msk32(fld))) )

#define vIO32WriteFldMulti_All(reg32,list) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
{ \
	UINT16 upk; \
	INT32 msk=(INT32)(list); \
	{UINT8 upk; \
	((U32)msk==0xffffffff)?(vIO32Write4B_All(reg32,(list))):(((U32)msk)?vIO32Write4BMsk_All(reg32,(list),((U32)msk)):0); \
	} \
}/*lint -restore */
#endif

#define REG_ACCESS_NAO_DGB 0
#define REG_ACCESS_PORTING_DGB 0
#define REG_SHUFFLE_REG_CHECK 0


#endif  // X_HAL_IO_H
