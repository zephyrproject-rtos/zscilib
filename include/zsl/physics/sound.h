/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @addtogroup SOUND Sounds
 *
 * @brief Sound functions.
 *
 * @ingroup PHYSICS
 *  @{ */

/**
 * @file
 * @brief API header file for sound in zscilib.
 *
 * This file contains the zscilib sound APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_SOUND_H_
#define ZEPHYR_INCLUDE_ZSL_SOUND_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculates the mamimum acoustic pressure of a sound wave in pascals.
 *
 * @param b    Bulk Modulus in pascals.
 * @param k    Wave number of the sound wave in radians per second.
 * @param a    Amplitude of the sound wave in meters.
 * @param p    Pointer to the output mamimum acoustic pressure in pascals. Will
 *             be set to NAN if the Bulk Modulus, the wave number or the wave
 *             amplitude are negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_sound_press_amp(zsl_real_t b, zsl_real_t k, zsl_real_t a,
			    zsl_real_t *p);

/**
 * @brief Calculates the sound intensity level in decibels based on the input
 *        intensity and a reference intensity.
 *
 * @param i     Input sound intensity in watts per meter squared.
 * @param i0    Reference sound intensity in watts per meter squared.
 * @param b     Pointer to the sound intensity level in decibels. Will be set to
 *              NAN if either the input intensity or the reference intensity
 *              are negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_sound_level(zsl_real_t i, zsl_real_t i0, zsl_real_t *b);

/**
 * @brief Calculates the sound intensity in watts per meter squared based on the
 *        Bulk Modulus (b) and density (d) of the medium and the maximum
 *        pressure (p).
 *
 * @param b    Bulk Modulus in pascals.
 * @param d    Density in kilograms per meter cubed.
 * @param p    Amplitude of the acoustic pressure in pascals.
 * @param i    Pointer to the output sound intensity in watts per meter squared.
 *             Will be set to NAN if the Bulk Modulus, the density or the
 *             pressure are negative, or if the density is zero.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_sound_intensity(zsl_real_t b, zsl_real_t d, zsl_real_t p,
			    zsl_real_t *i);

/**
 * @brief Calculates the angle of the mach wave created by a moving sound
 *        source of velocity 'vs'.
 *
 * @param v       Velocity of the sound in the medium in meters per second.
 * @param vs      Velocity of the sound source in meters per second.
 * @param theta   Pointer to the output angle in radians.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_sound_shock_wave_angle(zsl_real_t v, zsl_real_t vs,
				   zsl_real_t *theta);

/**
 * @brief Calculates the frequency measured by a moving receptor while a sound
 *        emiting source moves towards it.
 *
 * @param v     Velocity of sound in the medium in meters per second.
 * @param vs    Velocity of the sound source in meters per second.
 * @param vl    Velocity of the sound receptor in meters per second.
 * @param fs    Frequency of the sound in the source in hertzs.
 * @param fl    Pointer to the output frequency in hertzs. Will be set to NAN if
 *              either the input or the output frequency is negative, or if
 *              v + vs is zero.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_sound_dop_effect(zsl_real_t v, zsl_real_t vs, zsl_real_t vl,
			     zsl_real_t fs, zsl_real_t *fl);

/**
 * @brief Calculates the frequency of the beats created by the interference of
 *        two waves with different frequency.
 *
 * @param fa     Frequency of the first wave in hertzs.
 * @param fb     Frequency of the second wave in hertzs.
 * @param f      Pointer to the frequency of the beats in hertzs. Will be set to
 *               NAN if any of the input frequencies is negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_sound_beat(zsl_real_t fa, zsl_real_t fb, zsl_real_t *f);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_SOUND_H_ */

/** @} */ /* End of sound group */
