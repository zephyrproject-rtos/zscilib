/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/vectors.h>
#include <zsl/physics/optics.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_phy_opt_refrac_index)
{
	int rc;
	zsl_real_t n;
	
	rc = zsl_phy_opt_refrac_index(ZSL_LIGHT_SPEED * 0.9, &n);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(n, 0.9, 1E-6), NULL);

	/* Example for negative velocity. */
	rc = zsl_phy_opt_refrac_index(-ZSL_LIGHT_SPEED * 0.9, &n);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(n != n, NULL);
	
	/* Example for velocity greater than the speed of light. */
	rc = zsl_phy_opt_refrac_index(ZSL_LIGHT_SPEED * 1.1, &n);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(n != n, NULL);
}

ZTEST(zsl_tests, test_phy_opt_snell)
{
	int rc;
	zsl_real_t o2;
	
	rc = zsl_phy_opt_snell(0.3, 0.4, 0.8, &o2);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(o2, 0.5680829219, 1E-6), NULL);

	/* Example for negative 'n1'. */
	rc = zsl_phy_opt_snell(-0.3, 0.4, 0.8, &o2);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(o2 != o2, NULL);
	
	/* Example for negative 'n2'. */
	rc = zsl_phy_opt_snell(0.3, -0.4, 0.8, &o2);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(o2 != o2, NULL);
	
	/* Example for zero 'n2'. */
	rc = zsl_phy_opt_snell(0.3, 0.0, 0.8, &o2);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(o2 != o2, NULL);
	
	/* Example for no refraction. */
	rc = zsl_phy_opt_snell(0.9, 0.1, 0.8, &o2);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(o2 != o2, NULL);
}

ZTEST(zsl_tests, test_phy_opt_focus)
{
	int rc;
	zsl_real_t f;
	
	rc = zsl_phy_opt_focus(3.0, -2.0, &f);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(f, -6.0, 1E-6), NULL);

	/* Example for sr + si = 0. */
	rc = zsl_phy_opt_focus(3.0, -3.0, &f);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(f != f, NULL);
}

ZTEST(zsl_tests, test_phy_opt_critic_angle)
{
	int rc;
	zsl_real_t oc;
	
	rc = zsl_phy_opt_critic_angle(0.7, 0.4, &oc);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(oc, 0.6082455789, 1E-6), NULL);

	/* Example for negative 'n1'. */
	rc = zsl_phy_opt_critic_angle(-0.7, 0.4, &oc);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(oc != oc, NULL);
	
	/* Example for zero 'n1'. */
	rc = zsl_phy_opt_critic_angle(0.0, 0.4, &oc);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(oc != oc, NULL);
	
	/* Example for negative 'n2'. */
	rc = zsl_phy_opt_critic_angle(0.7, -0.4, &oc);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(oc != oc, NULL);
	
	/* Example for n2 > n1. */
	rc = zsl_phy_opt_critic_angle(0.4, 0.9, &oc);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(oc != oc, NULL);
}

ZTEST(zsl_tests, test_phy_opt_power)
{
	int rc;
	zsl_real_t p;
	
	/* Example for positive focal length. */
	rc = zsl_phy_opt_power(2.0, &p);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(p, 0.5, 1E-6), NULL);
	
	/* Example for negative focal length. */
	rc = zsl_phy_opt_power(-5.0, &p);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(p, -0.2, 1E-6), NULL);

	/* Example for zero focal length. */
	rc = zsl_phy_opt_power(0.0, &p);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(p != p, NULL);
}

ZTEST(zsl_tests, test_phy_opt_magn)
{
	int rc;
	zsl_real_t m;
	
	rc = zsl_phy_opt_magn(2.2, 4.4, &m);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(m, 2.0, 1E-6), NULL);

	/* Example for negative 'y1'. */
	rc = zsl_phy_opt_magn(-2.2, 4.4, &m);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(m != m, NULL);
	
	/* Example for zero 'y1'. */
	rc = zsl_phy_opt_magn(0.0, 4.4, &m);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(m != m, NULL);
	
	/* Example for negative 'y2'. */
	rc = zsl_phy_opt_magn(2.2, -4.4, &m);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(m != m, NULL);
}

ZTEST(zsl_tests, test_phy_opt_dif)
{
	int rc;
	zsl_real_t d;
	
	rc = zsl_phy_opt_dif(3, 550.0, 0.9, &d);
	zassert_true(rc == 0, NULL);
#ifdef CONFIG_ZSL_SINGLE_PRECISION
 	zassert_true(val_is_equal(d, 2106.4002522072, 1E-3), NULL);
#else
	zassert_true(val_is_equal(d, 2106.4002522072, 1E-8), NULL);
#endif

	/* Example for negative wavelength. */
	rc = zsl_phy_opt_dif(3, -550.0, 0.9, &d);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(d != d, NULL);
	
	/* Example for sin(o) = 0. */
	rc = zsl_phy_opt_dif(3, 550.0, 0.0, &d);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(d != d, NULL);
	
	/* Example for sin(o) < 0. */
	rc = zsl_phy_opt_dif(3, 550.0, -0.9, &d);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(d != d, NULL);
}
