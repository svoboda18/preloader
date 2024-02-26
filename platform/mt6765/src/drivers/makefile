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
	cust_msdc.c \
	msdc_io.c \
	i2c.c \
	keypad.c \
	usbd.c \
	usbphy.c \
	usbtty.c \
	timer.c \
	platform.c \
	lastpc.c \
	pll.c \
	device_apc.c \
	gpio.c \
	pmic_wrap_init.c \
	mtk_regulator_core.c \
	pmic.c \
	pmic_initial_setting.c \
	pmic_efuse.c \
	mt6357.c \
	mt6311.c \
	battery.c \
	memory.c \
	memory_test.s \
	sync_write.c \
	hotplug.s \
	plat_debug.c \
	clkbuf_ctl.c \
	rt5738.c \
	mtk_vcore_opp.c \
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

ifeq ("$(CFG_TB_SUPPORT)", "yes")
	MOD_SRC += tb_emi.c
	MOD_SRC += tb_dramc_pi_main.c
	MOD_SRC += tb_dramc_pi_ddr_reserve.c
	MOD_SRC += tb_dramc_pi_basic_api.c
	MOD_SRC += tb_dramc_pi_calibration_api.c
	MOD_SRC += tb_Hal_io.c
	MOD_SRC += mt_vcore.c
else
	MOD_SRC += emi.c
	MOD_SRC += dramc_pi_main.c
	MOD_SRC += dramc_pi_ddr_reserve.c
	MOD_SRC += dramc_pi_basic_api.c
	MOD_SRC += dramc_pi_calibration_api.c
	MOD_SRC += Hal_io.c
endif

ifeq ("$(CFG_GZ_SUPPORT)", "1")
ifeq ("$(CFG_GZ_REMAP)", "1")
	MOD_SRC += gz_remap_mt.c
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
