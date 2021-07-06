/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @addtogroup ATOMIC Atomic
 *
 * @brief Atomic functions.
 *
 * @ingroup PHYSICS
 *  @{ */

/**
 * @file
 * @brief API header file for atomics in zscilib.
 *
 * This file contains the zscilib atomic APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_ATOMIC_H_
#define ZEPHYR_INCLUDE_ZSL_ATOMIC_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculates an approximation of the nuclear radius of an atom of mass
 *        number 'a'.
 *
 * @param a      Mass number of the atom.
 * @param r      Pointer to the output radius in femtometers. Will be set to
 *               NAN if the mass number is zero.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_atom_nucl_radius(uint8_t a, zsl_real_t *r);

/**
 * @brief Calculates the radius of the orbit of an electron in the n-th orbit
 *        of an atom with 'z' atomic number, according to the atomic Bohr model.
 *
 * @param z      Atomic number of the atom.
 * @param n      Number of the orbit.
 * @param r      Pointer to the output radius in nanometers. Will be set to
 *               NAN if the atomic is zero.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_atom_bohr_orb_radius(uint8_t z, uint8_t n, zsl_real_t *r);

/**
 * @brief Calculates the velocity in kilometers per second of an electron in the
 *        n-th orbit of an atom with 'z' atomic number, according to the Bohr
 *        model of the atom.
 *
 * @param z      Atomic number of the atom.
 * @param n      Number of the orbit.
 * @param v      Pointer to the output velocity in kilometers per second. Will
 *               be set to NAN if the atomic number is zero.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_atom_bohr_orb_vel(uint8_t z, uint8_t n, zsl_real_t *v);

/**
 * @brief Calculates the energy in electronvolts of an electron in the n-th
 *        orbit of an atom with 'z' atomic number, according to the Bohr model
 *        of the atom.
 *
 * @param z      Atomic number of the atom.
 * @param n      Number of the orbit.
 * @param e      Pointer to the output energy in electronvolts. Will be set to
 *               NAN if the atomic number is zero.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_atom_bohr_orb_ener(uint8_t z, uint8_t n, zsl_real_t *e);

/**
 * @brief Calculates the radioactive activity in becquerels of a sample of
 *        radioactive material based on the initial activity (qi), the time (t)
 *        and the radioactive decay constant (lambda).
 *
 * @param qi      Initial radioactive activity in becquerels.
 * @param t       Time in seconds.
 * @param lambda  Radioactive decay constant defined as nuclear desintegrations
 *                per second.
 * @param qf      Pointer to the output radioactivity in becquerels. Will be
 *                set to NAN if either lambda or the time are negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_atom_rad_decay(zsl_real_t qi, zsl_real_t t, zsl_real_t lambda,
			   zsl_real_t *qf);

/**
 * @brief Calculates the average distance in meters between the atoms of a
 *        crystalline system according to the Bragg's condition of constructive
 *        interferences.
 *
 * @param n       Positive integer.
 * @param theta   Angle of incision in radians.
 * @param lambda  Wavelenght in meters.
 * @param d       Pointer to the output distance in meters. Will be set to
 *                NAN if lambda is negative or if sin(theta) is negative
 *                or zero.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_atom_bragg(uint8_t n, zsl_real_t theta, zsl_real_t lambda,
		       zsl_real_t *d);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_ATOMIC_H_ */

/** @} */ /* End of atomic group */
