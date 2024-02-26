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

#ifndef _MMU_H
#define _MMU_H

#define SIZE_OF_PGD   4
#define LARGE_PAGE_SIZE (64 * 1024)
#define COARSE_PGTBL_SIZE (1 * 1024)
#define COARSE_PGTBL_SIZE_MASK (0xFFFFFC00)
#define SECTION_INDEX 20
#define NS_INDEX 3

typedef unsigned int addr_t;

enum
{
    DESC_FAULT = 0,
    DESC_PAGE = 1,
    DESC_SECTION = 2,
};

enum
{
    APX_NO_LIMIT = 0,
    APX_NO_WRITE = 1
};

/* use AP[2:1] policy */
/* AP[1] */
#if 0
enum
{
    AP_USR = 0,
    AP_ALL = 1,
};

enum
{
    AF_NO_ACCESS = 0,
    AF_NO_LITMIT = 1,
};
#else
enum
{
    AP_USR_NO_ACCESS = 0,
    AP_USR_NO_LIMIT = 1,
    AP_ALL_NO_ACCESS = 2,
    AP_ALL_NO_LIMIT = 3,
};
#endif

enum
{
    DOMAIN00 = 0,
    DOMAIN01 = 1,
    DOMAIN02 = 2,
    DOMAIN03 = 3,
    DOMAIN04 = 4,
    DOMAIN05 = 5,
    DOMAIN06 = 6,
    DOMAIN07 = 7,
    DOMAIN08 = 8,
    DOMAIN09 = 9,
    DOMAIN10 = 10,
    DOMAIN11 = 11,
    DOMAIN12 = 12,
    DOMAIN13 = 13,
    DOMAIN14 = 14,
    DOMAIN15 = 15
};

enum
{
    DOMAIN_NO_ACCESS = 0,
    DOMAIN_CLIENT = 1,
    DOMAIN_RESERVED = 2,
    DOMAIN_MANAGER = 3
};

enum
{
    DESC_LARGE_PAGE = 1,
    DESC_SMALL_PAGE = 2
};

enum
{
	NON_SHAREABLE = 0,
	SHAREABLE = 1,
};

enum
{
    NC_ORDERED = 0,
    SHAREABLE_DEVICE = 1,
    INNER_WT_NO_WALLOC_OUTER_WT_NO_WALLOC_NORMAL = 2,
    INNER_WB_NO_WALLOC_OUTER_WB_NO_WALLOC_NORMAL = 3,
    INNER_NC_OUTER_NC_NORMAL = 4,
    INNER_WB_WALLOC_OUTER_WB_WALLOC_NORMAL = 7,
    NONSHAREABLE_DEVICE = 8,

    //INNER_NC_OUTER_NC_NORMAL = 16+0+0,    /* As the same as 4 */
    INNER_WB_WALLOC_OUTER_NC_NORMAL = 16+0+1,
    INNER_WT_NO_WALLOC_OUTER_NC_NORMAL = 16+0+2,
    INNER_WB_NO_WALLOC_OUTER_NC_NORMAL = 16+0+3,

    INNER_NC_OUTER_WB_WALLOC_NORMAL = 16+4+0,
    //INNER_WB_WALLOC_OUTER_WB_WALLOC_NORMAL = 16+4+1,    /* As the same as 7 */
    INNER_WT_NO_WALLOC_OUTER_WB_WALLOC_NORMAL = 16+4+2,
    INNER_WB_NO_WALLOC_OUTER_WB_WALLOC_NORMAL = 16+4+3,

    INNER_NC_OUTER_WT_NO_WALLOC_NORMAL = 16+8+0,
    INNER_WB_WALLOC_OUTER_WT_NO_WALLOC_NORMAL = 16+8+1,
    //INNER_WT_NO_WALLOC_OUTER_WT_NO_WALLOC_NORMAL = 16+8+2,    /* as the same as 2 */
    INNER_WB_NO_WALLOC_OUTER_WT_NO_WALLOC_NORMAL = 16+8+3,

    INNER_NC_OUTER_WB_NO_WALLOC_NORMAL = 16+12+0,
    INNER_WB_WALLOC_OUTER_WB_NO_WALLOC_NORMAL = 16+12+1,
    INNER_WT_NO_WALLOC_OUTER_WB_NO_WALLOC_NORMAL = 16+12+2,
    //INNER_WB_NO_WALLOC_OUTER_WB_NO_WALLOC_NORMAL = 16+12+3,    /* As the same as 3 */

};

#define EXECUTABLE	(0ULL)
#define NOT_EXECUTABLE	(1ULL)

#define SEC	(0ULL)
#define NONSEC	(1ULL)

#define SECTION_SIZE (1 << SECTION_INDEX)
#define SMALL_PAGE_SIZE (4 * 1024)
#define SMALL_PAGE_SIZE_SHIFT 12

/* memory type */
enum
{
    MT_DEVICE,  /* shared device */
    MT_DEVICE_NONSHARED,    /* non-shared device */
    MT_MEMORY,  /* cacheable normal (non-sharable) */
    MT_UNCACHED,    /* strongly ordered */
    MT_MEMORY_NONCACHED,    /* non-cacheable normal (non_sharable) */
    MT_MEMORY_SHARED,   /* cacheable normal (sharable) */
    MT_MEMORY_NONCACHED_SHARED,   /* non-cacheable normal (sharable) */
    MT_CUSTOMIZE = 0x80000000,
    MT_CUSTOMIZE_SHARED = 0xC0000000,
};

#define mk_cust_mem_type(_c) (MT_CUSTOMIZE | (_c))
#define mk_cust_mem_type_shared(_c) (MT_CUSTOMIZE_SHARED | (_c))
#define is_cust_mem_type(_t) (((_t) & MT_CUSTOMIZE) == MT_CUSTOMIZE)
#define is_cust_mem_type_shared(_t) (((_t) & MT_CUSTOMIZE_SHARED) == MT_CUSTOMIZE_SHARED)
#define c_attr_from_cust_mem_type(_t) ((_t) & ~MT_CUSTOMIZE & ~MT_CUSTOMIZE_SHARED)

static inline unsigned int mt2s(int mem_type)
{
    if (mem_type == MT_MEMORY_SHARED || mem_type == MT_MEMORY_NONCACHED_SHARED) {
		return SHAREABLE;
    } else if (is_cust_mem_type_shared(mem_type)) {
        return SHAREABLE;
    } else {
        return NON_SHAREABLE;
    }
}

static inline unsigned int mt2c(int mem_type)
{
    unsigned int c;

    if (is_cust_mem_type(mem_type)) {
        c = c_attr_from_cust_mem_type(mem_type);
    } else if (mem_type == MT_DEVICE) {
        c = SHAREABLE_DEVICE;
    } else if (mem_type == MT_DEVICE_NONSHARED) {
        c = NONSHAREABLE_DEVICE;
    } else if (mem_type == MT_MEMORY || mem_type == MT_MEMORY_SHARED) {
//            c = INNER_WB_NO_WALLOC_OUTER_WB_NO_WALLOC_NORMAL;
          c = INNER_WB_WALLOC_OUTER_WB_WALLOC_NORMAL;
    } else if (mem_type == MT_UNCACHED) {
        c = NC_ORDERED;
    } else if (mem_type == MT_MEMORY_NONCACHED || mem_type == MT_MEMORY_NONCACHED_SHARED) {
        c = INNER_NC_OUTER_NC_NORMAL;
    } else {
        c = NC_ORDERED;
    }

    return c;
}


#ifdef __LPAE__
#define LARGE_TABLE(Addr, NS, AP, XN, PXN)     \
				(((Addr)&0xFFFFF000) | ((NS)<<63) | ((AP)<<61) | ((XN)<<60) | ((PXN)<<59) | DESC_TABLE )

#define LARGE_BLOCK_DESC(Addr, XN, PXN, CONS, nG, AF, SH, AP, NS, AttrIndx)     \
				(((Addr)&0xC0000000) | ((XN)<<54) | ((PXN)<<53) | ((CONS)<<52)\
				| ((nG)<<11) | ((AF)<<10) | ((SH)<<8) | ((AP)<<6) | ((NS)<<5) | ((AttrIndx)<<2)\
				| DESC_BLOCK)

#define SECTION_TABLE(Addr, NS, AP, XN, PXN)     \
				(((Addr)&0xFFFFF000) | ((NS)<<63) | ((AP)<<61) | ((XN)<<60) | ((PXN)<<59) | DESC_TABLE)

#define SECTION_BLOCK_DESC(Addr, XN, PXN, CONS, nG, AF, SH, AP, NS, AttrIndx)     \
				(((Addr)&0xFFE00000) | ((XN)<<54) | ((PXN)<<53) | ((CONS)<<52)\
				| ((nG)<<11) | ((AF)<<10) | ((SH)<<8) | ((AP)<<6) | ((NS)<<5) | ((AttrIndx)<<2)\
				| DESC_BLOCK)

#define SMALL_PAGE_DESC(Addr, XN, PXN, CONS, nG, AF, SH, AP, NS, AttrIndx)     \
                (((Addr)&0xFFFFF000) | ((XN)<<54) | ((PXN)<<53) | ((CONS)<<52)\
				| ((nG)<<11) | ((AF)<<10) | ((SH)<<8) | ((AP)<<6) | ((NS)<<5) | ((AttrIndx)<<2)\
				| DESC_TABLE)
#else

#define SECTION_DESC(Addr, XN, APX, AP, TEXCB, nG, S,  P, NS, Domain)     \
                (((Addr)&0xFFF00000) | ((NS)<<19) | ((nG)<<17) | ((S)<<16)  \
                | ((APX)<<15) | (((TEXCB&0x1C)>>2)<<12) | ((AP)<<10) | ((P)<<9)         \
                | ((Domain)<<5) | ((XN)<<4) | ((TEXCB&0x3)<<2) | DESC_SECTION)

#define SUPERSEC_DESC(Addr, XN, APX, AP, TEXCB, nG, S,  P, NS, Domain)    \
                (((Addr)&0xFF000000) | ((NS)<<19) | (1<<18) | ((nG)<<17)    \
                | ((S)<<16) | ((APX)<<15) | (((TEXCB&0x1C)>>2)<<12) | ((AP)<<10)        \
                | ((P)<<9) | ((Domain)<<5) | ((XN)<<4) | ((TEXCB&0x3)<<2) | DESC_SECTION)

#define PAGE_TABLE(Addr, P, NS, Domain)                              \
                (((Addr)&COARSE_PGTBL_SIZE_MASK) | ((P)<<9) | ((Domain)<<5) | ((NS)<<3) | DESC_PAGE)

#define LARGE_PAGE_DESC(Addr, XN, APX, AP, TEXCB, nG, S)                   \
                (((Addr)&0xFFFF0000) | ((XN)<<15) | (((TEXCB&0x1C)>>2)<<12) | ((nG)<<11)\
                | ((S)<<10) | ((APX)<<9) | ((AP)<<4) |((TEXCB&0x3)<<2) | DESC_LARGE_PAGE)

#define SMALL_PAGE_DESC(Addr, XN, APX, AP, TEXCB, nG, S)               \
                (((Addr)&0xFFFFF000) | ((nG)<<11) | ((S)<<10) | ((APX)<<9)  \
                | (((TEXCB&0x1C)>>2)<<6) | ((AP)<<4) | ((TEXCB&0x3)<<2) |((XN)<<1) | DESC_SMALL_PAGE)
#endif

#define __ALIGN(x, a) __ALIGN_MASK((x), (typeof(x))(a) - 1)
#define __ALIGN_MASK(x, mask) (((x) + (mask)) & ~(mask))
#define ALIGN(x, a) __ALIGN((x), (a))

#define isb() __asm__ __volatile__ ("isb" : : : "memory")

#define cache_clean_invalidate()    \
        do {    \
	           clean_invalidate_dcache();\
	           dsb();  \
	           invalidate_icache();    \
	           dsb();  \
        } while (0)

#define NR_COARSE_PGTBL		4
#define RSV_1ST_PGTBL_SZ	0x4000
#define PLAT_IO_END		0x40000000
#define PLAT_EMI_END		0xFFFFFFFF

/* platform-dependent settings */
#define PLAT_CACHABLE_START	0x100000
#define PLAT_CACHABLE_END	0x300000

extern void paging_init(void);
extern void paging_uninit(void);

#endif  /* _MMU_H */

