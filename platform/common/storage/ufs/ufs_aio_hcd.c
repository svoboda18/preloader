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
#include "ufs_aio_compiler.h"
#include "ufs_aio_platform.h"
#include "ufs_aio.h"
#include "ufs_aio_utils.h"
#include "ufs_aio_hcd.h"
#include "ufs_aio_core.h"
#include "ufs_aio_quirks.h"
#include "ufs_aio_unipro.h"
#include "ufs_aio_error.h"

#if defined(MTK_UFS_DRV_PRELOADER)
#if CFG_ENABLE_DCACHE
#include "platform.h"
#endif
#endif

#if defined(MTK_UFS_DRV_DA)
#include <arch/ops.h>                   /* for cache maintanance APIs */
#include "dev/gpt_timer/gpt_timer.h"    /* for gpt4_time2tick_ms APIs */
#include <kernel/event.h>               /* for event_wait_timeout APIs */
#include "boot/system_objects.h"        /* for sysob_runtime_params and INT_MODE_ENABLE */
#include <dev/irq/irq.h>                /* for mt_irq_set_sens APIs */
#include <dev/irq/gic.h>		/* for mt_irq_ack APIs */
#endif

#if defined(MTK_UFS_DRV_LK)
#include <arch/ops.h>   /* for cache maintanance APIs */
#include <kernel/event.h>
#include <mt_gic.h>
#include <profiling.h>
#include <sync_write.h>
#include <mtk_secure_api.h>

/* used for secure call */
#define MTK_SIP_BL_UFS_CONTROL_AARCH32		0x8200040D
#endif

#if defined(MTK_UFS_DRV_DA) || defined(MTK_UFS_DRV_LK)
static event_t ufshcd_intr_event;
static u32 ufshcd_intr_status;
#endif

#if defined(MTK_UFS_DRV_CTP)
#include <common.h>
#include <api.h>           /* for cache maintanance APIs (cache_api.h) */
#include <kernel_to_ctp.h> /* for delay functions */

u8 ufs_crypto_rw_en = 0;
#endif

#if !defined(__WEAK)
#define __WEAK __attribute__((weak))
#endif

struct ufs_hba g_ufs_hba = {0};
struct ufs_platform_info ufs_platform = {0};
static struct utp_transfer_cmd_desc ufs_ucdl[UFS_AIO_MAX_NUTRS] __attribute__((aligned(UFS_ALIGN_UCD)));
static struct utp_transfer_req_desc ufs_utrdl[UTRD_NUTRS] __attribute__((aligned(UFS_ALIGN_UTRD)));
static struct ufshcd_lrb ufs_lrb[UFS_AIO_MAX_NUTRS];

unsigned char ufs_req_upiu[UPIU_CMD_BUF_SIZE];  /* for PIO only */
unsigned char ufs_resp_upiu[UPIU_CMD_BUF_SIZE]; /* for PIO only */

/* static struct utp_task_req_desc ufs_utmrdl[UFS_AIO_MAX_NUTMRS] __ALIGNED(UFS_ALIGN_UTMRD); */
/* static u32 ufs_sense_buf[(UFS_AIO_MAX_NUTRS * SCSI_SENSE_BUFFERSIZE) / sizeof(u32)]; */

/**
 * g_ufs_temp_buf, for
 * 1. ufshcd_read_desc_param: read partial descriptor case. Size limit: UFS_TEMP_BUF_SIZE (256 bytes)
 * 2. ufshcd_read_string_desc: used for destination buffer of utf16s_to_utf8s
 */
unsigned char g_ufs_temp_buf[UFS_TEMP_BUF_SIZE] __attribute__((aligned(UFS_PLATFORM_CACHE_LINE_SIZE)));

static u32 ufs_query_desc_max_size[] = {
	QUERY_DESC_DEVICE_MAX_SIZE,
	QUERY_DESC_CONFIGURAION_MAX_SIZE_HPB_20,
	QUERY_DESC_UNIT_MAX_SIZE,
	QUERY_DESC_RFU_MAX_SIZE,
	QUERY_DESC_INTERCONNECT_MAX_SIZE,
	QUERY_DESC_STRING_MAX_SIZE,
	QUERY_DESC_RFU_MAX_SIZE,
	QUERY_DESC_GEOMETRY_MAX_SIZE,
	QUERY_DESC_POWER_MAX_SIZE,
	QUERY_DESC_HEALTH_MAX_SIZE,
	QUERY_DESC_RFU_MAX_SIZE,
};

int         ufshcd_dme_get_attr(struct ufs_hba *hba, u32 attr_sel, u32 *mib_val, u8 peer);
int         ufshcd_dme_set_attr(struct ufs_hba *hba, u32 attr_sel, u8 attr_set, u32 mib_val, u8 peer);
int         ufshcd_wait_command(struct ufs_hba *hba, struct ufshcd_lrb *lrbp, u32 timeout_ms);
static void ufs_aio_reset_device(struct ufs_hba *hba);
static void ufs_aio_advertise_hci_quirks(struct ufs_hba *hba);
static int  ufs_aio_bootrom_deputy(struct ufs_hba *hba);
static void ufs_aio_dma_map(ufs_vaddr_t buf, ufs_size_t len, enum dma_data_direction dir);
static void ufs_aio_dma_unmap(ufs_vaddr_t buf, ufs_size_t len, enum dma_data_direction dir);
int         ufs_aio_pio_read_flag(struct ufs_hba *hba, unsigned char flag_idx, bool *value);
int         ufs_aio_pio_set_flag(struct ufs_hba *hba, unsigned char flag_idx);
static int  ufs_aio_post_link(struct ufs_hba *hba);
static int  ufs_aio_pre_link(struct ufs_hba *hba);
static int  ufs_aio_pre_pwr_change(struct ufs_hba *hba, struct ufs_pa_layer_attr *desired, struct ufs_pa_layer_attr *final);
static int  ufs_aio_prepare_new_ufs(struct ufs_hba *hba);
static int  ufs_aio_test_unit_ready_all_device(struct ufs_hba *hba);
static void ufshcd_init_desc_sizes(struct ufs_hba *hba);
static int  ufshcd_init_vendor_desc_sizes(struct ufs_hba *hba);

#if defined(MTK_UFS_DRV_DA) || defined(MTK_UFS_DRV_LK)
extern void arch_clean_invalidate_cache_range(ufs_vaddr_t start, ufs_size_t len);
extern void arch_sync_cache_range(ufs_vaddr_t start, ufs_size_t len);

void ufshcd_irq_handler(unsigned int irq)
{
	struct ufs_hba *hba = &g_ufs_hba;
	struct ufshcd_lrb *lrbp;
	u32 tag_mask;
	u32 tr_doorbell;

	tr_doorbell = ufshcd_readl(hba, REG_UTP_TRANSFER_REQ_DOOR_BELL);
	lrbp = &hba->lrb[0];
	tag_mask = 1 << lrbp->task_tag;

	if (!(tag_mask & tr_doorbell)) {
		/* Clear_bit(lrbp->task_tag, &hba->outstanding_reqs); */
		hba->outstanding_reqs &= ~(tag_mask);

		/* Save interrupt status and clear it */
		ufshcd_intr_status = ufs_aio_readl(hba, REG_INTERRUPT_STATUS);
		ufshcd_writel(hba, ufshcd_intr_status, REG_INTERRUPT_STATUS);

		/* dsb make sure clear initerrupt status */
		dsb();
		/* Wakeup ufshcd_wait_command */
		event_signal(&ufshcd_intr_event, 0);
	}

	/* Let irq handler can enter again, must put in irq handler bottom */
	mt_irq_ack(irq);
}

__WEAK int ufs_get_irq_id(void)
{
	return 0xFFFFFFFF;
}

static int ufshcd_irq_init(struct ufs_hba *hba)
{
	hba->irq = ufs_get_irq_id();
	if (hba->irq == 0xFFFFFFFF)
		return -1;

	/* Set ufs interrupt level, polarity */
	mt_irq_set_sens(hba->irq, MT65xx_LEVEL_SENSITIVE);
	mt_irq_set_polarity(hba->irq, MT65xx_POLARITY_LOW);

	/* Init event for thread function ufshcd_wait_command sleep/wakeup */
	event_init(&ufshcd_intr_event, false, EVENT_FLAG_AUTOUNSIGNAL);

	/*
	 * Mask all interrupt first, only unmask before thread sleep and wait.
	 * Else there may have un-wanted interrupt come in, which is need by
	 * polling function, like UIC_COMMAND_COMPL
	 */
	mt_irq_mask(hba->irq);

	return 0;
}
#endif

/* Platform different need implement it's ufs_get_platform_data */
__WEAK void ufs_get_platform_data(void)
{
	ufs_platform.hci_base = (void*) 0x11270000;
	ufs_platform.pericfg_base = (void *)0x10003000;
	ufs_platform.mphy_base = (void *)0x11FA0000;

	ufs_platform.reg_ufs_pericfg 		= 0x448;
	ufs_platform.reg_ufs_pericfg_rst_n_bit 	= 3;
	ufs_platform.reg_ufs_pericfg_ldo_n_bit 	= 0xF;
	ufs_platform.reg_ufs_pericfg_lp_n_bit	= 0xF;
}

static inline void ufshcd_get_host_capabilities(struct ufs_hba *hba)
{
	/* for AIO driver, only 1 task is enough */
	hba->nutrs = UFS_AIO_MAX_NUTRS;
	/* hba->nutmrs = UFS_AIO_MAX_NUTMRS; */
}

void ufshcd_power(struct ufs_hba *hba, bool on)
{

}

void ufshcd_clock(struct ufs_hba *hba, u8 on)
{

}

/**
 * ufshcd_memory_alloc - allocate memory for hba memory space data structures
 * @hba: per adapter instance
 *
 * 1. Allocate DMA memory for Command Descriptor array
 *    Each command descriptor consist of Command UPIU, Response UPIU and PRDT
 * 2. Allocate DMA memory for UTP Transfer Request Descriptor List (UTRDL).
 * 3. Allocate DMA memory for UTP Task Management Request Descriptor List
 *    (UTMRDL)
 * 4. Allocate memory for local reference block(lrb).
 *
 * Returns 0 for success, non-zero in case of failure
 */
static int ufshcd_memory_alloc(struct ufs_hba *hba)
{
/* u32 i; */
/* u8 *ptr; */

	/* Allocate memory for UTP command descriptors */
	hba->ucdl_base_addr = (struct utp_transfer_cmd_desc *)&ufs_ucdl[0];
	hba->ucdl_dma_addr = (ufs_paddr_t)hba->ucdl_base_addr;

	if (hba->ucdl_dma_addr & (UFS_ALIGN_UCD - 1)) {
		UFS_DBG_LOGD("memory alignment failed: ucdl, addr 0x%x, shall align %d\n", (unsigned int)hba->ucdl_dma_addr, UFS_ALIGN_UCD);
		goto out;
	}

	/*
	 * Allocate memory for UTP Transfer descriptors
	 * UFSHCI requires 1024 byte alignment of UTRD
	 */
	hba->utrdl_base_addr = (struct utp_transfer_req_desc *)&ufs_utrdl[0];
	hba->utrdl_dma_addr = (ufs_paddr_t)hba->utrdl_base_addr;

	if (hba->utrdl_dma_addr & (UFS_ALIGN_UTRD - 1)) {
		UFS_DBG_LOGD("memory alignment failed: utrdl, addr 0x%x, shall align %d\n", (unsigned int)hba->utrdl_dma_addr, UFS_ALIGN_UTRD);
		goto out;
	}

	/*
	 * Note. UFSHCI 2.1 requires 1024-byte alignment for UTRD (UTP Transfer Request Descriptor)
	 *
	 * SW tricks:
	 *  1. Fill-in UFS_HCI_REG_UTRLBA a 1024-byte alignment address to assume the 1st UTRD entry (task ID = 0) is in that address.
	 *  2. Since every UTRD entry is fixed to 32 bytes, now we can use our real UTRD base address by using "its" task ID.
	 */

	hba->active_tr_tag = (u8)(((unsigned long)hba->utrdl_base_addr & UFS_1KB_MASK) / sizeof(struct utp_transfer_req_desc));

#if 0
	/*
	 * Allocate memory for UTP Task Management descriptors
	 * UFSHCI requires 1024 byte alignment of UTMRD
	 */

	hba->utmrdl_base_addr = (utp_task_req_desc *)&ufs_utmrdl[0];
	hba->utmrdl_dma_addr = (ufs_paddr_t)hba->utmrdl_base_addr;

	if (hba->utmrdl_dma_addr & (UFS_ALIGN_UTMRD - 1)) {
		UFS_DBG_LOGD("memory alignment failed: utmrdl, addr 0x%x, shall align %d\n", hba->utmrdl_dma_addr, UFS_ALIGN_UTMRD);
		goto out;
	}

	hba->active_tm_tag = (hba->utmrdl_base_addr & UFS_1KB_MASK) / sizeof(struct utp_task_req_desc);
#endif

	/* Allocate memory for local reference block */
	hba->lrb = &ufs_lrb[0];

#if 0
	/* Allocate memory for SCSI sense buffer */
	ptr = &(ufs_sense_buf[0]);

	for (i = 0; i < UFS_AIO_MAX_NUTRS; i++, ptr += SCSI_SENSE_BUFFERSIZE)
		hba->sense_buf_base_addr[i] = hba->sense_buf_dma_addr[i] = ptr;

#endif

	return 0;
out:
	return -1;
}

static void ufshcd_host_memory_configure(struct ufs_hba *hba)
{
	int i;
	int cmd_desc_size;
	u16 prdt_offset;
	u16 response_offset;
	ufs_paddr_t cmd_desc_element_addr;
	ufs_paddr_t cmd_desc_dma_addr;
	struct utp_transfer_req_desc *utrdlp;
	struct utp_transfer_cmd_desc *cmd_descp;

	cmd_descp = hba->ucdl_base_addr;
	utrdlp = hba->utrdl_base_addr;

	prdt_offset     = offsetof(struct utp_transfer_cmd_desc, prd_table);
	response_offset = offsetof(struct utp_transfer_cmd_desc, response_upiu);

	cmd_desc_dma_addr = hba->ucdl_dma_addr;
	cmd_desc_size     = sizeof(struct utp_transfer_cmd_desc);

	for (i = 0; i < hba->nutrs; i++) {
		cmd_desc_element_addr =
		    (cmd_desc_dma_addr + (cmd_desc_size * i));
		utrdlp[i].command_desc_base_addr_hi = cpu_to_le32(upper_32_bits(cmd_desc_element_addr));
		utrdlp[i].command_desc_base_addr_lo = cpu_to_le32(lower_32_bits(cmd_desc_element_addr));

		utrdlp[i].response_upiu_length = cpu_to_le16(ALIGNED_UPIU_SIZE >> 2);
		utrdlp[i].prd_table_offset     = cpu_to_le16((prdt_offset >> 2));
		utrdlp[i].response_upiu_offset = cpu_to_le16((response_offset >> 2));

		hba->lrb[i].ucd_prdt_ptr = (struct ufshcd_sg_entry *)cmd_descp[i].prd_table;
		hba->lrb[i].ucd_req_ptr = (struct utp_upiu_req *)(cmd_descp + i);
		hba->lrb[i].ucd_rsp_ptr = (struct utp_upiu_rsp *)cmd_descp[i].response_upiu;
		hba->lrb[i].utr_descriptor_ptr = (utrdlp + i);
	}
}

/**
 * ufshcd_enable_intr - enable interrupts
 * @hba: per adapter instance
 * @intrs: interrupt bits
 */
static void ufshcd_enable_intr(struct ufs_hba *hba, u32 intrs)
{
	u32 set = ufshcd_readl(hba, REG_INTERRUPT_ENABLE);

	set |= intrs;

	ufshcd_writel(hba, set, REG_INTERRUPT_ENABLE);
}

static int ufshcd_hba_enable(struct ufs_hba *hba)
{
	int retry;

	if ((ufshcd_readl(hba, REG_CONTROLLER_ENABLE) & 0x1) == 1) {
		ufshcd_writel(hba, CONTROLLER_DISABLE,  REG_CONTROLLER_ENABLE);
		msleep(5);
	}

	/*
	 * Reset device after hba disabled
	 * This is to make sure host TX is idle
	 * when resetting device.
	 */
	ufs_aio_reset_device(hba);

	ufshcd_writel(hba, CONTROLLER_ENABLE, REG_CONTROLLER_ENABLE);
	msleep(1);

	retry = 20;

	while ((ufshcd_readl(hba, REG_CONTROLLER_ENABLE) & 0x1) == 0) {
		if (retry) {
			retry--;
		} else {
			UFS_DBG_LOGE("Controller enable failed\n");
			return -1;
		}
		msleep(5);
	}

	/* enable UIC related interrupts */
	ufshcd_enable_intr(hba, UFSHCD_UIC_MASK);

	return 0;
}

static int
__ufshcd_send_uic_cmd(struct ufs_hba *hba, struct uic_command *uic_cmd)
{
	if (!(ufshcd_readl(hba, REG_CONTROLLER_STATUS) & UIC_COMMAND_READY)) {
		UFS_DBG_LOGD("Controller not ready to accept UIC commands\n");
		return -1;
	}

	UFS_DBG_LOGD("[UFS] info: UCMD:%x,%x,%x,%x\n", uic_cmd->command, uic_cmd->argument1, uic_cmd->argument2, uic_cmd->argument3);

	hba->active_uic_cmd = uic_cmd;

	ufshcd_writel(hba, uic_cmd->argument1, REG_UIC_COMMAND_ARG_1);
	ufshcd_writel(hba, uic_cmd->argument2, REG_UIC_COMMAND_ARG_2);
	ufshcd_writel(hba, uic_cmd->argument3, REG_UIC_COMMAND_ARG_3);

	ufshcd_writel(hba, uic_cmd->command & COMMAND_OPCODE_MASK, REG_UIC_COMMAND);

	return 0;
}

static int ufshcd_wait_for_register(struct ufs_hba *hba, u32 reg, u32 mask,
				    u32 val, unsigned long interval_us, unsigned long timeout_ms)
{
	unsigned long elapsed_us = 0;
	int err = 0;

	val &= mask;

	while (val != (ufshcd_readl(hba, reg) & mask)) {

		/* wakeup within 50us of expiry */
		usleep(interval_us);
		elapsed_us += interval_us;

		if (elapsed_us > timeout_ms * 1000) {
			if ((ufshcd_readl(hba, reg) & mask) != val)
				err = -1;
			break;
		}
	}

	return err;
}

static void ufshcd_uic_cmd_compl(struct ufs_hba *hba, u32 intr_status)
{
	if ((intr_status & UIC_COMMAND_COMPL) && hba->active_uic_cmd) {
		hba->active_uic_cmd->argument2 |=
		    (ufshcd_readl(hba, REG_UIC_COMMAND_ARG_2) & MASK_UIC_COMMAND_RESULT);
		hba->active_uic_cmd->argument3 = ufshcd_readl(hba, REG_UIC_COMMAND_ARG_3);
	}
}

static int ufshcd_wait_for_uic_cmd(struct ufs_hba *hba, struct uic_command *uic_cmd)
{
	int ret;

	ret = ufshcd_wait_for_register(hba, REG_INTERRUPT_STATUS,
				       UIC_COMMAND_COMPL, UIC_COMMAND_COMPL,
				       100, UIC_CMD_TIMEOUT);
	if (!ret) {
		ufshcd_writel(hba, UIC_COMMAND_COMPL,
			      REG_INTERRUPT_STATUS);
		ufshcd_uic_cmd_compl(hba, UIC_COMMAND_COMPL);
	} else {
		UFS_DBG_LOGD("ufshcd_wait_for_uic_cmd error! ret: %d\n", ret);
	}

	hba->active_uic_cmd = NULL;

	return ret;
}

static int ufshcd_send_uic_cmd(struct ufs_hba *hba, struct uic_command *uic_cmd)
{
	int ret;

	ret = __ufshcd_send_uic_cmd(hba, uic_cmd);

	if (!ret)
		ret = ufshcd_wait_for_uic_cmd(hba, uic_cmd);

	return ret;
}

int ufshcd_uic_cmd_run(struct ufs_hba *hba, struct uic_command *cmds, int ncmds)
{
	int i;
	int err = 0;

	for (i = 0; i < ncmds; i++) {

		err = ufshcd_send_uic_cmd(hba, &cmds[i]);

		if (err) {
			UFS_DBG_LOGD("ufshcd_uic_cmd_run fail, cmd: %x, arg1: %x\n", cmds->command, cmds->argument1);

			/* send next commands anyway */
		}
	}

	return err;
}

static int ufshcd_dme_link_startup(struct ufs_hba *hba)
{
	struct uic_command uic_cmd = {0};

	uic_cmd.command = UIC_CMD_DME_LINK_STARTUP;

	return ufshcd_send_uic_cmd(hba, &uic_cmd);
}

/**
 * ufshcd_is_device_present - Check if any device connected to
 *                  the host controller
 * @hba: pointer to adapter instance
 *
 * Returns 1 if device present, 0 if no device detected
 */
static inline int ufshcd_is_device_present(struct ufs_hba *hba)
{
	return (ufshcd_readl(hba, REG_CONTROLLER_STATUS) &
		DEVICE_PRESENT) ? 1 : 0;
}

/**
 * ufshcd_get_lists_status - Check UCRDY, UTRLRDY and UTMRLRDY
 * @reg: Register value of host controller status
 *
 * Returns integer, 0 on Success and positive value if failed
 */
static inline int ufshcd_get_lists_status(u32 reg)
{
	/*
	 * The mask 0xFF is for the following HCS register bits
	 * Bit      Description
	 *  0       Device Present
	 *  1       UTRLRDY
	 *  2       UTMRLRDY
	 *  3       UCRDY
	 *  4       HEI
	 *  5       DEI
	 * 6-7      reserved
	 */
	return (((reg) & (0xFF)) >> 1) ^ (0x07);
}

/**
 * ufshcd_enable_run_stop_reg - Enable run-stop registers,
 *          When run-stop registers are set to 1, it indicates the
 *          host controller that it can process the requests
 * @hba: per adapter instance
 */
static void ufshcd_enable_run_stop_reg(struct ufs_hba *hba)
{
	ufshcd_writel(hba, UTP_TASK_REQ_LIST_RUN_STOP_BIT,
		      REG_UTP_TASK_REQ_LIST_RUN_STOP);
	ufshcd_writel(hba, UTP_TRANSFER_REQ_LIST_RUN_STOP_BIT,
		      REG_UTP_TRANSFER_REQ_LIST_RUN_STOP);
}

static int ufshcd_make_hba_operational(struct ufs_hba *hba)
{
	int err = 0;
	u32 reg;

	ufshcd_writel(hba, upper_32_bits(hba->utrdl_dma_addr),
		      REG_UTP_TRANSFER_REQ_LIST_BASE_H);
	ufshcd_writel(hba, lower_32_bits(hba->utrdl_dma_addr),
		      REG_UTP_TRANSFER_REQ_LIST_BASE_L);

	ufshcd_enable_intr(hba, UFSHCD_ENABLE_INTRS);

	reg = ufshcd_readl(hba, REG_CONTROLLER_STATUS);
	if ((ufshcd_get_lists_status(reg)) == 0) {
		ufshcd_enable_run_stop_reg(hba);
	} else {
		UFS_DBG_LOGD("Host controller not ready to process requests");
		err = -1;
	}

	return err;
}


/**
 * ufshcd_link_startup - Initialize unipro link startup
 * @hba: per adapter instance
 *
 * Returns 0 for success, non-zero in case of failure
 */
static int ufshcd_link_startup(struct ufs_hba *hba)
{
	int ret;
	int retries = DME_LINKSTARTUP_RETRIES;

	do {
		ufs_aio_pre_link(hba);

		ret = ufshcd_dme_link_startup(hba);

		if (ret) {
			UFS_DBG_LOGE("link: UIC command fail\n");
			goto retry;
		}

		/* check if device is detected by inter-connect layer */
		if (!ufshcd_is_device_present(hba)) {
			UFS_DBG_LOGE("link: Device not present\n");
			ret = -1;
			/* directly go through to retry section */
		}
retry:
		/*
		 * DME link lost indication is only received when link is up,
		 * but we can't be sure if the link is up until link startup
		 * succeeds. So reset the local Uni-Pro and try again.
		 */
		if (ret) {
			UFS_DBG_LOGE("link startup fail, retrying the %d times ...\n", DME_LINKSTARTUP_RETRIES - retries + 1);

			if (ufshcd_hba_enable(hba)) {
				/* critical error: controller is not working now, keep ret and goto out */
				goto out;
			}
		}
	} while (ret && retries--);

	if (ret)
		/* failed to get the link up... retire */
		goto out;

	/* Include any host controller configuration via UIC commands */
	ret = ufs_aio_post_link(hba);

	if (ret)
		goto out;

	ret = ufshcd_make_hba_operational(hba);
out:
	if (ret)
		UFS_DBG_LOGE("link startup failed %d\n", ret);
	return ret;
}

/**
* ufshcd_init_pwr_info - setting the POR (power on reset)
* values in hba power info
* @hba: per-adapter instance
*/
static void ufshcd_init_pwr_info(struct ufs_hba *hba)
{
	hba->pwr_info.gear_rx = UFS_PWM_G1;
	hba->pwr_info.gear_tx = UFS_PWM_G1;
	hba->pwr_info.lane_rx = 1;
	hba->pwr_info.lane_tx = 1;
	hba->pwr_info.pwr_rx = SLOW_MODE;
	hba->pwr_info.pwr_tx = SLOW_MODE;
	hba->pwr_info.hs_rate = 1;
}

/**
 * ufshcd_get_dev_cmd_tag - Get device management command tag
 * @hba: per-adapter instance
 * @tag: pointer to variable with available slot value
 *
 * Get a free slot and lock it until device management command
 * completes.
 *
 * Returns FALSE if free slot is unavailable for locking, else
 * return TRUE with tag value in @tag.
 */
bool ufshcd_get_free_tag(struct ufs_hba *hba, int *tag_out)
{
#ifdef UFS_CFG_SINGLE_COMMAND

	if (!tag_out)
		return FALSE;

	if (hba->lrb_in_use & (1 << hba->active_tr_tag)) {
		UFS_DBG_LOGE("[UFS] ufshcd_get_free_tag fail\n");
		return FALSE;
	}
	else {
		hba->lrb_in_use |= (1 << hba->active_tr_tag);
		*tag_out = hba->active_tr_tag;
		return TRUE;
	}

#else   /* !UFS_CFG_SINGLE_COMMAND */

	int tag;
	bool ret = FALSE;
	unsigned long tmp;

	if (!tag_out)
		goto out;

	do {
		tmp = ~hba->lrb_in_use;
		tag = find_last_bit(&tmp, hba->nutrs);

		if (tag >= hba->nutrs)
			goto out;

	} while (test_and_set_bit(tag, &hba->lrb_in_use));

	*tag_out = tag;
	ret = TRUE;
out:
	return ret;

#endif  /* UFS_CFG_SINGLE_COMMAND */
}

void ufshcd_put_tag(struct ufs_hba *hba, int tag)
{
	/* clear_bit(tag, &hba->lrb_in_use); */

	hba->lrb_in_use &= ~(1 << tag);
}

static void ufshcd_prepare_req_desc_hdr(struct ufs_hba *hba,
					struct ufshcd_lrb *lrbp,
					u32 *upiu_flags, enum dma_data_direction cmd_dir)
{
	struct utp_transfer_req_desc *req_desc = lrbp->utr_descriptor_ptr;
	u32 data_direction;
	u32 dword_0;

	if (cmd_dir == DMA_FROM_DEVICE) {
		data_direction = UTP_DEVICE_TO_HOST;
		*upiu_flags = UPIU_CMD_FLAGS_READ;
	} else if (cmd_dir == DMA_TO_DEVICE) {
		data_direction = UTP_HOST_TO_DEVICE;
		*upiu_flags = UPIU_CMD_FLAGS_WRITE;
	} else {
		data_direction = UTP_NO_DATA_TRANSFER;
		*upiu_flags = UPIU_CMD_FLAGS_NONE;
	}

	dword_0 = data_direction;
	dword_0 |= 1 << UPIU_COMMAND_TYPE_OFFSET;

#ifdef UFS_CFG_CRYPTO
	if (lrbp->crypto_en) {
		dword_0 |= (1 << UPIU_COMMAND_CRYPTO_EN_OFFSET);  /* crypto enable */
		dword_0 |= lrbp->crypto_cfgid;

		req_desc->header.dword_1 = cpu_to_le32(lrbp->crypto_dunl);
		req_desc->header.dword_3 = cpu_to_le32(lrbp->crypto_dunu);
	}
#endif

	if (lrbp->intr_cmd)
		dword_0 |= UTP_REQ_DESC_INT_CMD;

	req_desc->header.dword_2 = cpu_to_le32(OCS_INVALID_COMMAND_STATUS);
	req_desc->header.dword_0 = cpu_to_le32(dword_0);
}


static void ufshcd_prepare_utp_scsi_cmd_upiu(struct ufshcd_lrb *lrbp, u32 upiu_flags)
{
	struct utp_upiu_req *ucd_req = lrbp->ucd_req_ptr;

	ucd_req->sc.exp_data_transfer_len = cpu_to_be32(lrbp->cmd->exp_len);

	ucd_req->header.dword_2 = 0;

	ucd_req->header.dword_1 = UPIU_HEADER_DWORD(
					  UPIU_COMMAND_SET_TYPE_SCSI, 0, 0, 0);
	ucd_req->header.dword_0 = UPIU_HEADER_DWORD(
					  UPIU_TRANSACTION_COMMAND, upiu_flags,
					  lrbp->lun, lrbp->task_tag);

	memcpy(ucd_req->sc.cdb, lrbp->cmd->cmd_data, (min_t(unsigned short, lrbp->cmd->cmd_len, MAX_CDB_SIZE)));
}

static void ufshcd_prepare_utp_query_req_upiu(struct ufs_hba *hba,
	struct ufshcd_lrb *lrbp, u32 upiu_flags)
{
	u8 *descp = (u8 *)lrbp->ucd_req_ptr + GENERAL_UPIU_REQUEST_SIZE;
	struct ufs_query *query = &hba->dev_cmd.query;
	u16 len = be16_to_cpu(query->request.upiu_req.length);
	struct utp_upiu_req *ucd_req = lrbp->ucd_req_ptr;

	ucd_req->header.dword_2 = UPIU_HEADER_DWORD(
					  0, 0, len >> 8, (u8)len);

	ucd_req->header.dword_1 = UPIU_HEADER_DWORD(
					  0, query->request.query_func, 0, 0);

	ucd_req->header.dword_0 = UPIU_HEADER_DWORD(
					  UPIU_TRANSACTION_QUERY_REQ, upiu_flags,
					  lrbp->lun, lrbp->task_tag);

	/* Copy the Query Request buffer as is */
	memcpy(&ucd_req->qr, &query->request.upiu_req, QUERY_OSF_SIZE);

	/* Copy the Descriptor */
	if (query->request.upiu_req.opcode == UPIU_QUERY_OPCODE_WRITE_DESC) {
		memcpy(descp, query->descriptor, len);
		/*
		 * ufshcd_send_command() only flushes length of sizeof(struct utp_upiu_req).
		 * For Write Descriptor, we may need to flush more data range.
		 */
		ufs_aio_dma_map((unsigned long)descp, len, DMA_TO_DEVICE);
	}

}

static int ufshcd_compose_upiu(struct ufs_hba *hba, struct ufshcd_lrb *lrbp)
{
	u32 upiu_flags;
	int ret = 0;

	switch (lrbp->command_type) {
	case UTP_CMD_TYPE_SCSI:
		if (lrbp->cmd) {
			/* prepare transfer request descriptor */
			ufshcd_prepare_req_desc_hdr(hba, lrbp, &upiu_flags, lrbp->cmd->dir);

			/* prepare COMMAND UPIU */
			ufshcd_prepare_utp_scsi_cmd_upiu(lrbp, upiu_flags);
		} else {
			ret = -1;
		}
		break;
	case UTP_CMD_TYPE_DEV_MANAGE:
		ufshcd_prepare_req_desc_hdr(hba, lrbp, &upiu_flags, DMA_NONE);

		if (hba->dev_cmd.type == DEV_CMD_TYPE_QUERY)
			ufshcd_prepare_utp_query_req_upiu(hba, lrbp, upiu_flags);
		else if (hba->dev_cmd.type == DEV_CMD_TYPE_NOP) {
			memset(lrbp->ucd_req_ptr, 0, sizeof(struct utp_upiu_req));
			lrbp->ucd_req_ptr->header.dword_0 = UPIU_HEADER_DWORD(UPIU_TRANSACTION_NOP_OUT, 0, 0, lrbp->task_tag);

		}
		else
			ret = -1;

		break;
	case UTP_CMD_TYPE_UFS:
		/* For UFS native command implementation */
		ret = -1;
		UFS_DBG_LOGD("UFS native command are not supported\n");
		break;
	default:
		ret = -1;
		UFS_DBG_LOGD("unknown command type: 0x%x\n", lrbp->command_type);
		break;
	} /* end of switch */

	return ret;
}

static int ufshcd_compose_dev_cmd(struct ufs_hba *hba,
				  struct ufshcd_lrb *lrbp, enum dev_cmd_type cmd_type, int tag)
{
	lrbp->cmd = NULL;
/* lrbp->sense_bufflen = 0; */
/* lrbp->sense_buffer = NULL; */
	lrbp->task_tag = tag;
	lrbp->lun = 0; /* device management cmd is not specific to any LUN */
	lrbp->command_type = UTP_CMD_TYPE_DEV_MANAGE;
	lrbp->intr_cmd = TRUE; /* No interrupt aggregation */
	hba->dev_cmd.type = cmd_type;

#ifdef UFS_CFG_CRYPTO
	lrbp->crypto_en = 0;
#endif

	return ufshcd_compose_upiu(hba, lrbp);
}

/**
 * ufshcd_send_command - Send SCSI or device management commands
 * @hba: per adapter instance
 * @task_tag: Task tag of the command
 */
static inline void ufshcd_send_command(struct ufs_hba *hba, unsigned int task_tag)
{
	volatile u32 intr_status;

	/* set_bit(task_tag, &hba->outstanding_reqs); */
	hba->outstanding_reqs |= (1 << task_tag);

#ifdef UFS_CFG_SINGLE_COMMAND

	/* flush COMMAND UPIU */
	ufs_aio_dma_map((unsigned long)hba->lrb->ucd_req_ptr, sizeof(struct utp_upiu_req), DMA_TO_DEVICE);

	/* invalidate RESPONSE UPIU */
	ufs_aio_dma_map((unsigned long)hba->lrb->ucd_rsp_ptr, sizeof(struct utp_upiu_rsp), DMA_FROM_DEVICE);

	/* flush PRDT */
	ufs_aio_dma_map((unsigned long)hba->lrb->ucd_prdt_ptr, sizeof(struct ufs_aio_sg_entry) * UFS_AIO_MAX_SG_SEGMENTS, DMA_TO_DEVICE);

	/* invalidate transfer request descriptor (UTRD) */
	ufs_aio_dma_map((unsigned long)hba->lrb->utr_descriptor_ptr, sizeof(struct utp_transfer_req_desc), DMA_BIDIRECTIONAL);

#else

#error "Err: UFS AIO driver does not support multiple commands now."

#endif

	/* clear interrupt status */
	intr_status = ufs_aio_readl(hba, REG_INTERRUPT_STATUS);

	if (intr_status)
		ufs_aio_writel(hba, intr_status, REG_INTERRUPT_STATUS);

	ufshcd_writel(hba, 1 << task_tag, REG_UTP_TRANSFER_REQ_DOOR_BELL);
}

/**
 * ufshcd_exec_dev_cmd - API for sending device management requests
 * @hba - UFS hba
 * @cmd_type - specifies the type (NOP, Query...)
 * @timeout - time in seconds
 *
 * NOTE: Since there is only one available tag for device management commands,
 * it is expected you hold the hba->dev_cmd.lock mutex.
 */
static int ufshcd_exec_dev_cmd(struct ufs_hba *hba,
			       enum dev_cmd_type cmd_type, u32 timeout)
{
	struct ufshcd_lrb *lrbp;
	int err;
	int tag;

	if (!ufshcd_get_free_tag(hba, &tag))
		return -1;

#ifdef UFS_CFG_SINGLE_COMMAND
	lrbp = &hba->lrb[0];
#else
	lrbp = &hba->lrb[tag];
#endif

	err = ufshcd_compose_dev_cmd(hba, lrbp, cmd_type, tag);

	if (err)
		goto out_put_tag;

	ufshcd_send_command(hba, tag);

	err = ufshcd_wait_command(hba, lrbp, timeout);

out_put_tag:

	ufshcd_put_tag(hba, tag);

	return err;
}

static inline void ufshcd_init_query(struct ufs_hba *hba,
				     struct ufs_query_req **request, struct ufs_query_res **response,
				     enum query_opcode opcode, u8 idn, u8 index, u8 selector)
{
	*request = &hba->dev_cmd.query.request;
	*response = &hba->dev_cmd.query.response;
	memset(*request, 0, sizeof(struct ufs_query_req));
	memset(*response, 0, sizeof(struct ufs_query_res));
	(*request)->upiu_req.opcode = opcode;
	(*request)->upiu_req.idn = idn;
	(*request)->upiu_req.index = index;
	(*request)->upiu_req.selector = selector;
}


/**
 * ufshcd_complete_dev_init() - checks device readiness
 * hba: per-adapter instance
 *
 * Set fDeviceInit flag and poll until device toggles it.
 */
static int ufshcd_complete_dev_init(struct ufs_hba *hba)
{
	int retries, err = 0;
	bool flag_res = 1;
	u32 start_tick, timeout_tick;

	for (retries = QUERY_REQ_RETRIES; retries > 0; retries--) {

		/* Set the fDeviceInit flag */
		err = hba->query_flag(hba, UPIU_QUERY_OPCODE_SET_FLAG, QUERY_FLAG_IDN_FDEVICEINIT, NULL);

		if (!err)
			break;

		UFS_DBG_LOGD("error %d retrying\n", err);
	}
	if (err) {
		UFS_DBG_LOGD("setting fDeviceInit flag failed with error %d\n", err);
		goto out;
	}
	// get timeout tick
	timeout_tick = gpt4_time2tick_us (UFS_FDEVICEINIT_TIMEOUT_US);
	start_tick = gpt4_get_current_tick ();

	/* poll for max. UFS_FDEVICEINIT_TIMEOUT_US(5s) for fDeviceInit flag to clear */
	do {
		for (retries = QUERY_REQ_RETRIES; retries > 0; retries--) {
			err = hba->query_flag(hba, UPIU_QUERY_OPCODE_READ_FLAG, QUERY_FLAG_IDN_FDEVICEINIT, &flag_res);

			if (!err)
				break;

			UFS_DBG_LOGD("error %d retrying\n", err);
		}
	} while (!err && flag_res && !gpt4_timeout_tick (start_tick, timeout_tick));

	if (err)
		UFS_DBG_LOGD("reading fDeviceInit flag failed with error %d\n", err);
	else if (flag_res) {
		UFS_DBG_LOGD("fDeviceInit was not cleared by the device\n");
		err = UFS_ERR_FDEVICEINIT_NOT_CLR;
	}

out:
	return err;
}

int ufshcd_dme_get_attr(struct ufs_hba *hba, u32 attr_sel, u32 *mib_val, u8 peer)
{
	int ret;
	struct uic_command uic_cmd = {0};

	uic_cmd.argument1 = attr_sel;
	uic_cmd.command = peer ? UIC_CMD_DME_PEER_GET : UIC_CMD_DME_GET;

	ret = ufshcd_send_uic_cmd(hba, &uic_cmd);

	if (mib_val && !ret)
		*mib_val = uic_cmd.argument3;

	if (ret)
		UFS_DBG_LOGD("attr-id 0x%x error code %d\n", UIC_GET_ATTR_ID(attr_sel), ret);

	return ret;
}

/**
 * ufshcd_dme_set_attr - UIC command for DME_SET, DME_PEER_SET
 * @hba: per adapter instance
 * @attr_sel: uic command argument1
 * @attr_set: attribute set type as uic command argument2
 * @mib_val: setting value as uic command argument3
 * @peer: indicate whether peer or local
 *
 * Returns 0 on success, non-zero value on failure
 */
int ufshcd_dme_set_attr(struct ufs_hba *hba, u32 attr_sel, u8 attr_set, u32 mib_val, u8 peer)
{
	struct uic_command uic_cmd = {0};
	int ret;

	uic_cmd.command = peer ? UIC_CMD_DME_PEER_SET : UIC_CMD_DME_SET;
	uic_cmd.argument1 = attr_sel;
	uic_cmd.argument2 = UIC_ARG_ATTR_TYPE(attr_set);
	uic_cmd.argument3 = mib_val;

	ret = ufshcd_send_uic_cmd(hba, &uic_cmd);

	if (ret)
		UFS_DBG_LOGE("[UFS] err: attr-id 0x%x val 0x%x error code %d (peer %d)\n", UIC_GET_ATTR_ID(attr_sel), mib_val, ret, peer);

	return ret;
}

/**
 * ufshcd_get_max_pwr_mode - reads the max power mode negotiated with device
 * @hba: per-adapter instance
 */
static int ufshcd_get_max_pwr_mode(struct ufs_hba *hba)
{
	struct ufs_pa_layer_attr *pwr_info = &hba->max_pwr_info.info;

	if (hba->max_pwr_info.is_valid)
		return 0;

	pwr_info->pwr_tx = FAST_MODE;
	pwr_info->pwr_rx = FAST_MODE;
	pwr_info->hs_rate = PA_HS_MODE_B;

	/* Get the connected lane count */
	hba->dme_get(hba, UIC_ARG_MIB(PA_CONNECTEDRXDATALANES),
		     &pwr_info->lane_rx);

	UFS_DBG_LOGD("[UFS] info: ufshcd_get_max_pwr_mode, lane_rx: %d\n", pwr_info->lane_rx); /* mtk debug */

	hba->dme_get(hba, UIC_ARG_MIB(PA_CONNECTEDTXDATALANES),
		     &pwr_info->lane_tx);

	UFS_DBG_LOGD("[UFS] info: ufshcd_get_max_pwr_mode, lane_tx: %d\n", pwr_info->lane_tx); /* mtk debug */

	if (!pwr_info->lane_rx || !pwr_info->lane_tx) {
		UFS_DBG_LOGD("invalid connected lanes value. rx=%d, tx=%d\n",
			     pwr_info->lane_rx,
			     pwr_info->lane_tx);
		return -1;
	}

	/*
	 * First, get the maximum gears of HS speed.
	 * If a zero value, it means there is no HSGEAR capability.
	 * Then, get the maximum gears of PWM speed.
	 */
	hba->dme_get(hba, UIC_ARG_MIB(PA_MAXRXHSGEAR), &pwr_info->gear_rx);

	if (!pwr_info->gear_rx) {
		hba->dme_get(hba, UIC_ARG_MIB(PA_MAXRXPWMGEAR), &pwr_info->gear_rx);
		if (!pwr_info->gear_rx) {
			UFS_DBG_LOGD("invalid max pwm rx gear read = %d\n", pwr_info->gear_rx);
			return -1;
		}
		pwr_info->pwr_rx = SLOWAUTO_MODE;
	} else
		UFS_DBG_LOGD("[UFS] info: ufshcd_get_max_pwr_mode, gear_rx: %d\n", pwr_info->gear_rx); /* mtk debug */

	hba->dme_peer_get(hba, UIC_ARG_MIB(PA_MAXRXHSGEAR),
			  &pwr_info->gear_tx);

	if (!pwr_info->gear_tx) {
		hba->dme_peer_get(hba, UIC_ARG_MIB(PA_MAXRXPWMGEAR),
				  &pwr_info->gear_tx);
		if (!pwr_info->gear_tx) {
			UFS_DBG_LOGD("invalid max pwm tx gear read = %d\n", pwr_info->gear_tx);
			return -1;
		}
		pwr_info->pwr_tx = SLOWAUTO_MODE;
	} else
		UFS_DBG_LOGD("[UFS] info: ufshcd_get_max_pwr_mode, gear_tx: %d\n", pwr_info->gear_tx); /* mtk debug */

	hba->max_pwr_info.is_valid = TRUE;

	return 0;
}

/**
 * ufshcd_get_upmcrs - Get the power mode change request status
 * @hba: Pointer to adapter instance
 *
 * This function gets the UPMCRS field of HCS register
 * Returns value of UPMCRS field
 */
static inline u8 ufshcd_get_upmcrs(struct ufs_hba *hba)
{
	return (ufshcd_readl(hba, REG_CONTROLLER_STATUS) >> 8) & 0x7;
}

static int ufshcd_uic_pwr_ctrl(struct ufs_hba *hba, struct uic_command *cmd)
{
	u8 status;
	int ret;

	ret = __ufshcd_send_uic_cmd(hba, cmd);

	if (ret) {
		UFS_DBG_LOGD("pwr ctrl cmd 0x%x with mode 0x%x uic error %d\n", cmd->command, cmd->argument3, ret);
		goto out;
	}

	ret = ufshcd_wait_for_uic_cmd(hba, cmd);

	if (ret) {
		UFS_DBG_LOGD("pwr ctrl cmd 0x%x with mode 0x%x uic error %d\n", cmd->command, cmd->argument3, ret);
		goto out;
	}

	ret = ufshcd_wait_for_register(hba, REG_INTERRUPT_STATUS,
				       UIC_POWER_MODE, UIC_POWER_MODE,
				       100, UIC_CMD_TIMEOUT);
	if (ret) {
		UFS_DBG_LOGE("[UFS] err: wait UIC_POWER_MODE interrupt timeout\n");
		goto out;
	}

	ufshcd_writel(hba, UIC_POWER_MODE, REG_INTERRUPT_STATUS);

	status = ufshcd_get_upmcrs(hba);

	if (status != PWR_LOCAL) {
		UFS_DBG_LOGE("[UFS] err: pwr ctrl cmd 0x%0x failed, host umpcrs:0x%x\n", cmd->command, status);
		ret = (status != PWR_OK) ? status : -1;
	}

out:
	return ret;
}

static int ufshcd_uic_change_pwr_mode(struct ufs_hba *hba, u8 mode)
{
	int ret;
	struct uic_command uic_cmd = {0};

	uic_cmd.argument1 = UIC_ARG_MIB(PA_PWRMODE);
	uic_cmd.command = UIC_CMD_DME_SET;
	uic_cmd.argument3 = mode;
	ret = ufshcd_uic_pwr_ctrl(hba, &uic_cmd);

	return ret;
}

static int ufshcd_dme_copy_attr(struct ufs_hba *hba,
				u32 attr_sel_dst, u8 peer_dst,
				u32 attr_sel_src, u8 peer_src)
{
	int ret;
	u32 mib_val = 0;

	ret = ufshcd_dme_get_attr(hba, attr_sel_src, &mib_val, peer_src);
	if (ret)
		return ret;

	return ufshcd_dme_set_attr(hba, attr_sel_dst, ATTR_SET_NOR,
				   mib_val, peer_dst);
}

static int ufshcd_setup_userdata(struct ufs_hba *hba)
{
	int i;
	int err = 0;
	struct {
		u32 attr_userdata;
		u32 attr_timeout;
	} userdata_config[] = {
		{
			UIC_ARG_MIB(PA_PWRMODEUSERDATA0),
			UIC_ARG_MIB(DL_FC0PROTTIMEOUTVAL),
		},
		{
			UIC_ARG_MIB(PA_PWRMODEUSERDATA1),
			UIC_ARG_MIB(DL_TC0REPLAYTIMEOUTVAL),
		},
		{
			UIC_ARG_MIB(PA_PWRMODEUSERDATA2),
			UIC_ARG_MIB(DL_AFC0REQTIMEOUTVAL),
		},
	};

	for (i = 0; !err && (u32)i < UFS_ARRAY_SIZE(userdata_config); i++) {
		err = ufshcd_dme_copy_attr(hba,
					   userdata_config[i].attr_userdata, DME_LOCAL,
					   userdata_config[i].attr_timeout, DME_LOCAL);
	}

	return err;
}

static u32 get_termination(u32 pwr_mode)
{
	switch (pwr_mode) {
	case FASTAUTO_MODE:
	case FAST_MODE:
		return 1;
	case SLOWAUTO_MODE:
	case SLOW_MODE:
		return 0;
	}

	return 0;
}
static int pwr_mode_change(struct ufs_hba *hba,
			   u32 tx_pwr_mode, u32 rx_pwr_mode,
			   u32 tx_gear, u32 rx_gear,
			   u32 tx_lanes, u32 rx_lanes, u32 hs_series)
{
	u32 tx_term = get_termination(tx_pwr_mode);
	u32 rx_term = get_termination(rx_pwr_mode);
	struct uic_command cmds[7];
	int ret;

	ret = ufshcd_dme_copy_attr(hba,
				   UIC_ARG_MIB(PA_TXTRAILINGCLOCKS), DME_LOCAL,
				   UIC_ARG_MIB(PA_MINRXTRAILINGCLOCKS), DME_PEER);
	if (ret)
		return ret;

	ret = ufshcd_dme_copy_attr(hba,
				   UIC_ARG_MIB(PA_TXTRAILINGCLOCKS), DME_PEER,
				   UIC_ARG_MIB(PA_MINRXTRAILINGCLOCKS), DME_LOCAL);
	if (ret)
		return ret;

	cmds[0] = UIC_CMD_DME_SET(PA_TXTERMINATION, 0, tx_term);
	cmds[1] = UIC_CMD_DME_SET(PA_RXTERMINATION, 0, rx_term);

	cmds[2] = UIC_CMD_DME_SET(PA_TXGEAR, 0, tx_gear);
	cmds[3] = UIC_CMD_DME_SET(PA_RXGEAR, 0, rx_gear);

	cmds[4] = UIC_CMD_DME_SET(PA_ACTIVETXDATALANES, 0, tx_lanes);
	cmds[5] = UIC_CMD_DME_SET(PA_ACTIVERXDATALANES, 0, rx_lanes);

	cmds[6] = UIC_CMD_DME_SET(PA_HSSERIES, 0, hs_series);

	return ufshcd_uic_cmd_run(hba, cmds, UFS_ARRAY_SIZE(cmds));
}

int ufshcd_legacy_pwr_mode_change(struct ufs_hba *hba, u32 tx_pwr_mode,
				  u32 rx_pwr_mode, u32 tx_gear, u32 rx_gear,
				  u32 hs_series, u32 tx_lanes, u32 rx_lanes)
{
	int err;

	err = ufshcd_setup_userdata(hba);
	if (err)
		return err;

	err = pwr_mode_change(hba, tx_pwr_mode, rx_pwr_mode,
			      tx_gear, rx_gear, tx_lanes, rx_lanes, hs_series);
	if (err)
		return err;

	err = ufshcd_uic_change_pwr_mode(hba, rx_pwr_mode << 4 | tx_pwr_mode);

	return err;
}

/**
 * ufshcd_config_pwr_mode - configure a new power mode
 * @hba: per-adapter instance
 * @desired_pwr_mode: desired power configuration
 */
int ufshcd_config_pwr_mode(struct ufs_hba *hba,
				  struct ufs_pa_layer_attr *desired_pwr_mode)
{
	struct ufs_pa_layer_attr final_params = { 0 };
	int ret;

	ufs_aio_pre_pwr_change(hba, desired_pwr_mode, &final_params);

	ret = ufshcd_legacy_pwr_mode_change(hba,
					    final_params.pwr_tx, final_params.pwr_rx,
					    final_params.gear_tx, final_params.gear_rx,
					    final_params.hs_rate,
					    final_params.lane_tx, final_params.lane_rx);

	return ret;
}

int ufshcd_map_sg(struct ufs_hba *hba, struct ufshcd_lrb *lrbp)
{
	struct ufshcd_sg_entry *prd_table;
	struct ufs_aio_scsi_cmd *cmd;
	int sg_size = UFS_AIO_MAX_SIZE_PER_SG_SEGMENT;
	int sg_segments;
	int i;
	u32 residual;
	ufs_paddr_t buf_dma_addr;

	cmd = lrbp->cmd;

	if (cmd->exp_len == 0)
		sg_segments = 0;
	else
		sg_segments = ((cmd->exp_len + sg_size - 1) / sg_size);

	if (sg_segments) {
		lrbp->utr_descriptor_ptr->prd_table_length = cpu_to_le16((u16) (sg_segments));

		prd_table = (struct ufshcd_sg_entry *)lrbp->ucd_prdt_ptr;

		buf_dma_addr = (ufs_paddr_t)cmd->data_buf;

		residual = cmd->exp_len;

		for (i = 0; i < sg_segments; i++) {
			/* size should be 32-bit aligned, this is forced by spec */
			if (residual > (u32)sg_size)
				prd_table[i].size = cpu_to_le32((u32)sg_size - 1);
			else
				prd_table[i].size = cpu_to_le32((u32)residual - 1);

			/* addr should be 32-bit aligned, this is forced by spec */
			prd_table[i].base_addr = cpu_to_le32(lower_32_bits(buf_dma_addr));
			prd_table[i].upper_addr = cpu_to_le32(upper_32_bits(buf_dma_addr));

			buf_dma_addr += sg_size;
			residual -= sg_size;
		}

		/* invalidate data buffer if existed */
		ufs_aio_dma_map((ufs_vaddr_t)cmd->data_buf, cmd->exp_len, lrbp->cmd->dir);
	} else {
		lrbp->utr_descriptor_ptr->prd_table_length = 0;
	}

	return 0;
}

/**
 * ufshcd_get_tr_ocs - Get the UTRD Overall Command Status
 * @lrb: pointer to local command reference block
 *
 * This function is used to get the OCS field from UTRD
 * Returns the OCS field in the UTRD
 */
static inline int ufshcd_get_tr_ocs(struct ufshcd_lrb *lrbp)
{
	return le32_to_cpu(lrbp->utr_descriptor_ptr->header.dword_2) & MASK_OCS;
}

/**
 * ufshcd_get_rsp_upiu_result - Get the result from response UPIU
 * @ucd_rsp_ptr: pointer to response UPIU
 *
 * This function gets the response status and scsi_status from response UPIU
 * Returns the response result code.
 */
static inline int
ufshcd_get_rsp_upiu_result(struct utp_upiu_rsp *ucd_rsp_ptr)
{
	return be32_to_cpu(ucd_rsp_ptr->header.dword_1) & MASK_RSP_UPIU_RESULT;
}

static int ufshcd_copy_query_response(struct ufs_hba *hba, struct ufshcd_lrb *lrbp)
{
	struct ufs_query_res *query_res = &hba->dev_cmd.query.response;

	memcpy(&query_res->upiu_res, &lrbp->ucd_rsp_ptr->qr, QUERY_OSF_SIZE);

	/* Get the descriptor */
	if (lrbp->ucd_rsp_ptr->qr.opcode == UPIU_QUERY_OPCODE_READ_DESC) {
		u8 *descp = (u8 *)lrbp->ucd_rsp_ptr +
			    GENERAL_UPIU_REQUEST_SIZE;
		u16 resp_len;
		u16 buf_len;

		/* data segment length */
		resp_len = be32_to_cpu(lrbp->ucd_rsp_ptr->header.dword_2) &
			   MASK_QUERY_DATA_SEG_LEN;
		buf_len = be16_to_cpu(
			      hba->dev_cmd.query.request.upiu_req.length);
		if (buf_len >= resp_len) {
			/*
			 * ufshcd_wait_command() only invalidates length of "strcut utp_upiu_rsp".
			 * We need to invalidate more data range for Read Descriptor operation.
			 */
			ufs_aio_dma_unmap((U32)descp, resp_len, DMA_FROM_DEVICE);
			memcpy(hba->dev_cmd.query.descriptor, descp, resp_len);
		} else {
			UFS_DBG_LOGD("Response size is bigger than buffer");
			return -1;
		}
	}

	return 0;
}

static inline int ufshcd_get_req_rsp(struct utp_upiu_rsp *ucd_rsp_ptr)
{
	return be32_to_cpu(ucd_rsp_ptr->header.dword_0) >> 24;
}

static int ufshcd_dev_cmd_completion(struct ufs_hba *hba, struct ufshcd_lrb *lrbp)
{
	int err = 0;
	int resp;

	resp = ufshcd_get_req_rsp(lrbp->ucd_rsp_ptr);

	switch (resp) {
	case UPIU_TRANSACTION_REJECT_UPIU:
		/* TODO: handle Reject UPIU Response */
		err = -1;
		UFS_DBG_LOGD("Reject UPIU not fully implemented\n");
		break;

	case UPIU_TRANSACTION_QUERY_RSP:
		hba->dev_cmd.query.response.response = ufshcd_get_rsp_upiu_result(lrbp->ucd_rsp_ptr) >> UPIU_RSP_CODE_OFFSET;
		err = hba->dev_cmd.query.response.response ;
		if (!err)
			err = ufshcd_copy_query_response(hba, lrbp);
		break;

	case UPIU_TRANSACTION_NOP_IN:
		if (hba->dev_cmd.type != DEV_CMD_TYPE_NOP) {
			err = -1;
			UFS_DBG_LOGD("unexpected response %x\n", resp);
		}
		break;

	default:
		err = -1;
		UFS_DBG_LOGD("Invalid device management cmd response: %x\n", resp);
		break;
	}

	return err;
}

int ufshcd_wait_command(struct ufs_hba *hba, struct ufshcd_lrb *lrbp, u32 timeout_ms)
{
	u8 ocs;
	u16 resp;
	int ret = UFS_ERR_NONE;
	u32 value;
	u32 tag_mask = 1 << lrbp->task_tag;
	u32 tr_doorbell;
	u32 intr_status;
#ifdef MTK_UFS_DRV_DA
	u32 time_start;
	u32 time_timeout;
	u32 wait_loop = 0;
#else
	unsigned long elapsed_ms = 0;
#endif

#if defined(MTK_UFS_DRV_DA)
	time_start = gpt4_get_current_tick();
	time_timeout = gpt4_time2tick_ms(timeout_ms);
#endif

#if defined(MTK_UFS_DRV_DA) || defined(MTK_UFS_DRV_LK)
	if (hba->drv_status & UFS_DRV_STS_DMA_WAIT_INTERRUPT) {
		/* Unmask interrupt now and wait */
		mt_irq_unmask(hba->irq);
		ret = event_wait_timeout(&ufshcd_intr_event, timeout_ms);
		if (ret != 0)
			UFS_DBG_LOGE("[UFS]: failed to get event timeout:%d\n", timeout_ms);

		/* Mask interrupt */
		mt_irq_mask(hba->irq);
	} else
#endif
	do {
		tr_doorbell = ufshcd_readl(hba, REG_UTP_TRANSFER_REQ_DOOR_BELL);

		if (!(tag_mask & tr_doorbell)) {
			/* clear_bit(lrbp->task_tag, &hba->outstanding_reqs); */
			hba->outstanding_reqs &= ~(1 << lrbp->task_tag);
			break;
		}

#ifdef MTK_UFS_DRV_DA
		/* ensure to use timeout detector in consideration of wrap around case in gpt register */
		if (gpt4_timeout_tick(time_start, time_timeout)) /* timeout, stop waiting */
			break;

		wait_loop++;

		if (wait_loop && (wait_loop % 10000) == 0)
			UFS_DBG_LOGD("[UFS] info: waiting for cmd done, loop %d ...\n", wait_loop);
#else
		msleep(1);
		elapsed_ms += 1;

		if (elapsed_ms > timeout_ms)
			break;
#endif
	} while (1);

	/* clear_bit(lrbp->task_tag, &hba->lrb_in_use); // need to clear lrb_in_use for polling case, otherwise no one can use this tag in the future. */
	hba->lrb_in_use &= ~(1 << lrbp->task_tag);

	tr_doorbell = ufshcd_readl(hba, REG_UTP_TRANSFER_REQ_DOOR_BELL);

	if (tag_mask & tr_doorbell) {
		UFS_DBG_LOGE("[UFS] err: Query Request timeout.timeout_ms:%d\n", timeout_ms);

		ret = UFS_ERR_TIMEOUT_QUERY_REQUEST;
		goto out;
	}

	/* invalid data buffer */
	if (lrbp->cmd)
		ufs_aio_dma_unmap((unsigned long)lrbp->cmd->data_buf, lrbp->cmd->exp_len, lrbp->cmd->dir);

	/* dummy, just for pairing with ufs_aio_dma_map(UTRD) and ufs_aio_dma_map(PRDT) */
	/* ufs_aio_dma_unmap((unsigned long)lrbp->ucd_req_ptr, sizeof(struct utp_upiu_req), DMA_TO_DEVICE); */
	/* ufs_aio_dma_unmap((unsigned long)hba->lrb->ucd_prdt_ptr, sizeof(struct ufs_aio_sg_entry) * UFS_AIO_MAX_SG_SEGMENTS, DMA_TO_DEVICE); */

	/* check OCS */

	/* invalid UTRD cache */
	ufs_aio_dma_unmap((unsigned long)lrbp->utr_descriptor_ptr, sizeof(struct utp_transfer_req_desc), DMA_BIDIRECTIONAL);

#if defined(MTK_UFS_DRV_DA) || defined(MTK_UFS_DRV_LK)
	if (hba->drv_status & UFS_DRV_STS_DMA_WAIT_INTERRUPT)
		intr_status = ufshcd_intr_status;
	else
#endif
	intr_status = ufs_aio_readl(hba, REG_INTERRUPT_STATUS);

	/* check interrupt status if any error happens */
	if (intr_status & UFSHCD_ERROR_MASK) {

		UFS_DBG_LOGE("[UFS] ERR! intr_status: 0x%x\n", intr_status);

		if (intr_status & UIC_ERROR) {

			UFS_DBG_LOGE("[UFS] REG_UIC_ERROR_CODE_PHY_ADAPTER_LAYER: 0x%x\n", ufs_aio_readl(hba, REG_UIC_ERROR_CODE_PHY_ADAPTER_LAYER));
			UFS_DBG_LOGE("[UFS] REG_UIC_ERROR_CODE_DATA_LINK_LAYER: 0x%x\n", ufs_aio_readl(hba, REG_UIC_ERROR_CODE_DATA_LINK_LAYER));
			UFS_DBG_LOGE("[UFS] REG_UIC_ERROR_CODE_NETWORK_LAYER: 0x%x\n", ufs_aio_readl(hba, REG_UIC_ERROR_CODE_NETWORK_LAYER));
			UFS_DBG_LOGE("[UFS] REG_UIC_ERROR_CODE_TRANSPORT_LAYER: 0x%x\n", ufs_aio_readl(hba, REG_UIC_ERROR_CODE_TRANSPORT_LAYER));
			UFS_DBG_LOGE("[UFS] REG_UIC_ERROR_CODE_DME: 0x%x\n", ufs_aio_readl(hba, REG_UIC_ERROR_CODE_DME));
		}
	}

	ocs = ufshcd_get_tr_ocs(lrbp);

	if (ocs != OCS_SUCCESS) {
		UFS_DBG_LOGE("[UFS] err: OCS error = %x, T:%d\n", ocs, lrbp->task_tag);

#if (UFS_DBG_LVL <= UFS_DBG_LVL_DEBUG)
		{
			u8 cnt;

			value = ufshcd_readl(hba, REG_UFS_MTK_OCS_ERR_STATUS);
			UFS_DBG_LOGD("[UFS] err: OCS err status reg = %x\n", value);

			for (cnt = 0; g_ufs_ocs_error_str[cnt].err != 0xFF; cnt++) {
				if (value & (1 << cnt))
					UFS_DBG_LOGD("[UFS] err: %s\n", g_ufs_ocs_error_str[cnt].name);
			}
		}
#endif  /* UFS_DBG_LVL_DEBUG */

		ret = UFS_ERR_OCS_ERROR;
		goto out;
	}

	/* check response code */

	/* invalid RESPONSE UPIU cache
	 *
	 * NOTE: Here we only invalidate length of "strcut utp_upiu_rsp". For Response UPIU with
	 *       longer data size (e.g., Read Descriptor), need to invalidate more data.
	 *       Please see ufshcd_copy_query_response().
	 */
	ufs_aio_dma_unmap((U32)lrbp->ucd_rsp_ptr, sizeof(struct utp_upiu_rsp), DMA_FROM_DEVICE);

	if (UTP_CMD_TYPE_DEV_MANAGE == lrbp->command_type)  /* for device management commands */
		return ufshcd_dev_cmd_completion(hba, lrbp);
	else {                                              /* for SCSI commands */
		resp = ufshcd_get_rsp_upiu_result(lrbp->ucd_rsp_ptr);

		if (resp & MASK_TASK_RESPONSE) {

			value = ufshcd_get_req_rsp(lrbp->ucd_rsp_ptr);

			if (UPIU_TRANSACTION_RESPONSE == value)
				ufs_mtk_dump_asc_ascq(hba, lrbp->ucd_rsp_ptr->sr.sense_data[12], lrbp->ucd_rsp_ptr->sr.sense_data[13]);

			 /* sense key is UNIT_ATTENTION, will retry */
			if (lrbp->cmd && lrbp->cmd->cmd_data[0] == TEST_UNIT_READY && (lrbp->ucd_rsp_ptr->sr.sense_data[2] &0xF) == UNIT_ATTENTION)
				ret = UFS_TEST_UNIT_RDY_ERR;
			else {
				ret = UFS_ERR_TASK_RESP_ERROR;
				UFS_DBG_LOGE("[UFS] err: task response error = %x\n", (resp & MASK_TASK_RESPONSE) >> 8);
			}
			goto out;
		}
	}

out:
	if (lrbp->cmd)
		lrbp->cmd = NULL;

	UFS_DBG_LOGV("[UFS] info: CMD done\n");

	return ret;
}

void ufs_aio_crypto_cal_dun(u32 alg_id, u32 lba, u32 *dunl, u32 *dunu)
{
	if (UFS_CRYPTO_ALGO_BITLOCKER_AES_CBC != alg_id) {
		*dunl = lba;
		*dunu = 0;
	} else {                             /* bitlocker dun use byte address */
		*dunl = (lba & 0x7FFFF) << 12;   /* byte address for lower 32 bit */
		*dunu = (lba >> (32-12)) << 12;  /* byte address for higher 32 bit */
	}
}

static void ufshcd_queuecommand_timeout(struct ufs_aio_scsi_cmd *cmd, int *timeout)
{
	if(cmd->cmd_data[0] == UNMAP)
		*timeout = UTP_UNMAP_TIMEOUT_MS;
	else
		*timeout = UTP_TRANSFER_REQ_TIMEOUT_MS;
}

int ufshcd_queuecommand(struct ufs_hba *hba, struct ufs_aio_scsi_cmd *cmd)
{
	struct ufshcd_lrb *lrbp;
	int tag, lun;
	int err = 0;
#if defined(UFS_CFG_DEBUG)
	u32 blk_cnt;
#endif
#if defined(UFS_CFG_DEBUG) || defined(UFS_CFG_CRYPTO)
	u32 lba;
#endif
#ifdef UFS_CFG_CRYPTO
	u32 dunl, dunu;
#endif
	int timeout = UTP_TRANSFER_REQ_TIMEOUT_MS;

	tag = cmd->tag;
	lun = cmd->lun;

#if defined(UFS_CFG_DEBUG)
	lba = cmd->cmd_data[5] | (cmd->cmd_data[4] << 8) | (cmd->cmd_data[3] << 16) | (cmd->cmd_data[2] << 24);
	blk_cnt = cmd->cmd_data[8] | (cmd->cmd_data[7] << 8);

	UFS_DBG_LOGD("[UFS] info: QCMD,L:%x,T:%d,0x%x,A:0x%x,C:%d\n", lun, tag, cmd->cmd_data[0],lba, blk_cnt);
#endif

#ifdef UFS_CFG_SINGLE_COMMAND
	lrbp = &hba->lrb[0];
#else
	lrbp = &hba->lrb[tag];
#endif

#ifdef UFS_CFG_CRYPTO
	if (ufs_crypto_rw_en) {
		lba = ((cmd->cmd_data[2]) << 24) | ((cmd->cmd_data[3]) << 16) |
		((cmd->cmd_data[4]) << 8) | (cmd->cmd_data[5]);

		ufs_aio_crypto_cal_dun(UFS_CRYPTO_ALGO_ESSIV_AES_CBC, lba, &dunl, &dunu);
	}
#endif

	lrbp->cmd = cmd;
/* lrbp->sense_bufflen = SCSI_SENSE_BUFFERSIZE; */
/* lrbp->sense_buffer = hba->sense_buf_base_addr[tag]; */
	lrbp->task_tag = tag;
	lrbp->lun = lun;
	lrbp->intr_cmd = TRUE;    /* in AIO driver, use interrupt for every commands. */
	lrbp->command_type = UTP_CMD_TYPE_SCSI;

#ifdef UFS_CFG_CRYPTO
	if (ufs_crypto_rw_en) {
		lrbp->crypto_cfgid = 0;
		lrbp->crypto_dunl = dunl;
		lrbp->crypto_dunu = dunu;
		lrbp->crypto_en = 1;
	} else
		lrbp->crypto_en = 0;
#endif

	/* form UPIU before issuing the command */
	ufshcd_compose_upiu(hba, lrbp);
	err = ufshcd_map_sg(hba, lrbp);

	if (err) {
		UFS_DBG_LOGD("Err: ufshcd_map_sg_ut fails\n");
		lrbp->cmd = NULL;
		/* clear_bit(tag, &hba->lrb_in_use); */
		hba->lrb_in_use &= ~(1 << tag);
		goto out;
	}

	/* issue command to the controller */
	ufshcd_send_command(hba, tag);

	/* Setting utp transfer req command timeout according to different cmd */
	ufshcd_queuecommand_timeout(cmd, &timeout);

	err = ufshcd_wait_command(hba, lrbp, timeout);

out:
	return err;
}

#ifdef MTK_UFS_DRV_DA
int ufs_aio_get_ref_clk(struct ufs_hba *hba, u32 *ref)
{
	int ret;

	/*
	 * Reference Clock Frequency value
	 * 0h: 19.2MHz
	 * 1h: 26MHz
	 * 2h: 38.4MHz
	 * 3h: 52MHz
	 */
	ret = hba->query_attr(hba, UPIU_QUERY_OPCODE_READ_ATTR, ATTR_B_REF_CLK_FREQ, 0, 0, ref);

	if (ret != 0)
		UFS_DBG_LOGD("[UFS] err: %s error: %d\n", __func__, ret);

	return ret;
}

int ufs_aio_set_ref_clk(struct ufs_hba *hba, u32 ref)
{
	int ret;

	/*
	 * Reference Clock Frequency value
	 * 0h: 19.2MHz
	 * 1h: 26MHz
	 * 2h: 38.4MHz
	 * 3h: 52MHz
	 */
	ret = hba->query_attr(hba, UPIU_QUERY_OPCODE_WRITE_ATTR, ATTR_B_REF_CLK_FREQ, 0, 0, &ref);

	if (ret != 0)
		UFS_DBG_LOGD("[UFS] err: %s error: %d\n", __func__, ret);
	else
		UFS_DBG_LOGD("[UFS] info: %s %d done\n", __func__, ref);

	return ret;
}

__WEAK unsigned int config_TW_size_GB(void)
{
	return 0xffffffff;
}

__WEAK unsigned int config_TW_no_reduction(void)
{
	return 1;
}

__WEAK unsigned int config_HPB_size_GB(void)
{
	return 0xffffffff;
}

static void ufshcd_init_custom_info(struct ufs_hba *hba)
{
	/* Already set done and not changed by UI */
	if (hba->custom_info.custom_flag & CUSTOM_SET_DONE)
		return;

	/*
	 * Set to custom value in DA if flag not set.
	 * Set to UI config in flag set.
	 */
	if ((hba->custom_info.custom_flag & CUSTOM_FORCE_PROVISION) == 0)
		hba->custom_info.force_provision = FALSE;

	if ((hba->custom_info.custom_flag & CUSTOM_TW_GB) == 0)
		hba->custom_info.tw_size_gb = config_TW_size_GB();

	if ((hba->custom_info.custom_flag & CUSTOM_TW_NO_RED) == 0)
		hba->custom_info.tw_no_red = config_TW_no_reduction();

	if ((hba->custom_info.custom_flag & CUSTOM_HPB_REGION_COUNT) == 0)
		hba->custom_info.hpb_size_gb = config_HPB_size_GB();

	/* Set custom info done flag */
	hba->custom_info.custom_flag |= CUSTOM_SET_DONE;
}
#endif

static int ufshcd_probe_hba(struct ufs_hba *hba)
{
	int ret;
#ifdef MTK_UFS_DRV_DA
	u32 ref_clk;
#endif

	ret = ufshcd_link_startup(hba);
	if (ret)
		goto out;

	ufshcd_init_pwr_info(hba);

	ret = hba->nopin_nopout(hba);

	if (ret)
		goto out;

#if defined(MTK_UFS_DRV_LK)
	PROFILING_START("UFS device init");
#endif
	ret = ufshcd_complete_dev_init(hba);
#if defined(MTK_UFS_DRV_LK)
	PROFILING_END();
#endif
	if (ret)
		goto out;

	/* Init check for device descriptor sizes */
	ufshcd_init_desc_sizes(hba);

	/* get quirks according to Device Descritpor contents */
	ufs_advertise_fixup_device(hba);

#ifdef MTK_UFS_DRV_DA
	ufshcd_init_custom_info(hba);

	/* check 1. check if bootable UFS */
	/* get device infomation by reading Device Descriptor */
	ufs_aio_get_device_info(hba);

	ret = ufs_aio_check_lu_cfg(hba);

	/* new UFS or invalid LU configuration is found, start (re-)configuration flow */
	if ((hba->dev_info.bootable == 0) ||
	    (ret == UFS_ERR_INVALID_LU_CONFIGURATION) ||
	    (hba->custom_info.force_provision == TRUE)) {
		hba->custom_info.force_provision = FALSE;
		return ufs_aio_prepare_new_ufs(hba);
	}

	UFS_DBG_LOGI("[UFS] info: LU Configuration Check OK. Bootable UFS\n");

	/* check 2. check if Boot LU is configured as expectation */
	/* Boot LU 1/2 shall set in LU0/1 */

	ret = ufs_aio_read_unit_desc_cfg_param(hba);

	if (!(hba->unit_desc_cfg_param[0].b_lu_enable &&
		hba->unit_desc_cfg_param[0].b_boot_lun_id == 1 &&
		hba->unit_desc_cfg_param[1].b_lu_enable &&
		hba->unit_desc_cfg_param[1].b_boot_lun_id == 2))
		return ufs_aio_prepare_new_ufs(hba);

	UFS_DBG_LOGI("[UFS] info: Boot LU Configuration Check OK. Active Boot LU: LU0\n");

#else
	ufs_aio_check_lu_cfg(hba);
#endif  /* MTK_UFS_DRV_DA */

	ufs_aio_test_unit_ready_all_device(hba);

	/* ufshcd_force_reset_auto_bkops(hba); */

#ifdef MTK_UFS_DRV_DA
	/*
	 * Change device to 26MHz
	 * Need check first. If not, some old device cannot write and return F8
	 * as "Parameter already written". which cause ufs init fail.
	 */
	ret = ufs_aio_get_ref_clk(hba, &ref_clk);
	if ((ret == 0) && (ref_clk != 1)) {
		ret = ufs_aio_set_ref_clk(hba, 1);
		if (ret) {
			UFS_DBG_LOGE("[UFS] Set device 26Mhz fail! %d\n", ret);
			goto out;
		}
	}
#endif

	#if !defined(MTK_UFS_DRV_CTP)
	if (ufshcd_get_max_pwr_mode(hba))
		UFS_DBG_LOGE("[UFS] Failed getting max supported power mode\n");
	else {
		ret = ufshcd_config_pwr_mode(hba, &hba->max_pwr_info.info);
		if (ret) {
			UFS_DBG_LOGE("Failed setting power mode, err = %d\n", ret);

			/* ignore power mode change error to let UFS init go through anyway */
			ret = 0;
		}
	}
	#endif

out:
	return ret;
}

/**
 * ufshcd_init - Driver initialization routine
 * Returns 0 on success, non-zero value on failure
 */
int ufshcd_init(void)
{
	int err;
	int retry_init_cnt = 0;
	struct ufs_hba *hba = &g_ufs_hba;

	UFS_DBG_LOGD("[UFS] memory (ufs_ucdl): 0x%x\n", (u32)&ufs_ucdl[0]);
	UFS_DBG_LOGD("[UFS] memory (ufs_lrb): 0x%x\n", (u32)&ufs_lrb[0]);

	ufs_get_platform_data();
	hba->hci_base = ufs_platform.hci_base;
	hba->pericfg_base = ufs_platform.pericfg_base;
	hba->mphy_base = ufs_platform.mphy_base;

	/* get hba capabilities */
	ufshcd_get_host_capabilities(hba);

	/* init power */
	ufshcd_power(hba, TRUE);

	/* init clock */
	ufshcd_clock(hba, TRUE);

	ufs_aio_advertise_hci_quirks(hba);

	/* allocate memory for hba memory space */
	err = ufshcd_memory_alloc(hba);

	if (err) {
		UFS_DBG_LOGD("Memory allocation failed\n");
		goto out_error;
	}

	/* Configure LRB */
	ufshcd_host_memory_configure(hba);

ufshcd_init_reinit_host:
	/* Host controller enable */
	err = ufshcd_hba_enable(hba);

	if (err) {
		UFS_DBG_LOGE("[UFS] err: Host controller enable failed\n");
		goto out_error;
	}

	err = ufshcd_probe_hba(hba);

	if (UFS_ERR_NEED_REINIT_HOST == err) {
		retry_init_cnt++;

		if (retry_init_cnt > 3) {
			UFS_DBG_LOGE("[UFS] err: retry number exceeded (reinit host)\n");
			goto out_error;
		}

		goto ufshcd_init_reinit_host;
	}

	if (err) {
		UFS_DBG_LOGE("[UFS] err: ufshcd_probe_hba failed\n");
		goto out_error;
	}

#if defined(MTK_UFS_DRV_DA)
	if (sysob_runtime_params.flag & INT_MODE_ENABLE) {
		if (!ufshcd_irq_init(hba))
			hba->drv_status |= UFS_DRV_STS_DMA_WAIT_INTERRUPT;
	}
#elif defined(MTK_UFS_DRV_LK)
	if (!ufshcd_irq_init(hba))
		hba->drv_status |= UFS_DRV_STS_DMA_WAIT_INTERRUPT;
#endif

	return 0;

out_error:
	return err;
}

static int ufshcd_query_descriptor(struct ufs_hba *hba,
				   enum query_opcode opcode, enum desc_idn idn, u8 index,
				   u8 selector, u8 *desc_buf, u32 *buf_len)
{
	struct ufs_query_req *request = NULL;
	struct ufs_query_res *response = NULL;
	int err;

	BUG_ON(!hba);

	if (*buf_len < QUERY_DESC_MIN_SIZE || *buf_len > QUERY_DESC_MAX_SIZE) {
		UFS_DBG_LOGD("descriptor buffer size (%d) is out of range\n", *buf_len);
		err = -1;
		goto out;
	}

	if (!desc_buf) {
		UFS_DBG_LOGD("descriptor buffer required for opcode 0x%x\n", opcode);
		err = -1;
		goto out;
	}

	ufshcd_init_query(hba, &request, &response, opcode, idn, index, selector);
	request->upiu_req.length = cpu_to_be16(*buf_len);
	hba->dev_cmd.query.descriptor = desc_buf;

	switch (opcode) {
	case UPIU_QUERY_OPCODE_READ_DESC:
		request->query_func = UPIU_QUERY_FUNC_STANDARD_READ_REQUEST;
		break;

	case UPIU_QUERY_OPCODE_WRITE_DESC:
		request->query_func = UPIU_QUERY_FUNC_STANDARD_WRITE_REQUEST;
		break;

	default:
		UFS_DBG_LOGE("Expected query descriptor opcode but got = 0x%x\n", opcode);
		err = -1;
		goto out;
	}

	err = ufshcd_exec_dev_cmd(hba, DEV_CMD_TYPE_QUERY, QUERY_REQ_TIMEOUT);

	if (err) {
		UFS_DBG_LOGE("opcode 0x%x for idn %d failed, err = %d\n", opcode, idn, err);
		goto out;
	}

	*buf_len = be16_to_cpu(response->upiu_res.length);
	hba->dev_cmd.query.descriptor = NULL;

out:
	return err;
}

static int ufshcd_read_desc_param(struct ufs_hba *hba,
				  enum desc_idn desc_id,
				  int desc_index,
				  u8 selector,
				  u8 *param_read_buf,
				  u32 param_size)
{
	u32 desc_len;
	u8 *desc_buf;
	int ret;

	if (desc_id >= QUERY_DESC_IDN_MAX)
		return -1;

	desc_len = ufs_query_desc_max_size[desc_id];

	if (desc_len > param_size)
		desc_len = param_size;
	desc_buf = param_read_buf;

	ret = ufshcd_query_descriptor(hba, UPIU_QUERY_OPCODE_READ_DESC,
				      desc_id, desc_index, selector, desc_buf,
				      &desc_len);

	if (ret ||
		(desc_len > ufs_query_desc_max_size[desc_id]) ||
		(desc_buf[QUERY_DESC_DESC_TYPE_OFFSET] != desc_id)) {
		UFS_DBG_LOGE("[UFS] failed reading descriptor. desc_id %d desc_len %d ret %d\n", desc_id, desc_len, ret);

		if (!ret)
			ret = UFS_ERR_INVALID_DESCRIPTOR;

		goto out;
	}

out:

	return ret;
}

static int ufshcd_read_desc_length(struct ufs_hba *hba,
	enum desc_idn desc_id,
	int desc_index,
	u8 selector,
	u32 *desc_length)
{
	int ret;
	u8 header[QUERY_DESC_HDR_SIZE];
	u32 header_len = QUERY_DESC_HDR_SIZE;

	if (desc_id >= QUERY_DESC_IDN_MAX)
		ret = -1;

	ret = ufshcd_query_descriptor(hba, UPIU_QUERY_OPCODE_READ_DESC,
					desc_id, desc_index, selector, header,
					&header_len);

	if (ret) {
		UFS_DBG_LOGE("%s: Failed to get descriptor header id %d\n",
			__func__, desc_id);
		return ret;
	} else if (desc_id != header[QUERY_DESC_DESC_TYPE_OFFSET]) {
		UFS_DBG_LOGE("%s: descriptor header id %d and desc_id %d mismatch\n",
			__func__, header[QUERY_DESC_DESC_TYPE_OFFSET],
			desc_id);
		ret = -1;
	}

	*desc_length = header[QUERY_DESC_LENGTH_OFFSET];
	return ret;

}

static void ufshcd_init_desc_sizes(struct ufs_hba *hba)
{
	int err;
	static int init_done = 0;

	if (init_done)
		return;

	err = ufshcd_read_desc_length(hba, QUERY_DESC_IDN_DEVICE, 0, 0,
		&ufs_query_desc_max_size[QUERY_DESC_IDN_DEVICE]);
	if (err)
		ufs_query_desc_max_size[QUERY_DESC_IDN_DEVICE] = QUERY_DESC_DEVICE_MAX_SIZE;
	UFS_DBG_LOGI("device descriptor length = 0x%x\n", ufs_query_desc_max_size[QUERY_DESC_IDN_DEVICE]);

	err = ufshcd_read_desc_length(hba, QUERY_DESC_IDN_POWER, 0, 0,
		&ufs_query_desc_max_size[QUERY_DESC_IDN_POWER]);
	if (err)
		ufs_query_desc_max_size[QUERY_DESC_IDN_POWER] = QUERY_DESC_POWER_MAX_SIZE;
	UFS_DBG_LOGI("power descriptor length = 0x%x\n", ufs_query_desc_max_size[QUERY_DESC_IDN_POWER]);

	err = ufshcd_read_desc_length(hba, QUERY_DESC_IDN_INTERCONNECT, 0, 0,
		&ufs_query_desc_max_size[QUERY_DESC_IDN_INTERCONNECT]);
	if (err)
		ufs_query_desc_max_size[QUERY_DESC_IDN_INTERCONNECT] = QUERY_DESC_INTERCONNECT_MAX_SIZE;
	UFS_DBG_LOGI("interconnect descriptor length = 0x%x\n", ufs_query_desc_max_size[QUERY_DESC_IDN_INTERCONNECT]);

	err = ufshcd_read_desc_length(hba, QUERY_DESC_IDN_CONFIGURATION, 0, 0,
		&ufs_query_desc_max_size[QUERY_DESC_IDN_CONFIGURATION]);
	if (err)
		ufs_query_desc_max_size[QUERY_DESC_IDN_CONFIGURATION] = QUERY_DESC_CONFIGURAION_MAX_SIZE;
	UFS_DBG_LOGI("configuration descriptor length = 0x%x\n", ufs_query_desc_max_size[QUERY_DESC_IDN_CONFIGURATION]);

	err = ufshcd_read_desc_length(hba, QUERY_DESC_IDN_UNIT, 0, 0,
		&ufs_query_desc_max_size[QUERY_DESC_IDN_UNIT]);
	if (err)
		ufs_query_desc_max_size[QUERY_DESC_IDN_UNIT] = QUERY_DESC_UNIT_MAX_SIZE;
	UFS_DBG_LOGI("unit descriptor length = 0x%x\n", ufs_query_desc_max_size[QUERY_DESC_IDN_UNIT]);

	err = ufshcd_read_desc_length(hba, QUERY_DESC_IDN_GEOMETRY, 0, 0,
		&ufs_query_desc_max_size[QUERY_DESC_IDN_GEOMETRY]);
	if (err)
		ufs_query_desc_max_size[QUERY_DESC_IDN_GEOMETRY] = QUERY_DESC_GEOMETRY_MAX_SIZE;
	UFS_DBG_LOGI("geometry descriptor length = 0x%x\n", ufs_query_desc_max_size[QUERY_DESC_IDN_GEOMETRY]);

	init_done = 1;
}

static int ufshcd_init_vendor_desc_sizes(struct ufs_hba *hba)
{
	static int err = 0;
	static int vendor_init_done = 0;

	if (vendor_init_done)
		return err;

	err = ufshcd_read_desc_length(hba, QUERY_DESC_IDN_DEVICE, 0, 1,
		&ufs_query_desc_max_size[QUERY_DESC_IDN_DEVICE]);
	if (err)
		goto out;
	UFS_DBG_LOGI("vendor device descriptor length = 0x%x\n", ufs_query_desc_max_size[QUERY_DESC_IDN_DEVICE]);

	err = ufshcd_read_desc_length(hba, QUERY_DESC_IDN_CONFIGURATION, 0, 1,
		&ufs_query_desc_max_size[QUERY_DESC_IDN_CONFIGURATION]);
	if (err)
		goto out;
	UFS_DBG_LOGI("vendor configuration descriptor length = 0x%x\n", ufs_query_desc_max_size[QUERY_DESC_IDN_CONFIGURATION]);

	err = ufshcd_read_desc_length(hba, QUERY_DESC_IDN_GEOMETRY, 0, 1,
		&ufs_query_desc_max_size[QUERY_DESC_IDN_GEOMETRY]);
	if (err)
		goto out;
	UFS_DBG_LOGI("vendor geometry descriptor length = 0x%x\n", ufs_query_desc_max_size[QUERY_DESC_IDN_GEOMETRY]);

out:
	vendor_init_done = 1;
	return err;
}

/* replace non-printable or non-ASCII characters with spaces */
static inline void ufshcd_remove_non_printable(u8 *val)
{
	if (!val)
		return;

	if (*val < 0x20 || *val > 0x7e)
		*val = ' ';
}

static int ufs_aio_pre_pwr_change(struct ufs_hba *hba, struct ufs_pa_layer_attr *desired, struct ufs_pa_layer_attr *final)
{
	if (0 == hba->dev_info.wmanufacturerid) {
		UFS_DBG_LOGD("bug: manufacturer_id shall not be 0");
		return -1;
	}

	/* fill-in desired gear for different environment */
	/* make sure device capability */

#if defined(MTK_UFS_DRV_CTP)
	final->gear_rx = desired->gear_rx;
	final->gear_tx = desired->gear_tx;
	final->lane_rx = desired->lane_rx;
	final->lane_tx = desired->lane_tx;
	final->hs_rate = desired->hs_rate;
	final->pwr_rx = desired->pwr_rx;
	final->pwr_tx = desired->pwr_tx;
#else

#ifndef UFS_CFG_SAFE_BRING_UP
	if (final->pwr_rx == SLOW_MODE || final->pwr_rx == SLOWAUTO_MODE) {
		final->gear_rx = min_t(u32, UFS_DEV_MAX_GEAR_RX, 4);
		final->gear_tx = min_t(u32, UFS_DEV_MAX_GEAR_TX, 4);
	} else {
		final->gear_rx = min_t(u32, UFS_DEV_MAX_GEAR_RX, desired->gear_rx);
		final->gear_tx = min_t(u32, UFS_DEV_MAX_GEAR_TX, desired->gear_tx);
	}

	final->lane_rx = min_t(u32, UFS_DEV_MAX_LANE_RX, desired->lane_rx);
	final->lane_tx = min_t(u32, UFS_DEV_MAX_LANE_TX, desired->lane_tx);
#else
	final->gear_rx = UFS_DEV_MAX_GEAR_RX;
	final->gear_tx = UFS_DEV_MAX_GEAR_TX;
	final->lane_rx = UFS_DEV_MAX_LANE_RX;
	final->lane_tx = UFS_DEV_MAX_LANE_TX;
#endif

	final->hs_rate = UFS_DEV_DEFAULT_HS_RATE;
	final->pwr_rx = UFS_DEV_DEFAULT_PWR_RX;
	final->pwr_tx = UFS_DEV_DEFAULT_PWR_TX;
#endif

	if (final->pwr_rx == SLOW_MODE || final->pwr_rx == SLOWAUTO_MODE)
		UFS_DBG_LOGI("[UFS] info: PWM-G%d\n", final->gear_rx);
	else
		UFS_DBG_LOGI("[UFS] info: HS-G%d-%d\n", final->gear_rx, final->hs_rate);

	/* Set PAPowerModeUserData[0~5] = 0xffff, default is 0 */
	hba->dme_set(hba, UIC_ARG_MIB(PA_PWRMODEUSERDATA0), 0x1fff);
	hba->dme_set(hba, UIC_ARG_MIB(PA_PWRMODEUSERDATA1), 0xffff);
	hba->dme_set(hba, UIC_ARG_MIB(PA_PWRMODEUSERDATA2), 0x7fff);
	hba->dme_set(hba, UIC_ARG_MIB(PA_PWRMODEUSERDATA3), 0x1fff);
	hba->dme_set(hba, UIC_ARG_MIB(PA_PWRMODEUSERDATA4), 0xffff);
	hba->dme_set(hba, UIC_ARG_MIB(PA_PWRMODEUSERDATA5), 0x7fff);

	return 0;
}

int ufs_aio_enable_unipro_cg(struct ufs_hba *hba, bool enable)
{
	u32 tmp;

	if (enable) {

		hba->dme_get(hba, UIC_ARG_MIB(VENDOR_SAVEPOWERCONTROL), &tmp);
		tmp = tmp | (1 << RX_SYMBOL_CLK_GATE_EN) |
			    (1 << SYS_CLK_GATE_EN) |
			    (1 << TX_CLK_GATE_EN);
		hba->dme_set(hba, UIC_ARG_MIB(VENDOR_SAVEPOWERCONTROL), tmp);

		hba->dme_get(hba, UIC_ARG_MIB(VENDOR_DEBUGCLOCKENABLE), &tmp);
		tmp = tmp & ~(1 << TX_SYMBOL_CLK_REQ_FORCE);
		hba->dme_set(hba, UIC_ARG_MIB(VENDOR_DEBUGCLOCKENABLE), tmp);

	} else {

		hba->dme_get(hba, UIC_ARG_MIB(VENDOR_SAVEPOWERCONTROL), &tmp);
		tmp = tmp & ~((1 << RX_SYMBOL_CLK_GATE_EN) |
			      (1 << SYS_CLK_GATE_EN) |
			      (1 << TX_CLK_GATE_EN));
		hba->dme_set(hba, UIC_ARG_MIB(VENDOR_SAVEPOWERCONTROL), tmp);

		hba->dme_get(hba, UIC_ARG_MIB(VENDOR_DEBUGCLOCKENABLE), &tmp);
		tmp = tmp | (1 << TX_SYMBOL_CLK_REQ_FORCE);
		hba->dme_set(hba, UIC_ARG_MIB(VENDOR_DEBUGCLOCKENABLE), tmp);

	}

	return 0;
}

static int ufs_aio_pre_link(struct ufs_hba *hba)
{
	int ret = 0;
	u32 tmp;

	struct uic_command dme_setting_before_link[] = {
		UIC_CMD_DME_SET(PA_LOCALTXLCCENABLE, 0, 0x0), /* set LCC_Enable to 0 */
	};

	ufs_aio_bootrom_deputy(hba);

	ufs_aio_init_mphy(hba);

	if (hba->hci_quirks & UFSHCD_QUIRK_MTK_MPHY_TESTCHIP) {

		/* enable UIC related interrupts */
		ufshcd_enable_intr(hba, UIC_COMMAND_COMPL);

		/* apply setting before link startup */
		ret = ufshcd_uic_cmd_run(hba, dme_setting_before_link,
					 UFS_ARRAY_SIZE(dme_setting_before_link));

		if (ret)
			UFS_DBG_LOGD("dme_setting_before_link fail\n");
	}

	/* disable deep stall by default */
	hba->dme_get(hba, UIC_ARG_MIB(VENDOR_SAVEPOWERCONTROL), &tmp);
	tmp &= ~(1 << 6);
	hba->dme_set(hba, UIC_ARG_MIB(VENDOR_SAVEPOWERCONTROL), tmp);

	/* disable scrambling by default */
	hba->dme_set(hba, UIC_ARG_MIB(PA_SCRAMBLING), 0);

	/* disable unipro clock gating feature by default */
	ufs_aio_enable_unipro_cg(hba, FALSE);

	if (0 != ret)
		ret = 1;

	return ret;
}

static int ufs_aio_post_link(struct ufs_hba *hba)
{
	int ret = 0;
	u32 tmp;

	struct uic_command dme_setting_after_link[] = {
		UIC_CMD_DME_PEER_SET(PA_LOCALTXLCCENABLE, 0, 0x0), /* device LCC_Enable to 0 */
	};

	if (hba->hci_quirks & UFSHCD_QUIRK_MTK_MPHY_TESTCHIP) {

		/* apply setting after link startup */
		ret = ufshcd_uic_cmd_run(hba, dme_setting_after_link,
					 UFS_ARRAY_SIZE(dme_setting_after_link));

		if (ret)
			UFS_DBG_LOGD("dme_setting_after_link fail\n");
	}

#ifdef UFS_CFG_CRYPTO
	/* init HW FDE feature inlined in HCI */
	ufs_aio_crypto_init(hba);
#endif

	if (0 != ret)
		ret = 1;

	return ret;

}

/*
 * In early-porting stage, because of no bootrom, something finished by bootrom shall be finished here instead.
 * Returns:
 *  0: Successful.
 *  Non-zero: Failed.
 */
static int ufs_aio_bootrom_deputy(struct ufs_hba *hba)
{
#ifdef UFS_CFG_FPGA_PLATFORM
	u32 reg;
	u32 mask = 0;

	if (!hba->pericfg_base)
		return 1;

	reg = readl(hba->pericfg_base + ufs_platform.reg_ufs_pericfg);

	if (ufs_platform.reg_ufs_pericfg_ldo_n_bit != 0xF)
		mask |= (1 << ufs_platform.reg_ufs_pericfg_ldo_n_bit);

	if (ufs_platform.reg_ufs_pericfg_rst_n_bit != 0xF)
		mask |= (1 << ufs_platform.reg_ufs_pericfg_rst_n_bit);

	reg = reg & ~mask;
	writel(reg, hba->pericfg_base + ufs_platform.reg_ufs_pericfg);

	udelay(10);

	if (ufs_platform.reg_ufs_pericfg_ldo_n_bit != 0xF) {
		reg = readl(hba->pericfg_base + ufs_platform.reg_ufs_pericfg);
		reg = reg | (1 << ufs_platform.reg_ufs_pericfg_ldo_n_bit);
		writel(reg, hba->pericfg_base + ufs_platform.reg_ufs_pericfg);
		udelay(10);
	}

	if (ufs_platform.reg_ufs_pericfg_rst_n_bit != 0xF) {
		reg = readl(hba->pericfg_base + ufs_platform.reg_ufs_pericfg);
		reg = reg | (1 << ufs_platform.reg_ufs_pericfg_rst_n_bit);
		writel(reg, hba->pericfg_base + ufs_platform.reg_ufs_pericfg);
		udelay(10);
	}

	mdelay(1);

	return 0;

#else

	return 0;

#endif
}

static int ufs_aio_test_unit_ready_all_device(struct ufs_hba *hba)
{
	int i, tag;
	struct ufs_aio_scsi_cmd cmd;
	u32 start_tick, timeout_tick;
	int ret = UFS_ERR_NONE;

	if (0 == hba->dev_info.num_active_lu) {
		UFS_DBG_LOGE("bug: active_num_lu shall not be 0");
		return -1;
	}

	if (!ufshcd_get_free_tag(hba, &tag))
		return -1;

	/* send test unit ready to each LUN to remove possible UNIT ATTENTION */
	for (i = 0; i < hba->dev_info.num_active_lu; i++) {
		start_tick = gpt4_get_current_tick ();
		timeout_tick =
			gpt4_time2tick_us(UFS_TEST_UNIT_READY_TIMEOUT_US);

		do {
			ufs_aio_scsi_cmd_test_unit_ready(&cmd, tag, i);

			/* check error code if retrun with UNIT ATTENTION */
			ret = ufshcd_queuecommand(hba, &cmd);
		} while (ret && !gpt4_timeout_tick (start_tick, timeout_tick));
	}

	/* send test unit ready to WRPMB LUN */
	start_tick = gpt4_get_current_tick ();
	timeout_tick = gpt4_time2tick_us(UFS_TEST_UNIT_READY_TIMEOUT_US);
	do {
		ufs_aio_scsi_cmd_test_unit_ready(&cmd, tag, WLUN_RPMB);

		/* check error code if retrun with UNIT ATTENTION */
		ret = ufshcd_queuecommand(hba, &cmd);
	} while (ret && !gpt4_timeout_tick (start_tick, timeout_tick));

	ufshcd_put_tag(hba, tag);

	return 0;
}

static void ufs_aio_advertise_hci_quirks(struct ufs_hba *hba)
{
#ifdef UFS_CFG_FPGA_PLATFORM
	hba->hci_quirks |= UFSHCD_QUIRK_MTK_MPHY_TESTCHIP;
#endif
}

static void ufs_aio_reset_device(struct ufs_hba *hba)
{
#if defined(MTK_UFS_DRV_LK)
	/* reset device */
	mt_secure_call(MTK_SIP_BL_UFS_CONTROL_AARCH32, 2, 0, 0, 0);

	udelay(10);

	mt_secure_call(MTK_SIP_BL_UFS_CONTROL_AARCH32, 2, 1, 0, 0);
#else
	u32 reg;

	if (!hba->pericfg_base) {
		UFS_DBG_LOGD("[UFS] err: %s: pericfg_base is NULL\n", __func__);
		return;
	}

	reg = readl(hba->pericfg_base + ufs_platform.reg_ufs_pericfg);

	/* reset device */
	reg = reg & ~(1 << ufs_platform.reg_ufs_pericfg_rst_n_bit);
	writel(reg, hba->pericfg_base + ufs_platform.reg_ufs_pericfg);

	udelay(10);

	reg = reg | (1 << ufs_platform.reg_ufs_pericfg_rst_n_bit);
	writel(reg, hba->pericfg_base + ufs_platform.reg_ufs_pericfg);
#endif
	/* wait awhile after device reset */
	/* We add this as general solution even though only SK-Hynix needs this delay actually. */
	mdelay(10);
}

int ufs_aio_set_boot_lu(struct ufs_hba *hba, u32 b_boot_lun_en)
{
	int ret;

	ret = hba->query_attr(hba, UPIU_QUERY_OPCODE_WRITE_ATTR, ATTR_B_BOOT_LUN_EN, 0, 0, &b_boot_lun_en);

	if (0 != ret)
		UFS_DBG_LOGD("[UFS] err: ufs_aio_set_boot_lu error: %d\n", ret);
	else
		UFS_DBG_LOGD("[UFS] info: ufs_aio_set_boot_lu %d done\n", b_boot_lun_en);

	return ret;
}

#if defined(MTK_UFS_DRV_PRELOADER)
int ufs_aio_get_boot_lu(struct ufs_hba *hba, u32 *b_boot_lun)
{
	int ret;

	ret = hba->query_attr(hba, UPIU_QUERY_OPCODE_READ_ATTR, ATTR_B_BOOT_LUN_EN, 0, 0, b_boot_lun);

	if (0 != ret)
		UFS_DBG_LOGD("[UFS] err: ufs_aio_get_boot_lu error: %d\n", ret);
	else
		UFS_DBG_LOGD("[UFS] info: ufs_aio_get_boot_lu %d done\n", *b_boot_lun);

	return ret;
}
#endif

u32 ufs_aio_get_lu_cfg_size(struct ufs_hba *hba, struct ufs_geometry_info *info, u32 lun_id)
{
	u32 d_num_alloc_units;
	u32 tw_size_gb;
	u32 tw_no_red;
	u32 hpb_region_count;
	u32 hpb_size_gb;
	u64 lu2_size; /* unit : 512B */

	UFS_DBG_LOGE("[UFS] %s: lun %d\n", __func__, lun_id);

	UFS_DBG_LOGE("[UFS] d_segment_size=0x%x\n", info->d_segment_size);
	UFS_DBG_LOGE("[UFS] b_allocation_units_size=0x%x\n", info->b_allocation_units_size);
	UFS_DBG_LOGE("[UFS] q_total_raw_device_capacity=0x%llx\n", info->q_total_raw_device_capacity);

	if (lun_id < 2) {	/* boot lu */
		/*
		 * Boot size is 4MB which have enhance feature, real size
		 * shoule be 4MB * (w_adj_factor_enahnced_1 / 256)
		 */
		UFS_DBG_LOGE("[UFS] w_adj_factor_enahnced_1=%d\n", info->w_adj_factor_enahnced_1);

		d_num_alloc_units = ((UFS_BOOT_LU_SIZE_BYTE >> 9) * (info->w_adj_factor_enahnced_1 >> 8)) /
			info->d_segment_size / info->b_allocation_units_size;
	} else {			/* user lu */

		d_num_alloc_units = (u32)(((info->q_total_raw_device_capacity) - (u64)(2 * (UFS_BOOT_LU_SIZE_BYTE >> 9) * (info->w_adj_factor_enahnced_1 >> 8))) /
			info->d_segment_size / info->b_allocation_units_size);

		if (hba->dev_info.tw_support) {
			tw_size_gb = hba->custom_info.tw_size_gb;
			tw_no_red =  hba->custom_info.tw_no_red;

			/* default use ceiling[1/16 of lu 2] */
			if (tw_size_gb == 0xffffffff)
				tw_size_gb = (((d_num_alloc_units * info->b_allocation_units_size * info->d_segment_size) >> 21) + 15) >> 4;

			/* Samsung 3.0 not follow spec */
			if ((hba->dev_info.wmanufacturerid == UFS_VENDOR_SAMSUNG) && (hba->dev_info.ufs_ver == 0x0300))
				info->d_tw_buf_au = ((tw_size_gb << 21) / info->d_segment_size / info->b_allocation_units_size) * info->b_tw_buf_adj_fac;
			else
				info->d_tw_buf_au = ((tw_size_gb << 21) / info->d_segment_size / info->b_allocation_units_size);

			/* No check error, return err if custom provision fail, auto provision will set to max */
			if (info->d_tw_buf_au > info->d_tw_buf_max_au) {
				if (hba->custom_info.tw_size_gb == 0xffffffff)
					info->d_tw_buf_au = info->d_tw_buf_max_au;
				else
					UFS_DBG_LOGE("[UFS]======================> Err! Max AU=0x%x, but set=0x%x\n", info->d_tw_buf_max_au, info->d_tw_buf_au);
			}
			UFS_DBG_LOGI("[UFS] TW size=%dGB, TW AU=0x%x\n", tw_size_gb, info->d_tw_buf_au);

			if (tw_no_red == 0)
				d_num_alloc_units -= info->d_tw_buf_au;
		}

		if ((hba->dev_info.hpb_support) && (info->q_hpb_region_size)) {
			lu2_size = (u64)d_num_alloc_units * info->b_allocation_units_size * info->d_segment_size;

			info->w_hpb_lu_max_active_regions = (u32)((lu2_size + info->q_hpb_region_size - 1) / info->q_hpb_region_size);

			if (info->w_hpb_lu_max_active_regions > info->w_hpb_device_max_active_regions)
				info->w_hpb_lu_max_active_regions = info->w_hpb_device_max_active_regions;

			/* No check error, return err if provision fail */
			if (hba->custom_info.hpb_size_gb != 0xffffffff) {
				hpb_region_count = hba->custom_info.hpb_size_gb << 21 / info->q_hpb_region_size;

				if (hpb_region_count > info->w_hpb_lu_max_active_regions) {
					UFS_DBG_LOGE("[UFS]======================> Err! Max region count=0x%x, but set=0x%x\n",
						     info->w_hpb_lu_max_active_regions, hpb_region_count);
				}
				info->w_hpb_lu_max_active_regions = hpb_region_count;
			}
			hpb_size_gb = (u64)(info->w_hpb_lu_max_active_regions * info->q_hpb_region_size) >> 21;

			UFS_DBG_LOGI("[UFS] HPB size=%dGB, HPB LU regions=0x%x, LU size=0x%llx\n",
				hpb_size_gb, info->w_hpb_lu_max_active_regions, lu2_size);
		}
	}

	UFS_DBG_LOGE("[UFS] d_num_alloc_units=0x%x\n", d_num_alloc_units);

	return d_num_alloc_units;
}

static void ufs_aio_util_dump_config_table_hex(u8 *table, u32 bytes, u32 ud0_base_offset, u32 ud_config_len)
{
	u32 i, ptr;

	ptr = 0;

	for (i = 0; i < ud0_base_offset; i++) {
		UFS_DBG_LOGI("%x ", table[ptr]);
		ptr++;

		if (ptr >= bytes)
			break;
	}
	UFS_DBG_LOGE("\n");

	while (ptr < bytes) {

		for (i = 0; i < ud_config_len; i++) {
			UFS_DBG_LOGI("%x ", table[ptr]);
			ptr++;

			if (ptr >= bytes)
				break;
		}

		UFS_DBG_LOGE("\n");

		if (ptr >= bytes)
			return;
	}
}

void ufs_aio_util_dump_table_hex(u8 *table, u32 bytes)
{
	u32 i, ptr;

	ptr = 0;

	while (ptr < bytes) {

		for (i = 0; i < 16; i++) {
			UFS_DBG_LOGI("%x ", table[ptr]);
			ptr++;

			if (ptr >= bytes)
				break;
		}

		UFS_DBG_LOGE("\n");

		if (ptr >= bytes)
			return;
	}
}

int ufs_aio_check_lu_cfg(struct ufs_hba *hba)
{
	int ret;
	u32 i;
	u8 *p;
	u32 num_active_lu = 0;
	u32 num_boot_lu = 0;
#ifdef MTK_UFS_DRV_DA
#ifdef UFS_AIO_DEV_CHECK_LU_SIZE
	u32 d_num_alloc_units_boot_lu;
	u32 d_num_alloc_units_user_lu;
	struct ufs_geometry_info info = {0};
#endif
#endif

	/* HPB support, read configuration descriptor with HPB, selector = 1, length = 0xD0(HPB1.0)/0xE2(HPB2.0) */
	if ((hba->dev_info.hpb_support) && (hba->dev_info.wmanufacturerid == UFS_VENDOR_SAMSUNG || hba->dev_info.wmanufacturerid == UFS_VENDOR_MICRON_MP))
		ret = hba->read_descriptor(hba, QUERY_DESC_IDN_CONFIGURATION, 0, 1, (u8 *)&g_ufs_temp_buf[0], ufs_query_desc_max_size[QUERY_DESC_IDN_CONFIGURATION]);
	else
		ret = hba->read_descriptor(hba, QUERY_DESC_IDN_CONFIGURATION, 0, 0, (u8 *)&g_ufs_temp_buf[0], ufs_query_desc_max_size[QUERY_DESC_IDN_CONFIGURATION]);

	if (0 != ret) {
		UFS_DBG_LOGE("[UFS] err: ufs_aio_check_lu_cfg: read config descr error: %d\n", ret);
		return ret;
	}

	UFS_DBG_LOGE("[UFS] %s: original Configuration Desc:\n", __func__);

	ufs_aio_util_dump_config_table_hex(&g_ufs_temp_buf[0], ufs_query_desc_max_size[QUERY_DESC_IDN_CONFIGURATION], hba->dev_info.ud0_base_offset, hba->dev_info.ud_config_len);

	p = (u8 *)&g_ufs_temp_buf[0];

	/* check: number of active LU */
	for (i = 0; i < 8; i++) { /* traverse 8 Unit Descriptor configurable parameters */
		if (i == 0)
			p += hba->dev_info.ud0_base_offset;
		else
			p += hba->dev_info.ud_config_len;

#ifdef MTK_UFS_DRV_DA
		if ((hba->dev_info.hpb_support) && (i == 2) && (p[CONF_DESC_UNIT_B_LU_ENABLE] == 1)) {
			UFS_DBG_LOGE("[UFS] error: LU cfg error! HPB support, but not enable, need re-configuration.\n");
			return UFS_ERR_INVALID_LU_CONFIGURATION;
		}
#endif

		/* 01h: Logical Unit enabled, 02h: HPB Logical Unit enabled */
		if ((p[CONF_DESC_UNIT_B_LU_ENABLE] == 1) ||
			((hba->dev_info.hpb_support) && (p[CONF_DESC_UNIT_B_LU_ENABLE] == 2)))
			num_active_lu++;

		if ((i < 2) && (0 != p[CONF_DESC_UNIT_B_BOOT_LUN_ID])) {
			num_boot_lu++;
		}
	}

	hba->dev_info.num_active_lu = num_active_lu;

#ifdef MTK_UFS_DRV_DA
	if (3 != num_active_lu) {
		UFS_DBG_LOGE("[UFS] error: LU cfg error! incorrect number of active LUs: %d, need re-configuration.\n", num_active_lu);
		return UFS_ERR_INVALID_LU_CONFIGURATION;
	} else
		UFS_DBG_LOGI("[UFS] info: number of active LU: %d, check OK.\n", num_active_lu);

	/* check: must have 2 Boot LUs in the first 2 LUs */
	if (2 != num_boot_lu) {
		UFS_DBG_LOGE("[UFS] error: LU cfg error! shall have 2 Boot LUs in the first 2 LUs, need re-configuration.\n");
		return UFS_ERR_INVALID_LU_CONFIGURATION;
	} else
		UFS_DBG_LOGI("[UFS] info: number of Boot LU: %d, check OK.\n", num_boot_lu);

	/* todo: check Boot LU size and User LU size. */

#ifdef UFS_AIO_DEV_CHECK_LU_SIZE
	ret = hba->read_descriptor(hba, QUERY_DESC_IDN_GEOMETRY, 0, (u8 *)&g_ufs_temp_buf[0], ufs_query_desc_max_size[QUERY_DESC_IDN_GEOMETRY]);

	if (ret != 0) {
		UFS_DBG_LOGE("[UFS] err: ufs_aio_configure_new_ufs: read geometry desc error: %d\n", ret);
		return ret;
	}

	UFS_DBG_LOGE("[UFS] %s: Geometry Desc:\n", __func__);
	ufs_aio_util_dump_table_hex(&g_ufs_temp_buf[0], ufs_query_desc_max_size[QUERY_DESC_IDN_GEOMETRY]);

	p = (u8 *)&g_ufs_temp_buf[0];

	info.q_total_raw_device_capacity = ((u64)p[4] << 56) |
					   ((u64)p[5] << 48) |
					   ((u64)p[6] << 40) |
					   ((u64)p[7] << 32) |
					   (p[8] << 24) |
					   (p[9] << 16) |
					   (p[10] << 8) |
					   p[11];

	info.d_segment_size = (p[13] << 24) |
			      (p[14] << 16) |
			      (p[15] << 8) |
			      p[16];

	info.b_allocation_units_size = p[17];

	info.w_adj_factor_enahnced_1 = (p[0x30] << 8) |
				       p[0x31];

	/* decide LU size */
	d_num_alloc_units_boot_lu = ufs_aio_get_lu_cfg_size(hba, &info, UFS_LU_0);
	d_num_alloc_units_user_lu = ufs_aio_get_lu_cfg_size(hba, &info, UFS_LU_2);
#endif
#endif
	return UFS_ERR_NONE;
}

#if defined(MTK_UFS_DRV_PRELOADER)
int ufs_aio_rpmb_get_rw_size(void)
{
	struct ufs_hba *hba = &g_ufs_hba;
	int ret, size;
	u8 buf[QUERY_DESC_GEOMETRY_MAX_SIZE];

	ret = hba->read_descriptor(hba, QUERY_DESC_IDN_GEOMETRY, 0, 0, buf, QUERY_DESC_GEOMETRY_MAX_SIZE);

	if (ret) {
		UFS_DBG_LOGE("[UFS] err: ufs_aio_rpmb_get_rw_size: read geometry desc error: %d\n", ret);
		return ret;
	}

	size = (int)buf[GEOMETRY_DESC_PARAM_RPMB_READ_WRITE_SIZE];

	UFS_DBG_LOGD("[UFS] rpmb: rw size: %d frames\n", size);

	if (hba->dev_quirks & UFS_DEVICE_QUIRK_LIMITED_RPMB_MAX_RW_SIZE) {
		if (size > UFS_RPMB_DEV_MAX_RW_SIZE_LIMITATION) {
			size = UFS_RPMB_DEV_MAX_RW_SIZE_LIMITATION;

			UFS_DBG_LOGD("[UFS] rpmb: rw size: %d frames (limitation)\n", size);
		}
	}

	return size;
}

u64 ufs_aio_rpmb_get_lu_size(void)
{
	struct ufs_hba *hba = &g_ufs_hba;
	int ret, i;
	u8 buf[QUERY_DESC_UNIT_MAX_SIZE];
	u8 logical_blk_size;
	u32 logical_blk_size_bytes;
	u64 logical_blk_cnt;

	ret = hba->read_descriptor(hba, QUERY_DESC_IDN_UNIT, UFS_UPIU_RPMB_WLUN, 0, buf, QUERY_DESC_UNIT_MAX_SIZE);

	if (ret) {
		UFS_DBG_LOGE("[UFS] err: ufs_aio_rpmb_get_lu_size: read rpmb unit desc error: %d\n", ret);
		return ret;
	}

	logical_blk_size = buf[UNIT_DESC_PARAM_LOGICAL_BLK_SIZE];

	for (i = 0, logical_blk_size_bytes = 1; i < logical_blk_size; i++)
		logical_blk_size_bytes = logical_blk_size_bytes << 1;

	UFS_DBG_LOGD("[UFS] rpmb: logical_blk_size: %d bytes\n", (int)logical_blk_size_bytes);

	logical_blk_cnt = ((u64)buf[UNIT_DESC_PARAM_LOGICAL_BLK_COUNT] << 56) |
	                  ((u64)buf[UNIT_DESC_PARAM_LOGICAL_BLK_COUNT + 1] << 48) |
	                  ((u64)buf[UNIT_DESC_PARAM_LOGICAL_BLK_COUNT + 2] << 40) |
	                  ((u64)buf[UNIT_DESC_PARAM_LOGICAL_BLK_COUNT + 3] << 32) |
	                  ((u64)buf[UNIT_DESC_PARAM_LOGICAL_BLK_COUNT + 4] << 24) |
	                  ((u64)buf[UNIT_DESC_PARAM_LOGICAL_BLK_COUNT + 5] << 16) |
	                  ((u64)buf[UNIT_DESC_PARAM_LOGICAL_BLK_COUNT + 6] << 8) |
	                  ((u64)buf[UNIT_DESC_PARAM_LOGICAL_BLK_COUNT + 7]);

	UFS_DBG_LOGD("[UFS] rpmb: logical_blk_cnt: 0x%llx\n", logical_blk_cnt);

	return (u64)logical_blk_size_bytes * logical_blk_cnt;
}
#endif

int ufs_aio_configure_new_ufs(struct ufs_hba *hba)
{
	int ret;
	u32 i;
	u8 *p;
	u32 d_num_alloc_units_boot_lu;
	u32 d_num_alloc_units_user_lu;
	struct ufs_geometry_info info = {0};

	/* HPB support, read geometry descriptor with HPB, selector = 1 */
	if ((hba->dev_info.hpb_support) && (hba->dev_info.wmanufacturerid == UFS_VENDOR_SAMSUNG || hba->dev_info.wmanufacturerid == UFS_VENDOR_MICRON_MP))
		ret = hba->read_descriptor(hba, QUERY_DESC_IDN_GEOMETRY, 0, 1, (u8 *)&g_ufs_temp_buf[0], ufs_query_desc_max_size[QUERY_DESC_IDN_GEOMETRY]);
	else
		ret = hba->read_descriptor(hba, QUERY_DESC_IDN_GEOMETRY, 0, 0, (u8 *)&g_ufs_temp_buf[0], ufs_query_desc_max_size[QUERY_DESC_IDN_GEOMETRY]);

	if (0 != ret) {
		UFS_DBG_LOGE("[UFS] err: ufs_aio_configure_new_ufs: read geometry desc error: %d\n", ret);
		return ret;
	}

	UFS_DBG_LOGE("[UFS] %s: Geometry Desc:\n", __func__);
	ufs_aio_util_dump_table_hex(&g_ufs_temp_buf[0], ufs_query_desc_max_size[QUERY_DESC_IDN_GEOMETRY]);

	p = (u8 *)&g_ufs_temp_buf[0];

	info.q_total_raw_device_capacity = ((u64)p[4] << 56) |
					   ((u64)p[5] << 48) |
					   ((u64)p[6] << 40) |
					   ((u64)p[7] << 32) |
					   ((u64)p[8] << 24) |
					   ((u64)p[9] << 16) |
					   ((u64)p[10] << 8) |
					   (u64)p[11];

	info.d_segment_size = (p[13] << 24) |
			      (p[14] << 16) |
			      (p[15] << 8) |
			      p[16];

	info.b_allocation_units_size = p[17];

	info.w_adj_factor_enahnced_1 = (p[0x30] << 8) |
				       p[0x31];

	if (hba->dev_info.hpb_support) {
		info.q_hpb_region_size = (u64)1 << p[0x48];
		info.w_hpb_device_max_active_regions = (p[0x4B] << 8) | p[0x4C];
		UFS_DBG_LOGI("[UFS] HPB device max active regions=0x%x, region size=0x%llx\n",
			info.w_hpb_device_max_active_regions, info.q_hpb_region_size);
	}

	if (hba->dev_info.tw_support) {
		info.d_tw_buf_max_au = (p[0x4F] << 24)| (p[0x50] << 16) | (p[0x51] << 8) | p[0x52];
		info.b_tw_buf_adj_fac = p[0x54];
		info.b_tw_support_red_type = p[0x55];
		info.b_tw_support_buf_type = p[0x56];
		UFS_DBG_LOGI("[UFS] TW max AU=0x%x, TW adj=%d, TW reduction=%d, TW buffer type=%d\n",
			info.d_tw_buf_max_au, info.b_tw_buf_adj_fac,
			info.b_tw_support_red_type, info.b_tw_support_buf_type);
	}

	/* prepare configuration descriptor */
	memset(&g_ufs_temp_buf[0], 0, QUERY_DESC_CONFIGURAION_MAX_SIZE_HPB_20);

	p[CONF_DESC_B_LENGTH]       = ufs_query_desc_max_size[QUERY_DESC_IDN_CONFIGURATION];

	p[CONF_DESC_B_DESCRIPTOR_TYPE]      = QUERY_DESC_IDN_CONFIGURATION;
	p[CONF_DESC_B_BOOT_ENABLE]          = 1;
	p[CONF_DESC_B_DESCR_ACCESS_EN]      = 1;
	p[CONF_DESC_B_INIT_POWER_MODE]      = 1;
	p[CONF_DESC_B_HIGH_PRIORITY_LUN]    = 0x7F;    /* no high priority LU */


	/* decide LU size */
	d_num_alloc_units_boot_lu = ufs_aio_get_lu_cfg_size(hba, &info, UFS_LU_0);
	d_num_alloc_units_user_lu = ufs_aio_get_lu_cfg_size(hba, &info, UFS_LU_2);

	if (hba->dev_info.hpb_support)
		p[CONF_DESC_B_HPB_CINTROL] = 1; /* Device Control */

	if (hba->dev_info.tw_support) {
		p[CONF_DESC_B_TW_BUF_NO_RED_EN] = hba->custom_info.tw_no_red; /* user space reduction */

		if (info.b_tw_support_buf_type == 0x2) /* Support both */
			p[CONF_DESC_B_TW_BUF_TYPE] = 0; /* LU base WB buffer */
		else /* Only support LU base WB buffer(0), single share(1) */
			p[CONF_DESC_B_TW_BUF_TYPE] = info.b_tw_support_buf_type;

		/* JEDEC version */
		if (hba->dev_info.wmanufacturerid != UFS_VENDOR_SAMSUNG) {
			p[CONF_DESC_D_NUM_SHARED_WB_BUF_AU] = (info.d_tw_buf_au >> 24) & 0xFF;
			p[CONF_DESC_D_NUM_SHARED_WB_BUF_AU + 1] = (info.d_tw_buf_au >> 16) & 0xFF;
			p[CONF_DESC_D_NUM_SHARED_WB_BUF_AU + 2] = (info.d_tw_buf_au >> 8) & 0xFF;
			p[CONF_DESC_D_NUM_SHARED_WB_BUF_AU + 3] = info.d_tw_buf_au & 0xFF;
		}
	}

	for (i = 0; i < 8; i++) { /* traverse 8 Unit Descriptor configurable parameters */
		if (i == 0)
			p += hba->dev_info.ud0_base_offset;
		else
			p += hba->dev_info.ud_config_len;

		/* enable LU0 (boot 1), LU1 (boot 2), LU2 (user) only */
		if (i >= 3) {

			if (hba->dev_quirks & UFS_DEVICE_QUIRK_NEED_CONFIGURATION_FOR_UNUSED_LU) {
				/*
				 * Some device does not allow 0x0 in LOGICAL_BLOCK_SIZE even for unused LU.
				 *
				 * Device vendor shall fix this to ignore any checking for unused LU.
				 * Apply this quirk here for development purpose.
				 */
				p[CONF_DESC_UNIT_B_LOGICAL_BLOCK_SIZE]  = 0xC;  /* 4 KB */
			}

			continue;
		}

		/* common configurations for all LUs */
		if ((i == 2) && (hba->dev_info.hpb_support)) {
			 /* Enable HPB */
			p[CONF_DESC_UNIT_B_LU_ENABLE] = 2;
			p[CONF_DESC_UNIT_W_MAX_ACTIVE_HPB_REGIONS] = (info.w_hpb_lu_max_active_regions >> 8) & 0xFF;
			p[CONF_DESC_UNIT_W_MAX_ACTIVE_HPB_REGIONS + 1] = info.w_hpb_lu_max_active_regions & 0xFF;
			p[CONF_DESC_UNIT_W_HPB_PINNED_REGION_START_IDX] = 0;
			p[CONF_DESC_UNIT_W_HPB_PINNED_REGION_START_IDX + 1] = 0;
			p[CONF_DESC_UNIT_W_NUM_HPB_PINNED_REGIONS] = 0;
			p[CONF_DESC_UNIT_W_NUM_HPB_PINNED_REGIONS + 1] = 0;
		} else {
			p[CONF_DESC_UNIT_B_LU_ENABLE] = 1;
		}

		if ((i == 2) && (hba->dev_info.tw_support)) {
			p[CONF_DESC_UNIT_D_NUM_TW_BUF_AU] = (info.d_tw_buf_au >> 24) & 0xFF;
			p[CONF_DESC_UNIT_D_NUM_TW_BUF_AU + 1] = (info.d_tw_buf_au >> 16) & 0xFF;
			p[CONF_DESC_UNIT_D_NUM_TW_BUF_AU + 2] = (info.d_tw_buf_au >> 8) & 0xFF;
			p[CONF_DESC_UNIT_D_NUM_TW_BUF_AU + 3] = info.d_tw_buf_au & 0xFF;
		}

		p[CONF_DESC_UNIT_B_LOGICAL_BLOCK_SIZE]  = 0xC;  /* 4 KB */
		p[CONF_DESC_UNIT_B_PROVISIONING_TYPE]   = 0x2;  /* thin-provisioning with TPRZ = 0 */

		if (i < 2) { /* boot LU */
			p[CONF_DESC_UNIT_B_DATA_RELIABILITY] = 1;
			p[CONF_DESC_UNIT_B_BOOT_LUN_ID] = i + 1;    /* LU0 = Boot A, LU1 = Boot B */

			/*
			 * TODO: Set Memory Type of Boot LU according to different vendors
			 * SK-Hynix/Samsung: 3
			 * Toshiba: 4
			 */

			p[CONF_DESC_UNIT_B_MEMORY_TYPE] = UFS_MEMORY_TYPE_ENHANCED_1;
			p[CONF_DESC_UNIT_D_NUM_ALLOC_UNITS]     = (d_num_alloc_units_boot_lu >> 24) & 0xFF;
			p[CONF_DESC_UNIT_D_NUM_ALLOC_UNITS + 1] = (d_num_alloc_units_boot_lu >> 16) & 0xFF;
			p[CONF_DESC_UNIT_D_NUM_ALLOC_UNITS + 2] = (d_num_alloc_units_boot_lu >> 8) & 0xFF;
			p[CONF_DESC_UNIT_D_NUM_ALLOC_UNITS + 3] = d_num_alloc_units_boot_lu & 0xFF;
		} else {    /* non-boot LU (user LU) */
			p[CONF_DESC_UNIT_B_MEMORY_TYPE] = UFS_MEMORY_TYPE_NORMAL;
			p[CONF_DESC_UNIT_D_NUM_ALLOC_UNITS]     = (d_num_alloc_units_user_lu >> 24) & 0xFF;
			p[CONF_DESC_UNIT_D_NUM_ALLOC_UNITS + 1] = (d_num_alloc_units_user_lu >> 16) & 0xFF;
			p[CONF_DESC_UNIT_D_NUM_ALLOC_UNITS + 2] = (d_num_alloc_units_user_lu >> 8) & 0xFF;
			p[CONF_DESC_UNIT_D_NUM_ALLOC_UNITS + 3] = d_num_alloc_units_user_lu & 0xFF;
		}
	}

#ifdef MTK_UFS_DRV_DA
	hba->unit_desc_cfg_param_valid = 0;
#endif

	UFS_DBG_LOGE("[UFS] %s: new Configuration Desc:\n", __func__);

	/* HPB support, write configuration descriptor with HPB, selector = 1, length = 0xD0(HPB1.0)/0xE2(HPB2.0) */
	ufs_aio_util_dump_config_table_hex(&g_ufs_temp_buf[0], ufs_query_desc_max_size[QUERY_DESC_IDN_CONFIGURATION], hba->dev_info.ud0_base_offset, hba->dev_info.ud_config_len);
	if ((hba->dev_info.hpb_support) && (hba->dev_info.wmanufacturerid == UFS_VENDOR_SAMSUNG || hba->dev_info.wmanufacturerid == UFS_VENDOR_MICRON_MP))
		ret = hba->write_descriptor(hba, QUERY_DESC_IDN_CONFIGURATION, 0, 1, &g_ufs_temp_buf[0], ufs_query_desc_max_size[QUERY_DESC_IDN_CONFIGURATION]);
	else
		ret = hba->write_descriptor(hba, QUERY_DESC_IDN_CONFIGURATION, 0, 0, &g_ufs_temp_buf[0], ufs_query_desc_max_size[QUERY_DESC_IDN_CONFIGURATION]);

	if (ret) {
		UFS_DBG_LOGE("[UFS] err: ufs_aio_configure_new_ufs: write conf desc error: %d (0x%x)\n", ret, ret);
		return ret;
	} else
		UFS_DBG_LOGD("[UFS] info: ufs_aio_configure_new_ufs: write conf desc done\n");

	ret = ufs_aio_set_boot_lu(hba, ATTR_B_BOOT_LUN_EN_BOOT_LU_A);

	if (ret)
		return ret;

	// lock down configuration descriptor if required
	if (hba->dev_quirks & UFS_DEVICE_QUIRK_NEED_LOCK_CONFIG_DESC)
	{
		i = 0x1;

		if ((ret = (hba->query_attr(hba, UPIU_QUERY_OPCODE_WRITE_ATTR, ATTR_B_CONFIG_DESCR_LOCK, 0, 0, &i))))
			return ret;
	}

	return UFS_ERR_NONE;
}

static int ufs_aio_prepare_new_ufs(struct ufs_hba *hba)
{
	int ret;

	/* configure this new UFS device */

	UFS_DBG_LOGI("[UFS] info: new UFS is found, configuring it ...\n");

	ret = ufs_aio_configure_new_ufs(hba);

	if (UFS_ERR_NONE != ret) {
		UFS_DBG_LOGE("[UFS] err: ufs_aio_configure_new_ufs fail\n");
		return ret;
	}

	UFS_DBG_LOGI("[UFS] info: new UFS configuration done\n");

	/* reset device to apply new configurations */
#ifdef UFS_CFG_DEVICE_RESET_NONPROTECTED

	ufs_aio_reset_device(hba);
	return UFS_ERR_NEED_REINIT_HOST;

#else

	/* trigger watchdog reset */

#endif
	return UFS_ERR_NONE;	/* shall not happen */
}

struct ufs_hba *ufs_aio_get_host(u8 host_id)
{
	return &g_ufs_hba;
}

int ufs_aio_get_device_info(struct ufs_hba *hba)
{
	struct ufs_aio_scsi_cmd cmd;
	struct ufs_device_info *card_data;
	int err;
	int tag;
	u8 *p;
	int str_len;
	int sn, i;
	char *bp;

	card_data = &hba->dev_info;

	/* Have init and not reconfing device */
	if ((card_data->wmanufacturerid != 0) && (card_data->bootable == 1))
		return 0;

	memset(card_data, 0, sizeof(struct ufs_device_info));

	err = hba->read_descriptor(hba, QUERY_DESC_IDN_DEVICE, 0, 0, (u8 *)&g_ufs_temp_buf[0], ufs_query_desc_max_size[QUERY_DESC_IDN_DEVICE]);

	if (err) {
		UFS_DBG_LOGE("[UFS] err: ufs_aio_get_device_info: failed reading Device Desc. err = %d\n", err);
		return -1;
	}

	p = (u8 *)&g_ufs_temp_buf[0];

	card_data->ufs_ver = p[DEVICE_DESC_PARAM_SPEC_VER] << 8 |
				     p[DEVICE_DESC_PARAM_SPEC_VER + 1];
	UFS_DBG_LOGI("[UFS] UFS version %x.%x\n", p[DEVICE_DESC_PARAM_SPEC_VER], p[DEVICE_DESC_PARAM_SPEC_VER + 1]);
	UFS_DBG_LOGI("[UFS] Device Desc:\n");
	ufs_aio_util_dump_table_hex(p, ufs_query_desc_max_size[QUERY_DESC_IDN_DEVICE]);

	if (p[DEVICE_DESC_PARAM_BOOT_ENBL] == 0)
		card_data->bootable = 0;
	else
		card_data->bootable = 1;

	/*
	 * getting vendor (manufacturerID) and Bank Index in big endian format
	 */
	card_data->wmanufacturerid = p[DEVICE_DESC_PARAM_MANF_ID] << 8 |
				     p[DEVICE_DESC_PARAM_MANF_ID + 1];


	if (card_data->wmanufacturerid == UFS_VENDOR_MICRON_MP) {
		err = ufshcd_init_vendor_desc_sizes(hba); /* micron need use selector = 1 to read hpb support */
		if (err) /* not support selector = 1 */
			goto skip_selector;
		err = hba->read_descriptor(hba, QUERY_DESC_IDN_DEVICE, 0, 1,
			(u8 *)&g_ufs_temp_buf[0],
			ufs_query_desc_max_size[QUERY_DESC_IDN_DEVICE]);

		if (err) {
			UFS_DBG_LOGE("[UFS] err: ufs_aio_get_device_info: failed reading HPB Device Desc. err = %d\n", err);
			return -1;
		}

		UFS_DBG_LOGD("[UFS] %s: Device Desc:\n", __func__);
		ufs_aio_util_dump_table_hex(p, ufs_query_desc_max_size[QUERY_DESC_IDN_DEVICE]);
	}

skip_selector:
	/* Check support HPB feature or not */
	if (p[DEVICE_DESC_PARAM_FEAT_SUP] & 0x80)
		card_data->hpb_support = 1;
	else
		card_data->hpb_support = 0;

	if (card_data->hpb_support) {
		/* Samsung version re-read device descriptor with HPB selector = 1 */
		if (card_data->wmanufacturerid == UFS_VENDOR_SAMSUNG) {
			err = ufshcd_init_vendor_desc_sizes(hba); /* only init when hpb support */
			if (err) /* not support selector = 1 */
				goto skip_selector2;
			err = hba->read_descriptor(hba, QUERY_DESC_IDN_DEVICE, 0, 1,
				(u8 *)&g_ufs_temp_buf[0],
				ufs_query_desc_max_size[QUERY_DESC_IDN_DEVICE]);

			if (err) {
				UFS_DBG_LOGE("[UFS] err: ufs_aio_get_device_info: failed reading HPB Device Desc. err = %d\n", err);
				return -1;
			}

			UFS_DBG_LOGD("[UFS] %s: Device Desc:\n", __func__);
			ufs_aio_util_dump_table_hex(p, ufs_query_desc_max_size[QUERY_DESC_IDN_DEVICE]);
		}

skip_selector2:
		card_data->hpb_ver = (p[DEVICE_DESC_PARAM_HPB_VER] << 8) |
			p[DEVICE_DESC_PARAM_HPB_VER + 1];

		UFS_DBG_LOGI("HPB Version = %x.%x.%x\n",
			(card_data->hpb_ver >> 8) & 0xFF,
			(card_data->hpb_ver >> 4) & 0xF,
			card_data->hpb_ver & 0xF);
	}

	/* Check support TW feature or not */
	if (p[DEVICE_DESC_PARAM_EXT_FEAT_SUP + 2] & 0x1)
		card_data->tw_support = 1;
	else
		card_data->tw_support = 0;

	if (card_data->tw_support) {
		card_data->tw_red = p[DEVICE_DESC_PARAM_TW_BUF_USER_REDUCED];
		card_data->tw_type = p[DEVICE_DESC_PARAM_TW_BUF_TYPE];
		UFS_DBG_LOGI("TW Reduction = %d, TW Type = %d\n",
			card_data->tw_red,card_data->tw_type);

		if (card_data->wmanufacturerid == UFS_VENDOR_SAMSUNG) {
			card_data->tw_ver = (p[DEVICE_DESC_PARAM_TW_VER] << 8) |
				p[DEVICE_DESC_PARAM_TW_VER + 1];
			UFS_DBG_LOGI("TW Version = %x.%x.%x\n",
				(card_data->tw_ver >> 8) & 0xFF,
				(card_data->tw_ver >> 4) & 0xF,
				card_data->tw_ver & 0xF);
		} else {
			card_data->wb_buf_au = (p[DEVICE_DESC_PARAM_NUM_SHARED_WB_BUF_AU] << 24) |
					       (p[DEVICE_DESC_PARAM_NUM_SHARED_WB_BUF_AU + 1] << 16) |
					       (p[DEVICE_DESC_PARAM_NUM_SHARED_WB_BUF_AU + 2] << 5) |
					       (p[DEVICE_DESC_PARAM_NUM_SHARED_WB_BUF_AU + 3]);
			UFS_DBG_LOGI("Shared WB Buffer AU = 0x%x\n", card_data->wb_buf_au);
		}
	}

	/* Normal device:(0x10, 0x10) / HPB device:(0x10, 0x18) */
	card_data->ud0_base_offset = p[DEVICE_DESC_PARAM_UD_OFFSET];
	card_data->ud_config_len = p[DEVICE_DESC_PARAM_UD_LEN];
	UFS_DBG_LOGI("bbUD0BaseOffset = 0x%d, bUDConfigPLength = 0x%d\n", card_data->ud0_base_offset, card_data->ud_config_len);

	/* Serial number */
	sn = p[DEVICE_DESC_PARAM_SN];
	memset(&g_ufs_temp_buf[0], 0, QUERY_DESC_STRING_MAX_SIZE);
	err = hba->read_descriptor(hba, QUERY_DESC_IDN_STRING, sn, 0,
		(u8 *)&g_ufs_temp_buf[0], QUERY_DESC_STRING_MAX_SIZE);
	if (err) {
		UFS_DBG_LOGE("[UFS] err: read string desc error: %d\n", err);
	} else {
		p = (u8 *)&g_ufs_temp_buf[0];
		card_data->serial_number_len = p[0] - 2;
		UFS_DBG_LOGI("[UFS] String Desc: Serial Number INDEX %d, LENGTH %d\n",
			sn, card_data->serial_number_len);
		ufs_aio_util_dump_table_hex(p, p[0]);

		if (card_data->serial_number_len < MAX_SERAL_NUMBER_LEN) {
			bp = card_data->serial_number;

			for (i = 0; i < card_data->serial_number_len; i++)
				itoa(&bp, (int) p[i+2], 16);
			UFS_DBG_LOGI("[UFS] Serial Number: %s\n", card_data->serial_number);
		} else {
			UFS_DBG_LOGE("[UFS] Serial Number size over reserved 64\n");
		}
	}

#if defined(MTK_UFS_DRV_DA)
	/* Device Health */
	memset(&g_ufs_temp_buf[0], 0, QUERY_DESC_HEALTH_MAX_SIZE);
	err = hba->read_descriptor(hba, QUERY_DESC_IDN_HEALTH, 0, 0,
		(u8 *)&g_ufs_temp_buf[0], QUERY_DESC_HEALTH_MAX_SIZE);
	if (err) {
		UFS_DBG_LOGE("[UFS] err: read health desc error: %d\n", err);
	} else {
		p = (u8 *)&g_ufs_temp_buf[0];
		card_data->pre_eol_info = p[HEALTH_DESC_B_PRE_EOL_INFO];
		card_data->life_time_est_a = p[HEALTH_DESC_B_DEVICE_LIFE_TIME_EST_A];
		card_data->life_time_est_b = p[HEALTH_DESC_B_DEVICE_LIFE_TIME_EST_B];
		UFS_DBG_LOGI("[UFS] life cycle check, pre_eol_info=%d, life_time_est_a=%d, life_time_est_b=%d\n",
			card_data->pre_eol_info, card_data->life_time_est_a, card_data->life_time_est_b);
	}
#endif

	/* getting active LU number */
	/* card_data->num_active_lu = p[DEVICE_DESC_PARAM_NUM_LU]; */

	if (!ufshcd_get_free_tag(hba, &tag))
		return -1;

	ufs_aio_scsi_cmd_inquiry(&cmd, tag, 0, (unsigned long *)&g_ufs_temp_buf[0]);
	err = ufshcd_queuecommand(hba, &cmd);
	if (err) {
		UFS_DBG_LOGD("[UFS] err: ufshcd_queuecommand err\n");
		return -1;
	}

	ufshcd_put_tag(hba, tag);

	p = (u8 *)&g_ufs_temp_buf[0];

	/*
	 * get product ID and product revision level (fw ver)
	 * by INQUIRY command
	 */

	/* product ID */
	str_len = ufs_util_sanitize_inquiry_string(&p[16], 16);
	if (str_len) {
		UFS_DBG_LOGD("[UFS] get product id string len 0x%x\n", str_len);
		memcpy(card_data->product_id, &p[16], str_len);
	}

	/* product revision level */
	str_len = ufs_util_sanitize_inquiry_string(&p[32], 4);
	if (str_len) {
		UFS_DBG_LOGD("[UFS] get revision level string len 0x%x\n", str_len);
		memcpy(card_data->product_revision_level, &p[32], str_len);
	}

	return 0;
}

#if defined(MTK_UFS_DRV_DA)
int ufs_aio_get_device_buf_alignment_req(struct ufs_hba *hba)
{
	if (hba->dev_quirks & UFS_DEVICE_QUIRK_NEED_8_BYTE_ALIGNED_BUF)
		return 8;
	else
		return 4;
}

int ufs_aio_read_unit_desc_cfg_param(struct ufs_hba *hba)
{
	int ret;
	u8 *p;
	int i;

	if (1 == hba->unit_desc_cfg_param_valid)
		return 0;

	/* HPB support, read configuration descriptor with HPB, selector = 1, length = 0xD0(HPB1.0)/0xE2(HPB2.0) */
	if ((hba->dev_info.hpb_support) && (hba->dev_info.wmanufacturerid == UFS_VENDOR_SAMSUNG || hba->dev_info.wmanufacturerid == UFS_VENDOR_MICRON_MP))
		ret = hba->read_descriptor(hba, QUERY_DESC_IDN_CONFIGURATION, 0, 1, (u8 *)&g_ufs_temp_buf[0], ufs_query_desc_max_size[QUERY_DESC_IDN_CONFIGURATION]);
	else
		ret = hba->read_descriptor(hba, QUERY_DESC_IDN_CONFIGURATION, 0, 0, (u8 *)&g_ufs_temp_buf[0], ufs_query_desc_max_size[QUERY_DESC_IDN_CONFIGURATION]);

	if (0 != ret) {
		UFS_DBG_LOGD("[UFS] err: ufs_aio_read_unit_desc_cfg_param read desc error: %d\n", ret);
		return ret;
	}

	p = (u8 *)&g_ufs_temp_buf[0];

	if (((p[0] != ufs_query_desc_max_size[QUERY_DESC_IDN_CONFIGURATION]) && (p[0] != QUERY_DESC_CONFIGURAION_MAX_SIZE_HPB) && (p[0] != QUERY_DESC_CONFIGURAION_MAX_SIZE_HPB_20)) ||
		(p[1] != 0x1)) {
		UFS_DBG_LOGD("[UFS] err: invalid configuration descriptor, len: %d, id: %d\n", p[0], p[1]);
		return -1;
	}

	for (i = 0; i < UFS_UPIU_MAX_GENERAL_LUN; i++) {
		memcpy(&hba->unit_desc_cfg_param[i],
			(p + hba->dev_info.ud0_base_offset + (i * hba->dev_info.ud_config_len)),
			16);
	}

	hba->unit_desc_cfg_param_valid = 1;

	return 0;
}

#endif	/* MTK_UFS_DRV_DA */

int ufs_aio_get_lu_size(struct ufs_hba *hba, u32 lun, u32 *blk_size_in_byte, u32 *blk_cnt)
{
	struct ufs_aio_scsi_cmd cmd;
	int tag;
	int ret;
	u8 *p;
	u32 blk_cnt_int, blk_size_int;

	if (!ufshcd_get_free_tag(hba, &tag))
		return -1;

	ufs_aio_scsi_cmd_read_capacity(&cmd, (u32)tag, lun, (unsigned long *)&g_ufs_temp_buf[0]);
	ret = ufshcd_queuecommand(hba, &cmd);

	if (UFS_ERR_NONE != ret) {
		UFS_DBG_LOGE("[UFS] err: ufs_aio_get_lu_size: ufshcd_queuecommand err\n");
		return ret;
	}

	ufshcd_put_tag(hba, tag);

	p = (u8 *)&g_ufs_temp_buf[0];

	blk_cnt_int = (((u32)(p[0]) << 24) | ((u32)(p[1]) << 16) | ((u32)(p[2]) << 8) | (u32)(p[3])) + 1;

	if (blk_cnt)
		*blk_cnt = blk_cnt_int;

	blk_size_int = ((u32)(p[4]) << 24) | ((u32)(p[5]) << 16) | ((u32)(p[6]) << 8) | (u32)(p[7]);

	if (blk_size_in_byte)
		*blk_size_in_byte = blk_size_int;

	UFS_DBG_LOGE("[UFS] get lu size: lun=%d, blk_cnt=%d, blk_size_in_byte=%d\n",
			lun, blk_cnt_int, blk_size_int);

	return ret;
}

void ufs_aio_dbg_show_reg(struct ufs_hba *hba, u32 addr)
{
#if (UFS_DBG_LVL <= UFS_DBG_LVL_DEBUG)
	u32 data;

	data = ufshcd_readl(hba, addr);

	UFS_DBG_LOGD("\t: 0x%x, 0x%x\n", addr, data);
#endif
}

static void ufs_aio_dma_map(ufs_vaddr_t buf, ufs_size_t len, enum dma_data_direction dir)
{
	if (DMA_FROM_DEVICE == dir) {           /* read from device (or write to memory) */
#if defined(MTK_UFS_DRV_DA) || defined(MTK_UFS_DRV_LK)

		arch_clean_invalidate_cache_range((ufs_vaddr_t)buf, (ufs_size_t)len);

#elif defined(MTK_UFS_DRV_PRELOADER)

#if CFG_ENABLE_DCACHE
		plat_clean_invalidate_dcache();
#endif

#elif defined(MTK_UFS_DRV_CTP)

		cache_clean_invalidate();

#else

	#error "MUST PORTING ufs_aio_dma_map()\n"

#endif
	} else {                                /* bi-directional or read from memory (or write to device) */
#if defined(MTK_UFS_DRV_DA) || defined(MTK_UFS_DRV_LK)

		arch_sync_cache_range((ufs_vaddr_t)buf, (ufs_size_t)len);

#elif defined(MTK_UFS_DRV_PRELOADER)

#if CFG_ENABLE_DCACHE
		plat_clean_invalidate_dcache();
#endif

#elif defined(MTK_UFS_DRV_CTP)

		cache_clean();

#else

	#error "MUST PORTING ufs_aio_dma_map()\n"

#endif
	}
}

static void ufs_aio_dma_unmap(ufs_vaddr_t buf, ufs_size_t len, enum dma_data_direction dir)
{
	if (DMA_TO_DEVICE != dir) {             /* bi-directional or read from device (or write to memory) */
#if defined(MTK_UFS_DRV_DA) || defined(MTK_UFS_DRV_LK)

		arch_clean_invalidate_cache_range((ufs_vaddr_t)buf, (ufs_size_t)len);

#elif defined(MTK_UFS_DRV_PRELOADER)

#if CFG_ENABLE_DCACHE
		plat_clean_invalidate_dcache();
#endif

#elif defined(MTK_UFS_DRV_CTP)

		cache_clean_invalidate();

#endif
	}
}

int ufs_aio_dma_dme_get(struct ufs_hba *hba, u32 attr_sel, u32 *mib_val)
{
	return ufshcd_dme_get_attr(hba, attr_sel, mib_val, DME_LOCAL);
}

int ufs_aio_dma_dme_peer_get(struct ufs_hba *hba, u32 attr_sel, u32 *mib_val)
{
	return ufshcd_dme_get_attr(hba, attr_sel, mib_val, DME_PEER);
}

int ufs_aio_dma_dme_set(struct ufs_hba *hba, u32 attr_sel, u32 mib_val)
{
	return ufshcd_dme_set_attr(hba, attr_sel, ATTR_SET_NOR, mib_val, DME_LOCAL);
}

int ufs_aio_dma_dme_peer_set(struct ufs_hba *hba, u32 attr_sel, u32 mib_val)
{
	return ufshcd_dme_set_attr(hba, attr_sel, ATTR_SET_NOR, mib_val, DME_PEER);
}

int ufs_aio_dma_nopin_nopout(struct ufs_hba *hba)
{
	int err = 0;
	int retries;

	for (retries = NOP_OUT_RETRIES; retries > 0; retries--) {
		err = ufshcd_exec_dev_cmd(hba, DEV_CMD_TYPE_NOP,
					  NOP_OUT_TIMEOUT);

		if (!err)
			break;
	}

	if (err)
		UFS_DBG_LOGE("[UFS] err: NOP OUT failed\n");

	return err;
}

int ufs_aio_dma_query_flag(struct ufs_hba *hba, enum query_opcode opcode,
			   enum flag_idn idn, bool *flag_res)
{
	struct ufs_query_req *request = NULL;
	struct ufs_query_res *response = NULL;
	int err, index = 0, selector = 0;

	BUG_ON(!hba);

	ufshcd_init_query(hba, &request, &response, opcode, idn, index,
			  selector);

	switch (opcode) {
	case UPIU_QUERY_OPCODE_SET_FLAG:
	case UPIU_QUERY_OPCODE_CLEAR_FLAG:
	case UPIU_QUERY_OPCODE_TOGGLE_FLAG:
		request->query_func = UPIU_QUERY_FUNC_STANDARD_WRITE_REQUEST;
		break;
	case UPIU_QUERY_OPCODE_READ_FLAG:
		request->query_func = UPIU_QUERY_FUNC_STANDARD_READ_REQUEST;
		if (!flag_res) {
			/* No dummy reads */
			UFS_DBG_LOGD("Invalid argument for read request\n");
			err = -1;
			goto out_unlock;
		}
		break;
	default:
		UFS_DBG_LOGD("Expected query flag opcode but got = %d\n", opcode);
		err = -1;
		goto out_unlock;
	}

	err = ufshcd_exec_dev_cmd(hba, DEV_CMD_TYPE_QUERY, QUERY_REQ_TIMEOUT);

	if (err) {
		UFS_DBG_LOGD("Sending flag query for idn %d failed, err = %d\n", idn, err);
		goto out_unlock;
	}

	if (flag_res)
		*flag_res = (be32_to_cpu(response->upiu_res.value) &
			     MASK_QUERY_UPIU_FLAG_LOC) & 0x1;

out_unlock:

	return err;
}

int ufs_aio_dma_query_attr(struct ufs_hba *hba, enum query_opcode opcode,
			   enum attr_idn idn, u8 index, u8 selector, u32 *attr_val)
{
	struct ufs_query_req *request = NULL;
	struct ufs_query_res *response = NULL;
	int err;

	if (!attr_val) {
		UFS_DBG_LOGD("attribute value required for opcode 0x%x\n", opcode);
		err = -1;
		goto out;
	}

	ufshcd_init_query(hba, &request, &response, opcode, idn, index,    selector);

	switch (opcode) {
	case UPIU_QUERY_OPCODE_WRITE_ATTR:
		request->query_func = UPIU_QUERY_FUNC_STANDARD_WRITE_REQUEST;
		request->upiu_req.value = cpu_to_be32(*attr_val);
		break;
	case UPIU_QUERY_OPCODE_READ_ATTR:
		request->query_func = UPIU_QUERY_FUNC_STANDARD_READ_REQUEST;
		break;
	default:
		UFS_DBG_LOGD("Expected query attr opcode but got = 0x%.2x\n", opcode);
		err = -1;
		goto out;
	}

	err = ufshcd_exec_dev_cmd(hba, DEV_CMD_TYPE_QUERY, QUERY_REQ_TIMEOUT);

	if (err) {
		UFS_DBG_LOGD("opcode 0x%x for idn %d failed, err = %d\n", opcode, idn, err);
		goto out;
	}

	*attr_val = be32_to_cpu(response->upiu_res.value);

out:

	return err;
}

int ufs_aio_dma_read_desc(struct ufs_hba *hba, enum desc_idn desc_id, int desc_index, u8 selector, u8 *buf, u32 size)
{
	return ufshcd_read_desc_param(hba, desc_id, desc_index, selector, buf, size);
}

int ufs_aio_dma_write_desc(struct ufs_hba *hba, enum desc_idn idn, int index, u8 selector, u8 *src_buf, u32 buf_len)
{
	u32 buf_len_local = buf_len;

	return ufshcd_query_descriptor(hba, UPIU_QUERY_OPCODE_WRITE_DESC, idn, index, selector, src_buf, &buf_len_local);
}

#if defined(UFS_CFG_CRYPTO)

u32 sha256_prebuilt[3][16] = {
	{ /*for 128 bit key*/
		0xa5a5a5a5, 0x5a5a5a5a, 0x55aa55aa, 0xaa55aa55,
		0x0,         0x0,    0x0,        0x0,
		0x9309da32, 0x87a0c7fc, 0x1b58a359, 0x14fc38ea,
		0x68c7b319, 0x0dcb6355, 0xa0e565e9, 0x8765ccb5,
	},
	{ /*for 192 bit key*/
		0xa5a5a5a5, 0x5a5a5a5a, 0x55aa55aa, 0xaa55aa55,
		0xa5a5a5a5, 0x5a5a5a5a, 0x0,        0x0,
		0xad168c0f, 0x502b6ae1, 0x30199ad0, 0x69488013,
		0x42659ad8, 0xf94f95ca, 0xf8084fc1, 0x7a2059e6,
	},
	{ /*for 256 bit key*/
		0xa5a5a5a5, 0x5a5a5a5a, 0x55aa55aa, 0xaa55aa55,
		0xa5a5a5a5, 0x5a5a5a5a, 0x55aa55aa, 0xaa55aa55,
		0x6a3d2ed7, 0x228ff865, 0x7ea361c0, 0x88610f0c,
		0x5f3f1c57, 0x8bc5316e, 0x12ab5119, 0x82e947aa,
	},
};

int ufs_aio_crypto_init(struct ufs_hba *hba)
{
	u32 addr, key_bits, key_bytes, cfg_ptr, alg_id, i;
	u32 key[16];
	union ufs_cap_cfg cpt_cfg;
	union ufs_cpt_cap cpt_cap;
	union ufs_cpt_capx cpt_capx;

	u32 cap_id = 7; /* AES-CBC-ESSIV-128bit */
	u32 cfg_id = 0; /* use slot 0 by default */

	/* in-line encryption feature enable */
	ufs_aio_writel(hba, (ufs_aio_readl(hba, REG_CONTROLLER_ENABLE) | (0x1 << 1)), REG_CONTROLLER_ENABLE);

	/* get algo id */
	cpt_capx.capx_raw = ufs_aio_readl(hba, REG_CRYPTO_CAPABILITY_X + (cap_id << 2));
	alg_id = cpt_capx.capx.alg_id;

	/* get cfg ptr */
	cpt_cap.cap_raw = ufs_aio_readl(hba, REG_CRYPTO_CAPABILITY);
	cfg_ptr = cpt_cap.cap.cfg_ptr;
	addr = (cfg_ptr << 8) + (u32)(cfg_id << 7);

	/* set default key */
	memset(key, 0x5A, sizeof(key));

	/* set crypto cfg */
	memset(&cpt_cfg, 0, sizeof(cpt_cfg));
	cpt_cfg.cfgx.cfg_en = 1;
	cpt_cfg.cfgx.cap_id = (u8)cap_id;
	cpt_cfg.cfgx.du_size = (1 << UFS_CRYPTO_DATA_UNIT_SIZE_4KB);

	/* apply key value according to different algorithms */
	switch (cpt_capx.capx.key_size) {
	case 1:
		key_bits = 128;
		break;
	case 2:
		key_bits = 192;
		break;
	case 3:
		key_bits = 256;
		break;
	case 4:
		key_bits = 512;
		break;
	default :
		key_bits = 128; // shall not happen
		break;
	}

	key_bytes = key_bits >> 3;  /* byte count*/

	if (UFS_CRYPTO_ALGO_AES_XTS == alg_id) {               // AES-XTS

		memcpy((void *)&(cpt_cfg.cfgx.key[0]), (void *)key, key_bytes);
		memcpy((void *)&(cpt_cfg.cfgx.key[8]), (void *)(key + 16), key_bytes);

	} else if (UFS_CRYPTO_ALGO_ESSIV_AES_CBC == alg_id) {  // AES-CBC-ESSIV

		if (128 == key_bits)
			memcpy((void *)&(cpt_cfg.cfgx.key[0]), (void *)sha256_prebuilt[0], 64);  // always copy 64 byte = 512 bits, will cover both key and hash(key)
		else if (192 == key_bits)
			memcpy((void *)&(cpt_cfg.cfgx.key[0]), (void *)sha256_prebuilt[1], 64);  // always copy 64 byte = 512 bits, will cover both key and hash(key)
		else if (256 == key_bits)
			memcpy((void *)&(cpt_cfg.cfgx.key[0]), (void *)sha256_prebuilt[2], 64);  // always copy 64 byte = 512 bits, will cover both key and hash(key)

	} else                                                  // AES-ECB
		memcpy((void *)&(cpt_cfg.cfgx.key[0]), (void *)key, key_bytes);

	for (i = 0; i < 32; i++)
		ufs_aio_writel(hba, cpt_cfg.cfgx_raw[i], (addr + i * 4));

	return 0;
}

#endif /* MTK_UFS_DRV_CTP */

#ifdef UFS_CFG_ENABLE_PIO
/**
 * PIO mode related API bodies
 */

/* TODO: ensure GPT4 is initialized */
int ufs_aio_pio_cport_direct_write(struct ufs_hba *hba, const unsigned char *data, unsigned long len, int eom, int retry_ms)
{
	u32 ctrl = 0, reg = 0;
	u32 val_low = 0, val_high = 0;

	int cnt = 0;
	int last = 0;
	int wordLen;
	int i = 0;
	u32 timeout_tick = 0, start_tick = 0;

	wordLen = 8;

	cnt = len / wordLen;
	last = len % wordLen;

	if (last != 0)
		return UFS_CPORT_DIR_ACC_ALIGN_8BYTE_ERR; /* should be 8-byte align in bootROM */

	UFS_DBG_LOGD("cnt is %d\n", cnt);

	if (cnt) {
		/* write data */
		for (i = 0; i < len ; i += wordLen) {
			if (data) {
				val_low = ((*(data + i)) << 24) | ((*(data + i + 1)) << 16) | ((*(data + i + 2)) << 8) | (*(data + i + 3));
				val_high = ((*(data + i + 4)) << 24) | ((*(data + i + 5)) << 16) | ((*(data + i + 6)) << 8) | (*(data + i + 7));
			} else {
				return UFS_CPORT_DIR_ACC_ERR;
			}

			/* write bit enable and control */
			ctrl = 0xFF00 | (1 << 28) | (1 << 29); /* all bytes enable, CADEN on, word alignment on */

			if ((i >= len-wordLen) && (last == 0) && eom)
				ctrl |= (1<<16);

			ufshcd_writel(hba, ctrl, REG_CDACFG);
			ufshcd_writel(hba, val_high, REG_CDATX1);
			ufshcd_writel(hba, val_low, REG_CDATX2);

			/* get timeout tick */
			#ifdef MTK_UFS_DRV_DA
			timeout_tick  = gpt4_time2tick_ms(retry_ms);
			start_tick    = gpt4_get_current_tick();
			#endif

			while (1) {
				reg = ufshcd_readl(hba, REG_CDASTA);

				if ((reg & (UFS_HCI_REGISTER_FLAG_CDARES | UFS_HCI_REGISTER_FLAG_CDABUSY)) == 0) {
					break;    /* ready and no error */
				} else if ((reg & UFS_HCI_REGISTER_FLAG_CDARES) && ((reg & UFS_HCI_REGISTER_FLAG_CDABUSY) == 0)) { /* ready but error */
					UFS_DBG_LOGD("%s #%d: CPort direct access error. error code: 0x%04x\n", __func__, __LINE__, (reg)>>20);
					return UFS_CPORT_DIR_ACC_ERR;

				} else if (gpt4_timeout_tick(start_tick, timeout_tick)) {
					UFS_DBG_LOGD("%s #%d: CPort direct access time out\n", __func__, __LINE__);
					return UFS_CPORT_DIR_ACC_ERR_TIMEOUT;
				}

			}

		}
	}
	return UFS_ERR_NONE;
}

int ufs_aio_pio_cport_direct_read(struct ufs_hba *hba, unsigned char *data, unsigned long buflen, u32 *plen, int retry_ms, bool isDummy, u32 real_byte)
{
	u32 ctrl = 0, reg = 0;
	u32 val_low_u = 0, val_high_u = 0;
	int i = 0;
	u32 buf_index = 0;
	u32 timeout_tick = 0, start_tick = 0;
	/* UFS_DBG_LOGD("Isdummy: %d   real_byte:%d read_bytes: %d\n", isDummy, real_byte, *byte_read); */

	*plen = 0;

	/* get timeout tick */
#ifdef MTK_UFS_DRV_DA
	timeout_tick  = gpt4_time2tick_ms(retry_ms);
	start_tick    = gpt4_get_current_tick();
#endif
	while (1) {
		reg = ufshcd_readl(hba, REG_CDASTA);

		if ((reg & UFS_HCI_REGISTER_FLAG_CDASTA)) {
			break;
		} else if (gpt4_timeout_tick(start_tick, timeout_tick)) {
			UFS_DBG_LOGD("%s #%d: CPort direct access time out\n", __func__, __LINE__);
			return UFS_CPORT_DIR_ACC_ERR_TIMEOUT;
		}
	}

	while (reg & UFS_HCI_REGISTER_FLAG_CDASTA) { /* CDASTA (indicates if there is a new data in the buffer */
		if (buf_index >= buflen) {
			UFS_DBG_LOGD("Buffer full so stop reading. buf_index: %d, buflen: %d\n", (int)buf_index, (int)buflen);
			break;
		}

		val_high_u = ufshcd_readl(hba, REG_CDARX1);
		val_low_u = ufshcd_readl(hba, REG_CDARX2);

		ctrl = (reg & 0xFF00) >> 8;

#ifdef MSG_DEBUG
#ifdef UFS_UPIU_DEBUG
		/* for debug only */
		if (buflen == 32) {
			UFS_DBG_LOGD("read RX1: 0x%x\n", val_high_u);
			UFS_DBG_LOGD("read RX2: 0x%x\n", val_low_u);
			UFS_DBG_LOGD("read ctrl: 0x%x\n", ctrl);
		}
#endif
#endif

		for (i = 0; i < 4; i++) {
			if (ctrl & 0x80) {
#if 0
				if ((isDummy == TRUE) && (*byte_read >= real_byte)) {
					buf_index++;
					*byte_read++;
				} else
#endif
				{
					data[buf_index] = (val_low_u >> (3-i)*8) & 0xff;
					buf_index++;

#if 0
					if (isDummy == TRUE)
						*byte_read++;
#endif
				}

			}
			ctrl = ctrl << 1;
		}

		if (buf_index >= buflen) { /* Solve 4-byte align data over read problem. ex: expect: 20 byte read: 32byte */
			UFS_DBG_LOGD("Buffer full so stop reading. buf_index: %d, buflen: %d\n", (int)buf_index, (int)buflen);
			break;
		}

		if (i == 4) {
			for (i = 0; i < 4; i++) {
				if (ctrl & 0x80) {
#if 0
					if ((isDummy == TRUE) && (*byte_read >= real_byte)) {
						buf_index++;
						*byte_read++;
					} else
#endif
					{
						data[buf_index] = (val_high_u >> (3 - i) * 8) & 0xff;
						buf_index++;

#if 0
						if (isDummy == TRUE)
							*byte_read++;
#endif
					}
				}
				ctrl = ctrl << 1;
			}
		}

		reg = ufshcd_readl(hba, REG_CDASTA);

		if (buf_index < buflen) {
			/* get timeout tick */
			#ifdef MTK_UFS_DRV_DA
			timeout_tick  = gpt4_time2tick_ms(retry_ms);
			start_tick    = gpt4_get_current_tick();
			#endif
			while (1) {
				reg = ufshcd_readl(hba, REG_CDASTA);
				if ((reg & UFS_HCI_REGISTER_FLAG_CDASTA)) {
					break;
				} else if (gpt4_timeout_tick(start_tick, timeout_tick)) {
					UFS_DBG_LOGD("buf_index: %d, buflen: %d", (int)buf_index, (int)buflen);
					UFS_DBG_LOGD("CPort direct access time out\n");
					return UFS_CPORT_DIR_ACC_ERR_TIMEOUT;
				}
			}
		}
	}

	UFS_DBG_LOGD("data length is 0x%x\n", (int)buf_index);
	*plen = buf_index;

	return UFS_ERR_NONE;
}

int ufs_aio_pio_query_flag(struct ufs_hba *hba, enum query_opcode opcode, enum flag_idn idn, bool *flag_res)
{
	if (UPIU_QUERY_OPCODE_SET_FLAG == opcode)
		return ufs_aio_pio_set_flag(hba, idn);
	else if (UPIU_QUERY_OPCODE_READ_FLAG == opcode)
		return ufs_aio_pio_read_flag(hba, idn, flag_res);
	else
		return -1;
}

int ufs_aio_pio_query_attr(struct ufs_hba *hba, enum query_opcode opcode,
			   enum attr_idn idn, u8 index, u8 selector, u32 *attr_val)
{
	return -1;
}

int ufs_aio_pio_write_desc(struct ufs_hba *hba, enum desc_idn idn, int index, u8 selector, u8 *src_buf, u32 buf_len)
{
	return -1;
}

int ufs_aio_pio_read_flag(struct ufs_hba *hba, unsigned char flag_idx, bool *value)
{
	int ret = 0;
	/* unsigned char buf[14]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}; */
	u32 resp_read = 0;

	unsigned char req_upiu[32] = {0x0};
	unsigned char resp_upiu[32] = {0x0};

	req_upiu[0] = UPIU_TRANSACTION_QUERY_REQ;
	req_upiu[3] = 0;
	req_upiu[5] = UPIU_QUERY_FUNC_STANDARD_READ_REQUEST;
	req_upiu[12] = UPIU_QUERY_OPCODE_READ_FLAG;
	req_upiu[13] = flag_idx; /* flag_idn */

	ret = ufs_aio_pio_cport_direct_write(hba, req_upiu, LEN_32, 1, LEN_1000); /* send query request upiu */

	UFS_DBG_LOGD("Not sleep, use polling!!!\n");

	ret = ufs_aio_pio_cport_direct_read(hba, resp_upiu, LEN_32, &resp_read, LEN_1000, FALSE, 0); /* get query response upiu */

	if (resp_upiu[6] != UPIU_RESPONSE_SUCCESS)
		return UFS_EXIT_FAILURE;

	UFS_DBG_LOGD("flag read flag_ind:%d value:%d\n", flag_idx, resp_upiu[23]);

	*value = (bool)resp_upiu[23];

	if (resp_read > 0) {
		unsigned long i;

		UFS_DBG_LOGD("get read flag query response upiu (byte %d):\n", resp_read);

		for (i = 0; i < resp_read; i++) {
			UFS_DBG_LOGD("0x%x ", resp_upiu[i]);
		}

		UFS_DBG_LOGD("\n");
	}

	return ret;

}



/* @SET FLAG @In2:flag_idn */
/* ex: tools 57 1    fDeviceInit Set */

int ufs_aio_pio_set_flag(struct ufs_hba *hba, unsigned char flag_idx)
{
	int ret = 0;
	u32 resp_read = 0;

	unsigned char resp_upiu[32] = {0x0};

	unsigned char req_upiu[32] = {0x0};

	req_upiu[0] = UPIU_TRANSACTION_QUERY_REQ;
	req_upiu[3] = 0;
	req_upiu[5] = UPIU_QUERY_FUNC_STANDARD_WRITE_REQUEST;
	req_upiu[12] = UPIU_QUERY_OPCODE_SET_FLAG;
	req_upiu[13] = flag_idx;

	UFS_DBG_LOGD("flag set flag_ind:%d\n", flag_idx);
	ret = ufs_aio_pio_cport_direct_write(hba, req_upiu, LEN_32, 1, LEN_1000); /* send query request upiu */

	UFS_DBG_LOGD("Not sleep, use polling!!!\n");

	ret = ufs_aio_pio_cport_direct_read(hba, resp_upiu, LEN_32, &resp_read, LEN_1000, FALSE, 0); /* get query response upiu */

	if (resp_upiu[6] != UPIU_RESPONSE_SUCCESS)
		return UFS_EXIT_FAILURE;

	if (resp_read > 0) {
		unsigned long i;

		UFS_DBG_LOGD("get set flag query response upiu (byte: %d):\n", resp_read);
		for (i = 0; i < resp_read; i++) {
			UFS_DBG_LOGD("0x%x ", resp_upiu[i]);
		}
		UFS_DBG_LOGD("\n");
	}

	return ret;
}

int ufs_aio_pio_nopin_nopout(struct ufs_hba *hba)
{
	int ret = 0;
	unsigned char nopout_cmd[UFS_AIO_PIO_UPIU_CMD_BUF_SIZE], nopin_cmd[UFS_AIO_PIO_UPIU_CMD_BUF_SIZE];
	u32 resp_read = 0;

	memset(nopout_cmd, 0, sizeof(nopout_cmd));
	memset(nopin_cmd, 0, sizeof(nopin_cmd));

	ret = ufs_aio_pio_cport_direct_write(hba, nopout_cmd, LEN_32, 1, LEN_1000); /* send nopout */
	if (ret != UFS_ERR_NONE)
		return UFS_NOPOUT_NOPIN_CPORT_TX_ERR;

	ret = ufs_aio_pio_cport_direct_read(hba, nopin_cmd, LEN_32, &resp_read, LEN_1000, FALSE, 0); /* get nopin */
	if (ret != UFS_ERR_NONE)
		return UFS_NOPOUT_NOPIN_CPORT_RX_ERR;

#ifdef MSG_DEBUG
#ifdef UFS_UPIU_DEBUG
	if (resp_read > 0) {
		unsigned long i;

		UFS_DBG_LOGD("get nopin upiu (byte:%d):\n", resp_read);
		for (i = 0; i < resp_read; i++) {
			UFS_DBG_LOGD("0x%x ", nopin_cmd[i]);
		}
		UFS_DBG_LOGD("\n");
	}
#endif
#endif

	/* Error handling */
	if ((nopin_cmd[0] != UPIU_TRANSACTION_NOP_IN) ||  (nopin_cmd[6] != UPIU_RESPONSE_SUCCESS))
		return UFS_NOPOUT_NOPIN_ERR;

	return ret;

}

int ufs_aio_pio_read_desc(struct ufs_hba *hba, enum desc_idn desc_id, int desc_index, u8 selector, u8 *buf, u32 buf_size)
{
	int ret = UFS_ERR_NONE;
	u32 resp_read = 0;

	memset(ufs_req_upiu, 0, sizeof(ufs_req_upiu));
	memset(ufs_resp_upiu, 0, sizeof(ufs_resp_upiu));
	memset(buf, 0, buf_size);

	ufs_req_upiu[0] = UPIU_TRANSACTION_QUERY_REQ;
	ufs_req_upiu[3] = 0;            /* Task Tag */
	ufs_req_upiu[5] = UPIU_QUERY_FUNC_STANDARD_READ_REQUEST;
	ufs_req_upiu[12] = UPIU_QUERY_OPCODE_READ_DESC;
	ufs_req_upiu[13] = desc_id;     /* Descriptor IDN */
	ufs_req_upiu[14] = desc_index;  /* Index */
	ufs_req_upiu[15] = selector;  /* Index */

	ufs_req_upiu[10] = 0;                           /* Data segment length (MSB) */
	ufs_req_upiu[11] = buf_size;                    /* Data segment length (LSB) */
	ufs_req_upiu[18] = 0;                           /* Transaction Specific Fields for READ DESCRIPTOR OPCODE: Length (MSB) */
	ufs_req_upiu[19] = buf_size;                    /* Transaction Specific Fields for READ DESCRIPTOR OPCODE: Length (LSB) */

	ret = ufs_aio_pio_cport_direct_write(hba, ufs_req_upiu, LEN_32, 1, LEN_1000); /* send query request UPIU */
	if (ret != UFS_ERR_NONE)
		return UFS_READ_DESC_CPORT_TX_ERR;

	ret = ufs_aio_pio_cport_direct_read(hba, ufs_resp_upiu, LEN_32, &resp_read, LEN_1000, FALSE, 0); /* get query response UPIU (top 32 bytes, before data) */
	if (ret != UFS_ERR_NONE)
		return UFS_READ_DESC_CPORT_RX_ERR;

#ifdef MSG_DEBUG
#ifdef UFS_UPIU_DEBUG
	if (resp_read > 0) {
		unsigned long i;

		UFS_DBG_LOGD("get read device descriptor before data (byte:%d):\n", resp_read);
		for (i = 0; i < resp_read; i++) {
			UFS_DBG_LOGD("0x%x ", ufs_resp_upiu[i]);
		}
		UFS_DBG_LOGD("\n");
	}
#endif
#endif

	/* Error handling */
	if ((ufs_resp_upiu[0] != UPIU_TRANSACTION_QUERY_RSP) ||  (ufs_resp_upiu[6] != UPIU_RESPONSE_SUCCESS))
		return UFS_READ_DESC_ERR;

	ret = ufs_aio_pio_cport_direct_read(hba, buf, ((ufs_resp_upiu[18] << 8) | ufs_resp_upiu[19]), &resp_read, LEN_1000, FALSE, 0); /* get query response UPIU (data part) */

	if (ret != UFS_ERR_NONE)
		return UFS_READ_DESC_CPORT_RX_ERR;

#ifdef MSG_DEBUG
#ifdef UFS_UPIU_DEBUG
	if (resp_read > 0) {
		unsigned long i;

		UFS_DBG_LOGD("get device descriptor data (byte:%d)\n", resp_read);
		for (i = 0; i < resp_read; i++) {
			UFS_DBG_LOGD("0x%x ", buf[i]);
		}
		UFS_DBG_LOGD("\n");
	}
#endif
#endif

	return ret;

}

/* int ufs_aio_pio_dme_set(struct ufs_hba * hba, u32 uic_cmd, uint16 mib_attribute, uint16 gen_select_index, u32 value, u32 *return_code, int retry_ms) */
int ufs_aio_pio_dme_set_attr(struct ufs_hba *hba, u32 attr_sel, u8 attr_set, u32 mib_val, u8 peer, u32 *return_code, int retry_ms)
{
	u32 ret = 0;
	u32 timeout_tick = 0, start_tick = 0;

	if (return_code)
		*return_code = 0;

	ufshcd_writel(hba, attr_sel, REG_UIC_COMMAND_ARG_1);
	ufshcd_writel(hba, UIC_ARG_ATTR_TYPE(attr_set), REG_UIC_COMMAND_ARG_2);
	ufshcd_writel(hba, mib_val, REG_UIC_COMMAND_ARG_3);
	ufshcd_writel(hba, (peer ? UIC_CMD_DME_PEER_SET : UIC_CMD_DME_SET) & COMMAND_OPCODE_MASK, REG_UIC_COMMAND);

	/* get timeout tick */
#ifdef MTK_UFS_DRV_DA
	timeout_tick  = gpt4_time2tick_ms(retry_ms);
	start_tick    = gpt4_get_current_tick();
#endif

	while (1) {
		ret = ufshcd_readl(hba, REG_INTERRUPT_STATUS);

		if ((ret & 0x400) == 0x400) {
			ufshcd_writel(hba, 0x400, REG_INTERRUPT_STATUS);
			break;
		}

		if (gpt4_timeout_tick(start_tick, timeout_tick)) {
			UFS_DBG_LOGD("ERROR : UIC command fail (timeout).\n");
			return UFS_UIC_CMD_ERR_TIMEOUT;
		}
	}

	ret = ufshcd_readl(hba, REG_UIC_COMMAND_ARG_2);

	if (ret & 0xFF) {
		UFS_DBG_LOGD("ERROR : ufs_aio_pio_dme_set_attr fail (error arg2: 0x%x)\n", ret);

		ufs_aio_dbg_show_reg(hba, REG_UIC_ERROR_CODE_PHY_ADAPTER_LAYER);
		ufs_aio_dbg_show_reg(hba, REG_UIC_ERROR_CODE_DATA_LINK_LAYER);
		ufs_aio_dbg_show_reg(hba, REG_UIC_ERROR_CODE_NETWORK_LAYER);
		ufs_aio_dbg_show_reg(hba, REG_UIC_ERROR_CODE_TRANSPORT_LAYER);
		ufs_aio_dbg_show_reg(hba, REG_UIC_ERROR_CODE_DME);

		if (return_code)
			*return_code = ret;

		return UFS_UIC_CMD_ERR;
	}

	return UFS_ERR_NONE;
}

int ufs_aio_pio_dme_set(struct ufs_hba *hba, u32 attr_sel, u32 mib_val)
{
	return ufs_aio_pio_dme_set_attr(hba, attr_sel, ATTR_SET_NOR, mib_val, DME_LOCAL, NULL, 1000);
}

int ufs_aio_pio_dme_peer_set(struct ufs_hba *hba, u32 attr_sel, u32 mib_val)
{
	return ufs_aio_pio_dme_set_attr(hba, attr_sel, ATTR_SET_NOR, mib_val, DME_PEER, NULL, 1000);
}

int ufs_aio_pio_dme_get_attr(struct ufs_hba *hba, u32 attr_sel, u32 *mib_val, u8 peer, u32 *return_code, int retry_ms)
{
	u32 ret;
	u32 timeout_tick = 0, start_tick = 0;

	if (return_code)
		*return_code = 0;

	ufshcd_writel(hba, attr_sel, REG_UIC_COMMAND_ARG_1);
	ufshcd_writel(hba, 0, REG_UIC_COMMAND_ARG_2);
	ufshcd_writel(hba, 0, REG_UIC_COMMAND_ARG_3);
	ufshcd_writel(hba, (peer ? UIC_CMD_DME_PEER_GET : UIC_CMD_DME_GET) & COMMAND_OPCODE_MASK, REG_UIC_COMMAND);

	/* get timeout tick */
#ifdef MTK_UFS_DRV_DA
	timeout_tick  = gpt4_time2tick_ms(retry_ms);
	start_tick    = gpt4_get_current_tick();
#endif

	while (1) {
		ret = ufshcd_readl(hba, REG_INTERRUPT_STATUS);

		if ((ret & 0x400) == 0x400) {
			ufshcd_writel(hba, 0x400, REG_INTERRUPT_STATUS);
			break;
		}

		if (gpt4_timeout_tick(start_tick, timeout_tick)) {
			UFS_DBG_LOGD("ERROR : UIC command fail (timeout).\n");
			return UFS_UIC_CMD_ERR_TIMEOUT;
		}
	}

	ret = ufshcd_readl(hba, REG_UIC_COMMAND_ARG_2);

	if (ret & 0xFF) {
		UFS_DBG_LOGD("ERROR : UIC command fail.\n");

		ufs_aio_dbg_show_reg(hba, REG_UIC_ERROR_CODE_PHY_ADAPTER_LAYER);
		ufs_aio_dbg_show_reg(hba, REG_UIC_ERROR_CODE_DATA_LINK_LAYER);
		ufs_aio_dbg_show_reg(hba, REG_UIC_ERROR_CODE_NETWORK_LAYER);
		ufs_aio_dbg_show_reg(hba, REG_UIC_ERROR_CODE_TRANSPORT_LAYER);
		ufs_aio_dbg_show_reg(hba, REG_UIC_ERROR_CODE_DME);

		if (return_code)
			*return_code = ret;

		return UFS_UIC_CMD_ERR;
	}

	*mib_val = ufshcd_readl(hba, REG_UIC_COMMAND_ARG_3);

	return UFS_ERR_NONE;

}

int ufs_aio_pio_dme_get(struct ufs_hba *hba, u32 attr_sel, u32 *mib_val)
{
	return ufs_aio_pio_dme_get_attr(hba, attr_sel, mib_val, DME_LOCAL, NULL, 1000);
}

int ufs_aio_pio_dme_peer_get(struct ufs_hba *hba, u32 attr_sel, u32 *mib_val)
{
	return ufs_aio_pio_dme_get_attr(hba, attr_sel, mib_val, DME_PEER, NULL, 1000);
}

#endif /* UFS_CFG_ENABLE_PIO */

