/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/vectors.h>
#include <zsl/physics/gases.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_phy_gas_av_vel)
{
	int rc;
	zsl_real_t v;

	rc = zsl_phy_gas_av_vel(0.1, 0.5, 273.15, &v);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(v, 184.5709401872, 1E-6), NULL);

	/* Example for negative mass. */
	rc = zsl_phy_gas_av_vel(-0.1, 0.5, 273.15, &v);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(v != v, NULL);
	
	/* Example for zero mass. */
	rc = zsl_phy_gas_av_vel(0.0, 0.5, 273.15, &v);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(v != v, NULL);
	
	/* Example for negative 'n'. */
	rc = zsl_phy_gas_av_vel(0.1, -0.5, 273.15, &v);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(v != v, NULL);
	
	/* Example for negative temperature. */
	rc = zsl_phy_gas_av_vel(0.1, 0.5, -273.15, &v);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(v != v, NULL);
}

ZTEST(zsl_tests, test_phy_gas_press)
{
	int rc;
	zsl_real_t p;

	rc = zsl_phy_gas_press(1.0, 0.5, 273.15, &p);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(p, 1135.5477320534, 1E-6), NULL);

	/* Example for negative volume. */
	rc = zsl_phy_gas_press(-1.0, 0.5, 273.15, &p);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(p != p, NULL);
	
	/* Example for zero volume. */
	rc = zsl_phy_gas_press(0.0, 0.5, 273.15, &p);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(p != p, NULL);
	
	/* Example for negative 'n'. */
	rc = zsl_phy_gas_press(1.0, -0.5, 273.15, &p);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(p != p, NULL);
	
	/* Example for negative temperature. */
	rc = zsl_phy_gas_press(1.0, 0.5, -273.15, &p);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(p != p, NULL);
}

ZTEST(zsl_tests, test_phy_gas_boyle)
{
	int rc;
	zsl_real_t vf;

	rc = zsl_phy_gas_boyle(1.0E5, 12.6, 2.0E5, &vf);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(vf, 6.3, 1E-6), NULL);

	/* Example for negative initial pressure. */
	rc = zsl_phy_gas_boyle(-1.0E5, 12.6, 2.0E5, &vf);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vf != vf, NULL);
	
	/* Example for negative initial volume. */
	rc = zsl_phy_gas_boyle(1.0E5, -12.6, 2.0E5, &vf);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vf != vf, NULL);
	
	/* Example for negative final pressure. */
	rc = zsl_phy_gas_boyle(1.0E5, 12.6, -2.0E5, &vf);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vf != vf, NULL);
	
	/* Example for zero final pressure. */
	rc = zsl_phy_gas_boyle(1.0E5, 12.6, 0.0, &vf);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vf != vf, NULL);
}

ZTEST(zsl_tests, test_phy_gas_charles_lussac)
{
	int rc;
	zsl_real_t vf;

	rc = zsl_phy_gas_charles_lussac(300, 12.3, 200, &vf);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(vf, 8.2, 1E-6), NULL);

	/* Example for negative initial temperature. */
	rc = zsl_phy_gas_charles_lussac(-300, 12.3, 200, &vf);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vf != vf, NULL);
	
	/* Example for zero initial temperature */
	rc = zsl_phy_gas_charles_lussac(0.0, 12.3, 200, &vf);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vf != vf, NULL);
	
	/* Example for negative initial volume. */
	rc = zsl_phy_gas_charles_lussac(300, -12.3, 200, &vf);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vf != vf, NULL);
	
	/* Example for negative final temperature. */
	rc = zsl_phy_gas_charles_lussac(300, 12.3, -200, &vf);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vf != vf, NULL);
}
