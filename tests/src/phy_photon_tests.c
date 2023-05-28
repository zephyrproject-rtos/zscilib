/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/vectors.h>
#include <zsl/physics/photons.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_phy_photon_ener)
{
	int rc;
	zsl_real_t e;
	
	/* Green light: wavelength = 550 nanometers = 5.5e-7 meters. */
	rc = zsl_phy_photon_ener(5.5E-7, &e);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(e, 2.2542581533, 1E-6), NULL);

	/* Example for negative wavelength. */
	rc = zsl_phy_photon_ener(-5.5E-7, &e);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(e != e, NULL);
	
	/* Example for zero wavelength. */
	rc = zsl_phy_photon_ener(0.0, &e);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(e != e, NULL);
}

ZTEST(zsl_tests, test_phy_photon_mom)
{
	int rc;
	zsl_real_t p;
	
	rc = zsl_phy_photon_mom(5.5E-12, &p);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(p, 0.0007519396, 1E-6), NULL);

	/* Example for negative wavelength. */
	rc = zsl_phy_photon_mom(-5.5E-12, &p);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(p != p, NULL);
	
	/* Example for zero wavelength. */
	rc = zsl_phy_photon_mom(0.0, &p);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(p != p, NULL);
}

ZTEST(zsl_tests, test_phy_photon_wavelenght)
{
	int rc;
	zsl_real_t lambda;
	
	rc = zsl_phy_photon_wavelength(5.5E8, &lambda);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(lambda, 0.5450771964, 1E-6), NULL);

	/* Example for negative frequency. */
	rc = zsl_phy_photon_wavelength(-5.5E8, &lambda);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(lambda != lambda, NULL);
	
	/* Example for zero frequency. */
	rc = zsl_phy_photon_wavelength(0.0, &lambda);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(lambda != lambda, NULL);
}

ZTEST(zsl_tests, test_phy_photon_frequency)
{
	int rc;
	zsl_real_t f;
	
	rc = zsl_phy_photon_frequency(5.5E4, &f);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(f, 5450.7719636363, 1E-6), NULL);

	/* Example for negative wavelength. */
	rc = zsl_phy_photon_frequency(-5.5E8, &f);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(f != f, NULL);
	
	/* Example for zero wavelength. */
	rc = zsl_phy_photon_frequency(0.0, &f);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(f != f, NULL);
}

ZTEST(zsl_tests, test_phy_photon_photoelectric)
{
	int rc;
	zsl_real_t kmax;
	
	rc = zsl_phy_photon_photoelectric(5.5E14, 1.3, &kmax);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(kmax, 0.9746172333, 1E-6), NULL);

	/* Example for negative frequency. */
	rc = zsl_phy_photon_photoelectric(-5.5E14, 1.3, &kmax);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(kmax != kmax, NULL);
	
	/* Example for negative work function. */
	rc = zsl_phy_photon_photoelectric(5.5E14, -1.3, &kmax);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(kmax != kmax, NULL);
}
