/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ztest.h>
#include <zsl/zsl.h>
#include <zsl/complex.h>
#include "floatcheck.h"

/**
 * @brief zsl_com_add unit tests.
 *
 * This test verifies the zsl_com_add function.
 */
void test_complex_add(void)
{
    int rc;
    ZSL_COMPLEX_DEF(a, 1.0, 0.4);
    ZSL_COMPLEX_DEF(b, 1.0, 0.4);
    ZSL_COMPLEX_DEF(c, 0.0, 0.0);

    rc = zsl_com_add(&a, &b, &c);
    zassert_equal(rc, 0, NULL);
    zassert_true(val_is_equal(c.r, a.r + b.r, 1E-5), NULL);
    zassert_true(val_is_equal(c.i, a.i + b.i, 1E-5), NULL);
}
