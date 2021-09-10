/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ctype.h>
#include "floatcheck.h"

bool
val_is_equal(zsl_real_t a, zsl_real_t b, zsl_real_t epsilon)
{
	zsl_real_t c;

	c = a - b;

	if (c < epsilon && -c < epsilon) {
		return 1;
	} else {
		return 0;
	}
}

bool
val_is_at_least(zsl_real_t a, zsl_real_t b)
{
	return a >= b ? 1 : 0;
}

bool
val_is_less_than(zsl_real_t a, zsl_real_t b)
{
	return a < b ? 1 : 0;
}

bool
val_is_greater_than(zsl_real_t a, zsl_real_t b)
{
	return a > b ? 1 : 0;
}

bool
val_is_within(zsl_real_t a, zsl_real_t upper, zsl_real_t lower)
{
	if ((a <= upper) && (a >= lower)) {
		return 1;
	} else {
		return 0;
	}
}
