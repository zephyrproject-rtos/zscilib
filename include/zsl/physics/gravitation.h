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
 * @addtogroup GRAVITATION Gravitation
 *
 * @brief Gravitation functions.
 *
 * \ingroup PHYSICS
 *  @{ */

/**
 * @file
 * @brief API header file for gravitation in zscilib.
 *
 * This file contains the zscilib gravitation APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_GRAVITATION_H_
#define ZEPHYR_INCLUDE_ZSL_GRAVITATION_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculates the orbital period of a satelite orbiting another body in
 *        seconds based on the distance between the bodies (r) and their masses.
 *
 * @param mp        Mass of the central body in kilograms.
 * @param msat      Mass of the orbiting body in kilograms.
 * @param r         Semimajor axis of the orbit in meters (if the orbit is
 *                  circular, this will be equal to the radius of the orbit).
 * @param t         Pointer to the output orbital period in seconds. Will be
 *                  set to NAN if the sum of both masses is zero.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_grav_orb_period(zsl_real_t mp, zsl_real_t msat,
			    zsl_real_t r, zsl_real_t *t);

/**
 * @brief Calculates the velocity needed for an object to overcome the
 *        gravitational pull of the planet it is on in meters per second based
 *        on the mass (mp) and the radius (r) of the planet.
 *
 * @param mp    Mass of the planet in kilograms.
 * @param r     Radius of the planet in meters.
 * @param v     Pointer to the output escape velocity in meters per second.
 *              Will be set to NAN if the radius of the planet is zero.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_grav_esc_vel(zsl_real_t mp,  zsl_real_t r, zsl_real_t *v);

/**
 * @brief Calculates the gravitational acceleration at any point created by an
 *        object of mass 'm' in meters per second squared.
 *
 * @param m         Mass of the object in kilograms.
 * @param r         Distance between the point and the object in meters.
 * @param a         Pointer to the output acceleration in meters per second
 *                  squared. Will be set to NAN if the distance is zero.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_grav_acc(zsl_real_t m, zsl_real_t r, zsl_real_t *a);

/**
 * @brief Calculates the velocity of an object moving about a planet describing
 *        a circular orbit in meters per second based on the mass of the planet
 *        (mp) and the radius (r) of the orbit.
 *
 * @param mp    Mass of the planet in kilograms.
 * @param r     Radius of the orbit in meters.
 * @param v     Pointer to the output velocity tangent to the orbit in meters
 *              per second. Will be set to NAN if the radius of the orbit is
 *              zero.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_grav_orb_vel(zsl_real_t mp, zsl_real_t r, zsl_real_t *v);

/**
 * @brief Calculates the gravitational force between two objects in newtons
 *        based on their masses (m1 and m2) and the distance they are
 *        separeted (r).
 *
 * @param m1        Mass of the first object in kilograms.
 * @param m2        Mass of the second object in kilograms.
 * @param r         Distance between the objects in meters.
 * @param f         Pointer to the output force in newtons. Will be set to NAN
 *                  if their distance is zero.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_grav_force(zsl_real_t m1, zsl_real_t m2, zsl_real_t r,
		       zsl_real_t *f);

/**
 * @brief Calculates the gravitational potential energy between two objects in
 *        joules based on their masses (m1 and m2) and the distance they are
 *        separeted (r).
 *
 * @param m1        Mass of the first object in kilograms.
 * @param m2        Mass of the second object in kilograms.
 * @param r         Distance between the objects in meters.
 * @param u         Pointer to the output potential energy in joules. Will be
 *                  set to NAN if their distance is zero.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_grav_pot_ener(zsl_real_t m1, zsl_real_t m2, zsl_real_t r,
			  zsl_real_t *u);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_GRAVITATION_H_ */

/** @} */ /* End of gravitation group */
