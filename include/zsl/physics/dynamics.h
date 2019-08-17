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
 * @addtogroup DYNAMICS Dynamics
 *
 * @brief Dynamics functions.
 *
 * \ingroup PHYSICS
 *  @{ */

/**
 * @file
 * @brief API header file for dynamics in zscilib.
 *
 * This file contains the zscilib dynamics APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_DYNAMICS_H_
#define ZEPHYR_INCLUDE_ZSL_DYNAMICS_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculates the force in newtons that experiences a body based on its
 *        mass (m) and acceleration (accel).
 *
 * @param m      Mass of the body in kilograms.
 * @param accel  Acceleration in meters per second squared.
 * @param f      Pointer to the output force in newtons. Will be set to NAN if
 *               the mass of the body is negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_dyn_newton(zsl_real_t m, zsl_real_t accel, zsl_real_t *f);

/**
 * @brief Calculates the friction force experienced by a body moving in a
 *        surface with a friction coefficient 'mu'.
 *
 * @param n     Normal force that the surface exerts to the body, in newtons.
 * @param mu    Friction coefficient, that ranges from zero to one.
 * @param f     Pointer to the output friction force in newtons. Will be set to
 *              NAN if 'mu' is less than zero or greater than 1.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_dyn_fric(zsl_real_t n, zsl_real_t mu, zsl_real_t *f);

/**
 * @brief Calculates the normal force in newtons exerted by plane inclined
 *        'theta' radians on a body of mass 'm'.
 *
 * @param m      Mass of the body in kilograms.
 * @param theta  Angle of inclination in radians.
 * @param n      Pointer to the output normal force in newtons. Will be set to
 *               NAN if the value of the body's mass is negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_dyn_normal(zsl_real_t m, zsl_real_t theta, zsl_real_t *n);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_DYNAMICS_H_ */

/** @} */ /* End of dynamics group */
