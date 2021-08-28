/*
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <zsl/orientation/fusion/fusion.h>
#include <zsl/orientation/fusion/complementary.h>

static uint32_t zsl_fus_comp_freq = 0;

static int zsl_fus_comp(struct zsl_vec *a, struct zsl_vec *m,
			struct zsl_vec *g, zsl_real_t *alpha, struct zsl_quat *q)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the input vectors are tridimensional. Alpha must be
	 * between zero and one (included). */
	if ((a != NULL && (a->sz != 3)) || (m != NULL && (m->sz != 3)) ||
	    g->sz != 3 || *alpha < 0.0 || *alpha > 1.0) {
		rc = -EINVAL;
		goto err;
	}
	/* Make sure that the input quaternion is not zero. */
	if (ZSL_ABS(zsl_quat_magn(q)) < 1E-6) {
		rc = -EINVAL;
		goto err;
	}
#endif

	/* Normalize the input quaternion. */
	zsl_quat_to_unit_d(q);

	/* Estimate the orientation (q_w) using the angular velocity data from the
	 * gyroscope. */
	struct zsl_quat q_w;
	zsl_quat_from_ang_vel(g, q, 1.0 / zsl_fus_comp_freq, &q_w);

	/* Continue with the calculations only if the data from the accelerometer
	 * and magnetometer is valid (non zero). */
	if ((m != NULL) && (ZSL_ABS(zsl_vec_norm(m)) > 1E-6) &&
	    (a != NULL) && (ZSL_ABS(zsl_vec_norm(a)) > 1E-6)) {
		/* Estimate the orientation (q_am) using the acceleration and magnetic
		 * field data to calculate the attitude. Then convert this orientation
		 * to a unit quaternion. */
		struct zsl_attitude att;
		struct zsl_euler e;
		struct zsl_quat q_am;
		zsl_att_from_accelmag(a, m, &att);
		zsl_att_to_euler(&att, &e);
		zsl_quat_from_euler(&e, &q_am);

		/* The final estimation is a linear interpolation the previous
		 * estimations (q_w and q_am). Alpha is arbitrary and depends on the
		 * reliability of the sensors. For more accurate gyroscope data, alpha
		 * should be closer to zero. For more accurate accelerometer and
		 * magnetometer data, alpha should be closer to 1. */
		zsl_quat_lerp(&q_w, &q_am, *alpha, q);
	} else {
		/* If the data from the accelerometer and magnetometer is invalid, then
		 * set the output quaternion as just q_w. */
		q->r = q_w.r;
		q->i = q_w.i;
		q->j = q_w.j;
		q->k = q_w.k;
	}

	/* Normalize the output quaternion. */
	zsl_quat_to_unit_d(q);

err:
	return rc;
}

int zsl_fus_comp_init(uint32_t freq, void *cfg)
{
	int rc = 0;

	struct zsl_fus_comp_cfg *mcfg = cfg;

	(void)mcfg;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the sample frequency is positive. */
	if (freq <= 0.0) {
		rc = -EINVAL;
		goto err;
	}
#endif

	zsl_fus_comp_freq = freq;

err:
	return rc;
}

int zsl_fus_comp_feed(struct zsl_vec *a, struct zsl_vec *m,
		      struct zsl_vec *g, zsl_real_t *incl, struct zsl_quat *q, void *cfg)
{
	struct zsl_fus_comp_cfg *mcfg = cfg;

	if (mcfg->alpha < 0.0 || mcfg->alpha > 1.0) {
		return -EINVAL;
	}

	return zsl_fus_comp(a, m, g, &(mcfg->alpha), q);
}

void zsl_fus_comp_error(int error)
{
	/* ToDo: Log error in default handler. */
}
