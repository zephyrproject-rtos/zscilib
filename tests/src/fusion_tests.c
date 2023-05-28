/*
 * Copyright (c) 2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zsl/zsl.h>
#include <zsl/orientation/fusion/fusion.h>
#include "floatcheck.h"

ZTEST(zsl_tests, test_fus_madgwick)
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

	zsl_real_t incl = 4.0 / 3.0;

	/* Init filter at 0 Hz. An error is expected. */
	rc = madg_drv.init_handler(0.0, madg_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Init filter at 100 Hz. */
	rc = madg_drv.init_handler(100.0, madg_drv.config);
	zassert_true(rc == 0, NULL);

	/* Run the Madgwick algorithm. */
	rc = madg_drv.feed_handler(&a, &m, &g, NULL, &q, madg_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999735780642940, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.0039920127935612, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.0044945146835265, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.0040873395881737, 1E-6), NULL);

	/* Run the Madgwick algorithm with inclination angle provided. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = madg_drv.feed_handler(&a, &m, &g, &incl, &q, madg_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999757062543757, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.0049228858339429, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.0031924380087654, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.0037630354354918, 1E-6), NULL);

	/* Run the Madgwick algorithm without accelerometer data. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = madg_drv.feed_handler(NULL, &m, &g, NULL, &q, madg_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999988575019580, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, 0.0004499994858759, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.0013999984005027, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, -0.0003499996001257, 1E-6), NULL);

	/* Run the Madgwick algorithm without magnetometer data. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = madg_drv.feed_handler(&a, NULL, &g, NULL, &q, madg_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999774110335876, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.0065495169685490, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.0014692704230066, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, -0.0003499920938618, 1E-6), NULL);

	/* Run the Madgwick algorithm without gyroscope data. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = madg_drv.feed_handler(&a, &m, NULL, NULL, &q, madg_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Run the Madgwick algorithm with invalid accelerometer data. */
	a.data[0] = 0.0;
	a.data[1] = 0.0;
	a.data[2] = 0.0;
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = madg_drv.feed_handler(&a, &m, &g, NULL, &q, madg_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999988575019580, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, 0.0004499994858759, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.0013999984005027, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, -0.0003499996001257, 1E-6), NULL);

	/* Run the Madgwick algorithm with invalid magnetometer data. */
	a.data[0] = 0.01;
	a.data[1] = -1.01;
	a.data[2] = -0.02;
	m.data[0] = 0.0;
	m.data[1] = 0.0;
	m.data[2] = 0.0;
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = madg_drv.feed_handler(&a, &m, &g, NULL, &q, madg_drv.config);
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

	/* Run the Madgwick algorithm with invalid value of beta. */
	madg_cfg.beta = -0.1;
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = madg_drv.feed_handler(&a, &m, &g, NULL, &q, madg_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Run the Madgwick algorithm with invalid sensor data vector dimensions. */
	madg_cfg.beta = 0.7;
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = madg_drv.feed_handler(&a2, &m, &g, NULL, &q, madg_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = madg_drv.feed_handler(&a, &m2, &g, NULL, &q, madg_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = madg_drv.feed_handler(&a, &m, &g2, NULL, &q, madg_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Run the Madgwick algorithm with a zero input quaternion. An error is
	 * expected, because it can't be normalized. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_EMPTY);
	rc = madg_drv.feed_handler(&a, &m, &g, NULL, &q, madg_drv.config);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_fus_mahony)
{
	int rc = 0;

	zsl_real_t _mahn_intfb[3] = { 0.0, 0.0, 0.0 };
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

	zsl_real_t incl = 4.0 / 3.0;

	/* Init filter at 0 Hz. An error is expected. */
	rc = mahn_drv.init_handler(0.0, mahn_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Init filter at 100 Hz. */
	rc = mahn_drv.init_handler(100.0, mahn_drv.config);
	zassert_true(rc == 0, NULL);

	/* Run the Mahony algorithm. */
	rc = mahn_drv.feed_handler(&a, &m, &g, NULL, &q, mahn_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999308295647051, 1E-4), NULL);
	zassert_true(val_is_equal(q.i, -0.0068815222147659, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.0065074388222656, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.0069738065661901, 1E-6), NULL);

	/* Run the Mahony algorithm with inclination angle provided. */
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
	rc = mahn_drv.feed_handler(&a, &m, &g, &incl, &q, mahn_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999134883142473, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.0097283268194340, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.0047955090791204, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.0074416824173269, 1E-6), NULL);

	/* Run the Mahony algorithm without accelerometer data. */
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
	rc = mahn_drv.feed_handler(NULL, &m, &g, NULL, &q, mahn_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999988575019580, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, 0.0004499994858759, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.0013999984005027, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, -0.0003499996001257, 1E-6), NULL);

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
	rc = mahn_drv.feed_handler(&a, NULL, &g, NULL, &q, mahn_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999532404548704, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.0095471287140318, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.0014989158007184, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, -0.0003499836341592, 1E-6), NULL);

	/* Run the Mahony algorithm without gyroscope data. */
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
	rc = mahn_drv.feed_handler(&a, &m, NULL, NULL, &q, mahn_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Run the Mahony algorithm with invalid accelerometer data. */
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
	rc = mahn_drv.feed_handler(&a, &m, &g, NULL, &q, mahn_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999988575019580, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, 0.0004499994858759, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.0013999984005027, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, -0.0003499996001257, 1E-6), NULL);

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
	rc = mahn_drv.feed_handler(&a, &m, &g, NULL, &q, mahn_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999532404548704, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.0095471287140318, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.0014989158007184, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, -0.0003499836341592, 1E-6), NULL);

	a.data[0] = 0.01;
	a.data[1] = -1.01;
	a.data[2] = -0.02;

	m.data[0] = -66.0;
	m.data[1] = -98.0;
	m.data[2] = -43.0;

	g.data[0] = 0.09;
	g.data[1] = -0.28;
	g.data[2] = -0.07;

	ZSL_VECTOR_DEF(a2, 5);
	ZSL_VECTOR_DEF(m2, 5);
	ZSL_VECTOR_DEF(g2, 5);

	/* Run the Mahony algorithm with invalid ki and kp. */
	mahn_cfg.ki = -0.1;
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = mahn_drv.feed_handler(&a, &m, &g, NULL, &q, mahn_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	mahn_cfg.ki = 0.005;
	mahn_cfg.kp = -0.1;
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = mahn_drv.feed_handler(&a, &m, &g, NULL, &q, mahn_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Run the Mahony algorithm with invalid sensor data vector dimensions. */
	mahn_cfg.kp = 2.0;
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = mahn_drv.feed_handler(&a2, &m, &g, NULL, &q, mahn_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = mahn_drv.feed_handler(&a, &m2, &g, NULL, &q, mahn_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = mahn_drv.feed_handler(&a, &m, &g2, NULL, &q, mahn_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Run the Mahony algorithm with a zero input quaternion. An error is
	 * expected, because it can't be normalized. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_EMPTY);
	rc = mahn_drv.feed_handler(&a, &m, &g, NULL, &q, mahn_drv.config);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_fus_saam)
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

	g.data[0] = 0.09;
	g.data[1] = -0.28;
	g.data[2] = -0.07;

	zsl_real_t incl = 4.0 / 3.0;

	/* Init filter at 0 Hz. An error is expected. */
	rc = saam_drv.init_handler(0.0, saam_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Init filter at 100 Hz. */
	rc = saam_drv.init_handler(100.0, saam_drv.config);
	zassert_true(rc == 0, NULL);

	/* Run the SAAM algorithm. */
	rc = saam_drv.feed_handler(&a, &m, &g, NULL, &q, saam_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.1934598725297102, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.1905244514984266, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.6881852932869889, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.6728110531237675, 1E-6), NULL);

	/* Run the SAAM algorithm with inclination angle provided. It should give
	 * the same results. */
	rc = saam_drv.feed_handler(&a, &m, &g, &incl, &q, saam_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.1934598725297102, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.1905244514984266, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.6881852932869889, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.6728110531237675, 1E-6), NULL);

	/* Run the SAAM algorithm without gyroscope data. It should give the same
	 * results. */
	rc = saam_drv.feed_handler(&a, &m, NULL, NULL, &q, saam_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.1934598725297102, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.1905244514984266, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.6881852932869889, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.6728110531237675, 1E-6), NULL);

	/* Run the SAAM algorithm without magnetometer data. An error is
	 * expected. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = saam_drv.feed_handler(&a, NULL, &g, NULL, &q, saam_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Run the SAAM algorithm without accelerometer data. An error is
	 * expected. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = saam_drv.feed_handler(NULL, &m, &g, NULL, &q, saam_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Run the SAAM algorithm with all zero magnetometer data. An error is
	 * expected. */
	m.data[0] = 0.0;
	m.data[1] = 0.0;
	m.data[2] = 0.0;

	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = saam_drv.feed_handler(&a, &m, &g, NULL, &q, saam_drv.config);
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
	rc = saam_drv.feed_handler(&a, &m, &g, NULL, &q, saam_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	ZSL_VECTOR_DEF(a2, 5);
	ZSL_VECTOR_DEF(m2, 5);
	ZSL_VECTOR_DEF(g2, 5);

	m.data[0] = -66.0;
	m.data[1] = -98.0;
	m.data[2] = -43.0;

	/* Run the SAAM algorithm with invalid sensor data vector dimensions. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = saam_drv.feed_handler(&a2, &m, &g, NULL, &q, saam_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = saam_drv.feed_handler(&a, &m2, &g, NULL, &q, saam_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = saam_drv.feed_handler(&a, &m, &g2, NULL, &q, saam_drv.config);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_fus_complementary)
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

	zsl_real_t incl = 4.0 / 3.0;

	/* Init filter at 0 Hz. An error is expected. */
	rc = comp_drv.init_handler(0.0, comp_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Init filter at 100 Hz. */
	rc = comp_drv.init_handler(100.0, comp_drv.config);
	zassert_true(rc == 0, NULL);

	/* Run the complementary algorithm. */
	rc = comp_drv.feed_handler(&a, &m, &g, NULL, &q, comp_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.7806422574656674, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.4621974380629751, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, 0.2979802370474076, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.2969494442427867, 1E-6), NULL);

	/* Run the complementary algorithm with inclination angle provided. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = comp_drv.feed_handler(&a, &m, &g, &incl, &q, comp_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.7806422574656674, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.4621974380629751, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, 0.2979802370474076, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.2969494442427867, 1E-6), NULL);

	/* Run the complementary algorithm without magnetometer data. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = comp_drv.feed_handler(&a, NULL, &g, NULL, &q, comp_drv.config);
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
	rc = comp_drv.feed_handler(&a, &m, &g, NULL, &q, comp_drv.config);
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
	rc = comp_drv.feed_handler(NULL, &m, &g, NULL, &q, comp_drv.config);
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
	rc = comp_drv.feed_handler(&a, &m, &g, NULL, &q, comp_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999988575019579, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, 0.0004499994858759, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.0013999984005027, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, -0.0003499996001257, 1E-6), NULL);

	/* Run the complementary algorithm with no gyroscope. */
	a.data[0] = 0.01;
	a.data[1] = -1.01;
	a.data[2] = -0.02;
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = comp_drv.feed_handler(&a, &m, NULL, NULL, &q, comp_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	ZSL_VECTOR_DEF(a2, 5);
	ZSL_VECTOR_DEF(m2, 5);
	ZSL_VECTOR_DEF(g2, 5);

	/* Run the complementary algorithm with invalid alpha values. */
	comp_cfg.alpha = -0.7;
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = comp_drv.feed_handler(&a2, &m, &g, NULL, &q, comp_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	comp_cfg.alpha = 3.0;
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = comp_drv.feed_handler(&a2, &m, &g, NULL, &q, comp_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Run the complementary algorithm with invalid sensor data vector
	 * dimensions. */
	comp_cfg.alpha = 0.7;
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = comp_drv.feed_handler(&a2, &m, &g, NULL, &q, comp_drv.config);
	zassert_true(rc == -EINVAL, NULL);
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = comp_drv.feed_handler(&a2, &m, &g, NULL, &q, comp_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = comp_drv.feed_handler(&a, &m2, &g, NULL, &q, comp_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = comp_drv.feed_handler(&a, &m, &g2, NULL, &q, comp_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Run the complementary algorithm with a zero input quaternion. An error
	 * is expected, because it can't be normalized. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_EMPTY);
	rc = comp_drv.feed_handler(&a, &m, &g, NULL, &q, comp_drv.config);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_fus_aqua)
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

	zsl_real_t incl = 4.0 / 3.0;

	/* Init filter at 0 Hz. An error is expected. */
	rc = aqua_drv.init_handler(0.0, aqua_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Init filter at 100 Hz. */
	rc = aqua_drv.init_handler(100.0, aqua_drv.config);
	zassert_true(rc == 0, NULL);

	/* Run the AQUA algorithm. */
	rc = aqua_drv.feed_handler(&a, &m, &g, NULL, &q, aqua_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.7269775641540803, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.0012887113095840, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, 0.0007083207036711, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, -0.6866596381916891, 1E-6), NULL);

	/* Run the AQUA algorithm with inclination angle provided. */
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
	rc = aqua_drv.feed_handler(&a, &m, &g, &incl, &q, aqua_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.7269775641540803, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.0012887113095840, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, 0.0007083207036711, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, -0.6866596381916891, 1E-6), NULL);

	/* Run the AQUA algorithm without accelerometer data. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = aqua_drv.feed_handler(NULL, &m, &g, NULL, &q, aqua_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999988575019580, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, 0.0004499994858759, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, -0.0013999984005027, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, -0.0003499996001257, 1E-6), NULL);

	/* Run the AQUA algorithm without magnetometer data. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = aqua_drv.feed_handler(&a, NULL, &g, NULL, &q, aqua_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999988575019580, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.0004499994858759, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, 0.0013999984005027, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.0003499996001257, 1E-6), NULL);

	/* Run the AQUA algorithm without gyroscope data. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = aqua_drv.feed_handler(&a, &m, NULL, NULL, &q, aqua_drv.config);
	zassert_true(rc == -EINVAL, NULL);

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
	rc = aqua_drv.feed_handler(&a, &m, &g, NULL, &q, aqua_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.9999988575019580, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, -0.0004499994858759, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, 0.0013999984005027, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.0003499996001257, 1E-6), NULL);

	// /* Run the AQUA algorithm with invalid magnetometer data. */
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
	rc = aqua_drv.feed_handler(&a, &m, &g, NULL, &q, aqua_drv.config);
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

	/* Run the AQUA algorithm with invalid values of alpha and beta. */
	aqua_cfg.alpha = -0.7;
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = aqua_drv.feed_handler(&a, &m, &g, NULL, &q, aqua_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	aqua_cfg.alpha = 3.0;
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = aqua_drv.feed_handler(&a, &m, &g, NULL, &q, aqua_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	aqua_cfg.alpha = 0.7;
	aqua_cfg.beta = -1.9;
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = aqua_drv.feed_handler(&a, &m, &g, NULL, &q, aqua_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	aqua_cfg.beta = 5.0;
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = aqua_drv.feed_handler(&a, &m, &g, NULL, &q, aqua_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Run the AQUA algorithm with invalid sensor data vector dimensions. */
	aqua_cfg.beta = 0.7;
	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = aqua_drv.feed_handler(&a2, &m, &g, NULL, &q, aqua_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = aqua_drv.feed_handler(&a, &m2, &g, NULL, &q, aqua_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	zsl_quat_init(&q, ZSL_QUAT_TYPE_IDENTITY);
	rc = aqua_drv.feed_handler(&a, &m, &g2, NULL, &q, aqua_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Run the Madgwick algorithm with a zero input quaternion. An error is
	 * expected, because it can't be normalized. */
	zsl_quat_init(&q, ZSL_QUAT_TYPE_EMPTY);
	rc = aqua_drv.feed_handler(&a, &m, &g, NULL, &q, aqua_drv.config);
	zassert_true(rc == -EINVAL, NULL);
}

ZTEST(zsl_tests, test_fus_kalman)
{
	int rc = 0;

	zsl_real_t _kalm_P[16] = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};

	struct zsl_fus_kalm_cfg kalm_cfg = {
		.var_g = 0.3 * 0.3,
		.var_a = 0.5 * 0.5,
		.var_m = 0.8 * 0.8,
		.P = {
			.sz_rows = 4,
			.sz_cols = 4,
			.data = _kalm_P,
		},
	};

	struct zsl_fus_drv kalm_drv = {
		.init_handler = zsl_fus_kalm_init,
		.feed_handler = zsl_fus_kalm_feed,
		.error_handler = zsl_fus_kalm_error,
		.config = &kalm_cfg,
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

	zsl_real_t incl = 4.0 / 3.0;

	/* Init filter at 0 Hz. An error is expected. */
	rc = kalm_drv.init_handler(0.0, kalm_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Init filter at 100 Hz. */
	rc = kalm_drv.init_handler(100.0, kalm_drv.config);
	zassert_true(rc == 0, NULL);

	/* Run the kalman algorithm. */
	rc = kalm_drv.feed_handler(&a, &m, &g, NULL, &q, kalm_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.3186651048490019, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, 0.6386866767275943, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, 0.4721142772080947, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.5173393365852964, 1E-6), NULL);

	/* Run the kalman algorithm with dip angle provided. */
	rc = kalm_drv.feed_handler(&a, &m, &g, &incl, &q, kalm_drv.config);
	zassert_true(rc == 0, NULL);
	zassert_true(val_is_equal(q.r, 0.7715963603719183, 1E-6), NULL);
	zassert_true(val_is_equal(q.i, 0.0123858064715810, 1E-6), NULL);
	zassert_true(val_is_equal(q.j, 0.5043103125990586, 1E-6), NULL);
	zassert_true(val_is_equal(q.k, 0.3875006542769926, 1E-6), NULL);

	/* Run the kalman algorithm without accelerometer data. An error
	 * is expected. */
	rc = kalm_drv.feed_handler(NULL, &m, &g, NULL, &q, kalm_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Run the kalman algorithm without magnetometer data. An error
	 * is expected. */
	rc = kalm_drv.feed_handler(&a, NULL, &g, NULL, &q, kalm_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Run the kalman algorithm without gyroscope data. An error
	 * is expected. */
	rc = kalm_drv.feed_handler(&a, &m, NULL, NULL, &q, kalm_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	ZSL_VECTOR_DEF(a2, 5);
	ZSL_VECTOR_DEF(m2, 5);
	ZSL_VECTOR_DEF(g2, 5);

	/* Run the kalman algorithm with invalid var_g, var_a, var_m. */
	kalm_cfg.var_g = -0.8;
	rc = kalm_drv.feed_handler(&a, &m, &g, NULL, &q, kalm_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	kalm_cfg.var_g = 0.3 * 0.3;
	kalm_cfg.var_a = -2.1;
	rc = kalm_drv.feed_handler(&a, &m, &g, NULL, &q, kalm_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	kalm_cfg.var_a = 0.5 * 0.5;
	kalm_cfg.var_m = -5.0;
	rc = kalm_drv.feed_handler(&a, &m, &g, NULL, &q, kalm_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	/* Run the kalman algorithm with invalid sensor data vector dimensions. */
	kalm_cfg.var_m = 0.8 * 0.8;
	rc = kalm_drv.feed_handler(&a2, &m, &g, NULL, &q, kalm_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	rc = kalm_drv.feed_handler(&a, &m2, &g, NULL, &q, kalm_drv.config);
	zassert_true(rc == -EINVAL, NULL);

	rc = kalm_drv.feed_handler(&a, &m, &g2, NULL, &q, kalm_drv.config);
	zassert_true(rc == -EINVAL, NULL);
}
