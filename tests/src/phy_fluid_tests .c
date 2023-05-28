/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/vectors.h>
#include <zsl/physics/fluids.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_phy_fluid_dens)
{
	int rc;
	zsl_real_t d;
	
	rc = zsl_phy_fluid_dens(53.0, 20.0, &d);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(d, 2.65, 1E-6), NULL);

	/* Example for negative mass. */
	rc = zsl_phy_fluid_dens(-53.0, 20.0, &d);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(d != d, NULL);
	
	/* Example for negative volume. */
	rc = zsl_phy_fluid_dens(53.0, -20.0, &d);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(d != d, NULL);
	
	/* Example for zero volume. */
	rc = zsl_phy_fluid_dens(53.0, 0.0, &d);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(d != d, NULL);
}

ZTEST(zsl_tests, test_phy_fluid_simple_press)
{
	int rc;
	zsl_real_t p;
	
	rc = zsl_phy_fluid_simple_press(53.0, 20.0, &p);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(p, 2.65, 1E-6), NULL);

	/* Example for negative force. */
	rc = zsl_phy_fluid_simple_press(-53.0, 20.0, &p);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(p != p, NULL);
	
	/* Example for negative area. */
	rc = zsl_phy_fluid_simple_press(53.0, -20.0, &p);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(p != p, NULL);
	
	/* Example for zero area. */
	rc = zsl_phy_fluid_simple_press(53.0, 0.0, &p);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(p != p, NULL);
}

ZTEST(zsl_tests, test_phy_fluid_press)
{
	int rc;
	zsl_real_t p;
	
	rc = zsl_phy_fluid_press(10.3, 2.3, 997.0, &p);
	zassert_true(rc == 0, NULL);
#ifdef CONFIG_ZSL_SINGLE_PRECISION
 	zassert_true(val_is_equal(p, 32.7884317, 1E-5), NULL);
#else
	zassert_true(val_is_equal(p, 32.7884317, 1E-8), NULL);
#endif

	/* Example for negative superficial pressure. */
	rc = zsl_phy_fluid_press(-10.3, 2.3, 997.0, &p);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(p != p, NULL);
	
	/* Example for negative depth. */
	rc = zsl_phy_fluid_press(10.3, -2.3, 997.0, &p);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(p != p, NULL);
	
	/* Example for negative density of the fluid. */
	rc = zsl_phy_fluid_press(10.3, 2.3, -997.0, &p);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(p != p, NULL);
}

ZTEST(zsl_tests, test_phy_fluid_bouyant_force)
{
	int rc;
	zsl_real_t f;
	
	rc = zsl_phy_fluid_bouyant_force(1.3, 997.0, &f);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(f, 12.7108527, 1E-6), NULL);

	/* Example for negative volume. */
	rc = zsl_phy_fluid_bouyant_force(-1.3, 997.0, &f);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(f != f, NULL);
	
	/* Example for negative density. */
	rc = zsl_phy_fluid_bouyant_force(1.3, -997.0, &f);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(f != f, NULL);
}

ZTEST(zsl_tests, test_phy_fluid_flow_rate)
{
	int rc;
	zsl_real_t v2;
	
	rc = zsl_phy_fluid_flow_rate(0.5, 12.0, 0.25, &v2);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(v2, 24.0, 1E-6), NULL);

	/* Example for negative 'a1'. */
	rc = zsl_phy_fluid_flow_rate(-0.5, 12.0, 0.25, &v2);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(v2 != v2, NULL);
	
	/* Example for negative 'v1'. */
	rc = zsl_phy_fluid_flow_rate(0.5, -12.0, 0.25, &v2);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(v2 != v2, NULL);
	
	/* Example for negative 'a2'. */
	rc = zsl_phy_fluid_flow_rate(0.5, 12.0, -0.25, &v2);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(v2 != v2, NULL);
	
	/* Example for zero 'a2'. */
	rc = zsl_phy_fluid_flow_rate(0.5, 12.0, 0.0, &v2);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(v2 != v2, NULL);
}

ZTEST(zsl_tests, test_phy_fluid_force_prop)
{
	int rc;
	zsl_real_t f2;
	
	rc = zsl_phy_fluid_force_prop(0.5, 12.0, 0.25, &f2);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(f2, 6.0, 1E-6), NULL);

	/* Example for negative 'a1'. */
	rc = zsl_phy_fluid_force_prop(-0.5, 12.0, 0.25, &f2);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(f2 != f2, NULL);
	
	/* Example for zero 'a1'. */
	rc = zsl_phy_fluid_force_prop(0.0, 12.0, 0.25, &f2);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(f2 != f2, NULL);
	
	/* Example for negative 'f1'. */
	rc = zsl_phy_fluid_force_prop(0.5, -12.0, 0.25, &f2);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(f2 != f2, NULL);
	
	/* Example for negative 'a2'. */
	rc = zsl_phy_fluid_force_prop(0.5, 12.0, -0.25, &f2);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(f2 != f2, NULL);
}

ZTEST(zsl_tests, test_phy_fluid_bernoulli)
{
	int rc;
	zsl_real_t p2;
	
	rc = zsl_phy_fluid_bernoulli(997.0, 12.0, 10.3, 10.0, &p2);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(p2, 32.234, 1E-6), NULL);

	/* Example for negative density. */
	rc = zsl_phy_fluid_bernoulli(-997.0, 12.0, 10.3, 10.0, &p2);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(p2 != p2, NULL);
	
	/* Example for negative 'v1'. */
	rc = zsl_phy_fluid_bernoulli(997.0, -12.0, 10.3, 10.0, &p2);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(p2 != p2, NULL);
	
	/* Example for negative 'p1'. */
	rc = zsl_phy_fluid_bernoulli(997.0, 12.0, -10.3, 10.0, &p2);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(p2 != p2, NULL);
	
	/* Example for negative 'v2'. */
	rc = zsl_phy_fluid_bernoulli(997.0, 12.0, 10.3, -10.0, &p2);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(p2 != p2, NULL);
}

ZTEST(zsl_tests, test_phy_fluid_volume_flow_rate)
{
	int rc;
	zsl_real_t v;
	
	rc = zsl_phy_fluid_volume_flow_rate(12.0, 0.25, &v);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(v, 3.0, 1E-6), NULL);

	/* Example for negative velocity. */
	rc = zsl_phy_fluid_volume_flow_rate(-12.0, 0.25, &v);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(v != v, NULL);
	
	/* Example for negative area. */
	rc = zsl_phy_fluid_volume_flow_rate(12.0, -0.25, &v);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(v != v, NULL);
}
