/*
 * Copyright (c) 2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ztest.h>
#include <zsl/zsl.h>
#include <zsl/quaternions.h>
#include "floatcheck.h"

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

	printk("QA QB Sources\n");
	printk("r: %d %d\n", (int)(qa.r * 1000000), (int)(qb.r * 1000000));
	printk("i: %d %d\n", (int)(qa.i * 1000000), (int)(qb.i * 1000000));
	printk("j: %d %d\n", (int)(qa.j * 1000000), (int)(qb.j * 1000000));
	printk("k: %d %d\n\n", (int)(qa.k * 1000000), (int)(qb.k * 1000000));

	/* t = 0.0 */
	zsl_quat_init(&qi, ZSL_QUAT_TYPE_EMPTY);
	rc = zsl_quat_slerp(&qa, &qb, 0.0, &qi);
	zassert_true(rc == 0, NULL);
	printk("slerp: 0.0\n");
	printk("r: %d\n", (int)(qi.r * 1000000));
	printk("i: %d\n", (int)(qi.i * 1000000));
	printk("j: %d\n", (int)(qi.j * 1000000));
	printk("k: %d\n\n", (int)(qi.k * 1000000));
	zassert_true(val_is_equal(qi.r, qa.r, 1E-6), NULL);
	zassert_true(val_is_equal(qi.i, qa.i, 1E-6), NULL);
	zassert_true(val_is_equal(qi.j, qa.j, 1E-6), NULL);
	zassert_true(val_is_equal(qi.k, qa.k, 1E-6), NULL);

	/* t = 0.2 */
	zsl_quat_init(&qi, ZSL_QUAT_TYPE_EMPTY);
	rc = zsl_quat_slerp(&qa, &qb, 0.2, &qi);
	zassert_true(rc == 0, NULL);
	printk("slerp: 0.2\n");
	printk("r: %d\n", (int)(qi.r * 1000000));
	printk("i: %d\n", (int)(qi.i * 1000000));
	printk("j: %d\n", (int)(qi.j * 1000000));
	printk("k: %d\n\n", (int)(qi.k * 1000000));
	zassert_true(val_is_equal(qi.r, 0.951056516295154, 1E-6), NULL);
	zassert_true(val_is_equal(qi.i, 0.309016994374947, 1E-6), NULL);
	zassert_true(val_is_equal(qi.j, 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(qi.k, 0.0, 1E-6), NULL);

	/* t = 0.8 */
	zsl_quat_init(&qi, ZSL_QUAT_TYPE_EMPTY);
	rc = zsl_quat_slerp(&qa, &qb, 0.8, &qi);
	zassert_true(rc == 0, NULL);
	printk("slerp: 0.8\n");
	printk("r: %d\n", (int)(qi.r * 1000000));
	printk("i: %d\n", (int)(qi.i * 1000000));
	printk("j: %d\n", (int)(qi.j * 1000000));
	printk("k: %d\n\n", (int)(qi.k * 1000000));
	zassert_true(val_is_equal(qi.r, 0.309016994374947, 1E-6), NULL);
	zassert_true(val_is_equal(qi.i, 0.951056516295154, 1E-6), NULL);
	zassert_true(val_is_equal(qi.j, 0.0, 1E-6), NULL);
	zassert_true(val_is_equal(qi.k, 0.0, 1E-6), NULL);

	/* t = 1.0 */
	zsl_quat_init(&qi, ZSL_QUAT_TYPE_EMPTY);
	rc = zsl_quat_slerp(&qa, &qb, 1.0, &qi);
	zassert_true(rc == 0, NULL);
	printk("slerp: 1.0\n");
	printk("r: %d\n", (int)(qi.r * 1000000));
	printk("i: %d\n", (int)(qi.i * 1000000));
	printk("j: %d\n", (int)(qi.j * 1000000));
	printk("k: %d\n\n", (int)(qi.k * 1000000));
	zassert_true(val_is_equal(qi.r, qb.r, 1E-6), NULL);
	zassert_true(val_is_equal(qi.i, qb.i, 1E-6), NULL);
	zassert_true(val_is_equal(qi.j, qb.j, 1E-6), NULL);
	zassert_true(val_is_equal(qi.k, qb.k, 1E-6), NULL);
}