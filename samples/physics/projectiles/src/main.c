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
#include <zsl/physics/projectiles.h>

void projectiles_demo(void)
{
	printf("Physics projectile API demo\n");
	printf("---------------------------\n\n");

	/* Initial values: */
	zsl_real_t vi = 12.4;   /* Initial velocity. */
	zsl_real_t theta = 1.1; /* Angle. */
	zsl_real_t xi = 2.2;    /* Initial horizontal position. */
	zsl_real_t yi = 15;     /* Initial height. */

	printf("A rock is lauched from a %.2f meter tall building with\n", yi);
	printf("a velocity of %f and an angle of %f. Consider\n", vi, theta);
	printf("the origin x coordinate to be %f meters behind the\n", xi);
	printf("launching point.\n\n");

	printf("(a) Decompose the initial velocity into its components.\n");

	zsl_real_t vih, viv;

	zsl_phy_proj_init_vel(vi, theta, &vih, &viv);

	printf("The horizontal component is: %f.\n", vih);
	printf("The vertical component is: %f.\n\n", viv);

	printf("(b) Calculate the time it takes the rock to reach a height\n");
	printf("of 20 meters on its ascending phase.\n\n");

	zsl_real_t t;

	zsl_phy_proj_time_first(viv, yi, 20.0, &t);

	printf("It takes the rock %f seconds to reach 20 meters.\n\n", t);

	printf("(c) How much time does it take the rock to reach\n");
	printf("the ground?\n\n");

	zsl_phy_proj_time(viv, yi, 0.0, &t);

	printf("It takes the rock %f seconds to reach the ground.\n\n", t);

	printf("(d) How much time has elapsed when it reaches the\n");
	printf("maximum height?\n\n");

	/* At maximum height the vertical velocity is zero: */
	zsl_phy_proj_time2(viv, 0.0, &t);

	printf("It takes the rock %f seconds to reach the peak height.\n\n", t);

	printf("(e) Calculate its position and total velocity 2 seconds\n");
	printf("after being launched. What's the angle between its velocity\n");
	printf("and the ground at that instant?\n\n");

	zsl_real_t xf, yf, vfv, vf, thetaf;

	/* Calculate the position (xf, yf) at t = 2 seconds. */
	zsl_phy_proj_hor_motion(vih, 2.0, xi, &xf);
	zsl_phy_proj_ver_motion(viv, 2.0, yi, &yf);

	/* Calculate the vertical velocity at t = 2 seconds. */
	zsl_phy_proj_ver_vel(viv, 2.0, &vfv);

	/* Calculate the total velocity at t = 2 seconds. */
	zsl_phy_proj_vel(vih, vfv, &vf);

	/* Calculate the angle at t = 2 seconds. */
	zsl_phy_proj_angle(vih, vfv, &thetaf);

	printf("The position is (%f, %f) meters. The total\n", xf, yf);
	printf("velocity is %f meters per second and the angle is\n", vf);
	printf("%f radians.\n\n", thetaf);

	printf("(f) When the rock is 13.2 meters from the launch position,\n");
	printf("what is its height?\n\n");

	/* 13.2 meters from the point of launch means 13.2 + 2.2 = 15.4 meters
	 * from the origin. */
	zsl_phy_proj_trajectory(vih, viv, xi, yi, 15.4, &yf);

	printf("The height is %f meters.\n\n", yf);

	printf("(g) What is the total horizontal distance it travels?\n\n");

	zsl_real_t dist;

	zsl_phy_proj_range(vih, viv, xi, yi, &dist);

	printf("The total range is %f meters.\n\n", dist);
}

void main(void)
{
	printf("\n\nzscilib projectiles demo\n\n");

	projectiles_demo();

}
