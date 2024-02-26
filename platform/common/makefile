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
	div/crt.S \
	div/idiv.S \
	div/idivmod.S \
	div/ldivmod.S \
	div/umodsi3.S \
	addr_trans.c \
	div0.c \
	stdlib.c \
	sec_hrid.c \
	string.c \
	error.c \
	hmac.c

ifeq ("$(CFG_PROFILING_SUPPORT)", "yes")
MOD_SRC	 += profiling.c
else
MOD_SRC	 += profiling_dummy.c
endif

ifeq ("$(CFG_PLAT_DBG_INFO_LIB)", "1")
	MOD_SRC  += plat_dbg_info.c
endif

ifeq ("$(CFG_RAM_CONSOLE)", "1")
	MOD_SRC	+= ram_console.c
endif

ifeq ("$(MTK_AB_OTA_UPDATER)", "yes")
ifeq ("$(MTK_BOOTCTRL_VERSION)", "1.0")
MTK_BOOTCTRL_VERSION = 1.0
else
MTK_BOOTCTRL_VERSION = 2.0
endif
export MTK_BOOTCTRL_VERSION
MOD_SRC += bootctrl/$(MTK_BOOTCTRL_VERSION)/ab_bootcheck.c
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

SUBDIRS += partition
SUBDIRS += storage
SUBDIRS += loader
SUBDIRS += bootctrl
SUBDIRS += emimpu
SUBDIRS += mblock
SUBDIRS += watchdog
ifeq ("$(CFG_UART_COMMON)", "1")
	SUBDIRS += uart
endif

ifeq ($(CFG_ATF_SUPPORT),1)
SUBDIRS += trustzone
endif

ifeq ($(CFG_LOG_STORE_SUPPORT),1)
SUBDIRS += log_store
endif

ifeq ($(CFG_GZ_SUPPORT),1)
SUBDIRS += geniezone
endif

include $(COMMON_DIR_MK)

