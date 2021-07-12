/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ztest.h>
#include <zsl/zsl.h>
#include <zsl/statistics.h>
#include "floatcheck.h"

void test_sta_mean(void)
{
	int rc;
	zsl_real_t m;

	ZSL_VECTOR_DEF(v, 10);

	zsl_real_t a[10] = { -3.0, 1.0, 2.0, 8.5, -3.5, 4.0, 7.0, -2.0, 0.0, 6.0 };

	/* Assign arrays to the vectors. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);

	/* Compute the mean value. */
	rc = zsl_sta_mean(&v, &m);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(m, 2.0, 1E-6), NULL);
}

void test_sta_demean(void)
{
	int rc;
	zsl_real_t m;

	ZSL_VECTOR_DEF(v, 10);
	ZSL_VECTOR_DEF(w, 10);
	ZSL_VECTOR_DEF(u, 10);

	zsl_real_t a[10] = { -3.0, 1.0, 2.0, 8.5, -3.5, 4.0, 7.0, -2.0, 0.0, 6.0 };
	zsl_real_t b[10] = { -5.0, -1.0, 0.0, 6.5, -5.5, 2.0, 5.0, -4.0, -2.0, 4.0 };

	/* Assign arrays to the vectors. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&w, b);
	zassert_true(rc == 0, NULL);

	/* Calculate the de-meaned vector u. */
	rc = zsl_sta_demean(&v, &u);
	zassert_true(rc == 0, NULL);

	/* Check the output. */
	for (size_t g = 0; g < v.sz; g++) {
		zassert_true(val_is_equal(u.data[g], w.data[g], 1E-6), NULL);
	}

	/* Compute the mean of u and check if it is zero. */
	rc = zsl_sta_mean(&u, &m);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(m, 0.0, 1E-6), NULL);

}

void test_sta_percentile(void)
{
	int rc;
	zsl_real_t val;

	ZSL_VECTOR_DEF(v, 10);

	zsl_real_t a[10] = { -3.0, 1.0, 2.0, 8.5, -3.5, 4.0, 7.0, -2.0, 0.0, 6.0 };

	/* Assign array to the vector. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);

	/* Calculate the 20 percentile. */
	rc = zsl_sta_percentile(&v, 20, &val);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(val, -2.5, 1E-6), NULL);

	/* Calculate the 73 percentile. */
	rc = zsl_sta_percentile(&v, 73, &val);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(val, 6.0, 1E-6), NULL);

	/* Calculate the 44 percentile. */
	rc = zsl_sta_percentile(&v, 44, &val);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(val, 1.0, 1E-6), NULL);
}

void test_sta_median(void)
{
	int rc;
	zsl_real_t m;

	ZSL_VECTOR_DEF(v, 10);

	zsl_real_t a[10] = { -3.0, 1.0, 2.0, 8.5, -3.5, 4.0, 7.0, -2.0, 0.0, 6.0 };

	/* Assign arrays to the vector. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);

	/* Calculate the median. */
	rc = zsl_sta_median(&v, &m);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(m, 1.5, 1E-6), NULL);
}

void test_sta_quartiles(void)
{
	int rc;
	zsl_real_t q1, q2, q3;

	ZSL_VECTOR_DEF(v, 10);

	zsl_real_t a[10] = { -3.0, 1.0, 2.0, 8.5, -3.5, 4.0, 7.0, -2.0, 0.0, 6.0 };

	/* Assign arrays to the vector. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);

	/* Calculate the median. */
	rc = zsl_sta_quart(&v, &q1, &q2, &q3);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q1, -2.0, 1E-6), NULL);
	zassert_true(val_is_equal(q2, 1.5, 1E-6), NULL);
	zassert_true(val_is_equal(q3, 6.0, 1E-6), NULL);
}

void test_sta_quart_range(void)
{
	int rc;
	zsl_real_t r;

	ZSL_VECTOR_DEF(v, 10);

	zsl_real_t a[10] = { -3.0, 1.0, 2.0, 8.5, -3.5, 4.0, 7.0, -2.0, 0.0, 6.0 };

	/* Assign arrays to the vector. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);

	/* Calculate the median. */
	rc = zsl_sta_quart_range(&v, &r);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(r, 8.0, 1E-6), NULL);
}

void test_sta_mode(void)
{
	int rc;

	ZSL_VECTOR_DEF(va, 10);
	ZSL_VECTOR_DEF(vb, 10);
	ZSL_VECTOR_DEF(ma, 10);
	ZSL_VECTOR_DEF(mb, 10);

	/* Single mode vector. */
	zsl_real_t a[10] = { -2.0, 1.0, 2.0, 1.5, 1.5, -2.0, 1.0, -2.0, 1.0, -2.0 };

	/* Multiple modes vector. */
	zsl_real_t b[10] = { -2.0, 1.0, 4.0, 1.5, 1.5, -2.0, 1.0, -2.0, 1.0, 1.5 };

	/* Assign arrays to the vector. */
	rc = zsl_vec_from_arr(&va, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&vb, b);
	zassert_true(rc == 0, NULL);

	/* Calculate the mode(s). */
	rc = zsl_sta_mode(&va, &ma);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(ma.sz, 1.0, 1E-6), NULL);
	zassert_true(val_is_equal(ma.data[0], -2.0, 1E-6), NULL);

	rc = zsl_sta_mode(&vb, &mb);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(mb.sz, 3.0, 1E-6), NULL);
	zassert_true(val_is_equal(mb.data[0], -2.0, 1E-6), NULL);
	zassert_true(val_is_equal(mb.data[1], 1.0, 1E-6), NULL);
	zassert_true(val_is_equal(mb.data[2], 1.5, 1E-6), NULL);
}

void test_sta_data_range(void)
{
	int rc;

	ZSL_VECTOR_DEF(v, 10);
	zsl_real_t r;

	zsl_real_t a[10] = { -2.0, 1.0, 7.0, 1.5, 1.5, -2.0, 1.0, -5.0, 1.0, -2.0 };

	/* Assign arrays to the vector. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);

	/* Calculate the data range. */
	rc = zsl_sta_data_range(&v, &r);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(r, 12.0, 1E-6), NULL);
}

void test_sta_variance(void)
{
	int rc;

	ZSL_VECTOR_DEF(v, 10);
	zsl_real_t var;

	zsl_real_t a[10] = { -2.0, 1.0, 3.0, 1.5, 1.5, -2.0, 1.0, -5.0, 1.0, -2.0 };

	/* Assign arrays to the vector. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);

	/* Calculate the variance. */
	rc = zsl_sta_var(&v, &var);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(var, 5.9, 1E-6), NULL);
}

void test_sta_standard_deviation(void)
{
	int rc;

	ZSL_VECTOR_DEF(v, 10);
	zsl_real_t var;

	zsl_real_t a[10] = { -2.0, 1.0, 3.0, 1.5, 1.5, -2.0, 1.0, -5.0, 1.0, -2.0 };

	/* Assign arrays to the vector. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);

	/* Calculate the standard deviation. */
	rc = zsl_sta_sta_dev(&v, &var);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(var, ZSL_SQRT(5.9), 1E-6), NULL);
}

void test_sta_covariance(void)
{
	int rc;

	ZSL_VECTOR_DEF(va, 10);
	ZSL_VECTOR_DEF(vb, 10);
	ZSL_VECTOR_DEF(vc, 5);
	zsl_real_t co;

	zsl_real_t a[10] = { -1.0, -1.0, 0.0, 1.0, 1.0, -1.0, 1.0, -1.0, 0.0, 1.0 };
	zsl_real_t b[10] = { -4.0, -7.0, 3.0, 2.0, 8.0, -9.0, 9.0, -1.0, 0.0, 5.0 };
	zsl_real_t c[5] = { -1.0, 0.0, 5.0, 3.5, -4.5 };

	/* Assign arrays to the vector. */
	rc = zsl_vec_from_arr(&va, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&vb, b);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&vc, c);
	zassert_true(rc == 0, NULL);

	/* Calculate the covariance. */
	rc = zsl_sta_covar(&va, &vb, &co);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(co, 5.0, 1E-6), NULL);

	/* Calculate the covariance of 'va' and 'vc'. An error is expected due
	 * to the difference of the vectors in dimension. */
	rc = zsl_sta_covar(&va, &vc, &co);
	zassert_true(rc == -EINVAL, NULL);
}

void test_sta_covariance_matrix(void)
{
	int rc;

	ZSL_MATRIX_DEF(ma, 4, 3);
	ZSL_MATRIX_DEF(mb, 3, 3);
	ZSL_MATRIX_DEF(mc, 2, 3);
	ZSL_MATRIX_DEF(md, 2, 2);

	zsl_real_t a[12] = { -1.0, -6.5, 1.2,
			     7.0, 5.5, 0.0,
			     -0.5, 4.0, 6.5,
			     -1.0, 4.0, -8.5 };

	/* Assign array to the matrix. */
	rc = zsl_mtx_from_arr(&ma, a);
	zassert_true(rc == 0, NULL);

	/* Calculate the covariance matrix of 'ma' into 'mb'. */
	rc = zsl_sta_covar_mtx(&ma, &mb);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(mb.data[0], 15.395833, 1E-6), NULL);
	zassert_true(val_is_equal(mb.data[1], 10.375000, 1E-6), NULL);
	zassert_true(val_is_equal(mb.data[2], 1.650000, 1E-6), NULL);
	zassert_true(val_is_equal(mb.data[3], 10.375000, 1E-6), NULL);
	zassert_true(val_is_equal(mb.data[4], 30.750000, 1E-6), NULL);
	zassert_true(val_is_equal(mb.data[5], -4.800000, 1E-6), NULL);
	zassert_true(val_is_equal(mb.data[6], 1.650000, 1E-6), NULL);
	zassert_true(val_is_equal(mb.data[7], -4.800000, 1E-6), NULL);
	zassert_true(val_is_equal(mb.data[8], 38.593333, 1E-6), NULL);

	/* Calculate the covariance matrix of 'ma' into 'mc' and 'md' . An error
	 * is expected due to the wrong dimensions of the output matrix. */
	rc = zsl_sta_covar_mtx(&ma, &mc);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_sta_covar_mtx(&ma, &md);
	zassert_true(rc == -EINVAL, NULL);
}

void test_sta_linear_regression(void)
{
	int rc;

	ZSL_VECTOR_DEF(va, 15);
	ZSL_VECTOR_DEF(vb, 15);
	ZSL_VECTOR_DEF(vc, 5);
	struct zsl_sta_linreg coef;

	zsl_real_t a[15] = { 1.47, 1.50, 1.52, 1.55, 1.57,
			     1.60, 1.63, 1.65, 1.68, 1.70,
			     1.73, 1.75, 1.78, 1.80, 1.83 };
	zsl_real_t b[15] = { 52.21, 53.12, 54.48, 55.84, 57.20,
			     58.57, 59.93, 61.29, 63.11, 64.47,
			     66.28, 68.10, 69.92, 72.19, 74.46 };
	zsl_real_t c[5] = { -1.0, 0.0, 5.0, 3.5, -4.5 };

	/* Assign arrays to the vector. */
	rc = zsl_vec_from_arr(&va, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&vb, b);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&vc, c);
	zassert_true(rc == 0, NULL);


	/* Calculate the coefficiens of the linear regression and the
	 * correlation coefficient. */
	rc = zsl_sta_linear_reg(&va, &vb, &coef);
	zassert_true(rc == 0, NULL);
#ifdef CONFIG_ZSL_SINGLE_PRECISION
	zassert_true(val_is_equal(coef.slope, 61.273108, 1E-2), NULL);
	zassert_true(val_is_equal(coef.intercept, -39.063480, 1E-2), NULL);
	zassert_true(val_is_equal(coef.correlation, 0.994592, 1E-4), NULL);
#else
	zassert_true(val_is_equal(coef.slope, 61.2721865421074341, 1E-6), NULL);
	zassert_true(val_is_equal(coef.intercept, -39.061955918838656, 1E-6), NULL);
	zassert_true(val_is_equal(coef.correlation, 0.994583793576875, 1E-6), NULL);
#endif

	/* Calculate the linear regression of 'va' and 'vc'. An error is expected
	 * due to the difference of the vectors in dimension. */
	rc = zsl_sta_linear_reg(&va, &vc, &coef);
	zassert_true(rc == -EINVAL, NULL);
}

void test_sta_absolute_error(void)
{
	int rc;
	zsl_real_t val = 9.9985, exp_val = 10.0, err;

	/* Calculate the absolute error. */
	rc = zsl_sta_abs_err(&val, &exp_val, &err);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(err, 0.0015, 1E-6), NULL);
}

void test_sta_relative_error(void)
{
	int rc;
	zsl_real_t val = 9.9985, exp_val = 10.0, err;

	/* Calculate the relative error. */
	rc = zsl_sta_rel_err(&val, &exp_val, &err);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(err, 0.015, 1E-5), NULL);
}
