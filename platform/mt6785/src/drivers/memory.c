/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
*/
/* MediaTek Inc. (C) 2010. All rights reserved.
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
* THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
* CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
* SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
* STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
* CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
* AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
* OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
* MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
* The following software/firmware and/or related documentation ("MediaTek Software")
* have been modified by MediaTek Inc. All revisions are subject to any receiver's
* applicable license agreements with MediaTek Inc.
*/


#include "typedefs.h"
#include "platform.h"

#include "memory.h"
#include "emi.h"
#include "uart.h"

#define MOD "MEM"

#include "wdt.h"
#include "emi_hw.h"
#include <emi_mpu_mt.h>

#ifdef DDR_RESERVE_MODE
extern u32 g_ddr_reserve_enable;
extern u32 g_ddr_reserve_success;
extern u32 g_ddr_reserve_ready;
extern u32 g_ddr_reserve_ta_err;
#endif

#if MEM_TEST
int complex_mem_test (unsigned int start, unsigned int len);
static int simple_mem_test(unsigned int start, unsigned int len);
extern unsigned int set_emi_before_rank1_mem_test(void);
extern void restore_emi_after_rank1_mem_test(void);
#endif

// --------------------------------------------------------
// init EMI
// --------------------------------------------------------
void mt_mem_init(void)
{
	unsigned int emi_cona;
	int region, dgroup;

#ifdef LAST_DRAMC
	dram_fatal_exception_detection_start();

	if (check_last_dram_fatal_exception() != 0 || check_gating_err_in_dramc_latch() != 0) {
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
		int ret;
		print("[save time for cal] dram fatal exception found -> clear calibration data\n");
		ret = clean_dram_calibration_data();
		SET_DATA_FORMATTED_STORAGE_API_ERR();
		print("[save time for cal] clean_dram_calibration_data done (ret=%d)\n", ret);
#endif
	}
#endif

	/* clear EMI MPU protect setting */
	for (region = 0; region < EMI_MPU_REGION_NUM; region++) {
		DRV_WriteReg32(EMI_MPU_SA(region), 0x0);
		DRV_WriteReg32(EMI_MPU_EA(region), 0x0);
		for (dgroup = EMI_MPU_DGROUP_NUM - 1; dgroup >= 0; dgroup--)
			DRV_WriteReg32(EMI_MPU_APC(region, dgroup), 0x0);
	}

#ifdef DDR_RESERVE_MODE
	if((g_ddr_reserve_enable==1) && (g_ddr_reserve_success==1)) {
		/* EMI register dummy read: give clock to EMI APB register to avoid DRAM access hang */
		emi_cona = *(volatile unsigned int *)(EMI_CONA);
		print("[DDR Reserve mode] EMI dummy read CONA = 0x%x\n", emi_cona);

		/* disable transaction mask */
		*(volatile unsigned int *) 0x1022D3FC &= 0xFFFFFFFE;
		*(volatile unsigned int *) 0x102353FC &= 0xFFFFFFFE;

		/* disable EMI APB protect */
		DRV_WriteReg32(EMI_MPU_CTRL, DRV_Reg32(EMI_MPU_CTRL)&0xFFFFFFFE);

		if (g_ddr_reserve_ta_err != 0) {
			print("[DDR Reserve mode] TA2 ERR = %d -> overwrite tag.ddr_reserve_success to 0\n", g_ddr_reserve_ta_err);
		}
	} else /* normal boot */
#endif
	{
		/* disable EMI APB protect */
		printf("EMI_MPU_CTRL=%x 1st\n",DRV_Reg32(EMI_MPU_CTRL));
		DRV_WriteReg32(EMI_MPU_CTRL, DRV_Reg32(EMI_MPU_CTRL)&0xFFFFFFFE);
		printf("EMI_MPU_CTRL=%x 2nd\n",DRV_Reg32(EMI_MPU_CTRL));

		/* force clear RGU control for DRAMC before calibration */
		rgu_release_rg_dramc_conf_iso();//Release DRAMC/PHY conf ISO
		rgu_release_rg_dramc_iso();//Release PHY IO ISO
		rgu_release_rg_dramc_sref();//Let DRAM Leave SR

		mt_set_emi ();

#if defined(LAST_DRAMC) & SUPPORT_SAVE_TIME_FOR_CALIBRATION
		set_err_code_for_storage_api();
#endif
	}

#if MEM_TEST
	unsigned rk0_err = 0, rk1_err = 0;
	int i = 0;

#ifdef DDR_RESERVE_MODE
	g_ddr_reserve_ready = 0x9502;
	/* Disable DDR-reserve mode in pre-loader stage then enable it again in kernel stage */
	rgu_dram_reserved(0);
#endif

	/* CFG_UBOOT_MEMADDR is LK base addr */
	if ((i = complex_mem_test(CFG_UBOOT_MEMADDR, MEM_TEST_SIZE)) == 0) {
		print ("[%s] 1st complex R/W mem test pass (start addr:0x%x)\n", MOD, CFG_UBOOT_MEMADDR);
	} else {
		print ("[%s] 1st complex R/W mem test fail :%x (start addr:0x%x)\n", MOD, i, CFG_UBOOT_MEMADDR);
#ifdef LAST_DRAMC
		rk0_err = 1;
#else
		ASSERT(0);
#endif
	}

#if DUAL_RANK_ENABLE
	if (get_dram_rank_nr() >= 2) {
		/* need to test rank1 */
		unsigned int rank1_start_address = DDR_BASE + set_emi_before_rank1_mem_test();

#ifdef DDR_RESERVE_MODE
		/* simple mem test to avoid memory corruption in DDR reserve mode */
		if((g_ddr_reserve_enable==1) && (g_ddr_reserve_success==1)) {
			i = simple_mem_test(rank1_start_address, MEM_TEST_SIZE);
		} else
#endif
			i = complex_mem_test(rank1_start_address, MEM_TEST_SIZE);
		restore_emi_after_rank1_mem_test();
		if (i == 0) {
			print("[%s] 2nd complex R/W mem test pass (start addr:0x%x, 0x0 @Rank1)\n", MOD, rank1_start_address);
		} else {
			print("[%s] 2nd complex R/W mem test fail :%x (start addr:0x%x, 0x0 @Rank1)\n", MOD, i, rank1_start_address);
#ifdef LAST_DRAMC
			rk1_err = 1;
#else
			ASSERT(0);
#endif
		}
	}
#endif

#ifdef LAST_DRAMC
	init_ta2_all_channel();
	dram_fatal_set_cpu_rw_err((rk1_err << 1)|rk0_err);

	/* TODO: TA2 test */

	/* check gating error */
	check_gating_error();

	if (check_dram_fatal_exception() == 1) {
		print("[dramc] fatal dram exception found! reset system..\n");
		while(1);
	}
	dram_fatal_exception_detection_end();
#endif

#endif
}

#if MEM_TEST
// --------------------------------------------------------
// do memory test
// --------------------------------------------------------
#define PATTERN1 0x5A5A5A5A
#define PATTERN2 0xA5A5A5A5

static int simple_mem_test(unsigned int start, unsigned int len)
{
	unsigned int *MEM32_BASE = (unsigned int *) start;
	unsigned int i, orig_val, new_val;

	for (i = 0; i < (len >> 2); ++i) {
		orig_val = MEM32_BASE[i];
		dsb();
		MEM32_BASE[i] = PATTERN1;
		dsb();
		new_val = MEM32_BASE[i];
		if (new_val != PATTERN1)
			return -1;
		dsb();
		MEM32_BASE[i] = orig_val;
	}

	return 0;
}

int
complex_mem_test (unsigned int start, unsigned int len)
{
    unsigned char *MEM8_BASE = (unsigned char *) start;
    unsigned short *MEM16_BASE = (unsigned short *) start;
    unsigned int *MEM32_BASE = (unsigned int *) start;
    unsigned int *MEM_BASE = (unsigned int *) start;
    unsigned char pattern8;
    unsigned short pattern16;
    unsigned int i, j, size, pattern32;
    unsigned int value;

    size = len >> 2;

    /* === Verify the tied bits (tied high) === */
    for (i = 0; i < size; i++)
    {
        MEM32_BASE[i] = 0;
    }

    for (i = 0; i < size; i++)
    {
        if (MEM32_BASE[i] != 0)
        {
            return -1;
        }
        else
        {
            MEM32_BASE[i] = 0xffffffff;
        }
    }

    /* === Verify the tied bits (tied low) === */
    for (i = 0; i < size; i++)
    {
        if (MEM32_BASE[i] != 0xffffffff)
        {
            return -2;
        }
        else
            MEM32_BASE[i] = 0x00;
    }

    /* === Verify pattern 1 (0x00~0xff) === */
    pattern8 = 0x00;
    for (i = 0; i < len; i++)
        MEM8_BASE[i] = pattern8++;
    pattern8 = 0x00;
    for (i = 0; i < len; i++)
    {
        if (MEM8_BASE[i] != pattern8++)
        {
            return -3;
        }
    }

    /* === Verify pattern 2 (0x00~0xff) === */
    pattern8 = 0x00;
    for (i = j = 0; i < len; i += 2, j++)
    {
        if (MEM8_BASE[i] == pattern8)
            MEM16_BASE[j] = pattern8;
        if (MEM16_BASE[j] != pattern8)
        {
            return -4;
        }
        pattern8 += 2;
    }

    /* === Verify pattern 3 (0x00~0xffff) === */
    pattern16 = 0x00;
    for (i = 0; i < (len >> 1); i++)
        MEM16_BASE[i] = pattern16++;
    pattern16 = 0x00;
    for (i = 0; i < (len >> 1); i++)
    {
        if (MEM16_BASE[i] != pattern16++)
        {
            return -5;
        }
    }

    /* === Verify pattern 4 (0x00~0xffffffff) === */
    pattern32 = 0x00;
    for (i = 0; i < (len >> 2); i++)
        MEM32_BASE[i] = pattern32++;
    pattern32 = 0x00;
    for (i = 0; i < (len >> 2); i++)
    {
        if (MEM32_BASE[i] != pattern32++)
        {
            return -6;
        }
    }

    /* === Pattern 5: Filling memory range with 0x44332211 === */
    for (i = 0; i < size; i++)
        MEM32_BASE[i] = 0x44332211;

    /* === Read Check then Fill Memory with a5a5a5a5 Pattern === */
    for (i = 0; i < size; i++)
    {
        if (MEM32_BASE[i] != 0x44332211)
        {
            return -7;
        }
        else
        {
            MEM32_BASE[i] = 0xa5a5a5a5;
        }
    }

    /* === Read Check then Fill Memory with 00 Byte Pattern at offset 0h === */
    for (i = 0; i < size; i++)
    {
        if (MEM32_BASE[i] != 0xa5a5a5a5)
        {
            return -8;
        }
        else
        {
            MEM8_BASE[i * 4] = 0x00;
        }
    }

    /* === Read Check then Fill Memory with 00 Byte Pattern at offset 2h === */
    for (i = 0; i < size; i++)
    {
        if (MEM32_BASE[i] != 0xa5a5a500)
        {
            return -9;
        }
        else
        {
            MEM8_BASE[i * 4 + 2] = 0x00;
        }
    }

    /* === Read Check then Fill Memory with 00 Byte Pattern at offset 1h === */
    for (i = 0; i < size; i++)
    {
        if (MEM32_BASE[i] != 0xa500a500)
        {
            return -10;
        }
        else
        {
            MEM8_BASE[i * 4 + 1] = 0x00;
        }
    }

    /* === Read Check then Fill Memory with 00 Byte Pattern at offset 3h === */
    for (i = 0; i < size; i++)
    {
        if (MEM32_BASE[i] != 0xa5000000)
        {
            return -11;
        }
        else
        {
            MEM8_BASE[i * 4 + 3] = 0x00;
        }
    }

    /* === Read Check then Fill Memory with ffff Word Pattern at offset 1h == */
    for (i = 0; i < size; i++)
    {
        if (MEM32_BASE[i] != 0x00000000)
        {
            return -12;
        }
        else
        {
            MEM16_BASE[i * 2 + 1] = 0xffff;
        }
    }


    /* === Read Check then Fill Memory with ffff Word Pattern at offset 0h == */
    for (i = 0; i < size; i++)
    {
        if (MEM32_BASE[i] != 0xffff0000)
        {
            return -13;
        }
        else
        {
            MEM16_BASE[i * 2] = 0xffff;
        }
    }


    /*===  Read Check === */
    for (i = 0; i < size; i++)
    {
        if (MEM32_BASE[i] != 0xffffffff)
        {
            return -14;
        }
    }


    /************************************************
    * Additional verification
    ************************************************/
    /* === stage 1 => write 0 === */

    for (i = 0; i < size; i++)
    {
        MEM_BASE[i] = PATTERN1;
    }


    /* === stage 2 => read 0, write 0xF === */
    for (i = 0; i < size; i++)
    {
        value = MEM_BASE[i];

        if (value != PATTERN1)
        {
            return -15;
        }
        MEM_BASE[i] = PATTERN2;
    }


    /* === stage 3 => read 0xF, write 0 === */
    for (i = 0; i < size; i++)
    {
        value = MEM_BASE[i];
        if (value != PATTERN2)
        {
            return -16;
        }
        MEM_BASE[i] = PATTERN1;
    }


    /* === stage 4 => read 0, write 0xF === */
    for (i = 0; i < size; i++)
    {
        value = MEM_BASE[i];
        if (value != PATTERN1)
        {
            return -17;
        }
        MEM_BASE[i] = PATTERN2;
    }


    /* === stage 5 => read 0xF, write 0 === */
    for (i = 0; i < size; i++)
    {
        value = MEM_BASE[i];
        if (value != PATTERN2)
        {
            return -18;
        }
        MEM_BASE[i] = PATTERN1;
    }


    /* === stage 6 => read 0 === */
    for (i = 0; i < size; i++)
    {
        value = MEM_BASE[i];
        if (value != PATTERN1)
        {
            return -19;
        }
    }


    /* === 1/2/4-byte combination test === */
    i = (unsigned int) MEM_BASE;

    while (i < (unsigned int) MEM_BASE + (size << 2))
    {
        *((unsigned char *) i) = 0x78;
        i += 1;
        *((unsigned char *) i) = 0x56;
        i += 1;
        *((unsigned short *) i) = 0x1234;
        i += 2;
        *((unsigned int *) i) = 0x12345678;
        i += 4;
        *((unsigned short *) i) = 0x5678;
        i += 2;
        *((unsigned char *) i) = 0x34;
        i += 1;
        *((unsigned char *) i) = 0x12;
        i += 1;
        *((unsigned int *) i) = 0x12345678;
        i += 4;
        *((unsigned char *) i) = 0x78;
        i += 1;
        *((unsigned char *) i) = 0x56;
        i += 1;
        *((unsigned short *) i) = 0x1234;
        i += 2;
        *((unsigned int *) i) = 0x12345678;
        i += 4;
        *((unsigned short *) i) = 0x5678;
        i += 2;
        *((unsigned char *) i) = 0x34;
        i += 1;
        *((unsigned char *) i) = 0x12;
        i += 1;
        *((unsigned int *) i) = 0x12345678;
        i += 4;
    }
    for (i = 0; i < size; i++)
    {
        value = MEM_BASE[i];
        if (value != 0x12345678)
        {
            return -20;
        }
    }


    /* === Verify pattern 1 (0x00~0xff) === */
    pattern8 = 0x00;
    MEM8_BASE[0] = pattern8;
    for (i = 0; i < size * 4; i++)
    {
        unsigned char waddr8, raddr8;
        waddr8 = i + 1;
        raddr8 = i;
        if (i < size * 4 - 1)
            MEM8_BASE[waddr8] = pattern8 + 1;
        if (MEM8_BASE[raddr8] != pattern8)
        {
            return -21;
        }
        pattern8++;
    }


    /* === Verify pattern 2 (0x00~0xffff) === */
    pattern16 = 0x00;
    MEM16_BASE[0] = pattern16;
    for (i = 0; i < size * 2; i++)
    {
        if (i < size * 2 - 1)
            MEM16_BASE[i + 1] = pattern16 + 1;
        if (MEM16_BASE[i] != pattern16)
        {
            return -22;
        }
        pattern16++;
    }


    /* === Verify pattern 3 (0x00~0xffffffff) === */
    pattern32 = 0x00;
    MEM32_BASE[0] = pattern32;
    for (i = 0; i < size; i++)
    {
        if (i < size - 1)
            MEM32_BASE[i + 1] = pattern32 + 1;
        if (MEM32_BASE[i] != pattern32)
        {
            return -23;
        }
        pattern32++;
    }

    return 0;
}

#endif
