/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "zsl/matrices.h"
#include "zsl/vectors.h"

#define EPSILON 1e-4

int svd_test()
{
	size_t q = 18;
	size_t p = 3;

	zsl_real_t vi[3 * 18] = { 3.2, 5.0, -4.2, -3.1, 5.4, -2.2, 7.7, 8.7,
				  9.9, 8.9, 0.6, 5.4, 3.7, -3.3, 7.2, -5.4,
				  -0.6, 8.4, 2.4, 5.1, -5.7, 6.9, -2.1, 0.4,
				  6.4, -9.1, -4.4, 0.1, 7.4, 0.0, 6.1, -2.3,
				  5.5, 6.1, -8.4, -6.6, 7.1, 2.3, 4.1, -0.8,
				  -4.7, 9.9, -3.1, 1.2, 5.2, 6.4, -6.3, 5.2,
				  8.8, 7.3, 4.2, -4.7, 0.0, -0.4 };

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

	return 0;
}

int pinv_test()
{
	size_t q = 18;
	size_t p = 3;

	zsl_real_t vi[3 * 18] = { 3.2, 5.0, -4.2, -3.1, 5.4, -2.2, 7.7, 8.7,
				  9.9, 8.9, 0.6, 5.4, 3.7, -3.3, 7.2, -5.4,
				  -0.6, 8.4, 2.4, 5.1, -5.7, 6.9, -2.1, 0.4,
				  6.4, -9.1, -4.4, 0.1, 7.4, 0.0, 6.1, -2.3,
				  5.5, 6.1, -8.4, -6.6, 7.1, 2.3, 4.1, -0.8,
				  -4.7, 9.9, -3.1, 1.2, 5.2, 6.4, -6.3, 5.2,
				  8.8, 7.3, 4.2, -4.7, 0.0, -0.4 };

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

	return 0;
}

int
main(void)
{
	printf("Hello, zscilib!\n\n");

	printf("SVD TEST\n");
	printf("--------\n");
	svd_test();

	printf("PSEUDOINVERSE TEST\n");
	printf("------------------\n");
	pinv_test();

	return 0;
}
