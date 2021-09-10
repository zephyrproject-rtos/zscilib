/*
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <zsl/orientation/fusion/fusion.h>
#include <zsl/orientation/fusion/saam.h>

static uint32_t zsl_fus_saam_freq;

static int zsl_fus_saam(struct zsl_vec *a, struct zsl_vec *m,
			struct zsl_quat *q)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the input vectors are tridimensional. */
	if (a->sz != 3 || m->sz != 3) {
		rc = -EINVAL;
		goto err;
	}
#endif

	/* Return an error if either of the data in the sensors is missing or
	 * zero. */
	if ((m == NULL) || (ZSL_ABS(zsl_vec_norm(m)) < 1E-6) ||
	    (a == NULL) || (ZSL_ABS(zsl_vec_norm(a)) < 1E-6)) {
		rc = -EINVAL;
		goto err;
	}

	/* Normalize the acceleration vector. */
	zsl_vec_to_unit(a);

	/* Normalize the magnetic field vector. */
	zsl_vec_to_unit(m);

	/* Determine the x (north) and z (vertical) components of the magnetic
	 * field of the earth on the earth's reference frame. */
	zsl_real_t bz;
	zsl_vec_dot(a, m, &bz);
	zsl_real_t bx = ZSL_SQRT(1.0 - bz * bz);

	/* Calculate the orientation quaternion. */
	q->r = -a->data[1] * (bx + m->data[0]) + a->data[0] * m->data[1];
	q->i = (a->data[2] - 1.0) * (bx + m->data[0]) +
	       a->data[0] * (bz - m->data[2]);
	q->j = (a->data[2] - 1.0) * m->data[1] + a->data[1] * (bz - m->data[2]);
	q->k = a->data[2] * bz - a->data[0] * bx - m->data[2];

	/* Normalize the output quaternion. */
	zsl_quat_to_unit_d(q);

err:
	return rc;
}



int zsl_fus_saam_init(uint32_t freq, void *cfg)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the sample frequency is positive. */
	if (freq <= 0.0) {
		rc = -EINVAL;
		goto err;
	}
#endif

	zsl_fus_saam_freq = freq;

err:
	return rc;
}

int zsl_fus_saam_feed(struct zsl_vec *a, struct zsl_vec *m,
		      struct zsl_vec *g, zsl_real_t *incl, struct zsl_quat *q, void *cfg)
{
	return zsl_fus_saam(a, m, q);
}

void zsl_fus_saam_error(int error)
{
	/* ToDo: Log error in default handler. */
}
