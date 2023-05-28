/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/vectors.h>
#include <zsl/physics/electricity.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_phy_elcty_current)
{
	int rc;
	zsl_real_t i;

	rc = zsl_phy_elcty_current(15.0, 10.0, &i);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(i, 1.5, 1E-6), NULL);

	/* Example for negative time. */
	rc = zsl_phy_elcty_current(15.0, -10.0, &i);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(i != i, NULL);
}

ZTEST(zsl_tests, test_phy_elcty_res_series)
{
	int rc;
	zsl_real_t r;

	ZSL_VECTOR_DEF(va, 5);
	ZSL_VECTOR_DEF(vb, 5);

	zsl_real_t a[5] = { 1.0, 3.2, 4.0, 0.5, 2.5 };
	zsl_real_t b[5] = { 1.0, 3.2, 4.0, 0.5, -2.5 };

	/* Assign arrays to vectors. */
	rc = zsl_vec_from_arr(&va, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&vb, b);
	zassert_true(rc == 0, NULL);

	rc = zsl_phy_elcty_res_series(&va, &r);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(r, 11.2, 1E-6), NULL);

	/* Example for a negative resistance. */
	rc = zsl_phy_elcty_res_series(&vb, &r);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(r != r, NULL);
}

ZTEST(zsl_tests, test_phy_elcty_res_parallel)
{
	int rc;
	zsl_real_t r;

	ZSL_VECTOR_DEF(va, 5);
	ZSL_VECTOR_DEF(vb, 5);

	zsl_real_t a[5] = { 1.0, 3.2, 4.0, 0.5, 2.5 };
	zsl_real_t b[5] = { 1.0, 3.2, 4.0, 0.5, -2.5 };

	/* Assign arrays to vectors. */
	rc = zsl_vec_from_arr(&va, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&vb, b);
	zassert_true(rc == 0, NULL);

	rc = zsl_phy_elcty_res_parallel(&va, &r);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(r, 0.25236593059, 1E-6), NULL);

	/* Example for a negative resistance. */
	rc = zsl_phy_elcty_res_parallel(&vb, &r);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(r != r, NULL);
}

ZTEST(zsl_tests, test_phy_elcty_cap_series)
{
	int rc;
	zsl_real_t c;

	ZSL_VECTOR_DEF(va, 5);
	ZSL_VECTOR_DEF(vb, 5);

	zsl_real_t a[5] = { 1.0, 3.2, 4.0, 0.5, 3.5 };
	zsl_real_t b[5] = { 1.0, 3.2, 4.0, 0.5, -3.5 };

	/* Assign arrays to vectors. */
	rc = zsl_vec_from_arr(&va, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&vb, b);
	zassert_true(rc == 0, NULL);

	rc = zsl_phy_elcty_cap_series(&va, &c);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(c, 0.25986078886, 1E-6), NULL);

	/* Example for a negative capacitance. */
	rc = zsl_phy_elcty_cap_series(&vb, &c);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(c != c, NULL);
}

ZTEST(zsl_tests, test_phy_elcty_cap_parallel)
{
	int rc;
	zsl_real_t c;

	ZSL_VECTOR_DEF(va, 5);
	ZSL_VECTOR_DEF(vb, 5);

	zsl_real_t a[5] = { 1.0, 3.2, 4.0, 0.5, 3.5 };
	zsl_real_t b[5] = { 1.0, 3.2, 4.0, 0.5, -3.5 };

	/* Assign arrays to vectors. */
	rc = zsl_vec_from_arr(&va, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&vb, b);
	zassert_true(rc == 0, NULL);

	rc = zsl_phy_elcty_cap_parallel(&va, &c);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(c, 12.2, 1E-6), NULL);

	/* Example for a negative capacitance. */
	rc = zsl_phy_elcty_cap_parallel(&vb, &c);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(c != c, NULL);
}

ZTEST(zsl_tests, test_phy_elcty_resistivity)
{
	int rc;
	zsl_real_t rty;

	rc = zsl_phy_elcty_resistivity(250.0, 0.025, 0.001, &rty);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(rty, 6250.0, 1E-6), NULL);

	/* Example for negative resistance. */
	rc = zsl_phy_elcty_resistivity(-250.0, 0.025, 0.001, &rty);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(rty != rty, NULL);

	/* Example for negative area section. */
	rc = zsl_phy_elcty_resistivity(250.0, -0.025, 0.001, &rty);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(rty != rty, NULL);

	/* Example for negative length. */
	rc = zsl_phy_elcty_resistivity(250.0, 0.025, -0.001, &rty);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(rty != rty, NULL);
}

ZTEST(zsl_tests, test_phy_elcty_ohm_law)
{
	int rc;
	zsl_real_t v;

	rc = zsl_phy_elcty_ohm_law(0.025, 250.0, &v);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(v, 6.25, 1E-6), NULL);

	/* Example for negative resistance. */
	rc = zsl_phy_elcty_ohm_law(0.025, -250.0, &v);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(v != v, NULL);
}

ZTEST(zsl_tests, test_phy_elcty_power_vi)
{
	int rc;
	zsl_real_t p;

	rc = zsl_phy_elcty_power_vi(1.5, 0.025, &p);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(p, 0.0375, 1E-6), NULL);
}

ZTEST(zsl_tests, test_phy_elcty_power_ir)
{
	int rc;
	zsl_real_t p;

	rc = zsl_phy_elcty_power_ir(0.025, 250.0, &p);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(p, 0.15625, 1E-6), NULL);

	/* Example for negative resistance. */
	rc = zsl_phy_elcty_power_ir(0.025, -250.0, &p);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(p != p, NULL);
}

ZTEST(zsl_tests, test_phy_elcty_power_vr)
{
	int rc;
	zsl_real_t p;

	rc = zsl_phy_elcty_power_vr(1.5, 250.0, &p);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(p, 0.009, 1E-6), NULL);

	/* Example for negative resistance. */
	rc = zsl_phy_elcty_power_vr(1.5, -250.0, &p);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(p != p, NULL);
}