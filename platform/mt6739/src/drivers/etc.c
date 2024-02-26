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
 * MediaTek Inc. (C) 2016. All rights reserved.
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

#include "platform.h"
#include "wdt.h"
#include "pmic.h"

#define TAG	"ETC"

#define ETC_INFO(str, ...) do {             \
	print("[%s] " str, TAG, ##__VA_ARGS__); \
} while(0)

enum mt_vproc_id {
	MT_VPROC1, /* Big + LL */
	MT_VPROC2, /* L only */

	NR_VPROC,
};

struct mt_cluster_ops {
        void (*ctrl_slave_cpus) (unsigned int, int);
        void (*cfg_boot_addr) (unsigned int);
        int (*check_spm_pwr_status) (void);

#if DO_ISOLATION
        void (*cluster_ctrl_iso) (struct mt_vproc *, int);
        void (*cluster_iso_prepare) (struct mt_vproc *);
#endif
};

#define STA_POWER_DOWN  0
#define STA_POWER_ON    1

static void wfe_instruction(void)
{
        while (1)
                asm("WFE\n");
}

static void etc_calibration(void)
{
#if 1
	u16 i, j, err_n, verr_status, droop_verr, temp, k_times;
	u16 value, value_err0, value_err1, value_err2;
	u32 v_srh[3], tdmux_ctrl[3] = {0x01, 0x03, 0x05};

	ETC_INFO("[%s] entering...\n", __func__);
	DRV_WriteReg32(0x10a40100, 0x08000000);
	DRV_WriteReg32(0x1020260C, 0x000000A8);
	DRV_WriteReg32(0x10202610, 0x00000000);
	DRV_WriteReg32(0x10202614, 0x00000050);
	DRV_WriteReg32(0x10202618, 0x00000001);
	DRV_WriteReg32(0x1020261C, 0x0000B600);
	DRV_WriteReg32(0x10202620, 0x000011FF);
	DRV_WriteReg32(0x10202624, 0x00000000);
	DRV_WriteReg32(0x1020262C, 0x00000808);
	DRV_WriteReg32(0x10202638, 0x00000000);
	DRV_WriteReg32(0x10202644, 0x00008040);
	DRV_WriteReg32(0x10202648, 0x00000000);
	DRV_WriteReg32(0x1020264C, 0x00000FFF);
	DRV_WriteReg32(0x10202650, 0x00000000);
	DRV_WriteReg32(0x10202654, 0x00000000);
	DRV_WriteReg32(0x10202658, 0x00000038);
	DRV_WriteReg32(0x10202670, 0x000000A5);
	DRV_WriteReg32(0x10202674, 0x00000301);
	DRV_WriteReg32(0x1020267C, 0x00001F1F);
	DRV_WriteReg32(0x10202680, 0x00000000);
	DRV_WriteReg32(0x10202684, 0x00005000);
	DRV_WriteReg32(0x10202634, 0x00000000);
	DRV_WriteReg32(0x10202688, 0x00000000);
	DRV_WriteReg32(0x1020268C, 0x00008000);
	DRV_WriteReg32(0x10202634, 0x00000001);
	DRV_WriteReg32(0x10202690, 0x00000000);
	DRV_WriteReg32(0x10202694, 0x0000B000);
	DRV_WriteReg32(0x10202634, 0x00000002);
	DRV_WriteReg32(0x10202698, 0);
	DRV_WriteReg32(0x1020269C, 0x0000503D);
	DRV_WriteReg32(0x10202634, 0x00000003);
	DRV_WriteReg32(0x102026E0, 0x00000000);
	DRV_WriteReg32(0x102026E4, 0x00000000);

	v_srh[0] = 0;
	v_srh[1] = 0;
	v_srh[2] = 0;
	k_times = 0;
	while (k_times < 3) {
		for (err_n = 0; err_n < 3; err_n++) { /* 3 verr searching... */
			DRV_WriteReg32(0x10202674, tdmux_ctrl[err_n]); /* ATC_TDMUX_CTRL 0x01, 0x03, 0x05 */
			droop_verr = 0;
			while (1) {
				j = 0;
				while (1) { /* for 100 times stable */
					verr_status = DRV_Reg32(0x1020265C); /* ATC_TDMUX_STATUS */
					DRV_WriteReg32(0x10202648, 0x01);
					DRV_WriteReg32(0x10202614, 0x80 | (droop_verr + 6));
					ETC_INFO("900mv, Vout=0x%x, Verr[%d]=%d, Loop=%d, err[%d]_sts=0x%x,alrm=%d\n",
						DRV_Reg32(0x1020260C), err_n, droop_verr, j, err_n, verr_status, DRV_Reg32(0x10202618));
					if (verr_status == 1) {
						j++;
					} else {
						ETC_INFO("Verr[%d]=%d, Loop=%d, err[%d]_sts=0x%x ===>(x)\n",
							err_n, droop_verr, j, err_n, verr_status);
						j = 0;
						break;
					}

					if (j > 99) {
						v_srh[err_n] = droop_verr;
						ETC_INFO("Vout=0x%x, Vout=%d(mv), Verr[%d]=%d, OOOOOOOOOO\n",
							DRV_Reg32(0x1020260C),
							(((DRV_Reg32(0x1020260C) * 3125) + 400000) / 1),
							err_n,
							v_srh[err_n]);
						break;
					}
				}

				if (v_srh[err_n] != 0)
					break;
				DRV_WriteReg32(0x10202624,
					(DRV_Reg32(0x10202624) & ~(0x1F << (err_n * 5))) |
					((droop_verr++) << (err_n * 5)));
				udelay(1);
			}
		}
		ETC_INFO("*********** Searched verr = 0x%x(%duV), 0x%x(%duV), 0x%x(%duV) **********\n",
			v_srh[0],
			((v_srh[0] * 3125) + 18750),
			v_srh[1],
			((v_srh[1] * 3125) + 18750),
			v_srh[2],
			((v_srh[2] * 3125) + 18750));

		for (i = 0; i < 3; i++) {
			for (j = i; j < 3; j++) {
				if(v_srh[j] < v_srh[i]) {
					temp = v_srh[j];
					v_srh[j] = v_srh[i];
					v_srh[i] = temp;
				}
			}
		}

		if (v_srh[2] - v_srh[0] <= 2) {
				value = v_srh[1] + 6;
				value_err0 = (value > 0x1F) ? 0x1F: value;

				value = v_srh[1] + 6 + 6;
				value_err1 = (value > 0x1F) ? 0x1F: value;

				value = v_srh[1] + 6 + 8;
				value_err2 = (value > 0x1F) ? 0x1F: value;

				value = (value_err2 << 10) | (value_err1 << 5) | value_err0;
				ETC_INFO("v_srh[%d] = (%x), value_m=%x\n", i, v_srh[i], value);
			break;
		} else {
			k_times++;
		}
	}

	if (k_times > 2) {
		value = 0x7FFF; /* means disable etc */
	}
	/* write the value to sram for ATF of kernel use */
	DRV_WriteReg32(0x0012A294, value);
#endif
	/* while(1); */
}

#if 0
DRV_Reg32(0x1020671C);
DRV_WriteReg32(MTK_WDT_LENGTH, (timeout | MTK_WDT_LENGTH_KEY));
#endif

static unsigned int set_cur_volt_buck1(unsigned int volt_uV)
{
        isl91302b_set_buck(1, volt_uV);
        return 0;
}
static unsigned int get_cur_volt_buck1(struct mt_cpu_dvfs **p)
{
        return isl91302b_get_buck(1);
}

static unsigned int get_cur_volt_sram_buck1(void)
{
        return buck_get_voltage(VSRAM_DVFS1) / 10;
}

static int set_cur_volt_sram_buck1(struct mt_cpu_dvfs *p, unsigned int volt)
{
        buck_set_voltage(VSRAM_DVFS1, volt * 10);

        return 0;
}

void etc_entry(void)
{
	//struct mt_vproc *vproc = picachu_dvfs_get_vproc_info();
	struct mt_cluster_ops *c_ops;
	unsigned int stored_vproc, stored_vsram;

	c_ops = (struct mt_cluster_ops *)
			picachu_get_cluster_ops(MT_VPROC1);

	if (c_ops->check_spm_pwr_status() &&
			(g_rgu_status & RE_BOOT_BY_WDT_HW)) {
		ETC_INFO("Skip ETC\n");
		return;
	}

	isl91302b_init();
	stored_vproc = get_cur_volt_buck1(NULL);
	stored_vsram = get_cur_volt_sram_buck1();

	ETC_INFO("stored_vproc: %d\n", stored_vproc);
	ETC_INFO("stored_vsram: %d\n", stored_vsram);

	set_cur_volt_buck1(900000); // uV
	set_cur_volt_sram_buck1(NULL, 95000); // mv_x100

	c_ops->cfg_boot_addr((unsigned int) &wfe_instruction);
	c_ops->ctrl_slave_cpus(MT_VPROC1, STA_POWER_ON);

	etc_calibration();

	c_ops->ctrl_slave_cpus(MT_VPROC1, STA_POWER_DOWN);

	set_cur_volt_buck1(stored_vproc);
	set_cur_volt_sram_buck1(NULL, stored_vsram);

	isl91302b_exit();
}
