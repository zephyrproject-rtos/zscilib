/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @addtogroup ELECTRIC Electric
 *
 * @brief Electric functions.
 *
 * @ingroup PHYSICS
 *  @{ */

/**
 * @file
 * @brief API header file for electrics in zscilib.
 *
 * This file contains the zscilib electric APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_ELECTRIC_H_
#define ZEPHYR_INCLUDE_ZSL_ELECTRIC_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculates the uniform charge density of an charged body based on its
 *        charge and its volume.
 *
 * @param q    Charge of the body in coulombs.
 * @param v    Volume of the body in meters cubed.
 * @param d    Pointer to the output uniform charge density in coulombs per
 *             meter cubed. Will be set to NAN if the volume is negative
 *             or zero.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_elec_charge_dens(zsl_real_t q, zsl_real_t v, zsl_real_t *d);

/**
 * @brief Calculates the electric force in newtons between two charged
 *        bodies based on their charges and the distance between them.
 *
 * @param q1   Charge of the first body in coulombs.
 * @param q2   Charge of the second body in coulombs.
 * @param r    Distance between the two bodies.
 * @param f    Pointer to the output electric force in newtons. Will be set to
 *             NAN if the distance is negative or zero.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_elec_force(zsl_real_t q1, zsl_real_t q2, zsl_real_t r,
		       zsl_real_t *f);

/**
 * @brief Calculates the electric force in newtons that a charged body
 *        experiences when it enters an electric field.
 *
 * @param q    Charge of the body in coulombs.
 * @param e    Electric field in newtons per coulomb.
 * @param f    Pointer to the output electric force in newtons.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_elec_force2(zsl_real_t q, zsl_real_t e, zsl_real_t *f);

/**
 * @brief Calculates the electric field created by a charged body anywhere in
 *        space.
 *
 * @param q    Charge of the body in coulombs.
 * @param r    Distance to the body in meters.
 * @param e    Pointer to the output electric field in newtons per coulomb.
 *             Will be set to NAN if the distance is negative or zero.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_elec_field(zsl_real_t q, zsl_real_t r, zsl_real_t *e);

/**
 * @brief Calculates the electric potential energy in joules of two charged
 *        bodies based on their charges and the distance between them.
 *
 * @param q1   Charge of the first body in coulombs.
 * @param q2   Charge of the second body in coulombs.
 * @param r    Distance between the two bodies.
 * @param u    Pointer to the output potential energy in joules. Will be set to
 *             NAN if the distance is negative or zero.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_elec_pot_ener(zsl_real_t q1, zsl_real_t q2, zsl_real_t r,
			  zsl_real_t *u);

/**
 * @brief Calculates the Coulomb's potential of a charged body anywhere in
 *        space.
 *
 * @param q    Charge of the body in coulombs.
 * @param r    Distance to the body in meters.
 * @param v    Pointer to the output potential in volts. Will be set to NAN if
 *             the distance is negative or zero.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_elec_potential(zsl_real_t q, zsl_real_t r, zsl_real_t *v);

/**
 * @brief Calculates the electric flux that flows across a surface based on the
 *        area (a), the electric field (b) and the angle they form.
 *
 * @param e       Module of the electric field vector in newtons per coulomb.
 * @param a       Surface in meters squared.
 * @param theta   Angle between the vector normal to the surface and the
 *                electric field vector in radians.
 * @param fl      Pointer to the output electric flux in newtons and meters
 *                squared per coulomb. Will be set to NAN if the area or the
 *                electric field are negative values.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_elec_flux(zsl_real_t e, zsl_real_t a, zsl_real_t theta,
		      zsl_real_t *fl);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_ELECTRIC_H_ */

/** @} */ /* End of electric group */
