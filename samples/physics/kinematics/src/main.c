/*
 * Copyright (c) 2019 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>

void kinematics_demo(void)
{
        printf("TODO: implement kinematics_demo\n\n");
}

void main(void)
{
        printf("zscilib kinematics demo\n\n");

        kinematics_demo();

        while (1) {
                k_sleep(100);
        }
}
