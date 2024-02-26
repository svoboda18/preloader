/*
 * Copyright (c) 2015-2016 MICROTRUST Incorporated
 * All rights reserved
 *
 * This file and software is confidential and proprietary to MICROTRUST Inc.
 * Unauthorized copying of this file and software is strictly prohibited.
 * You MUST NOT disclose this file and software unless you get a license
 * agreement from MICROTRUST Incorporated.
 */

#ifndef __TZ_PLATFORM_TEEI_H__
#define __TZ_PLATFORM_TEEI_H__

/* TEEI Magic For Interface */
#define TEEI_BOOTCFG_MAGIC (0x434d4254)

/* TEEI argument version */
#define TEE_ARGUMENT_VERSION (0x00010000U)
#define MTK_ID "mtkInc"
#define TEE01_ID "tee_01"
#define TEE02_ID "tee_02"
#define TEE03_ID "tee_03"
#define TEE04_ID "tee_04"
#define TEE05_ID "tee_05"
#define TEE06_ID "tee_06"
#define TEE07_ID "tee_07"
#define TEE08_ID "tee_08"
#define TEE09_ID "tee_09"
#define TEE0A_ID "tee_0A"

#define GIC_BASE	(0x10000000 + 0x00211000)
#define GIC_CPU		(0x10000000 + 0x00212000)
#define GIC_VERSION	0x02

#define MT_UART0_IRQ (91 + 32)
#define MT_UART1_IRQ (92 + 32)
#define MT_UART2_IRQ (93 + 32)
#define MT_UART3_IRQ (94 + 32)

#define MT_UART0_DAPC (83)
#define MT_UART1_DAPC (84)
#define MT_UART2_DAPC (85)
#define MT_UART3_DAPC (86)

#define MT_SEC_GPT_BASE 0x1000a000
#define MT_SEC_GPT_IRQ (196 + 32)
#define MT_SEC_GPT_DAPC (10)

#define MT_SEC_WDT_BASE 0x1000a000
#define MT_SEC_WDT_IRQ (169)
#define MT_SEC_WDT_DAPC (10)

#define TEE_FCH_IRQ_NO  (161 + 32)       /* 193 */
#define TEE_DRV_IRQ_NO  (162 + 32)       /* 194 */
#define TEE_REV_IRQ_NO  (163 + 32)       /* 195 */
#define TEE_DUMPER_IRQ_NO  (0xffffffff)  /* skip */
#define TEEI_SGPT_32K_FREQ
/*
 * Crypto module info:
 * For base address, MT6753 series uses different address, others are the same
 * For interrupt number, MT6797 uses different IRQ numbers, others are the same
 */
#if defined(MACH_TYPE_MT6753)
#define MT_SEC_CRYPTO_BASE 0x10216000
#else
#define MT_SEC_CRYPTO_BASE 0x10210000
#endif
#define MT_SEC_CRYPTO_IRQ (0)
#define MT_SEC_CRYPTO_DAPC (0)

#endif
