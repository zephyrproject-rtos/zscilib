/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <zsl/zsl.h>
#include <zsl/physics/gases.h>

int
zsl_phy_gas_av_vel(zsl_real_t m, zsl_real_t n, zsl_real_t t, zsl_real_t *v)
{
	if (m <= 0 || n < 0 || t < 0) {
		*v = NAN;
		return -EINVAL;
	}

	*v = ZSL_SQRT((3.0 * n * ZSL_IDEAL_GAS_CONST * t) / m);

	return 0;
}

int
zsl_phy_gas_press(zsl_real_t v, zsl_real_t n, zsl_real_t t, zsl_real_t *p)
{
	if (v <= 0 || n < 0 || t < 0) {
		*p = NAN;
		return -EINVAL;
	}

	*p = (n * ZSL_IDEAL_GAS_CONST * t) / v;

	return 0;
}

int
zsl_phy_gas_boyle(zsl_real_t pi, zsl_real_t vi, zsl_real_t pf, zsl_real_t *vf)
{
	if (pf <= 0 || vi < 0 || pi < 0) {
		*vf = NAN;
		return -EINVAL;
	}

	*vf = (vi * pi) / pf;

	return 0;
}

int
zsl_phy_gas_charles_lussac(zsl_real_t ti, zsl_real_t vi, zsl_real_t tf,
			   zsl_real_t *vf)
{
	if (ti <= 0 || vi < 0 || tf < 0) {
		*vf = NAN;
		return -EINVAL;
	}

	*vf = (vi * tf) / ti;

	return 0;
}
