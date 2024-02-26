###################################################################
# flag
###################################################################

MOD_SRC  =
MOD_OBJ  = $(PRELOADER_OUT)/obj

###################################################################
# source files
# - add your file here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
###################################################################

MOD_SRC  := part_common.c

ifeq ("$(CFG_BOOT_DEV)", "BOOTDEV_NAND")
MOD_SRC	 += part_nand.c
endif

ifeq ("$(CFG_BOOT_DEV)", "BOOTDEV_SDMMC")
MOD_SRC	 += part_emmc_ufs.c
ifeq ("$(CFG_PARTITION_COMMON)", "1")
MOD_SRC  += efi.c
endif
endif

ifeq ("$(CFG_BOOT_DEV)", "BOOTDEV_UFS")
MOD_SRC	 += part_emmc_ufs.c
ifeq ("$(CFG_PARTITION_COMMON)", "1")
MOD_SRC  += efi.c
endif
endif

ifeq ("$(CFG_PARTITION_COMMON)", "1")
MOD_SRC  += dconfig_env.c
endif


###################################################################
# target
###################################################################

include $(COMMON_FILE_MK)

