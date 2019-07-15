/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <kernel.h>
#include <zsl/zsl.h>
#include <zsl/physics/projectiles.h>

int
zsl_phy_proj_init_vel(zsl_real_t vi, zsl_real_t theta, zsl_real_t *vih,
		      zsl_real_t *viv)
{
	*vih = vi * cos(theta);
	*viv = vi * sin(theta);

	return 0;
}

int
zsl_phy_proj_time(zsl_real_t viv, zsl_real_t yi, zsl_real_t yf,
		  zsl_real_t *t)
{
	zsl_real_t disc = viv * viv + 2 * ZSL_GRAV_EARTH * (yi - yf);

	if (disc < 0) {
		*t = NAN;
		return -EINVAL;
	}

	*t = (viv + sqrt(disc)) / ZSL_GRAV_EARTH;

	return 0;
}

int zsl_phy_proj_time_first(zsl_real_t viv, zsl_real_t yi, zsl_real_t yf,
			    zsl_real_t *t)
{
	zsl_real_t disc = viv * viv + 2 * ZSL_GRAV_EARTH * (yi - yf);

	if (disc < 0) {
		*t = NAN;
		return -EINVAL;
	}

	*t = (viv - sqrt(disc)) / ZSL_GRAV_EARTH;

	if (*t < 0) {
		*t = NAN;
		return -EINVAL;
	}

	return 0;
}

int
zsl_phy_proj_time2(zsl_real_t viv, zsl_real_t vfv, zsl_real_t *t)
{
	*t = (viv - vfv) / ZSL_GRAV_EARTH;

	if (*t < 0) {
		*t = NAN;
		return -EINVAL;
	}

	return 0;
}
