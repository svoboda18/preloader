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

#ifndef MT_EMI_H
#define MT_EMI_H

#define DDR_RESERVE_MODE

typedef volatile unsigned int *V_UINT32P;
int get_dram_rank_nr (void);
void get_dram_rank_size(unsigned long long dram_rank_size[]);
#define DDR1  1
#define LPDDR2  2
#define LPDDR3  3
#define PCDDR3  4
typedef struct
{
    int   sub_version;            // sub_version: 0x1 for new version
    int  type;                /* 0x0000 : Invalid
                                 0x0001 : Discrete DDR1
                                 0x0002 : Discrete LPDDR2
                                 0x0003 : Discrete LPDDR3
                                 0x0004 : Discrete PCDDR3
                                 0x0101 : MCP(NAND+DDR1)
                                 0x0102 : MCP(NAND+LPDDR2)
                                 0x0103 : MCP(NAND+LPDDR3)
                                 0x0104 : MCP(NAND+PCDDR3)
                                 0x0201 : MCP(eMMC+DDR1)
                                 0x0202 : MCP(eMMC+LPDDR2)
                                 0x0203 : MCP(eMMC+LPDDR3)
                                 0x0204 : MCP(eMMC+PCDDR3)
                              */
    int   id_length;              // EMMC and NAND ID checking length
    int   fw_id_length;              // FW ID checking length
    char  ID[16];
    char  fw_id[8];               // To save fw id
    int   EMI_CONA_VAL;           //@0x3000
    int   EMI_CONH_VAL;
    //int   DRAMC_DRVCTL0_VAL;      //@0x40B8               -> customized TX I/O driving
    //int   DRAMC_DRVCTL1_VAL;      //@0x40BC               -> customized TX I/O driving
    int   DRAMC_ACTIM_VAL;        //@0x4000
    int   DRAMC_GDDR3CTL1_VAL;    //@0x40F4
    int   DRAMC_CONF1_VAL;        //@0x4004
    int   DRAMC_DDR2CTL_VAL;      //@0x407C 
    int   DRAMC_TEST2_3_VAL;      //@0x4044
    int   DRAMC_CONF2_VAL;        //@0x4008
    int   DRAMC_PD_CTRL_VAL;      //@0x41DC
    //int   DRAMC_PADCTL3_VAL;      //@0x4014               -> customized TX DQS delay
    //int   DRAMC_DQODLY_VAL;       //@0x4200~0x420C        -> customized TX DQ delay
    //int   DRAMC_ADDR_OUTPUT_DLY;  // for E1 DDR2 only
    //int   DRAMC_CLK_OUTPUT_DLY;   // for E1 DDR2 only
    int   DRAMC_ACTIM1_VAL;       //@0x41E8
    int   DRAMC_MISCTL0_VAL;      //@0x40FC
    int   DRAMC_ACTIM05T_VAL;     //@0x41F8
    int   DRAMC_RKCFG_VAL;
    int   DRAMC_TEST2_4_VAL;    
    unsigned long long   DRAM_RANK_SIZE[4];
    int   reserved[10];
    union
    {
        struct
        {
            int   iLPDDR2_MODE_REG_1;
            int   iLPDDR2_MODE_REG_2;
            int   iLPDDR2_MODE_REG_3;
            int   iLPDDR2_MODE_REG_5;
            int   iLPDDR2_MODE_REG_10;
            int   iLPDDR2_MODE_REG_63;
        };
        struct
        {
            int   iDDR1_MODE_REG;
            int   iDDR1_EXT_MODE_REG;
        };
        struct
        {
            int   iPCDDR3_MODE_REG0;
            int   iPCDDR3_MODE_REG1;
            int   iPCDDR3_MODE_REG2;
            int   iPCDDR3_MODE_REG3;
        };
        struct
        {
            int   iLPDDR3_MODE_REG_1;
            int   iLPDDR3_MODE_REG_2;
            int   iLPDDR3_MODE_REG_3;
            int   iLPDDR3_MODE_REG_5;
            int   iLPDDR3_MODE_REG_10;
            int   iLPDDR3_MODE_REG_63;
        };
    };
} EMI_SETTINGS;

extern int num_of_emi_records;
extern int emi_setting_index;
extern EMI_SETTINGS emi_settings[];
extern EMI_SETTINGS *default_emi_setting;
extern EMI_SETTINGS emi_setting_default_lpddr2;
extern EMI_SETTINGS emi_setting_default_lpddr3;  


/* Vdram :for jade minus PMIC change to MT6353*/
#if defined (PMIC_CHIP_MT6353)
#define Vdram_REG MT6353_SLDO_ANA_CON5 
#define Vcore_REG_SW MT6353_BUCK_VCORE_VOL_CON1	
#define Vcore_REG_HW MT6353_BUCK_VCORE_VOL_CON2	
#define RG_VDRAM_VOSEL_1p24V	(0x0<<8)//1.24V
#define VDRAM_ANA_CON0_SUB20mV	0x1
#define VDRAM_ANA_CON0_SUB80mV	0x4
#define VDRAM_ANA_CON0_ADD60mV	0Xd

#define Vdram_NV_default (RG_VDRAM_VOSEL_1p24V | VDRAM_ANA_CON0_SUB20mV)		//1.22V
#define Vdram_HV (RG_VDRAM_VOSEL_1p24V | VDRAM_ANA_CON0_ADD60mV)	//1.30V
#define Vdram_NV Vdram_NV_default
#define Vdram_LV (RG_VDRAM_VOSEL_1p24V | VDRAM_ANA_CON0_SUB80mV)		//1.16V

#else //#if defined (PMIC_CHIP_MT6353)
#define Vdram_REG MT6351_VDRAM_ANA_CON0
#define Vcore_REG_SW MT6351_BUCK_VCORE_CON4
#define Vcore_REG_HW MT6351_BUCK_VCORE_CON5
#define RG_VDRAM_VOSEL_1p2V	(0x5<<8)//1.2V
#define VDRAM_ANA_CON0_ADD20mV	0x1e
#define VDRAM_ANA_CON0_SUB40mV	0x4
#define VDRAM_ANA_CON0_ADD100mV	0X16

#define Vdram_NV_default (RG_VDRAM_VOSEL_1p2V | VDRAM_ANA_CON0_ADD20mV)		//1.22V
#define Vdram_HV (RG_VDRAM_VOSEL_1p2V | VDRAM_ANA_CON0_ADD100mV)	//1.30V
#define Vdram_NV Vdram_NV_default
#define Vdram_LV (RG_VDRAM_VOSEL_1p2V | VDRAM_ANA_CON0_SUB40mV)		//1.16V

#endif //#if defined (PMIC_CHIP_MT6353)


//#define DRAM_HQA

#ifdef DRAM_HQA
//#define HVcore1		/*Vcore1=1.10, Vdram=1.3,  Vio18=1.8*/
//#define NV			/*Vcore1=1.00, Vdram=1.22, Vio18=1.8*/
//#define LVcore1		/*Vcore1=0.90, Vdram=1.16, Vio18=1.8*/
//#define HVcore1_LVdram	/*Vcore1=1.10, Vdram=1.16, Vio18=1.8*/
//#define LVcore1_HVdram	/*Vcore1=0.90, Vdram=1.3,  Vio18=1.8*/

/*Vcore :for DRAM HQA*/
#ifdef ONEPLL_TEST
#define Vcore1_HV 0x38 //0.95V
#define Vcore1_NV 0x30 //0.90V
#define Vcore1_LV 0x28 //0.85V
#else
#define Vcore1_HV 0x48 //1.05V
#define Vcore1_NV 0x40 //1.00V
#define Vcore1_LV 0x38 //0.95V
#endif //end #ifdef ONEPLL_TEST

#endif //end #ifdef DRAM_HQA

//Vcore1 for Impedance calibration
#ifdef DRAM_HQA
#if (defined (HVcore1) || defined(HVcore1_LVdram))
#define Vcore1_Before_ImpedanceK 0x40 //1.00V
#define Vcore1_After_ImpedanceK 0x48 //1.05V
#elif defined(NV)
#define Vcore1_Before_ImpedanceK 0x38 //0.95V
#define Vcore1_After_ImpedanceK 0x40 //1.00V
#elif (defined (LVcore1)|| defined (LVcore1_HVdram))
#define Vcore1_Before_ImpedanceK 0x30 //0.90V
#define Vcore1_After_ImpedanceK 0x38 //0.95V
#endif
#else//DRAM_HQA
#define Vcore1_Before_ImpedanceK 0x38 //0.95V
#define Vcore1_After_ImpedanceK 0x40 //1.00V
#endif //DRAM_HQA


int mt_get_dram_type (void); 
/* 0: invalid */
/* 1: mDDR1 */
/* 2: LPDDR2 */
/* 3: LPDDR3 */
/* 4: PCDDR3 */
#endif
