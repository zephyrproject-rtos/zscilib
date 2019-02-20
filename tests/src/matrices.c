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
    zsl_data_t x;
    ZSL_MATRIX_DEF(m, 3, 3);

    /* Initialise the matrix with the default (empty) entry_fn. */
    rc = zsl_mtx_init(&m, NULL);
    zassert_equal(rc, 0, "init:NULL == ERR");

    rc = zsl_mtx_get(&m, 1, 0, &x);
    zassert_equal(rc, 0, "init:empty:mtx[1][0] == ERR");
    zassert_equal(x, 0.0, "init:empty:mtx[1][0] != 0.0");

    rc = zsl_mtx_get(&m, 2, 2, &x);
    zassert_equal(rc, 0, "init:empty:mtx[2][2] == ERR");
    zassert_equal(x, 0.0, "init:empty:mtx[2][2] != 0.0");

    /* Reinitialise the matrix as an identity/diagonal matrix. */
    rc = zsl_mtx_init(&m, zsl_mtx_entry_fn_diagonal);
    zassert_equal(rc, 0, "init:diag == ERR");

    rc = zsl_mtx_get(&m, 0, 0, &x);
    zassert_equal(rc, 0, "init:diag:mtx[0][0] == ERR");
    zassert_true(val_is_equal(x, 1.0, 1E-5), "init:diag:mtx[0][0] != 1.0");

    rc = zsl_mtx_get(&m, 1, 1, &x);
    zassert_equal(rc, 0, "init:diag:mtx[1][1] == ERR");
    zassert_true(val_is_equal(x, 1.0, 1E-5), "init::diag:mtx[1][1] != 1.0");

    rc = zsl_mtx_get(&m, 2, 2, &x);
    zassert_equal(rc, 0, "init:diag:mtx[2][2] == ERR");
    zassert_true(val_is_equal(x, 1.0, 1E-5), "init::diag:mtx[2][2] != 1.0");

    rc = zsl_mtx_get(&m, 0, 1, &x);
    zassert_equal(x, 0.0, "init::diag:mtx[0][1] != 0.0");
    zassert_equal(rc, 0, "init:diag:mtx[0][1] == ERR");
}

/**
 * @brief zsl_mtx_from_arr unit tests.
 *
 * This test verifies the zsl_mtx_from_arr function.
 */
void test_matrix_from_arr(void)
{
    int rc;
    zsl_data_t x;

    /* Destination matrix. */
    ZSL_MATRIX_DEF(m, 3, 3);

    /* Source array. */
    zsl_data_t data[9] = { 1.0, 0.0, 0.0,
                           0.0, 0.5, 0.0,
                           0.0, 0.0, 0.1 };


    /* Initialise the matrix with the default (empty) entry_fn. */
    rc = zsl_mtx_from_arr(&m, data);
    zassert_true(rc == 0, "zsl_mtx_from_arr != 0");

    rc = zsl_mtx_get(&m, 0, 0, &x);
    zassert_equal(rc, 0, "from_arr:mtx[0][0] == ERR");
    zassert_true(val_is_equal(x, 1.0, 1E-5), "from_arr:mtx[0][0] != 1.0");

    rc = zsl_mtx_get(&m, 1, 1, &x);
    zassert_equal(rc, 0, "from_arr:mtx[1][1] == ERR");
    zassert_true(val_is_equal(x, 0.5, 1E-5), "from_arr:mtx[1][1] != 0.5");

    rc = zsl_mtx_get(&m, 2, 2, &x);
    zassert_equal(rc, 0, "from_arr:mtx[2][2] == ERR");
    zassert_true(val_is_equal(x, 0.1, 1E-5), "from_arr:mtx[2][2] != 0.1");

    rc = zsl_mtx_get(&m, 1, 0, &x);
    zassert_equal(rc, 0, "from_arr:mtx[1][0] == ERR");
    zassert_equal(x, 0.0, "from_arr:mtx[1][0] != 0.0");
}

/**
 * @brief zsl_mtx_get unit tests.
 *
 * This test verifies the zsl_mtx_get function.
 */
void test_matrix_get(void)
{
    int rc;
    zsl_data_t x;
    zsl_data_t data[9] = { 1.0, 0.0, 0.0,
                           0.0, 0.5, 0.0,
                           0.0, 0.0, 0.1 };

    struct zsl_mtx m = {
        .sz_rows = 3,
        .sz_cols = 3,
        .data = data
    };

    /* Read values from the matrix above. */
    rc = zsl_mtx_get(&m, 0, 0, &x);
    zassert_equal(rc, 0, "get:mtx[0][0] == ERR");
    zassert_true(val_is_equal(x, 1.0, 1E-5), "get:mtx[0][0] != 1.0");

    rc = zsl_mtx_get(&m, 1, 0, &x);
    zassert_equal(rc, 0, "get:mtx[1][0] == ERR");
    zassert_equal(x, 0.0, "get:mtx[1][0] != 0.0");

    rc = zsl_mtx_get(&m, 2, 2, &x);
    zassert_equal(rc, 0, "get:mtx[2][2] == ERR");
    zassert_true(val_is_equal(x, 0.1, 1E-5), "get:mtx[2][2] != 0.1");

    /* Check for out of bounbds error. */
    zassert_true(zsl_mtx_get(&m, 3, 3, &x) == -EINVAL, "get:mtx[3][3] != ERR");
}

/**
 * @brief zsl_mtx_set unit tests.
 *
 * This test verifies the zsl_mtx_set function.
 */
void test_matrix_set(void)
{
    int rc = 0;
    zsl_data_t x;
    ZSL_MATRIX_DEF(m, 3, 3);

    /* Initialise the matrix with the default (empty) entry_fn. */
    rc = zsl_mtx_init(&m, NULL);
    zassert_equal(rc, 0, "set:mtx:init == ERR");

    /* Set values in matrix m. */
    rc = zsl_mtx_set(&m, 0, 0, 1.0);
    zassert_equal(rc, 0, "set:mtx[0][0] == ERR");

    rc = zsl_mtx_set(&m, 1, 1, 0.5);
    zassert_equal(rc, 0, "set:mtx[1][1] == ERR");

    rc = zsl_mtx_set(&m, 2, 2, 0.1);
    zassert_equal(rc, 0, "set:mtx[2][2] == ERR");

    /* Verify assigned values */
    rc = zsl_mtx_get(&m, 0, 0, &x);
    zassert_equal(rc, 0, "set:get:mtx[0][0] == ERR");
    zassert_true(val_is_equal(x, 1.0, 1E-5), "set:get:mtx[0][0] != 1.0");

    rc = zsl_mtx_get(&m, 1, 1, &x);
    zassert_equal(rc, 0, "set:get:mtx[1][1] == ERR");
    zassert_true(val_is_equal(x, 0.5, 1E-5), "set:get:mtx[1][1] != 0.5");

    rc = zsl_mtx_get(&m, 2, 2, &x);
    zassert_equal(rc, 0, "set:get:mtx[2][2] == ERR");
    zassert_true(val_is_equal(x, 0.1, 1E-5), "set:get:mtx[2][2] != 0.1");

    /* Check for out of bounbds error. */
    zassert_true(zsl_mtx_set(&m, 3, 3, 0.0) == -EINVAL, "set:mtx[3][3] != ERR");
}

/**
 * @brief zsl_mtx_mult unit tests with square matrices.
 *
 * This test verifies the zsl_mtx_mult function with square matrices.
 */
void test_matrix_mult_sq(void)
{
    int rc = 0;
    ZSL_MATRIX_DEF(mc, 3, 3);

    /* Input matrix a. */
    zsl_data_t data_a[9] = { 1.0, 2.0, 3.0,
                             4.0, 5.0, 6.0,
                             7.0, 8.0, 9.0 };
    struct zsl_mtx ma = {
        .sz_rows = 3,
        .sz_cols = 3,
        .data = data_a
    };

    /* Input matrix b. */
    zsl_data_t data_b[9] = { 10.0, 20.0, 30.0,
                             40.0, 50.0, 60.0,
                             70.0, 80.0, 90.0 };
    struct zsl_mtx mb = {
        .sz_rows = 3,
        .sz_cols = 3,
        .data = data_b
    };

    /* Output reference matrix (for comparison). */
    zsl_data_t data_ref[9] = { 300.0,  360.0,  420.0,
                               660.0,  810.0,  960.0,
                               1020.0, 1260.0, 1500.0 };
    struct zsl_mtx mref = {
        .sz_rows = 3,
        .sz_cols = 3,
        .data = data_ref
    };

    /* Initialise the output matrix. */
    rc = zsl_mtx_init(&mc, NULL);
    zassert_equal(rc, 0, "mult:initc == ERR");

    /* Perform a valid 3x3 square matrix multiplication. */
    rc = zsl_mtx_mult(&ma, &mb, &mc);
    zassert_equal(rc, 0, "mult:exec == ERR");
    zassert_equal(mref.data[0], mc.data[0], "mult:valerr:0");
    zassert_equal(mref.data[1], mc.data[1], "mult:valerr:1");
    zassert_equal(mref.data[2], mc.data[2], "mult:valerr:2");
    zassert_equal(mref.data[3], mc.data[3], "mult:valerr:3");
    zassert_equal(mref.data[4], mc.data[4], "mult:valerr:4");
    zassert_equal(mref.data[5], mc.data[5], "mult:valerr:5");
    zassert_equal(mref.data[6], mc.data[6], "mult:valerr:6");
    zassert_equal(mref.data[7], mc.data[7], "mult:valerr:7");
    zassert_equal(mref.data[8], mc.data[8], "mult:valerr:8");
}

/**
 * @brief zsl_mtx_mult unit tests with rectangular matrices.
 *
 * This test verifies the zsl_mtx_mult function with rectangular matrices.
 */
void test_matrix_mult_rect(void)
{
    int rc = 0;
    ZSL_MATRIX_DEF(mc, 4, 3);
    ZSL_MATRIX_DEF(merr, 5, 3);

    /* Input matrix a (4x2). */
    zsl_data_t data_a[8] = { 2.0, 3.0,
                             1.0, 4.0,
                             4.0, 3.0,
                             3.0, 4.0 };
    struct zsl_mtx ma = {
        .sz_rows = 4,
        .sz_cols = 2,
        .data = data_a
    };

    /* Input matrix b (2x3). */
    zsl_data_t data_b[9] = { 3.0, 1.0, 2.0,
                             2.0, 4.0, 2.0 };
    struct zsl_mtx mb = {
        .sz_rows = 2,
        .sz_cols = 3,
        .data = data_b
    };

    /* Output reference matrix (4x3). */
    zsl_data_t data_ref[12] = { 12.0, 14.0, 10.0,
                                11.0, 17.0, 10.0,
                                18.0, 16.0, 14.0,
                                17.0, 19.0, 14.0 };
    struct zsl_mtx mref = {
        .sz_rows = 3,
        .sz_cols = 3,
        .data = data_ref
    };

    /* Attempt an invalid 4x2 x 5x3 matrix multiplication. */
    rc = zsl_mtx_mult(&merr, &mb, &mc);
    zassert_equal(rc, -EINVAL, "multr:badshape != ERR");

    /* Initialise the output matrix. */
    rc = zsl_mtx_init(&mc, NULL);
    zassert_equal(rc, 0, "multr:initc == ERR");

    /* Perform a valid 3x3 square matrix multiplication. */
    rc = zsl_mtx_mult(&ma, &mb, &mc);
    zassert_equal(rc, 0, "multr:exec == ERR");
    zassert_equal(mref.data[0], mc.data[0], "multr:valerr:0");
    zassert_equal(mref.data[1], mc.data[1], "multr:valerr:1");
    zassert_equal(mref.data[2], mc.data[2], "multr:valerr:2");
    zassert_equal(mref.data[3], mc.data[3], "multr:valerr:3");
    zassert_equal(mref.data[4], mc.data[4], "multr:valerr:4");
    zassert_equal(mref.data[5], mc.data[5], "multr:valerr:5");
    zassert_equal(mref.data[6], mc.data[6], "multr:valerr:6");
    zassert_equal(mref.data[7], mc.data[7], "multr:valerr:7");
    zassert_equal(mref.data[8], mc.data[8], "multr:valerr:8");
    zassert_equal(mref.data[9], mc.data[9], "multr:valerr:9");
    zassert_equal(mref.data[10], mc.data[10], "multr:valerr:10");
    zassert_equal(mref.data[11], mc.data[11], "multr:valerr:11");
}

/**
 * @brief zsl_mtx_scalar_mult unit tests.
 *
 * This test verifies the zsl_mtx_scalar_mult function.
 */
void test_matrix_scalar_mult(void)
{
    int rc = 0;
    zsl_data_t s = 10.0;

    /* Input matrix. */
    zsl_data_t data[8] = { 2.0, 3.0,
                           1.0, 4.0,
                           4.0, 3.0,
                           3.0, 4.0 };
    struct zsl_mtx m = {
        .sz_rows = 4,
        .sz_cols = 2,
        .data = data
    };

    rc = zsl_mtx_scalar_mult(&m, s);
    zassert_equal(rc, 0, "scalmult:exec == ERR");
    zassert_true(val_is_equal(m.data[0], 20.0, 1E-5), "scalmult:valerr:0");
    zassert_true(val_is_equal(m.data[1], 30.0, 1E-5), "scalmult:valerr:1");
    zassert_true(val_is_equal(m.data[2], 10.0, 1E-5), "scalmult:valerr:2");
    zassert_true(val_is_equal(m.data[3], 40.0, 1E-5), "scalmult:valerr:3");
    zassert_true(val_is_equal(m.data[4], 40.0, 1E-5), "scalmult:valerr:4");
    zassert_true(val_is_equal(m.data[5], 30.0, 1E-5), "scalmult:valerr:5");
    zassert_true(val_is_equal(m.data[6], 30.0, 1E-5), "scalmult:valerr:6");
    zassert_true(val_is_equal(m.data[7], 40.0, 1E-5), "scalmult:valerr:7");
}

/**
 * @brief zsl_mtx_scalar_trans unit tests.
 *
 * This test verifies the zsl_mtx_trans function.
 */
void test_matrix_trans(void)
{
    int rc = 0;

    ZSL_MATRIX_DEF(mt, 2, 4);

    /* Input matrix. */
    zsl_data_t data[8] = { 2.0, 3.0,
                           1.0, 4.0,
                           4.0, 3.0,
                           3.0, 4.0 };
    struct zsl_mtx m = {
        .sz_rows = 4,
        .sz_cols = 2,
        .data = data
    };

    rc = zsl_mtx_init(&mt, NULL);
    zassert_equal(rc, 0, "trans:initmt == ERR");

    rc = zsl_mtx_trans(&m, &mt);
    zassert_equal(rc, 0, "trans:exec == ERR");
    zassert_equal(mt.sz_cols, m.sz_rows, "trans:shapecol:err");
    zassert_equal(mt.sz_rows, m.sz_cols, "trans:shaperow:err");
    zassert_true(val_is_equal(mt.data[0], 2.0, 1E-5), "trans:valerr:0");
    zassert_true(val_is_equal(mt.data[1], 1.0, 1E-5), "trans:valerr:1");
    zassert_true(val_is_equal(mt.data[2], 4.0, 1E-5), "trans:valerr:2");
    zassert_true(val_is_equal(mt.data[3], 3.0, 1E-5), "trans:valerr:3");
    zassert_true(val_is_equal(mt.data[4], 3.0, 1E-5), "trans:valerr:4");
    zassert_true(val_is_equal(mt.data[5], 4.0, 1E-5), "trans:valerr:5");
    zassert_true(val_is_equal(mt.data[6], 3.0, 1E-5), "trans:valerr:6");
    zassert_true(val_is_equal(mt.data[7], 4.0, 1E-5), "trans:valerr:7");
}
