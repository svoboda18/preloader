/*
 * Copyright (c) 2015-2016 MICROTRUST Incorporated
 * All rights reserved
 *
 * This file and software is confidential and proprietary to MICROTRUST Inc.
 * Unauthorized copying of this file and software is strictly prohibited.
 * You MUST NOT disclose this file and software unless you get a license
 * agreement from MICROTRUST Incorporated.
 */
#ifndef _ISEE_IMSG_LOG_H_
#define _ISEE_IMSG_LOG_H_

/*************************************************************************/
/* Common used definition                                                */
/*************************************************************************/
#ifndef IMSG_TAG
#define IMSG_TAG "[ISEE]"
#endif

#ifdef TARGET_BUILD_VARIANT_ENG
#define IMSG_DEBUG_BUILD      1
#else
#define IMSG_RELEASE_BUILD    1
#endif

static inline unsigned long now_ms(void)
{
	return get_timer(0);
}
#if defined MICRO_PLATFORM_VERSION_MAJOR && MICRO_PLATFORM_VERSION_MAJOR == 8
#include<pal_log.h>

#define IMSG_PRINT(LEVEL, fmt, ...) \
	do { \
		pal_log_info("%s[%s]: " fmt, IMSG_TAG, #LEVEL, ##__VA_ARGS__); \
	} while(0)

#define IMSG_PRINT_INFO(LEVEL, fmt, ...) \
	do { \
		pal_log_info("%s[%s]: " fmt, IMSG_TAG, #LEVEL, ##__VA_ARGS__); \
	} while(0)

#define IMSG_PRINT_DBG(LEVEL, fmt, ...) \
	do { \
		pal_log_debug("%s[%s]: " fmt, IMSG_TAG, #LEVEL, ##__VA_ARGS__); \
	} while(0)

#define IMSG_PRINT_ERR(LEVEL, fmt, ...) \
	do { \
		pal_log_err("%s[%s]: " fmt, IMSG_TAG, #LEVEL, ##__VA_ARGS__); \
	} while(0)

#define IMSG_PRINT_WARN(LEVEL, fmt, ...) \
	do { \
		pal_log_warn("%s[%s]: " fmt, IMSG_TAG, #LEVEL, ##__VA_ARGS__); \
	} while(0)

#define IMSG_PRINT_TIME_S(fmt, ...) \
	unsigned long start_ms; \
do { \
	start_ms = now_ms(); \
	pal_log_info("%s[PROFILE]: " fmt "(start:%u)\n", IMSG_TAG, ##__VA_ARGS__, start_ms); \
} while(0)

#define IMSG_PRINT_TIME_E(fmt, ...) \
	do { \
		unsigned long end_ms, delta_ms; \
		end_ms = now_ms(); \
		delta_ms = end_ms - start_ms; \
		pal_log_info("%s[PROFILE]: " fmt "(end:%u, spend:%u ms)\n", IMSG_TAG, ##__VA_ARGS__, end_ms, delta_ms); \
	} while(0)
#else
#define IMSG_PRINT(LEVEL, fmt, ...) \
	do { \
		print("%s[%s]: " fmt, IMSG_TAG, #LEVEL, ##__VA_ARGS__); \
	} while(0)
#define IMSG_PRINT_INFO(LEVEL, fmt, ...) \
	do { \
		print("%s[%s]: " fmt, IMSG_TAG, #LEVEL, ##__VA_ARGS__); \
	} while(0)

#define IMSG_PRINT_DBG(LEVEL, fmt, ...) \
	do { \
		print("%s[%s]: " fmt, IMSG_TAG, #LEVEL, ##__VA_ARGS__); \
	} while(0)

#define IMSG_PRINT_ERR(LEVEL, fmt, ...) \
	do { \
		print("%s[%s]: " fmt, IMSG_TAG, #LEVEL, ##__VA_ARGS__); \
	} while(0)

#define IMSG_PRINT_WARN(LEVEL, fmt, ...) \
	do { \
		print("%s[%s]: " fmt, IMSG_TAG, #LEVEL, ##__VA_ARGS__); \
	} while(0)


#define IMSG_PRINT_TIME_S(fmt, ...) \
	unsigned long start_ms; \
do { \
	start_ms = now_ms(); \
	print("%s[PROFILE]: " fmt "(start:%u)\n", IMSG_TAG, ##__VA_ARGS__, start_ms); \
} while(0)

#define IMSG_PRINT_TIME_E(fmt, ...) \
	do { \
		unsigned long end_ms, delta_ms; \
		end_ms = now_ms(); \
		delta_ms = end_ms - start_ms; \
		print("%s[PROFILE]: " fmt "(end:%u, spend:%u ms)\n", IMSG_TAG, ##__VA_ARGS__, end_ms, delta_ms); \
	} while(0)
#endif
/*************************************************************************/
/* Declear macros                                                        */
/*************************************************************************/
#define IMSG_WARN(fmt, ...)         IMSG_PRINT_WARN(WARN, fmt, ##__VA_ARGS__)
#define IMSG_ERROR(fmt, ...)        IMSG_PRINT_ERR(ERROR, fmt, ##__VA_ARGS__)

#if IMSG_DEBUG_BUILD == 1
#define IMSG_INFO(fmt, ...)         IMSG_PRINT_INFO(INFO, fmt, ##__VA_ARGS__)
#define IMSG_DEBUG(fmt, ...)        IMSG_PRINT_DBG(DEBUG, fmt, ##__VA_ARGS__)
#define IMSG_TRACE(fmt, ...)        IMSG_PRINT_DBG(TRACE, fmt, ##__VA_ARGS__)
#define IMSG_ENTER(fmt, ...)        IMSG_PRINT_DBG(ENTER, "%s " fmt, __FUNCTION__)
#define IMSG_LEAVE(fmt, ...)        IMSG_PRINT_DBG(LEAVE, "%s " fmt, __FUNCTION__)
#define IMSG_PROFILE_S(fmt, ...)    IMSG_PRINT_TIME_S(fmt, ##__VA_ARGS__)
#define IMSG_PROFILE_E(fmt, ...)    IMSG_PRINT_TIME_E(fmt, ##__VA_ARGS__)
#else
#define IMSG_INFO(fmt, ...)         do { } while(0)
#define IMSG_DEBUG(fmt, ...)        do { } while(0)
#define IMSG_TRACE(fmt, ...)        do { } while(0)
#define IMSG_ENTER()                do { } while(0)
#define IMSG_LEAVE()                do { } while(0)
#define IMSG_PROFILE_S(fmt, ...)    do { } while(0)
#define IMSG_PROFILE_E(fmt, ...)    do { } while(0)
#endif

#endif //_ISEE_IMSG_LOG_H_
