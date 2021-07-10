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

	zsl_real_t d_to_r = 0.01745329252;

	e->x = a->roll * d_to_r;
	e->y = a->pitch * d_to_r;
	e->z = a->yaw * d_to_r;

	return rc;
}

int zsl_att_from_euler(struct zsl_euler *e, struct zsl_attitude *a)
{
	int rc = 0;

	zsl_real_t r_to_d = 57.295779513;

	a->roll = e->x * r_to_d;
	a->pitch = e->y * r_to_d;
	a->yaw = e->z * r_to_d;
	a->status_bits =  0;

	return rc;
}

int zsl_att_from_accelmag(struct zsl_vec *accel, struct zsl_vec *mag,
			  struct zsl_attitude *a)
{
	int rc = 0;

	/* ToDo: Check accel and gyro size, etc. */

	// Accel only
	//               y
	// roll = atan2(---)
	//               z

	// Accel only
	//                            -x
	// pitch = atan(-------------------------------)
	//               y * sin(roll) + z * cos(roll)

	// Mag + previous roll/pitch values
	//                --                                                    --
	// heading =      |         mz * sin(roll) - my * cos(roll)              |
	//          atan2 |------------------------------------------------------|
	//                | mx * cos(pitch) + my * sin(pitch) * sin(roll) + mz * |
	//                --            sin(pitch) * cos(roll))                 --

	return rc;
}

int zsl_att_from_accel(struct zsl_vec *accel, struct zsl_attitude *a)
{
	int rc = 0;

	/* ToDo: Check accel size, etc. */

	//               y
	// roll = atan2(---)
	//               z

	//                            -x
	// pitch = atan(-------------------------------)
	//               y * sin(roll) + z * cos(roll)

	return rc;
}