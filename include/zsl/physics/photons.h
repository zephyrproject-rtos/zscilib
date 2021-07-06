/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @addtogroup PHOTONS Photons
 *
 * @brief Photon-related functions.
 *
 * @ingroup PHYSICS
 *  @{ */

/**
 * @file
 * @brief API header file for photons in zscilib.
 *
 * This file contains the zscilib photons APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_PHOTONS_H_
#define ZEPHYR_INCLUDE_ZSL_PHOTONS_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculates the energy of a photon in electronvolts.
 *
 * @param lambda   Wavelength of the photon in meters.
 * @param e        Pointer to the output energy in electronvolts. Will be set to
 *                 NAN if the wavelength is zero or negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_photon_ener(zsl_real_t lambda, zsl_real_t *e);

/**
 * @brief Calculates the linear momentum of a photon in electronvolts and
 *        seconds per meter.
 *
 * @param lambda   Wavelength of the photon in meters.
 * @param p        Pointer to the output momentum in electronvolts and seconds
 *                 per meter. Will be set to NAN if the wavelength is zero or
 *                 negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_photon_mom(zsl_real_t lambda, zsl_real_t *p);

/**
 * @brief Calculates the wavelength of a photon in meters given its frequency.
 *
 * @param f        Frequency of the photon in hertz.
 * @param lambda   Pointer to the wavelength in meters. Will be set to NAN if
 *                 the frequency is zero or negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_photon_wavelength(zsl_real_t f, zsl_real_t *lambda);

/**
 * @brief Calculates the frequency of a photon in hertz given its wavelength.
 *
 * @param lambda   Wavelength of the photon in meters.
 * @param f        Pointer to the frequency in hertz. Will be set to NAN if
 *                 the wavelength is zero or negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_photon_frequency(zsl_real_t lambda, zsl_real_t *f);

/**
 * @brief Calculates the maximum kinetic energy in electronvolts of the
 *        electrons released in the photoelectric effect, when a light beam of
 *        frequency 'f' hits a material of work function 'w'.
 *
 * @param f      Frequency of the incident photons in hertzs.
 * @param w      Work function of the material in electronvolts.
 * @param kmax   Pointer to the output kinetic energy in electronvolts. Will be
 *               set to NAN if either 'f' or 'w' are negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_photon_photoelectric(zsl_real_t f, zsl_real_t w, zsl_real_t *kmax);



#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_PHOTONS_H_ */

/** @} */ /* End of photons group */
