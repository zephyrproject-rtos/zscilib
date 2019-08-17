/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/**
 * @addtogroup ENERGY Energy
 *
 * @brief Energy functions.
 *
 * \ingroup PHYSICS
 *  @{ */

/**
 * @file
 * @brief API header file for energy in zscilib.
 *
 * This file contains the zscilib energy APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_ENERGY_H_
#define ZEPHYR_INCLUDE_ZSL_ENERGY_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculates the kinetic energy of a moving object based on its
 *        mass (m) and velocity (v).
 *
 * @param v    Velocity in meters per second.
 * @param m    Mass of the object in kilograms.
 * @param ke   Pointer to the output kinetic energy in joules. Will be set to
 *             NAN if the mass is negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_ener_kin(zsl_real_t v, zsl_real_t m, zsl_real_t *ke);

/**
 * @brief Calculates the angular kinetic energy of a rotating object based on
 *        its angular velocity (omega) and moment of inertia (i).
 *
 * @param omega   Angular velocity in radians per second.
 * @param i       Moment of inertia in kilograms and meters squared.
 * @param rke     Pointer to the output angular kinetic energy in joules. Will 
 *                be set to NAN if the moment of inertia of the object
 *                is negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_ener_rot(zsl_real_t omega, zsl_real_t i, zsl_real_t *rke);

/**
 * @brief Calculates the gravitational potential energy in joules of an object
 *        based on its mass (m) and its height (h) with respect to a chosen
 *        initial height.
 *
 * @param m     Mass of the body in kilograms.
 * @param h     Height in meters.
 * @param ug    Pointer to the output gravitational potential energy in joules.
 *              Will be set to NAN if the mass of the body is negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_ener_grav_pot(zsl_real_t m, zsl_real_t h, zsl_real_t *ug);


/**
 * @brief Calculates the elastic potential energy of a spring based on the 
distance it has been compressed/expanded and its characteristic 'k' constant.
 *
 * @param k      Constant characteristic of the spring, in newtons per meter.
 * @param dist   Distance compressed/expanded in meters.
 * @param ue     Pointer to the output elastic potential energy in joules. Will
 *               be set to NAN if 'k' is negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_ener_elas_pot(zsl_real_t k, zsl_real_t dist, zsl_real_t *ue);

/**
 * @brief Calculates the power by dividing energy or work by time.
 *
 * @param ener   Energy or work supplied in joules.
 * @param time   Time in seconds..
 * @param power  Pointer to the output power in watts. Will be set to NAN if
 *               the time is zero or negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_ener_power(zsl_real_t ener, zsl_real_t time, zsl_real_t *power);

/**
 * @brief Calculates the energy lost due to a friction force based on the
 *        distance the object experiencing the friction has moved.
 *
 * @param fric   Module of the friction force in newtons.
 * @param dist   Distance through which the force has been applied, in meters.
 * @param e      Pointer to the output lost energy in joules. Will be set to
 *               NAN if either the friction force or the distance are negatives.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_ener_fric(zsl_real_t fric, zsl_real_t dist, zsl_real_t *e);

/**
 * @brief Calculates the total mechanical energy of a system by adding kinetic
 *        energy (linear and angular) and potential energy (gravitational and
 *        elastic).
 *
 * @param ke     Linear kinetic energy (AKA half of the mass times velocity
 *               squared) in joules.
 * @param rke    Angular kinetic energy (AKA half of the moment of inertia
 *               times angular velocity squared) in joules.
 * @param ug     Gravitational potential energy in joules.
 * @param ue     Elastic potential energy in joules.
 * @param me     Pointer to the output mechanical energy in joules.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_ener_mec(zsl_real_t ke, zsl_real_t rke, zsl_real_t ug,
		     zsl_real_t ue, zsl_real_t *me);

/**
 * @brief Given the initial values of the two kinetic (linear and rotational)
 *        and the two potential (gravitational and elastic) energies, and three
 *        of the four final values of these energies, this function calculates
 *        the fourth value of the unknown final energy using the conservation 
 *        of mechanical energy.
 *
 * @param ei1    Initial value of one of the energies in joules.
 * @param ei2    Initial value of one of the energies in joules.
 * @param ei3    Initial value of one of the energies in joules.
 * @param ei4    Initial value of one of the energies in joules.
 * @param ef1    Final value of one of the energies in joules.
 * @param ef2    Final value of one of the energies in joules.
 * @param ef3    Final value of one of the energies in joules.
 * @param ef4    Pointer to the output unknown final energy in joules.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_ener_final(zsl_real_t ei1, zsl_real_t ei2, zsl_real_t ei3,
		       zsl_real_t ei4, zsl_real_t ef1, zsl_real_t ef2,
		       zsl_real_t ef3, zsl_real_t *ef4);

/**
 * @brief Calculates the force in newtons that experiments a body based on its
 *        mass (m) and acceleration (accel).
 *
 * @param m      Mass of the body in kilograms.
 * @param accel  Acceleration in meters per second squared.
 * @param f      Pointer to the output force in newtons. Will be set to NAN if
 *               the mass of the body is negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_ener_photon(zsl_real_t f, zsl_real_t *e);

/**
 * @brief Calculates the force in newtons that experiments a body based on its
 *        mass (m) and acceleration (accel).
 *
 * @param m      Mass of the body in kilograms.
 * @param accel  Acceleration in meters per second squared.
 * @param f      Pointer to the output force in newtons. Will be set to NAN if
 *               the mass of the body is negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_ener_photon2(zsl_real_t lambda, zsl_real_t *e);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_ENERGY_H_ */

/** @} */ /* End of energy group */
