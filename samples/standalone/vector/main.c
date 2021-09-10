/*
 * Copyright (c) 2021 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "zsl/matrices.h"
#include "zsl/vectors.h"

int
main(void)
{
	ZSL_VECTOR_DEF(va, 3);
	ZSL_VECTOR_DEF(vb, 3);
	ZSL_VECTOR_DEF(vc, 3);

	printf("Hello, zscilib!\n");

	zsl_real_t a[3] = { 0.0, 1.0, 2.0 };
	zsl_real_t b[3] = { -1.0, 0.5, 0.1 };

	/* Assign arrays to vectors. */
	zsl_vec_from_arr(&va, a);
	zsl_vec_from_arr(&vb, b);

	/* Init vc with zero values. */
	zsl_vec_init(&vc);

	/* Add va and vb, results in vc. */
	zsl_vec_add(&va, &vb, &vc);
	zsl_vec_print(&vc);

	return 0;
}