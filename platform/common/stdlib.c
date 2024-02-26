/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */

/* MediaTek Inc. (C) 2017. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver\'s
 * applicable license agreements with MediaTek Inc.
 */

#include "platform.h"

#ifndef NULL
#define NULL 0
#endif

char *strchr(const char *p, int ch)
{
	for (;; ++p) {
		if (*p == ch)
			return((char *)p);
		if (!*p)
			return((char *)NULL);
	}
	/* NOTREACHED */
	return NULL;
}

int atoi(const char *s)
{
	static const char digits[] = "0123456789";  /* legal digits in order */
	unsigned val = 0;         /* value we're accumulating */
	int neg = 0;              /* set to true if we see a minus sign */

	/* skip whitespace */
	while (*s == ' ' || *s == '\t')
		s++;

	/* check for sign */
	if (*s == '-') {
		neg = 1;
		s++;
	} else if (*s == '+')
		s++;

	/* process each digit */
	while (*s) {
		const char *where;
		unsigned digit;

		/* look for the digit in the list of digits */
		where = strchr(digits, *s);
		/* not found; not a digit, so stop */
		if (where == 0)
			break;

		/* get the index into the digit list, which is the value */
		digit = (where - digits);

		/* could (should?) check for overflow here */

		/* shift the number over and add in the new digit */
		val = val * 10 + digit;

		/* look at the next character */
		s++;
	}

	/* handle negative numbers */
	if (neg)
		return -val;

	/* done */
	return val;
}

int isdigit(char c)
{
	return ((c >= '0') && (c <= '9'));
}

int isxdigit(char c)
{
	return isdigit(c) || ((c >= 'a') && (c <= 'f'))
		|| ((c >= 'A') && (c <= 'F'));
}

int hexval(char c)
{
	if ((c >= '0') && (c <= '9'))
		return c - '0';

	if ((c >= 'a') && (c <= 'f'))
		return c - 'a' + 10;

	if ((c >= 'A') && (c <= 'F'))
		return c - 'A' + 10;

	/* if input is not hex value, return 0 */
	return 0;
}

long long atoll(const char *num)
{
	long long value = 0;
	unsigned long long max;
	int neg = 0;

	if (num[0] == '0' && num[1] == 'x') {
		// hex
		num += 2;
		while (*num && isxdigit(*num))
			value = value * 16 + hexval(*num++);
	} else {
		// decimal
		if (num[0] == '-') {
			neg = 1;
			num++;
		}
		while (*num && isdigit(*num))
			value = value * 10 + *num++  - '0';
	}

	if (neg)
		value = -value;

	max = value;
	return value;
}

void longjmperror(void)
{
	ASSERT(0);
}

