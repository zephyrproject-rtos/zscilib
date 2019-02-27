/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ztest.h>
#include <zsl/zsl.h>
#include <zsl/vectors.h>
#include "floatcheck.h"

/**
 * @brief zsl_vec_from_arr unit tests.
 *
 * This test verifies the zsl_vec_from_arr function.
 */
void test_vector_from_arr(void)
{
    int rc;
    zsl_real_t x;

    /* Destination vector. */
    ZSL_VECTOR_STATIC_INIT(v, 3);

    /* Test array. */
    zsl_real_t a[3] = { 0.0, 1.0, 2.0 };

    /* Assign the array a to vector v. */
    rc = zsl_vec_from_arr(&v, a);
    zassert_true(rc == 0, "from_arr:ret != 0");

    /* Test values. */
    for (size_t i = 0; i < v.sz; i++) {
        x = v.data[i];
        zassert_true(x == a[i], "from_arr:unexpected val in .data");
    }
}

void test_vector_get_subset(void)
{
    int rc;

    ZSL_VECTOR_STATIC_INIT(vsrc, 10);
    ZSL_VECTOR_STATIC_INIT(vcp, 6);

    zsl_real_t a[10] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };

    /* Populate the source vector. */
    rc = zsl_vec_from_arr(&vsrc, a);
    zassert_true(rc == 0, "");

    /* Get a valid subset (no range issues). */
    rc = zsl_vec_get_subset(&vsrc, 3, 6, &vcp);
    zassert_true(rc == 0, "");
    zassert_equal(vcp.sz, 6, "");
    zassert_equal(vcp.data[0], a[3], "");
    zassert_equal(vcp.data[1], a[4], "");
    zassert_equal(vcp.data[2], a[5], "");
    zassert_equal(vcp.data[3], a[6], "");
    zassert_equal(vcp.data[4], a[7], "");
    zassert_equal(vcp.data[5], a[8], "");

    /* vcp->sz resize test (sz should truncate from 6 to 3). */
    zsl_vec_init(&vcp);
    rc = zsl_vec_get_subset(&vsrc, 3, 3, &vcp);
    zassert_true(rc == 0, "");
    zassert_equal(vcp.sz, 3, "");
    zassert_equal(vcp.data[0], a[3], "");
    zassert_equal(vcp.data[1], a[4], "");
    zassert_equal(vcp.data[2], a[5], "");

    /* Check for len overrun truncate (sz should truncate to 1). */
    vcp.sz = 6;
    zsl_vec_init(&vcp);
    rc = zsl_vec_get_subset(&vsrc, 9, 3, &vcp);
    zassert_true(rc == 0, "");
    zassert_equal(vcp.sz, 1, "");
    zassert_equal(vcp.data[0], a[9], "");

    /* Check to out of range error. */
    vcp.sz = 6;
    zsl_vec_init(&vcp);
    rc = zsl_vec_get_subset(&vsrc, 10, 3, &vcp);
    zassert_true(rc == -EINVAL, "");

    /* Check buffer too small error (needs 3 elements, has 2). */
    vcp.sz = 2;
    zsl_vec_init(&vcp);
    rc = zsl_vec_get_subset(&vsrc, 3, 3, &vcp);
    zassert_true(rc == -EINVAL, "");
}

void test_vector_add(void)
{
    int rc;

    ZSL_VECTOR_STATIC_INIT(va, 3);
    ZSL_VECTOR_STATIC_INIT(vb, 3);
    ZSL_VECTOR_STATIC_INIT(vc, 3);

    zsl_real_t a[3] = { 0.0, 1.0, 2.0 };
    zsl_real_t b[3] = {-1.0, 0.5, 0.1 };
    zsl_real_t c[3] = {-1.0, 1.5, 2.1 };

    /* Assign arrays to vectors. */
    rc = zsl_vec_from_arr(&va, a);
    zassert_true(rc == 0, "");
    rc = zsl_vec_from_arr(&vb, b);
    zassert_true(rc == 0, "");

    /* Perform the add operation. */
    rc = zsl_vec_add(&va, &vb, &vc);
    zassert_true(rc == 0, "");
    zassert_equal(vc.data[0], c[0], "");
    zassert_equal(vc.data[1], c[1], "");
    zassert_equal(vc.data[2], c[2], "");
}

void test_vector_sub(void)
{
    int rc;

    ZSL_VECTOR_STATIC_INIT(va, 3);
    ZSL_VECTOR_STATIC_INIT(vb, 3);
    ZSL_VECTOR_STATIC_INIT(vc, 3);

    zsl_real_t a[3] = { 0.0, 1.0, 2.0 };
    zsl_real_t b[3] = {-1.0, 0.5, 0.1 };
    zsl_real_t c[3] = { 1.0, 0.5, 1.9 };

    /* Assign arrays to vectors. */
    rc = zsl_vec_from_arr(&va, a);
    zassert_true(rc == 0, "");
    rc = zsl_vec_from_arr(&vb, b);
    zassert_true(rc == 0, "");

    /* Perform the add operation. */
    rc = zsl_vec_sub(&va, &vb, &vc);
    zassert_true(rc == 0, "");
    zassert_equal(vc.data[0], c[0], "");
    zassert_equal(vc.data[1], c[1], "");
    zassert_equal(vc.data[2], c[2], "");
}

void test_vector_neg(void)
{
    int rc;

    ZSL_VECTOR_STATIC_INIT(v, 3);

    zsl_real_t a[3] = {  0.0,  1.0,  2.0 };
    zsl_real_t b[3] = { -0.0, -1.0, -2.0 };

    /* Assign array to vector. */
    rc = zsl_vec_from_arr(&v, a);
    zassert_true(rc == 0, "");

    /* Perform the add operation. */
    rc = zsl_vec_neg(&v);
    zassert_true(rc == 0, "");
    zassert_equal(v.data[0], b[0], "");
    zassert_equal(v.data[1], b[1], "");
    zassert_equal(v.data[2], b[2], "");
}

void test_vector_sum(void)
{
    int rc;

    ZSL_VECTOR_STATIC_INIT(va, 3);
    ZSL_VECTOR_STATIC_INIT(vb, 3);
    ZSL_VECTOR_STATIC_INIT(vc, 3);
    ZSL_VECTOR_STATIC_INIT(vsum, 3);

    struct zsl_vec *vlist[3] = { &va, &vb, &vc };

    /* Assign values to vectors. */
    va.data[0] = 1.0;
    va.data[1] = 2.0;
    va.data[2] = 3.0;
    vb.data[0] = 3.0;
    vb.data[1] = 2.0;
    vb.data[2] = 1.0;
    vc.data[0] = -1.0;
    vc.data[1] = -2.0;
    vc.data[2] = -3.0;

    /* Perform sum operation on va..vc. */
    rc = zsl_vec_sum(vlist, 3, &vsum);
    zassert_true(rc == 0, "");

    /* Check output. */
    for (size_t i = 0; i <3; i++) {
        zassert_equal(vsum.data[i], va.data[i]+vb.data[i]+vc.data[i], "");
    }
}

void test_vector_magn(void)
{

}

void test_vector_scalar_add(void)
{

}

void test_vector_scalar_mult(void)
{

}

void test_vector_scalar_div(void)
{

}

void test_vector_dist(void)
{

}

void test_vector_dot(void)
{

}

void test_vector_norm(void)
{

}

void test_vector_to_unit(void)
{

}

void test_vector_cross(void)
{

}

void test_vector_sum_of_sqrs(void)
{

}

void test_vector_mean(void)
{

}

void test_vector_rev(void)
{
    int rc;
    ZSL_VECTOR_STATIC_INIT(v, 7);

    zsl_real_t a[7] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };

    /* Assign array to vector. */
    rc = zsl_vec_from_arr(&v, a);
    zassert_true(rc == 0, "rev:fromarr != 0");

    rc = zsl_vec_rev(&v);
    zassert_true(rc == 0, "rev:ret != 0");
    zassert_equal(v.data[0], 6.0, "rev:val[0] != 6.0");
    zassert_equal(v.data[1], 5.0, "rev:val[0] != 5.0");
    zassert_equal(v.data[2], 4.0, "rev:val[0] != 4.0");
    zassert_equal(v.data[3], 3.0, "rev:val[0] != 3.0");
    zassert_equal(v.data[4], 2.0, "rev:val[0] != 2.0");
    zassert_equal(v.data[5], 1.0, "rev:val[0] != 1.0");
    zassert_equal(v.data[6], 0.0, "rev:val[0] != 0.0");
}

/**
 * @brief zsl_vec_is_equal unit tests.
 *
 * This test verifies the zsl_vec_is_equal function.
 */
void test_vector_is_equal(void)
{
    int rc;
    bool eq;

    ZSL_VECTOR_STATIC_INIT(v, 3);
    ZSL_VECTOR_STATIC_INIT(w, 3);
    ZSL_VECTOR_STATIC_INIT(x, 2);

    /* Test arrays. */
    zsl_real_t a[3] = { 0.0, 1.0, 2.0 };
    zsl_real_t b[3] = { 0.0, 1.0, 2.0 };

    /* Check for unequal size. */
    eq = zsl_vec_is_equal(&v, &x);
    zassert_false(eq, "is_equal:size mismatch");

    /* Check equality for zero value vectors. */
    eq = zsl_vec_is_equal(&v, &w);
    zassert_true(eq, "is_equal:zerovals !equal");

    /* Assign the array a to vector v. */
    rc = zsl_vec_from_arr(&v, a);
    zassert_true(rc == 0, "is_equal:arr:retv != 0");
    rc = zsl_vec_from_arr(&w, b);
    zassert_true(rc == 0, "is_equal:arr:retw != 0");

    /* Check equality for array value vectors. */
    eq = zsl_vec_is_equal(&v, &w);
    zassert_true(eq, "is_equal:vals !equal");

    /* Check for inequality. */
    v.data[0] = 1.0;
    eq = zsl_vec_is_equal(&v, &w);
    zassert_false(eq, "is_equal:vals equal");
}

void test_vector_is_notneg(void)
{

}
