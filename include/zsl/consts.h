/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup CONSTANTS Constants
 *
 * @brief Constant values used by or with zscilib.
 *
 * @ingroup CONSTANTS
 *  @{ */

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

/**
 * The speed of light in vacuum, in meters per second.
 */
#define ZSL_LIGHT_SPEED                                 (299792458.0)

/**
 * The ideal gas constant, in joules per kelvin and mol.
 */
#define ZSL_IDEAL_GAS_CONST                             (8.314462618)

/**
 * Degrees to radians conversion.
 */
#define ZSL_DEG_TO_RAD                                  (0.01745329252)

/**
 * Radians to degrees conversion.
 */
#define ZSL_RAD_TO_DEG                                  (57.295779513)

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_CONSTS_H_ */

/** @} */ /* End of constants group */
