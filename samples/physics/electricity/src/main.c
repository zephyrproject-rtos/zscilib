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
#include <zsl/physics/electricity.h>

void electricity_demo(void)
{
	printf("Physics electricity API demo\n");
	printf("----------------------------\n\n");

	zsl_real_t c;

	ZSL_VECTOR_DEF(va, 5);

	zsl_real_t a[5] = { 1.0, 3.2, 4.0, 0.5, 3.5 };

	zsl_vec_from_arr(&va, a);

	zsl_phy_elcty_cap_series(&va, &c);

	zsl_vec_print(&va);

	printf("Val is: %f\n", c);
}

void main(void)
{
	printf("\n\nzscilib electricity demo\n\n");

	electricity_demo();
}
