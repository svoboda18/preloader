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

#include "typedefs.h"
#include "platform.h"
#include "download.h"
#include "sec.h"
#include "cust_sec_ctrl.h"
#include "buffer.h"
#include "sec_region.h"
#include "dram_buffer.h"
#if CFG_ATF_SUPPORT
#include "tz_init.h"
#include "tz_mem.h"
#endif
#include <pl_cmd_error.h>
#include <pal_log.h>


#if CFG_PRELOADER_AS_DA
typedef enum {
    IMAGE_LK_ID = 0,
    IMAGE_LOGO_ID = 1,
    IMAGE_BOOT_ID = 2,
    IMAGE_ATF_ID = 3,
    IMAGE_MAX_NUM = 4
} image_index_t;

typedef struct {
	u8 partition_name[64];
	u32 start_addr;
	u32 length;
} ImageInfo;
ImageInfo g_image_list[IMAGE_MAX_NUM];

#define tee1_buf g_dram_buf->tee1_buf
#endif

static struct bldr_comport *dlcomport = NULL;

#if CFG_USB_DOWNLOAD

#define MOD     "[UDL]"

//#define DEBUG
#ifdef DEBUG
#define DBGMSG(fmt, args...)             pal_log_info(fmt, ##args)
#define DBG_SND_BUF(buf, size) \
	do { \
		u32 i;\
		for (i = 0; i < size; i++) \
		pal_log_info("%s TGT--BUF[%d]=(0x%x)->TOOL\n", MOD, i, buf[i]); \
	}while(0)

#define DBG_RCV_BUF(buf, size)

#define DBG_RCV_BUF1(buf, size) \
	do { \
		u32 i;\
		for (i = 0; i < size; i++) \
		pal_log_info("%s TGT<-BUF[%d]=(0x%x)--TOOL\n", MOD, i, buf[i]); \
	}while(0)
#else
#define DBGMSG(fmt, args...)             do{}while(0)
#define DBG_SND_BUF(buf, size)           do{}while(0)
#define DBG_RCV_BUF(buf, size)           do{}while(0)
#endif

u8 g_da_verified = 0;
#define PART_HDR_BUF_SIZE 512
#define part_hdr_buf (g_dram_buf->part_hdr_buf)

#define bootarg g_dram_buf->bootarg

extern unsigned int seclib_get_me_id(unsigned char *me_id, unsigned int size);
extern unsigned int seclib_get_soc_id(unsigned char *soc_id, unsigned int size);

static int usbdl_put_data(u8 *buf, u32 size)
{
	DBG_SND_BUF(buf, size);
	return dlcomport->ops->send(buf, size);
}

static int usbdl_get_data(u8 *buf, u32 size)
{
	int ret = dlcomport->ops->recv(buf, size, 0);
	DBG_RCV_BUF(buf, size);
	return ret;
}

static int usbdl_put_byte(u8 byte)
{
	DBGMSG("%s TGT--(0x%x)->TOOL\n", MOD, byte);
	return dlcomport->ops->send(&byte, 1);
}

static int usbdl_put_word(u16 word)
{
	u8  buf[2];
	u8 *ptr = &buf[0];

	DBGMSG("%s TGT--(0x%x)->TOOL\n", MOD, word);

	/* high byte is first */
	*ptr++ = word >> 8;
	*ptr = word & 0xff;

	return dlcomport->ops->send(buf, 2);
}

static int usbdl_put_dword(u32 dword)
{
	u8  buf[4];
	u8 *ptr = &buf[0];

	DBGMSG("%s TGT--(0x%x)->TOOL\n", MOD, dword);

	/* high byte is first */
	*ptr++ = (u8)(dword >> 24) & 0xff;
	*ptr++ = (u8)(dword >> 16) & 0xff;
	*ptr++ = (u8)(dword >> 8) & 0xff;
	*ptr = (u8)(dword >> 0) & 0xff;

	return dlcomport->ops->send(buf, 4);
}

static int usbdl_get_byte(u8 *byte)
{
	int ret = dlcomport->ops->recv(byte, 1, 0);
	DBGMSG("%s TGT<-(0x%x)--TOOL\n", MOD, *byte);
	return ret;
}

static int usbdl_get_word(u16 *word)
{
	int ret;
	u8  buf[2];

	if (0 == (ret = dlcomport->ops->recv(buf, 2, 0))) {
		/* high byte is first */
		*word = (buf[0] << 8) | (buf[1]);
		DBGMSG("%s TGT<-(0x%x)--TOOL\n", MOD, *word);
	}

	return ret;
}

static int usbdl_get_dword(u32 *dword)
{
	int ret, i;
	u8 buf[4];
	u8 *ptr = &buf[0];
	u32 result = 0;

	if (0 == (ret = dlcomport->ops->recv(buf, 4, 0))) {
		for (i = 3; i >= 0; i--) {
			result |= (*ptr++ << (8 * i));
		}
		*dword = result;
		DBGMSG("%s TGT<-(0x%x)--TOOL\n", MOD, result);
	}

	return ret;
}

static u32 usbdl_write16(bool echo)
{
	u32 index;
	u32 base_addr=0;
	u32 addr=0;
	u32 len16=0;
	u32 len8=0;
	u16 data=0;
	u32 status=0;

	usbdl_get_dword(&base_addr);
	usbdl_put_dword(base_addr);

	usbdl_get_dword(&len16);
	usbdl_put_dword(len16);

	/* check addr alignment */
	if (0 != (base_addr & (2-1))) {
		status = -1;
		goto end;
	}

	/* check len */
	if (0 == len16) {
		status = -2;
		goto end;
	}

	/* convert half-word(2B) length to byte length */
	len8 = (len16<<1);

	// overflow attack check
	if (len16 >= len8) {
		status = -3;
		goto end;
	}

	/* check if addr range is valid */
	status = sec_region_check(base_addr, len8, OP_WRITE);
	if (status) {
		status = WRITE_REGION_CHK_FAIL;
		goto end;
	}

	/* return status */
	usbdl_put_word(status);

	for (index = 0; index < len16; index++) {
		usbdl_get_word(&data);
		if (echo)
			usbdl_put_word(data);

		addr = base_addr + (index<<1);
		*(u16*)(addr) = data;
	}

end:
	/* return status */
	usbdl_put_word(status);

	return status;
}

static u32 usbdl_write32(bool echo)
{
	u32 index;
	u32 base_addr=0;
	u32 addr=0;
	u32 len32=0;
	u32 len8=0;
	u32 data=0;
	u32 status=0;

	usbdl_get_dword(&base_addr);
	usbdl_put_dword(base_addr);

	usbdl_get_dword(&len32);
	usbdl_put_dword(len32);

	/* check addr alignment */
	if (0 != (base_addr & (4-1))) {
		status = -1;
		goto end;
	}

	/* check len */
	if (0 == len32) {
		status = -2;
		goto end;
	}

	/* convert dword(4B) length to byte length */
	len8 = (len32<<2);

	/* overflow attack check */
	if (len32 >= len8) {
		status = -3;
		goto end;
	}

	/* check if addr range is valid */
	status = sec_region_check(base_addr, len8, OP_WRITE);
	if (status) {
		status = WRITE_REGION_CHK_FAIL;
		goto end;
	}

	/* return status */
	usbdl_put_word(status);

	for (index = 0; index < len32; index++) {
		usbdl_get_dword(&data);
		if (echo)
			usbdl_put_dword(data);

		addr = base_addr + (index<<2);
		*(u32*)(addr) = data;
	}

end:
	/* return status */
	usbdl_put_word(status);

	return status;
}

static u32 usbdl_read16(bool legacy)
{
	u32 index;
	u32 base_addr=0;
	u32 len16=0;
	u32 len8=0;
	u16 data=0;
	u32 status=0;

	usbdl_get_dword(&base_addr);
	usbdl_put_dword(base_addr);

	usbdl_get_dword(&len16);
	usbdl_put_dword(len16);

	/* check addr alignment */
	if (0 != (base_addr & (2-1))) {
		status = -1;
		goto end;
	}

	/* check len */
	if (0 == len16) {
		status = -2;
		goto end;
	}

	/* convert half-word(2B) length to byte length */
	len8 = (len16 << 1);

	/* overflow attack check */
	if (len16 >= len8) {
		status = -3;
		goto end;
	}

	/* check if addr range is valid */
	status = sec_region_check(base_addr, len8, OP_READ);
	if (status) {
		status = READ_REGION_CHK_FAIL;
		goto end;
	}

	if (!legacy) {
		/* return status */
		usbdl_put_word(status);
	}

	for (index = 0; index < len16; index++) {
		data = *(u16*)(base_addr + (index << 1));
		usbdl_put_word(data);
	}

end:
	if(!legacy) {
		/* return status */
		usbdl_put_word(status);
	}

	return status;
}

static u32 usbdl_read32(bool echo)
{
	u32 index;
	u32 base_addr=0;
	u32 len32=0;
	u32 len8=0;
	u32 data=0;
	u32 status=0;

	usbdl_get_dword(&base_addr);
	usbdl_put_dword(base_addr);

	usbdl_get_dword(&len32);
	usbdl_put_dword(len32);

	/* check addr alignment */
	if (0 != (base_addr&(4-1))) {
		status = -1;
		goto end;
	}

	/* check len */
	if (0 == len32) {
		status = -2;
		goto end;
	}

	/* convert word(4B) length to byte length */
	len8 = (len32<<2);

	/* overflow attack check */
	if (len32 >= len8) {
		status = -3;
		goto end;
	}

	/* check if addr range is valid */
	status = sec_region_check(base_addr, len8, OP_READ);
	if (status) {
		status = READ_REGION_CHK_FAIL;
		goto end;
	}

	/* return status */
	usbdl_put_word(status);

	for(index=0; index<len32; index++) {
		data = *(u32*)(base_addr + (index<<2));
		usbdl_put_dword(data);
	}

end:
	/* return status */
	usbdl_put_word(status);

	return status;
}

static void usbdl_get_hw_code(void)
{
	u16 status = 0;
	u16 hwcode = *(u16*)APHW_CODE;

	usbdl_put_word(hwcode);
	usbdl_put_word(status);
}

static void usbdl_get_hw_sw_ver(void)
{
	u16 status = 0;

	usbdl_put_word(*(u16*)APHW_SUBCODE);
	usbdl_put_word(*(u16*)APHW_VER);
	usbdl_put_word(*(u16*)APSW_VER);
	usbdl_put_word(status);
}

static u16 usbdl_verify_da(u8* da_addr, u32 da_len, u32 sig_len)
{
	u16 status = 0;

#ifdef MTK_SECURITY_SW_SUPPORT
#ifdef SEC_USBDL_WITHOUT_SLA_ENABLE
	u8 *sig_addr;

	if (FALSE == seclib_sec_usbdl_enabled(TRUE)) {
		pal_log_info("usbdl_vfy_da:disabled\n");
	} else {
		/* init download agent authentication key */
		if (da_auth_init() != 0) {
			status = E_DAA_SIG_VERIFY_FAILED;
			goto end;
		}

		/* check for da relocate size */
		if (da_len > DA_RAM_LENGTH) {
			status = E_DA_EXCEED_MAX_DA_BLOCKS;
			goto end;
		}

		/* validate download agent */
		sig_addr = (u8*)da_addr + (da_len - sig_len);

		if (sec_auth(da_addr, (da_len - sig_len), sig_addr, sig_len) != 0) {
			status = E_DAA_SIG_VERIFY_FAILED;
		}
	}
#else
	if (FALSE == seclib_sec_usbdl_enabled(TRUE)) {
		pal_log_info("usbdl_vfy_da:disabled\n");
	} else {
		pal_log_info("usbdl_vfy_da:forbidden\n");
		status = E_DAA_SIG_VERIFY_FAILED;
	}
#endif
#endif

end:

	return status;
}

#if CFG_PRELOADER_AS_DA
#define lk      "lk"
#define logo    "logo"
#define boot    "boot"
#define atf     "tee1"

#define CFG_LK_ADDR     CFG_UBOOT_MEMADDR
#define CFG_LK_SIZE     0x900000    //9MB
#define CFG_LOGO_ADDR   0x4E900000
#define CFG_LOGO_SIZE   0x600000    //6MB
#define CFG_BOOT_ADDR   0x40080000
#define CFG_BOOT_SIZE   0x4000000   //64MB
#define CFG_ATF_ADDR    ATF_ROM_LOAD_ADDR
#define CFG_ATF_SIZE    0x80000     //512KB

static void usbdl_init_image(void)
{
	image_index_t id;

	// 0. Init image list
	memcpy(g_image_list[IMAGE_LK_ID].partition_name, lk, sizeof(lk));
	g_image_list[IMAGE_LK_ID].start_addr = CFG_LK_ADDR - PART_HDR_BUF_SIZE;
	g_image_list[IMAGE_LK_ID].length = CFG_LK_SIZE;

	memcpy(g_image_list[IMAGE_LOGO_ID].partition_name, logo, sizeof(logo));
	g_image_list[IMAGE_LOGO_ID].start_addr = CFG_LOGO_ADDR - PART_HDR_BUF_SIZE;
	g_image_list[IMAGE_LOGO_ID].length = CFG_LOGO_SIZE;

	memcpy(g_image_list[IMAGE_BOOT_ID].partition_name, boot, sizeof(boot));
	g_image_list[IMAGE_BOOT_ID].start_addr = CFG_BOOT_ADDR;
	g_image_list[IMAGE_BOOT_ID].length = CFG_BOOT_SIZE;

	memcpy(g_image_list[IMAGE_ATF_ID].partition_name, atf, sizeof(atf));
	g_image_list[IMAGE_ATF_ID].start_addr = CFG_ATF_ADDR - PART_HDR_BUF_SIZE;
	g_image_list[IMAGE_ATF_ID].length = CFG_ATF_SIZE;

	for (id = IMAGE_LK_ID; id < IMAGE_MAX_NUM; id++) {
		pal_log_info("%s part: %s, addr: 0x%x, size: 0x%x\n", MOD, g_image_list[id].partition_name, g_image_list[id].start_addr, g_image_list[id].length);
	}
}

// use this check sum provided by Xiao
static u32 checksum_plain(u8 *data, u32 length)
{
	register u32 chksum = 0;
	u8 *ptr = data;
	u32 i = 0;

	for (i = 0; i < (length & (~3)); i += sizeof(u32))
	{
		chksum += (*(u32 *)(data + i));
	}

	if (i < length) //can not aligned by 4
	{
		ptr += i;
		for (i = 0; i < (length & 3); i++) //remain
		{
			chksum += ptr[i];
		}
	}
	return chksum;
}

static void usbdl_boot_image(void)
{
	extern void bldr_jump(u32 addr, u32 arg1, u32 arg2);

	u8 img_name[64] = {0};
	u32 jump_arg;
	u16 status = 0;

	// 1. get partition name
	usbdl_get_data(img_name, 64);
	pal_log_info("%s BOOT: %s\n", MOD, img_name);

#if CFG_ATF_SUPPORT
	trustzone_pre_init();
#endif

	g_boot_mode = FASTBOOT;
	platform_set_boot_args();

#if CFG_ATF_SUPPORT
	trustzone_post_init();
#endif

#if CFG_BOOT_ARGUMENT_BY_ATAG
	jump_arg = (u32)&(g_dram_buf->boottag);
#else
	jump_arg = (u32)&bootarg;
#endif

	// 2. check if valid then jump
	if (!strcmp(img_name, lk)) {
		usbdl_put_word(status);
		pal_log_err("%s Jump to LK\n", MOD);
		bldr_jump(g_image_list[IMAGE_LK_ID].start_addr + PART_HDR_BUF_SIZE, jump_arg, sizeof(boot_arg_t));
	} else if (!strcmp(img_name, atf)) {
		usbdl_put_word(status);
		pal_log_err("%s Jump to ATF\n", MOD);
		bldr_jump64(g_image_list[IMAGE_LK_ID].start_addr + PART_HDR_BUF_SIZE, jump_arg, sizeof(boot_arg_t));
	} else {
		status = 1;
		usbdl_put_word(status);
		pal_log_err("%s Unknown Jump\n", MOD);
	}
}

static void usbdl_send_image(void)
{
	u32 img_addr = 0;
	u32 img_len;
	image_index_t id;
	u16 status = 0; // 0 = pass, 1 = fail
	u8 img_name[64] = {0};
	u32 checksum32 = 0;
	u32 my_checksum32 = 0;
	u8 debug_buf[32] = {0};

	// 1. get partition name
	usbdl_get_data(img_name, 64);
	pal_log_info("%s SEND: name = %s\n", MOD, img_name);

	// 2. get length, which includes sig len
	usbdl_get_dword(&img_len);
	pal_log_info("%s SEND: len = 0x%x\n", MOD, img_len);

	// 3. check lengh and partition name if valid.
	for (id = IMAGE_LK_ID; id < IMAGE_MAX_NUM; id++) {
		if ((!strcmp(img_name, g_image_list[id].partition_name)) && (img_len > 0) && (img_len <= g_image_list[id].length)) {
			pal_log_info("%s SEND: Verify PASS\n", MOD);
			if(id == IMAGE_ATF_ID) // in atf case, move trustzon.bin to dram buffer then extract atf.
				img_addr = (u32)tee1_buf;
			else
			img_addr = g_image_list[id].start_addr;
			break;
		}
	}
	pal_log_info("%s SEND: addr = 0x%x\n", MOD, img_addr);
	if (!img_addr) {
		pal_log_err("%s Unknown image\n", MOD);
		//return;
	}

	usbdl_put_word(status);

	// 4. receive Image data (no echo)
	usbdl_get_data((u8 *)img_addr, img_len);

	// 5. calculate new check sum
	my_checksum32 = checksum_plain((u8 *)img_addr, img_len);
	pal_log_info("%s SEND: PL Checksum = 0x%x\n", MOD, my_checksum32);

	usbdl_get_dword(&checksum32);
	pal_log_info("%s SEND: XFLASH checksum = 0x%x\n", MOD, checksum32);

	// 6. verify checksum
	if (my_checksum32 != checksum32) {
		pal_log_err("%s checksum mismatch!\n", MOD);
		return;
	}

	// 7. return status 2 by verify the check sum.
	//usbdl_put_word(status);

      if(id == IMAGE_ATF_ID) {
		// move atf from tee1_buf to CFG_ATF_ADDR
          part_hdr_t * cur_hdr = (part_hdr_t *)img_addr;
          u32 cur_part = img_addr + PART_HDR_BUF_SIZE;
          u32 maddr = cur_hdr->info.maddr;
          u32 dsize = cur_hdr->info.dsize;
          pal_log_info("%s MOVE ATF dst=0x%x, src=0x%x, size=0x%x\n", MOD, (g_image_list[id].start_addr+PART_HDR_BUF_SIZE), cur_part, dsize);
          memcpy( (u32 *)(g_image_list[id].start_addr+PART_HDR_BUF_SIZE), (u32 *)cur_part, dsize);

		// move secure os to tee address
#if CFG_TEE_SUPPORT
          cur_hdr = (part_hdr_t *)(cur_part + dsize);
          cur_part = cur_part + dsize + PART_HDR_BUF_SIZE;
          maddr = cur_hdr->info.maddr;
          dsize = cur_hdr->info.dsize;

		maddr = tee_get_load_addr(maddr);
          pal_log_info("%s MOVE TEE dst=0x%x, src=0x%x, size=0x%x\n", MOD, maddr, cur_part, dsize);
          memcpy((u32 *)maddr, (u32 *)cur_part, dsize);

		int ret = tee_verify_image(&maddr);
		if (ret)
			pal_log_err("%s TEE verify fail\n", MOD);
		tee_set_entry(maddr);
#endif
	}
}
#endif

static void usbdl_send_da(void)
{
	u32 da_addr = 0;
	u32 da_len = 0;
	u32 sig_len = 0;
	u32 i = 0;
	u32 count = 0;
	u32 last_byte = 0;
	u16 data16 = 0;
	u16 chksum16 = 0;
	u16 status = 0;
	da_info_t da_info;

	/* get da addr */
	usbdl_get_dword(&da_addr);
	usbdl_put_dword(da_addr);

	/* get da len, which includes sig len */
	usbdl_get_dword(&da_len);
	usbdl_put_dword(da_len);

	/* get DA sig len */
	usbdl_get_dword(&sig_len);
	usbdl_put_dword(sig_len);

	/* check if DA range is valid */

	/* check if DA overlap previous downloaded DA */

	/* return status 1 */
	usbdl_put_word(status);

	memset(&da_info, 0x0, sizeof(da_info_t));
	da_info.len = da_len;
	da_info.sig_len = sig_len;
	platform_set_dl_boot_args(&da_info);

	/* receive DA data (no echo) */
#ifdef CFG_DA_RAM_ADDR
	da_addr = CFG_DA_RAM_ADDR;
#endif
	usbdl_get_data((u8*)da_addr, da_len);

	/* calculate checksum */
	count     = (da_len >> 1);
	last_byte = (da_len & (2-1));
	chksum16  = 0;
	for (i = 0; i < count; i++) {
		chksum16 ^= *(u16*)(da_addr + (i<<1));
	}
	if (0 < last_byte) {
		data16 = *(u8*)(da_addr + (count<<1));
		chksum16 ^= data16;
	}
	usbdl_put_word(chksum16);

	status = usbdl_verify_da((u8*)da_addr, da_len, sig_len);

#ifdef DBG_PRELOADER
	pal_log_info("%s usbdl_send_da: status2 is 0x%x\n", MOD, status);
#endif

	/* return status 2 */
	usbdl_put_word(status);

	/* check status. if verify failed, clean invalid da to prevent re-try attack */
	if (STATUS_OK != status) {
		pal_log_err("%s usbdl_send_da: clean invalid da\n", MOD);
		memset((void*)da_addr,0x0,da_len);
		ASSERT(0);
	}

	/* to prevent tool from sending "jump da" without sending "send da" command first */
	g_da_verified = 1;

	return;
}

static void usbdl_jump_da(void)
{
	extern void bldr_jump(u32 addr, u32 arg1, u32 arg2);

	u32 da_addr = 0;
	u16 status = 0;
	DownloadArg *da_arg;
	u32 jump_arg = 0;

	/* get DA jump addr */
	usbdl_get_dword(&da_addr);
	usbdl_put_dword(da_addr);

	if (g_da_verified != 1)
		status = E_DAA_SIG_VERIFY_FAILED;

	usbdl_put_word(status);

	if (STATUS_OK != status) {
		pal_log_err("%s usbdl_jump_da: %x\n", MOD, status);
		ASSERT(0);
	}

	/* jump to da */
#ifdef CFG_DA_RAM_ADDR
	da_addr = CFG_DA_RAM_ADDR;
#endif
	da_arg = (DownloadArg*)(CFG_DA_RAM_ADDR - sizeof(DownloadArg));

	BUG_ON((u32)da_arg > CFG_DA_RAM_ADDR);

	da_arg->magic = DA_ARG_MAGIC;
	da_arg->ver   = DA_ARG_VER;
	da_arg->flags = DA_FLAG_SKIP_PLL_INIT | DA_FLAG_SKIP_EMI_INIT;

	apmcu_icache_invalidate();
	apmcu_disable_icache();
	apmcu_isb();
	apmcu_disable_smp();

#if FEATURE_DOWNLOAD_SCREEN
	{
		/* delegate download screen and jump da to second bootlaoder */
		u32 addr = CFG_UBOOT_MEMADDR;
		u32 size = 0;
		part_hdr_t *part_hdr = part_hdr_buf;
		blkdev_t *bdev = blkdev_get(CFG_BOOT_DEV);
		part_t *part;
		//u64 src;
		da_info_t da_info;

		if (bldr_load_part_lk(bdev, &addr, &size) != 0)
			goto error;

		/* prepare to jump to second bootloader */
		g_boot_mode = DOWNLOAD_BOOT;

		memset(&da_info, 0x0, sizeof(da_info_t));
		da_info.addr = da_addr;
		da_info.arg1 = (u32)da_arg;
		da_info.arg2 = (u32)sizeof(DownloadArg);
		da_info.len = BA_FIELD_BYPASS_MAGIC;
		da_info.sig_len = BA_FIELD_BYPASS_MAGIC;

		platform_set_dl_boot_args(&da_info);
		platform_set_boot_args();

#if CFG_BOOT_ARGUMENT_BY_ATAG
		jump_arg = (u32)&(g_dram_buf->boottag);
#else
		jump_arg = (u32)&bootarg;
#endif

		bldr_jump(addr, jump_arg, sizeof(boot_arg_t));

error:
		bldr_jump(da_addr, (u32)da_arg, (u32)sizeof(DownloadArg));
	}
#else
	bldr_jump(da_addr, (u32)da_arg, (u32)sizeof(DownloadArg));
#endif
}

static void usbdl_get_tgt_config(void)
{
	u32 tgt_cfg = 0;
	u16 status = 0;

#ifdef MTK_SECURITY_SW_SUPPORT
	if (seclib_sbc_enabled())
		tgt_cfg |= TGT_CFG_SBC_EN;
	if (seclib_sla_enabled())
		tgt_cfg |= TGT_CFG_SLA_EN;
	if (seclib_daa_enabled())
		tgt_cfg |= TGT_CFG_DAA_EN;
#endif

	usbdl_put_dword(tgt_cfg);
	usbdl_put_word(status);
}

static bool usbdl_check_start_command(struct bldr_comport *comport, u32 tmo)
{
	u8 startcmd[] = {0xa0, 0x0a, 0x50, 0x05};
	ulong start = get_timer(0);
	u32 i = 0;
	u8 cmd = 0, rsp;
	struct comport_ops *comm = comport->ops;

	do {
		if (get_timer(start) > tmo)
			return FALSE;

		/* timeout 1 ms */
		if (0 != comm->recv(&cmd, 1, 1)) {
			continue;
		}

		/* fault-tolerant additional start cmd 0xA0 */
		if ((cmd == startcmd[0]) && (i > 0)) {
			i = 1;
			continue;
		}

		if (cmd == startcmd[i]) {
			rsp = ~cmd;
			i++;
		} else {
			rsp = cmd + 1;
			i = 0;
		}
		DBGMSG("%s TGT<-(0x%x)--TOOL\n", MOD, cmd);
		DBGMSG("%s TGT--(0x%x)->TOOL\n", MOD, rsp);
		comm->send(&rsp, 1);
	} while(i < sizeof(startcmd));

	return TRUE;
}

static void usbdl_pwr_init(void)
{
	u32 config = 0, timeout = 0;
	u16 status = 0;

	usbdl_get_dword(&config);
	usbdl_put_dword(config);
	usbdl_get_dword(&timeout);
	usbdl_put_dword(timeout);

	/* dummy function here since pmic wrapper is already initialized */

	usbdl_put_word(status);

	return;
}

static void usbdl_pwr_deinit(void)
{
	u16 status = 0;

	/* dummy function */
	usbdl_put_word(status);

	return;
}

static void usbdl_pwr_read16(void)
{
	u16 status = 0;
	u16 addr = 0, data = 0;
	u32 data32 = 0;

	/* get pmic read address (2 bytes) */
	usbdl_get_word(&addr);
	usbdl_put_word(addr);

	/* check whether address is valid, dummy in this implementation */

	/* respond the result of parameter check */
	usbdl_put_word(status);

	status = (u16)pmic_read_interface((u32)addr, &data32, 0xffff, 0);

	/* respond pmic operation status */
	usbdl_put_word(status);

	if (0 != status)
		return;

	/* respond data read from pmic */
	data = (u16)data32;
	usbdl_put_word(data);

	return;
}

static void usbdl_pwr_write16(void)
{
	u16 status = 0;
	u16 addr = 0, data = 0;
	u32 data32 = 0;

	/* get pmic write address (2 bytes) */
	usbdl_get_word(&addr);
	usbdl_put_word(addr);

	/* get pmic write data (2 bytes) */
	usbdl_get_word(&data);
	usbdl_put_word(data);
	data32 = (u32)data;

	/* check whether address and data are valid, dummy in this implementation */

	/* respond the result of parameter check */
	usbdl_put_word(status);

	status = (u16)pmic_config_interface((u32)addr, data32, 0xffff, 0);

	/* respond pmic operation status */
	usbdl_put_word(status);

	return;
}

static void usbdl_get_me_id(void)
{
	unsigned int ret = 0;
	u16 status = 0;
	u8  me_id[ME_IDENTITY_LEN] = {0};

#ifdef MTK_SECURITY_SW_SUPPORT
	ret = seclib_get_me_id(me_id, ME_IDENTITY_LEN);
	if (ret) {
		memset(me_id, 0x0, ME_IDENTITY_LEN);
	}
#endif

	usbdl_put_dword(ME_IDENTITY_LEN);
	usbdl_put_data(me_id, ME_IDENTITY_LEN);
	usbdl_put_word(status);

	return;
}

static void usbdl_get_soc_id(void)
{
	unsigned int ret = 0;
	u16 status = 0;
	u8  soc_id[SOC_ID_LEN] = {0};

#ifdef MTK_SECURITY_SW_SUPPORT
	ret = seclib_get_soc_id(soc_id, SOC_ID_LEN);
	if (ret) {
		memset(soc_id, 0x0, SOC_ID_LEN);
	}
#endif

	usbdl_put_dword(SOC_ID_LEN);
	usbdl_put_data(soc_id, SOC_ID_LEN);
	usbdl_put_word(status);

	return;
}

static void usbdl_get_pl_cap(void)
{
	u32 cap0 = 0;
	u32 cap1 = 0;

	cap0 |= PL_CAP0_XFLASH_SUPPORT | PL_CAP0_MEID_SUPPORT | PL_CAP0_SOCID_SUPPORT;
	/* cap1 is reserved */

	usbdl_put_dword(cap0);
	usbdl_put_dword(cap1);

	return;
}

/* This function is used to destroy key in efuse and force chip
 * to enter RMA mode. Zeroization implementation is removed to avoid
 * being called accidentally since it's not recoverable. If you need
 * this function, please contact Mediatek for more information.
 */
#define ZEROIZATION_MAGIC (0x08825252)
static void usbdl_zeroization(void)
{
	u16 status = 0xffff;
	u32 magic = 0;

	usbdl_get_dword(&magic);
	/* further protection so zeroization will not be
	 * performed on cmd bit flip
	 */
	if (magic == ZEROIZATION_MAGIC) {
		/* zeroization implementation removed, please
		 * contact Mediatek if you really need it.
		 */
		status = 0;
	}

	usbdl_put_word(status);
	return;
}

static void usbdl_stay_still(void)
{
    u16 status = 0;
    usbdl_put_word(status);
    mdelay(100);

    usb_disconnect();
    pal_log_info("%s USB Disconnect and Enter Dead Loop.\n", MOD);

    while(1) {
        platform_wdt_kick();
        if(!usb_accessory_in()) {
            pl_power_off();
        }
    }
}

int usbdl_handler(struct bldr_comport *comport, u32 hshk_tmo_ms)
{
	u8 cmd = 0;

	if (usbdl_check_start_command(comport, hshk_tmo_ms) == FALSE) {
		pal_log_debug("%s start cmd handshake timeout (%dms)\n", MOD, hshk_tmo_ms);
		return -1;
	}

	/* if log is disabled, re-init log port and enable it */
	if (comport->type == COM_USB && log_status() == 0) {
		mtk_uart_init(UART_SRC_CLK_FRQ, CFG_LOG_BAUDRATE);
		log_ctrl(1);
	}

	dlcomport = comport;

#if CFG_PRELOADER_AS_DA
	usbdl_init_image();
#endif

	while (1) {
		platform_wdt_kick();

		usbdl_get_byte(&cmd);
		if (cmd != CMD_GET_BL_VER)
			usbdl_put_byte(cmd);    /* echo cmd */

		switch (cmd) {
			case CMD_GET_BL_VER:
				/* echo preloader version
				 * 1: not support xflash tool
				 * 2: support xflash tool
				 */
				DBGMSG("%s CMD_GET_BL_VER\n", MOD);
				usbdl_put_byte(3);
				break;
			case CMD_GET_PL_CAP:
				DBGMSG("%s CMD_GET_PL_CAP\n", MOD);
				usbdl_get_pl_cap();
				break;
			case CMD_GET_HW_SW_VER:
				DBGMSG("%s CMD_GET_HW_SW_VER\n", MOD);
				usbdl_get_hw_sw_ver();
				break;
			case CMD_GET_HW_CODE:
				DBGMSG("%s CMD_GET_HW_CODE\n", MOD);
				usbdl_get_hw_code();
				break;
			case CMD_SEND_DA:
				DBGMSG("%s CMD_SEND_DA\n", MOD);
				usbdl_send_da();
				break;
			case CMD_JUMP_DA:
				DBGMSG("%s CMD_JUMP_DA\n", MOD);
				usbdl_jump_da();
				break;
#if CFG_PRELOADER_AS_DA
			case CMD_SEND_IMAGE:
				DBGMSG("%s CMD_SEND_IMAGE\n", MOD);
				usbdl_send_image();
				break;
			case CMD_BOOT_IMAGE:
				DBGMSG("%s CMD_BOOT_IMAGE\n", MOD);
				usbdl_boot_image();
				break;
#endif
			case CMD_STAY_STILL:
				DBGMSG("%s CMD_STAY_STILL\n", MOD);
				usbdl_stay_still();
				break;
			case CMD_GET_TARGET_CONFIG:
				DBGMSG("%s CMD_GET_TARGET_CONFIG\n", MOD);
				usbdl_get_tgt_config();
				break;
			case CMD_UART1_LOG_EN:
				DBGMSG("%s CMD_UART1_LOG_EN\n", MOD);
				usbdl_put_word(0);   //status = S_DONE
				break;
			case CMD_LEGACY_READ:
				DBGMSG("%s CMD_LEGACY_READ\n", MOD);
				usbdl_read16(TRUE);
				break;
			case CMD_READ16:
				DBGMSG("%s CMD_READ16\n", MOD);
				usbdl_read16(FALSE);
				break;
			case CMD_LEGACY_WRITE:
				DBGMSG("%s CMD_LEGACY_WRITE\n", MOD);
				usbdl_write16(TRUE);
				break;
			case CMD_WRITE16:
				DBGMSG("%s CMD_WRITE16\n", MOD);
				usbdl_write16(TRUE);
				break;
			case CMD_WRITE16_NO_ECHO:
				DBGMSG("%s CMD_WRITE16_NO_ECHO\n", MOD);
				usbdl_write16(FALSE);
				break;
			case CMD_READ32:
				DBGMSG("%s CMD_READ32\n", MOD);
				usbdl_read32(FALSE);
				break;
			case CMD_WRITE32:
				DBGMSG("%s CMD_WRITE32\n", MOD);
				usbdl_write32(TRUE);
				break;
			case CMD_PWR_INIT:
				DBGMSG("%s CMD_PWR_INIT\n", MOD);
				usbdl_pwr_init();
				break;
			case CMD_PWR_DEINIT:
				DBGMSG("%s CMD_PWR_DEINIT\n", MOD);
				usbdl_pwr_deinit();
				break;
			case CMD_PWR_READ16:
				DBGMSG("%s CMD_PWR_READ16\n", MOD);
				usbdl_pwr_read16();
				break;
			case CMD_PWR_WRITE16:
				DBGMSG("%s CMD_PWR_WRITE16\n", MOD);
				usbdl_pwr_write16();
				break;
			case CMD_GET_ME_ID:
				DBGMSG("%s CMD_GET_ME_ID\n", MOD);
				usbdl_get_me_id();
				break;
			case CMD_GET_SOC_ID:
				DBGMSG("%s CMD_GET_SOC_ID\n", MOD);
				usbdl_get_soc_id();
				break;
			case CMD_ZEROIZATION:
				DBGMSG("%s CMD_ZEROIZATION\n", MOD);
				usbdl_zeroization();
				break;
			default:
				DBGMSG("%s Unhandled CMD 0x%x\n", MOD, cmd);
				break;
		}
		DBGMSG("\n", MOD);
	}
	return 0;
}

#endif /* CFG_USB_DOWNLOAD */
