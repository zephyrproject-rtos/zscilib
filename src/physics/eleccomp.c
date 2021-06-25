/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <zsl/zsl.h>
#include <zsl/physics/eleccomp.h>

int
zsl_phy_ecmp_capac_cpv(zsl_real_t q, zsl_real_t v, zsl_real_t *c)
{
	if (v == 0) {
		*c = NAN;
		return -EINVAL;
	}

	*c = q / v;

	return 0;
}

int
zsl_phy_ecmp_capac_ad(zsl_real_t a, zsl_real_t d, zsl_real_t *c)
{
	if (d == 0) {
		*c = NAN;
		return -EINVAL;
	}

	*c = ZSL_PERM_FREE_SPACE * (a / d);

	return 0;
}

int
zsl_phy_ecmp_ener_capac(zsl_real_t c, zsl_real_t v, zsl_real_t *u)
{
	*u = 0.5 * c * v * v;

	return 0;
}

int
zsl_phy_ecmp_ener_induc(zsl_real_t l, zsl_real_t i, zsl_real_t *u)
{
	*u = 0.5 * l * i * i;

	return 0;
}

int
zsl_phy_ecmp_trans(uint8_t t1, zsl_real_t v1, uint8_t t2, zsl_real_t *v2)
{
	if (t1 == 0) {
		*v2 = NAN;
		return -EINVAL;
	}

	*v2 = (v1 * (zsl_real_t) t2) / (zsl_real_t) t1;

	return 0;
}

int
zsl_phy_ecmp_rlc_volt(zsl_real_t vr, zsl_real_t vl, zsl_real_t vc,
		      zsl_real_t *v)
{
	*v = ZSL_SQRT(vr * vr + ZSL_POW(vl - vc, 2.0));

	return 0;
}

int
zsl_phy_ecmp_rc_charg_i(zsl_real_t r, zsl_real_t c, zsl_real_t t, zsl_real_t i0,
			zsl_real_t *i)
{
	if ((r * c) <= 0 || i0 < 0 || t < 0) {
		*i = NAN;
		return -EINVAL;
	}

	*i = i0 * ZSL_EXP(-t / (r * c));

	return 0;
}

int
zsl_phy_ecmp_rc_charg_q(zsl_real_t r, zsl_real_t c, zsl_real_t t, zsl_real_t q0,
			zsl_real_t *q)
{
	if ((r * c) <= 0 || q0 < 0 || t < 0) {
		*q = NAN;
		return -EINVAL;
	}

	*q = q0 * (1.0 - ZSL_EXP(-t / (r * c)));

	return 0;
}

int
zsl_phy_ecmp_rc_discharg_i(zsl_real_t r, zsl_real_t c, zsl_real_t t,
			   zsl_real_t i0, zsl_real_t *i)
{
	if ((r * c) <= 0 || i0 < 0 || t < 0) {
		*i = NAN;
		return -EINVAL;
	}

	*i = -i0 * ZSL_EXP(-t / (r * c));

	return 0;
}

int
zsl_phy_ecmp_rc_discharg_q(zsl_real_t r, zsl_real_t c, zsl_real_t t,
			   zsl_real_t q0, zsl_real_t *q)
{
	if ((r * c) <= 0 || q0 < 0 || t < 0) {
		*q = NAN;
		return -EINVAL;
	}

	*q = q0 * ZSL_EXP(-t / (r * c));

	return 0;
}

int
zsl_phy_ecmp_rl_current(zsl_real_t r, zsl_real_t l, zsl_real_t t, zsl_real_t i0,
			zsl_real_t *i)
{
	if (l <= 0 || i0 < 0 || t < 0 || r < 0) {
		*i = NAN;
		return -EINVAL;
	}

	*i = i0 * (1.0 - ZSL_EXP(-(r * t) / l));

	return 0;
}
