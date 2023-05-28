/*
 * Copyright (c) 2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/orientation/fusion/calibration.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_fus_cal_rot_mtx)
{
	int rc = 0;

	ZSL_VECTOR_DEF(v, 3);
	ZSL_VECTOR_DEF(v2, 4);
	ZSL_VECTOR_DEF(w, 3);
	ZSL_VECTOR_DEF(w2, 4);
	ZSL_MATRIX_DEF(m, 3, 3);
	ZSL_MATRIX_DEF(m2, 4, 3);
	ZSL_MATRIX_DEF(m3, 3, 4);

	zsl_real_t a[3] = { 1.0, 3.0, -2.0 };
	zsl_real_t b[9] = {
		-0.07407397629608892, 0.2592592572432197, 0.9629629710271213,
		0.8518518296753963, 0.5185185417029887, -0.07407416681195528,
		-0.5185185689195382, 0.8148148007025278, -0.2592592028101106
	};

	/* Assign arrays to vectors and matrices. */
	rc = zsl_vec_from_arr(&v, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_from_arr(&m, b);
	zassert_true(rc == 0, NULL);

	/* Rotate the vector v using the matrix m. */
	rc = zsl_fus_cal_rot_mtx(&v, &m, &w);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(w.data[0], -1.222222, 1E-6), NULL);
	zassert_true(val_is_equal(w.data[1], 2.555556, 1E-6), NULL);
	zassert_true(val_is_equal(w.data[2], 2.444444, 1E-6), NULL);

	/* Special cases where inputs are invalid. */
	rc = zsl_fus_cal_rot_mtx(&v2, &m, &w);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_rot_mtx(&v, &m2, &w);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_rot_mtx(&v, &m3, &w);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_rot_mtx(&v, &m, &w2);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_fus_cal_rot_axis_angle)
{
	int rc = 0;

	ZSL_VECTOR_DEF(v, 3);
	ZSL_VECTOR_DEF(v2, 4);
	ZSL_VECTOR_DEF(w, 3);
	ZSL_VECTOR_DEF(w2, 4);
	ZSL_VECTOR_DEF(a, 3);
	ZSL_VECTOR_DEF(a2, 4);

	zsl_real_t b = 1.3;

	zsl_real_t c[3] = { 1.0, 3.0, -2.0 };
	zsl_real_t d[3] = { -1.0, 3.1, 12.8 };

	/* Assign arrays to vectors and matrices. */
	rc = zsl_vec_from_arr(&v, c);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&a, d);
	zassert_true(rc == 0, NULL);

	/* Rotate the vector v using the axis a and angle b. */
	rc = zsl_fus_cal_rot_axis_angle(&v, &a, &b, &w);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(w.data[0], -2.913558, 1E-6), NULL);
	zassert_true(val_is_equal(w.data[1], 1.365200, 1E-6), NULL);
	zassert_true(val_is_equal(w.data[2], -1.909819, 1E-6), NULL);

	/* Special cases where inputs are invalid. */
	rc = zsl_fus_cal_rot_axis_angle(&v2, &a, &b, &w);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_rot_axis_angle(&v, &a2, &b, &w);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_rot_axis_angle(&v, &a, &b, &w2);
	zassert_true(rc == -EINVAL, NULL);
}

#ifndef CONFIG_ZSL_SINGLE_PRECISION
ZTEST(zsl_tests_double, test_fus_cal_magn)
{
	int rc = 0;

	ZSL_MATRIX_DEF(m, 12, 3);
	ZSL_MATRIX_DEF(m2, 12, 4);
	ZSL_MATRIX_DEF(K, 3, 3);
	ZSL_MATRIX_DEF(K2, 3, 7);
	ZSL_MATRIX_DEF(K3, 1, 3);
	ZSL_VECTOR_DEF(b, 3);
	ZSL_VECTOR_DEF(b2, 12);

	zsl_real_t l = 1.0;
	zsl_real_t mu = 10.0, mu2 = 0.25;

	zsl_real_t a[36] = {
		2.0,  4.0, -5.0,
		1.1,  8.3,  9.9,
		-0.4, -6.6,  3.4,
		1.5, -6.9,  0.1,
		0.0, -2.5, -9.8,
		1.2,  3.8, -0.9,
		6.1, -4.3,  8.8,
		-7.6, -0.9,  2.9,
		5.0,  7.1,  1.0,
		0.0, -4.7,  0.2,
		4.3,  0.0,  5.7,
		-9.0, -2.8,  7.3
	};

	/* Assign arrays to matrices. */
	rc = zsl_mtx_from_arr(&m, a);
	zassert_true(rc == 0, NULL);

	/* Compute the ellipsoid fitting. */
	rc = zsl_fus_cal_magn(&m, &l, &mu, &K, &b);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(K.data[0], 0.600621, 1E-6), NULL);
	zassert_true(val_is_equal(K.data[1], -0.610451, 1E-6), NULL);
	zassert_true(val_is_equal(K.data[2], -0.318351, 1E-6), NULL);
	zassert_true(val_is_equal(K.data[3], -0.610451, 1E-6), NULL);
	zassert_true(val_is_equal(K.data[4], 1.968980, 1E-6), NULL);
	zassert_true(val_is_equal(K.data[5], 0.152699, 1E-6), NULL);
	zassert_true(val_is_equal(K.data[6], -0.318351, 1E-6), NULL);
	zassert_true(val_is_equal(K.data[7], 0.152699, 1E-6), NULL);
	zassert_true(val_is_equal(K.data[8], 2.868776, 1E-6), NULL);

	zassert_true(val_is_equal(b.data[0], -1.211677, 1E-6), NULL);
	zassert_true(val_is_equal(b.data[1], -1.842987, 1E-6), NULL);
	zassert_true(val_is_equal(b.data[2], 0.087542, 1E-6), NULL);

	/* Special cases where inputs are invalid. */
	rc = zsl_fus_cal_magn(&m2, &l, &mu, &K, &b);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_magn(&m, &l, &mu2, &K, &b);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_magn(&m, &l, &mu, &K2, &b);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_magn(&m, &l, &mu, &K3, &b);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_magn(&m, &l, &mu, &K, &b2);
	zassert_true(rc == -EINVAL, NULL);
}
#endif

#ifndef CONFIG_ZSL_SINGLE_PRECISION
ZTEST(zsl_tests_double, test_fus_cal_magn_fast)
{
	int rc = 0;

	ZSL_MATRIX_DEF(m, 12, 3);
	ZSL_MATRIX_DEF(m2, 12, 4);
	ZSL_MATRIX_DEF(K, 3, 3);
	ZSL_MATRIX_DEF(K2, 3, 7);
	ZSL_MATRIX_DEF(K3, 1, 3);
	ZSL_VECTOR_DEF(b, 3);
	ZSL_VECTOR_DEF(b2, 12);

	zsl_real_t me = 44.3867, me2 = -7.0;

	zsl_real_t a[36] = {
		2.0,  4.0, -5.0,
		1.1,  8.3,  9.9,
		-0.4, -6.6,  3.4,
		1.5, -6.9,  0.1,
		0.0, -2.5, -9.8,
		1.2,  3.8, -0.9,
		6.1, -4.3,  8.8,
		-7.6, -0.9,  2.9,
		5.0,  7.1,  1.0,
		0.0, -4.7,  0.2,
		4.3,  0.0,  5.7,
		-9.0, -2.8,  7.3
	};

	/* Assign arrays to matrices. */
	rc = zsl_mtx_from_arr(&m, a);
	zassert_true(rc == 0, NULL);

	/* Compute the magnetometer data fitting. */
	rc = zsl_fus_cal_magn_fast(&m, &me, &K, &b);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(K.data[0], 5.685746, 1E-6), NULL);
	zassert_true(val_is_equal(K.data[1], 0.091864, 1E-6), NULL);
	zassert_true(val_is_equal(K.data[2], 0.870620, 1E-6), NULL);
	zassert_true(val_is_equal(K.data[3], 0.000000, 1E-6), NULL);
	zassert_true(val_is_equal(K.data[4], 6.375517, 1E-6), NULL);
	zassert_true(val_is_equal(K.data[5], -0.588939, 1E-6), NULL);
	zassert_true(val_is_equal(K.data[6], 0.000000, 1E-6), NULL);
	zassert_true(val_is_equal(K.data[7], 0.000000, 1E-6), NULL);
	zassert_true(val_is_equal(K.data[8], 2.783505, 1E-6), NULL);

	zassert_true(val_is_equal(b.data[0], 1.088338, 1E-6), NULL);
	zassert_true(val_is_equal(b.data[1], -0.900774, 1E-6), NULL);
	zassert_true(val_is_equal(b.data[2], -7.088266, 1E-6), NULL);

	/* Special cases where inputs are invalid. */
	rc = zsl_fus_cal_magn_fast(&m2, &me, &K, &b);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_magn_fast(&m, &me2, &K, &b);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_magn_fast(&m, &me, &K2, &b);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_magn_fast(&m, &me, &K3, &b);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_magn_fast(&m, &me, &K, &b2);
	zassert_true(rc == -EINVAL, NULL);
}
#endif

ZTEST(zsl_tests, test_fus_cal_corr_scalar)
{
	int rc = 0;

	zsl_real_t d, k, b, d_out;

	d = 3.5;
	k = 4.2;
	b = -2.1;

	/* Apply the correction. */
	rc = zsl_fus_cal_corr_scalar(&d, &k, &b, &d_out);
	zassert_true(val_is_equal(d_out, 5.88, 1E-6), NULL);
}

ZTEST(zsl_tests, test_fus_cal_corr_vec)
{
	int rc = 0;

	ZSL_MATRIX_DEF(K, 3, 3);
	ZSL_MATRIX_DEF(K2, 3, 7);
	ZSL_MATRIX_DEF(K3, 1, 3);
	ZSL_VECTOR_DEF(b, 3);
	ZSL_VECTOR_DEF(b2, 6);
	ZSL_VECTOR_DEF(v, 3);
	ZSL_VECTOR_DEF(v2, 1);
	ZSL_VECTOR_DEF(v_out, 3);
	ZSL_VECTOR_DEF(v_out2, 2);

	zsl_real_t a[9] = {
		0.99, 0.12, 0.01,
		0.12, 0.87, 0.09,
		0.01, 0.09, 0.92
	};

	zsl_real_t c[3] = { 1.2, -4.9, 5.7 };
	zsl_real_t d[3] = { 0.8, 1.2, 5.4 };

	/* Assign arrays to vectors and matrices. */
	rc = zsl_mtx_from_arr(&K, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&b, c);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&v, d);
	zassert_true(rc == 0, NULL);

	/* Correct the magnetometer data. */
	rc = zsl_fus_cal_corr_vec(&v, &K, &b, &v_out);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(v_out.data[0], 1.647000, 1E-6), NULL);
	zassert_true(val_is_equal(v_out.data[1], -1.980000, 1E-6), NULL);
	zassert_true(val_is_equal(v_out.data[2], 9.899000, 1E-6), NULL);

	/* Special cases where inputs are invalid. */
	rc = zsl_fus_cal_corr_vec(&v2, &K, &b, &v_out);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_corr_vec(&v, &K2, &b, &v_out);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_corr_vec(&v, &K3, &b, &v_out);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_corr_vec(&v, &K, &b2, &v_out);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_corr_vec(&v, &K, &b, &v_out2);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_fus_cal_madg)
{
	int rc = 0;

	ZSL_MATRIX_DEF(g, 12, 3);
	ZSL_MATRIX_DEF(g2, 12, 7);
	ZSL_MATRIX_DEF(g3, 14, 3);
	ZSL_MATRIX_DEF(a, 12, 3);
	ZSL_MATRIX_DEF(a2, 12, 1);
	ZSL_MATRIX_DEF(a3, 3, 3);
	ZSL_MATRIX_DEF(m, 12, 3);
	ZSL_MATRIX_DEF(m2, 12, 9);
	ZSL_MATRIX_DEF(m3, 20, 3);

	zsl_real_t beta;

	zsl_real_t datg[36] = {
		-0.060000,  0.130000, -0.060000,
		-0.190000, -0.130000,  0.060000,
		-0.060000,  0.130000,  0.060000,
		-0.060000,  0.190000,  0.130000,
		0.000000,  0.000000,  0.130000,
		0.000000,  0.130000,  0.130000,
		-0.190000,  0.130000,  0.130000,
		-0.060000,  0.130000,  0.060000,
		-0.130000,  0.250000,  0.000000,
		-0.060000,  0.000000,  0.130000,
		0.060000,  0.190000,  0.130000,
		-0.190000, -0.060000,  0.130000
	};

	zsl_real_t data[36] = {
		-38.690000, 5.120000, -59.190000,
		-39.380000, 5.870000, -59.190000,
		-37.940000, 5.870000, -59.190000,
		-39.380000, 5.870000, -60.380000,
		-36.810000, 5.500000, -59.190000,
		-40.500000, 5.870000, -59.560000,
		-38.310000, 5.870000, -59.940000,
		-36.810000, 6.620000, -60.750000,
		-38.690000, 7.000000, -60.000000,
		-38.690000, 7.000000, -58.440000,
		-37.560000, 6.620000, -59.190000,
		-37.940000, 7.000000, -58.000000
	};

	zsl_real_t datm[36] = {
		-0.200000, 0.050000, 10.090000,
		-0.170000, 0.030000, 10.090000,
		-0.190000, 0.010000, 10.120000,
		-0.190000, 0.060000, 10.080000,
		-0.210000, 0.080000, 10.080000,
		-0.190000, 0.040000, 10.080000,
		-0.190000, 0.040000, 10.130000,
		-0.180000, 0.050000, 10.110000,
		-0.190000, 0.050000, 10.100000,
		-0.200000, 0.030000, 10.070000,
		-0.190000, 0.060000, 10.080000,
		-0.180000, 0.030000, 10.100000
	};

	/* Assign arrays to matrices. */
	rc = zsl_mtx_from_arr(&g, datg);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_from_arr(&a, data);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_from_arr(&m, datm);
	zassert_true(rc == 0, NULL);

	/* Calculate the value of beta. */
	rc = zsl_fus_cal_madg(&g, &a, &m, 100.0, NULL, &beta);
	zassert_true(rc == 0, NULL);
	//zassert_true(val_is_equal(beta, 0.999, 1E-6), NULL);
	zassert_true(val_is_equal(beta, 0.0, 1E-6), NULL);

	/* Special cases where inputs are invalid. */
	rc = zsl_fus_cal_madg(&g, &a, &m, -100.0, NULL, &beta);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_madg(&g2, &a, &m, 100.0, NULL, &beta);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_madg(&g, &a2, &m, 100.0, NULL, &beta);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_madg(&g, &a, &m2, 100.0, NULL, &beta);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_madg(&g3, &a, &m, 100.0, NULL, &beta);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_madg(&g, &a3, &m, 100.0, NULL, &beta);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_madg(&g, &a, &m3, 100.0, NULL, &beta);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_fus_cal_mahn)
{
	int rc = 0;

	ZSL_MATRIX_DEF(g, 12, 3);
	ZSL_MATRIX_DEF(g2, 12, 7);
	ZSL_MATRIX_DEF(g3, 14, 3);
	ZSL_MATRIX_DEF(a, 12, 3);
	ZSL_MATRIX_DEF(a2, 12, 1);
	ZSL_MATRIX_DEF(a3, 3, 3);
	ZSL_MATRIX_DEF(m, 12, 3);
	ZSL_MATRIX_DEF(m2, 12, 9);
	ZSL_MATRIX_DEF(m3, 20, 3);

	zsl_real_t kp;

	zsl_real_t datg[36] = {
		-0.060000,  0.130000, -0.060000,
		-0.190000, -0.130000,  0.060000,
		-0.060000,  0.130000,  0.060000,
		-0.060000,  0.190000,  0.130000,
		0.000000,  0.000000,  0.130000,
		0.000000,  0.130000,  0.130000,
		-0.190000,  0.130000,  0.130000,
		-0.060000,  0.130000,  0.060000,
		-0.130000,  0.250000,  0.000000,
		-0.060000,  0.000000,  0.130000,
		0.060000,  0.190000,  0.130000,
		-0.190000, -0.060000,  0.130000
	};

	zsl_real_t data[36] = {
		-38.690000, 5.120000, -59.190000,
		-39.380000, 5.870000, -59.190000,
		-37.940000, 5.870000, -59.190000,
		-39.380000, 5.870000, -60.380000,
		-36.810000, 5.500000, -59.190000,
		-40.500000, 5.870000, -59.560000,
		-38.310000, 5.870000, -59.940000,
		-36.810000, 6.620000, -60.750000,
		-38.690000, 7.000000, -60.000000,
		-38.690000, 7.000000, -58.440000,
		-37.560000, 6.620000, -59.190000,
		-37.940000, 7.000000, -58.000000
	};

	zsl_real_t datm[36] = {
		-0.200000, 0.050000, 10.090000,
		-0.170000, 0.030000, 10.090000,
		-0.190000, 0.010000, 10.120000,
		-0.190000, 0.060000, 10.080000,
		-0.210000, 0.080000, 10.080000,
		-0.190000, 0.040000, 10.080000,
		-0.190000, 0.040000, 10.130000,
		-0.180000, 0.050000, 10.110000,
		-0.190000, 0.050000, 10.100000,
		-0.200000, 0.030000, 10.070000,
		-0.190000, 0.060000, 10.080000,
		-0.180000, 0.030000, 10.100000
	};

	/* Assign arrays to matrices. */
	rc = zsl_mtx_from_arr(&g, datg);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_from_arr(&a, data);
	zassert_true(rc == 0, NULL);
	rc = zsl_mtx_from_arr(&m, datm);
	zassert_true(rc == 0, NULL);

	/* Calculate the value of kp. */
	rc = zsl_fus_cal_mahn(&g, &a, &m, 100.0, NULL, &kp);
	zassert_true(rc == 0, NULL);
	//zassert_true(val_is_equal(kp, 0.999, 1E-6), NULL);
	zassert_true(val_is_equal(kp, 0.0, 1E-6), NULL);

	/* Special cases where inputs are invalid. */
	rc = zsl_fus_cal_mahn(&g, &a, &m, -100.0, NULL, &kp);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_mahn(&g2, &a, &m, 100.0, NULL, &kp);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_mahn(&g, &a2, &m, 100.0, NULL, &kp);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_mahn(&g, &a, &m2, 100.0, NULL, &kp);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_mahn(&g3, &a, &m, 100.0, NULL, &kp);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_mahn(&g, &a3, &m, 100.0, NULL, &kp);
	zassert_true(rc == -EINVAL, NULL);
	rc = zsl_fus_cal_mahn(&g, &a, &m3, 100.0, NULL, &kp);
	zassert_true(rc == -EINVAL, NULL);
}
