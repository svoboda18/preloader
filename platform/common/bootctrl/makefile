###################################################################
# flag
###################################################################

MOD_SRC  =
MOD_OBJ  = $(PRELOADER_OUT)/obj

###################################################################
# source files
# - add your file here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
###################################################################

MOD_SRC :=
ifeq ("$(MTK_AB_OTA_UPDATER)", "yes")
ifeq ("$(MTK_BOOTCTRL_VERSION)", "1.0")
	MOD_SRC += 1.0/bootctrl_api.c
else
	MOD_SRC += 2.0/bootctrl_api.c
	MOD_SRC += 2.0/avb/libavb/avb_util.c
	MOD_SRC += 2.0/avb/libavb/avb_crc32.c
endif
endif

###################################################################
# target
###################################################################
include $(COMMON_FILE_MK)


