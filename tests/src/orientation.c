/*
 * Copyright (c) 2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ztest.h>
#include <zsl/zsl.h>
#include <zsl/orientation/orientation.h>
#include "floatcheck.h"

void test_att_to_vec(void)
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

void test_att_to_euler(void)
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

void test_att_from_euler(void)
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

void test_att_from_accelmag(void)
{

}

void test_att_from_accel(void)
{

}

void test_eul_to_vec(void)
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

void test_quat_init(void)
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

void test_quat_magn(void)
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

void test_quat_to_unit(void)
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

void test_quat_is_unit(void)
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

void test_quat_scale(void)
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

void test_quat_mult(void)
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

void test_quat_exp(void)
{
	/* TODO: Verify results! */
}

void test_quat_log(void)
{
	/* TODO: Verify results! */
}

void test_quat_pow(void)
{
	/* TODO: Verify results! */
}

void test_quat_conj(void)
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

void test_quat_inv(void)
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

void test_quat_diff(void)
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

void test_quat_slerp(void)
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
}

void test_quat_to_euler(void)
{
	/* TODO */
}

void test_quat_from_euler(void)
{
	// int rc = 0;
	// struct zsl_quat q;
	// struct zsl_quat qcmp = {
	// 	.r = 0.5443311,
	// 	.i = 0.4082483,
	// 	.j = 0.6804138,
	// 	.k = 0.2721655
	// };
	// struct zsl_euler e = {
	// 	.x = 2.8632926,	/* Degrees = 164.0545819. */
	// 	.y = 1.2977838,	/* Degrees = 74.3575337 */
	// 	.z = -1.8490956	/* Degrees = -105.9453765 */
	// };

	// rc = zsl_quat_from_euler(&e, &q);
	// zassert_true(rc == 0, NULL);
	// printk("r: %d\n", (int)(q.r * 1000000));
	// printk("i: %d\n", (int)(q.i * 1000000));
	// printk("j: %d\n", (int)(q.j * 1000000));
	// printk("k: %d\n", (int)(q.k * 1000000));
	// zassert_true(val_is_equal(q.r, qcmp.r, 1E-6), NULL);
	// zassert_true(val_is_equal(q.i, qcmp.i, 1E-6), NULL);
	// zassert_true(val_is_equal(q.j, qcmp.j, 1E-6), NULL);
	// zassert_true(val_is_equal(q.k, qcmp.k, 1E-6), NULL);
}

void test_quat_to_rot_mtx(void)
{
	int rc = 0;
	struct zsl_quat q = {
		.r = 0.5443311,
		.i = 0.4082483,
		.j = 0.6804138,
		.k = 0.2721655
	};

	/* Rotation matrix contents */
	zsl_real_t data_cmp[16] = {
		-0.07407397629608892, 0.2592592572432197, 0.9629629710271213, 0.0,
		0.8518518296753963, 0.5185185417029887, -0.07407416681195528, 0.0,
		-0.5185185689195382, 0.8148148007025278, -0.2592592028101106, 0.0,
		0.0, 0.0, 0.0, 1.0
	};

	ZSL_MATRIX_DEF(merr, 3, 3);
	ZSL_MATRIX_DEF(rot, 4, 4);

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

void test_quat_from_rot_mtx(void)
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
	zsl_real_t data[16] = {
		-0.07407397629608892, 0.2592592572432197, 0.9629629710271213, 0.0,
		0.8518518296753963, 0.5185185417029887, -0.07407416681195528, 0.0,
		-0.5185185689195382, 0.8148148007025278, -0.2592592028101106, 0.0,
		0.0, 0.0, 0.0, 1.0
	};

	ZSL_MATRIX_DEF(merr, 3, 3);
	ZSL_MATRIX_DEF(rot, 4, 4);

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure inappropriately size matrices fail. */
	rc = zsl_quat_from_rot_mtx(&merr, &q);
	zassert_true(rc == -EINVAL, NULL);
#endif

	/* Make sure invalid 4x4 matrix expansions fail (missing 1.0 at 3,3). */
	zsl_mtx_init(&rot, NULL);
	rc = zsl_quat_from_rot_mtx(&rot, &q);
	zassert_true(rc == -EINVAL, NULL);

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
