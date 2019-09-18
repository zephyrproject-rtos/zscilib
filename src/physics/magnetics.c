/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <zsl/zsl.h>
#include <zsl/physics/magnetics.h>

int
zsl_phy_magn_force(zsl_real_t q, zsl_real_t v, zsl_real_t b, zsl_real_t theta,
		   zsl_real_t *f)
{
	if (v < 0 || b < 0) {
		*f = NAN;
		return -EINVAL;
	}

	*f = q * v * b * ZSL_SIN(theta);

	return 0;
}

int
zsl_phy_magn_force_wire(zsl_real_t i, zsl_real_t l, zsl_real_t b,
			zsl_real_t theta, zsl_real_t *f)
{
	if (l < 0 || b < 0) {
		*f = NAN;
		return -EINVAL;
	}

	*f = i * l * b * ZSL_SIN(theta);

	return 0;
}

int
zsl_phy_magn_torque_loop(zsl_real_t i, zsl_real_t a, zsl_real_t b,
			 zsl_real_t theta, zsl_real_t *tor)
{
	if (a < 0 || b < 0) {
		*tor = NAN;
		return -EINVAL;
	}

	*tor = i * a * b * ZSL_SIN(theta);

	return 0;
}

int
zsl_phy_magn_pot_dipole(zsl_real_t mom, zsl_real_t b, zsl_real_t theta,
			zsl_real_t *u)
{
	if (mom < 0 || b < 0) {
		*u = NAN;
		return -EINVAL;
	}

	*u = -mom * b * ZSL_COS(theta);

	return 0;
}

int
zsl_phy_magn_orb_radius(zsl_real_t q, zsl_real_t v, zsl_real_t b, zsl_real_t m,
			zsl_real_t *r)
{
	if (q == 0 || b == 0 || m < 0) {
		*r = NAN;
		return -EINVAL;
	}

	*r = (m * v) / (q * b);
	
	if (*r < 0) {
		*r = NAN;
		return -EINVAL;
	}

	return 0;
}

int
zsl_phy_magn_flux(zsl_real_t a, zsl_real_t b, zsl_real_t theta, zsl_real_t *fl)
{
	if (a < 0 || b < 0) {
		*fl = NAN;
		return -EINVAL;
	}

	*fl = a * b * ZSL_COS(theta);

	return 0;
}

int
zsl_phy_magn_mom(zsl_real_t i, zsl_real_t a, zsl_real_t *mom)
{
	if (a < 0) {
		*mom = NAN;
		return -EINVAL;
	}

	*mom = i * a;

	return 0;
}
