/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/vectors.h>
#include <zsl/physics/rotation.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_phy_rot_angle)
{
	int rc;
	zsl_real_t phi;

	rc = zsl_phy_rot_angle(1.1, 3.1, 0.6, &phi);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(phi, 6.293, 1E-6), NULL);

	/* Example for negative time. */
	rc = zsl_phy_rot_angle(1.1, -3.1, 0.6, &phi);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(phi != phi, NULL);
}

ZTEST(zsl_tests, test_phy_rot_dist)
{
	int rc;
	zsl_real_t dist;

	rc = zsl_phy_rot_dist(1.1, 6.0, &dist);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(dist, 6.6, 1E-6), NULL);

	/* Example for negative radius. */
	rc = zsl_phy_rot_dist(1.1, -6.0, &dist);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(dist != dist, NULL);
}

ZTEST(zsl_tests, test_phy_rot_turn)
{
	int rc;
	zsl_real_t turn;

	rc = zsl_phy_rot_turn(1.1, &turn);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(turn, 0.1750704374, 1E-6), NULL);
}

ZTEST(zsl_tests, test_phy_rot_time)
{
	int rc;
	zsl_real_t time;

	rc = zsl_phy_rot_time(3.1, 5.7, 2.0, &time);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(time, 1.3, 1E-6), NULL);
	
	/* Example for negative angular acceleration. */
	rc = zsl_phy_rot_time(4.1, 3.1, -2.0, &time);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(time, 0.5, 1E-6), NULL);
	
	/* Example for zero angular acceleration. */
	rc = zsl_phy_rot_time(4.1, 3.1, 0.0, &time);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(time != time, NULL);

	/* Example for negative time. */
	rc = zsl_phy_rot_time(4.1, 3.1, 2.3, &time);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(time != time, NULL);
}

ZTEST(zsl_tests, test_phy_rot_omega)
{
	int rc;
	zsl_real_t of;

	rc = zsl_phy_rot_omega(3.0, 5.0, 2.5, &of);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(of, 15.5, 1E-6), NULL);
	
	/* Example for negative time. */
	rc = zsl_phy_rot_omega(3.0, -5.0, 2.5, &of);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(of != of, NULL);
}

ZTEST(zsl_tests, test_phy_rot_omega2)
{
	int rc;
	zsl_real_t of;

	rc = zsl_phy_rot_omega2(3.0, 5.0, 2.5, &of);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(of, 5.8309518948, 1E-6), NULL);
	
	/* Example for complex final angular velocity. */
	rc = zsl_phy_rot_omega2(3.0, 5.0, -2.5, &of);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(of != of, NULL);
}

ZTEST(zsl_tests, test_phy_rot_av_omega)
{
	int rc;
	zsl_real_t omega;

	rc = zsl_phy_rot_av_omega(1.0, 2.0, &omega);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(omega, 0.5, 1E-6), NULL);
	
	/* Example for zero time. */
	rc = zsl_phy_rot_av_omega(1.0, 0.0, &omega);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(omega != omega, NULL);
	
	/* Example for negative time. */
	rc = zsl_phy_rot_av_omega(1.0, -2.0, &omega);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(omega != omega, NULL);
}

ZTEST(zsl_tests, test_phy_rot_vel)
{
	int rc;
	zsl_real_t vel;

	rc = zsl_phy_rot_vel(3.2, 6.0, &vel);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(vel, 19.2, 1E-6), NULL);

	/* Example for negative radius. */
	rc = zsl_phy_rot_vel(3.2, -6.0, &vel);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(vel != vel, NULL);
}

ZTEST(zsl_tests, test_phy_rot_ang_accel)
{
	int rc;
	zsl_real_t a;

	rc = zsl_phy_rot_ang_accel(1.0, 2.0, 4.0, &a);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(a, 0.25, 1E-6), NULL);
	
	/* Example for negative angular acceleration. */
	rc = zsl_phy_rot_ang_accel(4.5, 2.1, 1.2, &a);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(a, -2.0, 1E-6), NULL);
	
	/* Example for zero time. */
	rc = zsl_phy_rot_ang_accel(1.0, 2.0, 0.0, &a);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(a != a, NULL);
	
	/* Example for negative time. */
	rc = zsl_phy_rot_ang_accel(1.0, 2.0, -4.0, &a);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(a != a, NULL);
}

ZTEST(zsl_tests, test_phy_rot_accel)
{
	int rc;
	zsl_real_t accel;

	rc = zsl_phy_rot_accel(1.5, 6.0, &accel);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(accel, 9.0, 1E-6), NULL);

	/* Example for negative radius. */
	rc = zsl_phy_rot_accel(1.5, -6.0, &accel);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(accel != accel, NULL);
}

ZTEST(zsl_tests, test_phy_rot_ener)
{
	int rc;
	zsl_real_t rke;

	rc = zsl_phy_rot_ener(6.2, 4.1, &rke);
	zassert_true(rc == 0, NULL);
#ifdef CONFIG_ZSL_SINGLE_PRECISION
 	zassert_true(val_is_equal(rke, 78.802, 1E-5), NULL);
#else
        zassert_true(val_is_equal(rke, 78.802, 1E-8), NULL);
#endif
	
	/* Example for zero moment of inertia. */
	rc = zsl_phy_rot_ener(6.2, 0.0, &rke);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(rke, 0.0, 1E-6), NULL);

	/* Example for negative moment of inertia. */
	rc = zsl_phy_rot_ener(6.2, -4.1, &rke);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(rke != rke, NULL);
}

ZTEST(zsl_tests, test_phy_rot_period)
{
	int rc;
	zsl_real_t t;

	rc = zsl_phy_rot_period(6.2, &t);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(t, 1.0134169850, 1E-6), NULL);
	
	/* Example for zero angular velocity. */
	rc = zsl_phy_rot_period(0.0, &t);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(t != t, NULL);

	/* Example for negative angular velocity. */
	rc = zsl_phy_rot_period(-6.2, &t);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(t != t, NULL);
}

ZTEST(zsl_tests, test_phy_rot_frequency)
{
	int rc;
	zsl_real_t f;

	rc = zsl_phy_rot_frequency(6.2, &f);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(f, 0.9867606472, 1E-6), NULL);
	
	/* Example for zero angular velocity. */
	rc = zsl_phy_rot_frequency(0.0, &f);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(f, 0.0, 1E-6), NULL);

	/* Example for negative angular velocity. */
	rc = zsl_phy_rot_frequency(-6.2, &f);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(f != f, NULL);
}

ZTEST(zsl_tests, test_phy_rot_cent_accel)
{
	int rc;
	zsl_real_t ca;

	rc = zsl_phy_rot_cent_accel(6.1, 4.2, &ca);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(ca, 8.8595238095, 1E-6), NULL);
	
	/* Example for zero tangencial velocity. */
	rc = zsl_phy_rot_cent_accel(0.0, 4.2, &ca);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(ca, 0.0, 1E-6), NULL);

	/* Example for zero radius. */
	rc = zsl_phy_rot_cent_accel(6.1, 0.0, &ca);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(ca != ca, NULL);
	
	/* Example for negative radius. */
	rc = zsl_phy_rot_cent_accel(6.1, -4.2, &ca);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(ca != ca, NULL);
}

ZTEST(zsl_tests, test_phy_rot_cent_accel2)
{
	int rc;
	zsl_real_t ca;

	rc = zsl_phy_rot_cent_accel2(1.2, 4.2, &ca);
	zassert_true(rc == 0, NULL);
#ifdef CONFIG_ZSL_SINGLE_PRECISION
 	zassert_true(val_is_equal(ca, 115.1453846794, 1E-5), NULL);
#else
        zassert_true(val_is_equal(ca, 115.1453846794, 1E-8), NULL);
#endif
	
	/* Example for zero radius. */
	rc = zsl_phy_rot_cent_accel2(1.2, 0.0, &ca);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(ca, 0.0, 1E-6), NULL);
	
	/* Example for negative radius. */
	rc = zsl_phy_rot_cent_accel2(1.2, -4.2, &ca);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(ca != ca, NULL);

	/* Example for zero period. */
	rc = zsl_phy_rot_cent_accel2(0.0, 4.2, &ca);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(ca != ca, NULL);
	
	/* Example for negative period. */
	rc = zsl_phy_rot_cent_accel2(-1.2, 4.2, &ca);
	zassert_true(rc == -EINVAL, NULL);
	/* IEEE standard states that x != x is true only for NAN values. */
	zassert_true(ca != ca, NULL);
}

ZTEST(zsl_tests, test_phy_rot_total_accel)
{
	int rc;
	zsl_real_t at;

	rc = zsl_phy_rot_total_accel(3.8, 5.5, &at);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(at, 6.6850579653, 1E-6), NULL);
	
	/* Example for negative accelerations. */
	rc = zsl_phy_rot_total_accel(-3.8, -5.5, &at);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(at, 6.6850579653, 1E-6), NULL);
	
	/* Example for zero tangencial acceleration. */
	rc = zsl_phy_rot_total_accel(0.0, 5.5, &at);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(at, 5.5, 1E-6), NULL);
	
	/* Example for zero centripetal acceleration. */
	rc = zsl_phy_rot_total_accel(3.8, 0.0, &at);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(at, 3.8, 1E-6), NULL);
}

ZTEST(zsl_tests, test_phy_rot_power)
{
	int rc;
	zsl_real_t power;

	rc = zsl_phy_rot_power(6.7, 5.1, &power);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(power, 34.17, 1E-6), NULL);
}
