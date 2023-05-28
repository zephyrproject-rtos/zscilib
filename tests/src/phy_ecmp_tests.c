/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/vectors.h>
#include <zsl/physics/eleccomp.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_phy_ecmp_capac_cpv)
{
	int rc;
	zsl_real_t c;

	rc = zsl_phy_ecmp_capac_cpv(100.0, 10.0, &c);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(c, 10.0, 1E-6), NULL);

	rc = zsl_phy_ecmp_capac_cpv(100.0, 0.0, &c);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(c != c, NULL);
}

ZTEST(zsl_tests, test_phy_ecmp_capac_ad)
{
	int rc;
	zsl_real_t c;

	/* E * 0.015 m^2 plate / 0.006 m gap = 22.135 pF */
	rc = zsl_phy_ecmp_capac_ad(150E-4, 0.6E-2, &c);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(c, 2.2135E-11, 1E-13), NULL);

	rc = zsl_phy_ecmp_capac_ad(150E-4, 0.0, &c);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(c != c, NULL);
}

ZTEST(zsl_tests, test_phy_ecmp_ener_capac)
{
	int rc;
	zsl_real_t e;

	rc = zsl_phy_ecmp_ener_capac(3.0, 1.0, &e);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(e, 1.5, 1E-6), NULL);
}

ZTEST(zsl_tests, test_phy_ecmp_ener_induc)
{
	int rc;
	zsl_real_t e;

	rc = zsl_phy_ecmp_ener_induc(3.0, 1.0, &e);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(e, 1.5, 1E-6), NULL);
}

ZTEST(zsl_tests, test_phy_ecmp_trans)
{
	int rc;
	zsl_real_t v2;

	rc = zsl_phy_ecmp_trans(5, 2.0, 10, &v2);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(v2, 4.0, 1E-6), NULL);
	
	/* Example for zero 't1'. */
	rc = zsl_phy_ecmp_trans(0, 2.0, 10, &v2);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(v2 != v2, NULL);
}

ZTEST(zsl_tests, test_phy_ecmp_rlc_volt)
{
	int rc;
	zsl_real_t v;

	rc = zsl_phy_ecmp_rlc_volt(5.0, 2.0, 10.0, &v);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(v, 9.4339811321, 1E-6), NULL);
}

ZTEST(zsl_tests, test_phy_ecmp_rc_charg_i)
{
	int rc;
	zsl_real_t i;

	rc = zsl_phy_ecmp_rc_charg_i(200.0, 2.0, 10.0, 6.7, &i);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(i, 6.5345764106, 1E-6), NULL);
	
	/* Example for zero 'r * c'. */
	rc = zsl_phy_ecmp_rc_charg_i(0.0, 0.0, 10.0, 6.7, &i);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(i != i, NULL);
	
	/* Example for negative 'i0'. */
	rc = zsl_phy_ecmp_rc_charg_i(200.0, 2.0, 10.0, -6.7, &i);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(i != i, NULL);
	
	/* Example for negative time. */
	rc = zsl_phy_ecmp_rc_charg_i(200.0, 2.0, -10.0, 6.7, &i);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(i != i, NULL);
}

ZTEST(zsl_tests, test_phy_ecmp_rc_charg_q)
{
	int rc;
	zsl_real_t q;

	rc = zsl_phy_ecmp_rc_charg_q(200.0, 2.0, 10.0, 6.7, &q);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q, 0.1654235894, 1E-6), NULL);
	
	/* Example for zero 'r * c'. */
	rc = zsl_phy_ecmp_rc_charg_q(0.0, 0.0, 10.0, 6.7, &q);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(q != q, NULL);
	
	/* Example for negative 'q0'. */
	rc = zsl_phy_ecmp_rc_charg_q(200.0, 2.0, 10.0, -6.7, &q);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(q != q, NULL);
	
	/* Example for negative time. */
	rc = zsl_phy_ecmp_rc_charg_q(200.0, 2.0, -10.0, 6.7, &q);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(q != q, NULL);
}

ZTEST(zsl_tests, test_phy_ecmp_rc_discharg_i)
{
	int rc;
	zsl_real_t i;

	rc = zsl_phy_ecmp_rc_discharg_i(200.0, 2.0, 10.0, 6.7, &i);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(i, -6.5345764106, 1E-6), NULL);
	
	/* Example for zero 'r * c'. */
	rc = zsl_phy_ecmp_rc_discharg_i(0.0, 0.0, 10.0, 6.7, &i);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(i != i, NULL);
	
	/* Example for negative 'i0'. */
	rc = zsl_phy_ecmp_rc_discharg_i(200.0, 2.0, 10.0, -6.7, &i);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(i != i, NULL);
	
	/* Example for negative time. */
	rc = zsl_phy_ecmp_rc_discharg_i(200.0, 2.0, -10.0, 6.7, &i);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(i != i, NULL);
}

ZTEST(zsl_tests, test_phy_ecmp_rc_discharg_q)
{
	int rc;
	zsl_real_t q;

	rc = zsl_phy_ecmp_rc_discharg_q(200.0, 2.0, 10.0, 6.7, &q);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q, 6.5345764106, 1E-6), NULL);
	
	/* Example for zero 'r * c'. */
	rc = zsl_phy_ecmp_rc_discharg_q(0.0, 0.0, 10.0, 6.7, &q);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(q != q, NULL);
	
	/* Example for negative 'q0'. */
	rc = zsl_phy_ecmp_rc_discharg_q(200.0, 2.0, 10.0, -6.7, &q);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(q != q, NULL);
	
	/* Example for negative time. */
	rc = zsl_phy_ecmp_rc_discharg_q(200.0, 2.0, -10.0, 6.7, &q);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(q != q, NULL);
}

ZTEST(zsl_tests, test_phy_ecmp_rl_current)
{
	int rc;
	zsl_real_t i;

	rc = zsl_phy_ecmp_rl_current(2.0, 2.0, 10.0, 6.7, &i);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(i, 6.6996958205, 1E-6), NULL);
	
	/* Example for zero 'l'. */
	rc = zsl_phy_ecmp_rl_current(200.0, 0.0, 10.0, 6.7, &i);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(i != i, NULL);
	
	/* Example for negative 'i0'. */
	rc = zsl_phy_ecmp_rl_current(200.0, 2.0, 10.0, -6.7, &i);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(i != i, NULL);
	
	/* Example for negative time. */
	rc = zsl_phy_ecmp_rl_current(200.0, 2.0, -10.0, 6.7, &i);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(i != i, NULL);
	
	/* Example for negative resistance. */
	rc = zsl_phy_ecmp_rl_current(-200.0, 2.0, 10.0, 6.7, &i);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(i != i, NULL);
}
