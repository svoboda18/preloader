#s_chip_legacy
ifeq ($(MTK_EMMC_SUPPORT),yes)
GFH_INFO := $(GFH_PATH)/GFH_INFO_EMMC.txt
else
GFH_INFO := $(GFH_PATH)/GFH_INFO.txt
endif
GFH_SEC_KEY := $(GFH_PATH)/GFH_SEC_KEY.txt
GFH_ANTI_CLONE := $(GFH_PATH)/GFH_ANTI_CLONE.txt
GFH_HASH_SIGNATURE := $(GFH_PATH)/GFH_HASH_AND_SIG.txt
GFH_PADDING := $(GFH_PATH)/GFH_PADDING.txt
include $(CONFIG_PATH)/SECURE_JTAG_CONFIG.ini
ifeq ($(strip $(SECURE_JTAG_ENABLE)),TRUE)
SECURE_JTAG_GFH := $(GFH_PATH)/GFH_SEC_CFG_JTAG_ON.txt
else ifeq ($(strip $(SECURE_JTAG_ENABLE)),FALSE)
SECURE_JTAG_GFH := $(GFH_PATH)/GFH_SEC_CFG_JTAG_OFF.txt
else
$(error BUILD FAIL. Unexpected SECURE_JTAG_ENABLE=$(SECURE_JTAG_ENABLE) in $(CONFIG_PATH)/SECURE_JTAG_CONFIG.ini)
endif

$(D_BIN)/$(PL_IMAGE_NAME).bin: $(D_BIN)/$(PL_IMAGE_NAME)_NO_GFH.bin $(GFH_INFO) $(GFH_SEC_KEY) $(GFH_ANTI_CLONE) $(SECURE_JTAG_GFH) $(GFH_PADDING) $(GFH_HASH_SIGNATURE) $(PBP_TOOL) $(CHIP_CONFIG) $(CHIP_KEY) $(CONFIG_PATH)/SECURE_JTAG_CONFIG.ini
	@echo "[ Enable Secure Chip Support ]"
	@echo "============================================"
	@echo "INI_GFH_GEN=NO"
	@echo " : SECURE_JTAG_ENABLE - $(SECURE_JTAG_ENABLE)"
	@echo "[ Attach $(MTK_PLATFORM) GFH ]"
	@echo "============================================"
	@echo " : GFH_INFO             - $(GFH_INFO)"
	@echo " : GFH_SEC_KEY          - $(GFH_SEC_KEY)"
	@echo " : GFH_ANTI_CLONE       - $(GFH_ANTI_CLONE)"
	@echo " : GFH_JTAG_CFG         - $(SECURE_JTAG_GFH)"
	@echo " : GFH_PADDING          - $(GFH_PADDING)"
	@echo " : GFH_HASH_SIGNATURE   - $(GFH_HASH_SIGNATURE)"
	cp -f $(GFH_INFO) $@
	@chmod 777 $@
	cat $(GFH_SEC_KEY) >> $@
	cat $(GFH_ANTI_CLONE) >> $@
	cat $(SECURE_JTAG_GFH) >> $@
	cat $(GFH_PADDING) >> $@
	cat $< >> $@
	cat $(GFH_HASH_SIGNATURE) >> $@
	@echo "[ Load Configuration ]"
	@echo "============================================"
	@echo " : CONFIG               - $(CHIP_CONFIG)"
	@echo " : RSA KEY              - $(CHIP_KEY)"
	@echo " : AC_K                 - $(CHIP_KEY)"
	$(PBP_TOOL) -m $(CHIP_CONFIG) -i $(CHIP_KEY) $@
	@echo "$(PBP_TOOL) pass !!!!"
	PL_IMAGE=$(D_BIN)/$(PL_IMAGE_NAME)_LINKED.bin PL_FUN_MAP=$(PRELOADER_OUT)/function.map PRELOADER_OUT=$(PRELOADER_OUT) $(SHELL) $(MTK_PATH_PLATFORM)/check_size.sh
