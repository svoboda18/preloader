/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
*/
/* MediaTek Inc. (C) 2018. All rights reserved.
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
*/

#if defined(FEATURE_MMC_CM_TUNING)
int msdc_tune_cmdrsp(struct mmc_host *host, struct mmc_command *cmd)
{
	int result = MMC_ERR_CMDTUNEFAIL;
	unsigned int orig_rsmpl, cur_rsmpl, rsmpl, orig_clkmode;
#ifndef FEATURE_MMC_TUNING_EDGE_ONLY_WHEN_HIGH_SPEED
	unsigned int orig_dly1 = 0, orig_dly1_sel, cur_dly1;
	unsigned int orig_dly2 = 0, orig_dly2_sel, cur_dly2, cur_dly1_sel, cur_dly2_sel;
	unsigned int orig_dly = 0, cur_dly;
	unsigned int dly;
#endif
	u32 base = host->base;
	u8 hs400 = 0;
#if MSDC_TUNE_LOG
	u32 times = 0;
#endif

	MSDC_GET_FIELD(MSDC_CFG, MSDC_CFG_CKMOD, orig_clkmode);
	hs400 = (orig_clkmode == 3) ? 1 : 0;
	MSDC_GET_FIELD(MSDC_IOCON, MSDC_IOCON_RSPL, orig_rsmpl);

#ifndef FEATURE_MMC_TUNING_EDGE_ONLY_WHEN_HIGH_SPEED
	if (!host->base_top) {
		MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_CMDRDLY, orig_dly1);
		MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_CMDRRDLYSEL, orig_dly1_sel);
		MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE1_CMDRDLY2, orig_dly2);
		MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE1_CMDRRDLY2SEL, orig_dly2_sel);
	} else {
		MSDC_GET_FIELD(EMMC_TOP_CMD, PAD_CMD_RXDLY, orig_dly1);
		MSDC_GET_FIELD(EMMC_TOP_CMD, PAD_CMD_RD_RXDLY_SEL, orig_dly1_sel);
		MSDC_GET_FIELD(EMMC_TOP_CMD, PAD_CMD_RXDLY2, orig_dly2);
		MSDC_GET_FIELD(EMMC_TOP_CMD, PAD_CMD_RD_RXDLY2_SEL, orig_dly2_sel);
	}
	orig_dly = orig_dly1 * orig_dly1_sel + orig_dly2 * orig_dly2_sel;
	cur_dly	= orig_dly;
	cur_dly1 = orig_dly1;
	cur_dly2 = orig_dly2;
	cur_dly1_sel = orig_dly1_sel;
	cur_dly2_sel = orig_dly2_sel;

	dly = 0;

	do {
#endif

		for (rsmpl = 0; rsmpl < 2; rsmpl++) {
			cur_rsmpl = (orig_rsmpl + rsmpl) % 2;
			msdc_set_smpl(host, hs400, cur_rsmpl, TYPE_CMD_RESP_EDGE);
			if (host->cur_bus_clk <= 400000) {
				msdc_set_smpl(host, hs400, 0, TYPE_CMD_RESP_EDGE);
			}
			if (cmd->opcode != MMC_CMD_STOP_TRANSMISSION) {
				if (host->app_cmd){
					host->app_cmd = false;
					result = msdc_app_cmd(host);
					host->app_cmd = true;
					if (result != MMC_ERR_NONE)
						return MMC_ERR_CMDTUNEFAIL;
				}
				result = msdc_send_cmd(host, cmd);
				if (result == MMC_ERR_TIMEOUT)
					rsmpl--;
				if (result != MMC_ERR_NONE && cmd->opcode != MMC_CMD_STOP_TRANSMISSION) {
					if (cmd->opcode == MMC_CMD_READ_MULTIPLE_BLOCK
					 || cmd->opcode == MMC_CMD_WRITE_MULTIPLE_BLOCK
					 || cmd->opcode == MMC_CMD_READ_SINGLE_BLOCK
					 || cmd->opcode == MMC_CMD_WRITE_BLOCK
					 || cmd->opcode == MMC_CMD_SEND_WRITE_PROT_TYPE)
						msdc_abort_handler(host,1);
					continue;
				}
				result = msdc_wait_rsp(host, cmd);
			} else if (cmd->opcode == MMC_CMD_STOP_TRANSMISSION) {
				result = MMC_ERR_NONE;
				goto Pass;
			} else
				result = MMC_ERR_BADCRC;

#if MSDC_TUNE_LOG

			times++;

#ifndef FEATURE_MMC_TUNING_EDGE_ONLY_WHEN_HIGH_SPEED
			MSG(INF, "[SD%d] <TUNE_CMD%d><%d><%s> CMDRRDLY=%d, RSPL=%dh\n",
				host->id, (cmd->opcode & (~(SD_CMD_BIT | SD_CMD_APP_BIT))),
				times, (result == MMC_ERR_NONE) ? "PASS" : "FAIL",
				orig_dly + dly, cur_rsmpl);
			MSG(INF, "[SD%d] <TUNE_CMD><%d><%s> CMDRRDLY1=%xh, CMDRRDLY1SEL=%x,"
				" CMDRRDLY2=%xh, CMDRRDLY2SEL=%xh\n",
				host->id, times, (result == MMC_ERR_NONE) ? "PASS" : "FAIL",
				cur_dly1, cur_dly1_sel, cur_dly2, cur_dly2_sel);

#else
			MSG(INF, "[SD%d] <TUNE_CMD%d><%d><%s> RSPL=%dh\n",
				host->id, (cmd->opcode & (~(SD_CMD_BIT | SD_CMD_APP_BIT))),
				times, (result == MMC_ERR_NONE) ? "PASS" : "FAIL",
				cur_rsmpl);
#endif
#endif

			if (result == MMC_ERR_NONE) {
				host->app_cmd = false;
				goto Pass;
			}

			if (cmd->opcode == MMC_CMD_READ_MULTIPLE_BLOCK
			 || cmd->opcode == MMC_CMD_WRITE_MULTIPLE_BLOCK
			 || cmd->opcode == MMC_CMD_READ_SINGLE_BLOCK
			 || cmd->opcode == MMC_CMD_WRITE_BLOCK)
				msdc_abort_handler(host,1);
		}

#ifndef FEATURE_MMC_TUNING_EDGE_ONLY_WHEN_HIGH_SPEED
		cur_dly = (orig_dly + dly + 1) % 63;
		cur_dly1_sel = 1;
		if (cur_dly < 32) {
			cur_dly1 = cur_dly;
			cur_dly2 = 0;
			cur_dly2_sel = 0;
		} else {
			cur_dly1 = 31;
			cur_dly2 = cur_dly - 31;
			cur_dly2_sel = 1;
		}

		if (!host->base_top) {
			MSDC_SET_BIT32(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_CMDRRDLYSEL);
			MSDC_SET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_CMDRDLY, cur_dly1);
			MSDC_SET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_CMDRDLY2, cur_dly2);
			MSDC_SET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_CMDRRDLY2SEL, cur_dly2_sel);
		} else {
			MSDC_SET_BIT32(EMMC_TOP_CMD, PAD_CMD_RD_RXDLY_SEL);
			MSDC_SET_FIELD(EMMC_TOP_CMD, PAD_CMD_RXDLY, cur_dly1);
			MSDC_SET_FIELD(EMMC_TOP_CMD, PAD_CMD_RXDLY2, cur_dly2);
			MSDC_SET_FIELD(EMMC_TOP_CMD, PAD_CMD_RD_RXDLY2_SEL, cur_dly2_sel);
		}
	} while (++dly <= 62);
#endif

	MSG(INF, "msdc tune CMD failed\n");
	return MMC_ERR_CMDTUNEFAIL;
Pass:
	MSG(INF, "msdc tune CMD pass\n");
	return result;
}
#endif


#if defined(FEATURE_MMC_RD_TUNING)
int msdc_tune_bread(struct mmc_host *host, uchar *dst, ulong src, ulong nblks)
{
	int result = MMC_ERR_CMDTUNEFAIL;
	unsigned int orig_rdsmpl, cur_rdsmpl, rdsmpl, orig_clkmode;
	unsigned int rdsmpl_end;
#ifndef FEATURE_MMC_TUNING_EDGE_ONLY_WHEN_HIGH_SPEED
	unsigned int orig_dly1 = 0, orig_dly1_sel, cur_dly1;
	unsigned int orig_dly2 = 0, orig_dly2_sel, cur_dly2, cur_dly1_sel, cur_dly2_sel;
	unsigned int orig_dly = 0, cur_dly;
	unsigned int dly;
#endif
	u32 dcrc = 0;
	u32 hs400;
	u32 base = host->base;
#if MSDC_TUNE_LOG
	u32 times = 0;
#endif

	MSDC_GET_FIELD(MSDC_CFG, MSDC_CFG_CKMOD, orig_clkmode);
	hs400 = (orig_clkmode==3) ? 1 : 0;
	if (orig_clkmode==2 || orig_clkmode==3)
		rdsmpl_end = 0;
	else
		rdsmpl_end = 1;

	MSDC_GET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_RD_DAT_SEL, orig_rdsmpl);

#ifndef FEATURE_MMC_TUNING_EDGE_ONLY_WHEN_HIGH_SPEED
	if (!host->base_top) {
		MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLY, orig_dly1);
		MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLYSEL, orig_dly1_sel);
		MSDC_GET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2, orig_dly2);
		MSDC_GET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2SEL, orig_dly2_sel);
	} else {
		MSDC_GET_FIELD(EMMC_TOP_CONTROL, PAD_DAT_RD_RXDLY, orig_dly1);
		MSDC_GET_FIELD(EMMC_TOP_CONTROL, PAD_DAT_RD_RXDLY_SEL, orig_dly1_sel);
		MSDC_GET_FIELD(EMMC_TOP_CONTROL, PAD_DAT_RD_RXDLY2, orig_dly2);
		MSDC_GET_FIELD(EMMC_TOP_CONTROL, PAD_DAT_RD_RXDLY2_SEL, orig_dly2_sel);
	}
	orig_dly = orig_dly1 * orig_dly1_sel + orig_dly2 * orig_dly2_sel;
	cur_dly	= orig_dly;
	cur_dly1 = orig_dly1;
	cur_dly2 = orig_dly2;
	cur_dly1_sel = orig_dly1_sel;
	cur_dly2_sel = orig_dly2_sel;

	dly = 0;

	do {
#endif
		for (rdsmpl = 0; rdsmpl <= rdsmpl_end; rdsmpl++) {

			cur_rdsmpl = (orig_rdsmpl + rdsmpl) % 2;
			msdc_set_smpl(host, hs400, cur_rdsmpl, TYPE_READ_DATA_EDGE);

			result = host->blk_read(host, dst, src, nblks);
			if (result == MMC_ERR_CMDTUNEFAIL || result == MMC_ERR_CMD_RSPCRC || result == MMC_ERR_ACMD_RSPCRC)
				goto Pass;

			MSDC_GET_FIELD(SDC_DCRC_STS, SDC_DCRC_STS_POS|SDC_DCRC_STS_NEG, dcrc);

#if MSDC_TUNE_LOG

			times++;

#ifndef FEATURE_MMC_TUNING_EDGE_ONLY_WHEN_HIGH_SPEED
			MSG(INF, "[SD%d] <TUNE_READ><%d><%s> DATRRDLY=%d, RSPL=%dh\n",
				host->id, times, (result == MMC_ERR_NONE) ? "PASS" : "FAIL",
				orig_dly + dly, cur_rdsmpl);
			MSG(INF, "[SD%d] <TUNE_READ><%d><%s> DATRRDLY1=%xh, DATRRDLY1SEL=%x,"
				" DATRRDLY2=%xh, DATRRDLY2SEL=%xh\n",
				host->id, times, (result == MMC_ERR_NONE) ? "PASS" : "FAIL",
				cur_dly1, cur_dly1_sel, cur_dly2, cur_dly2_sel);
#else
			MSG(INF, "[SD%d] <TUNE_READ><%d><%s>RSPL=%dh\n",
				host->id, times, (result == MMC_ERR_NONE) ? "PASS" : "FAIL",
				cur_rdsmpl);
#endif
#endif

			if (result == MMC_ERR_NONE && dcrc == 0) {
				goto Pass;
			} else {
				result = MMC_ERR_BADCRC;
			}

		}

#ifndef FEATURE_MMC_TUNING_EDGE_ONLY_WHEN_HIGH_SPEED
		cur_dly = (orig_dly + dly + 1) % 63;
		cur_dly1_sel = 1;
		if (cur_dly < 32) {
			cur_dly1 = cur_dly;
			cur_dly2 = 0;
			cur_dly2_sel = 0;
		} else {
			cur_dly1 = 31;
			cur_dly2 = cur_dly - 31;
			cur_dly2_sel = 1;
		}

		if (!host->base_top) {
			MSDC_SET_BIT32(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLYSEL);
			MSDC_SET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLY, cur_dly1);
			MSDC_SET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2, cur_dly2);
			MSDC_SET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2SEL, cur_dly2_sel);
		} else {
			MSDC_SET_BIT32(EMMC_TOP_CONTROL, PAD_DAT_RD_RXDLY_SEL);
			MSDC_SET_FIELD(EMMC_TOP_CONTROL, PAD_DAT_RD_RXDLY, cur_dly1);
			MSDC_SET_FIELD(EMMC_TOP_CONTROL, PAD_DAT_RD_RXDLY2, cur_dly2);
			MSDC_SET_FIELD(EMMC_TOP_CONTROL, PAD_DAT_RD_RXDLY2_SEL, cur_dly2_sel);
		}
	} while (dly++ <= 62);
#endif

	MSG(INF, "msdc tune bread failed\n");
	return MMC_ERR_CMDTUNEFAIL;
Pass:
	MSG(INF, "msdc tune bread Pass\n");
	return result;
}

int msdc_tune_read(struct mmc_host *host)
{
	int result = MMC_ERR_CMDTUNEFAIL;
	unsigned int orig_rdsmpl, cur_rdsmpl, orig_clkmode;
#ifndef FEATURE_MMC_TUNING_EDGE_ONLY_WHEN_HIGH_SPEED
	unsigned int orig_dly1 = 0, orig_dly1_sel, cur_dly1;
	unsigned int orig_dly2 = 0, orig_dly2_sel, cur_dly2, cur_dly2_sel;
	unsigned int orig_dly = 0, cur_dly;
#endif
	unsigned int hs400;
	u32 base = host->base;
	u32 tune_limit_times;

	MSDC_GET_FIELD(MSDC_CFG, MSDC_CFG_CKMOD, orig_clkmode);
	hs400 = (orig_clkmode==3) ? 1 : 0;
	if (orig_clkmode==2 || orig_clkmode==3) {
#ifndef FEATURE_MMC_TUNING_EDGE_ONLY_WHEN_HIGH_SPEED
		tune_limit_times = 64;
#else
		tune_limit_times = 1;
#endif
	} else {
#ifndef FEATURE_MMC_TUNING_EDGE_ONLY_WHEN_HIGH_SPEED
		tune_limit_times = 127;
#else
		tune_limit_times = 2;
#endif

		MSDC_GET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_RD_DAT_SEL, orig_rdsmpl);
		cur_rdsmpl = (orig_rdsmpl + 1) % 2;
		msdc_set_smpl(host, hs400, cur_rdsmpl, TYPE_READ_DATA_EDGE);

		if (cur_rdsmpl == 1)
			goto out;
	}

#ifndef FEATURE_MMC_TUNING_EDGE_ONLY_WHEN_HIGH_SPEED
	if (!host->base_top) {
		MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLY, orig_dly1);
		MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLYSEL, orig_dly1_sel);
		MSDC_GET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2, orig_dly2);
		MSDC_GET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2SEL, orig_dly2_sel);
	} else {
		MSDC_GET_FIELD(EMMC_TOP_CONTROL, PAD_DAT_RD_RXDLY, orig_dly1);
		MSDC_GET_FIELD(EMMC_TOP_CONTROL, PAD_DAT_RD_RXDLY_SEL, orig_dly1_sel);
		MSDC_GET_FIELD(EMMC_TOP_CONTROL, PAD_DAT_RD_RXDLY2, orig_dly2);
		MSDC_GET_FIELD(EMMC_TOP_CONTROL, PAD_DAT_RD_RXDLY2_SEL, orig_dly2_sel);
	}
	orig_dly = orig_dly1 * orig_dly1_sel + orig_dly2 * orig_dly2_sel;

	cur_dly = (orig_dly + 1) % 63;
	if (cur_dly < 32) {
		cur_dly1 = cur_dly;
		cur_dly2 = 0;
		cur_dly2_sel = 0;
	} else {
		cur_dly1 = 31;
		cur_dly2 = cur_dly - 31;
		cur_dly2_sel = 1;
	}

	if (!host->base_top) {
		MSDC_SET_BIT32(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLYSEL);
		MSDC_SET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLY, cur_dly1);
		MSDC_SET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2, cur_dly2);
		MSDC_SET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2SEL, cur_dly2_sel);
	} else {
		MSDC_SET_BIT32(EMMC_TOP_CONTROL, PAD_DAT_RD_RXDLY_SEL);
		MSDC_SET_FIELD(EMMC_TOP_CONTROL, PAD_DAT_RD_RXDLY, cur_dly1);
		MSDC_SET_FIELD(EMMC_TOP_CONTROL, PAD_DAT_RD_RXDLY2, cur_dly2);
		MSDC_SET_FIELD(EMMC_TOP_CONTROL, PAD_DAT_RD_RXDLY2_SEL, cur_dly2_sel);
	}
#endif

out:
	++(host->time_read);
	if (host->time_read == tune_limit_times)
		result = MMC_ERR_READTUNEFAIL;

	return result;
}
#endif


#if defined(FEATURE_MMC_WR_TUNING)
int msdc_tune_bwrite(struct mmc_host *host, ulong dst, uchar *src, ulong nblks)
{
	int result = MMC_ERR_CMDTUNEFAIL;
	unsigned int orig_dsmpl, cur_dsmpl, dsmpl, dsmpl_end, orig_clkmode;
#ifndef FEATURE_MMC_TUNING_EDGE_ONLY_WHEN_HIGH_SPEED
	unsigned int orig_dly1 = 0, orig_dly1_sel, cur_dly1;
	unsigned int orig_dly2 = 0, orig_dly2_sel, cur_dly2, cur_dly1_sel, cur_dly2_sel;
	unsigned int orig_dly = 0, cur_dly;
	unsigned int dly;
#endif
	u32 base = host->base;
	u8 hs400 = 0;
#if MSDC_TUNE_LOG
	u32 times = 0;
#endif

	MSDC_GET_FIELD(MSDC_CFG, MSDC_CFG_CKMOD, orig_clkmode);
	hs400 = (orig_clkmode == 3) ? 1 : 0;
	if (orig_clkmode==2 || orig_clkmode==3)
		dsmpl_end = 0;
	else
		dsmpl_end = 1;

	if (host->id==0) {
		if (hs400)
			MSDC_GET_FIELD(EMMC50_CFG0, MSDC_EMMC50_CFG_CRC_STS_EDGE, orig_dsmpl);
		else
			MSDC_GET_FIELD(MSDC_PATCH_BIT2, MSDC_PB2_CFGCRCSTSEDGE, orig_dsmpl);
	} else {
		MSDC_GET_FIELD(MSDC_PATCH_BIT2, MSDC_PB2_CFGCRCSTSEDGE, orig_dsmpl);
	}

#ifndef FEATURE_MMC_TUNING_EDGE_ONLY_WHEN_HIGH_SPEED
	if (!host->base_top) {
		MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLY, orig_dly1);
		MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLYSEL, orig_dly1_sel);
		MSDC_GET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2, orig_dly2);
		MSDC_GET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2SEL, orig_dly2_sel);
	} else {
		MSDC_GET_FIELD(EMMC_TOP_CONTROL, PAD_DAT_RD_RXDLY, orig_dly1);
		MSDC_GET_FIELD(EMMC_TOP_CONTROL, PAD_DAT_RD_RXDLY_SEL, orig_dly1_sel);
		MSDC_GET_FIELD(EMMC_TOP_CONTROL, PAD_DAT_RD_RXDLY2, orig_dly2);
		MSDC_GET_FIELD(EMMC_TOP_CONTROL, PAD_DAT_RD_RXDLY2_SEL, orig_dly2_sel);
	}
	orig_dly = orig_dly1 * orig_dly1_sel + orig_dly2 * orig_dly2_sel;
	cur_dly	= orig_dly;
	cur_dly1 = orig_dly1;
	cur_dly2 = orig_dly2;
	cur_dly1_sel = orig_dly1_sel;
	cur_dly2_sel = orig_dly2_sel;

	dly = 0;

	do {
#endif

		for (dsmpl = 0; dsmpl <= dsmpl_end; dsmpl++) {

			cur_dsmpl = (orig_dsmpl + dsmpl) % 2;
			msdc_set_smpl(host, hs400, cur_dsmpl, TYPE_WRITE_CRC_EDGE);

			result = host->blk_write(host, dst, src, nblks);
			if (result == MMC_ERR_CMDTUNEFAIL || result == MMC_ERR_CMD_RSPCRC || result == MMC_ERR_ACMD_RSPCRC)
				goto Pass;

#if MSDC_TUNE_LOG

			times++;

#ifndef FEATURE_MMC_TUNING_EDGE_ONLY_WHEN_HIGH_SPEED
			MSG(INF, "[SD%d] <TUNE_WRITE><%d><%s> DATRRDLY=%d, RDSPL=%dh\n",
				host->id, times, (result == MMC_ERR_NONE) ? "PASS" : "FAIL",
				orig_dly + dly, cur_dsmpl);
			MSG(INF, "[SD%d] <TUNE_WRITE><%d><%s> DATRRDLY1=%xh, DATRRDLY1SEL=%x,"
				" DATRRDLY2=%xh, DATRRDLY2SEL=%xh\n",
				host->id, times, (result == MMC_ERR_NONE) ? "PASS" : "FAIL",
				cur_dly1, cur_dly1_sel, cur_dly2, cur_dly2_sel);
#else
			MSG(INF, "[SD%d] <TUNE_WRITE><%d><%s> DATRRDLY=%d, RDSPL=%dh\n",
				host->id, times, (result == MMC_ERR_NONE) ? "PASS" : "FAIL",
				cur_dsmpl);
#endif
#endif

			if (result == MMC_ERR_NONE) {
				goto Pass;
			} else {
				result = MMC_ERR_BADCRC;
			}
		}

#ifndef FEATURE_MMC_TUNING_EDGE_ONLY_WHEN_HIGH_SPEED
		cur_dly = (orig_dly + dly + 1) % 63;
		cur_dly1_sel = 1;
		if (cur_dly < 32) {
			cur_dly1 = cur_dly;
			cur_dly2 = 0;
			cur_dly2_sel = 0;
		} else {
			cur_dly1 = 31;
			cur_dly2 = cur_dly - 31;
			cur_dly2_sel = 1;
		}

		if (!host->base_top) {
			MSDC_SET_BIT32(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLYSEL);
			MSDC_SET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLY, cur_dly1);
			MSDC_SET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2, cur_dly2);
			MSDC_SET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2SEL, cur_dly2_sel);
		} else {
			MSDC_SET_BIT32(EMMC_TOP_CONTROL, PAD_DAT_RD_RXDLY_SEL);
			MSDC_SET_FIELD(EMMC_TOP_CONTROL, PAD_DAT_RD_RXDLY, cur_dly1);
			MSDC_SET_FIELD(EMMC_TOP_CONTROL, PAD_DAT_RD_RXDLY2, cur_dly2);
			MSDC_SET_FIELD(EMMC_TOP_CONTROL, PAD_DAT_RD_RXDLY2_SEL, cur_dly2_sel);
		}
	} while (dly++ <= 62);
#endif

	MSG(INF, "msdc tune bwrite failed\n");
	return MMC_ERR_CMDTUNEFAIL;
Pass:
	MSG(INF, "msdc tune bwrite Pass\n");
	return result;
}
#endif
