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
ifeq ($(CFG_GZ_SUPPORT),1)
MOD_SRC += gz_init.c
ifeq ($(CFG_GZ_REMAP), 1)
MOD_SRC += gz_remap.c
endif
endif

ifeq ($(strip $(TARGET_BUILD_VARIANT)),user)
MTK_CFLAGS += -DTARGET_BUILD_VARIANT_USER
endif

ifeq ($(strip $(TARGET_BUILD_VARIANT)),eng)
MTK_CFLAGS += -DTARGET_BUILD_VARIANT_ENG
endif

ifeq ($(strip $(TARGET_BUILD_VARIANT)),userdebug)
MTK_CFLAGS += -DTARGET_BUILD_VARIANT_USERDEBUG
endif

###################################################################
# target
###################################################################
include $(COMMON_FILE_MK)


