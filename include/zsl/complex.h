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
 * \defgroup COMPLEX Complex numbers
 *
 * @brief Support for complex numbers.
 */

/**
 * @file
 * @brief Complex number support.
 *
 * This file contains structs and functions for complex numbers in zscilib.
 */

#ifndef ZEPHYR_INCLUDE_ZSL_COMPLEX_H_
#define ZEPHYR_INCLUDE_ZSL_COMPLEX_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup COM_STRUCTS Structs and Macros
 *
 * @brief Common structs and macros for working with complex numbers.
 *
 * \ingroup COMPLEX
 *  @{ */

/** @brief Represents a complex number. */
struct zsl_complex {
        /** The real number in the complex number. */
        zsl_real_t r;
        /** The imaginary unit of the complex number, equal to sqroot(-1). */
        zsl_real_t i;
};

/**
 * Macro to declare a complex number.
 */
#define ZSL_COMPLEX_DEF(name, real, imag) \
        struct zsl_complex name = { \
                .r = real, \
                .i = imag \
        }

/** @} */ /* End of COM_STRUCTS group */

/**
 * @addtogroup COM_BASICMATH Basic Math
 *
 * @brief Basic mathematical operations for complex numbers.
 *
 * \ingroup COMPLEX
 *  @{ */

/**
 * @brief Adds complex numbers 'ca' and 'cb', assigning the output to 'cc'.
 *
 * @param ca    Pointer to the first input complex number.
 * @param cb    Pointer to the second input complex number.
 * @param cc    Pointer to the output complex number.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate error
 *          code.
 */
int zsl_com_add(struct zsl_complex *ca, struct zsl_complex *cb,
        struct zsl_complex *cc);

/** @} */ /* End of COM_BASICMATH group */

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_COMPLEX_H_ */

/** @} */ /* End of complex group */
