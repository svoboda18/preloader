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

#define TC_MSG              "[SD%d] <%s> TC%d: "

#define MMC_TST_CHK_RESULT   (1)
#define MMC_TST_SBLK_RW      (1)
#define MMC_TST_MBLK_RW      (1)
#define MMC_TST_IMBLK_RW     (1)
#define MMC_TST_COUNTS       (1)

#define ARRAY_SIZE(x)        (sizeof(x) / sizeof((x)[0]))

static unsigned int clkfreq[]  = {2000000};
static unsigned int buswidth[] = {HOST_BUS_WIDTH_4};

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


/* r: read only,  t: read only and update by hw
 * z: write 1 and then auto clear by hw
 * w: write only, it cannot be tested since read is nonsense
 * a: readable & writable
 * x: don't care, do not test
 * k: read clear, there is no such bits in MSDC and therefore no test code about it
 * s: readable and write 1 set, there is no such bits in MSDC and therefore no test code about it
 * c: readable and write 1 clear, to test of it need to trigger HW to set it first and therefore it test is covered by normal usage flow
 *
 * RU:  read only bit value update by the hw, denote as 't', like MSDC_RXDATA, MSDC_CFG[7]:CARD_CK_STABLE
 * RO:  read only, denote as 'r', like MSDC_VERSION
 * RW:  writeable and readable, denote as 'a'
 * WO:  write only, denote as 'w', like MSDC_TXDATA
 * W1:  write once, denote as '', not used yet.
 * RC:  clear on read, denote as 'k', not used yet.
 * A1:  auto set by the hw.
 * AO:  auto clear by the hw, denote as 'z' like MSDC_CS[31]
 * DC:  don't care, denote as 'x'
 * W1C: write 1 to bit wise-clear, denote as 'c', like SDC_CSTS
 * note: RU means the content of the regiter will change,
 *       RO means the content of the register will not change
 */

/* need confirmed with cui
 * card detect bit 0x8[1] default is set to 0, why fpga set to 1?
 * msdc inten bit 0x10[20,21.22], not define, why fpga test it as readonly bit? emmc read as '0', but sd card read as '1'
 * msdc cmd bit 0x34[31], not define, why fpga test it as r/w bit? emmc read as '0', but sd card read as '1'
 * msdc dma cfg bit 0x9c[17], not define, why fpga test it as readonly bit? emmc read as '0', but sd card read as '1'
 * sdwp 0x08[31], default is set to 0, why fpga set to 1? */

static reg_desc_t msdc0_reg_desc[] = {

	{OFFSET_MSDC_CFG      ,        {"aazaaattaaaaaaaaaaaaccaaaaxxxxxx"}, {"10011001000000000000000001xxxxxx"}}, //bit[2] is rw, but not test. cause this is a reset bit. write to "1" will back to "0" when reset sequence done
	{OFFSET_MSDC_IOCON    ,        {"aaaaaaxxaaaaaaxxaaaaaaaaxxxxxxxx"}, {"000000xx000000xx0000000000xxxxxx"}},//bit[24][25] can not write(Not in MT8320 MSDC IP)
	{OFFSET_MSDC_PS       ,        {"arxxxxxxxxxxaaaarrrrrrrrrxxxxxxr"}, {"01xxxxxxxxxx0000111111111xxxxxx1"}},//bit[1]/bit[16-20]/bit[24]/bit[31] default to 0
	{OFFSET_MSDC_INT      ,        {"ccxccccccccrccccccccccccxxxxxxxx"}, {"00x000000000000000000000xxxxxxxx"}},
	{OFFSET_MSDC_INTEN    ,        {"aaxaaaaaaaaaaaaaaaaarrraxxxxxxxx"}, {"00x000000000000000000000xxxxxxxx"}},//bit[2] to RW, default to 0
	{OFFSET_MSDC_FIFOCS   ,        {"rrrrrrrrxxxxxxxxrrrrrrrrxxxxxxxz"}, {"00000000xxxxxxxx00000000xxxxxxx0"}},//bit[0]~[7] are variable if you use memory tool in Code visor (memory tool must be closed)
	{OFFSET_MSDC_TXDATA   ,        {"wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"}, {"00000000000000000000000000000000"}},
	//{OFFSET_MSDC_RXDATA   ,      {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},/* Should not be touched. */
	{OFFSET_SDC_CFG       ,        {"aaxxxxxxxxxxxxxxaaxaaaxxaaaaaaaa"}, {"00xxxxxxxxxxxxxx00x010xx00000000"}},
	{OFFSET_SDC_CMD       ,        {"aaaaaaaaaaxaaaaaaaaaaaaaaaaaaaar"}, {"0000000000x000000000000000000000"}},
	{OFFSET_SDC_ARG       ,        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_STS       ,        {"ttxxxxxxxxxxxxxxcxxxxxxxxxxxxxxt"}, {"00xxxxxxxxxxxxxx0xxxxxxxxxxxxxx0"}},
	{OFFSET_SDC_RESP0     ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_RESP1     ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_RESP2     ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_RESP3     ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_BLK_NUM   ,        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"10000000000000000000000000000000"}},
	{OFFSET_SDC_VOL_CHG   ,        {"aaaaaaaaaaaaaaaaxxxxxxxxxxxxxxxx"}, {"1010001010000000xxxxxxxxxxxxxxxx"}},
	{OFFSET_SDC_CSTS      ,        {"cccccccccccccccccccccccccccccccc"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_CSTS_EN   ,        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_DCRC_STS  ,        {"rrrrrrrrrrrrrrrrxxxxxxxxxxxxxxxx"}, {"0000000000000000xxxxxxxxxxxxxxxx"}},//bit[12-15] modify
	{OFFSET_EMMC_CFG0     ,        {"wwaaxxxxxxxxaaaaxxxxxxxxxxxxxxxx"}, {"0000xxxxxxxx0000xxxxxxxxxxxxxxxx"}},
	{OFFSET_EMMC_CFG1     ,        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"11000000000000000000010000000000"}},
	{OFFSET_EMMC_STS      ,        {"ccccrcrxxxxxxxxxxxxxxxxxxxxxxxxx"}, {"0000000xxxxxxxxxxxxxxxxxxxxxxxxx"}},////bit[6]=1,0 is wrong,maybe has relationship with OFFSET_MSDC_FIFOCS register bit[0]~[7] which are variable
	{OFFSET_EMMC_IOCON    ,        {"axxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"}, {"0xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"}},
	{OFFSET_SDC_ACMD_RESP ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_ACMD19_TRG,        {"aaaaxxxxxxxxxxxxxxxxxxxxxxxxxxxx"}, {"0000xxxxxxxxxxxxxxxxxxxxxxxxxxxx"}},
	{OFFSET_SDC_ACMD19_STS,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_MSDC_DMA_SA_HIGH,      {"aaaaxxxxxxxxxxxxxxxxxxxxxxxxxxxx"}, {"0000xxxxxxxxxxxxxxxxxxxxxxxxxxxx"}},
	{OFFSET_MSDC_DMA_SA   ,        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"00000000000000000000000000000000"}},
	{OFFSET_MSDC_DMA_CA   ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_MSDC_DMA_CTRL ,        {"wrwxxxxxaxaxaaaxxxxxxxxxxxxxxxxx"}, {"000xxxxx0x0x011xxxxxxxxxxxxxxxxx"}}, //need new design, bit[1] = AO
	{OFFSET_MSDC_DMA_CFG  ,        {"raxxxxxxaaxxaaxxarxxxxxxxxxxxxxx"}, {"00xxxxxx00xx00xx00xxxxxxxxxxxxxx"}},
	{OFFSET_MSDC_DBG_SEL  ,        {"aaaaaaaaaaaaaaaaxxxxxxxxxxxxxxxx"}, {"0000000000000000xxxxxxxxxxxxxxxx"}},
	{OFFSET_MSDC_DBG_OUT  ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_MSDC_DMA_LEN  ,        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"00000000000000000000000000000000"}}, //new register in MT6582
	{OFFSET_MSDC_PATCH_BIT0,       {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"11110000000000000011110000000010"}},
	{OFFSET_MSDC_PATCH_BIT1,       {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"10010000000000001111111111111111"}},
	{OFFSET_MSDC_PATCH_BIT2,       {"aaaaaaaaaaxaaaaaaaaxaaaaaaaaaaaa"}, {"11000000000110000000000100101000"}},
	{OFFSET_DAT0_TUNE_CRC,         {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_DAT1_TUNE_CRC,         {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_DAT2_TUNE_CRC,         {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_DAT3_TUNE_CRC,         {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_CMD_TUNE_CRC,          {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},

	{OFFSET_MSDC_PAD_TUNE0,        {"aaaaaxxxaaaaaxxxaaaaaxaaaaaaaaaa"}, {"00000xxx00000xxx00000x0000000000"}},
	{OFFSET_MSDC_PAD_TUNE1,        {"xxxxxxxxaaaaaaxxaaaaaaxxxxxxxxxx"}, {"xxxxxxxx000000xx000000xxxxxxxxxx"}},
	{OFFSET_MSDC_DAT_RDDLY0,       {"aaaaaxxxaaaaaxxxaaaaaxxxaaaaaxxx"}, {"00000xxx00000xxx00000xxx00000xxx"}},
	{OFFSET_MSDC_DAT_RDDLY1,       {"aaaaaxxxaaaaaxxxaaaaaxxxaaaaaxxx"}, {"00000xxx00000xxx00000xxx00000xxx"}},
	{OFFSET_MSDC_DAT_RDDLY2,       {"aaaaaxxxaaaaaxxxaaaaaxxxaaaaaxxx"}, {"00000xxx00000xxx00000xxx00000xxx"}},
	{OFFSET_MSDC_DAT_RDDLY3,       {"aaaaaxxxaaaaaxxxaaaaaxxxaaaaaxxx"}, {"00000xxx00000xxx00000xxx00000xxx"}},
	{OFFSET_MSDC_HW_DBG   ,        {"aaaaaaaaaaaaaaxxaaaaaaaaaaaaaaax"}, {"00000000000000xx000000000000000x"}},
	{OFFSET_MSDC_VERSION  ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"11000000010010001100100000000100"}},//20131203
	{OFFSET_MSDC_ECO_VER  ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"10000000000000000000000000000000"}},//0

	{OFFSET_EMMC50_PAD_CTL0,       {"aaaaaaaaaaxxxxxxxxxxxxxxxxxxxxxx"}, {"0000000000xxxxxxxxxxxxxxxxxxxxxx"}},
	{OFFSET_EMMC50_PAD_DS_CTL0,    {"aaaaaaaaaaaaaaaaaaaxxxxxxxxxxxxx"}, {"0011100000000000000xxxxxxxxxxxxx"}},
	{OFFSET_EMMC50_PAD_DS_TUNE,    {"aaaaaaaaaaaaaaaaaxxxxxxxxxxxxxxx"}, {"10101000000000101xxxxxxxxxxxxxxx"}},
	{OFFSET_EMMC50_PAD_CMD_TUNE,   {"aaaaaaaaaaaxxxxxxxxxxxxxxxxxxxxx"}, {"00000000000xxxxxxxxxxxxxxxxxxxxx"}},
	{OFFSET_EMMC50_PAD_DAT01_TUNE, {"aaaaaaaaaaaxxxxxaaaaaaaaaaaxxxxx"}, {"00000000000xxxxx00000000000xxxxx"}},
	{OFFSET_EMMC50_PAD_DAT23_TUNE, {"aaaaaaaaaaaxxxxxaaaaaaaaaaaxxxxx"}, {"00000000000xxxxx00000000000xxxxx"}},
	{OFFSET_EMMC50_PAD_DAT45_TUNE, {"aaaaaaaaaaaxxxxxaaaaaaaaaaaxxxxx"}, {"00000000000xxxxx00000000000xxxxx"}},
	{OFFSET_EMMC50_PAD_DAT67_TUNE, {"aaaaaaaaaaaxxxxxaaaaaaaaaaaxxxxx"}, {"00000000000xxxxx00000000000xxxxx"}},
	{OFFSET_EMMC51_CFG0,           {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"00101000000000000110000100001000"}},
	{OFFSET_EMMC50_CFG0,           {"aaaaaaaaaaaaaaaaaaaaaaaaarraaaaa"}, {"01000111100100010001011101000000"}},
	{OFFSET_EMMC50_CFG1,           {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"00110000111000000000000010000000"}},
	{OFFSET_EMMC50_CFG2,           {"xaaaaaaaxxxxaaaaaaaaaaaaaaaarrrx"}, {"x0000000xxxx0000001100001111000x"}},
	{OFFSET_EMMC50_CFG3,           {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaarrr"}, {"10000011110001010001010011110000"}},
	{OFFSET_EMMC50_CFG4,           {"aaaaaaaaaaaaaaaaaaaaaaarrrrrrrrr"}, {"01111000000101001000000000000000"}},
	{OFFSET_EMMC50_BLOCK_LENGTH,   {"aaaaaaaaaxxxxxxxxxxxxxxxxxxxxxxx"}, {"00000001000000000000000000000000"}},

};

static reg_desc_t msdc1_reg_desc[] = {
	//{"01234567012345670123456701234567"}, {"01234567012345670123456701234567"}
	{OFFSET_MSDC_CFG      ,        {"aazaaattaaaaaaaaaaaaccxxxaxxxxxx"}, {"1001100100000000000000xxx1xxxxxx"}}, //bit[2] is rw, but not test. cause this is a reset bit. write to "1" will back to "0" when reset sequence done
	{OFFSET_MSDC_IOCON    ,        {"aaaaaaxxaaaaaaxxaaaaaaaaxxxxxxxx"}, {"000000xx000000xx0000000000xxxxxx"}},//bit[24][25] can not write(Not in MT8320 MSDC IP)
	{OFFSET_MSDC_PS       ,        {"atxxxxxxxxxxaaaattttxxxxtxxxxxxr"}, {"01xxxxxxxxxx00001111xxxx1xxxxxx0"}},//bit[1]/bit[16-20]/bit[24]/bit[31] default to 0
	{OFFSET_MSDC_INT      ,        {"ccxccccccccrccccccccccccxxxxxxxx"}, {"00x000000000000000000000xxxxxxxx"}},
	{OFFSET_MSDC_INTEN    ,        {"aaxaaaaaaaaaaaaaaaaaxxxxxxxxxxxx"}, {"00x00000000000000000xxxxxxxxxxxx"}},//bit[2] to RW, default to 0
	{OFFSET_MSDC_FIFOCS   ,        {"rrrrrrrrxxxxxxxxrrrrrrrrxxxxxxxz"}, {"00000000xxxxxxxx00000000xxxxxxx0"}},//bit[0]~[7] are variable if you use memory tool in Code visor (memory tool must be closed)
	{OFFSET_MSDC_TXDATA   ,        {"wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"}, {"00000000000000000000000000000000"}},
	//{OFFSET_MSDC_RXDATA   ,      {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},/* Should not be touched. */
	{OFFSET_SDC_CFG       ,        {"aaxxxxxxxxxxxxxxaaxaaaxxaaaaaaaa"}, {"00xxxxxxxxxxxxxx00x010xx00000000"}},
	{OFFSET_SDC_CMD       ,        {"xaaaaaaaaaxaaaaaaaaaaaaaaaaaaaax"}, {"x000000000x000000000000000000000"}},
	{OFFSET_SDC_ARG       ,        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_STS       ,        {"ttxxxxxxxxxxxxxxxxxxxxxxxxxxxxxt"}, {"00xxxxxxxxxxxxxxxxxxxxxxxxxxxxx0"}},
	{OFFSET_SDC_RESP0     ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_RESP1     ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_RESP2     ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_RESP3     ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_BLK_NUM   ,        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"10000000000000000000000000000000"}},
	{OFFSET_SDC_VOL_CHG   ,        {"aaaaaaaaaaaaaaaaxxxxxxxxxxxxxxxx"}, {"1010001010000000xxxxxxxxxxxxxxxx"}},
	{OFFSET_SDC_CSTS      ,        {"cccccccccccccccccccccccccccccccc"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_CSTS_EN   ,        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_DCRC_STS  ,        {"rrrrrrrrrrrrrrrrxxxxxxxxxxxxxxxx"}, {"0000000000000000xxxxxxxxxxxxxxxx"}},//bit[12-15] modify

	{OFFSET_SDC_ACMD_RESP ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_ACMD19_TRG,        {"aaaaxxxxxxxxxxxxxxxxxxxxxxxxxxxx"}, {"0000xxxxxxxxxxxxxxxxxxxxxxxxxxxx"}},
	{OFFSET_SDC_ACMD19_STS,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_MSDC_DMA_SA_HIGH,      {"aaaaxxxxxxxxxxxxxxxxxxxxxxxxxxxx"}, {"0000xxxxxxxxxxxxxxxxxxxxxxxxxxxx"}},
	{OFFSET_MSDC_DMA_SA   ,        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"00000000000000000000000000000000"}},
	{OFFSET_MSDC_DMA_CA   ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_MSDC_DMA_CTRL ,        {"wrwxxxxxaxaxaaaxxxxxxxxxxxxxxxxx"}, {"000xxxxx0x0x011xxxxxxxxxxxxxxxxx"}}, //need new design, bit[1] = AO
	{OFFSET_MSDC_DMA_CFG  ,        {"raxxxxxxaaxxaaxxaxxxxxxxxxxxxxxx"}, {"00xxxxxx00xx00xx0xxxxxxxxxxxxxxx"}},
	{OFFSET_MSDC_DBG_SEL  ,        {"aaaaaaaaaaaaaaaaxxxxxxxxxxxxxxxx"}, {"0000000000000000xxxxxxxxxxxxxxxx"}},
	{OFFSET_MSDC_DBG_OUT  ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_MSDC_DMA_LEN  ,        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"00000000000000000000000000000000"}}, //new register in MT6582
	{OFFSET_MSDC_PATCH_BIT0,       {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"11100000000000000011110000000010"}},
	{OFFSET_MSDC_PATCH_BIT1,       {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"10010000000000001111111111111111"}},
	{OFFSET_MSDC_PATCH_BIT2,       {"aaaaaaaaaaaaxaaaaaaaaxaaaaaaaaaa"}, {"00010100100000000001100000000011"}},
	{OFFSET_MSDC_PAD_TUNE0,        {"aaaaaxxxaaaaaxxxaaaaaxaaaaaaaaaa"}, {"00000xxx00000xxx00000x0000000000"}},
	{OFFSET_MSDC_PAD_TUNE1,        {"xxxxxxxxaaaaaaxxaaaaaaxxxxxxxxxx"}, {"xxxxxxxx000000xx000000xxxxxxxxxx"}},
	{OFFSET_MSDC_DAT_RDDLY0,       {"aaaaaxxxaaaaaxxxaaaaaxxxaaaaaxxx"}, {"00000xxx00000xxx00000xxx00000xxx"}},
	{OFFSET_MSDC_DAT_RDDLY1,       {"aaaaaxxxaaaaaxxxaaaaaxxxaaaaaxxx"}, {"00000xxx00000xxx00000xxx00000xxx"}},
	{OFFSET_MSDC_DAT_RDDLY2,       {"aaaaaxxxaaaaaxxxaaaaaxxxaaaaaxxx"}, {"00000xxx00000xxx00000xxx00000xxx"}},
	{OFFSET_MSDC_DAT_RDDLY3,       {"aaaaaxxxaaaaaxxxaaaaaxxxaaaaaxxx"}, {"00000xxx00000xxx00000xxx00000xxx"}},
	{OFFSET_MSDC_HW_DBG   ,        {"aaaaaaaaaaaaaaxxaaaaaaaaaaaaaaax"}, {"00000000000000xx000000000000000x"}},
	{OFFSET_MSDC_VERSION  ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"11000000010010001100100000000100"}},//20131203
	{OFFSET_MSDC_ECO_VER  ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},//eco value
};

static reg_desc_t msdc2_reg_desc[] = {

	{OFFSET_MSDC_CFG      ,        {"aazaaattaaaaaaaaaaaaccxxxaxxxxxx"}, {"1001100100000000000000xxx1xxxxxx"}}, //bit[2] is rw, but not test. cause this is a reset bit. write to "1" will back to "0" when reset sequence done
	{OFFSET_MSDC_IOCON    ,        {"aaaaaaxxaaaaaaxxaaaaaaaaxxxxxxxx"}, {"000000xx000000xx00000000xxxxxxxx"}},
	{OFFSET_MSDC_PS       ,        {"atxxxxxxxxxxaaaarrrrrrrrrxxxxxxt"}, {"01xxxxxxxxxx0000111111111xxxxxx1"}},//bit[1]/bit[16-20]/bit[24]/bit[31] default to 0
	{OFFSET_MSDC_INT      ,        {"ccxccccccccrcccccccccccxxxxxxxxx"}, {"00x00000000000000000000xxxxxxxxx"}},
	{OFFSET_MSDC_INTEN    ,        {"aaxaaaaaaaaaaaaaaaaaaaaxxxxxxxxx"}, {"00x00000000000000000000xxxxxxxxx"}},//bit[2] to ignore
	{OFFSET_MSDC_FIFOCS   ,        {"rrrrrrrrxxxxxxxxrrrrrrrrxxxxxxxz"}, {"00000000xxxxxxxx00000000xxxxxxx0"}},//bit[0]~[7] are variable if you use memory tool in Code visor (memory tool must be closed)
	{OFFSET_MSDC_TXDATA   ,        {"wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"}, {"00000000000000000000000000000000"}},
	//{OFFSET_MSDC_RXDATA   ,      {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},/* Should not be touched. */
	{OFFSET_SDC_CFG       ,        {"aaxxxxxxxxxxaaaaaaxaaaxxaaaaaaaa"}, {"00xxxxxxxxxx000100x010xx00000000"}},
	{OFFSET_SDC_CMD       ,        {"aaaaaaaaaaxaaaaaaaaaaaaaaaaaaaaa"}, {"0000000000x000000000000000000000"}},
	{OFFSET_SDC_ARG       ,        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_STS       ,        {"ttxxxxxxxxxxxxxxxxxxxxxxxxxxxxxt"}, {"00xxxxxxxxxxxxxxxxxxxxxxxxxxxxx0"}},
	{OFFSET_SDC_RESP0     ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_RESP1     ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_RESP2     ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_RESP3     ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},

	{OFFSET_SDC_BLK_NUM   ,        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"10000000000000000000000000000000"}},
	{OFFSET_SDC_CSTS      ,        {"cccccccccccccccccccccccccccccccc"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_CSTS_EN   ,        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_DCRC_STS  ,        {"rrrrrrrrrrrrrrrrxxxxxxxxxxxxxxxx"}, {"0000000000000000xxxxxxxxxxxxxxxx"}},//bit[12-15] modify

	{OFFSET_SDC_ACMD_RESP ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_ACMD19_TRG,        {"aaaaxxxxxxxxxxxxxxxxxxxxxxxxxxxx"}, {"0000xxxxxxxxxxxxxxxxxxxxxxxxxxxx"}},
	{OFFSET_SDC_ACMD19_STS,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_MSDC_DMA_SA_HIGH,      {"aaaaxxxxxxxxxxxxxxxxxxxxxxxxxxxx"}, {"0000xxxxxxxxxxxxxxxxxxxxxxxxxxxx"}},
	{OFFSET_MSDC_DMA_SA   ,        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"00000000000000000000000000000000"}},
	{OFFSET_MSDC_DMA_CA   ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_MSDC_DMA_CTRL ,        {"wrwrxxxxaaaaaaaxxxxxxxxxxxxxxxxx"}, {"0001xxxx0000011xxxxxxxxxxxxxxxxx"}}, //need new design, bit[1] = AO
	{OFFSET_MSDC_DMA_CFG  ,        {"raxxxxxxaaxxaaxxaaxxxxxxxxxxxxxx"}, {"00xxxxxx00xx00xx00xxxxxxxxxxxxxx"}},
	{OFFSET_MSDC_DBG_SEL  ,        {"aaaaaaaaaaaaaaaaxxxxxxxxxxxxxxxx"}, {"0000000000000000xxxxxxxxxxxxxxxx"}},
	{OFFSET_MSDC_DBG_OUT  ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_MSDC_DMA_LEN  ,        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"00000000000000000000000000000000"}}, //new register in MT6582
	{OFFSET_MSDC_PATCH_BIT0,       {"xaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"x1100000000000000011110000000010"}},
	{OFFSET_MSDC_PATCH_BIT1,       {"aaaaaaaaaaaaaaxaxxxxxxxaaaaaaaaa"}, {"10010000000000x0xxxxxxx111111111"}},
	{OFFSET_MSDC_PATCH_BIT2,       {"aaaaaaaaaaaaxaaaaaaaaxaaaaaaaaaa"}, {"00010100100000000001100000000011"}},

	{OFFSET_DAT0_TUNE_CRC,         {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_DAT1_TUNE_CRC,         {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_DAT2_TUNE_CRC,         {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_DAT3_TUNE_CRC,         {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_CMD_TUNE_CRC,          {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDIO_TUNE_WIND,        {"aaaaaxxxxxxxxxxxxxxxxxxxxxxxxxxx"}, {"00000xxxxxxxxxxxxxxxxxxxxxxxxxxx"}},

	{OFFSET_MSDC_PAD_TUNE0,        {"aaaaaxxxaaaaaxxxaaaaaxaaaaaaaaaa"}, {"00000xxx00000xxx00000x0000000000"}},
	{OFFSET_MSDC_PAD_TUNE1,        {"xxxxxxxxaaaaaaxxaaaaaaxxxxxxxxxx"}, {"xxxxxxxx000000xx000000xxxxxxxxxx"}},
	{OFFSET_MSDC_DAT_RDDLY0,       {"aaaaaxxxaaaaaxxxaaaaaxxxaaaaaxxx"}, {"00000xxx00000xxx00000xxx00000xxx"}},
	{OFFSET_MSDC_DAT_RDDLY1,       {"aaaaaxxxaaaaaxxxaaaaaxxxaaaaaxxx"}, {"00000xxx00000xxx00000xxx00000xxx"}},
	{OFFSET_MSDC_DAT_RDDLY2,       {"aaaaaxxxaaaaaxxxaaaaaxxxaaaaaxxx"}, {"00000xxx00000xxx00000xxx00000xxx"}},
	{OFFSET_MSDC_DAT_RDDLY3,       {"aaaaaxxxaaaaaxxxaaaaaxxxaaaaaxxx"}, {"00000xxx00000xxx00000xxx00000xxx"}},
	{OFFSET_MSDC_HW_DBG   ,        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"00000000000000000000000000000000"}},
	{OFFSET_MSDC_VERSION  ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"11000000010010001100100000000100"}},//20131203
	{OFFSET_MSDC_ECO_VER  ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},//eco value
};

static reg_desc_t msdc3_reg_desc[] = {

	{OFFSET_MSDC_CFG      ,        {"aazaaattaaaaaaaaaaaaccxxxaxxxxxx"}, {"1001100100000000000000xxx1xxxxxx"}}, //bit[2] is rw, but not test. cause this is a reset bit. write to "1" will back to "0" when reset sequence done
	{OFFSET_MSDC_IOCON    ,        {"aaaaaaxxaaaaaaxxaaaaaaaaxxxxxxxx"}, {"000000xx000000xx00000000xxxxxxxx"}},
	{OFFSET_MSDC_PS       ,        {"atxxxxxxxxxxaaaarrrrrrrrrxxxxxxt"}, {"01xxxxxxxxxx0000111111111xxxxxx1"}},//bit[1]/bit[16-20]/bit[24]/bit[31] default to 0
	{OFFSET_MSDC_INT      ,        {"ccxccccccccrcccccccccccxxxxxxxxx"}, {"00x00000000000000000000xxxxxxxxx"}},
	{OFFSET_MSDC_INTEN    ,        {"aaxaaaaaaaaaaaaaaaaaaaaxxxxxxxxx"}, {"00x00000000000000000000xxxxxxxxx"}},//bit[2] to ignore
	{OFFSET_MSDC_FIFOCS   ,        {"rrrrrrrrxxxxxxxxrrrrrrrrxxxxxxxz"}, {"00000000xxxxxxxx00000000xxxxxxx0"}},//bit[0]~[7] are variable if you use memory tool in Code visor (memory tool must be closed)
	{OFFSET_MSDC_TXDATA   ,        {"wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"}, {"00000000000000000000000000000000"}},
	//{OFFSET_MSDC_RXDATA   ,      {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},/* Should not be touched. */
	{OFFSET_SDC_CFG       ,        {"aaxxxxxxxxxxaaaaaaxaaaxxaaaaaaaa"}, {"00xxxxxxxxxx000100x010xx00000000"}},
	{OFFSET_SDC_CMD       ,        {"aaaaaaaaaaxaaaaaaaaaaaaaaaaaaaaa"}, {"0000000000x000000000000000000000"}},
	{OFFSET_SDC_ARG       ,        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_STS       ,        {"ttxxxxxxxxxxxxxx xxxxxxxxxxxxxxt"}, {"00xxxxxxxxxxxxxxxxxxxxxxxxxxxxx0"}},
	{OFFSET_SDC_RESP0     ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_RESP1     ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_RESP2     ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_RESP3     ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_BLK_NUM   ,        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"10000000000000000000000000000000"}},
	{OFFSET_SDC_CSTS      ,        {"cccccccccccccccccccccccccccccccc"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_CSTS_EN   ,        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_DCRC_STS  ,        {"rrrrrrrrrrrrrrrrxxxxxxxxxxxxxxxx"}, {"0000000000000000xxxxxxxxxxxxxxxx"}},//bit[12-15] modify

	{OFFSET_SDC_ACMD_RESP ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDC_ACMD19_TRG,        {"aaaaxxxxxxxxxxxxxxxxxxxxxxxxxxxx"}, {"0000xxxxxxxxxxxxxxxxxxxxxxxxxxxx"}},
	{OFFSET_SDC_ACMD19_STS,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_MSDC_DMA_SA_HIGH,      {"aaaaxxxxxxxxxxxxxxxxxxxxxxxxxxxx"}, {"0000xxxxxxxxxxxxxxxxxxxxxxxxxxxx"}},
	{OFFSET_MSDC_DMA_SA   ,        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"00000000000000000000000000000000"}},
	{OFFSET_MSDC_DMA_CA   ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_MSDC_DMA_CTRL ,        {"wrwrxxxxaaaaaaaxxxxxxxxxxxxxxxxx"}, {"0001xxxx0000011xxxxxxxxxxxxxxxxx"}}, //need new design, bit[1] = AO
	{OFFSET_MSDC_DMA_CFG  ,        {"raxxxxxxaaxxaaxxaaxxxxxxxxxxxxxx"}, {"00xxxxxx00xx00xx00xxxxxxxxxxxxxx"}},
	{OFFSET_MSDC_DBG_SEL  ,        {"aaaaaaaaaaaaaaaaxxxxxxxxxxxxxxxx"}, {"0000000000000000xxxxxxxxxxxxxxxx"}},
	{OFFSET_MSDC_DBG_OUT  ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_MSDC_DMA_LEN  ,        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"00000000000000000000000000000000"}}, //new register in MT6582
	{OFFSET_MSDC_PATCH_BIT0,       {"xaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"x1100000000000000011110000000010"}},
	{OFFSET_MSDC_PATCH_BIT1,       {"aaaaaaaaaaaaaaxaxxxxxxxaaaaaaaaa"}, {"10010000000000x0xxxxxxx111111111"}},
	{OFFSET_MSDC_PATCH_BIT2,       {"aaaaaaaaaaaaxaaaaaaaaxaaaaaaaaaa"}, {"00010100100000000001100000000011"}},

	{OFFSET_DAT0_TUNE_CRC,         {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_DAT1_TUNE_CRC,         {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_DAT2_TUNE_CRC,         {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_DAT3_TUNE_CRC,         {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_CMD_TUNE_CRC,          {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},
	{OFFSET_SDIO_TUNE_WIND,        {"aaaaaxxxxxxxxxxxxxxxxxxxxxxxxxxx"}, {"00000xxxxxxxxxxxxxxxxxxxxxxxxxxx"}},

	{OFFSET_MSDC_PAD_TUNE0,        {"aaaaaxxxaaaaaxxxaaaaaxaaaaaaaaaa"}, {"00000xxx00000xxx00000x0000000000"}},
	{OFFSET_MSDC_PAD_TUNE1,        {"xxxxxxxxaaaaaaxxaaaaaaxxxxxxxxxx"}, {"xxxxxxxx000000xx000000xxxxxxxxxx"}},
	{OFFSET_MSDC_DAT_RDDLY0,       {"aaaaaxxxaaaaaxxxaaaaaxxxaaaaaxxx"}, {"00000xxx00000xxx00000xxx00000xxx"}},
	{OFFSET_MSDC_DAT_RDDLY1,       {"aaaaaxxxaaaaaxxxaaaaaxxxaaaaaxxx"}, {"00000xxx00000xxx00000xxx00000xxx"}},
	{OFFSET_MSDC_DAT_RDDLY2,       {"aaaaaxxxaaaaaxxxaaaaaxxxaaaaaxxx"}, {"00000xxx00000xxx00000xxx00000xxx"}},
	{OFFSET_MSDC_DAT_RDDLY3,       {"aaaaaxxxaaaaaxxxaaaaaxxxaaaaaxxx"}, {"00000xxx00000xxx00000xxx00000xxx"}},
	{OFFSET_MSDC_HW_DBG   ,        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}, {"00000000000000000000000000000000"}},
	{OFFSET_MSDC_VERSION  ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"11000000010010001100100000000100"}},//20131203
	{OFFSET_MSDC_ECO_VER  ,        {"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"}, {"00000000000000000000000000000000"}},//eco value
};


int msdc_reg_test(struct mmc_host* host, int id, int clock_gate_test)
{
#if (MSDC_MAX_NUM==1)
	u32 baddr[] = {MSDC0_BASE};
#elif (MSDC_MAX_NUM==2)
	u32 baddr[] = {MSDC0_BASE, MSDC1_BASE};
#elif (MSDC_MAX_NUM==3)
	u32 baddr[] = {MSDC0_BASE, MSDC1_BASE, MSDC2_BASE};
#elif (MSDC_MAX_NUM==4)
	u32 baddr[] = {MSDC0_BASE, MSDC1_BASE, MSDC2_BASE, MSDC3_BASE};
#elif (MSDC_MAX_NUM==5)
	u32 baddr[] = {MSDC0_BASE, MSDC1_BASE, MSDC2_BASE, MSDC3_BASE, MSDC4_BASE};
#endif
	u32 base = baddr[id];
	u32 i, j, k, l_array_size;
	u32 val;
	u32 val_shall_be;
	u32 val_shall_be_cg;
	char v , v_r;
	int error = MMC_ERR_NONE;
	reg_desc_t* msdc_reg_desc;
	int cg_write_test=2;

	uint32 ttt;

	if (id == 0) {
		l_array_size = ARRAY_SIZE(msdc0_reg_desc);
		msdc_reg_desc = msdc0_reg_desc;
	} else if (id == 1) {
		l_array_size = ARRAY_SIZE(msdc1_reg_desc);
		msdc_reg_desc = msdc1_reg_desc;
	} else if (id == 2) {
		l_array_size = ARRAY_SIZE(msdc2_reg_desc);
		msdc_reg_desc = msdc2_reg_desc;
	} else if (id == 3) {
		l_array_size = ARRAY_SIZE(msdc3_reg_desc);
		msdc_reg_desc = msdc3_reg_desc;
	} else {
		msdc_printf("Invalid host index: %d\n", id);
		goto out;
	}

#if 0 //This test won't be performed if whole module reset is not supported
	/* [r/t/z/a/k/s/c] check register reset Value
	 * by pass the bit which can not read and the bit we don't cara about */
	msdc_printf("Reset default test start\n");

	if ( clock_gate_test ) msdc_clock(host, 0);

	for (i = 0; i < l_array_size; i++) {
		val = MSDC_READ32(base + msdc_reg_desc[i].offset);
		msdc_printf("Reg %x=%x\n",msdc_reg_desc[i].offset, val);
		for (j = 0; j < 32; j++) {
			if (('w' == (msdc_reg_desc[i].attr[j])) ||
			        ('x' == (msdc_reg_desc[i].attr[j])) ||
			        ('n' == (msdc_reg_desc[i].attr[j])))
				continue;

			v = (val >> j) & 0x1;
			if ( !clock_gate_test ) val_shall_be=msdc_reg_desc[i].reset[j] - '0';
			else val_shall_be=0;
			if (v != val_shall_be) {
				msdc_printf("[SD%d] Invalid Reset Value in 0x%x[%d]=%d != %d\n",
				            id, base + msdc_reg_desc[i].offset, j, v,
				            val_shall_be);
				error = __LINE__;
			}
		}
	}
	msdc_printf("Reset default test end\n\n");
#endif

	if ( clock_gate_test ) goto rw_test;
#if 1
	/* [r/t] check read only register: make sure the bit can not be write */
	msdc_printf("Read-only bits test start\n");
	for (i = 0; i < l_array_size; i++) {
		for (j = 0; j < 32; j++) {
			if (('r' != (msdc_reg_desc[i].attr[j])) && ('t' != (msdc_reg_desc[i].attr[j])) )
				continue;
			/* Special rule for SDC_STS:
			   When SDCC_STS.SDCBSY may becomes 1 occasionally after toggle other register bits,
			   Wait it comes 0 */
			if ((u32)SDC_STS == (base + msdc_reg_desc[i].offset)) {
				do {
					val_shall_be = MSDC_READ32(SDC_STS);
					//msdc_printf("SDC_STS = %x\n", v);
				} while ( val_shall_be & SDC_STS_SDCBUSY );
			}
			val_shall_be = MSDC_READ32(base + msdc_reg_desc[i].offset);

			v_r = (val_shall_be >> j) & 0x1;
			if (v_r == 0x0)
				MSDC_SET_BIT32(base + msdc_reg_desc[i].offset, 0x1 << j);
			else
				MSDC_CLR_BIT32(base + msdc_reg_desc[i].offset, 0x1 << j);

			v = (MSDC_READ32(base + msdc_reg_desc[i].offset) >> j) & 0x1;
			if (v != v_r) {
				msdc_printf("[SD%d] Read Only Reg Modified in 0x%x[%d]=%d != %d\n",
				            id, base + msdc_reg_desc[i].offset, j, v, v_r);
				error = __LINE__;
			}
		}
	}
	msdc_printf("Read-only bits test end\n\n");
#endif

rw_test:
#if 1
	/* [a] check write register: set or clear the bit, make sure anther bit will not affected */
	msdc_printf("RW bits test start\n");
	for (i = 0; i < l_array_size; i++) {
		for (j = 0; j < 32; j++) {
			//msdc_printf("j %d\n", j);
			if ('a' != (msdc_reg_desc[i].attr[j]))
				continue;

			if ( clock_gate_test && cg_write_test==0 ) goto write_0_test;

write_1_test:
			val_shall_be = MSDC_READ32(base + msdc_reg_desc[i].offset);
			if (clock_gate_test) {
				if (((val_shall_be >> j) & 0x1) == 1)
					goto write_0_test;
				msdc_clock(host, 0);
				val_shall_be_cg = MSDC_READ32(0x11250000);
				val_shall_be_cg = MSDC_READ32(base + msdc_reg_desc[i].offset);
			}
			//msdc_printf("1 val_shall_be %x, cg %x\n", val_shall_be, val_shall_be_cg);

			MSDC_SET_BIT32(base + msdc_reg_desc[i].offset, 0x1 << j);
			v = (MSDC_READ32(base + msdc_reg_desc[i].offset) >> j) & 0x1;

			if ( (v != 1) && (!clock_gate_test) ) {
				msdc_printf("[SD%d] Write 1 Failed in Reg 0x%x[%d]=%d != %d\n",
				            id, base + msdc_reg_desc[i].offset, j, v,
				            msdc_reg_desc[i].reset[j] - '0');
				error = __LINE__;
			}

			if ( clock_gate_test ) {
				//Value shall not change before clock is un-gated
				v = (MSDC_READ32(base + msdc_reg_desc[i].offset) >> j) & 0x1;
				if ( v !=((val_shall_be_cg >> j) & 0x1)) {
					msdc_printf("[SD%d] CG Write 1 wrongly effected in Reg 0x%x[%d]=%d != %d, before clock ungate\n",
					            id, base + msdc_reg_desc[i].offset, j, v,
					            (val_shall_be_cg >> j) & 0x1);
					error = __LINE__;
				}

				msdc_clock(host, 1);
				v = (MSDC_READ32(base + msdc_reg_desc[i].offset) >> j) & 0x1;
				//Value shall not change after clock is un-gated
				if ( v !=((val_shall_be >> j) & 0x1)) {
					msdc_printf("[SD%d] CG Write 1 wrongly effected in Reg 0x%x[%d]=%d != %d, after clock ungate\n",
					            id, base + msdc_reg_desc[i].offset, j, v,
					            (val_shall_be >> j) & 0x1);
					error = __LINE__;
				}
			}

			if ( !clock_gate_test ) {
				/* exception rule for clock stable */
				if (((u32)MSDC_CFG == (base + msdc_reg_desc[i].offset)) &&
				        (j >= 8 && j < 20)) { /* wait clock stable */
					while (!(MSDC_READ32(MSDC_CFG) & MSDC_CFG_CKSTB));
				}
			}

			/* check other bits are not affected by write 1 */
			if ( !clock_gate_test ) {
				for (k = 0; k < 32; k++) {
					if (k == j)
						continue;
					if (('w' == (msdc_reg_desc[i].attr[k])) ||
					        ('x' == (msdc_reg_desc[i].attr[k])) ||
					        ('t' == (msdc_reg_desc[i].attr[k])))
						continue;
					v = (MSDC_READ32(base + msdc_reg_desc[i].offset) >> k) & 0x1;
					if (v != ((val_shall_be>>k) & 0x1)) {
						msdc_printf("[SD%d] Affected by Write 1 to 0x%x[%d] and [%d]=%d != %d\n",
						            id, base + msdc_reg_desc[i].offset, j, k, v,
						            ((val_shall_be>>k) & 0x1));
						error = __LINE__;
					}
				}
			}

			if ( clock_gate_test &&  cg_write_test==1 ) goto restore_value;

write_0_test:
			/* write 0 to target bit */
			val_shall_be = MSDC_READ32(base + msdc_reg_desc[i].offset);
			if (clock_gate_test) {
				if (((val_shall_be >> j) & 0x1) == 0)
					goto restore_value;
				msdc_clock(host, 0);
				val_shall_be_cg = MSDC_READ32(baddr[id] + msdc_reg_desc[i].offset);
			}
			//msdc_printf("0 val_shall_be %x, cg %x\n", val_shall_be, val_shall_be_cg);

			MSDC_CLR_BIT32(base + msdc_reg_desc[i].offset, 0x1 << j);
			v = (MSDC_READ32(base + msdc_reg_desc[i].offset) >> j) & 0x1;

			if ( (v != 0) && ( !clock_gate_test ) ) {
				msdc_printf("[SD%d] Write 0 Reg Failed in 0x%x[%d]=%d != %d\n",
				            id, base + msdc_reg_desc[i].offset, j, v,
				            msdc_reg_desc[i].reset[j] - '0');
				error = __LINE__;
			}

			if ( clock_gate_test ) {
				//Value shall not change before clock is un-gated
				v = (MSDC_READ32(base + msdc_reg_desc[i].offset) >> j) & 0x1;
				if (v !=((val_shall_be_cg >> j) & 0x1)) {
					msdc_printf("[SD%d] CG Write 0 wrongly effected in Reg 0x%x[%d]=%d != %d, before clock ungate\n",
					            id, base + msdc_reg_desc[i].offset, j, v,
					            (val_shall_be_cg >> j) & 0x1);
					error = __LINE__;
				}

				//Value shall not change after clock is un-gated
				msdc_clock(host, 1);
				v = (MSDC_READ32(base + msdc_reg_desc[i].offset) >> j) & 0x1;
				if (v !=((val_shall_be >> j) & 0x1)) {
					msdc_printf("[SD%d] CG Write 0 wrongly effected in Reg 0x%x[%d]=%d != %d, after clock ungate\n",
					            id, base + msdc_reg_desc[i].offset, j, v,
					            (val_shall_be >> j) & 0x1);
					error = __LINE__;
				}
			}

			if ( !clock_gate_test ) {
				/* exception rule for clock stable */
				if (((u32)MSDC_CFG == (base + msdc_reg_desc[i].offset)) &&
				        (j >= 8 && j < 20)) { /* wait clock stable */
					while (!(MSDC_READ32(MSDC_CFG) & MSDC_CFG_CKSTB));
				}
			}

			/* check other bits are not affected by write 0 */
			if ( !clock_gate_test ) {
				for (k = 0; k < 32; k++) {
					if (k == j)
						continue;

					if (('w' == (msdc_reg_desc[i].attr[k])) ||
					        ('x' == (msdc_reg_desc[i].attr[k])) ||
					        ('t' == (msdc_reg_desc[i].attr[k])))
						continue;
					v = (MSDC_READ32(base + msdc_reg_desc[i].offset) >> k) & 0x1;
					if (v != ((val_shall_be>>k) & 0x1)) {
						msdc_printf("[SD%d] Affected by Write 0 to 0x%x[%d] and [%d]=%d != %d\n",
						            id, base + msdc_reg_desc[i].offset, j, k, v,
						            ((val_shall_be>>k) & 0x1));
						error = __LINE__;
					}
				}
			}

restore_value:
			/* restore default value */
			if ((msdc_reg_desc[i].reset[j] - '0') == 1) {
				MSDC_SET_BIT32(base + msdc_reg_desc[i].offset, 0x1 << j);
			} else {
				MSDC_CLR_BIT32(base + msdc_reg_desc[i].offset, 0x1 << j);
			}
		}
	}
	if ( clock_gate_test && cg_write_test==1 ) {
		cg_write_test=0;
		goto rw_test;
	}
	msdc_printf("RW bits test end\n\n");
#endif

	if ( clock_gate_test ) goto out;
#if 1
	/* [z] check AO register, write 1, wait 100ms, check the result */
	msdc_printf("AO bits test start\n");
	for (i = 0; i < l_array_size; i++) {
		for (j = 0; j < 32; j++) {
			if ('z' != (msdc_reg_desc[i].attr[j]))
				continue;

			MSDC_SET_BIT32(base + msdc_reg_desc[i].offset, 0x1 << j);

			mdelay(100);

			v =(MSDC_READ32(base + msdc_reg_desc[i].offset) >> j) & 0x1;
			if (v != 0) {
				msdc_printf("[SD%d] write 1, wait change to zero failed. 0x%x[%d]=%d after 100ms when write '1' to this bit\n",
				            id, base + msdc_reg_desc[i].offset, j, v);
				error = __LINE__;
			}
		}
	}
	msdc_printf("AO bits test end\n\n");
#endif

out:

	return error;
}

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
	int clksrc = (id == 0) ? MSDC50_CLKSRC_26MHZ: MSDC30_CLKSRC_26MHZ;

	host = mmc_get_host(id);
	mmc_init_host(host, id, cfg->clksrc, MSDC_MODE_PIO)

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
	unsigned char tests[3];
	char *tests_str[] = {"single", "multiple", "interleave"};
	unsigned int tests_idx;
	unsigned int inited = 0;

	id    = cfg->id;
	count = cfg->count;
	buf   = cfg->buf;
	blknr = cfg->blknr;
	blksz = cfg->blksz;

	chk_result = cfg->chk_result;
	chunk_blks = cfg->chunk_blks;
	total_blks = (cfg->total_size + blksz - 1) / blksz;
	forever    = (count == -1) ? 1 : 0;

	tests[0] = cfg->tst_single;
	tests[1] = cfg->tst_multiple;
	tests[2] = cfg->tst_interleave;

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
			msdc_printf("[TST] BlkAddr : %xh\n", (unsigned int)blknr);
			msdc_printf("[TST] BlkSize : %dbytes\n", blksz);
			msdc_printf("[TST] TstBlks : %d\n", total_blks);
			msdc_printf("[TST] AutoCMD : 12(%d), 23(%d)\n",
			            (cfg->autocmd & MSDC_AUTOCMD12) ? 1 : 0,
			            (cfg->autocmd & MSDC_AUTOCMD23) ? 1 : 0);
			msdc_printf("[TST] CheckResult : %d\n", cfg->chk_result);
			msdc_printf("[TST] ----------------------------------------------\n");

			if ((cfg->skip_reinit==0) || (inited==0)) {
				if (mmc_init_host(host, id, cfg->clksrc, cfg->mode) != 0) {
					result = -__LINE__;
					goto failure;
				}
				if (mmc_init_card(host, card) != 0) {
					result = -__LINE__;
					goto failure;
				}
				inited = 1;

				msdc_apply_timing(id);
				msdc_set_dma(host, (u8)cfg->burstsz, (u32)cfg->flags);
				msdc_set_autocmd(host, cfg->autocmd, 1);

				/* Since SLT use default setting after initialization,
				   skip redundant setting to reduce log for reducing test time */
#if (MSDC_SLT == 0)
				/* change clock */
				//msdc_printf("card->maxhz(%d), cfg->clock(%d)\n",card->maxhz,cfg->clock);
				if (cfg->clock) {
					clkhz = card->maxhz < cfg->clock ? card->maxhz : cfg->clock;
					mmc_set_clock(host, mmc_card_ddr(card), clkhz);
				}
				//msdc_printf("host->src_clk(%d)\n",host->src_clk);

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
#endif

				tid = result = 0;

				if (cfg->piobits) {
					msdc_printf("[TST] PIO bits: %d\n", cfg->piobits);
					msdc_set_pio_bits(host, cfg->piobits);
				}

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

				if (cfg->dcm_enable) {
					msdc_enable_dcm(host, 1, cfg->dcm_div1, cfg->dcm_div2);
				} else {
					msdc_enable_dcm(host, 0, 0, 0);
				}

				msdc_set_signal_burst(host, cfg->single_bst);
			}

			tid = result = 0;
#if 0 //UT only, change to "if 0" temporarily
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
			msdc_printf(TC_MSG "test block erase\n",
			            host->id, result == 0 ? "PASS" : "FAIL", tid++);
#endif
			mmc_send_status(host, card, &status);

			tests_idx = 0;

			while (tests_idx<2) {
				if (!tests[tests_idx]) {
					tests_idx++;
					continue;
				}

				if (tests_idx == 0)
					chunk_blks = 1;
				else
					chunk_blks = cfg->chunk_blks;

				chunks = total_blks / chunk_blks;
				left_blks = total_blks % chunk_blks;
				msdc_printf("total_blks(%d),chunks(%d),left_blks(%d)\n", total_blks, chunks, left_blks);
#if 1
				for (i = 0; i < chunks; i++) {
					if (tests_idx == 0)
						pattern = (i + count +1) % 256;
					else
						pattern = (i + count) % 256;
					memset(buf, pattern, blksz * chunk_blks);
#if 0 // dump the write buf
					msdc_printf("the %d block data in write buf\n", i);
					for (j = 0; j < blksz * chunk_blks; j++) {
						msdc_printf("0x%x ", buf[j]);
						if ((j+1)%8 == 0)
							msdc_printf("\n");
					}
#endif
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

				msdc_printf(TC_MSG "test %s block write\n",
				            host->id, result == 0 ? "PASS" : "FAIL", tid++, tests_str[tests_idx]);

				if (result)
					goto failure;
#endif

#if 1
				/* block read */
				for (i = 0; i < chunks; i++) {
					if (tests_idx == 0)
						pattern = (i + count +1) % 256;
					else
						pattern = (i + count) % 256;
					memset(buf, pattern^0xFF, blksz * chunk_blks);
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
							if (buf[j] != pattern) {
								msdc_printf("[SD%d]\t\t%lxh = %x (!= %x)\n",
								            host->id, blknr + i * chunk_blks, buf[j], pattern);
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
				}

				if (!result && left_blks) {
					if (tests_idx == 0)
						pattern = (i + count +1) % 256;
					else
						pattern = (i + count) % 256;
					memset(buf, pattern^0xFF, blksz * left_blks);
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
							if (buf[j] != pattern) {
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
				}

				msdc_printf(TC_MSG "test %s block read\n",
				            host->id, result == 0 ? "PASS" : "FAIL", tid++,  tests_str[tests_idx]);

				if (result)
					goto failure;
#endif

				mmc_send_status(host, card, &status);
				tests_idx++;
			}

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
					memset(buf, pattern^0xFF, blksz * chunk_blks);
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
					memset(buf, pattern^0xFF, blksz * left_blks);
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

				msdc_printf(TC_MSG "test multiple block interleave write-read\n",
				            host->id, result == 0 ? "PASS" : "FAIL", tid++);

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
static void emmc_r_w_compare_test()
{
	unsigned int i;

	/* write 0x40000000(1G) with 256 block(0x5a) */
	msdc_printf("[PL][%s:%d]eMMC simp test of user parittion start\n", __func__, __LINE__);
	for (i = 0; i < PL_MMC_TEST_SIZE; i++) {
		g_mmc_buf[i] = 0x5a;
	}

	/* 0 is for emmc */
	mmc_block_write(0, 0x40000000/512, PL_MMC_TEST_SIZE/512, g_mmc_buf);
	msdc_printf("[PL][%s:%d] finish write user partition\n", __func__, __LINE__);

	/* read */
	for (i = 0; i < PL_MMC_TEST_SIZE; i++) {
		g_mmc_buf[i] = 0x0;
	}
	mmc_block_read(0, 0x40000000/512, PL_MMC_TEST_SIZE/512, g_mmc_buf);
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
	mmc_bread_boot(NULL, 0x0/512, PL_MMC_TEST_SIZE/512, g_mmc_buf);
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
		msdc_printf("blkcnt is invalid\n");
		return;
	}
	msdc_printf("[LK][%s:%d]eMMC WP_FEATURE_TEST start\n", __func__, __LINE__);
	for (i = 0; i < 512 * blkcnt; i++) {
		g_mmc_buf[i] = 0x5a;
	}
	/* 0 is for emmc */
	//err = mmc_block_write(0, blknr, blkcnt, (unsigned long *)g_mmc_buf, part_id);
	err = mmc_part_write(card, part_id, blknr, blkcnt, (unsigned long *)g_mmc_buf);
	if (err == MMC_ERR_WP_VIOLATION) {
		msdc_printf("[LK][%s:%d] MMC_ERR_WP_VIOLATION\n", __func__, __LINE__);
		return;
	}
	if (err) {
		msdc_printf("[LK][%s:%d] other write error(%d)\n", __func__, __LINE__, err);
		return;
	}
	msdc_printf("[LK][%s:%d] finish write\n", __func__, __LINE__);
	/* read */
	for (i = 0; i < 512 * blkcnt; i++) {
		g_mmc_buf[i] = 0x0;
	}
	//mmc_block_read(0, blknr, blkcnt, (unsigned long *)g_mmc_buf, part_id);
	mmc_part_read(card, part_id, blknr, blkcnt, (unsigned long *)g_mmc_buf);
	msdc_printf("[LK][%s:%d] finish read\n", __func__, __LINE__);
	/* compara */
	for (i = 0; i < 512 * blkcnt; i++) {
		if (g_mmc_buf[i] != 0x5a) {
			msdc_printf("[LK][%s:%d]mmc WP_FEATURE_TEST r/w compare failed(%d is %d, not 0x5a)\n", __func__, __LINE__, i, g_mmc_buf[i]);
			break;
		}
	}
	msdc_printf("[LK][%s:%d]eMMC simp test end\n", __func__, __LINE__);
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
	    msdc_printf("[case %d] blknr=%ld, blkcnt=%x,partition=%d \n",
	                case_id,blknr,blkcnt,part_id);
	    err = mmc_set_write_protect(dev_num, part_id, blknr, blkcnt, WP_POWER_ON);
	    if(err)
	        msdc_printf("[%s]: (case %d) wp error\n", __func__,case_id++);
	    else
	        msdc_printf("[%s]: (case %d) wp successfully\n", __func__,case_id++);
	*/

	//case2
	blknr = 0;
	blkcnt = card->wp_size;
	part_id = EMMC_PART_USER;
	msdc_printf("[case %d] blknr=0x%x, blkcnt=0x%x,partition=%d \n",
	            case_id,blknr,blkcnt,part_id);
	err = mmc_set_write_protect(dev_num, part_id, blknr, blkcnt, WP_POWER_ON);
	if (err)
		msdc_printf("[%s]: (case %d) wp error\n", __func__,case_id++);
	else
		msdc_printf("[%s]: (case %d) wp successfully\n", __func__,case_id++);


	//case3
	blknr = card->wp_size;
	blkcnt = 10 * card->wp_size;
	part_id = EMMC_PART_USER;
	msdc_printf("[case %d] blknr=0x%x, blkcnt=0x%x,partition=%d \n",
	            case_id,blknr,blkcnt,part_id);
	u64 time_before= gpt4_get_current_tick();//%lld
	err = mmc_set_write_protect(dev_num, part_id, blknr, blkcnt, WP_POWER_ON);
	u64 time_after= gpt4_get_current_tick();//%lld
	if (err)
		msdc_printf("[%s]: (case %d) wp error,[time cost of 10 wp grps]: %lld \n",
		            __func__,case_id++,(time_after - time_before)*76);
	else
		msdc_printf("[%s]: (case %d) wp successfully\n", __func__,case_id++);


	//case4
	blknr = 1;
	blkcnt = card->wp_size;
	part_id = EMMC_PART_USER;
	msdc_printf("[case %d] blknr=0x%x, blkcnt=0x%x,partition=%d \n",
	            case_id,blknr,blkcnt,part_id);
	err = mmc_set_write_protect(dev_num, part_id, blknr, blkcnt, WP_POWER_ON);
	if (err)
		msdc_printf("[%s]: (case %d) wp error\n", __func__,case_id++);
	else
		msdc_printf("[%s]: (case %d) wp successfully\n", __func__,case_id++);


	//case5
	blknr = card->wp_size;
	blkcnt = 1;
	part_id = EMMC_PART_USER;
	msdc_printf("[case %d] blknr=0x%x, blkcnt=0x%x,partition=%d \n",
	            case_id,blknr,blkcnt,part_id);
	err = mmc_set_write_protect(dev_num, part_id, blknr, blkcnt, WP_POWER_ON);
	if (err)
		msdc_printf("[%s]: (case %d) wp error\n", __func__,case_id++);
	else
		msdc_printf("[%s]: (case %d) wp successfully\n", __func__,case_id++);


	//case6
	blknr = card->wp_size;
	blkcnt = card->nblks;
	part_id = EMMC_PART_USER;
	msdc_printf("[case %d] blknr=0x%x, blkcnt=0x%x,partition=%d \n",
	            case_id,blknr,blkcnt,part_id);
	err = mmc_set_write_protect(dev_num, part_id, blknr, blkcnt, WP_POWER_ON);
	if (err)
		msdc_printf("[%s]: (case %d) wp error\n", __func__,case_id++);
	else
		msdc_printf("[%s]: (case %d) wp successfully\n", __func__,case_id++);


	//case7
	blknr = card->wp_size;
	blkcnt = card->wp_size;;
	part_id = EMMC_PART_END;
	msdc_printf("[case %d] blknr=0x%x, blkcnt=0x%x,partition=%d \n",
	            case_id,blknr,blkcnt,part_id);
	err = mmc_set_write_protect(dev_num, part_id, blknr, blkcnt, WP_POWER_ON);
	if (err)
		msdc_printf("[%s]: (case %d) wp error\n", __func__,case_id++);
	else
		msdc_printf("[%s]: (case %d) wp successfully\n", __func__,case_id++);


	//case8
	blknr = 11 * card->wp_size;
	blkcnt = card->wp_size;
	part_id = EMMC_PART_USER;
	msdc_printf("[case %d] blknr=0x%x, blkcnt=0x%x,partition=%d \n",
	            case_id,blknr,blkcnt,part_id);
	err = mmc_set_write_protect(dev_num, part_id, blknr, blkcnt, WP_PERMANENT);
	if (err)
		msdc_printf("[%s]: (case %d) wp error\n", __func__,case_id++);
	else
		msdc_printf("[%s]: (case %d) wp successfully\n", __func__,case_id++);


	//case9
	blknr = 0;
	blkcnt = card->wp_size;
	part_id = EMMC_PART_GP1;
	msdc_printf("[case %d] blknr=0x%x, blkcnt=0x%x,partition=%d \n",
	            case_id,blknr,blkcnt,part_id);
	err = mmc_set_write_protect(dev_num, part_id, blknr, blkcnt, WP_POWER_ON);
	if (err)
		msdc_printf("[%s]: (case %d) wp error\n", __func__,case_id++);
	else
		msdc_printf("[%s]: (case %d) wp successfully\n", __func__,case_id++);


	//case10
	blknr = 0;
	blkcnt = card->wp_size;
	part_id = EMMC_PART_USER;
	msdc_printf("[case %d] blknr=0x%x, blkcnt=0x%x,partition=%d \n",
	            case_id,blknr,blkcnt,part_id);
	err = mmc_set_write_protect(dev_num, part_id, blknr, blkcnt, WP_TEMPORARY);
	if (err)
		msdc_printf("[%s]: (case %d) wp error\n", __func__,case_id++);
	else
		msdc_printf("[%s]: (case %d) wp successfully\n", __func__,case_id++);

#endif
	// partiton test case
	msdc_printf("[%s]: (partition case: 1) preloader->lk\n", __func__);
	err = partition_write_prot_set("preloader", "lk", WP_POWER_ON);
	msdc_printf("[%s]: (partition case: 1) preloader->lk, err=%d\n", __func__, err);

	msdc_printf("[%s]: (partition case: 2) boot->recovery\n", __func__);
	err = partition_write_prot_set("boot", "recovery", WP_POWER_ON);
	msdc_printf("[%s]: (partition case: 2) boot->recovery, err=%d\n", __func__, err);

	msdc_printf("[%s]: (partition case: 3) keystore->oemkeystore\n", __func__);
	err = partition_write_prot_set("keystore", "oemkeystore", WP_POWER_ON);
	msdc_printf("[%s]: (partition case: 3) keystore->oemkeystore, err=%d\n", __func__, err);

	msdc_printf("[%s]: (partition case: 4) oemkeystore->secro\n", __func__);
	err = partition_write_prot_set("oemkeystore", "secro", WP_POWER_ON);
	msdc_printf("[%s]: (partition case: 4) oemkeystore->secro, err=%d\n", __func__, err);

	msdc_printf("[%s]: (partition case: 5) test_start->keystore\n", __func__);
	err = partition_write_prot_set("test_starte", "keystor", WP_POWER_ON);
	msdc_printf("[%s]: (partition case: 5) test_start->keystore, err=%d\n", __func__, err);

	msdc_printf("[%s]: (partition case: 6) oemkeystore->test_end\n", __func__);
	err = partition_write_prot_set("oemkeystore", "test_end", WP_POWER_ON);
	msdc_printf("[%s]: (partition case: 6) oemkeystore->test_end, err=%d\n", __func__, err);

	msdc_printf("[%s]: (partition case: 7) keystore->keystore\n", __func__);
	err = partition_write_prot_set("keystore", "keystore", WP_POWER_ON);
	msdc_printf("[%s]: (partition case: 7) keystore->keystore, err=%d\n", __func__, err);

	return err;
}
#endif

#endif /* MMC_TEST */

