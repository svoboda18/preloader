/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 * 
 * MediaTek Inc. (C) 2010. All rights reserved.
 * 
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

#ifndef __EMI_H__
#define __EMI_H__

#define IO_PHYS            	    0x10000000
#define EMI_BASE                (IO_PHYS + 0x00203000)
#define EMI_MPU_BASE            (IO_PHYS + 0x00226000)

/*EMI PSRAM (NOR) and DRAM control registers*/
#define EMI_CONA                 ((P_U32)(EMI_BASE+0x0000))  /* EMI control register for bank 0 */
#define EMI_CONB                 ((P_U32)(EMI_BASE+0x0008))  /* EMI control register for bank 1 */
#define EMI_CONC                 ((P_U32)(EMI_BASE+0x0010))  /* EMI control register for bank 2 */
#define EMI_COND                 ((P_U32)(EMI_BASE+0x0018))  /* EMI control register for bank 3 */
#define EMI_CONE                 ((P_U32)(EMI_BASE+0x0020))  /* EMI control register for bank 0 */
#define EMI_CONF                 ((P_U32)(EMI_BASE+0x0028))  /* EMI control register for bank 1 */
#define EMI_CONG                 ((P_U32)(EMI_BASE+0x0030))  /* EMI control register for bank 0 */
#define EMI_CONH                 ((P_U32)(EMI_BASE+0x0038))  /* EMI control register for bank 1 */
#define EMI_CONI                 ((P_U32)(EMI_BASE+0x0040))  /* EMI control register 0 for Mobile-RAM */
#define EMI_CONJ                 ((P_U32)(EMI_BASE+0x0048))  /* EMI control register 1 for Mobile-RAM */
#define EMI_CONK                 ((P_U32)(EMI_BASE+0x0050))  /* EMI control register 2 for Mobile-RAM */
#define EMI_CONL                 ((P_U32)(EMI_BASE+0x0058))  /* EMI control register 3 for Mobile-RAM */
#define EMI_CONM                 ((P_U32)(EMI_BASE+0x0060))
#define EMI_CONN                 ((P_U32)(EMI_BASE+0x0068))
#define CAL_EN                   (1 << 8)
#define EMI_GENA                 ((P_U32)(EMI_BASE+0x0070))
#define EMI_REMAP                 EMI_GENA
#define EMI_DRCT                 ((P_U32)(EMI_BASE+0x0078))
#define EMI_DDRV                 ((P_U32)(EMI_BASE+0x0080))
#define EMI_GEND                 ((P_U32)(EMI_BASE+0x0088))
#define EMI_PPCT                 ((P_U32)(EMI_BASE+0x0090)) /* EMI Performance and Power Control Register */

#define EMI_DLLV                 ((P_U32)(EMI_BASE+0x00A0))

#define EMI_DFTC                 ((P_U32)(EMI_BASE+0x00F0))
#define EMI_DFTD                 ((P_U32)(EMI_BASE+0x00F8))

/* EMI bandwith filter and MPU control registers */
#define EMI_ARBA                 ((P_U32)(EMI_BASE+0x0100))
#define EMI_ARBB                 ((P_U32)(EMI_BASE+0x0108))
#define EMI_ARBC                 ((P_U32)(EMI_BASE+0x0110))
#define EMI_ARBD                 ((P_U32)(EMI_BASE+0x0118))
#define EMI_ARBE                 ((P_U32)(EMI_BASE+0x0120))
#define EMI_ARBF                 ((P_U32)(EMI_BASE+0x0128))

#define EMI_SLCT                 ((P_U32)(EMI_BASE+0x0150))
#define EMI_ABCT	             ((P_U32)(EMI_BASE+0x0158))

/* EMI Memory Protect Unit */
#define EMI_MPUA                 ((P_U32)(EMI_MPU_BASE+0x0160))
#define EMI_MPUB                 ((P_U32)(EMI_MPU_BASE+0x0168))
#define EMI_MPUC                 ((P_U32)(EMI_MPU_BASE+0x0170))
#define EMI_MPUD                 ((P_U32)(EMI_MPU_BASE+0x0178))
#define EMI_MPUE                 ((P_U32)(EMI_MPU_BASE+0x0180))
#define EMI_MPUF	               ((P_U32)(EMI_MPU_BASE+0x0188))
#define EMI_MPUG	               ((P_U32)(EMI_MPU_BASE+0x0190))
#define EMI_MPUH	               ((P_U32)(EMI_MPU_BASE+0x0198))
#define EMI_MPUA2		             ((P_U32)(EMI_MPU_BASE+0x0260))  
#define EMI_MPUB2	               ((P_U32)(EMI_MPU_BASE+0x0268))  
#define EMI_MPUC2	             	 ((P_U32)(EMI_MPU_BASE+0x0270))  
#define EMI_MPUD2		             ((P_U32)(EMI_MPU_BASE+0x0278))
#define EMI_MPUE2             	 ((P_U32)(EMI_MPU_BASE+0x0280))  
#define EMI_MPUF2            		 ((P_U32)(EMI_MPU_BASE+0x0288))
#define EMI_MPUG2		             ((P_U32)(EMI_MPU_BASE+0x0290)) 
#define EMI_MPUH2		             ((P_U32)(EMI_MPU_BASE+0x0298))  
#define EMI_MPUA3		             ((P_U32)(EMI_MPU_BASE+0x0360))  
#define EMI_MPUB3	               ((P_U32)(EMI_MPU_BASE+0x0368))  
#define EMI_MPUC3	             	 ((P_U32)(EMI_MPU_BASE+0x0370))  
#define EMI_MPUD3		             ((P_U32)(EMI_MPU_BASE+0x0378))

#define EMI_MPUI	               ((P_U32)(EMI_MPU_BASE+0x01A0))
#define EMI_MPUI_2ND	           ((P_U32)(EMI_MPU_BASE+0x01A4))
#define EMI_MPUJ                 ((P_U32)(EMI_MPU_BASE+0x01A8))
#define EMI_MPUJ_2ND	           ((P_U32)(EMI_MPU_BASE+0x01AC))
#define EMI_MPUK                 ((P_U32)(EMI_MPU_BASE+0x01B0))
#define EMI_MPUK_2ND             ((P_U32)(EMI_MPU_BASE+0x01B4))
#define EMI_MPUL                 ((P_U32)(EMI_MPU_BASE+0x01B8))
#define EMI_MPUL_2ND             ((P_U32)(EMI_MPU_BASE+0x01BC))
#define EMI_MPUI2		             ((P_U32)(EMI_MPU_BASE+0x02A0))  
#define EMI_MPUI2_2ND            ((P_U32)(EMI_MPU_BASE+0x02A4))  
#define EMI_MPUJ2            	 	 ((P_U32)(EMI_MPU_BASE+0x02A8))  
#define EMI_MPUJ2_2ND	           ((P_U32)(EMI_MPU_BASE+0x02AC))  
#define EMI_MPUK2		             ((P_U32)(EMI_MPU_BASE+0x02B0)) 
#define EMI_MPUK2_2ND	           ((P_U32)(EMI_MPU_BASE+0x02B4))  
#define EMI_MPUL2		             ((P_U32)(EMI_MPU_BASE+0x02B8))  
#define EMI_MPUL2_2ND	           ((P_U32)(EMI_MPU_BASE+0x02BC))  
#define EMI_MPUI3	               ((P_U32)(EMI_MPU_BASE+0x03A0))
#define EMI_MPUJ3                ((P_U32)(EMI_MPU_BASE+0x03A8))
#define EMI_MPUK3                ((P_U32)(EMI_MPU_BASE+0x03B0))
#define EMI_MPUL3                ((P_U32)(EMI_MPU_BASE+0x03B8))

#define EMI_MPUM                 ((P_U32)(EMI_MPU_BASE+0x01C0))
#define EMI_MPUN                 ((P_U32)(EMI_MPU_BASE+0x01C8))
#define EMI_MPUO                 ((P_U32)(EMI_MPU_BASE+0x01D0))
#define EMI_MPUU                 ((P_U32)(EMI_MPU_BASE+0x0200))
#define EMI_MPUM2		             ((P_U32)(EMI_MPU_BASE+0x02C0)) 
#define EMI_MPUN2	               ((P_U32)(EMI_MPU_BASE+0x02C8)) 
#define EMI_MPUO2		             ((P_U32)(EMI_MPU_BASE+0x02D0))
#define EMI_MPUU2                ((P_U32)(EMI_MPU_BASE+0x0300)) 

#define EMI_MPUP                 ((P_U32)(EMI_BASE+0x01D8))
#define EMI_MPUQ                 ((P_U32)(EMI_BASE+0x01E0))
#define EMI_MPUR                 ((P_U32)(EMI_BASE+0x01E8))
#define EMI_MPUS                 ((P_U32)(EMI_BASE+0x01F0))
#define EMI_MPUT                 ((P_U32)(EMI_BASE+0x01F8))
#define EMI_MPUY                 ((P_U32)(EMI_BASE+0x0220))

#define EMI_MPUP2	               ((P_U32)(EMI_BASE+0x02D8)) 
#define EMI_MPUQ2		             ((P_U32)(EMI_BASE+0x02E0)) 
#define EMI_MPUR2		             ((P_U32)(EMI_BASE+0x02E8))  
#define EMI_MPUY2	               ((P_U32)(EMI_BASE+0x0320))

/* EMI IO delay, driving and MISC control registers */
#define EMI_IDLA            ((P_U32)(EMI_BASE+0x0200))
#define EMI_IDLB            ((P_U32)(EMI_BASE+0x0208))
#define EMI_IDLC            ((P_U32)(EMI_BASE+0x0210))
#define EMI_IDLD            ((P_U32)(EMI_BASE+0x0218))
#define EMI_IDLE            ((P_U32)(EMI_BASE+0x0220))
#define EMI_IDLF            ((P_U32)(EMI_BASE+0x0228))
#define EMI_IDLG            ((P_U32)(EMI_BASE+0x0230))
#define EMI_IDLH            ((P_U32)(EMI_BASE+0x0238))
#define EMI_IDLI            ((P_U32)(EMI_BASE+0x0240)) // IO input delay (DQS0 ~ DQS4)
#define EMI_IDLJ            ((P_U32)(EMI_BASE+0x0248))
#define EMI_IDLK            ((P_U32)(EMI_BASE+0x0250))

#define EMI_ODLA           ((P_U32)(EMI_BASE+0x0258))
#define EMI_ODLB           ((P_U32)(EMI_BASE+0x0260))
#define EMI_ODLC           ((P_U32)(EMI_BASE+0x0268))
#define EMI_ODLD           ((P_U32)(EMI_BASE+0x0270))
#define EMI_ODLE           ((P_U32)(EMI_BASE+0x0278))
#define EMI_ODLF           ((P_U32)(EMI_BASE+0x0280))
#define EMI_ODLG           ((P_U32)(EMI_BASE+0x0288))

#define EMI_DUTA           ((P_U32)(EMI_BASE+0x0290))
#define EMI_DUTB           ((P_U32)(EMI_BASE+0x0298))
#define EMI_DUTC           ((P_U32)(EMI_BASE+0x02A0))

#define EMI_DRVA           ((P_U32)(EMI_BASE+0x02A8))
#define EMI_DRVB           ((P_U32)(EMI_BASE+0x02B0))

#define EMI_IOCL           ((P_U32)(EMI_BASE+0x02B8))
#define EMI_IOCM           ((P_U32)(EMI_BASE+0x02C0)) //IvanTseng, for 4T mode
#define EMI_IODC           ((P_U32)(EMI_BASE+0x02C8))

#define EMI_ODTA           ((P_U32)(EMI_BASE+0x02D0))
#define EMI_ODTB           ((P_U32)(EMI_BASE+0x02D8))

/* EMI auto-tracking control registers */
#define EMI_DQSA           ((P_U32)(EMI_BASE+0x0300))
#define EMI_DQSB           ((P_U32)(EMI_BASE+0x0308))
#define EMI_DQSC           ((P_U32)(EMI_BASE+0x0310))
#define EMI_DQSD           ((P_U32)(EMI_BASE+0x0318))


#define EMI_DQSE           ((P_U32)(EMI_BASE+0x0320))
#define EMI_DQSV           ((P_U32)(EMI_BASE+0x0328))

#define EMI_CALA           ((P_U32)(EMI_BASE+0x0330))
#define EMI_CALB           ((P_U32)(EMI_BASE+0x0338))
#define EMI_CALC           ((P_U32)(EMI_BASE+0x0340))
#define EMI_CALD           ((P_U32)(EMI_BASE+0x0348))


#define EMI_CALE           ((P_U32)(EMI_BASE+0x0350)) //DDR data auto tracking control
#define EMI_CALF           ((P_U32)(EMI_BASE+0x0358))
#define EMI_CALG           ((P_U32)(EMI_BASE+0x0360)) //DDR data auto tracking control
#define EMI_CALH           ((P_U32)(EMI_BASE+0x0368))

#define EMI_CALI           ((P_U32)(EMI_BASE+0x0370))
#define EMI_CALJ           ((P_U32)(EMI_BASE+0x0378))
#define EMI_CALK           ((P_U32)(EMI_BASE+0x0380))
#define EMI_CALL           ((P_U32)(EMI_BASE+0x0388))


#define EMI_CALM           ((P_U32)(EMI_BASE+0x0390))
#define EMI_CALN           ((P_U32)(EMI_BASE+0x0398))

#define EMI_CALO           ((P_U32)(EMI_BASE+0x03A0))
#define EMI_CALP           ((P_U32)(EMI_BASE+0x03A8))

#define EMI_DUCA           ((P_U32)(EMI_BASE+0x03B0))
#define EMI_DUCB           ((P_U32)(EMI_BASE+0x03B8))
#define EMI_DUCC           ((P_U32)(EMI_BASE+0x03C0))
#define EMI_DUCD           ((P_U32)(EMI_BASE+0x03C8))
#define EMI_DUCE           ((P_U32)(EMI_BASE+0x03D0))

/* EMI bus monitor control registers */
#define EMI_BMEN           ((P_U32)(EMI_BASE+0x0400))
#define EMI_BCNT           ((P_U32)(EMI_BASE+0x0408))
#define EMI_TACT           ((P_U32)(EMI_BASE+0x0410))
#define EMI_TSCT           ((P_U32)(EMI_BASE+0x0418))
#define EMI_WACT           ((P_U32)(EMI_BASE+0x0420))
#define EMI_WSCT           ((P_U32)(EMI_BASE+0x0428))
#define EMI_BACT           ((P_U32)(EMI_BASE+0x0430))
#define EMI_BSCT           ((P_U32)(EMI_BASE+0x0438))
#define EMI_MSEL           ((P_U32)(EMI_BASE+0x0440))
#define EMI_TSCT2           ((P_U32)(EMI_BASE+0x0448))
#define EMI_TSCT3           ((P_U32)(EMI_BASE+0x0450))
#define EMI_WSCT2           ((P_U32)(EMI_BASE+0x0458))
#define EMI_WSCT3           ((P_U32)(EMI_BASE+0x0460))
#define EMI_MSEL2           ((P_U32)(EMI_BASE+0x0468))
#define EMI_MSEL3           ((P_U32)(EMI_BASE+0x0470))
#define EMI_MSEL4           ((P_U32)(EMI_BASE+0x0478))
#define EMI_MSEL5           ((P_U32)(EMI_BASE+0x0480))
#define EMI_MSEL6           ((P_U32)(EMI_BASE+0x0488))
#define EMI_MSEL7           ((P_U32)(EMI_BASE+0x0490))
#define EMI_MSEL8           ((P_U32)(EMI_BASE+0x0498))
#define EMI_MSEL9           ((P_U32)(EMI_BASE+0x04A0))
#define EMI_MSEL10           ((P_U32)(EMI_BASE+0x04A8))

#define EMI_TTYPE1            ((P_U32)(EMI_BASE+0x0500))
#define EMI_TTYPE2            ((P_U32)(EMI_BASE+0x0508))
#define EMI_TTYPE3            ((P_U32)(EMI_BASE+0x0510))
#define EMI_TTYPE4            ((P_U32)(EMI_BASE+0x0518))
#define EMI_TTYPE5            ((P_U32)(EMI_BASE+0x0520))
#define EMI_TTYPE6            ((P_U32)(EMI_BASE+0x0528))
#define EMI_TTYPE7            ((P_U32)(EMI_BASE+0x0530))
#define EMI_TTYPE8            ((P_U32)(EMI_BASE+0x0538))
#define EMI_TTYPE9            ((P_U32)(EMI_BASE+0x0540))
#define EMI_TTYPE10            ((P_U32)(EMI_BASE+0x0548))
#define EMI_TTYPE11            ((P_U32)(EMI_BASE+0x0550))
#define EMI_TTYPE12            ((P_U32)(EMI_BASE+0x0558))
#define EMI_TTYPE13            ((P_U32)(EMI_BASE+0x0560))
#define EMI_TTYPE14            ((P_U32)(EMI_BASE+0x0568))
#define EMI_TTYPE15            ((P_U32)(EMI_BASE+0x0570))
#define EMI_TTYPE16            ((P_U32)(EMI_BASE+0x0578))
#define EMI_TTYPE17            ((P_U32)(EMI_BASE+0x0580))
#define EMI_TTYPE18            ((P_U32)(EMI_BASE+0x0588))
#define EMI_TTYPE19            ((P_U32)(EMI_BASE+0x0590))
#define EMI_TTYPE20            ((P_U32)(EMI_BASE+0x0598))
#define EMI_TTYPE21            ((P_U32)(EMI_BASE+0x05A0))

#endif // __EMI_H__
