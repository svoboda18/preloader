/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2016. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER\'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER\'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER\'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK\'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK\'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver\'s
 * applicable license agreements with MediaTek Inc.
 */

#include "lpdma.h"
#include "emi_hw.h"

int lpdma_init(lpdma_dram_spec_t *spec) {

	unsigned int val;
	unsigned int ILV_STR, ILV_END;
	unsigned int ALL_STR;
	unsigned int LPCT1_OFFSET1, LPCT1_OFFSET2;
	unsigned int PAGE_32M;

	/* trigger */
	*(volatile unsigned int*)LPDMA_CONA = 0x0;

	/*
	 * [2:0] = 0x1 normal mode 4 channels, migrate to 2 channels (default)
	 * [2:0] = 0x2 normal mode 2 channels, migrate to 1 channels (FPGA)
	 */
	/*
	    31	PAGE_32M	MMU Page size
	    1: For DRAM size is less than 4GB
	    0: For DRAM size is large than 4GB
	*/
	PAGE_32M = ((spec->dram_size > 0x100000000)?1:0);
	val = *(volatile unsigned int*)LPDMA_CONB;
	*(volatile unsigned int*)LPDMA_CONB = (val & 0x7F3FFD6C | (PAGE_32M << 14) & ~(FORCE_ACC_LOW<< 9)| (DGB_CLK_EN << 10) | (SWAP_MODE << 4) | MODE_CH_4to2 | (BW_LIMIT<<16) | (BW_FILTER_LEN<<22));
	print("LPDMA_CONB=0x%x\n", *(volatile unsigned int*)LPDMA_CONB);

	/* no cascaed region */
	*(volatile unsigned int*)LPDMA_CONC = 0x0;

	/* Interleave region (unit:256KB)
	 * ILV_STR: [14:0]
	 * ILV_END: [30:16]
	 */
	ILV_STR = 0;
	ILV_END = (spec->interleave_region - spec->dram_start) >> 18;
	*(volatile unsigned int*)LPDMA_COND = (ILV_STR | (ILV_END << 16));
	print("LPDMA_COND=0x%x (interleave region)\n", *(volatile unsigned int*)LPDMA_COND);

	/* All channels region (unit: 4B)
	 * ALL_STR: [30:0]
	 */
	ALL_STR = (spec->all_channel_region - spec->dram_start) >> 2;
	*(volatile unsigned int*)LPDMA_CONE = ALL_STR;

	/* LPCT1 setting (unit: 32MB(Elbrus) / 64MB(Whitney))
	 * LPCT1_OFFSET1: [7:0]
	 * LPCT1_OFFSET2: [23:16]
	 */
	LPCT1_OFFSET1 = ((spec->interleave_region - spec->dram_start) >> 25) / spec->channel_num;
	LPCT1_OFFSET2 = LPCT1_OFFSET1 * 2;
	*(volatile unsigned int*)EMI_LPCT1 = ((LPCT1_OFFSET2 << 16) | LPCT1_OFFSET1);
	print("EMI_LPCT1=0x%x\n", *(volatile unsigned int*)EMI_LPCT1);

	/* dma register */
	*(volatile unsigned int*)LPDMA_CONF = 0x0;
	/* dma register */
	*(volatile unsigned int*)LPDMA_CONG = 0x0;
	/* Channel one start / end address for only copy data during swap mode */
	*(volatile unsigned int*)LPDMA_CONH = 0x0;
	/* Channel two start / end address for only copy data during swap mode */
	*(volatile unsigned int*)LPDMA_CONI = 0x0;
	/* Channel three start / end address for only copy data during swap mode */
	*(volatile unsigned int*)LPDMA_CONJ = 0x0;
	/* ultra priority */
	*(volatile unsigned int*)LPDMA_CONK = 0x0;
	/* pre-ultra priority */
	*(volatile unsigned int*)LPDMA_CONL = 0x0;
	/* migration status */
	*(volatile unsigned int*)LPDMA_CONM = 0x0;
	/* memory protection 1 */
	*(volatile unsigned int*)LPDMA_CONN = 0x0;
	/* memory protection 2 */
	*(volatile unsigned int*)LPDMA_CONO = 0x0;
	/* coherence check) */
	*(volatile unsigned int*)LPDMA_CONP = 0x0;
	/* misc */
	*(volatile unsigned int*)LPDMA_CONQ = 0x8;

	/*enable Enable DCS scramble*/
        *(volatile unsigned int*)EMI_LPCT0 |= (1<<16);


	/*1. PEND_CHK_THR (0x1023_1080[22:20]) =0
		EMI_MDCT_2ND[19:16](0x1023_007C) >=4
		EMI_SHF1[19:16] (0x1023_0718) >=4
	*/
	*(volatile unsigned int*)LPDMA_CONQ &= ~(0x7<<20);

	val = *(volatile unsigned int*)EMI_MDCT_2ND;
	val = (val & ~(0xf << 16))|(0x5 <<16);
	*(volatile unsigned int*)EMI_MDCT_2ND = val;

        val = *(volatile unsigned int*)EMI_SHF1;
        val = (val & ~(0xf << 16))|(0x5 <<16);
        *(volatile unsigned int*)EMI_SHF1 = val;
	print("EMI_MDCT_2ND[19:16] >4=0x%x \nEMI_SHF1[19:16] >4=0x%x\n",(((*(volatile unsigned int*)EMI_MDCT_2ND)>>16)&0xf),(((*(volatile unsigned int*)EMI_SHF1)>>16)&0xf));

	/*Need close pending write feature cause counter overflow bug.
	(LPDMA_CONB(0x1023_1008[27:25] = 3'h7))*/

	*(volatile unsigned int*)LPDMA_CONB |= (0x7<<25);

	*(volatile unsigned int*)INFRA_SLEEP_PRO &= ~(0xf) ;

      /*TEST DCM disable*/
	//*(volatile unsigned int*)LPDMA_CONB |= (0x8<<1);


	return 0;
}

/*
 * lpdma_set_initial_status
 *
 * @initial_status, one of
 * CASCADE_NORMAL_INTERLEAVE_NORMAL
 * CASCADE_LOWPWR_INTERLEAVE_NORMAL
 * CASCADE_NORMAL_INTERLEAVE_LOWPWR
 * CASCADE_LOWPWR_INTERLEAVE_LOWPWR
 *
 * LPDMA_CONA[6:5] = 0b00 => CASCADE_NORMAL_INTERLEAVE_NORMAL
 * LPDMA_CONA[6:5] = 0b01 => CASCADE_LOWPWR_INTERLEAVE_NORMAL
 * LPDMA_CONA[6:5] = 0b10 => CASCADE_NORMAL_INTERLEAVE_LOWPWR
 * LPDMA_CONA[6:5] = 0b11 => CASCADE_LOWPWR_INTERLEAVE_LOWPWR
 *
 */
void lpdma_set_initial_status(enum initial_status status)
{
	unsigned int val;

	val = *(volatile unsigned int*)LPDMA_CONA;
	val &= ~(0x3 << 5);

	switch (status) {
	case CASCADE_NORMAL_INTERLEAVE_NORMAL:
		val |= (0x0 << 5);
		break;
	case CASCADE_LOWPWR_INTERLEAVE_NORMAL:
		val |= (0x1 << 5);
		break;
	case CASCADE_NORMAL_INTERLEAVE_LOWPWR:
		val |= (0x2 << 5);
		break;
	case CASCADE_LOWPWR_INTERLEAVE_LOWPWR:
		val |= (0x3 << 5);
		break;
	}

	*(volatile unsigned int*)LPDMA_CONA = val;
}

/*
 * lpdma_reload_initial_status
 */
void lpdma_reload_initial_status(void) {

	unsigned int val,val2;

	/*a. Set restore register to restore information */
	val = *(volatile unsigned int*)LPDMA_CONA;

	/* load initial status */
	val |= (1 << 4);
	*(volatile unsigned int*)LPDMA_CONA = val;

	/* clear the bit */
	val &= ~(1 << 4);
	*(volatile unsigned int*)LPDMA_CONA = val;

	/* b.Set cascade region the same as interleave max address, direction down and trigger cascade mode */
	/*
	W : 0x1023_1010[31:16] = 0x1023_1018[31:16]
	W : 0x1023_1010[15:0] = 0x1023_1018[31:16]
	W : 0x1023_1000[8] = 1'b0
	W : 0x1023_1000[1] = 1'b1
	W : 0x1023_1000[1] = 1'b0 (if not set HW auto clear )(if conb[5]=1 does not need)
	*/
	val = *(volatile unsigned int*)LPDMA_COND;
	val = (val & 0xffff0000)>>16;

	*(volatile unsigned int*)LPDMA_CONC |= val;
	*(volatile unsigned int*)LPDMA_CONC |= (val << 16);

	*(volatile unsigned int*)LPDMA_CONA &= ~(1<<8);
	*(volatile unsigned int*)LPDMA_CONA |= (1<<1);
	//*(volatile unsigned int*)LPDMA_CONA &= ~(1<<1);

	/**c. Set restore register to restore again information*/
	val = *(volatile unsigned int*)LPDMA_CONA;

	/* load initial status */
	val |= (1 << 4);
	*(volatile unsigned int*)LPDMA_CONA = val;

	/* clear the bit */
	val &= ~(1 << 4);
	*(volatile unsigned int*)LPDMA_CONA = val;
}

/*
 * lpdma_set_dummy_write_pattern
 */
void lpdma_set_security_dummy_write_pattern(unsigned int pattern){

	*(volatile unsigned int*)LPDMA_CONR = pattern;
	print("set lpdma security dummy write pattern = 0x%x\n", *(volatile unsigned int*)LPDMA_CONR);
}

/*
 * lpdma_dump_debug_info
 */
void lpdma_dump_debug_info(void){
	int i;
	for (i=0; i<=0x88; i+=8){
		print("lpdma debug: 0x%x = 0x%x\n", LPDMA_CONA+i, *(volatile unsigned int*)(LPDMA_CONA+i));
	}
	for (i=0; i<=0xc0; i+=8){
		print("lpdma debug: 0x%x = 0x%x\n", LPDMA_INTLV+i, *(volatile unsigned int*)(LPDMA_INTLV+i));
	}
}
