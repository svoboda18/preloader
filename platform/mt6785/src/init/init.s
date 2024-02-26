.section .text.start

.equ MODE_USR       ,0x10
.equ MODE_FIQ       ,0x11
.equ MODE_IRQ       ,0x12
.equ MODE_SVC       ,0x13
.equ MODE_MON       ,0x16
.equ MODE_ABT       ,0x17
.equ MODE_UNDEF     ,0x1B
.equ MODE_SYS       ,0x1F
.equ I_BIT          ,0x80
.equ F_BIT          ,0x40
.equ INT_BIT        ,0xC0

.extern sys_stack
.extern sys_stack_sz
.extern bl31_base_addr
.extern rst_vector_base_addr

/* bldr argument address */
.globl bldr_args_addr
bldr_args_addr:
    .word 0x0

.globl _start
_start:
    b resethandler
bss1_start:
    .word _bss1_start
bss1_end:
    .word _bss1_end
bss2_start:
    .word _bss2_start
bss2_end:
    .word _bss2_end
stack:
    .long sys_stack
stacksz:
    .long sys_stack_sz

resethandler:
    LDR r6, =bldr_args_addr
    STR r4, [r6]
    MOV r0, #0
    MOV r1, #0
    MOV r2, #0
    MOV r3, #0
    MOV r4, #0
    MOV r5, #0
    MOV r6, #0
    MOV r7, #0
    MOV r8, #0
    MOV r9, #0
    MOV r10, #0
    MOV r11, #0
    MOV r12, #0
    MOV sp, #0
    MOV lr, #0

    /* set the cpu to SVC32 mode */
    MRS	r0,cpsr
    BIC	r0,r0,#0x1f
    ORR	r0,r0,#0xd3
    MSR	cpsr,r0

    /* disable interrupt */
    MRS r0, cpsr
    MOV r1, #INT_BIT
    ORR r0, r0, r1
    MSR cpsr_cxsf, r0

    /* enable I cache and disable D cache */
    MRC p15, 0, ip, c1, c0, 0
    ORR ip, ip, #0x1000   /* I bit */
    BIC ip, ip, #0x4      /* C bit */
    MCR p15, 0, ip, c1, c0, 0

clear_bss1 :
    LDR r0, bss1_start  /* find start of bss segment */
    LDR r1, bss1_end    /* stop here */
    MOV r2, #0x00000000 /* clear */

    CMP r0, r1
    BEQ clear_bss2

    /*  clear loop... */
clbss1_l :
    STR r2, [r0]
    ADD r0, r0, #4
    CMP r0, r1
    BNE clbss1_l

clear_bss2 :
    LDR r0, bss2_start  /* find start of bss segment */
    LDR r1, bss2_end    /* stop here */
    MOV r2, #0x00000000 /* clear */

    CMP r0, r1
    BEQ setup_stk

    /*  clear loop... */
clbss2_l :
    STR r2, [r0]
    ADD r0, r0, #4
    CMP r0, r1
    BNE clbss2_l

setup_stk :
    /* setup stack */
    LDR r0, stack
    LDR r1, stacksz

    /* buffer overflow detect pattern */
    LDR r2, =0xDEADBEFF
    STR r2, [r0]

    LDR r1, [r1]
    SUB r1, r1, #0x04
    ADD r1, r0, r1

    MOV sp, r1

entry :
    LDR r0, =bldr_args_addr
    B   main

.globl jump
jump:
    MOV r4, r1   /* r4 argument */
    MOV r5, r2   /* r5 argument */
    MOV pc, r0    /* jump to addr */

.globl apmcu_icache_invalidate
apmcu_icache_invalidate:
    MOV r0, #0
    MCR p15, 0, r0, c7, c5, 0  /* CHECKME: c5 or c1 */
    BX  lr

.globl apmcu_isb
apmcu_isb:
    ISB
    BX  lr

.globl apmcu_disable_icache
apmcu_disable_icache:
    MOV r0,#0
    MCR p15,0,r0,c7,c5,6   /* Flush entire branch target cache */
    MRC p15,0,r0,c1,c0,0
    BIC r0,r0,#0x1800      /* I+Z bits */
    MCR p15,0,r0,c1,c0,0
    BX  lr

.globl apmcu_disable_smp
apmcu_disable_smp:
    MRC p15,0,r0,c1,c0,1
    BIC r0,r0,#0x040       /* SMP bit */
    MCR p15,0,r0,c1,c0,1
    BX  lr

.section .text.arch64
.globl jumparch64
jumparch64:
    MOV r5, r1   /* reserved variable */
    MOV r6, r0   /* bl31 bootarg */

    /* setup the reset vector base address after warm reset to Aarch64 */
    LDR r0, =bl31_base_addr
    LDR r0,[r0]

    LDR r1, =rst_vector_base_addr
    LDR r1,[r1]
    str r0,[r1]

    /* setup the excution state after warm reset: 1:Aarch64, 0:Aarch32 */
    MRC p15,0,r0,c12,c0,2
    orr r0, r0, #1
    MCR p15,0,r0,c12,c0,2
    DSB
    ISB

    /* do warm reset:reset request */
    MRC p15,0,r0,c12,c0,2
    orr r0, r0, #2
    MCR p15,0,r0,c12,c0,2
    DSB
    ISB

    /* set r0 as bl31 bootarg and r1 as reserved variable for ATF */
    MOV r0, r6
    MOV r1, r5

.globl WFI_LOOP
WFI_LOOP:
    /* enter WFI to request a warm reset */
    WFI
    B WFI_LOOP
