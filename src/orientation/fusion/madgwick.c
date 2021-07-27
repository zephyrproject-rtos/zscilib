/*
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <zsl/orientation/fusion/fusion.h>
#include <zsl/orientation/fusion/madgwick.h>

static uint32_t zsl_fus_madg_freq = 0;

static int zsl_fus_madgwick_imu(struct zsl_vec *g, struct zsl_vec *a,
				zsl_real_t *beta, struct zsl_quat *q)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the input vectors are tridimensional. */
	if (g->sz != 3 || a->sz != 3) {
		rc = -EINVAL;
		goto err;
	}
#endif

	zsl_real_t Norm;
	zsl_real_t s0, s1, s2, s3;
	zsl_real_t qDot1, qDot2, qDot3, qDot4;
	zsl_real_t _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2, _8q1, _8q2;
	zsl_real_t q0q0, q1q1, q2q2, q3q3;

    /* ToDo: Remove! */
    printf("Running accel + gyro madgwick\n");

	// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * (-q->i * g->data[0] - q->j * g->data[1] - q->k * g->data[2]);
	qDot2 = 0.5f * (q->r * g->data[0] + q->j * g->data[2] - q->k * g->data[1]);
	qDot3 = 0.5f * (q->r * g->data[1] - q->i * g->data[2] + q->k * g->data[0]);
	qDot4 = 0.5f * (q->r * g->data[2] + q->i * g->data[1] - q->j * g->data[0]);

	/* Continue with the calculations only if the data from the accelerometer
	 * is valid (non zero). */
	if (ZSL_ABS(zsl_vec_norm(a)) > 1E-6) {

		/* Normalise the accelerometer vector. */
		zsl_vec_to_unit(a);

		/* Auxiliary variables to avoid repeated arithmetic. */
		_2q0 = 2.0 * q->r;
		_2q1 = 2.0 * q->i;
		_2q2 = 2.0 * q->j;
		_2q3 = 2.0 * q->k;
		_4q0 = 4.0 * q->r;
		_4q1 = 4.0 * q->i;
		_4q2 = 4.0 * q->j;
		_8q1 = 8.0 * q->i;
		_8q2 = 8.0 * q->j;
		q0q0 = q->r * q->r;
		q1q1 = q->i * q->i;
		q2q2 = q->j * q->j;
		q3q3 = q->k * q->k;

		/* Gradient decent algorithm corrective step. */
		s0 = _4q0 * q2q2 + _2q2 * a->data[0] +
		     _4q0 * q1q1 - _2q1 * a->data[1];
		s1 = _4q1 * q3q3 - _2q3 * a->data[0] +
		     4.0 * q0q0 * q->i - _2q0 * a->data[1] -
		     _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * a->data[2];
		s2 = 4.0 * q0q0 * q->j + _2q0 * a->data[0] +
		     _4q2 * q3q3 - _2q3 * a->data[1] - _4q2 +
		     _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * a->data[2];
		s3 = 4.0 * q1q1 * q->k - _2q1 * a->data[0] +
		     4.0 * q2q2 * q->k - _2q2 * a->data[1];

		/* Normalise step magnitude. */
		Norm = ZSL_SQRT(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3);
		s0 /= Norm;
		s1 /= Norm;
		s2 /= Norm;
		s3 /= Norm;

		/* Apply feedback step. */
		qDot1 -= *beta * s0;
		qDot2 -= *beta * s1;
		qDot3 -= *beta * s2;
		qDot4 -= *beta * s3;
	}

	/* Integrate rate of change of quaternion to yield quaternion. */
	q->r += qDot1 * (1.0 / zsl_fus_madg_freq);
	q->i += qDot2 * (1.0 / zsl_fus_madg_freq);
	q->j += qDot3 * (1.0 / zsl_fus_madg_freq);
	q->k += qDot4 * (1.0 / zsl_fus_madg_freq);

	/* Normalise quaternion. */
	Norm = ZSL_SQRT(q->r * q->r + q->i * q->i + q->j * q->j + q->k * q->k);
	q->r /= Norm;
	q->i /= Norm;
	q->j /= Norm;
	q->k /= Norm;

err:
	return rc;
}

/**
 * @brief
 *
 * @param g
 * @param a
 * @param m
 * @param beta
 * @param q
 *
 * @return int
 */
static int zsl_fus_madgwick(struct zsl_vec *g, struct zsl_vec *a,
			    struct zsl_vec *m, zsl_real_t *beta, struct zsl_quat *q)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the input vectors are tridimensional. */
	if (a->sz != 3 || m->sz != 3 || g->sz != 3) {
		rc = -EINVAL;
		goto err;
	}
#endif

	zsl_real_t Norm;
	zsl_real_t s0, s1, s2, s3;
	zsl_real_t qDot1, qDot2, qDot3, qDot4;
	zsl_real_t hx, hy;
	zsl_real_t _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz;
	zsl_real_t _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3;
	zsl_real_t q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;

	/* Use IMU algorithm if the magnetometer measurement is invalid. */
	if ((m == NULL) || (ZSL_ABS(zsl_vec_norm(m)) < 1E-6)) {
		return zsl_fus_madgwick_imu(g, a, beta, q);
	}

    /* ToDo: Remove! */
    printf("Running full madgwick\n");

	/* Rate of change of quaternion from gyroscope. */
	qDot1 = 0.5f * (-q->i * g->data[0] - q->j * g->data[1] - q->k * g->data[2]);
	qDot2 = 0.5f * (q->r * g->data[0] + q->j * g->data[2] - q->k * g->data[1]);
	qDot3 = 0.5f * (q->r * g->data[1] - q->i * g->data[2] + q->k * g->data[0]);
	qDot4 = 0.5f * (q->r * g->data[2] + q->i * g->data[1] - q->j * g->data[0]);

	/* Continue with the calculations only if the data from the accelerometer
	 * is valid (non zero). */
	if (ZSL_ABS(zsl_vec_norm(a)) > 1E-6) {

		/* Normalise the accelerometer vector. */
		zsl_vec_to_unit(a);

		/* Normalise the magnetometer vector. */
		zsl_vec_to_unit(m);

		/* Auxiliary variables to avoid repeated arithmetic. */
		_2q0mx = 2.0 * q->r * m->data[0];
		_2q0my = 2.0 * q->r * m->data[1];
		_2q0mz = 2.0 * q->r * m->data[2];
		_2q1mx = 2.0 * q->i * m->data[0];
		_2q0 = 2.0 * q->r;
		_2q1 = 2.0 * q->i;
		_2q2 = 2.0 * q->j;
		_2q3 = 2.0 * q->k;
		_2q0q2 = 2.0 * q->r * q->j;
		_2q2q3 = 2.0 * q->j * q->k;
		q0q0 = q->r * q->r;
		q0q1 = q->r * q->i;
		q0q2 = q->r * q->j;
		q0q3 = q->r * q->k;
		q1q1 = q->i * q->i;
		q1q2 = q->i * q->j;
		q1q3 = q->i * q->k;
		q2q2 = q->j * q->j;
		q2q3 = q->j * q->k;
		q3q3 = q->k * q->k;

		/* Reference direction of Earth's magnetic field. */
		hx = m->data[0] * q0q0 - _2q0my * q->k +
		     _2q0mz * q->j + m->data[0] * q1q1 + _2q1 * m->data[1] * q->j +
		     _2q1 * m->data[2] * q->k - m->data[0] * q2q2 - m->data[0] * q3q3;
		hy = _2q0mx * q->k + m->data[1] * q0q0 -
		     _2q0mz * q->i + _2q1mx * q->j - m->data[1] * q1q1 +
		     m->data[1] * q2q2 + _2q2 * m->data[2] * q->k - m->data[1] * q3q3;
		_2bx = ZSL_SQRT(hx * hx + hy * hy);
		_2bz = -_2q0mx * q->j + _2q0my * q->i +
		       m->data[2] * q0q0 + _2q1mx * q->k - m->data[2] * q1q1 +
		       _2q2 * m->data[1] * q->k - m->data[2] * q2q2 + m->data[2] * q3q3;
		_4bx = 2.0 * _2bx;
		_4bz = 2.0 * _2bz;

		/* Gradient decent algorithm corrective step. */
		s0 = -_2q2 * (2.0 * q1q3 - _2q0q2 - a->data[0]) +
		     _2q1 * (2.0 * q0q1 + _2q2q3 - a->data[1]) -
		     _2bz * q->j *
		     (_2bx * (0.5 - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - m->data[0]) +
		     (-_2bx * q->k + _2bz * q->i) *
		     (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - m->data[1]) +
		     _2bx * q->j *
		     (_2bx * (q0q2 + q1q3) + _2bz * (0.5 - q1q1 - q2q2) - m->data[2]);
		s1 = _2q3 * (2.0 * q1q3 - _2q0q2 - a->data[0]) +
		     _2q0 * (2.0 * q0q1 + _2q2q3 - a->data[1]) -
		     4.0 * q->i * (1.0 - 2.0 * q1q1 - 2.0 * q2q2 - a->data[2]) +
		     _2bz * q->k *
		     (_2bx * (0.5 - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - m->data[0]) +
		     (_2bx * q->j + _2bz * q->r) *
		     (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - m->data[1]) +
		     (_2bx * q->k - _4bz * q->i) *
		     (_2bx * (q0q2 + q1q3) + _2bz * (0.5 - q1q1 - q2q2) - m->data[2]);
		s2 = -_2q0 * (2.0 * q1q3 - _2q0q2 - a->data[0]) +
		     _2q3 * (2.0 * q0q1 + _2q2q3 - a->data[1]) -
		     4.0 * q->j * (1.0 - 2.0 * q1q1 - 2.0 * q2q2 - a->data[2]) +
		     (-_4bx * q->j - _2bz * q->r) *
		     (_2bx * (0.5 - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - m->data[0]) +
		     (_2bx * q->i + _2bz * q->k) *
		     (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - m->data[1]) +
		     (_2bx * q->r - _4bz * q->j) *
		     (_2bx * (q0q2 + q1q3) + _2bz * (0.5 - q1q1 - q2q2) - m->data[2]);
		s3 = _2q1 * (2.0 * q1q3 - _2q0q2 - a->data[0]) +
		     _2q2 * (2.0 * q0q1 + _2q2q3 - a->data[1]) +
		     (-_4bx * q->k + _2bz * q->i) *
		     (_2bx * (0.5 - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - m->data[0]) +
		     (-_2bx * q->r + _2bz * q->j) *
		     (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - m->data[1]) +
		     _2bx * q->i *
		     (_2bx * (q0q2 + q1q3) + _2bz * (0.5 - q1q1 - q2q2) - m->data[2]);

		/* Normalise step magnitude. */
		Norm = ZSL_SQRT(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3);
		s0 /= Norm;
		s1 /= Norm;
		s2 /= Norm;
		s3 /= Norm;

		/* Apply feedback step. */
		qDot1 -= *beta * s0;
		qDot2 -= *beta * s1;
		qDot3 -= *beta * s2;
		qDot4 -= *beta * s3;
	}

	/* Integrate rate of change of quaternion to yield quaternion. */
	q->r += qDot1 * (1.0 / zsl_fus_madg_freq);
	q->i += qDot2 * (1.0 / zsl_fus_madg_freq);
	q->j += qDot3 * (1.0 / zsl_fus_madg_freq);
	q->k += qDot4 * (1.0 / zsl_fus_madg_freq);

	/* Normalise quaternion. */
	Norm = ZSL_SQRT(q->r * q->r + q->i * q->i + q->j * q->j + q->k * q->k);
	q->r /= Norm;
	q->i /= Norm;
	q->j /= Norm;
	q->k /= Norm;

err:
	return rc;
}

int zsl_fus_madg_init(uint32_t freq)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the sample frequency is positive. */
	if (freq <= 0) {
		rc = -EINVAL;
		goto err;
	}
#endif

    /* ToDo: Remove once working! */
    printf("Setting madgwick freq to %d\n", freq);

	zsl_fus_madg_freq = freq;

err:
	return rc;
}

int zsl_fus_madg_feed(struct zsl_vec *a, struct zsl_vec *m, struct zsl_vec *g,
		      struct zsl_quat *q)
{
	zsl_real_t beta = 0.8;

	return zsl_fus_madgwick(g, a, m, &beta, q);
}

void zsl_fus_madg_error(int error)
{
	/* ToDo: Log error in default handler. */
}
