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
	Hal_io.c \
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
	mt_ptp.c \

ifeq ("$(PMIC_CHIP)","MT6355")
	MOD_SRC += pmic_wrap_init_v1.c
	MOD_SRC += pmic_mt6355.c
	MOD_SRC += regulator_core.c
	MOD_SRC += regulator.c
	MOD_SRC += charging_bat_mt6355.c
	MOD_SRC += rtc_mt6355.c
	MOD_SRC += battery.c
else
	MOD_SRC += pmic_wrap_init.c
	MOD_SRC += pmic.c
	MOD_SRC += mt6311.c
	MOD_SRC += fan53526.c
	MOD_SRC += fan53528buc08x.c
	MOD_SRC += charging_bat.c
	MOD_SRC += rtc.c
endif

ifeq ("$(PMIC_CHIP)","MT6355")
	MOD_SRC += dramc_pi_basic_api.c
	MOD_SRC += dramc_pi_calibration_api.c
	MOD_SRC += dramc_pi_main.c
else
	MOD_SRC += dramc_pi_basic_api_n.c
	MOD_SRC += dramc_pi_calibration_api_n.c
	MOD_SRC += dramc_pi_main_n.c
endif

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

ifeq ("$(RT5081_PMU)","yes")
	MOD_SRC += rt5081.c
endif

###################################################################
# target
###################################################################

include $(COMMON_FILE_MK)

###################################################################
# trigger emigen
###################################################################
$(MOD_OBJ)/emi.o: $(EMIGEN_H_OUT)/custom_emi.h
