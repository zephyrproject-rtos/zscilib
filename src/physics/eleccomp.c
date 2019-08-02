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
