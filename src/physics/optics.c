/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <zsl/zsl.h>
#include <zsl/physics/optics.h>

int
zsl_phy_opt_refrac_index(zsl_real_t v, zsl_real_t *n)
{
	if (v < 0 || v > ZSL_LIGHT_SPEED) {
		*n = NAN;
		return -EINVAL;
	}
	
	*n = v / ZSL_LIGHT_SPEED;

	return 0;
}

int
zsl_phy_opt_snell(zsl_real_t n1, zsl_real_t n2, zsl_real_t o1, zsl_real_t *o2)
{
	zsl_real_t o = (n1 * ZSL_SIN(o1)) / n2;
	
	if (n1 < 0 || n2 <= 0 || ZSL_ABS(o) > 1) {
		*o2 = NAN;
		return -EINVAL;
	}
	
	*o2 = ZSL_ASIN((n1 * ZSL_SIN(o1)) / n2);

	return 0;
}

int
zsl_phy_opt_focus(zsl_real_t sr, zsl_real_t si, zsl_real_t *f)
{
	if (sr + si == 0) {
		*f = NAN;
		return -EINVAL;
	}
	
	*f = (sr * si) / (sr + si);

	return 0;
}

int
zsl_phy_opt_critic_angle(zsl_real_t n1, zsl_real_t n2, zsl_real_t *oc)
{
	if (n1 <= 0 || n2 < 0 || n2 > n1) {
		*oc = NAN;
		return -EINVAL;
	}
	
	*oc = ZSL_ASIN(n2 / n1);

	return 0;
}

int
zsl_phy_opt_power(zsl_real_t f, zsl_real_t *p)
{
	if (f == 0) {
		*p = NAN;
		return -EINVAL;
	}
	
	*p = 1 / f;

	return 0;
}

int
zsl_phy_opt_magn(zsl_real_t y1, zsl_real_t y2, zsl_real_t *m)
{
	if (y1 <= 0 || y2 < 0) {
		*m = NAN;
		return -EINVAL;
	}
	
	*m = y2 / y1;

	return 0;
}

int
zsl_phy_opt_dif(uint8_t n, zsl_real_t lambda, zsl_real_t o, zsl_real_t *d)
{	
	if (lambda < 0 || ZSL_SIN(o) <= 0) {
		*d = NAN;
		return -EINVAL;
	}
	
	*d = ((zsl_real_t) n * lambda) / ZSL_SIN(o);

	return 0;
}
