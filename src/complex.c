/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zsl/zsl.h>
#include <zsl/complex.h>

int
zsl_com_add(struct zsl_complex *ca, struct zsl_complex *cb,
        struct zsl_complex *cc)
{
    cc->r = ca->r + cb->r;
    cc->i = ca->i + cb->i;

    return 0;
}
