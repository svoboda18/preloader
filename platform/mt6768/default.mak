###################################################################
# Default Internal Feautre
###################################################################
HW_INIT_ONLY :=

###################################################################
# Default Project Feautre  (cust_bldr.h)
###################################################################

CFG_BOOT_DEV :=BOOTDEV_SDMMC
CFG_MMC_COMBO_DRV :=1

# Setting for bring up, booting from SRAM
CFG_BYPASS_EMI :=0

# no load ATF/LK, and force booting to ATF
CFG_BYPASS_LOAD_IMG_FORCE_ATF :=0

#Pull Hi Flash enable bit for UART META
CFG_ENABLE_GPIO_CHK_POINT_WITH_UART_META :=0

CFG_FPGA_PLATFORM :=0
CFG_EVB_PLATFORM :=0
CFG_BATTERY_DETECT :=1

CFG_USB_TOOL_HANDSHAKE :=1
CFG_USB_DOWNLOAD :=1
CFG_PMT_SUPPORT :=0
CFG_UART_COMMON :=1
CFG_LOG_BAUDRATE :=921600
CFG_META_BAUDRATE :=115200
CFG_EVB_UART_CLOCK :=26000000
CFG_FPGA_UART_CLOCK :=10000000
CFG_UART_LOG :=UART1
CFG_UART_META :=UART1

CFG_OUTPUT_PL_LOG_TO_UART1:=0

CFG_EMERGENCY_DL_SUPPORT :=1
CFG_EMERGENCY_DL_TIMEOUT_MS :=1000*5
CFG_EMERGENCY_MAX_TIMEOUT_MS :=1000*0x3fff
CFG_USBIF_COMPLIANCE :=0
CFG_MMC_ADDR_TRANS :=1
CFG_LEGACY_USB_DOWNLOAD :=0

MTK_UART_USB_SWITCH:=no

#
# AP WDT driver configuration
# The version of common driver is aligned to kernel
#
# CFG_APWDT :=
#   0       : Disable driver (not built-in)
#   V2      : Use common driver version 2
#
CFG_APWDT :=V2

#
# AP WDT watchdog
#
# CFG_APWDT_DISABLE :=
#   0       : Disable watchdog
#   1       : Enable watchdog
#
CFG_APWDT_DISABLE :=0

CFG_BOOT_ARGUMENT :=1
CFG_BOOT_ARGUMENT_BY_ATAG := 1
CFG_RAM_CONSOLE :=1
CFG_MTJTAG_SWITCH :=0
CFG_MDMETA_DETECT :=0
CFG_MDWDT_DISABLE :=0
# 3K: 3*1024
CFG_SYS_STACK_SZ :=3072
# Check if stack usage of a function is larger than the size above.
CFG_STACK_USAGE_CHECK :=1

CFG_WORLD_PHONE_SUPPORT :=1
CFG_SVP3_PLATFORM :=0

CFG_LPRST_SUPPORT :=1
ONEKEY_REBOOT_NORMAL_MODE_PL :=1
KPD_PMIC_LPRST_TD :=0

CFG_USB_AUTO_DETECT :=0
CFG_USB_AUTO_DETECT_TIMEOUT_MS :=1000*3

CFG_FEATURE_ENCODE :=v1

FEATURE_DOWNLOAD_SCREEN :=0

MTK_EFUSE_WRITER_RESERVE_CODESIZE :=yes

MTK_EFUSE_WRITER_SUPPORT :=no

# dynamic switch UART log, and disable UART handshake in user load
ifeq ("$(TARGET_BUILD_VARIANT)","user")
	CFG_UART_DYNAMIC_SWITCH :=1
	CFG_BOOT_TIME_PROFILE :=1
	CFG_UART_TOOL_HANDSHAKE :=0
else
	CFG_UART_DYNAMIC_SWITCH :=0
	CFG_BOOT_TIME_PROFILE :=0
	CFG_UART_TOOL_HANDSHAKE :=1
endif

# DRAM Calibration Optimization:
# DRAM calib data will be stored to storage device to enhance DRAM init speed.
CFG_DRAM_CALIB_OPTIMIZATION :=1

CFG_FUNCTION_PICACHU_SUPPORT :=1

CFG_PRELOADER_AS_DA :=0

CFG_DRDI_EVB_PHONE :=1

CFG_FAST_META_SUPPORT :=0
CFG_FAST_META_GPIO_CHECK :=0
CFG_FAST_META_GPIO :=GPIO178
CFG_FAST_META_FLAG :=1

# For log level
# LOG_LEVEL_ALWAYS     (0)
# LOG_LEVEL_ERROR      (1)
# LOG_LEVEL_WARNING    (2)
# LOG_LEVEL_INFO       (3)
# LOG_LEVEL_DEBUG      (4)
# Any log messages with levels less than and equal to this will be printed.
CFG_LOG_LEVEL :=3

###################################################################
# image loading options
###################################################################

#For Normal Boot
CFG_LOAD_UBOOT :=1
#For Dummy AP
CFG_LOAD_MD_ROM :=0
CFG_LOAD_MD_RAMDISK :=0
CFG_LOAD_MD_DSP :=0
CFG_LOAD_MD3_ROM :=0
#For SLT and Dummy AP
CFG_LOAD_AP_ROM :=0

#For CTP
CFG_LOAD_CONN_SYS :=0
#For SLT
CFG_LOAD_SLT_MD :=0
CFG_LOAD_SLT_SCP :=0
CFG_LOAD_SLT_MD32 :=0
CFG_LOAD_SLT_SSPM :=0

###################################################################
# Dummy Load address
# If image header have load address, we use address in image header
# If not, we use address here
###################################################################
#For Normal Boot
CFG_UBOOT_MEMADDR       :=0x4C400000
#For Dummy AP
CFG_MD1_ROM_MEMADDR     :=0x42000000
CFG_MD1_RAMDISK_MEMADDR :=0x49CC0000
CFG_MD2_ROM_MEMADDR     :=0x42000000
CFG_MD2_RAMDISK_MEMADDR :=0x43400000
CFG_MD_DSP_MEMADDR      :=0x4A000000
CFG_MD3_ROM_MEMADDR     :=0x52000000
#For SLT and Dummy AP
CFG_AP_ROM_MEMADDR      :=0x62000000
#For CTP
CFG_CONN_SYS_MEMADDR    :=0x45A00000
#For SLT
CFG_TDD_MD_ROM_MEMADDR   :=0x40000000
CFG_TDD_ONLY_ROM_MEMADDR :=0x41000000
CFG_FDD_MD_ROM_MEMADDR   :=0x42000000
CFG_2G_MD_ROM_MEMADDR    :=0x43000000
CFG_MD32P_ROM_MEMADDR    :=0x44000000
CFG_MD32D_ROM_MEMADDR    :=0x45002000
CFG_BOOTA64_MEMADDR 	 :=0x40000000
CFG_DTB_MEMADDR 	 :=0x40000300
CFG_IMAGE_AARCH64_MEMADDR :=0x40080000

#For SSPM
CFG_SSPM_MEMADDR	:=0x10400000

CFG_ATF_SUPPORT :=1
CFG_ATF_LOG_SUPPORT :=1
CFG_TEE_SUPPORT :=0
ifeq ("$(TRUSTONIC_TEE_SUPPORT)","yes")
CFG_RPMB_SET_KEY :=1
else
CFG_RPMB_SET_KEY :=0
endif
CFG_TEE_VIRTUAL_RPMB_SUPPORT :=1
CFG_MICROTRUST_TEE_SUPPORT :=0
CFG_TRUSTONIC_TEE_SUPPORT :=0
ifeq ("$(TARGET_BUILD_VARIANT)","eng")
CFG_TEE_SECURE_MEM_PROTECTED :=0
else
CFG_TEE_SECURE_MEM_PROTECTED :=1
endif
CFG_TEE_SECURE_MEM_TAG_FORMAT :=1

ifeq ("$(MTK_TEE_TRUSTED_UI_SUPPORT)","yes")
CFG_TEE_TUI_HEAP_SIZE :=0xE40000
else ifeq ("$(MTK_FINGERPRINT_SUPPORT)","yes")
CFG_TEE_FP_HEAP_SIZE :=0x800000
endif
CFG_TEE_TUI_HEAP_SIZE ?=0
CFG_TEE_FP_HEAP_SIZE ?=0
CFG_TEE_TRUSTED_APP_HEAP_SIZE :=0x500000

CFG_TEE_SECURE_MEM_SHARED :=1
CFG_GOOGLE_TRUSTY_SUPPORT :=0
ifeq ("$(TARGET_BUILD_VARIANT)","eng")
CFG_LAST_EMI_BW_DUMP :=1
else
CFG_LAST_EMI_BW_DUMP :=0
endif

#For boot time speed up by data cache
CFG_ENABLE_DCACHE := 1

#For SRAM Protection
CFG_NON_SECURE_SRAM_ADDR :=0x0010DC00
CFG_NON_SECURE_SRAM_SIZE :=0x4400
CFG_EMI_MPU_LIB := 1
CFG_MBLOCK_LIB := 2
CFG_PLAT_DBG_INFO_LIB := 1

# For Boot Voltage Config
CFG_BOOT_VOL :=1
SEC_ROM_INFO_V2 :=yes

# partition common driver support
CFG_PARTITION_COMMON := 1

# doe config env data api support
CFG_DOE_CONFIG_ENV_SUPPORT := 1

# access partition table before dram init
CFG_EARLY_PARTITION_ACCESS := 1

# For GenieZone
ifeq ("$(MTK_ENABLE_GENIEZONE)", "yes")
CFG_GZ_SUPPORT := 1
CFG_GZ_REMAP := 1
CFG_GZ_PWRAP_ENABLE := 1
else
CFG_GZ_SUPPORT := 0
CFG_GZ_REMAP := 0
CFG_GZ_PWRAP_ENABLE := 0
endif

# For Nebula
ifeq ("$(MTK_NEBULA_VM_SUPPORT)", "yes")
CFG_NEBULA_VM_SUPPORT := 1
CFG_NEBULA_LOAD_IN_PART2 := 0
else
CFG_NEBULA_VM_SUPPORT := 0
CFG_NEBULA_LOAD_IN_PART2 := 0
endif

CFG_LOG_STORE_SUPPORT :=1
C_OPTION += -DLOG_STORE_SUPPORT

MTK_AEE_SUPPORT := yes
ifeq ("$(TARGET_BUILD_VARIANT)","user")
ifeq ("$(MTK_AEE_SUPPORT)", "yes")
CFG_AEE_SUPPORT := 1
else
CFG_AEE_SUPPORT := 0
endif
else
CFG_AEE_SUPPORT := 1
endif
