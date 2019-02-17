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
    memcpy(v->data, a, v->sz * sizeof(zsl_data_t));

    return 0;
}

bool
zsl_vec_is_equal(struct zsl_vec *v, struct zsl_vec *w)
{
    if (v->sz != w->sz) {
        return false;
    }

    for (size_t i = 0; i < v->sz; i++) {
        if (v->data[i] != w->data[i]) {
            return false;
        }
    }

    return true;
}

int
zsl_vec_cross(struct zsl_vec *v, struct zsl_vec *w, struct zsl_vec *c)
{
    /* Make sure this is a 3-vector. */
    if ((v->sz != 3) || (w->sz != 3) || (c->sz != 3)) {
        return -EINVAL;
    }

    /*
     * Using column vectors, if ...
     *
     *       |Cx|      |Vx|      |Wx|
     *   C = |Cy|, V = |Vy|, W = |Wy|
     *       |Cz|      |Vz|      |Wz|
     *
     * ... then then cross product can be represented as:
     *
     *   Cx = VyWz - VzWy
     *   Cy = VzWx - VxWz
     *   Cz = VxWy - VyWx
     */

    c->data[0] = v->data[1] * w->data[2] - v->data[2] * w->data[1];
    c->data[1] = v->data[2] * w->data[0] - v->data[0] * w->data[2];
    c->data[2] = v->data[0] * w->data[1] - v->data[1] * w->data[0];

    return 0;
}
