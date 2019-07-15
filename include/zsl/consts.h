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
 * \defgroup CONSTANTS Constants
 *
 * @brief Constant values used by or with zscilib.
 */

/**
 * @file
 * @brief Constant values for zscilib.
 *
 * This file contains constant values used by the various APIs in zscilib.
 */

#ifndef ZEPHYR_INCLUDE_ZSL_CONSTS_H_
#define ZEPHYR_INCLUDE_ZSL_CONSTS_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The permitivity of free space (a vacuum), AKA vacuum permitivity or
 * electric constant, in farads per meter.
 */
#define ZSL_PERM_FREE_SPACE                             (8.85E-12)

/**
 * The universal gravitational constant or Newtonian constant of gravitation,
 * in meters cubed per kilogram and second squared.
 */
#define ZSL_UNIV_GRAV                                   (6.67408E-11)

/**
 * The irrational number Pi, defined as the ratio between the perimeter and the
 * diameter of a circle.
 */
#define ZSL_PI                                          (3.14159265359)

/**
 * The gravitational acceleration at the surface of the Earth, in meters per
 * second squared.
 */
#define ZSL_GRAV_EARTH                                  (9.807)

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_CONSTS_H_ */

/** @} */ /* End of constants group */
