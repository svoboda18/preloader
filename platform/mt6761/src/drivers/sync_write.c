#include "mt6761.h"

static void reg_dummy_read(void)
{
    int a = *(unsigned int *)(APHW_CODE);
    a++;
}

void dsb(void)
{
  __asm__ __volatile__ ("dsb" : : : "memory");
}

void mt_reg_sync_writel(int v, int a)
{
    *(volatile unsigned int *)(a) = (v);
    dsb();
    reg_dummy_read();
}

void mt_reg_sync_writew(int v, int a)
{
    *(volatile unsigned short *)(a) = (v);
     dsb();
     reg_dummy_read();
}
void mt_reg_sync_writeb(int v, int a)
{
    *(volatile unsigned char *)(a) = (v);
    dsb();
    reg_dummy_read();
}

