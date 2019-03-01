/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include "floatcheck.h"

/**
 * @brief zsl_mtx_init unit tests.
 *
 * This test verifies the zsl_mtx_init function.
 */
void test_matrix_init(void)
{
    int rc;
    zsl_real_t x;
    ZSL_MATRIX_STATIC_INIT(m, 3, 3);

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
    rc = zsl_mtx_init(&m, zsl_mtx_entry_fn_diagonal);
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
void test_matrix_from_arr(void)
{
    int rc;
    zsl_real_t x;

    /* Destination matrix. */
    ZSL_MATRIX_STATIC_INIT(m, 3, 3);

    /* Source array. */
    zsl_real_t data[9] = { 1.0, 0.0, 0.0,
                           0.0, 0.5, 0.0,
                           0.0, 0.0, 0.1 };


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
 * @brief zsl_mtx_get unit tests.
 *
 * This test verifies the zsl_mtx_get function.
 */
void test_matrix_get(void)
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

    /* Check for out of bounbds error. */
    zassert_true(zsl_mtx_get(&m, 3, 3, &x) == -EINVAL, NULL);
}

/**
 * @brief zsl_mtx_set unit tests.
 *
 * This test verifies the zsl_mtx_set function.
 */
void test_matrix_set(void)
{
    int rc = 0;
    zsl_real_t x;
    ZSL_MATRIX_STATIC_INIT(m, 3, 3);

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

void test_matrix_get_set_row(void)
{

}

void test_matrix_get_set_col(void)
{

}

void test_matrix_add(void)
{

}

void test_matrix_add_d(void)
{

}

void test_matrix_sub(void)
{

}

void test_matrix_sub_d(void)
{

}

/**
 * @brief zsl_mtx_mult unit tests with square matrices.
 *
 * This test verifies the zsl_mtx_mult function with square matrices.
 */
void test_matrix_mult_sq(void)
{
    int rc = 0;
    ZSL_MATRIX_STATIC_INIT(mc, 3, 3);

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
void test_matrix_mult_rect(void)
{
    int rc = 0;
    ZSL_MATRIX_STATIC_INIT(mc, 4, 3);
    ZSL_MATRIX_STATIC_INIT(merr, 5, 3);

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

/**
 * @brief zsl_mtx_scalar_mult unit tests.
 *
 * This test verifies the zsl_mtx_scalar_mult function.
 */
void test_matrix_scalar_mult(void)
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

    rc = zsl_mtx_scalar_mult(&m, s);
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

/**
 * @brief zsl_mtx_scalar_trans unit tests.
 *
 * This test verifies the zsl_mtx_trans function.
 */
void test_matrix_trans(void)
{
    int rc = 0;

    ZSL_MATRIX_STATIC_INIT(mt, 2, 4);

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

    rc = zsl_mtx_trans(&m, &mt);
    zassert_equal(rc, 0, NULL);
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

void test_matrix_adj(void)
{

}

void test_matrix_deter(void)
{

}

void test_matrix_inv(void)
{
    int rc = 0;

    ZSL_MATRIX_STATIC_INIT(mi, 3, 3);

    /* Input matrix. */
    zsl_real_t data[9] = {  67.5,  43.0,  31.5,
                           226.5, 256.5,  94.5,
                           226.5, 415.0, 302.0 };
    struct zsl_mtx m = {
        .sz_rows = 3,
        .sz_cols = 3,
        .data = data
    };

    rc = zsl_mtx_inv(&m, &mi);
    zassert_equal(rc, 0, NULL);
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

void test_matrix_min(void)
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

void test_matrix_max(void)
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

void test_matrix_min_idx(void)
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

void test_matrix_max_idx(void)
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

void test_matrix_is_equal(void)
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

void test_matrix_is_notneg(void)
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
