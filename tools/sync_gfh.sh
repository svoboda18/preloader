#!/bin/bash

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

CUR_DIR=$(pwd)
STORAGE_TYPE=

cd ${CUR_DIR}/../custom

for d in * ; do
	if [ -d "$d" ]; then
		if [ -e  $d/cust_bldr.mak ]; then
			grep "CFG_BOOT_DEV\s*:=\s*BOOTDEV_UFS" $d/cust_bldr.mak > /dev/null
			if [ $? -eq "0" ]; then
				STORAGE_TYPE="ufs"
			else
				grep "MTK_EMMC_SUPPORT" $d/cust_bldr.mak > /dev/null
				# storage type is determined within cust_bldr.mak
				if [ ! $? -eq "0" ]; then
					grep "CFG_BOOT_DEV\s*:=\s*BOOTDEV_SDMMC" $d/cust_bldr.mak > /dev/null
					if [ $? -eq "0" ]; then
						STORAGE_TYPE="emmc"
					fi
					grep "CFG_BOOT_DEV\s*:=\s*BOOTDEV_NAND" $d/cust_bldr.mak > /dev/null
					if [ $? -eq "0" ]; then
						STORAGE_TYPE="nand"
					fi
				else
					# used MTK_EMMC_SUPPORT to determine storage type
					if [ -e $d/$d.mk ]; then
						grep "MTK_EMMC_SUPPORT\s*=\s*yes" $d/$d.mk > /dev/null
						if [ $? -eq "0" ]; then
							STORAGE_TYPE="emmc"
						else
							STORAGE_TYPE="nand"
						fi
					else
						# default storage type is emmc
						STORAGE_TYPE="emmc"
					fi
				fi
			fi
			GFH_LEGACY_PATH=$d/security/chip_config/s/gfh/GFH_CONFIG.ini
			GFH_PSS_PATH=$d/security/chip_config/s/gfh/pl_gfh_config_pss.ini
			GFH_CERT_CHAIN_PATH=$d/security/chip_config/s/gfh/pl_gfh_config_cert_chain.ini
			# process legacy GFH
			if [ -e ${GFH_LEGACY_PATH} ]; then
				sed -ri "s/(flash_dev\s+=)[^=]*$/\1 \"${STORAGE_TYPE}\"/" ${GFH_LEGACY_PATH}
			fi
			# process pss GFH
			if [ -e ${GFH_PSS_PATH} ]; then
				sed -ri "s/(flash_dev\s+=)[^=]*$/\1 \"${STORAGE_TYPE}\"/" ${GFH_PSS_PATH}
			fi
			# process cert chain GFH
			if [ -e ${GFH_CERT_CHAIN_PATH} ]; then
				sed -ri "s/(flash_dev\s+=)[^=]*$/\1 \"${STORAGE_TYPE}\"/" ${GFH_CERT_CHAIN_PATH}
			fi
		fi
	fi
done

cd ${CUR_DIR}
