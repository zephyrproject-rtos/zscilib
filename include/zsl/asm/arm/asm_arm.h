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
 * @brief Optimised functions for zscilib using ARM Thumb or ARM Thumb2.
 *
 * This file contains optimised functions for ARM Thumb or ARM Thumb2.
 */

#ifndef ZEPHYR_INCLUDE_ZSL_ASM_ARM_H_
#define ZEPHYR_INCLUDE_ZSL_ASM_ARM_H_

#define REG_RW "+l"
#define REG_WRITE "=l"
#define REG_RW_LO "+l"
#define REG_WRITE_LO "=l"

#if (CONFIG_ZSL_PLATFORM == 2)  /* ARM Thumb2 */
    #define RESUME_SYNTAX
#else
    #define RESUME_SYNTAX ".syntax divided \n\t"
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_ASM_ARM_H_ */
