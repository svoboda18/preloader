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
sec_ctrl.c \
sec_rom_info.c \
sec_util.c \
sec_region.c \
sec_efuse.c \
rpmb_key.c \
sec_policy.c \
oaep.c \
rsa_oaep_sample.c \
rsa_oaep.c \

ifeq ($(CFG_ATF_SUPPORT),1)
SUBDIRS += trustzone
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
