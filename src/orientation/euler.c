/*
 * Copyright (c) 2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <stdio.h>
#include <zsl/zsl.h>
#include <zsl/vectors.h>
#include <zsl/orientation/euler.h>

int zsl_eul_to_vec(struct zsl_euler *e, struct zsl_vec *v)
{
	int rc = 0;

	v->sz = 3;
	v->data = e->idx;

	return rc;
}

int zsl_eul_print(struct zsl_euler *e)
{
	printf("(%f, %f, %f)\n\n", (double)e->x, (double)e->y, (double)e->z);
	return 0;
}
