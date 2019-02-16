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
 * @brief API header file for vectors in ZSL.
 *
 * This file contains the ZSL vector APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_VECTORS_H_
#define ZEPHYR_INCLUDE_ZSL_VECTORS_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Represents a vector. */
struct zsl_vec {
    /** The number of elements in the vector. */
    size_t sz;
    /** The single-precision data assigned to the vector. */
    zsl_data_t *data;
};

/** Macro to declare a vector with static memory allocation. */
#define ZSL_VECTOR_DEF(name, n)\
  zsl_data_t name##_vec[n];\
  struct zsl_vec name = {\
      .sz   = n,\
      .data = name##_vec\
  };

/**
 * @brief Converts an array of values into a vector. The number of elements in
 *        array 'a' must match the number of elements in vector 'v'.
 *        As such, 'v' should be previously initialised with an appropriate
 *        value assigned to v.sz.
 *
 * @param v The vector that the contents of array 'a' should be assigned to.
 *          The v.sz value must match the number of elements in 'a', meaning
 *          that the vector should be initialised before being passed in to
 *          this function.
 * @param a Pointer to the array containing the values to assign to 'v'.
 *          The array will be read v.sz elements deep.
 *
 * @return 0 on success, and non-zero error code on failure
 */
int zsl_vec_from_arr(struct zsl_vec *v, zsl_data_t *a);

/**
 * @brief Adds corresponding vector elements.
 *
 * @param v The first vector.
 * @param w The second vector.
 *
 * @return 0 on success, and non-zero error code on failure
 */
int zsl_vec_add(struct zsl_vec *v, struct zsl_vec *w);

/**
 * @brief Subtracts corresponding vector elements.
 *
 * @param v The first vector.
 * @param w The second vector.
 *
 * @return 0 on success, and non-zero error code on failure
 */
int zsl_vec_sub(struct zsl_vec *v, struct zsl_vec *w);

/**
 * @brief Component-wise sum of a set of vectors.
 *
 * @param v  Pointer to the array of vectors.
 * @param n  The number of vectors in 'v'.
 * @param w  Pointer to the output vector containing the component-wise sum.
 *
 * @return 0 on success, and non-zero error code on failure
 */
int zsl_vec_sum(struct zsl_vec **v, size_t n, struct zsl_vec *w);

/**
 * @brief Multiply a vector by a scalar.
 *
 * @param v The vector to use.
 * @param s The scalar to multiply by.
 *
 * @return 0 on success, and non-zero error code on failure
 */
int zsl_vec_scalar_mult(struct zsl_vec *v, zsl_data_t s);

/**
 * @brief Computes the component-wise mean of a set of identically-sized
 * vectors.
 *
 * @param v  Pointer to the array of vectors.
 * @param n  The number of vectors in 'v'.
 * @param i  The vector whose ith element is the mean of the ith elements
 *           of the input vectors.
 *
 * @return 0 on success, and non-zero error code on failure
 */
int zsl_vec_mean(struct zsl_vec **v, size_t n, int i);

/**
 * @brief Computes the dot product of two vectors (the sum of their
 *        component-wise products).
 *
 * @param v The first vector.
 * @param w The second vector.
 *
 * @return 0 on success, and non-zero error code on failure
 */
int zsl_vec_dot(struct zsl_vec *v, struct zsl_vec *w);

/**
 * @brief Computes the vector's sum of squares.
 *
 * @param v The vector to use.
 * @param w The output vector.
 *
 * @return 0 on success, and non-zero error code on failure
 */
int zsl_vec_sum_of_sqrs(struct zsl_vec *v, struct zsl_vec *w);

/**
 * @brief Computes the magnitude of a vector.
 *
 * @param v The vector to use.
 * @param m The magnitude of vector 'v'.
 *
 * @return 0 on success, and non-zero error code on failure
 */
int zsl_vec_magnitude(struct zsl_vec *v, zsl_data_t *m);

/**
 * @brief Calculates the distance between two vectors, which is equal to the
 *        magnitude of vector v - vector w.
 *
 * @param v The first vector.
 * @param w The second vector.
 * @param d The magnitude of vector v = vector w.
 *
 * @return 0 on success, and non-zero error code on failure
 */
int zsl_vec_distance(struct zsl_vec *v, struct zsl_vec *w, zsl_data_t *d);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_VECTORS_H_ */
