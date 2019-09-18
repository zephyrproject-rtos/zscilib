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
 * @addtogroup GASES Gases
 *
 * @brief Gas functions.
 *
 * \ingroup PHYSICS
 *  @{ */

/**
 * @file
 * @brief API header file for gases in zscilib.
 *
 * This file contains the zscilib gases APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_GASES_H_
#define ZEPHYR_INCLUDE_ZSL_GASES_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculates the average velocity of a gas molecule based on the mass of
 *        gas (m), its temperature (t) and the number of moles of gas (n).
 *
 * @param m    Mass of gas in kilograms.
 * @param n    Number of moles of gas.
 * @param t    Temperature in kelvins.
 * @param v    Pointer to the  calculated velocity in meters per second. Will be
 *             set to NAN if the temperature, mass or moles of gas are negative,
 *             or if m = 0.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_gas_av_vel(zsl_real_t m, zsl_real_t n, zsl_real_t t, zsl_real_t *v);

/**
 * @brief Calculates the pressure of an ideal gas based on the number of moles,
 *        the temperature and the volume of gas.
 *
 * @param v    Volume of gas in meters cubed.
 * @param n    Number of moles of gas.
 * @param t    Temperature in kelvins.
 * @param p    Pointer to the placeholder for calculated pressure in pascals.
 *             Will be set to NAN if the temperature, volume or moles of gas
 *             are negative, or if v = 0.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_gas_press(zsl_real_t v, zsl_real_t n, zsl_real_t t, zsl_real_t *p);

/**
 * @brief Given the initial and final pressure, and the initial volume of an
 *        ideal gas, this function calculates the final volume of the gas
 *        supposing the temperature hasn't changed through the process.
 *
 * @param pi    Initial pressure in pascals.
 * @param vi    Initial volume in meters cubed.
 * @param pf    Final pressure in pascals.
 * @param vf    Pointer to the output final volume in meters cubed. Will be set
 *              to NAN if any of the input values are negative or if pf = 0.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_gas_boyle(zsl_real_t pi, zsl_real_t vi, zsl_real_t pf,
		      zsl_real_t *vf);

/**
 * @brief Given the initial and final temperature, and the initial volume of an
 *        ideal gas, this function calculates the final volume of the gas
 *        supposing its pressure hasn't changed through the process.
 *
 * @param ti    Initial temperature in kelvins.
 * @param vi    Initial volume in meters cubed.
 * @param tf    Final temperature in kelvins.
 * @param vf    Pointer to the output final volume in meters cubed. Will be set
 *              to NAN if any of the input values are negative or if ti = 0.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_gas_charles_lussac(zsl_real_t ti, zsl_real_t vi, zsl_real_t tf,
			       zsl_real_t *vf);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_GASES_H_ */

/** @} */ /* End of gases group */
