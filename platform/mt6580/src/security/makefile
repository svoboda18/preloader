###################################################################
# flag
###################################################################

MOD_SRC  =
MOD_OBJ  = $(PRELOADER_OUT)/obj

###################################################################
# source files
# - add your file here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
###################################################################

MOD_SRC  := \
sec.c \
sec_boot.c \
sec_ctrl.c \
sec_rom_info.c \
sec_secroimg.c \
sec_util.c \
sec_region.c \
sec_flashtool_cfg.c \
verified_boot_dummy.c

ifeq ($(CFG_ATF_SUPPORT),1)
SUBDIRS += trustzone
else
# TEE ARMV7 SUPPORT
ifeq ($(CFG_TRUSTONIC_TEE_SUPPORT),1)
SUBDIRS += trustzone
else
# TRUSTY ARMV7 SUPPORT
ifeq ($(CFG_GOOGLE_TRUSTY_SUPPORT),1)
SUBDIRS += trustzone
endif
ifeq ($(CFG_MICROTRUST_TEE_SUPPORT),1)
SUBDIRS += trustzone
endif
ifeq ($(CFG_MICROTRUST_TEE_LITE_SUPPORT),1)
SUBDIRS += trustzone
endif
endif
endif
###################################################################
# target
###################################################################

include $(COMMON_FILE_MK)

###################################################################
# include sub dir
###################################################################

ifeq ($(CUSTOM_SEC_AUTH_SUPPORT), yes)
SUBDIRS += $(MTK_ROOT_CUSTOM)/common/security/preloader
else
SUBDIRS += auth
endif

include $(COMMON_DIR_MK)
