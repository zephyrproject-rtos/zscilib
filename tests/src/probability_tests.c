/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ztest.h>
#include <zsl/zsl.h>
#include <zsl/vectors.h>
#include <zsl/probability.h>
#include "floatcheck.h"

void test_prob_uniform_pdf(void)
{
	zsl_real_t rc;
	zsl_real_t a = 3.0, b = 5.0, x;

	x = 0.0;
	rc = zsl_prob_uni_pdf(&a, &b, &x);
	zassert_true(val_is_equal(rc, 0.0, 1E-6), NULL);

	x = 4.0;
	rc = zsl_prob_uni_pdf(&a, &b, &x);
	zassert_true(val_is_equal(rc, 0.5, 1E-6), NULL);

	rc = zsl_prob_uni_pdf(&b, &a, &x);
	zassert_true(rc == -EINVAL, NULL);
}

void test_prob_uniform_mean(void)
{
	int rc;
	zsl_real_t a = 3.0, b = 5.0, m;

	/* Calculate the mean. */
	rc = zsl_prob_uni_mean(&a, &b, &m);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(m, 4.0, 1E-6), NULL);

	rc = zsl_prob_uni_mean(&b, &a, &m);
	zassert_true(rc == -EINVAL, NULL);
}

void test_prob_uniform_variance(void)
{
	int rc;
	zsl_real_t a = 3.0, b = 6.0, v;

	/* Calculate the variance. */
	rc = zsl_prob_uni_var(&a, &b, &v);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(v, 0.75, 1E-6), NULL);

	rc = zsl_prob_uni_var(&b, &a, &v);
	zassert_true(rc == -EINVAL, NULL);
}

void test_prob_uniform_cdf(void)
{
	zsl_real_t rc;
	zsl_real_t a = 3.0, b = 5.0, x;

	x = 0.0;
	rc = zsl_prob_uni_cdf(&a, &b, &x);
	zassert_true(val_is_equal(rc, 0.0, 1E-6), NULL);

	x = 3.5;
	rc = zsl_prob_uni_cdf(&a, &b, &x);
	zassert_true(val_is_equal(rc, 0.25, 1E-6), NULL);

	x = 4.0;
	rc = zsl_prob_uni_cdf(&a, &b, &x);
	zassert_true(val_is_equal(rc, 0.5, 1E-6), NULL);

	x = 5.0;
	rc = zsl_prob_uni_cdf(&a, &b, &x);
	zassert_true(val_is_equal(rc, 1, 1E-6), NULL);

	x = 8.0;
	rc = zsl_prob_uni_cdf(&a, &b, &x);
	zassert_true(val_is_equal(rc, 1, 1E-6), NULL);

	rc = zsl_prob_uni_cdf(&b, &a, &x);
	zassert_true(rc == -EINVAL, NULL);
}

void test_prob_normal_pdf(void)
{
	zsl_real_t rc;
	zsl_real_t m = 0.0, s = 1.0, x;

	x = 0.0;
	rc = zsl_prob_normal_pdf(&m, &s, &x);
	zassert_true(val_is_equal(rc, 0.3989422804, 1E-6), NULL);

	x = 2.0;
	rc = zsl_prob_normal_pdf(&m, &s, &x);
	zassert_true(val_is_equal(rc, 0.05399096651, 1E-6), NULL);

	x = -2.0;
	rc = zsl_prob_normal_pdf(&m, &s, &x);
	zassert_true(val_is_equal(rc, 0.05399096651, 1E-6), NULL);
}

void test_prob_normal_cdf(void)
{
	zsl_real_t rc;
	zsl_real_t m = 0.0, s = 1.0, x;

	x = 0.0;
	rc = zsl_prob_normal_cdf(&m, &s, &x);
	zassert_true(val_is_equal(rc, 0.5, 1E-6), NULL);

	x = 2.0;
	rc = zsl_prob_normal_cdf(&m, &s, &x);
	zassert_true(val_is_equal(rc, 0.9772498680518208, 1E-6), NULL);

	x = -2.0;
	rc = zsl_prob_normal_cdf(&m, &s, &x);
	zassert_true(val_is_equal(rc, 1. - 0.9772498680518208, 1E-6), NULL);
}

void test_prob_erf_inverse(void)
{
	zsl_real_t rc;
	zsl_real_t x;

	x = 0.0;
	rc = zsl_prob_erf_inv(&x);
	zassert_true(val_is_equal(rc, 0.0, 1E-6), NULL);

	x = 0.75;
	rc = zsl_prob_erf_inv(&x);
	zassert_true(val_is_equal(rc, 0.81341985, 1E-6), NULL);

	x = -0.25;
	rc = zsl_prob_erf_inv(&x);
	zassert_true(val_is_equal(rc, -0.22531206, 1E-6), NULL);

	x = 1.0;
	rc = zsl_prob_erf_inv(&x);
	zassert_true(rc == -EINVAL, NULL);

	x = -1.5;
	rc = zsl_prob_erf_inv(&x);
	zassert_true(rc == -EINVAL, NULL);
}

void test_prob_normal_cdf_inv(void)
{
	zsl_real_t rc;
	zsl_real_t m = 0.0, s = 1.0, x;

	x = 0.5;
	rc = zsl_prob_normal_cdf_inv(&m, &s, &x);
	zassert_true(val_is_equal(rc, 0.0, 1E-6), NULL);

	x = 0.9772498680518208;
	rc = zsl_prob_normal_cdf_inv(&m, &s, &x);
	zassert_true(val_is_equal(rc, 2.0, 1E-6), NULL);

	x = 1. - 0.9772498680518208;
	rc = zsl_prob_normal_cdf_inv(&m, &s, &x);
	zassert_true(val_is_equal(rc, -2.0, 1E-6), NULL);

	x = 0.0;
	rc = zsl_prob_normal_cdf_inv(&m, &s, &x);
	zassert_true(rc == -EINVAL, NULL);

	x = 2.0;
	rc = zsl_prob_normal_cdf_inv(&m, &s, &x);
	zassert_true(rc == -EINVAL, NULL);
}

void test_prob_entropy(void)
{
	zsl_real_t rc;
	zsl_real_t h;

	ZSL_VECTOR_DEF(va, 6);
	ZSL_VECTOR_DEF(vb, 6);

	zsl_real_t a[6] = { 0.25, 0.1, 0.15, 0.05, 0.4, 0.05 };
	zsl_real_t b[6] = { 0.75, 0.1, 0.85, 0.05, 0.4, 0.15 };

	/* Assign arrays to vectors. */
	rc = zsl_vec_from_arr(&va, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&vb, b);
	zassert_true(rc == 0, NULL);

	/* Compute the entropy of va. */
	rc = zsl_prob_entropy(&va, &h);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(h, 2.2037016960573483, 1E-6), NULL);

	/* Compute the entropy of vb. It should return an error */
	rc = zsl_prob_entropy(&vb, &h);
	zassert_true(rc == -EINVAL, NULL);	
}