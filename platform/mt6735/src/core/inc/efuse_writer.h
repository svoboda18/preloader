#ifndef EFUSE_WRITER_H
#define EFUSE_WRITER_H

#define S_EFUSE_DONE                0
#define S_EFUSE_MARK_SKIP           0x10000000
#define S_EFUSE_MARK_REBLOW         0x20000000
#define S_EFUSE_VBAT_ERR            0x40000000
#define S_EFUSE_DATA_NOT_VALID      0x80000000
#define S_EFUSE_WRITE_TIMEOUT       0x01000000
#define S_EFUSE_RELOAD_TIMEOUT      0x02000000
#define S_EFUSE_ALREADY_BROKEN      0x04000000
#define S_EFUSE_VALUE_IS_NOT_ZERO   0x08000000
#define S_EFUSE_BLOW_ERROR          0x00100000
#define S_EFUSE_BLOW_PARTIAL        0x00200000
#define S_EFUSE_CONFIG_ERR          0x00400000
#define S_EFUSE_PART_ERR            0x00800000
#define S_EFUSE_DISABLE             0x00010000
#define S_EFUSE_PMIC_ERR            0x00020000
#define S_EFUSE_DRAM_RESV_ERR       0x00040000

/* entry funtion */
extern int efuse_write_all(struct efuse_param);

#endif /* EFUSE_WRITER_H */
