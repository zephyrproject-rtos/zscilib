/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
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
 * @brief Optimised vector functions for zscilib using ARM Thumb or ARM Thumb2.
 *
 * This file contains optimised vector functions for ARM Thumb or ARM Thumb2.
 */

#include <zsl/zsl.h>
#include <zsl/asm/arm/asm_arm.h>

#ifndef ZEPHYR_INCLUDE_ZSL_ASM_ARM_VECTORS_H_
#define ZEPHYR_INCLUDE_ZSL_ASM_ARM_VECTORS_H_

#if !asm_vec_scalar_mult
int zsl_vec_scalar_mult(struct zsl_vec *v, zsl_real_t s)
{
    for (size_t i = 0; i < v->sz; i++) {
        v->data[i] *= s;
    }

    return 0;
}
#define asm_vec_scalar_mult 1
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_ASM_ARM_VECTORS_H_ */
