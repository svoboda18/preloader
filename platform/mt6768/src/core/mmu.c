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
#include "mmu.h"

addr_t *page_tbl = 0xf000;
addr_t *sec_page_tbl;

addr_t __NOBITS_SECTION__(.coarse_page_tbl) coarse_page_tbl[NR_COARSE_PGTBL * COARSE_PGTBL_SIZE / SIZE_OF_PGD] __attribute__ ((aligned (COARSE_PGTBL_SIZE)));

static unsigned int coarse_page_tbl_bitmask[NR_COARSE_PGTBL];

static bool check_already_mapping(addr_t pgd)
{
	return (pgd & DESC_PAGE) ? true : false;
}

/*
 * alloc_pte: allocate a page table.
 * Return the allocate page table or NULL.
 */
static addr_t alloc_pte(void)
{
	int i;
	addr_t pte;

	for (i = 0; i < NR_COARSE_PGTBL; i++) {
		if (coarse_page_tbl_bitmask[i] == 0) {
			coarse_page_tbl_bitmask[i] = 1;
			break;
		}
	}

	if (i < NR_COARSE_PGTBL) {
		pte = (addr_t)(&(coarse_page_tbl[i * COARSE_PGTBL_SIZE / SIZE_OF_PGD]));
		memset((addr_t)pte, 0, COARSE_PGTBL_SIZE);
		return pte;
	}

	return 0;
}

/*
 * free_pte: free a page table.
 * @pte: address of the page table to free
 */
static void free_pte(addr_t pte)
{
	addr_t i, error;

	i = (pte - (addr_t)coarse_page_tbl) / (COARSE_PGTBL_SIZE / SIZE_OF_PGD);

	if (coarse_page_tbl_bitmask[i] == 1) {
		coarse_page_tbl_bitmask[i] = 0;
	} else {
		print("[ERROR] free_pte: double-free pte (pte: 0x%x)\n", pte);
	}
}

/*
 * remap_area_pte: remap page table entries for a specified area.
 * @start: start virtual address
 * @end: end virtual address
 * @phys: physical address to remap
 * @mem_type: memory type of the remapped region.
 * Return 0 for success; return negative values for failure.
 */
static int remap_area_pte(addr_t start, addr_t end, addr_t phys, int mem_type)
{
	addr_t pte;
	unsigned int cacheability, share;

	if (check_already_mapping(sec_page_tbl[start >> SECTION_INDEX])) {
		pte = sec_page_tbl[start >> SECTION_INDEX] & COARSE_PGTBL_SIZE_MASK;
		pte += SIZE_OF_PGD * ((start & (SECTION_SIZE - 1)) >> SMALL_PAGE_SIZE_SHIFT);
	} else {
		/* allocate a pgtbl */
		pte = alloc_pte();
		if (!pte)
			return -1;
		sec_page_tbl[start >> SECTION_INDEX] = PAGE_TABLE(pte, 0, SEC, DOMAIN00);
		pte += SIZE_OF_PGD * ((start & (SECTION_SIZE - 1)) >> SMALL_PAGE_SIZE_SHIFT);
	}

	cacheability = mt2c(mem_type);
	share = mt2s(mem_type);

	do {
		*(volatile addr_t *)pte = SMALL_PAGE_DESC(phys, EXECUTABLE, APX_NO_LIMIT, AP_USR_NO_LIMIT, cacheability, 0, share);
		phys += SMALL_PAGE_SIZE;
	} while (pte += SIZE_OF_PGD, start += SMALL_PAGE_SIZE, start < end);

	return 0;
}

/*
 * remap_mem_range: remap memory to a specified region.
 * @virt: virtual address to map
 * @phys: physical address to remap
 * @size: size of the memory region
 * @mem_type: memory type of the remapped region.
 * Return 0 for success; return negative values for failure.
 */
int remap_mem_range(addr_t virt, addr_t phys, addr_t size, int mem_type)
{
	unsigned int cacheability, share;
	addr_t addr, next, end;
	addr_t pte;
	int err = 0;

	virt = virt & ~(SMALL_PAGE_SIZE - 1);
	phys = phys & ~(SMALL_PAGE_SIZE - 1);
	size = ALIGN(size, SMALL_PAGE_SIZE);

	cacheability = mt2c(mem_type);
	share = mt2s(mem_type);

	if (!((phys | size | virt) & ((SECTION_SIZE) - 1))) {
		/* section */
		for (addr = virt; (addr - virt) < size; addr += SECTION_SIZE) {
			if (check_already_mapping(sec_page_tbl[addr >> SECTION_INDEX]))
				free_pte((unsigned int)(sec_page_tbl[addr >> SECTION_INDEX] & COARSE_PGTBL_SIZE_MASK));

            		pte = SECTION_DESC(phys + (addr - virt), EXECUTABLE, APX_NO_LIMIT, AP_USR_NO_LIMIT, cacheability, 0, share, 0, SEC, DOMAIN00);
			sec_page_tbl[addr >> SECTION_INDEX] = pte;
		}
	} else {
#if 0
		addr = virt;
		end = virt + size;
		do {
			next = ((addr + SECTION_SIZE) < end)? ((addr + SECTION_SIZE) & ~(SECTION_SIZE-1)): end;
			if (next < addr)
				next = end;
			err = remap_area_pte(addr, next, phys + (addr - virt), mem_type);
			if (err)
				break;
		} while (addr = next, addr < end);
#else
		/* small page, not support for code size */
		print("[ERROR] remap_mem_range: please align size to section size (0x%x)\n", SECTION_SIZE);
#endif
	}

	cache_clean_invalidate();
	tlbiall();
	__asm__ volatile ("mcr p15, 0, %0, c7, c5, 6\n" : : "r"(0) : "cc");
	dsb();
	isb();

	return err;
}

static void mem_init(void)
{
	int i;

	/* init page table */
	memset((void *)page_tbl, 0x0, RSV_1ST_PGTBL_SZ);
	sec_page_tbl = page_tbl;
}

/*
 * io_table_init: Create mapping for io_table[] and others.
 * Return 0.
 */
static int io_table_init(void)
{
	addr_t addr;
	unsigned int i;

	/* create mapping for IO regions */
	remap_mem_range(0x0, 0x0, PLAT_CACHABLE_START, MT_DEVICE);
	remap_mem_range(PLAT_CACHABLE_END, PLAT_CACHABLE_END, PLAT_IO_END - PLAT_CACHABLE_END, MT_DEVICE);

	/* create mapping for cacheable SRAM region */
	remap_mem_range(PLAT_CACHABLE_START, PLAT_CACHABLE_START, PLAT_CACHABLE_END - PLAT_CACHABLE_START, MT_MEMORY_SHARED);

	/* create mapping for EMI regions */
	remap_mem_range(PLAT_IO_END, PLAT_IO_END, PLAT_EMI_END - PLAT_IO_END, MT_DEVICE);
}

/*
 * paging_init: initialize page table settings.
 */
void paging_init(void)
{
	setup_page_table();
	mem_init();
	io_table_init();
	__enable_mmu();
	__enable_dcache();
}

void paging_uninit(void)
{
	/* set cacheable SRAM region to device memory before disabling MMU to avoid out-of-order transactions */
	remap_mem_range(PLAT_CACHABLE_START, PLAT_CACHABLE_START, PLAT_CACHABLE_END - PLAT_CACHABLE_START, MT_DEVICE);

	/* disable cache and MMU */
	__disable_dcache();
	clean_invalidate_dcache();
	tlbiall();
	__disable_mmu();
}
