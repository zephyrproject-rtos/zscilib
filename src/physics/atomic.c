/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <zsl/zsl.h>
#include <zsl/physics/atomic.h>

int
zsl_phy_atom_nucl_radius(uint8_t a, zsl_real_t *r)
{
	if (a == 0) {
		*r = NAN;
		return -EINVAL;
	}

	*r = ZSL_NUCL_RADIUS * ZSL_POW((zsl_real_t) a, 1.0 / 3.0) * 1E15;

	return 0;
}

int
zsl_phy_atom_bohr_orb_radius(uint8_t z, uint8_t n, zsl_real_t *r)
{
	if (z == 0) {
		*r = NAN;
		return -EINVAL;
	}

#ifdef CONFIG_ZSL_SINGLE_PRECISION
	/*
	 * Numbers get too small for single percision so multiply a large
	 * constant in to the numerator and denomerator to prevent numbers
	 * from going to 0.0f
	 */
	*r = ((zsl_real_t) n * (zsl_real_t) n * ZSL_RED_PLANCK * 1E30 * 
	     ZSL_RED_PLANCK * 1E9) / ((zsl_real_t) z * ZSL_COULOMB *
	     ZSL_E_CHARGE * 1E30 * ZSL_E_CHARGE * ZSL_E_MASS);
#else
	*r = ((zsl_real_t) n * (zsl_real_t) n * ZSL_RED_PLANCK * 
	     ZSL_RED_PLANCK * 1E9) / ((zsl_real_t) z * ZSL_COULOMB *
	     ZSL_E_CHARGE * ZSL_E_CHARGE * ZSL_E_MASS);
#endif

	return 0;
}

int
zsl_phy_atom_bohr_orb_vel(uint8_t z, uint8_t n, zsl_real_t *v)
{
	if (z == 0) {
		*v = NAN;
		return -EINVAL;
	}

	*v = ((zsl_real_t) z * ZSL_COULOMB * ZSL_E_CHARGE * ZSL_E_CHARGE *
	1E-3) / ((zsl_real_t) n * ZSL_RED_PLANCK);

	return 0;
}

int
zsl_phy_atom_bohr_orb_ener(uint8_t z, uint8_t n, zsl_real_t *e)
{
	if (z == 0) {
		*e = NAN;
		return -EINVAL;
	}

	*e = -(ZSL_POW(((zsl_real_t) z * ZSL_COULOMB * ZSL_E_CHARGE *
	       ZSL_E_CHARGE) / ((zsl_real_t) n * ZSL_RED_PLANCK), 2.0) * 0.5 *
	       ZSL_E_MASS) / 1.602176634E-19;
	
	return 0;
}

int
zsl_phy_atom_rad_decay(zsl_real_t qi, zsl_real_t t, zsl_real_t lambda,
		       zsl_real_t *qf)
{
	if (t < 0 || lambda < 0) {
		*qf = NAN;
		return -EINVAL;
	}

	*qf = qi * ZSL_POW(ZSL_E, -lambda * t);

	return 0;
}

int
zsl_phy_atom_bragg(uint8_t n, zsl_real_t theta, zsl_real_t lambda,
		   zsl_real_t *d)
{
	if (lambda < 0 || sin(theta) <= 0) {
		*d = NAN;
		return -EINVAL;
	}

	*d = ((zsl_real_t) n * lambda) / (2.0 * ZSL_SIN(theta));

	return 0;
}
