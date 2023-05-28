/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/vectors.h>
#include <zsl/physics/energy.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_phy_ener_kin)
{
	int rc;
	zsl_real_t ke;

	rc = zsl_phy_ener_kin(5.0, 4.0, &ke);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(ke, 50.0, 1E-6), NULL);

	/* Example for negative mass. */
	rc = zsl_phy_ener_kin(5.0, -4.0, &ke);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(ke != ke, NULL);
}

ZTEST(zsl_tests, test_phy_ener_rot)
{
	int rc;
	zsl_real_t rke;

	rc = zsl_phy_ener_rot(3.0, 1.5, &rke);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(rke, 6.75, 1E-6), NULL);

	/* Example for negative moment of inertia. */
	rc = zsl_phy_ener_rot(3.0, -1.5, &rke);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(rke != rke, NULL);
}

ZTEST(zsl_tests, test_phy_ener_grav_pot)
{
	int rc;
	zsl_real_t ug;

	rc = zsl_phy_ener_grav_pot(5.0, 30.0, &ug);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(ug, 1471.05, 1E-6), NULL);

	/* Example for negative mass. */
	rc = zsl_phy_ener_grav_pot(-5.0, 30.0, &ug);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(ug != ug, NULL);
}

ZTEST(zsl_tests, test_phy_ener_elas_pot)
{
	int rc;
	zsl_real_t ue;

	rc = zsl_phy_ener_elas_pot(5.0, 30.0, &ue);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(ue, 2250.0, 1E-6), NULL);

	/* Example for negative k. */
	rc = zsl_phy_ener_elas_pot(-5.0, 30.0, &ue);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(ue != ue, NULL);
}

ZTEST(zsl_tests, test_phy_ener_power)
{
	int rc;
	zsl_real_t power;

	rc = zsl_phy_ener_power(1500.0, 30.0, &power);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(power, 50.0, 1E-6), NULL);

	/* Example for zero time. */
	rc = zsl_phy_ener_power(1500.0, 0.0, &power);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(power != power, NULL);

	/* Example for negative time. */
	rc = zsl_phy_ener_power(1500.0, -30.0, &power);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(power != power, NULL);
}

ZTEST(zsl_tests, test_phy_ener_fric)
{
	int rc;
	zsl_real_t e;

	rc = zsl_phy_ener_fric(50.0, 3.0, &e);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(e, -150.0, 1E-6), NULL);

	/* Example for negative friction force. */
	rc = zsl_phy_ener_fric(-50.0, 3.0, &e);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(e != e, NULL);

	/* Example for negative distance. */
	rc = zsl_phy_ener_fric(50.0, -3.0, &e);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(e != e, NULL);
}

ZTEST(zsl_tests, test_phy_ener_mec)
{
	int rc;
	zsl_real_t me;

	rc = zsl_phy_ener_mec(500.0, 1800.0, 6650.0, 403.0, &me);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(me, 9353.0, 1E-6), NULL);
}

ZTEST(zsl_tests, test_phy_ener_final)
{
	int rc;
	zsl_real_t ef4;

	rc = zsl_phy_ener_final(500.0, 1800.0, 6650.0, 403.0, 306.0, 2340.0,
				3900.0, &ef4);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(ef4, 2807.0, 1E-6), NULL);
}

ZTEST(zsl_tests, test_phy_ener_photon)
{
	int rc;
	zsl_real_t e;

	/* Green light: frequency = 560 Teraherzs = 5.6e14 herzs. */
	rc = zsl_phy_ener_photon(5.6E14, &e);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(e, 2.3159739103, 1E-6), NULL);

	/* Example for negative frequency. */
	rc = zsl_phy_ener_photon(-5.6E14, &e);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(e != e, NULL);
}

ZTEST(zsl_tests, test_phy_ener_photon2)
{
	int rc;
	zsl_real_t e;

	/* Green light: wavelength = 550 nanometers. */
	rc = zsl_phy_ener_photon2(550.0, &e);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(e, 2.2542581533, 1E-6), NULL);

	/* Example for zero wavelenght. */
	rc = zsl_phy_ener_photon2(0.0, &e);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(e != e, NULL);

	/* Example for negative wavelenght. */
	rc = zsl_phy_ener_photon2(-5.5E-7, &e);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(e != e, NULL);
}
