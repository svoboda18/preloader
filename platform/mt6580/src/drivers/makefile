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
	rtc.c \
	mmc_common_inter.c \
	mmc_core.c \
        mmc_rpmb.c \
	mmc_test.c \
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
 	pmic_wrap_init.c \
	pmic.c \
	mt6311.c \
	memory.c \
	memory_test.s \
	sync_write.c \
	spm_mtcmos.c \
	hotplug.s \
	clkbuf_ctl.c \
	emi_drv.c

ifeq ("$(MTK_FAN5405_SUPPORT)","yes")
	MOD_SRC  += fan5405.c 
endif

ifneq ("$(MTK_EMMC_SUPPORT)","yes")
ifneq ("$(MTK_TLC_NAND_SUPPORT)","yes")
	MOD_SRC  += nand.c
	MOD_SRC  +=	bmt.c
	MOD_SRC  +=	partition_mt.c
endif
ifeq ("$(MTK_TLC_NAND_SUPPORT)","yes")
	MOD_SRC  += nand_tlc.c
	MOD_SRC  +=	bmt_tlc.c
	MOD_SRC  +=	partition_tlc.c
endif
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
