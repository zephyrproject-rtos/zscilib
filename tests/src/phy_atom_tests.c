/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/vectors.h>
#include <zsl/physics/atomic.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_phy_atom_nucl_radius)
{
	int rc;
	zsl_real_t r;

	rc = zsl_phy_atom_nucl_radius(5, &r);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(r, 2.137469933, 1E-6), NULL);

	/* Example for zero mass number. */
	rc = zsl_phy_atom_nucl_radius(0, &r);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(r != r, NULL);
}

ZTEST(zsl_tests, test_phy_atom_bohr_orb_radius)
{
	int rc;
	zsl_real_t r;

	rc = zsl_phy_atom_bohr_orb_radius(3, 5, &r);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(r, 0.4409810014, 1E-6), NULL);

	/* Example for zero Z. */
	rc = zsl_phy_atom_bohr_orb_radius(0, 5, &r);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(r != r, NULL);
}

ZTEST(zsl_tests, test_phy_atom_bohr_orb_vel)
{
	int rc;
	zsl_real_t v;

	rc = zsl_phy_atom_bohr_orb_vel(3, 5, &v);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(v, 1312.6147793774928, 1E-6), NULL);

	/* Example for zero Z. */
	rc = zsl_phy_atom_bohr_orb_vel(0, 5, &v);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(v != v, NULL);
}

ZTEST(zsl_tests, test_phy_atom_bohr_orb_ener)
{
	int rc;
	zsl_real_t e;

	rc = zsl_phy_atom_bohr_orb_ener(3.0, 5.0, &e);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(e, -4.8980496069, 1E-6), NULL);

	/* Example for zero Z. */
	rc = zsl_phy_atom_bohr_orb_ener(0, 5, &e);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(e != e, NULL);
}

ZTEST(zsl_tests, test_phy_atom_rad_decay)
{
	int rc;
	zsl_real_t qf;

	rc = zsl_phy_atom_rad_decay(5.32E2, 3.1536E9, 8.0821918E-10, &qf);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(qf, 41.589323317, 1E-6), NULL);

	/* Example for negative time. */
	rc = zsl_phy_atom_rad_decay(10.0, -20.0, 3.0, &qf);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(qf != qf, NULL);

	/* Example for negative lambda. */
	rc = zsl_phy_atom_rad_decay(10.0, 20.0, -3.0, &qf);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(qf != qf, NULL);
}

ZTEST(zsl_tests, test_phy_atom_bragg)
{
	int rc;
	zsl_real_t d;

	rc = zsl_phy_atom_bragg(3, 1.1, 500.0, &d);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(d, 841.5549889, 1E-6), NULL);

	/* Example for negative sin(theta). */
	rc = zsl_phy_atom_bragg(3, 5.9, 500.0, &d);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(d != d, NULL);

	/* Example for zero sin(theta). */
	rc = zsl_phy_atom_bragg(3, 0.0, 500.0, &d);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(d != d, NULL);

	/* Example for negative lambda. */
	rc = zsl_phy_atom_bragg(3, 5.9, -500.0, &d);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(d != d, NULL);
}
