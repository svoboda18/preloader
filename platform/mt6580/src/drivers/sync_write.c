#include "mt6580.h"

static void reg_dummy_read(void)
{
    int a = *(unsigned int *)(APHW_CODE);
    a++;
}

void dsb(void)
{
  __asm__ __volatile__ ("dsb" : : : "memory");
}

void mt_reg_sync_writel(unsigned int v, unsigned int a)
{
    *(volatile unsigned int *)(a) = (v);
    dsb();
    reg_dummy_read();
}

void mt_reg_sync_writew(unsigned short v, unsigned int a)
{
    *(volatile unsigned short *)(a) = (v);
     dsb();
     reg_dummy_read();
}

void mt_reg_sync_writeb(unsigned char v, unsigned int a)
{
    *(volatile unsigned char *)(a) = (v);
    dsb();
    reg_dummy_read();
}

