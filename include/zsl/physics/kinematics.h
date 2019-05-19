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
 * \defgroup KINEMATICS Kinematics
 *
 * @brief Kinematics functions.
 */

/**
 * @file
 * @brief API header file for kinematics in zscilib.
 *
 * This file contains the zscilib kinematics APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_KINEMATICS_H_
#define ZEPHYR_INCLUDE_ZSL_KINEMATICS_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculates the change in distance based on initial velocity,
 *        time and acceleration.
 *
 * @param vi    Initial velocity in meters per second.
 * @param time  Time in seconds.
 * @param accel Acceleration in meters per second squared.
 * @param dist  Pointer to the placeholder for calculated distance in meters.
 *
 * @return 0 on success, and non-zero error code on failure
 */
int zsl_phys_kin_dist(zsl_real_t vi, zsl_real_t time, zsl_real_t accel,
                      zsl_real_t *dist);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_KINEMATICS_H_ */

/** @} */ /* End of kinematics group */
