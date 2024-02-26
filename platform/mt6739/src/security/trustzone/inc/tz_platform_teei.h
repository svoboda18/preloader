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

/* GIC setting */
#define GIC_VERSION		0x03

#define GIC_BASE		GICD_BASE
#if GIC_VERSION == 0x02
#define GIC_CPU			GICC_BASE
#else
#define GIC_RBASE		GICR_BASE
#endif

/* UART setting */
#define MT_UART0_IRQ	(123)
#define MT_UART1_IRQ	(124)
#define MT_UART2_IRQ	(125)
#define MT_UART3_IRQ	(126)
#define MT_UART0_DAPC	(72)
#define MT_UART1_DAPC	(73)
#define MT_UART2_DAPC	(74)
#define MT_UART3_DAPC	(75)

/* Secure Timer setting (SEJ) */
#define MT_SEC_GPT_BASE	0x1000A000
#define MT_SEC_GPT_IRQ	(213)
#define MT_SEC_GPT_DAPC	(10)

/* Secure watch Dog setting (SEJ) */
#define MT_SEC_WDT_BASE	0x1000A000
#define MT_SEC_WDT_IRQ	(214)
#define MT_SEC_WDT_DAPC	(10)

#define TEE_REV_IRQ_NO		(165)
#define TEE_FCH_IRQ_NO		(175)
#define TEE_DRV_IRQ_NO		(256)
#define TEE_DUMPER_IRQ_NO	(257)

/*
 * Crypto module info:
 * For base address, MT6753 series uses different base address, others are the same
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
