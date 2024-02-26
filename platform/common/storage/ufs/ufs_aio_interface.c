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

#include "ufs_aio_cfg.h"
#include "ufs_aio_types.h"
#include "ufs_aio.h"
#include "ufs_aio_hcd.h"
#include "ufs_aio_core.h"
#include "ufs_aio_utils.h"
#include "ufs_aio_error.h"
#include "ufs_aio_rpmb.h"

#if defined(MTK_UFS_DRV_PRELOADER)
#include "blkdev.h"
#include "boot_device.h"
#include "storage_api.h"

static blkdev_t g_ufs_dev;

int ufs_switch_part(u32 part_id)
{
	u8 lun;

	switch (part_id) {
		case UFS_LU_BOOT1:
			lun = UFS_LU_0;
			break;
		case UFS_LU_BOOT2:
			lun = UFS_LU_1;
			break;
		case UFS_LU_USER:
			lun = UFS_LU_2;
			break;
		default:
			UFS_DBG_LOGE("[UFS] err: ufs_switch_part, invalid UFS LU %d\n", part_id);
			return UFS_ERR_INVALID_LU;
	}

	UFS_DBG_LOGV("[UFS] switch to part %d, lun %d\n", part_id, lun);

	return (int)lun;
}

static int ufs_bread(blkdev_t *bdev, u32 blknr, u32 blks, u8 *buf, u32 part_id)
{
	int lun;

	UFS_DBG_LOGV("[UFS] info: r, %d, %d, %d\n", blknr, blks, part_id);

	lun = ufs_switch_part(part_id);

	if (lun < 0)
		return lun;

	return ufs_aio_block_read(0, (u32)lun, blknr, blks, (unsigned long *)buf);
}

static int ufs_bwrite(blkdev_t *bdev, u32 blknr, u32 blks, u8 *buf, u32 part_id)
{
	int lun;

	UFS_DBG_LOGV("[UFS] info: w, %d, %d, %d\n", blknr, blks, part_id);

	lun = ufs_switch_part(part_id);

	if (lun < 0)
		return lun;

	return ufs_aio_block_write(0, (u32)lun, blknr, blks, (unsigned long *)buf);
}

static u64 ufs_get_part_size(blkdev_t *dev, u32 part_id)
{
	struct ufs_hba * hba = ufs_aio_get_host(UFS_DEFAULT_HOST_ID);
	int lun;
	int ret;
	u32 blk_size = 0, blk_cnt = 0;
	u64 size_in_bytes;

	lun = ufs_switch_part(part_id);

	if (lun < 0)
		return lun;

	ret = ufs_aio_get_lu_size(hba, (u32)lun, &blk_size, &blk_cnt);

	size_in_bytes = (u64)blk_size * (u64)blk_cnt;

	if (ret < 0) {
		UFS_DBG_LOGE("[UFS] err: ufs_get_part_size fail, part_id %d, ret %d\n", part_id, ret);
		return 0;
	}

	UFS_DBG_LOGV("[UFS] info: ufs_get_part_size, %d, %llx\n", part_id, size_in_bytes);

	return size_in_bytes;
}

//==========================================================
// UFS Common Interface - Init
//==========================================================
u32 ufs_init_device(void)
{
	struct ufs_hba * hba = ufs_aio_get_host(UFS_DEFAULT_HOST_ID);
	int ret;

#ifdef UFS_CFG_HQA_MODE
	ufs_aio_hqa_mode();
#endif

	if (!blkdev_get(BOOTDEV_UFS)) {

		ret = ufs_aio_cfg_mode(UFS_DEFAULT_HOST_ID, UFS_MODE_DEFAULT);

		if (ret != 0) {
			UFS_DBG_LOGD("[UFS] err: ufs_aio_cfg_mode failed\n");
			return ret;
		}

		ret = ufshcd_init();

		if (ret != 0) {
			UFS_DBG_LOGD("[UFS] err: ufshcd_init failed\n");
			return ret;
		}

		memset(&g_ufs_dev, 0, sizeof(blkdev_t));

		/*
		 * Some blkdev_t members are not used by upper layer, can be undefined.
		 * Necessary members: type, blksz, blkbuf, next, bread and bwrite.
		 */

		g_ufs_dev.type          = BOOTDEV_UFS;
		g_ufs_dev.blksz         = UFS_BLOCK_SIZE;
		g_ufs_dev.bread         = ufs_bread;
		g_ufs_dev.bwrite        = ufs_bwrite;
		g_ufs_dev.get_part_size = ufs_get_part_size;
		g_ufs_dev.erasesz       = UFS_BLOCK_SIZE;

		/*
		 * Use SRAM buffer for blkdev operations.
		 *
		 * Since MT6799, storage device shall be initialized before DRAM init.
		 * In such case, make sure all buffers that we will touch are all located
		 * in SRAM.
		 */
		g_ufs_dev.blkbuf        = NULL;
		g_ufs_dev.priv          = ufs_aio_get_host(UFS_DEFAULT_HOST_ID);

		blkdev_register(&g_ufs_dev);
	}

	return 0;
}

int ufs_rpmb_get_rw_size(void)
{
	return ufs_aio_rpmb_get_rw_size();
}

u64 ufs_rpmb_get_lu_size(void)
{
	return ufs_aio_rpmb_get_lu_size();
}

int ufs_get_device_id(u8 * id, u32 buf_len, u32 * fw_len)
{
	struct ufs_hba * hba;
	u32 id_len, i;

	if (0 != ufs_init_device())
		return -1;

	hba = ufs_aio_get_host(UFS_DEFAULT_HOST_ID);

	if (0 == hba->dev_info.wmanufacturerid)
		return -1;

	id_len = strlen(hba->dev_info.product_id);
	id_len = min_t(u32, id_len, buf_len);

	memcpy(id, hba->dev_info.product_id, id_len);

	*fw_len = id_len;

	for (i = 0; i < id_len; i++)
		UFS_DBG_LOGE("%x" , *(id+i));
	UFS_DBG_LOGE("\nufs id:%s, len:%d\n", id, id_len);

	return 0;
}

int ufs_get_boot_part(int *part_id)
{
	struct ufs_hba * hba;
	int ret;
	u32 b_boot_lun = 0;

	hba = ufs_aio_get_host(UFS_DEFAULT_HOST_ID);

	ret = ufs_aio_get_boot_lu(hba, &b_boot_lun);

	if (0 != ret)
		UFS_DBG_LOGD("[UFS] err: ufs_aio_get_boot_lu error: %d\n", ret);
	else {
		if (b_boot_lun == ATTR_B_BOOT_LUN_EN_BOOT_LU_A)
			*part_id = STORAGE_PHYS_PART_BOOT1;
		else if (b_boot_lun == ATTR_B_BOOT_LUN_EN_BOOT_LU_B)
			*part_id = STORAGE_PHYS_PART_BOOT2;

		UFS_DBG_LOGD("[UFS] info: ufs_aio_get_boot_lu, part_id=%d\n", *part_id);
	}

	return ret;
}

int ufs_set_boot_part(int part_id)
{
	struct ufs_hba * hba;
	int ret;
	u32 b_boot_lun = 0;

	hba = ufs_aio_get_host(UFS_DEFAULT_HOST_ID);

	if (part_id == STORAGE_PHYS_PART_BOOT1)
		b_boot_lun = ATTR_B_BOOT_LUN_EN_BOOT_LU_A;
	else if (part_id == STORAGE_PHYS_PART_BOOT2)
		b_boot_lun = ATTR_B_BOOT_LUN_EN_BOOT_LU_B;

	ret = ufs_aio_set_boot_lu(hba, b_boot_lun);

	return ret;
}
#endif /* MTK_UFS_DRV_PRELOADER */

#if defined(MTK_UFS_DRV_LK)
#include "block_generic_interface.h"
static block_dev_desc_t g_ufs_dev[UFS_AIO_MAX_DEVICE];
static part_dev_t g_ufs_boot_dev;



unsigned long ufs_wrap_bread(int dev_num, unsigned long blknr, lbaint_t blkcnt, void *dst, unsigned int part_id)
{
	int lun;
	int ret;

	lun = ufs_switch_part(part_id);

	if (lun < 0)
		return (unsigned long) -1;

	ret = ufs_aio_block_read(dev_num, lun, blknr, blkcnt, (unsigned long *)dst);

	UFS_DBG_LOGD("[UFS] r, %lu, %lu, %d, %d\n", blknr, blkcnt, lun, ret);

	if (!ret)
		return blkcnt;
	else
		return (unsigned long) -1;
}
unsigned long ufs_wrap_bwrite(int dev_num, unsigned long blknr, lbaint_t blkcnt, const void *src, unsigned int part_id)
{
	int lun;
	int ret;

	lun = ufs_switch_part(part_id);

	if (lun < 0)
		return (unsigned long) -1;

	ret = ufs_aio_block_write(dev_num, lun, blknr, blkcnt, (unsigned long *)src);

	UFS_DBG_LOGD("[UFS] w, %lu, %lu, %d, %d\n", blknr, blkcnt, lun, ret);

	if (!ret)
		return blkcnt;
	else
		return (unsigned long) -1;
}

int ufs_lk_get_active_boot_part(u32 *active_boot_part)
{
	struct ufs_hba * hba = ufs_aio_get_host(UFS_DEFAULT_HOST_ID);
	u32 b_boot_lun_en;
	int ret;

	if (!active_boot_part)
		return -1;

	ret = hba->query_attr(hba, UPIU_QUERY_OPCODE_READ_ATTR, ATTR_B_BOOT_LUN_EN, 0, 0, &b_boot_lun_en);

	if (UFS_ERR_NONE != ret) {
		UFS_DBG_LOGE("[UFS] err: read ATTR_B_BOOT_LUN_EN error: %d\n", ret);
		return ret;
	}

	if (ATTR_B_BOOT_LUN_EN_BOOT_LU_A == b_boot_lun_en)      // Enabled boot from Boot LU A
		*active_boot_part = UFS_LU_BOOT1;
	else if (ATTR_B_BOOT_LUN_EN_BOOT_LU_B == b_boot_lun_en) // Enabled boot from Boot LU B
		*active_boot_part = UFS_LU_BOOT2;
	else {
		UFS_DBG_LOGE("[UFS] err: invalid ATTR_B_BOOT_LUN_EN %d\n", *active_boot_part);
		return -1;
	}

	return ret;
}

static u64 ufs_lk_get_part_size(part_dev_t *dev, u32 part_id)
{
	struct ufs_hba * hba = ufs_aio_get_host(UFS_DEFAULT_HOST_ID);
	int lun;
	int ret;
	u32 blk_size = 0, blk_cnt = 0;
	u64 size_in_bytes;

	lun = ufs_switch_part(part_id);

	if (lun < UFS_ERR_NONE)
		return lun;

	ret = ufs_aio_get_lu_size(hba, (u32)lun, &blk_size, &blk_cnt);

	size_in_bytes = (u64)blk_size * (u64)blk_cnt;

	if (ret < 0) {
		UFS_DBG_LOGE("[UFS] err: ufs_get_part_size fail, part_id %d, ret %d\n", part_id, ret);
		return 0;
	}

	UFS_DBG_LOGV("[UFS] info: ufs_get_part_size, %d, %llx\n", part_id, size_in_bytes);

	return size_in_bytes;
}

unsigned long long ufs_lk_get_device_size()
{
	struct ufs_hba *hba = ufs_aio_get_host(UFS_DEFAULT_HOST_ID);
	unsigned long long size;
	u32 i;

	for (i = 0, size = 0; i < UFS_LU_INTERNAL_CNT; i++)
		size += (unsigned long long)hba->blk_cnt[i] * (unsigned long long)UFS_BLOCK_SIZE;

	return size;
}

static int ufs_lk_erase(int dev_num, u64 start_addr, u64 len,u32 part_id);
int ufs_lk_init()
{
	struct ufs_hba * hba = ufs_aio_get_host(UFS_DEFAULT_HOST_ID);
	int ret;
	int i;
	block_dev_desc_t *bdev;
	bdev = &g_ufs_dev[UFS_DEFAULT_HOST_ID];

	ufs_aio_cfg_mode(UFS_DEFAULT_HOST_ID, UFS_MODE_DEFAULT);

	ret = ufshcd_init();

	if (ret != 0) {
		UFS_DBG_LOGE("[UFS] err: ufshcd_init failed\n");
		return ret;
	}

	if (ret == UFS_ERR_NONE) {
		// fill in device description

		bdev->dev         = UFS_DEFAULT_HOST_ID;
		bdev->type        = BOOTDEV_UFS;
		bdev->blksz       = UFS_BLOCK_SIZE;
		bdev->blk_bits    = 12;
		bdev->part_boot1  = UFS_LU_BOOT1;
		bdev->part_boot2  = UFS_LU_BOOT2;
		bdev->part_user   = UFS_LU_USER;
		bdev->block_read  = ufs_wrap_bread;
		bdev->block_write = ufs_wrap_bwrite;

		g_ufs_boot_dev.id = UFS_DEFAULT_HOST_ID;
		g_ufs_boot_dev.init = 1;
		g_ufs_boot_dev.blkdev = bdev;
		g_ufs_boot_dev.erase = ufs_lk_erase;
		g_ufs_boot_dev.get_part_size = ufs_lk_get_part_size;

		/*
		 * Leave g_ufs_boot_dev.read and g_ufs_boot_dev.write as NULL for mt_part_register_device().
		 *
		 * This will let mt_part_register_device() hook mt_part_generic_read() and mt_part_generic_write()
		 * to structure part_dev_t to handle non-aligned (including start address and length) access.
		 */

		mt_part_register_device(&g_ufs_boot_dev);

		UFS_DBG_LOGI("[UFS] info: boot device found\n");

		// get LU size

		for (i = 0; i < UFS_LU_INTERNAL_CNT; i++) {
			ret = ufs_aio_get_lu_size(hba, i, NULL, &(hba->blk_cnt[i]));

			if (ret) {
				UFS_DBG_LOGE("[UFS] err: ufs_aio_get_lu_size(%d) fail, ret %d\n", i, ret);
				break;
			}
		}
	}

	UFS_DBG_LOGI("[UFS] info: ufs init OK\n");

	return ret;
}

int ufs_lk_erase(int dev_num, u64 start_addr, u64 len,u32 part_id)
{
	struct ufs_hba *hba = ufs_aio_get_host(UFS_DEFAULT_HOST_ID);
	block_dev_desc_t *bdev = &g_ufs_dev[UFS_DEFAULT_HOST_ID];
	u32 start_blk, end_blk;
	int lun;
	int ret;

	if (!len) {
		UFS_DBG_LOGE("[UFS] err: nvalid erase size! len: 0x%llx\n", len);
		return UFS_ERR_INVALID_ERASE_SIZE;
	}

	if ((start_addr % bdev->blksz) || (len % bdev->blksz)) {
		UFS_DBG_LOGE("[UFS] err: non-alignment erase address or length! start: 0x%llx, len: 0x%llx\n", start_addr, len);
		return UFS_ERR_INVALID_ALIGNMENT;
	}

	lun = ufs_switch_part(part_id);

	if (lun < UFS_ERR_NONE) {
		UFS_DBG_LOGE("[UFS] err: swtich partition failed, part %d, ret %d\n", part_id, lun);
		return lun;
	}

	end_blk = (u32)((start_addr + len) / (u64)bdev->blksz) - 1;

	if (end_blk >= hba->blk_cnt[lun]) {
		UFS_DBG_LOGE("[UFS] err: erase address out of range! lun %d, blk_cnt %d, start <0x%llx>, len <0x%llx>, end_blk %d\n", lun, hba->blk_cnt[lun], start_addr, len, end_blk);
		return UFS_ERR_OUT_OF_RANGE;
	}

	start_blk = (u32)(start_addr / (u64)bdev->blksz);

	ret = hba->blk_erase(hba, lun, start_blk, end_blk - start_blk + 1);

	if (ret)
		UFS_DBG_LOGE("[UFS] err: erase fail <0x%llx - 0x%llx>, <%d - %d> ret %d\n", start_addr, start_addr + len, start_blk, end_blk, ret);

	return ret;
}

int ufs_lk_otp_lock_req(char *otp_part_name)
{
	return ufs_aio_otp_lock_req(otp_part_name);
}

int ufs_lk_otp_read(u32 current_user_id, off_t offset, u8* data, size_t size)
{
	return ufs_aio_otp_read(current_user_id, offset, data, size);

}

int ufs_lk_otp_write(u32 current_user_id, off_t offset, u8* data, size_t size)
{
	return ufs_aio_otp_write(current_user_id, offset, data, size);

}

int ufs_lk_otp_lock(void)
{
	return ufs_aio_otp_lock();
}

int ufs_get_unique_id(struct ufs_unique_id *id)
{
	struct ufs_hba *hba;

	hba = ufs_aio_get_host(UFS_DEFAULT_HOST_ID);

	memset(id, 0, sizeof(struct ufs_unique_id));

	if (0 == hba->dev_info.wmanufacturerid)
		return -1;

	/* UFS vendor id */
	id->vid = hba->dev_info.wmanufacturerid;

	/* UFS product ID */
	if (hba->dev_info.product_id[0] != 0)
		strlcpy((char *)id->pid, hba->dev_info.product_id, MAX_PRODUCT_ID_LEN + 1);

	/* UFS serial number */
	if (hba->dev_info.serial_number_len != 0)
		strlcpy((char *)id->sn, hba->dev_info.serial_number, hba->dev_info.serial_number_len * 2 + 1);

	return 0;
}
#endif

#if defined(MTK_UFS_DRV_DA)

#include "boot/dev_interface/ufs_interface.h"
#include "interface_struct.h"
#include "debug.h"
#include "lib/string.h"
#include "ufs_aio_blkdev.h"
#include "boot/dev_interface/gpt_timer_interface.h"

static ufs_aio_blkdev_t g_ufs_dev;
static u32 g_ufs_internal_buf[UFS_BLOCK_SIZE / sizeof(u32)];

#ifdef UFS_CFG_PERFORMANCE_PROFILING
u32 g_ufs_last_read_time = 0;
u32 g_ufs_last_write_time = 0;
#endif

__WEAK int ufs_bread(ufs_aio_blkdev_t *bdev, u32 blknr, u32 blks, u8 *buf, u32 lun)
{
	int ret;
#ifdef UFS_CFG_PERFORMANCE_PROFILING
	u32 time_start, time_end;
#endif

	UFS_DBG_LOGD("[UFS] ufs_bread,%d,%d,%d\n", blknr, blks, (int)lun);

#ifdef UFS_CFG_PERFORMANCE_PROFILING
	time_start = Get_Current_Time_us();
#endif

	ret = ufs_aio_block_read(UFS_DEFAULT_HOST_ID, lun, blknr, blks, (unsigned long *)buf);

#ifdef UFS_CFG_PERFORMANCE_PROFILING
	time_end = Get_Current_Time_us();

	if (time_start != time_end)
		UFS_DBG_LOGD("[UFS] perf (ufs_bread): r,%d,%d,%d,%d KB/s\n", lun, blknr, blks, (blks * 4 * 1000000) / (time_end - time_start));
#endif

	UFS_DBG_LOGD("[UFS] ufs_bread,ret=%d\n", ret);

	return ret;
}

__WEAK int ufs_bwrite(ufs_aio_blkdev_t *bdev, u32 blknr, u32 blks, u8 *buf, u32 lun)
{
	int ret;
#ifdef UFS_CFG_PERFORMANCE_PROFILING
	u32 time_start, time_end;
#endif

	UFS_DBG_LOGD("[UFS] ufs_bwrite,%d,%d,%d\n", blknr, blks, (int)lun);

#ifdef UFS_CFG_PERFORMANCE_PROFILING
	time_start = Get_Current_Time_us();
#endif

	ret = ufs_aio_block_write(UFS_DEFAULT_HOST_ID, lun, blknr, blks, (unsigned long *)buf);

#ifdef UFS_CFG_PERFORMANCE_PROFILING
	time_end = Get_Current_Time_us();

	if (time_start != time_end)
		UFS_DBG_LOGD("[UFS] perf (ufs_bwrite): w,%d,%d,%d,%d KB/s\n", lun, blknr, blks, (blks * 4 * 1000000) / (time_end - time_start));
#endif

	UFS_DBG_LOGD("[UFS] ufs_bwrite,ret=%d\n", ret);

	return ret;
}

__WEAK status_t interface_ufs_init(u32 boot_channel)
{
	status_t ret;
	struct ufs_hba * hba = &g_ufs_hba;
	ufs_aio_blkdev_t *bdev = &g_ufs_dev;

	LOGI("[UFS] info: ufs init start\n");

	ufs_aio_cfg_mode(UFS_DEFAULT_HOST_ID, UFS_MODE_DEFAULT);

	ret = ufshcd_init();

	hba->active_lun = -1;

	if (UFS_ERR_NONE != ret) {
		LOGI("[UFS] err: ufshcd_init failed, err: %d\n", ret);
		return STATUS_UFS_ERR;
	}

	memset(bdev, 0, sizeof(ufs_aio_blkdev_t));
	bdev->type = BLKDEV_UFS;
	bdev->blkbuf = (u8 *)&g_ufs_internal_buf[0];
	bdev->blksz = UFS_BLOCK_SIZE;
	bdev->erasesz = UFS_BLOCK_SIZE;
	bdev->bread = ufs_bread;
	bdev->bwrite = ufs_bwrite;
	//bdev->blks = card->nblks;
	//bdev->priv = NULL;

	if (0 != ufs_aio_blkdev_register(bdev)) {
		LOGI("[UFS] err: blkdev_register failed, err: %d\n", ret);
		return STATUS_UFS_ERR;
	}

#ifdef UFS_CFG_PERFORMANCE_PROFILING
	GPT_Timer_Init();
#endif

	LOGI("[UFS] info: ufs clear write protect\n");
	if (ufs_clr_write_protect())
		LOGE("[UFS] err: clear write protect fail!!!\n");

	LOGI("[UFS] info: ufs init ok\n");

	return STATUS_OK;
}

__WEAK status_t interface_switch_ufs_section(u32 section)
{
	struct ufs_hba *hba = ufs_aio_get_host(UFS_DEFAULT_HOST_ID);
	u8 lun;

	LOGV("[UFS] switch to section %d\n", section);

	switch (section) {
		case UFS_SECTION_LU0:
			lun = 0;
			break;
		case UFS_SECTION_LU1:
			lun = 1;
			break;
		case UFS_SECTION_LU2:
			lun = 2;
			break;
		default:
			LOGE("[UFS] err: interface_switch_ufs_section: Invalid UFS LU %d\n", section);
			return STATUS_UFS_ERR;
	}

	hba->active_lun = lun;

	return STATUS_OK;
}

__WEAK status_t interface_ufs_write(u64 address, u8* buffer, u64 length)
{
	int ret = STATUS_OK;
	struct ufs_hba *hba = ufs_aio_get_host(UFS_DEFAULT_HOST_ID);
#ifdef UFS_CFG_PERFORMANCE_PROFILING
	u32 time_start, time_end;
	u32 last_write_time;
#endif

	if (-1 == hba->active_lun) {
		LOGD("[UFS] err: interface_ufs_write: active_lun is not initialized");
		return STATUS_UFS_ERR;
	}

	UFS_DBG_LOGD("[UFS] interface_ufs_write, %llx, %llx, 0x%x\n", address, length, (u32)&buffer[0]);

#ifdef UFS_CFG_PERFORMANCE_PROFILING
	time_start = Get_Current_Time_us();
	last_write_time = g_ufs_last_write_time;
	g_ufs_last_write_time = time_start;
#endif

	ret = ufs_aio_generic_write(address, buffer, length);

#ifdef UFS_CFG_PERFORMANCE_PROFILING
	time_end = Get_Current_Time_us();

	if (time_start != time_end)
		UFS_DBG_LOGD("[UFS] perf (interface_ufs_write): w,%d,%lld,%d,%d KB/s,%d us\n", hba->active_lun, address, (u32)length, (u32)((length / 1024) * 1000000) / (time_end - time_start), time_start - last_write_time);
#endif

	LOGD("[UFS] interface_ufs_write, ret: %d\n", ret);

	return ufs_aio_err_trans(ret);
}

__WEAK status_t interface_ufs_read(u64 address, u8* buffer, u64 length)
{
	int ret = STATUS_OK;
	struct ufs_hba *hba = ufs_aio_get_host(UFS_DEFAULT_HOST_ID);
#ifdef UFS_CFG_PERFORMANCE_PROFILING
	u32 time_start, time_end;
	u32 last_read_time;
#endif

	if (-1 == hba->active_lun) {
		LOGD("[UFS] err: interface_ufs_write: active_lun is not initialized");
		return STATUS_UFS_ERR;
	}

	UFS_DBG_LOGD("[UFS] interface_ufs_read,%llx, %llx, 0x%x\n", address, length, (u32)&buffer[0]);

#ifdef UFS_CFG_PERFORMANCE_PROFILING
	time_start = Get_Current_Time_us();
	last_read_time = g_ufs_last_read_time;
	g_ufs_last_read_time = time_start;
#endif

	ret = ufs_aio_generic_read(address, buffer, length);

#ifdef UFS_CFG_PERFORMANCE_PROFILING
	time_end = Get_Current_Time_us();

	if (time_start != time_end)
		UFS_DBG_LOGD("[UFS] perf (interface_ufs_read): r,%d,%lld,%d,%d KB/s,%d us\n", hba->active_lun, address, (u32)length, (u32)((length / 1024) * 1000000) / (time_end - time_start), time_start - last_read_time);
#endif

	LOGD("[UFS] interface_ufs_read, ret: %d\n", ret);

	return ufs_aio_err_trans(ret);
}

__WEAK status_t interface_ufs_erase(u64 address, u64 length, const struct progress_cb* cb)
{
	int ret = STATUS_OK;
	uint32 stop_flag = 0;
	uint32 progress = 0;
	struct ufs_hba *hba = ufs_aio_get_host(UFS_DEFAULT_HOST_ID);
	u32 blk_start = (u32)(address / (u64)UFS_BLOCK_SIZE);
	u32 blk_cnt = (u32)(length / (u64)UFS_BLOCK_SIZE);
	u32 blk_cnt_1_100;

	if (-1 == hba->active_lun) {
		LOGE("[UFS] err: interface_ufs_write: active_lun is not initialized");
		return STATUS_UFS_ERR;
	}

	UFS_DBG_LOGD("[UFS] info: erase %llx,%llx,%d,%d,%d\n", address, length, hba->active_lun, blk_start, blk_cnt);

	/* If erase size < 4MB, no separate, erase in one time */
	blk_cnt_1_100 = (blk_cnt / 100) & (0xFFFFFC00);

	while (blk_cnt) {
		if ((blk_cnt >= blk_cnt_1_100) && (blk_cnt_1_100 > 0)) {
			ret = hba->blk_erase(hba, hba->active_lun, blk_start, blk_cnt_1_100);
			if (ret != UFS_ERR_NONE) {
				LOGE("[UFS] err: erase failed, ret: %d\n", ret);
				break;
			}

			blk_start += blk_cnt_1_100;
			blk_cnt -= blk_cnt_1_100;
			if ((cb) && (progress < 100)) {
				ret = cb->cb(cb->user_arg, progress, &stop_flag);
				if(FAIL(ret))
					break;
				progress++;
			}
		} else {
			ret = hba->blk_erase(hba, hba->active_lun, blk_start, blk_cnt);
			if (ret != UFS_ERR_NONE) {
				LOGE("[UFS] err: erase failed, ret: %d\n", ret);
				break;
			}

			if (cb)
				cb->cb(cb->user_arg, 100, &stop_flag);
			break;
		}
	}

	return ufs_aio_err_trans(ret);
}

__WEAK status_t interface_get_ufs_info(struct ufs_info_struct* info)
{
	int ret, i;
	u32 blk_size=0, blk_cnt=0;
	struct ufs_hba *hba = ufs_aio_get_host(UFS_DEFAULT_HOST_ID);

	memset(info, 0, sizeof(struct ufs_info_struct));

	// get type

	info->type = STORAGE_UFS;

	// get LU size

	ret = ufs_aio_read_unit_desc_cfg_param(hba);

	if (UFS_ERR_NONE != ret) {
		UFS_DBG_LOGD("[UFS] err: interface_get_ufs_info is failed\n");
		return STATUS_UFS_ERR;
	}

	for (i = 0; i < UFS_UPIU_MAX_GENERAL_LUN; i++) {
		if (hba->unit_desc_cfg_param[i].b_lu_enable) {
			ufs_aio_get_lu_size(hba, i, &blk_size, &blk_cnt);

			*((u64 *)&info->lu0_size + i) = (u64)blk_size * (u64)blk_cnt;
		}
	}

	info->block_size = blk_size;

	// get UFS product ID and fw ver.

	if (hba->dev_info.product_id[0] != 0) {
		strlcpy((char *)info->cid, hba->dev_info.product_id, MAX_PRODUCT_ID_LEN + 1);
		UFS_DBG_LOGE("[UFS] ufs id : %s\n", info->cid);
	}

	if (hba->dev_info.product_revision_level[0] != 0) {
		strlcpy((char *)info->fwver, hba->dev_info.product_revision_level, MAX_PRODUCT_REVISION_LEVEL_LEN + 1);
		UFS_DBG_LOGE("[UFS] fwver: %s\n", info->fwver);
	}

	// get UFS serial number
	if (hba->dev_info.serial_number_len != 0) {
		strlcpy((char *)info->sn, hba->dev_info.serial_number, hba->dev_info.serial_number_len * 2 + 1);
		UFS_DBG_LOGE("[UFS] sn: %s\n", info->sn);
	}

	// get UFS vendor id
	info->vendor_id = hba->dev_info.wmanufacturerid;
	UFS_DBG_LOGE("[UFS] vendor id: 0x%x\n", info->vendor_id);

	// get UFS health
	info->pre_eol_info = hba->dev_info.pre_eol_info;
	info->life_time_est_a = hba->dev_info.life_time_est_a;
	info->life_time_est_b = hba->dev_info.life_time_est_b;

	return STATUS_OK;
}

__WEAK extern status_t interface_ufs_device_ctrl(u32 ctrl_code, void* in, u32 in_len, void* out, u32 out_len, u32* ret_len)
{
	int ret = STATUS_OK;;
	struct ufs_hba *hba = ufs_aio_get_host(UFS_DEFAULT_HOST_ID);
	u64 otp_start_address = 0;

	switch (ctrl_code) {
		case STORAGE_CRCODE_GET_ACTIVE_BOOT_SECTION:
			/* deprecated */
			break;

		case STORAGE_CRCODE_OTP_LOCK:

			otp_start_address = *(u64*)in;

			UFS_DBG_LOGD("[UFS][%s] STORAGE_CRCODE_OTP_LOCK, otp_start: %llx (blk %d), otp_len: %d\n",
				__func__, otp_start_address, (u32)(otp_start_address / UFS_BLOCK_SIZE), in_len);

			#if !defined(UFS_CFG_OTP_FOR_SQC_UNLOCK)
			ret = ufs_aio_otp_lock(hba, otp_start_address);
			#else
			ret = ufs_aio_otp_unlock(hba, otp_start_address);
			#endif

			UFS_DBG_LOGD("[UFS][%s] STORAGE_CRCODE_OTP_LOCK, ret: %d\n", __func__, ret);

			ret = ufs_aio_err_trans(ret);
			break;

		case STORAGE_CRCODE_CHECK_OTP_LOCK_STATUS:
			otp_start_address = *(u64*)in;

			ret = ufs_aio_otp_get_lock_status(hba, otp_start_address);

			if (ret < 0) {
				 UFS_DBG_LOGE("[UFS][%s] ERROR: get lock status failed\n", __func__);
				 break;
			}

			UFS_DBG_LOGD("[UFS][%s] STORAGE_CRCODE_CHECK_OTP_LOCK_STATUS, ret: %d (1: locked, 0: unlocked)\n", __func__, ret);

			if (ret == 1)
				ret = STATUS_OTP_LOCKED;
			else
				ret = STATUS_OTP_UNLOCKED;

			break;

		case STORAGE_CRCODE_RECTIFY_OTP_ADDRESS:

#define ROUND_TO_BLOCK(x,y) (((x) + (y-1)) & (~(y-1)))
			otp_start_address = *(u64*)in;

			UFS_DBG_LOGD("[UFS][%s] STORAGE_CRCODE_RECTIFY_OTP_ADDRESS, otp_start: %llx (blk %d)\n",
							__func__, otp_start_address, (u32)(otp_start_address / UFS_BLOCK_SIZE));

			otp_start_address = ROUND_TO_BLOCK(otp_start_address, (u64)UFS_BLOCK_SIZE);

			*(u64*)out = otp_start_address ;

			UFS_DBG_LOGD("[UFS][%s] STORAGE_CRCODE_RECTIFY_OTP_ADDRESS, otp_start (aligned): %llx (blk %d)\n",
				__func__, otp_start_address, (u32)(otp_start_address / UFS_BLOCK_SIZE));

			if (ret_len != NULL)
				*ret_len = sizeof(u64);
			break;

		case STORAGE_CRCODE_DOWNLOAD_BL_PROCESS:
			// in DA, always set boot from LU A
			// this attribute may be changed by OTA(MOTA/FOTA) process in the future
			ret = ufs_aio_set_boot_lu(hba, ATTR_B_BOOT_LUN_EN_BOOT_LU_A);
			break;

		case STORAGE_CRCODE_STOR_LIFE_CYCLE_CHECK:
			if (hba->dev_info.pre_eol_info == 0x3
				|| hba->dev_info.life_time_est_a >= 0xa || hba->dev_info.life_time_est_b >= 0xa)
					ret = STATUS_STOR_LIFE_EXHAUST;
			else if (hba->dev_info.pre_eol_info == 0x2
				|| hba->dev_info.life_time_est_a == 0x9 || hba->dev_info.life_time_est_b == 0x9)
					ret = STATUS_STOR_LIFE_WARN;
			else if (hba->dev_info.pre_eol_info == 0x1 || (hba->dev_info.life_time_est_a >= 0x1 && hba->dev_info.life_time_est_a <= 0x8)
				|| (hba->dev_info.life_time_est_b >= 0x1 && hba->dev_info.life_time_est_b <= 0x8))
					ret = STATUS_OK;
			break;

		case STORAGE_CRCODE_FIELD_FIRMWARE_UPDATE:
			ret = hba->ffu_write(hba, in, in_len);
			if (ret == UFS_ERR_NONE) {
				UFS_DBG_LOGI("[UFS] ffu success, fw size=%d\n", in_len);
				/* Clear bootable to make sure device desc update and provision */
				hba->dev_info.bootable = 0;
			} else {
				ret = STATUS_UFS_ERR;
				UFS_DBG_LOGE("[UFS] ffu fail, fw size=%d\n", in_len);
			}
			break;

		/* For custom setting */
		case STORAGE_CRCODE_SET_UFS_FORCE_PROVISION:
			hba->custom_info.force_provision = TRUE;
			hba->custom_info.custom_flag |= CUSTOM_FORCE_PROVISION;
			break;

		case STORAGE_CRCODE_SET_UFS_TW_GB:
			hba->custom_info.tw_size_gb = *(u32*) in;;
			hba->custom_info.custom_flag |= CUSTOM_TW_GB;
			break;

		case STORAGE_CRCODE_SET_UFS_TW_NO_RED:
			hba->custom_info.tw_no_red = *(u32*) in;;
			hba->custom_info.custom_flag |= CUSTOM_TW_NO_RED;
			break;

		case STORAGE_CRCODE_SET_UFS_HPB_REGION_COUNT:
			hba->custom_info.hpb_size_gb = *(u32*) in;
			hba->custom_info.custom_flag |= CUSTOM_HPB_REGION_COUNT;
			break;

		default:
			UFS_DBG_LOGI("[UFS] unsupport ctrl_code=%d", ctrl_code);
			ret = STATUS_UNSUPPORT_OP;
			break;
	}

	return ret;
}

#endif


#if defined(MTK_UFS_DRV_CTP)

__WEAK int ufs_ctp_write(u32 blknr, u32 blkcnt, u8 *buf, u8 lu)
{
	int ret;

	ret = ufs_aio_block_write(UFS_DEFAULT_HOST_ID, lu, blknr, blkcnt, (unsigned long *)buf);

	return ret;
}

__WEAK int ufs_ctp_read(u32 blknr, u32 blkcnt, u8 *buf, u8 lu)
{
	int ret;

	ret = ufs_aio_block_read(UFS_DEFAULT_HOST_ID, lu, blknr, blkcnt, (unsigned long *)buf);

	return ret;
}

__WEAK int ufs_ctp_init(void)
{
	int ret;

	UFS_DBG_LOGI("[UFS] info: ufs init start\n");

	ufs_aio_cfg_mode(UFS_DEFAULT_HOST_ID, UFS_MODE_DEFAULT);

	ret = ufshcd_init();

	if (UFS_ERR_NONE != ret) {
		UFS_DBG_LOGI("[UFS] err: ufshcd_init failed, err: %d\n", ret);
		goto out;
	}

	if (ret) {
		UFS_DBG_LOGI("[UFS] err: blkdev_register failed, err: %d\n", ret);
		goto out;
	}

	UFS_DBG_LOGI("[UFS] info: ufs init ok\n");

out:
	return ret;
}

int ufs_switch_part(u32 part_id)
{
	u8 lun;

	switch (part_id) {
		case UFS_LU_BOOT1:
			lun = UFS_LU_0;
			break;
		case UFS_LU_BOOT2:
			lun = UFS_LU_1;
			break;
		case UFS_LU_USER:
			lun = UFS_LU_2;
			break;
		default:
			UFS_DBG_LOGD("[UFS] err: ufs_switch_part, invalid UFS LU %d\n", part_id);
			return UFS_ERR_INVALID_LU;
	}

	UFS_DBG_LOGD("[UFS] switch to part %d, lun %d\n", part_id, lun);

	return (int)lun;
}

/* Exported API for read storage device initialization */
void storage_init(void)
{
	return ufs_ctp_init();
}

/* Exported API for read user partition */
int storage_read(u32 blk_start, u32 blk_cnt, void *buf)
{
	int lun;

	if ((unsigned long)buf % 4) {
		UFS_DBG_LOGE("[UFS] ERROR: %s: buf 0x%x shall be 4-byte aligned\n", __func__, (unsigned long *)buf);
		return UFS_ERR_INVALID_ALIGNMENT;
	}

	UFS_DBG_LOGV("[UFS] storage_ctp_read: r, %d, %d, 0x%x\n", __func__, blk_start, blk_cnt, (unsigned long *)buf);

	lun = ufs_switch_part(UFS_LU_USER);

	if (lun < 0)
		return lun;

	return ufs_ctp_read(blk_start, blk_cnt, buf, lun);
}

/* Exported API for write user partition */
int storage_write(u32 blk_start, u32 blk_cnt, void *buf)
{
	int lun;

	if ((unsigned long)buf % 4) {
		UFS_DBG_LOGE("[UFS] ERROR: %s: buf 0x%x shall be 4-byte aligned\n", __func__, (unsigned long *)buf);
		return UFS_ERR_INVALID_ALIGNMENT;
	}

	UFS_DBG_LOGV("[UFS] %s: r, %d, %d, 0x%x\n", __func__, blk_start, blk_cnt, (unsigned long *)buf);

	lun = ufs_switch_part(UFS_LU_USER);

	if (lun < 0)
		return lun;

	return ufs_ctp_write(blk_start, blk_cnt, buf, lun);
}

#endif

