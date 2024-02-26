/* This software/firmware and related documentation ("MediaTek Software") are
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
#include <platform.h>
#include "partition.h"
#include "log_store_pl.h"
#include "dram_buffer.h"

#define MOD "PL_LOG_STORE"
#define AEE_IPANIC_PLABLE "expdb"

//#define   PRINT_EARLY_KERNEL_LOG

#define DEBUG_LOG

// !!!!!!! Because log store be called by print, so these function don't use print log to debug.
#ifdef DEBUG_LOG
#define LOG_DEBUG(fmt, ...) \
    log_store_enable = false; \
    print(fmt, ##__VA_ARGS__); \
    log_store_enable = true
#else
#define LOG_DEBUG(fmt, ...)
#endif

#define LOG_MEM_ALIGNMENT (0x1000)  //PAGE Alignment(4K)
#define EMMC_LOG_BUF_SIZE (0x200000)

typedef enum {
	LOG_WRITE = 0x1,        // Log is write to buff
	LOG_READ_KERNEL = 0x2,  // Log have readed by kernel
	LOG_WRITE_EMMC = 0x4,   // log need save to emmc
	LOG_EMPTY = 0x8,        // log is empty
	LOG_FULL = 0x10,        // log is full
	LOG_PL_FINISH = 0X20,   // pl boot up finish
	LOG_LK_FINISH = 0X40,   // lk boot up finish
	LOG_DEFAULT = LOG_WRITE_EMMC|LOG_EMPTY,
} BLOG_FLAG;


//printk log store buff, 1 DRAM, 0 SRAM. only printk user DRAM,
// we can user printk SRAM buff to store log.
extern int  g_log_drambuf;
#define C_LOG_SRAM_BUF_SIZE (20480)
extern char log_sram_buf[C_LOG_SRAM_BUF_SIZE];
#define bootarg g_dram_buf->bootarg


static int log_store_status = BUFF_NOT_READY;
static struct sram_log_header *sram_header = (struct sram_log_header*)SRAM_LOG_ADDR;
static struct pl_lk_log *dram_curlog_header = &(((struct sram_log_header*)SRAM_LOG_ADDR)->dram_curlog_header);
static struct dram_buf_header *sram_dram_buff = &(((struct sram_log_header*)SRAM_LOG_ADDR)->dram_buf);
static char *pbuff = NULL;
static int log_store_sram = 1;
static int sram_store_count = 0;
static bool log_store_enable = true;
static u32 part_start_addr, part_end_addr, part_size;
static blkdev_t *bootdev;
static part_t *part_ptr;



void format_log_buff(void)
{
	memset(dram_curlog_header, 0, sizeof(struct pl_lk_log));
	dram_curlog_header->sig = LOG_STORE_SIG;
	dram_curlog_header->buff_size = LOG_STORE_SIZE;
	dram_curlog_header->off_pl = sizeof(struct pl_lk_log);
	dram_curlog_header->off_lk = sizeof(struct pl_lk_log);
	dram_curlog_header->pl_flag = LOG_DEFAULT;
	dram_curlog_header->lk_flag = LOG_DEFAULT;

	return;
}

bool trigger_once = true;
int logbuf_valid(void)
{
	if ((dram_curlog_header != NULL) && (dram_curlog_header->sig == LOG_STORE_SIG) &&
		(dram_curlog_header->buff_size == LOG_STORE_SIZE) && (dram_curlog_header->off_pl == sizeof(struct pl_lk_log))
		&& ((dram_curlog_header->sz_lk + dram_curlog_header->sz_pl + dram_curlog_header->off_pl) <= LOG_STORE_SIZE)
		&& (dram_curlog_header->pl_flag <= 0x80 - 1) && (dram_curlog_header->lk_flag <= 0x80 - 1))
		return 1;

	if (trigger_once && dram_curlog_header) {
		LOG_DEBUG("%s: logbuf_valid failed\n", MOD);
		LOG_DEBUG("%s: dram_curlog_header 0x%x sig 0x%x buff_size 0x%x\n", MOD, dram_curlog_header, dram_curlog_header->sig, dram_curlog_header->buff_size);
		LOG_DEBUG("%s: off_pl 0x%x sz_pl 0x%x pl_flag 0x%x\n", MOD, dram_curlog_header->off_pl, dram_curlog_header->sz_pl, dram_curlog_header->pl_flag);
		LOG_DEBUG("%s: off_lk 0x%x sz_lk 0x%x lk_flag 0x%x\n", MOD, dram_curlog_header->off_lk, dram_curlog_header->sz_lk, dram_curlog_header->lk_flag);
		trigger_once = false;
	}

	return 0;
}

bool read_config_emmc(struct log_emmc_header *buf) {
	if (bootdev == NULL || 	part_ptr == NULL) {
		LOG_DEBUG("%s: partition information is error.\n", MOD);
		return false;
	}

	int ret = 0;

	log_store_enable = false;
	ret = blkdev_read(bootdev, part_end_addr - bootdev->blksz, sizeof(struct log_emmc_header), (u8 *)buf, part_ptr->part_id);
	log_store_enable = true;

	if (ret) {
		LOG_DEBUG("%s:read postfix error, ret value = 0x%x\n", MOD, ret);
		return false;
	}

	if (buf->sig != LOG_EMMC_SIG) { //sig error, format it
		LOG_DEBUG("%s:read config error, sig 0x%x, size 0x%d, format them.\n", MOD, buf->sig, bootdev->blksz);
		memset(buf, 0, sizeof(struct log_emmc_header));
		buf->sig = LOG_EMMC_SIG;
		log_store_enable = false;
		ret = blkdev_write(bootdev, part_end_addr - bootdev->blksz, sizeof(struct log_emmc_header), (u8 *)buf, part_ptr->part_id);
		log_store_enable = true;
		if (ret)
			return false;
	}
	return true;
}

bool write_config_emmc(u32 type, u32 value) {
	if (bootdev == NULL || 	part_ptr == NULL) {
		LOG_DEBUG("%s: write partition information is error.\n", MOD);
		return false;
	}

	struct log_emmc_header log_header;
	int ret = 0;

	if (read_config_emmc(&log_header)) {
		if (type == 0) { // can add other item
			log_header.offset = value;
		}

		log_store_enable = false;
		ret = blkdev_write(bootdev, part_end_addr - bootdev->blksz, sizeof(log_header), (u8 *)&log_header, part_ptr->part_id);
		log_store_enable = true;
		if (ret)
			return false;
	}
	return true;
}

#ifndef DRAM_PHY_ADDR
#define DRAM_PHY_ADDR (0x40000000)
#endif
u32 store_emmc(char * buf, u32 store_size, u32 offset) {
	int ret = 0;
	u32 emmc_remain_buf_size = 0;

	if ((unsigned long)buf < (unsigned long)DRAM_PHY_ADDR)
		return 0;

	if (offset % bootdev->blksz)
		offset = (offset / bootdev->blksz + 1) * bootdev->blksz;

	if (offset >= EMMC_LOG_BUF_SIZE - bootdev->blksz)
		offset = 0;

	if (store_size > EMMC_LOG_BUF_SIZE/4) {
		/* store size max 0.25 emm log, now is 512K */
		buf = buf + (store_size - EMMC_LOG_BUF_SIZE/4);
		store_size = EMMC_LOG_BUF_SIZE/4;
	}

	emmc_remain_buf_size = EMMC_LOG_BUF_SIZE - bootdev->blksz - offset;

	if (store_size > emmc_remain_buf_size) {
		log_store_enable = false;	// write remain space
		ret = blkdev_write(bootdev, part_end_addr - EMMC_LOG_BUF_SIZE + offset, emmc_remain_buf_size, buf, part_ptr->part_id);
		log_store_enable = true;
		if (ret) {
			LOG_DEBUG("%s:write log to emmc error, ret value = 0x%x\n", MOD, ret);
			return offset;
		}
		log_store_enable = false; // write from begain with remain data
		ret = blkdev_write(bootdev, part_end_addr - EMMC_LOG_BUF_SIZE, store_size - emmc_remain_buf_size, buf + emmc_remain_buf_size, part_ptr->part_id);
		log_store_enable = true;
		if (ret) {
			LOG_DEBUG("%s:write log to emmc error, ret value = 0x%x\n", MOD, ret);
			return 0;
		}
		offset = store_size - emmc_remain_buf_size;
	} else {
		log_store_enable = false;
		ret = blkdev_write(bootdev, part_end_addr - EMMC_LOG_BUF_SIZE + offset, store_size, buf, part_ptr->part_id);
		log_store_enable = true;
		if (ret) {
			LOG_DEBUG("%s:write log to emmc error, ret value = 0x%x\n", MOD, ret);
			return offset;
		}
		offset = offset + store_size;
	}

	if (offset % bootdev->blksz)
			offset = (offset / bootdev->blksz + 1) * bootdev->blksz;

	return offset;
}

// store log buff to emmc
void log_to_emmc(struct sram_log_header *sram_buff)
{
	int ret = 0;
	u32 atf_end = 0;
#if CFG_GZ_SUPPORT
	u32 gz_end = 0;
#endif

	LOG_DEBUG("%s:log_to_emmc function flag 0x%x!\n",MOD, sram_dram_buff->flag);

	if (bootdev == NULL ||	part_ptr == NULL) {
		LOG_DEBUG("%s: write partition information is error.\n", MOD);
		return;
	}

	if (sram_buff == NULL) {
		LOG_DEBUG("%s:sram_buff is null!\n",MOD);
		return;
	}
	if (sram_dram_buff == NULL) {
		LOG_DEBUG("%s:sram_dram_buff is NULL!\n",MOD);
		return;
	}

	if (sram_dram_buff->sig != DRAM_HEADER_SIG) {
		LOG_DEBUG("%s:sram_dram_buff->sig 0x%x!\n",MOD,sram_dram_buff->sig);
		return;
	}

	if ((u32 *)sram_dram_buff->buf_addr == NULL) {
		LOG_DEBUG("%s:sram_dram_buff->buf_addr is NULL!\n",MOD);
		return;
	}

	if (sram_dram_buff->buf_size != LOG_STORE_SIZE) {
		LOG_DEBUG("%s:error:sram_dram_buff->buf_size 0x%x not 0x%x!\n",MOD,sram_dram_buff->buf_size, LOG_STORE_SIZE);
		return;
	}

	if ((sram_dram_buff->flag & NEED_SAVE_TO_EMMC) != NEED_SAVE_TO_EMMC) {
		LOG_DEBUG("%s:don't need to store to emmc, flag 0x%x!\n",MOD,sram_dram_buff->flag);
		return;
	}
	sram_buff->reboot_count++;
	pbuff = (char *)sram_dram_buff->buf_addr;
	dram_curlog_header = &(sram_header->dram_curlog_header);

	if (!logbuf_valid()) {
		LOG_DEBUG("%s:logbuf not valid!\n", MOD);
		return;
	}

	//save the top 3 times, after that, 10 times scale
	if (sram_buff->reboot_count >= sram_buff->save_to_emmc) {
		u32 ret;
		struct log_emmc_header header;
		u32 write_size;

		if (read_config_emmc(&header) == false)
			return;

		LOG_DEBUG("%s:last pl log size 0x%x, lk log size 0x%x!\n", MOD, dram_curlog_header->sz_pl, dram_curlog_header->sz_lk);

		/* write preloader/lk log */
		LOG_DEBUG("%s: /* write preloader/lk log addr 0x%x size 0x%x, offset 0x%x*/\n", MOD,
		pbuff + dram_curlog_header->off_pl, dram_curlog_header->sz_lk + dram_curlog_header->sz_pl, header.offset);
		write_size = dram_curlog_header->sz_lk + dram_curlog_header->sz_pl;
		if (write_size % bootdev->blksz) // write as block
			write_size = (write_size / bootdev->blksz + 1) * bootdev->blksz;
		header.offset = store_emmc(pbuff + dram_curlog_header->off_pl, write_size, header.offset);

		/* write ATF log */
		if (sram_dram_buff->atf_log_addr != 0 && sram_dram_buff->atf_log_len != 0) {
			atf_end = sram_dram_buff->atf_log_addr;
			while (*(u64 *)atf_end != 0) {
                                atf_end += bootdev->blksz;
				if (atf_end > (sram_dram_buff->atf_log_addr
					+ sram_dram_buff->atf_log_len)) {
					atf_end -= bootdev->blksz;
					break;
				}
			}
			LOG_DEBUG("%s: /* write ATF log addr 0x%x size 0x%x, offset 0x%x*/\n",MOD,
				sram_dram_buff->atf_log_addr, atf_end - sram_dram_buff->atf_log_addr, header.offset);
			header.offset = store_emmc((void*)sram_dram_buff->atf_log_addr, atf_end - sram_dram_buff->atf_log_addr, header.offset);
		}

#if CFG_GZ_SUPPORT
		/* write GZ log */
		if (sram_header->gz_log_addr && sram_header->gz_log_len) {
			gz_end = sram_header->gz_log_addr;
			while (*(u64 *)gz_end != 0) {
				gz_end += bootdev->blksz;
				if (gz_end > (sram_header->gz_log_addr
					+ sram_header->gz_log_len)) {
					gz_end -= bootdev->blksz;
					break;
				}
			}
			LOG_DEBUG("%s: /* write GZ log addr 0x%x size 0x%x, offset 0x%x*/\n",MOD,
				sram_header->gz_log_addr, gz_end - sram_header->gz_log_addr, header.offset);
			header.offset = store_emmc((void*)sram_header->gz_log_addr, gz_end - sram_header->gz_log_addr, header.offset);
		}
#endif

		/* write kernel log, kernel size by block */
		if ((sram_dram_buff->flag & BUFF_EARLY_PRINTK) && sram_dram_buff->klog_addr != 0
			&& sram_dram_buff->klog_size != 0) {
			LOG_DEBUG("%s: /* write kernel log addr 0x%x size 0x%x, offset 0x%x*/\n",MOD,
				header.offset, sram_dram_buff->klog_size, header.offset);
			header.offset = store_emmc((void*)sram_dram_buff->klog_addr, sram_dram_buff->klog_size, header.offset);
		}

		write_config_emmc(0, header.offset);

		if (sram_buff->reboot_count >= 3)
			sram_buff->save_to_emmc = 5 * sram_buff->reboot_count;
	} else {
		LOG_DEBUG("%s:reboot_count %d,save_to_emmc %d.\n", MOD, sram_buff->reboot_count, sram_buff->save_to_emmc);
	}
	return;
}

#ifdef PRINT_EARLY_KERNEL_LOG
void kernel_log_show(void)
{
	int i = 0;
	char value;
	if (sram_dram_buff == NULL)
		return;

	// print early printk message
	if ((sram_dram_buff->flag & BUFF_EARLY_PRINTK) && sram_dram_buff->klog_addr != 0
	        && sram_dram_buff->klog_size != 0) {
		log_store_enable = false;
		for (i=0; i < sram_dram_buff->klog_size; i++) {
			value = *((char *)sram_dram_buff->klog_addr+i);
			print("%c",value);
		}
		log_store_enable = true;
	}

}
#endif

void get_emmc_add(void) {


	log_store_enable = false;
	bootdev = blkdev_get(CFG_BOOT_DEV);
	log_store_enable = true;
	if (NULL == bootdev) {
		LOG_DEBUG("%s can't find boot device(%d)\n", MOD, CFG_BOOT_DEV);
		return ;
	}

	log_store_enable = false;
	part_ptr = (part_t*)part_get(AEE_IPANIC_PLABLE);
	log_store_enable = true;
	if (part_ptr == NULL) {
		LOG_DEBUG("%s:log_to_emmc get partition error!\n",MOD);
		return;
	}

#if ((CFG_BOOT_DEV == BOOTDEV_SDMMC) || (CFG_BOOT_DEV == BOOTDEV_UFS))
	part_start_addr = part_ptr->start_sect * bootdev->blksz;
	part_end_addr = (part_ptr->start_sect + part_ptr->nr_sects) * bootdev->blksz;
	part_size = part_ptr->nr_sects * bootdev->blksz;
	LOG_DEBUG("%s:%s partition start addr 0x%x, end addr 0x%x, partition size 0x%x, nr_sects 0x%x, blksz 0x%x!\n",
		MOD, AEE_IPANIC_PLABLE, part_start_addr, part_end_addr, part_size, part_ptr->nr_sects, bootdev->blksz);
#else
	part_start_addr = mt_part_get_start_addr(part_ptr) * bootdev->blksz;
	part_end_addr = (mt_part_get_start_addr(part_ptr)+ mt_part_get_size(part_ptr)) * bootdev->blksz;
	part_size = mt_part_get_size(part_ptr) * bootdev->blksz;
	LOG_DEBUG("%s:%s partition start addr 0x%x, end addr 0x%x, partition size 0x%x, nr_sects 0x%x, blksz 0x%x!\n",
		MOD, AEE_IPANIC_PLABLE, part_start_addr, part_end_addr, part_size, mt_part_get_size(part_ptr), bootdev->blksz);
#endif

}

#if CFG_UART_DYNAMIC_SWITCH
void update_uart_log_flag(void)
{
	struct log_emmc_header header;

	if (read_config_emmc(&header) && header.uart_flag == 0x01) {
		set_log_switch(1);
	}
	LOG_DEBUG("%s:get uart flag= 0x%x\n", MOD, header.uart_flag);
}
#endif

void log_store_init(void)
{
	if (log_store_status != BUFF_NOT_READY) {
		LOG_DEBUG("%s:log_sotore_status is ready!\n",MOD);
		return;
	}
// SRAM buff header init
	sram_header = (struct sram_log_header*)SRAM_LOG_ADDR;
	LOG_DEBUG("%s:sram->sig value 0x%x!\n",MOD,sram_header->sig);
	if (sram_header->sig != SRAM_HEADER_SIG) {
		memset(sram_header,0, sizeof(struct sram_log_header));
		LOG_DEBUG("%s:sram header is not match, format all!\n",MOD);
		sram_header->sig = SRAM_HEADER_SIG;
		LOG_DEBUG("%s:set ram_header->sig = 0x%x\n", MOD, sram_header->sig);
	}

	get_emmc_add();
#if CFG_UART_DYNAMIC_SWITCH
	update_uart_log_flag();
#endif
	// Save log to emmc
	log_to_emmc(sram_header);

#ifdef PRINT_EARLY_KERNEL_LOG
	kernel_log_show();
#endif
	memset(sram_dram_buff, 0, sizeof(struct dram_buf_header));
	sram_dram_buff->sig = DRAM_HEADER_SIG;

	log_store_enable = false;
	pbuff = (char *)((u32)mblock_reserve_ext(&bootarg.mblock_info,
	                            (u64)(LOG_STORE_SIZE), (u64)LOG_MEM_ALIGNMENT, 0x80000000, 1, "log_store"));
	log_store_enable = true;

	if (!pbuff) {
		LOG_DEBUG("%s:dram log allocation error!\n",MOD);
		sram_dram_buff->flag = BUFF_ALLOC_ERROR;
		log_store_status = BUFF_ALLOC_ERROR;
		return;
	}

	memset(pbuff, 0, LOG_STORE_SIZE);
	sram_dram_buff->buf_addr = (u32)pbuff;
	sram_dram_buff->buf_offsize = sizeof(struct pl_lk_log);
	sram_dram_buff->buf_size = LOG_STORE_SIZE;
	sram_dram_buff->flag = BUFF_VALID | CAN_FREE | NEED_SAVE_TO_EMMC | ARRAY_BUFF;
	sram_dram_buff->buf_point = 0;

	// init DRAM buff
	format_log_buff();
	log_store_status = BUFF_READY;

	LOG_DEBUG("%s:sram_header 0x%x,sig 0x%x, sram_dram_buff 0x%x, buf_addr 0x%x\n", MOD,\
		sram_header, sram_header->sig, sram_dram_buff, sram_dram_buff->buf_addr);
	return;
}

void store_switch_to_dram(void)
{
	int i=0;
	log_store_sram = 0;
	log_store_init();
	if (g_log_drambuf == 1) {
		for (i=0; i < sram_store_count; i++) {
			pl_log_store(log_sram_buf[i]);
		}
	}
}

void pl_log_store(char c)
{
	if (log_store_enable == false) {
		return;
	}

	if ((log_store_status == BUFF_ALLOC_ERROR) || (log_store_status == BUFF_FULL)) {
		return;
	}

	if ((log_store_sram ==1) && (g_log_drambuf == 1)) {
		if (sram_store_count < C_LOG_SRAM_BUF_SIZE) {
			log_sram_buf[sram_store_count++] = c;
		}
		return;
	}

	if (log_store_status == BUFF_NOT_READY) {
		log_store_init();
		return;
	}

	if (logbuf_valid() == 0) {
		return;
	}

	if (log_store_status != BUFF_READY) {
		return;
	}

	*(pbuff + dram_curlog_header->off_pl + dram_curlog_header->sz_pl) = c;
	dram_curlog_header->sz_pl++; // dram_curlog_header->sz_pl++;
	sram_dram_buff->buf_point = dram_curlog_header->sz_pl; // dram_curlog_header->sz_pl;
	if ((dram_curlog_header->off_pl + dram_curlog_header->sz_pl) >= LOG_STORE_SIZE) {
		log_store_status = BUFF_FULL;
		LOG_DEBUG("%s: dram buff full", MOD);
	}

	return;
}
