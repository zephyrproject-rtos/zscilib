/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <zsl/zsl.h>
#include <zsl/physics/sound.h>

int
zsl_phy_sound_press_amp(zsl_real_t b, zsl_real_t k, zsl_real_t a, zsl_real_t *p)
{
	if (b < 0 || k < 0 || a < 0) {
		*p = NAN;
		return -EINVAL;
	}

	*p = b * k * a;

	return 0;
}

int
zsl_phy_sound_level(zsl_real_t i, zsl_real_t i0, zsl_real_t *b)
{
	if (i < 0 || i0 < 0) {
		*b = NAN;
		return -EINVAL;
	}

	*b = 10 * ZSL_LOG10(i / i0);

	return 0;
}

int
zsl_phy_sound_intensity(zsl_real_t b, zsl_real_t d, zsl_real_t p, zsl_real_t *i)
{
	if (b < 0 || d <= 0 || p < 0) {
		*i = NAN;
		return -EINVAL;
	}
	
	*i = (p * p * ZSL_SQRT(b / d)) / (2 * b);

	return 0;
}

int
zsl_phy_sound_shock_wave_angle(zsl_real_t v, zsl_real_t vs, zsl_real_t *theta)
{
	*theta = ZSL_ASIN(v / vs);

	return 0;
}

int
zsl_phy_sound_dop_effect(zsl_real_t v, zsl_real_t vs, zsl_real_t vl,
			 zsl_real_t fs, zsl_real_t *fl)
{
	if (fs < 0 || (v + vs) == 0) {
		*fl = NAN;
		return -EINVAL;
	}
	
	*fl = (fs * (v + vl)) / (v + vs);
	
	if (*fl < 0) {
		*fl = NAN;
		return -EINVAL;
	}

	return 0;
}

int
zsl_phy_sound_beat(zsl_real_t fa, zsl_real_t fb, zsl_real_t *f)
{
	if (fa < 0 || fb < 0) {
		*f = NAN;
		return -EINVAL;
	}
	
	*f = ZSL_ABS(fa - fb);

	return 0;
}
