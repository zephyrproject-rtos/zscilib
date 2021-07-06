/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @addtogroup THERMODYNAMICS Thermo-dynamics
 *
 * @brief Thermo-dynamic functions.
 *
 * @ingroup PHYSICS
 *  @{ */

/**
 * @file
 * @brief API header file for thermo-dynamics in zscilib.
 *
 * This file contains the zscilib thermo-dynamics APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_THERMO_H_
#define ZEPHYR_INCLUDE_ZSL_THERMO_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief If 'ctf' is set to true, this function transforms the celcius input
 *        temperature into fahrenheit. If 'ctf' is set to false, this function
 *        transforms the fahrenheit input temperature into celcius.
 *
 * @param t1    Input temperature.
 * @param ctf   If set to true, reverses the function.
 * @param t2    Pointer to the output temperature.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_thermo_fahren_cels(zsl_real_t t1, zsl_real_t *t2, bool ctf);

/**
 * @brief If 'ktc' is set to true, this function transforms the kelvin input
 *        temperature into celcius. If 'ktc' is set to false, this function
 *        transforms the celcius input temperature into kelvin.
 *
 * @param t1    Input temperature.
 * @param ktc   If set to true, reverses the function.
 * @param t2    Pointer to the output temperature.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_thermo_cels_kel(zsl_real_t t1, zsl_real_t *t2, bool ktc);

/**
 * @brief Calculates the necessary heat to melt a material based on its latent
 *        heat (lh) and mass (m).
 *
 * @param m    Mass of the material in kilograms.
 * @param lh   Melting latent heat of the material in kilojoules per kilogram.
 * @param q    Pointer to the output heat in kilojoules. Will be set to NAN if
 *             any of the input values is negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_thermo_heat_fusion(zsl_real_t m, zsl_real_t lh, zsl_real_t *q);

/**
 * @brief Calculates the amount of heat required to increase the temperature of
 *        a material.
 *
 * @param m    Mass of the material in kilograms.
 * @param sh   Specific heat of the material in kilojoules per kilogram and
 *             kelvin.
 * @param t    Increment of temperature in kelvins.
 * @param q    Pointer to the output amount of heat in kilojoules. Will be set
 *             to NAN if either the mass or the specific heat are negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_thermo_heat(zsl_real_t m, zsl_real_t sh, zsl_real_t t,
			zsl_real_t *q);

/**
 * @brief Calculates the final length of an object under heat expansion based
on its initial length, its linear thermal expansion coeffitient and the
difference in temperature.
 *
 * @param l0   Initial length in meters.
 * @param a    Linear thermal expansion coeffitient of the material
 *             in 1 / kelvin.
 * @param t    Increment of temperature in kelvin.
 * @param l    Pointer to the output final length in meters. Will be set to NAN
 *             if either 'l0' or 'a' are negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_thermo_expan(zsl_real_t l0, zsl_real_t a, zsl_real_t t,
			 zsl_real_t *l);

/**
 * @brief Calculates the average distance between the collisions of molecules
 *        in a gas based on the number of particles per unit volume and the
 *        diameter of the gas molecules.
 *
 * @param nv       Number of particles per unit volume in 1 / meter cubed.
 * @param d        Diameter of the molecules in meters.
 * @param lambda   Pointer to the output distance between collisions in
 *                 micrometers. Will be set to NAN if any of the input values is
 *                 zero or negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_thermo_mean_free_path(zsl_real_t nv, zsl_real_t d,
				  zsl_real_t *lambda);

/**
 * @brief Calculates the efficiency of a heat engine based on the energy of the
 *        cold (qc) and the hot (qh) chambers.
 *
 * @param qc    Energy of the cold chamber in joules.
 * @param qh    Energy of the hot chamber in joules.
 * @param e     Pointer to the output efficiency. Will be set to NAN if qh = 0
 *              or if the energy of the cold chamber is greater than the energy
 *              of the hot chamber.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_thermo_effic_heat_engine(zsl_real_t qc, zsl_real_t qh,
				     zsl_real_t *e);

/**
 * @brief Calculates the energy of the hot chamber of a Carnot engine when the
 *        energy of the cold chamber and the temperatures in both chambers are
 *        known values.
 *
 * @param tc    Temperature in the cold chamber in kelvins.
 * @param qc    Energy of the cold chamber in joules.
 * @param th    Temperature in the hot chamber in kelvins.
 * @param qh    Pointer to the output energy of the hot chamber in joules. Will
 *              be set to NAN if any of the temperatures is negative or
 *              if tc = 0.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_thermo_carnot_engine(zsl_real_t tc, zsl_real_t qc, zsl_real_t th,
				 zsl_real_t *qh);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_THERMO_H_ */

/** @} */ /* End of thermo-dynamics group */
