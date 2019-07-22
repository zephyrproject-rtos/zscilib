/*
 * Copyright (c) 2019 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <kernel.h>
#include <zephyr.h>
#include <zsl/zsl.h>
#include <zsl/physics/projectiles.h>

void projectiles_demo(void)
{
	printf("Physics projectile API demo\n");
	printf("---------------------------\n\n");

	/* Enter the initial values: */
	zsl_real_t vi = 12.4;   /* Initial velocity */
	zsl_real_t theta = 1.1; /* Angle */
	zsl_real_t xi = 2.2;    /* Initial horizontal position */
	zsl_real_t yi = 15;     /* Initial height */

	printf("A rock is lauched from a %lf-meter-tall building with a\n", yi);
	printf("velocity of %lf and an angle of %lf. Consider the\n", vi, theta);
	printf("x coordinate zero to be %lf meters behind the launching\n", xi);
	printf("point.\n\n");

	printf("(a) Decompose the initial velocity into its components.\n");

	zsl_real_t vih, viv;

	zsl_phy_proj_init_vel(vi, theta, &vih, &viv);

	printf("The horizontal component is: %lf.\n", vih);
	printf("The vertical component is: %lf.\n\n", viv);

	printf("(b) Calculate the time it takes the rock to reach a height\n"
	       "of 20 meters on its ascending phase.\n\n");

	zsl_real_t t;

	zsl_phy_proj_time_first(viv, yi, 20.0, &t);

	printf("It takes the rock %lf seconds to reach 20 meters.\n\n", t);

	printf("(c) How much time does it take the rock to reach\n");
	printf("the ground?\n\n");

	zsl_phy_proj_time(viv, yi, 0.0, &t);

	printf("It takes the rock %lf seconds to reach the ground.\n\n", t);

	printf("(d) How much time has transcurred when it reaches the\n");
	printf("maximum height?\n\n");

	/* At maximum height the vertical velocity is zero: */
	zsl_phy_proj_time2(viv, 0.0, &t);

	printf("It takes the rock %lf seconds to reach the top.\n\n", t);

	printf("(e) Calculate its position and total velocity 2 seconds\n");
	printf("after the launchment. What's the angle between its velocity\n");
	printf("and the ground at this moment?\n\n");

	zsl_real_t xf, yf, vfv, vf, thetaf;

	/* Calculates the position (xf, yf) at t = 2 seconds. */
	zsl_phy_proj_hor_motion(vih, 2.0, xi, &xf);
	zsl_phy_proj_ver_motion(viv, 2.0, yi, &yf);

	/* Calculates the vertical velocity at t = 2 seconds. */
	zsl_phy_proj_ver_vel(viv, 2.0, &vfv);

	/* Calculates the total velocity at t = 2 seconds. */
	zsl_phy_proj_vel(vih, vfv, &vf);

	/* Calculates the angle at t = 2 seconds. */
	zsl_phy_proj_angle(vih, vfv, &thetaf);

	printf("The position is (%lf, %lf) meters. The total\n", xf, yf);
	printf("velocity is %lf meters per second and the angle is\n", vf);
	printf("%lf radians\n\n.", thetaf);

	printf("(f) When the rock is 13.2 meters from the launcher, what is\n");
	printf("its height?\n\n");

	/* 13.2 meters from the point of launch means 13.2 + 2.2 = 15.4 meters
	 * from the origin. */

	zsl_phy_proj_trajectory(vih, viv, xi, yi, 15.4, &yf);

	printf("The height is %lf meters.\n\n", yf);

	printf("(g) What is the total horizontal distance it travels?\n\n");

	zsl_real_t dist;

	zsl_phy_proj_range(vih, viv, xi, yi, &dist);

	printf("The total range is %lf meters.\n\n", dist);
}

void main(void)
{
	printf("zscilib projectiles demo\n\n");

	projectiles_demo();

}
