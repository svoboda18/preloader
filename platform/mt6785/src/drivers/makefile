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
	clkbuf_ctl.c \
	rtc.c \
	cust_msdc.c \
	msdc_io.c \
	i2c.c \
	keypad.c \
	usbd.c \
	usbphy.c \
	usbtty.c \
	timer.c \
	platform.c \
	pll.c \
	device_apc.c \
	gpio.c \
	pmic_wrap_init.c \
	pmic.c \
	pmic_efuse.c \
	pmic_initial_setting.c \
	mtk_regulator_core.c \
	mt6359.c \
	charging_bat.c \
	battery.c \
	memory.c \
	memory_test.s \
	sync_write.c \
	spm_mtcmos.c \
	hotplug.s \
	mt_drcc.c \
	lastpc.c \
	plat_debug.c \
	mtk_eem.c \
	mtk_vcore_opp.c \
	ufs_aio_platform.c \
	mtk_subpmic.c


ifeq ("$(CFG_BOOT_DEV)", "BOOTDEV_NAND")
	MOD_SRC  += nand.c
	MOD_SRC  += bmt.c
endif

ifeq ("$(CFG_APWDT)","PLATFORM")
	MOD_SRC  += wdt.c
endif

ifeq ("$(PL_MODE)","DUMMY_AP")
	MOD_SRC  += gpio_init.c
endif

ifeq ("$(PL_MODE)","TINY")
	MOD_SRC  += gpio_init.c
endif

ifeq ("$(MT6370_PMU)", "yes")
	MOD_SRC	+= mt6370.c
endif

ifeq ("$(MT6360_PMU)","yes")
	MOD_SRC += mt6360.c
endif

ifeq ("$(MT6360_PMIC)","yes")
	MOD_SRC += mt6360_pmic.c
endif

ifeq ("$(MT6360_LDO)","yes")
	MOD_SRC += mt6360_ldo.c
endif

ifeq ("$(CFG_CV_SUPPORT)", "yes")
	MOD_SRC	+= cv_emi.c
	MOD_SRC	+= cv_dramc_pi_main.c
	MOD_SRC	+= cv_dramc_pi_ddr_reserve.c
	MOD_SRC	+= cv_dramc_pi_basic_api.c
	MOD_SRC	+= cv_dramc_pi_calibration_api.c
	MOD_SRC	+= cv_Hal_io.c
	MOD_SRC += cv_rtc.c
else
	MOD_SRC	+= emi.c
	MOD_SRC	+= dramc_pi_main.c
	MOD_SRC	+= dramc_pi_ddr_reserve.c
	MOD_SRC	+= dramc_pi_basic_api.c
	MOD_SRC	+= dramc_pi_calibration_api.c
	MOD_SRC	+= Hal_io.c
	MOD_SRC += dramc_actiming.c
	MOD_SRC += dramc_debug.c
	MOD_SRC += dramc_dvfs.c
	MOD_SRC += dramc_lowpower.c
	MOD_SRC += dramc_tracking.c
	MOD_SRC += dramc_unit_test.c
	MOD_SRC += dramc_utility.c
	MOD_SRC += dramc_slt.c
	MOD_SRC += rtc.c
endif

ifeq ("$(CFG_GZ_SUPPORT)", "1")
ifeq ("$(CFG_GZ_REMAP)", "1")
	MOD_SRC	+= gz_remap_mt.c
endif
endif

###################################################################
# target
###################################################################

include $(COMMON_FILE_MK)

###################################################################
# trigger emigen
###################################################################
$(MOD_OBJ)/emi.o: $(EMIGEN_H_OUT)/custom_emi.h
