/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/vectors.h>
#include <zsl/physics/mass.h>
#include "floatcheck.h"

void test_phy_mass_center(void)
{
	int rc;
	zsl_real_t mx, my, mz;

	/* Example with 3 objects. */
	zsl_real_t m[3] = { 1.0, 2.0, 3.0 };
	zsl_real_t x[3] = { 0.0, 3.0, 0.0 };
	zsl_real_t y[3] = { -1.0, 0.5, 0.1 };
	zsl_real_t z[3] = { 3.0, 1.5, 2.0 };

	rc = zsl_phy_mass_center(3, m, x, y, z, &mx, &my, &mz);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(mx, 1.0, 1E-6), NULL);
	zassert_true(val_is_equal(my, 0.05, 1E-6), NULL);
	zassert_true(val_is_equal(mz, 2.0, 1E-6), NULL);

	/* Examle with 6 objects. */
	zsl_real_t m1[6] = { 6.0, 1.0, 3.5, 7.0, 4.2, 1.7 };
	zsl_real_t x1[6] = { 5.0, -3.0, -2.1, 4.9, 5.3, -1.1 };
	zsl_real_t y1[6] = { -1.0, 0.5, 0.1, 0.7, 4.4, -0.3 };
	zsl_real_t z1[6] = { 3.0, -1.5, 2.0, 8.1, 2.4, -5.5 };

	rc = zsl_phy_mass_center(6, m1, x1, y1, z1, &mx, &my, &mz);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(mx, 3.176923077, 1E-6), NULL);
	zassert_true(val_is_equal(my, 0.7572649573, 1E-6), NULL);
	zassert_true(val_is_equal(mz, 3.458547009, 1E-6), NULL);

	/* Example in which some of the objects have negative mass. */
	zsl_real_t m2[3] = { -2.0, -1.0, 3.0 };
	zsl_real_t x2[3] = { 0.0, 3.0, 0.0 };
	zsl_real_t y2[3] = { -1.0, 0.5, 0.1 };
	zsl_real_t z2[3] = { 3.0, 1.5, 2.0 };

	rc = zsl_phy_mass_center(3, m2, x2, y2, z2, &mx, &my, &mz);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(mx != mx, NULL);
	zassert_true(my != my, NULL);
	zassert_true(mz != mz, NULL);

	/* Example in which the total mass is zero. */
	zsl_real_t m3[3] = { 0.0, 0.0, 0.0 };
	zsl_real_t x3[3] = { 0.0, 3.0, 0.0 };
	zsl_real_t y3[3] = { -1.0, 0.5, 0.1 };
	zsl_real_t z3[3] = { 3.0, 1.5, 2.0 };

	rc = zsl_phy_mass_center(3, m3, x3, y3, z3, &mx, &my, &mz);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(mx != mx, NULL);
	zassert_true(my != my, NULL);
	zassert_true(mz != mz, NULL);
}
