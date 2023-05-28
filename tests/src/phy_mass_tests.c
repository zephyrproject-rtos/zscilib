/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "floatcheck.h"

#include <zephyr/ztest.h>

#include <zsl/matrices.h>
#include <zsl/physics/mass.h>
#include <zsl/vectors.h>
#include <zsl/zsl.h>

ZTEST(zsl_tests, test_phy_mass_center)
{
	int rc;
	zsl_real_t mx, my, mz;

	ZSL_VECTOR_DEF(m1, 3);
	ZSL_VECTOR_DEF(x1, 3);
	ZSL_VECTOR_DEF(y1, 3);
	ZSL_VECTOR_DEF(z1, 3);

	/* Example with 3 objects. */
	zsl_real_t md1[3] = {1.0, 2.0, 3.0};
	zsl_real_t xd1[3] = {0.0, 3.0, 0.0};
	zsl_real_t yd1[3] = {-1.0, 0.5, 0.1};
	zsl_real_t zd1[3] = {3.0, 1.5, 2.0};

	zsl_vec_from_arr(&m1, md1);
	zsl_vec_from_arr(&x1, xd1);
	zsl_vec_from_arr(&y1, yd1);
	zsl_vec_from_arr(&z1, zd1);

	rc = zsl_phy_mass_center(&m1, &x1, &y1, &z1, &mx, &my, &mz);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(mx, 1.0, 1E-6), NULL);
	zassert_true(val_is_equal(my, 0.05, 1E-6), NULL);
	zassert_true(val_is_equal(mz, 2.0, 1E-6), NULL);

	/* Example with 6 objects. */
	ZSL_VECTOR_DEF(m2, 6);
	ZSL_VECTOR_DEF(x2, 6);
	ZSL_VECTOR_DEF(y2, 6);
	ZSL_VECTOR_DEF(z2, 6);

	zsl_real_t md2[6] = {6.0, 1.0, 3.5, 7.0, 4.2, 1.7};
	zsl_real_t xd2[6] = {5.0, -3.0, -2.1, 4.9, 5.3, -1.1};
	zsl_real_t yd2[6] = {-1.0, 0.5, 0.1, 0.7, 4.4, -0.3};
	zsl_real_t zd2[6] = {3.0, -1.5, 2.0, 8.1, 2.4, -5.5};

	zsl_vec_from_arr(&m2, md2);
	zsl_vec_from_arr(&x2, xd2);
	zsl_vec_from_arr(&y2, yd2);
	zsl_vec_from_arr(&z2, zd2);

	rc = zsl_phy_mass_center(&m2, &x2, &y2, &z2, &mx, &my, &mz);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(mx, 3.176923077, 1E-6), NULL);
	zassert_true(val_is_equal(my, 0.7572649573, 1E-6), NULL);
	zassert_true(val_is_equal(mz, 3.458547009, 1E-6), NULL);

	/* Example in which some of the objects have negative mass. */
	ZSL_VECTOR_DEF(m3, 3);
	ZSL_VECTOR_DEF(x3, 3);
	ZSL_VECTOR_DEF(y3, 3);
	ZSL_VECTOR_DEF(z3, 3);

	zsl_real_t md3[3] = {-2.0, -1.0, 3.0};
	zsl_real_t xd3[3] = {0.0, 3.0, 0.0};
	zsl_real_t yd3[3] = {-1.0, 0.5, 0.1};
	zsl_real_t zd3[3] = {3.0, 1.5, 2.0};

	zsl_vec_from_arr(&m3, md3);
	zsl_vec_from_arr(&x3, xd3);
	zsl_vec_from_arr(&y3, yd3);
	zsl_vec_from_arr(&z3, zd3);

	rc = zsl_phy_mass_center(&m3, &x3, &y3, &z3, &mx, &my, &mz);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(mx != mx, NULL);
	zassert_true(my != my, NULL);
	zassert_true(mz != mz, NULL);

	/* Example in which the total mass is zero. */
	ZSL_VECTOR_DEF(m4, 3);
	ZSL_VECTOR_DEF(x4, 3);
	ZSL_VECTOR_DEF(y4, 3);
	ZSL_VECTOR_DEF(z4, 3);

	zsl_real_t md4[3] = {0.0, 0.0, 0.0};
	zsl_real_t xd4[3] = {0.0, 3.0, 0.0};
	zsl_real_t yd4[3] = {-1.0, 0.5, 0.1};
	zsl_real_t zd4[3] = {3.0, 1.5, 2.0};

	zsl_vec_from_arr(&m4, md4);
	zsl_vec_from_arr(&x4, xd4);
	zsl_vec_from_arr(&y4, yd4);
	zsl_vec_from_arr(&z4, zd4);

	rc = zsl_phy_mass_center(&m4, &x4, &y4, &z4, &mx, &my, &mz);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(mx != mx, NULL);
	zassert_true(my != my, NULL);
	zassert_true(mz != mz, NULL);

	/* Example in which the dimension of the vectors is not the same. */
	ZSL_VECTOR_DEF(m5, 4);
	ZSL_VECTOR_DEF(x5, 3);
	ZSL_VECTOR_DEF(y5, 3);
	ZSL_VECTOR_DEF(z5, 3);

	zsl_real_t md5[4] = {2.0, 3.0, 8.0, 5.0};
	zsl_real_t xd5[3] = {0.0, 3.0, 0.0};
	zsl_real_t yd5[3] = {-1.0, 0.5, 0.1};
	zsl_real_t zd5[3] = {3.0, 1.5, 2.0};

	zsl_vec_from_arr(&m5, md5);
	zsl_vec_from_arr(&x5, xd5);
	zsl_vec_from_arr(&y5, yd5);
	zsl_vec_from_arr(&z5, zd5);

	rc = zsl_phy_mass_center(&m5, &x5, &y5, &z5, &mx, &my, &mz);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(mx != mx, NULL);
	zassert_true(my != my, NULL);
	zassert_true(mz != mz, NULL);
}
