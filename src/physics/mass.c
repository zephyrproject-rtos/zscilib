/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <zsl/zsl.h>
#include <zsl/physics/mass.h>


int
zsl_phy_mass_center(struct zsl_vec *m, struct zsl_vec *x,
		    struct zsl_vec *y, struct zsl_vec *z, zsl_real_t *mx,
		    zsl_real_t *my, zsl_real_t *mz)
{
	/* TO DO: Check if the number of arguments in m, x, y and z
	 * is equal to n. */

	int rc;
	zsl_real_t mt = 0.0;
	zsl_real_t mtx = 0.0;
	zsl_real_t mty = 0.0;
	zsl_real_t mtz = 0.0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Ensure that all vectors have the same size. */
	if ((m->sz != x->sz) || (x->sz != y->sz) || (y->sz != z->sz)) {
		*mx = NAN;
		*my = NAN;
		*mz = NAN;
		return -EINVAL;
	}
#endif

	rc = zsl_vec_dot(m, x, &mtx);
	if (rc) {
		return -EINVAL;
	}
	
	rc = zsl_vec_dot(m, y, &mty);
	if (rc) {
		return -EINVAL;
	}
	
	rc = zsl_vec_dot(m, z, &mtz);
	if (rc) {
		return -EINVAL;
	}

	/* Calculate arithematic mean of all masses in vector m. */
	rc = zsl_vec_ar_mean(m, &mt);
	if (rc) {
		return -EINVAL;
	}

	/* Ensure there are no negative values for mass. */
	if (zsl_vec_is_nonneg(m) == false) {
		*mx = NAN;
		*my = NAN;
		*mz = NAN;
		return -EINVAL;
	}

	mt *= m->sz;

	/* Avoid divide by zero errors. */
	if (mt == 0.0) {
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
