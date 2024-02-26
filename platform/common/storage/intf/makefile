###################################################################
# flag
###################################################################

MOD_SRC  =
MOD_OBJ  = $(PRELOADER_OUT)/obj

###################################################################
# source files
# - add your file here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
###################################################################

MOD_SRC  :=

ifeq ("$(CFG_BOOT_DEV)", "BOOTDEV_NAND")
MOD_SRC	 += storage_nand_intf.c
endif

ifeq ("$(CFG_BOOT_DEV)", "BOOTDEV_SDMMC")
MOD_SRC	 += storage_emmc_ufs_intf.c
endif

ifeq ("$(CFG_BOOT_DEV)", "BOOTDEV_UFS")
MOD_SRC	 += storage_emmc_ufs_intf.c
endif

ifeq ("$(CFG_PARTITION_COMMON)", "1")
MOD_SRC  += blkdev.c
endif

###################################################################
# target
###################################################################

include $(COMMON_FILE_MK)

