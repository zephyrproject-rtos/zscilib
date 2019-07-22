/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <kernel.h>
#include <zsl/zsl.h>
#include <zsl/physics/kinematics.h>

int
zsl_phy_kin_dist(zsl_real_t vi, zsl_real_t time, zsl_real_t accel,
		 zsl_real_t *dist)
{
	if (time < 0) {
		*dist = NAN;
		return -EINVAL;
	}

	*dist = vi * time + 0.5 * accel * time * time;

	return 0;
}

int
zsl_phy_kin_time(zsl_real_t vi, zsl_real_t vf, zsl_real_t accel,
		 zsl_real_t *time)
{
	*time = (vf - vi) / accel;

	if (*time < 0 || accel == 0) {
		*time = NAN;
		return -EINVAL;
	}

	return 0;
}

int
zsl_phy_kin_vel(zsl_real_t vi, zsl_real_t time, zsl_real_t accel,
		zsl_real_t *vf)
{
	*vf = vi + time * accel;

	if (time < 0) {
		*vf = NAN;
		return -EINVAL;
	}

	return 0;
}

int
zsl_phy_kin_vel2(zsl_real_t vi, zsl_real_t dist, zsl_real_t accel,
		 zsl_real_t *vf)
{
	zsl_real_t vf2 = vi * vi + 2 * accel * dist;

	if (vf2 < 0) {
		*vf = NAN;
		return -EINVAL;
	}

	*vf = sqrt(vf2);

	return 0;
}

int
zsl_phy_kin_av_vel(zsl_real_t dist, zsl_real_t time, zsl_real_t *vel)
{
	if (time <= 0) {
		*vel = NAN;
		return -EINVAL;
	}

	*vel = dist / time;

	return 0;
}

int
zsl_phy_kin_accel(zsl_real_t vi, zsl_real_t vf, zsl_real_t time,
		  zsl_real_t *accel)
{
	if (time <= 0) {
		*accel = NAN;
		return -EINVAL;
	}

	*accel = (vf - vi) / time;

	return 0;
}

int
zsl_phy_kin_ener(zsl_real_t v, zsl_real_t m, zsl_real_t *ke)
{
	if (m <= 0) {
		*ke = NAN;
		return -EINVAL;
	}

	*ke = 0.5 * m * v * v;

	return 0;
}
