/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/vectors.h>
#include <zsl/physics/kinematics.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_phy_kin_dist)
{
	int rc;
	zsl_real_t dist;

	rc = zsl_phy_kin_dist(15.0, 5.0, -2.0, &dist);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(dist, 50.0, 1E-6), NULL);
	
	/* Example for negative time. */
	rc = zsl_phy_kin_dist(15.0, -1.0, -2.0, &dist);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(dist != dist, NULL);
}

ZTEST(zsl_tests, test_phy_kin_init_pos)
{
	int rc;
	zsl_real_t xi;

	rc = zsl_phy_kin_init_pos(15.0, 5.0, -2.0, 70.0, &xi);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(xi, 20.0, 1E-6), NULL);
	
	/* Example for negative time. */
	rc = zsl_phy_kin_init_pos(15.0, -1.0, -2.0, 70.0, &xi);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(xi != xi, NULL);
}

ZTEST(zsl_tests, test_phy_kin_init_pos2)
{
	int rc;
	zsl_real_t xi;

	rc = zsl_phy_kin_init_pos2(10.0, 12.0, 2.0, 20.0, &xi);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(xi, 9.0, 1E-6), NULL);
	
	/* Example for zero acceleration. */
	rc = zsl_phy_kin_init_pos2(10.0, 12.0, 0.0, 20.0, &xi);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(xi != xi, NULL);
}

ZTEST(zsl_tests, test_phy_kin_time)
{
	int rc;
	zsl_real_t time;

	rc = zsl_phy_kin_time(15.0, 10.0, -5.0, &time);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(time, 1.0, 1E-6), NULL);

	/* Example for negative time. */
	rc = zsl_phy_kin_time(15.0, 10.0, 5.0, &time);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(time != time, NULL);

	/* Example for zero acceleration. */
	rc = zsl_phy_kin_time(15.0, 10.0, 0.0, &time);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(time != time, NULL);
}

ZTEST(zsl_tests, test_phy_kin_vel)
{
	int rc;
	zsl_real_t vf;

	rc = zsl_phy_kin_vel(30.0, 10.0, -5.0, &vf);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(vf, -20.0, 1E-6), NULL);

	/* Example for negative time. */
	rc = zsl_phy_kin_vel(30.0, -10.0, -5.0, &vf);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vf != vf, NULL);
}

ZTEST(zsl_tests, test_phy_kin_vel2)
{
	int rc;
	zsl_real_t vf;

	rc = zsl_phy_kin_vel2(5.0, 12.0, 6.0, &vf);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(vf, 13.0, 1E-6), NULL);
	
	/* Example for complex final velocity. */
	rc = zsl_phy_kin_vel2(5.0, 12.0, -6.0, &vf);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vf != vf, NULL);
}

ZTEST(zsl_tests, test_phy_kin_init_vel)
{
	int rc;
	zsl_real_t vi;

	rc = zsl_phy_kin_init_vel(15.0, 6.0, 2.0, &vi);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(vi, 3.0, 1E-6), NULL);

	/* Example for negative time. */
	rc = zsl_phy_kin_init_vel(15.0, 6.0, -2.0, &vi);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vi != vi, NULL);
}

ZTEST(zsl_tests, test_phy_kin_init_vel2)
{
	int rc;
	zsl_real_t vi;

	rc = zsl_phy_kin_init_vel2(24.0, 2.0, 4.0, &vi);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(vi, 2.0, 1E-6), NULL);

	/* Example for negative time. */
	rc = zsl_phy_kin_init_vel2(24.0, 2.0, -4.0, &vi);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vi != vi, NULL);
	
	/* Example for zero time. */
	rc = zsl_phy_kin_init_vel2(24.0, 2.0, 0.0, &vi);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vi != vi, NULL);
}

ZTEST(zsl_tests, test_phy_kin_init_vel3)
{
	int rc;
	zsl_real_t vi;

	rc = zsl_phy_kin_init_vel3(4.0, 2.0, 3.0, &vi);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(vi, 2.0, 1E-6), NULL);

	/* Example for complex initial velocity. */
	rc = zsl_phy_kin_init_vel3(4.0, 4.0, 3.0, &vi);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vi != vi, NULL);
}

ZTEST(zsl_tests, test_phy_kin_av_vel)
{
	int rc;
	zsl_real_t vel;

	rc = zsl_phy_kin_av_vel(15.0, 5.0, &vel);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(vel, 3.0, 1E-6), NULL);

	/* Example for zero time. */
	rc = zsl_phy_kin_av_vel(15.0, 0.0, &vel);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vel != vel, NULL);

	/* Example for negative time. */
	rc = zsl_phy_kin_av_vel(15.0, -5.0, &vel);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vel != vel, NULL);
}

ZTEST(zsl_tests, test_phy_kin_accel)
{
	int rc;
	zsl_real_t accel;

	rc = zsl_phy_kin_accel(15.0, -5.0, 4.0, &accel);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(accel, -5.0, 1E-6), NULL);

	/* Example for zero time. */
	rc = zsl_phy_kin_accel(15.0, -5.0, 0.0, &accel);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(accel != accel, NULL);

	/* Example for negative time. */
	rc = zsl_phy_kin_accel(15.0, -5.0, -4.0, &accel);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(accel != accel, NULL);
}

ZTEST(zsl_tests, test_phy_kin_ener)
{
	int rc;
	zsl_real_t ke;

	rc = zsl_phy_kin_ener(10.0, 1.1, &ke);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(ke, 55.0, 1E-6), NULL);

	/* Example for negative mass. */
	rc = zsl_phy_kin_ener(10.0, -1.1, &ke);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(ke != ke, NULL);
}
