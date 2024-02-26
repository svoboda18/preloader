#ifndef __EFUSE_HRID_H__
#define __EFUSE_HRID_H__

#define E_SUCCESS               0x00000000
#define E_BUF_NOT_ENOUGH        0x10000000
#define E_BUF_ZERO_OR_NULL        0x20000000
#define E_BUF_SIZE_ZERO_OR_NULL   0x40000000

extern u32 get_hrid_size(void);
extern u32 get_hrid(unsigned char *rid, unsigned char *rid_sz);

#endif /* __EFUSE_HRID_H__ */
