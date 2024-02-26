ifndef EMIGEN_OUT
EMIGEN_OUT := $(PRELOADER_OUT)
endif
ifndef PERL
PERL := perl
endif
export EMIGEN_OUT

ALL_EMIGEN_FILE := \
	inc/custom_emi.h \
	MTK_Loader_Info.tag

ifeq ($(MTK_PLATFORM),MT2601)
ALL_EMIGEN_FILE += \
	custom_emi.c
endif

EMIGEN_FILE_LIST := $(addprefix $(EMIGEN_OUT)/,$(ALL_EMIGEN_FILE))
CUSTOM_MEMORY_HDR := $(MTK_PATH_CUSTOM)/inc/custom_MemoryDevice.h
ifeq ($(MACH_TYPE),mt6735m)
MEMORY_DEVICE_XLS := $(D_ROOT)/tools/emigen/MT6735/MemoryDeviceList_MT6735M.xls
else ifeq ($(MACH_TYPE),mt6753)
MEMORY_DEVICE_XLS := $(D_ROOT)/tools/emigen/MT6735/MemoryDeviceList_MT6753.xls
else ifeq ($(MACH_TYPE),mt6737t)
MEMORY_DEVICE_XLS := $(D_ROOT)/tools/emigen/MT6735/MemoryDeviceList_MT6737T.xls
else ifeq ($(MACH_TYPE),mt6737m)
MEMORY_DEVICE_XLS := $(D_ROOT)/tools/emigen/MT6735/MemoryDeviceList_MT6737M.xls
else
MEMORY_DEVICE_XLS := $(D_ROOT)/tools/emigen/$(MTK_PLATFORM)/MemoryDeviceList_$(MTK_PLATFORM).xls
endif
EMIGEN_SCRIPT := $(D_ROOT)/tools/emigen/$(MTK_PLATFORM)/emigen.pl
EMIGEN_PREBUILT_PATH := $(MTK_PATH_CUSTOM)
EMIGEN_PREBUILT_CHECK := $(filter-out $(wildcard $(addprefix $(EMIGEN_PREBUILT_PATH)/,$(ALL_EMIGEN_FILE))),$(addprefix $(EMIGEN_PREBUILT_PATH)/,$(ALL_EMIGEN_FILE)))

##############################################################
# Emigen generate parameter for header and tag files
#
BUILD_EMI_H := 0
BUILD_LOADER_TAG := 1
export EMIGEN_TAG_OUT := $(EMIGEN_OUT)
export EMIGEN_H_OUT := $(EMIGEN_OUT)/inc

.PHONY: emigen
emigen: $(EMIGEN_FILE_LIST)
ifneq ($(EMIGEN_PREBUILT_CHECK),)
$(EMIGEN_H_OUT)/custom_emi.h: $(EMIGEN_SCRIPT) $(CUSTOM_MEMORY_HDR) $(MEMORY_DEVICE_XLS)
	@mkdir -p $(dir $@)
	$(PERL) $(EMIGEN_SCRIPT) $(CUSTOM_MEMORY_HDR) $(MEMORY_DEVICE_XLS) $(MTK_PLATFORM) $(MTK_PROJECT) $(EMIGEN_H_OUT) $(BUILD_EMI_H) $(MACH_TYPE)
	echo "heeeeeeee loooooooooooooooooooooooooooooooooooooooooooo"

$(EMIGEN_TAG_OUT)/MTK_Loader_Info.tag: $(EMIGEN_SCRIPT) $(CUSTOM_MEMORY_HDR) $(MEMORY_DEVICE_XLS)
	@mkdir -p $(dir $@)
	$(PERL) $(EMIGEN_SCRIPT) $(CUSTOM_MEMORY_HDR) $(MEMORY_DEVICE_XLS) $(MTK_PLATFORM) $(MTK_PROJECT) $(EMIGEN_TAG_OUT) $(BUILD_LOADER_TAG) $(MACH_TYPE)

else
$(EMIGEN_FILE_LIST): $(EMIGEN_OUT)/% : $(EMIGEN_PREBUILT_PATH)/%
	@mkdir -p $(dir $@)
	cp -f $< $@
endif
