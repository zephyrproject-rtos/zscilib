/*
 * Copyright (c) 2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <zsl/zsl.h>
#include <zsl/orientation/orientation.h>

int zsl_grav_lat_alt(zsl_real_t *lat, zsl_real_t *alt, zsl_real_t *g)
{
	*g = 9.780318 * (1.0 + 0.0053024 * ZSL_SIN(*lat) * ZSL_SIN(*lat) - 
         0.0000058 * ZSL_SIN(2.0 * *lat) * ZSL_SIN(2.0 * *lat)) -
         0.000003085 * *alt;

    return 0;
}