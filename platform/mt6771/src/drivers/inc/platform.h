/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

#ifndef PLATFORM_H
#define PLATFORM_H

#include "print.h"
#include "string.h"
#include "boot_device.h"
#include "blkdev.h"
#include "preloader.h"

#include "mt6771.h"

#include "cust_bldr.h"
#include "partition.h"
#include "cust_rtc.h"
#include "cust_nand.h"
#include "cust_usb.h"
#include "sec_limit.h"
#include "mblock.h"
#ifdef LP4_DRAM_SUPPORT
#include "emi_lp4.h"
#else
#include "emi.h"
#endif
#include "plat_dbg_info.h"

#include "cust_drdi.h"
#include "pal_log.h"

/*=======================================================================*/
/* Pre-Loader Internal Features                                          */
/*=======================================================================*/

/* if not defined in cust_usb.h, use default setting */
#if !defined(CFG_USB_ENUM_TIMEOUT)
#define CFG_USB_ENUM_TIMEOUT            (8000)           /* 8s */
#endif

/* if not defined in cust_usb.h, use default setting */
#if !defined(CFG_USB_HANDSHAKE_TIMEOUT)
#define CFG_USB_HANDSHAKE_TIMEOUT       (2500)           /* 2.5s */
#endif

/* support switch to modem com or not */
#if (defined( MTK_DT_SUPPORT) && !defined(EVDO_DT_SUPPORT))
#define CFG_DT_MD_DOWNLOAD              (1)
#else
#define CFG_DT_MD_DOWNLOAD              (0)
#endif

/*=======================================================================*/
/* Platform Setting                                                      */
/*=======================================================================*/
#if CFG_FPGA_PLATFORM
#define I2C_SRC_CLK_FRQ                 (12000000)
#define UART_SRC_CLK_FRQ                (10000000)
#define MSDC_SRC_CLK_FRQ                (12000000)

#else /* !CFG_FPGA_PLATFORM */
#define UART_SRC_CLK_FRQ                (0)         /* use default */
#endif

#define PART_MAX_NUM                    20

/*=======================================================================*/
/* Image Address                                                         */
/*=======================================================================*/

#if CFG_BYPASS_EMI
#if CFG_FPGA_PLATFORM
#define CFG_DRAM_ADDR                   (0x0011C000)
#else
//fpga will overwrite the cluster 0 cache
#define CFG_DRAM_ADDR                   (0x0011C000)
#endif
#define CFG_DA_RAM_ADDR                 (0x0010DC00)
#else
#define CFG_DRAM_ADDR                   (0x40000000)
#define CFG_DA_RAM_ADDR                 (CFG_DRAM_ADDR + 0x00200000)
#endif



#if CFG_TEE_SUPPORT
#ifdef CFG_TEE_TRUSTED_APP_HEAP_SIZE
#define CFG_TEE_CORE_SIZE               (0x500000 + CFG_TEE_TRUSTED_APP_HEAP_SIZE + CFG_TEE_FP_HEAP_SIZE)
#else
#define CFG_TEE_CORE_SIZE               (0x500000)
#endif
#if CFG_TRUSTONIC_TEE_SUPPORT
#define CFG_MIN_TEE_DRAM_SIZE           (0x600000)
#define CFG_MAX_TEE_DRAM_SIZE           (160 * 1024 * 1024) /* TEE max DRAM size is 160MB */
#else
#define CFG_MIN_TEE_DRAM_SIZE           (0)
#define CFG_MAX_TEE_DRAM_SIZE           (0) /* TEE max DRAM size is 0 if TEE is not enabled */
#endif
#endif

//ALPS00427972, implement the analog register formula
//Add here for eFuse, chip version checking -> analog register calibration
#define M_HW_RES3	                    0x10009170
//#define M_HW_RES3_PHY                   IO_PHYS+M_HW_RES3
#define RG_USB20_TERM_VREF_SEL_MASK     0xE000      //0b 1110,0000,0000,0000     15~13
#define RG_USB20_CLKREF_REF_MASK        0x1C00      //0b 0001,1100,0000,0000     12~10
#define RG_USB20_VRT_VREF_SEL_MASK      0x0380      //0b 0000,0011,1000,0000     9~7
//ALPS00427972, implement the analog register formula

//add abstractlayer to avoid hard code in Preloader
#define PL_PMIC_PWR_KEY	MTK_PMIC_PWR_KEY
//#define PL_PMIC_PWR_KEY	8

#if CFG_BOOT_TIME_PROFILE
extern U32 profiling_time;	//declare in main.c
#define BOOTING_TIME_PROFILING_LOG(stage) \
do { \
	print("#T#%s=%d\n", stage, get_timer(profiling_time)); \
	profiling_time = get_timer(0);	\
}while(0)
#else
#define BOOTING_TIME_PROFILING_LOG(stage) do {}while(0)
#endif

typedef enum {
    NORMAL_BOOT         = 0,
    META_BOOT           = 1,
    RECOVERY_BOOT       = 2,
    SW_REBOOT           = 3,
    FACTORY_BOOT        = 4,
    ADVMETA_BOOT        = 5,
    ATE_FACTORY_BOOT    = 6,
    ALARM_BOOT          = 7,
    FASTBOOT            = 99,

    DOWNLOAD_BOOT       = 100,
    UNKNOWN_BOOT
} boot_mode_t;

typedef enum {
    BR_POWER_KEY = 0,
    BR_USB,
    BR_RTC,
    BR_WDT,
    BR_WDT_BY_PASS_PWK,
    BR_TOOL_BY_PASS_PWK,
    BR_2SEC_REBOOT,
    BR_UNKNOWN,
    BR_KERNEL_PANIC,
    BR_WDT_SW,
    BR_WDT_HW
} boot_reason_t;

typedef enum {
    META_UNKNOWN_COM = 0,
    META_UART_COM,
    META_USB_COM
} meta_com_t;

/* boot argument magic */
#define BOOT_ARGUMENT_MAGIC             (0x504c504c)
#define BA_FIELD_BYPASS_MAGIC           (0x5A5B5A5B)

#define ME_IDENTITY_LEN                 (16)
#define MAX_RECORD_SIZE                 (512)
#define MAX_BL_NUM                      (8)

#define SOC_ID_LEN                      (32)
#define PROVISIONING_KEY_LEN            (16)

typedef struct {
    u32 bl_load_addr;
    u32 bl_length;
    u32 p_bl_desc;
    u8  bl_index;
    u8  state;
    u16 code_1;
    u16 code_2;
} bl_info_t;

typedef struct {
    u32 raw_data[MAX_RECORD_SIZE>>2];
} hdrblk_info_t;

typedef struct {
    u32 ver;
    u32 hdr_blk_dev_addr;
    hdrblk_info_t hdr_blk_info[4];
    bl_info_t bl_loadinfo[MAX_BL_NUM];
    u32 brom_flags;
    u32 meid[ME_IDENTITY_LEN>>2];
	u32 sctrl_cert_file_addr;
	u32 sctrl_cert_file_len;
	u32 tool_auth_file_addr;
	u32 tool_auth_file_len;
	u32 reserved0[10];
	u8  socid[SOC_ID_LEN];
	u8  prov_key[PROVISIONING_KEY_LEN];
	u32 cc_lcs;
	u32 cc_flags;
	u32 reserved1[2];
	u32 sw_version;
	u32 reserved2;
} bl_param_t;

typedef struct {
    u32 addr;    /* download agent address */
    u32 arg1;    /* download agent argument 1 */
    u32 arg2;    /* download agent argument 2 */
    u32 len;     /* length of DA */
    u32 sig_len; /* signature length of DA */
} da_info_t;

typedef struct {
    u32 magic;
    boot_mode_t mode;
    u32 e_flag;
    u32 log_port;
    u32 log_baudrate;
    u8  log_enable;
    u8  part_num;
    u8  reserved[2];
    u32 dram_rank_num;
    u64 dram_rank_size[4];
    mblock_info_t mblock_info;	/* number of mblock should be max rank number + 1 */
    dram_info_t orig_dram_info;
    mem_desc_t lca_reserved_mem;
    mem_desc_t tee_reserved_mem;
    u32 boot_reason;
    u32 meta_com_type;
    u32 meta_com_id;
    u32 boot_time;
    da_info_t da_info;
    SEC_LIMIT sec_limit;
    part_hdr_t *part_info;
    u8  md_type[4];
    u32  ddr_reserve_enable;
    u32  ddr_reserve_success;
    u32  ddr_reserve_ready;
    u32  dram_buf_size;
    u32  meta_uart_port;
    u32 non_secure_sram_addr;
    u32 non_secure_sram_size;
    u32 charger_type;
	u32 boot_voltage;
	u32 shutdown_time;
} boot_arg_t;

typedef enum {
    CHIP_VER_E1 = 0x0,
    CHIP_VER_E2 = 0x1,
} chip_ver_t;

typedef enum {
    COM_UART = 0,
    COM_USB
} comport_t;

struct comport_ops {
    int (*send)(u8 *buf, u32 len);
    int (*recv)(u8 *buf, u32 len, u32 tmo);
};

struct bldr_comport {
    u32 type;
    u32 tmo;
    struct comport_ops *ops;
};

struct bldr_command {
    u8  *data;
    u32  len;
};

struct bldr_command_handler;

typedef bool (*bldr_cmd_handler_t)(struct bldr_command_handler *handler, struct bldr_command *cmd, struct bldr_comport *comm);

struct bldr_command_handler {
    void *priv;
    u32   attr;
    bldr_cmd_handler_t cb;
};

#define CMD_HNDL_ATTR_COM_FORBIDDEN               (1<<0)

/* bootarg for ATF */
typedef struct {
    u64 bootarg_loc;
    u64 bootarg_size;
    u64 bl33_start_addr;
    u64 tee_info_addr;
} mtk_bl_param_t;

/* ====== Preloader to LK Tags START ===== */

/* boot reason */
#define BOOT_TAG_BOOT_REASON     0x88610001
struct boot_tag_boot_reason {
    u32 boot_reason;
};

/* boot mode */
#define BOOT_TAG_BOOT_MODE       0x88610002
struct boot_tag_boot_mode {
    u32 boot_mode;
};

/* META com port information */
#define BOOT_TAG_META_COM        0x88610003
struct boot_tag_meta_com {
    u32 meta_com_type;
    u32 meta_com_id;
    u32 meta_uart_port;
    u32 meta_log_disable;
    u32 fast_meta_gpio;
};

/* log com port information */
#define BOOT_TAG_LOG_COM         0x88610004
struct boot_tag_log_com {
    u32 log_port;
    u32 log_baudrate;
    u32 log_enable;
#if CFG_UART_DYNAMIC_SWITCH
    u32 log_dynamic_switch;
#endif
};

/* memory information */
#define BOOT_TAG_MEM             0x88610005
struct boot_tag_mem {
    u32 dram_rank_num;
    u64 dram_rank_size[4];
    mblock_info_t mblock_info;
    dram_info_t orig_dram_info;
    mem_desc_t lca_reserved_mem;
    mem_desc_t tee_reserved_mem;
};

/* MD information */
#define BOOT_TAG_MD_INFO         0x88610006
struct boot_tag_md_info{
    u32 md_type[4];
};

/* boot time */
#define BOOT_TAG_BOOT_TIME       0x88610007
struct boot_tag_boot_time {
    u32 boot_time;
};

/* DA information */
#define BOOT_TAG_DA_INFO         0x88610008
struct boot_tag_da_info {
    da_info_t da_info;
};

/* security limitation information */
#define BOOT_TAG_SEC_INFO        0x88610009
struct boot_tag_sec_info {
    SEC_LIMIT sec_limit;
};

/* (for dummy AP) partition number */
#define BOOT_TAG_PART_NUM        0x8861000A
struct boot_tag_part_num {
    u32 part_num;
};

/* (for dummy AP) partition info */
#define BOOT_TAG_PART_INFO       0x8861000B
struct boot_tag_part_info {
    part_hdr_t *part_info;
};

/* eflag */
#define BOOT_TAG_EFLAG           0x8861000C
struct boot_tag_eflag {
    u32 e_flag;
};

/* DDR reserve */
#define BOOT_TAG_DDR_RESERVE     0x8861000D
struct boot_tag_ddr_reserve {
    u32 ddr_reserve_enable;
    u32 ddr_reserve_success;
    u32 ddr_reserve_ready;
};

/* DRAM BUFF */
#define BOOT_TAG_DRAM_BUF        0x8861000E
struct boot_tag_dram_buf {
    u32 dram_buf_size;
};

/* sram info */
#define BOOT_TAG_SRAM_INFO      0x88610012
struct boot_tag_sram_info {
        u32 non_secure_sram_addr;
        u32 non_secure_sram_size;
};

/* platform, DRAM, EMI debug info */
#define BOOT_TAG_PLAT_DBG_INFO      0x88610013
struct boot_tag_plat_dbg_info {
    u32 info_max;
    dbg_info_in_bootargs info[INFO_TYPE_MAX];
};

/* charger type info */
#define BOOT_TAG_CHR_INFO      0x88610018
struct boot_tag_chr_info {
        u32 charger_type;
};

/* charger type info */
#define BOOT_TAG_BAT_INFO      0x88610019
struct boot_tag_bat_info {
		u32 boot_voltage;
		u32 shutdown_time;
};

#define BOOT_TAG_IS_ABNORMAL_BOOT     0x8861001B
struct boot_tag_is_abnormal_boot {
    u32 is_abnormal_boot;
};

#define BOOT_TAG_RAM_CONSOLE_INFO   0x8861001C
struct boot_tag_ram_console_info {
    u32 sram_addr;
    u32 sram_size;
    u32 def_type;
    u32 memory_info_offset;
};

/* emi info */
#define BOOT_TAG_EMI_INFO	0x88610020
struct boot_tag_emi_info {
	u32 dram_type;
	u32 ch_num;
	u32 rk_num;
	u64 rank_size[MAX_RK];
};

#define BOOT_TAG_SOC_ID        0x88610021
struct boot_tag_soc_id {
	u8 id[SOC_ID_LEN];
};

struct boot_tag_header {
    u32 size;
    u32 tag;
};

#if CFG_GZ_SUPPORT
#define BOOT_TAG_GZ_INFO  (0x88610023)
#define BOOT_TAG_GZ_PARAM (0x88610024)
#define BOOT_TAG_GZ_PLAT  (0x88610025)
#include <gz_atag.h>
#endif

#if CFG_GZ_PWRAP_ENABLE
#define BOOT_TAG_GZ_PWRAP (0x88610026)
#endif

typedef struct {
    struct boot_tag_header hdr;
    union {
        struct boot_tag_boot_reason boot_reason;
        struct boot_tag_boot_mode boot_mode;
        struct boot_tag_meta_com meta_com;
        struct boot_tag_log_com log_com;
        struct boot_tag_mem mem;
        struct boot_tag_md_info md_info;
        struct boot_tag_boot_time boot_time;
        struct boot_tag_da_info da_info;
        struct boot_tag_sec_info sec_info;
        struct boot_tag_part_num part_num;
        struct boot_tag_part_info part_info;
        struct boot_tag_eflag eflag;
        struct boot_tag_ddr_reserve ddr_reserve;
        struct boot_tag_dram_buf dram_buf;
        struct boot_tag_emi_info emi_info;
        struct boot_tag_sram_info sram_info;
        struct boot_tag_plat_dbg_info plat_dbg_info;
        struct boot_tag_chr_info chr_info;
        struct boot_tag_bat_info bat_info;
#if CFG_L2C_SHARE_WORKAROUND
        struct boot_tag_is_volt_up volt_info;
#endif
        struct boot_tag_is_abnormal_boot is_abnormal_boot;
        struct boot_tag_soc_id socid;
        struct boot_tag_ram_console_info ram_console_info;
#if CFG_GZ_SUPPORT
        struct boot_tag_gz_info gz_info;
        struct boot_tag_gz_param gz_param;
        struct boot_tag_gz_platform gz_plat;
#endif
#if CFG_GZ_PWRAP_ENABLE
        struct boot_tag_gz_pwrap gz_pwrap;
#endif
    } u;
}boot_tag;

#define boot_tag_next(t)    ((struct boot_tag *)((u32 *)(t) + (t)->hdr.size))
#define boot_tag_size(type)	((sizeof(struct boot_tag_header) + sizeof(struct type)) >> 2)

/* ====== Preloader to LK Tags END ===== */

extern int usb_cable_in(void);

extern int  platform_get_mcp_id(u8 *id, u32 len, u32 *fw_id_len);
extern void platform_vusb_on(void);
extern void platform_wdt_kick(void);
extern void platform_wdt_all_kick(void);
extern void platform_pre_init(void);
extern void platform_init(void);
extern void platform_post_init(void);
extern void platform_error_handler(void);
extern void platform_assert(char *file, int line, char *expr);
extern chip_ver_t platform_chip_ver(void);
extern u64 platform_memory_size(void);

extern void apmcu_isb(void);
extern void apmcu_disable_icache(void);
extern void apmcu_icache_invalidate(void);
extern void apmcu_disable_smp(void);
extern void jump(u32 addr, u32 arg1, u32 arg2);
extern void jumparch64(mtk_bl_param_t *bl31_info, u32 bl31_reserve);

extern boot_mode_t g_boot_mode;
extern boot_dev_t  g_boot_dev;
extern boot_reason_t g_boot_reason;
extern meta_com_t g_meta_com_type;
extern u32 g_meta_com_id;
extern unsigned char g_meta_log_disable;

/* emi functions */
extern void get_orig_dram_rank_info(dram_info_t *orig_dram_info);


#define AARCH64_SLT_DONE_ADDRESS (0x110000)
#define AARCH64_SLT_DONE_MAGIC   (0x5A5A5A5A)

typedef enum {
    HW_VER_V0 = 0, // EVB
    HW_VER_V1 = 1, // Phone
} hw_ver_t;
hw_ver_t drdi_get_hw_ver(void);

#if CFG_FAST_META_SUPPORT
extern void fast_meta_gpio_init(void);
extern bool check_meta_short_pin(void);
extern void read_meta_proinfo(u8 *meta_boot, u8*meta_log);
#endif

#endif /* PLATFORM_H */
