#ifndef __GENERIC_REGULATOR_MTK_REGULATOR_PLATFORM_H_
#define __GENERIC_REGULATOR_MTK_REGULATOR_PLATFORM_H_

/*
 * This file defines platform dependent functions
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <debug.h>
#include <ostimer.h>
#include "FreeRTOS.h"

typedef unsigned int mreg_reg_t;
typedef struct mutex mreg_mutex_t;

#define MTK_REGULATOR_SUSPEND_OPS
#define MTK_REGULATOR_EXT_OPS
#define MTK_REGULATOR_MODE_OPS
#define MTK_REGULATOR_GET_EXCLUSIVE
#define MTK_REGULATOR_FORCE_DISABLE

#ifdef CFG_MTK_TINYSYS_SSPM_SUPPORT /* SSPM MODE */
#define MTK_REGULATOR_FRAMEWORK_DEBUG 0

#if MTK_REGULATOR_FRAMEWORK_DEBUG
#define mreg_dbg_printf(fmt, ...) PRINTF_D(fmt, ##__VA_ARGS__)
#else
#define mreg_dbg_printf(...) do { } while(0)
#endif
#endif /* CFG_MTK_TINYSYS_SSPM_SUPPORT */

#define mreg_mutex_t char
#define mreg_mutex_init(lock)
#define mreg_mutex_lock(lock)
#define mreg_mutex_unlock(lock)
#define mreg_mutex_destroy(lock)
#define MREG_DEFINE_MUTEX(mutex_name)
#define mreg_mdelay(delay) mdelay(delay)
#define mreg_udelay(delay) udelay(delay)

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#endif /* __GENERIC_REGULATOR_MTK_REGULATOR_PLATFORM_H_ */
