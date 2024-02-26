# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.
#
# MediaTek Inc. (C) 2010. All rights reserved.
#
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
# RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
# AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
#
# The following software/firmware and/or related documentation ("MediaTek Software")
# have been modified by MediaTek Inc. All revisions are subject to any receiver's
# applicable license agreements with MediaTek Inc.


include $(MTK_PATH_PLATFORM)/makefile.mak

CURDIR_FLAG := $(PRELOADER_OUT)/obj/$(subst /,_,$(patsubst $(D_ROOT)/%,%,$(patsubst %/,%,$(CURDIR)))).flag

all: $(CURDIR_FLAG)
.PHONY: all


#########
# recursive
#
# $(1): subdir
# $(2): target flag
# $(3): dependency
define build-subdirs
$(2): $(3) FORCE
	$$(MAKE) -e -r -C $(1) --no-print-directory
endef

.PHONY: FORCE
FORCE:
SUBDIRS_FLAGS :=
$(foreach d,$(SUBDIRS),\
  $(eval r := $(patsubst $(D_ROOT)/%,%,$(patsubst %/,%,$(if $(filter /%,$(d)),,$(CURDIR)/)$(d))))\
  $(eval t := $(PRELOADER_OUT)/obj/$(subst /,_,$(r)).flag)\
  $(eval $(call build-subdirs,$(d),$(t),$(ALL_PREGEN_FILE)))\
  $(eval SUBDIRS_FLAGS += $(t))\
)

$(CURDIR_FLAG): $(SUBDIRS_FLAGS)

ifndef CURDIR_FLAG_RULE
CURDIR_FLAG_RULE := true
$(CURDIR_FLAG):
	@mkdir -p $(dir $@)
	echo "$^" > $@

endif
