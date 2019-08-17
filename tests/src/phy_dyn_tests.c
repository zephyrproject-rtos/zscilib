/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/vectors.h>
#include <zsl/physics/dynamics.h>
#include "floatcheck.h"

void test_phy_dyn_newton(void)
{
	int rc;
	zsl_real_t f;

 	rc = zsl_phy_dyn_newton(5.1, 4.2, &f);
 	zassert_true(rc == 0, NULL);
#ifdef CONFIG_ZSL_SINGLE_PRECISION
 	zassert_true(val_is_equal(f, 21.42, 1E-5), NULL);
#else
	zassert_true(val_is_equal(f, 21.42, 1E-8), NULL);
#endif

	rc = zsl_phy_dyn_newton(5.0, 4.0, &f);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(f, 20.0, 1E-8), NULL);

	/* Example for negative mass. */
	rc = zsl_phy_dyn_newton(-5.1, 4.2, &f);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(f != f, NULL);
}

void test_phy_dyn_fric(void)
{
	int rc;
	zsl_real_t f;

	rc = zsl_phy_dyn_fric(3.3, 0.2, &f);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(f, 0.66, 1E-6), NULL);

	/* Example for mu less than zero. */
	rc = zsl_phy_dyn_fric(3.3, -2.3, &f);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(f != f, NULL);

	/* Example for mu greater than one. */
	rc = zsl_phy_dyn_fric(3.3, 6.7, &f);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(f != f, NULL);
}

void test_phy_dyn_normal(void)
{
	int rc;
	zsl_real_t n;

	rc = zsl_phy_dyn_normal(51.3, 0.9, &n);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(n, 312.7314155880, 1E-6), NULL);

	/* Example for negative mass. */
	rc = zsl_phy_dyn_normal(-21.12, 1.1, &n);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(n != n, NULL);
}
