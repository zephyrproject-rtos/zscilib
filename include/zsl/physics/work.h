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
 * @addtogroup WORK Work
 *
 * @brief Work functions.
 *
 * \ingroup PHYSICS
 *  @{ */

/**
 * @file
 * @brief API header file for work in zscilib.
 *
 * This file contains the zscilib work APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_WORK_H_
#define ZEPHYR_INCLUDE_ZSL_WORK_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculates the module of the work done by a force (f) along a
 *        distance (d).
 *
 * @param f    Applied force in newtons.
 * @param d    Distance in meters.
 * @param w    Pointer to the output work in joules. Will be set to NAN
 *             if either the force or the distance are negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_work_module(zsl_real_t f, zsl_real_t d, zsl_real_t *w);

/**
 * @brief Calculates the horizontal component of the work done by a force (f)
 *        along a distance (d).
 *
 * @param f    Applied force in newtons.
 * @param d    Distance in meters.
 * @param theta The angle between the force and distance vectors.
 * @param w    Pointer to the output work in the x-axis in joules. Will be set
 *             to NAN if either the force or the distance are negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_work_x(zsl_real_t f, zsl_real_t d, zsl_real_t theta, zsl_real_t *w);

/**
 * @brief Calculates the vertical component of the work done by a force (f)
 *        along a distance (d).
 *
 * @param f    Applied force in newtons.
 * @param d    Distance in meters.
 * @param theta The angle between the force and distance vectors.
 * @param w    Pointer to the output work in the y-axis in joules. Will be set
 *             to NAN if either the force or the distance are negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_work_y(zsl_real_t f, zsl_real_t d, zsl_real_t theta, zsl_real_t *w);

/**
 * @brief Calculates the work required to change the kinetic energy of an
 *        object from 'k1' to 'k2'.
 *
 * @param k1    Initial kinetic energy of the object in joules.
 * @param k2    Final kinetic energy of the object in joules.
 * @param w     Pointer to the output work in joules.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_work_kin(zsl_real_t k1, zsl_real_t k2, zsl_real_t *w);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_WORK_H_ */

/** @} */ /* End of work group */
