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

/**
 * The radius of a nucleon, in meters. This is an empirically determined
 * constant and it's value is approximately between 1.2 and 1.5 femtometers.
 */
#define ZSL_NUCL_RADIUS                                 (1.25E-15)

/**
 * The irrational number 'e', also known as the Euler's constant.
 */
#define ZSL_E                                           (2.71828182845)

/**
 * Planck's constant, in kilograms and meters squared per second.
 */
#define ZSL_PLANCK                                      (6.62607015E-34)

/**
 * Reduced Planck's constant or Dirac's constant, in kilograms and meters
 * squared per second. Defined as the Planck's constant divided by 2 pi.
 */
#define ZSL_RED_PLANCK                                  (1.054571800E-34)

/**
 * The elementary change, AKA the electric charge of a proton and minus the
 * electric charge of an electron, in coulombs.
 */
#define ZSL_E_CHARGE                                    (1.602176634E-19)

/**
 * Coulomb's constant or the electrostatic constant, in newtons and meters
 * squared per coulombs squared.
 */
#define ZSL_COULOMB                                     (8.987551787E9)

/**
 * The mass of a single electron, in kilograms.
 */
#define ZSL_E_MASS                                      (9.1093835611E-31)

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_CONSTS_H_ */

/** @} */ /* End of constants group */
