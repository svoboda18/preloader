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
	msdc_io.c \
	cust_msdc.c \
	i2c.c \
	keypad.c \
	usbd.c \
	usbd_ext.c \
	usbphy.c \
	usbtty.c \
	emi.c \
	emi_init.c \
	dramc_dqs_gw.c  \
	dramc_dle_cal.c \
	dramc_calib.c \
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
	hotplug.s \

ifeq ("$(PMIC_CHIP)","MT6356")
	MOD_SRC += pmic_wrap_init.c
	MOD_SRC += pmic_mt6356.c
	MOD_SRC += pmic_efuse_mt6356.c
	MOD_SRC += pmic_initial_setting_mt6356.c
	MOD_SRC += mtk_regulator_core.c
	MOD_SRC += mt6356.c
	MOD_SRC += charging_bat_mt6356.c
	MOD_SRC += battery_mt6356.c
	MOD_SRC += rtc_mt6356.c
else ifeq ("$(PMIC_CHIP)","MT6357")
	MOD_SRC += pmic_wrap_init_v1.c
	MOD_SRC += pmic_mt6357.c
	MOD_SRC += pmic_efuse_mt6357.c
	MOD_SRC += pmic_initial_setting_mt6357.c
	MOD_SRC += mtk_regulator_core.c
	MOD_SRC += mt6357.c
	MOD_SRC += charging_bat.c
	MOD_SRC += rtc.c
	MOD_SRC += battery.c
else
	MOD_SRC += pmic.c
endif

ifeq ("$(CFG_BOOT_DEV)", "BOOTDEV_UFS")
	MOD_SRC  += ufs_aio_platform.c
endif


ifeq ("$(CFG_BOOT_DEV)", "BOOTDEV_NAND")
	MOD_SRC  += nand.c
	MOD_SRC  += bmt.c
	MOD_SRC  += partition_nand.c
endif


ifeq ("$(PL_MODE)","DUMMY_AP")
	MOD_SRC  += gpio_init.c
endif

ifeq ("$(PL_MODE)","TINY")
	MOD_SRC  += gpio_init.c
endif

ifeq ("$(CFG_FUNCTION_PICACHU_SUPPORT)", "1")
	MOD_SRC	+= etc.c
endif

ifeq ("$(MTK_PTP)", "yes")
	MOD_SRC += mt_ptp.c
endif

###################################################################
# target
###################################################################

include $(COMMON_FILE_MK)

###################################################################
# trigger emigen
###################################################################
$(MOD_OBJ)/emi.o: $(EMIGEN_H_OUT)/custom_emi.h
