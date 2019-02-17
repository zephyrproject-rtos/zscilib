/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <string.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>

int
zsl_mtx_entry_fn_empty(struct zsl_mtx *m, size_t i, size_t j)
{
    return zsl_mtx_set(m, i, j, 0.0);
}

int
zsl_mtx_entry_fn_diagonal(struct zsl_mtx *m, size_t i, size_t j)
{
    return zsl_mtx_set(m, i, j, i == j ? 1.0 : 0.0);
}

int
zsl_mtx_entry_fn_random(struct zsl_mtx *m, size_t i, size_t j)
{
    /* TODO: Determine an appropriate random number generator. */
    return zsl_mtx_set(m, i, j, 0.0);
}

int
zsl_mtx_init(struct zsl_mtx *m, zsl_mtx_init_entry_fn_t entry_fn)
{
    int rc;

    for (size_t i = 0; i < m->sz_rows; i++) {
        for (size_t j = 0; j < m->sz_cols; j++) {
            /* If entry_fn is NULL, assign 0.0 values. */
            if (entry_fn == NULL) {
                rc = zsl_mtx_entry_fn_empty(m, i, j);
            } else {
                rc = entry_fn(m, i, j);
            }
            /* Abort if entry_fn returned an error code. */
            if (rc) {
                goto err;
            }
        }
    }

    return 0;
err:
    return rc;
}

int
zsl_mtx_from_arr(struct zsl_mtx *m, zsl_data_t *a)
{
    memcpy(m->data, a, (m->sz_rows * m->sz_cols) * sizeof(zsl_data_t));

    return 0;
}

int
zsl_mtx_get(struct zsl_mtx *m, size_t i, size_t j, zsl_data_t *x)
{
    if ((i >= m->sz_rows) || (j >= m->sz_cols)) {
        return -EINVAL;
    }

    *x = m->data[(i * m->sz_cols) + j];

    return 0;
}

int
zsl_mtx_set(struct zsl_mtx *m, size_t i, size_t j, zsl_data_t x)
{
    if ((i >= m->sz_rows) || (j >= m->sz_cols)) {
        return -EINVAL;
    }

    m->data[(i * m->sz_cols) + j] = x;

    return 0;
}
