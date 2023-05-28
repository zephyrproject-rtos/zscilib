/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/vectors.h>
#include <zsl/physics/projectiles.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_phy_proj_init_vel)
{
	int rc;
	zsl_real_t vi_hor;
	zsl_real_t vi_ver;

	rc = zsl_phy_proj_init_vel(10.0, ZSL_PI / 6, &vi_hor, &vi_ver);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(vi_hor, 8.660254038, 1E-6), NULL);
	zassert_true(val_is_equal(vi_ver, 5.0, 1E-6), NULL);
}

ZTEST(zsl_tests, test_phy_proj_time)
{
	int rc;
	zsl_real_t t;

	/* Example where the projectile only reaches the final height once
	 * descending), i.e. the first value for the time is negative and is
	 * set to NAN. */
	rc = zsl_phy_proj_time(8.0, 20.0, 10.0, &t);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(t, 2.460370769, 1E-6), NULL);

	/* Example where the projectile reaches the final height twice
	 * (ascending and descending), i.e. there are two values for the
	 * time. */
	rc = zsl_phy_proj_time(8.0, 10.0, 12.0, &t);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(t, 1.323253572, 1E-6), NULL);

	/* Example where the projectile never reaches the final height. Both
	 * times (ascending and descending) are set to NAN. */
	rc = zsl_phy_proj_time(8.0, 20.0, 30.0, &t);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(t != t, NULL);
}

ZTEST(zsl_tests, test_phy_proj_time_first)
{
	int rc;
	zsl_real_t t;

	/* Example where the projectile only reaches the final height once
	 * descending), i.e. the first value for the time is negative and is
	 * set to NAN. */
	rc = zsl_phy_proj_time_first(8.0, 20.0, 10.0, &t);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(t != t, NULL);

	/* Example where the projectile reaches the final height twice
	 * (ascending and descending), i.e. there are two values for the
	 * time. */
	rc = zsl_phy_proj_time_first(8.0, 10.0, 12.0, &t);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(t, 0.3082341412, 1E-6), NULL);

	/* Example where the projectile never reaches the final height. Both
	 * times (ascending and descending) are set to NAN. */
	rc = zsl_phy_proj_time_first(8.0, 20.0, 30.0, &t);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(t != t, NULL);
}

ZTEST(zsl_tests, test_phy_proj_time2)
{
	int rc;
	zsl_real_t t;

	rc = zsl_phy_proj_time2(20.0, -30.0, &t);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(t, 5.098399103, 1E-6), NULL);

	rc = zsl_phy_proj_time2(20.0, 30.0, &t);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(t != t, NULL);
}

ZTEST(zsl_tests, test_phy_proj_ver_motion)
{
	int rc;
	zsl_real_t yf;

	rc = zsl_phy_proj_ver_motion(20.0, 2.0, 5.0, &yf);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(yf, 25.386, 1E-6), NULL);

	rc = zsl_phy_proj_ver_motion(20.0, -2.0, 5.0, &yf);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(yf != yf, NULL);
}

ZTEST(zsl_tests, test_phy_proj_ver_vel)
{
	int rc;
	zsl_real_t vfv;

	rc = zsl_phy_proj_ver_vel(20.0, 2.0, &vfv);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(vfv, 0.386, 1E-6), NULL);

	rc = zsl_phy_proj_ver_vel(20.0, -2.0, &vfv);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vfv != vfv, NULL);
}

ZTEST(zsl_tests, test_phy_proj_hor_motion)
{
	int rc;
	zsl_real_t xf;

	rc = zsl_phy_proj_hor_motion(2.0, 5.0, 10.0, &xf);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(xf, 20.0, 1E-6), NULL);

	rc = zsl_phy_proj_hor_motion(2.0, -5.0, 10.0, &xf);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(xf != xf, NULL);
}

ZTEST(zsl_tests, test_phy_proj_trajectory)
{
	int rc;
	zsl_real_t yf;

	rc = zsl_phy_proj_trajectory(5.0, 4.0, 10.0, 12.0, 15.0, &yf);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(yf, 11.0965, 1E-6), NULL);

	/* Example where the horizontal velocity is zero. */
	rc = zsl_phy_proj_trajectory(0.0, 4.0, 10.0, 12.0, 15.0, &yf);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(yf != yf, NULL);

	/* Example where the time is negative. */
	rc = zsl_phy_proj_trajectory(5.0, 4.0, 10.0, 12.0, 5.0, &yf);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(yf != yf, NULL);
}

ZTEST(zsl_tests, test_phy_proj_vel)
{
	int rc;
	zsl_real_t vf;

	rc = zsl_phy_proj_vel(3.0, 4.0, &vf);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(vf, 5.0, 1E-6), NULL);
}

ZTEST(zsl_tests, test_phy_proj_angle)
{
	int rc;
	zsl_real_t theta;

	rc = zsl_phy_proj_angle(3.0, 4.0, &theta);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(theta, 0.927295218, 1E-6), NULL);

	/* Example where the horizontal velocity is zero. */
	rc = zsl_phy_proj_angle(0.0, 4.0, &theta);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(theta, (ZSL_PI / 2), 1E-6), NULL);
}

ZTEST(zsl_tests, test_phy_proj_range)
{
	int rc;
	zsl_real_t dist;

	rc = zsl_phy_proj_range(7.0, 12.0, 3.0, 15.0, &dist);
	zassert_true(rc == 0, NULL);
#ifdef CONFIG_ZSL_SINGLE_PRECISION
 	zassert_true(val_is_equal(dist, 23.50711351, 1E-5), NULL);
#else
	zassert_true(val_is_equal(dist, 23.50711351, 1E-8), NULL);
#endif
}
