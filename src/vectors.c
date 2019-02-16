/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <string.h>
#include <zsl/zsl.h>
#include <zsl/vectors.h>

int
zsl_vec_init(struct zsl_vec *v)
{
    memset(v->data, 0, v->sz * sizeof(zsl_data_t));

    return 0;
}

int
zsl_vec_from_arr(struct zsl_vec *v, zsl_data_t *a)
{
    memcpy(v->data, a, v->sz * sizeof (zsl_data_t));

    return 0;
}

int
zsl_vec_add(struct zsl_vec *v, struct zsl_vec *w, struct zsl_vec *x)
{
    return 0;
}
