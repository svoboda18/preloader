/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2018
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

#ifndef TZ_TAGS_H
#define TZ_TAGS_H

#define SEC_MEM_TAG_FMT_MAGIC (0x544147U) /* 'TAG' */
#define SEC_MEM_TAG_ENG_MAGIC (0x474154U) /* 'GAT' */

/* ----------------------- */
/* |  TAG HEADER (4B)    | */
/* |  (TAG_FMT_MAGIC)    | */
/* ----------------------- */
/* |  TAG HEADER (4B)    | */
/* |  TAG CONTENT (DEPS) | */
/* ----------------------- */
/* |  TAG HEADER (4B)    | */
/* |  TAG CONTENT (DEPS) | */
/* ----------------------- */
/* |  ...                | */
/* ----------------------- */
/* |  TAG HEADER (4B)    | */
/* |  (TAG_ENG_MAGIC)    | */
/* ----------------------- */

#define PACKED __attribute__((packed))

#define TZ_TAG_SEC_MEM_CFG              (0x535301)
struct tz_sec_mem_cfg {
    unsigned long long svp_mem_start;
    unsigned long long svp_mem_end;
    unsigned char secmem_obfuscation;
    unsigned char shared_secmem;
    unsigned char reserved[2];
} PACKED;

#define TZ_TAG_RESERVED_MEM_TPLAY_TABLE (0x535310)
#define TZ_TAG_RESERVED_MEM_TPLAY_BUF   (0x535311)
#define TZ_TAG_RESERVED_MEM_M4U_BUF     (0x535312)
#define TZ_TAG_RESERVED_MEM_CMDQ_BUF    (0x535313)
#define TZ_TAG_RESERVED_MEM_SPI_BUF     (0x535314)
#define TZ_TAG_RESERVED_MEM_I2C_BUF     (0x535315)
struct tz_sec_reserved_mem {
    unsigned int start;
    unsigned int size;
} PACKED;

#define TZ_TAG_FLASH_DEV_INFO           (0x535320)
struct tz_sec_flash_dev_cfg {
    unsigned int rpmb_msg_auth_key[8];
    unsigned int rpmb_size;
    unsigned int emmc_rel_wr_sec_c;
    unsigned int emmc_setkey_flag;
} PACKED;

#define TZ_TAG_SEC_DEVICE_CFG           (0x535330)
struct tz_sec_device_cfg {
    unsigned long long remap_offset;
} PACKED;

#define TZ_TAG_SEC_GZ_CFG               (0x535331)
struct tz_sec_gz_cfg {
    unsigned int sdsp_fw_pa;
    unsigned int sdsp_fw_size;
    unsigned int sdsp_shared_mem_pa;
    unsigned int sdsp_shared_mem_size;
    unsigned int gz_tee_shared_mem_pa;
    unsigned int gz_tee_shared_mem_size;
} PACKED;

struct tz_tag_header {
    unsigned int tag : 24;  /* 24 bits: tag magic */
    unsigned int size : 8;  /* 8 bits: size of content, max is 255 bytes */
} PACKED;

struct tz_tag {
    struct tz_tag_header hdr;
    union {
        struct tz_sec_mem_cfg sec_mem_cfg;
        struct tz_sec_reserved_mem res_mem_cfg;
        struct tz_sec_flash_dev_cfg flash_dev_cfg;
        struct tz_sec_device_cfg sec_hw_cfg;
        struct tz_sec_gz_cfg gz_cfg;
    } u;
};

#define TAG_STRUCT_MAX_SIZE (0x100)
#define TAG_HDR_SIZE sizeof(struct tz_tag_header)
#define TAG_CONTENT_SIZE(tz_tag) ((tz_tag)->hdr.size)
#define NEXT_TAG(tz_tag) (struct tz_tag*)((unsigned char*)tz_tag + \
    TAG_HDR_SIZE + TAG_CONTENT_SIZE(tz_tag))
#define IS_TAG_FMT(tz_tag) ((tz_tag)->hdr.tag == SEC_MEM_TAG_FMT_MAGIC)
#define IS_TAG_END(tz_tag) ((tz_tag)->hdr.tag == SEC_MEM_TAG_ENG_MAGIC)
#define IS_NULL_TAG(tz_tag) (tz_tag == NULL)

#define PTR_DIFF_BYTES(a,b) ((unsigned char*)(a) - (unsigned char*)(b))
#define OUT_OF_TAG_SEARCH_RANGE(tag_ptr, search_start) \
    ((PTR_DIFF_BYTES(tag_ptr, search_start) >= TAG_STRUCT_MAX_SIZE)?1:0)

static inline struct tz_tag* find_tag(unsigned long start_addr,
    unsigned int to_search) {
    struct tz_tag *tag = (struct tz_tag *) start_addr;

    if (IS_NULL_TAG(tag))
        return NULL;

    do {
        if (tag->hdr.tag == to_search)
            return tag;
        tag = (struct tz_tag*) NEXT_TAG(tag);

        if (OUT_OF_TAG_SEARCH_RANGE(tag, start_addr))
            return NULL;
    } while (!IS_TAG_END(tag));

    if (tag->hdr.tag == to_search)
        return tag;
    return NULL;
}

extern void tz_secmem_tags_prepare(unsigned long param_addr,
    unsigned int tz_sec_mem_start, unsigned int tz_sec_mem_end);

#endif /* TZ_TAGS_H */
