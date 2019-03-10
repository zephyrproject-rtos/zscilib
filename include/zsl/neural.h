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
 * @brief API header file for neural networks in zscilib.
 *
 * This file contains the zscilib neural network APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_NEURAL_H_
#define ZEPHYR_INCLUDE_ZSL_NEURAL_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * TODO: Implement the following activation functions:
 *
 * - Softmax
 * - Exponential linear unit (ELU)
 * - Scalar exponential linear unit (SELU)
 * - Softplus
 * - Softsign
 * - Rectified linear unit (RELU)
 * - Hyperbolic tangent
 * - Exponential (base e)
 * - Linear (i.e. identity)
 */

 /**
  * TODO: Implement the following optimisers:
  *
  * - Stochastic gradient descent
  * - RMS prop.
  * - Adagrad
  * - Adadelta
  * - Adam
  * - Adamax
  * - Nesterov adam (Nadam)
  */

/**
 * TODO: Implement the following loss calculation methods:
 *
 * - Mean squared error (MSE)
 * - Cross-entropy loss (CEL)
 * - ???
 */

/**
 * Activation function for a neuron.
 *
 * @param s     Pointer to the sum to pass into the activation function.
 * @param out   Pointer to the output of the activation function.
 *
 * @return 0 on success, and non-zero error code on failure
 */
typedef int (*zsl_nn_activ_fn_t)(zsl_reql_t *s, zsl_real_t *out);

/**
 * @brief Implementation of the sigmoid activation function.
 *
 * @param s     Pointer to the sum to pass into the activation function.
 * @param out   Pointer to the output of the activation function.
 *
 * @return 0 on success, and non-zero error code on failure
 */
int zsl_nn_activ_fn_sigmoid(zsl_reql_t *s, zsl_real_t *out));

/** @brief Represents a neuron in a neural network. */
struct zsl_nn_neuron {
    /** The number of inputs available in 'inputs'. */
    size_t num_inputs;
    /** The array of input valuess for the neuron. */
    zsl_real_t **inputs;
    /** The weight coefficient applied to each input value. */
    zsl_real_t **input_weight;
    /** The bias used when adding all weight inputs together. */
    zsl_real_t bias;
    /** The activation function the sum of weighted values is passed through. */
    zsl_nn_activ_fn_t activ_fn;
    /** The neuron's output value. */
    zsl_real_t *output;
};

int
zsl_nn_feedfwd(void)
{
    return 0,
}

int
zsl_nn_train(void)
{
    return 0;
}


#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_NEURAL_H_ */
