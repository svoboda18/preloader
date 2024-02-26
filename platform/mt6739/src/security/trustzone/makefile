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

ifeq ($(CFG_GOOGLE_TRUSTY_SUPPORT),1)
MOD_SRC  += tz_trusty.c
endif

ifeq ($(CFG_MICROTRUST_TEE_SUPPORT),1)
MOD_SRC  += tz_teei.c
endif

ifeq ($(CFG_TRUSTKERNEL_TEE_SUPPORT),1)
MOD_SRC += tz_tkcore.c
endif

###################################################################
# target
###################################################################

include $(COMMON_FILE_MK)

###################################################################
# include sub dir
###################################################################

include $(COMMON_DIR_MK)
