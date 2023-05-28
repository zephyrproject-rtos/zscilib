/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/statistics.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_sta_mean)
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

ZTEST(zsl_tests, test_sta_trim_mean)
{
	int rc;
	zsl_real_t m;

	ZSL_VECTOR_DEF(v, 10);

	zsl_real_t a[10] = { -3.0, 1.0, 2.0, 8.5, -3.5, 4.0, 7.0, -2.0, 0.0, 6.0 };

	/* Assign arrays to the vectors. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);

	/* Compute the trimmed mean value for p = 10. */
	rc = zsl_sta_trim_mean(&v, 10, &m);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(m, 1.875000, 1E-6), NULL);

	/* Compute the trimmed mean value for p = 50. */
	rc = zsl_sta_trim_mean(&v, 50, &m);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(m, 0.0, 1E-6), NULL);

	/* Compute the trimmed mean value. An error is expected due to invalid
	 * size of the percentile (p > 50 or p < 0). */
	rc = zsl_sta_trim_mean(&v, -31, &m);
	zassert_true(rc == -EINVAL, NULL);

	/* Compute the trimmed mean value. An error is expected due to invalid
	 * size of the percentile (p > 50 or p < 0). */
	rc = zsl_sta_trim_mean(&v, 98, &m);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_sta_weighted_mean)
{
	int rc;
	zsl_real_t m;

	ZSL_VECTOR_DEF(v, 10);
	ZSL_VECTOR_DEF(w, 10);
	ZSL_VECTOR_DEF(w2, 10);
	ZSL_VECTOR_DEF(w3, 10);
	ZSL_VECTOR_DEF(w4, 15);

	zsl_real_t a[10] = { -3.0, 1.0, 2.0, 8.5, -3.5, 4.0, 7.0, -2.0, 0.0, 6.0 };
	zsl_real_t b[10] = { 0.1, 0.30, 0.15, 0.05, 0.05,
			     0.1, 0.03, 0.07, 0.09, 0.06 };
	zsl_real_t b2[10] = { 0.1, -0.30, 0.15, 0.05, 0.05,
			      0.1, -0.03, 0.07, 0.09, 0.06 };

	/* Assign arrays to the vectors. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&w, b);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&w2, b2);
	zassert_true(rc == 0, NULL);

	/* Initialize w3. */
	rc = zsl_vec_init(&w3);
	zassert_true(rc == 0, NULL);

	/* Compute the weighted mean value with weights from the vector 'w'. */
	rc = zsl_sta_weighted_mean(&v, &w, &m);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(m, 1.38, 1E-6), NULL);

	/* Compute the weighted mean value with weights from the vector 'w2'.
	 * An error is expected due to negative weights. */
	rc = zsl_sta_weighted_mean(&v, &w2, &m);
	zassert_true(rc == -EINVAL, NULL);

	/* Compute the weighted mean value with weights from the vector 'w3'.
	 * An error is expected due to all zero weights. */
	rc = zsl_sta_weighted_mean(&v, &w3, &m);
	zassert_true(rc == -EINVAL, NULL);

	/* Compute the weighted mean value with weights from the vector 'w4'.
	 * An error is expected due to difference in sizes of w4 and v vector. */
	rc = zsl_sta_weighted_mean(&v, &w4, &m);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_sta_time_weighted_mean)
{
	int rc;
	zsl_real_t m;

	ZSL_VECTOR_DEF(v, 5);
	ZSL_VECTOR_DEF(v2, 5);
	ZSL_VECTOR_DEF(t, 5);
	ZSL_VECTOR_DEF(t2, 5);
	ZSL_VECTOR_DEF(t3, 7);

	zsl_real_t a[5] = { 1.0, 7.0, 4.0, 8.0, 4.0 };
	zsl_real_t a2[5] = { 1.0, -7.0, 4.0, 8.0, -4.0 };
	zsl_real_t b[5] = { 3.0, 10.0, -5.0, 1.0, 5.0 };
	zsl_real_t b2[5] = { 3.0, 3.0, -5.0, 1.0, 5.0 };

	/* Assign arrays to the vectors. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&t, b);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&v2, a2);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&t2, b2);
	zassert_true(rc == 0, NULL);

	/* Compute the time-weighted mean value with times from the vector 't'. */
	rc = zsl_sta_time_weighted_mean(&v, &t, &m);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(m, 5.166667, 1E-6), NULL);

	/* An error is expected due to negative coeficients in 'v'. */
	rc = zsl_sta_time_weighted_mean(&v2, &t, &m);
	zassert_true(rc == -EINVAL, NULL);

	/* An error is expected due to repeated coeficients in 't'. */
	rc = zsl_sta_time_weighted_mean(&v, &t2, &m);
	zassert_true(rc == -EINVAL, NULL);

	/* An error is expected due to difference in sizes of 'v' and 't'
	 * vectors. */
	rc = zsl_sta_time_weighted_mean(&v, &t3, &m);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_sta_demean)
{
	int rc;
	zsl_real_t m;

	ZSL_VECTOR_DEF(v, 10);
	ZSL_VECTOR_DEF(w, 10);
	ZSL_VECTOR_DEF(u, 10);
	ZSL_VECTOR_DEF(t, 5);

	zsl_real_t a[10] = { -3.0,  1.0,  2.0,  8.5, -3.5,
			     4.0,  7.0, -2.0,  0.0,  6.0 };
	zsl_real_t b[10] = { -5.0, -1.0,  0.0,  6.5, -5.5,
			     2.0,  5.0, -4.0, -2.0,  4.0 };

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

	/* Calculate the de-meaned vector t. An error is epected due to invalid
	 * vector dimensions. */
	rc = zsl_sta_demean(&v, &t);
	zassert_true(rc == -EINVAL, NULL);

}

ZTEST(zsl_tests, test_sta_percentile)
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

	/* Calculate the 103 percentile. An error is expected. */
	rc = zsl_sta_percentile(&v, 103, &val);
	zassert_true(rc == -EINVAL, NULL);

	/* Calculate the -10 percentile. An error is expected. */
	rc = zsl_sta_percentile(&v, -10, &val);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_sta_median)
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

ZTEST(zsl_tests, test_sta_weighted_median)
{
	int rc;
	zsl_real_t m;

	ZSL_VECTOR_DEF(v, 10);
	ZSL_VECTOR_DEF(w, 10);
	ZSL_VECTOR_DEF(w2, 10);
	ZSL_VECTOR_DEF(w3, 10);
	ZSL_VECTOR_DEF(w4, 10);
	ZSL_VECTOR_DEF(w5, 3);

	zsl_real_t a[10] = { -3.0, 1.0, 2.0, 8.5, -3.5, 4.0, 7.0, -2.0, 0.0, 6.0 };
	zsl_real_t b[10] = { 0.1, 0.30, 0.15, 0.05, 0.05,
			     0.1, 0.03, 0.07, 0.09, 0.06 };
	zsl_real_t b2[10] = { 0.1, 0.30, 0.1, 0.05, 0.05,
			      0.1, 0.03, 0.07, 0.14, 0.06 };
	zsl_real_t b3[10] = { 0.1, -0.30, 0.15, 0.05, 0.05,
			      0.1, -0.03, 0.07, 0.09, 0.06 };
	zsl_real_t b4[10] = { 0.1, 0.30, 0.15, 0.05, 0.05,
			      0.1, 0.05, 0.07, 0.09, 0.06 };

	/* Assign arrays to the vector. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&w, b);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&w2, b2);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&w3, b3);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&w4, b4);
	zassert_true(rc == 0, NULL);

	/* Calculate the weighted median with the weights in the vector 'w'. */
	rc = zsl_sta_weighted_median(&v, &w, &m);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(m, -2.0, 1E-6), NULL);

	/* Calculate the weighted median with the weights in the vector 'w2'. */
	rc = zsl_sta_weighted_median(&v, &w2, &m);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(m, -1.0, 1E-6), NULL);

	/* Calculate the weighted median with the weights in the vector 'w3'.
	 * An error is expected because of negative weights. */
	rc = zsl_sta_weighted_median(&v, &w3, &m);
	zassert_true(rc == -EINVAL, NULL);

	/* Calculate the weighted median with the weights in the vector 'w4'.
	 * An error is expected because of weights that don't add up to 1. */
	rc = zsl_sta_weighted_median(&v, &w4, &m);
	zassert_true(rc == -EINVAL, NULL);

	/* Calculate the weighted median with the weights in the vector 'w5'.
	 * An error is expected due to different sizes of vectors. */
	rc = zsl_sta_weighted_median(&v, &w5, &m);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_sta_quartiles)
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

ZTEST(zsl_tests, test_sta_quart_range)
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

ZTEST(zsl_tests, test_sta_mode)
{
	int rc;

	ZSL_VECTOR_DEF(va, 10);
	ZSL_VECTOR_DEF(vb, 10);
	ZSL_VECTOR_DEF(ma, 10);
	ZSL_VECTOR_DEF(mb, 10);
	ZSL_VECTOR_DEF(mc, 5);

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

	/* If the size of the input vectors doesn't match, an error should
	 * occur. */
	rc = zsl_sta_mode(&va, &mc);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_sta_data_range)
{
	int rc;

	ZSL_VECTOR_DEF(v, 10);
	zsl_real_t r;

	zsl_real_t a[10] = { -2.0, 1.0, 7.0, 1.5, 1.5, -2.0, 1.0, -5.0, 1.0, -2.0 };

	/* Assign array to the vector. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);

	/* Calculate the data range. */
	rc = zsl_sta_data_range(&v, &r);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(r, 12.0, 1E-6), NULL);
}

ZTEST(zsl_tests, test_sta_mean_abs_dev)
{
	int rc;

	ZSL_VECTOR_DEF(v, 6);
	ZSL_VECTOR_DEF(w, 0);
	zsl_real_t m;

	zsl_real_t a[6] = { 10.0, 15.0, 15.0, 17.0, 18.0, 21.0 };

	/* Assign array to the vector. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);

	/* Calculate the mean absolute deviation of the data in 'v'. */
	rc = zsl_sta_mean_abs_dev(&v, &m);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(m, 8.0 / 3.0, 1E-6), NULL);

	/* Calculate the mean absolute deviation. An error is expected due to the
	 * invalid dimension of 'w'. */
	rc = zsl_sta_mean_abs_dev(&w, &m);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_sta_median_abs_dev)
{
	int rc;

	ZSL_VECTOR_DEF(v, 7);
	zsl_real_t m;

	zsl_real_t a[7] = { 1.0, 1.0, 2.0, 2.0, 4.0, 6.0, 9.0 };

	/* Assign array to the vector. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);

	/* Calculate the median absolute deviation of the data in 'v'. */
	rc = zsl_sta_median_abs_dev(&v, &m);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(m, 1.0, 1E-6), NULL);
}

ZTEST(zsl_tests, test_sta_variance)
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

ZTEST(zsl_tests, test_sta_standard_deviation)
{
	int rc;

	ZSL_VECTOR_DEF(v, 10);
	zsl_real_t var;

	zsl_real_t a[10] = { -2.0, 1.0, 3.0, 1.5, 1.5, -2.0, 1.0, -5.0, 1.0, -2.0 };

	/* Assign arrays to the vector. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);

	/* Calculate the standard deviation. */
	rc = zsl_sta_std_dev(&v, &var);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(var, ZSL_SQRT(5.9), 1E-6), NULL);
}

ZTEST(zsl_tests, test_sta_covariance)
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

ZTEST(zsl_tests, test_sta_covariance_matrix)
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

ZTEST(zsl_tests, test_sta_linear_regression)
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

#ifndef CONFIG_ZSL_SINGLE_PRECISION
ZTEST(zsl_tests_double, test_sta_mult_linear_regression)
{
	int rc;

	ZSL_MATRIX_DEF(x, 10, 4);
	ZSL_MATRIX_DEF(x2, 10, 4);
	ZSL_VECTOR_DEF(y, 10);
	ZSL_VECTOR_DEF(y2, 7);
	ZSL_VECTOR_DEF(b, 5);
	ZSL_VECTOR_DEF(b2, 2);

	zsl_real_t r = 0.0;
	zsl_real_t a[40] = {  1.0,  4.0, -3.5,  8.0,
			      2.0, -5.5,  4.0, -9.5,
			      5.5,  1.0,  0.0, -8.0,
			      -2.5, -1.0,  7.5, -6.0,
			      -1.5,  2.5, -5.0,  4.0,
			      3.5,  9.0,  8.0, -6.0,
			      -4.0,  7.0,  0.0,  2.5,
			      0.0, -5.5, -0.5,  0.5,
			      6.5, -9.5,  1.0, -1.5,
			      5.5, -0.5, -8.0,  0.0 };

	zsl_real_t c[40] = {  1.0,  2.0, -3.5,  8.0,
			      2.0,  4.0,  4.0, -9.5,
			      5.5, 11.0,  0.0, -8.0,
			      -2.5, -5.0,  7.5, -6.0,
			      -1.5, -3.0, -5.0,  4.0,
			      3.5,  7.0,  8.0, -6.0,
			      -4.0, -8.0,  0.0,  2.5,
			      0.0,  0.0, -0.5,  0.5,
			      6.5, 13.0,  1.0, -1.5,
			      5.5, 11.0, -8.0,  0.0 };

	zsl_real_t d[10] = {  1.0,  2.5,  4.0,  5.5,  7.0,
			      8.5, 10.0, 11.5, 13.0, 14.5 };

	/* Assign arrays to the vectors and matrices and initialize
	 * the empty ones. */
	rc = zsl_mtx_from_arr(&x, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_from_arr(&x2, c);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&y, d);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_init(&b);
	zassert_true(rc == 0, NULL);


	/* Calculate the coefficiens and the coefficient of determination of the
	 * multiple linear regression. */
	rc = zsl_sta_mult_linear_reg(&x, &y, &b, &r);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(b.data[0], 7.617854, 1E-6), NULL);
	zassert_true(val_is_equal(b.data[1], 0.255721, 1E-6), NULL);
	zassert_true(val_is_equal(b.data[2], -0.145225, 1E-6), NULL);
	zassert_true(val_is_equal(b.data[3], -0.099143, 1E-6), NULL);
	zassert_true(val_is_equal(b.data[4], 0.137828, 1E-6), NULL);
	zassert_true(val_is_equal(r, 0.140693, 1E-6), NULL);

	/* In the next case, the columns of the matrix 'x2' are not linearly
	 * independent. In this case, an error should occur. */
	rc = zsl_sta_mult_linear_reg(&x2, &y, &b, &r);
	zassert_true(rc == -EINVAL, NULL);

	/* In the next cases, the dimensions of the input and output matrices are
	 * invalid (they don't match). */
	rc = zsl_sta_mult_linear_reg(&x, &y2, &b, &r);
	zassert_true(rc == -EINVAL, NULL);

	rc = zsl_sta_mult_linear_reg(&x, &y, &b2, &r);
	zassert_true(rc == -EINVAL, NULL);
}
#endif

#ifndef CONFIG_ZSL_SINGLE_PRECISION
ZTEST(zsl_tests_double, test_sta_weighted_mult_linear_regression)
{
	int rc;

	ZSL_MATRIX_DEF(x, 10, 4);
	ZSL_MATRIX_DEF(x2, 10, 4);
	ZSL_VECTOR_DEF(y, 10);
	ZSL_VECTOR_DEF(y2, 7);
	ZSL_VECTOR_DEF(b, 5);
	ZSL_VECTOR_DEF(b2, 2);
	ZSL_VECTOR_DEF(w, 10);
	ZSL_VECTOR_DEF(w2, 15);

	zsl_real_t r = 0.0;
	zsl_real_t a[40] = {  1.0,  4.0, -3.5,  8.0,
			      2.0, -5.5,  4.0, -9.5,
			      5.5,  1.0,  0.0, -8.0,
			      -2.5, -1.0,  7.5, -6.0,
			      -1.5,  2.5, -5.0,  4.0,
			      3.5,  9.0,  8.0, -6.0,
			      -4.0,  7.0,  0.0,  2.5,
			      0.0, -5.5, -0.5,  0.5,
			      6.5, -9.5,  1.0, -1.5,
			      5.5, -0.5, -8.0,  0.0 };

	zsl_real_t c[40] = {  1.0,  2.0, -3.5,  8.0,
			      2.0,  4.0,  4.0, -9.5,
			      5.5, 11.0,  0.0, -8.0,
			      -2.5, -5.0,  7.5, -6.0,
			      -1.5, -3.0, -5.0,  4.0,
			      3.5,  7.0,  8.0, -6.0,
			      -4.0, -8.0,  0.0,  2.5,
			      0.0,  0.0, -0.5,  0.5,
			      6.5, 13.0,  1.0, -1.5,
			      5.5, 11.0, -8.0,  0.0 };

	zsl_real_t d[10] = {  1.0,  2.5,  4.0,  5.5,  7.0,
			      8.5, 10.0, 11.5, 13.0, 14.5 };

	zsl_real_t e[10] = {  15.0,  9.0,  9.0,  8.0,  7.0,
			      10.0, 12.5, 5.0, 4.0, 13.5 };

	/* Assign arrays to the vectors and matrices and initialize
	 * the empty ones. */
	rc = zsl_mtx_from_arr(&x, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_from_arr(&x2, c);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&y, d);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_init(&b);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&w, e);
	zassert_true(rc == 0, NULL);


	/* Calculate the coefficiens and the coefficient of determination of the
	 * weighted multiple linear regression. */
	rc = zsl_sta_weighted_mult_linear_reg(&x, &y, &w, &b, &r);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(b.data[0], 8.087948, 1E-6), NULL);
	zassert_true(val_is_equal(b.data[1], 0.403896, 1E-6), NULL);
	zassert_true(val_is_equal(b.data[2], -0.230993, 1E-6), NULL);
	zassert_true(val_is_equal(b.data[3], 0.142673, 1E-6), NULL);
	zassert_true(val_is_equal(b.data[4], 0.449494, 1E-6), NULL);
	zassert_true(val_is_equal(r, 0.078979, 1E-6), NULL);

	/* In the next case, the columns of the matrix 'x2' are not linearly
	 * independent. In this case, an error should occur. */
	rc = zsl_sta_weighted_mult_linear_reg(&x2, &y, &w, &b, &r);
	zassert_true(rc == -EINVAL, NULL);

	/* In the next cases, the dimensions of the input and output matrices are
	 * invalid (they don't match). */
	rc = zsl_sta_weighted_mult_linear_reg(&x, &y2, &w, &b, &r);
	zassert_true(rc == -EINVAL, NULL);

	rc = zsl_sta_weighted_mult_linear_reg(&x, &y, &w2, &b, &r);
	zassert_true(rc == -EINVAL, NULL);

	rc = zsl_sta_weighted_mult_linear_reg(&x, &y, &w, &b2, &r);
	zassert_true(rc == -EINVAL, NULL);
}
#endif

#ifndef CONFIG_ZSL_SINGLE_PRECISION
ZTEST(zsl_tests_double, test_sta_quadric_fitting)
{
	int rc;

	ZSL_MATRIX_DEF(m, 12, 3);
	ZSL_MATRIX_DEF(m2, 9, 4);
	ZSL_VECTOR_DEF(b, 9);
	ZSL_VECTOR_DEF(b2, 5);

	/* Data points. */
	zsl_real_t a[36] = {
		7.0,  22.0, 31.0,
		7.0,  19.0, 28.0,
		9.0,  23.0, 31.0,
		9.0,  19.0, 27.0,
		11.0, 24.0, 29.0,
		11.0, 20.0, 26.0,
		8.0,  21.0, 32.0,
		8.0,  17.0, 29.0,
		10.0, 22.0, 32.0,
		10.0, 18.0, 28.0,
		12.0, 23.0, 31.0,
		12.0, 19.0, 28.0
	};

	/* Assign arrays to the matrices. */
	rc = zsl_mtx_from_arr(&m, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_from_arr(&m2, a);
	zassert_true(rc == 0, NULL);

	/* Calculate the coefficiens of the least square fitting. */
	rc = zsl_sta_quad_fit(&m, &b);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(b.data[0], -0.000555, 1E-6), NULL);
	zassert_true(val_is_equal(b.data[1], -0.000834, 1E-6), NULL);
	zassert_true(val_is_equal(b.data[2], -0.000977, 1E-6), NULL);
	zassert_true(val_is_equal(b.data[3], 0.000483, 1E-6), NULL);
	zassert_true(val_is_equal(b.data[4], -0.000468, 1E-6), NULL);
	zassert_true(val_is_equal(b.data[5], 0.000258, 1E-6), NULL);
	zassert_true(val_is_equal(b.data[6], 0.009216, 1E-6), NULL);
	zassert_true(val_is_equal(b.data[7], 0.004983, 1E-6), NULL);
	zassert_true(val_is_equal(b.data[8], 0.027798, 1E-6), NULL);

	/* Calculate the coefficiens of the least square fitting. An error is
	 * expected due to invalid matrix/vector dimensions. */
	rc = zsl_sta_quad_fit(&m2, &b);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_sta_quad_fit(&m, &b2);
	zassert_true(rc == -EINVAL, NULL);
}
#endif

ZTEST(zsl_tests, test_sta_absolute_error)
{
	int rc;
	zsl_real_t val = 9.9985, exp_val = 10.0, err;

	/* Calculate the absolute error. */
	rc = zsl_sta_abs_err(&val, &exp_val, &err);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(err, 0.0015, 1E-6), NULL);
}

ZTEST(zsl_tests, test_sta_relative_error)
{
	int rc;
	zsl_real_t val = 9.9985, exp_val = 10.0, err;

	/* Calculate the relative error. */
	rc = zsl_sta_rel_err(&val, &exp_val, &err);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(err, 0.015, 1E-5), NULL);
}

ZTEST(zsl_tests, test_sta_standard_error)
{
	int rc;

	ZSL_VECTOR_DEF(v, 6);
	ZSL_VECTOR_DEF(w, 0);
	zsl_real_t err;

	zsl_real_t a[6] = { 10.0, 15.0, 15.0, 17.0, 18.0, 21.0 };

	/* Assign array to the vector. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);

	/* Calculate the standard error of the data in 'v'. */
	rc = zsl_sta_sta_err(&v, &err);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(err, 1.50554530542, 1E-6), NULL);

	/* Calculate the standard error. An error is expected due to the invalud
	 * dimension of 'w'. */
	rc = zsl_sta_sta_err(&w, &err);
	zassert_true(rc == -EINVAL, NULL);
}
