#include "typedefs.h"
#include "print.h"
#include "blkdev.h"
#include "dram_buffer.h"
#include "partition_api.h"
#include "pal_log.h"
#include "storage_api.h"

typedef struct {
    u8 b[16];
} __attribute__((packed)) efi_guid_t;


typedef struct {
    u64 signature;
    u32 revision;
    u32 header_size;
    u32 header_crc32;
    u32 reserved;
    u64 my_lba;
    u64 alternate_lba;
    u64 first_usable_lba;
    u64 last_usable_lba;
    efi_guid_t disk_guid;
    u64 partition_entry_lba;
    u32 num_partition_entries;
    u32 sizeof_partition_entry;
    u32 partition_entry_array_crc32;
} __attribute__((packed)) gpt_header;

#define GPT_ENTRY_NAME_LEN  (72 / sizeof(u16))

typedef struct {
    efi_guid_t partition_type_guid;
    efi_guid_t unique_partition_guid;
    u64 starting_lba;
    u64 ending_lba;
    u64 attributes;
    u16 partition_name[GPT_ENTRY_NAME_LEN];
} __attribute__((packed))gpt_entry;

static part_t *part_ptr = NULL;

/*
 ********** Definition of Debug Macro **********
 */
#define TAG "[GPT_PL]"

/*
 ********** Definition of GPT buffer **********
 */
#define GPT_HEADER_BUF  g_dram_buf->gpt_header_buf
#define GPT_ENTRIES_BUF g_dram_buf->gpt_entries_buf

/*
 ********** Definition of CRC32 Calculation **********
 */
#define gpt_dram_crc32_table g_dram_buf->crc32_table

#define GPT_HEADER_SIGNATURE    0x5452415020494645ULL

#ifdef EARLY_PARTITION_ACCESS
#define GPT_SRAM_BUF_SIZE   4096
unsigned char __NOBITS_SECTION__(.gpt_sram_buf) gpt_sram_buf[GPT_SRAM_BUF_SIZE];
u32 __NOBITS_SECTION__(.gpt_sram_crc32_table) gpt_sram_crc32_table[256];
part_t __NOBITS_SECTION__(.gpt_sram_part_info) gpt_sram_part_info;
struct part_meta_info __NOBITS_SECTION__(.gpt_sram_part_meta_info) gpt_sram_part_meta_info;
#endif

static void init_crc32_table(u32* crc32_table)
{
    int i, j;
    u32 crc;
    for (i = 0; i < 256; i++) {
        crc = i;
        for (j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320;
            } else {
                crc >>= 1;
            }
        }
        crc32_table[i] = crc;
    }
}
static u32 crc32(u32 crc, u8 *p, u32 len, u32* crc32_table)
{
    while (len--) {
        crc ^= *p++;
        crc = (crc >> 8) ^ crc32_table[crc & 255];
    }

    return crc;
}
static u32 efi_crc32(u32 crc, u8 *p, u32 len, u32* crc32_table)
{
	return crc32(crc, p, len, crc32_table);
}
static u32 efi_crc32_finalize(u32 crc)
{
	return crc ^ ~0L;
}

static void get_part_name(u8 *dst, u16 *src, int len)
{
    int i = 0;
    if ((src[0] & 0xFF00) == 0) { // word to ascii string
        while (i < len - 1) {
            if (!src[i])
                break;
            dst[i] = src[i] & 0xFF;
            i++;
        }
        dst[i] = 0;
    } else {
        memcpy(dst, src, len - 1);
        dst[len-1] = 0;
    }
}

static void get_part_info(u32 part_id, part_t *part_ptr, gpt_entry *entry)
{
    get_part_name(part_ptr->info->name, entry->partition_name, min(PART_META_INFO_NAMELEN, GPT_ENTRY_NAME_LEN));
    part_ptr->start_sect = (unsigned long)entry->starting_lba;
    part_ptr->nr_sects = (unsigned long)(entry->ending_lba - entry->starting_lba + 1);
    part_ptr->part_id = part_id;
    part_ptr->part_attr = (unsigned long)entry->attributes;
    pal_log_debug("%sname=%s, start_sect=0x%x, nr_sects=0x%x\n", TAG, part_ptr->info->name,
        (unsigned long)entry->starting_lba, (unsigned long)(entry->ending_lba - entry->starting_lba + 1));

}

extern u64 g_emmc_user_size;

static u64 last_lba(u32 part_id)
{
#if !defined(BOOTDEV_SDMMC_UFS_COMBO)
    /* Only support USER region now */
    #if (CFG_BOOT_DEV == BOOTDEV_SDMMC)
        return g_emmc_user_size / 512 - 1;
    #elif (CFG_BOOT_DEV == BOOTDEV_UFS)
    {
        blkdev_t *dev;
        u64 bytes;

        dev = blkdev_get(CFG_BOOT_DEV);
        if (dev == NULL)
            return 0;

        bytes = dev->get_part_size(dev, part_id);
        if (bytes)
            return bytes / dev->blksz - 1;
        else
            return 0;
    }
    #endif
#else
#if (CFG_BOOT_DEV == BOOTDEV_SDMMC) || (CFG_BOOT_DEV == BOOTDEV_UFS)
    blkdev_t *dev;
    u64 bytes;

    dev = blkdev_get(CFG_BOOT_DEV);
    if (dev == NULL)
        return 0;

    if (dev->type == BOOTDEV_SDMMC) {
        return g_emmc_user_size / 512 - 1;
    } else if (dev->type == BOOTDEV_UFS) {
        bytes = dev->get_part_size(dev, part_id);
        if (bytes)
            return bytes / dev->blksz - 1;
        else
            return 0;
    }
#endif
#endif
    return 0;
}

static int read_data(u8 *buf, u32 part_id, u64 lba, u64 size)
{
    int err;
    blkdev_t *dev;

    dev = blkdev_get(CFG_BOOT_DEV);
    if (!dev) {
        pal_log_err("%sread data, err(no dev)\n", TAG);
        return 1;
    }

    err = blkdev_read(dev, lba * dev->blksz, (u32)size, buf, part_id);
    if (err) {
        pal_log_err("%sread data, err(%d)\n", TAG, err);
        return err;
    }

    return 0;
}

static int get_gpt_header(u32 part_id, u64 header_lba, u8 *header_buf, u32 len, u32 *crc32_table)
{
    int err;
    u32 calc_crc, orig_header_crc;
    gpt_header *header = (gpt_header *)header_buf;
    err = read_data(header_buf, part_id, header_lba, len);
    if (err) {
        pal_log_err("%sread header(part_id=%d,lba=%llx), err(%d)\n",
                TAG, part_id, header_lba, err);
        return err;
    }

    if (header->signature != GPT_HEADER_SIGNATURE) {
        pal_log_err("%scheck header, err(signature 0x%llx!=0x%llx)\n",
                TAG, header->signature, GPT_HEADER_SIGNATURE);
        return 1;
    }

    orig_header_crc = header->header_crc32;
    header->header_crc32 = 0;
    calc_crc = efi_crc32(~0L, (u8 *)header, header->header_size, crc32_table);
    calc_crc = efi_crc32_finalize(calc_crc);
    if (orig_header_crc != calc_crc) {
        pal_log_err("%scheck header, err(crc 0x%x!=0x%x(calc))\n",
                TAG, orig_header_crc, calc_crc);
        return 1;
    }

    header->header_crc32 = orig_header_crc;

    if (header->my_lba != header_lba) {
        pal_log_err("%scheck header, err(my_lba 0x%llx!=0x%llx)\n",
                TAG, header->my_lba, header_lba);
        return 1;
    }
    return 0;
}


#ifdef EARLY_PARTITION_ACCESS
static u64 get_part_ptr_by_gpt(u32 part_id, u64 header_lba, u8 *entries_buf, part_t *part_ptr, const char *part_name, u32 blksz) {
    int err, i, found;
    int entries_cnt, cur_entry_idx;
    u32 calc_crc, orig_header_crc;
    u64 entries_read_size, num_block_entry, read_lba;
    gpt_header gpt_header_buf;
    gpt_header *header = &gpt_header_buf;
    gpt_entry *entries = (gpt_entry *)entries_buf;

    init_crc32_table(gpt_sram_crc32_table);
    part_ptr->start_sect = 0;
    if (get_gpt_header(part_id, header_lba, (u8*) &gpt_header_buf, sizeof(gpt_header_buf), gpt_sram_crc32_table)) {
        pal_log_err("%s get_gpt_header fail\n", TAG);
        return 1;
    }

    num_block_entry = (u64)(blksz / header->sizeof_partition_entry);
    entries_read_size = (u64)((header->num_partition_entries + (num_block_entry - 1)) / num_block_entry) * blksz;

    calc_crc = ~0L;
    read_lba = header->partition_entry_lba;
    found = 0;
    while (entries_read_size > 0) {
        err = read_data((u8*)entries_buf, part_id, read_lba, GPT_SRAM_BUF_SIZE);
        read_lba = read_lba + GPT_SRAM_BUF_SIZE/blksz;
        calc_crc = efi_crc32(calc_crc, (u8 *)entries_buf, (u32)GPT_SRAM_BUF_SIZE, gpt_sram_crc32_table);
        entries_read_size -= GPT_SRAM_BUF_SIZE;
        entries_cnt = GPT_SRAM_BUF_SIZE/header->sizeof_partition_entry;
        for (i = 0; i < entries_cnt && found == 0; i++) {
            if(!entries[i].starting_lba)
                break;
            get_part_info(part_id, part_ptr, &entries[i]);
            if (!strcmp(part_ptr->info->name, part_name)) {
                found = 1;
                break;
            }
        }
    }
    calc_crc = efi_crc32_finalize(calc_crc);
    if (header->partition_entry_array_crc32 != calc_crc) {
        pal_log_err("%scheck header, err(entries crc 0x%x!=0x%x(calc))\n",
            TAG, header->partition_entry_array_crc32, calc_crc);
        memset(part_ptr, 0, sizeof(part_t));
        return 1;
    }
    if (found == 0)
        memset(part_ptr, 0, sizeof(part_t));

    return 0;
}

part_t *mt_get_part_sram(const char *name)
{
    u32 part_id;

    part_id = storage_get_part_id(STORAGE_PHYS_PART_USER);

    if (name == NULL) {
        pal_log_err("%s[%s]invalid argument\n", TAG, __func__);
        return NULL;
    }
    blkdev_t *dev = blkdev_get(CFG_BOOT_DEV);
    if (!dev) {
        pal_log_err("%s%s fail, err(no dev)\n", TAG, __func__);
        return NULL;
    }
    gpt_sram_part_info.info = &gpt_sram_part_meta_info;
    if (get_part_ptr_by_gpt(part_id, 1, gpt_sram_buf, &gpt_sram_part_info, name, dev->blksz) != 0) {
        if (get_part_ptr_by_gpt(part_id, last_lba(part_id), gpt_sram_buf, &gpt_sram_part_info, name, dev->blksz) != 0) {
            pal_log_err("%sFailure to find valid GPT.\n", TAG);
            return NULL;
        }
    }
    if (gpt_sram_part_info.start_sect == 0 && gpt_sram_part_info.nr_sects == 0)
        return NULL;
    else
        return &gpt_sram_part_info;
}

int mt_get_part_info_by_name(const char *name, struct part_info_t *part_info)
{
    part_t *part;
    blkdev_t *dev = blkdev_get(CFG_BOOT_DEV);

    if (!dev) {
        pal_log_err("%s%s fail, err(no dev)\n", TAG, __func__);
        return 1;
    }

    part = mt_get_part_sram(name);
    if (part != NULL && part_info != NULL) {
        part_info->addr = (u64)part->start_sect * (u64)dev->blksz;
        part_info->active = (part->part_attr & PART_ATTR_LEGACY_BIOS_BOOTABLE);
        return 0;
    }
    return 1;
}

u64 get_part_addr(const char *name) {
    struct part_info_t part_info;

    if (mt_get_part_info_by_name(name, &part_info) != 0) {
        pal_log_err("%smt_get_part_info_by_name fail\n", TAG);
        return 0;
    } else {
        return (u64)part_info.addr;
    }
}
#endif


static int parse_gpt_header(u32 part_id, u64 header_lba, u8 *header_buf, u8 *entries_buf)
{
    int i;
    blkdev_t *dev;
    int err;
    u32 calc_crc, orig_header_crc;
    u64 entries_real_size, entries_read_size, num_block_entry;
    gpt_header *header = (gpt_header *)header_buf;
    gpt_entry *entries = (gpt_entry *)entries_buf;

    init_crc32_table(gpt_dram_crc32_table);
    dev = blkdev_get(CFG_BOOT_DEV);
    if (!dev) {
        pal_log_err("%sparse_gpt_header fail, err(no dev)\n", TAG);
        return 1;
    }

    if (get_gpt_header(part_id, header_lba, header_buf, dev->blksz, gpt_dram_crc32_table)) {
        pal_log_err("%s get_gpt_header fail\n", TAG);
        return 1;
    }

    entries_real_size = (u64)header->num_partition_entries * header->sizeof_partition_entry;
    num_block_entry = (u64)(dev->blksz / header->sizeof_partition_entry);
    entries_read_size = (u64)((header->num_partition_entries + (num_block_entry - 1)) / num_block_entry) * dev->blksz;

    err = read_data(entries_buf, part_id, header->partition_entry_lba, entries_read_size);
    if (err) {
        pal_log_err("%sread entries(part_id=%d,lba=%llx), err(%d)\n",
                TAG, part_id, header->partition_entry_lba, err);
        return err;
    }

    calc_crc = efi_crc32(~0L, (u8 *)entries, (u32)entries_real_size, gpt_dram_crc32_table);
    calc_crc = efi_crc32_finalize(calc_crc);

    if (header->partition_entry_array_crc32 != calc_crc) {
        pal_log_err("%scheck header, err(entries crc 0x%x!=0x%x(calc))\n",
                TAG, header->partition_entry_array_crc32, calc_crc);
        return 1;
    }

    for (i = 0; i < header->num_partition_entries; i++) {
        /* break if partition entry is empty */
        if(!entries[i].starting_lba)
            break;
        part_ptr[i].info = &g_dram_buf->meta_info[i];
        get_part_info(part_id, &part_ptr[i], &entries[i]);
    }

    return 0;
}


int read_gpt(part_t *part)
{
    int err;
    u64 lba;
    u32 part_id;

    part_id = storage_get_part_id(STORAGE_PHYS_PART_USER);

    part_ptr = part;

    pal_log_debug("%sParsing Primary GPT now...\n", TAG);
    err = parse_gpt_header(part_id, 1, GPT_HEADER_BUF, GPT_ENTRIES_BUF);
    if (!err) {
        goto find;
    }

    pal_log_debug("%sParsing Secondary GPT now...\n", TAG);
    lba = last_lba(part_id);
    if (!lba){
        pal_log_err("%sFailure to find last lba.\n", TAG);
        return 1;
    }
    err = parse_gpt_header(part_id, lba, GPT_HEADER_BUF, GPT_ENTRIES_BUF);
    if (!err) {
        goto find;
    }

    pal_log_err("%sFailure to find valid GPT.\n", TAG);
    return err;

find:
    pal_log_err("%sSuccess to find valid GPT.\n", TAG);
    return 0;
}
