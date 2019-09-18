/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <zsl/zsl.h>
#include <zsl/physics/gravitation.h>

int
zsl_phy_grav_orb_period(zsl_real_t mp, zsl_real_t msat, zsl_real_t r,
			zsl_real_t *t)
{
	if ((mp + msat) == 0) {
		*t = NAN;
		return -EINVAL;
	}

	*t = ZSL_SQRT((4.0 * ZSL_PI * ZSL_PI * r * r * r) / (ZSL_UNIV_GRAV *
							    (mp + msat)));

	return 0;
}

int
zsl_phy_grav_esc_vel(zsl_real_t mp, zsl_real_t r, zsl_real_t *v)
{
	if (r == 0) {
		*v = NAN;
		return -EINVAL;
	}

	*v = ZSL_SQRT((2.0 * ZSL_UNIV_GRAV * mp) / r);

	return 0;
}

int
zsl_phy_grav_acc(zsl_real_t m, zsl_real_t r, zsl_real_t *a)
{
	if (r == 0) {
		*a = NAN;
		return -EINVAL;
	}

	*a = (-ZSL_UNIV_GRAV * m) / (r * r);

	return 0;
}

int
zsl_phy_grav_orb_vel(zsl_real_t mp, zsl_real_t r, zsl_real_t *v)
{
	if (r == 0) {
		*v = NAN;
		return -EINVAL;
	}

	*v = ZSL_SQRT((ZSL_UNIV_GRAV * mp) / r);

	return 0;
}

int
zsl_phy_grav_force(zsl_real_t m1, zsl_real_t m2, zsl_real_t r, zsl_real_t *f)
{
	if (r == 0) {
		*f = NAN;
		return -EINVAL;
	}

	*f = (-ZSL_UNIV_GRAV * m1 * m2) / (r * r);

	return 0;
}

int
zsl_phy_grav_pot_ener(zsl_real_t m1, zsl_real_t m2, zsl_real_t r,
		      zsl_real_t *u)
{
	if (r == 0) {
		*u = NAN;
		return -EINVAL;
	}

	*u = (-ZSL_UNIV_GRAV * m1 * m2) / (r * 1E3);

	return 0;
}
