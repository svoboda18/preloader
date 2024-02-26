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

#include "ufs_aio_cfg.h" // Include ufs_aio_cfg.h for defining MTK_UFS_DRV_PRELOADER
#include "ufs_aio_types.h"
#include "ufs_aio.h"
#include "ufs_aio_hcd.h"
#include "ufs_aio_core.h"
#include "ufs_aio_utils.h"
#include "ufs_aio_error.h"
#include "ufs_aio_rpmb.h"

#if defined(MTK_UFS_DRV_LK)
#include <stdlib.h>
#include <arch/arm/mmu.h>
#include "partition_wp.h"
#include <arch/ops.h> /* arch_clean_invalidate_cache_range() is defined in bootable/bootloader/include/arch/ops.h */
#endif

#if CFG_RPMB_KEY_RETRY_IN_PL
#include <hmac.h>
extern char g_rpmb_key[];
#endif

#if defined(MTK_UFS_DRV_DA)
#include "malloc.h"
#include "boot/security_export/rpmb_hmac.h"
#endif

void rpmb_req_copy_data_for_hmac(u8 *buf, struct rpmb_data_frame *f)
{
	u32 size;

	/*
	 * Copy below members for HMAC calculation
	 * one by one with specifically assigning
	 * buf to each member to pass buffer-overrun checker.
	 *
	 * u8 data[RPMB_FRM_DATA_LEN];
	 * u8 nonce[RPMB_FRM_NONCE_LEN];
	 * u32 wr_cnt;
	 * u16 addr;
	 * u16 blk_cnt;
	 * u16 result;
	 * u16 req_resp;
	 */

	memcpy(buf, f->data, RPMB_FRM_DATA_LEN);
	buf += RPMB_FRM_DATA_LEN;

	size = sizeof(f->nonce);
	memcpy(buf, f->nonce, size);
	buf += size;

	size = sizeof(f->wr_cnt);
	memcpy(buf, &f->wr_cnt, size);
	buf += size;

	size = sizeof(f->addr);
	memcpy(buf, &f->addr, size);
	buf += size;

	size = sizeof(f->blk_cnt);
	memcpy(buf, &f->blk_cnt, size);
	buf += size;

	size = sizeof(f->result);
	memcpy(buf, &f->result, size);
	buf += size;

	size = sizeof(f->req_resp);
	memcpy(buf, &f->req_resp, size);
	buf += size;
}

#if defined(MTK_UFS_DRV_LK)
extern int32_t rpmb_hmac(uint32_t pattern, uint32_t size);
extern int32_t rpmb_init(void);

u32 *shared_msg_addr1 = (u32 *)(LK_SHARED_MEM_ADDR + 0x40);                /* fixed and pre-allocated in lk */
u32 *shared_hmac_addr1 = (u32 *)(LK_SHARED_MEM_ADDR+ 0xCE0);                /* fixed and pre-allocated in lk */

int init_rpmb_sharemem()
{
	uint32_t start, size;
	int ret = 0;

	/* map shared memory for rpmb */
	start = ROUNDDOWN((uint32_t)LK_SHARED_MEM_ADDR, PAGE_SIZE);
	size = PAGE_SIZE;
	arch_mmu_map((uint64_t)start, (uint32_t)start,
	             MMU_MEMORY_TYPE_NORMAL_WRITE_BACK | MMU_MEMORY_AP_P_RW_U_NA, ROUNDUP(size, PAGE_SIZE));

	ret = rpmb_init();
	if (ret) {
		UFS_DBG_LOGE("[%s]: RPMB rpmb_init error, ret = %d \n", __func__, ret);
		return ret;
	}
	return 0;
}

static int rpmb_get_mac(struct rpmb_data_frame *pfrm)
{
	int ret = 0;
	u8 *data_for_hmac;

	if (pfrm == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB pfrm pointer NULL\n", __func__);
		return -1;
	}

	data_for_hmac = pfrm->data;

	memcpy(shared_msg_addr1, data_for_hmac, HMAC_TEXT_LEN);

	arch_clean_invalidate_cache_range((addr_t) LK_SHARED_MEM_ADDR, 4096);

	ret = rpmb_hmac(0, 284);
	if (ret) {
		UFS_DBG_LOGE("[%s]: RPMB rpmb_hmac error, ret = %d \n", __func__, ret);
		return ret;
	}
	return 0;
}
#endif //#if defined(MTK_UFS_DRV_LK)
static void cmd_scsi_security_protocol_out(struct ufs_aio_scsi_cmd *cmd, u32 tag, u32 blk_cnt, u8 protocol, u32 protocol_specific)
{
	if (cmd == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB cmd pointer NULL\n", __func__);
		return;
	}

	memset(cmd->cmd_data, 0, MAX_CDB_SIZE);

	cmd->lun = WLUN_RPMB;
	cmd->tag = tag;
	cmd->dir = DMA_TO_DEVICE;
	cmd->exp_len = blk_cnt  << 9;  //block size:512

	cmd->attr = ATTR_SIMPLE;

	cmd->cmd_len = 12;
	cmd->cmd_data[0] = SECURITY_PROTOCOL_OUT; //opcode
	cmd->cmd_data[1] = protocol; //security protocol
	cmd->cmd_data[2] = (protocol_specific >> 8) & 0xFF; //security protocol specific
	cmd->cmd_data[3] = protocol_specific & 0xFF; //security protocol specific
	cmd->cmd_data[4] = 0x0;  // INC_512 = 0
	cmd->cmd_data[5] = 0x0; //Reserved
	cmd->cmd_data[6] = ((blk_cnt  << 9) >> 24) & 0xFF; //Reserved
	cmd->cmd_data[7] = ((blk_cnt  << 9) >> 16) & 0xFF; //Reserved
	cmd->cmd_data[8] = ((blk_cnt  << 9) >> 8) & 0xFF; //Reserved
	cmd->cmd_data[9] = 0x0; //(blk_cnt  << 9) & 0xFF; actually = 0 //Reserved
	cmd->cmd_data[10] = 0x0; //Reserved
	cmd->cmd_data[11] = 0x0; //control = 0
}
static void  ufshcd_authen_key_prog_req_prepare(struct ufs_aio_scsi_cmd *cmd,
        const char *authenKey, unsigned int key_sz)
{
	struct rpmb_data_frame *pfrm = NULL;

	if (cmd == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB cmd pointer NULL\n", __func__);
		return;
	}
	if (authenKey == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB authenKey pointer NULL\n", __func__);
		return;
	}

	pfrm = cmd->data_buf;

	if (pfrm == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB pfrm pointer NULL\n", __func__);
		return;
	}

	memset(pfrm, 0, sizeof(struct rpmb_data_frame));

	memcpy(pfrm->key_MAC, authenKey, key_sz);

	pfrm->req_resp = cpu_to_be16(RPMB_REQ_AUTHEN_KEY_PRG);
}
static void  ufshcd_authen_result_read_req_prepare(struct ufs_aio_scsi_cmd *cmd)
{
	struct rpmb_data_frame *pfrm = NULL;

	if (cmd == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB cmd pointer NULL\n", __func__);
		return;
	}

	pfrm = cmd->data_buf;

	if (pfrm == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB pfrm pointer NULL\n", __func__);
		return;
	}

	memset(pfrm, 0, sizeof(struct rpmb_data_frame));

	pfrm->req_resp = cpu_to_be16(RPMB_REQ_RESULT_RD);
}
static void cmd_scsi_security_protocol_in(struct ufs_aio_scsi_cmd *cmd, u32 tag, u32 blk_cnt, u8 protocol, u32 protocol_specific)
{
	if (cmd == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB cmd pointer NULL\n", __func__);
		return;
	}

	memset(cmd->cmd_data, 0, MAX_CDB_SIZE);

	cmd->lun = WLUN_RPMB;
	cmd->tag = tag;
	cmd->dir = DMA_FROM_DEVICE;
	cmd->exp_len = blk_cnt * 512;

	cmd->attr = ATTR_SIMPLE;

	cmd->cmd_len = 12;
	cmd->cmd_data[0] = SECURITY_PROTOCOL_IN; //opcode
	cmd->cmd_data[1] = protocol; //security protocol
	cmd->cmd_data[2] = (protocol_specific >> 8) & 0xFF; //security protocol specific
	cmd->cmd_data[3] = protocol_specific & 0xFF; //security protocol specific
	cmd->cmd_data[4] = 0x0;  // INC_512 = 0
	cmd->cmd_data[5] = 0x0; //Reserved
	cmd->cmd_data[6] = ((blk_cnt  << 9) >> 24) & 0xFF; //Reserved
	cmd->cmd_data[7] = ((blk_cnt  << 9) >> 16) & 0xFF; //Reserved
	cmd->cmd_data[8] = ((blk_cnt  << 9) >> 8) & 0xFF; //Reserved
	cmd->cmd_data[9] = 0x0; //(blk_cnt  << 9) & 0xFF; actually = 0 //Reserved
	cmd->cmd_data[10] = 0x0; //Reserved
	cmd->cmd_data[11] = 0x0; //control = 0
}
/*Response Frame check for Authenkey program request and write data request*/
static int ufshcd_authen_result_read_rsp_check(u32 host_id, struct ufs_aio_scsi_cmd *cmd,
        u16 addr, u32 wcnt,
        enum enum_rpmb_msg type)
{
	struct rpmb_data_frame *rpmb_frame = NULL;
	unsigned int idx = 0;

	if (cmd == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB cmd pointer NULL\n", __func__);
		return -1;
	}

	rpmb_frame = cmd->data_buf;

	if (rpmb_frame == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB rpmb_frame pointer NULL\n", __func__);
		return -1;
	}

	if ( rpmb_frame->req_resp != cpu_to_be16(type)) {
		/* Check response type */
		UFS_DBG_LOGE("[%s]: RPMB Fail in checking response type : 0x%x\r\n", __func__ , cpu_to_be16(rpmb_frame->req_resp));
		return -1;
	} else if ((idx = (cpu_to_be16(rpmb_frame->result) & 0x7F))) {
		/* Check response type */
		UFS_DBG_LOGE("[%s]: RPMB Fail in checking result : %s \r\n", __func__ , rpmb_oper_result[idx]);
		return -1;
	}

	return 0;
}
unsigned char ufs_rpmb_buf[sizeof(struct rpmb_data_frame)];  /* for RPMB only */

int rpmb_authen_key_program(u32 host_id, const char *authen_key, u32 Key_sz)
{
	struct ufs_aio_scsi_cmd cmd;
	struct ufs_hba *hba;
	int ret = 0;
	int tag;

	if (authen_key == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB authen_key pointer NULL\n", __func__);
		return -1;
	}

	cmd.data_buf = ufs_rpmb_buf;

	hba = ufs_aio_get_host(host_id);

	if (!ufshcd_get_free_tag(hba, &tag))
		return -1;

	/*Step1. Authen key Data */
	cmd_scsi_security_protocol_out(&cmd, tag, 1,
	                               UFS_SECURITY_PROTOCOL,
	                               RPMB_PROTOCOL_ID);
	ufshcd_authen_key_prog_req_prepare(&cmd, authen_key,
	                                   Key_sz);

	if ((ret = ufshcd_queuecommand(hba, &cmd))) {
		UFS_DBG_LOGE("[%s]: RPMB Fail in Sending authen key program request, ret = %d \n", __func__, ret);
		goto exit;
	}

	/*Step2. Result Register Read Request*/
	cmd_scsi_security_protocol_out(&cmd, tag, 1,
	                               UFS_SECURITY_PROTOCOL,
	                               RPMB_PROTOCOL_ID);
	ufshcd_authen_result_read_req_prepare(&cmd);

	if ((ret = ufshcd_queuecommand(hba, &cmd))) {
		UFS_DBG_LOGE("[%s]: RPMB Fail in Sending result register read request, ret = %d \n", __func__, ret);
		goto exit;

	}
	/*Step3. Resp for Resutl Register Request*/
	cmd_scsi_security_protocol_in(&cmd, tag, 1,
	                              UFS_SECURITY_PROTOCOL,
	                              RPMB_PROTOCOL_ID);
	if ((ret = ufshcd_queuecommand(hba, &cmd))) {
		UFS_DBG_LOGE("[%s]: RPMB Fail in Sending request get result register response, ret = %d \n", __func__, ret);
		goto exit;

	}
	/*Step4. Resp frame check*/
	if ((ret = ufshcd_authen_result_read_rsp_check(host_id, &cmd, 0, 0,
	           RPMB_RSP_AUTHEN_KEY_PRG))) {
		UFS_DBG_LOGE("[%s]: RPMB Authentication Key Program Fail, ret = %d \n", __func__, ret);
		goto exit;

	}

exit:
	ufshcd_put_tag(hba, tag);

	return ret;
}

////////////////////////////////////////////////////////////////////////////
#define RPMB_MAC_DUMP_DEBUG 0

#if RPMB_MAC_DUMP_DEBUG
static inline void ufshcd_rpmb_mac_dump(const u8 *mac, u32 mac_len)
{
	u32 i = 0;
	UFS_DBG_LOGE("mac dump:\n");
	if (mac == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB mac pointer NULL\n", __func__);
		return;
	}
	for (i = 0; i < mac_len; i++) {
		UFS_DBG_LOGE("[%d]:0x%x\n", i, *(mac+i));
	}
	UFS_DBG_LOGE("\n");
}
#else
static inline void ufshcd_rpmb_mac_dump(const u8 *mac, u32 mac_len)
{
}
#endif


static int  ufshcd_authen_wcnt_read_req_prepare(struct ufs_aio_scsi_cmd *cmd,
        u8 *nonce, unsigned int nonce_len)
{
	struct rpmb_data_frame *pfrm = NULL;
//	struct ufs_hba *hba;
	unsigned int i = 0;
	int ret = 0;
//	hba = ufs_aio_get_host(host_id);
	if (cmd == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB cmd pointer NULL\n", __func__);
		return -1;
	}
	if (nonce ==NULL) {
		UFS_DBG_LOGE("[%s]: RPMB nonce pointer NULL\n", __func__);
		return -1;
	}

	pfrm = cmd->data_buf;

	if (pfrm == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB pfrm pointer NULL\n", __func__);
		return -1;
	}

	if (nonce_len > RPMB_FRM_NONCE_LEN) {
		UFS_DBG_LOGE(
		    "[%s]: RPMB Fail Nonce data length larger than max allow len(%d)", __func__ ,
		    RPMB_FRM_NONCE_LEN);
		return -1;
	}
	memset(pfrm, 0, sizeof(struct rpmb_data_frame));

	/* Prepare the max 16 bytes nonce */
	for (i = 0; i < nonce_len; i++)
		nonce[i] = (u8)i & 0xFF; /* ToDO: get random nonce */

	memcpy(pfrm->nonce, nonce, nonce_len);

	pfrm->req_resp = cpu_to_be16(RPMB_REQ_READ_WR_CNT);

	return ret;

}
static int ufshcd_authensss_wcnt_read_rsp_check(u32 host_id,
        struct ufs_aio_scsi_cmd *cmd, u32 *wcnt,
        u8 *nonce, unsigned int nonce_len,
        enum enum_rpmb_msg type)
{
	struct rpmb_data_frame *rpmb_frame = NULL;
	unsigned int idx = 0;
	int ret = 0;
#if (CFG_RPMB_KEY_RETRY_IN_PL || defined(MTK_UFS_DRV_DA))
	u8 rpmb_hmac_data[HMAC_TEXT_LEN] = {0};
	u8 rpmb_hmac_derived[RPMB_FRM_KEY_MAC_LEN] = {0};
#endif
	if (cmd == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB cmd pointer NULL\n", __func__);
		return -1;
	}
	if (wcnt == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB wcnt pointer NULL\n", __func__);
		return -1;
	}
	if (nonce == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB nonce pointer NULL\n", __func__);
		return -1;
	}

	rpmb_frame = cmd->data_buf;

	if (rpmb_frame == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB rpmb_frame pointer NULL\n", __func__);
		return -1;
	}

	if ( rpmb_frame->req_resp != cpu_to_be16(type)) {
		/* Check response type */
		UFS_DBG_LOGE("[%s]: RPMB Fail in checking response type : 0x%x\r\n", __func__, cpu_to_be16(rpmb_frame->req_resp));
		return -1;
	} else if ((idx = (cpu_to_be16(rpmb_frame->result) & 0x7F))) {
		/* Check response type */
		UFS_DBG_LOGE("[%s]: RPMB Fail in checking result : %s \r\n", __func__, rpmb_oper_result[idx]);
		return -1;
	} else if (cpu_to_be16(rpmb_frame->result) & 0x80) {
		/*Check Write Counter*/
		UFS_DBG_LOGE("[%s]: RPMB write counter is expired\r\n", __func__);
	}

	if ( memcmp(rpmb_frame->nonce, nonce, nonce_len) ) {
		/*for other response type noce will be "0"*/
		UFS_DBG_LOGE("[%s]: RPMB Fail in checking nonce\r\n", __func__);
		return -1;
	}

#if defined(MTK_UFS_DRV_LK)
	// Then check MAC
	ret = rpmb_get_mac(rpmb_frame);
	if (ret) {
		UFS_DBG_LOGE("[%s]: RPMB ufshcd_authensss_wcnt_read_rsp_check rpmb_get_mac fail\r\n", __func__);
		return -1;
	}

	if (memcmp(rpmb_frame->key_MAC, shared_hmac_addr1, RPMB_FRM_KEY_MAC_LEN)) {
		UFS_DBG_LOGE("[%s]: RPMB Fail in Mac Check!\n", __func__);
		UFS_DBG_LOGE("Device Return ");
		ufshcd_rpmb_mac_dump((const u8*)rpmb_frame->key_MAC, RPMB_FRM_KEY_MAC_LEN);
		UFS_DBG_LOGE("Host Recalc");
		ufshcd_rpmb_mac_dump((const u8*)shared_hmac_addr1, RPMB_FRM_KEY_MAC_LEN);
		return -1;
	}
#endif

#if CFG_RPMB_KEY_RETRY_IN_PL
	/* copy rpmb_frame contents for hmac one by one to avoid coverity issue */
	rpmb_req_copy_data_for_hmac(rpmb_hmac_data, rpmb_frame);

	ret = hmac_sha256(g_rpmb_key, 32, rpmb_hmac_data, HMAC_TEXT_LEN, rpmb_hmac_derived);
	if (ret) {
		UFS_DBG_LOGE("%s, rpmb_hmac error.\n", __func__);
		return -1;
	}

	if (memcmp(rpmb_frame->key_MAC, rpmb_hmac_derived, RPMB_FRM_KEY_MAC_LEN)) {
		UFS_DBG_LOGE("%s, compare hmac error!!\n", __func__);
		return -1;
	}
#endif //CFG_RPMB_KEY_RETRY_IN_PL

#if defined(MTK_UFS_DRV_DA)
	rpmb_req_copy_data_for_hmac(rpmb_hmac_data, rpmb_frame);

	ret = rpmb_hmac(rpmb_hmac_data, HMAC_TEXT_LEN, rpmb_hmac_derived);
	if (ret) {
		UFS_DBG_LOGE("[%s]: RPMB ufshcd_authensss_wcnt_read_rsp_check rpmb_get_mac fail\r\n", __func__);
		return -1;
	}

	if (memcmp(rpmb_frame->key_MAC, rpmb_hmac_derived, RPMB_FRM_KEY_MAC_LEN)) {
		UFS_DBG_LOGE("[%s]: RPMB Fail in Mac Check!\n", __func__);
		UFS_DBG_LOGE("Device Return ");
		ufshcd_rpmb_mac_dump((const u8*)rpmb_frame->key_MAC, RPMB_FRM_KEY_MAC_LEN);
		UFS_DBG_LOGE("Host Recalc");
		ufshcd_rpmb_mac_dump((const u8*)rpmb_hmac_derived, RPMB_FRM_KEY_MAC_LEN);
		return -1;
	}
#endif

	// Then Return WCNT
	*wcnt = cpu_to_be32(rpmb_frame->wr_cnt);

	return 0;
}

int rpmb_authen_read_counter(u32 host_id, u8 *nonce_buf, u32 nonce_len,
                                    u32 *wr_cnt)
{
	struct ufs_aio_scsi_cmd cmd;
	struct ufs_hba *hba;
	int ret = 0;
	int tag;

	if (nonce_buf == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB nonce_buf pointer NULL\n", __func__);
		return -1;
	}
	if (wr_cnt == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB wr_cnt pointer NULL\n", __func__);
		return -1;
	}

	cmd.data_buf = ufs_rpmb_buf;

	hba = ufs_aio_get_host(host_id);

	if (!ufshcd_get_free_tag(hba, &tag))
		return -1;

	/*Step1 write counter read request */
	cmd_scsi_security_protocol_out(&cmd, tag, 1,
	                               UFS_SECURITY_PROTOCOL,
	                               RPMB_PROTOCOL_ID);
	ufshcd_authen_wcnt_read_req_prepare(&cmd,
	                                    nonce_buf, nonce_len);

	if ((ret = ufshcd_queuecommand(hba, &cmd))) {
		UFS_DBG_LOGE("[%s]: RPMB Fail in Sending Write counter read request, ret = %d \n", __func__, ret);
		goto exit;
	}
	/*Step2. Resp for write counter read Request*/
	cmd_scsi_security_protocol_in(&cmd, tag, 1, UFS_SECURITY_PROTOCOL, RPMB_PROTOCOL_ID);
	if ((ret = ufshcd_queuecommand(hba, &cmd))) {
		UFS_DBG_LOGE("[%s]: RPMB Fail in Sending request get result register response, ret = %d \n", __func__, ret);
		goto exit;
	}
	/*Step3. Response frame check*/
	if ((ret = ufshcd_authensss_wcnt_read_rsp_check(host_id, &cmd, wr_cnt,
	           nonce_buf, nonce_len,
	           RPMB_RSP_READ_WR_CNT))) {
		UFS_DBG_LOGE("[%s]: RPMB WriteCounter Read Resp Check Fail, ret = %d \n", __func__, ret);
	}
exit:
	ufshcd_put_tag(hba, tag);
	return ret;
}

int ufs_rpmb_authen_key_program(const char *authen_key, int *rpmb_result)
{
	int ret = 0;
	u32 host_id = 0;
	u8 *nonce_buf = NULL;
	u32 wr_cnt = 0;

	if (authen_key == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB authen_key pointer NULL\n", __func__);
		return -1;
	}

	nonce_buf = (u8 *)malloc(RPMB_FRM_NONCE_LEN);
	if (nonce_buf == NULL)
		return -1;
	memset(nonce_buf, 0, RPMB_FRM_NONCE_LEN);
	/* If read counter success, it means key has been written -> return without program key again */
	ret = rpmb_authen_read_counter(host_id, nonce_buf, RPMB_FRM_NONCE_LEN,
	                               &wr_cnt);
	if (!ret) {
		UFS_DBG_LOGE("[%s]: RPMB Key has been written! ret = %d\n", __func__, ret);
		free(nonce_buf);
		return 0;
	}


#if CFG_RPMB_KEY_PROGRAMED_IN_KERNEL
	if (rpmb_result != NULL)
		*rpmb_result = 7; /* Key not program */

	print("UFS rpmb key will be programmed in kernel, skip now!\n" );
#else

	ret = rpmb_authen_key_program(host_id, authen_key, RPMB_FRM_KEY_MAC_LEN);
	if (ret)
		UFS_DBG_LOGE("[%s]: RPMB [Failed]Authentication Key not Programed this time, ret = %d\n", __func__, ret);
	else
		UFS_DBG_LOGI("[%s]: RPMB [PASS]Authentication Key Program Success\n", __func__);

#endif
	free(nonce_buf);
	return ret;
}
#if defined(MTK_UFS_DRV_LK) || defined(MTK_UFS_DRV_DA)

#ifdef MTK_UFS_OTP
int otp_found = 0;
#endif

static void dump_swpcb(struct secure_wp_config_block *swpcb)
{
	int i;

	UFS_DBG_LOGE("swpcb lun %d\n", swpcb->lun);
	UFS_DBG_LOGE("swpcb data_length %d\n", swpcb->data_length);

	for (i = 0; i < (swpcb->data_length/WP_ENTRY_SIZE); i++) {
		UFS_DBG_LOGE("swpcb entry[%d] wpt_wpf 0x%x\n", i, swpcb->wp_entry[i].wpt_wpf);
		UFS_DBG_LOGE("swpcb entry[%d] upper_addr 0x%x\n", i, swpcb->wp_entry[i].upper_addr);
		UFS_DBG_LOGE("swpcb entry[%d] lower_addr 0x%x\n", i, swpcb->wp_entry[i].lower_addr);
		UFS_DBG_LOGE("swpcb entry[%d] block_num 0x%x\n", i, swpcb->wp_entry[i].block_num);
	}
}


static int  ufshcd_swp_conf_block_write_prepare(u32 host_id, struct ufs_aio_scsi_cmd *cmd,
        u16 blk_num, u32 wr_cnt, STORAGE_WP_TYPE type, unsigned long blknr, u32 blkcnt, ufs_logical_unit partition,u8 wp_entry,const u8 *data_buf)
{
	struct rpmb_data_frame *pfrm = NULL;
	int err = 0;
	struct secure_wp_config_block *swpcb = (void *)data_buf;
	int skip_new_entry = 0;
#if defined(MTK_UFS_DRV_DA)
	u8 rpmb_hmac_data[HMAC_TEXT_LEN] = {0};
	u8 rpmb_hmac_derived[RPMB_FRM_KEY_MAC_LEN] = {0};
#endif

	if (cmd == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB cmd pointer NULL\n", __func__);
		return -1;
	}
	if (data_buf == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB data_buf pointer NULL\n", __func__);
		return -1;
	}

	pfrm = cmd->data_buf;

	if (pfrm == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB pfrm pointer NULL\n", __func__);
		return -1;
	}

#ifdef MTK_UFS_OTP
	if (type == WP_PERMANENT && otp_found) {
		/*
		 * Do not add new entry if we want to add an OTP entry but
		 * OTP region is already existed in configuration block.
		 *
		 * We are here because we still need to write
		 * modified configuration block after clean job in the 1st round.
		 */
		skip_new_entry = 1;
	}
#endif

	memset(pfrm, 0, sizeof(struct rpmb_data_frame) * blk_num);

	pfrm->req_resp = cpu_to_be16(RPMB_REQ_SEC_WPCB_WRITE);
	pfrm->blk_cnt = cpu_to_be16(blk_num);
	pfrm->addr = 0;
	pfrm->wr_cnt = cpu_to_be32(wr_cnt);
	/*fix nonce data to 0*/

	/* LUN */
	swpcb->lun= (partition - 1);

	/* Data length */
	if ( partition ==  UFS_LU_BOOT1 || partition ==  UFS_LU_BOOT2)
		swpcb->data_length = WP_ENTRY_SIZE; //Boot partition only needs one secure write protect block entry
	else {
		if (skip_new_entry)
			swpcb->data_length = WP_ENTRY_SIZE * wp_entry;
		else
			swpcb->data_length = WP_ENTRY_SIZE * (wp_entry + 1);
	}

	/* WP entries configuration */
	/* Protect entire BOOT1 & BOOT2 area */
	if ( partition ==  UFS_LU_BOOT1 || partition ==  UFS_LU_BOOT2) {
		if (type == WP_DISABLE)
			swpcb->wp_entry[0].wpt_wpf = WP_WPF_DISABLE;
		else
			swpcb->wp_entry[0].wpt_wpf = WP_WPF_EN | (type << WPT_BIT);
		//Number of Logical Blocks      : 0x0 protect all blocks
		swpcb->wp_entry[0].block_num = 0;
		swpcb->wp_entry[0].upper_addr = 0;
		swpcb->wp_entry[0].lower_addr = 0;
	} else { //only update latest wp_entry
		if (!skip_new_entry) {
			if (type == WP_DISABLE)
				swpcb->wp_entry[wp_entry].wpt_wpf = WP_WPF_DISABLE;
			else
				swpcb->wp_entry[wp_entry].wpt_wpf = WP_WPF_EN | (type << WPT_BIT);

			//Logical Block Address
			//swpcb->wp_entry[wp_entry].upper_addr = ((blknr>>56) & 0xFF) | (((blknr>>48) & 0xFF) <<8) | (((blknr>>40) & 0xFF) <<16) | (((blknr>>32) & 0xFF) <<24); /* unsigned long in lk is only 32bits */
			swpcb->wp_entry[wp_entry].lower_addr = ((blknr>>24) & 0xFF) | (((blknr>>16) & 0xFF) <<8) | (((blknr>>8) & 0xFF) <<16) | ((blknr& 0xFF) <<24);

			//Number of Logical Blocks
			swpcb->wp_entry[wp_entry].block_num = ((blkcnt>>24) & 0xFF) | (((blkcnt>>16) & 0xFF) <<8) |  (((blkcnt>>8) & 0xFF) <<16) | ((blkcnt & 0xFF) <<24);
		}
	}

	UFS_DBG_LOGE("[%s]: Write swpcb\n", __func__);
	dump_swpcb(swpcb);

	/* Copy entire "data_buf" to "pfrm->data" before calculate HMAC */
	memcpy(pfrm->data, data_buf, WP_CONF_BLOCK_SIZE);

#if defined(MTK_UFS_DRV_LK)
	// Calculate MAC
	err = rpmb_get_mac(pfrm);
	if (err) {
		UFS_DBG_LOGE("[%s]: RPMB rpmb_get_mac fail, ret = %d\r\n", __func__, err);
		return -1;
	}
	memcpy(pfrm->key_MAC, shared_hmac_addr1, RPMB_FRM_KEY_MAC_LEN);
#endif

#if defined(MTK_UFS_DRV_DA)
	rpmb_req_copy_data_for_hmac(rpmb_hmac_data, pfrm);

	err = rpmb_hmac(rpmb_hmac_data, HMAC_TEXT_LEN, rpmb_hmac_derived);
	if (err) {
		UFS_DBG_LOGE("[%s]: RPMB rpmb_get_mac fail, ret = %d\r\n", __func__, err);
		return -1;
	}

	memcpy(pfrm->key_MAC, rpmb_hmac_derived, RPMB_FRM_KEY_MAC_LEN);
#endif

	return err;
}
static int ufshcd_swp_conf_block_read_prepare(u32 host_id, struct ufs_aio_scsi_cmd *cmd,
        u16 blk_num,
        u8 *nonce, unsigned int nonce_len, ufs_logical_unit partition)
{
	struct rpmb_data_frame *pfrm = NULL;
	unsigned int i = 0;
	int ret = 0;

	if (cmd == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB cmd pointer NULL\n", __func__);
		return -1;
	}
	if (nonce == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB nonce pointer NULL\n", __func__);
		return -1;
	}

	pfrm = cmd->data_buf;

	if (pfrm == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB pfrm pointer NULL\n", __func__);
		return -1;
	}

	memset(pfrm, 0, sizeof(struct rpmb_data_frame));


	pfrm->req_resp = cpu_to_be16(RPMB_REQ_SEC_WPCB_READ);
	pfrm->blk_cnt = cpu_to_be16(blk_num);
	pfrm->addr = 0;

	/* Prepare the max 16 bytes nonce */
	for (i = 0; i < nonce_len; i++)
		nonce[i] = (u8)i & 0xFF; /* ToDO: get random nonce */
	memcpy(pfrm->nonce, nonce, nonce_len);

	pfrm->data[0] = partition - 1;

	return ret;
}
static int ufshcd_authen_data_write_rsp_check(u32 host_id, struct ufs_aio_scsi_cmd *cmd, u32 wcnt,
        enum enum_rpmb_msg type)
{
	struct rpmb_data_frame *rpmb_frame = NULL;
	unsigned int idx = 0;
	int ret = 0;
#if defined(MTK_UFS_DRV_DA)
	u8 rpmb_hmac_data[HMAC_TEXT_LEN] = {0};
	u8 rpmb_hmac_derived[RPMB_FRM_KEY_MAC_LEN] = {0};
#endif

	if (cmd == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB cmd pointer NULL\n", __func__);
		return -1;
	}

	rpmb_frame = cmd->data_buf;

	if (rpmb_frame == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB rpmb_frame pointer NULL\n", __func__);
		return -1;
	}

	if ( rpmb_frame->req_resp != cpu_to_be16(type)) {
		/* Check response type */
		UFS_DBG_LOGE("[%s]: RPMB Fail in checking response type : 0x%x\r\n", __func__, cpu_to_be16(rpmb_frame->req_resp));
		return -1;
	} else if ((idx = (cpu_to_be16(rpmb_frame->result) & 0x7F))) {
		/* Check response type */
		UFS_DBG_LOGE("[%s]: RPMB Fail in checking result : %s \r\n", __func__, rpmb_oper_result[idx]);
		return -1;
	} else if (cpu_to_be16(rpmb_frame->result) & 0x80) {
		/*Check Write Counter*/
		UFS_DBG_LOGE("[%s]: RPMB write counter is expired\r\n", __func__);
	}

#if defined(MTK_UFS_DRV_LK)
	// Then check MAC
	ret = rpmb_get_mac(rpmb_frame);
	if (ret) {
		UFS_DBG_LOGE("[%s]: RPMB ufshcd_authen_data_write_rsp_check rpmb_get_mac fail, ret = %d\r\n", __func__, ret);
		return -1;
	}

	if (memcmp(rpmb_frame->key_MAC, shared_hmac_addr1, RPMB_FRM_KEY_MAC_LEN)) {
		UFS_DBG_LOGE("[%s]: RPMB Fail in Mac Check!\n", __func__);
		UFS_DBG_LOGE("Device Return ");
		ufshcd_rpmb_mac_dump((const u8*)rpmb_frame->key_MAC, RPMB_FRM_KEY_MAC_LEN);
		UFS_DBG_LOGE("Host Recalc");
		ufshcd_rpmb_mac_dump((const u8*)shared_hmac_addr1, RPMB_FRM_KEY_MAC_LEN);
		return -1;
	}
#endif

#if defined(MTK_UFS_DRV_DA)
	rpmb_req_copy_data_for_hmac(rpmb_hmac_data, rpmb_frame);

	ret = rpmb_hmac(rpmb_hmac_data, HMAC_TEXT_LEN, rpmb_hmac_derived);
	if (ret) {
		UFS_DBG_LOGE("%s, rpmb_hmac error.\n", __func__);
		return -1;
	}

	if (memcmp(rpmb_frame->key_MAC, rpmb_hmac_derived, RPMB_FRM_KEY_MAC_LEN)) {
		UFS_DBG_LOGE("[%s]: RPMB Fail in Mac Check!\n", __func__);
		UFS_DBG_LOGE("Device Return ");
		ufshcd_rpmb_mac_dump((const u8*)rpmb_frame->key_MAC, RPMB_FRM_KEY_MAC_LEN);
		UFS_DBG_LOGE("Host Recalc");
		ufshcd_rpmb_mac_dump((const u8*)rpmb_hmac_derived, RPMB_FRM_KEY_MAC_LEN);
		return -1;
	}
#endif

	// Check Wr_cnt increasement
	if (rpmb_frame->wr_cnt != cpu_to_be32(wcnt + 1)) {
		UFS_DBG_LOGE("[%s]: RPMB Fail in Write Counter Increasement!\n", __func__);
		return -1;
	}

	return ret;
}
static int ufshcd_authen_data_read_rsp_check(u32 host_id, struct ufs_aio_scsi_cmd *cmd,
        const u8 *nonce_buf, unsigned int  nonce_len, u8 *data_buf,
        enum enum_rpmb_msg type)
{
	struct rpmb_data_frame *pfrm = NULL;
	unsigned int idx = 0;
	int ret = 0;
#if defined(MTK_UFS_DRV_DA)
	u8 rpmb_hmac_data[HMAC_TEXT_LEN] = {0};
	u8 rpmb_hmac_derived[RPMB_FRM_KEY_MAC_LEN] = {0};
#endif

	if (cmd == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB cmd pointer NULL\n", __func__);
		return -1;
	}
	if (nonce_buf == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB nonce_buf pointer NULL\n", __func__);
		return -1;
	}
	if (data_buf == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB ata_buf pointer NULL\n", __func__);
		return -1;
	}

	pfrm = cmd->data_buf;

	if (pfrm == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB pfrm pointer NULL\n", __func__);
		return -1;
	}

	if (pfrm->req_resp != cpu_to_be16(type)) {
		/* Check response type */
		UFS_DBG_LOGE("[%s]: RPMB Fail in checking response type : 0x%x\r\n", __func__, cpu_to_be16(pfrm->req_resp));
		return -1;
	} else if ((idx = (cpu_to_be16(pfrm->result) & 0x7F))) {
		/* Check response type */
		UFS_DBG_LOGE("[%s]: RPMB Fail in checking result : %s \r\n", __func__, rpmb_oper_result[idx]);
		return -1;
	} else if (cpu_to_be16(pfrm->result) & 0x80) {
		/*Check Write Counter*/
		UFS_DBG_LOGE("[%s]: RPMB write counter is expired\r\n", __func__);
	}

	if (memcmp(pfrm->nonce, nonce_buf, nonce_len) ) {
		/*for other response type noce will be "0"*/
		UFS_DBG_LOGE("[%s]: RPMB Fail in checking nonce\r\n", __func__);
		return -1;
	}

#if defined(MTK_UFS_DRV_LK)
	ret = rpmb_get_mac(pfrm);
	if (ret) {
		UFS_DBG_LOGE("[%s]: RPMB rpmb_get_mac fail, ret = %d \r\n", __func__, ret);
		return -1;
	}

	if (memcmp(pfrm->key_MAC, shared_hmac_addr1, RPMB_FRM_KEY_MAC_LEN) ) {
		/*for other response type noce will be "0"*/
		UFS_DBG_LOGE("[%s]: RPMB Fail in checking MAC\r\n", __func__);
		UFS_DBG_LOGE("Device Return");
		ufshcd_rpmb_mac_dump((const u8*)pfrm->key_MAC, RPMB_FRM_KEY_MAC_LEN);
		UFS_DBG_LOGE("Host Recalc");
		ufshcd_rpmb_mac_dump((const u8*)shared_hmac_addr1, RPMB_FRM_KEY_MAC_LEN);
		return -1;
	}
#endif

#if defined(MTK_UFS_DRV_DA)
	rpmb_req_copy_data_for_hmac(rpmb_hmac_data, pfrm);

	ret = rpmb_hmac(rpmb_hmac_data, HMAC_TEXT_LEN, rpmb_hmac_derived);
	if (ret) {
		UFS_DBG_LOGE("%s, rpmb_hmac error.\n", __func__);
		return -1;
	}

	if (memcmp(pfrm->key_MAC, rpmb_hmac_derived, RPMB_FRM_KEY_MAC_LEN)) {
		UFS_DBG_LOGE("[%s]: RPMB Fail in checking MAC\r\n", __func__);
		UFS_DBG_LOGE("Device Return");
		ufshcd_rpmb_mac_dump((const u8*)pfrm->key_MAC, RPMB_FRM_KEY_MAC_LEN);
		UFS_DBG_LOGE("Host Recalc");
		ufshcd_rpmb_mac_dump((const u8*)rpmb_hmac_derived, RPMB_FRM_KEY_MAC_LEN);
		return -1;
	}
#endif

	memcpy(data_buf, pfrm->data, WP_CONF_BLOCK_SIZE); //Copy Secure Write Protect Block to data_buf

	return 0;
}

static int rpmb_authen_secure_write_protect_conf_block_write(u32 host_id, u32 wr_cnt,
        const u8 *dat_buf,
        u32 blk_num, STORAGE_WP_TYPE type, unsigned long blknr, u32 blkcnt, ufs_logical_unit partition, u8 wp_entry)

{
	struct ufs_aio_scsi_cmd cmd;
	struct ufs_hba *hba;
	int ret = 0;
	int tag;

	if (dat_buf == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB data_buf pointer NULL\n", __func__);
		return -1;
	}

	cmd.data_buf = ufs_rpmb_buf;

	hba = ufs_aio_get_host(host_id);

	if (!ufshcd_get_free_tag(hba, &tag))
		return -1;


	/*Step1 write counter read request */
	cmd_scsi_security_protocol_out(&cmd, tag, blk_num,
	                               UFS_SECURITY_PROTOCOL,
	                               RPMB_PROTOCOL_ID);
	//cmd.cmd_data[4] = 0x80; //INC_512 = 1
	ufshcd_swp_conf_block_write_prepare(host_id, &cmd,
	                                    blk_num, wr_cnt, type, blknr, blkcnt, partition, wp_entry, dat_buf);

	ufshcd_rpmb_mac_dump(dat_buf, RPMB_SWP_CB_VALID_DATA_SIZE);

	if ((ret = ufshcd_queuecommand(hba, &cmd))) {
		UFS_DBG_LOGE("[%s]: RPMB Fail in Sending Secure Write Protect Conf. Block write request, ret = %d \n", __func__, ret);
		goto exit;
	}
	/*Step2. Result Register Read Request*/
	cmd_scsi_security_protocol_out(&cmd, tag, 1,
	                               UFS_SECURITY_PROTOCOL,
	                               RPMB_PROTOCOL_ID);
	ufshcd_authen_result_read_req_prepare(&cmd);

	if ((ret = ufshcd_queuecommand(hba, &cmd))) {
		UFS_DBG_LOGE("[%s]: RPMB Fail in Sending result register read request, ret = %d \n", __func__, ret);
		return ret;
	}
	/*Step3 Check Response for Result Read Request*/
	cmd_scsi_security_protocol_in(&cmd, tag, 1, UFS_SECURITY_PROTOCOL, RPMB_PROTOCOL_ID);
	if ((ret = ufshcd_queuecommand(hba, &cmd))) {
		UFS_DBG_LOGE("[%s]: RPMB Fail in Sending request get result register response, ret = %d \n", __func__, ret);
		goto exit;
	}
	/*Step3. Response frame check*/
	if ((ret = ufshcd_authen_data_write_rsp_check(host_id, &cmd, wr_cnt,
	           RPMB_RSP_SEC_WPCB_WRITE))) {
		UFS_DBG_LOGE("[%s]: RPMB WriteCounter Read Resp Check Fail, ret = %d \n", __func__, ret);
		goto exit;
	}
exit:
	ufshcd_put_tag(hba, tag);
	return ret;
}
static int rpmb_authen_secure_write_protect_conf_block_read(u32 host_id,u8 * data_buf,
        u8 *nonce_buf, u32 nonce_len,
        u32 blk_num, ufs_logical_unit partition)
{
	struct ufs_aio_scsi_cmd cmd;
	struct ufs_hba *hba;
	int ret = 0;
	int tag;

	if (data_buf == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB data_buf pointer NULL\n", __func__);
		return -1;
	}
	if (nonce_buf == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB nonce_buf pointer NULL\n", __func__);
		return -1;
	}

	cmd.data_buf = ufs_rpmb_buf;

	hba = ufs_aio_get_host(host_id);

	if (!ufshcd_get_free_tag(hba, &tag))
		return -1;

	/*Step1 authen data read request */
	cmd_scsi_security_protocol_out(&cmd, tag, 1,
	                               UFS_SECURITY_PROTOCOL,
	                               RPMB_PROTOCOL_ID);
	//cmd.cmd_data[4] = 0x80; //INC_512 = 1
	ufshcd_swp_conf_block_read_prepare(host_id, &cmd,
	                                   blk_num, nonce_buf, nonce_len, partition);
	if ((ret = ufshcd_queuecommand(hba, &cmd))) {
		UFS_DBG_LOGE("[%s]: RPMB Fail in Sending Secure Write Protect Conf. Block read request, ret = %d \n", __func__, ret);
		goto exit;
	}
	/*Step2 Send protocol in cmd for data read*/
	cmd_scsi_security_protocol_in(&cmd, tag, blk_num, UFS_SECURITY_PROTOCOL, RPMB_PROTOCOL_ID);
	if ((ret = ufshcd_queuecommand(hba, &cmd))) {
		UFS_DBG_LOGE("[%s]: RPMB Fail in Sending request get result register response, ret = %d \n", __func__, ret);
		goto exit;
	}
	/*Step3 Check Response and parser data*/
	if ((ret = ufshcd_authen_data_read_rsp_check(host_id, &cmd,
	           nonce_buf, nonce_len, data_buf, RPMB_RSP_SEC_WPCB_READ))) {
		UFS_DBG_LOGE("[%s]: RPMB Data Read Resp Check Fail, ret = %d \n", __func__, ret);
		goto exit;
	}

	/* ufshcd_rpmb_mac_dump(data_buf, RPMB_SWP_CB_VALID_DATA_SIZE); */
exit:
	ufshcd_put_tag(hba, tag);
	return ret;
}

static int rpmb_authen_secure_write_protect_conf_block_check(u8 *data_buf, u8 *wp_entry , unsigned long blknr, u32 blkcnt, ufs_logical_unit partition, STORAGE_WP_TYPE type)
{
	static int round = 0;
#ifdef MTK_UFS_OTP
	u8 i = 0;
#endif
	struct secure_wp_config_block *swpcb = (void *)data_buf;

	if (data_buf == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB data_buf pointer NULL\n", __func__);
		return -1;
	}
	if (wp_entry==NULL) {
		UFS_DBG_LOGE("[%s]: RPMB wp_entry pointer NULL\n", __func__);
		return -1;
	}

	UFS_DBG_LOGE("[%s]: Read swpcb\n", __func__);
	dump_swpcb(swpcb);

	/* For Boot1/Boot2, only lock all region. And only have one wp_entry */
	if (partition == UFS_LU_BOOT1 || partition == UFS_LU_BOOT2 || type == WP_DISABLE) {
		memset(&(swpcb->wp_entry[0]), 0x00, WP_ENTRY_SIZE * SWPWP_ENTRY_NUM);
		*wp_entry = 0;
		return 0;
	}

	/* For USER partition */
	/* For OTP: Each Boot, we re-write secure write protect Entries. Only keep OTP entry. */
	/* Because protect start & size may change, SW have no idea how they change. */
#ifdef MTK_UFS_OTP
	/* First enter write protect secure write protect entries, need to keep OTP entery if any */
	if (round == 0) {

		UFS_DBG_LOGD("[%s] Finding OTP entry ...\n", __func__);

		for (i = 0 ; i < (swpcb->data_length / WP_ENTRY_SIZE); i++) {

			UFS_DBG_LOGD("[%s] Checking entry %d, wpt_wtf: 0x%x\n", __func__, i, swpcb->wp_entry[i].wpt_wpf);

			/* If find OTP entries, copy OTP to entry 0, clear other entries, set wp_entry to 1 */
			if ((swpcb->wp_entry[i].wpt_wpf & 0x7) == 0x1) { /* WPT = 0, WPF = 1 */
				UFS_DBG_LOGD("[%s] OTP entry is found in index %d\n", __func__, i);

				if (i != 0) {
					/* only need copy i to 0 if i != 0 */
					UFS_DBG_LOGD("[%s] Copy OTP entry %d -> 0\n", __func__, i);
					memcpy(&(swpcb->wp_entry[0]), &(swpcb->wp_entry[i]), sizeof(struct secure_wp_entry));
					memset(&(swpcb->wp_entry[i]), 0x00, WP_ENTRY_SIZE * (SWPWP_ENTRY_NUM - 1));
				}

				otp_found++;

			} else {
				/* clear all other entries */
				memset(&(swpcb->wp_entry[i]), 0x00, sizeof(struct secure_wp_entry));
			}
		}

		if (otp_found > 1) {
			UFS_DBG_LOGE("[%s] Found multiple OTP entries, ERROR!\n", __func__);
			return UFS_OTP_INVALID_ENTRY;
		}

		if (otp_found)
			*wp_entry = 1;
		else
			*wp_entry = 0;

	} else { /* Not first enter, just write to last free entry later */
		if (swpcb->data_length == WP_MAX_ENTRY_SIZE) {

			UFS_DBG_LOGE("[%s]: RPMB exceed WP entries size!!! \n", __func__);
			return UFS_POWER_ON_WP_ENTRY_FULL;
		}

		*wp_entry = swpcb->data_length / WP_ENTRY_SIZE;
	}

	round++;

	if (otp_found > 0 && round > 1 && type == WP_PERMANENT) {
		/*
		 * If we want to lock OTP but OTP entry is found in configruation block,
		 * do not modify original entry, just write it (old one) to configuration block.
		 *
		 * For round == 1, we still need to write modified configuration block (after clean job)
		 * anyway, so do not return error code here to let programming flow continue.
		 *
		 * For round > 1, just return error code because we do not need any programming.
		 */
		UFS_DBG_LOGD("[%s]: OTP already locked previously! Skipped.\n", __func__);
		return UFS_OTP_ALREADY_LOCKED;
	}
#else
	/*
	 * For non OTP project, always clean wp entries at first time.
	 *
	 * Note: OTP protected region will be clear and reset by this job!
	 */
	if (round == 0) {
		memset(&(swpcb->wp_entry[0]), 0x00, WP_ENTRY_SIZE * SWPWP_ENTRY_NUM);
		*wp_entry = 0;
	} else {
		if (swpcb->data_length == WP_MAX_ENTRY_SIZE) {
			UFS_DBG_LOGE("[%s]: RPMB exceed WP entries size!!! \n", __func__);
			return UFS_POWER_ON_WP_ENTRY_FULL;
		}
		*wp_entry = swpcb->data_length/WP_ENTRY_SIZE;
	}

	round++;
#endif
	return 0;

}

int rpmb_ufs_set_wp(u32 host_id, STORAGE_WP_TYPE type, unsigned long blknr,
                    u32 blkcnt,ufs_logical_unit partition)
{
	u8 *nonce_buf = NULL;
	u8 *data_buf = NULL;
	u32 wr_cnt = 0;
	u32 blk_num = 1;
	int ret = 0;
	u8 wp_entry = 0;

	nonce_buf = (u8 *)malloc(RPMB_FRM_NONCE_LEN);
	if (nonce_buf == NULL)
		return -1;
	memset(nonce_buf, 0, RPMB_FRM_NONCE_LEN);

	data_buf = (u8 *)malloc(256 * blk_num);
	if (data_buf == NULL) {
		free(nonce_buf);
		return -1;
	}
	memset(data_buf, 0x0, 256 * blk_num);

	/* Read Old Secure Write Protect Conf Block */
	ret = rpmb_authen_read_counter(host_id, nonce_buf, RPMB_FRM_NONCE_LEN,
	                               &wr_cnt);
	if (ret) {
		UFS_DBG_LOGE("[%s]: RPMB rpmb_authen_read_counter fail! ret = %d\n", __func__, ret);
		goto exit;
	}

	ret = rpmb_authen_secure_write_protect_conf_block_read(host_id, data_buf, nonce_buf, RPMB_FRM_NONCE_LEN, blk_num, partition);
	if (ret ) {
		UFS_DBG_LOGE("[%s]: RPMB rpmb_authen_secure_write_protect_conf_block_read fail! ret = %d\n", __func__, ret);
		goto exit;
	} else
		UFS_DBG_LOGD("Authenticated Data Read Success!\n");

	ret = rpmb_authen_secure_write_protect_conf_block_check(data_buf, &wp_entry, blknr, blkcnt, partition, type);
	if (ret) {
		UFS_DBG_LOGE("[%s]: RPMB rpmb_authen_secure_write_protect_conf_block_check fail! ret = %d\n", __func__, ret);
		goto exit;
	}

	/* Get Current Write Counter */
	ret = rpmb_authen_read_counter(host_id, nonce_buf, RPMB_FRM_NONCE_LEN,
	                               &wr_cnt);
	if (ret) {
		UFS_DBG_LOGE("[%s]: RPMB rpmb_authen_read_counter fail! ret = %d\n", __func__, ret);
		goto exit;
	}

	/* Authenticated Data Write */
	ret = rpmb_authen_secure_write_protect_conf_block_write(host_id, wr_cnt,
	        data_buf, blk_num, type, blknr, blkcnt, partition, wp_entry);

	if (ret ) {
		UFS_DBG_LOGE("[%s]: RPMB rpmb_authen_secure_write_protect_conf_block_write Fail! ret = %d\n", __func__, ret);
		goto exit;
	} else
		UFS_DBG_LOGD("Authenticated Data Write Success 0x%x blks\n",blk_num);

exit:
	free(nonce_buf);
	free(data_buf);

	return ret;
}

int rpmb_ufs_clr_wp(u32 host_id, ufs_logical_unit partition)
{
	u8 *nonce_buf = NULL;
	u8 *data_buf = NULL;
	u32 wr_cnt = 0;
	u32 blk_num = 1;
	int ret = 0;
	u8 wp_entry = 0;
	struct secure_wp_config_block *swpcb;

	nonce_buf = (u8 *)malloc(RPMB_FRM_NONCE_LEN);
	if (nonce_buf == NULL)
		return -1;
	memset(nonce_buf, 0, RPMB_FRM_NONCE_LEN);

	data_buf = (u8 *)malloc(256 * blk_num);
	if (data_buf == NULL) {
		free(nonce_buf);
		return -1;
	}
	memset(data_buf, 0x0, 256 * blk_num);

	swpcb = (void *)data_buf;
	swpcb->data_length = 16;
	wp_entry = 0;

	/* Get Current Write Counter */
	ret = rpmb_authen_read_counter(host_id, nonce_buf, RPMB_FRM_NONCE_LEN,
	                               &wr_cnt);
	if (ret) {
		UFS_DBG_LOGE("[%s]: RPMB rpmb_authen_read_counter fail! ret = %d\n", __func__, ret);
		goto exit;
	}

	/* Authenticated Data Write */
	ret = rpmb_authen_secure_write_protect_conf_block_write(host_id, wr_cnt,
	        data_buf, blk_num, WP_DISABLE, 0, 0, partition, wp_entry);

	if (ret ) {
		UFS_DBG_LOGE("[%s]: RPMB rpmb_authen_secure_write_protect_conf_block_write Fail! ret = %d\n", __func__, ret);
		goto exit;
	} else
		UFS_DBG_LOGD("Authenticated Data Write Success 0x%x blks\n", blk_num);

exit:
	free(nonce_buf);
	free(data_buf);

	return ret;
}

#if 0 /* Test code only */
int ts_secure_write_protect(u32 host_id)
{
	int ret = 0;
	off_t offset = 0;
	size_t size = 32;
	u8 *buf_test[32] = {1};

	ret = ufs_set_write_protect(host_id, UFS_LU_USER, 0x0, 0x100, WP_POWER_ON);
	if (ret)
		goto exit;
	ret = ufs_set_write_protect(host_id, UFS_LU_USER, 0x100, 0x100, WP_POWER_ON);
	if (ret)
		goto exit;
	ret = ufs_set_write_protect(host_id, UFS_LU_USER, 0x200, 0x100, WP_POWER_ON);
	if (ret)
		goto exit;
	/* Note: Different for other projects need to contain nvram region */
	if (ret)
		ret = ufs_set_write_protect(host_id, UFS_LU_USER, 0xf000, 0xc000, WP_POWER_ON);
	goto exit;
	ret = ufs_set_write_protect(host_id, UFS_LU_BOOT1, 0x0, 0x200, WP_POWER_ON);
	if (ret)
		goto exit;
	if (32!= partition_write("nvram", offset, buf_test, size))
		UFS_DBG_LOGE("[%s]: RPMB write nvram fail!\n", __func__);
	if (32!= partition_write("preloader", offset, buf_test, size))
		UFS_DBG_LOGE("[%s]: RPMB write preloader fail!\n", __func__);

exit:
	return ret;
}
#endif

int ufs_set_write_protect(int dev_num, ufs_logical_unit partition,unsigned long blknr, u32 blkcnt, STORAGE_WP_TYPE type)
{
	int err = UFS_POWER_ON_WP_ERR;

	UFS_DBG_LOGI("[%s]: RPMB ufs_set_write_protect start: partition = %d blknr = 0x%lx, blkcnt = 0x%x, wp_type = %d  \n", __func__, partition, blknr, blkcnt, type);

	if ((type == WP_POWER_ON || type == WP_PERMANENT || type == WP_PERMANENT_AWP) && partition == UFS_LU_USER) {
		UFS_DBG_LOGD("[%s]: blknr: 0x%lx blkcnt: 0x%x\n",__func__ , blknr, blkcnt);
		err = rpmb_ufs_set_wp(dev_num,type, blknr, blkcnt, partition);
		if (err) {
			UFS_DBG_LOGE("[%s]: RPMB rpmb_ufs_set_wp err! ret = %d\n", __func__, err);
			return err;
		}
	} else if (((type == WP_POWER_ON || type == WP_PERMANENT_AWP) && partition == UFS_LU_BOOT1) ||
	           ((type == WP_POWER_ON || type == WP_PERMANENT_AWP) && partition == UFS_LU_BOOT2)) {
		if (partition == UFS_LU_BOOT1) {
			err = rpmb_ufs_set_wp(dev_num, type, blknr, blkcnt, UFS_LU_BOOT1);
			if (err) {
				UFS_DBG_LOGE("[%s]: RPMB rpmb_ufs_set_wp boot1 partition err ret= %d\n", __func__, err);
				return err;
			}
		} else if (partition == UFS_LU_BOOT2) {
			err = rpmb_ufs_set_wp(dev_num, type, blknr, blkcnt, UFS_LU_BOOT2);
			if (err) {
				UFS_DBG_LOGE("[%s]: RPMB rpmb_ufs_set_wp boot2 partition err%d\n", __func__,err);
				return err;
			}
		}
	} else if (type == WP_TEMPORARY ) {
		UFS_DBG_LOGE("[%s]: RPMB WP Type Not Support\n", __func__);
		return 0;
	} else if (type == WP_DISABLE) {
		err = rpmb_ufs_clr_wp(dev_num, partition);
		if (err) {
			UFS_DBG_LOGE("[%s]: RPMB rpmb_ufs_clear_wp err%d\n", __func__, err);
			return err;
		}
		return 0;
	} else if (partition == UFS_LU_UNKNOWN ||partition == UFS_LU_RPMB
	           ||partition == UFS_LU_END) {
		UFS_DBG_LOGE("[%s]: RPMB partition Not Support\n", __func__);
		return 0;
	} else {
		return err;
	}

	return err;
}

int ufs_clr_write_protect(void)
{
	int err = 0;

	err = ufs_set_write_protect(0, UFS_LU_BOOT1, 0, 0, WP_DISABLE);
	err |= ufs_set_write_protect(0, UFS_LU_BOOT2, 0, 0, WP_DISABLE);
	err |= ufs_set_write_protect(0, UFS_LU_USER, 0, 0, WP_DISABLE);

	return err;
}
#endif /* #if defined(MTK_UFS_DRV_LK) || defined(MTK_UFS_DRV_DA) */

#if defined(MTK_UFS_DRV_LK)
static int ufshcd_rpmb_read_data_req_prepare(struct ufs_aio_scsi_cmd *cmd,
        u32 size, u16 addr)
{
	struct rpmb_data_frame *pfrm = NULL;

	if (cmd == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB cmd pointer NULL\n", __func__);
		return -1;
	}

	pfrm = cmd->data_buf;

	if (pfrm == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB pfrm pointer NULL\n", __func__);
		return -1;
	}

	memset(pfrm, 0, sizeof(struct rpmb_data_frame));

	/* Prepare write frame */
	pfrm->addr = cpu_to_be16(addr);
	pfrm->blk_cnt = cpu_to_be16(1);
	pfrm->req_resp = cpu_to_be16(RPMB_REQ_AUTHEN_READ);

	return 0;
}

int ufs_rpmb_read_data(u32 addr, u32 *buf, u32 buf_len)
{
	struct ufs_aio_scsi_cmd cmd;
	struct ufs_hba *hba;
	int tag;
	u8 *nonce_buf = NULL;
	u8 *data_buf;
	u32 host_id = 0;
	u32 blk_len;
	u32 left_size = buf_len;
	u32 tran_size;
	u32 i;
	int ret = 0;

	cmd.data_buf = ufs_rpmb_buf;
	hba = ufs_aio_get_host(0);

	if (buf_len % RPMB_FRM_DATA_LEN != 0) {
		UFS_DBG_LOGE("[%s]: RPMB read write need 256 ytes align\n", __func__);
		return -1;
	}
	blk_len = buf_len / RPMB_FRM_DATA_LEN;
	data_buf = (u8 *)buf;

	if (!ufshcd_get_free_tag(hba, &tag))
		return -1;

	nonce_buf = (u8 *)malloc(RPMB_FRM_NONCE_LEN);
	if (nonce_buf == NULL)
		return -1;
	memset(nonce_buf, 0, RPMB_FRM_NONCE_LEN);

	for (i = 0; i < blk_len; i++) {
		/* Step 1. Authen data read request */
		if (left_size >= RPMB_FRM_DATA_LEN)
			tran_size = RPMB_FRM_DATA_LEN;
		else
			tran_size = left_size;
		cmd_scsi_security_protocol_out(&cmd, tag, 1,
					       UFS_SECURITY_PROTOCOL,
					       RPMB_PROTOCOL_ID);
		ret = ufshcd_rpmb_read_data_req_prepare(&cmd, tran_size, addr + i);
		if (ret) {
			UFS_DBG_LOGE("[%s]: RPMB Fail in prepare frame, ret = %d \n", __func__, ret);
			goto exit;
		}
		ret = ufshcd_queuecommand(hba, &cmd);
		if (ret) {
			UFS_DBG_LOGE("[%s]: RPMB Fail in send read frame, ret = %d \n", __func__, ret);
			goto exit;
		}

		/* Step 2. Authen data read response */
		cmd_scsi_security_protocol_in(&cmd, tag, 1,
					       UFS_SECURITY_PROTOCOL,
					       RPMB_PROTOCOL_ID);
		ret = ufshcd_queuecommand(hba, &cmd);
		if (ret) {
			UFS_DBG_LOGE("[%s]: RPMB Fail in get read response, ret = %d \n", __func__, ret);
			goto exit;
		}

		/* Step3. Check Response and parser data*/
		ret = ufshcd_authen_data_read_rsp_check(host_id, &cmd,
			   nonce_buf, RPMB_FRM_NONCE_LEN, (data_buf + i * RPMB_FRM_DATA_LEN), RPMB_RSP_AUTHEN_READ);
		if (ret) {
			UFS_DBG_LOGE("[%s]: RPMB Data Read Resp Check Fail, ret = %d \n", __func__, ret);
			goto exit;
		}

		left_size -= tran_size;
	}

exit:
	ufshcd_put_tag(hba, tag);
	free(nonce_buf);

	return ret;
}

static int ufshcd_rpmb_write_data_req_prepare(struct ufs_aio_scsi_cmd *cmd,
        const u8 *data_buf, u32 size, u32 wr_cnt, u16 addr)
{
	struct rpmb_data_frame *pfrm = NULL;
	int ret = 0;

	if (cmd == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB cmd pointer NULL\n", __func__);
		return -1;
	}
	if (data_buf == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB data pointer NULL\n", __func__);
		return -1;
	}

	pfrm = cmd->data_buf;

	if (pfrm == NULL) {
		UFS_DBG_LOGE("[%s]: RPMB pfrm pointer NULL\n", __func__);
		return -1;
	}

	memset(pfrm, 0, sizeof(struct rpmb_data_frame));

	/* Prepare write frame */
	memcpy(pfrm->data, data_buf, size);
	pfrm->wr_cnt = cpu_to_be32(wr_cnt);
	pfrm->addr = cpu_to_be16(addr);
	pfrm->blk_cnt = cpu_to_be16(1);
	pfrm->req_resp = cpu_to_be16(RPMB_REQ_AUTHEN_WRITE);

	/* Compute hmac. */
	ret = rpmb_get_mac(pfrm);
	if (ret) {
		UFS_DBG_LOGE("[%s]: rpmb_get_mac fail\r\n", __func__);
		return -1;
	}
	memcpy(pfrm->key_MAC, shared_hmac_addr1, RPMB_FRM_KEY_MAC_LEN);

	return 0;
}

int ufs_rpmb_write_data(u32 addr, u32 *buf, u32 buf_len)
{
	struct ufs_aio_scsi_cmd cmd;
	struct ufs_hba *hba;
	int tag;
	u8 *nonce_buf = NULL;
	u8 *data_buf;
	u32 wr_cnt = 0;
	u32 host_id = 0;
	u32 blk_len;
	u32 left_size = buf_len;
	u32 tran_size;
	u32 i;
	int ret = 0;

	cmd.data_buf = ufs_rpmb_buf;
	hba = ufs_aio_get_host(0);
	if (buf_len % RPMB_FRM_DATA_LEN != 0) {
		UFS_DBG_LOGE("[%s]: RPMB read write need 256 ytes align\n", __func__);
		return -1;
	}
	blk_len = buf_len / RPMB_FRM_DATA_LEN;
	data_buf = (u8 *)buf;

	/* Read RPMB write counter */
	nonce_buf = (u8 *)malloc(RPMB_FRM_NONCE_LEN);
	if (nonce_buf == NULL)
		return -1;
	memset(nonce_buf, 0, RPMB_FRM_NONCE_LEN);
	ret = rpmb_authen_read_counter(host_id, nonce_buf, RPMB_FRM_NONCE_LEN,
				       &wr_cnt);
	if (ret) {
		UFS_DBG_LOGE("[%s]: RPMB rpmb_authen_read_counter fail! ret = %d\n", __func__, ret);
		goto free_nonce_buf;
	}

	if (!ufshcd_get_free_tag(hba, &tag)) {
		ret = -1;
		goto free_nonce_buf;
	}

	for (i = 0; i < blk_len; i++) {
		/* Step 1. Authen data write request */
		if (left_size >= RPMB_FRM_DATA_LEN)
			tran_size = RPMB_FRM_DATA_LEN;
		else
			tran_size = left_size;
		cmd_scsi_security_protocol_out(&cmd, tag, 1,
					       UFS_SECURITY_PROTOCOL,
					       RPMB_PROTOCOL_ID);
		ret = ufshcd_rpmb_write_data_req_prepare(&cmd, (data_buf + i * RPMB_FRM_DATA_LEN), tran_size, wr_cnt, addr + i);
		if (ret) {
			UFS_DBG_LOGE("[%s]: RPMB Fail in prepare frame, ret = %d \n", __func__, ret);
			goto exit;
		}
		ret = ufshcd_queuecommand(hba, &cmd);
		if (ret) {
			UFS_DBG_LOGE("[%s]: RPMB Fail in send write frame, ret = %d \n", __func__, ret);
			goto exit;
		}

		/* Step 2. Result Register Read Request */
		cmd_scsi_security_protocol_out(&cmd, tag, 1,
					       UFS_SECURITY_PROTOCOL,
					       RPMB_PROTOCOL_ID);
		ufshcd_authen_result_read_req_prepare(&cmd);
		ret = ufshcd_queuecommand(hba, &cmd);
		if (ret) {
			UFS_DBG_LOGE("[%s]: RPMB Fail in sending result register read request, ret = %d \n", __func__, ret);
			goto exit;
		}

		/* Step3. Check Response for Result Read Request */
		cmd_scsi_security_protocol_in(&cmd, tag, 1,
						UFS_SECURITY_PROTOCOL,
						RPMB_PROTOCOL_ID);
		if ((ret = ufshcd_queuecommand(hba, &cmd))) {
			UFS_DBG_LOGE("[%s]: RPMB Fail in sending request get result register response, ret = %d \n", __func__, ret);
			goto exit;
		}

		/* Step3. Response frame check */
		if ((ret = ufshcd_authen_data_write_rsp_check(host_id, &cmd, wr_cnt,
			   RPMB_RSP_AUTHEN_WRITE))) {
			UFS_DBG_LOGE("[%s]: RPMB WriteCounter Read Resp Check Fail, ret = %d \n", __func__, ret);
			goto exit;
		}

		left_size -= tran_size;
		wr_cnt++;
	}

exit:
	ufshcd_put_tag(hba, tag);

free_nonce_buf:
	free(nonce_buf);

	return ret;
}

#endif //#if defined(MTK_UFS_DRV_LK)

