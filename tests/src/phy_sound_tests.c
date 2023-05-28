/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/vectors.h>
#include <zsl/physics/sound.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_phy_sound_press_amp)
{
	int rc;
	zsl_real_t p;

	rc = zsl_phy_sound_press_amp(2.15E9, 6.1E-4, 5.1E-3, &p);
	zassert_true(rc == 0, NULL);
#ifdef CONFIG_ZSL_SINGLE_PRECISION
	zassert_true(val_is_equal(p, 6688.65, 1E-3), NULL);
#else
	zassert_true(val_is_equal(p, 6688.65, 1E-8), NULL);
#endif

	/* Example for negative Bulk Modulus. */
	rc = zsl_phy_sound_press_amp(-2.15E9, 6.1E-4, 5.1E-3, &p);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(p != p, NULL);
	
	/* Example for negative wave number. */
	rc = zsl_phy_sound_press_amp(2.15E9, -6.1E-4, 5.1E-3, &p);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(p != p, NULL);
	
	/* Example for negative amplitude. */
	rc = zsl_phy_sound_press_amp(2.15E9, 6.1E-4, -5.1E-3, &p);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(p != p, NULL);
}

ZTEST(zsl_tests, test_phy_sound_level)
{
	int rc;
	zsl_real_t b;

	rc = zsl_phy_sound_level(1.7E5, 3.2E2, &b);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(b, 27.2529894306, 1E-6), NULL);

	/* Example for negative sound intensity. */
	rc = zsl_phy_sound_level(-1.7E5, 3.2E2, &b);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(b != b, NULL);
	
	/* Example for negative reference sound intensity. */
	rc = zsl_phy_sound_level(1.7E5, -3.2E2, &b);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(b != b, NULL);
}

ZTEST(zsl_tests, test_phy_sound_intensity)
{
	int rc;
	zsl_real_t i;

	rc = zsl_phy_sound_intensity(2.15E9, 997.6, 7851.5, &i);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(i, 21.0464001945, 1E-6), NULL);

	/* Example for negative Bulk Modulus. */
	rc = zsl_phy_sound_intensity(-2.15E9, 997.6, 7851.5, &i);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(i != i, NULL);
	
	/* Example for zero density. */
	rc = zsl_phy_sound_intensity(2.15E9, 0.0, 7851.5, &i);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(i != i, NULL);
	
	/* Example for negative density. */
	rc = zsl_phy_sound_intensity(2.15E9, -997.6, 7851.5, &i);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(i != i, NULL);
	
	/* Example for negative maximum pressure. */
	rc = zsl_phy_sound_intensity(2.15E9, 997.6, -7851.5, &i);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(i != i, NULL);
}

ZTEST(zsl_tests, test_phy_sound_shock_wave_angle)
{
	int rc;
	zsl_real_t theta;

	/* Example for velocities with same direction. */
	rc = zsl_phy_sound_shock_wave_angle(302.2, 505.1, &theta);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(theta, 0.6413745117, 1E-6), NULL);
	
	/* Example for velocities with opposite direction. */
	rc = zsl_phy_sound_shock_wave_angle(-302.2, 505.1, &theta);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(theta, -0.6413745117, 1E-6), NULL);
}

ZTEST(zsl_tests, test_phy_sound_dop_effect)
{
	int rc;
	zsl_real_t fl;

	rc = zsl_phy_sound_dop_effect(54.1, 33.9, 81.2, 556.0, &fl);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(fl, 854.85, 1E-6), NULL);

	/* Example for negative source frequency. */
	rc = zsl_phy_sound_dop_effect(54.1, 33.9, 81.2, -556.0, &fl);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(fl != fl, NULL);
	
	/* Example for (v + vs) = 0 m/s. */
	rc = zsl_phy_sound_dop_effect(0.0, 0.0, 81.2, 556.0, &fl);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(fl != fl, NULL);
	
	/* Example for negative listener intensity. */
	rc = zsl_phy_sound_dop_effect(54.1, -73.9, 81.2, 556.0, &fl);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(fl != fl, NULL);
}

ZTEST(zsl_tests, test_phy_sound_beat)
{
	int rc;
	zsl_real_t f;

	/* Example for 'fa' > 'fb'. */
	rc = zsl_phy_sound_beat(563.8, 541.3, &f);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(f, 22.5, 1E-6), NULL);
	
	/* Example for 'fa' < 'fb'. */
	rc = zsl_phy_sound_beat(563.8, 697.9, &f);
	zassert_true(rc == 0, NULL);
#ifdef CONFIG_ZSL_SINGLE_PRECISION
	zassert_true(val_is_equal(f, 134.1, 1E-4), NULL);
#else
	zassert_true(val_is_equal(f, 134.1, 1E-8), NULL);
#endif

	/* Example for negative 'fa'. */
	rc = zsl_phy_sound_beat(-563.8, 541.3, &f);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(f != f, NULL);
	
	/* Example for negative 'fb'. */
	rc = zsl_phy_sound_beat(563.8, -541.3, &f);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(f != f, NULL);
}
