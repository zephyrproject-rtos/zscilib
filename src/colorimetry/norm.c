/*
 * Copyright (c) 2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <zsl/colorimetry.h>

int zsl_clr_norm_spd(struct zsl_clr_spd *spd)
{
	zsl_real_t max = 0.0;

	/* Determine the max value. */
	for (size_t i = 0; i < spd->size; i++) {
		if (max < spd->comps[i].value) {
			max = spd->comps[i].value;
		}
	}

	/* Avoid divide by zero. */
	if (max == 0.0) {
		return -EINVAL;
	}

	/* Scale values by max. */
	for (size_t i = 0; i < spd->size; i++) {
		spd->comps[i].value /= max;
	}

	return 0;
}
