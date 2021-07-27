/*
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <zsl/orientation/fusion/fusion.h>
#include <zsl/orientation/fusion/mahony.h>

static uint32_t zsl_fus_mahn_freq = 0;

static int zsl_fus_mahony_imu(struct zsl_vec *g, struct zsl_vec *a,
			      zsl_real_t *twoKp, zsl_real_t *twoKi,
			      struct zsl_vec *integralFB, struct zsl_quat *q)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the input vectors are tridimensional. */
	if (g->sz != 3 || a->sz != 3 || integralFB->sz != 3) {
		rc = -EINVAL;
		goto err;
	}
#endif

	zsl_real_t Norm;
	zsl_real_t halfvx, halfvy, halfvz;
	zsl_real_t halfex, halfey, halfez;
	zsl_real_t qa, qb, qc;

	/* Continue with the calculations only if the data from the accelerometer
	 * is valid (non zero). */
	if (ZSL_ABS(zsl_vec_norm(a)) > 1E-6) {

		/* Normalise the accelerometer vector. */
		zsl_vec_to_unit(a);

		/* Estimated direction of the gravity vector. */
		halfvx = q->i * q->k - q->r * q->j;
		halfvy = q->r * q->i + q->j * q->k;
		halfvz = q->r * q->r - 0.5 + q->k * q->k;

		/* Error is a sum of cross product between estimated direction and
		 * measured direction of gravity. */
		halfex = (a->data[1] * halfvz - a->data[2] * halfvy);
		halfey = (a->data[2] * halfvx - a->data[0] * halfvz);
		halfez = (a->data[0] * halfvy - a->data[1] * halfvx);

		/* Compute and apply integral feedback if enabled. */
		if (*twoKi > 1E-6) {
			/* Integral error scaled by Ki. */
			integralFB->data[0] += *twoKi * halfex * (1.0 / zsl_fus_mahn_freq);
			integralFB->data[1] += *twoKi * halfey * (1.0 / zsl_fus_mahn_freq);
			integralFB->data[2] += *twoKi * halfez * (1.0 / zsl_fus_mahn_freq);
			/* Apply integral feedback. */
			g->data[0] += integralFB->data[0];
			g->data[1] += integralFB->data[1];
			g->data[2] += integralFB->data[2];
		} else {
			integralFB->data[0] = 0.0;
			integralFB->data[1] = 0.0;
			integralFB->data[2] = 0.0;
		}

		/* Apply proportional feedback. */
		g->data[0] += *twoKp * halfex;
		g->data[1] += *twoKp * halfey;
		g->data[2] += *twoKp * halfez;
	}

	/* Integrate rate of change of quaternion. */
	g->data[0] *= (0.5 * (1.0 / zsl_fus_mahn_freq));
	g->data[1] *= (0.5 * (1.0 / zsl_fus_mahn_freq));
	g->data[2] *= (0.5 * (1.0 / zsl_fus_mahn_freq));
	qa = q->r;
	qb = q->i;
	qc = q->j;
	q->r += (-qb * g->data[0] - qc * g->data[1] - q->k * g->data[2]);
	q->i += (qa * g->data[0] + qc * g->data[2] - q->k * g->data[1]);
	q->j += (qa * g->data[1] - qb * g->data[2] + q->k * g->data[0]);
	q->k += (qa * g->data[2] + qb * g->data[1] - qc * g->data[0]);

	/* Normalise quaternion. */
	Norm = ZSL_SQRT(q->r * q->r + q->i * q->i + q->j * q->j + q->k * q->k);
	q->r /= Norm;
	q->i /= Norm;
	q->j /= Norm;
	q->k /= Norm;

err:
	return rc;
}

static int zsl_fus_mahony(struct zsl_vec *g, struct zsl_vec *a,
			  struct zsl_vec *m,
			  zsl_real_t *twoKp, zsl_real_t *twoKi,
			  struct zsl_vec *integralFB, struct zsl_quat *q)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the input vectors are tridimensional. */
	if (g->sz != 3 || a->sz != 3 || m->sz != 3 || integralFB->sz != 3) {
		rc = -EINVAL;
		goto err;
	}
#endif

	zsl_real_t Norm;
	zsl_real_t q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;
	zsl_real_t hx, hy, bx, bz;
	zsl_real_t halfvx, halfvy, halfvz, halfwx, halfwy, halfwz;
	zsl_real_t halfex, halfey, halfez;
	zsl_real_t qa, qb, qc;

	/* Use IMU algorithm if the magnetometer measurement is invalid. */
	if ((m == NULL) || (ZSL_ABS(zsl_vec_norm(m)) < 1E-6)) {
		return zsl_fus_mahony_imu(g, a, twoKp, twoKi, integralFB, q);
	}

	/* Continue with the calculations only if the data from the accelerometer
	 * is valid (non zero). */
	if (ZSL_ABS(zsl_vec_norm(a)) > 1E-6) {

		/* Normalise the accelerometer vector. */
		zsl_vec_to_unit(a);

		/* Normalise the magnetometer vector. */
		zsl_vec_to_unit(m);

		/* Auxiliary variables to avoid repeated arithmetic. */
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
		hx = 2.0 * (m->data[0] * (0.5 - q2q2 - q3q3) + m->data[1] *
			    (q1q2 - q0q3) + m->data[2] * (q1q3 + q0q2));
		hy = 2.0 * (m->data[0] * (q1q2 + q0q3) + m->data[1] *
			    (0.5 - q1q1 - q3q3) + m->data[2] * (q2q3 - q0q1));
		bx = ZSL_SQRT(hx * hx + hy * hy);
		bz = 2.0 * (m->data[0] * (q1q3 - q0q2) + m->data[1] *
			    (q2q3 + q0q1) + m->data[2] * (0.5 - q1q1 - q2q2));

		/* Estimated direction of gravity and magnetic field. */
		halfvx = q1q3 - q0q2;
		halfvy = q0q1 + q2q3;
		halfvz = q0q0 - 0.5 + q3q3;
		halfwx = bx * (0.5 - q2q2 - q3q3) + bz * (q1q3 - q0q2);
		halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
		halfwz = bx * (q0q2 + q1q3) + bz * (0.5 - q1q1 - q2q2);

		/* Error is a sum of cross product between estimated direction and
         * measured direction of field vectors. */
		halfex = (a->data[1] * halfvz - a->data[2] * halfvy) +
			 (m->data[1] * halfwz - m->data[2] * halfwy);
		halfey = (a->data[2] * halfvx - a->data[0] * halfvz) +
			 (m->data[2] * halfwx - m->data[0] * halfwz);
		halfez = (a->data[0] * halfvy - a->data[1] * halfvx) +
			 (m->data[0] * halfwy - m->data[1] * halfwx);

		/* Compute and apply integral feedback if enabled. */
		if (*twoKi > 1E-6) {
			/* Integral error scaled by Ki. */
			integralFB->data[0] += *twoKi * halfex * (1.0 / zsl_fus_mahn_freq);
			integralFB->data[1] += *twoKi * halfey * (1.0 / zsl_fus_mahn_freq);
			integralFB->data[2] += *twoKi * halfez * (1.0 / zsl_fus_mahn_freq);
			/* Apply integral feedback. */
			g->data[0] += integralFB->data[0];
			g->data[1] += integralFB->data[1];
			g->data[2] += integralFB->data[2];
		} else {
			integralFB->data[0] = 0.0;
			integralFB->data[1] = 0.0;
			integralFB->data[2] = 0.0;
		}

		/* Apply proportional feedback. */
		g->data[0] += *twoKp * halfex;
		g->data[1] += *twoKp * halfey;
		g->data[2] += *twoKp * halfez;
	}

	/* Integrate rate of change of quaternion. */
	g->data[0] *= (0.5 * (1.0 / zsl_fus_mahn_freq));
	g->data[1] *= (0.5 * (1.0 / zsl_fus_mahn_freq));
	g->data[2] *= (0.5 * (1.0 / zsl_fus_mahn_freq));
	qa = q->r;
	qb = q->i;
	qc = q->j;
	q->r += (-qb * g->data[0] - qc * g->data[1] - q->k * g->data[2]);
	q->i += (qa * g->data[0] + qc * g->data[2] - q->k * g->data[1]);
	q->j += (qa * g->data[1] - qb * g->data[2] + q->k * g->data[0]);
	q->k += (qa * g->data[2] + qb * g->data[1] - qc * g->data[0]);

	/* Normalise quaternion. */
	Norm = ZSL_SQRT(q->r * q->r + q->i * q->i + q->j * q->j + q->k * q->k);
	q->r /= Norm;
	q->i /= Norm;
	q->j /= Norm;
	q->k /= Norm;

err:
	return rc;
}

int zsl_fus_mahn_init(uint32_t freq)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the sample frequency is positive. */
	if (freq <= 0.0) {
		rc = -EINVAL;
		goto err;
	}
#endif

	zsl_fus_mahn_freq = freq;

err:
	return rc;
}

int zsl_fus_mahn_feed(struct zsl_vec *a, struct zsl_vec *m, struct zsl_vec *g,
		      struct zsl_quat *q)
{
    zsl_real_t twoKp = 0.0;
    zsl_real_t twoKi = 0.0;
	ZSL_VECTOR_DEF(integralFB, 3);

	return zsl_fus_mahony(g, a, m, &twoKp, &twoKi, &integralFB, q);
}

void zsl_fus_mahn_error(int error)
{
	/* ToDo: Log error in default handler. */
}
