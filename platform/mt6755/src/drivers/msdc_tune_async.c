/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
*/
/* MediaTek Inc. (C) 2015. All rights reserved.
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

#if defined(FEATURE_ASYNC_PATH_ENABLE)

#if defined(FEATURE_MMC_CM_TUNING)
int msdc_async_tune_cmd(struct mmc_host *host, struct mmc_command *cmd)
{
	int result = MMC_ERR_CMDTUNEFAIL;
	unsigned int orig_rsmpl, cur_rsmpl, rsmpl;
	unsigned int orig_dly1 = 0, orig_dly1_sel, cur_dly1;
	unsigned int orig_dly2 = 0, orig_dly2_sel, cur_dly2;
	unsigned int orig_dly = 0, cur_dly;
	unsigned int dly;
	u32 times = 0;
	u32 base = host->base;

	u8 hs400 = 0, orig_clkmode;

	MSDC_GET_FIELD(MSDC_CFG, MSDC_CFG_CKMOD, orig_clkmode);
	hs400 = (orig_clkmode == 3) ? 1 : 0;
	MSDC_GET_FIELD(MSDC_IOCON, MSDC_IOCON_RSPL, orig_rsmpl);
	MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_CMDRDLY, orig_dly1);
	MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_CMDRRDLYSEL, orig_dly1_sel);
	MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE1_CMDRDLY2, orig_dly2);
	MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE1_CMDRRDLY2SEL, orig_dly2_sel);
	orig_dly = orig_dly1 * orig_dly1_sel + orig_dly2 * orig_dly2_sel;

	//printf("BBB1 %x %x %x\n", MSDC_READ32(0x112400b0), MSDC_READ32(0x112400b4), MSDC_READ32(0x112400b8));
	dly = 0;
	do {
		for (rsmpl = 0; rsmpl < 2; rsmpl++) {
			cur_rsmpl = (orig_rsmpl + rsmpl) % 2;
			msdc_set_smpl(host, hs400, cur_rsmpl, TYPE_CMD_RESP_EDGE);
			if (host->cur_bus_clk <= 400000) {
				msdc_set_smpl(host, hs400, 0, TYPE_CMD_RESP_EDGE);
			}
			if (cmd->opcode != MMC_CMD_STOP_TRANSMISSION) {
				if (host->app_cmd) {
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
			/* for debugging */
			{
				u32 t_dly1, t_dly2, t_dly1_sel, t_dly2_sel, t_rsmpl;

				MSDC_GET_FIELD(MSDC_IOCON, MSDC_IOCON_RSPL, t_rsmpl);
				MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_CMDRDLY, t_dly1);
				MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_CMDRRDLYSEL, t_dly1_sel);
				MSDC_GET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_CMDRDLY2, t_dly2);
				MSDC_GET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_CMDRRDLY2SEL, t_dly2_sel);

				times++;
				msdc_printf("[SD%d] <TUNE_CMD%d><%d><%s> CMDRRDLY=%d, RSPL=%dh\n",
				            host->id, (cmd->opcode & (~(SD_CMD_BIT | SD_CMD_APP_BIT))),
				            times, (result == MMC_ERR_NONE) ? "PASS" : "FAIL",
				            t_dly1 * t_dly1_sel + t_dly2 * t_dly2_sel, t_rsmpl);
				msdc_printf("[SD%d] <TUNE_CMD><%d><%s> CMDRRDLY1=%xh, CMDRRDLY1SEL=%x,"
				            " CMDRRDLY2=%xh, CMDRRDLY2SEL=%x,h\n",
				            host->id, times, (result == MMC_ERR_NONE) ? "PASS" : "FAIL",
				            t_dly1, t_dly1_sel, t_dly2, t_dly2_sel);
			}
#endif

			if (result == MMC_ERR_NONE) {
				host->app_cmd = false;
				goto Pass;
			}

			if (cmd->opcode == MMC_CMD_READ_MULTIPLE_BLOCK
			        || cmd->opcode == MMC_CMD_WRITE_MULTIPLE_BLOCK
			        || cmd->opcode == MMC_CMD_READ_SINGLE_BLOCK
			        ||cmd->opcode == MMC_CMD_WRITE_BLOCK)
				msdc_abort_handler(host,1);
		}

		cur_dly = (orig_dly + dly + 1) % 63;
		if (cur_dly < 32) {
			MSDC_SET_BIT32(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_CMDRRDLYSEL);
			MSDC_CLR_BIT32(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_CMDRRDLY2SEL);
			MSDC_SET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_CMDRDLY, cur_dly);
		} else {
			MSDC_SET_BIT32(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_CMDRRDLYSEL);
			MSDC_SET_BIT32(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_CMDRRDLY2SEL);
			MSDC_SET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_CMDRDLY, cur_dly);
			MSDC_SET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_CMDRDLY2, cur_dly - 31);
		}
	} while (++dly < 62);

failed:
	msdc_printf("Msdc async tune CMD failed\n");
	return MMC_ERR_CMDTUNEFAIL;
Pass:
	msdc_printf("Msdc async tune CMD Pass\n");
	return result;
}
#endif


#if defined(FEATURE_MMC_RD_TUNING)
int msdc_async_tune_bread(struct mmc_host *host, uchar *dst, ulong src, ulong nblks)
{
	int result = MMC_ERR_CMDTUNEFAIL;
	unsigned int orig_rdsmpl, cur_rdsmpl, rdsmpl;
	unsigned int rdsmpl_end;
	unsigned int orig_dly1 = 0, orig_dly1_sel, cur_dly1;
	unsigned int orig_dly2 = 0, orig_dly2_sel, cur_dly2;
	unsigned int orig_dly = 0, cur_dly;
	unsigned int dly;
	u32 times = 0;
	u32 dcrc = 0;
	u32 hs400;
	u32 base = host->base;
	u8 ddr = 0, orig_clkmode;

	MSDC_GET_FIELD(MSDC_CFG, MSDC_CFG_CKMOD, orig_clkmode);
	hs400 = (orig_clkmode==3) ? 1 : 0;
	if (orig_clkmode==2 || orig_clkmode==3)
		ddr = 1;
	else
		ddr = 0;

	MSDC_GET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_RD_DAT_SEL, orig_rdsmpl);

	MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLY, orig_dly1);
	MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLYSEL, orig_dly1_sel);
	MSDC_GET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2, orig_dly2);
	MSDC_GET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2SEL, orig_dly2_sel);
	orig_dly = orig_dly1 * orig_dly1_sel + orig_dly2 * orig_dly2_sel;

	if (host->id == 0 || ddr)
		rdsmpl_end = 0;
	else
		rdsmpl_end = 1;

	dly = 0;
	do {

		for (rdsmpl = 0; rdsmpl <= rdsmpl_end; rdsmpl++) {

			cur_rdsmpl = (orig_rdsmpl + rdsmpl) % 2;
			msdc_set_smpl(host, hs400, cur_rdsmpl, TYPE_READ_DATA_EDGE);

			result = host->blk_read(host, dst, src, nblks);
			if (result == MMC_ERR_CMDTUNEFAIL || result == MMC_ERR_CMD_RSPCRC || result == MMC_ERR_ACMD_RSPCRC)
				goto Pass;

			MSDC_GET_FIELD(SDC_DCRC_STS, SDC_DCRC_STS_POS|SDC_DCRC_STS_NEG, dcrc);

#if MSDC_TUNE_LOG
			/* for debugging */
			{
				u32 t_dly1, t_dly2, t_dly1_sel, t_dly2_sel, t_rdsmpl;
				times++;

				if (hs400)
					MSDC_GET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D_SMPL, t_rdsmpl);
				else
					MSDC_GET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_RD_DAT_SEL, t_rdsmpl);
				MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLY, t_dly1);
				MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLYSEL, t_dly1_sel);
				MSDC_GET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2, t_dly2);
				MSDC_GET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2SEL, t_dly2_sel);

				msdc_printf("[SD%d] <TUNE_READ><%d><%s> DATRRDLY=%d, RSPL=%dh\n",
				            host->id, times, (result == MMC_ERR_NONE) ? "PASS" : "FAIL",
				            t_dly1 * t_dly1_sel + t_dly2 * t_dly2_sel, t_rdsmpl);
				msdc_printf("[SD%d] <TUNE_READ><%d><%s> DATRRDLY1=%xh, DATRRDLY1SEL=%x,"
				            " DATRRDLY2=%xh, DATRRDLY2SEL=%x,h\n",
				            host->id, times, (result == MMC_ERR_NONE) ? "PASS" : "FAIL",
				            t_dly1, t_dly1_sel, t_dly2, t_dly2_sel);
			}
#endif

			if (result == MMC_ERR_NONE && dcrc == 0) {
				goto Pass;
			} else {
				result = MMC_ERR_BADCRC;
			}

		}

		cur_dly = (orig_dly + dly + 1) % 63;
		if (cur_dly < 32) {
			MSDC_SET_BIT32(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLYSEL);
			MSDC_CLR_BIT32(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2SEL);
			MSDC_SET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLY, cur_dly);
			MSDC_SET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2, 0);
		} else {
			MSDC_SET_BIT32(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLYSEL);
			MSDC_SET_BIT32(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2SEL);
			MSDC_SET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLY, 31);
			MSDC_SET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2, cur_dly - 31);
		}

	} while (dly++ < 62);

failed:
	msdc_printf("Msdc async tune bread failed\n");
	return MMC_ERR_CMDTUNEFAIL;
Pass:
	msdc_printf("Msdc async tune bread Pass\n");
	return result;
}

int msdc_async_tune_read(struct mmc_host *host)
{
	int result = MMC_ERR_CMDTUNEFAIL;
	unsigned int orig_rdsmpl, cur_rdsmpl, rdsmpl = 0;
	unsigned int orig_dly1 = 0, orig_dly1_sel, cur_dly1;
	unsigned int orig_dly2 = 0, orig_dly2_sel, cur_dly2;
	unsigned int orig_dly = 0, cur_dly;
	unsigned int dly;
	unsigned int hs400;
	u32 base = host->base;
	u32 tune_limit_times;

	u8 ddr = 0, orig_clkmode;

	MSDC_GET_FIELD(MSDC_CFG, MSDC_CFG_CKMOD, orig_clkmode);
	hs400 = (orig_clkmode==3) ? 1 : 0;
	if (orig_clkmode==2 || orig_clkmode==3)
		ddr = 1;
	else
		ddr = 0;

	if (host->id == 0 || ddr) {
		tune_limit_times = 64;
	} else {
		tune_limit_times = 127;

		MSDC_GET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_RD_DAT_SEL, orig_rdsmpl);
		cur_rdsmpl = (orig_rdsmpl + rdsmpl) % 2;
		msdc_set_smpl(host, hs400, cur_rdsmpl, TYPE_READ_DATA_EDGE);

		if (cur_rdsmpl == 1)
			goto out;
	}

	MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLY, orig_dly1);
	MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLYSEL, orig_dly1_sel);
	MSDC_GET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2, orig_dly2);
	MSDC_GET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2SEL, orig_dly2_sel);
	orig_dly = orig_dly1 * orig_dly1_sel + orig_dly2 * orig_dly2_sel;

	cur_dly = (orig_dly + 1) % 63;
	if (cur_dly < 32) {
		MSDC_SET_BIT32(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_CMDRRDLYSEL);
		MSDC_CLR_BIT32(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_CMDRRDLY2SEL);
		MSDC_SET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_CMDRDLY, cur_dly);
	} else {
		MSDC_SET_BIT32(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_CMDRRDLYSEL);
		MSDC_SET_BIT32(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_CMDRRDLY2SEL);
		MSDC_SET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_CMDRDLY, cur_dly);
		MSDC_SET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_CMDRDLY2, cur_dly - 31);
	}

out:
	++(host->time_read);
	if (host->time_read == tune_limit_times)
		result = MMC_ERR_READTUNEFAIL;

	return result;
}
#endif


#if defined(FEATURE_MMC_WR_TUNING)
int msdc_async_tune_bwrite(struct mmc_host *host, ulong dst, uchar *src, ulong nblks)
{
	int result = MMC_ERR_CMDTUNEFAIL;
	unsigned int orig_dsmpl, cur_dsmpl, dsmpl, dsmpl_end;
	unsigned int orig_dly1 = 0, orig_dly1_sel, cur_dly1;
	unsigned int orig_dly2 = 0, orig_dly2_sel, cur_dly2;
	unsigned int orig_dly = 0, cur_dly;
	unsigned int dly;
	u32 times = 0;
	u32 base = host->base;
	u8 hs400 = 0, orig_clkmode, ddr;

	MSDC_GET_FIELD(MSDC_CFG, MSDC_CFG_CKMOD, orig_clkmode);
	hs400 = (orig_clkmode == 3) ? 1 : 0;
	if (orig_clkmode==2 || orig_clkmode==3)
		ddr = 1;
	else
		ddr = 0;

	if (host->id==0) {
		if (hs400)
			MSDC_GET_FIELD(EMMC50_CFG0, MSDC_EMMC50_CFG_CRC_STS_EDGE, orig_dsmpl);
		else
			MSDC_GET_FIELD(MSDC_PATCH_BIT2, MSDC_PB2_CFGCRCSTSEDGE, orig_dsmpl);
	} else {
		MSDC_GET_FIELD(MSDC_PATCH_BIT2, MSDC_PB2_CFGCRCSTSEDGE, orig_dsmpl);
	}

	MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLY, orig_dly1);
	MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLYSEL, orig_dly1_sel);
	MSDC_GET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2, orig_dly2);
	MSDC_GET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2SEL, orig_dly2_sel);
	orig_dly = orig_dly1 * orig_dly1_sel + orig_dly2 * orig_dly2_sel;

	if (host->id == 0 || ddr)
		dsmpl_end = 0;
	else
		dsmpl_end = 1;
	dly = 0;
	do {
		for (dsmpl = 0; dsmpl <= dsmpl_end; dsmpl++) {

			cur_dsmpl = (orig_dsmpl + dsmpl) % 2;
			msdc_set_smpl(host, hs400, cur_dsmpl, TYPE_WRITE_CRC_EDGE);

			result = host->blk_write(host, dst, src, nblks);
			if (result == MMC_ERR_CMDTUNEFAIL || result == MMC_ERR_CMD_RSPCRC || result == MMC_ERR_ACMD_RSPCRC)
				goto Pass;

#if MSDC_TUNE_LOG
			/* for debugging */
			{
				u32 t_dly1, t_dly2, t_dly1_sel, t_dly2_sel, t_dsmpl;
				times++;

				if (hs400)
					MSDC_GET_FIELD(EMMC50_CFG0, MSDC_EMMC50_CFG_CRC_STS_EDGE, t_dsmpl);
				else
					MSDC_GET_FIELD(MSDC_PATCH_BIT2, MSDC_PB2_CFGCRCSTSEDGE, t_dsmpl);
				MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLY, t_dly1);
				MSDC_GET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLYSEL, t_dly1_sel);
				MSDC_GET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2, t_dly2);
				MSDC_GET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2SEL, t_dly2_sel);

				msdc_printf("[SD%d] <TUNE_WRITE><%d><%s> DATRRDLY=%d, RDSPL=%dh\n",
				            host->id, times, (result == MMC_ERR_NONE) ? "PASS" : "FAIL",
				            t_dly1 * t_dly1_sel + t_dly2 * t_dly2_sel, t_dsmpl);
				msdc_printf("[SD%d] <TUNE_WRITE><%d><%s> DATRRDLY1=%xh, DATRRDLY1SEL=%x,"
				            " DATRRDLY2=%xh, DATRRDLY2SEL=%xh\n",
				            host->id, times, (result == MMC_ERR_NONE) ? "PASS" : "FAIL",
				            t_dly1, t_dly1_sel, t_dly2, t_dly2_sel);
			}
#endif

			if (result == MMC_ERR_NONE) {
				goto Pass;
			} else {
				result = MMC_ERR_BADCRC;
			}
		}

		cur_dly = (orig_dly + dly + 1) % 63;
		if (cur_dly < 32) {
			MSDC_SET_BIT32(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLYSEL);
			MSDC_CLR_BIT32(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2SEL);
			MSDC_SET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLY, cur_dly);
			MSDC_SET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2, 0);
		} else {
			MSDC_SET_BIT32(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLYSEL);
			MSDC_SET_BIT32(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2SEL);
			MSDC_SET_FIELD(MSDC_PAD_TUNE0, MSDC_PAD_TUNE0_DATRRDLY, 31);
			MSDC_SET_FIELD(MSDC_PAD_TUNE1, MSDC_PAD_TUNE1_DATRRDLY2, cur_dly - 31);
		}

	} while (dly++ < 62);

failed:
	msdc_printf("Msdc async tune bwrite failed\n");
	return MMC_ERR_CMDTUNEFAIL;
Pass:
	msdc_printf("Msdc async tune bwrite Pass\n");
	return result;
}
#endif

#endif