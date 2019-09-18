/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
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

	*n = m * ZSL_GRAV_EARTH * ZSL_COS(theta);

	return 0;
}

int
zsl_phy_dyn_pendul_period(zsl_real_t l, zsl_real_t *t)
{
	if (l < 0) {
		*t = NAN;
		return -EINVAL;
	}

	*t = 2.0 * ZSL_PI * ZSL_SQRT(l / ZSL_GRAV_EARTH);

	return 0;
}

int
zsl_phy_dyn_pendul_max_speed(zsl_real_t l, zsl_real_t oi, zsl_real_t *vmax)
{
	if (l < 0) {
		*vmax = NAN;
		return -EINVAL;
	}

	*vmax = ZSL_SQRT(2.0 * ZSL_GRAV_EARTH * l * (1.0 - ZSL_COS(oi)));

	return 0;
}

int
zsl_phy_dyn_tension(zsl_real_t m, zsl_real_t a, zsl_real_t *t)
{
	if (m < 0) {
		*t = NAN;
		return -EINVAL;
	}

	*t = m * ZSL_GRAV_EARTH + m * a;

	return 0;
}

int
zsl_phy_dyn_lever(zsl_real_t d1, zsl_real_t f1, zsl_real_t d2, zsl_real_t *f2)
{
	if (d1 < 0 || d2 <= 0) {
		*f2 = NAN;
		return -EINVAL;
	}

	*f2 = (d1 * f1) / d2;

	return 0;
}
