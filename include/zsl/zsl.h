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
 * @file
 * @brief API header file for zscilib.
 *
 * This file contains the entry points to the zscilib APIs.
 */

#ifndef ZEPHYR_INCLUDE_ZSL_H_
#define ZEPHYR_INCLUDE_ZSL_H_

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <zsl/consts.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Version numbers take the form of "MAJOR.MINOR.REVISION-STATUS".
 *
 * ZSL_VERSION_STATUS should be one of:
 *
 *   o "-prerelease"
 *   o "-alpha"
 *   o "-beta"
 *   o "-rc0", "-rc1", etc.
 *   o "-release"
 */

#define ZSL_VERSION             "0.1.0-prerelease"
#define ZSL_VERSION_MAJOR       0
#define ZSL_VERSION_MINOR       1
#define ZSL_VERSION_REVISION    0
#define ZSL_VERSION_STATUS      "-prerelease"
#define ZSL_VERSION_DATE        __DATE__

extern const char * zsl_version;

#if CONFIG_ZSL_SINGLE_PRECISION
typedef float zsl_real_t;
#else
typedef double zsl_real_t;
#endif

#if !defined(s8_t)
typedef signed char         s8_t;
#endif
#if !defined(s16_t)
typedef signed short        s16_t;
#endif
#if !defined(s32_t)
typedef signed int          s32_t;
#endif
#if !defined(s64_t)
typedef signed long long    s64_t;
#endif

#if !defined(u8_t)
typedef unsigned char       u8_t;
#endif
#if !defined(u16_t)
typedef unsigned short      u16_t;
#endif
#if !defined(u32_t)
typedef unsigned int        u32_t;
#endif
#if !defined(u64_t)
typedef unsigned long long  u64_t;
#endif

/* Map math functions based on single or double precision. */
/* TODO: Add an option for fast approximations of trig operations. */
#if CONFIG_ZSL_SINGLE_PRECISION
#define ZSL_CEIL       ceilf
#define ZSL_FLOOR      floorf
#define ZSL_ROUND      roundf
#define ZSL_ABS        fabsf
#define ZSL_MIN        fminf
#define ZSL_MAX        fmaxf
#define ZSL_POW        powf
#define ZSL_EXP        expf
#define ZSL_LOG        logf
#define ZSL_LOG10      log10f
#define ZSL_SQRT       sqrtf
#define ZSL_SIN        sinf
#define ZSL_COS        cosf
#define ZSL_TAN        tanf
#define ZSL_ASIN       asinf
#define ZSL_ACOS       acosf
#define ZSL_ATAN       atanf
#define ZSL_SINH       sinhf
#define ZSL_COSH       coshf
#define ZSL_TANH       tanhf
#else
#define ZSL_CEIL       ceil
#define ZSL_FLOOR      floor
#define ZSL_ROUND      round
#define ZSL_ABS        fabs
#define ZSL_MIN        fmin
#define ZSL_MAX        fmax
#define ZSL_POW        pow
#define ZSL_EXP        exp
#define ZSL_LOG        log
#define ZSL_LOG10      log10
#define ZSL_SQRT       sqrt
#define ZSL_SIN        sin
#define ZSL_COS        cos
#define ZSL_TAN        tan
#define ZSL_ASIN       asin
#define ZSL_ACOS       acos
#define ZSL_ATAN       atan
#define ZSL_SINH       sinh
#define ZSL_COSH       cosh
#define ZSL_TANH       tanh
#endif


/* TODO: Define common errors like shape mismatch, etc. */

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_H_ */
