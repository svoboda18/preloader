#ifndef _SYNC_WRITE_H
#define _SYNC_WRITE_H

extern void mt_reg_sync_writel(unsigned int v, unsigned int a);
extern void mt_reg_sync_writew(unsigned short v, unsigned int a);
extern void mt_reg_sync_writeb(unsigned char v, unsigned int a);

#endif
