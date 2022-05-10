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
#include <zsl/physics/momentum.h>

void momentum_demo(void)
{
	printf("Physics momentum API demo\n");
	printf("-------------------------\n\n");

	zsl_real_t m1 = 2.0;     /* Mass of the first ball. */
	zsl_real_t m2 = 7.1;     /* Mass of the second ball. */

	printf("A ball of %f kg of mass is moving with a velocity of 5\n", m1);
	printf("meters per second.\n\n");

	printf("(a) Calculate its momentum.\n\n");

	zsl_real_t p;

	zsl_phy_mom_mom(m1, 5.0, &p);

	printf("The momentum of the ball is %f kilograms and meters\n", p);
	printf("per second.\n\n");

	printf("(b) The ball is accelerated with a force of 4.1 newtons\n");
	printf("during 12 seconds. Calculate the impulse experienced by the\n");
	printf("ball.\n\n");

	zsl_real_t imp;

	zsl_phy_mom_imp(4.1, 12.0, &imp);

	printf("The impulse is %f newtons and meters.\n\n", imp);

	printf("(c) The ball then starts decelerating due to friction with\n");
	printf("the ground until it reaches a velocity of 3.4 meters per\n");
	printf("second. Determine the amount of momentum lost.\n\n");

	zsl_real_t v, pt;

	/* Since the impulse equals the gain in momentum, before starting to
	 * decelerate, the ball has a momentum of p + imp. */
	p += imp;

	/* Calculate the initial velocity before decelerating. */
	v = p / m1;

	zsl_phy_mom_change(m1, v, 3.4, &pt);

	printf("The ball loses %f kilograms and meters per second.\n\n", pt);

	printf("(d) With this velocity, the ball collides with another ball\n");
	printf("of mass %f kg and velocity -5.1 meters per second.\n", m2);
	printf("Calculate the velocity of the second ball after the\n");
	printf("collision knowing that the final velocity of the first ball\n");
	printf("is -3.1 m/s and assuming it was an elastic collision.\n\n");

	zsl_real_t vf2;

	zsl_phy_mom_elas(m1, 3.4, -3.1, m2, -5.1, &vf2);

	printf("The final velocity of the second ball is %f m/s.\n\n", vf2);

	printf("(e) Suppose now that the collision was perfectly inelastic\n");
	printf("and the two balls merged into one. What would be the final\n");
	printf("velocity of the merged ball?\n\n");

	zsl_real_t vf;

	zsl_phy_mom_inelas(m1, 3.4, m2, -5.1, &vf);

	printf("Its final velocity would be %f m/s.\n\n", vf);
}

void main(void)
{
	printf("\n\nzscilib momentum demo\n\n");

	momentum_demo();
}
