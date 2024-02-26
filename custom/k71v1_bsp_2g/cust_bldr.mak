###################################################################
# Include Project Feautre  (cust_bldr.h)
###################################################################

#ifeq ("$(MTK_EMMC_SUPPORT)","yes")
ifdef MTK_EMMC_SUPPORT
CFG_BOOT_DEV :=BOOTDEV_SDMMC
else
CFG_BOOT_DEV :=BOOTDEV_NAND
endif

CFG_UART_LOG :=UART1
CFG_UART_META :=UART1

CFG_RPMB_KEY_RETRY_IN_PL :=1
CFG_RPMB_PROGRAM_KEY :=1
