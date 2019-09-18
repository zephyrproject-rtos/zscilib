/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <zsl/zsl.h>
#include <zsl/physics/rotation.h>

int
zsl_phy_rot_angle(zsl_real_t oi, zsl_real_t time, zsl_real_t a, zsl_real_t *phi)
{
	if (time < 0) {
		*phi = NAN;
		return -EINVAL;
	}

	*phi = oi * time + 0.5 * a * time * time;

	return 0;
}

int
zsl_phy_rot_dist(zsl_real_t phi, zsl_real_t r, zsl_real_t *dist)
{
	if (r < 0) {
		*dist = NAN;
		return -EINVAL;
	}

	*dist = phi * r;

	return 0;
}

int
zsl_phy_rot_turn(zsl_real_t phi, zsl_real_t *turn)
{
	*turn = phi / (2.0 * ZSL_PI);

	return 0;
}

int
zsl_phy_rot_time(zsl_real_t oi, zsl_real_t of, zsl_real_t a, zsl_real_t *time)
{
	*time = (of - oi) / a;

	if (*time < 0 || a == 0) {
		*time = NAN;
		return -EINVAL;
	}

	return 0;
}

int
zsl_phy_rot_omega(zsl_real_t oi, zsl_real_t time, zsl_real_t a, zsl_real_t *of)
{
	*of = oi + time * a;

	if (time < 0) {
		*of = NAN;
		return -EINVAL;
	}

	return 0;
}

int
zsl_phy_rot_omega2(zsl_real_t oi, zsl_real_t phi, zsl_real_t a, zsl_real_t *of)
{
	zsl_real_t of2 = oi * oi + 2.0 * a * phi;

	if (of2 < 0) {
		*of = NAN;
		return -EINVAL;
	}

	*of = ZSL_SQRT(of2);

	return 0;
}

int
zsl_phy_rot_av_omega(zsl_real_t phi, zsl_real_t time, zsl_real_t *omega)
{
	if (time <= 0) {
		*omega = NAN;
		return -EINVAL;
	}

	*omega = phi / time;

	return 0;
}

int
zsl_phy_rot_vel(zsl_real_t omega, zsl_real_t r, zsl_real_t *vel)
{
	if (r < 0) {
		*vel = NAN;
		return -EINVAL;
	}

	*vel = omega * r;

	return 0;
}

int
zsl_phy_rot_ang_accel(zsl_real_t oi, zsl_real_t of, zsl_real_t time,
		      zsl_real_t *a)
{
	if (time <= 0) {
		*a = NAN;
		return -EINVAL;
	}

	*a = (of - oi) / time;

	return 0;
}

int
zsl_phy_rot_accel(zsl_real_t a, zsl_real_t r, zsl_real_t *accel)
{
	if (r < 0) {
		*accel = NAN;
		return -EINVAL;
	}

	*accel = a * r;

	return 0;
}

int
zsl_phy_rot_ener(zsl_real_t omega, zsl_real_t i, zsl_real_t *rke)
{
	if (i < 0) {
		*rke = NAN;
		return -EINVAL;
	}

	*rke = 0.5 * i * omega * omega;

	return 0;
}

int
zsl_phy_rot_period(zsl_real_t omega, zsl_real_t *t)
{
	if (omega <= 0) {
		*t = NAN;
		return -EINVAL;
	}

	*t = (2.0 * ZSL_PI) / omega;

	return 0;
}

int
zsl_phy_rot_frequency(zsl_real_t omega, zsl_real_t *f)
{
	if (omega < 0) {
		*f = NAN;
		return -EINVAL;
	}

	*f = omega / (2.0 * ZSL_PI);

	return 0;
}

int
zsl_phy_rot_cent_accel(zsl_real_t vel, zsl_real_t r, zsl_real_t *ca)
{
	if (r <= 0) {
		*ca = NAN;
		return -EINVAL;
	}

	*ca = (vel * vel) / r;

	return 0;
}

int
zsl_phy_rot_cent_accel2(zsl_real_t t, zsl_real_t r, zsl_real_t *ca)
{
	if (r < 0 || t <= 0) {
		*ca = NAN;
		return -EINVAL;
	}

	*ca = (4.0 * ZSL_PI * ZSL_PI * r) / (t * t);

	return 0;
}

int
zsl_phy_rot_total_accel(zsl_real_t accel, zsl_real_t ca, zsl_real_t *at)
{
	*at = ZSL_SQRT(accel * accel + ca * ca);

	return 0;
}

int
zsl_phy_rot_power(zsl_real_t torque, zsl_real_t omega, zsl_real_t *power)
{
	*power = torque * omega;

	return 0;
}
