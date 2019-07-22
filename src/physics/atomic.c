/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <kernel.h>
#include <zsl/zsl.h>
#include <zsl/physics/atomic.h>

int
zsl_phy_atom_nucl_radius(u8_t a, zsl_real_t *r)
{
        if (!a) {
                *r = NAN;
                return -EINVAL;
        }

	*r = ZSL_NUCL_RADIUS * pow((zsl_real_t)a, 1.0 / 3.0);

	return 0;
}

int
zsl_phy_atom_bohr_orb_radius(zsl_real_t Z, zsl_real_t n, zsl_real_t *r)
{
	if (Z <= 0 || n <= 0) {
		*r = NAN;
		return -EINVAL;
	}

	*r = (n * n * ZSL_RED_PLANCK * ZSL_RED_PLANCK) /
	     (Z * ZSL_COULOMB *
	      ZSL_E_CHARGE * ZSL_E_CHARGE * ZSL_E_MASS);


	return 0;
}

int
zsl_phy_atom_bohr_orb_vel(zsl_real_t Z, zsl_real_t n, zsl_real_t *v)
{
	if (Z <= 0 || n <= 0) {
		*v = NAN;
		return -EINVAL;
	}

	*v = (Z * ZSL_COULOMB * ZSL_E_CHARGE * ZSL_E_CHARGE) /
	     (n * ZSL_RED_PLANCK);

	return 0;
}

int
zsl_phy_atom_bohr_orb_ener(zsl_real_t Z, zsl_real_t n, zsl_real_t *e)
{
	if (Z <= 0 || n <= 0) {
		*e = NAN;
		return -EINVAL;
	}

	*e = -pow((Z * ZSL_COULOMB * ZSL_E_CHARGE * ZSL_E_CHARGE) /
		  (n * ZSL_RED_PLANCK), 2.0) * 0.5 * ZSL_E_MASS;

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

	*qf = qi * pow(ZSL_E, -lambda * t);

	return 0;
}

int
zsl_phy_atom_bragg(zsl_real_t n, zsl_real_t theta, zsl_real_t lambda,
		   zsl_real_t *d)
{
	if (d < 0 || lambda < 0 || sin(theta) <= 0) {
		*d = NAN;
		return -EINVAL;
	}

	*d = (n * lambda) / (2.0 * sin(theta));

	return 0;
}
