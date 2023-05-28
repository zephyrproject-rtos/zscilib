/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/vectors.h>
#include <zsl/probability.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_prob_uniform_pdf)
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

ZTEST(zsl_tests, test_prob_uniform_mean)
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

ZTEST(zsl_tests, test_prob_uniform_variance)
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

ZTEST(zsl_tests, test_prob_uniform_cdf)
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

ZTEST(zsl_tests, test_prob_normal_pdf)
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

ZTEST(zsl_tests, test_prob_normal_cdf)
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

ZTEST(zsl_tests, test_prob_erf_inverse)
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

ZTEST(zsl_tests, test_prob_normal_cdf_inv)
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

ZTEST(zsl_tests, test_prob_factorial)
{
	int rc;
	int n;

	n = 0;
	rc = zsl_prob_factorial(&n);
	zassert_true(val_is_equal(rc, 1, 1E-6), NULL);

	n = 1;
	rc = zsl_prob_factorial(&n);
	zassert_true(val_is_equal(rc, 1, 1E-6), NULL);

	n = 8;
	rc = zsl_prob_factorial(&n);
	zassert_true(val_is_equal(rc, 40320, 1E-6), NULL);
}

ZTEST(zsl_tests, test_prob_binomial_coef)
{
	int rc;
	int n = 7, k, c;

	k = 6;
	rc = zsl_prob_binomial_coef(&n, &k, &c);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(c, 7, 1E-6), NULL);

	k = 4;
	rc = zsl_prob_binomial_coef(&n, &k, &c);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(c, 35, 1E-6), NULL);

	k = 0;
	rc = zsl_prob_binomial_coef(&n, &k, &c);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(c, 1, 1E-6), NULL);

	k = 1;
	rc = zsl_prob_binomial_coef(&n, &k, &c);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(c, 7, 1E-6), NULL);

	k = -3;
	rc = zsl_prob_binomial_coef(&n, &k, &c);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(c, 0, 1E-6), NULL);

	k = 9;
	rc = zsl_prob_binomial_coef(&n, &k, &c);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(c, 0, 1E-6), NULL);

	n = 0;
	k = 0;
	rc = zsl_prob_binomial_coef(&n, &k, &c);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(c, 1, 1E-6), NULL);

	/* n has to be positive. Negative n should return an error. */
	n = -1;
	rc = zsl_prob_binomial_coef(&n, &k, &c);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_prob_binomial_pdf)
{
	zsl_real_t rc;
	zsl_real_t p = 0.3;
	int n = 5, x;

	x = -3;
	rc = zsl_prob_binomial_pdf(&n, &p, &x);
	zassert_true(val_is_equal(rc, 0.0, 1E-6), NULL);

	x = 0;
	rc = zsl_prob_binomial_pdf(&n, &p, &x);
	zassert_true(val_is_equal(rc, 0.16807, 1E-6), NULL);

	x = 4;
	rc = zsl_prob_binomial_pdf(&n, &p, &x);
	zassert_true(val_is_equal(rc, 0.02835, 1E-6), NULL);

	x = 7;
	rc = zsl_prob_binomial_pdf(&n, &p, &x);
	zassert_true(val_is_equal(rc, 0.0, 1E-6), NULL);

	/* Errors. */
	p = -0.1;
	x = 4;
	rc = zsl_prob_binomial_pdf(&n, &p, &x);
	zassert_true(rc == -EINVAL, NULL);

	p = 3.1;
	rc = zsl_prob_binomial_pdf(&n, &p, &x);
	zassert_true(rc == -EINVAL, NULL);

	p = 0.3;
	n = -5;
	rc = zsl_prob_binomial_pdf(&n, &p, &x);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_prob_binomial_mean)
{
	zsl_real_t rc;
	zsl_real_t p = 0.3, m;
	int n = 5;

	rc = zsl_prob_binomial_mean(&n, &p, &m);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(m, 1.5, 1E-6), NULL);

	/* Errors. */
	p = -0.1;
	rc = zsl_prob_binomial_mean(&n, &p, &m);
	zassert_true(rc == -EINVAL, NULL);

	p = 3.1;
	rc = zsl_prob_binomial_mean(&n, &p, &m);
	zassert_true(rc == -EINVAL, NULL);

	n = -5;
	p = 0.3;
	rc = zsl_prob_binomial_mean(&n, &p, &m);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_prob_binomial_variance)
{
	zsl_real_t rc;
	zsl_real_t p = 0.3, v;
	int n = 5;

	rc = zsl_prob_binomial_var(&n, &p, &v);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(v, 1.05, 1E-6), NULL);

	/* Errors. */
	p = -0.1;
	rc = zsl_prob_binomial_var(&n, &p, &v);
	zassert_true(rc == -EINVAL, NULL);

	p = 3.1;
	rc = zsl_prob_binomial_var(&n, &p, &v);
	zassert_true(rc == -EINVAL, NULL);

	n = -5;
	p = 0.3;
	rc = zsl_prob_binomial_var(&n, &p, &v);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_prob_binomial_cdf)
{
	zsl_real_t rc;
	zsl_real_t p = 0.3;
	int n = 5, x;

	x = -3;
	rc = zsl_prob_binomial_cdf(&n, &p, &x);
	zassert_true(val_is_equal(rc, 0.0, 1E-6), NULL);

	x = 0;
	rc = zsl_prob_binomial_cdf(&n, &p, &x);
	zassert_true(val_is_equal(rc, 0.16807, 1E-6), NULL);

	x = 4;
	rc = zsl_prob_binomial_cdf(&n, &p, &x);
	zassert_true(val_is_equal(rc, 0.99757, 1E-6), NULL);

	x = 7;
	rc = zsl_prob_binomial_cdf(&n, &p, &x);
	zassert_true(val_is_equal(rc, 1.0, 1E-6), NULL);

	/* Errors. */
	p = -0.1;
	x = 4;
	rc = zsl_prob_binomial_cdf(&n, &p, &x);
	zassert_true(rc == -EINVAL, NULL);

	p = 3.1;
	rc = zsl_prob_binomial_cdf(&n, &p, &x);
	zassert_true(rc == -EINVAL, NULL);

	p = 0.3;
	n = -5;
	rc = zsl_prob_binomial_cdf(&n, &p, &x);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_prob_entropy)
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

	/* Compute the entropy of vb. It should return an error. */
	rc = zsl_prob_entropy(&vb, &h);
	zassert_true(rc == -EINVAL, NULL);

	/* Compute the entropy of va, which now has a negative probability. It
	 * should return an error. */
	va.data[2] *= -1.0;
	rc = zsl_prob_entropy(&va, &h);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_prob_bayes)
{
	zsl_real_t rc;
	zsl_real_t pa = 0.15, pb = 0.2, pba = 0.4, pab;

	/* Calculate the probability of A given B (pab). */
	rc = zsl_prob_bayes(&pa, &pb, &pba, &pab);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(pab, 0.3, 1E-6), NULL);

	/* Case for negative and bigger than one probabilities. */
	pa *= -1.;
	rc = zsl_prob_bayes(&pa, &pb, &pba, &pab);
	zassert_true(rc == -EINVAL, NULL);

	pb *= -1.;
	pa *= -1.;
	rc = zsl_prob_bayes(&pa, &pb, &pba, &pab);
	zassert_true(rc == -EINVAL, NULL);

	pba *= -1.;
	pb *= -1.;
	rc = zsl_prob_bayes(&pa, &pb, &pba, &pab);
	zassert_true(rc == -EINVAL, NULL);

	pa += 2.;
	pba *= -1.;
	rc = zsl_prob_bayes(&pa, &pb, &pba, &pab);
	zassert_true(rc == -EINVAL, NULL);

	pb += 2.;
	pa -= 2.;
	rc = zsl_prob_bayes(&pa, &pb, &pba, &pab);
	zassert_true(rc == -EINVAL, NULL);

	pba += 2.;
	pb -= 2.;
	rc = zsl_prob_bayes(&pa, &pb, &pba, &pab);
	zassert_true(rc == -EINVAL, NULL);

	pba -= 2.;
	pb = 0.0;
	rc = zsl_prob_bayes(&pa, &pb, &pba, &pab);
	zassert_true(rc == -EINVAL, NULL);

	/* Case for P(A) * P(B|A) > P(B). */
	pa = 0.9;
	pb = 0.1;
	pba = 0.8;
	rc = zsl_prob_bayes(&pa, &pb, &pba, &pab);
	zassert_true(rc == -EINVAL, NULL);
}
