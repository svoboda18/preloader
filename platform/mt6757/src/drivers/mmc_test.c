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

//#define MMC_ICE_DOWNLOAD
//#define MMC_BOOT_TEST
/*=======================================================================*/
/* HEADER FILES                                                          */
/*=======================================================================*/
#include "msdc.h"

#if defined(MMC_MSDC_DRV_LK) && defined(MTK_EMMC_POWER_ON_WP)
#include <platform/partition_wp.h>
#endif

#define MMC_BUF_ADDR       (0x01000000)
#define BLK_SIZE           (512)

#if defined(MMC_TEST)

#define TC_MSG              "[SD%d] <%s> TC%d: %s"

#define MMC_TST_CHK_RESULT   (1)
#define MMC_TST_SBLK_RW      (1)
#define MMC_TST_MBLK_RW      (1)
#define MMC_TST_IMBLK_RW     (1)
#define MMC_TST_COUNTS       (1)

#define MMC_TST_BUF_ADDR     (0x01000000)
#ifdef MMC_PROFILING
#define MMC_TST_SIZE         (4 * 1024 * 1024)  /* 4MB  */
#define MMC_TST_CHUNK_BLKS   (128)              /* 64KB */
#else
#define MMC_TST_SIZE         (1 * 1024 * 1024)  /* 1MB */
#define MMC_TST_CHUNK_BLKS   (128)              /* 8KB */
#endif
#define MMC_TST_START_ADDR   (128 * 1024 * 1024) /* 128MB */
#define MMC_TST_START_BLK    (MMC_TST_START_ADDR / MMC_BLOCK_SIZE)
#define MMC_TST_BLK_NR(x)    (MMC_TST_START_BLK+(x)*(MMC_TST_SIZE/MMC_BLOCK_SIZE))

#define ARRAY_SIZE(x)        (sizeof(x) / sizeof((x)[0]))

static unsigned int clkfreq[]  = { MSDC_200M_SCLK};
static unsigned int buswidth[] = { HOST_BUS_WIDTH_4};

#ifdef MMC_PROFILING
static struct mmc_op_perf mmc_perf[MSDC_MAX_NUM];

struct mmc_op_perf *mmc_prof_handle(int id)
{
	return &mmc_perf[id];
}

void mmc_prof_init(int id, struct mmc_host *host, struct mmc_card *card)
{
	memset(&mmc_perf[id], 0, sizeof(struct mmc_op_perf));
	mmc_perf[id].host = host;
	mmc_perf[id].card = card;
	msdc_timer_init();
	msdc_timer_stop_clear();
}

void mmc_prof_start(void)
{
	msdc_timer_stop_clear();
	msdc_timer_start();
}

void mmc_prof_stop(void)
{
	msdc_timer_stop();
}

unsigned int mmc_prof_count(void)
{
	return msdc_timer_get_count();
}

void mmc_prof_update(mmc_prof_callback cb, ulong id, void *data)
{
	ulong counts = (ulong)msdc_timer_get_count();
	if (cb) {
		cb(data, id, counts);
	}
}

void mmc_prof_report(struct mmc_op_report *rpt)
{
	msdc_printf("\t\tCount      : %d\n", rpt->count);
	msdc_printf("\t\tMax. Time  : %d counts\n", rpt->max_time);
	msdc_printf("\t\tMin. Time  : %d counts\n", rpt->min_time);
	msdc_printf("\t\tTotal Size : %d KB\n", rpt->total_size / 1024);
	msdc_printf("\t\tTotal Time : %d counts\n", rpt->total_time);
	if (rpt->total_time) {
		msdc_printf("\t\tPerformance: %d KB/sec\n",
		       ((rpt->total_size / 1024) * 32768) / rpt->total_time);
	}
}

int mmc_prof_dump(int dev_id)
{
	struct mmc_host *host;
	struct mmc_card *card;
	struct mmc_op_perf *perf;
	u32 total_read_size, total_write_size;
	u32 total_read_time, total_write_time;

	perf = &mmc_perf[dev_id];
	host = mmc_perf[dev_id].host;
	card = mmc_perf[dev_id].card;

	total_read_size = total_write_size = 0;
	total_read_time = total_write_time = 0;

	msdc_printf("\tSD Host ID     : %d\n", dev_id);
	msdc_printf("\tOP Clock Freq. : %d khz\n", host->src_clk / 1000);
	msdc_printf("\tSD Clock Freq. : %d khz\n", host->cur_bus_clk / 1000);
	msdc_printf("\tCard Type      : %s card\n", (card->type == MMC_TYPE_MMC) ? "MMC" : "SD/SDHC/SDXC");
	msdc_printf("\tCard Mode      : UHS1(%d) DDR(%d) HS(%d)\n",
	       mmc_card_uhs1(card) ? 1 : 0, mmc_card_ddr(card) ? 1 : 0,
	       mmc_card_highspeed(card) ? 1 : 0);
	msdc_printf("\tCard Size      : %d MB\n", (card->nblks * card->blklen) / 1024 / 1024);
	msdc_printf("\tCard Max. Freq.: %d khz\n", card->maxhz / 1000);

	if (perf->multi_blks_read.count) {
		msdc_printf("\tMulti-Blks-Read:\n");
		mmc_prof_report(&perf->multi_blks_read);
		total_read_size += perf->multi_blks_read.total_size;
		total_read_time += perf->multi_blks_read.total_time;
	}
	if (perf->multi_blks_write.count) {
		msdc_printf("\tMulti-Blks-Write:\n");
		mmc_prof_report(&perf->multi_blks_write);
		total_write_size += perf->multi_blks_write.total_size;
		total_write_time += perf->multi_blks_write.total_time;
	}
	if (perf->single_blk_read.count) {
		msdc_printf("\tSingle-Blk-Read:\n");
		mmc_prof_report(&perf->single_blk_read);
		total_read_size += perf->single_blk_read.total_size;
		total_read_time += perf->single_blk_read.total_time;
	}
	if (perf->single_blk_write.count) {
		msdc_printf("\tSingle-Blk-Write:\n");
		mmc_prof_report(&perf->single_blk_write);
		total_write_size += perf->single_blk_write.total_size;
		total_write_time += perf->single_blk_write.total_time;
	}
	if (total_read_time) {
		msdc_printf("\tPerformance Read : %d KB/sec\n",
		       ((total_read_size / 1024) * 32768) / total_read_time);
	}
	if (total_write_time) {
		msdc_printf("\tPerformance Write: %d KB/sec\n",
		       ((total_write_size / 1024) * 32768) / total_write_time);
	}

	return 0;
}
#endif

#ifdef MMC_ICE_DOWNLOAD
volatile u32 mmc_download_addr;
volatile u32 mmc_download_size;
volatile u32 mmc_image_addr;

int mmc_download(int dev_id, u32 imgaddr, u32 size, u32 addr, int bootarea)
{
	int ret;
	int i, j, result = 0;
	u8 val;
	u8 *ext_csd;
	uchar *buf, *chkbuf;
	u32 chunks, chunk_blks = 128, left_blks, blknr;
	u32 total_blks;
	struct mmc_card *card;

	if (!size)
		return 0;

	if (addr % MMC_BLOCK_SIZE)
		return MMC_ERR_FAILED;

	card    = mmc_get_card(dev_id);
	ext_csd = &card->raw_ext_csd[0];

	if (bootarea && !mmc_card_sd(card) && card->ext_csd.part_en) {
		/* configure to specified partition */
		val = (ext_csd[EXT_CSD_PART_CFG] & ~0x7) | EXT_CSD_PART_CFG_BOOT_PART_1;
		if (mmc_set_part_config(card, val) != MMC_ERR_NONE) {
			result = -__LINE__;
			goto done;
		}
	}

	blknr      = addr / MMC_BLOCK_SIZE;
	total_blks = (size + MMC_BLOCK_SIZE - 1) / MMC_BLOCK_SIZE;

	/* multiple block write */
	chunks    = total_blks / chunk_blks;
	left_blks = total_blks % chunk_blks;
	buf       = (uchar*)imgaddr;
	chkbuf    = (uchar*)MMC_BUF_ADDR;

	for (i = 0; i < chunks; i++) {
		ret = mmc_block_write(dev_id, blknr + i * chunk_blks,
		                      chunk_blks, (unsigned long*)buf);
		if (ret != MMC_ERR_NONE) {
			result = -__LINE__;
			goto done;
		}
		ret = mmc_block_read(dev_id, blknr + i * chunk_blks,
		                     chunk_blks, (unsigned long*)chkbuf);
		if (ret != MMC_ERR_NONE) {
			result = -__LINE__;
			goto done;
		}

		for (j = 0; j < chunk_blks * MMC_BLOCK_SIZE; j++) {
			if (buf[j] == chkbuf[j])
				continue;
			result = -__LINE__;
			goto done;
		}
		msdc_printf("[SD%d] Write %3d blocks from 0x%.8x(RAM) to 0x%.8x(FLASH).\n",
		       dev_id, chunk_blks, (unsigned int)buf,
		       (blknr + i * chunk_blks) * MMC_BLOCK_SIZE);

		buf += (chunk_blks * MMC_BLOCK_SIZE);
	}

	if (left_blks) {
		ret = mmc_block_write(dev_id, blknr + chunks * chunk_blks,
		                      left_blks, (unsigned long*)buf);
		if (ret != MMC_ERR_NONE) {
			result = -__LINE__;
			goto done;
		}
		ret = mmc_block_read(dev_id, blknr + chunks * chunk_blks,
		                     left_blks, (unsigned long*)chkbuf);
		if (ret != MMC_ERR_NONE) {
			result = -__LINE__;
			goto done;
		}
		for (j = 0; j < left_blks * MMC_BLOCK_SIZE; j++) {
			if (buf[j] == chkbuf[j])
				continue;
			msdc_printf("[SD%d] chkbuf[%d] = %xh (!= %xh) \n", dev_id,
			       j, chkbuf[j], buf[j]);
			result = -__LINE__;
			goto done;
		}
		msdc_printf("[SD%d] Write %3d blocks from 0x%.8x(RAM) to 0x%.8x(FLASH).\n",
		       dev_id, left_blks, (unsigned int)buf,
		       (blknr + chunks * chunk_blks) * MMC_BLOCK_SIZE);
	}

done:
	if (bootarea && !mmc_card_sd(card) && card->ext_csd.part_en) {
		/* configure to user partition */
		val = (ext_csd[EXT_CSD_PART_CFG] & ~0x7) | EXT_CSD_PART_CFG_DEFT_PART;
		if (mmc_set_part_config(card, val) != MMC_ERR_NONE)
			result = -__LINE__;
	}

	if (!result) {
		msdc_printf("[SD%d] Download %d blocks (%d bytes) to 0x%.8x successfully\n",
		       dev_id, total_blks, total_blks * MMC_BLOCK_SIZE, blknr * MMC_BLOCK_SIZE);
	} else {
		msdc_printf("[SD%d] Download %d blocks (%d bytes) to 0x%.8x failed %d\n",
		       dev_id, total_blks, total_blks * MMC_BLOCK_SIZE, blknr * MMC_BLOCK_SIZE, result);
	}
	return result;
}

int mmc_download_part(int dev_id, char *part_name, int bootarea)
{
	int ret = -1;
	struct mmc_card *card;
	struct mmc_host *host;
	part_t *part = mt6573_part_get_partition(part_name);

	mmc_download_addr = 0;
	mmc_download_size = 0;
	mmc_image_addr = 0;

	host = mmc_get_host(dev_id);
	card = mmc_get_card(dev_id);

	if (part) {
		msdc_printf("[SD%d] Waiting for '%s' image loading from ICE...\n", dev_id, part_name);
		while (!mmc_download_size); /* Wait for loading image from ICE */
		ret = mmc_download(dev_id, mmc_image_addr, mmc_download_size,
		                   part->startblk * BLK_SIZE, bootarea);
		if (ret != 0)
			goto done;
		if (bootarea) {
			/* set reset signal function */
			//ret = mmc_set_reset_func(card, 1);
			//if (ret != 0)
			//    goto done;
			/* set boot config */
			ret = mmc_boot_config(card, EXT_CSD_PART_CFG_EN_ACK,
			                      EXT_CSD_PART_CFG_EN_BOOT_PART_1, EXT_CSD_BOOT_BUS_WIDTH_1,
			                      EXT_CSD_BOOT_BUS_MODE_DEFT);
			if (ret != 0)
				goto done;
			ret = mmc_read_ext_csd(host, card);
		}
	}
done:
	return ret;
}
#endif

int mmc_readback_blks(int dev_id, unsigned long addr, int blks, int bootarea)
{
	int i, j, result = 0;
	u8 val;
	u8 *ext_csd;
	unsigned long blknr = addr / MMC_BLOCK_SIZE;
	unsigned char *buf = (unsigned char*)MMC_BUF_ADDR;
	struct mmc_card *card;
	struct mmc_host *host;

	host = mmc_get_host(dev_id);
	card = mmc_get_card(dev_id);
	ext_csd = &card->raw_ext_csd[0];

	if (bootarea && !mmc_card_sd(card)) {
		/* configure to specified partition */
		val = (ext_csd[EXT_CSD_PART_CFG] & ~0x7) | EXT_CSD_PART_CFG_BOOT_PART_1;
		if (mmc_set_part_config(card, val) != MMC_ERR_NONE) {
			result = -__LINE__;
			goto done;
		}
		if (mmc_read_ext_csd(host, card) != MMC_ERR_NONE) {
			result = -__LINE__;
			goto done;
		}
	}

	msdc_printf("[SD%d] Dump %d blks from 0x%lx (FLASH)\n", dev_id, blks,
	       blknr * MMC_BLOCK_SIZE);
	for (i = 0; i < blks; i++) {
		memset(buf, 0, MMC_BLOCK_SIZE);
		if (MMC_ERR_NONE != mmc_block_read(dev_id, blknr + i, 1, (unsigned long*)buf)) {
			msdc_printf("\n[SD%d] Read from %ldth block error\n", dev_id, blknr + i);
			break;
		}

		for (j = 0; j < MMC_BLOCK_SIZE; j++) {
			if (j % 16 == 0)
				msdc_printf("\n%lxh: ", (blknr + i) * MMC_BLOCK_SIZE + j);
			msdc_printf("%x ",  buf[j]);
		}
		msdc_printf("\n");
		buf += MMC_BLOCK_SIZE;
	}
done:

	if (bootarea && !mmc_card_sd(card)) {
		/* configure to user partition */
		val = (ext_csd[EXT_CSD_PART_CFG] & ~0x7) | EXT_CSD_PART_CFG_DEFT_PART;
		if (mmc_set_part_config(card, val) != MMC_ERR_NONE)
			result = -__LINE__;
		if (mmc_read_ext_csd(host, card) != MMC_ERR_NONE) {
			result = -__LINE__;
		}
	}

	return result;
}

#if 0
int mmc_readback_part(int dev_id, char *part_name, int bootarea)
{
	int ret = -1;
	part_t *part = mt6573_part_get_partition(part_name);

	if (part) {
		ret = mmc_readback_blks(dev_id, part->startblk * BLK_SIZE,
		                        1, bootarea);
	}
	return ret;
}
#endif

int mmc_erase_blks(int dev_id, u32 addr, u32 size, int bootarea)
{
	int result = 0;
	u8 val;
	u8 *ext_csd;
	u32 blknr;
	u32 total_blks;
	struct mmc_card *card;

	if (!size)
		return 0;

	if (addr % MMC_BLOCK_SIZE)
		return MMC_ERR_FAILED;

	card    = mmc_get_card(dev_id);
	ext_csd = &card->raw_ext_csd[0];

	blknr      = addr / MMC_BLOCK_SIZE;
	total_blks = (size + MMC_BLOCK_SIZE - 1) / MMC_BLOCK_SIZE;

	if (bootarea && !mmc_card_sd(card) && card->ext_csd.part_en) {
		/* configure to specified partition */
		val = (ext_csd[EXT_CSD_PART_CFG] & ~0x7) | EXT_CSD_PART_CFG_BOOT_PART_1;
		if (mmc_set_part_config(card, val) != MMC_ERR_NONE) {
			result = -__LINE__;
			goto done;
		}
	}

	if (mmc_erase_start(card, blknr * MMC_BLOCK_SIZE) != MMC_ERR_NONE) {
		result = -__LINE__;
		goto done;
	}
	if (mmc_erase_end(card, (blknr + total_blks) * MMC_BLOCK_SIZE) != MMC_ERR_NONE) {
		result = -__LINE__;
		goto done;
	}
	if (mmc_erase(card, MMC_ERASE_NORMAL) != MMC_ERR_NONE) {
		result = -__LINE__;
		goto done;
	}

done:
	if (bootarea && !mmc_card_sd(card) && card->ext_csd.part_en) {
		/* configure to user partition */
		val = (ext_csd[EXT_CSD_PART_CFG] & ~0x7) | EXT_CSD_PART_CFG_DEFT_PART;
		if (mmc_set_part_config(card, val) != MMC_ERR_NONE)
			result = -__LINE__;
	}

	if (!result) {
		msdc_printf("[SD%d] Erase %d blocks (%d bytes) from 0x%x successfully\n",
		       dev_id, total_blks, total_blks * MMC_BLOCK_SIZE, blknr * MMC_BLOCK_SIZE);
	} else {
		msdc_printf("[SD%d] Erase %d blocks (%d bytes) from 0x%x failed %d\n",
		       dev_id, total_blks, total_blks * MMC_BLOCK_SIZE, blknr * MMC_BLOCK_SIZE, result);
	}
	return result;
}

#if 0
int mmc_erase_part(int dev_id, char *part_name, int bootarea)
{
	int ret = -1;
	part_t *part = mt6573_part_get_partition(part_name);

	if (part) {
		/* Notice that the block size is different with different emmc.
		 * Thus, it could overwrite other partitions while erasing data.
		 */
		ret = mmc_erase_blks(dev_id, part->startblk * BLK_SIZE,
		                     part->blknum * BLK_SIZE, bootarea);
	}
	return ret;
}

#endif

#ifdef MMC_BOOT_TEST
int mmc_boot_up_test(int id, int reset)
{
	int err = MMC_ERR_FAILED;
	struct mmc_host *host;

	host = mmc_get_host(id);
	mmc_init_host(host, id);

	msdc_emmc_boot_reset(host, reset);

	err = msdc_emmc_boot_start(host, 25000000, 0, EMMC_BOOT_RST_CMD_MODE, (u64)0);
	if (err) {
		msdc_printf("[EMMC] Boot Error: %d\n", err);
		goto done;
	}
	err = msdc_emmc_boot_read(host, (u64)128 * 1024, MMC_BUF_ADDR);
	msdc_emmc_boot_stop(host);
done:
	if (!err) {
		int i, j;
		char *buf = (char*)MMC_BUF_ADDR;
		for (i = 0; i < 16; i++) {
			for (j = 0; j < MMC_BLOCK_SIZE; j++) {
				if (j % 16 == 0)
					msdc_printf("\n%.8xh: ", i * MMC_BLOCK_SIZE + j);
				msdc_printf("%.2x ",  buf[j]);
			}
			msdc_printf("\n");
			buf += MMC_BLOCK_SIZE;
		}
	}
	return err;
}

int mmc_boot_enable(int id, int bootpart)
{
	int err = MMC_ERR_FAILED;
	struct mmc_host *host;
	struct mmc_card *card;

	host = mmc_get_host(id);
	card = mmc_get_card(id);

	err = mmc_boot_config(card, EXT_CSD_PART_CFG_EN_ACK,
	                      bootpart, EXT_CSD_BOOT_BUS_WIDTH_1, EXT_CSD_BOOT_BUS_MODE_DEFT);
	if (err != 0)
		goto done;
	err = mmc_read_ext_csd(host, card);
done:
	return err;
}
#endif
int mmc_test_dump_buff(char *buffer,int index,int buf_size)
{
	int start_index = index - 256 > 0 ? index-256:0;
	int end_index = index + 256 < buf_size ? index + 256 : buf_size;
	int i = 0;
	msdc_printf("dump error buffer\n");
	for (; i < end_index - start_index; i++) {
		if (i%8 == 0)
			msdc_printf("\n");
		msdc_printf("0x%x ",buffer[start_index+i]);
	}

	return 0;
}

int indata = 0;

int mmc_test_mem_card(struct mmc_test_config *cfg)
{
	int id, count, forever;
	int ret, chk_result, tid = 0, result = 0;
	unsigned int chunks, chunk_blks, left_blks, pass = 0, fail = 0;
	unsigned int total_blks;
	unsigned int i, j;
	unsigned int blksz;
	unsigned int clkhz;
	unsigned int status;
	//unsigned int base;
	char pattern = 0;
	char *buf;
	unsigned long blknr;
	struct mmc_host *host;
	struct mmc_card *card;

	id    = cfg->id;
	count = cfg->count;
	buf   = cfg->buf;
	blknr = cfg->blknr;
	blksz = cfg->blksz;

	chk_result = cfg->chk_result;
	chunk_blks = cfg->chunk_blks;
	total_blks = (cfg->total_size + blksz - 1) / blksz;
	forever    = (count == -1) ? 1 : 0;

	host = mmc_get_host(id);
	card = mmc_get_card(id);

	while (forever || count--) {
			msdc_printf("[TST] ==============================================\n");
			msdc_printf("[TST] read/write buf address : 0x%x\n", (unsigned int)buf);
			msdc_printf("[TST] BEGIN: %d/%d, No Stop(%d)\n",
			       (cfg->count != -1) ? cfg->count - count : 0,
			       (cfg->count != -1) ? cfg->count : 0, forever);
			msdc_printf("[TST] ----------------------------------------------\n");
			msdc_printf("[TST] Mode    : %d\n", cfg->mode);
			msdc_printf("[TST] Clock   : %d kHz\n", cfg->clock / 1000);
			msdc_printf("[TST] BusWidth: %d bits\n", cfg->buswidth);
			msdc_printf("[TST] DDR     : %d \n", (msdc_cap[id].flags & MSDC_DDR)? 1:0);
			msdc_printf("[TST] BurstSz : %d bytes\n", 0x1 << cfg->burstsz);
			msdc_printf("[TST] BlkAddr : %lxh\n", blknr);
			msdc_printf("[TST] BlkSize : %dbytes\n", blksz);
			msdc_printf("[TST] TstBlks : %d\n", total_blks);
			msdc_printf("[TST] AutoCMD : 12(%d), 23(%d)\n",
			       (cfg->autocmd & MSDC_AUTOCMD12) ? 1 : 0,
			       (cfg->autocmd & MSDC_AUTOCMD23) ? 1 : 0);
			msdc_printf("[TST] CheckResult : %d\n", cfg->chk_result);
			msdc_printf("[TST] ----------------------------------------------\n");

			indata= 0;

			if (mmc_init_host(host, id, cfg->clksrc, cfg->mode) != 0) {
				result = -__LINE__;
				goto failure;
			}
			if (mmc_init_card(host, card) != 0) {
				result = -__LINE__;
				goto failure;
			}

			msdc_set_dma(host, (u8)cfg->burstsz, (u32)cfg->flags);
			msdc_set_autocmd(host, cfg->autocmd, 1);


			/* change uhs-1 mode */
#if 0
			if (mmc_card_uhs1(card)) {
				if (mmc_switch_uhs1(host, card, cfg->uhsmode) != 0) {
					result = -__LINE__;
					goto failure;
				}
			}
#endif

			/* change clock */
			msdc_printf("card->maxhz(%d), cfg->clock(%d)\n",card->maxhz,cfg->clock);
			if (cfg->clock) {
				clkhz = card->maxhz < cfg->clock ? card->maxhz : cfg->clock;
				mmc_set_clock(host, mmc_card_ddr(card), clkhz);
			}
			msdc_printf("host->src_clk(%d)\n",host->src_clk);
			if (mmc_card_sd(card) && cfg->buswidth == HOST_BUS_WIDTH_8) {
				msdc_printf("[TST] SD card doesn't support 8-bit bus width (SKIP)\n");
				result = MMC_ERR_NONE;
			}

			msdc_printf("cfg->buswidth(%d)\n",cfg->buswidth);
			if (mmc_set_bus_width(host, card, cfg->buswidth) != 0) {
				result = -__LINE__;
				goto failure;
			}

			/* cmd16 is illegal while card is in ddr mode */
			if (!(mmc_card_mmc(card) && (mmc_card_ddr(card) || mmc_card_hs400(card)))) {
				if (mmc_set_blk_length(host, blksz) != 0) {
					result = -__LINE__;
					goto failure;
				}
			}
			tid = result = 0;

			//msdc_printf("host->sclk(%d)\n",host->sclk);
			if (cfg->piobits) {
				msdc_printf("[TST] PIO bits: %d\n", cfg->piobits);
				msdc_set_pio_bits(host, cfg->piobits);
			}
			//msdc_printf("host->sclk(%d)\n",host->sclk);

			/* show the init test condition, its useful for debug */
			//base = host->base;
			//MSDC_WRITE32(MSDC_PATCH_BIT1, 0xFFFE00C9); /* 2013-1-6 close KPI for e2 eco verify */
			//msdc_dump_register(host);

			if (cfg->start_bit) {
				msdc_printf("start bit=%d\n", (cfg->start_bit - 1));
				msdc_set_startbit(host,(cfg->start_bit - 1));
			}

			if (cfg->axi_burst_len) {
				msdc_printf("axi_burst_len=%d\n", (cfg->axi_burst_len - 1));
				msdc_set_axi_burst_len(host,(cfg->axi_burst_len - 1));
			}

			if (cfg->axi_rd_os) {
				msdc_printf("axi_rd_os=%d\n", (cfg->axi_rd_os));
				msdc_set_axi_outstanding(host, 0, (cfg->axi_rd_os));
			}

			if (cfg->axi_wr_os) {
				msdc_printf("axi_wr_os=%d\n", (cfg->axi_wr_os));
				msdc_set_axi_outstanding(host, 1, (cfg->axi_wr_os));
			}

			tid = result = 0;
#if 0 //UT only, change as if 0 temporarily
			if (mmc_erase_start(card, blknr * blksz) != MMC_ERR_NONE) {
				result = -__LINE__;
				goto failure;
			}
			if (mmc_erase_end(card, (blknr + total_blks) * blksz) != MMC_ERR_NONE) {
				result = -__LINE__;
				goto failure;
			}
			if (mmc_erase(card, MMC_ERASE_NORMAL) != MMC_ERR_NONE) {
				result = -__LINE__;
				goto failure;
			}
			msdc_printf("[TST] 0x%x - 0x%x Erased\n", blknr * blksz,
			       (blknr + total_blks) * blksz);
			msdc_printf("host->sclk(%d)\n",host->sclk);
			msdc_printf(TC_MSG, host->id, result == 0 ? "PASS" : "FAIL", tid++,
			       "test block erase\n");
#endif
			mmc_send_status(host, card, &status);

			if (cfg->tst_single) {

				indata = 1;
#if 1

				/* single block write */
				for (i = 0; i < total_blks; i++) {
					pattern = (i + count + 1) % 256;
					memset(buf, pattern, blksz);
#if 0 // dump the write buf
					msdc_printf("the %d block data in write buf\n", i);
					for (j = 0; j < blksz; j++) {
						msdc_printf("0x%x ", buf[j]);
						if ((j+1)%8 == 0)
							msdc_printf("\n");
					}
#endif
					ret = mmc_block_write(id, blknr + i, 1, (unsigned long*)buf);
					if (ret != MMC_ERR_NONE) {
						msdc_printf("test single block write failed (%d)\n", i);
						result = -__LINE__;
						goto failure;
					}
				}

				msdc_printf(TC_MSG, host->id, result == 0 ? "PASS" : "FAIL", tid++,
				       "test single block write\n");

				if (result)
					break;
#endif
#if 1
				/* single block read */
				for (i = 0; i < total_blks && !result; i++) {
					pattern = (i + count + 1) % 256;
					memset(buf, pattern + 1, blksz);
					ret = mmc_block_read(id, blknr + i, 1, (unsigned long*)buf);
					if (ret != MMC_ERR_NONE) {
						result = -__LINE__;
						goto failure;
					}
					if (chk_result) {
						for (j = 0; j < blksz; j++) {
							if (buf[j] != pattern) {
								msdc_printf("compare,i=%d, j=%d, buf[j]=%d, pattern(%d), buf(0x%x)\n", i, j, buf[j], pattern, (unsigned int)buf);
								mmc_test_dump_buff(buf,j,blksz);
								result = -__LINE__;
								goto failure;
							}
						}
					}
				}
				msdc_printf(TC_MSG, host->id, result == 0 ? "PASS" : "FAIL", tid++,
				       "test single block read\n");

				if (result) {
					msdc_printf("[SD%d]\t\tread back pattern(0x%.2x) failed\n",
					       id, pattern);
					goto failure;
				}
#endif
			}

			mmc_send_status(host, card, &status);

#if 0 // Change mode
			mmc_card_clear_hs400(card);
			mmc_card_clear_ddr(card);

			mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_BUS_WIDTH, EXT_CSD_BUS_WIDTH_8);
			mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 2);
			msdc_printf("[SD0] HS400 change to HS200 TC41 \n");
			if (cfg->clock) {
				clkhz = card->maxhz < cfg->clock ? card->maxhz : cfg->clock;
				msdc_config_clksrc(host, 6); // Change clock source to 200Mzh
				mmc_set_clock(host,card->state, clkhz);
			}
#endif

			if (cfg->tst_multiple) {
				/* multiple block write */
				chunks = total_blks / chunk_blks;
				left_blks = total_blks % chunk_blks;
				msdc_printf("total_blks(%d),chunks(%d),left_blks(%d)\n",total_blks,chunks,left_blks);
#if 1
				for (i = 0; i < chunks; i++) {
					pattern = (i + count) % 256;
					memset(buf, pattern, blksz * chunk_blks);
					msdc_printf("write %d blocks\n",chunk_blks); //Light
					ret = mmc_block_write(id, blknr + i * chunk_blks,
					                      chunk_blks, (unsigned long*)buf);
					if (ret != MMC_ERR_NONE) {
						result = -__LINE__;
						goto failure;
					}
				}

				if (!result && left_blks) {
					pattern = (i + count) % 256;
					memset(buf, pattern, blksz * left_blks);
					ret = mmc_block_write(id, blknr + chunks * chunk_blks,
					                      left_blks, (unsigned long*)buf);
					if (ret != MMC_ERR_NONE) {
						result = -__LINE__;
						goto failure;
					}
				}

				msdc_printf(TC_MSG, host->id, result == 0 ? "PASS" : "FAIL", tid++,
				       "test multiple block write\n");

				if (result)
					goto failure;
#endif

#if 1
				/* multiple block read */
				for (i = 0; i < chunks; i++) {
					pattern = (i + count) % 256;
					memset(buf, pattern + 1, blksz * chunk_blks);
					ret = mmc_block_read(id, blknr + i * chunk_blks,
					                     chunk_blks, (unsigned long*)buf);
					if (ret != MMC_ERR_NONE) {
						msdc_printf("[SD%d]\t\tread %d blks failed(ret = %d blks), buf(0x%x)\n",
						       host->id, chunk_blks, ret, (unsigned int)buf);
						result = -__LINE__;
						goto failure;
					}
					if (chk_result) {
						for (j = 0; j < chunk_blks * blksz; j++) {
							if (buf[j] == pattern)
								continue;
							result = -__LINE__;
							msdc_printf("[SD%d]\t\t%lxh = %x (!= %x)\n",
							       host->id, blknr + i * chunk_blks, buf[j], pattern);
							msdc_printf("j(%d),buf[j](%d),pattern(%d)\n",j,buf[j],pattern);
							msdc_printf("dump read value at addr0x%x", (unsigned int)(buf + j));
							mmc_test_dump_buff(buf,j,chunk_blks * blksz);
							msdc_printf("dump write value at addr0x%x", (unsigned int)(buf + j));
							mmc_test_dump_buff(buf,j,chunk_blks * blksz);
							goto failure;
						}
					}
				}

				if (!result && left_blks) {
					pattern = (i + count) % 256;
					memset(buf, pattern + 1, blksz * left_blks);
					ret = mmc_block_read(id, blknr + chunks * chunk_blks,
					                     left_blks, (unsigned long*)buf);
					if (ret != MMC_ERR_NONE) {
						msdc_printf("[SD%d]\t\tread %d blks failed(ret = %d blks)\n",
						       host->id, left_blks, ret);
						result = -__LINE__;
						goto failure;
					}
					if (chk_result) {
						for (j = 0; j < left_blks * blksz; j++) {
							if (buf[j] == pattern)
								continue;
							msdc_printf("[SD%d]\t\t%lxh = %x (!= %x),j(%d)\n",
							       host->id, blknr + chunks * chunk_blks, buf[j], pattern,j);
							msdc_printf("j(%d),buf[j](%d),pattern(%d)\n",j,buf[j],pattern);
							msdc_printf("dump read value at addr0x%x", (unsigned int)(buf + j));
							mmc_test_dump_buff(buf,j,chunk_blks * blksz);
							msdc_printf("dump write value at addr0x%x", (unsigned int)(buf + j));
							mmc_test_dump_buff(buf,j,chunk_blks * blksz);
							result = -__LINE__;
							goto failure;
						}
					}
				}

				msdc_printf(TC_MSG, host->id, result == 0 ? "PASS" : "FAIL", tid++,
				       "test multiple block read\n");
				if (result)
					goto failure;
#endif
			}

			mmc_send_status(host, card, &status);
#if 1
			if (cfg->tst_interleave) {
				/* multiple block write */
				chunks = total_blks / chunk_blks;
				left_blks = total_blks % chunk_blks;
				for (i = 0; i < chunks; i++) {
					pattern = (i + count) % 256;
					memset(buf, pattern, blksz * chunk_blks);
					ret = mmc_block_write(id, blknr + i * chunk_blks,
					                      chunk_blks, (unsigned long*)buf);
					if (ret != MMC_ERR_NONE) {
						result = -__LINE__;
						goto failure;
					}

					/* populate buffer with different pattern */
					memset(buf, pattern + 1, blksz * chunk_blks);
					ret = mmc_block_read(id, blknr + i * chunk_blks,
					                     chunk_blks, (unsigned long*)buf);
					if (ret != MMC_ERR_NONE) {
						result = -__LINE__;
						goto failure;
					}

					if (chk_result) {
						for (j = 0; j < chunk_blks * blksz; j++) {
							if (buf[j] == pattern)
								continue;
							msdc_printf("[SD%d]\t\t%lxh = %x (!= %x),j(%d)\n",
							       host->id, blknr + i * chunk_blks, buf[j], pattern,j);
							msdc_printf("j(%d),buf[j](%d),pattern(%d)\n",j,buf[j],pattern);
							mmc_test_dump_buff(buf,j,chunk_blks * blksz);
							result = -__LINE__;
							goto failure;
						}
					}
				}

				if (!result && left_blks) {
					pattern = (i + count) % 256;
					memset(buf, pattern, blksz * left_blks);
					ret = mmc_block_write(id, blknr + chunks * chunk_blks,
					                      left_blks, (unsigned long*)buf);
					if (ret != MMC_ERR_NONE) {
						result = -__LINE__;
						goto failure;
					}

					/* populate buffer with different pattern */
					memset(buf, pattern + 1, blksz * left_blks);
					ret = mmc_block_read(id, blknr + chunks * chunk_blks,
					                     left_blks, (unsigned long*)buf);
					if (ret != MMC_ERR_NONE) {
						result = -__LINE__;
						break;
					}
					if (chk_result) {
						for (j = 0; j < left_blks * blksz; j++) {
							if (buf[j] == pattern)
								continue;
							msdc_printf("[SD%d]\t\t%lxh = %x (!= %x),j(%d)\n",
							       host->id, blknr + chunks * chunk_blks, buf[j], pattern,j);
							msdc_printf("j(%d),buf[j](%d),pattern(%d)\n",j,buf[j],pattern);
							mmc_test_dump_buff(buf,j,left_blks * blksz);
							result = -__LINE__;
							goto failure;
						}
					}
				}

				msdc_printf(TC_MSG, host->id, result == 0 ? "PASS" : "FAIL", tid++,
				       "test multiple block interleave write-read\n");

				if (result)
					goto failure;
			}
#endif
			if (cfg->desc) {
				msdc_printf("[TST] ----------------------------------------------\n");
				msdc_printf("[TST] Report - %s \n", cfg->desc);
				msdc_printf("[TST] ----------------------------------------------\n");
			}
			mmc_prof_dump(id);

failure:
			if (result) {
				msdc_printf("[SD%d] mmc test failed (%d)\n", host->id, result);
				fail++;
			} else {
				pass++;
			}
			msdc_printf("[TST] ----------------------------------------------\n");
			msdc_printf("[TST] Test Result: TOTAL(%d/%d), PASS(%d), FAIL(%d) \n",
			       cfg->count - count, cfg->count, pass, fail);
			msdc_printf("[TST] ----------------------------------------------\n");
			//mdelay(1000);
		}

	return result;
}

#if 0
#define MSDC_NO_USE_RANDOM_TEST_PATTERN  (1)
int mmc_test_mem_card(struct mmc_test_config *cfg)
{
	int id, count, forever;
	int ret, chk_result, tid = 0, result = 0;
	unsigned int chunks, chunk_blks, left_blks, pass = 0, fail = 0;
	unsigned int total_blks;
	unsigned int i, j;
	unsigned int blksz;
	unsigned int clkhz;
	unsigned int status;
	unsigned int base;
	char pattern = 0;
	char *buf;
	unsigned long blknr;
	struct mmc_host *host;
	struct mmc_card *card;

#if (0 == MSDC_NO_USE_RANDOM_TEST_PATTERN)
	u32 k, l_low_tick0, l_high_tick0;
#endif

	id    = cfg->id;
	count = cfg->count;
	buf   = cfg->buf;
	blknr = cfg->blknr;
	blksz = cfg->blksz;

	chk_result = cfg->chk_result;
	chunk_blks = cfg->chunk_blks;
	total_blks = (cfg->total_size + blksz - 1) / blksz;
	forever    = (count == -1) ? 1 : 0;

	host = mmc_get_host(id);
	card = mmc_get_card(id);

	while (forever || count--) {
			msdc_printf("[TST] ==============================================\n");
			msdc_printf("[TST]buf address : 0x%x\n",buf);
			msdc_printf("[TST] BEGIN: %d/%d, No Stop(%d)\n",
			       (cfg->count != -1) ? cfg->count - count : 0,
			       (cfg->count != -1) ? cfg->count : 0, forever);
			msdc_printf("[TST] ----------------------------------------------\n");
			msdc_printf("[TST] Clock   : %d kHz\n", cfg->clock / 1000);
			msdc_printf("[TST] BusWidth: %d bits\n", cfg->buswidth);
			msdc_printf("[TST] BurstSz : %d bytes\n", 0x1 << cfg->burstsz);
			msdc_printf("[TST] BlkAddr : %xh\n", blknr);
			msdc_printf("[TST] BlkSize : %dbytes\n", blksz);
			msdc_printf("[TST] TstBlks : %d\n", total_blks);
			msdc_printf("[TST] AutoCMD : 12(%d), 23(%d)\n",
			       (cfg->autocmd & MSDC_AUTOCMD12) ? 1 : 0,
			       (cfg->autocmd & MSDC_AUTOCMD23) ? 1 : 0);
			msdc_printf("[TST] CheckResult : %d\n", cfg->chk_result);
			msdc_printf("[TST] ----------------------------------------------\n");


			if (mmc_init_host(id, host, cfg->clksrc, cfg->mode) != 0) {
				result = -__LINE__;
				goto failure;
			}

//#if (1 == (MSDC_USE_SD30_USH50 || MSDC_USE_SD30_USH104))
			if (cfg->clock > 100000000) {
				host->f_max = MSDC_208M_SCLK;
			} else if (cfg->clock <= 100000000 && cfg->clock > 50000000) {
				host->f_max = MSDC_100M_SCLK;
			} else if (cfg->clock <= 50000000 && cfg->clock > 25000000) {
				host->f_max = MSDC_50M_SCLK;
			} else {
				host->f_max = MSDC_25M_SCLK;
			}
//#endif /* end of MSDC_USE_SD30_USH50 || MSDC_USE_SD30_USH104 */

			if (mmc_init_card(host, card) != 0) {
				result = -__LINE__;
				goto failure;
			}

			msdc_set_dma(host, (u8)cfg->burstsz, (u32)cfg->flags);
			msdc_set_autocmd(host, cfg->autocmd, 1);

			/* change clock */
			msdc_printf("host->sclk(%d), card->maxhz(%d), cfg->clock(%d)\n",host->sclk, card->maxhz, cfg->clock);
			if (cfg->clock) {
				clkhz = card->maxhz < cfg->clock ? card->maxhz : cfg->clock;
				mmc_set_clock(host, card->state, clkhz);
			}

			msdc_printf("host->sclk(%d)\n",host->sclk);
			if (mmc_card_sd(card) && cfg->buswidth == HOST_BUS_WIDTH_8) {
				msdc_printf("[TST] SD card doesn't support 8-bit bus width (SKIP)\n");
				result = MMC_ERR_NONE;
			}

			msdc_printf("cfg->buswidth(%d)\n",cfg->buswidth);
			if (mmc_set_bus_width(host, card, cfg->buswidth) != 0) {
				result = -__LINE__;
				goto failure;
			}

			/* cmd16 is illegal while card is in ddr mode */
			if (!(mmc_card_mmc(card) && (mmc_card_ddr(card)|| mmc_card_hs400(card)))) {
				if (mmc_set_blk_length(host, blksz) != 0) {
					result = -__LINE__;
					goto failure;
				}
			}

			if (cfg->piobits) {
				msdc_printf("[TST] PIO bits: %d\n", cfg->piobits);
				msdc_set_pio_bits(host, cfg->piobits);
			}
			//msdc_printf("host->sclk(%d)\n",host->sclk);

#if 0
//#if (1 == MSDC_USE_SD30)
			/* change uhs-1 mode, switch will failed, but don't know why? */
			if (mmc_card_uhs1(card)) {
				if (mmc_switch_uhs1(host, card, cfg->uhsmode) != 0) {
					result = -__LINE__;
					goto failure;
				}
			}

			mmc_send_status(host, card, &status);
#endif

			/* show the init test condition, its useful for debug */
			base = host->base;
			MSDC_WRITE32(MSDC_PATCH_BIT1, 0xFFFE00C9); /* 2013-1-6 close KPI for e2 eco verify */
			msdc_dump_register(host);

			tid = result = 0;
			if (mmc_erase_start(card, blknr * blksz) != MMC_ERR_NONE) {
				result = -__LINE__;
				goto failure;
			}
			if (mmc_erase_end(card, (blknr + total_blks) * blksz) != MMC_ERR_NONE) {
				result = -__LINE__;
				goto failure;
			}
			if (mmc_erase(card, MMC_ERASE_NORMAL) != MMC_ERR_NONE) {
				result = -__LINE__;
				goto failure;
			}
			msdc_printf("[TST] 0x%x - 0x%x Erased\n", blknr * blksz, (blknr + total_blks) * blksz);

			mmc_send_status(host, card, &status);

			if (cfg->tst_single) {
				/* single block write */
				for (i = 0; i < total_blks; i++) {
#if (1 == MSDC_NO_USE_RANDOM_TEST_PATTERN)
#ifdef MSDC_USE_FIXED_PATTERN1
					pattern = (i + count + 1) % 256;
#endif /* end of MSDC_USE_FIXED_PATTERN1 */

#ifdef MSDC_USE_FIXED_PATTERN2
					pattern = MSDC_TEST_PATTERN;
#endif /* end of MSDC_USE_FIXED_PATTERN2 */
					memset(buf, pattern, blksz);
#else
					for (k = 0; k < blksz; k++) {
						/* random pattern */
						l_low_tick0 = GPT_GetTickCount(&l_high_tick0);
						buf[k] = (l_low_tick0 + i + k) % 255;
						sg_wbuf_single[i][k] = buf[k];

						//msdc_printf("0x%x ", buf[k]);
						//if ((k % 32) == 0)
						//msdc_printf("\n");
					}
#endif /* end of MSDC_NO_USE_RANDOM_TEST_PATTERN */

					//msdc_printf("signal block write: 0x%x\n", blknr + i);
					ret = mmc_block_write(id, blknr + i, 1, (unsigned long*)buf);
					if (ret != MMC_ERR_NONE) {
						msdc_printf("test single block write failed (%d)\n", i);
						result = -__LINE__;
						goto failure;
					}
				}

				msdc_printf(TC_MSG, host->id, result == 0 ? "PASS" : "FAIL", tid++,
				       "test single block write\n");

				if (result)
					break;

				/* single block read */
				for (i = 0; i < total_blks && !result; i++) {
#if (1 == MSDC_NO_USE_RANDOM_TEST_PATTERN)
#ifdef MSDC_USE_FIXED_PATTERN1
					pattern = (i + count + 1) % 256;
#endif /* end of MSDC_USE_FIXED_PATTERN1 */

#ifdef MSDC_USE_FIXED_PATTERN2
					pattern = MSDC_TEST_PATTERN;
#endif /* end of MSDC_USE_FIXED_PATTERN2 */

#endif  /* end of MSDC_NO_USE_RANDOM_TEST_PATTERN */

					memset(buf, 0, blksz);

					//msdc_printf("signal block read: 0x%x\n", blknr + i);
					ret = mmc_block_read(id, blknr + i, 1, (unsigned long*)buf);
					if (ret != MMC_ERR_NONE) {
						result = -__LINE__;
						goto failure;
					}

					if (chk_result) {
						for (j = 0; j < blksz; j++) {
#if (1 == MSDC_NO_USE_RANDOM_TEST_PATTERN)
							if (buf[j] != pattern) {
								msdc_printf("ops:0x%x j(%d),buf[j](%d),pattern(%d)\n", blknr + i, j, buf[j], pattern);
#else
							if (buf[j] != sg_wbuf_single[i][j]) {
								msdc_printf("ops:0x%x j(%d),buf[j](%d),pattern(%d)\n", blknr + i, j, buf[j], sg_wbuf_single[i][j]);
#endif
								mmc_test_dump_buff(buf, j, blksz);
								result = -__LINE__;
								goto failure;
							}
						}
					}
				}
				msdc_printf(TC_MSG, host->id, result == 0 ? "PASS" : "FAIL", tid++,
				       "test single block read\n");

				if (result) {
					msdc_printf("[SD%d]\t\tread back pattern(0x%.2x) failed\n",
					       id, pattern);
					goto failure;
				}
			}

			mmc_send_status(host, card, &status);

			if (cfg->tst_multiple) {
				/* multiple block write */
				chunks = total_blks / chunk_blks;
				left_blks = total_blks % chunk_blks;
				//msdc_printf("total_blks(%d),chunks(%d),left_blks(%d)\n",total_blks,chunks,left_blks);

				for (i = 0; i < chunks; i++) {
#if (1 == MSDC_NO_USE_RANDOM_TEST_PATTERN)
#ifdef MSDC_USE_FIXED_PATTERN1
					pattern = (i + count + 1) % 256;
#endif /* end of MSDC_USE_FIXED_PATTERN1 */

#ifdef MSDC_USE_FIXED_PATTERN2
					pattern = MSDC_TEST_PATTERN;
#endif /* end of MSDC_USE_FIXED_PATTERN2 */
					memset(buf, pattern, blksz * chunk_blks);
#else
					for (k = 0; k < blksz * chunk_blks; k++) {
						/* random pattern */
						l_low_tick0 = GPT_GetTickCount(&l_high_tick0);
						buf[k] = (l_low_tick0 + i + k) % 255;
						sg_wbuf_multi[i][k] = buf[k];

						//msdc_printf("0x%x ", buf[k]);
						//if ((k % 32) == 0)
						//msdc_printf("\n");
					}
#endif /* end of MSDC_NO_USE_RANDOM_TEST_PATTERN */

					//msdc_printf("multi block write: 0x%x\n(%d)", blknr + i* chunk_blks, chunk_blks);
					ret = mmc_block_write(id, blknr + i * chunk_blks,
					                      chunk_blks, (unsigned long*)buf);
					if (ret != MMC_ERR_NONE) {
						result = -__LINE__;
						goto failure;
					}
				}

				if (!result && left_blks) {
#if (1 == MSDC_NO_USE_RANDOM_TEST_PATTERN)
#ifdef MSDC_USE_FIXED_PATTERN1
					pattern = (i + count + 1) % 256;
#endif /* end of MSDC_USE_FIXED_PATTERN1 */

#ifdef MSDC_USE_FIXED_PATTERN2
					pattern = MSDC_TEST_PATTERN;
#endif /*end of MSDC_USE_FIXED_PATTERN2 */
					memset(buf, pattern, blksz * left_blks);
#else
					for (k = 0; k < blksz * left_blks; k++) {
						/* random pattern */
						l_low_tick0 = GPT_GetTickCount(&l_high_tick0);
						buf[k] = (l_low_tick0 + i + k) % 255;
						sg_wbuf_multi[i][k] = buf[k];

						//msdc_printf("0x%x ", buf[k]);
						//if ((k % 32) == 0)
						//msdc_printf("\n");
					}
#endif /* end of MSDC_NO_USE_RANDOM_TEST_PATTERN */

					//msdc_printf("multi block write: 0x%x\n(%d)", blknr + i* chunk_blks, left_blks);
					ret = mmc_block_write(id, blknr + chunks * chunk_blks,
					                      left_blks, (unsigned long*)buf);
					if (ret != MMC_ERR_NONE) {
						result = -__LINE__;
						goto failure;
					}
				}

				msdc_printf(TC_MSG, host->id, result == 0 ? "PASS" : "FAIL", tid++,
				       "test multiple block write\n");

				if (result)
					goto failure;

				/* multiple block read */
				for (i = 0; i < chunks; i++) {
#if (1 == MSDC_NO_USE_RANDOM_TEST_PATTERN)
#ifdef MSDC_USE_FIXED_PATTERN1
					pattern = (i + count + 1) % 256;
#endif

#ifdef MSDC_USE_FIXED_PATTERN2
					pattern = MSDC_TEST_PATTERN;
#endif

#endif /* end of MSDC_NO_USE_RANDOM_TEST_PATTERN */

					/* populate buffer with different pattern */
					memset(buf, 0, blksz * chunk_blks);

					//msdc_printf("multi block read: 0x%x\n(%d)", blknr + i* chunk_blks, chunk_blks);
					ret = mmc_block_read(id, blknr + i * chunk_blks,
					                     chunk_blks, (unsigned long*)buf);
					if (ret != MMC_ERR_NONE) {
						msdc_printf("[SD%d]\t\tread %d blks failed(ret = %d blks)\n",
						       host->id, chunk_blks, ret);
						result = -__LINE__;
						goto failure;
					}

					//pattern = 0x07;
					if (chk_result) {
						for (j = 0; j < chunk_blks * blksz; j++) {
#if (1 == MSDC_NO_USE_RANDOM_TEST_PATTERN)
							if (buf[j] != pattern) {
								msdc_printf("ops:0x%x, j(%d),buf[j](%d),pattern(%d)\n", blknr + i * chunk_blks, j, buf[j], pattern);
#else
							if (buf[j] != sg_wbuf_multi[i][j]) {
								msdc_printf("ops:0x%x, j(%d),buf[j](%d),pattern(%d)\n", blknr + i * chunk_blks, j, buf[j], sg_wbuf_multi[i][j]);
#endif

								mmc_test_dump_buff(buf, j, blksz);
								result = -__LINE__;
								goto failure;
							}
						}
					}
				}

				if (!result && left_blks) {
#if (1 == MSDC_NO_USE_RANDOM_TEST_PATTERN)
#ifdef MSDC_USE_FIXED_PATTERN1
					pattern = (i + count + 1) % 256;
#endif

#ifdef MSDC_USE_FIXED_PATTERN2
					pattern = MSDC_TEST_PATTERN;
#endif

#endif /* end of MSDC_NO_USE_RANDOM_TEST_PATTERN */

					/* populate buffer with different pattern */
					memset(buf, 0, blksz * left_blks);

					//msdc_printf("multi block read: 0x%x\n(%d)", blknr + i* chunk_blks, left_blks);
					ret = mmc_block_read(id, blknr + chunks * chunk_blks,
					                     left_blks, (unsigned long*)buf);
					if (ret != MMC_ERR_NONE) {
						msdc_printf("[SD%d]\t\tread %d blks failed(ret = %d blks)\n",
						       host->id, left_blks, ret);
						result = -__LINE__;
						goto failure;
					}

					if (chk_result) {
						for (j = 0; j < left_blks * blksz; j++) {
#if (1 == MSDC_NO_USE_RANDOM_TEST_PATTERN)
							if (buf[j] != pattern) {
								msdc_printf("ops:0x%x, j(%d),buf[j](%d),pattern(%d)\n", blknr + i * chunk_blks, j, buf[j], pattern);
#else
							if (buf[j] != sg_wbuf_multi[i][j]) {
								msdc_printf("ops:0x%x, j(%d),buf[j](%d),pattern(%d)\n", blknr + i * chunk_blks, j, buf[j], sg_wbuf_multi[i][j]);
#endif

								mmc_test_dump_buff(buf, j, blksz);
								result = -__LINE__;
								goto failure;
							}
						}
					}
				}

				msdc_printf(TC_MSG, host->id, result == 0 ? "PASS" : "FAIL", tid++,
				       "test multiple block read\n");

				if (result)
					goto failure;
			}

			mmc_send_status(host, card, &status);

			if (cfg->tst_interleave) {
				/* multiple block write */
				chunks = total_blks / chunk_blks;
				left_blks = total_blks % chunk_blks;
				for (i = 0; i < chunks; i++) {
#if (1 == MSDC_NO_USE_RANDOM_TEST_PATTERN)
#ifdef MSDC_USE_FIXED_PATTERN1
					pattern = (i + count + 1) % 256;
#endif

#ifdef MSDC_USE_FIXED_PATTERN2
					pattern = MSDC_TEST_PATTERN;
#endif
					memset(buf, pattern, blksz * chunk_blks);
#else
					for (k = 0; k < blksz * chunk_blks; k++) {
						/* random pattern */
						l_low_tick0 = GPT_GetTickCount(&l_high_tick0);
						buf[k] = (l_low_tick0 + i + k) % 255;
						sg_wbuf_multi[i][k] = buf[k];

						//msdc_printf("0x%x ", buf[k]);
						//if ((k % 32) == 0)
						//msdc_printf("\n");
					}
#endif /* end of MSDC_NO_USE_RANDOM_TEST_PATTERN */

					//msdc_printf("multi block write: 0x%x\n(%d)", blknr + i* chunk_blks, chunk_blks);
					ret = mmc_block_write(id, blknr + i * chunk_blks,
					                      chunk_blks, (unsigned long*)buf);
					if (ret != MMC_ERR_NONE) {
						result = -__LINE__;
						goto failure;
					}


					/* populate buffer with different pattern */
					memset(buf, 0, blksz * chunk_blks);
					//msdc_printf("multi block read: 0x%x\n(%d)", blknr + i* chunk_blks, chunk_blks);
					ret = mmc_block_read(id, blknr + i * chunk_blks,
					                     chunk_blks, (unsigned long*)buf);
					if (ret != MMC_ERR_NONE) {
						result = -__LINE__;
						goto failure;
					}
					//pattern = 0x07;
					if (chk_result) {
						for (j = 0; j < chunk_blks * blksz; j++) {
#if (1 == MSDC_NO_USE_RANDOM_TEST_PATTERN)
							if (buf[j] != pattern) {
								msdc_printf("ops:0x%x, j(%d),buf[j](%d),pattern(%d)\n", blknr + i * chunk_blks, j, buf[j], pattern);
#else
							if (buf[j] != sg_wbuf_multi[i][j]) {
								msdc_printf("ops:0x%x, j(%d),buf[j](%d),pattern(%d)\n", blknr + i * chunk_blks, j, buf[j], sg_wbuf_multi[i][j]);
#endif
								mmc_test_dump_buff(buf, j, blksz);
								result = -__LINE__;
								goto failure;
							}
						}
					}
				}

				if (!result && left_blks) {
#if (1 == MSDC_NO_USE_RANDOM_TEST_PATTERN)
#ifdef MSDC_USE_FIXED_PATTERN1
					pattern = (i + count + 1) % 256;
#endif

#ifdef MSDC_USE_FIXED_PATTERN2
					pattern = MSDC_TEST_PATTERN;
#endif
					memset(buf, pattern, blksz * left_blks);
#else
					for (k = 0; k < blksz * left_blks; k++) {
						/* random pattern */
						l_low_tick0 = GPT_GetTickCount(&l_high_tick0);
						buf[k] = (l_low_tick0 + i + k) % 255;
						sg_wbuf_multi[i][k] = buf[k];

						//msdc_printf("0x%x ", buf[k]);
						//if ((k % 32) == 0)
						//msdc_printf("\n");
					}
#endif /* end of MSDC_NO_USE_RANDOM_TEST_PATTERN */

					//msdc_printf("multi block write: 0x%x\n(%d)", blknr + i* chunk_blks, left_blks);
					ret = mmc_block_write(id, blknr + chunks * chunk_blks,
					                      left_blks, (unsigned long*)buf);
					if (ret != MMC_ERR_NONE) {
						result = -__LINE__;
						goto failure;
					}

					/* populate buffer with different pattern */
					memset(buf, 0, blksz * left_blks);

					//msdc_printf("multi block read: 0x%x\n(%d)", blknr + i* chunk_blks, left_blks);
					ret = mmc_block_read(id, blknr + chunks * chunk_blks,
					                     left_blks, (unsigned long*)buf);
					if (ret != MMC_ERR_NONE) {
						result = -__LINE__;
						break;
					}
					//pattern = 0x07;
					if (chk_result) {
						for (j = 0; j < left_blks * blksz; j++) {
#if (1 == MSDC_NO_USE_RANDOM_TEST_PATTERN)
							if (buf[j] != pattern) {
								msdc_printf("ops:0x%x, j(%d),buf[j](%d),pattern(%d)\n", blknr + i * chunk_blks, j, buf[j], pattern);
#else
							if (buf[j] != sg_wbuf_multi[i][j]) {
								msdc_printf("ops:0x%x, j(%d),buf[j](%d),pattern(%d)\n", blknr + i * chunk_blks, j, buf[j], sg_wbuf_multi[i][j]);
#endif
								mmc_test_dump_buff(buf, j, blksz);
								result = -__LINE__;
								goto failure;
							}
						}
					}
				}

				msdc_printf(TC_MSG, host->id, result == 0 ? "PASS" : "FAIL", tid++,
				       "test multiple block interleave write-read\n");

				if (result)
					goto failure;
			}

			if (cfg->desc) {
				msdc_printf("[TST] ----------------------------------------------\n");
				msdc_printf("[TST] Report - %s \n", cfg->desc);
				msdc_printf("[TST] ----------------------------------------------\n");
			}
			mmc_prof_dump(id);

failure:
			if (result) {
				msdc_printf("[SD%d] mmc test failed (%d)\n", host->id, result);
				fail++;
			} else {
				pass++;
			}
			msdc_printf("[TST] ----------------------------------------------\n");
			msdc_printf("[TST] Test Result: TOTAL(%d/%d), PASS(%d), FAIL(%d) \n",
			       cfg->count - count, cfg->count, pass, fail);
			msdc_printf("[TST] ----------------------------------------------\n");
		}

	return result;
}
#endif

int mmc_test(int argc, char *argv[])
{
	unsigned int i, j;
	int result = -1;
	struct mmc_test_config cfg;

#ifdef MMC_ICE_DOWNLOAD
	mmc_readback_part(0, PART_UBOOT, 0);
	mmc_erase_part(0, PART_UBOOT, 0);
	mmc_readback_part(0, PART_UBOOT, 0);
	mmc_download_part(0, PART_UBOOT, 0);
	mmc_readback_part(0, PART_UBOOT, 0);
	while (1);
#endif

#ifdef MMC_BOOT_TEST
	msdc_printf("[EMMC] Boot up with power reset (MMCv4.3 above)\n");
	mmc_boot_up_test(0, EMMC_BOOT_PWR_RESET);
	mdelay(100);
	for (i = 0; i < 20; i++) {
		msdc_printf("[EMMC] Boot up with RST_n reset (MMCv4.41 above)\n");
		mmc_boot_up_test(0, EMMC_BOOT_RST_N_SIG);
		mdelay(100);
	}
	msdc_printf("[EMMC] Boot up with PRE_IDLE_CMD reset (MMCv4.41 above)\n");
	mmc_boot_up_test(0, EMMC_BOOT_PRE_IDLE_CMD);

	//mmc_boot_enable(0, EXT_CSD_PART_CFG_EN_NO_BOOT);
	//mmc_boot_enable(0, EXT_CSD_PART_CFG_EN_BOOT_PART_1);
#endif

	memset(&cfg, 0, sizeof(struct mmc_test_config));

	cfg.id = 0;
	cfg.desc = "Memory Card Read/Write Test";
	cfg.count = MMC_TST_COUNTS;
	cfg.blksz = MMC_BLOCK_SIZE;
	cfg.blknr = MMC_TST_BLK_NR(0);
	cfg.total_size = MMC_TST_SIZE;
	cfg.chunk_blks = MMC_TST_CHUNK_BLKS;

	cfg.buf = (char*) MMC_TST_BUF_ADDR;
#if defined(MMC_MSDC_DRV_PRELOADER) || defined(MMC_MSDC_DRV_LK)
	cfg.buf = (char*) malloc(MMC_TST_SIZE);
#endif

	cfg.chk_result = MMC_TST_CHK_RESULT;
	cfg.tst_single = MMC_TST_SBLK_RW;
	cfg.tst_multiple = MMC_TST_MBLK_RW;
	cfg.tst_interleave = MMC_TST_IMBLK_RW;
	cfg.mode = MSDC_MODE_DEFAULT;

	for (i = 0; i < ARRAY_SIZE(clkfreq); i++) {
		for (j = 0; j < ARRAY_SIZE(buswidth); j++) {
			cfg.clock = clkfreq[i];
			cfg.buswidth = buswidth[j];
			if (mmc_test_mem_card(&cfg) != 0)
				goto exit;
		}
	}
	result = 0;
	//mmc_readback_blks(0, MMC_TST_BLK_NR(0) * MMC_BLOCK_SIZE, 16, 0);

exit:
	while (1);

	return result;
}

#if defined(MMC_MSDC_DRV_PRELOADER)
#ifdef MTK_MSDC_PL_TEST
#define PL_MMC_TEST_SIZE   (8*512)
unsigned char g_mmc_buf[PL_MMC_TEST_SIZE + 1];
void emmc_r_w_compare_test()
{
	unsigned int i;

	/* write 0x40000000(1G) with 256 block(0x5a) */
	msdc_printf("[PL][%s:%d]eMMC simp test of user parittion start\n", __func__, __LINE__);
	for (i = 0; i < PL_MMC_TEST_SIZE; i++) {
		g_mmc_buf[i] = 0x5a;
	}

	/* 0 is for emmc */
	if (mmc_block_write(0, 0x40000000/512, PL_MMC_TEST_SIZE/512, g_mmc_buf))
		msdc_printf("[PL][%s:%d] Write user partition failed\n", __func__, __LINE__);
	else
		msdc_printf("[PL][%s:%d] finish write user partition\n", __func__, __LINE__);

	/* read */
	for (i = 0; i < PL_MMC_TEST_SIZE; i++) {
		g_mmc_buf[i] = 0x0;
	}
	if (mmc_block_read(0, 0x40000000/512, PL_MMC_TEST_SIZE/512, g_mmc_buf))
		msdc_printf("[PL][%s:%d] Read user partition failed\n", __func__, __LINE__);
	else
		msdc_printf("[PL][%s:%d] finish read user partition \n", __func__, __LINE__);

	/* compara */
	for (i = 0; i < PL_MMC_TEST_SIZE; i++) {
		if (g_mmc_buf[i] != 0x5a) {
			msdc_printf("[PL][%s:%d]mmc simple r/w user partition compare failed(%d is %d, not 0x5a)\n", __func__, __LINE__, i, g_mmc_buf[i]);
			break;
		}
	}
	/* write 0x40000000(1G) with 256 block(0x5a) */
#if 0
	msdc_printf("[PL][%s:%d]eMMC simp test of boot partition start\n", __func__, __LINE__);
	for (i = 0; i < PL_MMC_TEST_SIZE; i++) {
		g_mmc_buf[i] = 0x5a;
	}

	/* 0 is for emmc */
	mmc_bwrite_boot(NULL, 0x40000000/512, PL_MMC_TEST_SIZE/512, g_mmc_buf);
	msdc_printf("[PL][%s:%d] finish write boot partition\n", __func__, __LINE__);
#endif
	/* read */
	for (i = 0; i < PL_MMC_TEST_SIZE; i++) {
		g_mmc_buf[i] = 0x0;
	}
	if (mmc_bread_boot(NULL, 0x0/512, PL_MMC_TEST_SIZE/512, g_mmc_buf))
		msdc_printf("[PL][%s:%d] Read boot partition failed\n", __func__, __LINE__);
	else
		msdc_printf("[PL][%s:%d] finish read boot partition\n", __func__, __LINE__);
#if 0
	/* compara */
	for (i = 0; i < PL_MMC_TEST_SIZE; i++) {
		if (g_mmc_buf[i] != 0x5a) {
			msdc_printf("[PL][%s:%d]mmc simple r/w boot partition compare failed(%d is %d, not 0x5a)\n", __func__, __LINE__, i, g_mmc_buf[i]);
			break;
		}
	}
#endif
	msdc_printf("[PL][%s:%d]eMMC simp test end\n", __func__, __LINE__);

}
#endif
#endif

#if defined(MMC_MSDC_DRV_LK) && defined(MTK_EMMC_POWER_ON_WP)
#define LK_MMC_TEST_SIZE   (8*512)
unsigned char g_mmc_buf[LK_MMC_TEST_SIZE + 1];
static void emmc_r_w_compare_test(struct mmc_card *card, u32 part_id, unsigned long blknr,
                                  u32 blkcnt)
{
	unsigned int i;
	int err = 0;
	//unsigned char g_mmc_buf[LK_MMC_TEST_SIZE + 1];
	/* write 0x40000000(1G) with 256 block(0x5a) , maybe 8 block*/
	if (blkcnt > 8) {
		dmsdc_printf(CRITICAL,"blkcnt is invalid\n");
		return;
	}
	dmsdc_printf(CRITICAL, "[LK][%s:%d]eMMC WP_FEATURE_TEST start\n", __func__, __LINE__);
	for (i = 0; i < 512 * blkcnt; i++) {
		g_mmc_buf[i] = 0x5a;
	}
	/* 0 is for emmc */
	//err = mmc_block_write(0, blknr, blkcnt, (unsigned long *)g_mmc_buf, part_id);
	err = mmc_part_write(card, part_id, blknr, blkcnt, (unsigned long *)g_mmc_buf);
	if (err == MMC_ERR_WP_VIOLATION) {
		dmsdc_printf(CRITICAL, "[LK][%s:%d] MMC_ERR_WP_VIOLATION\n", __func__, __LINE__);
		return;
	}
	if (err) {
		dmsdc_printf(CRITICAL, "[LK][%s:%d] other write error(%d)\n", __func__, __LINE__, err);
		return;
	}
	dmsdc_printf(CRITICAL, "[LK][%s:%d] finish write\n", __func__, __LINE__);
	/* read */
	for (i = 0; i < 512 * blkcnt; i++) {
		g_mmc_buf[i] = 0x0;
	}
	//mmc_block_read(0, blknr, blkcnt, (unsigned long *)g_mmc_buf, part_id);
	mmc_part_read(card, part_id, blknr, blkcnt, (unsigned long *)g_mmc_buf);
	dmsdc_printf(CRITICAL, "[LK][%s:%d] finish read\n", __func__, __LINE__);
	/* compara */
	for (i = 0; i < 512 * blkcnt; i++) {
		if (g_mmc_buf[i] != 0x5a) {
			dmsdc_printf(CRITICAL, "[LK][%s:%d]mmc WP_FEATURE_TEST r/w compare failed(%d is %d, not 0x5a)\n", __func__, __LINE__, i, g_mmc_buf[i]);
			break;
		}
	}
	dmsdc_printf(CRITICAL, "[LK][%s:%d]eMMC simp test end\n", __func__, __LINE__);
}

int mmc_wp_test()
{
	int err;

#if 0
	int dev_num = 0;
	unsigned long blknr;
	u32 blkcnt;
	u32 part_id;
	u8 case_id=1;
//	struct mmc_host *host = mmc_get_host(dev_num);
	struct mmc_card *card = mmc_get_card(dev_num);


	//case 1
	/*  blknr = 2;
	    blkcnt = 6;
	    part_id = EMMC_PART_BOOT1;
	    dmsdc_printf(CRITICAL, "[case %d] blknr=%ld, blkcnt=%x,partition=%d \n",
	                case_id,blknr,blkcnt,part_id);
	    err = mmc_set_write_protect(dev_num, part_id, blknr, blkcnt, WP_POWER_ON);
	    if(err)
	        dmsdc_printf(CRITICAL, "[%s]: (case %d) wp error\n", __func__,case_id++);
	    else
	        dmsdc_printf(CRITICAL, "[%s]: (case %d) wp successfully\n", __func__,case_id++);
	*/

	//case2
	blknr = 0;
	blkcnt = card->wp_size;
	part_id = EMMC_PART_USER;
	dmsdc_printf(CRITICAL, "[case %d] blknr=0x%x, blkcnt=0x%x,partition=%d \n",
	        case_id,blknr,blkcnt,part_id);
	err = mmc_set_write_protect(dev_num, part_id, blknr, blkcnt, WP_POWER_ON);
	if (err)
		dmsdc_printf(CRITICAL, "[%s]: (case %d) wp error\n", __func__,case_id++);
	else
		dmsdc_printf(CRITICAL, "[%s]: (case %d) wp successfully\n", __func__,case_id++);


	//case3
	blknr = card->wp_size;
	blkcnt = 10 * card->wp_size;
	part_id = EMMC_PART_USER;
	dmsdc_printf(CRITICAL, "[case %d] blknr=0x%x, blkcnt=0x%x,partition=%d \n",
	        case_id,blknr,blkcnt,part_id);
	u64 time_before= gpt4_get_current_tick();//%lld
	err = mmc_set_write_protect(dev_num, part_id, blknr, blkcnt, WP_POWER_ON);
	u64 time_after= gpt4_get_current_tick();//%lld
	if (err)
		dmsdc_printf(CRITICAL, "[%s]: (case %d) wp error,[time cost of 10 wp grps]: %lld \n",
		        __func__,case_id++,(time_after - time_before)*76);
	else
		dmsdc_printf(CRITICAL, "[%s]: (case %d) wp successfully\n", __func__,case_id++);


	//case4
	blknr = 1;
	blkcnt = card->wp_size;
	part_id = EMMC_PART_USER;
	dmsdc_printf(CRITICAL, "[case %d] blknr=0x%x, blkcnt=0x%x,partition=%d \n",
	        case_id,blknr,blkcnt,part_id);
	err = mmc_set_write_protect(dev_num, part_id, blknr, blkcnt, WP_POWER_ON);
	if (err)
		dmsdc_printf(CRITICAL, "[%s]: (case %d) wp error\n", __func__,case_id++);
	else
		dmsdc_printf(CRITICAL, "[%s]: (case %d) wp successfully\n", __func__,case_id++);


	//case5
	blknr = card->wp_size;
	blkcnt = 1;
	part_id = EMMC_PART_USER;
	dmsdc_printf(CRITICAL, "[case %d] blknr=0x%x, blkcnt=0x%x,partition=%d \n",
	        case_id,blknr,blkcnt,part_id);
	err = mmc_set_write_protect(dev_num, part_id, blknr, blkcnt, WP_POWER_ON);
	if (err)
		dmsdc_printf(CRITICAL, "[%s]: (case %d) wp error\n", __func__,case_id++);
	else
		dmsdc_printf(CRITICAL, "[%s]: (case %d) wp successfully\n", __func__,case_id++);


	//case6
	blknr = card->wp_size;
	blkcnt = card->nblks;
	part_id = EMMC_PART_USER;
	dmsdc_printf(CRITICAL, "[case %d] blknr=0x%x, blkcnt=0x%x,partition=%d \n",
	        case_id,blknr,blkcnt,part_id);
	err = mmc_set_write_protect(dev_num, part_id, blknr, blkcnt, WP_POWER_ON);
	if (err)
		dmsdc_printf(CRITICAL, "[%s]: (case %d) wp error\n", __func__,case_id++);
	else
		dmsdc_printf(CRITICAL, "[%s]: (case %d) wp successfully\n", __func__,case_id++);


	//case7
	blknr = card->wp_size;
	blkcnt = card->wp_size;;
	part_id = EMMC_PART_END;
	dmsdc_printf(CRITICAL, "[case %d] blknr=0x%x, blkcnt=0x%x,partition=%d \n",
	        case_id,blknr,blkcnt,part_id);
	err = mmc_set_write_protect(dev_num, part_id, blknr, blkcnt, WP_POWER_ON);
	if (err)
		dmsdc_printf(CRITICAL, "[%s]: (case %d) wp error\n", __func__,case_id++);
	else
		dmsdc_printf(CRITICAL, "[%s]: (case %d) wp successfully\n", __func__,case_id++);


	//case8
	blknr = 11 * card->wp_size;
	blkcnt = card->wp_size;
	part_id = EMMC_PART_USER;
	dmsdc_printf(CRITICAL, "[case %d] blknr=0x%x, blkcnt=0x%x,partition=%d \n",
	        case_id,blknr,blkcnt,part_id);
	err = mmc_set_write_protect(dev_num, part_id, blknr, blkcnt, WP_PERMANENT);
	if (err)
		dmsdc_printf(CRITICAL, "[%s]: (case %d) wp error\n", __func__,case_id++);
	else
		dmsdc_printf(CRITICAL, "[%s]: (case %d) wp successfully\n", __func__,case_id++);


	//case9
	blknr = 0;
	blkcnt = card->wp_size;
	part_id = EMMC_PART_GP1;
	dmsdc_printf(CRITICAL, "[case %d] blknr=0x%x, blkcnt=0x%x,partition=%d \n",
	        case_id,blknr,blkcnt,part_id);
	err = mmc_set_write_protect(dev_num, part_id, blknr, blkcnt, WP_POWER_ON);
	if (err)
		dmsdc_printf(CRITICAL, "[%s]: (case %d) wp error\n", __func__,case_id++);
	else
		dmsdc_printf(CRITICAL, "[%s]: (case %d) wp successfully\n", __func__,case_id++);


	//case10
	blknr = 0;
	blkcnt = card->wp_size;
	part_id = EMMC_PART_USER;
	dmsdc_printf(CRITICAL, "[case %d] blknr=0x%x, blkcnt=0x%x,partition=%d \n",
	        case_id,blknr,blkcnt,part_id);
	err = mmc_set_write_protect(dev_num, part_id, blknr, blkcnt, WP_TEMPORARY);
	if (err)
		dmsdc_printf(CRITICAL, "[%s]: (case %d) wp error\n", __func__,case_id++);
	else
		dmsdc_printf(CRITICAL, "[%s]: (case %d) wp successfully\n", __func__,case_id++);

#endif
	// partiton test case
	dmsdc_printf(CRITICAL, "[%s]: (partition case: 1) preloader->lk\n", __func__);
	err = partition_write_prot_set("preloader", "lk", WP_POWER_ON);
	dmsdc_printf(CRITICAL, "[%s]: (partition case: 1) preloader->lk, err=%d\n", __func__, err);

	dmsdc_printf(CRITICAL, "[%s]: (partition case: 2) boot->recovery\n", __func__);
	err = partition_write_prot_set("boot", "recovery", WP_POWER_ON);
	dmsdc_printf(CRITICAL, "[%s]: (partition case: 2) boot->recovery, err=%d\n", __func__, err);

	dmsdc_printf(CRITICAL, "[%s]: (partition case: 3) keystore->oemkeystore\n", __func__);
	err = partition_write_prot_set("keystore", "oemkeystore", WP_POWER_ON);
	dmsdc_printf(CRITICAL, "[%s]: (partition case: 3) keystore->oemkeystore, err=%d\n", __func__, err);

	dmsdc_printf(CRITICAL, "[%s]: (partition case: 4) oemkeystore->secro\n", __func__);
	err = partition_write_prot_set("oemkeystore", "secro", WP_POWER_ON);
	dmsdc_printf(CRITICAL, "[%s]: (partition case: 4) oemkeystore->secro, err=%d\n", __func__, err);

	dmsdc_printf(CRITICAL, "[%s]: (partition case: 5) test_start->keystore\n", __func__);
	err = partition_write_prot_set("test_starte", "keystor", WP_POWER_ON);
	dmsdc_printf(CRITICAL, "[%s]: (partition case: 5) test_start->keystore, err=%d\n", __func__, err);

	dmsdc_printf(CRITICAL, "[%s]: (partition case: 6) oemkeystore->test_end\n", __func__);
	err = partition_write_prot_set("oemkeystore", "test_end", WP_POWER_ON);
	dmsdc_printf(CRITICAL, "[%s]: (partition case: 6) oemkeystore->test_end, err=%d\n", __func__, err);

	dmsdc_printf(CRITICAL, "[%s]: (partition case: 7) keystore->keystore\n", __func__);
	err = partition_write_prot_set("keystore", "keystore", WP_POWER_ON);
	dmsdc_printf(CRITICAL, "[%s]: (partition case: 7) keystore->keystore, err=%d\n", __func__, err);

	return err;
}
#endif

#endif /* MMC_TEST */

