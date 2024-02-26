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
	circbuf.c \
	mmc_common_inter.c \
	mmc_core.c \
        mmc_rpmb.c \
	mmc_test.c \
	msdc_io.c \
	msdc.c \
	msdc_dma.c \
	msdc_irq.c \
	msdc_utils.c \
	i2c.c \
	keypad.c \
	usbd.c \
	usbphy.c \
	usbtty.c \
	emi.c \
	timer.c \
	wdt.c \
	platform.c \
	pll.c \
	device_apc.c \
	gpio.c \
	uart.c \
	memory.c \
	memory_test.s \
	sync_write.c \
	spm_mtcmos.c \
	hotplug.s \
	clkbuf_ctl.c \
	flashlight.c
ifeq ("$(PMIC_CHIP)","mt6353")
	MOD_SRC += pmic_wrap_init_v1.c
	MOD_SRC += pmic_mt6353.c
	MOD_SRC += mt6311.c
	MOD_SRC += charging_bat_mt6353.c
else
	MOD_SRC += pmic_wrap_init.c
	MOD_SRC += pmic.c
	MOD_SRC += mt6311.c
	MOD_SRC += charging_bat.c
endif
ifeq ("$(PMIC_CHIP)","mt6353")
	MOD_SRC += dramc_pi_basic_api_mt6750.c
	MOD_SRC += dramc_pi_calibration_api_mt6750.c
	MOD_SRC += dramc_pi_main_mt6750.c
else
	MOD_SRC += dramc_pi_basic_api_mt6755.c
	MOD_SRC += dramc_pi_calibration_api_mt6755.c
	MOD_SRC += dramc_pi_main_mt6755.c
endif
	MOD_SRC += rtc.c \

#ifneq ("$(MTK_EMMC_SUPPORT)","yes")
ifndef MTK_EMMC_SUPPORT
	MOD_SRC  += nand.c
	MOD_SRC  +=	bmt.c
endif

ifeq ("$(PL_MODE)","DUMMY_AP")
	MOD_SRC  += gpio_init.c
endif

ifeq ("$(PL_MODE)","TINY")
	MOD_SRC  += gpio_init.c
endif

###################################################################
# target
###################################################################

include $(COMMON_FILE_MK)

###################################################################
# trigger emigen
###################################################################
$(MOD_OBJ)/emi.o: $(EMIGEN_H_OUT)/custom_emi.h
