/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <zsl/zsl.h>
#include <zsl/physics/projectiles.h>

int
zsl_phy_proj_init_vel(zsl_real_t vi, zsl_real_t theta, zsl_real_t *vih,
		      zsl_real_t *viv)
{
	*vih = vi * ZSL_COS(theta);
	*viv = vi * ZSL_SIN(theta);

	return 0;
}

int
zsl_phy_proj_time(zsl_real_t viv, zsl_real_t yi, zsl_real_t yf,
		  zsl_real_t *t)
{
	zsl_real_t disc = viv * viv + 2.0 * ZSL_GRAV_EARTH * (yi - yf);

	if (disc < 0) {
		*t = NAN;
		return -EINVAL;
	}

	*t = (viv + ZSL_SQRT(disc)) / ZSL_GRAV_EARTH;

	return 0;
}

int zsl_phy_proj_time_first(zsl_real_t viv, zsl_real_t yi, zsl_real_t yf,
			    zsl_real_t *t)
{
	zsl_real_t disc = viv * viv + 2.0 * ZSL_GRAV_EARTH * (yi - yf);

	if (disc < 0) {
		*t = NAN;
		return -EINVAL;
	}

	*t = (viv - ZSL_SQRT(disc)) / ZSL_GRAV_EARTH;

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

int
zsl_phy_proj_ver_motion(zsl_real_t viv, zsl_real_t t, zsl_real_t yi,
			zsl_real_t *yf)
{
	if (t < 0) {
		*yf = NAN;
		return -EINVAL;
	}

	*yf = yi + viv * t - (ZSL_GRAV_EARTH * t * t) / 2.0;

	return 0;
}

int
zsl_phy_proj_ver_vel(zsl_real_t viv, zsl_real_t t, zsl_real_t *vfv)
{
	if (t < 0) {
		*vfv = NAN;
		return -EINVAL;
	}

	*vfv = viv - ZSL_GRAV_EARTH * t;

	return 0;
}

int
zsl_phy_proj_hor_motion(zsl_real_t vih, zsl_real_t t, zsl_real_t xi,
			zsl_real_t *xf)
{
	*xf = xi + vih * t;

	if (t < 0) {
		*xf = NAN;
		return -EINVAL;
	}

	return 0;
}

int
zsl_phy_proj_trajectory(zsl_real_t vih, zsl_real_t viv, zsl_real_t xi,
			zsl_real_t yi, zsl_real_t xf, zsl_real_t *yf)
{
	zsl_real_t t = (xf - xi) / vih;

	if (vih == 0 || t < 0) {
		*yf = NAN;
		return -EINVAL;
	}

	zsl_phy_proj_ver_motion(viv, t, yi, yf);

	return 0;
}

int
zsl_phy_proj_vel(zsl_real_t vfh, zsl_real_t vfv, zsl_real_t *vf)
{
	*vf = ZSL_SQRT(vfh * vfh + vfv * vfv);

	return 0;
}

int
zsl_phy_proj_angle(zsl_real_t vfh, zsl_real_t vfv, zsl_real_t *theta)
{
	if (vfh == 0) {
		*theta = ZSL_PI / 2.0;
		return 0;
	}

	*theta = ZSL_ATAN(vfv / vfh);

	return 0;
}

int
zsl_phy_proj_range(zsl_real_t vih, zsl_real_t viv, zsl_real_t xi,
		   zsl_real_t yi, zsl_real_t *dist)
{
	zsl_real_t t, xf;

	zsl_phy_proj_time(viv, yi, 0.0, &t);

	zsl_phy_proj_hor_motion(vih, t, xi, &xf);

	*dist = xf - xi;

	return 0;
}
