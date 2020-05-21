/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Optimised functions for zscilib using ARM Thumb or ARM Thumb2.
 *
 * This file contains optimised functions for ARM Thumb or ARM Thumb2.
 */

#ifndef ZEPHYR_INCLUDE_ZSL_ASM_ARM_H_
#define ZEPHYR_INCLUDE_ZSL_ASM_ARM_H_

/** Read/Write constraint with Thumb general purpose registers (R0..R7). */
#define OP_RW_T "+l"
/** Write (output) constraint with Thumb general purpose registers (RO..R7). */
#define OP_WR_T "=l"

#if (CONFIG_ZSL_PLATFORM_OPT == 2)  /* ARM Thumb2 */
    #define RESUME_SYNTAX
#else
    #define RESUME_SYNTAX ".syntax divided \n\t"
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_ASM_ARM_H_ */
