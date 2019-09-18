/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <zsl/zsl.h>
#include <zsl/physics/energy.h>

int
zsl_phy_ener_kin(zsl_real_t v, zsl_real_t m, zsl_real_t *ke)
{
	if (m < 0) {
		*ke = NAN;
		return -EINVAL;
	}

	*ke = 0.5 * m * v * v;

	return 0;
}

int
zsl_phy_ener_rot(zsl_real_t omega, zsl_real_t i, zsl_real_t *rke)
{
	if (i < 0) {
		*rke = NAN;
		return -EINVAL;
	}

	*rke = 0.5 * i * omega * omega;

	return 0;
}

int
zsl_phy_ener_grav_pot(zsl_real_t m, zsl_real_t h, zsl_real_t *ug)
{
	if (m < 0) {
		*ug = NAN;
		return -EINVAL;
	}

	*ug = m * ZSL_GRAV_EARTH * h;

	return 0;
}

int
zsl_phy_ener_elas_pot(zsl_real_t k, zsl_real_t dist, zsl_real_t *ue)
{
	if (k < 0) {
		*ue = NAN;
		return -EINVAL;
	}

	*ue = 0.5 * k * dist * dist;

	return 0;
}

int
zsl_phy_ener_power(zsl_real_t ener, zsl_real_t time, zsl_real_t *power)
{
	if (time <= 0) {
		*power = NAN;
		return -EINVAL;
	}

	*power = ener / time;

	return 0;
}

int
zsl_phy_ener_fric(zsl_real_t fric, zsl_real_t dist, zsl_real_t *e)
{
	if (dist < 0 || fric < 0) {
		*e = NAN;
		return -EINVAL;
	}

	*e = -fric * dist;

	return 0;
}

int
zsl_phy_ener_mec(zsl_real_t ke, zsl_real_t rke, zsl_real_t ug, zsl_real_t ue,
		 zsl_real_t *me)
{
	*me = ke + rke + ug + ue;

	return 0;
}

int
zsl_phy_ener_final(zsl_real_t ei1, zsl_real_t ei2, zsl_real_t ei3,
		   zsl_real_t ei4, zsl_real_t ef1, zsl_real_t ef2,
		   zsl_real_t ef3, zsl_real_t *ef4)
{
	/* According to the conservation of mechanical energy, the equation
	 * ei1 + ei2 + ei3 + ei4 = ef1 + ef2 + ef3 + ef4 must be always true. */
	*ef4 = ei1 + ei2 + ei3 + ei4 - ef1 - ef2 - ef3;

	return 0;
}

int
zsl_phy_ener_photon(zsl_real_t f, zsl_real_t *e)
{
	if (f < 0) {
		*e = NAN;
		return -EINVAL;
	}

	*e = (ZSL_PLANCK * f) / 1.602176634E-19;

	return 0;
}

int
zsl_phy_ener_photon2(zsl_real_t lambda, zsl_real_t *e)
{
	if (lambda <= 0) {
		*e = NAN;
		return -EINVAL;
	}

        /* Scale lamba from nm to m. */
        lambda *= 1E-9;

	*e = (ZSL_PLANCK * ZSL_LIGHT_SPEED) / (lambda * 1.602176634E-19);

	return 0;
}
