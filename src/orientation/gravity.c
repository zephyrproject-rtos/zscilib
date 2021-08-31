/*
 * Copyright (c) 2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <zsl/zsl.h>
#include <zsl/orientation/orientation.h>

int zsl_grav_lat_alt(zsl_real_t lat, zsl_real_t alt, zsl_real_t *g)
{
	if (alt < 0.0 || lat > 90.0 || lat < -90.0) {
		*g = NAN;
		return -EINVAL;
	}

	zsl_real_t d_to_r = ZSL_PI / 180.0;

	*g = 9.780318 * (1.0 + 0.0053024 * ZSL_SIN(lat * d_to_r) *
			 ZSL_SIN(lat * d_to_r) - 0.0000058 * ZSL_SIN(2.0 * lat * d_to_r) *
			 ZSL_SIN(2.0 * lat * d_to_r)) - 0.000003085 * alt;

	return 0;
}
