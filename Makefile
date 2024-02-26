# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.
#
# MediaTek Inc. (C) 2011. All rights reserved.
#
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
# RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
# AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
#
# The following software/firmware and/or related documentation ("MediaTek Software")
# have been modified by MediaTek Inc. All revisions are subject to any receiver's
# applicable license agreements with MediaTek Inc.

# Compatible with Ubuntu 12.04
SHELL := /bin/bash

.DELETE_ON_ERROR:
all:

HOST_OS ?= $(shell uname | tr '[A-Z]' '[a-z]')
##############################################################
# Including Neccesary Files
#

ifndef PRELOADER_OUT
  PRELOADER_OUT := .
endif
ifndef MTK_PROJECT
  $(error MTK_PROJECT is not defined)
else
  export MTK_PROJECT
endif


##############################################################
# Secure Library Building Control
#

export CREATE_SEC_LIB := FALSE
export STANDALONE_SEC_DEVINFO := TRUE
export CREATE_DEVINFO_LIB := FALSE

##############################################################
# Variable Initialization
#

export D_ROOT           := $(CURDIR)
export MTK_ROOT_OUT     := $(CURDIR)
export PWD              := $(CURDIR)
ALL                     ?= show_title show_internal_feature show_feature
ALL_PREGEN_FILE         :=
export MTK_ROOT_CUSTOM  := $(D_ROOT)/custom

PROJECT_CONFIGS         := $(MTK_ROOT_CUSTOM)/$(MTK_PROJECT)/$(MTK_PROJECT).mk
include $(PROJECT_CONFIGS)

ifndef TARGET
  $(error TARGET is not defined)
else
  export TARGET
endif
export MTK_PATH_CUSTOM  := $(MTK_ROOT_CUSTOM)/$(TARGET)

ifndef MTK_PLATFORM
  $(error MTK_PLATFORM is not defined)
else
  export MTK_PLATFORM
  PLATFORM := $(shell echo $(MTK_PLATFORM) | tr A-Z a-z)
endif
export MTK_PATH_PLATFORM := $(D_ROOT)/platform/$(PLATFORM)

export MTK_PATH_COMMON         := $(D_ROOT)/platform/common
export MTK_PATH_PAL            := $(D_ROOT)/platform/pal

export INCLUDE_FILE_COMMON := -I$(MTK_PATH_COMMON)/inc \
				-I$(MTK_PATH_COMMON)/geniezone/inc \
				-I$(MTK_PATH_PAL)/inc \
				-I$(PRELOADER_OUT)/inc \
				-I$(MTK_PATH_PLATFORM)/src/init/inc \
				-I$(MTK_PATH_PLATFORM)/src/core/inc \
				-I$(MTK_PATH_PLATFORM)/src/drivers/inc \
				-I$(MTK_PATH_PLATFORM)/src/security/inc \
				-I$(MTK_PATH_PLATFORM)/src/security/trustzone/inc \
				-I$(MTK_ROOT_CUSTOM)/$(PLATFORM)/drdi

#
# Image Auth
#
CUSTOM_PATH             := $(MTK_PATH_CUSTOM)/security
IMAGE_AUTH_CFG_FILE     := ${PRELOADER_OUT}/inc/KEY_IMAGE_AUTH.h
#IMAGE_AUTH_CONFIG_PATH  := ${CUSTOM_PATH}/image_auth
IMAGE_AUTH_CONFIG_PATH  := ${CUSTOM_PATH}/image_auth
IMAGE_AUTH_KEY          := ${IMAGE_AUTH_CONFIG_PATH}/IMG_AUTH_KEY.ini
IMAGE_AUTH_CFG          := ${IMAGE_AUTH_CONFIG_PATH}/IMG_AUTH_CFG.ini
IMAGE_AUTH_KEY_EXIST    := $(if $(wildcard $(IMAGE_AUTH_KEY)),TRUE,FALSE)
ifeq ("$(IMAGE_AUTH_KEY_EXIST)","TRUE")
ALL                     += img_auth_info
ALL_PREGEN_FILE         += $(IMAGE_AUTH_CFG_FILE)
endif
CHIP_CONFIG_PATH        := $(CUSTOM_PATH)/chip_config
GFH_PATH                := $(CHIP_CONFIG_PATH)/s/gfh
CONFIG_PATH             := $(CHIP_CONFIG_PATH)/s/cfg
KEY_PATH                := $(CHIP_CONFIG_PATH)/s/key
ifneq ($(wildcard $(CONFIG_PATH)/PADDING_TYPE.ini),)
PADDING_TYPE            := $(shell cat $(CONFIG_PATH)/PADDING_TYPE.ini)
endif

#
# preloader extension
#
PRELOADER_EXT_BIN       := $(MTK_ROOT_CUSTOM)/$(PLATFORM)/preloader/preloader_ext.bin
PRELOADER_EXT_BIN_EXIST := $(if $(wildcard $(PRELOADER_EXT_BIN)),TRUE,FALSE)

#
# Tool
#
EFUSE_PY_FILE			:= tools/efuse/efuse_bingen_v2.py
EFUSE_PY_DEF			:= tools/efuse/$(MTK_PLATFORM)/efuse_definition.xml
EFUSE_INPUT_FILE		:= $(D_ROOT)/../../../../../../vendor/mediatek/proprietary/custom/$(TARGET)/security/efuse/input.xml
EFUSE_IMAGE_NAME        := efuse_$(MTK_PROJECT).img

TOOL_PATH               ?= $(D_ROOT)/../../../../../../device/mediatek/build/build/tools

ifeq ($(HOST_OS),darwin)
SIGN_TOOL               := tools/signtool/SignTool_PL.darwin
CIPHER_TOOL             := tools/ciphertool/CipherTool.darwin
else
SIGN_TOOL               := tools/signtool/SignTool_PL
CIPHER_TOOL             := tools/ciphertool/CipherTool
endif

ifdef PL_MODE
PL_IMAGE_NAME           := preloader_$(MTK_PROJECT)_$(PL_MODE)
else
PL_IMAGE_NAME           := preloader_$(MTK_PROJECT)
endif

SEC_LIB                 := $(MTK_PATH_PLATFORM)/lib/libsec.a
SEC_PLAT_LIB            := $(MTK_PATH_PLATFORM)/lib/libsecplat.a
CRYPTO_LIB              := $(MTK_PATH_PLATFORM)/lib/libcrypto.a
DEVINFO_LIB     	    := $(MTK_PATH_PLATFORM)/lib/libdevinfo.a
DEVINFO_DEBUG_LIB       := $(MTK_PATH_PLATFORM)/lib/libdevinfo_debug.a

HW_CRYPTO_LIB           := $(MTK_PATH_PLATFORM)/src/HwCryptoLib.a
PICACHU_LIB             := $(MTK_PATH_PLATFORM)/src/PicachuLib.a
ALL                     += build_info project_info
ALL_PREGEN_FILE         += $(PRELOADER_OUT)/inc/preloader.h $(PRELOADER_OUT)/inc/proj_cfg.h
COMMON_DIR_MK           := $(D_ROOT)/build/common-dir.mak
COMMON_FILE_MK          := $(D_ROOT)/build/common.mak
export COMMON_DIR_MK COMMON_FILE_MK

SUBDIRS                 := $(MTK_PATH_PLATFORM)/src/ $(MTK_PATH_CUSTOM)/ $(MTK_PATH_COMMON)/ $(MTK_PATH_PAL)/
ifeq ($(CREATE_SEC_LIB),TRUE)
    ALL                 += $(SEC_LIB) $(CRYPTO_LIB) $(SEC_PLAT_LIB)
ifeq ($(STANDALONE_SEC_DEVINFO),TRUE)
    ALL                 +=  $(DEVINFO_LIB) $(DEVINFO_DEBUG_LIB)
endif
endif
ifeq ($(CREATE_DEVINFO_LIB),TRUE)
    ALL                 +=  $(DEVINFO_LIB) $(DEVINFO_DEBUG_LIB)
endif
ALL                     += preloader_bin
ALL                     += dump_build_info

include build/debug.in build/setting.in
include $(MTK_PATH_PLATFORM)/makefile.mak
export EMIGEN_OUT := $(PRELOADER_OUT)
export DRVGEN_OUT := $(PRELOADER_OUT)
export PTGEN_OUT := $(PRELOADER_OUT)
include tools/emigen/emigen.mk
include tools/drvgen/drvgen.mk
-include tools/ptgen/$(MTK_PLATFORM)/ptgen.mk
ALL_PREGEN_FILE += $(EMIGEN_FILE_LIST)
ALL_PREGEN_FILE += $(DRVGEN_FILE_LIST)
ALL_PREGEN_FILE += $(PTGEN_FILE_LIST)
export ALL_DEPENDENCY_FILE := $(foreach m,$(MAKEFILE_LIST),$(if $(filter /% ~%,$(m)),$(m),$(D_ROOT)/$(m)))

.PHONY: clean
.PHONY: $(ALL)
all: $(ALL)

#########
# recursive
#
# $(1): subdir
# $(2): target flag
# $(3): dependency
define build-topdirs
$(2): $(3) FORCE
	@mkdir -p $$(dir $$@)
	$$(MAKE) -C $(1) --no-print-directory
endef

.PHONY: FORCE
FORCE:
SUBDIRS_FLAGS :=
$(foreach d,$(SUBDIRS),\
  $(eval r := $(patsubst $(D_ROOT)/%,%,$(patsubst %/,%,$(if $(filter /%,$(d)),,$(CURDIR)/)$(d))))\
  $(eval t := $(PRELOADER_OUT)/obj/$(subst /,_,$(r)).flag)\
  $(eval $(call build-topdirs,$(d),$(t),$(ALL_PREGEN_FILE)))\
  $(eval SUBDIRS_FLAGS += $(t))\
)

##############################################################
# ELF Generation
#
CFG_PRELOADER_EXT_COMMON_BUILD := 0
ifeq ($(CFG_PRELOADER_EXTENSION), 1)
ifeq ($(CREATE_SEC_LIB),FALSE)
	CFG_PRELOADER_EXT_COMMON_BUILD := 1
endif
endif

ifeq ($(CFG_PRELOADER_EXT_COMMON_BUILD), 1)
	SECLIB2OBJ_TOOL := tools/efuse/seclib2obj_convertor.py
	SECLIB_OBJ_LIST_FILE := $(PRELOADER_OUT)/seclib_obj_list.txt
	DEVINFO_OBJ_DIR       := $(PRELOADER_OUT)/obj/libdevinfo_obj
else
	SECLIB2OBJ_TOOL :=
	SECLIB_OBJ_LIST_FILE :=
	DEVINFO_OBJ_DIR       :=
endif

ifeq ($(CFG_PRELOADER_DRAM_USE), 1)
  PL_IMG_SECOND_PARTION_SECTION :=.pl_dram.text .pl_dram.data .pl_dram.rodata .pl_dram.start
  OBJONCHIP_FLAG := $(addprefix -R , $(PL_IMG_SECOND_PARTION_SECTION))
  OBJSECOND_FLAG := $(addprefix -j , $(PL_IMG_SECOND_PARTION_SECTION))
  PL_DRAM_IMAGE_NAME := $(PL_IMAGE_NAME)_dram
  PL_DRAM_HDRBIN_PATH := $(D_BIN)/hd_$(PL_DRAM_IMAGE_NAME).bin
  LDSCRIPT := $(D_BIN)/$(MTK_PROJECT)_gen.ld
else
  OBJONCHIP_FLAG :=
  OBJSECOND_FLAG :=
  PL_DRAM_IMAGE_NAME :=
  PL_DRAM_HDRBIN_PATH :=
endif

ifdef CFG_TEE_DRAM_SIZE
PL_DRAM_RESERVE_SIZE := $(CFG_TEE_DRAM_SIZE)
else
PL_DRAM_RESERVE_SIZE := 0
endif

ifdef CFG_PRELOADER_DRAM_USE
ifeq ($(CFG_PRELOADER_DRAM_USE), 1)
$(LDSCRIPT): $(LDSCRIPT_DRAM)
$(LDSCRIPT): $(ALL_DEPENDENCY_FILE)
	@echo generating $@
	$(MKDIR)
	sed "s/%CFG_DRAM_SIZE%/$(CFG_DRAM_SIZE)/;s/%EXT_PRELOAER_DRAMSIZE%/$(EXT_PRELOAER_DRAMSIZE)/;s/%PL_DRAM_RESERVE_SIZE%/$(PL_DRAM_RESERVE_SIZE)/" < $< > $@
endif
endif

ifeq ($(CFG_PRELOADER_EXTENSION), 1)
MKIMAGE                    := tools/mkimage/mkimage
LOADER_EXT_NAME            := loader_ext
LOADER_EXT_ETC_HEADER_CFG  := loader_ext_etc_hdr.cfg
LOADER_EXT_DRAM_HEADER_CFG := loader_ext_dram_hdr.cfg
endif #($(CFG_PRELOADER_EXTENSION), 1)

ifneq ($(wildcard $(CONFIG_PATH)/PBP_PY_SUPPORT),)
PBP_TOOL := tools/pbp/pbp.py
BIN_TO_TXT_TOOL := tools/pbp/bin2txt.py
CHIP_KEY := $(KEY_PATH)/pl_key.ini
else
ifeq ($(HOST_OS),darwin)
PBP_TOOL := tools/pbp/PBP.darwin
else
PBP_TOOL := tools/pbp/PBP
endif
CHIP_CONFIG := $(CONFIG_PATH)/CHIP_CONFIG.ini
CHIP_KEY := $(KEY_PATH)/CHIP_TEST_KEY.ini
endif

preloader_bin: $(D_BIN)/$(PL_IMAGE_NAME).bin $(PRELOADER_OUT)/report-codesize.csv $(PRELOADER_OUT)/codesize_profiling.csv
ifeq ($(MTK_EFUSE_WRITER_SUPPORT),yes)
preloader_bin: $(D_BIN)/efuse.img
endif

$(D_BIN)/keyhash.bin: $(PBP_TOOL) $(KEY_PATH)/root_prvk.pem
ifneq (,$(findstring legacy,$(PADDING_TYPE)))
	PYTHONDONTWRITEBYTECODE=True python $(PBP_TOOL) -j $(KEY_PATH)/root_prvk.pem -func keyhash_legacy -o $@
else
	PYTHONDONTWRITEBYTECODE=True python $(PBP_TOOL) -j $(KEY_PATH)/root_prvk.pem -func keyhash_pss -o $@
endif

$(D_BIN)/keyhash.txt: $(D_BIN)/keyhash.bin $(BIN_TO_TXT_TOOL) $(PBP_TOOL) $(CHIP_KEY)
ifneq ($(wildcard $(CONFIG_PATH)/PBP_PY_SUPPORT),)
	python $(BIN_TO_TXT_TOOL) $< $@
else
	$(PBP_TOOL) -func keyhash -i $(CHIP_KEY) -o $@
endif

$(D_BIN)/$(EFUSE_IMAGE_NAME): $(D_BIN)/keyhash.txt $(EFUSE_PY_FILE) $(EFUSE_INPUT_FILE) $(EFUSE_PY_DEF)
	rm -rf $(PRELOADER_OUT)/efuse_bingen.log
	python $(EFUSE_PY_FILE) -f $(EFUSE_INPUT_FILE) -d $(EFUSE_PY_DEF) -k $< -o $@ -l $(PRELOADER_OUT)/efuse_bingen.log
	rm -rf $(D_BIN)/keyhash.txt

$(D_BIN)/efuse.img: $(D_BIN)/$(EFUSE_IMAGE_NAME)
ifeq ($(MTK_EFUSE_WRITER_SUPPORT),yes)
	rm -rf $@
	cp -f $< $@
endif

$(D_BIN)/$(PL_IMAGE_NAME).elf: $(SUBDIRS_FLAGS) $(LDSCRIPT)
$(D_BIN)/$(PL_IMAGE_NAME).elf: $(wildcard $(PICACHU_LIB)) $(SEC_LIB) $(CRYPTO_LIB) $(SEC_PLAT_LIB) $(wildcard $(HW_CRYPTO_LIB)) $(wildcard $(DEVINFO_LIB)) $(wildcard $(DEVINFO_DEBUG_LIB))
$(D_BIN)/$(PL_IMAGE_NAME).elf: $(ALL_DEPENDENCY_FILE)
	@mkdir -p $(dir $@)

ifeq ($(CFG_PRELOADER_EXT_COMMON_BUILD), 1)
	rm -rf $(DEVINFO_OBJ_DIR)
	mkdir -p $(DEVINFO_OBJ_DIR)
	$(AR) -t $(DEVINFO_LIB) | grep "\.o" > $(SECLIB_OBJ_LIST_FILE)
	python $(SECLIB2OBJ_TOOL) -b $(SECLIB_OBJ_LIST_FILE) -r $(D_ROOT) -p $(DEVINFO_OBJ_DIR) -s $(DEVINFO_LIB) -t $(PRELOADER_OUT)
	rm -rf $(SECLIB_OBJ_LIST_FILE)
	$(LD) --gc-sections -Bstatic -T$(LDSCRIPT) \
	`cat $(PRELOADER_OUT)/obj/*.flag | tr ' ' '\n' | grep "\.o$$" | LC_COLLATE=C sort` \
	$(wildcard $(PICACHU_LIB)) \
	--start-group \
	$(SEC_LIB) \
	$(CRYPTO_LIB) \
	$(SEC_PLAT_LIB) \
	$(DEVINFO_OBJ_DIR)/*.o \
	$(wildcard $(DEVINFO_DEBUG_LIB)) \
	$(wildcard $(HW_CRYPTO_LIB)) \
	--end-group \
	-Map $(PRELOADER_OUT)/system.map -o $(D_BIN)/$(PL_IMAGE_NAME).elf
	rm -rf $(DEVINFO_OBJ_DIR)
else
	$(LD) --gc-sections -Bstatic -T$(LDSCRIPT) \
	`cat $(PRELOADER_OUT)/obj/*.flag | tr ' ' '\n' | grep "\.o$$" | LC_COLLATE=C sort` \
	$(wildcard $(PICACHU_LIB)) \
	--start-group \
	$(SEC_LIB) \
	$(CRYPTO_LIB) \
	$(SEC_PLAT_LIB) \
	$(wildcard $(DEVINFO_LIB)) \
	$(wildcard $(DEVINFO_DEBUG_LIB)) \
	$(wildcard $(HW_CRYPTO_LIB)) \
	--end-group \
	-Map $(PRELOADER_OUT)/system.map -o $(D_BIN)/$(PL_IMAGE_NAME).elf
endif

$(D_BIN)/preloader.elf: $(D_BIN)/$(PL_IMAGE_NAME).elf
	$(OBJCOPY) -R .dram $(D_BIN)/$(PL_IMAGE_NAME).elf -O elf32-littlearm $(D_BIN)/preloader.elf

ifeq ($(CFG_PRELOADER_DRAM_USE), 1)
preloader_bin: $(D_BIN)/$(PL_DRAM_IMAGE_NAME).bin
$(D_BIN)/$(PL_DRAM_IMAGE_NAME).bin: $(D_BIN)/$(PL_IMAGE_NAME).elf
	$(hide) $(OBJCOPY) $(OBJCFLAGS) $(OBJSECOND_FLAG) $(D_BIN)/$(PL_IMAGE_NAME).elf -O binary $(D_BIN)/$(PL_DRAM_IMAGE_NAME).bin
endif

$(D_BIN)/$(PL_IMAGE_NAME)_LINKED.bin: $(D_BIN)/$(PL_IMAGE_NAME).elf
ifeq ($(PRELOADER_EXT_BIN_EXIST),TRUE)
$(D_BIN)/$(PL_IMAGE_NAME)_LINKED.bin: $(PRELOADER_EXT_BIN)
endif
$(D_BIN)/$(PL_IMAGE_NAME)_LINKED.bin: $(PRELOADER_OUT)/MTK_Loader_Info.tag
$(D_BIN)/$(PL_IMAGE_NAME)_LINKED.bin: zero_padding.sh
$(D_BIN)/$(PL_IMAGE_NAME)_LINKED.bin: $(PRELOADER_OUT)/report-objsize.txt
$(D_BIN)/$(PL_IMAGE_NAME)_LINKED.bin: $(PRELOADER_OUT)/report-codesize.txt
$(D_BIN)/$(PL_IMAGE_NAME)_LINKED.bin: $(PRELOADER_OUT)/function.map
ifeq ($(CFG_PRELOADER_DRAM_USE), 1)
	$(hide) $(OBJCOPY) $(OBJCFLAGS) $(OBJONCHIP_FLAG) $(D_BIN)/$(PL_IMAGE_NAME).elf -O binary $@
else
ifeq ($(CFG_PRELOADER_EXTENSION), 1)
	# loader_ext_etc
	$(hide) $(OBJCOPY) $(OBJCFLAGS) -j .loader_ext_etc $(D_BIN)/$(PL_IMAGE_NAME).elf -O binary $(D_BIN)/$(PL_IMAGE_NAME)_ETC_NO_HDR.bin
	$(hide) $(MKIMAGE) $(D_BIN)/$(PL_IMAGE_NAME)_ETC_NO_HDR.bin $(MTK_PATH_PLATFORM)/$(LOADER_EXT_ETC_HEADER_CFG) > $(D_BIN)/$(PL_IMAGE_NAME)_ETC.bin

	# loader_ext_dram
	$(hide) $(OBJCOPY) $(OBJCFLAGS) -j .loader_ext_dram $(D_BIN)/$(PL_IMAGE_NAME).elf -O binary $(D_BIN)/$(PL_IMAGE_NAME)_DRAM_NO_HDR.bin
	$(hide) $(MKIMAGE) $(D_BIN)/$(PL_IMAGE_NAME)_DRAM_NO_HDR.bin $(MTK_PATH_PLATFORM)/$(LOADER_EXT_DRAM_HEADER_CFG) > $(D_BIN)/$(PL_IMAGE_NAME)_DRAM.bin

	# loader_ext.img = loader_ext_dram + loader_ext_etc
	cp $(D_BIN)/$(PL_IMAGE_NAME)_DRAM.bin $(D_BIN)/$(LOADER_EXT_NAME).img
	cat $(D_BIN)/$(PL_IMAGE_NAME)_ETC.bin >> $(D_BIN)/$(LOADER_EXT_NAME).img

	$(hide) $(OBJCOPY) $(OBJCFLAGS) -R .loader_ext_etc -R .loader_ext_dram $(D_BIN)/$(PL_IMAGE_NAME).elf -O binary $@
else # original flow
	$(hide) $(OBJCOPY) $(OBJCFLAGS) $(D_BIN)/$(PL_IMAGE_NAME).elf -O binary $@
endif #($(CFG_PRELOADER_EXTENSION), 1)
endif
ifeq ("$(PRELOADER_EXT_BIN_EXIST)","TRUE")
	cat $(PRELOADER_EXT_BIN) >> $@
endif
	./zero_padding.sh $@ 4
	./alignment_check.sh $(PRELOADER_OUT)/MTK_Loader_Info.tag 4
	cat $(PRELOADER_OUT)/MTK_Loader_Info.tag >> $@

$(PRELOADER_OUT)/report-codesize.txt: $(D_BIN)/$(PL_IMAGE_NAME).elf
	$(CROSS_COMPILE)readelf -s $(D_BIN)/$(PL_IMAGE_NAME).elf | awk -F':' '/Num/ {print $$2}' > $@
	$(CROSS_COMPILE)readelf -s $(D_BIN)/$(PL_IMAGE_NAME).elf | awk -F':' '{if ($$1>0) print $$2}' | awk -F' ' '{if ($$2>0) print $$0}' | sort -k1 >> $@

$(PRELOADER_OUT)/report-codesize.csv: $(PRELOADER_OUT)/report-codesize.txt
	cat $(PRELOADER_OUT)/report-codesize.txt | sed 's/[ ]+/,/g' > $@

$(PRELOADER_OUT)/report-objsize.txt: $(D_BIN)/$(PL_IMAGE_NAME).elf
	$(CROSS_COMPILE)size `ls $(PRELOADER_OUT)/obj/*.o` > $@

$(PRELOADER_OUT)/codesize_profiling.csv: $(D_BIN)/$(PL_IMAGE_NAME).elf
$(PRELOADER_OUT)/codesize_profiling.csv: codesize_profiling.sh
	./codesize_profiling.sh $(PRELOADER_OUT)/system.map $@

##############################################################
# Security Library Generation
#

ifeq ($(CREATE_SEC_LIB),TRUE)
$(SEC_LIB): $(SUBDIRS_FLAGS)
	rm -rf $@
	@echo AR $(PRELOADER_OUT)/out/*.o
	$(AR) -r $@ $(PRELOADER_OUT)/out/*.o
	@echo =================================================================
	@echo Security Library Common
	@echo '$@' built at
	@echo time : $(shell date)
	@echo =================================================================

$(CRYPTO_LIB): $(SUBDIRS_FLAGS)
	rm -rf $@
	@echo AR $(PRELOADER_OUT)/out_crypto/*.o
	$(AR) -r $@ $(PRELOADER_OUT)/out_crypto/*.o
	@echo =================================================================
	@echo Crypto Library
	@echo '$@' built at
	@echo time : $(shell date)
	@echo =================================================================

$(SEC_PLAT_LIB): $(SUBDIRS_FLAGS)
	rm -rf $@
	@echo AR $(PRELOADER_OUT)/out_plat/*.o
	$(AR) -r $@ $(PRELOADER_OUT)/out_plat/*.o
	@echo =================================================================
	@echo Security Library Platform
	@echo '$@' built at
	@echo time : $(shell date)
	@echo =================================================================
endif

ifeq ($(CREATE_DEVINFO_LIB), TRUE)
$(DEVINFO_LIB): $(SUBDIRS_FLAGS)
	rm -rf $@
	@echo AR $(PRELOADER_OUT)/out_devinfo/*.o
	-$(AR) -r $@ $(PRELOADER_OUT)/out_devinfo/*.o
	@echo =================================================================
	@echo Devinfo Security Library
	@echo '$@' built at
	@echo time : $(shell date)
	@echo =================================================================

$(DEVINFO_DEBUG_LIB): $(SUBDIRS_FLAGS)
	rm -rf $@
	@echo AR $(PRELOADER_OUT)/out_devinfo_debug/*.o
	-$(AR) -r $@ $(PRELOADER_OUT)/out_devinfo_debug/*.o
	@echo =================================================================
	@echo Devinfo Security Library Debug Version
	@echo '$@' built at
	@echo time : $(shell date)
	@echo =================================================================
endif

##############################################################
# File for Debugging
#

$(PRELOADER_OUT)/function.map: $(D_BIN)/$(PL_IMAGE_NAME).elf
	$(NM) $< | grep -v '\(compiled\)\|\(\.o$$\)\|\( [aUw] \)\|\(\.\.ng$$\)\|\(LASH[RL]DI\)' | sort > $@

##############################################################
# Dump Configurations
#

show_title:
	@echo =================================================================
	@echo Building Configuration:
	@echo Project              = $(MTK_PROJECT)
	@echo Platform             = $(MTK_PLATFORM)
	@echo Buildspec            = buildspec.mak
	@echo Create SecLib        = $(CREATE_SEC_LIB)
	@echo Image Auth key exist = $(IMAGE_AUTH_KEY_EXIST)
	@echo Preloader EXT exist  = $(PRELOADER_EXT_BIN_EXIST)
	@echo SECRO AC support     = $(MTK_SEC_SECRO_AC_SUPPORT)
	@echo =================================================================

show_internal_feature:
	@echo =================================================================
	@echo Internal Feature:
	@echo HW_INIT_ONLY         = $(HW_INIT_ONLY)
	@echo CFG_MDJTAG_SWITCH= $(CFG_MDJTAG_SWITCH)
	@echo CFG_MDMETA_DETECT= $(CFG_MDMETA_DETECT)
	@echo CFG_HW_WATCHDOG= $(CFG_HW_WATCHDOG)
	@echo CFG_APWDT= $(CFG_APWDT)
	@echo CFG_APWDT_DISABLE= $(CFG_APWDT_DISABLE)
	@echo CFG_MDWDT_DISABLE= $(CFG_MDWDT_DISABLE)
	@echo CFG_SYS_STACK_SZ= $(CFG_SYS_STACK_SZ)
	@echo CFG_MMC_ADDR_TRANS= $(CFG_MMC_ADDR_TRANS)
	@echo CFG_BOOT_ARGUMENT= $(CFG_BOOT_ARGUMENT)
	@echo CFG_RAM_CONSOLE= $(CFG_RAM_CONSOLE)
	@echo =================================================================
	@echo MTK_PATH_PLATFORM= $(MTK_PATH_PLATFORM)
	@echo MTK_PATH_CUSTOM= $(MTK_PATH_CUSTOM)
	@echo MTK_ROOT_CUSTOM= $(MTK_ROOT_CUSTOM)
	@echo CUSTOM_PATH= $(CUSTOM_PATH)
	@echo =================================================================

show_feature:
	@if [ -e "$(MTK_PATH_CUSTOM)/cust_bldr.mak" ]; then \
	echo  =========================================== ; \
	echo Platform Feature: ; \
	echo 'CFG_FPGA_PLATFORM'= $(CFG_FPGA_PLATFORM) ; \
	echo 'CFG_EVB_PLATFORM'= $(CFG_EVB_PLATFORM) ; \
	echo 'CFG_BATTERY_DETECT'= $(CFG_BATTERY_DETECT) ; \
	echo 'CFG_PMT_SUPPORT'= $(CFG_PMT_SUPPORT) ; \
	echo  =========================================== ; \
	echo Communication Feature: ; \
	echo 'CFG_UART_TOOL_HANDSHAKE'= $(CFG_UART_TOOL_HANDSHAKE) ; \
	echo 'CFG_USB_TOOL_HANDSHAKE'= $(CFG_USB_TOOL_HANDSHAKE) ; \
	echo 'CFG_USB_DOWNLOAD'= $(CFG_USB_DOWNLOAD) ; \
	echo 'CFG_LOG_BAUDRATE'= $(CFG_LOG_BAUDRATE) ; \
	echo 'CFG_META_BAUDRATE'= $(CFG_META_BAUDRATE) ; \
	echo 'CFG_UART_LOG'= $(CFG_UART_LOG) ; \
	echo 'CFG_UART_META'= $(CFG_UART_META) ; \
	echo 'CFG_EMERGENCY_DL_SUPPORT'= $(CFG_EMERGENCY_DL_SUPPORT) ; \
	echo 'CFG_EMERGENCY_DL_TIMEOUT_MS'= $(CFG_EMERGENCY_DL_TIMEOUT_MS) ; \
	echo 'MTK_UART_USB_SWITCH'= $(MTK_UART_USB_SWITCH) ; \
	echo  =========================================== ; \
	echo Image Loading: ; \
	echo 'CFG_LOAD_UBOOT'= $(CFG_LOAD_UBOOT) ; \
	echo 'CFG_LOAD_AP_ROM'= $(CFG_LOAD_AP_ROM) ; \
	echo 'CFG_LOAD_MD_ROM'= $(CFG_LOAD_MD_ROM) ; \
	echo 'CFG_LOAD_MD_RAMDISK'= $(CFG_LOAD_MD_RAMDISK) ; \
	echo 'CFG_LOAD_CONN_SYS'= $(CFG_LOAD_CONN_SYS) ; \
	echo 'CFG_UBOOT_MEMADDR'= $(CFG_UBOOT_MEMADDR) ; \
	echo 'CFG_AP_ROM_MEMADDR'= $(CFG_AP_ROM_MEMADDR) ; \
	echo 'CFG_MD1_ROM_MEMADDR'= $(CFG_MD1_ROM_MEMADDR) ; \
	echo 'CFG_MD1_RAMDISK_MEMADDR'= $(CFG_MD1_RAMDISK_MEMADDR) ; \
	echo 'CFG_MD2_ROM_MEMADDR'= $(CFG_MD2_ROM_MEMADDR) ; \
	echo 'CFG_MD2_RAMDISK_MEMADDR'= $(CFG_MD2_RAMDISK_MEMADDR) ; \
	echo 'CFG_CONN_SYS_MEMADDR'= $(CFG_CONN_SYS_MEMADDR) ; \
	echo 'ONEKEY_REBOOT_NORMAL_MODE_PL'= $(ONEKEY_REBOOT_NORMAL_MODE_PL) ; \
	echo 'KPD_USE_EXTEND_TYPE'= $(KPD_USE_EXTEND_TYPE) ; \
	echo 'KPD_PMIC_LPRST_TD'= $(KPD_PMIC_LPRST_TD) ; \
	echo 'MTK_PMIC_RST_KEY'= $(MTK_PMIC_RST_KEY) ; \
	echo 'CFG_ATF_SUPPORT'= $(CFG_ATF_SUPPORT) ; \
	echo 'CFG_ATF_LOG_SUPPORT'= $(CFG_ATF_LOG_SUPPORT) ; \
	echo 'CFG_TEE_SUPPORT'= $(CFG_TEE_SUPPORT) ; \
	echo 'CFG_TEE_VIRTUAL_RPMB_SUPPORT'= $(CFG_TEE_VIRTUAL_RPMB_SUPPORT) ; \
	echo 'CFG_RPMB_SET_KEY'= $(CFG_RPMB_SET_KEY) ; \
	echo 'CFG_TRUSTONIC_TEE_SUPPORT'= $(CFG_TRUSTONIC_TEE_SUPPORT) ; \
	echo 'CFG_MICROTRUST_TEE_SUPPORT'= $(CFG_MICROTRUST_TEE_SUPPORT) ; \
	echo 'CFG_MICROTRUST_TEE_LITE_SUPPORT'= $(CFG_MICROTRUST_TEE_LITE_SUPPORT) ; \
	echo 'CFG_TRUSTKERNEL_TEE_SUPPORT'= $(CFG_TRUSTKERNEL_TEE_SUPPORT) ; \
	echo 'CFG_TEE_SECURE_MEM_PROTECTED'= $(CFG_TEE_SECURE_MEM_PROTECTED) ; \
	echo 'CFG_TEE_SECURE_MEM_TAG_FORMAT'= $(CFG_TEE_SECURE_MEM_TAG_FORMAT) ; \
	echo 'CFG_TEE_SECURE_MEM_SHARED'= $(CFG_TEE_SECURE_MEM_SHARED) ; \
	echo 'CFG_TEE_TRUSTED_APP_HEAP_SIZE'= $(CFG_TEE_TRUSTED_APP_HEAP_SIZE) ; \
	echo 'CFG_RPMB_KEY_PROGRAMED_IN_KERNEL'= $(CFG_RPMB_KEY_PROGRAMED_IN_KERNEL) ; \
	echo 'MTK_EFUSE_WRITER_SUPPORT'= $(MTK_EFUSE_WRITER_SUPPORT) ; \
	echo 'CFG_GOOGLE_TRUSTY_SUPPORT'= $(CFG_GOOGLE_TRUSTY_SUPPORT) ; \
	echo 'CFG_GZ_SUPPORT'= $(CFG_GZ_SUPPORT) ; \
	echo 'CFG_GZ_NEED_DESCRAMBLE'= $(CFG_GZ_NEED_DESCRAMBLE) ; \
	echo 'CFG_GZ_REMAP'= $(CFG_GZ_REMAP) ; \
	echo 'CFG_NEBULA_VM_SUPPORT'= $(CFG_NEBULA_VM_SUPPORT) ; \
	echo 'CFG_NEBULA_LOAD_IN_PART2'= $(CFG_NEBULA_LOAD_IN_PART2) ; \
	echo 'CFG_GZ_PWRAP_ENABLE'= $(CFG_GZ_PWRAP_ENABLE) ; \
	fi
##############################################################
# Adding Build Time
#

build_info: $(PRELOADER_OUT)/inc/preloader.h
$(PRELOADER_OUT)/inc/preloader.h: $(ALL_DEPENDENCY_FILE)
	@mkdir -p $(dir $@)
	@echo // Auto generated. Build Time Information > $@
	@echo '#'define BUILD_TIME '"'$(shell date +%Y%m%d-%H%M%S)'"' >> $@

##############################################################
# Adding Project Configuration
#

project_info: $(PRELOADER_OUT)/inc/proj_cfg.h
$(PRELOADER_OUT)/inc/proj_cfg.h: $(MTK_PATH_PLATFORM)/default.mak $(MTK_PATH_CUSTOM)/cust_bldr.mak $(PROJECT_CONFIGS)
$(PRELOADER_OUT)/inc/proj_cfg.h: $(ALL_DEPENDENCY_FILE)
	@mkdir -p $(dir $@)
	@echo // Auto generated. Import ProjectConfig.mk > $@
	@echo '#'define CUSTOM_SUSBDL_CFG $(MTK_SEC_USBDL) >> $@
	@echo '#'define CUSTOM_SBOOT_CFG $(MTK_SEC_BOOT) >> $@
	@echo '#'define MTK_SEC_MODEM_AUTH $(MTK_SEC_MODEM_AUTH) >> $@
ifdef MTK_SEC_SECRO_AC_SUPPORT
	@echo '#'define MTK_SEC_SECRO_AC_SUPPORT $(MTK_SEC_SECRO_AC_SUPPORT) >> $@
endif

##############################################################
# Generate Key Info File
#

img_auth_info: $(IMAGE_AUTH_CFG_FILE)
$(IMAGE_AUTH_CFG_FILE): $(SIGN_TOOL) $(IMAGE_AUTH_KEY) $(IMAGE_AUTH_CFG)
$(IMAGE_AUTH_CFG_FILE): $(ALL_DEPENDENCY_FILE)
	@mkdir -p $(dir $@)
	@touch $@
	@chmod 777 $@
	$(SIGN_TOOL) $(IMAGE_AUTH_KEY) $(IMAGE_AUTH_CFG) $@ IMG

##############################################################
# Clean
#

clean:
	@rm -rf $(PRELOADER_OUT)/obj
	@rm -rf $(PRELOADER_OUT)/inc
	@rm -rf $(PRELOADER_OUT)/bin
	@rm -rf $(PRELOADER_OUT)/out_crypto
	@rm -rf $(PRELOADER_OUT)/out_plat
	@rm -rf $(PRELOADER_OUT)/out_devinfo
	@rm -rf $(PRELOADER_OUT)/out_devinfo_debug
	@rm -rf $(PRELOADER_OUT)/out_hw_crypto_lib

#
# Encode Key
#
ifeq ($(HOST_OS),darwin)
KEY_ENCODE_TOOL := tools/ke/KeyEncode.darwin
else
KEY_ENCODE_TOOL := tools/ke/KeyEncode
endif

#key_encode
$(D_BIN)/KEY_ENCODED_PL: $(D_BIN)/$(PL_IMAGE_NAME)_LINKED.bin | $(KEY_ENCODE_TOOL)
	$(KEY_ENCODE_TOOL) $< $@
	@echo "$(KEY_ENCODE_TOOL) pass !!!!"

ifdef SEC_ROM_INFO_V2
$(D_BIN)/$(PL_IMAGE_NAME)_NO_GFH.bin: $(D_BIN)/$(PL_IMAGE_NAME)_LINKED.bin
	@echo "[ Load Chip Config. ]"
	@echo "============================================"
	@echo " : MTK_SEC_CHIP_SUPPORT - $(MTK_SEC_CHIP_SUPPORT)"
	cp -f $< $@
else
$(D_BIN)/$(PL_IMAGE_NAME)_NO_GFH.bin: $(D_BIN)/KEY_ENCODED_PL
	@echo "[ Load Chip Config. ]"
	@echo "============================================"
	@echo " : MTK_SEC_CHIP_SUPPORT - $(MTK_SEC_CHIP_SUPPORT)"
	cp -f $< $@
endif

ifneq ($(wildcard $(CONFIG_PATH)/PBP_PY_SUPPORT),)
include bl_sign_v21.mak
else
ifneq ($(wildcard $(GFH_PATH)/GFH_CONFIG.ini),)
include bl_sign_v11.mak
else
include bl_sign_v10.mak
endif
endif

dump_build_info: $(D_BIN)/$(PL_IMAGE_NAME)_LINKED.bin $(D_BIN)/$(PL_IMAGE_NAME).elf
	@echo "============================================"
	@echo "${MTK_PROJECT} preloader load"
	@echo "$(D_BIN)/$(PL_IMAGE_NAME)_LINKED.bin built at"
	@echo "time : $(shell date)"
	@echo "img  size : `wc -c < $(D_BIN)/$(PL_IMAGE_NAME)_LINKED.bin` byte"
	@echo "bss  size : 0x`$(CROSS_COMPILE)readelf -SW $(D_BIN)/$(PL_IMAGE_NAME).elf | grep bss | awk '{if (NF==11) print $$6; else print $$7;}'` byte"
	@echo "dram size : 0x`$(CROSS_COMPILE)readelf -SW $(D_BIN)/$(PL_IMAGE_NAME).elf | grep dram | awk '{if (NF==11) print $$6; else print $$7;}'` byte"
	@echo ============================================

