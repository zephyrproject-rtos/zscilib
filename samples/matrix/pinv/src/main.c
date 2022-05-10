/*
 * Copyright (c) 2019 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include "zsl/vectors.h"

#ifndef EPSILON
/* Define EPSILON for floating point comparisons. */
#define EPSILON 1e-6
#endif

void pinv_demo(void)
{
	size_t q = 18;
	size_t p = 3;

	zsl_real_t vi[18 * 3] = {
		 3.2,  5.0, -4.2,
		-3.1,  5.4, -2.2,
		 7.7,  8.7,  9.9,
		 8.9,  0.6,  5.4,
		 3.7, -3.3,  7.2,
		-5.4, -0.6,  8.4,
		 2.4,  5.1, -5.7,
		 6.9, -2.1,  0.4,
		 6.4, -9.1, -4.4,
		 0.1,  7.4,  0.0,
		 6.1, -2.3,  5.5,
		 6.1, -8.4, -6.6,
		 7.1,  2.3,  4.1,
		-0.8, -4.7,  9.9,
		-3.1,  1.2,  5.2,
		 6.4, -6.3,  5.2,
		 8.8,  7.3,  4.2,
		-4.7,  0.0, -0.4
	};

	/* Large stack space warning. */
	printf("WARNING: Operations involving large matrices and eigenvectors\n");
	printf("         require a large amount of stack memory. This sample\n");
	printf("         may not work on small embedded devices, and may\n");
	printf("         cause a stack overflow.\n\n");

	/* Define the 18x3 matrix, assigning the chosen values. */
	ZSL_MATRIX_DEF(mep, q, p);
	zsl_mtx_from_arr(&mep, vi);

	/* Define the pseudoinverse output matrix. */
	ZSL_MATRIX_DEF(pinv, p, q);

	/* Display the input matrix for reference. */
	printf("INPUT (18x3 MATRIX)\n");
	printf("-------------------\n");
	zsl_mtx_print(&mep);
	printf("\n");

	/* Calculate and display the pseudoinverse using 150 iterations. */
	zsl_mtx_pinv(&mep, &pinv, 150);
	printf("PSEUDOINVERSE (3x18 MATRIX)\n");
	printf("--------------------------\n");
	zsl_mtx_print(&pinv);
	printf("\n");
}


int main(void)
{
	printf("\n\nzscilib pseudo-inverse demo\n\n");

	pinv_demo();

	return 0;
}
