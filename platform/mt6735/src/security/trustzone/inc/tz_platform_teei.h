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
#define GIC_VERSION  (0x02)
/*#define GIC_BASE 0x10221000*/
/*#define GIC_CPU 0x10222000*/

#define GIC_BASE	GICD_BASE

#if GIC_VERSION == 0x02
#define GIC_CPU		GICC_BASE
#else
#define GIC_RBASE	GICR_BASE
#endif

/* UART setting */
#define MT_UART0_IRQ (91+32)
#define MT_UART1_IRQ (92+32)
#define MT_UART2_IRQ (93+32)
#define MT_UART3_IRQ (94+32)

#define MT_UART0_DAPC (46)
#define MT_UART1_DAPC (47)
#define MT_UART2_DAPC (48)
#define MT_UART3_DAPC (49)

/* Secure Timer setting (SEJ) */
#define MT_SEC_GPT_BASE 0x10008000
#define MT_SEC_GPT_IRQ (173+32)
#define MT_SEC_GPT_DAPC (8)

/* Secure watch Dog setting (SEJ) */
#define MT_SEC_WDT_BASE 0x10008000
#define MT_SEC_WDT_IRQ (174+32)
#define MT_SEC_WDT_DAPC (8)

#define TEE_FCH_IRQ_NO 286
#define TEE_DRV_IRQ_NO 279
#define TEE_REV_IRQ_NO 280
#define TEE_DUMPER_IRQ_NO 282

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
