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
    rc = zsl_mtx_get(&x, &m, 1, 0);
    zassert_equal(x, 0.0, "init:empty:mtx[1][0] != 0.0");
    rc = zsl_mtx_get(&x, &m, 2, 2);
    zassert_equal(x, 0.0, "init:empty:mtx[2][2] != 0.0");

    /* Reinitialise the matrix as an identity/diagonal matrix. */
    rc = zsl_mtx_init(&m, zsl_mtx_entry_fn_diagonal);
    rc = zsl_mtx_get(&x, &m, 0, 0);
    zassert_equal(x, 1.0, "init:diag:mtx[0][0] != 1.0");
    rc = zsl_mtx_get(&x, &m, 1, 1);
    zassert_equal(x, 1.0, "init::diag:mtx[1][1] != 1.0");
    rc = zsl_mtx_get(&x, &m, 2, 2);
    zassert_equal(x, 1.0, "init::diag:mtx[2][2] != 1.0");
    rc = zsl_mtx_get(&x, &m, 0, 1);
    zassert_equal(x, 0.0, "init::diag:mtx[0][1] != 0.0");
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
    zassert_true(rc == 0, "zsl_mtx_from_arr != 0");
    rc = zsl_mtx_get(&x, &m, 0, 0);
    zassert_equal(x, 1.0, "from_arr:mtx[0][0] != 1.0");
    rc = zsl_mtx_get(&x, &m, 1, 1);
    zassert_equal(x, 0.5, "from_arr:mtx[1][1] != 0.5");
    rc = zsl_mtx_get(&x, &m, 2, 2);
    zassert_equal(x, 0.1, "from_arr:mtx[2][2] != 0.1");
    rc = zsl_mtx_get(&x, &m, 1, 0);
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

    rc = zsl_mtx_get(&x, &m, 0, 0);
    zassert_equal(x, 1.0, "get:mtx[0][0] != 1.0");
    rc = zsl_mtx_get(&x, &m, 1, 0);
    zassert_equal(x, 0.0, "get:mtx[1][0] != 0.0");
    rc = zsl_mtx_get(&x, &m, 2, 2);
    zassert_equal(x, 0.1, "get:mtx[2][2] != 0.1");

    zassert_true(zsl_mtx_get(&x, &m, 3, 3) == -EINVAL, "get:mtx[3][3] != ERR");
}

void test_main(void)
{
	ztest_test_suite(matrix_tests,
        ztest_unit_test(test_matrix_init),
        ztest_unit_test(test_matrix_from_arr),
        ztest_unit_test(test_matrix_get)
	);

	ztest_run_test_suite(matrix_tests);
}
