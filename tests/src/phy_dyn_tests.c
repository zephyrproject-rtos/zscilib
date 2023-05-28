/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/vectors.h>
#include <zsl/physics/dynamics.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_phy_dyn_newton)
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

ZTEST(zsl_tests, test_phy_dyn_fric)
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

ZTEST(zsl_tests, test_phy_dyn_normal)
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

ZTEST(zsl_tests, test_phy_dyn_pendul_period)
{
	int rc;
	zsl_real_t t;

	rc = zsl_phy_dyn_pendul_period(5.1, &t);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(t, 4.5310292812, 1E-6), NULL);

	/* Example for negative length. */
	rc = zsl_phy_dyn_pendul_period(-5.1, &t);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(t != t, NULL);
}

ZTEST(zsl_tests, test_phy_dyn_pendul_max_speed)
{
	int rc;
	zsl_real_t vmax;

	rc = zsl_phy_dyn_pendul_max_speed(5.1, 0.3, &vmax);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(vmax, 2.1137061105, 1E-6), NULL);

	/* Example for negative length. */
	rc = zsl_phy_dyn_pendul_max_speed(-5.1, 0.3, &vmax);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vmax != vmax, NULL);
}

ZTEST(zsl_tests, test_phy_dyn_tension)
{
	int rc;
	zsl_real_t t;

	rc = zsl_phy_dyn_tension(15.0, 3.6, &t);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(t, 201.105, 1E-6), NULL);

	/* Example for negative mass. */
	rc = zsl_phy_dyn_tension(-15.0, 3.6, &t);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(t != t, NULL);
}

ZTEST(zsl_tests, test_phy_dyn_lever)
{
	int rc;
	zsl_real_t f2;

	rc = zsl_phy_dyn_lever(1.2, 44.0, 3.6, &f2);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(f2, 14.6666666666, 1E-6), NULL);

	/* Example for negative distance. */
	rc = zsl_phy_dyn_lever(-1.2, 44.0, -3.6, &f2);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(f2 != f2, NULL);
	
	/* Example for zero 'd2'. */
	rc = zsl_phy_dyn_lever(1.2, 44.0, 0.0, &f2);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(f2 != f2, NULL);
}
