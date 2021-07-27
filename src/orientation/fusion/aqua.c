/*
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <zsl/orientation/fusion/fusion.h>
#include <zsl/orientation/fusion/aqua.h>

static uint32_t zsl_fus_aqua_freq = 0;
static uint32_t zsl_fus_aqua_initialised = 0;

static int zsl_fus_aqua(struct zsl_vec *a, struct zsl_vec *m,
            struct zsl_vec *g, zsl_real_t *e_a, zsl_real_t *e_m,
			zsl_real_t *alpha, zsl_real_t *beta, struct zsl_quat *q)
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

	/* Convert the input quaternion to a unit quaternion. */
	zsl_quat_to_unit_d(q);

	/* Calculate an estimation of the orientation using only the data of the
	 * gyroscope and quaternion integration. */
	zsl_vec_scalar_mult(g, -1.0);
	zsl_quat_from_ang_vel(g, q, 1.0 / zsl_fus_aqua_freq, q);
	
	/* Continue with the calculations only if the data from the accelerometer
	 * is valid (non zero). */
	if ((a != NULL) && ZSL_ABS(zsl_vec_norm(a)) > 1E-6) {

		/* Normalize the acceleration vector. */
		zsl_vec_to_unit(a);

		/* Turn the data of the accelerometer into a pure quaterion. */
		struct zsl_quat qa = {
			.r = 0.0,
			.i = a->data[0],
			.j = a->data[1],
			.k = a->data[2]
		};
	
		/* Invert q. */
		struct zsl_quat q_inv;
		zsl_quat_inv(q, &q_inv);

		/* Rotate the accelerometer data quaternion (qa) using the inverse of
		 * previous estimation of the orientation (q_inv). */
		struct zsl_quat qg;
		zsl_quat_rot(&q_inv, &qa, &qg);

		/* Compute the delta q_acc quaternion. */
		struct zsl_quat Dq_acc = {
			.r = ZSL_SQRT((qg.k + 1.0) / 2.0),
			.i = -qg.j / ZSL_SQRT(2.0 * (qg.k + 1.0)),
			.j =  qg.i / ZSL_SQRT(2.0 * (qg.k + 1.0)),
			.k = 0.0
		};

		/* Scale down the delta q_acc by using spherical linear interpolation
		 * or simply linear interpolation to minimize the effects of high
		 * frequency noise in the accelerometer. */
		struct zsl_quat q_acc;
		struct zsl_quat qi;
		zsl_quat_init(&qi, ZSL_QUAT_TYPE_IDENTITY);
		if (Dq_acc.r > *e_a) {
			zsl_quat_lerp(&qi, &Dq_acc, *alpha, &q_acc);
		} else {
			zsl_quat_slerp(&qi, &Dq_acc, *alpha, &q_acc);
		}

		/* New orientation estimation with accelerometer correction. */
		zsl_quat_mult(q, &q_acc, q);

		/* Continue with the calculations only if the data from the
	 	 * magnetometer is valid (non zero). */
		if ((m != NULL) && ZSL_ABS(zsl_vec_norm(m)) > 1E-6) {
			
			/* Normalize the magnetic field vector. */
			zsl_vec_to_unit(m);
		
			/* Turn the data of the magnetometer into a pure quaterion. */
			struct zsl_quat qm = {
				.r = 0.0,
				.i = m->data[0],
				.j = m->data[1],
				.k = m->data[2]
			};

			/* Invert q again. */
			zsl_quat_inv(q, &q_inv);

			/* Rotate the magnetometer data quaternion (qm) using the inverse
			 * of the previous estimation of the orientation (q_inv). */
			struct zsl_quat ql;
			zsl_quat_rot(&q_inv, &qm, &ql);

			/* Compute the delta q_mag quaternion. */
			zsl_real_t y = ZSL_SQRT(ql.i * ql.i + ql.j * ql.j);
			struct zsl_quat Dq_mag = {
				.r = ZSL_SQRT((y + ql.i * ZSL_SQRT(y)) / (2.0 * y)),
				.i = 0.0,
				.j = 0.0,
				.k = ql.j / ZSL_SQRT(2.0 * (y + ql.i * ZSL_SQRT(y)))
			};

			/* Scale down the delta q_mag quaternion by using spherical linear
			 * interpolation or simply linear interpolation to minimize the
			 * effects of high frequency noise in the magnetometer. */
			struct zsl_quat q_mag;
			if (Dq_acc.r > *e_m) {
				zsl_quat_lerp(&qi, &Dq_mag, *beta, &q_mag);
			} else {
				zsl_quat_slerp(&qi, &Dq_mag, *beta, &q_mag);
			}

			/* New orientation estimation with magnetometer correction. */
			zsl_quat_mult(q, &q_mag, q);
		}
	}

	/* Normalize the output quaternion. */
	zsl_quat_to_unit_d(q);

err:
	return rc;
}

static int zsl_fus_aqua_quat_init(struct zsl_vec *a, struct zsl_vec *m,
                struct zsl_quat *q)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the input vectors are tridimensional. */
	if ((a != NULL && (a->sz != 3)) || (m != NULL && (m->sz != 3))) {
		rc = -EINVAL;
		goto err;
	}
#endif

	/* Initialize the q_acc and q_mag vectors as the identity vectors. */
	struct zsl_quat q_acc = { .r = 1.0, .i = 0.0, .j = 0.0, .k = 0.0};
	struct zsl_quat q_mag = { .r = 1.0, .i = 0.0, .j = 0.0, .k = 0.0};

	/* Continue with the calculations only if the data from the accelerometer
	 * is valid (non zero). */
	if ((a != NULL) && ZSL_ABS(zsl_vec_norm(a)) > 1E-6) {

		/* Normalize accelerometer data vector. */
		zsl_vec_to_unit(a);

		/* Calculate q_acc in two different ways to avoid singularities. */
		if (a->data[2] >= 0.0) {
			q_acc.r = ZSL_SQRT((a->data[2] + 1.0) / 2.0);
			q_acc.i = -a->data[1] / ZSL_SQRT(2.0 * (a->data[2] + 1.0));
			q_acc.j =  a->data[0] / ZSL_SQRT(2.0 * (a->data[2] + 1.0));
			q_acc.k = 0.0;
		} else {
			q_acc.r = -a->data[1] / ZSL_SQRT(2.0 * (1.0 - a->data[2]));
			q_acc.i = ZSL_SQRT((1.0 - a->data[2]) / 2.0);
			q_acc.j = 0.0;
			q_acc.k =  a->data[0] / ZSL_SQRT(2.0 * (1.0 - a->data[2]));
		}

		/* Continue with the calculations only if the data from the
	 	 * magnetometer is valid (non zero). */
		if ((m != NULL) && ZSL_ABS(zsl_vec_norm(m)) > 1E-6) {
			
			/* Normalize the magnetometer data vector. */
			zsl_vec_to_unit(m);

			/* Turn the data of the magnetometer into a pure quaterion. */
			struct zsl_quat qm = {
				.r = 0.0,
				.i = m->data[0],
				.j = m->data[1],
				.k = m->data[2]
			};

			/* Use the previously calculated q_acc quaternion to rotate the
			 * magnetometer data quaternion (qm). */
			struct zsl_quat ql;
			zsl_quat_rot(&q_acc, &qm, &ql);

			/* Calculate q_mag in two different ways to avoid singularities. */
			zsl_real_t y = ZSL_SQRT(ql.i * ql.i + ql.j * ql.j);
			if (ql.i >= 0.0) {
				q_mag.r = ZSL_SQRT((y + ql.i * ZSL_SQRT(y)) / (2.0 * y));
				q_mag.i = 0.0;
				q_mag.j = 0.0;
				q_mag.k = ql.j / ZSL_SQRT(2.0 * (y + ql.i * ZSL_SQRT(y)));
			} else {
				q_mag.r = ql.j / ZSL_SQRT(2.0 * (y - ql.i * ZSL_SQRT(y)));
				q_mag.i = 0.0;
				q_mag.j = 0.0;
				q_mag.k = ZSL_SQRT((y - ql.i * ZSL_SQRT(y)) / (2.0 * y));
			}
		}
	}

	/* Calculate the orientation by multiplying both estimations of the initial
	 * orientation: q_acc and q_mag. */
	zsl_quat_mult(&q_acc, &q_mag, q);

	/* Inidicate that we have already called this function. */
	zsl_fus_aqua_initialised = 1;

err:
	return rc;
}

static int zsl_fus_aqua_alpha_init(struct zsl_vec *a, zsl_real_t *alpha)
{
	/* Calculate the value of alpha, which depends on the magnitude error
	 * (m_e) and the value of alpha in static conditions. */
	zsl_real_t n = zsl_vec_norm(a);
	zsl_real_t gr = 9.81;					 /* Earth's gravity. */
	zsl_real_t m_e = ZSL_ABS(n - gr) / gr;   /* Magnitude error. */
	if (m_e >= 0.2) {
		*alpha = 0.0;
	} else if (m_e > 0.1 && m_e < 0.2) {
		*alpha *= (0.2 - m_e) / 0.1;
	}

	return 0;
}

int zsl_fus_aqua_init(uint32_t freq, void* cfg)
{
	int rc = 0;

	struct zsl_fus_aqua_cfg *mcfg = cfg;
	(void)mcfg;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the sample frequency is positive. */
	if (freq <= 0.0) {
		rc = -EINVAL;
		goto err;
	}
#endif

	zsl_fus_aqua_freq = freq;

err:
	return rc;
}

int zsl_fus_aqua_feed(struct zsl_vec *a, struct zsl_vec *m,
		      struct zsl_vec *g, struct zsl_quat *q, void* cfg)
{	
	
	struct zsl_fus_aqua_cfg *mcfg = cfg;

	/* This function should only be called once. */
    if (!zsl_fus_aqua_initialised) {
		zsl_fus_aqua_quat_init(a, m, q);
		zsl_fus_aqua_alpha_init(a, &(mcfg->alpha));
	}

	return zsl_fus_aqua(a, m, g, &(mcfg->e_a), &(mcfg->e_m), &(mcfg->alpha),
					&(mcfg->beta), q);
}

void zsl_fus_aqua_error(int error)
{
	/* ToDo: Log error in default handler. */
}
