/*
 * Copyright (c) 2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/orientation/orientation.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_att_to_vec)
{
	int rc;
	struct zsl_attitude a = { .roll = 15.0, .pitch = 2.5, .yaw = -10.0 };
	struct zsl_vec v;

	/* Map the pointers in both structs to the same address. */
	rc = zsl_att_to_vec(&a, &v);
	zassert_true(rc == 0, NULL);
	zassert_true(v.sz == 3, NULL);

	/* Make sure both structs have the same values. */
	zassert_true(v.data[0] == a.roll, NULL);
	zassert_true(v.data[1] == a.pitch, NULL);
	zassert_true(v.data[2] == a.yaw, NULL);

	/* Verify that modifying attitude still updates vector. */
	a.pitch = 45.0;
	zassert_true(v.data[1] == 45.0, NULL);

	/* And vice versa. */
	v.data[2] = -5.5;
	zassert_true(a.yaw == -5.5, NULL);
}

ZTEST(zsl_tests, test_att_to_euler)
{
	int rc;
	struct zsl_attitude a = {
		.roll = 164.0545819,
		.pitch = 74.3575337,
		.yaw = -105.9453765
	};
	struct zsl_euler e;
	struct zsl_euler ecmp = {
		.x = 2.8632926,
		.y = 1.2977838,
		.z = -1.8490956
	};

	rc = zsl_att_to_euler(&a, &e);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(e.x, ecmp.x, 1E-4), NULL);
	zassert_true(val_is_equal(e.y, ecmp.y, 1E-4), NULL);
	zassert_true(val_is_equal(e.z, ecmp.z, 1E-4), NULL);
}

ZTEST(zsl_tests, test_att_from_euler)
{
	int rc;
	struct zsl_euler e = {
		.x = 2.8632926,
		.y = 1.2977838,
		.z = -1.8490956
	};
	struct zsl_attitude a;
	struct zsl_attitude acmp = {
		.roll = 164.0545819,
		.pitch = 74.3575337,
		.yaw = -105.9453765
	};

	rc = zsl_att_from_euler(&e, &a);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(a.roll, acmp.roll, 1E-4), NULL);
	zassert_true(val_is_equal(a.pitch, acmp.pitch, 1E-4), NULL);
	zassert_true(val_is_equal(a.yaw, acmp.yaw, 1E-4), NULL);
}

ZTEST(zsl_tests, test_att_from_accelmag)
{
	int rc;

	ZSL_VECTOR_DEF(accel, 3);
	ZSL_VECTOR_DEF(accel2, 4);
	ZSL_VECTOR_DEF(mag, 3);
	ZSL_VECTOR_DEF(mag2, 4);
	struct zsl_attitude att;
	struct zsl_attitude acmp = {
		.roll = 131.185925,
		.pitch = -52.790738,
		.yaw = -58.180353
	};

	zsl_real_t a[3] = { 7.0, 4.0, -3.5 };
	zsl_real_t b[3] = { 0.1, -5.0, 123.0 };

	/* Assign arrays to the accelerometer and magnetometer vectors. */
	rc = zsl_vec_from_arr(&accel, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&mag, b);
	zassert_true(rc == 0, NULL);

	/* Calculate the attitude from the accelerometer vector. */
	rc = zsl_att_from_accelmag(&accel, &mag, &att);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(att.roll, acmp.roll, 1E-6), NULL);
	zassert_true(val_is_equal(att.pitch, acmp.pitch, 1E-6), NULL);
	zassert_true(val_is_equal(att.yaw, acmp.yaw, 1E-4), NULL);

	/* In this case, the dimension of the accelerometer data vector is 4, which
	 * should return an error. */
	rc = zsl_att_from_accelmag(&accel2, &mag, &att);
	zassert_true(rc == -EINVAL, NULL);

	/* In this case, the dimension of the magnetometer data vector is 4, which
	 * should return an error. */
	rc = zsl_att_from_accelmag(&accel, &mag2, &att);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_att_from_accel)
{
	int rc;

	ZSL_VECTOR_DEF(accel, 3);
	ZSL_VECTOR_DEF(accel2, 4);
	struct zsl_attitude att;
	struct zsl_attitude acmp = {
		.roll = 131.185925,
		.pitch = -52.790738,
		.yaw = 0.0
	};

	zsl_real_t a[3] = { 7.0, 4.0, -3.5 };

	/* Assign array to the accelerometer vector. */
	rc = zsl_vec_from_arr(&accel, a);
	zassert_true(rc == 0, NULL);

	/* Calculate the attitude from the accelerometer vector. */
	rc = zsl_att_from_accel(&accel, &att);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(att.roll, acmp.roll, 1E-6), NULL);
	zassert_true(val_is_equal(att.pitch, acmp.pitch, 1E-6), NULL);
	zassert_true(val_is_equal(att.yaw, acmp.yaw, 1E-6), NULL);

	/* In this case, the dimension of the accelerometer data vector is 4, which
	 * should return an error. */
	rc = zsl_att_from_accel(&accel2, &att);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_att_accel_angle)
{
	int rc;

	ZSL_VECTOR_DEF(a1, 3);
	ZSL_VECTOR_DEF(a2, 3);
	ZSL_VECTOR_DEF(a3, 7);
	ZSL_VECTOR_DEF(a4, 1);
	zsl_real_t d;

	zsl_real_t a[3] = {  1.0, -13.0, 0.7 };
	zsl_real_t b[3] = { -5.1, 2.1, 147.8 };

	/* Assign array to the accelerometer vectors. */
	rc = zsl_vec_from_arr(&a1, a);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_from_arr(&a2, b);
	zassert_true(rc == 0, NULL);

	/* Calculate the angle between accelerometers. */
	rc = zsl_att_accel_angle(&a1, &a2, &d);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(d, 1.533992, 1E-6), NULL);

	/* In this case, the dimension of one of the accelerometer data vectors
	 * is 7, which should return an error. */
	rc = zsl_att_accel_angle(&a3, &a2, &d);
	zassert_true(rc == -EINVAL, NULL);

	/* In this case, the dimension of one of the accelerometer data vectors
	 * is 1, which should return an error. */
	rc = zsl_att_accel_angle(&a1, &a4, &d);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_eul_to_vec)
{
	int rc;
	struct zsl_euler e = { .x = 15.0, .y = 2.5, .z = -10.0 };
	struct zsl_vec v;

	/* Map the pointers in both structs to the same address. */
	rc = zsl_eul_to_vec(&e, &v);
	zassert_true(rc == 0, NULL);
	zassert_true(v.sz == 3, NULL);

	/* Make sure both structs have the same values. */
	zassert_true(v.data[0] == e.x, NULL);
	zassert_true(v.data[1] == e.y, NULL);
	zassert_true(v.data[2] == e.z, NULL);

	/* Verify that modifying euler still updates vector. */
	e.y = 45.0;
	zassert_true(v.data[1] == 45.0, NULL);

	/* And vice versa. */
	v.data[2] = -5.5;
	zassert_true(e.z == -5.5, NULL);
}

ZTEST(zsl_tests, test_grav_lat_alt)
{
	int rc;
	zsl_real_t g;

	/* Compute the gravity from latitude and altitude. */
	rc = zsl_grav_lat_alt(31.7, 350.0, &g);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(g, 9.793512, 1E-6), NULL);

	/* Invalid altitude. */
	rc = zsl_grav_lat_alt(31.7, -350.0, &g);
	zassert_true(rc == -EINVAL, NULL);

	/* Invalid latitude. */
	rc = zsl_grav_lat_alt(102.0, 350.0, &g);
	zassert_true(rc == -EINVAL, NULL);

	/* Invalid latitude. */
	rc = zsl_grav_lat_alt(-97.3, 350.0, &g);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_quat_init)
{
	struct zsl_quat q;

	zsl_quat_init(&q, ZSL_QUAT_TYPE_EMPTY);
	zassert_true(val_is_equal(q.r, 0.0, 1E-8), NULL);
	zassert_true(val_is_equal(q.i, 0.0, 1E-8), NULL);
	zassert_true(val_is_equal(q.j, 0.0, 1E-8), NULL);
	zassert_true(val_is_equal(q.k, 0.0, 1E-8), NULL);

	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	zassert_true(val_is_equal(q.r, 1.0, 1E-8), NULL);
	zassert_true(val_is_equal(q.i, 0.0, 1E-8), NULL);
	zassert_true(val_is_equal(q.j, 0.0, 1E-8), NULL);
	zassert_true(val_is_equal(q.k, 0.0, 1E-8), NULL);
}

ZTEST(zsl_tests, test_quat_magn)
{
	zsl_real_t m = 0;
	struct zsl_quat q;

	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	m = zsl_quat_magn(&q);
	zassert_true(val_is_equal(m, 1.0, 1E-8), NULL);

	zsl_quat_init(&q, ZSL_QUAT_TYPE_EMPTY);
	m = zsl_quat_magn(&q);
	zassert_true(val_is_equal(m, 0.0, 1E-8), NULL);

	zsl_quat_init(&q, ZSL_QUAT_TYPE_EMPTY);
	q.i = 0.33333333;
	q.j = 0.33333333;
	q.k = 0.33333333;
	q.r = 1.0;
	m = zsl_quat_magn(&q);
	zassert_true(val_is_equal(m, 1.1547002497042251, 1E-6), NULL);
}

ZTEST(zsl_tests, test_quat_to_unit)
{
	int rc;
	struct zsl_quat q;
	struct zsl_quat qn;

	zsl_quat_init(&q, ZSL_QUAT_TYPE_EMPTY);
	zsl_quat_init(&qn, ZSL_QUAT_TYPE_EMPTY);

	/* Normalising an empty quat == 0. */
	rc = zsl_quat_to_unit(&q, &qn);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(qn.r, 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(qn.i, 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(qn.j, 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(qn.k, 0.0, 1E-6), NULL);

	/* Normalise to unit quaternion. */
	q.r = 1.0;
	q.i = 0.75;
	q.j = 1.25;
	q.k = 0.50;
	rc = zsl_quat_to_unit(&q, &qn);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(qn.r, 0.5443311, 1E-6), NULL);
	zassert_true(val_is_equal(qn.i, 0.4082483, 1E-6), NULL);
	zassert_true(val_is_equal(qn.j, 0.6804138, 1E-6), NULL);
	zassert_true(val_is_equal(qn.k, 0.2721655, 1E-6), NULL);

	/* Verify qn = "unit quaternion", meaning sqrt(r^2+i^2+j^2+k^2) = 1.0. */
	zassert_true(zsl_quat_is_unit(&qn), NULL);
}

ZTEST(zsl_tests, test_quat_is_unit)
{
	struct zsl_quat q;

	zsl_quat_init(&q, ZSL_QUAT_TYPE_EMPTY);
	q.r = 0.5443311;
	q.i = 0.4082483;
	q.j = 0.6804138;
	q.k = 0.2721655;

	/* Verify qn = "unit quaternion", meaning sqrt(r^2+i^2+j^2+k^2) = 1.0. */
	zassert_true(zsl_quat_is_unit(&q), NULL);
}

ZTEST(zsl_tests, test_quat_scale)
{
	int rc;
	struct zsl_quat q = { .r = 1.0, .i = 0.5, .j = 0.25, .k = 2.0 };
	struct zsl_quat qs;

	zsl_quat_init(&qs, ZSL_QUAT_TYPE_EMPTY);

	rc = zsl_quat_scale(&q, 0.5, &qs);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(qs.r, q.r / 2.0, 1E-6), NULL);
	zassert_true(val_is_equal(qs.i, q.i / 2.0, 1E-6), NULL);
	zassert_true(val_is_equal(qs.j, q.j / 2.0, 1E-6), NULL);
	zassert_true(val_is_equal(qs.k, q.k / 2.0, 1E-6), NULL);
}

ZTEST(zsl_tests, test_quat_mult)
{
	int rc;
	struct zsl_quat qa = { .r = 1.0, .i = 0.25, .j = 0.5, .k = 0.75 };
	struct zsl_quat qb = { .r = 1.0, .i = 0.25, .j = 0.5, .k = 0.75 };
	struct zsl_quat qm;

	zsl_quat_init(&qm, ZSL_QUAT_TYPE_EMPTY);

	rc = zsl_quat_mult(&qa, &qb, &qm);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(qm.r, 0.125, 1E-6), NULL);
	zassert_true(val_is_equal(qm.i, 0.5, 1E-6), NULL);
	zassert_true(val_is_equal(qm.j, 1.0, 1E-6), NULL);
	zassert_true(val_is_equal(qm.k, 1.5, 1E-6), NULL);
}

ZTEST(zsl_tests, test_quat_exp)
{
	int rc;
	struct zsl_quat q = { .r = 5.0, .i = 1.6, .j = -2.1, .k = 123.0 };
	struct zsl_quat qe;

	zsl_quat_init(&qe, ZSL_QUAT_TYPE_EMPTY);

	rc = zsl_quat_exp(&q, &qe);
	zassert_true(rc == 0, NULL);
#ifdef CONFIG_ZSL_SINGLE_PRECISION
	zassert_true(val_is_equal(qe.r, -129.799958, 1E-3), NULL);
	zassert_true(val_is_equal(qe.i, -0.935867, 1E-4), NULL);
	zassert_true(val_is_equal(qe.j, 1.228326, 1E-4), NULL);
	zassert_true(val_is_equal(qe.k, -71.944785, 1E-3), NULL);
#else
	zassert_true(val_is_equal(qe.r, -129.799958, 1E-6), NULL);
	zassert_true(val_is_equal(qe.i, -0.935867, 1E-6), NULL);
	zassert_true(val_is_equal(qe.j, 1.228326, 1E-6), NULL);
	zassert_true(val_is_equal(qe.k, -71.944785, 1E-6), NULL);
#endif

}

ZTEST(zsl_tests, test_quat_log)
{
	int rc;
	struct zsl_quat q = { .r = 5.0, .i = 1.6, .j = -2.1, .k = 123.0 };
	struct zsl_quat ql;

	zsl_quat_init(&ql, ZSL_QUAT_TYPE_EMPTY);

	rc = zsl_quat_log(&q, &ql);
	zassert_true(rc == 0, NULL);

	zassert_true(val_is_equal(ql.r, 4.813240, 1E-6), NULL);
	zassert_true(val_is_equal(ql.i, 0.019900, 1E-6), NULL);
	zassert_true(val_is_equal(ql.j, -0.026119, 1E-6), NULL);
	zassert_true(val_is_equal(ql.k, 1.529825, 1E-6), NULL);
}

ZTEST(zsl_tests, test_quat_pow)
{
	int rc;
	struct zsl_quat q = { .r = 5.0, .i = 1.6, .j = -2.1, .k = 123.0 };
	struct zsl_quat qp;

	zsl_quat_init(&qp, ZSL_QUAT_TYPE_EMPTY);

	rc = zsl_quat_pow(&q, 1. / 3., &qp);
	zassert_true(rc == 0, NULL);

	zassert_true(val_is_equal(qp.r, 4.341708, 1E-6), NULL);
	zassert_true(val_is_equal(qp.i, 0.031588, 1E-6), NULL);
	zassert_true(val_is_equal(qp.j, -0.041460, 1E-6), NULL);
	zassert_true(val_is_equal(qp.k, 2.428350, 1E-6), NULL);

	rc = zsl_quat_pow(&q, 3., &qp);
	zassert_true(rc == 0, NULL);

#ifdef CONFIG_ZSL_SINGLE_PRECISION
	zassert_true(val_is_equal(qp.r, -226914.550000, 10), NULL);
	zassert_true(val_is_equal(qp.i, -24097.552000, 10), NULL);
	zassert_true(val_is_equal(qp.j, 31628.037000, 10), NULL);
	zassert_true(val_is_equal(qp.k, -1852499.310000, 10), NULL);
#else
	zassert_true(val_is_equal(qp.r, -226914.550000, 1E-3), NULL);
	zassert_true(val_is_equal(qp.i, -24097.552000, 1E-3), NULL);
	zassert_true(val_is_equal(qp.j, 31628.037000, 1E-3), NULL);
	zassert_true(val_is_equal(qp.k, -1852499.310000, 1E-3), NULL);
#endif
}

ZTEST(zsl_tests, test_quat_conj)
{
	int rc;
	struct zsl_quat q;
	struct zsl_quat qc;

	zsl_quat_init(&q, ZSL_QUAT_TYPE_EMPTY);
	zsl_quat_init(&qc, ZSL_QUAT_TYPE_EMPTY);

	q.r = 1.0;
	q.i = 0.333333;
	q.j = 0.333333;
	q.k = 0.333333;

	rc = zsl_quat_conj(&q, &qc);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(qc.r, 1.0, 1E-6), NULL);
	zassert_true(val_is_equal(qc.i, -0.333333, 1E-6), NULL);
	zassert_true(val_is_equal(qc.j, -0.333333, 1E-6), NULL);
	zassert_true(val_is_equal(qc.k, -0.333333, 1E-6), NULL);
}

ZTEST(zsl_tests, test_quat_inv)
{
	int rc;
	struct zsl_quat q;
	struct zsl_quat qi;

	zsl_quat_init(&q, ZSL_QUAT_TYPE_EMPTY);
	zsl_quat_init(&qi, ZSL_QUAT_TYPE_EMPTY);

	q.r = 1.0;
	q.i = 0.333333;
	q.j = 0.333333;
	q.k = 0.333333;

	rc = zsl_quat_inv(&q, &qi);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(qi.r, 0.7500004, 1E-6), NULL);
	zassert_true(val_is_equal(qi.i, -0.2499999, 1E-6), NULL);
	zassert_true(val_is_equal(qi.j, -0.2499999, 1E-6), NULL);
	zassert_true(val_is_equal(qi.k, -0.2499999, 1E-6), NULL);
}

ZTEST(zsl_tests, test_quat_diff)
{
	int rc;
	struct zsl_quat qa = { .r = 1.0, .i = 0.25, .j = 0.5, .k = 0.75 };
	struct zsl_quat qb = { .r = 1.0, .i = 0.5, .j = 0.5, .k = 0.5 };
	struct zsl_quat qd;
	struct zsl_quat qcomp;

	zsl_quat_init(&qd, ZSL_QUAT_TYPE_EMPTY);
	rc = zsl_quat_diff(&qa, &qb, &qd);
	zassert_true(rc == 0, NULL);

	/* qb = qa * qd if this is correct. */
	rc = zsl_quat_mult(&qa, &qd, &qcomp);
	zassert_true(val_is_equal(qb.r, qcomp.r, 1E-6), NULL);
	zassert_true(val_is_equal(qb.i, qcomp.i, 1E-6), NULL);
	zassert_true(val_is_equal(qb.j, qcomp.j, 1E-6), NULL);
	zassert_true(val_is_equal(qb.k, qcomp.k, 1E-6), NULL);
}

ZTEST(zsl_tests, test_quat_rot)
{
	int rc;
	struct zsl_quat qa = {
		.r = -0.936457,
		.i = -0.093751,
		.j = -0.281252,
		.k = -0.187502
	};
	struct zsl_quat qb = { .r = 0.0, .i = 1.6, .j = -2.1, .k = 123.0 };
	struct zsl_quat qr;

	zsl_quat_init(&qr, ZSL_QUAT_TYPE_EMPTY);

	rc = zsl_quat_rot(&qa, &qb, &qr);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(qr.r, 0.0, 1E-6), NULL);
#ifdef CONFIG_ZSL_SINGLE_PRECISION
	zassert_true(val_is_equal(qr.i, 70.976938, 1E-4), NULL);
	zassert_true(val_is_equal(qr.j, -9.893547, 1E-4), NULL);
	zassert_true(val_is_equal(qr.k, 100.001810, 1E-4), NULL);
#else
	zassert_true(val_is_equal(qr.i, 70.976938, 1E-6), NULL);
	zassert_true(val_is_equal(qr.j, -9.893547, 1E-6), NULL);
	zassert_true(val_is_equal(qr.k, 100.001810, 1E-6), NULL);
#endif

	/* The quaternion qb must be a pure quaternion (zero real part). */
	qb.r = 1.0;
	rc = zsl_quat_rot(&qa, &qb, &qr);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_quat_lerp)
{
	int rc;
	struct zsl_quat qa = { .r = 1.0, .i = 0.0, .j = 0.0, .k = 0.0 };
	struct zsl_quat qb = { .r = 0.0, .i = 1.0, .j = 0.0, .k = 0.0 };
	struct zsl_quat qi;

	/* Make sure inputs are unit quaternions. */
	zassert_true(zsl_quat_is_unit(&qa), NULL);
	zassert_true(zsl_quat_is_unit(&qb), NULL);

	/* t = 0.0 */
	zsl_quat_init(&qi, ZSL_QUAT_TYPE_EMPTY);
	rc = zsl_quat_lerp(&qa, &qb, 0.0, &qi);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(qi.r, qa.r, 1E-6), NULL);
	zassert_true(val_is_equal(qi.i, qa.i, 1E-6), NULL);
	zassert_true(val_is_equal(qi.j, qa.j, 1E-6), NULL);
	zassert_true(val_is_equal(qi.k, qa.k, 1E-6), NULL);

	/* t = 0.2 */
	zsl_quat_init(&qi, ZSL_QUAT_TYPE_EMPTY);
	rc = zsl_quat_lerp(&qa, &qb, 0.2, &qi);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(qi.r, 0.9701425001453318, 1E-6), NULL);
	zassert_true(val_is_equal(qi.i, 0.2425356250363329, 1E-6), NULL);
	zassert_true(val_is_equal(qi.j, 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(qi.k, 0.0, 1E-6), NULL);

	/* t = 0.8 */
	zsl_quat_init(&qi, ZSL_QUAT_TYPE_EMPTY);
	rc = zsl_quat_lerp(&qa, &qb, 0.8, &qi);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(qi.r, 0.2425356250363329, 1E-6), NULL);
	zassert_true(val_is_equal(qi.i, 0.9701425001453318, 1E-6), NULL);
	zassert_true(val_is_equal(qi.j, 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(qi.k, 0.0, 1E-6), NULL);

	/* t = 1.0 */
	zsl_quat_init(&qi, ZSL_QUAT_TYPE_EMPTY);
	rc = zsl_quat_lerp(&qa, &qb, 1.0, &qi);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(qi.r, qb.r, 1E-6), NULL);
	zassert_true(val_is_equal(qi.i, qb.i, 1E-6), NULL);
	zassert_true(val_is_equal(qi.j, qb.j, 1E-6), NULL);
	zassert_true(val_is_equal(qi.k, qb.k, 1E-6), NULL);

	/* t = 1.7. An error is expected. */
	zsl_quat_init(&qi, ZSL_QUAT_TYPE_EMPTY);
	rc = zsl_quat_lerp(&qa, &qb, 1.7, &qi);
	zassert_true(rc == -EINVAL, NULL);

	/* t = -1.1. An error is expected. */
	zsl_quat_init(&qi, ZSL_QUAT_TYPE_EMPTY);
	rc = zsl_quat_lerp(&qa, &qb, -1.1, &qi);
	zassert_true(rc == -EINVAL, NULL);

	/* Non unit quaternions. */
	qa.r = 3.0;
	qa.i = 5.0;
	qa.j = -1.3;
	qa.k = 0.0;

	qb.r = -2.0;
	qb.i = 9.0;
	qb.j = 0.6;
	qb.k = -2.9;

	zsl_quat_init(&qi, ZSL_QUAT_TYPE_EMPTY);
	rc = zsl_quat_lerp(&qa, &qb, 0.7, &qi);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(qi.r, 0.0065594554771123, 1E-6), NULL);
	zassert_true(val_is_equal(qi.i, 0.9737279382225760, 1E-6), NULL);
	zassert_true(val_is_equal(qi.j, -0.0236596309754489, 1E-6), NULL);
	zassert_true(val_is_equal(qi.k, -0.2263870529221157, 1E-6), NULL);

	/* qa = qb. */
	qb.r = 3.0;
	qb.i = 5.0;
	qb.j = -1.3;
	qb.k = 0.0;

	struct zsl_quat qa_u;

	zsl_quat_to_unit(&qa, &qa_u);

	zsl_quat_init(&qi, ZSL_QUAT_TYPE_EMPTY);
	rc = zsl_quat_lerp(&qa, &qb, 0.3, &qi);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(qi.r, qa_u.r, 1E-6), NULL);
	zassert_true(val_is_equal(qi.i, qa_u.i, 1E-6), NULL);
	zassert_true(val_is_equal(qi.j, qa_u.j, 1E-6), NULL);
	zassert_true(val_is_equal(qi.k, qa_u.k, 1E-6), NULL);

	/* qa = -qb. */
	qb.r *= -1.0;
	qb.i *= -1.0;
	qb.j *= -1.0;
	qb.k *= -1.0;

	zsl_quat_init(&qi, ZSL_QUAT_TYPE_EMPTY);
	rc = zsl_quat_lerp(&qa, &qb, 0.5, &qi);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(qi.r, 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(qi.i, 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(qi.j, 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(qi.k, 0.0, 1E-6), NULL);
}

ZTEST(zsl_tests, test_quat_slerp)
{
	int rc;
	struct zsl_quat qa = { .r = 1.0, .i = 0.0, .j = 0.0, .k = 0.0 };
	struct zsl_quat qb = { .r = 0.0, .i = 1.0, .j = 0.0, .k = 0.0 };
	struct zsl_quat qi;

	/* Make sure inputs are unit quaternions. */
	zassert_true(zsl_quat_is_unit(&qa), NULL);
	zassert_true(zsl_quat_is_unit(&qb), NULL);

	/* t = 0.0 */
	zsl_quat_init(&qi, ZSL_QUAT_TYPE_EMPTY);
	rc = zsl_quat_slerp(&qa, &qb, 0.0, &qi);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(qi.r, qa.r, 1E-6), NULL);
	zassert_true(val_is_equal(qi.i, qa.i, 1E-6), NULL);
	zassert_true(val_is_equal(qi.j, qa.j, 1E-6), NULL);
	zassert_true(val_is_equal(qi.k, qa.k, 1E-6), NULL);

	/* t = 0.2 */
	zsl_quat_init(&qi, ZSL_QUAT_TYPE_EMPTY);
	rc = zsl_quat_slerp(&qa, &qb, 0.2, &qi);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(qi.r, 0.951056516295154, 1E-6), NULL);
	zassert_true(val_is_equal(qi.i, 0.309016994374947, 1E-6), NULL);
	zassert_true(val_is_equal(qi.j, 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(qi.k, 0.0, 1E-6), NULL);

	/* t = 0.8 */
	zsl_quat_init(&qi, ZSL_QUAT_TYPE_EMPTY);
	rc = zsl_quat_slerp(&qa, &qb, 0.8, &qi);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(qi.r, 0.309016994374947, 1E-6), NULL);
	zassert_true(val_is_equal(qi.i, 0.951056516295154, 1E-6), NULL);
	zassert_true(val_is_equal(qi.j, 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(qi.k, 0.0, 1E-6), NULL);

	/* t = 1.0 */
	zsl_quat_init(&qi, ZSL_QUAT_TYPE_EMPTY);
	rc = zsl_quat_slerp(&qa, &qb, 1.0, &qi);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(qi.r, qb.r, 1E-6), NULL);
	zassert_true(val_is_equal(qi.i, qb.i, 1E-6), NULL);
	zassert_true(val_is_equal(qi.j, qb.j, 1E-6), NULL);
	zassert_true(val_is_equal(qi.k, qb.k, 1E-6), NULL);

	/* t = 1.7. An error is expected. */
	zsl_quat_init(&qi, ZSL_QUAT_TYPE_EMPTY);
	rc = zsl_quat_slerp(&qa, &qb, 1.7, &qi);
	zassert_true(rc == -EINVAL, NULL);

	/* t = -1.1. An error is expected. */
	zsl_quat_init(&qi, ZSL_QUAT_TYPE_EMPTY);
	rc = zsl_quat_slerp(&qa, &qb, -1.1, &qi);
	zassert_true(rc == -EINVAL, NULL);

	/* Non unit quaternions. */
	qa.r = 3.0;
	qa.i = 5.0;
	qa.j = -1.3;
	qa.k = 0.0;

	qb.r = -2.0;
	qb.i = 9.0;
	qb.j = 0.6;
	qb.k = -2.9;

	zsl_quat_init(&qi, ZSL_QUAT_TYPE_EMPTY);
	rc = zsl_quat_slerp(&qa, &qb, 0.7, &qi);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(qi.r, 0.0144930459747461, 1E-6), NULL);
	zassert_true(val_is_equal(qi.i, 0.9742497802658620, 1E-6), NULL);
	zassert_true(val_is_equal(qi.j, -0.0268229081092286, 1E-6), NULL);
	zassert_true(val_is_equal(qi.k, -0.2234006465318697, 1E-6), NULL);

	/* qa = qb. */
	qb.r = 3.0;
	qb.i = 5.0;
	qb.j = -1.3;
	qb.k = 0.0;

	struct zsl_quat qa_u;

	zsl_quat_to_unit(&qa, &qa_u);

	zsl_quat_init(&qi, ZSL_QUAT_TYPE_EMPTY);
	rc = zsl_quat_slerp(&qa, &qb, 0.3, &qi);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(qi.r, qa_u.r, 1E-6), NULL);
	zassert_true(val_is_equal(qi.i, qa_u.i, 1E-6), NULL);
	zassert_true(val_is_equal(qi.j, qa_u.j, 1E-6), NULL);
	zassert_true(val_is_equal(qi.k, qa_u.k, 1E-6), NULL);

	/* qa = -qb. An error is expected, impossible spherical interpolation.  */
	qb.r *= -1.0;
	qb.i *= -1.0;
	qb.j *= -1.0;
	qb.k *= -1.0;

	zsl_quat_init(&qi, ZSL_QUAT_TYPE_EMPTY);
	rc = zsl_quat_slerp(&qa, &qb, 0.3, &qi);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_quat_from_ang_vel)
{
	int rc;
	struct zsl_quat qin = { .r = 1.0, .i = 0.0, .j = 0.0, .k = 0.0 };
	struct zsl_quat qcomp = {
		.r = 0.9976899045387927,
		.i = 0.0074826742840409,
		.j = 0.0219491778998534,
		.k = -0.0638521538904827
	};
	struct zsl_quat qout;

	ZSL_VECTOR_DEF(w, 3);
	ZSL_VECTOR_DEF(w2, 7);
	ZSL_VECTOR_DEF(w3, 1);
	zsl_real_t a[3] = { 1.5, 4.4, -12.8 };
	zsl_real_t t = 0.01, t2 = -0.15;

	zsl_vec_from_arr(&w, a);

	/* Calcuate the orientation from the previous orientation, the time and the
	 * angular velocity. */
	rc = zsl_quat_from_ang_vel(&w, &qin, t, &qout);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(qout.r, qcomp.r, 1E-6), NULL);
	zassert_true(val_is_equal(qout.i, qcomp.i, 1E-6), NULL);
	zassert_true(val_is_equal(qout.j, qcomp.j, 1E-6), NULL);
	zassert_true(val_is_equal(qout.k, qcomp.k, 1E-6), NULL);

	/* In this case, an error is expected due to the invalid dimension of the
	 * angular veolcity vector. */
	rc = zsl_quat_from_ang_vel(&w2, &qin, t, &qout);
	zassert_true(rc == -EINVAL, NULL);

	/* In this case, an error is expected due to the invalid dimension of the
	 * angular veolcity vector. */
	rc = zsl_quat_from_ang_vel(&w3, &qin, t, &qout);
	zassert_true(rc == -EINVAL, NULL);

	/* In this case, an error is expected due to a negative time. */
	rc = zsl_quat_from_ang_vel(&w, &qin, t2, &qout);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_quat_from_ang_mom)
{
	int rc;
	struct zsl_quat qin = { .r = 1.0, .i = 0.0, .j = 0.0, .k = 0.0 };
	struct zsl_quat qcomp = {
		.r = 0.9976899045387927,
		.i = 0.0074826742840409,
		.j = 0.0219491778998534,
		.k = -0.0638521538904827
	};
	struct zsl_quat qout;

	ZSL_VECTOR_DEF(l, 3);
	ZSL_VECTOR_DEF(l2, 7);
	ZSL_VECTOR_DEF(l3, 1);
	zsl_real_t a[3] = { 4.5, 13.2, -38.4 };
	zsl_real_t t = 0.01, t2 = -0.15;
	zsl_real_t i = 3.0, i2 = 0.0, i3 = -3.0;

	zsl_vec_from_arr(&l, a);

	/* Calcuate the orientation from the previous orientation, the time and the
	 * angular momentum and inertia. */
	rc = zsl_quat_from_ang_mom(&l, &qin, &i, t, &qout);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(qout.r, qcomp.r, 1E-6), NULL);
	zassert_true(val_is_equal(qout.i, qcomp.i, 1E-6), NULL);
	zassert_true(val_is_equal(qout.j, qcomp.j, 1E-6), NULL);
	zassert_true(val_is_equal(qout.k, qcomp.k, 1E-6), NULL);

	/* In this case, an error is expected due to the invalid dimension of the
	 * angular momentum vector. */
	rc = zsl_quat_from_ang_mom(&l2, &qin, &i, t, &qout);
	zassert_true(rc == -EINVAL, NULL);

	/* In this case, an error is expected due to the invalid dimension of the
	 * angular momentum vector. */
	rc = zsl_quat_from_ang_mom(&l3, &qin, &i, t, &qout);
	zassert_true(rc == -EINVAL, NULL);

	/* In this case, an error is expected due to a negative time. */
	rc = zsl_quat_from_ang_mom(&l, &qin, &i, t2, &qout);
	zassert_true(rc == -EINVAL, NULL);

	/* In this case, an error is expected due to zero inertia. */
	rc = zsl_quat_from_ang_mom(&l, &qin, &i2, t, &qout);
	zassert_true(rc == -EINVAL, NULL);

	/* In this case, an error is expected due to a negative inertia. */
	rc = zsl_quat_from_ang_mom(&l, &qin, &i3, t, &qout);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_quat_to_euler)
{
	int rc;
	struct zsl_quat q = { .r = 5.0, .i = 1.6, .j = -2.1, .k = 123.0 };
	struct zsl_quat q2 = {
		.r = -0.294260,
		.i = 0.642970,
		.j = -0.294260,
		.k = 0.642970
	};
	struct zsl_euler e = { .x = 0.0, .y = 0.0, .z = 0.0 };

	rc = zsl_quat_to_euler(&q, &e);
	zassert_true(rc == 0, NULL);

	zassert_true(val_is_equal(e.x, 0.035148, 1E-6), NULL);
	zassert_true(val_is_equal(e.y, 0.024579, 1E-6), NULL);
	zassert_true(val_is_equal(e.z, 3.059905, 1E-6), NULL);

	/* Case in which gimbal lock ocurrs. */
	rc = zsl_quat_to_euler(&q2, &e);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(e.x, -2.283185, 1E-6), NULL);
#ifdef CONFIG_ZSL_SINGLE_PRECISION
	zassert_true(val_is_equal(e.y, 1.570796, 1E-3), NULL);
#else
	zassert_true(val_is_equal(e.y, 1.570796, 1E-6), NULL);
#endif
	zassert_true(val_is_equal(e.z, 0.0, 1E-6), NULL);
}

ZTEST(zsl_tests, test_quat_from_euler)
{
	int rc;
	struct zsl_quat q;
	struct zsl_euler e = { .x = 3.0, .y = 12.1, .z = -7.2 };
	struct zsl_euler e2 = { .x = 0.0, .y = 0.0, .z = 0.0 };
	struct zsl_euler e3 = { .x = 2.1, .y = ZSL_PI / 2.0, .z = 1.6 };

	zsl_quat_init(&q, ZSL_QUAT_TYPE_EMPTY);

	rc = zsl_quat_from_euler(&e, &q);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.040283, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.877536, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.414807, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.237157, 1E-6), NULL);

	rc = zsl_quat_from_euler(&e2, &q);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 1.0, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.0, 1E-6), NULL);

	/* Case in which gimbal lock ocurrs. */
	rc = zsl_quat_from_euler(&e3, &q);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, -0.1948717, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, 0.6797242, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.1948717, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.6797242, 1E-6), NULL);
}

ZTEST(zsl_tests, test_quat_to_rot_mtx)
{
	int rc = 0;
	struct zsl_quat q = {
		.r = 0.5443311,
		.i = 0.4082483,
		.j = 0.6804138,
		.k = 0.2721655
	};

	/* Rotation matrix contents */
	zsl_real_t data_cmp[9] = {
		-0.07407397629608892, 0.2592592572432197, 0.9629629710271213,
		0.8518518296753963, 0.5185185417029887, -0.07407416681195528,
		-0.5185185689195382, 0.8148148007025278, -0.2592592028101106
	};

	ZSL_MATRIX_DEF(merr, 2, 2);
	ZSL_MATRIX_DEF(rot, 3, 3);

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure inappropriately size matrices fail. */
	rc = zsl_quat_to_rot_mtx(&q, &merr);
	zassert_true(rc == -EINVAL, NULL);
#endif

	zsl_mtx_init(&rot, NULL);

	rc = zsl_quat_to_rot_mtx(&q, &rot);
	zassert_true(rc == 0, NULL);
	for (int i = 0; i < rot.sz_cols * rot.sz_rows; i++) {
		zassert_true(val_is_equal(rot.data[i], data_cmp[i], 1E-6), NULL);
	}
}

ZTEST(zsl_tests, test_quat_from_rot_mtx)
{
	int rc = 0;
	struct zsl_quat q;
	struct zsl_quat qcmp = {
		.r = 0.5443311,
		.i = 0.4082483,
		.j = 0.6804138,
		.k = 0.2721655
	};

	/* Rotation matrix contents */
	zsl_real_t data[9] = {
		-0.07407397629608892, 0.2592592572432197, 0.9629629710271213,
		0.8518518296753963, 0.5185185417029887, -0.07407416681195528,
		-0.5185185689195382, 0.8148148007025278, -0.2592592028101106
	};

	ZSL_MATRIX_DEF(merr, 2, 2);
	ZSL_MATRIX_DEF(rot, 3, 3);

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure inappropriately size matrices fail. */
	rc = zsl_quat_from_rot_mtx(&merr, &q);
	zassert_true(rc == -EINVAL, NULL);
#endif

	/* Populate rotation matrix with valid data. */
	rc = zsl_mtx_from_arr(&rot, data);
	zassert_true(rc == 0, NULL);

	/* Matrix to quat conversion. */
	rc = zsl_quat_from_rot_mtx(&rot, &q);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, qcmp.r, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, qcmp.i, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, qcmp.j, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, qcmp.k, 1E-6), NULL);
}

ZTEST(zsl_tests, test_quat_to_axis_angle)
{
	int rc;
	struct zsl_quat q = { .r = 5.0, .i = 1.6, .j = -2.1, .k = 123.0 };
	struct zsl_quat q2 = { .r = 1.0, .i = 0.0, .j = 0.0, .k = 0.0 };
	zsl_real_t b;

	ZSL_VECTOR_DEF(a, 3);
	ZSL_VECTOR_DEF(a2, 4);

	rc = zsl_vec_init(&a);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_init(&a2);
	zassert_true(rc == 0, NULL);

	rc = zsl_quat_to_axis_angle(&q, &a, &b);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(a.data[0], 0.013005, 1E-6), NULL);
	zassert_true(val_is_equal(a.data[1], -0.017069, 1E-6), NULL);
	zassert_true(val_is_equal(a.data[2], 0.999770, 1E-6), NULL);
	zassert_true(val_is_equal(b, 3.060355, 1E-6), NULL);

	rc = zsl_quat_to_axis_angle(&q2, &a, &b);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(a.data[0], 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(a.data[1], 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(a.data[2], 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(b, 0.0, 1E-6), NULL);

	/* Invalid axis vector size. */
	rc = zsl_quat_to_axis_angle(&q, &a2, &b);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_quat_from_axis_angle)
{
	int rc;
	struct zsl_quat q;
	zsl_real_t b = 7.0;

	ZSL_VECTOR_DEF(a, 3);
	ZSL_VECTOR_DEF(a2, 3);
	ZSL_VECTOR_DEF(a3, 4);
	zsl_real_t v[3] = { 1.0, 3.0, 2.0 };

	rc = zsl_vec_from_arr(&a, v);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_init(&a2);
	zassert_true(rc == 0, NULL);
	rc = zsl_vec_init(&a3);
	zassert_true(rc == 0, NULL);

	rc = zsl_quat_from_axis_angle(&a, &b, &q);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, -0.936457, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.093751, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.281252, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, -0.187502, 1E-6), NULL);

	rc = zsl_quat_from_axis_angle(&a2, &b, &q);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.0, 1E-6), NULL);

	/* Invalid axis vector size. */
	rc = zsl_quat_from_axis_angle(&a3, &b, &q);
	zassert_true(rc == -EINVAL, NULL);
}
