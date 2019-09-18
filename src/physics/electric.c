/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <zsl/zsl.h>
#include <zsl/physics/electric.h>

int
zsl_phy_elec_charge_dens(zsl_real_t q, zsl_real_t v, zsl_real_t *d)
{
	if (v <= 0) {
		*d = NAN;
		return -EINVAL;
	}

	*d = q / v;

	return 0;
}

int
zsl_phy_elec_force(zsl_real_t q1, zsl_real_t q2, zsl_real_t r, zsl_real_t *f)
{
	if (r <= 0) {
		*f = NAN;
		return -EINVAL;
	}

	*f = (ZSL_COULOMB * q1 * q2) / (r * r);

	return 0;
}

int
zsl_phy_elec_force2(zsl_real_t q, zsl_real_t e, zsl_real_t *f)
{
	*f = q * e;

	return 0;
}

int
zsl_phy_elec_field(zsl_real_t q, zsl_real_t r, zsl_real_t *e)
{
	if (r <= 0) {
		*e = NAN;
		return -EINVAL;
	}

	*e = (ZSL_COULOMB * q) / (r * r);

	return 0;
}

int
zsl_phy_elec_pot_ener(zsl_real_t q1, zsl_real_t q2, zsl_real_t r, zsl_real_t *u)
{
	if (r <= 0) {
		*u = NAN;
		return -EINVAL;
	}

	*u = (ZSL_COULOMB * q1 * q2) / r;

	return 0;
}

int
zsl_phy_elec_potential(zsl_real_t q, zsl_real_t r, zsl_real_t *v)
{
	if (r <= 0) {
		*v = NAN;
		return -EINVAL;
	}

	*v = (ZSL_COULOMB * q) / r;

	return 0;
}

int
zsl_phy_elec_flux(zsl_real_t e, zsl_real_t a, zsl_real_t theta, zsl_real_t *fl)
{
	if (e < 0 || a < 0) {
		*fl = NAN;
		return -EINVAL;
	}

	*fl = e * a * ZSL_COS(theta);

	return 0;
}
