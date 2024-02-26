#include "mt6757.h"

static void reg_dummy_read()
{
    int a = *(unsigned int *)(APHW_CODE);
    a++;
}

void dsb()
{
  __asm__ __volatile__ ("dsb" : : : "memory");
}

void mt_reg_sync_writel(v, a)
{
    *(volatile unsigned int *)(a) = (v);
    dsb();
    reg_dummy_read();
}

void mt_reg_sync_writew(v, a)
{
    *(volatile unsigned short *)(a) = (v);
     dsb();
     reg_dummy_read();
}
void mt_reg_sync_writeb(v, a)
{
    *(volatile unsigned char *)(a) = (v);
    dsb();
    reg_dummy_read();
}

