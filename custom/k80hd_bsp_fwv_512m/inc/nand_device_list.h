
#ifndef __NAND_DEVICE_LIST_H__
#define __NAND_DEVICE_LIST_H__

#define NAND_MAX_ID		5
#define CHIP_CNT		2
#define P_SIZE		2048
#define P_PER_BLK		64
#define C_SIZE		512
#define RAMDOM_READ		(1<<0)
#define CACHE_READ		(1<<1)

#define RAND_TYPE_SAMSUNG 0
#define RAND_TYPE_TOSHIBA 1
#define RAND_TYPE_NONE 2

#define READ_RETRY_MAX 10
struct gFeature
{
	u32 address;
	u32 feature;
};

enum readRetryType
{
	RTYPE_FEATURE,
	RTYPE_PRECMD,
	RTYPE_PARAPAGE,
	RTYPE_HYNIX_16NM,
	RTYPE_HYNIX
};

struct gFeatureSet
{
	u8 sfeatureCmd;
	u8 gfeatureCmd;
	u8 readRetryPreCmd;
	u8 readRetryCnt;
	u32 readRetryAddress;
	u32 readRetryDefault;
	u32 readRetryStart;
	enum readRetryType rtype;
	struct gFeature Interface;
	struct gFeature Async_timing;
};

struct gRandConfig
{
	u8 type;
	u32 seed[6];
};

enum pptbl
{
	MICRON_8K,
	HYNIX_8K,
	SANDISK_16K,
};

struct MLC_feature_set
{
	enum pptbl ptbl_idx;
	struct gFeatureSet 	 FeatureSet;
	struct gRandConfig   randConfig;
};

enum flashdev_vendor
{
	VEND_SAMSUNG,
	VEND_MICRON,
	VEND_TOSHIBA,
	VEND_HYNIX,
	VEND_SANDISK,
	VEND_NONE,
};

enum flashdev_IOWidth
{
	IO_8BIT = 8,
	IO_16BIT = 16,
	IO_TOGGLEDDR = 9,
	IO_TOGGLESDR = 10,
	IO_ONFI = 12,
};

typedef struct
{
   u8 id[NAND_MAX_ID];
   u8 id_length;
   u8 addr_cycle;
   enum flashdev_IOWidth iowidth;
   u16 totalsize;
   u16 blocksize;
   u16 pagesize;
   u16 sparesize;
   u32 timmingsetting;
   u32 s_acccon;
   u32 s_acccon1;
   u32 freq;
   enum flashdev_vendor vendor;
   u16 sectorsize;
   u8 devciename[30];
   u32 advancedmode;
   struct MLC_feature_set feature_set;
}flashdev_info,*pflashdev_info;

static const flashdev_info gen_FlashTable[]={
	{{0xAD,0xBC,0x90,0x55,0x54}, 5,5,IO_16BIT,512,128,2048,64,0x21014133, 0,0,0,VEND_NONE,512, "H9DA4GH4JJAMCR_4EM",0}, 
	{{0xAD,0xBC,0x90,0x55,0x56}, 5,5,IO_16BIT,512,128,2048,64,0x21014133, 0,0,0,VEND_NONE,512, "H9TA4GH2GDMCPR ",0}, 
};

#endif
