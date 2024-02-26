.equ C1_MBIT ,0x00000001
.equ C1_CBIT ,0x00000004

PGTBL:
    .long page_tbl
pgtbl_start:
    .long _pgtbl_start

.global setup_page_table
setup_page_table:
    ldr r0, =_pgtbl_start
    mov r0, r0, lsr #14
    mov r0, r0, lsl #14

    mrc p15, 0, r1, c3, c0, 0
    orr r1, r1, #3  /* domain00: MANAGER */
    mcr p15, 0, r1, c3, c0, 0   /* set domain access register */
    mcr p15, 0, r0, c2, c0, 0   /* load TTBR0 */
    mov r1, #0
    mcr p15, 0, r1, c8, c3, 0   /* invalidate unified TLB inner shareable*/
    isb
    dsb

    ldr r1, PGTBL
    str r0, [r1]
    BX lr

.global __enable_mmu
__enable_mmu:
    MOV r0,#0
    MCR p15,0,r0,c8,c6,0    @ invalidate TLB
    MRC p15,0,r0,c1,c0,0
    ORR r0,r0,#C1_MBIT
    MCR p15,0,r0,c1,c0,0
    ISB
    BX lr

.global __disable_mmu
__disable_mmu:
    MOV r0,#0
    MCR p15,0,r0,c8,c6,0    @ invalidate TLB
    MRC p15,0,r0,c1,c0,0
    BIC r0,r0,#C1_MBIT
    MCR p15,0,r0,c1,c0,0
    BX lr

.global tlbiall
tlbiall:
    @   Uses unified TLB, so we do not have to specify the I/D/Unified TLB.
    @      Their results are the same.
    MCR p15,0,r0,c8,c3,0
    @MCR p15,0,r0,c8,c7,0
    DSB
    ISB
    BX lr

.global clean_invalidate_dcache
clean_invalidate_dcache:
        push    {r4,r5,r7,r9,r10,r11}
        dmb                                     @ ensure ordering with previous memory accesses
        mrc     p15, 1, r0, c0, c0, 1           @ read clidr
        ands    r3, r0, #0x7000000              @ extract loc from clidr
        mov     r3, r3, lsr #23                 @ left align loc bit field
        beq     ci_finished                     @ if loc is 0, then no need to clean
        mov     r10, #0
ci_loop1:
        add     r2, r10, r10, lsr #1            @ work out 3x current cache level
        mov     r1, r0, lsr r2                  @ extract cache type bits from clidr
        and     r1, r1, #7                      @ mask of the bits for current cache only
        cmp     r1, #2                          @ see what cache we have at this level
        blt     ci_skip                            @ skip if no cache, or just i-cache
        mcr     p15, 2, r10, c0, c0, 0          @ select current cache level in cssr
        isb                                     @ isb to sych the new cssr&csidr
        mrc     p15, 1, r1, c0, c0, 0           @ read the new csidr
        and     r2, r1, #7                      @ extract the length of the cache lines
        add     r2, r2, #4                      @ add 4 (line length offset)
        ldr     r4, =0x3ff
        ands    r4, r4, r1, lsr #3              @ find maximum number on the way size
        clz     r5, r4                          @ find bit position of way size increment
        ldr     r7, =0x7fff
        ands    r7, r7, r1, lsr #13             @ extract max number of the index size
ci_loop2:
        mov     r9, r4                          @ create working copy of max way size
ci_loop3:
        orr     r11, r10, r9, lsl r5            @ factor way and cache number into r11
        orr     r11, r11, r7, lsl r2            @ factor index number into r11
                cmp     r10, #2
        mcrne   p15, 0, r11, c7, c10, 2         @ clean by set/way
        mcrne   p15, 0, r11, c7, c6, 2         @ invalidate by set/way
        mcreq   p15, 0, r11, c7, c14, 2         @ clean & invalidate by set/way
        #mcr     p15, 0, r11, c7, c14, 2         @ clean & invalidate by set/way
        #mcr     p15, 0, r11, c7, c10, 2
	#mcr	p15, 0, r11, c7, c6, 2
	subs    r9, r9, #1                      @ decrement the way
        bge     ci_loop3
        subs    r7, r7, #1                      @ decrement the index
        bge     ci_loop2
ci_skip:
        add     r10, r10, #2                    @ increment cache number
        cmp     r3, r10
        bgt     ci_loop1
ci_finished:
        mov     r10, #0                         @ swith back to cache level 0
        mcr     p15, 2, r10, c0, c0, 0          @ select current cache level in cssr
        dsb
        isb
        pop     {r4,r5,r7,r9,r10,r11}
        bx      lr

.global invalidate_icache
invalidate_icache:
    MOV r0,#0
    MCR p15,0,r0,c7,c1,0    @icache invalidate all to PoU inner shareable
    @MCR p15,0,r0,c7,c5,0    @icache invalidate all to PoU
    dsb
    isb
    BX lr

.global __enable_dcache
__enable_dcache:
    MRC p15,0,r0,c1,c0,0
    ORR r0,r0,#C1_CBIT
    MCR p15,0,r0,c1,c0,0
    dsb
    isb
    BX lr

.global __disable_dcache
__disable_dcache:
    MRC p15,0,r0,c1,c0,0
    BIC r0,r0,#C1_CBIT
    MCR p15,0,r0,c1,c0,0
    dsb
    isb
    BX lr
