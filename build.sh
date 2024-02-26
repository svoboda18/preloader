#!/bin/bash

# WARNING: build.sh is used only for fast standalone build, nolonger called from Android.mk
# WARNING: post_process is moved to Makefile for incremental build
# WARNING: parameters in build.sh and Android.mk must be identical

# tells sh to exit with a non-zero status as soon as any executed command fails
# (i.e. exits with a non-zero exit status).
set -e

if [ -z ${TARGET_PRODUCT} ]; then
  echo "[ERROR] TARGET_PRODUCT is not defined"
  exit 1
fi
if [ -z ${MTK_TARGET_PROJECT} ]; then 
  MTK_TARGET_PROJECT=${TARGET_PRODUCT/full_}
fi
if [ -z ${MTK_PROJECT} ]; then
  MTK_PROJECT=${MTK_TARGET_PROJECT}
fi
if [ -z ${PRELOADER_ROOT_DIR} ]; then
  if [ -z ${ANDROID_BUILD_TOP} ]; then
    PRELOADER_ROOT_DIR=`pwd`/../../..
  else
    PRELOADER_ROOT_DIR=${ANDROID_BUILD_TOP}
  fi
fi
if [ -z ${PRELOADER_OUT} ]; then
  if [ -z ${ANDROID_PRODUCT_OUT} ]; then
    PRELOADER_OUT=${PRELOADER_ROOT_DIR}/out/target/product/${MTK_TARGET_PROJECT}/obj/PRELOADER_OBJ
  else
    PRELOADER_OUT=${ANDROID_PRODUCT_OUT}/obj/PRELOADER_OBJ
  fi
fi
TOOL_PATH=${PRELOADER_ROOT_DIR}/device/mediatek/build/build/tools 

if [ "${HOST_OS}" = "" ]; then
HOST_OS=`uname | awk '{print tolower($0)}'`
fi

PRELOADER_CROSS_COMPILE=${PRELOADER_ROOT_DIR}/prebuilts/gcc/${HOST_OS}-x86/arm/arm-linux-androideabi-4.9/bin/arm-linux-androideabi-

echo Clean PRELOADER_OUT
rm -rf ${PRELOADER_OUT}
echo make -f Makefile PRELOADER_OUT=${PRELOADER_OUT} MTK_PROJECT=${MTK_PROJECT} TOOL_PATH=${TOOL_PATH} ROOTDIR=${PRELOADER_ROOT_DIR}
make -f Makefile CROSS_COMPILE=${PRELOADER_CROSS_COMPILE} PRELOADER_OUT=${PRELOADER_OUT} MTK_PROJECT=${MTK_PROJECT} TOOL_PATH=${TOOL_PATH} ROOTDIR=${PRELOADER_ROOT_DIR}

if [ -f "../../../../internal/sboot_disable" ]; then
  if [ -z ${ANDROID_PRODUCT_OUT} ]; then
    PRELOADER_OUT=${PRELOADER_ROOT_DIR}/out/target/product/${MTK_TARGET_PROJECT}/obj/PRELOADER_SBOOT_DIS_OBJ
  else
    PRELOADER_OUT=${ANDROID_PRODUCT_OUT}/obj/PRELOADER_SBOOT_DIS_OBJ
  fi

echo Clean PRELOADER_OUT
rm -rf ${PRELOADER_OUT}
echo make -f Makefile PRELOADER_OUT=${PRELOADER_OUT} MTK_PROJECT=${MTK_PROJECT} TOOL_PATH=${TOOL_PATH} ROOTDIR=${PRELOADER_ROOT_DIR}
make -f Makefile CROSS_COMPILE=${PRELOADER_CROSS_COMPILE} PL_MODE=SBOOT_DIS PRELOADER_OUT=${PRELOADER_OUT} MTK_PROJECT=${MTK_PROJECT} TOOL_PATH=${TOOL_PATH} ROOTDIR=${PRELOADER_ROOT_DIR}
fi
