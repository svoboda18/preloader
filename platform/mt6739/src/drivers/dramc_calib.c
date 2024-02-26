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

#include <dramc.h>
#include <typedefs.h>
#include <platform.h>
#include <emi_hw.h>
#include <emi.h>
#include <pll.h>
#include <wdt.h>
#define DEC_DQS_VALUE (0)
#define DEFAULT_DQSSTEP_VALUE 4
#define HW_DQ_DQS_CALIB_TIMEOUT 10000


#define DEBUG_DRAMC_CALIB
#define DEBUG
#ifdef DEBUG
#define dbg_print print
#else
#define dbg_print
#endif

#ifdef WRITE_LEVELING
unsigned int wrlevel_dqs_final_delay[4];
#endif

#ifdef fcDATLAT_SAME_CHECK
unsigned int r0_datlat_value;
//unsigned int r1_datlat_value;
#endif

#ifdef  DQ_DQS_DQM_REMAPPING        

#if LPDDR == 2 || LPDDR == 3
unsigned char Bit_DQO_Mapping[32] = 
	{0, 1, 2, 3, 8, 9,10,11, 4, 5,  6,  7, 12, 13, 14, 15, 16, 17, 18, 19, 24,25,26,27, 20, 21, 22, 23, 28,29,30,31};
//	{0, 1, 2, 3, 4, 5,  6,  7, 8, 9,10,11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,25,26,27,28,29,30,31};
unsigned char DQSO_Mapping[4] = {0, 1, 2, 3}; 	// Test
unsigned char DQM_Mapping[4] = {0, 1, 2, 3}; 
#else 
unsigned char Bit_DQO_Mapping[32] = 
	{0, 1, 2, 3, 8, 9,10,11, 4, 5,  6,  7, 12, 13, 14, 15, 16, 17, 18, 19, 24,25,26,27, 20, 21, 22, 23, 28,29,30,31};
//	{0, 1, 2, 3, 4, 5,  6,  7, 8, 9,10,11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,25,26,27,28,29,30,31};
unsigned char DQSO_Mapping[4] = {0, 1, 2, 3}; 	// Test
unsigned char DQM_Mapping[4] = {0, 1, 2, 3}; 
#endif

#endif

int RANK_CURR = 0;
extern char *opt_gw_coarse_value, *opt_gw_fine_value;
extern char *opt_gw_coarse_value0, *opt_gw_fine_value0;
extern char *opt_gw_coarse_value1, *opt_gw_fine_value1;

#ifdef fcGATING_SAME_MCK
extern char *pre_gw_coarse_value0;
extern char *pre_gw_fine_value0;
extern char *pre_gw_coarse_value1;
extern char *pre_gw_fine_value1;
#endif

extern char* opt_dle_value;
extern void dqsi_gw_dly_coarse_factor_handler(char *);
extern void dqsi_gw_dly_coarse_factor_handler_rank1(char *);
extern void dqsi_gw_dly_fine_factor_handler(char *);
extern void dqsi_gw_dly_fine_factor_handler_rank1(char *);
unsigned int opt_rx_dqs0;
unsigned int opt_rx_dqs1;
unsigned int opt_tx_dq[4];
unsigned int opt_tx_dqs;
unsigned int opt_tx_dqm;
int DQS_PRINT;
typedef struct _RXDQS_PERBIT_DLY_T
{
    S8 min_cur;
    S8 max_cur;
    S8 min_best;
    S8 max_best;
    U8 center;
    U8 dq_dly_last;
} RXDQS_PERBIT_DLY_T;
#define DQ_DATA_WIDTH 32
#define MAX_RX_DQSDLY_TAPS 128
#define MAX_RX_DQDLY_TAPS 16 
#define DQS_NUMBER 4
#define DQS_BIT_NUMBER (DQ_DATA_WIDTH/DQS_NUMBER)

#if 1
unsigned int ta2_enable_write_read(unsigned int len)
{
    unsigned int data, cmp_err;

#if 0
    unsigned int temp_addr  ;
    unsigned int test_agent_base_temp = CFG_UBOOT_MEMADDR;

    /* set test patern length*/
    test_agent_base_temp = test_agent_base_temp -0x40000000;
    temp_addr = ( 0x55000000 | ((test_agent_base_temp>>4) & 0xffffff));
    DRAMC_WRITE_REG(temp_addr,0x3C);


    data = DRAMC_READ_REG(0x38);	
    temp_addr = (( data & (~0xf)) |((test_agent_base_temp>>28) & 0xf));
    DRAMC_WRITE_REG(temp_addr,0x38);
#endif

    DRAMC_WRITE_REG(0x55000000,0x3C);
    data = DRAMC_READ_REG(0x40);
    DRAMC_WRITE_REG((data & 0xAA000000) | len, 0x40);
    //Test Agent 2 write enabling
    DRAMC_WRITE_SET((1 << 31),DRAMC_CONF2); 
    //check write finish
    while(!(DRAMC_READ_REG(DRAMC_TESTRPT)&(1 << 10)));
    //Disable write
    DRAMC_WRITE_CLEAR((1 << 31),DRAMC_CONF2);

    //Test Agent 2 read enabling
    DRAMC_WRITE_SET((1 << 30),DRAMC_CONF2); 
    //check read finish
    while(!(DRAMC_READ_REG(DRAMC_TESTRPT)&(1 << 10)));    

    delay_a_while(400);

    cmp_err = DRAMC_READ_REG(DRAMC_CMP_ERR);
    //Disable read
    DRAMC_WRITE_CLEAR((1 << 30),DRAMC_CONF2); //disable test agent2 read


    return cmp_err;
}
#endif

/* Description
  *	RX DQ/DQS per bit calibration.
  * Registers
  *	- DQIDLY[1:8] : one register had 4 settings (4bits: 0~15, unit 20ps) with corresponding DQx
  *	- R0DELDLY : 4 settings for rank 0 DQS0~DQS3. 7 bits (0~127) with unit 30ps. 
  *	- R1DELDLY : 4 settings for rank 1 DQS0~DQS3. 7 bits (0~127) with unit 30ps.
  * Algorithm
  *	- Set DQS/DQ input delay to 0.
  *	- Delay all DQs from 0 to 15 until all failed.
  *	- Delay DQSs from 0 to 127 to find the pass range (min & max) of each DQ. Further find the largest pass range.
  *	- For each DQS, find the largest middle value of corresponding DQ byte. Then use this value to set each DQS input delay.
  *	- For each DQ, find the difference between original middle DQS delay and max DQS delay per byte. Then delay the difference more to align the middle of DQS per byte.
  */
int do_sw_rx_dq_dqs_calib(void)
{
    int result;
    unsigned int data, backup;
    int temp, timeout;
    unsigned int dqsi_dly0, dqsi_dly1, dqsi_dly2, dqsi_dly3;
    unsigned int test_len = 0x100;
    unsigned int dqidly1, dqidly2, dqidly3, dqidly4,dqidly5,dqidly6,dqidly7;
    unsigned int i,j;
    unsigned int dqs_input_delay;
    unsigned int cmp_err;
    unsigned int max;

    unsigned int dq_dly_max;
    char dqs_delay[DQS_NUMBER];
    char dq_delay_per_bit[DQ_DATA_WIDTH];
    unsigned int dqidly[DQ_DATA_WIDTH/DQS_NUMBER];
    unsigned int dq_tap;
    unsigned int dq_delay_done[DQ_DATA_WIDTH];
    RXDQS_PERBIT_DLY_T dqs_perbit_dly[DQ_DATA_WIDTH];
    result = 0;

    dbg_print("in do_sw_rx_dq_dqs_calib()\n");

#ifndef RELEASE
    dbg_print("*DQIDLY1 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY1));
    dbg_print("*DQIDLY2 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY2));
    dbg_print("*DQIDLY3 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY3));
    dbg_print("*DQIDLY4 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY4));
    dbg_print("*DQIDLY5 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY5));
    dbg_print("*DQIDLY6 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY6));
    dbg_print("*DQIDLY7 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY7));
    dbg_print("*DQIDLY8 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY8));
    dbg_print("*DRAMC_R0DELDLY = 0x%x\n", DRAMC_READ_REG(DRAMC_R0DELDLY));


#endif


    /*1. set DQS delay to 0 first*/
  
        DRAMC_WRITE_REG(0x0,DRAMC_R0DELDLY);
   

    // set DQ delay to 0x0.
    for (i = 0; i < 8; i++)
    {
        DRAMC_WRITE_REG(0x0,DRAMC_DQIDLY1+4*i);
    }

    // set DQ delay structure to 0x0.
    for (i = 0; i < DQ_DATA_WIDTH; i++)
    {
        dq_delay_per_bit[i] = 0x0; 
        dq_delay_done[i] = 0x0;
    }

    // delay DQ to find all failed 
    for(dq_tap = 0 ; dq_tap < MAX_RX_DQDLY_TAPS; dq_tap++ ){
#if 0
        /* set test patern length*/
        DRAMC_WRITE_REG(0x55000000,0x3C);
        data = DRAMC_READ_REG(0x40);
        DRAMC_WRITE_REG((data & 0xAA000000) | test_len, 0x40);
        //Test Agent 2 write enabling, Test Agent 2 read enabling
        DRAMC_WRITE_SET((1 << 30) | (1 << 31),DRAMC_CONF2); 
        
        while(!(DRAMC_READ_REG(DRAMC_TESTRPT)&(1 << 10)));

        delay_a_while(400);

        cmp_err = DRAMC_READ_REG(DRAMC_CMP_ERR);        
        DRAMC_WRITE_CLEAR(((1 << 30) | (1 << 31)),DRAMC_CONF2); //disable test agent2 r/w        
#else
        cmp_err = ta2_enable_write_read(test_len);        
#endif
        //dbg_print("cmp_err:%x\n",cmp_err);

        if (cmp_err == 0xFFFFFFFF) break; 


	/* Bit i compare result
	  * 	-Compare success & never fail before, record the delay value. (dq_delay_per_bit[i] = delay value)
	  *	-Compare fail. Record fail. (dq_delay_done[i] = 1)
           */

        for (i = 0; i < DQ_DATA_WIDTH; i++)
        {
            if (!(cmp_err&(0x1<<i)) && dq_delay_done[i] == 0)
            {
                dq_delay_per_bit[i] = dq_tap; 
            }
            else
            {
                dq_delay_done[i] = 1;
            }
                //dbg_print("%d)0x%x \n",i,dq_delay_per_bit[i]);
        }
        //dbg_print("\n");

        for (i = 0; i < DQ_DATA_WIDTH; i+=4)
        {
            dqidly[i/4] = (dq_delay_per_bit[i]) + (dq_delay_per_bit[i+1] << 8) + (dq_delay_per_bit[i+2] << 16) + (dq_delay_per_bit[i+3] << 24);

            //dbg_print("dqidly[%d]=0x%x\n",i/4,dqidly[i/4]);
        }
        
        for (i = 0; i < 8; i++)
        {
            DRAMC_WRITE_REG(dqidly[i],DRAMC_DQIDLY1+4*i);
        }
#if 0
        dbg_print("*DQIDLY1 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY1));
        dbg_print("*DQIDLY2 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY2));
        dbg_print("*DQIDLY3 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY3));
        dbg_print("*DQIDLY4 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY4));
        dbg_print("*DQIDLY5 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY5));
        dbg_print("*DQIDLY6 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY6));
        dbg_print("*DQIDLY7 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY7));
        dbg_print("*DQIDLY8 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY8));
#endif
    }
    // After loop, dq_delay_per_bit[0:31] value non-zero mean the last valid settings when DQS input delay is 0. dq_delay_per_bit[0:31] value 0 means it is already  failed when DQS input delay is 0. Also, current DQIDLY[1:8] settings is the setting of dq_delay_per_bit[0:31].
    // We got the dq input delay in dq_delay_per_bit[i]
    /* 2. initialize parameters */
    for (i = 0; i < DQ_DATA_WIDTH; i++)
    {
        dqs_perbit_dly[i].min_cur = -1;
        dqs_perbit_dly[i].max_cur = -1;
        dqs_perbit_dly[i].min_best = -1;
        dqs_perbit_dly[i].max_best = -1;
        dqs_perbit_dly[i].center = 0;
        dqs_perbit_dly[i].dq_dly_last = dq_delay_per_bit[i];
    }
    /* find the minimum and maximum DQS input delay*/
    for (i = 0; i < MAX_RX_DQSDLY_TAPS; i++)
    {
        dqs_input_delay = (i) + (i << 8) + (i << 16) + (i << 24);
        
        DRAMC_WRITE_REG(dqs_input_delay,DRAMC_R0DELDLY);

#if 0
        /* set test patern length*/
        data = DRAMC_READ_REG(0x40);
        DRAMC_WRITE_REG((data & 0xFF000000) | test_len, 0x40);
        //Test Agent 2 write enabling, Test Agent 2 read enabling
        DRAMC_WRITE_SET((1 << 30) | (1 << 31),DRAMC_CONF2); 
        
        while(!(DRAMC_READ_REG(DRAMC_TESTRPT)&(1 << 10)));

        delay_a_while(400);

        cmp_err = DRAMC_READ_REG(DRAMC_CMP_ERR);
        DRAMC_WRITE_CLEAR(((1 << 30) | (1 << 31)),DRAMC_CONF2); //disable test agent2 r/w
#else
        cmp_err = ta2_enable_write_read(test_len);
#endif 

	/* if bit x test pass the first time, record to min input delay. (dqs_per_bit[x].min_cur = delay value.)
	  * If bit x already had min value and no max value and pass fail => max value is this delay-1. (dqs_per_bit[x].max_cur = delay value-1)
	  * If bit x already had min value and no max value and pass and delay value = 127 => max value = 127 (dqs_per_bit[x].max_cur = 127)
           */
        
        for (j = 0; j < DQ_DATA_WIDTH; j++)
        {
            if ((dqs_perbit_dly[j].min_cur == -1) && ((cmp_err&((U32)1<<j)) == 0x0))
            {
                // min pass delay
                dqs_perbit_dly[j].min_cur = i;
            }
            if ((dqs_perbit_dly[j].min_cur != -1) && (dqs_perbit_dly[j].max_cur == -1) && (((cmp_err&((U32)1<<j)) != 0x0) || (i == (MAX_RX_DQSDLY_TAPS-1))) )
            {
                // we get the dqs_perbit_dly pass max
                if ((i == (MAX_RX_DQSDLY_TAPS-1)) && ((cmp_err&((U32)1<<j)) == 0x0))
                {
                    dqs_perbit_dly[j].max_cur = MAX_RX_DQSDLY_TAPS-1;
                }
                else
                {
                    dqs_perbit_dly[j].max_cur = i - 1;
                }

                // there may be more than 1 pass range, find the max range
                // ex: x00xxxxxx00000000000000xx...(get the second one)
                if ((dqs_perbit_dly[j].max_cur-dqs_perbit_dly[j].min_cur) > (dqs_perbit_dly[j].max_best-dqs_perbit_dly[j].min_best))
                {
                    dqs_perbit_dly[j].max_best = dqs_perbit_dly[j].max_cur;
                    dqs_perbit_dly[j].min_best = dqs_perbit_dly[j].min_cur;
                }
                // clear to find the next pass range if it has
                dqs_perbit_dly[j].max_cur = -1;
                dqs_perbit_dly[j].min_cur = -1;
            }

        }
    }
    // 3
    // get dqs delay center per bit
    for (j = 0; j < DQ_DATA_WIDTH; j++)
    {
        if ((dqs_perbit_dly[j].max_best != -1) && (dqs_perbit_dly[j].min_best != -1))
        {
            dqs_perbit_dly[j].center = (dqs_perbit_dly[j].max_best + dqs_perbit_dly[j].min_best) / 2;
            //dbg_print("dqs_perbit_dly[%d].center=0x%x\n",j,dqs_perbit_dly[j].center);
        }
    }

    // we get the delay value of the 4 DQS (min of center)
    for (i = 0; i < DQS_NUMBER; i++)
    {
        max = 0;
        // find the max of center
        for (j = 0; j < DQS_BIT_NUMBER; j++)
        {
            if (dqs_perbit_dly[i*DQS_BIT_NUMBER+j].center > max)
            {
                max = dqs_perbit_dly[i*DQS_BIT_NUMBER+j].center;
            }
        }
        // save dqs delay
        dqs_delay[i] = max;
        //dbg_print("dqs_delay[%d]=0x%x\n",i,max);
    }
    data = ((U32) dqs_delay[0]) + (((U32)dqs_delay[1])<<8) + (((U32)dqs_delay[2])<<16) + (((U32)dqs_delay[3])<<24);
    /*set dqs input delay*/
    DRAMC_WRITE_REG(data,DRAMC_R0DELDLY);


    // delay DQ ,let dqsdly_ok_center == DQS_delay
    for (i = 0; i < DQ_DATA_WIDTH; i = i+4)
    {
        // every 4-bit dq have the same delay register address
        // dq_dly_max: taps for dq delay to be add
        for (j = 0; j < 4; j++)
        {
            dq_dly_max =  dqs_delay[i/DQS_BIT_NUMBER] - dqs_perbit_dly[i+j].center;
            //dbg_print("1.bit:%d)dq_per_bit_dly:0x%x,dq_dly:0x%x\n",i+j,dqs_perbit_dly[i+j].dq_dly_last,dq_dly_max);
            data = dqs_perbit_dly[i+j].dq_dly_last + dq_dly_max;
            data = ((data > (MAX_RX_DQDLY_TAPS-1)) ? (MAX_RX_DQDLY_TAPS-1) : data);
            dqs_perbit_dly[i+j].dq_dly_last = data;

            //dbg_print("2.bit:%d)dq_per_bit_dly:0x%x\n",i+j,dqs_perbit_dly[i+j].dq_dly_last);
        }

        data = ((U32) dqs_perbit_dly[i].dq_dly_last) + (((U32)dqs_perbit_dly[i+1].dq_dly_last)<<8) + (((U32)dqs_perbit_dly[i+2].dq_dly_last)<<16) + (((U32)dqs_perbit_dly[i+3].dq_dly_last)<<24);

        DRAMC_WRITE_REG(data,DRAMC_DQIDLY1+i);
    }
#if 0
    for (j = 0; j < DQ_DATA_WIDTH; j++)
    {
        dbg_print("%d)min:0x%x,max:0x%x\n",j, dqs_perbit_dly[j].min_best, dqs_perbit_dly[j].max_best);
    }
#endif
#if 0 //defined(DEBUG_DRAMC_CALIB)
    print("*DQIDLY1 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY1));
    print("*DQIDLY2 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY2));
    print("*DQIDLY3 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY3));
    print("*DQIDLY4 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY4));
    print("*DQIDLY5 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY5));
    print("*DQIDLY6 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY6));
    print("*DQIDLY7 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY7));
    print("*DQIDLY8 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY8));
    print("*DRAMC_R0DELDLY = 0x%x\n", DRAMC_READ_REG(DRAMC_R0DELDLY));
#endif

#if defined(DEBUG_DRAMC_CALIB)
    print("*DQIDLY1 = 0x%x\n", DRAMC_READ_REG(DRAMC_DQIDLY1));
    // finish we can put result now .
    print("==================================================================\n");
    print("		RX	DQS perbit delay software calibration \n");
    print("==================================================================\n");        
    print("1.0-31 bit dq delay value\n");
    print("==================================================================\n");        
    print("bit|     0  1  2  3  4  5  6  7  8  9\n");
    print("--------------------------------------");
    for (i = 0; i < DQ_DATA_WIDTH; i++)
    {
        j = i / 10;
        if (i == (j*10))
        {
            print("\n");
            print("%d |    ", i);
        }
        print("%d ", dq_delay_per_bit[i]);
    }
    print("\n--------------------------------------\n\n");
    print("==================================================================\n");
    print("2.dqs window\nx=pass dqs delay value (min~max)center \ny=0-7bit DQ of every group\n");
    print("input delay:DQS0 =%d DQS1 = %d DQS2 =%d DQS3 = %d\n", dqs_delay[0], dqs_delay[1], dqs_delay[2], dqs_delay[3]);
    print("==================================================================\n");
    print("bit	DQS0	 bit      DQS1     bit     DQS2     bit     DQS3\n");
    for (i = 0; i < DQS_BIT_NUMBER; i++)
    {
        print("%d  (%d~%d)%d  %d  (%d~%d)%d  %d  (%d~%d)%d  %d  (%d~%d)%d\n", \
            i,    dqs_perbit_dly[i].min_best, dqs_perbit_dly[i].max_best, dqs_perbit_dly[i].center, \
            i+8,  dqs_perbit_dly[i+8].min_best, dqs_perbit_dly[i+8].max_best, dqs_perbit_dly[i+8].center, \
            i+16, dqs_perbit_dly[i+16].min_best, dqs_perbit_dly[i+16].max_best, dqs_perbit_dly[i+16].center, \
            i+24, dqs_perbit_dly[i+24].min_best, dqs_perbit_dly[i+24].max_best, dqs_perbit_dly[i+24].center);
    }
    print("==================================================================\n");
    print("3.dq delay value last\n");
    print("==================================================================\n");
    print("bit|    0  1  2  3  4  5  6  7  8   9\n");
    print("--------------------------------------");
    for (i = 0; i < DQ_DATA_WIDTH; i++)
    {
        j = i / 10;
        if (i == (j*10))
        {
            print("\n");
            print("%d |    ", i);
        }
        print("%d ", dqs_perbit_dly[i].dq_dly_last);
    }
    print("\n==================================================================\n");

#endif

    return 0;
}

typedef struct _TXDQS_PERBYTE_DLY_T
{
    S8 first_dqdly_pass;
    S8 last_dqdly_pass;
    S8 total_dqdly_pass;
    S8 first_dqsdly_pass;
    S8 last_dqsdly_pass;
    S8 total_dqsdly_pass;
    U8 best_dqdly;
    U8 best_dqsdly;
    U8 dq;
    U8 dqs;
} TXDQS_PERBYTE_DLY_T;
static void  calib_clk_output_dly_factor_handler(unsigned int value)
{
        unsigned int data;
        //adjust CLK output delay
        data = DRAMC_READ_REG(DRAMC_PADCTL1);
        data = (data & 0xF0FFFFFF) | (value << 24);
        DRAMC_WRITE_REG(data,DRAMC_PADCTL1);
}

static void  calib_cmd_addr_output_dly_factor_handler(unsigned int value)
{
        unsigned int data;
        //adjust CMD delay
        data = DRAMC_READ_REG(DRAMC_CMDDLY0);
        data &= 0xE0E0E0E0;
        data |= (value  << 24) | (value << 16) | (value << 8) | value;
        DRAMC_WRITE_REG(data, DRAMC_CMDDLY0);

        data = DRAMC_READ_REG(DRAMC_CMDDLY1);
        data &= 0xE0E0E0E0;
        data |= (value  << 24) | (value << 16) | (value << 8) | value;
        DRAMC_WRITE_REG(data, DRAMC_CMDDLY1);

        data = DRAMC_READ_REG(DRAMC_CMDDLY2);
        data &= 0xE0E0E0E0;
        data |= (value  << 24) | (value << 16) | (value << 8) | value;
        DRAMC_WRITE_REG(data, DRAMC_CMDDLY2);

        data = DRAMC_READ_REG(DRAMC_CMDDLY3);
        data &= 0xE0E0E0E0;
        data |= (value  << 24) | (value << 16) | (value << 8) | value;
        DRAMC_WRITE_REG(data, DRAMC_CMDDLY3);

        data = DRAMC_READ_REG(DRAMC_CMDDLY4);
        data &= 0xE0E0E0E0;
        data |= (value  << 24) | (value << 16) | (value << 8) | value;
        DRAMC_WRITE_REG(data, DRAMC_CMDDLY4);

        data = DRAMC_READ_REG(DRAMC_CMDDLY5);
        data &= 0xE0E0E0EF;
        data |= (value << 24) | (value << 16) | (value << 8);
        DRAMC_WRITE_REG(data, DRAMC_CMDDLY5);

        data = DRAMC_READ_REG(DRAMC_DQSCAL0);
        data &= 0xF0F0FFFF;
        data |= (value << 24) | (value << 16);
        DRAMC_WRITE_REG(data, DRAMC_DQSCAL0);

}


#define MAX_TX_DQDLY_TAPS 16 
#define MAX_TX_DQSDLY_TAPS 16 
#define TX_STEPS_LPDDR2 0   // This value is obtained by measurement of waveform for the relationship between DQS and CLK.
#define TX_STEPS_DDR3 0
#define TX_STEPS 0

/* Description
  *	TX DQ/DQS per byte calibration
  * Related registers
  *	- PADCTL3 : DQS[0:3]ODLY (0x10011014)
  *		- 4 bits (0~15)
  *		- 1 step 20ps.
  *	- PADCTL2 : DQM[0:3]DLY (0x10011010)
  *		- 4 bits (0~15)
  *		- 1 step 20ps.
  *	- DQODLY[1:4] : DQ[0:31]DLY (0x10011200~0x1001120c)
  *		- 4 bits (0~15)
  *		- 1 step 20ps
  *	- DRAMC_PADCTL1 
  * 		- bit 27~bit 24 : clock delay.
  *	- DRAMC_CMDDLY[0:5], DRAMC_DQSCAL0
  *		- Address/cmd delay
  * Algorithm
  *	- Set DQ/DQM output delay to 0 and loop DQS output delay value from 0 to 15. Check the pass range.
  *	- Set DQS output delay value to 0 and loop DQ/DQM output delay value from 0 to 15. Check the pass range.
  *	- If two pass range equal, set both output delay to 8.
  *	- If not equal, add (delay difference)/2 to the one with larger pass range.
  */
int do_sw_tx_dq_dqs_calib(void){
    TXDQS_PERBYTE_DLY_T dqs_perbyte_dly[4];
#ifdef TX_PERBIT_WIN    
    TXDQS_PERBYTE_DLY_T tx_perbit_dly[32];
    unsigned int uiBit;
    unsigned int ucmax_dqsdly_byte[DQS_NUMBER], ucdqmdly_byte[DQS_NUMBER];
#endif
    unsigned int data;
    unsigned int byte;
    int i,j;
    unsigned int mask;
    unsigned int test_len = 0x3FF;
    unsigned int cmp_err_1,cmp_err_2;
    unsigned int dq,dqs;
    unsigned int finish_count, uiWinSize=0;
    unsigned int tx_steps;

    if((*(V_UINT32P)(DRAMC0_BASE + (0x00e4)) >> 7) & 0x1) {
        tx_steps = TX_STEPS_DDR3;
    } else {
        tx_steps = TX_STEPS_LPDDR2;
    }
     

 
    dbg_print("in do_sw_tx_dq_dqs_calib()\n");
    for(i = 0; i < 4; i++)
    {
        //dqs_perbyte_dly[i].first_dqdly_pass = -1; 
        dqs_perbyte_dly[i].first_dqdly_pass = 0; 
        dqs_perbyte_dly[i].last_dqdly_pass = -1; 
        //dqs_perbyte_dly[i].first_dqsdly_pass = -1; 
        dqs_perbyte_dly[i].first_dqsdly_pass = 0; 
        dqs_perbyte_dly[i].last_dqsdly_pass = -1; 
        dqs_perbyte_dly[i].best_dqdly= -1; 
        dqs_perbyte_dly[i].best_dqsdly= -1; 
        dqs_perbyte_dly[i].dq= 0; 
        dqs_perbyte_dly[i].dqs= 0; 
        dqs_perbyte_dly[i].total_dqdly_pass= 0; 
        dqs_perbyte_dly[i].total_dqsdly_pass= 0; 
    }

#ifdef TX_PERBIT_WIN    
    for (i=0; i<32; i++) {
        tx_perbit_dly[i].first_dqdly_pass = 0; 
        tx_perbit_dly[i].last_dqdly_pass = -1; 
        tx_perbit_dly[i].first_dqsdly_pass = 0; 
        tx_perbit_dly[i].last_dqsdly_pass = -1; 
        tx_perbit_dly[i].total_dqdly_pass= 0; 
        tx_perbit_dly[i].total_dqsdly_pass= 0; 
     }
#endif

    // used for early break
    finish_count = 0;
    
    /* 1. Tx DQS/DQ all zero*/
    /* 1.1 DQM*/
    //DRAMC_WRITE_CLEAR(0x0000FFFF,DRAMC_PADCTL3);
    DRAMC_WRITE_REG(0x0,DRAMC_PADCTL2);
    /* 1.2 DQ*/
    for (i = 0 ; i < 4; i ++)
    {
        //dbg_print("addr:%x\n",DRAMC_DQODLY1+4*i);
        DRAMC_WRITE_REG(0x0, (DRAMC_DQODLY1+4*i)); 
        
    }
    //for (byte = 0; byte < 4; byte ++)
    //{
        /*for (i = 0 ; i < 4; i ++)
        {
            //dbg_print("addr:%x\n",DRAMC_DQODLY1+4*i);
            DRAMC_WRITE_REG(0x0, (DRAMC_DQODLY1+4*i)); 

        }*/
        //dqs_perbyte_dly[byte].first_dqsdly_pass = 0;
        //dbg_print("dqs_perbyte_dly.first_dqsdly_pass=%x \n",dqs_perbyte_dly[byte].first_dqsdly_pass);
        //dqs_perbyte_dly[byte].last_dqsdly_pass =  MAX_TX_DQSDLY_TAPS - 1;
        /* 2. fix DQ delay = 0, delay DQS to find the pass range  */
        for (i = MAX_TX_DQSDLY_TAPS-1; i >= 0 ; i--)
        {
            data = DRAMC_READ_REG(DRAMC_PADCTL3);
            mask = (0xFFFF0000);
            data = (data & mask) | ((i <<0) | (i<<4) | (i <<8) | (i <<12));
            //dbg_print("mask:%x, data:%x\n",mask,data);
            DRAMC_WRITE_REG(data,DRAMC_PADCTL3);

#ifdef ENABLE_TX_CMD_CLK_SHIFT
	    if(mt_get_dram_type() != TYPE_PCDDR3)
            {//PCDDR3 no need 
                //adjust CMD addr output delay
                calib_cmd_addr_output_dly_factor_handler(i);
                //adjust CLK delay
                calib_clk_output_dly_factor_handler(i);
                //gating window calibration
                #ifdef GW_TA2                
                    do_dqs_gw_calib_2();        
                #else
                    //do_dqs_gw_calib_1();
                #endif
            }
#endif
            /* 2.2 use test agent to find the pass range */
#if 0
            /* set test patern length*/
            DRAMC_WRITE_REG(0x55000000,0x3C);
            data = DRAMC_READ_REG(0x40);
            DRAMC_WRITE_REG((data & 0xAA000000) | test_len, 0x40);

            //Test Agent 2 write enabling
            DRAMC_WRITE_SET( (1 << 31),DRAMC_CONF2); //enable write
            DRAMC_WRITE_SET( (1 << 30)| (1 << 31),DRAMC_CONF2); //enable read
            while(!(DRAMC_READ_REG(DRAMC_TESTRPT)&(1 << 10)));
            delay_a_while(400);
            cmp_err_1 = DRAMC_READ_REG(DRAMC_CMP_ERR);
            DRAMC_WRITE_CLEAR(((1 << 30) | (1 << 31)),DRAMC_CONF2); //disable test agent2 r/w
#else
            cmp_err_1 = ta2_enable_write_read(test_len);
#endif
            print("DQS loop = %d, cmp_err_1 = %x \n",i, cmp_err_1);

            for (byte=0; byte < 4; byte++)
            {
                if (dqs_perbyte_dly[byte].last_dqsdly_pass == -1)
                {
                    if (!((cmp_err_1) & (0xFF<<(byte*8))))
                    {
                        dqs_perbyte_dly[byte].last_dqsdly_pass = i;
                        finish_count++;
                        //print("dqs_perbyte_dly.last_dqsdly_pass[%d]=%d,  finish count=%d \n",byte, dqs_perbyte_dly[byte].last_dqsdly_pass, finish_count);                        
                        //break;
                    }
                }
#ifdef TX_PERBIT_WIN    
	      for (uiBit=0; uiBit<8; uiBit++) 
	      {
	          if (tx_perbit_dly[(byte<<3)+uiBit].last_dqsdly_pass == -1)
	          {
	              if (!((cmp_err_1) & (0x01<<((byte<<3)+uiBit))))
	              {
	                  tx_perbit_dly[(byte<<3)+uiBit].last_dqsdly_pass = i;
	              }
	          }
	      }
#endif 
            }

#ifndef TX_PERBIT_WIN    
            if (finish_count==4)
            {
                break;
            }
#endif
        }
        
        /* 3. fix DQS delay = 0, delay DQ to find the pass range  */
        DRAMC_WRITE_CLEAR(0xFFFF ,DRAMC_PADCTL3);

#ifdef ENABLE_TX_CMD_CLK_SHIFT
        if(mt_get_dram_type() != TYPE_PCDDR3)
        {//PCDDR3 no need 
            //adjust CMD addr output delay
            calib_cmd_addr_output_dly_factor_handler(0);
            //adjust CLK delay
            calib_clk_output_dly_factor_handler(0);
            //gating window calibration
            #ifdef GW_TA2                
                do_dqs_gw_calib_2();        
            #else
                //do_dqs_gw_calib_1();
            #endif
        }
#endif

        //dqs_perbyte_dly[byte].first_dqdly_pass = 0;
        //dbg_print("dqs_perbyte_dly.first_dqdly_pass=%x \n",dqs_perbyte_dly[byte].first_dqdly_pass);

        finish_count = 0;
        
        for (i = MAX_TX_DQDLY_TAPS-1; i >= 0; i--)
        {

            /* 3.1 delay DQ output delay */
            data = 
                ((i & 0xF) << 0)       /* DQM0DLY: DRAMC_PADCTL2[0:3],         4 bits */
              | ((i & 0xF) << 4)       /* DQM1DLY: DRAMC_PADCTL2[4:7], 4 bits */
              | ((i & 0xF) << 8)       /* DQM2DLY: DRAMC_PADCTL2[8:11],        4 bits */
              | ((i & 0xF) << 12);
            DRAMC_WRITE_REG(data , DRAMC_PADCTL2/* 0x10 */);
            data =    
                ((i & 0xF) << 0)             /* DQ0DLY: DRAMC_DQODLY1[0:3],          4 bits */
              | ((i & 0xF) << 4)               /* DQ1DLY: DRAMC_DQODLY1[4:7],          4 bits */
              | ((i & 0xF) << 8)               /* DQ2DLY: DRAMC_DQODLY1[8:11],         4 bits */
              | ((i & 0xF) << 12)      /* DQ3DLY: DRAMC_DQODLY1[12:15],        4 bits */
              | ((i & 0xF) << 16)      /* DQ4DLY: DRAMC_DQODLY1[16:19],        4 bits */
              | ((i & 0xF) << 20)      /* DQ5DLY: DRAMC_DQODLY1[20:23],        4 bits */
              | ((i & 0xF) << 24)      /* DQ6DLY: DRAMC_DQODLY1[24:27],        4 bits */
              | ((i & 0xF) << 28); 
            DRAMC_WRITE_REG(data,DRAMC_DQODLY1);
            DRAMC_WRITE_REG(data,DRAMC_DQODLY2);
            DRAMC_WRITE_REG(data,DRAMC_DQODLY3);
            DRAMC_WRITE_REG(data,DRAMC_DQODLY4);


            //dbg_print("%x = %x\n",DRAMC_DQODLY1+(4*byte),DRAMC_READ_REG(DRAMC_DQODLY1+(4*byte)));
            /* 3.2 use test agent to find the pass range */
#if 0
            /* set test patern length*/
            DRAMC_WRITE_REG(0x55000000,0x3C);
            data = DRAMC_READ_REG(0x40);
            DRAMC_WRITE_REG((data & 0xAA000000) | test_len, 0x40);
            //Test Agent 2 write enabling
            DRAMC_WRITE_SET( (1 << 31),DRAMC_CONF2); //enable write
            DRAMC_WRITE_SET( (1 << 30)| (1 << 31),DRAMC_CONF2); //enable read
            while(!(DRAMC_READ_REG(DRAMC_TESTRPT)&(1 << 10)));
            delay_a_while(400);

            cmp_err_1 = DRAMC_READ_REG(DRAMC_CMP_ERR);
            DRAMC_WRITE_CLEAR(((1 << 30) | (1 << 31)),DRAMC_CONF2); //disable test agent2 r/w
#else
            cmp_err_1 = ta2_enable_write_read(test_len);
#endif
            print("DQ loop=%d, cmp_err_1 = %x\n",i, cmp_err_1);
 
            for (byte=0; byte < 4; byte++)
            {
                if (dqs_perbyte_dly[byte].last_dqdly_pass == -1)
                {
                    if (!(cmp_err_1 &(0xFF<<(byte*8))))
                    {
                        dqs_perbyte_dly[byte].last_dqdly_pass = i;                        
                        finish_count++;
                        //print("dqs_perbyte_dly.last_dqdly_pass[%d]=%d,  finish count=%d \n",byte, dqs_perbyte_dly[byte].last_dqdly_pass, finish_count);   
                        //break;
                    }                    
                }
#ifdef TX_PERBIT_WIN    
	      for (uiBit=0; uiBit<8; uiBit++) 
	      {
	          if (tx_perbit_dly[(byte<<3)+uiBit].last_dqdly_pass == -1)
	          {
	              if (!((cmp_err_1) & (0x01<<((byte<<3)+uiBit))))
	              {
	                  tx_perbit_dly[(byte<<3)+uiBit].last_dqdly_pass = i;
	              }
	          }
	      }
#endif   
            }

#ifndef TX_PERBIT_WIN
            if (finish_count==4)
            {
                break;
            }
#endif
        }

        for (byte=0; byte < 4; byte++)
        {

        dqs_perbyte_dly[byte].total_dqsdly_pass = dqs_perbyte_dly[byte].last_dqsdly_pass - dqs_perbyte_dly[byte].first_dqsdly_pass + 1;
        //print("total_dqsdly_pass:%x\n", dqs_perbyte_dly[byte].total_dqsdly_pass);
        dqs_perbyte_dly[byte].total_dqdly_pass = dqs_perbyte_dly[byte].last_dqdly_pass - dqs_perbyte_dly[byte].first_dqdly_pass + 1;
        //dbg_print("total_dqdly_pass:%x\n", dqs_perbyte_dly[byte].total_dqdly_pass);
        /* 4. find the middle of the pass range of DQ and DQS*/
        /* 5. if the middle of the pass range is in the DQ, the delay of (DQS,DQ) is (0,DQ), 
         *    if the middle of the pass range is in the DQS the delay of (DQS,DQ) is (DQS,0)*/
        if (dqs_perbyte_dly[byte].total_dqdly_pass == dqs_perbyte_dly[byte].total_dqsdly_pass)
        {
            dqs_perbyte_dly[byte].dqs = dqs_perbyte_dly[byte].dq = tx_steps; 
        }
        else if (dqs_perbyte_dly[byte].total_dqdly_pass > dqs_perbyte_dly[byte].total_dqsdly_pass) 
        {
            dqs_perbyte_dly[byte].dqs = 0 + tx_steps; 
            dqs_perbyte_dly[byte].dq += (dqs_perbyte_dly[byte].total_dqdly_pass - dqs_perbyte_dly[byte].total_dqsdly_pass)/2 + tx_steps; 
            if (dqs_perbyte_dly[byte].dq > (MAX_TX_DQDLY_TAPS-1)) {
		dqs_perbyte_dly[byte].dq = MAX_TX_DQDLY_TAPS-1;
		dbg_print("warning:byte %d,dq:%x",byte,dqs_perbyte_dly[byte].dq);
	    }
        } 
        else
        {
            dqs_perbyte_dly[byte].dq = 0 + tx_steps;
            dqs_perbyte_dly[byte].dqs += (dqs_perbyte_dly[byte].total_dqsdly_pass - dqs_perbyte_dly[byte].total_dqdly_pass)/2 + TX_STEPS;
            if (dqs_perbyte_dly[byte].dqs > (MAX_TX_DQSDLY_TAPS-1)) {
		dqs_perbyte_dly[byte].dqs = MAX_TX_DQSDLY_TAPS-1;
		dbg_print("warning:byte %d,dqs:%x",byte,dqs_perbyte_dly[byte].dqs);
	    }
            dqs_perbyte_dly[byte].dq = 0 + tx_steps; 
        }
        print("byte:%x, (DQS,DQ)=(%x,%x)\n",byte,dqs_perbyte_dly[byte].dqs, dqs_perbyte_dly[byte].dq);
        /* 6. fix the (DQS,DQ) for this byte, find the next byte */
        uiWinSize += (dqs_perbyte_dly[byte].total_dqdly_pass+dqs_perbyte_dly[byte].total_dqsdly_pass-1);
    }
  //}

    uiWinSize = uiWinSize/4;
    dbg_print("===== Average TX window size=%d\n",uiWinSize);
        
#ifdef TX_PERBIT_WIN    
    dbg_print("Bit : TX total window size	DQ window size	DQS window size\n");
    dbg_print("============================================================================\n");
    for (byte = 0; byte < 4; byte++)
    {
    	ucmax_dqsdly_byte[byte] = ucdqmdly_byte[byte] = 0;
    }
    for (uiBit=0; uiBit<32; uiBit++) 
    {
        tx_perbit_dly[uiBit].total_dqdly_pass = tx_perbit_dly[uiBit].last_dqdly_pass - tx_perbit_dly[uiBit].first_dqdly_pass + 1;
        tx_perbit_dly[uiBit].total_dqsdly_pass = tx_perbit_dly[uiBit].last_dqsdly_pass - tx_perbit_dly[uiBit].first_dqsdly_pass + 1;
        dbg_print("%d : Total=%d	 DQ=%d	DQS=%d", 
        		uiBit,  
        		tx_perbit_dly[uiBit].total_dqdly_pass+ tx_perbit_dly[uiBit].total_dqsdly_pass-1,
        		 tx_perbit_dly[uiBit].total_dqdly_pass,
        		 tx_perbit_dly[uiBit].total_dqsdly_pass);
        if (tx_perbit_dly[uiBit].total_dqsdly_pass > tx_perbit_dly[uiBit].total_dqdly_pass) {
       		tx_perbit_dly[uiBit].best_dqdly = 0+TX_STEPS;
       		tx_perbit_dly[uiBit].best_dqsdly = (tx_perbit_dly[uiBit].total_dqsdly_pass-tx_perbit_dly[uiBit].total_dqdly_pass)/2+TX_STEPS;
       		if (tx_perbit_dly[uiBit].best_dqsdly > ucmax_dqsdly_byte[uiBit>>3]) {
       			ucmax_dqsdly_byte[uiBit>>3] = tx_perbit_dly[uiBit].best_dqsdly;
       		}
        } else if (tx_perbit_dly[uiBit].total_dqdly_pass > tx_perbit_dly[uiBit].total_dqsdly_pass) {
         	tx_perbit_dly[uiBit].best_dqsdly = 0+TX_STEPS;
         	tx_perbit_dly[uiBit].best_dqdly = (tx_perbit_dly[uiBit].total_dqdly_pass - tx_perbit_dly[uiBit].total_dqsdly_pass)/2+TX_STEPS;
        } else {
         	tx_perbit_dly[uiBit].best_dqsdly = 0+TX_STEPS;
        	tx_perbit_dly[uiBit].best_dqdly = 0+TX_STEPS;
        }
        dbg_print("   Best	 DQ=%d	DQS=%d\n", tx_perbit_dly[uiBit].best_dqdly, tx_perbit_dly[uiBit].best_dqsdly);
        
    }

    for (byte = 0; byte < 4; byte++) {
	dbg_print("DQS %d Final Delay=%d (%xh)\n",byte,ucmax_dqsdly_byte[byte], ucmax_dqsdly_byte[byte]);
    }
    
    for (uiBit=0; uiBit<32 ; uiBit++) {
    	if (tx_perbit_dly[uiBit].best_dqsdly < ucmax_dqsdly_byte[uiBit>>3]) {
    		tx_perbit_dly[uiBit].best_dqdly += (ucmax_dqsdly_byte[uiBit>>3] - tx_perbit_dly[uiBit].best_dqsdly);
    		if (tx_perbit_dly[uiBit].best_dqdly >= (MAX_TX_DQDLY_TAPS-1)) {
    			tx_perbit_dly[uiBit].best_dqdly = MAX_TX_DQDLY_TAPS-1;
    		}
    	}
	dbg_print("DQ %d Final Delay=%d (%xh)\n",uiBit,tx_perbit_dly[uiBit].best_dqdly,tx_perbit_dly[uiBit].best_dqdly);
    	ucdqmdly_byte[uiBit>>3] += tx_perbit_dly[uiBit].best_dqdly;
    }
    for (byte = 0; byte < 4 ; byte++) {
    	ucdqmdly_byte[byte] = (ucdqmdly_byte[byte]/8);
	dbg_print("DQM %d Final Delay=%d(%xh)\n", byte, ucdqmdly_byte[byte], ucdqmdly_byte[byte]);
    }

#ifdef WRITE_LEVELING
    for (byte=0; byte<4 ; byte++) 
    {
    //#ifdef DQ_DQS_DQM_REMAPPING
    //    byte_remap = DQSO_Mapping[byte];
    //#else
    //    byte_remap = byte;
    //#endif
        if (ucmax_dqsdly_byte[byte] <= wrlevel_dqs_final_delay[byte])
        {
            dbg_print("DQSO %d in TX per-bit = %d <= DQSO %d in WL = %d  \n", 
                byte, ucmax_dqsdly_byte[byte], byte, wrlevel_dqs_final_delay[byte]);
                        
            // DQ
        //#ifdef DQ_DQS_DQM_REMAPPING
        //    for (uiBit=Bit_DQO_Mapping[8*byte]; uiBit<=Bit_DQO_Mapping[8*byte+7]; uiBit++)
        //#else
            for (uiBit=8*byte; uiBit<=8*byte+7; uiBit++)
        //#endif
            {
                tx_perbit_dly[uiBit].best_dqdly += (wrlevel_dqs_final_delay[byte] - ucmax_dqsdly_byte[byte]);
            }
            // DQM
            ucdqmdly_byte[byte] += (wrlevel_dqs_final_delay[byte] - ucmax_dqsdly_byte[byte]);

            // DQS
            ucmax_dqsdly_byte[byte] = wrlevel_dqs_final_delay[byte];
        }
        else
        {
            // ucmax_dqsdly_byte[byte] > wrlevel_dqs_final_delay[byte])
            // Originally should move clk delay and CA delay accordingly. Then GW calibration again. Too complicated.
            // DQ/DQS skew should not be large according to DE. So sacrifice the Clk/DQS margin by keeping the clk out delay.
            dbg_print("[Warning] DQSO %d in TX per-bit = %d > DQSO %d in WL = %d  \n", 
                byte, ucmax_dqsdly_byte[byte], byte, wrlevel_dqs_final_delay[byte]);
        }               
    }

    for (byte = 0; byte < 4; byte++)
    {
        dbg_print("------------------------------------\n");
        dbg_print("DQS %d Final Delay (after WL)=%d (%xh)\n",byte,ucmax_dqsdly_byte[byte], ucmax_dqsdly_byte[byte]);
        for (uiBit=8*byte; uiBit<=8*byte+7; uiBit++)
            dbg_print("DQ %d Final Delay (after WL)=%d (%xh)\n",uiBit,tx_perbit_dly[uiBit].best_dqdly,tx_perbit_dly[uiBit].best_dqdly);
        dbg_print("DQM %d Final Delay (after WL)=%d(%xh)\n",byte,ucdqmdly_byte[byte], ucdqmdly_byte[byte]); 
        dbg_print("------------------------------------\n");
    }
#endif    // WRITE_LEVELING

#endif	// TX_PERBIT_WIN

#ifdef DQ_DQS_DQM_REMAPPING        
    data = 0;
    for (byte = 0 ; byte < 4; byte ++)
    {
    	#ifdef TX_PERBIT_WIN    
    	data |= ucdqmdly_byte[byte] << (DQM_Mapping[byte]*4);
    	#else
    	data |= (dqs_perbyte_dly[byte].dq << (DQM_Mapping[byte]*4));
    	#endif
    }

    DRAMC_WRITE_REG(data , DRAMC_PADCTL2/* 0x10 */);
    opt_tx_dqm = data;

    for (byte = 0 ; byte < 4; byte ++)
    {
    	opt_tx_dq[byte] = 0;
    }

#ifdef TX_PERBIT_WIN    
    for (uiBit=0; uiBit<32; uiBit++) {
    	unsigned int uiRemapDQ;

	uiRemapDQ = Bit_DQO_Mapping[uiBit];
       	opt_tx_dq[uiRemapDQ>>3] |= tx_perbit_dly[uiBit].best_dqdly << ((uiRemapDQ & 0x07)<<2);
    }
#else
    for (byte = 0 ; byte < 4; byte ++)
    {
         unsigned int uiRemapDQ;
         dq = dqs_perbyte_dly[byte].dq;
    	for (i=0; i<8; i++) {
    		uiRemapDQ = Bit_DQO_Mapping[(byte<<3)+i];
    		opt_tx_dq[uiRemapDQ>>3] |= ((dq & 0xF) << ((uiRemapDQ & 0x07)<<2));
    	}
    }
#endif
    
    for (byte = 0 ; byte < 4; byte ++)
    {
    	   //dbg_print("DRAMC_DQODLY%d=%xh\n", byte+1, opt_tx_dq[byte]);
            DRAMC_WRITE_REG(opt_tx_dq[byte], DRAMC_DQODLY1+(4*byte));
    }

    data = 0;
    for (byte = 0 ; byte < 4; byte ++)
    {
    #ifdef TX_PERBIT_WIN    
    	data |= (ucmax_dqsdly_byte[byte] << (DQSO_Mapping[byte]*4));
    #else
    	data |= (dqs_perbyte_dly[byte].dqs << (DQSO_Mapping[byte]*4));
    #endif
    }

    DRAMC_WRITE_REG(data , DRAMC_PADCTL3/* 0x14 */);
    opt_tx_dqs = data;
    //print("*(%d):%x\n",DRAMC_PADCTL3,data); 
    //DRAMC_WRITE_REG(data,DRAMC_PADCTL3);

#else
    #ifdef TX_PERBIT_WIN
    data = 
        ((ucdqmdly_byte[0] & 0xF) << 0)       /* DQM0DLY: DRAMC_PADCTL2[0:3], 4 bits */
      | ((ucdqmdly_byte[1] & 0xF) << 4)       /* DQM1DLY: DRAMC_PADCTL2[4:7], 4 bits */
      | ((ucdqmdly_byte[2] & 0xF) << 8)       /* DQM2DLY: DRAMC_PADCTL2[8:11], 4 bits */
      | ((ucdqmdly_byte[3] & 0xF) << 12);     /*  DQM3DLY: DRAMC_PADCTL2[12:15], 4 bits */
    #else
    data = 
        ((dqs_perbyte_dly[0].dq & 0xF) << 0)       /* DQM0DLY: DRAMC_PADCTL2[0:3], 4 bits */
      | ((dqs_perbyte_dly[1].dq & 0xF) << 4)       /* DQM1DLY: DRAMC_PADCTL2[4:7], 4 bits */
      | ((dqs_perbyte_dly[2].dq & 0xF) << 8)       /* DQM2DLY: DRAMC_PADCTL2[8:11], 4 bits */
      | ((dqs_perbyte_dly[3].dq & 0xF) << 12);     /*  DQM3DLY: DRAMC_PADCTL2[12:15], 4 bits */ 
    #endif
    DRAMC_WRITE_REG(data , DRAMC_PADCTL2/* 0x10 */);
    opt_tx_dqm = data;
    for (byte = 0 ; byte < 4; byte ++)
    {
        #ifdef TX_PERBIT_WIN
            uiBit = 8*byte;
            data =    
                ((tx_perbit_dly[uiBit+0].best_dqdly & 0xF) << 0)        /* DQ0DLY: DRAMC_DQODLY1[0:3],          4 bits */
              | ((tx_perbit_dly[uiBit+1].best_dqdly & 0xF) << 4)       /* DQ1DLY: DRAMC_DQODLY1[4:7],          4 bits */
              | ((tx_perbit_dly[uiBit+2].best_dqdly & 0xF) << 8)       /* DQ2DLY: DRAMC_DQODLY1[8:11],         4 bits */
              | ((tx_perbit_dly[uiBit+3].best_dqdly & 0xF) << 12)      /* DQ3DLY: DRAMC_DQODLY1[12:15],        4 bits */
              | ((tx_perbit_dly[uiBit+4].best_dqdly & 0xF) << 16)      /* DQ4DLY: DRAMC_DQODLY1[16:19],        4 bits */
              | ((tx_perbit_dly[uiBit+5].best_dqdly & 0xF) << 20)      /* DQ5DLY: DRAMC_DQODLY1[20:23],        4 bits */
              | ((tx_perbit_dly[uiBit+6].best_dqdly & 0xF) << 24)      /* DQ6DLY: DRAMC_DQODLY1[24:27],        4 bits */
              | ((tx_perbit_dly[uiBit+7].best_dqdly & 0xF) << 28);     /*  DQ7DLY: DRAMC_DQODLY1[28:31],        4 bits */ 
        #else
            dq = dqs_perbyte_dly[byte].dq;
            data =    
                ((dq & 0xF) << 0)             /* DQ0DLY: DRAMC_DQODLY1[0:3],          4 bits */
              | ((dq & 0xF) << 4)               /* DQ1DLY: DRAMC_DQODLY1[4:7],          4 bits */
              | ((dq & 0xF) << 8)               /* DQ2DLY: DRAMC_DQODLY1[8:11],         4 bits */
              | ((dq & 0xF) << 12)      /* DQ3DLY: DRAMC_DQODLY1[12:15],        4 bits */
              | ((dq & 0xF) << 16)      /* DQ4DLY: DRAMC_DQODLY1[16:19],        4 bits */
              | ((dq & 0xF) << 20)      /* DQ5DLY: DRAMC_DQODLY1[20:23],        4 bits */
              | ((dq & 0xF) << 24)      /* DQ6DLY: DRAMC_DQODLY1[24:27],        4 bits */
              | ((dq & 0xF) << 28); 
	#endif
            DRAMC_WRITE_REG(data,DRAMC_DQODLY1+(4*byte));
            opt_tx_dq[byte] = data;

    }
    #ifdef TX_PERBIT_WIN 
    data =(ucmax_dqsdly_byte[0]) | (ucmax_dqsdly_byte[1]<<4 | ucmax_dqsdly_byte[2] << 8) | (ucmax_dqsdly_byte[3] << 12);
    #else
    data =(dqs_perbyte_dly[0].dqs) | (dqs_perbyte_dly[1].dqs<<4 | dqs_perbyte_dly[2].dqs << 8) | (dqs_perbyte_dly[3].dqs << 12);
    #endif
    DRAMC_WRITE_REG(data , DRAMC_PADCTL3/* 0x14 */);
    opt_tx_dqs = data;
    //print("%d,data:%x\n",DRAMC_PADCTL3,data); DRAMC_WRITE_REG(data,DRAMC_PADCTL3);

#endif 
    return 0;
}

unsigned int ta2_rx_scan(unsigned int len)
{
//#define dbg_print     
    unsigned int i;
    unsigned int dqs_input_delay;
    unsigned int cmp_err;    
    unsigned int u4prv_register_018, u4prv_register_210[8];

    // backup registers
    u4prv_register_018 = DRAMC_READ_REG(DRAMC_R0DELDLY);
    for (i = 0; i < 8; i++)
    {
        u4prv_register_210[i] = DRAMC_READ_REG(DRAMC_DQIDLY1+4*i);
    }

    // set DQ delay to 0x0.
    for (i = 0; i < 8; i++)
    {
        DRAMC_WRITE_REG(0x0,DRAMC_DQIDLY1+4*i);
    }       
   
    /* find the minimum and maximum DQS input delay*/
    for (i = 0; i < 32; i=i+2)
    {
        dqs_input_delay = (i) + (i << 8) + (i << 16) + (i << 24);
        DRAMC_WRITE_REG(dqs_input_delay,DRAMC_R0DELDLY);      
   
        cmp_err = ta2_enable_write_read(len);   

        if (cmp_err == 0)
        {
            break;
        }       
    }

    print("RX Scan: DQS delay %d \n", i);

    // restore registers
    DRAMC_WRITE_REG(u4prv_register_018, DRAMC_R0DELDLY);  
    for (i = 0; i < 8; i++)
    {
        DRAMC_WRITE_REG(u4prv_register_210[i], DRAMC_DQIDLY1+4*i);        
    }

    return cmp_err;    
}

unsigned int DramcRxdatlatCal(unsigned int use_default_rx)
{
    unsigned int ii, jj = 0xFFFFFFFF;
    unsigned int u4prv_register_07c, u4prv_register_0e4;
    unsigned int u4value, u4err_value;
    unsigned int ucfirst, ucbegin, ucsum, ucbest_step;    
    unsigned int result, start_tap, end_tap;

    

    print("==============================================================\n");
    if (use_default_rx == 0)
    {
        print("    NEW DATLAT calibration \n");
    }
    else
    {
        print("    DATLAT calibration \n");
    }
    print("==============================================================\n");

    if (use_default_rx == 0)
    {
        start_tap = 3;
        end_tap = 12;
    }
    else
    {
        start_tap = 0;
        end_tap = 15;
    }   
    
    // pre-save
    // 0x07c[6:4]   DATLAT bit2-bit0
    u4prv_register_07c = DRAMC_READ_REG(0x07c);
    // 0x0e4[4]     DALLAT bit3
    u4prv_register_0e4 = DRAMC_READ_REG(0x0e4);

    // init best_step to default
    ucbest_step = (U8) (((u4prv_register_07c>>4)&0x7) | ((u4prv_register_0e4>>1)&0x8));
    print("DATLAT Default value = 0x%x\n", ucbest_step);

    // 1.set DATLAT 0-15
    // 2.enable engine1 or engine2 
    // 3.check result  ,2-3 taps pass 
    // 4.set DATLAT 1st value when 2 taps pass ,2nd value when 3taps pass

    // Initialize
    ucfirst = 0xff;
    ucbegin = 0;
    ucsum = 0;    	
    u4value = u4prv_register_07c;
    //mcCLR_MASK(u4value, MASK_DDR2CTL_DATLAT);
    (u4value) &= (~(0x00000070));
    DRAMC_WRITE_REG(u4value, 0x07c);

    u4value = u4prv_register_0e4;
    //mcCLR_BIT(u4value, POS_PADCTL4_DATLAT3);
    (u4value) &= (~(0x00000010));
    DRAMC_WRITE_REG(u4value, 0x0e4);
    
    for (ii = start_tap; ii <= end_tap; ii++)
    {        
        // 1
        if (ii == 8)
        {
            // bit3=1 ,bit2=bit1=bit0 =0
            u4value = u4prv_register_0e4;
            //mcSET_BIT(u4value, POS_PADCTL4_DATLAT3);
            (u4value) |= (0x00000010);
            DRAMC_WRITE_REG(u4value, 0x0e4);

            u4value = u4prv_register_07c;
            (u4value) &= (~(0x00000070));
            DRAMC_WRITE_REG(u4value, 0x07c);
        }
        else
        {
            u4value = u4prv_register_07c;
            //mcSET_FIELD(u4value, (ii&0x7), MASK_DDR2CTL_DATLAT, POS_DDR2CTL_DTALAT);
            (u4value) &= (~(0x00000070));
            (u4value) |= ((ii&0x7)<<4);
            DRAMC_WRITE_REG(u4value, 0x07c);
        }

        // 2
        if (use_default_rx == 0)
        {
            u4err_value = ta2_rx_scan(0x20);
        }
        else
        {
            u4err_value = ta2_enable_write_read(0x100);    
        }    
        
        if (u4err_value == 0)
        {
            if (ucbegin == 0)
            {
                // first tap which is pass
                ucfirst = ii;
                ucbegin = 1;
            }
            if (ucbegin == 1)
            {
                ucsum++;                
            }
        }
        else
        {
            if (ucbegin == 1)
            {
                // pass range end
                ucbegin = 0xff;
            }
        }

        print("TAP=%d, err_value=0x%x, begin=%d, first=%d, sum=%d\n", ii, u4err_value, ucbegin, ucfirst, ucsum);

        if ((ucsum>=2) && (use_default_rx == 0))
        {
            print("New DATLAT with RX scan. Early Break!!\n");
            break;
        }
    }

    // 4
    if (ucsum == 0)
    {
        print("no DATLAT taps pass\n");
        result = 1;
    }    
    else
    {
        //if test engine2, DLE return not so density, once pass, always pass
        //so we choose the second one as the best
        ucbest_step = ucfirst + 1;
        result = 0;
    }
    

    print("first_step=%d total pass=%d best_step=%d\n", ucfirst, ucsum, ucbest_step);
    
    //if (apply == 1)
    {
        if (ucsum == 0)
        {
            print("DATLAT calibration fail, write back to default values!\n");
            DRAMC_WRITE_REG(u4prv_register_07c, 0x07c);
            DRAMC_WRITE_REG(u4prv_register_0e4, 0x0e4);
            //ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DDR2CTL), u4prv_register_07c);
            //ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL4), u4prv_register_0e4);
        }
        else
        {
        #ifdef fcDATLAT_SAME_CHECK
            if (RANK_CURR == 1)
            {
                if (r0_datlat_value < ucbest_step)
                {
                    ucbest_step = r0_datlat_value;
                    print("rank 0 < rank 1 datlat, set to %d \n", ucbest_step);
                }
            }
            else
            {
                r0_datlat_value = ucbest_step;
            }
        #endif
            if (ucbest_step >= 8)
            {
                u4value = u4prv_register_0e4;
                //mcSET_BIT(u4value, POS_PADCTL4_DATLAT3);
                (u4value) |= (0x00000010);
                DRAMC_WRITE_REG(u4value, 0x0e4);
            }
            else
            {
                u4value = u4prv_register_0e4;
                //mcCLR_BIT(u4value, POS_PADCTL4_DATLAT3);
                (u4value) &= (~(0x00000010));
                DRAMC_WRITE_REG(u4value, 0x0e4);
                //ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL4), u4value);
            }
            u4value = u4prv_register_07c;
            (u4value) &= (~(0x00000070));
            (u4value) |= ((ucbest_step&0x7)<<4);
            DRAMC_WRITE_REG(u4value, 0x07c);
            //mcSET_FIELD(u4value, (ucbest_step&0x7), MASK_DDR2CTL_DATLAT, POS_DDR2CTL_DTALAT);
            //ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DDR2CTL), u4value);
        }
    }
    //else
    //{
    //    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DDR2CTL), u4prv_register_07c);
    //    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL4), u4prv_register_0e4);
    //}


    // Log Example
/*
==============================================================
    DATLAT calibration
    channel=2(2:cha, 3:chb)    apply = 1
==============================================================
DATLAT Default value = 0xe
TAP= 0, err_value=0xffffffff, begin=0, first=255, sum=0
TAP= 1, err_value=0xffffffff, begin=0, first=255, sum=0
TAP= 2, err_value=0xffffffff, begin=0, first=255, sum=0
TAP= 3, err_value=0xffffffff, begin=0, first=255, sum=0
TAP= 4, err_value=0xffffffff, begin=0, first=255, sum=0
TAP= 5, err_value=0xffffffff, begin=0, first=255, sum=0
TAP= 6, err_value=0xffffffff, begin=0, first=255, sum=0
TAP= 7, err_value=0xffffffff, begin=0, first=255, sum=0
TAP= 8, err_value=0xffffffff, begin=0, first=255, sum=0
TAP= 9, err_value=0xffffffff, begin=0, first=255, sum=0
TAP=10, err_value=0xffffffff, begin=0, first=255, sum=0
TAP=11, err_value=0xffffffff, begin=0, first=255, sum=0
TAP=12, err_value=0x       0, begin=1, first= 12, sum=1
TAP=13, err_value=0x       0, begin=1, first= 12, sum=2
TAP=14, err_value=0x       0, begin=1, first= 12, sum=3
TAP=15, err_value=0x       0, begin=1, first= 12, sum=4
pattern=1(0: ISI, 1: AUDIO, 2: TA4, 3: TA4-3) first_step=12 total pass=4 best_step=13
*/

    return result;
    
}


/*
 * dramc_calib: Do DRAMC calibration.
 * Return error code;
 */
int dramc_calib(EMI_SETTINGS *emi_setting)
{
    int err;
    int reg_val;
    int rank0_col,rank1_col,bak_cona,bak_conf1;
#ifdef fcGATING_SAME_MCK
    int r0_gating_coase_MCK, r1_gating_coase_MCK;
#endif
//#define DEBUG_DRAMC_CALIB
#if defined(DEBUG_DRAMC_CALIB)
    int temp;

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
    RANK_CURR = 0;
    /*modify MA type*/
#if 0
    bak_conf1 = *((volatile unsigned *)(DRAMC0_BASE + 0x04));

    //printf("before:0x04:%x\n",*((volatile unsigned *)(DRAMC0_BASE + 0x04)));
    rank0_col = (*(volatile unsigned *)(EMI_CONA) & (0x3<<4))>>4;
    // NOTE EMI register definition for COL bits
    rank0_col++;
    *((volatile unsigned *)(DRAMC0_BASE + 0x04)) = (*((volatile unsigned *)(DRAMC0_BASE + 0x04)) & (~(0x3 << 8)))| (rank0_col << 8);
    //printf("after:reg_val:%x,0x04:%x\n",rank0_col,*((volatile unsigned *)(DRAMC0_BASE + 0x04)));
#endif

#ifdef BYPASS_DQSDUMMYPAD
    *((volatile unsigned *)(DRAMC0_BASE + 0x124)) |= 0x00060000;
    *((volatile unsigned *)(DDRPHY_BASE + 0x124)) |= 0x00060000;
#endif

#ifdef TXDELAY_ENABLE
    *((volatile unsigned *)(DRAMC0_BASE + 0x124)) |= 0x0000ff00;
    *((volatile unsigned *)(DDRPHY_BASE + 0x124)) |= 0x0000ff00;
#endif

#if 1//defined(MACH_TYPE_MT6753) || defined(MACH_TYPE_MT6737T)
    if ((emi_setting->type & 0xF) == TYPE_LPDDR3) {
#ifdef fcCA_TRAINING
        DramcCATraining(emi_setting);
        #ifdef fcRESET_DDR_AFTER_CA_WR
        init_lpddr3_only(emi_setting);
        #endif
#endif
#ifdef WRITE_LEVELING
        DramcWriteLeveling(emi_setting);
        #ifdef fcRESET_DDR_AFTER_CA_WR
        init_lpddr3_only(emi_setting);
        #endif
#endif
    }
#endif//defined(MACH_TYPE_MT6753) || defined(MACH_TYPE_MT6737T)

    /* do calibration for DQS gating window (phase 1) for rank 0*/
    DQS_PRINT=1;
#ifdef GW_TA2    
    err = do_dqs_gw_calib_2(); // use test engine2
#else
    //err = do_dqs_gw_calib_1();
#endif
    if (err < 0) {
        goto dramc_calib_exit;
    }
    DQS_PRINT=0;
    opt_gw_coarse_value0 = opt_gw_coarse_value; 
    opt_gw_fine_value0 = opt_gw_fine_value;
    print("rank 0 coarse = %s\n", opt_gw_coarse_value0 );
    print("rank 0 fine = %s\n", opt_gw_fine_value0  );
    /* do DLE calibration */
    /*err = do_dle_calib();
    if (err < 0) {
        goto dramc_calib_exit;
    }*/
    err = DramcRxdatlatCal(0);

    // for testing, to check RX DQ/DQS restore to default
    //print_DBG_info();
    
    if (err)
    {
        DramcRxdatlatCal(1);
    }
    /* do SW RX calibration for DQ/DQS input delay */
    err = do_sw_rx_dq_dqs_calib();
    if (err < 0) {
        goto dramc_calib_exit;
    }
    opt_rx_dqs0 = DRAMC_READ_REG(DRAMC_R0DELDLY);

    bak_cona = *(volatile unsigned *)(EMI_CONA);

    if ( *(volatile unsigned *)(EMI_CONA)& 0x20000)      // Check dual ranks.
    {
        /*modify MA type*/
        //printf("before:0x04:%x\n",*((volatile unsigned *)(DRAMC0_BASE + 0x04)));
        RANK_CURR = 1;
#if 0
        rank1_col = (*(volatile unsigned *)(EMI_CONA) & (0x3<<6))>>6;
        *((volatile unsigned *)(DRAMC0_BASE + 0x04)) = (*((volatile unsigned *)(DRAMC0_BASE + 0x04)) & (~(0x3 << 8)))| (rank1_col << 8);
        *((volatile unsigned *)(EMI_CONA)) = (*((volatile unsigned *)(EMI_CONA)) & (~(0x3 << 4)))| (rank1_col << 4);
        //print("after:rank1_col:%x,0x04:%x,%x\n",rank1_col,*((volatile unsigned *)(DRAMC0_BASE + 0x04)), *((volatile unsigned *)(EMI_CONA)));
#endif

        //make sure we in chip select 1. Swap CS0 and CS1.
        *((volatile unsigned *)(DRAMC0_BASE + 0x0110)) |= (0x8);
        *((volatile unsigned *)(DDRPHY_BASE + 0x0110)) |= (0x8);

        /* do calibration for DQS gating window (phase 1) for rank 1*/
        DQS_PRINT=1;
#ifdef GW_TA2    
        err = do_dqs_gw_calib_2(); // use test engine2
#else
        //err = do_dqs_gw_calib_1();
#endif
        if (err < 0) {
            goto dramc_calib_exit;
        }
        DQS_PRINT=0;
        opt_gw_coarse_value1 = opt_gw_coarse_value; 
        opt_gw_fine_value1 = opt_gw_fine_value;
        print("rank 1 coarse = %s\n", opt_gw_coarse_value1 );
        print("rank 1 fine = %s\n", opt_gw_fine_value1  );
        /* do DLE calibration */
        /*err = do_dle_calib();
        if (err < 0) {
            goto dramc_calib_exit;
        }*/
        err = DramcRxdatlatCal(0);

        // for testing, to check RX DQ/DQS restore to default
        //print_DBG_info();
        
        if (err)
        {
            DramcRxdatlatCal(1);
        }
        /* do SW RX calibration for DQ/DQS input delay */
        err = do_sw_rx_dq_dqs_calib();
        if (err < 0) {
            goto dramc_calib_exit;
        }
        opt_rx_dqs1 = DRAMC_READ_REG(DRAMC_R0DELDLY);

        dqsi_gw_dly_coarse_factor_handler_rank1(opt_gw_coarse_value1);
        dqsi_gw_dly_fine_factor_handler_rank1(opt_gw_fine_value1);
    
        DDR_PHY_RESET_NEW();

        DRAMC_WRITE_REG(opt_rx_dqs1,DRAMC_R1DELDLY);
        //swap back
        //make sure we in chip select 0 now
        *((volatile unsigned *)(DRAMC0_BASE + 0x0110)) &= (~0x8);
        *((volatile unsigned *)(DDRPHY_BASE + 0x0110)) &= (~0x8);
    }
    RANK_CURR = 0;
    *(volatile unsigned *)(EMI_CONA) = bak_cona;
    //*((volatile unsigned *)(DRAMC0_BASE + 0x04))= bak_conf1;
    dqsi_gw_dly_coarse_factor_handler(opt_gw_coarse_value0);
    dqsi_gw_dly_fine_factor_handler(opt_gw_fine_value0);
    DRAMC_WRITE_REG(opt_rx_dqs0,DRAMC_R0DELDLY);

    //print("[MEM]CONA%x,conf1:%x\n",*((volatile unsigned *)(EMI_CONA)),*((volatile unsigned *)(DRAMC0_BASE + 0x04)));
    DDR_PHY_RESET_NEW();
     /* do SW TX calibration for DQ/DQS output delay */
    err = do_sw_tx_dq_dqs_calib();
    if (err < 0) {
        goto dramc_calib_exit;
    }
    dqsi_gw_dly_coarse_factor_handler(opt_gw_coarse_value0);
    dqsi_gw_dly_fine_factor_handler(opt_gw_fine_value0);    

    DDR_PHY_RESET_NEW();
    
#ifdef fcGATING_SAME_MCK
    if (emi_setting->EMI_CONA_VAL & 0x20000) 
    { /* support dual rank */
        r0_gating_coase_MCK = atoi(opt_gw_coarse_value0);
        r0_gating_coase_MCK = r0_gating_coase_MCK >> 2;
        r1_gating_coase_MCK = atoi(opt_gw_coarse_value1);
        r1_gating_coase_MCK = r1_gating_coase_MCK >> 2;
        dbg_print("[fcGATING_SAME_MCK]Rank0 MCK Coarse Tune = %d, ", r0_gating_coase_MCK);
        dbg_print("Rank1 MCK Coarse Tune = %d\n", r1_gating_coase_MCK);
        
        if (r0_gating_coase_MCK > r1_gating_coase_MCK)
        {
            dqsi_gw_dly_coarse_factor_handler(pre_gw_coarse_value0);
            dqsi_gw_dly_fine_factor_handler(pre_gw_fine_value0);
            DDR_PHY_RESET_NEW();
            dbg_print("[fcGATING_SAME_MCK]Modify rank0 gating to N-1, coarse = %s, ", pre_gw_coarse_value0);
            dbg_print("fine = %s\n", pre_gw_fine_value0);
        }
        else if (r0_gating_coase_MCK < r1_gating_coase_MCK)
        {
            dqsi_gw_dly_coarse_factor_handler_rank1(pre_gw_coarse_value1);
            dqsi_gw_dly_fine_factor_handler_rank1(pre_gw_fine_value1); 
            DDR_PHY_RESET_NEW();
            dbg_print("[fcGATING_SAME_MCK]Modify rank1 gating to N-1, coarse = %s, ", pre_gw_coarse_value1);
            dbg_print("fine = %s\n", pre_gw_fine_value1);
        }
        else
        {
            print("[fcGATING_SAME_MCK]Rand 0/1 MCK coarse tune are the same!!\n");
        }        
    }
#endif


dramc_calib_exit:

#if defined(DEBUG_DRAMC_CALIB)
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
    print("DRAMC calibration takes %d CPU cycles\n\r", temp);
#endif
    return err;
}

/*************************************************************************/
/*****************************MEMPLL init*********************************/
/*************************************************************************/

/***********************
 * MEMPLL Configuration
 ***********************/
#define r_bias_en_stb_time            (0x00000000 << 24)  //170[31:24]
#define r_bias_lpf_en_stb_time        (0x00000000 << 16)  //170[23:16]
#define r_mempll_en_stb_time          (0x00000000 << 8)   //170[15:8]
#define r_dmall_ck_en_stb_time        (0x00000000 << 0)   //170[7:0]

#define r_dds_en_stb_time             (0x00000000 << 24)  //171[31:24]
#define r_div_en_stb_time             (0x00000000 << 16)  //171[23:16]
#define r_dmpll2_ck_en_stb_time       (0x00000000 << 8)   //171[15:8]
#define r_iso_en_stb_time             (0x00000000 << 0)   //171[7:0]

#define r_bias_en_stb_dis             (0x00000001 << 28)  //172[28]
#define r_bias_en_src_sel             (0x00000001 << 24)  //172[24]
#define r_bias_lpf_en_stb_dis         (0x00000001 << 20)  //172[20]
#define r_bias_lpf_en_src_sel         (0x00000001 << 16)  //172[16]
#define r_mempll4_en_stb_dis          (0x00000001 << 15)  //172[15]
#define r_mempll3_en_stb_dis          (0x00000001 << 14)  //172[14]
#define r_mempll2_en_stb_dis          (0x00000001 << 13)  //172[13]
#define r_mempll_en_stb_dis           (0x00000001 << 12)  //172[12]
#define r_mempll4_en_src_sel          (0x00000001 << 11)  //172[11]
#define r_mempll3_en_src_sel          (0x00000001 << 10)  //172[10]
#define r_mempll2_en_src_sel          (0x00000001 << 9)   //172[9]
#define r_mempll_en_src_sel           (0x00000001 << 8)   //172[8]
#define r_dmall_ck_en_stb_dis         (0x00000001 << 4)   //172[4]
#define r_dmall_ck_en_src_sel         (0x00000001 << 0)   //172[0]

#define r_dds_en_stb_dis              (0x00000001 << 28)  //173[28]
#define r_dds_en_src_sel              (0x00000001 << 24)  //173[24]
#define r_div_en_stb_dis              (0x00000001 << 20)  //173[20]
#define r_div_en_src_sel              (0x00000001 << 16)  //173[16]
#define r_dmpll2_ck_en_stb_dis        (0x00000001 << 12)  //173[12]
#define r_dmpll2_ck_en_src_sel        (0x00000001 << 8)   //173[8]
#define r_iso_en_stb_dis              (0x00000001 << 4)   //173[4]
#define r_iso_en_src_sel              (0x00000001 << 0)   //173[0]

#define r_dmbyp_pll4                  (0x00000001 << 0)   //190[0]
#define r_dmbyp_pll3                  (0x00000001 << 1)   //190[1]
#define r_dm1pll_sync_mode            (0x00000001 << 2)   //190[2]
#define r_dmall_ck_en                 (0x00000001 << 4)   //190[4]
#define r_dmpll2_clk_en               (0x00000001 << 5)   //190[5]

#define pllc1_postdiv_1_0             (0x00000003 << 14)  //180[15:14]
#define pllc1_blp                     (0x00000001 << 12)  //180[12]
#define pllc1_mempll_n_info_chg       (0x00000001 << 0)   //189[0]
#define pllc1_dmss_pcw_ncpo_30_0      (0x7fffffff << 1)   //189[31:1]
#define pllc1_mempll_div_en           (0x00000001 <<24)   //181[24]
#define pllc1_mempll_div_6_0          (0x0000007f <<25)   //181[31:25]
#define pllc1_mempll_reserve_2        (0x00000001 <<18)   //181[18]
#define pllc1_mempll_top_reserve_2_0  (0x00000000 <<16)   //182[18:16]
#define pllc1_mempll_bias_en          (0x00000001 <<14)   //181[14]
#define pllc1_mempll_bias_lpf_en      (0x00000001 <<15)   //181[15]
#define pllc1_mempll_en               (0x00000001 << 2)   //180[2]
#define pllc1_mempll_sdm_prd_1        (0x00000001 <<11)   //188[11]

#define mempll2_prediv_1_0            (0x00000000 << 0)   //182[1:0]
#define mempll2_vco_div_sel           (0x00000001 <<29)   //183[29]
#define mempll2_m4pdiv_1_0            (0x00000003 <<10)   //183[11:10],P9
#define mempll2_fbdiv_6_0             (0x0000007f << 2)   //182[8:2],P6
#define mempll2_fb_mck_sel            (0x00000001 << 9)   //183[9]
#define mempll2_fbksel_1_0            (0x00000003 <<10)   //182[11:10]
#define mempll2_bp_br                 (0x00000003 <<26)   //183[27:26]
#define mempll2_posdiv_1_0            (0x00000000 <<30)   //183[31:30]
#define mempll2_ref_dl_4_0            (0x00000000 <<27)   //184[31:27]
#define mempll2_fb_dl_4_0             (0x00000000 <<22)   //184[26:22]
#define mempll2_en                    (0x00000001 <<18)   //183[18]

#define mempll3_prediv_1_0            (0x00000000 << 0)   //184[1:0]
#define mempll3_vco_div_sel           (0x00000001 <<29)   //185[29]
#define mempll3_m4pdiv_1_0            (0x00000003 <<10)   //185[11:10]
#define mempll3_fbdiv_6_0             (0x0000007f << 2)   //184[8:2]
#define mempll3_bp_br                 (0x00000003 <<26)   //185[27:26]
#define mempll3_fb_mck_sel            (0x00000001 << 9)   //185[9]
#define mempll3_fbksel_1_0            (0x00000003 <<10)   //184[11:10]
#define mempll3_posdiv_1_0            (0x00000000 <<30)   //185[31:30]
#define mempll3_ref_dl_4_0            (0x00000000 <<27)   //186[31:27]
#define mempll3_fb_dl_4_0             (0x00000000 <<22)   //186[26:22]
#define mempll3_en                    (0x00000001 <<18)   //185[18]

#define mempll4_prediv_1_0            (0x00000000 << 0)   //186[1:0]
#define mempll4_vco_div_sel           (0x00000001 <<29)   //187[29]
#define mempll4_m4pdiv_1_0            (0x00000003 <<10)   //187[11:10]
#define mempll4_fbdiv_6_0             (0x0000007f << 2)   //186[8:2]
#define mempll4_fbksel_1_0            (0x00000003 <<10)   //186[11:10]
#define mempll4_bp_br                 (0x00000003 <<26)   //187[27:26]
#define mempll4_fb_mck_sel            (0x00000001 << 9)   //187[9]
#define mempll4_posdiv_1_0            (0x00000000 <<30)   //187[31:30]
#define mempll4_ref_dl_4_0            (0x00000000 <<27)   //188[31:27]
#define mempll4_fb_dl_4_0             (0x00000000 <<22)   //188[26:22]
#define mempll4_en                    (0x00000001 <<18)   //187[18]

/***********************
 * MEMPLL Calibration
 ***********************/
#define MEMPLL_JMETER_CNT               1024
#define MEMPLL_JMETER_CONFIDENCE_CNT    (MEMPLL_JMETER_CNT/10)
#define MEMPLL_JMETER_WAIT_TIME_BASE    10 // time base
#define MEMPLL_JMETER_WAIT_TIMEOUT      1000/MEMPLL_JMETER_WAIT_TIME_BASE // timeout ~ 1000us


void mt_mempll_init(int type, int pll_mode)
{
    unsigned int temp;

    /*********************************
    * (1) Setup DDRPHY operation mode
    **********************************/

    *((UINT32P)(DRAMC0_BASE + 0x007c)) |= 0x00000001; //DFREQ_DIV2=1
    *((UINT32P)(DDRPHY_BASE + 0x007c)) |= 0x00000001;

#ifdef DDRPHY_3PLL_MODE
    if (pll_mode == PLL_MODE_3)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00014020;    //3PLL sync mode, OK
    }
    else if (pll_mode== PLL_MODE_2)
    {
#ifndef DDRPHY_2PLL_LONG_CKTREE
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00014022;
#else
        // just for D-3 test, enable longer clock tree
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00014026;
#endif
    }
    else // 1 PLL mode
#endif
    {
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00004007;    //1PLL sync mode, OK.
    }

    /*****************************************************************************************
    * (2) Setup MEMPLL operation case & frequency. May set according to dram type & frequency
    ******************************************************************************************/
    *((UINT32P)(DDRPHY_BASE + (0x0170 <<2))) = r_bias_en_stb_time | r_bias_lpf_en_stb_time | r_mempll_en_stb_time | r_dmall_ck_en_stb_time;
    *((UINT32P)(DDRPHY_BASE + (0x0171 <<2))) = r_dds_en_stb_time | r_div_en_stb_time | r_dmpll2_ck_en_stb_time | r_iso_en_stb_time;
    *((UINT32P)(DDRPHY_BASE + (0x0172 <<2))) = r_bias_en_stb_dis| r_bias_en_src_sel | r_bias_lpf_en_stb_dis| r_bias_lpf_en_src_sel | r_mempll4_en_stb_dis| r_mempll3_en_stb_dis| r_mempll2_en_stb_dis| r_mempll_en_stb_dis| r_mempll4_en_src_sel | r_mempll3_en_src_sel | r_mempll2_en_src_sel | r_mempll_en_src_sel | r_dmall_ck_en_stb_dis | r_dmall_ck_en_src_sel;
    *((UINT32P)(DDRPHY_BASE + (0x0173 <<2))) = r_dds_en_stb_dis| r_dds_en_src_sel | r_div_en_stb_dis| r_div_en_src_sel | r_dmpll2_ck_en_stb_dis| r_dmpll2_ck_en_src_sel | r_iso_en_stb_dis| r_iso_en_src_sel;

    // MEMPLL common setting
    *((UINT32P)(DDRPHY_BASE + (0x0180 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0180 <<2))) & (~pllc1_postdiv_1_0)) | 0x00000000; //RG_MEMPLL_POSDIV[1:0] = 2'b00;
    *((UINT32P)(DDRPHY_BASE + (0x0180 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0180 <<2))) & (~pllc1_blp)) | (0x00000001 << 12); //RG_MEMPLL_BLP = 1'b1;
    *((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) & (~pllc1_mempll_div_6_0)) | (0x00000052 << 25); //RG_MEMPLL_DIV = 7'h52;
    *((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) & (~pllc1_mempll_reserve_2)) | (0x00000001 << 18); //RG_MEMPLL_RESERVE[2] = 1;
    
    *((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbksel_1_0)) | 0x00000000; //RG_MEMPLL2_FBKSEL[1:0] = 2'b00;
    *((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_bp_br)) | (0x00000003 << 26); //RG_MEMPLL2_BP = 1, RG_MEMPLL2_BR=1;

#ifdef DDRPHY_3PLL_MODE
    if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
    {
        *((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_fb_mck_sel)) | (0x00000001 << 9); //RG_MEMPLL2_FB_MCK_SEL;
    }
#endif
    
    *((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbksel_1_0)) | 0x00000000; //RG_MEMPLL3_FBKSEL = 2'b00;
    *((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_bp_br)) | (0x00000003 << 26); //RG_MEMPLL3_BP = 1, RG_MEMPLL3_BR=1;

#ifdef DDRPHY_3PLL_MODE
    if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
    {
        *((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_fb_mck_sel)) | (0x00000001 << 9); //RG_MEMPLL3_FB_MCK_SEL = 1;
    }
#endif
    
    *((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbksel_1_0)) | 0x00000000; //RG_MEMPLL4_FBKSEL = 2'b00;
    *((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_bp_br)) | (0x00000003 << 26); //RG_MEMPLL4_BP = 1, RG_MEMPLL4_BR=1;

#ifdef DDRPHY_3PLL_MODE
    if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
    {
        *((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_fb_mck_sel)) | (0x00000001 << 9); //RG_MEMPLL4_FB_MCK_SEL = 1;
    }
#endif

    //MEMPLL different setting for different frequency begin
#if 1
    if (type == DDR1280) // real DDR-1280 (sign-off)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_vco_div_sel)) | 0x00000000; //RG_MEMPLL2_VCO_DIV_SEL =0;
        *((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_vco_div_sel)) | 0x00000000; //RG_MEMPLL3_VCO_DIV_SEL =0;
        *((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_vco_div_sel)) | 0x00000000; //RG_MEMPLL4_VCO_DIV_SEL =0;
        // DDR-1333
        //*((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x50d8fe7c << 1); //RG_DMSS_PCW_NCPO[30:0]
        // DDR-1280
        *((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x4da21535 << 1); //RG_DMSS_PCW_NCPO[30:0]
        
        if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
        {
            *((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x0000000d << 2); //RG_MEMPLL2_FBDIV = 7'h0d;
            *((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x0000000d << 2); //RG_MEMPLL3_FBDIV = 7'h0d;
            *((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x0000000d << 2); //RG_MEMPLL4_FBDIV = 7'h0d;
        }
        else
        {
            *((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x00000034 << 2); //RG_MEMPLL2_FBDIV = 7'h34;
            *((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x00000034 << 2); //RG_MEMPLL3_FBDIV = 7'h34;
            *((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x00000034 << 2); //RG_MEMPLL4_FBDIV = 7'h34;
        }
    }
    else 
#endif
	if (type == DDR1333) // real DDR-1333    
    {
        *((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_vco_div_sel)) | 0x00000000; //RG_MEMPLL2_VCO_DIV_SEL =0;
        *((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_vco_div_sel)) | 0x00000000; //RG_MEMPLL3_VCO_DIV_SEL =0;
        *((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_vco_div_sel)) | 0x00000000; //RG_MEMPLL4_VCO_DIV_SEL =0;
        // DDR-1333
        *((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x50d8fe7c << 1); //RG_DMSS_PCW_NCPO[30:0]
        // DDR-1280
        //*((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x4da21535 << 1); //RG_DMSS_PCW_NCPO[30:0]
    #ifdef DDRPHY_3PLL_MODE
        if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
        {
            *((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x0000000d << 2); //RG_MEMPLL2_FBDIV = 7'h0d;
            *((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x0000000d << 2); //RG_MEMPLL3_FBDIV = 7'h0d;
            *((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x0000000d << 2); //RG_MEMPLL4_FBDIV = 7'h0d;
        }
        else
	#endif
        {
            *((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x00000034 << 2); //RG_MEMPLL2_FBDIV = 7'h34;
            *((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x00000034 << 2); //RG_MEMPLL3_FBDIV = 7'h34;
            *((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x00000034 << 2); //RG_MEMPLL4_FBDIV = 7'h34;
        }
    }
#if 0
    else if (type == DDR938) // for DVFS_low (DVS HQA), the same settings as DDR-1333 other than NCPO
    {
        *((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_vco_div_sel)) | 0x00000000; //RG_MEMPLL2_VCO_DIV_SEL =0;
        *((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_vco_div_sel)) | 0x00000000; //RG_MEMPLL3_VCO_DIV_SEL =0;
        *((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_vco_div_sel)) | 0x00000000; //RG_MEMPLL4_VCO_DIV_SEL =0;
        
    #if 1
        *((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x38e3f9f0 << 1); //RG_DMSS_PCW_NCPO[30:0]
    #else // only for debug (lowest bring up frequency DDR-667)
        *((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x287442a6 << 1); //RG_DMSS_PCW_NCPO[30:0]
    #endif
    
        if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
        {
            *((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x0000000d << 2); //RG_MEMPLL2_FBDIV = 7'h0d;
            *((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x0000000d << 2); //RG_MEMPLL3_FBDIV = 7'h0d;
            *((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x0000000d << 2); //RG_MEMPLL4_FBDIV = 7'h0d;
        }
        else
        {
            *((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x00000034 << 2); //RG_MEMPLL2_FBDIV = 7'h34;
            *((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x00000034 << 2); //RG_MEMPLL3_FBDIV = 7'h34;
            *((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x00000034 << 2); //RG_MEMPLL4_FBDIV = 7'h34;
        }
    }
    else if (type == DDR1466)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_vco_div_sel)) | 0x00000000; //RG_MEMPLL2_VCO_DIV_SEL =0;
        *((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_vco_div_sel)) | 0x00000000; //RG_MEMPLL3_VCO_DIV_SEL =0;
        *((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_vco_div_sel)) | 0x00000000; //RG_MEMPLL4_VCO_DIV_SEL =0;
        *((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x52902d02 << 1); //RG_DMSS_PCW_NCPO[30:0] = 31'h52902d02;
        if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
        {
            *((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x0000000e << 2); //RG_MEMPLL2_FBDIV = 7'h0e;
            *((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x0000000e << 2); //RG_MEMPLL3_FBDIV = 7'h0e;
            *((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x0000000e << 2); //RG_MEMPLL4_FBDIV = 7'h0e;
        }
        else
        {
            *((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x00000038 << 2); //RG_MEMPLL2_FBDIV = 7'h38;
            *((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x00000038 << 2); //RG_MEMPLL3_FBDIV = 7'h38;
            *((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x00000038 << 2); //RG_MEMPLL4_FBDIV = 7'h38;
        }
    }
    else if (type == DDR1313) // for Denali-3, DVFS-low frequency, the same settings as DDR-1466 other than NCPO
    {
        *((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_vco_div_sel)) | 0x00000000; //RG_MEMPLL2_VCO_DIV_SEL =0;
        *((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_vco_div_sel)) | 0x00000000; //RG_MEMPLL3_VCO_DIV_SEL =0;
        *((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_vco_div_sel)) | 0x00000000; //RG_MEMPLL4_VCO_DIV_SEL =0;
        *((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x49F24924 << 1); //RG_DMSS_PCW_NCPO[30:0] = 31'h49F24924;
        if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
        {
            *((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x0000000e << 2); //RG_MEMPLL2_FBDIV = 7'h0e;
            *((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x0000000e << 2); //RG_MEMPLL3_FBDIV = 7'h0e;
            *((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x0000000e << 2); //RG_MEMPLL4_FBDIV = 7'h0e;
        }
        else
        {
            *((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x00000038 << 2); //RG_MEMPLL2_FBDIV = 7'h38;
            *((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x00000038 << 2); //RG_MEMPLL3_FBDIV = 7'h38;
            *((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x00000038 << 2); //RG_MEMPLL4_FBDIV = 7'h38;
        }
    }
    else if (type == DDR1600) // for Denali-3, DVFS-high frequency, the same settings as DDR-1466 other than NCPO
    {
        *((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_vco_div_sel)) | 0x00000000; //RG_MEMPLL2_VCO_DIV_SEL =0;
        *((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_vco_div_sel)) | 0x00000000; //RG_MEMPLL3_VCO_DIV_SEL =0;
        *((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_vco_div_sel)) | 0x00000000; //RG_MEMPLL4_VCO_DIV_SEL =0;
        *((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x5A1C21C2 << 1); //RG_DMSS_PCW_NCPO[30:0] = 31'h5A1C21C2;
        if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
        {
            *((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x0000000e << 2); //RG_MEMPLL2_FBDIV = 7'h0e;
            *((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x0000000e << 2); //RG_MEMPLL3_FBDIV = 7'h0e;
            *((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x0000000e << 2); //RG_MEMPLL4_FBDIV = 7'h0e;
        }
        else
        {
            *((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x00000038 << 2); //RG_MEMPLL2_FBDIV = 7'h38;
            *((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x00000038 << 2); //RG_MEMPLL3_FBDIV = 7'h38;
            *((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x00000038 << 2); //RG_MEMPLL4_FBDIV = 7'h38;
        }
    }
#endif
    else // DDR-1066
    {
        *((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x4c68b439 << 1); //RG_DMSS_PCW_NCPO[30:0] = 31'h4c68b439;
    #ifdef DDRPHY_3PLL_MODE
		if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
        {
            *((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x0000000b << 2); //RG_MEMPLL2_FBDIV = 7'h0b;
            *((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x0000000b << 2); //RG_MEMPLL3_FBDIV = 7'h0b;
            *((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x0000000b << 2); //RG_MEMPLL4_FBDIV = 7'h0b;
        }
        else
	#endif
        {    
            *((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x0000002c << 2); //RG_MEMPLL2_FBDIV = 7'h2c;
            *((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x0000002c << 2); //RG_MEMPLL3_FBDIV = 7'h2c;
            *((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x0000002c << 2); //RG_MEMPLL4_FBDIV = 7'h2c;
        }
    }    
    //MEMPLL different setting for different frequency end

#ifdef DDRPHY_3PLL_MODE
    if (pll_mode == PLL_MODE_2)
    {
#ifndef DDRPHY_2PLL_LONG_CKTREE
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00014022  | r_dmpll2_clk_en;
#else
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00014026  | r_dmpll2_clk_en;
#endif
    }
    else if (pll_mode == PLL_MODE_3)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00014020 | r_dmpll2_clk_en;    //3PLL sync mode
    }
    else // 1-PLL mode
#endif
    {    
        //*((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00000007 | r_dmpll2_clk_en;    //1PLL sync mode
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00004007 ;    //1PLL sync mode
    }

    /***********************************
    * (3) Setup MEMPLL power on sequence
    ************************************/

    gpt_busy_wait_us(2);

    *((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) & (~pllc1_mempll_bias_en)) | (0x00000001 << 14); //RG_MEMPLL_BIAS_EN = 1'b1;

    gpt_busy_wait_us(2);

    *((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) & (~pllc1_mempll_bias_lpf_en)) | (0x00000001 << 15); //RG_MEMPLL_BIAS_LPF_EN = 1'b1;

    gpt_busy_wait_us(1000);

    *((UINT32P)(DDRPHY_BASE + (0x0180 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0180 <<2))) & (~pllc1_mempll_en)) | (0x00000001 << 2); //RG_MEMPLL_EN = 1'b1;

    gpt_busy_wait_us(20);

    *((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) & (~pllc1_mempll_div_en)) | (0x00000001 << 24); //RG_MEMPLL_DIV_EN = 1'b1;

    gpt_busy_wait_us(1);

#ifdef DDRPHY_3PLL_MODE
    if (pll_mode == PLL_MODE_3)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_en)) | (0x00000001 << 18); //RG_MEMPLL2_EN = 1'b1;
        *((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_en)) | (0x00000001 << 18); //RG_MEMPLL3_EN = 1'b1;
        *((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_en)) | (0x00000001 << 18); //RG_MEMPLL4_EN = 1'b1;
    }
    else if (pll_mode == PLL_MODE_2)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_en)) | (0x00000001 << 18); //RG_MEMPLL2_EN = 1'b1;
        *((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_en)) | (0x00000000 << 18); //RG_MEMPLL3_EN = 1'b0;
        *((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_en)) | (0x00000001 << 18); //RG_MEMPLL4_EN = 1'b1;
    }
    else
#endif
    {
        *((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_en)) | (0x00000001 << 18); //RG_MEMPLL2_EN = 1'b1;
        *((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_en)) | (0x00000000 << 18); //RG_MEMPLL3_EN = 1'b0;
        *((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_en)) | (0x00000000 << 18); //RG_MEMPLL4_EN = 1'b0;
    }

    gpt_busy_wait_us(23);

#ifdef DDRPHY_3PLL_MODE
    if (pll_mode == PLL_MODE_2)
    {
#ifndef DDRPHY_2PLL_LONG_CKTREE
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00014022  | r_dmpll2_clk_en | r_dmall_ck_en;
#else
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00014026  | r_dmpll2_clk_en | r_dmall_ck_en;
#endif
    }
    else if (pll_mode == PLL_MODE_3)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00014020 | r_dmpll2_clk_en | r_dmall_ck_en;    //3PLL sync mode
    }
    else // 1-PLL mode
#endif
    {
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00004007 | r_dmpll2_clk_en | r_dmall_ck_en;    //1PLL sync mode
    }
   
    /**********************************
    * (4) MEMPLL control switch to SPM
    ***********************************/
#ifdef fcSWITCH_SPM_CONTROL
#ifdef DDRPHY_3PLL_MODE
    if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
    {
        *((UINT32P)(DDRPHY_BASE + (0x0173 <<2))) = 0x40101000;   //[0]ISO_EN_SRC=0,[22]DIV_EN_SC_SRC=0 (pll2off),[16]DIV_EN_SRC=0,[8]PLL2_CK_EN_SRC=1(1pll),[8]PLL2_CK_EN_SRC=0(3pll)
    }
    else // 1-PLL mode
#endif
    {
        *((UINT32P)(DDRPHY_BASE + (0x0173 <<2))) = 0x40101000;   //[0]ISO_EN_SRC=0,[22]DIV_EN_SC_SRC=0 (pll2off),[16]DIV_EN_SRC=0,[8]PLL2_CK_EN_SRC=1(1pll),[8]PLL2_CK_EN_SRC=0(3pll)
    }

#ifdef DDRPHY_3PLL_MODE
    if (pll_mode == PLL_MODE_3)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0172 <<2))) = 0x0000F010;   //[24]BIAS_EN_SRC=0,[16]BIAS_LPF_EN_SRC=0,[8]MEMPLL_EN,[9][10][11]MEMPLL2,3,4_EN_SRC,[0]ALL_CK_EN_SRC=0
    }
    else if (pll_mode == PLL_MODE_2)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0172 <<2))) = 0x0000F410;   //PLL3 switch to SW control
    }
    else // 1-PLL mode
#endif
    {
        *((UINT32P)(DDRPHY_BASE + (0x0172 <<2))) = 0x0000FC10;   //1PLL mode, MEMPLL3,4_EN no change to spm controller.eep to 1'b0 for power saving.
        //*((UINT32P)(DDRPHY_BASE + (0x0172 <<2))) = 0x0000F010; // sim ok
    }

    //*((UINT32P)(DDRPHY_BASE + (0x0170 <<2))) = 0x003C1B96;   //setting for delay time
    *((UINT32P)(DDRPHY_BASE + (0x0170 <<2))) = 0x063C0000;   //setting for delay time
#endif 

// MEMPLL configuration end
}

#ifdef DDRPHY_3PLL_MODE
int mt_mempll_cali(void)
{
    int one_count = 0, zero_count = 0;
    int pll2_done = 0, pll3_done = 0, pll4_done = 0, ret = 0;

    unsigned int temp = 0, pll2_dl = 0, pll3_dl = 0, pll4_dl = 0;
    int pll2_phase=0, pll3_phase=0, pll4_phase=0;
    unsigned int jmeter_wait_count;

    pll3_done = 1;
    print("[PLL_Phase_Calib] 2PLL mode. Only calibrate PLL2 & 4!!\n");
    
    /***********************************************
    * 1. Set jitter meter clock to internal FB path
    ************************************************/
    //temp = DRV_Reg32(0x1021360C);
    //DRV_WriteReg32(0x1021360C, temp & ~0x200); // 0x1021360C[9] = 0 PLL2

    //temp = DRV_Reg32(0x10213614);
    //DRV_WriteReg32(0x10213614, temp & ~0x200); // 0x10213614[9] = 0 PLL3

    //temp = DRV_Reg32(0x1021361C);
    //DRV_WriteReg32(0x1021361C, temp & ~0x200); // 0x1021361C[9] = 0 PLL4

    /***********************************************
    * 2. Set jitter meter count number
    ************************************************/
    // PLL2 0x102131CC  0x1021332C[0]: JMETER_DONE 
    // PLL3 0x102131D0  0x1021332C[1]: JMETER_DONE 
    // PLL4 0x102131D4  0x1021332C[2]: JMETER_DONE

    temp = DRV_Reg32(0x102131CC) & 0x0000FFFF;
    DRV_WriteReg32(0x102131CC, (temp | (MEMPLL_JMETER_CNT<<16))); // 0x102131CC[31:16] PLL2 0x400 = 1024 count

    temp = DRV_Reg32(0x102131D0) & 0x0000FFFF;
    DRV_WriteReg32(0x102131D0, (temp | (MEMPLL_JMETER_CNT<<16))); // 0x102131D0[31:16] PLL3 0x400 = 1024 count

    temp = DRV_Reg32(0x102131D4) & 0x0000FFFF;
    DRV_WriteReg32(0x102131D4, (temp | (MEMPLL_JMETER_CNT<<16))); // 0x102131D4[31:16] PLL4 0x400 = 1024 count


    while(1)
    { 
        /***********************************************
        * 3. Adjust delay chain tap number
        ************************************************/
        if (!pll2_done)
        {
            if (pll2_phase == 0)    // initial phase set to 0 for REF and FBK
            {
                temp = DRV_Reg32(0x10213610) & ~0xF8000000;
                DRV_WriteReg32(0x10213610, (temp | (0x00 << 27 )));
                
                temp = DRV_Reg32(0x10213610) & ~0x07C00000;
                DRV_WriteReg32(0x10213610, (temp | (0x00 << 22)));
            }
            else if (pll2_phase == 1)   // REF lag FBK, delay FBK
            {
                temp = DRV_Reg32(0x10213610) & ~0xF8000000;
                DRV_WriteReg32(0x10213610, (temp | (0x00 << 27 )));
                
                temp = DRV_Reg32(0x10213610) & ~0x07C00000;
                DRV_WriteReg32(0x10213610, (temp | (pll2_dl << 22)));
            }
            else   // REF lead FBK, delay REF
            {
            
                temp = DRV_Reg32(0x10213610) & ~0xF8000000;
                DRV_WriteReg32(0x10213610, (temp | (pll2_dl << 27 )));
                
                temp = DRV_Reg32(0x10213610) & ~0x07C00000;
                DRV_WriteReg32(0x10213610, (temp | (0x00 << 22)));            
            }               
        }

        if (!pll3_done)
        {
            if (pll3_phase == 0)    // initial phase set to 0 for REF and FBK
            {
                temp = DRV_Reg32(0x10213618) & ~0xF8000000;
                DRV_WriteReg32(0x10213618, (temp | (0x00 << 27 )));
                
                temp = DRV_Reg32(0x10213618) & ~0x07C00000;
                DRV_WriteReg32(0x10213618, (temp | (0x00 << 22)));
            }
            else if (pll3_phase == 1)   // REF lag FBK, delay FBK
            {
                temp = DRV_Reg32(0x10213618) & ~0xF8000000;
                DRV_WriteReg32(0x10213618, (temp | (0x00 << 27 )));
                
                temp = DRV_Reg32(0x10213618) & ~0x07C00000;
                DRV_WriteReg32(0x10213618, (temp | (pll3_dl << 22)));
            }
            else   // REF lead FBK, delay REF
            {
                temp = DRV_Reg32(0x10213618) & ~0xF8000000;
                DRV_WriteReg32(0x10213618, (temp | (pll3_dl << 27 )));
                
                temp = DRV_Reg32(0x10213618) & ~0x07C00000;
                DRV_WriteReg32(0x10213618, (temp | (0x00 << 22)));
            }
        }

        if (!pll4_done)
        {
            if (pll4_phase == 0)    // initial phase set to 0 for REF and FBK
            {
                temp = DRV_Reg32(0x10213620) & ~0xF8000000;
                DRV_WriteReg32(0x10213620, (temp | (0x00 << 27 )));
                
                temp = DRV_Reg32(0x10213620) & ~0x07C00000;
                DRV_WriteReg32(0x10213620, (temp | (0x00 << 22)));
            }
            else if (pll4_phase == 1)   // REF lag FBK, delay FBK
            {
                temp = DRV_Reg32(0x10213620) & ~0xF8000000;
                DRV_WriteReg32(0x10213620, (temp | (0x00 << 27 )));
                
                temp = DRV_Reg32(0x10213620) & ~0x07C00000;
                DRV_WriteReg32(0x10213620, (temp | (pll4_dl << 22)));
            }
            else   // REF lead FBK, delay REF
            {
                temp = DRV_Reg32(0x10213620) & ~0xF8000000;
                DRV_WriteReg32(0x10213620, (temp | (pll4_dl << 27 )));
                
                temp = DRV_Reg32(0x10213620) & ~0x07C00000;
                DRV_WriteReg32(0x10213620, (temp | (0x00 << 22)));
            }
        }

        gpt_busy_wait_us(20); // wait for external loop ready

        /***********************************************
        * 4. Enable jitter meter
        ************************************************/
        if (!pll2_done)
        {
            temp = DRV_Reg32(0x102131CC);
            DRV_WriteReg32(0x102131CC, temp | 0x1); // 0x102131CC[0]=1 PLL2
        }

        if (!pll3_done)
        {
            temp = DRV_Reg32(0x102131D0);
            DRV_WriteReg32(0x102131D0, temp | 0x1); // 0x102131D0[0]=1 PLL3
        }

        if (!pll4_done)
        {
            temp = DRV_Reg32(0x102131D4);
            DRV_WriteReg32(0x102131D4, temp | 0x1); // 0x102131D4[0]=1 PLL4
        }

        gpt_busy_wait_us(80); // wait for jitter meter complete, 1/26MHz*1024

/*
        jmeter_wait_count = 0;
        if (!pll2_done) 
        {
            while (!(DRV_Reg32(0x1021332C)&0x00000001))
            {
                gpt_busy_wait_us(MEMPLL_JMETER_WAIT_TIME_BASE);
                jmeter_wait_count++;
                if (jmeter_wait_count>MEMPLL_JMETER_WAIT_TIMEOUT)
                {
                    mcSHOW_ERR_MSG(("[ERROR] PLL2 Jeter Meter Count Timeout > %d us!!\n", MEMPLL_JMETER_WAIT_TIME_BASE*MEMPLL_JMETER_WAIT_TIMEOUT));
                    break;
                }
            }            
        }
        
        jmeter_wait_count = 0;
        if (!pll3_done) 
        {
            while (!(DRV_Reg32(0x1021332C)&0x00000002))
            {
                gpt_busy_wait_us(MEMPLL_JMETER_WAIT_TIME_BASE);
                jmeter_wait_count++;
                if (jmeter_wait_count>MEMPLL_JMETER_WAIT_TIMEOUT)
                {
                    mcSHOW_ERR_MSG(("[ERROR] PLL3 Jeter Meter Count Timeout > %d us!!\n", MEMPLL_JMETER_WAIT_TIME_BASE*MEMPLL_JMETER_WAIT_TIMEOUT));
                    break;
                }
            }
        }
        
        jmeter_wait_count = 0;
        if (!pll4_done) 
        {
            while (!(DRV_Reg32(0x1021332C)&0x00000004))
            {
                gpt_busy_wait_us(MEMPLL_JMETER_WAIT_TIME_BASE);
                jmeter_wait_count++;
                if (jmeter_wait_count>MEMPLL_JMETER_WAIT_TIMEOUT)
                {
                    mcSHOW_ERR_MSG(("[ERROR] PLL4 Jeter Meter Count Timeout > %d us!!\n", MEMPLL_JMETER_WAIT_TIME_BASE*MEMPLL_JMETER_WAIT_TIMEOUT));
                    break;
                }
            }
        }
*/

        /***********************************************
        * 5. Check jitter meter counter value
        ************************************************/
        if (!pll2_done)
        {        
            one_count = DRV_Reg32(0x10213320) >> 16; // 0x10213320[31:16] PLL2 one count
            zero_count = DRV_Reg32(0x10213320) & 0x0000FFFF; // 0x10213320[15:0] PLL2 zero count
            
            if (pll2_phase == 0)
            {                   
                if (one_count > (zero_count+MEMPLL_JMETER_CONFIDENCE_CNT))
                {
                    // REF lag FBK
                    pll2_phase = 1;
                    pll2_dl++;
            
                    print("[PLL_Phase_Calib] PLL2 initial phase: REF lag FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count);
                }
                else if (zero_count > (one_count+MEMPLL_JMETER_CONFIDENCE_CNT))
                {
                    // REF lead FBK
                    pll2_phase = 2;
                    pll2_dl++;

                    print("[PLL_Phase_Calib] PLL2 initial phase: REF lead FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count);
                }
                else
                {
                    // in phase at initial
                    pll2_done = 1;

                    print("[PLL_Phase_Calib] PLL2 initial phase: REF in-phase FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count);                        
                }
            }
            else if (pll2_phase == 1)
            {
                if ((zero_count+MEMPLL_JMETER_CONFIDENCE_CNT) >= one_count)
                {
                    pll2_done = 1;
                    print("[PLL_Phase_Calib] PLL2 REF_DL: 0x0, FBK_DL: 0x%x, one_cnt/zero_cnt = %d/%d\n", pll2_dl, one_count, zero_count);
                }
                else
                {
                    pll2_dl++;
                    //print("[PLL_Phase_Calib] PLL2 REF_DL: 0x0, FBK_DL: 0x%x, one_cnt/zero_cnt = %d/%d\n", pll2_dl, one_count, zero_count);
                
                }
            }
            else
            {
                if ((one_count+MEMPLL_JMETER_CONFIDENCE_CNT) >= zero_count)
                {
                    pll2_done = 1;
                    print("[PLL_Phase_Calib] PLL2 REF_DL: 0x%x, FBK_DL: 0x0, one_cnt/zero_cnt = %d/%d\n", pll2_dl, one_count, zero_count);
                }
                else
                {
                    pll2_dl++;
                    //print("[PLL_Phase_Calib] PLL2 REF_DL: 0x0, FBK_DL: 0x%x, one_cnt/zero_cnt = %d/%d\n", pll2_dl, one_count, zero_count);
                
                }
            }            
        }

        if (!pll3_done)
        {
            one_count = DRV_Reg32(0x10213324) >> 16; // 0x10213324[31:16] PLL3 one count
            zero_count = DRV_Reg32(0x10213324) & 0x0000FFFF; // 0x10213324[15:0] PLL3 zero count

            if (pll3_phase == 0)
            {                   
                if (one_count > (zero_count+MEMPLL_JMETER_CONFIDENCE_CNT))
                {
                    // REF lag FBK
                    pll3_phase = 1;
                    pll3_dl++;
            
                    print("[PLL_Phase_Calib] PLL3 initial phase: REF lag FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count);
                }
                else if (zero_count > (one_count+MEMPLL_JMETER_CONFIDENCE_CNT))
                {
                    // REF lead FBK
                    pll3_phase = 2;
                    pll3_dl++;

                    print("[PLL_Phase_Calib] PLL3 initial phase: REF lead FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count);
                }
                else
                {
                    // in phase at initial
                    pll3_done = 1;

                    print("[PLL_Phase_Calib] PLL3 initial phase: REF in-phase FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count);
                }
            }
            else if (pll3_phase == 1)
            {
                if ((zero_count+MEMPLL_JMETER_CONFIDENCE_CNT) >= one_count)
                {
                    pll3_done = 1;
                    print("[PLL_Phase_Calib] PLL3 REF_DL: 0x0, FBK_DL: 0x%x, one_cnt/zero_cnt = %d/%d\n", pll3_dl, one_count, zero_count);
                }
                else
                {
                    pll3_dl++;
                }
            }
            else
            {
                if ((one_count+MEMPLL_JMETER_CONFIDENCE_CNT) >= zero_count)
                {
                    pll3_done = 1;
                    print("[PLL_Phase_Calib] PLL3 REF_DL: 0x%x, FBK_DL: 0x0, one_cnt/zero_cnt = %d/%d\n", pll3_dl, one_count, zero_count);
                }
                else
                {
                    pll3_dl++;
                }
            }
        }

        if (!pll4_done)
        {
            one_count = DRV_Reg32(0x10213328) >> 16; // 0x10213328[31:16] PLL4 one count
            zero_count = DRV_Reg32(0x10213328) & 0x0000FFFF; // 0x10213328[15:0] PLL4 zero count

            if (pll4_phase == 0)
            {                   
                if (one_count > (zero_count+MEMPLL_JMETER_CONFIDENCE_CNT))
                {
                    // REF lag FBK
                    pll4_phase = 1;
                    pll4_dl++;
            
                    print("[PLL_Phase_Calib] PLL4 initial phase: REF lag FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count);
                }
                else if (zero_count > (one_count+MEMPLL_JMETER_CONFIDENCE_CNT))
                {
                    // REF lead FBK
                    pll4_phase = 2;
                    pll4_dl++;

                    print("[PLL_Phase_Calib] PLL4 initial phase: REF lead FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count);
                }
                else
                {
                    // in phase at initial
                    pll4_done = 1;

                    print("[PLL_Phase_Calib] PLL4 initial phase: REF in-phase FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count);                        
                }
            }
            else if (pll4_phase == 1)
            {
                if ((zero_count+MEMPLL_JMETER_CONFIDENCE_CNT) >= one_count)
                {
                    pll4_done = 1;
                    print("[PLL_Phase_Calib] PLL4 REF_DL: 0x0, FBK_DL: 0x%x, one_cnt/zero_cnt = %d/%d\n", pll4_dl, one_count, zero_count);
                }
                else
                {
                    pll4_dl++;
                }
            }
            else
            {
                if ((one_count+MEMPLL_JMETER_CONFIDENCE_CNT) >= zero_count)
                {
                    pll4_done = 1;
                    print("[PLL_Phase_Calib] PLL4 REF_DL: 0x%x, FBK_DL: 0x0, one_cnt/zero_cnt = %d/%d\n", pll4_dl, one_count, zero_count);
                }
                else
                {
                    pll4_dl++;
                }
            }
        }

        /***********************************************
        * 6. Reset jitter meter value
        ************************************************/

        if (!pll2_done)
        {
            //pll2_dl++;        
            temp = DRV_Reg32(0x102131CC);
            DRV_WriteReg32(0x102131CC, temp & ~0x1); // 0x102131CC[0]=0 PLL2
        }

        if (!pll3_done)
        {
            //pll3_dl++;
            temp = DRV_Reg32(0x102131D0);
            DRV_WriteReg32(0x102131D0, temp & ~0x1); // 0x102131D0[0]=0 PLL3
        }

        if (!pll4_done)
        {
            //pll4_dl++;
            temp = DRV_Reg32(0x102131D4);
            DRV_WriteReg32(0x102131D4, temp & ~0x1); // 0x102131D4[0]=0 PLL4
        }

        /*************************************************************
        * Then return to step 1 to adjust next delay chain tap value.
        * Until we have ~ 50% of one or zero count on jitter meter
        **************************************************************/
        if (pll2_done && pll3_done && pll4_done)
        {
            ret = 0;
            break;
        }

        if (pll2_dl >= 32 || pll3_dl >= 32 || pll4_dl >= 32)
        {
            ret = -1;
            break;
        }
    }
         
    // Reset jitter meter counter for DDR reserved mode
    temp = DRV_Reg32(0x102131CC);
    DRV_WriteReg32(0x102131CC, temp & ~0x1); // 0x102131CC[0]=0 PLL2

    temp = DRV_Reg32(0x102131D0);
    DRV_WriteReg32(0x102131D0, temp & ~0x1); // 0x102131D0[0]=0 PLL3

    temp = DRV_Reg32(0x102131D4);
    DRV_WriteReg32(0x102131D4, temp & ~0x1); // 0x102131D4[0]=0 PLL4

    if (ret != 0)
    {
        print("[PLL_Phase_Calib] MEMPLL 2/3PLL mode calibration fail\n");
    #if 0 // for FT, enable it...
        while(1); // TBD
    #endif
    }
    else
    {
	print("[PLL_Phase_Calib] MEMPLL 2/3PLL mode calibration PASS\n");

    }
    /***********************************************
    * 7. Set jitter meter clock to external FB path
    ************************************************/
/*
    temp = DRV_Reg32(0x1021360C);
    DRV_WriteReg32(0x1021360C, temp | 0x200); // 0x1021360C[9] = 1 PLL2

    temp = DRV_Reg32(0x10213614);
    DRV_WriteReg32(0x10213614, temp | 0x200); // 0x10213614[9] = 1 PLL3

    temp = DRV_Reg32(0x1021361C);
    DRV_WriteReg32(0x1021361C, temp | 0x200); // 0x1021361C[9] = 1 PLL4
*/
    return ret;
  
}
#endif 

void mempll_init_main(void)
{
    unsigned int temp;
    int DDR_type;
    unsigned int memfreq_val = 0;
#ifdef DDR_RESERVE_MODE  
    unsigned int wdt_mode;
    unsigned int wdt_dbg_ctrl;
#endif

    DDR_type = mt_get_dram_type();
    if (TYPE_LPDDR2 == DDR_type) {
        mt_mempll_init(DDR1066, PLL_MODE_1);
        memfreq_val = 1066000;
    } else { 
#ifndef k_LP3_1066 
            mt_mempll_init(DDR1333, PLL_MODE_1);
            memfreq_val = 1333000;
#else
            mt_mempll_init(DDR1066, PLL_MODE_1);
            memfreq_val = 1066000;
#endif

    }

#ifdef DDR_RESERVE_MODE  
    wdt_mode = DRV_Reg32(MTK_WDT_MODE);
    wdt_dbg_ctrl = DRV_Reg32(MTK_WDT_DEBUG_CTL);

    print("before mt_mempll_cali, wdt_mode = 0x%x, wdt_dbg_ctrl = 0x%x\n", wdt_mode, wdt_dbg_ctrl);     
    if(((wdt_mode & MTK_WDT_MODE_DDR_RESERVE) !=0) && ((wdt_dbg_ctrl & MTK_DDR_RESERVE_RTA) != 0)) {
        // toggle INFO_CHG for reserve mode
        *((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_mempll_n_info_chg));
        *((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) | (pllc1_mempll_n_info_chg));
        print("[DDR Reserved Mode] toggle mempll freq for sync to high freq!!!\n");
    }
  #ifdef DDRPHY_3PLL_MODE
    else /* MEMPLL Calibration */
        mt_mempll_cali();
  #endif
#endif


	//set mem_clk
	DRV_WriteReg32(CLK_CFG_0_SET, 0x00000100);           // mem=mempll_ck
	DRV_WriteReg32(CLK_CFG_UPDATE, 0x00000002);            //update mem clock

	//MEMPLL_DIVIDER
	//*((V_UINT32P)(DDRPHY_BASE + 0x640)) = (*((V_UINT32P)(DDRPHY_BASE + 0x640)) | (1<<21)|(1<<22)|(1<<24));
}


