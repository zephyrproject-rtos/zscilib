/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <zsl/zsl.h>
#include <zsl/physics/thermo.h>

int
zsl_phy_thermo_fahren_cels(zsl_real_t t1, zsl_real_t *t2, bool ctf)
{
	if (ctf == true) {
		*t2 = ((9 * t1) / 5) + 32;
		return 0;
	}
	
	*t2 = ((t1 - 32) * 5) / 9;

	return 0;
}

int
zsl_phy_thermo_cels_kel(zsl_real_t t1, zsl_real_t *t2, bool ktc)
{
	if (ktc == true) {
		*t2 = t1 - 273.15;
		return 0;
	}
	
	*t2 = t1 + 273.15;

	return 0;
}

int
zsl_phy_thermo_heat_fusion(zsl_real_t m, zsl_real_t lh, zsl_real_t *q)
{
	if (m < 0 || lh < 0) {
		*q = NAN;
		return -EINVAL;
	}

	*q = m * lh;

	return 0;
}

int
zsl_phy_thermo_heat(zsl_real_t m, zsl_real_t sh, zsl_real_t t, zsl_real_t *q)
{
	if (m < 0 || sh < 0) {
		*q = NAN;
		return -EINVAL;
	}

	*q = m * sh * t;

	return 0;
}

int
zsl_phy_thermo_expan(zsl_real_t l0, zsl_real_t a, zsl_real_t t, zsl_real_t *l)
{
	if (l0 < 0 || a < 0) {
		*l = NAN;
		return -EINVAL;
	}

	*l = l0 + l0 * a * t;

	return 0;
}

int
zsl_phy_thermo_mean_free_path(zsl_real_t nv, zsl_real_t d, zsl_real_t *lambda)
{
	if (nv <= 0 || d <= 0) {
		*lambda = NAN;
		return -EINVAL;
	}

	*lambda = 1.0E6 / (ZSL_PI * ZSL_SQRT(2.0) * nv * d * d);

	return 0;
}

int
zsl_phy_thermo_effic_heat_engine(zsl_real_t qc, zsl_real_t qh, zsl_real_t *e)
{
	if (qh == 0 || qc > qh) {
		*e = NAN;
		return -EINVAL;
	}

	*e = 1.0 - qc / qh;

	return 0;
}

int
zsl_phy_thermo_carnot_engine(zsl_real_t tc, zsl_real_t qc, zsl_real_t th,
			     zsl_real_t *qh)
{
	if (tc <= 0 || th < tc ) {
		*qh = NAN;
		return -EINVAL;
	}

	*qh = (qc * th) / tc;

	return 0;
}
