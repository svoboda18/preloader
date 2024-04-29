###################################################################
# Include Files Directery
###################################################################

#include $(D_ROOT)/mtk_cust.mak

###################################################################
# Using GCC
###################################################################

CROSS_COMPILE ?= arm-linux-androideabi-

AS	= $(CROSS_COMPILE)as
LD	= $(CROSS_COMPILE)ld
CC	= $(CROSS_COMPILE)gcc
CPP	= $(CC)-E
AR	= $(CROSS_COMPILE)ar
NM	= $(CROSS_COMPILE)nm
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
RANLIB	= $(CROSS_COMPILE)RANLIB
THUMB_MODE = TRUE


###################################################################
# Initialize GCC Compile Parameter
###################################################################
DEFINE           = -D$(MTK_PLATFORM)
OBJCFLAGS 	 = --gap-fill=0xff
AFLAGS_DEBUG 	 = -Wa,-gstabs,
STRIP_SYMBOL	 = -fdata-sections -ffunction-sections

INCLUDE_FILE = $(INCLUDE_FILE_COMMON)
INCLUDE_FILE += \
    -I$(EMIGEN_OUT)/inc \
    -I$(MTK_PATH_CUSTOM)/inc \
    -I$(D_ROOT)/custom/common/inc \
    -I$(D_ROOT)/inc/$(_CHIP) \
    -I$(MTK_ROOT_CUSTOM)/$(TARGET)/common \
    -I$(MTK_ROOT_CUSTOM)/kernel/dct \
    -I$(MTK_ROOT_OUT)/PTGEN/common \
    -I$(MTK_ROOT_OUT)/NANDGEN/common \
    -I$(MTK_PATH_PLATFORM)/src/drivers/inc/ptgen

ifeq ($(CFG_MICROTRUST_TEE_SUPPORT),1)
INCLUDE_FILE    += -I$(MTK_PATH_PLATFORM)/src/security/trustzone/kdflib/inc
endif
###################################################################
# GCC Compile Options
###################################################################

C_OPTION	 := -Os -march=armv7-a -mtune=cortex-a7 -fdata-sections -ffunction-sections -fno-strict-aliasing -fno-common -ffixed-r8 -fno-builtin -ffreestanding -pipe -mno-thumb-interwork -Wstrict-prototypes $(DEFINE) -c $(INCLUDE_FILE) -msoft-float -D__ASSEMBLY__  -DPRELOADER_HEAP -mno-unaligned-access
C_OPTION_OPTIMIZE	 := -Os -march=armv7-a -mtune=cortex-a7 -fdata-sections -ffunction-sections -fno-strict-aliasing -fno-common -ffixed-r8 -fno-builtin -ffreestanding -pipe -mno-thumb-interwork -Wstrict-prototypes $(DEFINE) -c $(INCLUDE_FILE) -msoft-float -D__ASSEMBLY__  -DPRELOADER_HEAP -mno-unaligned-access
AFLAGS 		 := -c -mcpu=cortex-a7
AFLAGS_OPTIMIZE	 := -c -mcpu=cortex-a7

ifeq ($(THUMB_MODE),TRUE)
#thumb
C_OPTION            += -mthumb-interwork -mthumb
C_OPTION_OPTIMIZE   += -mthumb-interwork -mthumb
else
C_OPTION	    += -mno-thumb-interwork
C_OPTION_OPTIMIZE   += -mno-thumb-interwork
endif

#priority: mode > project config > cust > default
include $(MTK_PATH_PLATFORM)/default.mak
include ${MTK_ROOT_CUSTOM}/${TARGET}/cust_bldr.mak
PROJECT_CONFIGS := $(MTK_ROOT_CUSTOM)/$(MTK_PROJECT)/$(MTK_PROJECT).mk
include $(PROJECT_CONFIGS)

ifdef PL_MODE
C_OPTION += -D$(PL_MODE)
C_OPTION_OPTIMIZE += -D$(PL_MODE)
include $(MTK_PATH_PLATFORM)/mode/$(PL_MODE).mak
endif

ifeq ("$(MTK_SECURITY_SW_SUPPORT)","yes")
	C_OPTION += -DMTK_SECURITY_SW_SUPPORT
endif

ifeq ($(strip $(TARGET_BUILD_VARIANT)),eng)
    C_OPTION += -DTARGET_BUILD_VARIANT_ENG
endif

ifeq ("$(MTK_EMMC_SUPPORT)","yes")
    C_OPTION += -DMTK_EMMC_SUPPORT
endif

include $(MTK_PATH_PLATFORM)/feature.mak

MTK_CDEFS := $(PL_MTK_CDEFS)
MTK_ADEFS := $(PL_MTK_ADEFS)

# make unmark for PL booting time profiling
#C_OPTION += -DPL_PROFILING
C_OPTION += $(MTK_CFLAGS) $(MTK_CDEFS) $(MTK_INC)
AFLAGS   += $(MTK_AFLAGS)

ifeq ("$(CFG_FUNCTION_TRACER_SUPPORT)", "1")
C_OPTION += -finstrument-functions
endif

###################################################################
# gcc link descriptor
###################################################################

ifeq ($(findstring fpga, $(TARGET)),fpga)
LDSCRIPT	:= $(MTK_PATH_PLATFORM)/link_descriptor_fpga.ld
else
LDSCRIPT	:= $(MTK_PATH_PLATFORM)/link_descriptor.ld
endif

LINKFILE	:= $(LD)
LINK		:= $(LINKFILE) -Bstatic -T $(LDSCRIPT) --gc-sections

###################################################################
# Object File
###################################################################

export All_OBJS
