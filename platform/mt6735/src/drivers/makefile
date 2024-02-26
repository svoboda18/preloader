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
	dramc_dqs_gw.c  \
	dramc_dle_cal.c \
	dramc_calib.c \
	rtc.c \
	mmc_common_inter.c \
	mmc_core.c \
	mmc_rpmb.c \
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
	clkbuf_ctl.c

ifeq ("$(MACH_TYPE)","mt6735")
	MOD_SRC  += pll.c
	MOD_SRC  += mt_ptp.c
endif
ifeq ("$(MACH_TYPE)","mt6735m")
	MOD_SRC  += pll2.c
	MOD_SRC  += mt_ptp.c
endif
ifeq ("$(MACH_TYPE)","mt6753")
	MOD_SRC  += pll3.c
	MOD_SRC  += mt_ptp_d3.c
endif

ifeq ("$(MACH_TYPE)","mt6737t")
	MOD_SRC  += pll.c
	MOD_SRC  += mt_ptp.c
endif
ifeq ("$(MACH_TYPE)","mt6737m")
	MOD_SRC  += pll2.c
	MOD_SRC  += mt_ptp.c
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

ifeq ("$(MTK_MT8193_SUPPORT)","yes")
	MOD_SRC  += mt8193_init.c
	MOD_SRC  += mt8193_ckgen.c
	MOD_SRC  += mt8193_i2c.c
endif

ifeq ("$(MTK_NCP1854_SUPPORT)","yes")
	MOD_SRC  += ncp1854.c
endif

###################################################################
# target
###################################################################

include $(COMMON_FILE_MK)

###################################################################
# trigger emigen
###################################################################
$(MOD_OBJ)/emi.o: $(EMIGEN_H_OUT)/custom_emi.h
