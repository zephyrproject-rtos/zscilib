/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <zsl/zsl.h>
#include <zsl/physics/photons.h>

int
zsl_phy_photon_ener(zsl_real_t lambda, zsl_real_t *e)
{
	if (lambda <= 0) {
		*e = NAN;
		return -EINVAL;
	}

	*e = (ZSL_LIGHT_SPEED * ZSL_PLANCK) / (lambda * 1.602176634E-19);

	return 0;
}

int
zsl_phy_photon_mom(zsl_real_t lambda, zsl_real_t *p)
{
	if (lambda <= 0) {
		*p = NAN;
		return -EINVAL;
	}

	*p = ZSL_PLANCK / (lambda * 1.602176634E-19);

	return 0;
}

int
zsl_phy_photon_wavelength(zsl_real_t f, zsl_real_t *lambda)
{
	if (f <= 0) {
		*lambda = NAN;
		return -EINVAL;
	}

	*lambda = ZSL_LIGHT_SPEED / f;

	return 0;
}

int
zsl_phy_photon_frequency(zsl_real_t lambda, zsl_real_t *f)
{
	if (lambda <= 0) {
		*f = NAN;
		return -EINVAL;
	}

	*f = ZSL_LIGHT_SPEED / lambda;

	return 0;
}

int
zsl_phy_photon_photoelectric(zsl_real_t f, zsl_real_t w, zsl_real_t *kmax)
{
	if (f < 0 || w < 0) {
		*kmax = NAN;
		return -EINVAL;
	}

	*kmax = ((ZSL_PLANCK * f) / 1.602176634E-19) - w;

	return 0;
}
