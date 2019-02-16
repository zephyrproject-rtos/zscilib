/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ztest.h>
#include <zsl/zsl.h>
#include <zsl/vectors.h>

/**
 * @brief zsl_vec_from_arr unit tests.
 *
 * This test verifies the zsl_vec_from_arr function.
 */
void test_vector_from_arr(void)
{
    int rc;
    zsl_data_t x;

    /* Destination vector. */
    ZSL_VECTOR_DEF(v, 3);

    /* Test array. */
    zsl_data_t a[3] = { 0.0, 1.0, 2.0 };

    /* Initialise the vector. */
    rc = zsl_vec_init(&v);
    zassert_true(rc == 0, "from_arr:vec_init != 0");

    /* Assign the array a to vector v. */
    rc = zsl_vec_from_arr(&v, a);
    zassert_true(rc == 0, "from_arr:ret != 0");

    /* Test values. */
    for (size_t i = 0; i < v.sz; i++) {
        x = v.data[i];
        zassert_true(x == a[i], "from_arr:unexpected val in .data");
    }
}
