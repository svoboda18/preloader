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

#include "msdc_cfg.h" //Include msdc_cfg.h for defining MMC_MSDC_DRV_PRELOADER
#include "msdc.h"

#if defined(MMC_MSDC_DRV_LK)
#include "block_generic_interface.h"
#include "mt_gpt.h"
#endif

#if !defined(MMC_MSDC_DRV_CTP)
#define MMC_HOST_ID                 0
#define BUF_BLK_NUM                 4   /* 4 * 512bytes = 2KB */

/**************************************************************************
*  DEBUG CONTROL
**************************************************************************/

/**************************************************************************
*  MACRO DEFINITION
**************************************************************************/

/**************************************************************************
*  EXTERNAL DECLARATION
**************************************************************************/
static int mmc_do_erase(int dev_num, u64 start_addr, u64 len, u32 part_id);

#if defined(MMC_MSDC_DRV_PRELOADER)
extern struct nand_chip g_nand_chip;
static blkdev_t g_mmc_bdev;
#endif

#if defined(MMC_MSDC_DRV_LK)
u64 g_emmc_size = 0;
#endif
u64 g_emmc_user_size = 0;

int mmc_switch_part(u32 part_id)
{
	int err = MMC_ERR_NONE;
	struct mmc_card *card;
	struct mmc_host *host;
	u8 part = (u8)part_id;
	u8 cfg;
	u8 *ext_csd;

	card = mmc_get_card(MMC_HOST_ID);
	host = mmc_get_host(MMC_HOST_ID);

	if (!card)
		return MMC_ERR_INVALID;

	ext_csd = &card->raw_ext_csd[0];

	if (mmc_card_mmc(card) && ext_csd[EXT_CSD_REV] >= 3) {
#ifdef MTK_EMMC_SUPPORT
		if (part_id == EMMC_PART_USER)
			part = EXT_CSD_PART_CFG_DEFT_PART;
		else if (part_id == EMMC_PART_BOOT1)
			part = EXT_CSD_PART_CFG_BOOT_PART_1;
		else if (part_id == EMMC_PART_BOOT2)
			part = EXT_CSD_PART_CFG_BOOT_PART_2;

		cfg = card->raw_ext_csd[EXT_CSD_PART_CFG];

		/* already set to specific partition */
		if (part == (cfg & 0x7))
			return MMC_ERR_NONE;

		cfg = (cfg & ~0x7) | part;

		err = mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_PART_CFG, cfg);

		if (err == MMC_ERR_NONE) {
			err = mmc_read_ext_csd(host, card);
			if (err == MMC_ERR_NONE) {
				ext_csd = &card->raw_ext_csd[0];
				if (ext_csd[EXT_CSD_PART_CFG] != cfg)
					err = MMC_ERR_FAILED;
			}
		}
#endif
	}

	if (err)
		msdc_pr_err("[%s]: failed to switch part_id:%d, part:%d, err=%d\n", __func__, part_id, part, err);

	return err;
}

static u64 mmc_get_emmc_size(struct mmc_card *card)
{
	u64 size_tmp = 0ULL;

	size_tmp += card->ext_csd.boot_part_sz * 2;
	size_tmp += card->ext_csd.rpmb_sz;
	size_tmp += 512ULL * card->wp_size *
			(card->ext_csd.gp_sz_mult[0] + card->ext_csd.gp_sz_mult[1] + card->ext_csd.gp_sz_mult[2] + card->ext_csd.gp_sz_mult[3]);
	size_tmp += (u64)card->blklen * card->nblks;

	return size_tmp;
}

#if defined(FEATURE_MMC_BOOT_ACCESS)
int mmc_get_boot_part(u32 *bootpart)
{
	struct mmc_card *card;
	struct mmc_host *host;
	int err = MMC_ERR_NONE;
	u8 *ext_csd;
	u8 part_config;

	if (!bootpart)
		return -1;

	card = mmc_get_card(MMC_HOST_ID);
	host = mmc_get_host(MMC_HOST_ID);

	if (!card || !host)
		return -1;
	err = mmc_read_ext_csd(host, card);

	if (err == MMC_ERR_NONE) {
		ext_csd = &card->raw_ext_csd[0];

		part_config = (ext_csd[EXT_CSD_PART_CFG] >> 3) & 0x07;
		if (part_config == 1) {
			*bootpart = EMMC_PART_BOOT1;
		} else if (part_config == 2) {
			*bootpart = EMMC_PART_BOOT2;
		} else if (part_config == 7) {
			*bootpart = EMMC_PART_USER;
		} else {
			msdc_pr_err("[PL MTK] fail to get boot part\n");
			return -1;
		}
	}

	return err;
}

int mmc_set_boot_part(u32 bootpart)
{
	struct mmc_card *card;
	struct mmc_host *host;
	int err = MMC_ERR_NONE;
	u8 *ext_csd;
	u8 part_config;
	u8 cur_bootpart;

	if (bootpart !=  EMMC_PART_BOOT1
         && bootpart !=  EMMC_PART_BOOT2
         && bootpart !=  EMMC_PART_USER)
		return -1;

	card = mmc_get_card(MMC_HOST_ID);
	host = mmc_get_host(MMC_HOST_ID);
	if (!card || !host)
		return -1;

	err = mmc_read_ext_csd(host, card);
	if (err != MMC_ERR_NONE)
		goto out;

	part_config = card->raw_ext_csd[EXT_CSD_PART_CFG];

	cur_bootpart = (part_config >> 3) & 0x7;
	if (((u32)cur_bootpart) == bootpart)
		goto out;
	msdc_pr_info("[SD0] Current boot part is %d\n", cur_bootpart);

	part_config = part_config & (~(0x7 << 3)) | (bootpart << 3);
	msdc_pr_info("[SD0] Set boot part as %d\n", bootpart);
	err = mmc_switch(host, card,
		EXT_CSD_CMD_SET_NORMAL, EXT_CSD_PART_CFG, part_config);

out:
	return err;
}
#endif

#if defined(MMC_MSDC_DRV_PRELOADER)
static int mmc_bread(blkdev_t *bdev, u32 blknr, u32 blks, u8 *buf, u32 part_id)
{
	struct mmc_host *host = (struct mmc_host*)bdev->priv;

	mmc_switch_part(part_id);
	return mmc_block_read(host->id, (unsigned long)blknr, blks, (unsigned long*)buf);
}

static int mmc_bwrite(blkdev_t *bdev, u32 blknr, u32 blks, u8 *buf, u32 part_id)
{
	struct mmc_host *host = (struct mmc_host*)bdev->priv;

	mmc_switch_part(part_id);
	return mmc_block_write(host->id, (unsigned long)blknr, blks, (unsigned long*)buf);
}

// ==========================================================
// MMC Common Interface - Init
// ==========================================================
u32 mmc_init_device(void)
{
	int ret;
	struct mmc_card *card;

	if (!blkdev_get(BOOTDEV_SDMMC)) {

		ret = mmc_init(MMC_HOST_ID, MSDC_MODE_DEFAULT);
		if (ret != 0) {
			msdc_pr_err("[SD0] init card failed\n");
			return ret;
		}

		#if defined(SLT)
		msdc_apply_timing(MMC_HOST_ID);
		#endif

		memset(&g_mmc_bdev, 0, sizeof(blkdev_t));

		card = mmc_get_card(MMC_HOST_ID);

		g_mmc_bdev.blksz   = MMC_BLOCK_SIZE;
		g_mmc_bdev.erasesz = MMC_BLOCK_SIZE;
		g_mmc_bdev.blks    = card->nblks;
		g_mmc_bdev.bread   = mmc_bread;
		g_mmc_bdev.bwrite  = mmc_bwrite;
		g_mmc_bdev.type    = BOOTDEV_SDMMC;
		g_mmc_bdev.blkbuf  = NULL;
		g_mmc_bdev.priv    = (void*)mmc_get_host(MMC_HOST_ID);

		g_mmc_bdev.offset = mmc_get_emmc_size(card) / MMC_BLOCK_SIZE; /* in blksz unit */
		g_emmc_user_size = (u64)card->blklen * card->nblks;

		blkdev_register(&g_mmc_bdev);
	}
	return 0;
}

#if defined(FEATURE_EMCP)
u32 mmc_get_device_id(u8 *id, u32 len,u32 *fw_len)
{
	u8 buf[16]; /* CID = 128 bits */
	struct mmc_card *card;
	u8 buf_sanid[512];
	u8 i;

	if (0 != mmc_init_device())
		return -1;

	card = mmc_get_card(MMC_HOST_ID);

	buf[0] = (card->raw_cid[0] >> 24) & 0xFF; /* Manufacturer ID */
	buf[1] = (card->raw_cid[0] >> 16) & 0xFF; /* Reserved(6)+Card/BGA(2) */
	buf[2] = (card->raw_cid[0] >> 8 ) & 0xFF; /* OEM/Application ID */
	buf[3] = (card->raw_cid[0] >> 0 ) & 0xFF; /* Product name [0] */
	buf[4] = (card->raw_cid[1] >> 24) & 0xFF; /* Product name [1] */
	buf[5] = (card->raw_cid[1] >> 16) & 0xFF; /* Product name [2] */
	buf[6] = (card->raw_cid[1] >> 8 ) & 0xFF; /* Product name [3] */
	buf[7] = (card->raw_cid[1] >> 0 ) & 0xFF; /* Product name [4] */
	buf[8] = (card->raw_cid[2] >> 24) & 0xFF; /* Product name [5] */
	buf[9] = (card->raw_cid[2] >> 16) & 0xFF; /* Product revision */
	buf[10] =(card->raw_cid[2] >> 8 ) & 0xFF; /* Serial Number [0] */
	buf[11] =(card->raw_cid[2] >> 0 ) & 0xFF; /* Serial Number [1] */
	buf[12] =(card->raw_cid[3] >> 24) & 0xFF; /* Serial Number [2] */
	buf[13] =(card->raw_cid[3] >> 16) & 0xFF; /* Serial Number [3] */
	buf[14] =(card->raw_cid[3] >> 8 ) & 0xFF; /* Manufacturer date */
	buf[15] =(card->raw_cid[3] >> 0 ) & 0xFF; /* CRC7 + stuff bit*/
	*fw_len = 1;

	/* sandisk */
	if (buf[0] == 0x45) {
		/* before v4.41 */
		if (card->raw_ext_csd[EXT_CSD_REV] <= 5) {
			if (0 == mmc_get_sandisk_fwid(MMC_HOST_ID,buf_sanid)) {
				*fw_len = 6;
			}
		} else if (card->raw_ext_csd[EXT_CSD_REV] == 6) { /* v4.5, v4.51 */
			/* do nothing, same as other vendor */
		} else if (card->raw_ext_csd[EXT_CSD_REV] == 7) { /* v5.0 */
			/* sandisk only use 6 bytes */
			*fw_len = 6;
			for (i=0; i<6; i++) {
				buf_sanid[32+i] = card->raw_ext_csd[EXT_CSD_FIRMWARE_VERSION+i];
			}
		}
	}

	len = len > 16 ? 16 : len;
	memcpy(id, buf, len);
	if (*fw_len == 6)
		memcpy(id+len,buf_sanid+32,6);
	else
		*(id+len) = buf[9];

#if defined(MMC_MSDC_DRV_PRELOADER)
	msdc_pr_err("eMMC cid: %x,%x,%x,%x\n", card->raw_cid[0], card->raw_cid[1],
		card->raw_cid[2], card->raw_cid[3]);
#endif

	return 0;
}
#endif

#if defined(FEATURE_MMC_BOOT_ACCESS)
int mmc_bread_boot(blkdev_t *bdev, u32 blknr, u32 blks, u8 *buf)
{
	u32 pid, id;
	struct mmc_host *host;

	if (bdev && bdev->priv) {
		host = (struct mmc_host*)bdev->priv;
		id = host->id;
	} else {
		msdc_pr_err("[%s]: invalid bdev, force the host id to 0\n", __func__);
		id = 0;
	}

	if (mmc_get_boot_part(&pid)) {
		msdc_pr_err("[%s]: invalid pid=%d\n", __func__, pid);
		return -1;
	}

	mmc_switch_part(pid);

	return mmc_block_read(id, (unsigned long)blknr, blks, (unsigned long*)buf);
}

int mmc_bwrite_boot(blkdev_t *bdev, u32 blknr, u32 blks, u8 *buf)
{
	u32 pid, id;
	struct mmc_host *host;

	if (bdev && bdev->priv) {
		host = (struct mmc_host*)bdev->priv;
		id = host->id;
	} else {
		id = 0;
	}

	if (mmc_get_boot_part(&pid)) {
		msdc_pr_err("[%s]: invalid pid=%d\n", __func__, pid);
		return -1;
	}

	mmc_switch_part(pid);

	return mmc_block_write(id, (unsigned long)blknr, blks, (unsigned long*)buf);
}
#endif
#endif //#if defined(MMC_MSDC_DRV_PRELOADER)


#if defined(MMC_MSDC_DRV_LK)
#include "part_dev.h"
static block_dev_desc_t sd_dev[MSDC_MAX_NUM];
static int boot_dev_found = 0;
static part_dev_t boot_dev;

unsigned long mmc_wrap_bread(int dev_num, unsigned long blknr, lbaint_t blkcnt, void *dst, unsigned int part_id)
{
	unsigned long ret;

	#ifdef LK_PROFILING
	unsigned long t1, t2;
	t1 = get_timer(0);
	#endif

	mmc_switch_part(part_id);
	if (mmc_block_read(dev_num, blknr, blkcnt, (unsigned long *)dst) == MMC_ERR_NONE)
		ret = blkcnt;
	else
		ret = (unsigned long) -1;

	#ifdef LK_PROFILING
	t2 = get_timer(t1);
	msdc_pr_err("[PROFILE] mmc read %d blks in %d ms: %dKB/s\n", blkcnt, t2, blkcnt * 500 / t2);
	#endif

	return ret;
}
unsigned long mmc_wrap_bwrite(int dev_num, unsigned long blknr, lbaint_t blkcnt, const void *src, unsigned int part_id)
{
	unsigned long ret;

	#ifdef LK_PROFILING
	unsigned long t1, t2;
	t1 = get_timer(0);
	#endif

	mmc_switch_part(part_id);
	if (mmc_block_write(dev_num, blknr, blkcnt, (unsigned long *)src) == MMC_ERR_NONE)
		ret = blkcnt;
	else
		ret = (unsigned long) -1;

	#ifdef LK_PROFILING
	t2 = get_timer(t1);
	msdc_pr_err("[PROFILE] mmc write %d blks in %d ms: %dKB/s\n", blkcnt, t2, blkcnt * 500 / t2);
	#endif

	return ret;
}

int mmc_legacy_init(int verbose)
{
	int id = verbose - 1;
	int err = MMC_ERR_NONE;
	struct mmc_host *host;
	struct mmc_card *card;
	block_dev_desc_t *bdev;

	bdev = &sd_dev[id];

	//msdc_hard_reset(host);

	err = mmc_init(id, MSDC_MODE_DEFAULT);

	if (err == MMC_ERR_NONE && !boot_dev_found) {
		/* fill in device description */
		card=mmc_get_card(id);
		host=mmc_get_host(id);
		g_emmc_size = mmc_get_emmc_size(card);
		g_emmc_user_size = (u64)card->blklen * card->nblks;

		bdev->dev         = id;
		bdev->blksz       = MMC_BLOCK_SIZE;
		bdev->lba         = card->nblks * card->blklen / MMC_BLOCK_SIZE;
		bdev->blk_bits    = 9;
		bdev->part_boot1  = EMMC_PART_BOOT1;
		bdev->part_boot2  = EMMC_PART_BOOT2;
		bdev->part_user   = EMMC_PART_USER;
		bdev->block_read  = mmc_wrap_bread;
		bdev->block_write = mmc_wrap_bwrite;

#if defined(MEM_PRESERVED_MODE_ENABLE)
		if (id==1) {
			host->boot_type = NON_BOOTABLE;
			return err;
		}
#endif

		host->boot_type   = RAW_BOOT;

		/* FIXME. only one RAW_BOOT dev */
		if (host->boot_type == RAW_BOOT) {
			boot_dev.id = id;
			boot_dev.init = 1;
			boot_dev.blkdev = bdev;
			boot_dev.erase = mmc_do_erase;
			mt_part_register_device(&boot_dev);
			boot_dev_found = 1;
			msdc_pr_info("[SD%d] boot device found\n", id);
		} else if (host->boot_type == FAT_BOOT) {
#if (CONFIG_COMMANDS & CFG_CMD_FAT)
			if (0 == fat_register_device(bdev, 1)) {
				boot_dev_found = 1;
				msdc_pr_info("[SD%d] FAT partition found\n", id);
			}
#endif
		}
	}

	return err;
}

// ==========================================================
// MMC Common Interface - Erase
// ==========================================================
static int __mmc_do_erase(struct mmc_host *host,struct mmc_card *card, u64 start_addr, u64 len)
{
	int err = MMC_ERR_NONE;
	u64 end_addr =((start_addr + len + card->blklen - 1)/card->blklen - 1) * card->blklen;
	u8 buf[512];

	if (end_addr/card->blklen > card->nblks - 1) {
		msdc_pr_err("[MSDC%d]Erase address out of range! start<0x%llx>,len<0x%llx>,card_nblks<0x%x>\n",
			host->id, start_addr, len, card->nblks);
		return MMC_ERR_INVALID;
	}

resend:
	err = mmc_erase_start(card, start_addr);
	if (err != MMC_ERR_NONE) {
		msdc_pr_err("[MSDC%d]Set erase start addrees 0x%llx failed,Err<%d>\n", host->id, start_addr, err);

		if (err == MMC_ERR_ERASE_SEQ) {
			msdc_pr_err("[MSDC%d]Erase sequence error, retry erase.\n", host->id);
			goto dummy_read;
		} else {
			goto out;
		}
	}

	err = mmc_erase_end(card, end_addr);
	if (err != MMC_ERR_NONE) {
		msdc_pr_err("[MSDC%d]Set erase end addrees 0x%llx + 0x%llx failed,Err<%d>\n", host->id, start_addr, len, err);

		if (err == MMC_ERR_ERASE_SEQ) {
			msdc_pr_err("[MSDC%d]Erase sequence error, retry erase.\n", host->id);
			goto dummy_read;
		} else {
			goto out;
		}
	}

	err = mmc_erase(card, MMC_ERASE_TRIM);
	if (err != MMC_ERR_NONE) {
		msdc_pr_err("[MSDC%d]Set erase <0x%llx - 0x%llx> failed,Err<%d>\n", host->id, start_addr, start_addr + len, err);
		goto out;
	}

	msdc_pr_info("[MSDC%d]0x%llx - 0x%llx Erased\n", host->id, start_addr, start_addr + len);
out:
	return err;

dummy_read:
	/* Add dummy read to reset erase sequence error. */
	err = mmc_block_read(host->id, 0, 1, (unsigned long*)buf);
	if (err != MMC_ERR_NONE) {
		msdc_pr_err("[MSDC%d]Fail in read card\n", host->id);
		goto out;
	}

	goto resend;
}

int mmc_do_erase(int dev_num, u64 start_addr, u64 len, u32 part_id)
{
	struct mmc_host *host = mmc_get_host(dev_num);
	struct mmc_card *card = mmc_get_card(dev_num);
	//struct mmc_erase_part erase_part[EMMC_PART_END];
	u32 err;

	if (!card || !host) {
		msdc_pr_err("[mmc_do_erase] card or host is NULL\n");
		return MMC_ERR_INVALID;
	}

	if (!len) {
		msdc_pr_err("[MSDC%d] invalid erase size! len<0x%llx>\n",host->id,len);
		return MMC_ERR_INVALID;
	}
	if ((start_addr % card->blklen) || (len % card->blklen)) {
		msdc_pr_err("[MSDC%d] non-alignment erase address! start<0x%llx>,len<0x%llx>,card_nblks<0x%x>\n",
			host->id, start_addr, len, card->nblks);
		return MMC_ERR_INVALID;
	}

	if ((err = mmc_switch_part(part_id))) {
		msdc_pr_err("[MSDC%d] mmc swtich failed.part<%d> error <%d> \n", host->id, part_id, err);
		return err;
	}

	if ((err = __mmc_do_erase(host, card, start_addr, len))) {
		msdc_pr_err("[MSDC%d] mmc erase failed.error <%d> \n",host->id, err);
		return err;
	}

	return err;

}

#endif // #if defined(MMC_MSDC_DRV_LK)

#endif //!defined(MMC_MSDC_DRV_CTP)

#if defined(SLT) || defined(MMC_MSDC_DRV_CTP)
/* copy from autok.c */
#define AUTOK_CKGEN_VALUE                       (0)
#define AUTOK_CMD_LATCH_EN_HS400_PORT0_VALUE    (3)
#define AUTOK_CRC_LATCH_EN_HS400_PORT0_VALUE    (3)
#define AUTOK_CMD_LATCH_EN_DDR208_PORT3_VALUE   (3)
#define AUTOK_CRC_LATCH_EN_DDR208_PORT3_VALUE   (3)
#define AUTOK_CMD_LATCH_EN_HS200_PORT0_VALUE    (2)
#define AUTOK_CRC_LATCH_EN_HS200_PORT0_VALUE    (2)
#define AUTOK_CMD_LATCH_EN_SDR104_PORT1_VALUE   (2)
#define AUTOK_CRC_LATCH_EN_SDR104_PORT1_VALUE   (2)
#define AUTOK_CMD_LATCH_EN_HS_VALUE             (1)
#define AUTOK_CRC_LATCH_EN_HS_VALUE             (1)

void msdc_apply_timing(unsigned int id)
{
	struct mmc_host *host;
	msdc_priv_t *priv;
	int mode;
	u32 base;
	u32 base_top;

	u32 hz, div;

	host = mmc_get_host(id);
	priv = host->priv;
	base = host->base;
	base_top = host->base_top;

	hz = host->cur_bus_clk;

	MSDC_SET_FIELD(host->base + 0x208, (0x1 << 9), 0);
	if (priv->tuning_mode == MSDC_PATH_USE_DELAY_LINE) {
		msdc_pr_err("[SD%d] \"delay cell + clock tune\" is not supported\n", id);

	} else if (priv->tuning_mode == MSDC_PATH_USE_ASYNC_FIFO) {
		MSDC_GET_FIELD(MSDC_CFG, MSDC_CFG_CKMOD, mode);
		MSDC_GET_FIELD(MSDC_CFG, MSDC_CFG_CKDIV, div);

		if ((id == 0) && (mode == 3)) {
			/*eMMC HS400*/
			msdc_pr_err("[SD%d] %s %d: HS400\n", id,  __func__, __LINE__);

			MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_RSPL, 0);    /* MSDC_IOCON[MSDC_IOCON_RSPL] 0x4 bit1 */
			MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D_SMPL, 0);     /* MSDC_IOCON[MSDC_IOCON_R_D_SMPL] 0x4 bit2 */
			MSDC_SET_FIELD(MSDC_PATCH_BIT1 , MSDC_PB1_STOP_DLY_SEL, 3);    /* MSDC_PB1_STOP_DLY_SEL 0xb4[11:8] */
			MSDC_SET_FIELD(MSDC_PATCH_BIT2 , MSDC_PB2_CRCSTSENSEL, 0); /* 0xb8[31:29] */
			MSDC_SET_FIELD(MSDC_PATCH_BIT2 , MSDC_PB2_RESPSTENSEL, 0); /* 0xb8[18:16] */
			MSDC_SET_FIELD(EMMC50_CFG0, MSDC_EMMC50_CFG_CRC_STS_EDGE, 0); /* 0x208 bit3 */
			/*top reg*/
			MSDC_WRITE32(EMMC_TOP_CONTROL, 0xa080);  /* EMMC_TOP_CONTROL 0x0 */
			MSDC_WRITE32(EMMC_TOP_CMD, 0xac0);  /* EMMC_TOP_CMD 0x4 */
			MSDC_WRITE32(TOP_EMMC50_PAD_CTL0, 0x0);  /* TOP_EMMC50_PAD_CTL0 0x8 */
			MSDC_WRITE32(TOP_EMMC50_PAD_DS_TUNE, 0x12c14);  /* TOP_EMMC50_PAD_DS_TUNE 0xc */
		} else if ((id == 0) && (div == 0)) {
			/*eMMC HS200*/
			msdc_pr_err("[SD%d] %s %d: HS200\n", id,  __func__, __LINE__);

			MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_RSPL, 0);    /*MSDC_IOCON[MSDC_IOCON_RSPL] 0x4 bit1 */
			MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D_SMPL, 0);     /* MSDC_IOCON[MSDC_IOCON_R_D_SMPL] 0x4 bit2 */
			MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_INT_DAT_LATCH_CK_SEL, 0);      /* PATCH_BIT0[MSDC_PB0_INT_DAT_LATCH_CK_SEL] 0xb0[9:7] */
			MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_RD_DAT_SEL, 0); /* 0xb0 bit3 */
			MSDC_SET_FIELD(MSDC_PATCH_BIT1, MSDC_PB1_STOP_DLY_SEL, 3);    /* MSDC_PB1_STOP_DLY_SEL 0xb4[11:8] */
			MSDC_SET_FIELD(MSDC_PATCH_BIT2, MSDC_PB2_CRCSTSENSEL, 0); /* 0xb8[31:29] */
			MSDC_SET_FIELD(MSDC_PATCH_BIT2, MSDC_PB2_RESPSTENSEL, 0); /* 0xb8[18:16] */
			MSDC_SET_FIELD(MSDC_PATCH_BIT2, MSDC_PB2_CFGCRCSTSEDGE, 0);      /* MSDC_PB2_CFGCRCSTSEDGE, write crc stage edge */ /* 0xb8 bit25 */
			/*top reg*/
			MSDC_WRITE32(EMMC_TOP_CONTROL, 0xaa80);  /* EMMC_TOP_CONTROL 0x0 */
			MSDC_WRITE32(EMMC_TOP_CMD, 0xac0);  /* EMMC_TOP_CMD 0x4 */
			MSDC_WRITE32(TOP_EMMC50_PAD_CTL0, 0x0);  /* TOP_EMMC50_PAD_CTL0 0x8 */
			MSDC_WRITE32(TOP_EMMC50_PAD_DS_TUNE, 0x0);  /* TOP_EMMC50_PAD_DS_TUNE 0xc */
		}
	}
}
#endif
