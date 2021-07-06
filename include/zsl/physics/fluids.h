/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @addtogroup FLUIDS Fluids
 *
 * @brief Fluids functions.
 *
 * @ingroup PHYSICS
 *  @{ */

/**
 * @file
 * @brief API header file for fluids in zscilib.
 *
 * This file contains the zscilib fluids APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_FLUIDS_H_
#define ZEPHYR_INCLUDE_ZSL_FLUIDS_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculates the density of a material based on its mass and volume.
 *
 * @param m    Mass of the material in kilograms.
 * @param v    Volume of the body in meters cubed.
 * @param d    Pointer to the output density in kilograms per meter cubed. Will
 *             be set to NAN if any of the input values is negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_fluid_dens(zsl_real_t m, zsl_real_t v, zsl_real_t *d);

/**
 * @brief Calculates the pressure in pascals based on a force (f) against a
 *        surface (a).
 *
 * @param f    Force in newtons.
 * @param a    Area in meters squared.
 * @param p    Pointer to the output pressure in pascals. Will be set to NAN if
 *             any of the input values is negative or if a = 0.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_fluid_simple_press(zsl_real_t f, zsl_real_t a, zsl_real_t *p);

/**
 * @brief Calculates pressure in pascals of a body submerged 'h' meters down
 *        the surface of a fluid, given the pressure of the fluid at its surface
 *        and its density.
 *
 * @param p0    Pressure on the surface of the fluid in kilopascals.
 * @param h     Distance to the surface of the fluid in meters.
 * @param d     Density of the fluid in kilograms per meter cubed.
 * @param p     Pointer to the output pressure in kilopascals. Will be set to
 *              NAN if any of the input values is negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_fluid_press(zsl_real_t p0, zsl_real_t h, zsl_real_t d,
			zsl_real_t *p);

/**
 * @brief Calculates the module of the upwards force experienced by a body
 *        submerged into a fluid, AKA bouyant force.
 *
 * @param v    Volume of the submerged body in meters cubed.
 * @param d    Density of the fluid in kilograms per meter cubed.
 * @param f    Pointer to the output bouyant force in kilonewtons. Will be set
 *             to NAN if any of the input values is negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_fluid_bouyant_force(zsl_real_t v, zsl_real_t d, zsl_real_t *f);

/**
 * @brief Given two sections of a pipe with different surfaces 'a1' and 'a2' and
 *        knowing the value of the velocity of the fluid when passing through
 *        'a1', this function calculates the velocity of the fluid in 'a2'.
 *
 * @param a1    Area of the first section of the pipe in meters squared.
 * @param v1    Velocity of the fluid in 'a1' in meters per second.
 * @param a2    Area of the second section of the pipe in meters squared.
 * @param v2    Pointer to the output velocity in meters per second. Will be
 *              set to NAN if any of the input values is negative or if a2 = 0.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_fluid_flow_rate(zsl_real_t a1, zsl_real_t v1, zsl_real_t a2,
			    zsl_real_t *v2);

/**
 * @brief Given two sections of a pipe with different surfaces 'a1' and 'a2' and
 *        knowing the value of the force with which the fluid goes through 'a1',
 *        this function calculates the force with which the fluid goes
 *        through 'a2'.
 *
 * @param a1    Area of the first section of the pipe in meters squared.
 * @param f1    The force with which the fluid goes through 'a1' in newtons.
 * @param a2    Area of the second section of the pipe in meters squared.
 * @param f2    Pointer to the output force in newtons. Will be set to NAN if
 *              any of the input values is negative or if a1 = 0.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_fluid_force_prop(zsl_real_t a1, zsl_real_t f1, zsl_real_t a2,
			     zsl_real_t *f2);

/**
 * @brief Given a pipe in which a fluid flows at different velocities 'v1' and
 *        'v2' in two different sections and knowing the pressure of the fluid
 *        in the first section (p1), this function calculates the pressure of
 *        fluid in the second section.
 *
 * @param d     Density of the fluid in kilograms per meter cubed.
 * @param v1    Velocity in the first section in meters per second.
 * @param p1    Pressure in the first section in kilopascals.
 * @param v2    Velocity in the second section in meters per second.
 * @param p2    Pointer to the output pressure in kilopascals. Will be set to
 *              NAN ifany of the input values is negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_fluid_bernoulli(zsl_real_t d, zsl_real_t v1, zsl_real_t p1,
			    zsl_real_t v2, zsl_real_t *p2);

/**
 * @brief Calculates the volume flow rate of a flowing fluid based on its
 *        velocity (vel) in a section of surface (a).
 *
 * @param vel   Velocity of the fluid in meters per second.
 * @param a     Area of the section in meters squared.
 * @param v     Pointer to the output volume flow rate in meters cubed per
 *              second. Will be set to NAN if any of the input values is
 *              negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_fluid_volume_flow_rate(zsl_real_t vel, zsl_real_t a, zsl_real_t *v);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_FLUIDS_H_ */

/** @} */ /* End of fluids group */
