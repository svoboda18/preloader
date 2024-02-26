LOCAL_PATH := $(call my-dir)
PRELOADER_ROOT_DIR := $(PWD)

ifdef PRELOADER_TARGET_PRODUCT
ifeq ($(BUILD_PRELOADER),yes)

  PRELOADER_DIR := $(LOCAL_PATH)

include $(LOCAL_PATH)/platform/$(MTK_PLATFORM_DIR)/default.mak
include $(LOCAL_PATH)/custom/$(MTK_PROJECT_NAME)/cust_bldr.mak

PRELOADER_UFS_HEADER_BLOCK := $(LOCAL_PATH)/platform/$(MTK_PLATFORM_DIR)/headerblock_for_ota_fastboot_ufs
PRELOADER_EMMC_HEADER_BLOCK := $(LOCAL_PATH)/platform/$(MTK_PLATFORM_DIR)/headerblock_for_ota_fastboot_emmc

ifeq ("$(CFG_BOOT_DEV)", "BOOTDEV_UFS")
  PRELOADER_HEADER_BLOCK := $(LOCAL_PATH)/platform/$(MTK_PLATFORM_DIR)/headerblock_for_ota_fastboot_ufs
else
  ifeq ("$(CFG_BOOT_DEV)", "BOOTDEV_SDMMC")
    PRELOADER_HEADER_BLOCK := $(LOCAL_PATH)/platform/$(MTK_PLATFORM_DIR)/headerblock_for_ota_fastboot_emmc
  endif
endif

  ifeq ($(PRELOADER_CROSS_COMPILE),)
    ifeq ($(TARGET_ARCH), arm)
      PRELOADER_CROSS_COMPILE := $(PRELOADER_ROOT_DIR)/$(TARGET_TOOLS_PREFIX)
    else ifeq ($(TARGET_2ND_ARCH), arm)
      PRELOADER_CROSS_COMPILE := $(PRELOADER_ROOT_DIR)/$($(TARGET_2ND_ARCH_VAR_PREFIX)TARGET_TOOLS_PREFIX)
    endif
  endif
  # Check flavor support
  PROJECT_CONFIGS         := $(PRELOADER_DIR)/custom/$(PRELOADER_TARGET_PRODUCT)/$(PRELOADER_TARGET_PRODUCT).mk

  PL_MODE :=
  include $(LOCAL_PATH)/build_preloader.mk

  ifeq (yes,$(BOARD_BUILD_SBOOT_DIS_PL))
    PL_MODE := SBOOT_DIS
    include $(LOCAL_PATH)/build_preloader.mk
  endif

  ifeq (yes,$(BOARD_BUILD_RPMB_KEY_PL))
    PL_MODE := RPMB_KEY
    include $(LOCAL_PATH)/build_preloader.mk
  endif

.PHONY: clean-preloader check-mtk-config check-pl-config
preloader pl: check-pl-config

check-mtk-config: check-pl-config
check-pl-config:
ifneq (yes,$(strip $(DISABLE_MTK_CONFIG_CHECK)))
	python device/mediatek/build/build/tools/check_kernel_config.py -c $(MTK_TARGET_PROJECT_FOLDER)/ProjectConfig.mk -b $(PROJECT_CONFIGS) -p $(MTK_PROJECT_NAME)
else
	-python device/mediatek/build/build/tools/check_kernel_config.py -c $(MTK_TARGET_PROJECT_FOLDER)/ProjectConfig.mk -b $(PROJECT_CONFIGS) -p $(MTK_PROJECT_NAME)
endif

else

  PL_MODE :=
  include $(LOCAL_PATH)/prebuilt_preloader.mk

endif#BUILD_PRELOADER
.PHONY: preloader pl
droidcore: preloader
endif#PRELOADER_TARGET_PRODUCT

