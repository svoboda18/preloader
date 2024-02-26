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
	timer.c \
	wdt.c \
	platform.c \
	lastpc.c \
	pll.c \
	device_apc.c \
	gpio.c \
	uart.c \
	pmic_wrap_init.c \
	mtk_regulator_core.c \
	pmic.c \
	pmic_initial_setting.c \
	pmic_efuse.c \
	mt6356.c \
	mt6311.c \
	battery.c \
	memory.c \
	memory_test.s \
	sync_write.c \
	spm_mtcmos.c \
	spm.c \
	hotplug.s \
	clkbuf_ctl.c \
	mt_ptp.c \
	rt5738.c \
	mt_vcore.c \
	mtk_subpmic.c

ifeq ("$(CFG_BOOT_DEV)", "BOOTDEV_UFS")
	MOD_SRC  += ufs_aio_platform.c
endif

ifeq ("$(PMIC_CHIP)","mt6353")
	MOD_SRC += pmic_wrap_init_v1.c
	MOD_SRC += pmic_mt6353.c
	MOD_SRC += charging_bat_mt6353.c
else
	MOD_SRC += charging_bat.c
endif
	MOD_SRC += rtc.c \

ifeq ("$(CFG_BOOT_DEV)", "BOOTDEV_NAND")
	MOD_SRC  += nand.c
	MOD_SRC  += bmt.c
endif

ifeq ("$(PL_MODE)","DUMMY_AP")
	MOD_SRC  += gpio_init.c
endif

ifeq ("$(PL_MODE)","TINY")
	MOD_SRC  += gpio_init.c
endif

ifeq ("$(MT6370_PMU)","yes")
	MOD_SRC += mt6370.c
endif

ifeq ("$(MTK_LP4_DRAM_SUPPORT)","yes")
	MOD_SRC += emi_lp4.c
	MOD_SRC += dramc_pi_basic_api_lp4.c
	MOD_SRC += dramc_pi_calibration_api_lp4.c
	MOD_SRC += dramc_pi_main_lp4.c
	MOD_SRC += Hal_io_lp4.c
else
	MOD_SRC += emi.c
	MOD_SRC += dramc_pi_basic_api.c
	MOD_SRC += dramc_pi_calibration_api.c
	MOD_SRC += dramc_pi_main.c
	MOD_SRC += Hal_io.c
endif

###################################################################
# target
###################################################################

include $(COMMON_FILE_MK)

###################################################################
# trigger emigen
###################################################################
$(MOD_OBJ)/emi.o: $(EMIGEN_H_OUT)/custom_emi.h
