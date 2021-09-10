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

static uint32_t zsl_fus_madg_freq;

static int zsl_fus_madgwick_imu(struct zsl_vec *g, struct zsl_vec *a,
				zsl_real_t *beta, zsl_real_t *incl, struct zsl_quat *q)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the input vectors are tridimensional. */
	if ((a != NULL && (a->sz != 3)) || g->sz != 3) {
		rc = -EINVAL;
		goto err;
	}
	/* Make sure that the input quaternion is not zero. */
	if (ZSL_ABS(zsl_quat_magn(q)) < 1E-6) {
		rc = -EINVAL;
		goto err;
	}
#endif

	/* Convert the input quaternion to a unit quaternion. */
	zsl_quat_to_unit_d(q);

	/* Declare the gradient vector and set it to zero. It will only be
	 * modified if the acceleration vector is valid. */
	ZSL_VECTOR_DEF(grad, 4);
	zsl_vec_init(&grad);

	/* Continue with the calculations only if the data from the accelerometer
	 * is valid (non zero). */
	if ((a != NULL) && ZSL_ABS(zsl_vec_norm(a)) > 1E-6) {

		/* Normalize the acceleration vector. */
		zsl_vec_to_unit(a);

		/* Define the normalized quaternion of acceleration on the earth's
		 * reference frame, which only has a vertical component. */
		struct zsl_quat qa = {
			.r = 0.0,
			.i = 0.0,
			.j = 0.0,
			.k = 1.0
		};

		/* Calculate the function f by using the input quaternion to rotate the
		 * normalised acceleration vector in the earth's reference frame, and
		 * then substracting the acceleration vector. */
		ZSL_MATRIX_DEF(f, 3, 1);
		struct zsl_quat qaq;
		zsl_quat_rot(q, &qa, &qaq);
		f.data[0] = qaq.i - a->data[0];
		f.data[1] = qaq.j - a->data[1];
		f.data[2] = qaq.k - a->data[2];

		/* Define and compute the transpose of the Jacobian matrix of the
		 * function f. */
		ZSL_MATRIX_DEF(jt, 4, 3);
		zsl_real_t jt_data[12] = {
			-2.0 * q->j, 2.0 * q->i, 0.0,
			2.0 * q->k, 2.0 * q->r, -4.0 * q->i,
			-2.0 * q->r, 2.0 * q->k, -4.0 * q->j,
			2.0 * q->i, 2.0 * q->j, 0.0
		};

		zsl_mtx_from_arr(&jt, jt_data);

		/* Calculate the gradient of f by multiplying the transposed Jacobian
		 * matrix by the function itself. */
		ZSL_MATRIX_DEF(jtf, 4, 1);
		zsl_mtx_mult(&jt, &f, &jtf);

		/* Normalize the gradient vector. */
		zsl_vec_from_arr(&grad, jtf.data);
		zsl_vec_to_unit(&grad);
	}

	/* Update the input quaternion with a modified quaternion integration. */
	zsl_quat_from_ang_vel(g, q, 1.0 / zsl_fus_madg_freq, q);
	q->r -= (1.0 / zsl_fus_madg_freq) * (*beta * grad.data[0]);
	q->i -= (1.0 / zsl_fus_madg_freq) * (*beta * grad.data[1]);
	q->j -= (1.0 / zsl_fus_madg_freq) * (*beta * grad.data[2]);
	q->k -= (1.0 / zsl_fus_madg_freq) * (*beta * grad.data[3]);

	/* Normalize the output quaternion. */
	zsl_quat_to_unit_d(q);

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
			    struct zsl_vec *m, zsl_real_t *beta, zsl_real_t *incl,
			    struct zsl_quat *q)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the input vectors are tridimensional. */
	if ((a != NULL && (a->sz != 3)) || (m != NULL && (m->sz != 3)) ||
	    g->sz != 3) {
		rc = -EINVAL;
		goto err;
	}
	/* Make sure that the input quaternion is not zero. */
	if (ZSL_ABS(zsl_quat_magn(q)) < 1E-6) {
		rc = -EINVAL;
		goto err;
	}
#endif

	/* Use IMU algorithm if the magnetometer measurement is invalid. */
	if ((m == NULL) || (ZSL_ABS(zsl_vec_norm(m)) < 1E-6)) {
		return zsl_fus_madgwick_imu(g, a, beta, incl, q);
	}

	/* Convert the input quaternion to a unit quaternion. */
	zsl_quat_to_unit_d(q);

	/* Declare the gradient vector and set it to zero. It will only be
	 * modified if the acceleration vector is valid. */
	ZSL_VECTOR_DEF(grad, 4);
	zsl_vec_init(&grad);

	/* Continue with the calculations only if the data from the accelerometer
	 * is valid (non zero). */
	if ((a != NULL) && ZSL_ABS(zsl_vec_norm(a)) > 1E-6) {

		/* Normalize the acceleration vector. */
		zsl_vec_to_unit(a);

		/* Normalize the magnetic field vector. */
		zsl_vec_to_unit(m);

		/* Define the normalized vector of acceleration on the earth's
		 * reference frame, which only has a vertical component. */
		struct zsl_quat qa = {
			.r = 0.0,
			.i = 0.0,
			.j = 0.0,
			.k = 1.0
		};

		/* Calculate the function f_g by using the input quaternion to rotate
		 * the normalised acceleration quaternion in the earth's reference
		 * frame 'qa', and then substracting the acceleration vector. */
		ZSL_MATRIX_DEF(f_g, 3, 1);
		struct zsl_quat qaq;
		zsl_quat_rot(q, &qa, &qaq);
		f_g.data[0] = qaq.i - a->data[0];
		f_g.data[1] = qaq.j - a->data[1];
		f_g.data[2] = qaq.k - a->data[2];

		/* Define and compute the transpose of the Jacobian matrix of the
		 * function f_g. */
		ZSL_MATRIX_DEF(jt_g, 4, 3);
		zsl_real_t jt_g_data[12] = {
			-2.0 * q->j,    2.0 * q->i,              0.0,
			2.0 * q->k,    2.0 * q->r,             -4.0 * q->i,
			-2.0 * q->r,    2.0 * q->k,             -4.0 * q->j,
			2.0 * q->i,    2.0 * q->j,              0.0
		};

		zsl_mtx_from_arr(&jt_g, jt_g_data);

		/* Calculate the gradient of f_g by multiplying the transposed Jacobian
		 * matrix by the function itself. */
		ZSL_MATRIX_DEF(jtf_g, 4, 1);
		zsl_mtx_mult(&jt_g, &f_g, &jtf_g);

		/* Turn the data of the magnetometer into a pure quaterion. */
		struct zsl_quat qm = {
			.r = 0.0,
			.i = m->data[0],
			.j = m->data[1],
			.k = m->data[2]
		};

		/* Define the normalized quaternion 'b' of magnetic field on the
		 * earth's reference frame, which only has a x (north) and z (vertical)
		 * components. If the declination angle is known, use it to calculate
		 * this vector. */
		zsl_real_t bx;
		zsl_real_t bz;

		if (incl == NULL) {
			struct zsl_quat h;
			zsl_quat_rot(q, &qm, &h);
			bx = ZSL_SQRT(h.i * h.i + h.j * h.j);
			bz = h.k;
		} else {
			bx = ZSL_COS(*incl * ZSL_PI / 180.0);
			bz = ZSL_SIN(*incl * ZSL_PI / 180.0);
		}

		struct zsl_quat b = { .r = 0.0, .i = bx, .j = 0.0, .k = bz };

		/* Calculate the function f_b by using the input quaternion to rotate
		 * the normalised magnetic field quaternion in the earth's reference
		 * frame 'b', and then substracting the magnetometer data vector. */
		ZSL_MATRIX_DEF(f_b, 3, 1);
		struct zsl_quat qbq;
		zsl_quat_rot(q, &b, &qbq);
		f_b.data[0] = qbq.i - m->data[0];
		f_b.data[1] = qbq.j - m->data[1];
		f_b.data[2] = qbq.k - m->data[2];

		/* Define and compute the transpose of the Jacobian matrix of the
		 * function f_b. */
		ZSL_MATRIX_DEF(jt_b, 4, 3);
		zsl_real_t jt_b_data[12] = {
			/* 1x1..1x3 */
			-2.0 * bz * q->j,
			-2.0 * bx * q->k + 2.0 * bz * q->i,
			2.0 * bx * q->j,
			/* 2x1..2x3 */
			2.0 * bz * q->k,
			2.0 * bz * q->j + 2.0 * bz * q->r,
			2.0 * bx * q->k - 4.0 * bz * q->i,
			/* 3x1..3x3 */
			-4.0 * bx * q->j - 2.0 * bz * q->r,
			2.0 * bx * q->i - 2.0 * bz * q->k,
			2.0 * bx * q->r - 2.0 * bz * q->j,
			/* 4x1..4x3 */
			-4.0 * bx * q->k + 2.0 * bz * q->i,
			-2.0 * bx * q->r + 2.0 * bz * q->j,
			2.0 * bx * q->i
		};

		zsl_mtx_from_arr(&jt_b, jt_b_data);

		/* Calculate the gradient of f_b by multiplying the transposed Jacobian
		 * matrix by the function itself. */
		ZSL_MATRIX_DEF(jtf_b, 4, 1);
		zsl_mtx_mult(&jt_b, &f_b, &jtf_b);

		/* Compute the total gradient vector by adding the gradient of f_g and
		 * total gradient of f_b. Then normalize the total gradient vector. */
		grad.data[0] = jtf_g.data[0] + jtf_b.data[0];
		grad.data[1] = jtf_g.data[1] + jtf_b.data[1];
		grad.data[2] = jtf_g.data[2] + jtf_b.data[2];
		grad.data[3] = jtf_g.data[3] + jtf_b.data[3];

		zsl_vec_to_unit(&grad);
	}

	/* Update the input quaternion with a modified quaternion integration. */
	zsl_quat_from_ang_vel(g, q, 1.0 / zsl_fus_madg_freq, q);
	q->r -= (1.0 / zsl_fus_madg_freq) * (*beta * grad.data[0]);
	q->i -= (1.0 / zsl_fus_madg_freq) * (*beta * grad.data[1]);
	q->j -= (1.0 / zsl_fus_madg_freq) * (*beta * grad.data[2]);
	q->k -= (1.0 / zsl_fus_madg_freq) * (*beta * grad.data[3]);

	/* Normalize the output quaternion. */
	zsl_quat_to_unit_d(q);

err:
	return rc;
}

int zsl_fus_madg_init(uint32_t freq, void *cfg)
{
	int rc = 0;

	struct zsl_fus_madg_cfg *mcfg = cfg;

	(void)mcfg;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the sample frequency is positive. */
	if (freq <= 0) {
		rc = -EINVAL;
		goto err;
	}
#endif

	zsl_fus_madg_freq = freq;

err:
	return rc;
}

int zsl_fus_madg_feed(struct zsl_vec *a, struct zsl_vec *m, struct zsl_vec *g,
		      zsl_real_t *incl, struct zsl_quat *q, void *cfg)
{
	struct zsl_fus_madg_cfg *mcfg = cfg;

	if (mcfg->beta < 0.0) {
		return -EINVAL;
	}

	return zsl_fus_madgwick(g, a, m, &(mcfg->beta), incl, q);
}

void zsl_fus_madg_error(int error)
{
	/* ToDo: Log error in default handler. */
}
