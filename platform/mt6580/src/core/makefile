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
	download.c \
	handshake_uart.c \
	handshake_usb.c \
	main.c \
	print.c \
	dram_buffer.c
ifeq ("$(MTK_EMMC_SUPPORT)","no")
MOD_SRC += \
part.c
endif
ifeq ("$(MTK_EMMC_SUPPORT)","yes")
MOD_SRC += \
partition.c
endif
ifeq ("$(CFG_FUNCTION_TRACER_SUPPORT)", "1")
  MOD_SRC += function_tracer.c
endif

###################################################################
# target
###################################################################

include $(COMMON_FILE_MK)
