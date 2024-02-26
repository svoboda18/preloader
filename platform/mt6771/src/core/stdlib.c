
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
}

int atoi(const char *s)
{
    static const char digits[] = "0123456789";  /* legal digits in order */
    unsigned val=0;         /* value we're accumulating */
    int neg=0;              /* set to true if we see a minus sign */

    /* skip whitespace */
    while (*s==' ' || *s=='\t') {
        s++;
    }

    /* check for sign */
    if (*s=='-') {
        neg=1;
        s++;
    } else if (*s=='+') {
        s++;
    }

    /* process each digit */
    while (*s) {
        const char *where;
        unsigned digit;

        /* look for the digit in the list of digits */
        where = strchr(digits, *s);
        if (where == 0) {
            /* not found; not a digit, so stop */
            break;
        }

        /* get the index into the digit list, which is the value */
        digit = (where - digits);

        /* could (should?) check for overflow here */

        /* shift the number over and add in the new digit */
        val = val*10 + digit;

        /* look at the next character */
        s++;
    }

    /* handle negative numbers */
    if (neg) {
        return -val;
    }

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
	if ((c >= '0') && (c <= '9')) {
		return c - '0';
	}

	if ((c >= 'a') && (c <= 'f')) {
		return c - 'a' + 10;
	}

	if ((c >= 'A') && (c <= 'F')) {
		return c - 'A' + 10;
	}
}

long long atoll(const char *num)
{
	long long value = 0;
	unsigned long long max;
	int neg = 0;

	if (num[0] == '0' && num[1] == 'x') {
		// hex
		num += 2;
		while (*num && isxdigit(*num)) {
			value = value * 16 + hexval(*num++);
		}
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

