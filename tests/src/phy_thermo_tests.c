/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/vectors.h>
#include <zsl/physics/thermo.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_phy_thermo_fahren_cels)
{
	int rc;
	zsl_real_t t2;
	
	/* Fahrenheit to celsius. */
	rc = zsl_phy_thermo_fahren_cels(41.0, &t2, false);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(t2, 5.0, 1E-6), NULL);

	/* Celsius to fahrenheit. */
	rc = zsl_phy_thermo_fahren_cels(5.0, &t2, true);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(t2, 41.0, 1E-6), NULL);
}

ZTEST(zsl_tests, test_phy_thermo_cels_kel)
{
	int rc;
	zsl_real_t t2;
	
	/* Celsius to kelvin. */
	rc = zsl_phy_thermo_cels_kel(27.0, &t2, false);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(t2, 300.15, 1E-6), NULL);

	/* Kelvin to celsius. */
	rc = zsl_phy_thermo_cels_kel(300.15, &t2, true);
	zassert_true(rc == 0, NULL);
#ifdef CONFIG_ZSL_SINGLE_PRECISION
 	zassert_true(val_is_equal(t2, 27.0, 1E-5), NULL);
#else
	zassert_true(val_is_equal(t2, 27.0, 1E-8), NULL);
#endif
}

ZTEST(zsl_tests, test_phy_thermo_heat_fusion)
{
	int rc;
	zsl_real_t q;
	
	rc = zsl_phy_thermo_heat_fusion(3.0, 334.0, &q);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q, 1002.0, 1E-6), NULL);

	/* Example for negative mass. */
	rc = zsl_phy_thermo_heat_fusion(-3.0, 334.0, &q);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(q != q, NULL);
	
	/* Example for negative latent heat. */
	rc = zsl_phy_thermo_heat_fusion(3.0, -334.0, &q);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(q != q, NULL);
}

ZTEST(zsl_tests, test_phy_thermo_heat)
{
	int rc;
	zsl_real_t q;
	
	rc = zsl_phy_thermo_heat(3.0, 4.18, 17.0, &q);
	zassert_true(rc == 0, NULL);
#ifdef CONFIG_ZSL_SINGLE_PRECISION
 	zassert_true(val_is_equal(q, 213.18, 1E-4), NULL);
#else
	zassert_true(val_is_equal(q, 213.18, 1E-8), NULL);
#endif

	/* Example for negative mass. */
	rc = zsl_phy_thermo_heat(-3.0, 4.18, 17.0, &q);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(q != q, NULL);
	
	/* Example for negative specific heat. */
	rc = zsl_phy_thermo_heat(3.0, -4.18, 17.0, &q);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(q != q, NULL);
}

ZTEST(zsl_tests, test_phy_thermo_expan)
{
	int rc;
	zsl_real_t l;
	
	rc = zsl_phy_thermo_expan(33.0, 11.4E-6, 530.0, &l);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(l, 33.199386, 1E-6), NULL);

	/* Example for negative initial length. */
	rc = zsl_phy_thermo_expan(-33.0, 11.4E-6, 530.0, &l);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(l != l, NULL);
	
	/* Example for negative 'a'. */
	rc = zsl_phy_thermo_expan(33.0, -11.4E-6, 530.0, &l);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(l != l, NULL);
}

ZTEST(zsl_tests, test_phy_thermo_mean_free_path)
{
	int rc;
	zsl_real_t lambda;
	
	rc = zsl_phy_thermo_mean_free_path(6.02E23, 2.75E-10, &lambda);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(lambda, 4.9439406725, 1E-6), NULL);

	/* Example for negative 'nv'. */
	rc = zsl_phy_thermo_mean_free_path(-6.02E23, 2.75E-10, &lambda);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(lambda != lambda, NULL);
	
	/* Example for zero 'nv'. */
	rc = zsl_phy_thermo_mean_free_path(0.0, 2.75E-10, &lambda);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(lambda != lambda, NULL);
	
	/* Example for negative molecule's diameter. */
	rc = zsl_phy_thermo_mean_free_path(6.02E23, -2.75E-10, &lambda);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(lambda != lambda, NULL);
	
	/* Example for zero molecule's diameter. */
	rc = zsl_phy_thermo_mean_free_path(6.02E23, 0.0, &lambda);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(lambda != lambda, NULL);
}

ZTEST(zsl_tests, test_phy_thermo_effic_heat_engine)
{
	int rc;
	zsl_real_t e;
	
	rc = zsl_phy_thermo_effic_heat_engine(340.0, 500.0, &e);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(e, 0.32, 1E-6), NULL);

	/* Example for qh = 0. */
	rc = zsl_phy_thermo_effic_heat_engine(340.0, 0.0, &e);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(e != e, NULL);
	
	/* Example for qc > qh. */
	rc = zsl_phy_thermo_effic_heat_engine(340.0, 40.0, &e);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(e != e, NULL);
}

ZTEST(zsl_tests, test_phy_thermo_carnot_engine)
{
	int rc;
	zsl_real_t qh;
	
	rc = zsl_phy_thermo_carnot_engine(263.0, 340.0, 312.0, &qh);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(qh, 403.3460076046, 1E-6), NULL);

	/* Example for negative cold temperature. */
	rc = zsl_phy_thermo_carnot_engine(-263.0, 340.0, 312.0, &qh);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(qh != qh, NULL);
	
	/* Example for zero cold temperature. */
	rc = zsl_phy_thermo_carnot_engine(0.0, 340.0, 312.0, &qh);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(qh != qh, NULL);
	
	/* Example for th < tc. */
	rc = zsl_phy_thermo_carnot_engine(363.0, 340.0, 312.0, &qh);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(qh != qh, NULL);
}
