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
tz_init.c \
tz_sec_cfg.c \
tz_emi_mpu.c

ifeq ($(CFG_TRUSTONIC_TEE_SUPPORT),1)
MOD_SRC  += tz_tbase.c
endif

ifeq ($(CFG_MICROTRUST_TEE_SUPPORT),1)
MOD_SRC  += tz_teei.c
endif

ifeq ($(CFG_MICROTRUST_TEE_LITE_SUPPORT),1)
MOD_SRC  += tz_teei.c
endif
###################################################################
# target
###################################################################

include $(COMMON_FILE_MK)
