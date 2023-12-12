/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/vectors.h>
#include <zsl/physics/gravitation.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_phy_grav_orb_period)
{
	int rc;
	zsl_real_t t;

	rc = zsl_phy_grav_orb_period(1E12, 100.0, 5.0, &t);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(t, 8.598826195, 1E-6), NULL);

	rc = zsl_phy_grav_orb_period(0.0, 0.0, 25.0, &t);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(t != t, NULL);
}

ZTEST(zsl_tests, test_phy_grav_esc_vel)
{
	int rc;
	zsl_real_t v;

	rc = zsl_phy_grav_esc_vel(1E15, 400.0, &v);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(v, 18.2675668878, 1E-6), NULL);

	rc = zsl_phy_grav_esc_vel(1E15, 0.0, &v);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(v != v, NULL);
}

ZTEST(zsl_tests, test_phy_grav_acc)
{
	int rc;
	zsl_real_t a;

	rc = zsl_phy_grav_acc(1E15, 200.0, &a);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(a, -1.66852, 1E-6), NULL);

	rc = zsl_phy_grav_acc(1E15, 0.0, &a);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(a != a, NULL);
}

ZTEST(zsl_tests, test_phy_grav_orb_vel)
{
	int rc;
	zsl_real_t v;

	rc = zsl_phy_grav_orb_vel(1E15, 400.0, &v);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(v, 12.9171204221, 1E-6), NULL);

	rc = zsl_phy_grav_orb_vel(1E15, 0.0, &v);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(v != v, NULL);
}

ZTEST(zsl_tests, test_phy_grav_force)
{
	int rc;
	zsl_real_t f;

	rc = zsl_phy_grav_force(1E15, 200.0, 500.0, &f);
	zassert_true(rc == 0, NULL);
#ifdef CONFIG_ZSL_SINGLE_PRECISION
	zassert_true(val_is_equal(f, -53.39264, 1E-5), NULL);
#else
	zassert_true(val_is_equal(f, -53.39264, 1E-6), NULL);
#endif

	rc = zsl_phy_grav_force(1E15, 200.0, 0.0, &f);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(f != f, NULL);
}

ZTEST(zsl_tests, test_phy_grav_pot_ener)
{
	int rc;
	zsl_real_t u;

	rc = zsl_phy_grav_pot_ener(1E15, 200.0, 500.0, &u);
	zassert_true(rc == 0, NULL);
#ifdef CONFIG_ZSL_SINGLE_PRECISION
	zassert_true(val_is_equal(u, -26.69632, 1E-5), NULL);
#else
	zassert_true(val_is_equal(u, -26.69632, 1E-6), NULL);
#endif
	rc = zsl_phy_grav_pot_ener(1E15, 200.0, 0.0, &u);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(u != u, NULL);
}
