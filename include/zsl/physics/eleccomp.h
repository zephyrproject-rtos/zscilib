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
 * @addtogroup ELECTRICAL_COMPS Electrical components.
 *
 * @brief Electrical component functions.
 *
 * \ingroup PHYSICS
 *  @{ */

/**
 * @file
 * @brief API header file for electrical components in zscilib.
 *
 * This file contains the zscilib electrical component APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_ELEC_COMPS_H_
#define ZEPHYR_INCLUDE_ZSL_ELEC_COMPS_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculates capacitance in farads based on charge (q) and voltage (v).
 *
 * @param q     Charge in coulombs
 * @param v     Voltage in volts
 * @param c     Pointer to the output capacitance in farad. Will be set to
 *              NAN if v = 0.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_ecmp_capac_cpv(zsl_real_t q, zsl_real_t v, zsl_real_t *c);

/**
 * @brief Calculates capacitance in farads based on the permitivity
 *        of freespace multiplied by area (a) and divided by distance (d).
 *        i.e.: C = E (A / D) where E = 8.85 x 10^-12 farad per meter (F/m).
 *
 * This function can be used to calculate the value of a parallel plate
 * capacitor, consisting of two metallic plates of area a, separated by a
 * vacuum with a gap of d.
 *
 * @param a     Area in square meters.
 * @param d     Distance in meters.
 * @param c     Pointer to the output capacitance in farad. Will be set to
 *              NAN if d = 0.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_ecmp_capac_ad(zsl_real_t a, zsl_real_t d, zsl_real_t *c);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_ELEC_COMPS_H_ */

/** @} */ /* End of electrical components group */
