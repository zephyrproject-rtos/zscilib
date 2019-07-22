/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <kernel.h>
#include <zsl/zsl.h>
#include <zsl/physics/mass.h>

int
zsl_phy_mass_center(size_t n, zsl_real_t m[n], zsl_real_t x[n], zsl_real_t y[n],
		    zsl_real_t z[n], zsl_real_t *mx, zsl_real_t *my,
		    zsl_real_t *mz)
{
	/* TO DO: Check if the number of arguments in m, x, y and z
	 * is equal to n. */

	zsl_real_t mt = 0.0;
	zsl_real_t mtx = 0.0;
	zsl_real_t mty = 0.0;
	zsl_real_t mtz = 0.0;

	for (size_t g = 0; g < n; g++) {
		if (m[g] < 0) {
			*mx = NAN;
			*my = NAN;
			*mz = NAN;
			return -EINVAL;
		}
		x[g] *= m[g];
		y[g] *= m[g];
		z[g] *= m[g];
		mt += m[g];
		mtx += x[g];
		mty += y[g];
		mtz += z[g];
	}

	if (mt == 0) {
		*mx = NAN;
		*my = NAN;
		*mz = NAN;
		return -EINVAL;
	}

	*mx = mtx / mt;
	*my = mty / mt;
	*mz = mtz / mt;

	return 0;
}
