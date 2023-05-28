/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/vectors.h>
#include "floatcheck.h"

/**
 * @brief zsl_mtx_init unit tests.
 *
 * This test verifies the zsl_mtx_init function.
 */
ZTEST(zsl_tests, test_matrix_init)
{
	int rc;
	zsl_real_t x;

	ZSL_MATRIX_DEF(m, 3, 3);

	/* Initialise the matrix with the default (empty) entry_fn. */
	rc = zsl_mtx_init(&m, NULL);
	zassert_equal(rc, 0, NULL);

	rc = zsl_mtx_get(&m, 1, 0, &x);
	zassert_equal(rc, 0, NULL);
	zassert_equal(x, 0.0, NULL);

	rc = zsl_mtx_get(&m, 2, 2, &x);
	zassert_equal(rc, 0, NULL);
	zassert_equal(x, 0.0, NULL);

	/* Reinitialise the matrix as an identity/diagonal matrix. */
	rc = zsl_mtx_init(&m, zsl_mtx_entry_fn_identity);
	zassert_equal(rc, 0, NULL);

	rc = zsl_mtx_get(&m, 0, 0, &x);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(x, 1.0, 1E-5), NULL);

	rc = zsl_mtx_get(&m, 1, 1, &x);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(x, 1.0, 1E-5), NULL);

	rc = zsl_mtx_get(&m, 2, 2, &x);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(x, 1.0, 1E-5), NULL);

	rc = zsl_mtx_get(&m, 0, 1, &x);
	zassert_equal(x, 0.0, NULL);
	zassert_equal(rc, 0, NULL);
}

/**
 * @brief zsl_mtx_from_arr unit tests.
 *
 * This test verifies the zsl_mtx_from_arr function.
 */
ZTEST(zsl_tests, test_matrix_from_arr)
{
	int rc;
	zsl_real_t x;

	/* Destination matrix. */
	ZSL_MATRIX_DEF(m, 3, 3);

	/* Source array. */
	zsl_real_t data[9] = { 1.0, 0.0, 0.0,
			       0.0, 0.5, 0.0,
			       0.0, 0.0, 0.1 };

	/* Init matrix m. */
	zsl_mtx_init(&m, NULL);

	/* Initialise the matrix with the default (empty) entry_fn. */
	rc = zsl_mtx_from_arr(&m, data);
	zassert_true(rc == 0, NULL);

	rc = zsl_mtx_get(&m, 0, 0, &x);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(x, 1.0, 1E-5), NULL);

	rc = zsl_mtx_get(&m, 1, 1, &x);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(x, 0.5, 1E-5), NULL);

	rc = zsl_mtx_get(&m, 2, 2, &x);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(x, 0.1, 1E-5), NULL);

	rc = zsl_mtx_get(&m, 1, 0, &x);
	zassert_equal(rc, 0, NULL);
	zassert_equal(x, 0.0, NULL);
}

/**
 * @brief zsl_mtx_copy and zsl_mtx_is_equal unit tests.
 *
 * This test verifies the zsl_mtx_copy and zsl_mtx_is_equal functions.
 */
ZTEST(zsl_tests, test_matrix_copy)
{
	int rc;

	/* Source array. */
	zsl_real_t data[9] = { 1.0, 0.0, 0.0,
			       0.0, 0.5, 0.0,
			       0.0, 0.0, 0.1 };

	/* Destination matrix. */
	ZSL_MATRIX_DEF(m, 3, 3);
	zsl_mtx_init(&m, NULL);

	/* Source matrix. */
	ZSL_MATRIX_DEF(msrc, 3, 3);
	rc = zsl_mtx_from_arr(&msrc, data);
	zassert_true(rc == 0, NULL);

	/* Copy msrc to m. */
	rc = zsl_mtx_copy(&m, &msrc);
	zassert_true(rc == 0, NULL);

	/* Verify copy using zsl_mtx_is_equal. */
	zassert_true(zsl_mtx_is_equal(&m, &msrc), NULL);
}

/**
 * @brief zsl_mtx_get unit tests.
 *
 * This test verifies the zsl_mtx_get function.
 */
ZTEST(zsl_tests, test_matrix_get)
{
	int rc;
	zsl_real_t x;
	zsl_real_t data[9] = { 1.0, 0.0, 0.0,
			       0.0, 0.5, 0.0,
			       0.0, 0.0, 0.1 };

	struct zsl_mtx m = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data
	};

	/* Read values from the matrix above. */
	rc = zsl_mtx_get(&m, 0, 0, &x);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(x, 1.0, 1E-5), NULL);

	rc = zsl_mtx_get(&m, 1, 0, &x);
	zassert_equal(rc, 0, NULL);
	zassert_equal(x, 0.0, NULL);

	rc = zsl_mtx_get(&m, 2, 2, &x);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(x, 0.1, 1E-5), NULL);

	/* Check for out of bounds error. */
	zassert_true(zsl_mtx_get(&m, 3, 3, &x) == -EINVAL, NULL);
}

/**
 * @brief zsl_mtx_set unit tests.
 *
 * This test verifies the zsl_mtx_set function.
 */
ZTEST(zsl_tests, test_matrix_set)
{
	int rc = 0;
	zsl_real_t x;

	ZSL_MATRIX_DEF(m, 3, 3);

	/* Init matrix m. */
	zsl_mtx_init(&m, NULL);

	/* Set values in matrix m. */
	rc = zsl_mtx_set(&m, 0, 0, 1.0);
	zassert_equal(rc, 0, NULL);

	rc = zsl_mtx_set(&m, 1, 1, 0.5);
	zassert_equal(rc, 0, NULL);

	rc = zsl_mtx_set(&m, 2, 2, 0.1);
	zassert_equal(rc, 0, NULL);

	/* Verify assigned values */
	rc = zsl_mtx_get(&m, 0, 0, &x);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(x, 1.0, 1E-5), NULL);

	rc = zsl_mtx_get(&m, 1, 1, &x);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(x, 0.5, 1E-5), NULL);

	rc = zsl_mtx_get(&m, 2, 2, &x);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(x, 0.1, 1E-5), NULL);

	/* Check for out of bounbds error. */
	zassert_true(zsl_mtx_set(&m, 3, 3, 0.0) == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_matrix_get_set_row)
{
	int rc = 0;
	zsl_real_t x;
	zsl_real_t v[] = { 1.0, 2.0, 3.0 };

	ZSL_MATRIX_DEF(m, 3, 3);
	ZSL_VECTOR_DEF(v2, 3);

	/* Init matrix m. */
	zsl_mtx_init(&m, NULL);

	/* Set row 0 in m with the values in 3-vector v. */
	rc = zsl_mtx_set_row(&m, 0, v);
	zassert_equal(rc, 0, NULL);

	/* Set row 2 in m with the values in 3-vector v. */
	rc = zsl_mtx_set_row(&m, 2, v);
	zassert_equal(rc, 0, NULL);

	/* Verify row 0. */
	rc = zsl_mtx_get(&m, 0, 0, &x);
	zassert_true(val_is_equal(x, v[0], 1E-5), NULL);
	rc = zsl_mtx_get(&m, 0, 1, &x);
	zassert_true(val_is_equal(x, v[1], 1E-5), NULL);
	rc = zsl_mtx_get(&m, 0, 2, &x);
	zassert_true(val_is_equal(x, v[2], 1E-5), NULL);

	/* Verify row 1 (should be all zeroes). */
	rc = zsl_mtx_get(&m, 1, 0, &x);
	zassert_true(val_is_equal(x, 0.0, 1E-5), NULL);
	rc = zsl_mtx_get(&m, 1, 0, &x);
	zassert_true(val_is_equal(x, 0.0, 1E-5), NULL);
	rc = zsl_mtx_get(&m, 1, 0, &x);
	zassert_true(val_is_equal(x, 0.0, 1E-5), NULL);

	/* Verify row 2. */
	rc = zsl_mtx_get(&m, 2, 0, &x);
	zassert_true(val_is_equal(x, v[0], 1E-5), NULL);
	rc = zsl_mtx_get(&m, 2, 1, &x);
	zassert_true(val_is_equal(x, v[1], 1E-5), NULL);
	rc = zsl_mtx_get(&m, 2, 2, &x);
	zassert_true(val_is_equal(x, v[2], 1E-5), NULL);

	/* Now test the get method. */
	rc = zsl_vec_init(&v2);
	zassert_equal(rc, 0, NULL);

	/* Read row zero, assigning it to v2.data. */
	rc = zsl_mtx_get_row(&m, 0, v2.data);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(v2.data[0], v[0], 1E-5), NULL);
	zassert_true(val_is_equal(v2.data[1], v[1], 1E-5), NULL);
	zassert_true(val_is_equal(v2.data[2], v[2], 1E-5), NULL);
}

ZTEST(zsl_tests, test_matrix_get_set_col)
{
	int rc = 0;
	zsl_real_t x;
	zsl_real_t v[] = { 1.0, 2.0, 3.0 };

	ZSL_MATRIX_DEF(m, 3, 3);
	ZSL_VECTOR_DEF(v2, 3);

	/* TODO: Test get method! */

	/* Init matrix m. */
	zsl_mtx_init(&m, NULL);

	/* Set col 0 in m with the values in 3-vector v. */
	rc = zsl_mtx_set_col(&m, 0, v);
	zassert_equal(rc, 0, NULL);

	/* Set col 2 in m with the values in 3-vector v. */
	rc = zsl_mtx_set_col(&m, 2, v);
	zassert_equal(rc, 0, NULL);

	/* Verify col 0. */
	rc = zsl_mtx_get(&m, 0, 0, &x);
	zassert_true(val_is_equal(x, v[0], 1E-5), NULL);
	rc = zsl_mtx_get(&m, 1, 0, &x);
	zassert_true(val_is_equal(x, v[1], 1E-5), NULL);
	rc = zsl_mtx_get(&m, 2, 0, &x);
	zassert_true(val_is_equal(x, v[2], 1E-5), NULL);

	/* Verify col 1 (should be all zeroes). */
	rc = zsl_mtx_get(&m, 0, 1, &x);
	zassert_true(val_is_equal(x, 0.0, 1E-5), NULL);
	rc = zsl_mtx_get(&m, 1, 1, &x);
	zassert_true(val_is_equal(x, 0.0, 1E-5), NULL);
	rc = zsl_mtx_get(&m, 2, 1, &x);
	zassert_true(val_is_equal(x, 0.0, 1E-5), NULL);

	/* Verify col 2. */
	rc = zsl_mtx_get(&m, 0, 2, &x);
	zassert_true(val_is_equal(x, v[0], 1E-5), NULL);
	rc = zsl_mtx_get(&m, 1, 2, &x);
	zassert_true(val_is_equal(x, v[1], 1E-5), NULL);
	rc = zsl_mtx_get(&m, 2, 2, &x);
	zassert_true(val_is_equal(x, v[2], 1E-5), NULL);

	/* Now test the get method. */
	rc = zsl_vec_init(&v2);
	zassert_equal(rc, 0, NULL);

	/* Read row zero, assigning it to v2.data. */
	rc = zsl_mtx_get_col(&m, 2, v2.data);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(v2.data[0], v[0], 1E-5), NULL);
	zassert_true(val_is_equal(v2.data[1], v[1], 1E-5), NULL);
	zassert_true(val_is_equal(v2.data[2], v[2], 1E-5), NULL);
}

ZTEST(zsl_tests, test_matrix_row_from_vec)
{
	int rc;
	zsl_real_t x;

	ZSL_VECTOR_DEF(v, 3);
	ZSL_MATRIX_DEF(m, 3, 3);

	/* Init matrix m. */
	zsl_mtx_init(&m, NULL);

	/* Assign some values to the vector. */
	v.data[0] = 1.0;
	v.data[1] = 2.0;
	v.data[2] = 3.0;

	/* Now assign the vector to matrix row 1 via the .data field. */
	rc = zsl_mtx_set_row(&m, 1, v.data);
	zassert_true(rc == 0, NULL);

	/* Make sure the row assignment worked. */
	rc = zsl_mtx_get(&m, 1, 0, &x);
	zassert_true(val_is_equal(x, v.data[0], 1E-5), NULL);
	rc = zsl_mtx_get(&m, 1, 1, &x);
	zassert_true(val_is_equal(x, v.data[1], 1E-5), NULL);
	rc = zsl_mtx_get(&m, 1, 2, &x);
	zassert_true(val_is_equal(x, v.data[2], 1E-5), NULL);

	/* Now read one back. */
	rc = zsl_mtx_get_row(&m, 0, v.data);
	zassert_true(rc == 0, NULL);

	/* Vector values should have changed to all be 0.0 now. */
	zassert_true(val_is_equal(v.data[0], 0.0, 1E-5), NULL);
	zassert_true(val_is_equal(v.data[1], 0.0, 1E-5), NULL);
	zassert_true(val_is_equal(v.data[2], 0.0, 1E-5), NULL);
}

ZTEST(zsl_tests, test_matrix_unary_op)
{
	int rc;

	zsl_real_t data[9] = { 1.0, 0.0, 0.0,
			       0.0, 0.5, 0.0,
			       0.0, 0.0, 0.1 };

	struct zsl_mtx m = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data
	};

	rc = zsl_mtx_unary_op(&m, ZSL_MTX_UNARY_OP_INCREMENT);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(m.data[0], 2.0, 1E-5), NULL);
	zassert_true(val_is_equal(m.data[8], 1.1, 1E-5), NULL);

	/* TODO: Test other operands! */
}

ZTEST(zsl_tests, test_matrix_unary_func)
{

}

ZTEST(zsl_tests, test_matrix_binary_op)
{
	int rc;

	ZSL_MATRIX_DEF(mc, 3, 3);

	zsl_real_t data_a[9] = { 1.0, 0.0, 0.0,
				 0.0, 0.5, 0.0,
				 0.0, 0.0, 0.1 };

	zsl_real_t data_b[9] = { 1.0, 0.0, 0.0,
				 0.0, 0.5, 0.0,
				 0.0, 0.0, 0.1 };

	struct zsl_mtx ma = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data_a
	};

	struct zsl_mtx mb = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data_b
	};

	/* Init matrix mc. */
	zsl_mtx_init(&mc, NULL);

	rc = zsl_mtx_binary_op(&ma, &mb, &mc, ZSL_MTX_BINARY_OP_ADD);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(mc.data[0], ma.data[0] + mb.data[0], 1E-5),
		     NULL);
	zassert_true(val_is_equal(mc.data[8], ma.data[8] + mb.data[8], 1E-5),
		     NULL);

	/* TODO: Test other operands! */
}

ZTEST(zsl_tests, test_matrix_binary_func)
{

}

ZTEST(zsl_tests, test_matrix_add)
{
	int rc;
	zsl_real_t x;

	ZSL_MATRIX_DEF(mc, 3, 4);

	zsl_real_t a[12] = { 1.0, 2.0, 4.0, 7.0,
			     0.0, 0.5, 0.0, 6.2,
			     9.0, 0.8, 0.1, 0.4 };

	zsl_real_t b[12] = { 3.0, 0.0, 0.0, 5.1,
			     6.0, 2.5, 1.0, 4.4,
			     0.0, 7.0, 4.1, 2.1 };

	struct zsl_mtx ma = {
		.sz_rows = 3,
		.sz_cols = 4,
		.data = a
	};

	struct zsl_mtx mb = {
		.sz_rows = 3,
		.sz_cols = 4,
		.data = b
	};

	/* Init matrix mc. */
	rc = zsl_mtx_init(&mc, NULL);
	zassert_true(rc == 0, NULL);

	/* Add 'ma' and 'mb'. */
	rc = zsl_mtx_add(&ma, &mb, &mc);
	zassert_true(rc == 0, NULL);

	/* Check the output. */
	for (size_t g = 0; g < (ma.sz_rows * ma.sz_cols); g++) {
		x = ma.data[g] + mb.data[g];
		zassert_true(val_is_equal(mc.data[g], x, 1E-5), NULL);
	}
}

ZTEST(zsl_tests, test_matrix_add_d)
{
	int rc;
	zsl_real_t x;

	ZSL_MATRIX_DEF(mcopy, 3, 4);

	zsl_real_t a[12] = { 1.0, 2.0, 4.0, 7.0,
			     0.0, 0.5, 0.0, 6.2,
			     9.0, 0.8, 0.1, 0.4 };

	zsl_real_t b[12] = { 3.0, 0.0, 0.0, 5.1,
			     6.0, 2.5, 1.0, 4.4,
			     0.0, 7.0, 4.1, 2.1 };

	struct zsl_mtx ma = {
		.sz_rows = 3,
		.sz_cols = 4,
		.data = a
	};

	struct zsl_mtx mb = {
		.sz_rows = 3,
		.sz_cols = 4,
		.data = b
	};

	/* Copy 'ma' into 'mcopy'. */
	rc = zsl_mtx_copy(&mcopy, &ma);
	zassert_equal(rc, 0, NULL);

	/* Add 'ma' and 'mb'. */
	rc = zsl_mtx_add_d(&ma, &mb);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	for (size_t g = 0; g < (ma.sz_rows * ma.sz_cols); g++) {
		x = mcopy.data[g] + mb.data[g];
		zassert_true(val_is_equal(ma.data[g], x, 1E-5), NULL);
	}
}

ZTEST(zsl_tests, test_matrix_sum_rows_d)
{
	int rc;

	ZSL_MATRIX_DEF(mcopy, 3, 4);

	zsl_real_t a[12] = { 1.0, 2.0, 4.0, 7.0,
			     0.0, 0.5, 5.4, 6.2,
			     9.0, 0.8, 0.1, 0.4 };

	struct zsl_mtx ma = {
		.sz_rows = 3,
		.sz_cols = 4,
		.data = a
	};

	/* Copy 'ma' into 'mcopy'. */
	rc = zsl_mtx_copy(&mcopy, &ma);
	zassert_true(rc == 0, NULL);

	/* Add row 3 to row 2. */
	rc = zsl_mtx_sum_rows_d(&ma, 1, 2);
	zassert_true(rc == 0, NULL);

	/* Check the output. */
	rc = zsl_mtx_set(&mcopy, 1, 0, 9.0);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_set(&mcopy, 1, 1, 1.3);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_set(&mcopy, 1, 2, 5.5);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_set(&mcopy, 1, 3, 6.6);
	zassert_true(rc == 0, NULL);

	for (size_t i = 0; i < (ma.sz_rows * ma.sz_cols); i++) {
		zassert_true(val_is_equal(mcopy.data[i], ma.data[i], 1E-6),
			     NULL);
	}

	/* Add row 2 to row 1. */
	rc = zsl_mtx_sum_rows_d(&ma, 0, 1);
	zassert_true(rc == 0, NULL);

	/* Check the output. */
	rc = zsl_mtx_set(&mcopy, 0, 0, 10.0);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_set(&mcopy, 0, 1, 3.3);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_set(&mcopy, 0, 2, 9.5);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_set(&mcopy, 0, 3, 13.6);
	zassert_true(rc == 0, NULL);

	for (size_t i = 0; i < (ma.sz_rows * ma.sz_cols); i++) {
		zassert_true(val_is_equal(mcopy.data[i], ma.data[i], 1E-6),
			     NULL);
	}

	/* Add row 2 to row 5. An error is expected. */
	rc = zsl_mtx_sum_rows_d(&ma, 4, 1);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_matrix_sum_rows_scaled_d)
{
	int rc;

	ZSL_MATRIX_DEF(mcopy, 3, 4);

	zsl_real_t a[12] = { 1.0, 2.0, 4.0, 7.0,
			     0.0, 0.5, 5.4, 6.2,
			     9.0, 0.8, 0.1, 0.4 };

	struct zsl_mtx ma = {
		.sz_rows = 3,
		.sz_cols = 4,
		.data = a
	};

	/* Copy 'ma' into 'mcopy'. */
	rc = zsl_mtx_copy(&mcopy, &ma);
	zassert_true(rc == 0, NULL);

	/* Add 4.1 times row 3 to row 2. */
	rc = zsl_mtx_sum_rows_scaled_d(&ma, 1, 2, 4.1);
	zassert_true(rc == 0, NULL);

	/* Check the output. */
	rc = zsl_mtx_set(&mcopy, 1, 0, 36.9);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_set(&mcopy, 1, 1, 3.78);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_set(&mcopy, 1, 2, 5.81);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_set(&mcopy, 1, 3, 7.84);
	zassert_true(rc == 0, NULL);

	for (size_t i = 0; i < (ma.sz_rows * ma.sz_cols); i++) {
#ifdef CONFIG_ZSL_SINGLE_PRECISION
		zassert_true(val_is_equal(mcopy.data[i], ma.data[i], 1E-5),
			     NULL);
#else
		zassert_true(val_is_equal(mcopy.data[i], ma.data[i], 1E-8),
			     NULL);
#endif
	}

	/* Add -0.2 time row 2 to row 1. */
	rc = zsl_mtx_sum_rows_scaled_d(&ma, 0, 1, -0.2);
	zassert_true(rc == 0, NULL);

	/* Check the output. */
	rc = zsl_mtx_set(&mcopy, 0, 0, -6.38);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_set(&mcopy, 0, 1, 1.244);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_set(&mcopy, 0, 2, 2.838);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_set(&mcopy, 0, 3, 5.432);
	zassert_true(rc == 0, NULL);

	for (size_t i = 0; i < (ma.sz_rows * ma.sz_cols); i++) {
#ifdef CONFIG_ZSL_SINGLE_PRECISION
		zassert_true(val_is_equal(mcopy.data[i], ma.data[i], 1E-5),
			     NULL);
#else
		zassert_true(val_is_equal(mcopy.data[i], ma.data[i], 1E-8),
			     NULL);
#endif
	}

	/* Add two times row 2 to row 5. An error is expected. */
	rc = zsl_mtx_sum_rows_scaled_d(&ma, 4, 1, 2.0);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_matrix_sub)
{
	int rc;
	zsl_real_t x;

	ZSL_MATRIX_DEF(mc, 3, 4);

	zsl_real_t a[12] = { 1.0, 2.0, 4.0, 7.0,
			     0.0, 0.5, 0.0, 6.2,
			     9.0, 0.8, 0.1, 0.4 };

	zsl_real_t b[12] = { 3.0, 0.0, 0.0, 5.1,
			     6.0, 2.5, 1.0, 4.4,
			     0.0, 7.0, 4.1, 2.1 };

	struct zsl_mtx ma = {
		.sz_rows = 3,
		.sz_cols = 4,
		.data = a
	};

	struct zsl_mtx mb = {
		.sz_rows = 3,
		.sz_cols = 4,
		.data = b
	};

	/* Init matrix mc. */
	rc = zsl_mtx_init(&mc, NULL);
	zassert_true(rc == 0, NULL);

	/* Substract 'mb' from 'ma'. */
	rc = zsl_mtx_sub(&ma, &mb, &mc);
	zassert_true(rc == 0, NULL);

	/* Check the output. */
	for (size_t g = 0; g < (ma.sz_rows * ma.sz_cols); g++) {
		x = ma.data[g] - mb.data[g];
		zassert_true(val_is_equal(mc.data[g], x, 1E-5), NULL);
	}
}

ZTEST(zsl_tests, test_matrix_sub_d)
{
	int rc;
	zsl_real_t x;

	ZSL_MATRIX_DEF(mcopy, 3, 4);

	zsl_real_t a[12] = { 1.0, 2.0, 4.0, 7.0,
			     0.0, 0.5, 0.0, 6.2,
			     9.0, 0.8, 0.1, 0.4 };

	zsl_real_t b[12] = { 3.0, 0.0, 0.0, 5.1,
			     6.0, 2.5, 1.0, 4.4,
			     0.0, 7.0, 4.1, 2.1 };

	struct zsl_mtx ma = {
		.sz_rows = 3,
		.sz_cols = 4,
		.data = a
	};

	struct zsl_mtx mb = {
		.sz_rows = 3,
		.sz_cols = 4,
		.data = b
	};

	/* Copy 'ma' into 'mcopy'. */
	rc = zsl_mtx_copy(&mcopy, &ma);
	zassert_equal(rc, 0, NULL);

	/* Substract 'mb' from 'ma'. */
	rc = zsl_mtx_sub_d(&ma, &mb);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	for (size_t g = 0; g < (ma.sz_rows * ma.sz_cols); g++) {
		x = mcopy.data[g] - mb.data[g];
		zassert_true(val_is_equal(ma.data[g], x, 1E-5), NULL);
	}
}

/**
 * @brief zsl_mtx_mult unit tests with square matrices.
 *
 * This test verifies the zsl_mtx_mult function with square matrices.
 */
ZTEST(zsl_tests, test_matrix_mult_sq)
{
	int rc = 0;

	ZSL_MATRIX_DEF(mc, 3, 3);

	/* Input matrix a. */
	zsl_real_t data_a[9] = { 1.0, 2.0, 3.0,
				 4.0, 5.0, 6.0,
				 7.0, 8.0, 9.0 };
	struct zsl_mtx ma = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data_a
	};

	/* Input matrix b. */
	zsl_real_t data_b[9] = { 10.0, 20.0, 30.0,
				 40.0, 50.0, 60.0,
				 70.0, 80.0, 90.0 };
	struct zsl_mtx mb = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data_b
	};

	/* Init matrix mc. */
	zsl_mtx_init(&mc, NULL);

	/* Output reference matrix (for comparison). */
	zsl_real_t data_ref[9] = { 300.0,  360.0,  420.0,
				   660.0,  810.0,  960.0,
				   1020.0, 1260.0, 1500.0 };
	struct zsl_mtx mref = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data_ref
	};

	/* Perform a valid 3x3 square matrix multiplication. */
	rc = zsl_mtx_mult(&ma, &mb, &mc);
	zassert_equal(rc, 0, NULL);
	zassert_equal(mref.data[0], mc.data[0], NULL);
	zassert_equal(mref.data[1], mc.data[1], NULL);
	zassert_equal(mref.data[2], mc.data[2], NULL);
	zassert_equal(mref.data[3], mc.data[3], NULL);
	zassert_equal(mref.data[4], mc.data[4], NULL);
	zassert_equal(mref.data[5], mc.data[5], NULL);
	zassert_equal(mref.data[6], mc.data[6], NULL);
	zassert_equal(mref.data[7], mc.data[7], NULL);
	zassert_equal(mref.data[8], mc.data[8], NULL);
}

/**
 * @brief zsl_mtx_mult unit tests with rectangular matrices.
 *
 * This test verifies the zsl_mtx_mult function with rectangular matrices.
 */
ZTEST(zsl_tests, test_matrix_mult_rect)
{
	int rc = 0;

	ZSL_MATRIX_DEF(mc, 4, 3);
	ZSL_MATRIX_DEF(merr, 5, 3);

	/* Init matrix mc, merr */
	zsl_mtx_init(&mc, NULL);
	zsl_mtx_init(&merr, NULL);

	/* Input matrix a (4x2). */
	zsl_real_t data_a[8] = { 2.0, 3.0,
				 1.0, 4.0,
				 4.0, 3.0,
				 3.0, 4.0 };
	struct zsl_mtx ma = {
		.sz_rows = 4,
		.sz_cols = 2,
		.data = data_a
	};

	/* Input matrix b (2x3). */
	zsl_real_t data_b[9] = { 3.0, 1.0, 2.0,
				 2.0, 4.0, 2.0 };
	struct zsl_mtx mb = {
		.sz_rows = 2,
		.sz_cols = 3,
		.data = data_b
	};

	/* Output reference matrix (4x3). */
	zsl_real_t data_ref[12] = { 12.0, 14.0, 10.0,
				    11.0, 17.0, 10.0,
				    18.0, 16.0, 14.0,
				    17.0, 19.0, 14.0 };
	struct zsl_mtx mref = {
		.sz_rows = 4,
		.sz_cols = 3,
		.data = data_ref
	};

	/* Attempt an invalid 4x2 x 5x3 matrix multiplication. */
	rc = zsl_mtx_mult(&merr, &mb, &mc);
	zassert_equal(rc, -EINVAL, NULL);

	/* Perform a valid 3x3 square matrix multiplication. */
	rc = zsl_mtx_mult(&ma, &mb, &mc);
	zassert_equal(rc, 0, NULL);
	zassert_equal(mref.data[0], mc.data[0], NULL);
	zassert_equal(mref.data[1], mc.data[1], NULL);
	zassert_equal(mref.data[2], mc.data[2], NULL);
	zassert_equal(mref.data[3], mc.data[3], NULL);
	zassert_equal(mref.data[4], mc.data[4], NULL);
	zassert_equal(mref.data[5], mc.data[5], NULL);
	zassert_equal(mref.data[6], mc.data[6], NULL);
	zassert_equal(mref.data[7], mc.data[7], NULL);
	zassert_equal(mref.data[8], mc.data[8], NULL);
	zassert_equal(mref.data[9], mc.data[9], NULL);
	zassert_equal(mref.data[10], mc.data[10], NULL);
	zassert_equal(mref.data[11], mc.data[11], NULL);
}

ZTEST(zsl_tests, test_matrix_mult_d)
{
	int rc = 0;

	/* Input matrix a. */
	zsl_real_t data_a[9] = { 1.0, 2.0, 3.0,
				 4.0, 5.0, 6.0,
				 7.0, 8.0, 9.0 };
	struct zsl_mtx ma = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data_a
	};

	/* Input matrix b. */
	zsl_real_t data_b[9] = { 10.0, 20.0, 30.0,
				 40.0, 50.0, 60.0,
				 70.0, 80.0, 90.0 };
	struct zsl_mtx mb = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data_b
	};

	/* Output reference matrix (for comparison). */
	zsl_real_t data_ref[9] = { 300.0,  360.0,  420.0,
				   660.0,  810.0,  960.0,
				   1020.0, 1260.0, 1500.0 };
	struct zsl_mtx mref = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data_ref
	};

	/* Perform a valid matrix multiplication. */
	rc = zsl_mtx_mult_d(&ma, &mb);
	zassert_equal(rc, 0, NULL);
	zassert_equal(mref.data[0], ma.data[0], NULL);
	zassert_equal(mref.data[1], ma.data[1], NULL);
	zassert_equal(mref.data[2], ma.data[2], NULL);
	zassert_equal(mref.data[3], ma.data[3], NULL);
	zassert_equal(mref.data[4], ma.data[4], NULL);
	zassert_equal(mref.data[5], ma.data[5], NULL);
	zassert_equal(mref.data[6], ma.data[6], NULL);
	zassert_equal(mref.data[7], ma.data[7], NULL);
	zassert_equal(mref.data[8], ma.data[8], NULL);

	/* Perform a invalid matrix multiplications. */
	ZSL_MATRIX_DEF(mb2, 3, 4);
	rc = zsl_mtx_mult_d(&ma, &mb2);
	zassert_equal(rc, -EINVAL, NULL);

	ZSL_MATRIX_DEF(ma2, 1, 5);
	rc = zsl_mtx_mult_d(&ma2, &mb);
	zassert_equal(rc, -EINVAL, NULL);
}

/**
 * @brief zsl_mtx_scalar_mult_d unit tests.
 *
 * This test verifies the zsl_mtx_scalar_mult_d function.
 */
ZTEST(zsl_tests, test_matrix_scalar_mult_d)
{
	int rc = 0;
	zsl_real_t s = 10.0;

	/* Input matrix. */
	zsl_real_t data[8] = { 2.0, 3.0,
			       1.0, 4.0,
			       4.0, 3.0,
			       3.0, 4.0 };
	struct zsl_mtx m = {
		.sz_rows = 4,
		.sz_cols = 2,
		.data = data
	};

	rc = zsl_mtx_scalar_mult_d(&m, s);
	zassert_equal(rc, 0, NULL);
	zassert_true(val_is_equal(m.data[0], 20.0, 1E-5), NULL);
	zassert_true(val_is_equal(m.data[1], 30.0, 1E-5), NULL);
	zassert_true(val_is_equal(m.data[2], 10.0, 1E-5), NULL);
	zassert_true(val_is_equal(m.data[3], 40.0, 1E-5), NULL);
	zassert_true(val_is_equal(m.data[4], 40.0, 1E-5), NULL);
	zassert_true(val_is_equal(m.data[5], 30.0, 1E-5), NULL);
	zassert_true(val_is_equal(m.data[6], 30.0, 1E-5), NULL);
	zassert_true(val_is_equal(m.data[7], 40.0, 1E-5), NULL);
}

ZTEST(zsl_tests, test_matrix_scalar_mult_row_d)
{
	int rc;

	ZSL_MATRIX_DEF(mcopy, 3, 4);

	zsl_real_t a[12] = { 1.0, 2.0, 4.0, 7.0,
			     0.0, 0.5, 5.4, 6.2,
			     9.0, 0.8, 0.1, 0.4 };

	struct zsl_mtx ma = {
		.sz_rows = 3,
		.sz_cols = 4,
		.data = a
	};

	/* Copy 'ma' into 'mcopy'. */
	rc = zsl_mtx_copy(&mcopy, &ma);
	zassert_true(rc == 0, NULL);

	/* Multiply row 3 by 1.3. */
	rc = zsl_mtx_scalar_mult_row_d(&ma, 2, 1.3);
	zassert_true(rc == 0, NULL);

	/* Check the output. */
	rc = zsl_mtx_set(&mcopy, 2, 0, 11.7);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_set(&mcopy, 2, 1, 1.04);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_set(&mcopy, 2, 2, 0.13);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_set(&mcopy, 2, 3, 0.52);
	zassert_true(rc == 0, NULL);

	for (size_t i = 0; i < (ma.sz_rows * ma.sz_cols); i++) {
		zassert_true(val_is_equal(mcopy.data[i], ma.data[i], 1E-6),
			     NULL);
	}

	/* Multiply row 1 by -0.5. */
	rc = zsl_mtx_scalar_mult_row_d(&ma, 0, -0.5);
	zassert_true(rc == 0, NULL);

	/* Check the output. */
	rc = zsl_mtx_set(&mcopy, 0, 0, -0.5);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_set(&mcopy, 0, 1, -1.0);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_set(&mcopy, 0, 2, -2.0);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_set(&mcopy, 0, 3, -3.5);
	zassert_true(rc == 0, NULL);

	for (size_t i = 0; i < (ma.sz_rows * ma.sz_cols); i++) {
		zassert_true(val_is_equal(mcopy.data[i], ma.data[i], 1E-6),
			     NULL);
	}

	/* Multiply row 5 by 3. An error is expected. */
	rc = zsl_mtx_scalar_mult_row_d(&ma, 4, 3);
	zassert_true(rc == -EINVAL, NULL);
}

/**
 * @brief zsl_mtx_scalar_trans unit tests.
 *
 * This test verifies the zsl_mtx_trans function.
 */
ZTEST(zsl_tests, test_matrix_trans)
{
	int rc = 0;

	ZSL_MATRIX_DEF(mt, 2, 4);

	/* Input matrix. */
	zsl_real_t data[8] = { 2.0, 3.0,
			       1.0, 4.0,
			       4.0, 3.0,
			       3.0, 4.0 };

	struct zsl_mtx m = {
		.sz_rows = 4,
		.sz_cols = 2,
		.data = data
	};

	/* Init matrix 'mt'. */
	rc = zsl_mtx_init(&mt, NULL);
	zassert_equal(rc, 0, NULL);

	rc = zsl_mtx_trans(&m, &mt);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	zassert_equal(mt.sz_cols, m.sz_rows, NULL);
	zassert_equal(mt.sz_rows, m.sz_cols, NULL);
	zassert_true(val_is_equal(mt.data[0], 2.0, 1E-5), NULL);
	zassert_true(val_is_equal(mt.data[1], 1.0, 1E-5), NULL);
	zassert_true(val_is_equal(mt.data[2], 4.0, 1E-5), NULL);
	zassert_true(val_is_equal(mt.data[3], 3.0, 1E-5), NULL);
	zassert_true(val_is_equal(mt.data[4], 3.0, 1E-5), NULL);
	zassert_true(val_is_equal(mt.data[5], 4.0, 1E-5), NULL);
	zassert_true(val_is_equal(mt.data[6], 3.0, 1E-5), NULL);
	zassert_true(val_is_equal(mt.data[7], 4.0, 1E-5), NULL);
}

ZTEST(zsl_tests, test_matrix_adjoint_3x3)
{
	int rc = 0;

	ZSL_MATRIX_DEF(mad, 3, 3);

	/* Input matrix. */
	zsl_real_t data[9] = {  2.0, -3.0,  1.0,
				-4.0,  4.0,  3.0,
				3.0,  4.0, -5.0 };

	struct zsl_mtx m = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data
	};

	/* Init matrix 'mad'. */
	rc = zsl_mtx_init(&mad, NULL);
	zassert_equal(rc, 0, NULL);

	rc = zsl_mtx_adjoint_3x3(&m, &mad);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	zassert_true(val_is_equal(mad.data[0], -32.0, 1E-6), NULL);
	zassert_true(val_is_equal(mad.data[1], -11.0, 1E-6), NULL);
	zassert_true(val_is_equal(mad.data[2], -13.0, 1E-6), NULL);
	zassert_true(val_is_equal(mad.data[3], -11.0, 1E-6), NULL);
	zassert_true(val_is_equal(mad.data[4], -13.0, 1E-6), NULL);
	zassert_true(val_is_equal(mad.data[5], -10.0, 1E-6), NULL);
	zassert_true(val_is_equal(mad.data[6], -28.0, 1E-6), NULL);
	zassert_true(val_is_equal(mad.data[7], -17.0, 1E-6), NULL);
	zassert_true(val_is_equal(mad.data[8], -4.0, 1E-6), NULL);
}

ZTEST(zsl_tests, test_matrix_adjoint)
{
	int rc = 0;

	ZSL_MATRIX_DEF(mad, 4, 4);

	/* Input matrix. */
	zsl_real_t data[16] = {  2.0, -3.0,  1.0,  5.0,
				 -4.0,  4.0,  3.0, -3.0,
				 -2.0,  6.0,  1.0,  0.0,
				 3.0,  4.0, -5.0, -8.0 };

	struct zsl_mtx m = {
		.sz_rows = 4,
		.sz_cols = 4,
		.data = data
	};

	/* Init matrix 'mad'. */
	rc = zsl_mtx_init(&mad, NULL);
	zassert_equal(rc, 0, NULL);

	rc = zsl_mtx_adjoint(&m, &mad);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	zassert_true(val_is_equal(mad.data[0],   214.0, 1E-6), NULL);
	zassert_true(val_is_equal(mad.data[1],   37.0,  1E-6), NULL);
	zassert_true(val_is_equal(mad.data[2],   206.0, 1E-6), NULL);
	zassert_true(val_is_equal(mad.data[3],  -30.0,  1E-6), NULL);
	zassert_true(val_is_equal(mad.data[4],   98.0,  1E-6), NULL);
	zassert_true(val_is_equal(mad.data[5],   3.0,   1E-6), NULL);
	zassert_true(val_is_equal(mad.data[6],   178.0, 1E-6), NULL);
	zassert_true(val_is_equal(mad.data[7],  -73.0,  1E-6), NULL);
	zassert_true(val_is_equal(mad.data[8],  -23.0,  1E-6), NULL);
	zassert_true(val_is_equal(mad.data[9],   64.0,  1E-6), NULL);
	zassert_true(val_is_equal(mad.data[10], -57.0,  1E-6), NULL);
	zassert_true(val_is_equal(mad.data[11],  59.0,  1E-6), NULL);
	zassert_true(val_is_equal(mad.data[12],  97.0,  1E-6), NULL);
	zassert_true(val_is_equal(mad.data[13],  22.0,  1E-6), NULL);
	zassert_true(val_is_equal(mad.data[14],  62.0,  1E-6), NULL);
	zassert_true(val_is_equal(mad.data[15], -38.0,  1E-6), NULL);
}

#ifndef CONFIG_ZSL_SINGLE_PRECISION
ZTEST(zsl_tests_double, test_matrix_vector_wedge)
{
	int rc = 0;

	ZSL_MATRIX_DEF(m, 5, 6);
	ZSL_MATRIX_DEF(m2, 7, 7);
	ZSL_VECTOR_DEF(v, 6);
	ZSL_VECTOR_DEF(v2, 2);
	ZSL_VECTOR_DEF(v3, 9);

	zsl_real_t a[30] = {
		2.0,  4.0, -5.0, 1.1,  8.3,  9.9,
		-0.4, -6.6,  3.4, 1.5, -6.9,  0.1,
		0.0, -2.5, -9.8, 1.2,  3.8, -0.9,
		6.1, -4.3,  8.8, 7.6, -0.9,  2.9,
		5.0,  7.1,  1.0, 0.0, -4.7,  0.2
	};

	/* Assign array to matrix. */
	rc = zsl_mtx_from_arr(&m, a);
	zassert_true(rc == 0, NULL);

	/* Compute the wedge product. */
	rc = zsl_mtx_vec_wedge(&m, &v);
	zassert_true(rc == 0, NULL);

	/* Check the results. The vector 'v' should be perpendicular to all the
	 * input vectors, the columns in the matrix 'm', so the dot product. Should
	 * be zero. */
	zsl_real_t d;

	ZSL_VECTOR_DEF(w, 6);
	for (size_t i = 0; i < 5; i++) {
		zsl_mtx_get_row(&m, i, w.data);
		zsl_vec_dot(&v, &w, &d);
		zassert_true(val_is_equal(d, 0.0, 1E-6), NULL);
	}

	/* In the following cases, the dimensions of 'm' and/or 'v' are invalid,
	 * so the function should return an error. */
	rc = zsl_mtx_vec_wedge(&m2, &v);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_mtx_vec_wedge(&m, &v2);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_mtx_vec_wedge(&m, &v3);
	zassert_true(rc == -EINVAL, NULL);
}
#endif

ZTEST(zsl_tests, test_matrix_reduce)
{
	int rc = 0;

	ZSL_MATRIX_DEF(mred, 3, 3);

	/* Input matrix. */
	zsl_real_t data[16] = {  2.0, -3.0,  1.0,  5.0,
				 -4.0,  4.0,  3.0, -3.0,
				 -2.0,  6.0,  1.0,  0.0,
				 3.0,  4.0, -5.0, -8.0 };

	struct zsl_mtx m = {
		.sz_rows = 4,
		.sz_cols = 4,
		.data = data
	};

	/* Init matrix 'mred'. */
	rc = zsl_mtx_init(&mred, NULL);
	zassert_equal(rc, 0, NULL);

	rc = zsl_mtx_reduce(&m, &mred, 0, 2);
	zassert_equal(rc, 0, NULL);

	/* Expected output. */
	zsl_real_t data2[9] = { -4.0,  4.0, -3.0,
				-2.0,  6.0,  0.0,
				3.0,  4.0, -8.0 };

	struct zsl_mtx m2 = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data2
	};

	/* Check the output. */
	for (size_t i = 0; i < (mred.sz_rows * mred.sz_cols); i++) {
		zassert_true(val_is_equal(mred.data[i], m2.data[i], 1E-6),
			     NULL);
	}

	rc = zsl_mtx_reduce(&m, &mred, 1, 3);
	zassert_equal(rc, 0, NULL);

	/* Expected output. */
	zsl_real_t data3[9] = {  2.0, -3.0,  1.0,
				 -2.0,  6.0,  1.0,
				 3.0,  4.0, -5.0 };

	struct zsl_mtx m3 = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data3
	};

	/* Check the output. */
	for (size_t i = 0; i < (mred.sz_rows * mred.sz_cols); i++) {
		zassert_true(val_is_equal(mred.data[i], m3.data[i], 1E-6),
			     NULL);
	}

	rc = zsl_mtx_reduce(&m, &mred, 1, 5);
	zassert_equal(rc, -EINVAL, NULL);
}

ZTEST(zsl_tests, test_matrix_reduce_iter)
{
	int rc = 0;
	ZSL_MATRIX_DEF(mred, 3, 3);
	ZSL_MATRIX_DEF(mred2, 2, 2);

	/* place holder matrices for iteration*/
	ZSL_MATRIX_DEF(place1, 4, 4);
	ZSL_MATRIX_DEF(place2, 4, 4);


	/* Input matrix. */
	zsl_real_t data[16] = {  2.0, -3.0,  1.0,  5.0,
				 -4.0,  4.0,  3.0, -3.0,
				 -2.0,  6.0,  1.0,  0.0,
				 3.0,  4.0, -5.0, -8.0 };

	struct zsl_mtx m = {
		.sz_rows = 4,
		.sz_cols = 4,
		.data = data
	};

	/* Init output matrices. */
	rc = zsl_mtx_init(&mred, NULL);
	zassert_equal(rc, 0, NULL);
	rc = zsl_mtx_init(&mred2, NULL);
	zassert_equal(rc, 0, NULL);

	do{
		rc = zsl_mtx_reduce_iter(&m, &mred, &place1, &place2);
	} while(rc != 0);
	zassert_equal(rc, 0, NULL);

	/* Expected output. */
	zsl_real_t data2[9] = { 4.0,  3.0, -3.0,
				6.0,  1.0,  0.0,
				4.0, -5.0, -8.0 };

	struct zsl_mtx m2 = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data2
	};

	/* Check the output. */
	for (size_t i = 0; i < (mred.sz_rows * mred.sz_cols); i++) {
		zassert_true(val_is_equal(mred.data[i], m2.data[i], 1E-6),
			     NULL);
	}

	do{
		rc = zsl_mtx_reduce_iter(&m, &mred2, &place1, &place2);
	} while(rc != 0);
	zassert_equal(rc, 0, NULL);

	/* Expected output. */
	zsl_real_t data3[4] = {  1.0,  0.0,
				 -5.0, -8.0 };
	struct zsl_mtx m3 = {
		.sz_rows = 2,
		.sz_cols = 2,
		.data = data3
	};

	/* Check the output. */
	for (size_t i = 0; i < (mred2.sz_rows * mred2.sz_cols); i++) {
		zassert_true(val_is_equal(mred2.data[i], m3.data[i], 1E-6),
			     NULL);
	}
}

ZTEST(zsl_tests, test_matrix_augm_diag)
{
	int rc = 0;

	ZSL_MATRIX_DEF(maugm, 3, 3);
	ZSL_MATRIX_DEF(maugm2, 4, 4);

	/* Input matrix. */
	zsl_real_t data[4] = {  1.0,  0.0,
				-5.0, -8.0 };

	struct zsl_mtx m = {
		.sz_rows = 2,
		.sz_cols = 2,
		.data = data
	};

	/* Init output matrices. */
	rc = zsl_mtx_init(&maugm, NULL);
	zassert_equal(rc, 0, NULL);
	rc = zsl_mtx_init(&maugm2, NULL);
	zassert_equal(rc, 0, NULL);

	rc = zsl_mtx_augm_diag(&m, &maugm);
	zassert_equal(rc, 0, NULL);

	/* Expected output. */
	zsl_real_t data2[9] = { 1.0,  0.0,  0.0,
				0.0,  1.0,  0.0,
				0.0, -5.0, -8.0 };

	struct zsl_mtx m2 = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data2
	};

	/* Check the output. */
	for (size_t i = 0; i < (maugm.sz_rows * maugm.sz_cols); i++) {
		zassert_true(val_is_equal(maugm.data[i], m2.data[i], 1E-6),
			     NULL);
	}

	rc = zsl_mtx_augm_diag(&m, &maugm2);
	zassert_equal(rc, 0, NULL);

	/* Expected output. */
	zsl_real_t data3[16] = { 1.0, 0.0,  0.0,  0.0,
				 0.0, 1.0,  0.0,  0.0,
				 0.0, 0.0,  1.0,  0.0,
				 0.0, 0.0, -5.0, -8.0 };

	struct zsl_mtx m3 = {
		.sz_rows = 4,
		.sz_cols = 4,
		.data = data3
	};

	/* Check the output. */
	for (size_t i = 0; i < (maugm2.sz_rows * maugm2.sz_cols); i++) {
		zassert_true(val_is_equal(maugm2.data[i], m3.data[i], 1E-6),
			     NULL);
	}
}

ZTEST(zsl_tests, test_matrix_deter_3x3)
{
	int rc = 0;
	zsl_real_t x = 0.0;

	/* Input matrix. */
	zsl_real_t data[9] = { 4.0,  3.0, -3.0,
			       6.0,  1.0,  0.0,
			       4.0, -5.0, -8.0 };

	struct zsl_mtx m = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data
	};

	rc = zsl_mtx_deter_3x3(&m, &x);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	zassert_equal(x, 214.0, NULL);
}

ZTEST(zsl_tests, test_matrix_deter)
{
	int rc = 0;
	zsl_real_t x = 0.0;

	/* Input matrix. */
	zsl_real_t data[25] = {  2.0, -3.0,  1.0,  5.0,  7.0,
				 -4.0,  4.0,  3.0, -3.0, -4.0,
				 5.0,  3.0,  0.0, -2.0, -1.0,
				 -2.0,  6.0,  1.0,  0.0,  8.0,
				 3.0,  4.0, -5.0, -8.0, -9.0 };
	struct zsl_mtx m = {
		.sz_rows = 5,
		.sz_cols = 5,
		.data = data
	};

	rc = zsl_mtx_deter(&m, &x);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	zassert_equal(x, -509.0, NULL);
}

ZTEST(zsl_tests, test_matrix_gauss_elim)
{
	int rc = 0;

	ZSL_MATRIX_DEF(mg, 3, 3);
	ZSL_MATRIX_DEF(mi, 3, 3);

	/* Input matrix. */
	zsl_real_t data[9] = { 4.0,  3.0,  0.0,
			       2.0,  1.0, -3.0,
			       -4.0, -5.0, -8.0 };

	struct zsl_mtx m = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data
	};

	/* Init the output matrices. */
	rc = zsl_mtx_init(&mg, NULL);
	zassert_equal(rc, 0, NULL);
	rc = zsl_mtx_init(&mi, NULL);
	zassert_equal(rc, 0, NULL);

	rc = zsl_mtx_gauss_elim(&m, &mg, &mi, 1, 0);
	zassert_equal(rc, 0, NULL);

	/* Expected output. */
	zsl_real_t data2[9] = { 0.0,  1.0,  6.0,
				2.0,  1.0, -3.0,
				0.0, -3.0, -14.0 };

	struct zsl_mtx m2 = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data2
	};

	/* Check the output. */
	for (size_t i = 0; i < (mg.sz_rows * mg.sz_cols); i++) {
		zassert_true(val_is_equal(mg.data[i], m2.data[i], 1E-6), NULL);
	}

	rc = zsl_mtx_gauss_elim(&m, &mg, &mi, 0, 2);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	for (size_t i = 0; i < (mg.sz_rows * mg.sz_cols); i++) {
		zassert_true(val_is_equal(mg.data[i], m.data[i], 1E-6), NULL);
	}
}

ZTEST(zsl_tests, test_matrix_gauss_elim_d)
{
	int rc = 0;

	ZSL_MATRIX_DEF(mi, 3, 3);
	ZSL_MATRIX_DEF(mcopy, 3, 3);

	/* Input matrix. */
	zsl_real_t data[9] = { 4.0,  3.0,  0.0,
			       2.0,  1.0, -3.0,
			       -4.0, -5.0, -8.0 };

	struct zsl_mtx m = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data
	};

	/* Init the output matrix. */
	rc = zsl_mtx_init(&mi, NULL);
	zassert_equal(rc, 0, NULL);

	rc = zsl_mtx_gauss_elim_d(&m, &mi, 1, 0);
	zassert_equal(rc, 0, NULL);

	/* Expected output. */
	zsl_real_t data2[9] = { 0.0,  1.0,  6.0,
				2.0,  1.0, -3.0,
				0.0, -3.0, -14.0 };

	struct zsl_mtx m2 = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data2
	};

	/* Check the output. */
	for (size_t i = 0; i < (m.sz_rows * m.sz_cols); i++) {
		zassert_true(val_is_equal(m.data[i], m2.data[i], 1E-6), NULL);
	}

	/* Copy 'm' into 'mcopy'. */
	rc = zsl_mtx_copy(&mcopy, &m);
	zassert_equal(rc, 0, NULL);

	rc = zsl_mtx_gauss_elim_d(&m, &mi, 2, 0);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	for (size_t i = 0; i < (m.sz_rows * m.sz_cols); i++) {
		zassert_true(val_is_equal(m.data[i], mcopy.data[i], 1E-6),
			     NULL);
	}
}

ZTEST(zsl_tests, test_matrix_gauss_reduc)
{
	int rc = 0;

	ZSL_MATRIX_DEF(va, 3, 3);
	ZSL_MATRIX_DEF(vb, 3, 3);

	/* Input matrix with range 3. */
	zsl_real_t data[9] = { 4.0,  3.0,  0.0,
			       2.0,  1.0, -3.0,
			       -4.0, -5.0, -8.0 };

	struct zsl_mtx ma = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data
	};

	/* Input matrix with range 2. */
	zsl_real_t datb[9] = { 4.0,  2.0,  0.0,
			       2.0,  1.0, -3.0,
			       -4.0, -2.0, -8.0 };

	struct zsl_mtx mb = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = datb
	};

	/* Init the output matrices. */
	rc = zsl_mtx_init(&va, NULL);
	zassert_equal(rc, 0, NULL);
	rc = zsl_mtx_init(&vb, NULL);
	zassert_equal(rc, 0, NULL);

	rc = zsl_mtx_gauss_reduc(&ma, &vb, &va);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	zsl_mtx_init(&vb, zsl_mtx_entry_fn_identity);
	for (size_t i = 0; i < (va.sz_rows * va.sz_cols); i++) {
		zassert_true(val_is_equal(va.data[i], vb.data[i], 1E-6), NULL);
	}

	rc = zsl_mtx_gauss_reduc(&mb, &va, &vb);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	zsl_mtx_init(&va, zsl_mtx_entry_fn_identity);
	zsl_mtx_set(&va, 0, 1, 0.5);
	zsl_mtx_set(&va, 1, 1, 0.0);
	for (size_t i = 0; i < (vb.sz_rows * vb.sz_cols); i++) {
		zassert_true(val_is_equal(va.data[i], vb.data[i], 1E-6), NULL);
	}
}

ZTEST(zsl_tests, test_matrix_gram_schmidt_sq)
{
	int rc;

	ZSL_MATRIX_DEF(mot, 3, 3);

	/* Input matrix. */
	zsl_real_t data[9] = { 1.0, 5.0, -1.0,
			       2.0, -4.0, -2.0,
			       4.0, 3.0, 0.0 };

	struct zsl_mtx m = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data
	};

	/* Expected output. */
	zsl_real_t dtst[9] = { 1.0, 4.5714285714, -1.2714138287,
			       2.0, -4.8571428571, -0.9824561404,
			       4.0, 1.2857142857, 0.8090815273 };

	struct zsl_mtx mt = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = dtst
	};

	/* Init matrix mot. */
	rc = zsl_mtx_init(&mot, NULL);
	zassert_equal(rc, 0, NULL);

	/* Perform the Gram-Schmidt process. */
	rc = zsl_mtx_gram_schmidt(&m, &mot);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	for (size_t g = 0; g < (m.sz_rows * m.sz_cols); g++) {
		zassert_true(val_is_equal(mot.data[g], mt.data[g], 1E-6), NULL);
	}
}

ZTEST(zsl_tests, test_matrix_gram_schmidt_rect)
{
	int rc;

	ZSL_MATRIX_DEF(mot, 4, 3);

	/* Input matrix. */
	zsl_real_t data[12] = {  1.0,  5.0,  6.0,
				 -1.0,  2.0, -2.0,
				 -4.0, -2.0,  8.0,
				 4.0,  3.0,  0.0 };

	struct zsl_mtx m = {
		.sz_rows = 4,
		.sz_cols = 3,
		.data = data
	};

	/* Expected output. */
	zsl_real_t dtst[12] = {  1.0, 4.3235294118,  2.4160177976,
				 -1.0, 2.6764705882, -5.3615127920,
				 -4.0, 0.7058823529,  4.4760845384,
				 4.0, 0.2941176471,  2.5317018910 };

	struct zsl_mtx mt = {
		.sz_rows = 4,
		.sz_cols = 3,
		.data = dtst
	};

	/* Init matrix mot. */
	rc = zsl_mtx_init(&mot, NULL);
	zassert_equal(rc, 0, NULL);

	/* Perform the Gram-Schmidt process. */
	rc = zsl_mtx_gram_schmidt(&m, &mot);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	for (size_t g = 0; g < (m.sz_rows * m.sz_cols); g++) {
		zassert_true(val_is_equal(mot.data[g], mt.data[g], 1E-6), NULL);
	}
}

ZTEST(zsl_tests, test_matrix_cols_norm)
{
	int rc = 0;

	zsl_real_t norm = 0.0;

	ZSL_MATRIX_DEF(m2, 3, 3);
	ZSL_VECTOR_DEF(v, 3);

	/* Input matrix. */
	zsl_real_t data[9] = {  67.5,  47.0,  31.5,
				32.0,  256.5, 94.5,
				226.5, 415.0, 302.0 };
	struct zsl_mtx m = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data
	};

	/* Init the output matrix. */
	rc = zsl_mtx_init(&m2, NULL);
	zassert_equal(rc, 0, NULL);

	rc = zsl_mtx_cols_norm(&m, &m2);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */

	for (size_t i = 0; i < m.sz_rows; i++) {
		zsl_mtx_get_col(&m2, i, v.data);
		norm = zsl_vec_norm(&v);
		zassert_true(val_is_equal(norm, 1.0, 1E-6), NULL);
	}
}

ZTEST(zsl_tests, test_matrix_norm_elem)
{
	int rc = 0;

	ZSL_MATRIX_DEF(m2, 3, 3);
	ZSL_MATRIX_DEF(m3, 3, 3);

	/* Input matrix. */
	zsl_real_t data[9] = {  67.5,  40.0,  31.5,
				0.0,   256.5, 94.5,
				226.5, 415.0, 302.0 };
	struct zsl_mtx m = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data
	};

	/* Init the output matrices. */
	rc = zsl_mtx_init(&m2, NULL);
	zassert_equal(rc, 0, NULL);
	rc = zsl_mtx_init(&m3, NULL);
	zassert_equal(rc, 0, NULL);

	rc = zsl_mtx_norm_elem(&m, &m2, &m3, 0, 1);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	zassert_true(val_is_equal(m2.data[0], 1.6875, 1E-6), NULL);
	zassert_true(val_is_equal(m2.data[1], 1.0, 1E-6), NULL);
	zassert_true(val_is_equal(m2.data[2], 0.7875, 1E-6), NULL);
	zassert_true(val_is_equal(m2.data[3], 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(m2.data[4], 256.5, 1E-6), NULL);
	zassert_true(val_is_equal(m2.data[5], 94.5, 1E-6), NULL);
	zassert_true(val_is_equal(m2.data[6], 226.5, 1E-6), NULL);
	zassert_true(val_is_equal(m2.data[7], 415.0, 1E-6), NULL);
	zassert_true(val_is_equal(m2.data[8], 302.0, 1E-6), NULL);

	for (size_t i = 0; i < m.sz_rows; i++) {
		zassert_equal(m3.data[i], 0.0, NULL);
	}

	rc = zsl_mtx_norm_elem(&m, &m2, &m3, 1, 0);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	zassert_true(zsl_mtx_is_equal(&m, &m2), NULL);

	for (size_t i = 0; i < m.sz_rows; i++) {
		zassert_equal(m3.data[i], 0.0, NULL);
	}
}

ZTEST(zsl_tests, test_matrix_norm_elem_d)
{
	int rc = 0;

	ZSL_MATRIX_DEF(m2, 3, 3);

	/* Input matrix. */
	zsl_real_t data[9] = {  67.5,  47.0,  31.5,
				32.0,  256.5, 94.5,
				226.5, 415.0, 302.0 };
	struct zsl_mtx m = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data
	};

	/* Init the output matrix. */
	rc = zsl_mtx_init(&m2, NULL);
	zassert_equal(rc, 0, NULL);

	rc = zsl_mtx_norm_elem_d(&m, &m2, 1, 0);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	zassert_true(val_is_equal(m.data[0], 67.5, 1E-6), NULL);
	zassert_true(val_is_equal(m.data[1], 47.0, 1E-6), NULL);
	zassert_true(val_is_equal(m.data[2], 31.5, 1E-6), NULL);
	zassert_true(val_is_equal(m.data[3], 1.0, 1E-6), NULL);
	zassert_true(val_is_equal(m.data[4], 8.015625, 1E-6), NULL);
	zassert_true(val_is_equal(m.data[5], 2.953125, 1E-6), NULL);
	zassert_true(val_is_equal(m.data[6], 226.5, 1E-6), NULL);
	zassert_true(val_is_equal(m.data[7], 415.0, 1E-6), NULL);
	zassert_true(val_is_equal(m.data[8], 302.0, 1E-6), NULL);

	for (size_t i = 0; i < m.sz_rows; i++) {
		zassert_equal(m2.data[i], 0.0, NULL);
	}
}

ZTEST(zsl_tests, test_matrix_inv_3x3)
{
	int rc = 0;

	ZSL_MATRIX_DEF(mi, 3, 3);

	/* Input matrix. */
	zsl_real_t data[9] = {  67.5,  43.0,  31.5,
				226.5, 256.5,  94.5,
				226.5, 415.0, 302.0 };
	struct zsl_mtx m = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data
	};

	/* Init matrix mi. */
	rc = zsl_mtx_init(&mi, NULL);
	zassert_equal(rc, 0, NULL);

	rc = zsl_mtx_inv_3x3(&m, &mi);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	zassert_true(val_is_equal(mi.data[0],  0.02261063, 1E-6), NULL);
	zassert_true(val_is_equal(mi.data[1],  0.00005114, 1E-6), NULL);
	zassert_true(val_is_equal(mi.data[2], -0.00237440, 1E-6), NULL);
	zassert_true(val_is_equal(mi.data[3], -0.02778553, 1E-6), NULL);
	zassert_true(val_is_equal(mi.data[4],  0.00783351, 1E-6), NULL);
	zassert_true(val_is_equal(mi.data[5],  0.00044695, 1E-6), NULL);
	zassert_true(val_is_equal(mi.data[6],  0.02122413, 1E-6), NULL);
	zassert_true(val_is_equal(mi.data[7], -0.01080295, 1E-6), NULL);
	zassert_true(val_is_equal(mi.data[8],  0.00447788, 1E-6), NULL);
}

ZTEST(zsl_tests, test_matrix_inv)
{
	int rc = 0;

	ZSL_MATRIX_DEF(mi, 5, 5);

	/* Input matrix. */
	zsl_real_t data[25] = { 1.0, 1.0, 2.0, 2.0, 1.0,
				0.0, 0.0, 0.0, 1.0, 2.0,
				0.0, 0.0, 1.0, 2.0, 2.0,
				0.0, 0.0, 1.0, 1.0, 2.0,
				0.0, 1.0, 1.0, 2.0, 1.0 };
	struct zsl_mtx m = {
		.sz_rows = 5,
		.sz_cols = 5,
		.data = data
	};

	/* The inverse of data for test purposes. */
	zsl_real_t dtst[25] = {  1.0,  1.0,  0.0, -1.0, -1.0,
				 0.0,  0.5, -1.5,  0.5,  1.0,
				 0.0, -1.0,  0.0,  1.0,  0.0,
				 -0.0, -0.0,  1.0, -1.0, -0.0,
				 -0.0,  0.5, -0.5,  0.5, -0.0 };
	struct zsl_mtx mtst = {
		.sz_rows = 5,
		.sz_cols = 5,
		.data = dtst
	};

	/* Init matrix mi. */
	rc = zsl_mtx_init(&mi, NULL);
	zassert_equal(rc, 0, NULL);

	rc = zsl_mtx_inv(&m, &mi);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	zassert_true(zsl_mtx_is_equal(&mi, &mtst), NULL);
}

ZTEST(zsl_tests, test_matrix_cholesky)
{
	int rc;

	ZSL_MATRIX_DEF(mc, 7, 4);
	ZSL_MATRIX_DEF(l, 3, 3);
	ZSL_MATRIX_DEF(l2, 5, 3);

	/* Input symmetric matrix. */
	zsl_real_t data[9] = {   4.0,  12.0, -16.0,
				 12.0,  37.0, -43.0,
				 -16.0, -43.0,  98.0 };

	struct zsl_mtx ma = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data
	};

	/* Input non-symmetric matrix. */
	zsl_real_t datb[16] = {  5.1, -3.2,  4.9, -8.1,
				 2.3, -4.1, -2.8,  0.2,
				 0.0, -7.7,  2.1,  0.0,
				 -0.7,  8.1, -5.5,  3.7 };

	struct zsl_mtx mb = {
		.sz_rows = 4,
		.sz_cols = 4,
		.data = datb
	};

	/* Expected output. */
	zsl_real_t dt[9] = {  2.0, 0.0, 0.0,
			      6.0, 1.0, 0.0,
			      -8.0, 5.0, 3.0 };

	struct zsl_mtx lo = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = dt
	};

	/* Init the output matrix. */
	rc = zsl_mtx_init(&l, NULL);
	zassert_equal(rc, 0, NULL);

	/* Compute the Cholesky decomposition. */
	rc = zsl_mtx_cholesky(&ma, &l);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	for (size_t g = 0; g < (ma.sz_rows * ma.sz_cols); g++) {
		zassert_true(val_is_equal(l.data[g], lo.data[g], 1E-6), NULL);
	}

	/* In the following examples, an error is expected due to invalid input
	 * matrices. */
	rc = zsl_mtx_cholesky(&mb, &l);
	zassert_equal(rc, -EINVAL, NULL);
	rc = zsl_mtx_cholesky(&mc, &l);
	zassert_equal(rc, -EINVAL, NULL);
	rc = zsl_mtx_cholesky(&ma, &l2);
	zassert_equal(rc, -EINVAL, NULL);
}

ZTEST(zsl_tests, test_matrix_balance)
{
	int rc;

	ZSL_MATRIX_DEF(moa, 4, 4);
	ZSL_MATRIX_DEF(mob, 4, 4);

	/* Input non-symmetric matrix. */
	zsl_real_t data[16] = { 5.1, -3.2, 4.9, -8.1,
				2.3, -4.1, -2.8, 0.2,
				0.0, -7.7, 2.1, 0.0,
				-0.7, 8.1, -5.5, 3.7 };

	struct zsl_mtx ma = {
		.sz_rows = 4,
		.sz_cols = 4,
		.data = data
	};

	/* Input symmetric matrix. */
	zsl_real_t datb[16] = { 5.1, 2.3, 0.0, -8.1,
				2.3, -4.1, -2.8, 0.2,
				0.0, -2.8, 2.1, -5.5,
				-8.1, 0.2, -5.5, 3.7 };

	struct zsl_mtx mb = {
		.sz_rows = 4,
		.sz_cols = 4,
		.data = datb
	};

	/* Expected output. */
	zsl_real_t dt[16] = { 5.1, -1.6, 2.45, -8.1,
			      4.6, -4.1, -2.8, 0.4,
			      0.0, -7.7, 2.1, 0.0,
			      -0.7, 4.05, -2.75, 3.7 };

	struct zsl_mtx mt = {
		.sz_rows = 4,
		.sz_cols = 4,
		.data = dt
	};

	/* Init output matrices. */
	rc = zsl_mtx_init(&moa, NULL);
	zassert_equal(rc, 0, NULL);
	rc = zsl_mtx_init(&mob, NULL);
	zassert_equal(rc, 0, NULL);

	/* Balance the input matrices. */
	rc = zsl_mtx_balance(&ma, &moa);
	zassert_equal(rc, 0, NULL);
	rc = zsl_mtx_balance(&mb, &mob);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	for (size_t g = 0; g < (ma.sz_rows * ma.sz_cols); g++) {
		zassert_true(val_is_equal(moa.data[g], mt.data[g], 1E-6), NULL);
		zassert_true(val_is_equal(mob.data[g], mb.data[g], 1E-6), NULL);
	}
}

ZTEST(zsl_tests, test_matrix_householder_sq)
{
	int rc;

	ZSL_MATRIX_DEF(h, 3, 3);

	/* Input matrix. */
	zsl_real_t data[9] = { 0.0, 0.0, 4.0,
			       2.0, 4.0, -2.0,
			       0.0, 4.0, 2.0 };

	struct zsl_mtx m = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data
	};

	/* Init matrix h. */
	rc = zsl_mtx_init(&h, NULL);
	zassert_equal(rc, 0, NULL);

	/* Compute the Householder matrix. */
	rc = zsl_mtx_householder(&m, &h, false);
	zassert_equal(rc, 0, NULL);

	/* Expected output. */
	zsl_real_t dtst[9] = { 0.0, 1.0, 0.0,
			       1.0, 0.0, 0.0,
			       0.0, 0.0, 1.0 };

	struct zsl_mtx mt = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = dtst
	};

	/* Check the output. */
	for (size_t g = 0; g < (m.sz_rows * m.sz_cols); g++) {
		zassert_true(val_is_equal(h.data[g], mt.data[g], 1E-6), NULL);
	}

	/* Compute the Hessenberg-Householder matrix. */
	rc = zsl_mtx_householder(&m, &h, true);
	zassert_equal(rc, 0, NULL);

	/* Expected output. */
	zsl_real_t dtsa[16] = { 1.0, 0.0, 0.0,
				0.0, 1.0, 0.0,
				0.0, 0.0, 1.0 };

	struct zsl_mtx mt2 = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = dtsa
	};

	/* Check the output. */
	for (size_t g = 0; g < (h.sz_rows * h.sz_cols); g++) {
		zassert_true(val_is_equal(h.data[g], mt2.data[g], 1E-6), NULL);
	}
}

ZTEST(zsl_tests, test_matrix_householder_rect)
{
	int rc;

	ZSL_MATRIX_DEF(h, 4, 4);

	/* Input matrix. */
	zsl_real_t data[12] = { 1.0, -1.0, 4.0,
				1.0, 4.0, -2.0,
				1.0, 4.0, 2.0,
				1.0, -1.0, 0.0 };

	struct zsl_mtx m = {
		.sz_rows = 4,
		.sz_cols = 3,
		.data = data
	};

	/* Init matrix h. */
	rc = zsl_mtx_init(&h, NULL);
	zassert_equal(rc, 0, NULL);

	/* Compute the Householder matrix. */
	rc = zsl_mtx_householder(&m, &h, false);
	zassert_equal(rc, 0, NULL);

	/* Expected output. */
	zsl_real_t dtst[16] = { 0.5, 0.5, 0.5, 0.5,
				0.5, 0.5, -0.5, -0.5,
				0.5, -0.5, 0.5, -0.5,
				0.5, -0.5, -0.5, 0.5 };

	struct zsl_mtx mt = {
		.sz_rows = 4,
		.sz_cols = 4,
		.data = dtst
	};

	/* Check the output. */
	zassert_true(zsl_mtx_is_equal(&h, &mt), NULL);

	/* Compute the Hessenberg-Householder matrix. */
	rc = zsl_mtx_householder(&m, &h, true);
	zassert_equal(rc, 0, NULL);

	/* Expected output. */
	zsl_real_t dtsa[16] = { 1.0, 0.000000, 0.000000, 0.000000,
				0.0, 0.577350, 0.577350, 0.577350,
				0.0, 0.577350, 0.211325, -0.788675,
				0.0, 0.577350, -0.788675, 0.211325 };

	struct zsl_mtx mt2 = {
		.sz_rows = 4,
		.sz_cols = 4,
		.data = dtsa
	};

	/* Check the output. */
	for (size_t g = 0; g < (h.sz_rows * h.sz_cols); g++) {
		zassert_true(val_is_equal(h.data[g], mt2.data[g], 1E-6), NULL);
	}
}

ZTEST(zsl_tests, test_matrix_qrd)
{
	int rc;

	ZSL_MATRIX_DEF(q, 3, 3);
	ZSL_MATRIX_DEF(r, 3, 3);

	/* Input matrix. */
	zsl_real_t data[9] = { 0.0, 0.0, 4.0,
			       2.0, 4.0, -2.0,
			       0.0, 4.0, 2.0 };

	struct zsl_mtx m = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = data
	};

	/* The expected results for Q and R matrices for test purposes. */
	zsl_real_t qdata[9] = { 0.0, 0.0, 1.0,
				1.0, 0.0, 0.0,
				0.0, 1.0, 0.0 };

	struct zsl_mtx q2 = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = qdata
	};

	zsl_real_t rdata[9] = { 2.0, 4.0, -2.0,
				0.0, 4.0, 2.0,
				0.0, 0.0, 4.0 };

	struct zsl_mtx r2 = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = rdata
	};


	/* Init matrices Q and R. */
	rc = zsl_mtx_init(&q, NULL);
	zassert_equal(rc, 0, NULL);
	rc = zsl_mtx_init(&r, NULL);
	zassert_equal(rc, 0, NULL);

	/* Calculate the QR decomposition. */
	rc = zsl_mtx_qrd(&m, &q, &r, false);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	for (size_t g = 0; g < (m.sz_rows * m.sz_cols); g++) {
		zassert_true(val_is_equal(q.data[g], q2.data[g], 1E-6), NULL);
		zassert_true(val_is_equal(r.data[g], r2.data[g], 1E-6), NULL);
	}
}

ZTEST(zsl_tests, test_matrix_qrd_hess)
{
	int rc;

	ZSL_MATRIX_DEF(q, 4, 4);
	ZSL_MATRIX_DEF(r, 4, 4);

	/* Input matrix. */
	zsl_real_t data[16] = { 1.0, 0.0, 4.0, -3.0,
				2.0, 4.0, -2.0, 7.0,
				-3.0, 4.0, 2.0, 0.0,
				5.0, -4.0, 0.0, 6.0 };

	struct zsl_mtx m = {
		.sz_rows = 4,
		.sz_cols = 4,
		.data = data
	};

	/* Init matrices Q and R. */
	rc = zsl_mtx_init(&q, NULL);
	zassert_equal(rc, 0, NULL);
	rc = zsl_mtx_init(&r, NULL);
	zassert_equal(rc, 0, NULL);

	/* Perform the Hessenberg transformation on the input matrix. */
	rc = zsl_mtx_qrd(&m, &q, &r, true);
	zassert_equal(rc, 0, NULL);

	/* Expected output. */
	zsl_real_t hdata[16] = {
		1.0000000000, -4.3799783705, -2.0074108339, -1.3364472371,
		6.1644140030, 5.3157894737, 4.9566180780, -3.9888797432,
		0.0000000000, -6.9114902923, 4.1365576515, 0.8655838518,
		-0.0000000000, 0.0000000000, 0.3789195885, 2.5476528748
	};

	struct zsl_mtx hess = {
		.sz_rows = 4,
		.sz_cols = 4,
		.data = hdata
	};


	/* Check the output. */
	for (size_t g = 0; g < (m.sz_rows * m.sz_cols); g++) {
#ifdef CONFIG_ZSL_SINGLE_PRECISION
		zassert_true(val_is_equal(r.data[g], hess.data[g], 1E-4), NULL);
#else
		zassert_true(val_is_equal(r.data[g], hess.data[g], 1E-8), NULL);
#endif
	}
}

#ifndef CONFIG_ZSL_SINGLE_PRECISION
ZTEST(zsl_tests_double, test_matrix_qrd_iter)
{
	int rc;

	ZSL_MATRIX_DEF(m2, 4, 4);
	ZSL_VECTOR_DEF(v, 4);
	ZSL_VECTOR_DEF(v2, 4);

	/* Input matrix. */
	zsl_real_t data[16] = { 1.0, 2.0, -1.0, 0.0,
				0.0, 3.0, 4.0, -2.0,
				4.0, 4.0, -3.0, 0.0,
				5.0, 3.0, -5.0, 2.0 };

	struct zsl_mtx m = {
		.sz_rows = 4,
		.sz_cols = 4,
		.data = data
	};

	/* Init the output matrix. */
	rc = zsl_mtx_init(&m2, NULL);
	zassert_equal(rc, 0, NULL);

	/* Perform the QR method 1500 times matrix. */
	rc = zsl_mtx_qrd_iter(&m, &m2, 1500);
	zassert_equal(rc, 0, NULL);

	/* Check if the output matrix is upper triangular and if it is similar
	 * to the input matrix. */
	zassert_true(val_is_equal(m2.data[4], 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(m2.data[8], 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(m2.data[9], 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(m2.data[12], 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(m2.data[13], 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(m2.data[14], 0.0, 1E-6), NULL);

	zsl_mtx_eigenvalues(&m, &v, 500);
	zsl_mtx_eigenvalues(&m2, &v2, 500);

	zassert_true(zsl_vec_is_equal(&v, &v2, 1E-6), NULL);
}
#endif

#ifndef CONFIG_ZSL_SINGLE_PRECISION
ZTEST(zsl_tests_double, test_matrix_eigenvalues)
{
	int rc;

	ZSL_VECTOR_DEF(va, 4);
	ZSL_VECTOR_DEF(vb, 4);
	ZSL_VECTOR_DEF(vc, 4);

	ZSL_VECTOR_DEF(va2, 4);
	ZSL_VECTOR_DEF(vb2, 2);
	ZSL_VECTOR_DEF(vc2, 4);

	/* Input real-eigenvalue matrix. */
	zsl_real_t data[16] = { 1.0, 2.0, -1.0, 0.0,
				0.0, 3.0, 4.0, -2.0,
				4.0, 4.0, -3.0, 0.0,
				5.0, 3.0, -5.0, 2.0 };

	struct zsl_mtx ma = {
		.sz_rows = 4,
		.sz_cols = 4,
		.data = data
	};

	/* Input complex-eigenvalue matrix. */
	zsl_real_t datb[16] = { 1.0, 2.0, -1.0, 0.0,
				0.0, 3.0, 4.0, -2.0,
				4.0, 4.0, -3.0, 0.0,
				9.0, 3.0, -5.0, 2.0 };

	struct zsl_mtx mb = {
		.sz_rows = 4,
		.sz_cols = 4,
		.data = datb
	};

	/* Input symmetric matrix. */
	zsl_real_t datc[16] = { 1.0, 2.0, 4.0, 0.0,
				2.0, 3.0, 4.0, -2.0,
				4.0, 4.0, -3.0, 5.0,
				0.0, -2.0, 5.0, -1.0 };

	struct zsl_mtx mc = {
		.sz_rows = 4,
		.sz_cols = 4,
		.data = datc
	};

	/* Expected output. */
	va2.data[0] = 4.8347780554139375;
	va2.data[1] = -2.6841592178899276;
	va2.data[2] = 1.8493811427083884;
	va2.data[3] = -0.9999999802303374;

	vb2.data[0] = -3.0925670160610634;
	vb2.data[1] = -1.0000000075030784;

	vc2.data[0] = -9.2890349032381003;
	vc2.data[1] = 7.4199113544017665;
	vc2.data[2] = 2.7935849909013921;
	vc2.data[3] = -0.9244614420638188;


	/* Init the output vectors. */
	rc = zsl_vec_init(&va);
	zassert_equal(rc, 0, NULL);
	rc = zsl_vec_init(&vb);
	zassert_equal(rc, 0, NULL);
	rc = zsl_vec_init(&vc);
	zassert_equal(rc, 0, NULL);

	/* Calculate the eigenvalues of 'ma', 'mb' and 'mc'. */
	rc = zsl_mtx_eigenvalues(&ma, &va, 150);
	zassert_equal(rc, 0, NULL);

	rc = zsl_mtx_eigenvalues(&mb, &vb, 150);
	zassert_equal(rc, -ECOMPLEXVAL, NULL);

	rc = zsl_mtx_eigenvalues(&mc, &vc, 150);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	zassert_true(zsl_vec_is_equal(&va, &va2, 1E-6), NULL);
	zassert_true(zsl_vec_is_equal(&vb, &vb2, 1E-6), NULL);
	zassert_true(zsl_vec_is_equal(&vc, &vc2, 1E-6), NULL);
}
#endif

#ifndef CONFIG_ZSL_SINGLE_PRECISION
ZTEST(zsl_tests_double, test_matrix_eigenvectors)
{
	int rc;

	ZSL_MATRIX_DEF(va, 4, 4);
	ZSL_MATRIX_DEF(vb, 4, 4);
	ZSL_MATRIX_DEF(vc, 4, 4);
	ZSL_MATRIX_DEF(vd, 3, 3);

	ZSL_MATRIX_DEF(va2, 4, 4);
	ZSL_MATRIX_DEF(vb2, 4, 2);
	ZSL_MATRIX_DEF(vc2, 4, 2);
	ZSL_MATRIX_DEF(vd2, 3, 3);

	/* Input real-eigenvalue matrix. */
	zsl_real_t data[16] = { 1.0, 2.0, -1.0, 0.0,
				0.0, 3.0, 4.0, -2.0,
				4.0, 4.0, -3.0, 0.0,
				5.0, 3.0, -5.0, 2.0 };

	struct zsl_mtx ma = {
		.sz_rows = 4,
		.sz_cols = 4,
		.data = data
	};

	/* Input complex-eigenvalue matrix. */
	zsl_real_t datb[16] = { 1.0, 2.0, -1.0, 0.0,
				0.0, 3.0, 4.0, -2.0,
				4.0, 4.0, -3.0, 0.0,
				9.0, 3.0, -5.0, 2.0 };

	struct zsl_mtx mb = {
		.sz_rows = 4,
		.sz_cols = 4,
		.data = datb
	};

	/* Input real-eigenvalue non-diagonalisable matrix with repeated
	 * eigenvalues. */
	zsl_real_t datc[16] = { 1.0, 2.0, 4.0, 0.0,
				0.0, 3.0, 4.0, -2.0,
				0.0, 0.0, 3.0, 5.0,
				0.0, 0.0, 0.0, 1.0 };

	struct zsl_mtx mc = {
		.sz_rows = 4,
		.sz_cols = 4,
		.data = datc
	};

	/* Input real-eigenvalue diagonalisable matrix with repeated
	 * eigenvalues. */
	zsl_real_t datd[9] = { 5.0, -4.0, 4.0,
			       12.0, -11.0, 12.0,
			       4.0, -4.0, 5.0 };

	struct zsl_mtx md = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = datd
	};

	/* Init the output matrices. */
	rc = zsl_mtx_init(&va, NULL);
	zassert_equal(rc, 0, NULL);
	rc = zsl_mtx_init(&vb, NULL);
	zassert_equal(rc, 0, NULL);
	rc = zsl_mtx_init(&vc, NULL);
	zassert_equal(rc, 0, NULL);
	rc = zsl_mtx_init(&vd, NULL);
	zassert_equal(rc, 0, NULL);

	/* Calculate the eigenvectors of 'ma', 'mb' and 'mc'
	 * non-orthonormalised. */
	rc = zsl_mtx_eigenvectors(&ma, &va, 1500, false);
	zassert_equal(rc, 0, NULL);
	rc = zsl_mtx_eigenvectors(&mb, &vb, 1500, false);
	zassert_equal(rc, -EEIGENSIZE, NULL);
	rc = zsl_mtx_eigenvectors(&mc, &vc, 1500, false);
	zassert_equal(rc, -EEIGENSIZE, NULL);
	rc = zsl_mtx_eigenvectors(&md, &vd, 1500, false);
	zassert_equal(rc, 0, NULL);

	/* Expected output. */
	zsl_real_t a[16] = {
		0.7555042357,  0.6223771803,  0.2074844660,  5.5000009316,
		2.2040997676, -0.5240911326,  0.2956011625, -3.5000005906,
		1.5110084714,  1.2447543606,  0.4149689321,  4.0000005733,
		1.0000000000,  1.0000000000,  1.0000000000,  1.0000000000
	};

	zsl_real_t b[8] = { 1.2304303063, -0.5000000054,
			    -1.2873789372, 0.5000000045,
			    2.4608606125, -0.0000000055,
			    1.0000000000, 1.0000000000 };

	zsl_real_t c[8] = { 1.0, 1.0,
			    0.0, 1.0,
			    0.0, 0.0,
			    0.0, 0.0 };

	zsl_real_t d[9] = { 1.0, 1.0, -1.0,
			    3.0, 1.0, 0.0,
			    1.0, 0.0, 1.0 };

	rc = zsl_mtx_from_arr(&va2, a);
	zassert_equal(rc, 0, NULL);
	rc = zsl_mtx_from_arr(&vb2, b);
	zassert_equal(rc, 0, NULL);
	rc = zsl_mtx_from_arr(&vc2, c);
	zassert_equal(rc, 0, NULL);
	rc = zsl_mtx_from_arr(&vd2, d);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	for (size_t g = 0; g < (va.sz_rows * va.sz_cols); g++) {
		zassert_true(val_is_equal(va.data[g], va2.data[g], 1E-6), NULL);
	}

	for (size_t g = 0; g < (vb.sz_rows * vb.sz_cols); g++) {
		zassert_true(val_is_equal(vb.data[g], vb2.data[g], 1E-6), NULL);
		zassert_true(val_is_equal(vc.data[g], vc2.data[g], 1E-6), NULL);
	}

	for (size_t g = 0; g < (vd.sz_rows * vd.sz_cols); g++) {
		zassert_true(val_is_equal(vd.data[g], vd2.data[g], 1E-6), NULL);
	}

	/* Calculate the eigenvectors of 'ma', 'mb' and 'mc' orthonormalised. */
	rc = zsl_mtx_eigenvectors(&ma, &va, 1500, true);
	zassert_equal(rc, 0, NULL);
	rc = zsl_mtx_eigenvectors(&mb, &vb, 1500, true);
	zassert_equal(rc, -EEIGENSIZE, NULL);
	rc = zsl_mtx_eigenvectors(&mc, &vc, 1500, true);
	zassert_equal(rc, -EEIGENSIZE, NULL);
	rc = zsl_mtx_eigenvectors(&md, &vd, 1500, true);
	zassert_equal(rc, 0, NULL);

	/* Expected output. */
	zsl_real_t a2[16] = {
		0.2559636199, 0.3472992698, 0.1817921832, 0.7130241030,
		0.7467454562, -0.2924536333, 0.2589976094, -0.4537426107,
		0.5119272398, 0.6945985397, 0.3635843664, 0.5185629705,
		0.3387983916, 0.5580205715, 0.8761724995, 0.1296407240
	};


	zsl_real_t b2[8] = { 0.3847511767, -0.4082482935,
			     -0.4025588109, 0.4082482928,
			     0.7695023535, -0.0000000045,
			     0.3126964402, 0.8164965782 };

	zsl_real_t c2[8] = { 1.0, 0.7071067812,
			     0.0, 0.7071067812,
			     0.0, 0.0,
			     0.0, 0.0 };

	zsl_real_t d2[9] = { 0.3015113441, 0.7071067750, -0.4082482533,
			     0.9045340323, 0.7071067874, 0.4082481887,
			     0.3015113494, 0.0000000498, 0.8164966504 };

	rc = zsl_mtx_from_arr(&va2, a2);
	zassert_equal(rc, 0, NULL);
	rc = zsl_mtx_from_arr(&vb2, b2);
	zassert_equal(rc, 0, NULL);
	rc = zsl_mtx_from_arr(&vc2, c2);
	zassert_equal(rc, 0, NULL);
	rc = zsl_mtx_from_arr(&vd2, d2);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	for (size_t g = 0; g < (va.sz_rows * va.sz_cols); g++) {
		zassert_true(val_is_equal(va.data[g], va2.data[g], 1E-6), NULL);
	}

	for (size_t g = 0; g < (vb.sz_rows * vb.sz_cols); g++) {
		zassert_true(val_is_equal(vb.data[g], vb2.data[g], 1E-6), NULL);
		zassert_true(val_is_equal(vc.data[g], vc2.data[g], 1E-6), NULL);
	}

	for (size_t g = 0; g < (vd.sz_rows * vd.sz_cols); g++) {
		zassert_true(val_is_equal(vd.data[g], vd2.data[g], 1E-6), NULL);
	}
}
#endif

#ifndef CONFIG_ZSL_SINGLE_PRECISION
ZTEST(zsl_tests_double, test_matrix_svd)
{
	int rc;

	ZSL_MATRIX_DEF(u, 3, 3);
	ZSL_MATRIX_DEF(e, 3, 4);
	ZSL_MATRIX_DEF(v, 4, 4);

	ZSL_MATRIX_DEF(u2, 3, 3);
	ZSL_MATRIX_DEF(e2, 3, 4);
	ZSL_MATRIX_DEF(v2, 4, 4);

	/* Input  matrix. */
	zsl_real_t data[12] = { 1.0, 2.0, -1.0, 0.0,
				0.0, 3.0, 4.0, -2.0,
				4.0, 4.0, -3.0, 0.0 };

	struct zsl_mtx m = {
		.sz_rows = 3,
		.sz_cols = 4,
		.data = data
	};

	/* Init the output matrices. */
	rc = zsl_mtx_init(&u, NULL);
	zassert_equal(rc, 0, NULL);
	rc = zsl_mtx_init(&e, NULL);
	zassert_equal(rc, 0, NULL);
	rc = zsl_mtx_init(&v, NULL);
	zassert_equal(rc, 0, NULL);

	/* Calculate the svd of 'm'. */
	rc = zsl_mtx_svd(&m, &u, &e, &v, 1500);
	zassert_equal(rc, 0, NULL);

	/* Expected output. */
	zsl_real_t a[9] = { -0.3481845133, -0.0474852763, 0.9362225661,
			    -0.0396196056, -0.9970784021, -0.0653065614,
			    -0.9365884003, 0.0598315021, -0.3452859102 };

	zsl_real_t b[12] = { 6.8246886030, 0.0, 0.0, 0.0,
			     0.0, 5.3940011894, 0.0, 0.0,
			     0.0, 0.0, 0.5730415692, 0.0 };

	zsl_real_t c[16] = { -0.5999596982, 0.0355655710, -0.7764202435,
			     0.1896181853, -0.6683940777, -0.5277862667, 0.5154631408, 0.0948090926,
			     0.4395030259, -0.7638713255, -0.2819884104, 0.3792363705, 0.0116106706,
			     0.3696989923, 0.2279295777, 0.9006863800 };

	rc = zsl_mtx_from_arr(&u2, a);
	zassert_equal(rc, 0, NULL);
	rc = zsl_mtx_from_arr(&e2, b);
	zassert_equal(rc, 0, NULL);
	rc = zsl_mtx_from_arr(&v2, c);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	for (size_t g = 0; g < (u.sz_rows * u.sz_cols); g++) {
		zassert_true(val_is_equal(u.data[g], u2.data[g], 1E-8), NULL);
	}

	for (size_t g = 0; g < (e.sz_rows * e.sz_cols); g++) {
		zassert_true(val_is_equal(e.data[g], e2.data[g], 1E-8), NULL);
	}

	for (size_t g = 0; g < (v.sz_rows * v.sz_cols); g++) {
		zassert_true(val_is_equal(v.data[g], v2.data[g], 1E-8), NULL);
	}
}
#endif

#ifndef CONFIG_ZSL_SINGLE_PRECISION
ZTEST(zsl_tests_double, test_matrix_pinv)
{
	int rc;

	ZSL_MATRIX_DEF(pinv, 4, 3);

	ZSL_MATRIX_DEF(pinv2, 4, 3);

	/* Input  matrix. */
	zsl_real_t data[12] = { 1.0, 2.0, -1.0, 0.0,
				0.0, 3.0, 4.0, -2.0,
				4.0, 4.0, -3.0, 0.0 };

	struct zsl_mtx m = {
		.sz_rows = 3,
		.sz_cols = 4,
		.data = data
	};

	/* Init the output matrix. */
	rc = zsl_mtx_init(&pinv, NULL);
	zassert_equal(rc, 0, NULL);

	/* Calculate the pseudo-inverse of 'm'. */
	rc = zsl_mtx_pinv(&m, &pinv, 1500);
	zassert_equal(rc, 0, NULL);

	/* Expected output. */
	zsl_real_t a[12] = {
		-1.2382022472, 0.0853932584, 0.5505617978,
		0.8808988764, 0.0426966292, -0.2247191011,
		-0.4764044944, 0.1707865169, 0.1011235955,
		0.3685393258, -0.0943820225, -0.1348314607
	};

	rc = zsl_mtx_from_arr(&pinv2, a);
	zassert_equal(rc, 0, NULL);

	/* Check the output. */
	for (size_t g = 0; g < (pinv.sz_rows * pinv.sz_cols); g++) {
		zassert_true(val_is_equal(pinv.data[g], pinv2.data[g], 1E-8),
			     NULL);
	}
}
#endif

ZTEST(zsl_tests, test_matrix_min)
{
	int rc = 0;
	zsl_real_t min;

	/* Input matrix. */
	zsl_real_t data[8] = { 2.0, 3.0,
			       1.0, 4.0,
			       4.0, 3.0,
			       3.0, 4.0 };
	struct zsl_mtx m = {
		.sz_rows = 4,
		.sz_cols = 2,
		.data = data
	};

	rc = zsl_mtx_min(&m, &min);
	zassert_equal(rc, 0, NULL);
	zassert_equal(min, 1.0, NULL);
}

ZTEST(zsl_tests, test_matrix_max)
{
	int rc = 0;
	zsl_real_t max;

	/* Input matrix. */
	zsl_real_t data[8] = { 2.0, 3.0,
			       1.0, 4.0,
			       4.0, 3.0,
			       3.0, 4.0 };
	struct zsl_mtx m = {
		.sz_rows = 4,
		.sz_cols = 2,
		.data = data
	};

	rc = zsl_mtx_max(&m, &max);
	zassert_equal(rc, 0, NULL);
	zassert_equal(max, 4.0, NULL);
}

ZTEST(zsl_tests, test_matrix_min_idx)
{
	int rc = 0;
	size_t min_i;
	size_t min_j;

	/* Input matrix. */
	zsl_real_t data[8] = { 2.0, 3.0,
			       1.0, 4.0,
			       4.0, 3.0,
			       3.0, 4.0 };
	struct zsl_mtx m = {
		.sz_rows = 4,
		.sz_cols = 2,
		.data = data
	};

	rc = zsl_mtx_min_idx(&m, &min_i, &min_j);
	zassert_equal(rc, 0, NULL);
	zassert_equal(min_i, 1, NULL);
	zassert_equal(min_j, 0, NULL);
}

ZTEST(zsl_tests, test_matrix_max_idx)
{
	int rc = 0;
	size_t max_i;
	size_t max_j;


	/* Input matrix. */
	zsl_real_t data[8] = { 2.0, 3.0,
			       1.0, 4.0,
			       4.0, 3.0,
			       3.0, 4.0 };
	struct zsl_mtx m = {
		.sz_rows = 4,
		.sz_cols = 2,
		.data = data
	};

	rc = zsl_mtx_max_idx(&m, &max_i, &max_j);
	zassert_equal(rc, 0, NULL);
	zassert_equal(max_i, 1, NULL);
	zassert_equal(max_j, 1, NULL);
}

ZTEST(zsl_tests, test_matrix_is_equal)
{
	bool res;

	zsl_real_t data_a[8] = { 2.0, 3.0,
				 1.0, 4.0,
				 4.0, 3.0,
				 3.0, 4.0 };
	struct zsl_mtx ma = {
		.sz_rows = 4,
		.sz_cols = 2,
		.data = data_a
	};

	zsl_real_t data_b[8] = { 2.0, 3.0,
				 1.0, 4.0,
				 4.0, 3.0,
				 3.0, 4.0 };
	struct zsl_mtx mb = {
		.sz_rows = 4,
		.sz_cols = 2,
		.data = data_b
	};

	/* Perform a test of equal elements. */
	res = zsl_mtx_is_equal(&ma, &mb);
	zassert_equal(res, true, NULL);

	/* Perform a test of unequal elements. */
	zsl_mtx_set(&mb, 1, 1, 0.5);
	res = zsl_mtx_is_equal(&ma, &mb);
	zassert_equal(res, false, "");
}

ZTEST(zsl_tests, test_matrix_is_notneg)
{
	bool res;

	/* Input matrix. */
	zsl_real_t data[8] = { 2.0, 3.0,
			       1.0, 4.0,
			       4.0, 3.0,
			       3.0, 4.0 };
	struct zsl_mtx m = {
		.sz_rows = 4,
		.sz_cols = 2,
		.data = data
	};

	res = zsl_mtx_is_notneg(&m);
	zassert_equal(res, true, NULL);

	zsl_mtx_set(&m, 1, 1, -0.01);
	res = zsl_mtx_is_notneg(&m);
	zassert_equal(res, false, NULL);
}

ZTEST(zsl_tests, test_matrix_is_sym)
{
	bool res;

	/* Input matrixes. */
	zsl_real_t a[9] = { 2.0, 3.0, 6.0,
			    3.0, 4.0, -1.0,
			    6.0, -1.0, 0.0 };
	struct zsl_mtx ma = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = a
	};

	zsl_real_t b[9] = { 5.0, 3.0, 4.0,
			    7.0, -5.0, 0.0,
			    3.0, -2.0, 3.0 };
	struct zsl_mtx mb = {
		.sz_rows = 3,
		.sz_cols = 3,
		.data = b
	};

	/* Perform a test with a symmetric matrix. */
	res = zsl_mtx_is_sym(&ma);
	zassert_equal(res, true, NULL);

	/* Perform a test with a non-symmetric matrix. */
	res = zsl_mtx_is_sym(&mb);
	zassert_equal(res, false, NULL);
}
