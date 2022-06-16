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

static uint32_t zsl_fus_mahn_freq;

static int zsl_fus_mahony_imu(struct zsl_vec *g, struct zsl_vec *a,
			      zsl_real_t *Kp, zsl_real_t *Ki,
			      struct zsl_vec *integralFB, 
				  zsl_real_t integral_limit,
				  zsl_real_t *incl,
			      struct zsl_quat *q)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the input vectors are tridimensional. */
	if ((a != NULL && (a->sz != 3)) || g->sz != 3 || integralFB->sz != 3) {
		rc = -EINVAL;
		goto err;
	}
	/* Make sure that the input quaternion is not zero. */
	if (ZSL_ABS(zsl_quat_magn(q)) < 1E-6) {
		rc = -EINVAL;
		goto err;
	}
#endif

	/* Continue with the calculations only if the data from the accelerometer
	 * is valid (non zero). */
	if ((a != NULL) && ZSL_ABS(zsl_vec_norm(a)) > 1E-6) {

		/* Normalise the accelerometer vector. */
		zsl_vec_to_unit(a);

		/* Define the normalized quaternion of acceleration on the earth's
		 * reference frame, which only has a vertical component. */
		struct zsl_quat qa = {
			.r = 0.0,
			.i = 0.0,
			.j = 0.0,
			.k = 1.0
		};

		/* Calculate an estimation of the gravity field 'v' using the input
		 * quaternion to rotate the normalised acceleration vector in the
		 * earth's reference frame 'qa'. */
		ZSL_VECTOR_DEF(v, 3);
		struct zsl_quat qaq;
		zsl_quat_rot(q, &qa, &qaq);
		v.data[0] = qaq.i;
		v.data[1] = qaq.j;
		v.data[2] = qaq.k;

		/* The orientatiom error is the cross product between measured
		 * direction of gravity 'a' and estimated direction of gravity 'v'. */
		ZSL_VECTOR_DEF(e, 3);
		zsl_vec_cross(a, &v, &e);

		/* Compute integral feedback. */
		integralFB->data[0] += e.data[0] * (1.0 / zsl_fus_mahn_freq);
		integralFB->data[1] += e.data[1] * (1.0 / zsl_fus_mahn_freq);
		integralFB->data[2] += e.data[2] * (1.0 / zsl_fus_mahn_freq);

		/* Limit integral values */
		if(integralFB->data[0] > integral_limit) {
			integralFB->data[0] = integral_limit;
		} else if (integralFB->data[0] < -integral_limit) {
			integralFB->data[0] = -integral_limit;
		}

		if(integralFB->data[1] > integral_limit) {
			integralFB->data[1] = integral_limit;
		} else if (integralFB->data[1] < -integral_limit) {
			integralFB->data[1] = -integral_limit;
		}

		if(integralFB->data[2] > integral_limit) {
			integralFB->data[2] = integral_limit;
		} else if (integralFB->data[2] < -integral_limit) {
			integralFB->data[2] = -integral_limit;
		}

		/* Apply integral feedback multiplied by Ki. */
		g->data[0] += *Ki * integralFB->data[0];
		g->data[1] += *Ki * integralFB->data[1];
		g->data[2] += *Ki * integralFB->data[2];

		/* Apply proportional feedback. */
		g->data[0] += *Kp * e.data[0];
		g->data[1] += *Kp * e.data[1];
		g->data[2] += *Kp * e.data[2];
	}

	/* Integrate rate of change of the input quaternion using the modified
	 * angular velocity data from the gyroscope. */
	zsl_quat_from_ang_vel(g, q, 1.0 / zsl_fus_mahn_freq, q);

	/* Normalize the output quaternion. */
	zsl_quat_to_unit_d(q);

err:
	return rc;
}

static int zsl_fus_mahony(struct zsl_vec *g, struct zsl_vec *a,
			  struct zsl_vec *m, zsl_real_t *Kp, zsl_real_t *Ki,
			  struct zsl_vec *integralFB,
			  zsl_real_t integral_limit, zsl_real_t *incl, struct zsl_quat *q)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the input vectors are tridimensional. */
	if ((a != NULL && (a->sz != 3)) || (m != NULL && (m->sz != 3)) ||
	    g->sz != 3 || integralFB->sz != 3) {
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
		return zsl_fus_mahony_imu(g, a, Kp, Ki, integralFB, integral_limit, incl, q);
	}

	/* Continue with the calculations only if the data from the accelerometer
	 * is valid (non zero). */
	if ((a != NULL) && ZSL_ABS(zsl_vec_norm(a)) > 1E-6) {

		/* Normalise the accelerometer vector. */
		zsl_vec_to_unit(a);

		/* Normalise the magnetometer vector. */
		zsl_vec_to_unit(m);

		/* Define the normalized quaternion of acceleration on the earth's
		 * reference frame, which only has a vertical component. */
		struct zsl_quat qa = {
			.r = 0.0,
			.i = 0.0,
			.j = 0.0,
			.k = 1.0
		};

		/* Calculate an estimation of the gravity field 'v' using the input
		 * quaternion to rotate the normalised acceleration vector in the
		 * earth's reference frame 'qa'. */
		ZSL_VECTOR_DEF(v, 3);
		struct zsl_quat qaq;
		zsl_quat_rot(q, &qa, &qaq);
		v.data[0] = qaq.i;
		v.data[1] = qaq.j;
		v.data[2] = qaq.k;

		/* The gravity orientatiom error is the cross product between measured
		 * direction of gravity 'a' and estimated direction of gravity 'v'. */
		ZSL_VECTOR_DEF(e_g, 3);
		zsl_vec_cross(a, &v, &e_g);

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

		/* Calculate an estimation of the magnetic field 'bf' using the input
		 * quaternion to rotate the normalised magnetic field quaternion in the
		 * earth's reference frame 'b'. */
		ZSL_VECTOR_DEF(bf, 3);
		struct zsl_quat qbq;
		zsl_quat_rot(q, &b, &qbq);
		bf.data[0] = qbq.i;
		bf.data[1] = qbq.j;
		bf.data[2] = qbq.k;

		/* The magnetic field orientation error is the cross product between
		 * measured direction of magnetic field 'm' and estimated direction of
		 * magnetic field 'bf'. */
		ZSL_VECTOR_DEF(e_b, 3);
		zsl_vec_cross(m, &bf, &e_b);

		/* The total orientation error is the sum of the gravity and the
		 * magnetic field errors. */
		ZSL_VECTOR_DEF(e, 3);
		zsl_vec_add(&e_g, &e_b, &e);

		/* Compute and apply integral feedback if enabled. */
		integralFB->data[0] += e.data[0] * (1.0 / zsl_fus_mahn_freq);
		integralFB->data[1] += e.data[1] * (1.0 / zsl_fus_mahn_freq);
		integralFB->data[2] += e.data[2] * (1.0 / zsl_fus_mahn_freq);

		/* Limit integral values */
		if(integralFB->data[0] > integral_limit) {
			integralFB->data[0] = integral_limit;
		} else if (integralFB->data[0] < -integral_limit) {
			integralFB->data[0] = -integral_limit;
		}

		if(integralFB->data[1] > integral_limit) {
			integralFB->data[1] = integral_limit;
		} else if (integralFB->data[1] < -integral_limit) {
			integralFB->data[1] = -integral_limit;
		}

		if(integralFB->data[2] > integral_limit) {
			integralFB->data[2] = integral_limit;
		} else if (integralFB->data[2] < -integral_limit) {
			integralFB->data[2] = -integral_limit;
		}

		/* Apply integral feedback multiplied by Ki. */
		g->data[0] += *Ki * integralFB->data[0];
		g->data[1] += *Ki * integralFB->data[1];
		g->data[2] += *Ki * integralFB->data[2];

		/* Apply proportional feedback. */
		g->data[0] += *Kp * e.data[0];
		g->data[1] += *Kp * e.data[1];
		g->data[2] += *Kp * e.data[2];
	}

	/* Integrate rate of change of the input quaternion using the modified
	 * angular velocity data from the gyroscope. */
	zsl_quat_from_ang_vel(g, q, 1.0 / zsl_fus_mahn_freq, q);

	/* Normalize the output quaternion. */
	zsl_quat_to_unit_d(q);

err:
	return rc;
}

int zsl_fus_mahn_init(uint32_t freq, void *cfg)
{
	int rc = 0;

	struct zsl_fus_mahn_cfg *mcfg = cfg;

	(void)mcfg;

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
		      zsl_real_t *incl, struct zsl_quat *q, void *cfg)
{
	struct zsl_fus_mahn_cfg *mcfg = cfg;

	if (mcfg->kp < 0.0 || mcfg->ki < 0.0) {
		return -EINVAL;
	}

	return zsl_fus_mahony(g, a, m, &(mcfg->kp), &(mcfg->ki), &(mcfg->intfb),
			      mcfg->integral_limit, incl, q);
}

void zsl_fus_mahn_error(int error)
{
	/* ToDo: Log error in default handler. */
}
