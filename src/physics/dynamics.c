/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <kernel.h>
#include <zsl/zsl.h>
#include <zsl/physics/dynamics.h>

int
zsl_phy_dyn_newton(zsl_real_t m, zsl_real_t accel, zsl_real_t *f)
{
	if (m < 0) {
		*f = NAN;
		return -EINVAL;
	}

	*f = m * accel;

	return 0;
}

int
zsl_phy_dyn_fric(zsl_real_t n, zsl_real_t mu, zsl_real_t *f)
{
	if (mu < 0 || mu > 1) {
		*f = NAN;
		return -EINVAL;
	}

	*f = n * mu;

	return 0;
}

int
zsl_phy_dyn_normal(zsl_real_t m, zsl_real_t theta, zsl_real_t *n)
{
	if (m < 0) {
		*n = NAN;
		return -EINVAL;
	}

	*n = m * ZSL_GRAV_EARTH * cos(theta);

	return 0;
}
