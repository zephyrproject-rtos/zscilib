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

void svd_demo(void)
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

	/* Define the 18x3 matrix, assigning the values from 'vi'. */
	ZSL_MATRIX_DEF(mep, q, p);
	zsl_mtx_from_arr(&mep, vi);

	/* Define the three SVD output matrices. */
	ZSL_MATRIX_DEF(u, mep.sz_rows, mep.sz_rows);
	ZSL_MATRIX_DEF(e, mep.sz_rows, mep.sz_cols);
	ZSL_MATRIX_DEF(v, mep.sz_cols, mep.sz_cols);

	/* Define placeholder matrices for SVD and related operations. */
	ZSL_MATRIX_DEF(ue, mep.sz_rows, mep.sz_cols);
	ZSL_MATRIX_DEF(vt, mep.sz_cols, mep.sz_cols);
	ZSL_MATRIX_DEF(uevt, mep.sz_rows, mep.sz_cols);

	/* Display the input matrix for reference. */
	printf("INPUT (18x3 MATRIX)\n");
	printf("-------------------\n");
	zsl_mtx_print(&mep);
	printf("\n");

	/* Calculate and display the SVD using 150 iterations. */
	zsl_mtx_svd(&mep, &u, &e, &v, 150);
	printf("SVD OUTPUT\n");
	printf("----------\n");
	printf("U matrix (18x18):\n");
	zsl_mtx_print(&u);
	printf("\n");
	printf("Sigma matrix (18x3):\n");
	zsl_mtx_print(&e);
	printf("\n");
	printf("V matrix: (3x3)\n");
	zsl_mtx_print(&v);
	printf("\n");

	/* Multiply u, sigma and v(transposed) from the SVD calculations,
	 * which shoud return the original matrix. */
	printf("VERIFICATION\n");
	printf("------------\n");
	zsl_mtx_mult(&u, &e, &ue);
	zsl_mtx_trans(&v, &vt);
	zsl_mtx_mult(&ue, &vt, &uevt);
	printf("U * Sigma * V^t:\n");
	zsl_mtx_print(&uevt);
	printf("\n");
}

int main(void)
{
        printf("\n\nzscilib SVD demo\n\n");

        svd_demo();

        return 0;
}
