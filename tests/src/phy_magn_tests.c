/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/vectors.h>
#include <zsl/physics/magnetics.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_phy_magn_force)
{
	int rc;
	zsl_real_t f;

	rc = zsl_phy_magn_force(1.5, 50.0, 4.3, 1.1, &f);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(f, 287.4143736198, 1E-6), NULL);

	/* Example for negative velocity. */
	rc = zsl_phy_magn_force(1.5, -50.0, 4.3, 1.1, &f);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(f != f, NULL);
	
	/* Example for negative magnetic field. */
	rc = zsl_phy_magn_force(1.5, 50.0, -4.3, 1.1, &f);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(f != f, NULL);
}

ZTEST(zsl_tests, test_phy_magn_force_wire)
{
	int rc;
	zsl_real_t f;

	rc = zsl_phy_magn_force_wire(1.5, 1.0, 4.3, 1.1, &f);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(f, 5.7482874724, 1E-6), NULL);

	/* Example for negative longitude. */
	rc = zsl_phy_magn_force_wire(1.5, -1.0, 4.3, 1.1, &f);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(f != f, NULL);
	
	/* Example for negative magnetic field. */
	rc = zsl_phy_magn_force_wire(1.5, 1.0, -4.3, 1.1, &f);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(f != f, NULL);
}

ZTEST(zsl_tests, test_phy_magn_torque_loop)
{
	int rc;
	zsl_real_t tor;

	rc = zsl_phy_magn_torque_loop(1.5, 10.0, 4.3, 1.1, &tor);
	zassert_true(rc == 0, NULL);
#ifdef CONFIG_ZSL_SINGLE_PRECISION
	zassert_true(val_is_equal(tor, 57.482874724, 1E-5), NULL);
#else
	zassert_true(val_is_equal(tor, 57.482874724, 1E-8), NULL);
#endif

	/* Example for negative area. */
	rc = zsl_phy_magn_torque_loop(1.5, -10.0, 4.3, 1.1, &tor);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(tor != tor, NULL);
	
	/* Example for negative magnetic field. */
	rc = zsl_phy_magn_torque_loop(1.5, 10.0, -4.3, 1.1, &tor);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(tor != tor, NULL);
}

ZTEST(zsl_tests, test_phy_magn_pot_dipole)
{
	int rc;
	zsl_real_t u;

	rc = zsl_phy_magn_pot_dipole(21.3, 4.3, 1.1, &u);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(u, -41.5448687614, 1E-6), NULL);

	/* Example for negative magnetic momentum. */
	rc = zsl_phy_magn_pot_dipole(-21.3, 4.3, 1.1, &u);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(u != u, NULL);
	
	/* Example for negative magnetic field. */
	rc = zsl_phy_magn_pot_dipole(21.3, -4.3, 1.1, &u);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(u != u, NULL);
}

ZTEST(zsl_tests, test_phy_magn_orb_radius)
{
	int rc;
	zsl_real_t r;

	rc = zsl_phy_magn_orb_radius(1.6, 13.9, 4.0, 5.6, &r);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(r, 12.1625, 1E-6), NULL);

	/* Example for negative mass. */
	rc = zsl_phy_magn_orb_radius(1.6, 13.9, 4.0, -5.6, &r);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(r != r, NULL);
	
	/* Example for zero charge. */
	rc = zsl_phy_magn_orb_radius(0.0, 13.9, 4.0, 5.6, &r);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(r != r, NULL);
	
	/* Example for zero magnetic field. */
	rc = zsl_phy_magn_orb_radius(1.6, 13.9, 0.0, 5.6, &r);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(r != r, NULL);
	
	/* Example for negative radius. */
	rc = zsl_phy_magn_orb_radius(1.6, -13.9, 4.0, 5.6, &r);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(r != r, NULL);
}

ZTEST(zsl_tests, test_phy_magn_flux)
{
	int rc;
	zsl_real_t fl;

	rc = zsl_phy_magn_flux(10.25, 4.3, 1.1, &fl);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(fl, 19.9922490518, 1E-6), NULL);

	/* Example for negative area. */
	rc = zsl_phy_magn_flux(-10.25, 4.3, 1.1, &fl);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(fl != fl, NULL);
	
	/* Example for negative magnetic field. */
	rc = zsl_phy_magn_flux(10.25, -4.3, 1.1, &fl);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(fl != fl, NULL);
}

ZTEST(zsl_tests, test_phy_magn_mom)
{
	int rc;
	zsl_real_t mom;

	rc = zsl_phy_magn_mom(1.7, 10.25, &mom);
	zassert_true(rc == 0, NULL);
#ifdef CONFIG_ZSL_SINGLE_PRECISION
	zassert_true(val_is_equal(mom, 17.425, 1E-5), NULL);
#else
	zassert_true(val_is_equal(mom, 17.425, 1E-8), NULL);
#endif

	/* Example for negative area. */
	rc = zsl_phy_magn_mom(1.7, -10.25, &mom);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(mom != mom, NULL);
}
