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
#include <zsl/physics/gravitation.h>
#include <zsl/physics/energy.h>

void gravitation_demo(void)
{
	printf("Physics gravitation API demo\n");
	printf("----------------------------\n\n");

	zsl_real_t me = 5.972E24;       /* Mass of the Earth. */
	zsl_real_t msat = 750;          /* Mass of a medium artif. satellite. */
	zsl_real_t re = 6.731E6;        /* Radius of the Earth. */
	zsl_real_t tropo = 18E3;        /* Average height of the troposphere. */

	printf("An artificial satellite with a mass of %f kg is\n", msat);
	printf("going to be launched into space.\n\n");

	printf("(a) Calculate the min. initial velocity it needs in order\n");
	printf("to leave the Earth.\n\n");

	zsl_real_t vesc;

	zsl_phy_grav_esc_vel(me, re, &vesc);

	printf("The velocity required to escape the gravitational pull of\n");
	printf("the Earth is %f meters per second.\n\n", vesc);

	printf("(b) Once in space, the satellite starts orbiting at a\n");
	printf("height of 40 KMs over the surface of the Earth. Determine\n");
	printf("its orbital period in hours and its velocity.\n\n");

	zsl_real_t period, period2, v;

	/* Calculate the total distance between the Earth and the satellite. */
	zsl_real_t rt = 40000 + re;

	/* Calculate the period in seconds. */
	zsl_phy_grav_orb_period(me, msat, rt, &period);

	/* Convert the period to hours. */
	period2 = period / 3600;

	/* Calculate the orbital velocity. */
	zsl_phy_grav_orb_vel(me, rt, &v);

	printf("It takes %f hours (%f seconds) for the\n", period2, period);
	printf("satellite to go arround the Earth. Its orbital velocity\n");
	printf("is %f meters per second.\n\n", v);

	printf("(c) Calculate the force that the satellite experiences due\n");
	printf("to the Earth's gravitational pull and the total energy\n");
	printf("it has in this orbit.\n\n");

	zsl_real_t f, u, ke;

	/* Calculate the force. */
	zsl_phy_grav_force(me, msat, rt, &f);

	/* Calculate the potential energy in kilojoules. */
	zsl_phy_grav_pot_ener(me, msat, rt, &u);

	/* Calculate the kinetic energy in joules. */
	zsl_phy_ener_kin(v, msat, &ke);
	/* Convert value to kilojoules. */
	ke /= 1E3;

	zsl_real_t e = u + ke;

	printf("The Earth atracts the satellite w/a force of %f newtons\n", f);
	printf("and the satellite has a total energy of %f kilojoules.\n\n", e);

	printf("(d) The satellite collides with an asteroid and starts\n");
	printf("falling to the Earth. Calculate its acceleration when\n");
	printf("it enters the troposphere.\n\n");

	zsl_real_t g;

	/* Calculate the gravity at 18 kilometers over the surface of the Earth
	 * (troposphere's height). */
	rt = re + tropo;
	zsl_phy_grav_acc(me, rt, &g);

	printf("Its acceleration is %f meters per second squared.\n\n", g);
}

void main(void)
{
	printf("\n\nzscilib gravitation demo\n\n");

	gravitation_demo();
}
