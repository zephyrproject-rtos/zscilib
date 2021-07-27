/*
 * Copyright (c) 2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ztest.h>
#include <zsl/zsl.h>
#include <zsl/orientation/fusion/fusion.h>
#include "floatcheck.h"

void test_fus_madgwick(void)
{
	int rc = 0;

	struct zsl_fus_madg_cfg madg_cfg = {
		.beta = 0.7,
	};

	struct zsl_fus_drv madg_drv = {
		.init_handler = zsl_fus_madg_init,
		.feed_handler = zsl_fus_madg_feed,
		.error_handler = zsl_fus_madg_error,
		.config = &madg_cfg,
	};

	struct zsl_quat q = { .r = 1.0, .i = 0.0, .j = 0.0, .k = 0.0 };

	ZSL_VECTOR_DEF(a, 3);
	ZSL_VECTOR_DEF(m, 3);
	ZSL_VECTOR_DEF(g, 3);

	a.data[0] = 0.01;
	a.data[1] = -1.01;
	a.data[2] = -0.02;

	m.data[0] = -66.0;
	m.data[1] = -98.0;
	m.data[2] = -43.0;

	g.data[0] = 0.09;
	g.data[1] = -0.28;
	g.data[2] = -0.07;

	/* Init filter at 0 Hz. An error is expected. */
	rc = madg_drv.init_handler(0.0, madg_drv.config);
	zassert_true(rc == -EINVAL, NULL);
	
	/* Init filter at 100 Hz. */
	rc = madg_drv.init_handler(100.0, madg_drv.config);
	zassert_true(rc == 0, NULL);

	/* Run the Madgwick algorithm with magnetometer data. */
	rc = madg_drv.feed_handler(&a, &m, &g, &q, madg_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999735780642940, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.0039920127935612, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.0044945146835265, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.0040873395881737, 1E-6), NULL);

	/* Run the Madgwick algorithm without magnetometer data. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = madg_drv.feed_handler(&a, NULL, &g, &q, madg_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999774110335876, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.0065495169685490, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.0014692704230066, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, -0.0003499920938618, 1E-6), NULL);

	/* Run the Madgwick algorithm with invalid magnetometer data. */
	m.data[0] = 0.0;
	m.data[1] = 0.0;
	m.data[2] = 0.0;
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = madg_drv.feed_handler(&a, &m, &g, &q, madg_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999774110335876, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.0065495169685490, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.0014692704230066, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, -0.0003499920938618, 1E-6), NULL);

	ZSL_VECTOR_DEF(a2, 5);
	ZSL_VECTOR_DEF(m2, 5);
	ZSL_VECTOR_DEF(g2, 5);

	m.data[0] = -66.0;
	m.data[1] = -98.0;
	m.data[2] = -43.0;

	/* Run the Madgwick algorithm with invalid sensor data vector dimensions. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = madg_drv.feed_handler(&a2, &m, &g, &q, madg_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = madg_drv.feed_handler(&a, &m2, &g, &q, madg_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = madg_drv.feed_handler(&a, &m, &g2, &q, madg_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Run the Madgwick algorithm with a zero input quaternion. An error is
	 * expected, because it can't be normalized. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_EMPTY);
	rc = madg_drv.feed_handler(&a, &m, &g, &q, madg_drv.config);
	zassert_true(rc == -EINVAL, NULL);
}

void test_fus_mahony(void)
{
	int rc = 0;

	zsl_real_t _mahn_intfb[3] = {0.0, 0.0, 0.0};
	struct zsl_fus_mahn_cfg mahn_cfg = {
		.kp = 2.0,
		.ki = 0.005,
		.intfb = {
			.sz = 3,
			.data = _mahn_intfb,
		},
	};

	struct zsl_fus_drv mahn_drv = {
		.init_handler = zsl_fus_mahn_init,
		.feed_handler = zsl_fus_mahn_feed,
		.error_handler = zsl_fus_mahn_error,
		.config = &mahn_cfg,
	};

	struct zsl_quat q = { .r = 1.0, .i = 0.0, .j = 0.0, .k = 0.0 };

	ZSL_VECTOR_DEF(a, 3);
	ZSL_VECTOR_DEF(m, 3);
	ZSL_VECTOR_DEF(g, 3);

	a.data[0] = 0.01;
	a.data[1] = -1.01;
	a.data[2] = -0.02;

	m.data[0] = -66.0;
	m.data[1] = -98.0;
	m.data[2] = -43.0;

	g.data[0] = 0.09;
	g.data[1] = -0.28;
	g.data[2] = -0.07;

	/* Init filter at 0 Hz. An error is expected. */
	rc = mahn_drv.init_handler(0.0, mahn_drv.config);
	zassert_true(rc == -EINVAL, NULL);
	
	/* Init filter at 100 Hz. */
	rc = mahn_drv.init_handler(100.0, mahn_drv.config);
	zassert_true(rc == 0, NULL);

	/* Run the Mahony algorithm with magnetometer data. */
	rc = mahn_drv.feed_handler(&a, &m, &g, &q, mahn_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999308295647051, 1E-4), NULL);
	zassert_true(val_is_equal(q.i, -0.0068815222147659, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.0065074388222656, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.0069738065661901, 1E-6), NULL);

	/* Run the Mahony algorithm without magnetometer data. */
	a.data[0] = 0.01;
	a.data[1] = -1.01;
	a.data[2] = -0.02;

	m.data[0] = -66.0;
	m.data[1] = -98.0;
	m.data[2] = -43.0;

	g.data[0] = 0.09;
	g.data[1] = -0.28;
	g.data[2] = -0.07;

	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = mahn_drv.feed_handler(&a, NULL, &g, &q, mahn_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999532404548704, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.0095471287140318, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.0014989158007184, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, -0.0003499836341592, 1E-6), NULL);

	/* Run the Mahony algorithm with invalid magnetometer data. */
	a.data[0] = 0.01;
	a.data[1] = -1.01;
	a.data[2] = -0.02;

	m.data[0] = 0.0;
	m.data[1] = 0.0;
	m.data[2] = 0.0;

	g.data[0] = 0.09;
	g.data[1] = -0.28;
	g.data[2] = -0.07;
	
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = mahn_drv.feed_handler(&a, &m, &g, &q, mahn_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999532404548704, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.0095471287140318, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.0014989158007184, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, -0.0003499836341592, 1E-6), NULL);

	ZSL_VECTOR_DEF(a2, 5);
	ZSL_VECTOR_DEF(m2, 5);
	ZSL_VECTOR_DEF(g2, 5);

	m.data[0] = -66.0;
	m.data[1] = -98.0;
	m.data[2] = -43.0;

	/* Run the Mahony algorithm with invalid sensor data vector dimensions. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = mahn_drv.feed_handler(&a2, &m, &g, &q, mahn_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = mahn_drv.feed_handler(&a, &m2, &g, &q, mahn_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = mahn_drv.feed_handler(&a, &m, &g2, &q, mahn_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Run the Mahony algorithm with a zero input quaternion. An error is
	 * expected, because it can't be normalized. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_EMPTY);
	rc = mahn_drv.feed_handler(&a, &m, &g, &q, mahn_drv.config);
	zassert_true(rc == -EINVAL, NULL);
}

void test_fus_saam(void)
{
	int rc = 0;

	struct zsl_fus_drv saam_drv = {
		.init_handler = zsl_fus_saam_init,
		.feed_handler = zsl_fus_saam_feed,
		.error_handler = zsl_fus_saam_error,
	};

	struct zsl_quat q = { .r = 1.0, .i = 0.0, .j = 0.0, .k = 0.0 };

	ZSL_VECTOR_DEF(a, 3);
	ZSL_VECTOR_DEF(m, 3);
	ZSL_VECTOR_DEF(g, 3);

	a.data[0] = 0.01;
	a.data[1] = -1.01;
	a.data[2] = -0.02;

	m.data[0] = -66.0;
	m.data[1] = -98.0;
	m.data[2] = -43.0;

	/* Init filter at 0 Hz. An error is expected. */
	rc = saam_drv.init_handler(0.0, saam_drv.config);
	zassert_true(rc == -EINVAL, NULL);
	
	/* Init filter at 100 Hz. */
	rc = saam_drv.init_handler(100.0, saam_drv.config);
	zassert_true(rc == 0, NULL);

	/* Run the SAAM algorithm. */
	rc = saam_drv.feed_handler(&a, &m, &g, &q, saam_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.1934598725297102, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.1905244514984266, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.6881852932869889, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.6728110531237675, 1E-6), NULL);

	/* Run the SAAM algorithm without gyroscope data. It should give the same
	 * results. */
	rc = saam_drv.feed_handler(&a, &m, NULL, &q, saam_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.1934598725297102, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.1905244514984266, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.6881852932869889, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.6728110531237675, 1E-6), NULL);

	/* Run the SAAM algorithm without magnetometer data. An error is
	 * expected. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = saam_drv.feed_handler(&a, NULL, &g, &q, saam_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Run the SAAM algorithm without accelerometer data. An error is
	 * expected. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = saam_drv.feed_handler(NULL, &m, &g, &q, saam_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Run the SAAM algorithm with all zero magnetometer data. An error is
	 * expected. */
	m.data[0] = 0.0;
	m.data[1] = 0.0;
	m.data[2] = 0.0;

	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = saam_drv.feed_handler(&a, &m, &g, &q, saam_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Run the SAAM algorithm with all zero magnetometer data. An error is
	 * expected. */
	a.data[0] = 0.0;
	a.data[1] = 0.0;
	a.data[2] = 0.0;

	m.data[0] = -66.0;
	m.data[1] = -98.0;
	m.data[2] = -43.0;

	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = saam_drv.feed_handler(&a, &m, &g, &q, saam_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	ZSL_VECTOR_DEF(a2, 5);
	ZSL_VECTOR_DEF(m2, 5);

	m.data[0] = -66.0;
	m.data[1] = -98.0;
	m.data[2] = -43.0;

	/* Run the SAAM algorithm with invalid sensor data vector dimensions. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = saam_drv.feed_handler(&a2, &m, &g, &q, saam_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = saam_drv.feed_handler(&a, &m2, &g, &q, saam_drv.config);
	zassert_true(rc == -EINVAL, NULL);
}

void test_fus_complementary(void)
{
	int rc = 0;

	struct zsl_fus_comp_cfg comp_cfg = {
		.alpha = 0.7,
	};

	struct zsl_fus_drv comp_drv = {
		.init_handler = zsl_fus_comp_init,
		.feed_handler = zsl_fus_comp_feed,
		.error_handler = zsl_fus_comp_error,
		.config = &comp_cfg,
	};

	struct zsl_quat q = { .r = 1.0, .i = 0.0, .j = 0.0, .k = 0.0 };

	ZSL_VECTOR_DEF(a, 3);
	ZSL_VECTOR_DEF(m, 3);
	ZSL_VECTOR_DEF(g, 3);

	a.data[0] = 0.01;
	a.data[1] = -1.01;
	a.data[2] = -0.02;

	m.data[0] = -66.0;
	m.data[1] = -98.0;
	m.data[2] = -43.0;

	g.data[0] = 0.09;
	g.data[1] = -0.28;
	g.data[2] = -0.07;

	/* Init filter at 0 Hz. An error is expected. */
	rc = comp_drv.init_handler(0.0, comp_drv.config);
	zassert_true(rc == -EINVAL, NULL);
	
	/* Init filter at 100 Hz. */
	rc = comp_drv.init_handler(100.0, comp_drv.config);
	zassert_true(rc == 0, NULL);

	/* Run the full complementary algorithm (with magnetometer and
	 * accelerometer data). */
	rc = comp_drv.feed_handler(&a, &m, &g, &q, comp_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.7806422574656674, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.4621974380629751, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, 0.2979802370474076, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.2969494442427867, 1E-6), NULL);

	/* Run the complementary algorithm without magnetometer data. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = comp_drv.feed_handler(&a, NULL, &g, &q, comp_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999988575019579, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, 0.0004499994858759, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.0013999984005027, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, -0.0003499996001257, 1E-6), NULL);

	/* Run the complementary algorithm with invalid magnetometer data. */
	m.data[0] = 0.0;
	m.data[1] = 0.0;
	m.data[2] = 0.0;
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = comp_drv.feed_handler(&a, &m, &g, &q, comp_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999988575019579, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, 0.0004499994858759, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.0013999984005027, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, -0.0003499996001257, 1E-6), NULL);

	/* Run the complementary algorithm without accelerometer data. */
	m.data[0] = -66.0;
	m.data[1] = -98.0;
	m.data[2] = -43.0;
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = comp_drv.feed_handler(NULL, &m, &g, &q, comp_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999988575019579, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, 0.0004499994858759, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.0013999984005027, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, -0.0003499996001257, 1E-6), NULL);

	/* Run the complementary algorithm with invalid accelerometer data. */
	a.data[0] = 0.0;
	a.data[1] = 0.0;
	a.data[2] = 0.0;
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = comp_drv.feed_handler(&a, &m, &g, &q, comp_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999988575019579, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, 0.0004499994858759, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.0013999984005027, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, -0.0003499996001257, 1E-6), NULL);

	ZSL_VECTOR_DEF(a2, 5);
	ZSL_VECTOR_DEF(m2, 5);

	a.data[0] = 0.01;
	a.data[1] = -1.01;
	a.data[2] = -0.02;

	/* Run the complementary algorithm with invalid sensor data vector
	 * dimensions. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = comp_drv.feed_handler(&a2, &m, &g, &q, comp_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = comp_drv.feed_handler(&a, &m2, &g, &q, comp_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Run the complementary algorithm with a zero input quaternion. An error
	 * is expected, because it can't be normalized. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_EMPTY);
	rc = comp_drv.feed_handler(&a, &m, &g, &q, comp_drv.config);
	zassert_true(rc == -EINVAL, NULL);
}

void test_fus_aqua(void)
{
	int rc = 0;

	struct zsl_fus_aqua_cfg aqua_cfg = {
		.alpha = 0.7,
		.beta = 0.7,
		.e_a = 0.9,
		.e_m = 0.9,
	};

	struct zsl_fus_drv aqua_drv = {
		.init_handler = zsl_fus_aqua_init,
		.feed_handler = zsl_fus_aqua_feed,
		.error_handler = zsl_fus_aqua_error,
		.config = &aqua_cfg,
	};

	struct zsl_quat q = { .r = 1.0, .i = 0.0, .j = 0.0, .k = 0.0 };

	ZSL_VECTOR_DEF(a, 3);
	ZSL_VECTOR_DEF(m, 3);
	ZSL_VECTOR_DEF(g, 3);

	a.data[0] = 0.01;
	a.data[1] = -1.01;
	a.data[2] = -0.02;

	m.data[0] = -66.0;
	m.data[1] = -98.0;
	m.data[2] = -43.0;

	g.data[0] = 0.09;
	g.data[1] = -0.28;
	g.data[2] = -0.07;

	/* Init filter at 0 Hz. An error is expected. */
	rc = aqua_drv.init_handler(0.0, aqua_drv.config);
	zassert_true(rc == -EINVAL, NULL);
	
	/* Init filter at 100 Hz. */
	rc = aqua_drv.init_handler(100.0, aqua_drv.config);
	zassert_true(rc == 0, NULL);

	/* Run the full AQUA algorithm (with magnetometer and accelerometer
	 * data). */
	rc = aqua_drv.feed_handler(&a, &m, &g, &q, aqua_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, -0.0515281078976572, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.0450027755826364, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.7123466379237229, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.6984854126788296, 1E-6), NULL);

	/* Run the AQUA algorithm without accelerometer data. */
	a.data[0] = 0.01;
	a.data[1] = -1.01;
	a.data[2] = -0.02;

	m.data[0] = -66.0;
	m.data[1] = -98.0;
	m.data[2] = -43.0;

	g.data[0] = 0.09;
	g.data[1] = -0.28;
	g.data[2] = -0.07;
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = aqua_drv.feed_handler(NULL, &m, &g, &q, aqua_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999988575019580, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.0004499994858759, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, 0.0013999984005027, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.0003499996001257, 1E-6), NULL);

	/* Run the AQUA algorithm with invalid accelerometer data. */
	a.data[0] = 0.0;
	a.data[1] = 0.0;
	a.data[2] = 0.0;

	m.data[0] = -66.0;
	m.data[1] = -98.0;
	m.data[2] = -43.0;

	g.data[0] = 0.09;
	g.data[1] = -0.28;
	g.data[2] = -0.07;
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = aqua_drv.feed_handler(&a, &m, &g, &q, aqua_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999988575019580, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.0004499994858759, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, 0.0013999984005027, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.0003499996001257, 1E-6), NULL);
	
	/* Run the AQUA algorithm without magnetometer data. */
	a.data[0] = 0.01;
	a.data[1] = -1.01;
	a.data[2] = -0.02;

	m.data[0] = -66.0;
	m.data[1] = -98.0;
	m.data[2] = -43.0;

	g.data[0] = 0.09;
	g.data[1] = -0.28;
	g.data[2] = -0.07;
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = aqua_drv.feed_handler(&a, NULL, &g, &q, aqua_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999988575019580, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.0004499994858759, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, 0.0013999984005027, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.0003499996001257, 1E-6), NULL);

	/* Run the AQUA algorithm with invalid magnetometer data. */
	a.data[0] = 0.01;
	a.data[1] = -1.01;
	a.data[2] = -0.02;
	
	m.data[0] = 0.0;
	m.data[1] = 0.0;
	m.data[2] = 0.0;

	g.data[0] = 0.09;
	g.data[1] = -0.28;
	g.data[2] = -0.07;
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = aqua_drv.feed_handler(&a, &m, &g, &q, aqua_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999988575019580, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.0004499994858759, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, 0.0013999984005027, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.0003499996001257, 1E-6), NULL);

	ZSL_VECTOR_DEF(a2, 5);
	ZSL_VECTOR_DEF(m2, 5);
	ZSL_VECTOR_DEF(g2, 5);

	m.data[0] = -66.0;
	m.data[1] = -98.0;
	m.data[2] = -43.0;

	/* Run the AQUA algorithm with invalid sensor data vector dimensions. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = aqua_drv.feed_handler(&a2, &m, &g, &q, aqua_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = aqua_drv.feed_handler(&a, &m2, &g, &q, aqua_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = aqua_drv.feed_handler(&a, &m, &g2, &q, aqua_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	// /* Run the AQUA algorithm with a zero input quaternion. An error is
	//  * expected, because it can't be normalized. */
	// zsl_quat_init(&q, ZSL_QUAT_TYPE_EMPTY);
	// rc = aqua_drv.feed_handler(&a, &m, &g, &q);
	// zassert_true(rc == -EINVAL, NULL);
}

void test_fus_kalman(void)
{
	// int rc = 0;

	// struct zsl_fus_kalm_cfg kalm_cfg = {
	// 	.var_g = 0.01,
	// };

	// struct zsl_fus_drv kalm_drv = {
	// 	.init_handler = zsl_fus_kalm_init,
	// 	.feed_handler = zsl_fus_kalm_feed,
	// 	.error_handler = zsl_fus_kalm_error,
	// 	.config = &kalm_cfg,
	// };

	// struct zsl_quat q = { .r = 1.0, .i = 0.0, .j = 0.0, .k = 0.0 };

	// ZSL_VECTOR_DEF(a, 3);
	// ZSL_VECTOR_DEF(m, 3);
	// ZSL_VECTOR_DEF(g, 3);

	// a.data[0] = 0.01;
	// a.data[1] = -1.01;
	// a.data[2] = -0.02;

	// m.data[0] = -66.0;
	// m.data[1] = -98.0;
	// m.data[2] = -43.0;

	// g.data[0] = 0.09;
	// g.data[1] = -0.28;
	// g.data[2] = -0.07;

	// /* Init filter at 0 Hz. An error is expected. */
	// rc = kalm_drv.init_handler(0.0, kalm_drv.config);
	// zassert_true(rc == -EINVAL, NULL);
	
	// /* Init filter at 100 Hz. */
	// rc = kalm_drv.init_handler(100.0, kalm_drv.config);
	// zassert_true(rc == 0, NULL);

	// /* Run the kalman algorithm with magnetometer data. */
	// rc = kalm_drv.feed_handler(&a, &m, &g, &q, kalm_drv.config);
	// zassert_true(rc == 0, NULL);
	// zassert_true(val_is_equal(q.r, , 1E-6), NULL);
	// zassert_true(val_is_equal(q.i, , 1E-6), NULL);
	// zassert_true(val_is_equal(q.j, , 1E-6), NULL);
	// zassert_true(val_is_equal(q.k, , 1E-6), NULL);

	// /* Run the kalman algorithm without magnetometer data. */
	// zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	// rc = kalm_drv.feed_handler(&a, NULL, &g, &q, kalm_drv.config);
	// zassert_true(rc == 0, NULL);
	// zassert_true(val_is_equal(q.r, , 1E-6), NULL);
	// zassert_true(val_is_equal(q.i, , 1E-6), NULL);
	// zassert_true(val_is_equal(q.j, , 1E-6), NULL);
	// zassert_true(val_is_equal(q.k, , 1E-6), NULL);

	// /* Run the kalman algorithm with invalid magnetometer data. */
	// m.data[0] = 0.0;
	// m.data[1] = 0.0;
	// m.data[2] = 0.0;
	// zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	// rc = kalm_drv.feed_handler(&a, &m, &g, &q, kalm_drv.config);
	// zassert_true(rc == 0, NULL);
	// zassert_true(val_is_equal(q.r, , 1E-6), NULL);
	// zassert_true(val_is_equal(q.i, , 1E-6), NULL);
	// zassert_true(val_is_equal(q.j, , 1E-6), NULL);
	// zassert_true(val_is_equal(q.k, , 1E-6), NULL);

	// ZSL_VECTOR_DEF(a2, 5);
	// ZSL_VECTOR_DEF(m2, 5);
	// ZSL_VECTOR_DEF(g2, 5);

	// m.data[0] = -66.0;
	// m.data[1] = -98.0;
	// m.data[2] = -43.0;

	// /* Run the kalman algorithm with invalid sensor data vector dimensions. */
	// zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	// rc = kalm_drv.feed_handler(&a2, &m, &g, &q, kalm_drv.config);
	// zassert_true(rc == -EINVAL, NULL);

	// zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	// rc = kalm_drv.feed_handler(&a, &m2, &g, &q, kalm_drv.config);
	// zassert_true(rc == -EINVAL, NULL);

	// zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	// rc = kalm_drv.feed_handler(&a, &m, &g2, &q, kalm_drv.config);
	// zassert_true(rc == -EINVAL, NULL);

	// /* Run the kalman algorithm with a zero input quaternion. An error is
	//  * expected, because it can't be normalized. */
	// zsl_quat_init(&q, ZSL_QUAT_TYPE_EMPTY);
	// rc = kalm_drv.feed_handler(&a, &m, &g, &q, kalm_drv.config);
	// zassert_true(rc == -EINVAL, NULL);
}
