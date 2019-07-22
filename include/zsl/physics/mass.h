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
 * @addtogroup CENTER_OF_MASS Center of mass
 *
 * @brief Center of mass functions.
 *
 * \ingroup PHYSICS
 *  @{ */

/**
 * @file
 * @brief API header file for center of mass in zscilib.
 *
 * This file contains the zscilib center of mass APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_CMASS_H_
#define ZEPHYR_INCLUDE_ZSL_CMASS_H_

#include <zsl/zsl.h>
#include <zsl/vectors.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculates the three coordinate position of the center of mass of a
 *        set of objects given their masses and positions.
 *
 * @param n     Number of objects.
 * @param m     Vector that contains the mass of each object in kilograms.
 * @param x     Vector that contains the x coordinate of each object in meters.
 * @param y     Vector that contains the y coordinate of each object in meters.
 * @param z     Vector that contains the z coordinate of each object in meters.
 * @param mx    Pointer to the x position of the center of mass in meters.
 * @param mx    Pointer to the y position of the center of mass in meters.
 * @param mx    Pointer to the z position of the center of mass in meters.
 *
 * @return 0 if everything executed properly, -EINVAL if any of the masses has
 *         a negative value or the total mass is zero.
 */
int zsl_phy_mass_center(struct zsl_vec *m, struct zsl_vec *x,
			struct zsl_vec *y, struct zsl_vec *z, zsl_real_t *mx,
			zsl_real_t *my, zsl_real_t *mz);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_CMASS_H_ */

/** @} */ /* End of center of mass group */
