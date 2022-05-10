/*
 * Copyright (c) 2019 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <zephyr/kernel.h>
#include <zephyr/kernel.h>
#include <zsl/zsl.h>
#include <zsl/physics/kinematics.h>

void kinematics_demo(void)
{
	printf("Physics kinematics API demo\n");
	printf("---------------------------\n\n");

	zsl_real_t a = 4.8;        /* Initial acceleration. */
	zsl_real_t m = 1500.0;     /* Average mass of a car. */

	printf("A car of %f kg of mass, at rest, starts moving with an\n", m);
	printf("acceleration of %f meters per second squared.\n\n", a);

	printf("(a) Calculate its velocity after 5 seconds.\n\n");

	zsl_real_t vel;

	zsl_phy_kin_vel(0.0, 5.0, a, &vel);

	printf("The velocity at this moment is %f meters per second.\n\n", vel);

	printf("(b) How much time has elapsed when the car has a velocity\n");
	printf("of 42 meters per second? What distance has it travelled?\n");
	printf("Calculate the average velocity of this small journey.\n\n");

	zsl_real_t t, dist;

	zsl_phy_kin_time(0.0, 42.0, a, &t);

	zsl_phy_kin_dist(0.0, 5.0, a, &dist);

	zsl_phy_kin_av_vel(dist, t, &vel);

	printf("%f seconds have elapsed since the car started moving.\n", t);
	printf("It has moved %f meters and its average velocity is\n", dist);
	printf("%f meters per second.\n\n", vel);

	printf("(c) The driver travels 28 more meters and sees someone\n");
	printf("crossing the road, so he starts stopping the car. Calculate\n");
	printf("the velocity and kinetic energy of the car in kilojoules\n");
	printf("before the driver starts braking.\n\n");

	zsl_real_t ke;

	zsl_phy_kin_vel2(42.0, 28.0, a, &vel);

	zsl_phy_kin_ener(vel, m, &ke);

	ke /= 1E3;

	printf("The velocity of the car is %f meters per second and\n", vel);
	printf("its kinetic energy is %f joules right before braking.\n\n", ke);

	printf("(d) The car stops completely in 3 seconds. Calculate its\n");
	printf("acceleration (suppose it is constant) when it is braking.\n\n");

	zsl_real_t accel;

	zsl_phy_kin_accel(vel, 0.0, 3.0, &accel);

	printf("Its acceleration is %f meters per second squared.\n\n", accel);
}

void main(void)
{
	printf("\n\nzscilib kinematics demo\n\n");

	kinematics_demo();
}
