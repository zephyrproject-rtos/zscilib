/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <zsl/zsl.h>
#include <zsl/physics/electricity.h>

int
zsl_phy_elcty_current(zsl_real_t q, zsl_real_t t, zsl_real_t *i)
{
	if (t <= 0) {
		*i = NAN;
		return -EINVAL;
	}

	*i = q / t;

	return 0;
}

int
zsl_phy_elcty_res_series(struct zsl_vec *v, zsl_real_t *r)
{
	for (size_t j = 0; j < v->sz; j++) {
		if (v->data[j] < 0) {
			*r = NAN;
			return -EINVAL;
		}
	}

	*r = 0.0;
	for (size_t j = 0; j < v->sz; j++) {
		*r += v->data[j];
	}

	return 0;
}

int
zsl_phy_elcty_res_parallel(struct zsl_vec *v, zsl_real_t *r)
{
	for (size_t j = 0; j < v->sz; j++) {
		if (v->data[j] <= 0) {
			*r = NAN;
			return -EINVAL;
		}
	}

	zsl_real_t r_temp = 0.0;

	for (size_t j = 0; j < v->sz; j++) {
		r_temp += 1. / v->data[j];
	}

	*r = 1. / r_temp;

	return 0;
}

int
zsl_phy_elcty_cap_series(struct zsl_vec *v, zsl_real_t *c)
{
	for (size_t j = 0; j < v->sz; j++) {
		if (v->data[j] <= 0) {
			*c = NAN;
			return -EINVAL;
		}
	}

	zsl_real_t c_temp = 0.0;

	for (size_t j = 0; j < v->sz; j++) {
		c_temp += 1. / v->data[j];
	}

	*c = 1. / c_temp;

	return 0;
}

int
zsl_phy_elcty_cap_parallel(struct zsl_vec *v, zsl_real_t *c)
{
	for (size_t j = 0; j < v->sz; j++) {
		if (v->data[j] < 0) {
			*c = NAN;
			return -EINVAL;
		}
	}

	*c = 0.0;
	for (size_t j = 0; j < v->sz; j++) {
		*c += v->data[j];
	}

	return 0;
}

int
zsl_phy_elcty_resistivity(zsl_real_t r, zsl_real_t a, zsl_real_t l, zsl_real_t *rty)
{
	if (a < 0.0 || l <= 0.0 || r < 0.0) {
		*rty = NAN;
		return -EINVAL;
	}

	*rty = r * (a / l);

	return 0;
}

int
zsl_phy_elcty_ohm_law(zsl_real_t i, zsl_real_t r, zsl_real_t *v)
{
	if (r < 0.0) {
		*v = NAN;
		return -EINVAL;
	}

	*v = i * r;

	return 0;
}

int
zsl_phy_elcty_power_vi(zsl_real_t v, zsl_real_t i, zsl_real_t *p)
{
	*p = v * i;

	return 0;
}

int
zsl_phy_elcty_power_ir(zsl_real_t i, zsl_real_t r, zsl_real_t *p)
{
	if (r < 0.0) {
		*p = NAN;
		return -EINVAL;
	}

	*p = i * i * r;

	return 0;
}

int
zsl_phy_elcty_power_vr(zsl_real_t v, zsl_real_t r, zsl_real_t *p)
{
	if (r <= 0.0) {
		*p = NAN;
		return -EINVAL;
	}

	*p = v * v / r;

	return 0;
}
