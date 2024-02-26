ifneq (,$(findstring cert_chain,$(PADDING_TYPE)))
$(D_BIN)/$(PL_IMAGE_NAME).bin: $(GFH_PATH)/pl_gfh_config_cert_chain.ini
endif
ifneq (,$(findstring pss,$(PADDING_TYPE)))
$(D_BIN)/$(PL_IMAGE_NAME).bin: $(GFH_PATH)/pl_gfh_config_pss.ini
endif
ifneq (,$(findstring legacy,$(PADDING_TYPE)))
$(D_BIN)/$(PL_IMAGE_NAME).bin: $(GFH_PATH)/GFH_CONFIG.ini
endif
$(D_BIN)/$(PL_IMAGE_NAME).bin: $(D_BIN)/$(PL_IMAGE_NAME)_NO_GFH.bin $(PBP_TOOL) $(CHIP_KEY)
	@echo "[ Enable Secure Chip Support ]"
	@echo "============================================"
	@echo "PBP_PY_SUPPORT=YES"
	cp -f $< $@
	@echo "PBP_PATH = " $(PBP_TOOL)
ifneq (,$(findstring cert_chain,$(PADDING_TYPE)))
	@echo "padding = cert_chain"
	PYTHONDONTWRITEBYTECODE=True python $(PBP_TOOL) -i $(CHIP_KEY) -k ${PRELOADER_OUT}/key_cert.bin -g $(GFH_PATH)/pl_gfh_config_cert_chain.ini -c ${KEY_PATH}/pl_content.ini -func sign -o $@  $(D_BIN)/$(PL_IMAGE_NAME)_NO_GFH.bin
endif
ifneq (,$(findstring pss,$(PADDING_TYPE)))
	@echo "padding = pss"
	PYTHONDONTWRITEBYTECODE=True python $(PBP_TOOL) -i $(CHIP_KEY) -g $(GFH_PATH)/pl_gfh_config_pss.ini -func sign -o $@  $(D_BIN)/$(PL_IMAGE_NAME)_NO_GFH.bin
endif
ifneq (,$(findstring legacy,$(PADDING_TYPE)))
	@echo "padding = legacy"
	PYTHONDONTWRITEBYTECODE=True python $(PBP_TOOL) -i $(CHIP_KEY) -g $(GFH_PATH)/GFH_CONFIG.ini -func sign -o $@  $(D_BIN)/$(PL_IMAGE_NAME)_NO_GFH.bin
endif
	@echo "$(PBP_TOOL) pass !!!!"
	@PL_IMAGE=$(D_BIN)/$(PL_IMAGE_NAME)_LINKED.bin PL_FUN_MAP=$(PRELOADER_OUT)/function.map PRELOADER_OUT=$(PRELOADER_OUT) $(SHELL) $(MTK_PATH_PLATFORM)/check_size.sh
