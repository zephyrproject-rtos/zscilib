/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <zsl/zsl.h>
#include <zsl/physics/work.h>

int
zsl_phy_work_module(zsl_real_t f, zsl_real_t d, zsl_real_t *w)
{
	if (f < 0 || d < 0) {
		*w = NAN;
		return -EINVAL;
	}

	*w = f * d;

	return 0;
}

int
zsl_phy_work_x(zsl_real_t f, zsl_real_t d, zsl_real_t theta, zsl_real_t *w)
{
	if (f < 0 || d < 0) {
		*w = NAN;
		return -EINVAL;
	}
	
	zsl_phy_work_module(f, d, w);

	*w *= ZSL_COS(theta);

	return 0;
}

int
zsl_phy_work_y(zsl_real_t f, zsl_real_t d, zsl_real_t theta, zsl_real_t *w)
{
	if (f < 0 || d < 0) {
		*w = NAN;
		return -EINVAL;
	}
	
	zsl_phy_work_module(f, d, w);

	*w *= ZSL_SIN(theta);

	return 0;
}

int
zsl_phy_work_kin(zsl_real_t k1, zsl_real_t k2, zsl_real_t *w)
{
	*w = k2 - k1;

	return 0;
}
