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
 * @addtogroup PROJECTILES Projectiles
 *
 * @brief Projectile functions.
 *
 * \ingroup PHYSICS
 *  @{ */

/**
 * @file
 * @brief API header file for projectiles in zscilib.
 *
 * This file contains the zscilib projectiles APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_PROJECTILES_H_
#define ZEPHYR_INCLUDE_ZSL_PROJECTILES_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Decomposes the initial velocity of an object under projectile motion
 *        into its horizontal and vertical components based on the angle
 *        (theta) it is launched at.
 *
 * @param vi     Initial velocity of the object in meters per second.
 * @param theta  Angle between the floor and the initial velocity in radians.
 * @param vih    Pointer to the output horizontal component of the initial
 *               velocity in meters per second.
 * @param viv    Pointer to the output vertical component of the initial
 *               velocity in meters per second.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_proj_init_vel(zsl_real_t vi, zsl_real_t theta, zsl_real_t *vih,
			  zsl_real_t *viv);

/**
 * @brief Calculates the total time of travel in seconds of an object under
 *        projectile motion from an initial height (yi) and a final height (yf)
 *        given the vertical velocity at the starting point. Measurements are
 *        based on reaching yf on descent.
 *
 * @param viv    Initial vertical velocity of the object at the starting point
 *               in meters per second.
 * @param yi     Initial height in meters.
 * @param yf     Final height in meters.
 * @param t      Pointer to the output total time of flight of the object in
 *               seconds.
 *
 * @return 0 if everything executed properly, -EINVAL if there is a mismatch
 *         between yi and yf such that .
 */
int zsl_phy_proj_time(zsl_real_t viv, zsl_real_t yi, zsl_real_t yf,
		      zsl_real_t *t);

/**
 * @brief Calculates the total time of travel in seconds of an object under
 *        projectile motion from an initial height (yi) and a final height (yf)
 *        given the vertical velocity at the starting point. Measurements are
 *        based on the first instance that yf is reached, which can occur
 *        during the ascent or descent stages depending on values provided.
 *
 * @param viv    Initial vertical velocity of the object at the starting point
 *               in meters per second.
 * @param yi     Initial height in meters.
 * @param yf     Final height in meters.
 * @param t      Pointer to the output total time of flight of the object in
 *               seconds. If t is a negative value, it will be set to NAN.
 *
 * @return 0 if everything executed properly, -EINVAL if there is a mismatch
 *         between yi and yf such that .
 */
int zsl_phy_proj_time_first(zsl_real_t viv, zsl_real_t yi, zsl_real_t yf,
		            zsl_real_t *t);

/**
 * @brief Calculates the total time of travel in seconds of an object under
 *        projectile motion based on the vertical velocity at the starting and
 *        ending points.
 *
 * @param viv    Initial vertical velocity of the object at the starting point
 *               in meters per second.
 * @param vfv    Final vertical velocity of the object at the ending point in
 *               meters per second.
 * @param t      Pointer to the output total time of flight of the object in
 *               seconds. If t is a negative value, it will be set to NAN.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_proj_time2(zsl_real_t viv, zsl_real_t vfv, zsl_real_t *t);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_PROJECTILES_H_ */

/** @} */ /* End of projectiles group */
