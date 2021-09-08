/*
 * Copyright (c) 2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <zsl/zsl.h>
#include <zsl/orientation/orientation.h>

int zsl_att_to_vec(struct zsl_attitude *a, struct zsl_vec *v)
{
	int rc = 0;

	v->sz = 3;
	v->data = a->idx;

	return rc;
}

int zsl_att_to_euler(struct zsl_attitude *a, struct zsl_euler *e)
{
	int rc = 0;

	e->x = a->roll * ZSL_DEG_TO_RAD;
	e->y = a->pitch * ZSL_DEG_TO_RAD;
	e->z = a->yaw * ZSL_DEG_TO_RAD;

	return rc;
}

int zsl_att_from_euler(struct zsl_euler *e, struct zsl_attitude *a)
{
	int rc = 0;

	a->roll = e->x * ZSL_RAD_TO_DEG;
	a->pitch = e->y * ZSL_RAD_TO_DEG;
	a->yaw = e->z * ZSL_RAD_TO_DEG;
	a->status_bits =  0;

	return rc;
}

int zsl_att_from_accelmag(struct zsl_vec *accel, struct zsl_vec *mag,
			  struct zsl_attitude *a)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the accelerometer and magnetometer vectors are size 3. */
	if (accel->sz != 3 || mag->sz != 3) {
		rc = -EINVAL;
		goto err;
	}
#endif

	ZSL_VECTOR_DEF(mag_unit, 3);
	zsl_vec_copy(&mag_unit, mag);
	zsl_vec_to_unit(&mag_unit);

	struct zsl_attitude att;

	zsl_att_from_accel(accel, &att);

	zsl_real_t nom = mag_unit.data[2] * ZSL_SIN(att.pitch * ZSL_DEG_TO_RAD) -
			 mag_unit.data[1] * ZSL_COS(att.pitch * ZSL_DEG_TO_RAD);
	zsl_real_t den = mag_unit.data[0] * ZSL_COS(att.roll * ZSL_DEG_TO_RAD) +
			 ZSL_SIN(att.roll * ZSL_DEG_TO_RAD) *
			 (mag_unit.data[1] * ZSL_SIN(att.pitch * ZSL_DEG_TO_RAD) +
			  mag_unit.data[2] * ZSL_COS(att.pitch * ZSL_DEG_TO_RAD));

	a->roll = att.roll;
	a->pitch = att.pitch;
	a->yaw = ZSL_ATAN2(nom, den) * ZSL_RAD_TO_DEG;

#if CONFIG_ZSL_BOUNDS_CHECKS
err:
#endif
	return rc;
}

int zsl_att_from_accel(struct zsl_vec *accel, struct zsl_attitude *a)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the accelerometer vector is size 3. */
	if (accel->sz != 3) {
		rc = -EINVAL;
		goto err;
	}
#endif

	ZSL_VECTOR_DEF(accel_unit, 3);
	zsl_vec_copy(&accel_unit, accel);
	zsl_vec_to_unit(&accel_unit);

	zsl_real_t ss = accel_unit.data[1] * accel_unit.data[1] +
			accel_unit.data[2] * accel_unit.data[2];

	a->roll = ZSL_ATAN2(accel_unit.data[1], accel_unit.data[2])
		  * ZSL_RAD_TO_DEG;
	a->pitch = ZSL_ATAN2(-accel_unit.data[0], ZSL_SQRT(ss))
		   * ZSL_RAD_TO_DEG;
	a->yaw = 0.0;

#if CONFIG_ZSL_BOUNDS_CHECKS
err:
#endif
	return rc;
}

int zsl_att_accel_angle(struct zsl_vec *a1, struct zsl_vec *a2, zsl_real_t *b)
{
	int rc = 0;

#if CONFIG_ZSL_BOUNDS_CHECKS
	/* Make sure that the accelerometer vectors are size 3. */
	if (a1->sz != 3 || a2->sz != 3) {
		rc = -EINVAL;
		goto err;
	}
#endif

	zsl_real_t a1_norm, a2_norm, dot;

	a1_norm = zsl_vec_norm(a1);
	a2_norm = zsl_vec_norm(a2);
	zsl_vec_dot(a1, a2, &dot);

	*b = ZSL_ACOS(dot / (a1_norm * a2_norm));

#if CONFIG_ZSL_BOUNDS_CHECKS
err:
#endif
	return rc;
}
