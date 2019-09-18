/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <zsl/zsl.h>
#include <zsl/physics/fluids.h>

int
zsl_phy_fluid_dens(zsl_real_t m, zsl_real_t v, zsl_real_t *d)
{
	if (m < 0 || v <= 0) {
		*d = NAN;
		return -EINVAL;
	}

	*d = m / v;

	return 0;
}

int
zsl_phy_fluid_simple_press(zsl_real_t f, zsl_real_t a, zsl_real_t *p)
{
	if (f < 0 || a <= 0) {
		*p = NAN;
		return -EINVAL;
	}

	*p = f / a;

	return 0;
}

int
zsl_phy_fluid_press(zsl_real_t p0, zsl_real_t h, zsl_real_t d, zsl_real_t *p)
{
	if (p0 < 0 || h < 0 || d < 0) {
		*p = NAN;
		return -EINVAL;
	}

	*p = p0 + h * d * ZSL_GRAV_EARTH * 1.0E-3;

	return 0;
}

int
zsl_phy_fluid_bouyant_force(zsl_real_t v, zsl_real_t d, zsl_real_t *f)
{
	if (v < 0 || d < 0) {
		*f = NAN;
		return -EINVAL;
	}

	*f = v * d * ZSL_GRAV_EARTH * 1.0E-3;

	return 0;
}

int
zsl_phy_fluid_flow_rate(zsl_real_t a1, zsl_real_t v1, zsl_real_t a2,
			zsl_real_t *v2)
{
	if (v1 < 0 || a1 < 0 || a2 <= 0) {
		*v2 = NAN;
		return -EINVAL;
	}

	*v2 = (a1 * v1) / a2;

	return 0;
}

int
zsl_phy_fluid_force_prop(zsl_real_t a1, zsl_real_t f1, zsl_real_t a2,
			 zsl_real_t *f2)
{
	if (f1 < 0 || a2 < 0 || a1 <= 0) {
		*f2 = NAN;
		return -EINVAL;
	}

	*f2 = (a2 * f1) / a1;

	return 0;
}

int
zsl_phy_fluid_bernoulli(zsl_real_t d, zsl_real_t v1, zsl_real_t p1,
			zsl_real_t v2, zsl_real_t *p2)
{
	if (v1 < 0 || v2 < 0 || p1 < 0 || d < 0) {
		*p2 = NAN;
		return -EINVAL;
	}
	
	/* Bernoulli's principle: 
	 * p1 + 0.5 * d * v1 * v1 = p2 + 0.5 * d * v2 * v2
	 */

	*p2 = p1 + 0.5 * d * (v1 * v1 - v2 * v2) * 1.0E-3;

	return 0;
}

int
zsl_phy_fluid_volume_flow_rate(zsl_real_t vel, zsl_real_t a, zsl_real_t *v)
{
	if (vel < 0 || a < 0) {
		*v = NAN;
		return -EINVAL;
	}
	
	*v = vel * a;

	return 0;
}
