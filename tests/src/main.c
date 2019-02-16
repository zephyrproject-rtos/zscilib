/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ztest.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>

/**
 * @brief zsl_mtx_init unit tests.
 *
 * This test verifies the zsl_mtx_init function.
 */
static void test_matrix_init(void)
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
    zassert_equal(x, 1.0, "init:diag:mtx[0][0] != 1.0");

    rc = zsl_mtx_get(&m, 1, 1, &x);
    zassert_equal(rc, 0, "init:diag:mtx[1][1] == ERR");
    zassert_equal(x, 1.0, "init::diag:mtx[1][1] != 1.0");

    rc = zsl_mtx_get(&m, 2, 2, &x);
    zassert_equal(rc, 0, "init:diag:mtx[2][2] == ERR");
    zassert_equal(x, 1.0, "init::diag:mtx[2][2] != 1.0");

    rc = zsl_mtx_get(&m, 0, 1, &x);
    zassert_equal(x, 0.0, "init::diag:mtx[0][1] != 0.0");
    zassert_equal(rc, 0, "init:diag:mtx[0][1] == ERR");
}

/**
 * @brief zsl_mtx_from_arr unit tests.
 *
 * This test verifies the zsl_mtx_from_arr function.
 */
static void test_matrix_from_arr(void)
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
    zassert_equal(rc, 0, "from_arr:init == ERR");
    zassert_true(rc == 0, "zsl_mtx_from_arr != 0");

    rc = zsl_mtx_get(&m, 0, 0, &x);
    zassert_equal(rc, 0, "from_arr:mtx[0][0] == ERR");
    zassert_equal(x, 1.0, "from_arr:mtx[0][0] != 1.0");

    rc = zsl_mtx_get(&m, 1, 1, &x);
    zassert_equal(rc, 0, "from_arr:mtx[1][1] == ERR");
    zassert_equal(x, 0.5, "from_arr:mtx[1][1] != 0.5");

    rc = zsl_mtx_get(&m, 2, 2, &x);
    zassert_equal(rc, 0, "from_arr:mtx[2][2] == ERR");
    zassert_equal(x, 0.1, "from_arr:mtx[2][2] != 0.1");

    rc = zsl_mtx_get(&m, 1, 0, &x);
    zassert_equal(rc, 0, "from_arr:mtx[1][0] == ERR");
    zassert_equal(x, 0.0, "from_arr:mtx[1][0] != 0.0");
}

/**
 * @brief zsl_mtx_get unit tests.
 *
 * This test verifies the zsl_mtx_get function.
 */
static void test_matrix_get(void)
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
    zassert_equal(x, 1.0, "get:mtx[0][0] != 1.0");

    rc = zsl_mtx_get(&m, 1, 0, &x);
    zassert_equal(rc, 0, "get:mtx[1][0] == ERR");
    zassert_equal(x, 0.0, "get:mtx[1][0] != 0.0");

    rc = zsl_mtx_get(&m, 2, 2, &x);
    zassert_equal(rc, 0, "get:mtx[2][2] == ERR");
    zassert_equal(x, 0.1, "get:mtx[2][2] != 0.1");

    /* Check for out of bounbds error. */
    zassert_true(zsl_mtx_get(&m, 3, 3, &x) == -EINVAL, "get:mtx[3][3] != ERR");
}

/**
 * @brief zsl_mtx_set unit tests.
 *
 * This test verifies the zsl_mtx_set function.
 */
static void test_matrix_set(void)
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
    zassert_true(x == 1.0, "set:get:mtx[0][0] != 1.0");

    rc = zsl_mtx_get(&m, 1, 1, &x);
    zassert_equal(rc, 0, "set:get:mtx[1][1] == ERR");
    zassert_true(x == 0.5, "set:get:mtx[1][1] != 0.5");

    rc = zsl_mtx_get(&m, 2, 2, &x);
    zassert_equal(rc, 0, "set:get:mtx[2][2] == ERR");
    zassert_true(x == 0.1, "set:get:mtx[2][2] != 0.1");

    /* Check for out of bounbds error. */
    zassert_true(zsl_mtx_set(&m, 3, 3, 0.0) == -EINVAL, "set:mtx[3][3] != ERR");
}

void test_main(void)
{
	ztest_test_suite(matrix_tests,
        ztest_unit_test(test_matrix_init),
        ztest_unit_test(test_matrix_from_arr),
        ztest_unit_test(test_matrix_get),
        ztest_unit_test(test_matrix_set)
	);

	ztest_run_test_suite(matrix_tests);
}
