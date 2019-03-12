/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <string.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>

/*
 * WARNING: Work in progress!
 *
 * The code in this module is very 'naive' in the sense that no attempt
 * has been made at efficiency. It is written from the perspective
 * that code should be written to be 'reliable, elegant, efficient' in that
 * order.
 *
 * Clarity and reliability have been absolutely prioritized in this
 * early stage, with the key goal being good unit test coverage before
 * moving on to any form of general-purpose or architecture-specific
 * optimisation.
 */

int
zsl_mtx_entry_fn_empty(struct zsl_mtx *m, size_t i, size_t j)
{
    /* TODO: Consider inlining to avoid multiple unnecessary branches. */

    return zsl_mtx_set(m, i, j, 0);
}

int
zsl_mtx_entry_fn_diagonal(struct zsl_mtx *m, size_t i, size_t j)
{
    /* TODO: Consider inlining to avoid multiple unnecessary branches. */

    return zsl_mtx_set(m, i, j, i == j ? 1.0 : 0);
}

int
zsl_mtx_entry_fn_random(struct zsl_mtx *m, size_t i, size_t j)
{
    /* TODO: Consider inlining to avoid multiple unnecessary branches. */

    /* TODO: Determine an appropriate random number generator. */
    return zsl_mtx_set(m, i, j, 0);
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
                return rc;
            }
        }
    }

    return 0;
}

int
zsl_mtx_from_arr(struct zsl_mtx *m, zsl_real_t *a)
{
    /* TODO: Evaluate inlining function to avoid repetitive branches. */

    memcpy(m->data, a, (m->sz_rows * m->sz_cols) * sizeof(zsl_real_t));

    return 0;
}

int
zsl_mtx_get(struct zsl_mtx *m, size_t i, size_t j, zsl_real_t *x)
{
    /* TODO: Evaluate inlining function to avoid repetitive branches. */

#if CONFIG_ZSL_BOUNDS_CHECKS
    if ((i >= m->sz_rows) || (j >= m->sz_cols)) {
        return -EINVAL;
    }
#endif

    *x = m->data[(i * m->sz_cols) + j];

    return 0;
}

int
zsl_mtx_set(struct zsl_mtx *m, size_t i, size_t j, zsl_real_t x)
{
    /* TODO: Evaluate inlining function to avoid repetitive branches. */

#if CONFIG_ZSL_BOUNDS_CHECKS
    if ((i >= m->sz_rows) || (j >= m->sz_cols)) {
        return -EINVAL;
    }
#endif

    m->data[(i * m->sz_cols) + j] = x;

    return 0;
}

int
zsl_mtx_get_row(struct zsl_mtx *m, size_t i, zsl_real_t *v)
{
    int rc;
    zsl_real_t x;

    /*
     * TODO: Optimise by determining start pos and perform consecutive
     * reads for sz_rows values, avoiding multiple unnecessary branches.
     */

    for (size_t j = 0; j < m->sz_cols; j++) {
        rc = zsl_mtx_get(m, i, j, &x);
        if (rc) {
            return rc;
        }
        v[j] = x;
    }

    return 0;
}

int
zsl_mtx_set_row(struct zsl_mtx *m, size_t i, zsl_real_t *v)
{
    int rc;

    /*
     * TODO: Optimise by determining start pos and perform consecutive
     * writes for sz_rows values, avoiding multiple unnecessary branches.
     */

    for (size_t j = 0; j < m->sz_cols; j++) {
        rc = zsl_mtx_set(m, i, j, v[j]);
        if (rc) {
            return rc;
        }
    }

    return 0;
}

int
zsl_mtx_get_col(struct zsl_mtx *m, size_t j, zsl_real_t *v)
{
    int rc;
    zsl_real_t x;

    /*
     * TODO: Optimise by determining start pos and perform consecutive
     * reads every sz_rows values for sz_cols entries, avoiding multiple
     * unnecessary branches.
     */

    for (size_t i = 0; i < m->sz_rows; i++) {
        rc = zsl_mtx_get(m, i, j, &x);
        if (rc) {
            return rc;
        }
        v[i] = x;
    }

    return 0;
}

int
zsl_mtx_set_col(struct zsl_mtx *m, size_t j, zsl_real_t *v)
{
    int rc;

    /*
     * TODO: Optimise by determining start pos and perform consecutive
     * writes every sz_rows values for sz_cols entries, avoiding multiple
     * unnecessary branches.
     */

    for (size_t i = 0; i < m->sz_rows; i++) {
        rc = zsl_mtx_set(m, i, j, v[i]);
        if (rc) {
            return rc;
        }
    }

    return 0;
}

int
zsl_mtx_unary_op(struct zsl_mtx *m, zsl_mtx_unary_op_t op)
{
        /* Execute the unary operation component by component. */
        for (size_t i = 0; i < m->sz_cols * m->sz_rows; i++) {
                switch (op) {
                case ZSL_MTX_UNARY_OP_INCREMENT:
                        m->data[i] += 1.0;
                        break;
                case ZSL_MTX_UNARY_OP_DECREMENT:
                        m->data[i] -= 1.0;
                        break;
                case ZSL_MTX_UNARY_OP_NEGATIVE:
                        m->data[i] = -m->data[i];
                        break;
                case ZSL_MTX_UNARY_OP_LOGICAL_NEGATION:
                        m->data[i] = !m->data[i];
                        break;
                case ZSL_MTX_UNARY_OP_ROUND:
                case ZSL_MTX_UNARY_OP_ABS:
                case ZSL_MTX_UNARY_OP_FLOOR:
                case ZSL_MTX_UNARY_OP_CEIL:
                case ZSL_MTX_UNARY_OP_EXP:
                case ZSL_MTX_UNARY_OP_LOG:
                case ZSL_MTX_UNARY_OP_LOG10:
                case ZSL_MTX_UNARY_OP_SQRT:
                case ZSL_MTX_UNARY_OP_SIN:
                case ZSL_MTX_UNARY_OP_COS:
                case ZSL_MTX_UNARY_OP_TAN:
                case ZSL_MTX_UNARY_OP_ASIN:
                case ZSL_MTX_UNARY_OP_ACOS:
                case ZSL_MTX_UNARY_OP_ATAN:
                case ZSL_MTX_UNARY_OP_SINH:
                case ZSL_MTX_UNARY_OP_COSH:
                case ZSL_MTX_UNARY_OP_TANH:
                default:
                        /* Not yet implemented! */
                        return -ENOSYS;
                }
        }

        return 0;
}

int
zsl_mtx_unary_func(struct zsl_mtx *m, zsl_mtx_unary_fn_t fn)
{
        int rc;

        for (size_t i = 0; i < m->sz_rows; i++) {
            for (size_t j = 0; j < m->sz_cols; j++) {
                /* If fn is NULL, do nothing. */
                if (fn != NULL) {
                    rc = fn(m, i, j);
                    if (rc) {
                        return rc;
                    }
                }
            }
        }

        return 0;
}

int
zsl_mtx_binary_op(struct zsl_mtx *ma, struct zsl_mtx *mb, struct zsl_mtx *mc,
        zsl_mtx_binary_op_t op)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
        if ((ma->sz_rows != mb->sz_rows) || (mb->sz_rows != mc->sz_rows) ||
            (ma->sz_cols != mb->sz_cols) || (mb->sz_cols != mc->sz_cols)) {
            return -EINVAL;
        }
#endif

        /* Execute the binary operation component by component. */
        for (size_t i = 0; i < ma->sz_cols * ma->sz_rows; i++) {
                switch (op) {
                case ZSL_MTX_BINARY_OP_ADD:
                        mc->data[i] = ma->data[i] + mb->data[i];
                        break;
                case ZSL_MTX_BINARY_OP_SUB:
                        mc->data[i] = ma->data[i] - mb->data[i];
                        break;
                case ZSL_MTX_BINARY_OP_MULT:
                        mc->data[i] = ma->data[i] * mb->data[i];
                        break;
                case ZSL_MTX_BINARY_OP_DIV:
                        if (mb->data[i] == 0.0) {
                                mc->data[i] = 0.0;
                        } else {
                                mc->data[i] = ma->data[i] / mb->data[i];
                        }
                        break;
                case ZSL_MTX_BINARY_OP_EXPON:
                case ZSL_MTX_BINARY_OP_MIN:
                case ZSL_MTX_BINARY_OP_MAX:
                case ZSL_MTX_BINARY_OP_EQUAL:
                case ZSL_MTX_BINARY_OP_NEQUAL:
                case ZSL_MTX_BINARY_OP_LESS:
                case ZSL_MTX_BINARY_OP_GREAT:
                case ZSL_MTX_BINARY_OP_LEQ:
                case ZSL_MTX_BINARY_OP_GEQ:
                default:
                        /* Not yet implemented! */
                        return -ENOSYS;
                }
        }

        return 0;
}

int
zsl_mtx_binary_func(struct zsl_mtx *ma, struct zsl_mtx *mb,
        struct zsl_mtx *mc, zsl_mtx_binary_fn_t fn)
{
        int rc;

#if CONFIG_ZSL_BOUNDS_CHECKS
        if ((ma->sz_rows != mb->sz_rows) || (mb->sz_rows != mc->sz_rows) ||
            (ma->sz_cols != mb->sz_cols) || (mb->sz_cols != mc->sz_cols)) {
            return -EINVAL;
        }
#endif

        for (size_t i = 0; i < ma->sz_rows; i++) {
            for (size_t j = 0; j < ma->sz_cols; j++) {
                /* If fn is NULL, do nothing. */
                if (fn != NULL) {
                    rc = fn(ma, mb, mc, i, j);
                    if (rc) {
                        return rc;
                    }
                }
            }
        }

        return 0;
}

int
zsl_mtx_add(struct zsl_mtx *ma, struct zsl_mtx *mb, struct zsl_mtx *mc)
{
        return zsl_mtx_binary_op(ma, mb, mc, ZSL_MTX_BINARY_OP_ADD);
}

int
zsl_mtx_add_d(struct zsl_mtx *ma, struct zsl_mtx *mb)
{
        return zsl_mtx_binary_op(ma, mb, ma, ZSL_MTX_BINARY_OP_ADD);
}

int
zsl_mtx_sub(struct zsl_mtx *ma, struct zsl_mtx *mb, struct zsl_mtx *mc)
{
        return zsl_mtx_binary_op(ma, mb, mc, ZSL_MTX_BINARY_OP_SUB);
}

int
zsl_mtx_sub_d(struct zsl_mtx *ma, struct zsl_mtx *mb)
{
        return zsl_mtx_binary_op(ma, mb, ma, ZSL_MTX_BINARY_OP_SUB);
}

int
zsl_mtx_mult(struct zsl_mtx *ma, struct zsl_mtx *mb, struct zsl_mtx *mc)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
    /* Ensure that ma has the same number as columns as mb has rows. */
    if (ma->sz_cols != mb->sz_rows) {
        return -EINVAL;
    }

    /* Ensure that mc has ma rows and mb cols */
    if ((mc->sz_rows != ma->sz_rows) || (mc->sz_cols != mb->sz_cols)) {
        return -EINVAL;
    }
#endif

    for (size_t i = 0; i < ma->sz_rows; i++) {
        for (size_t j = 0; j < mb->sz_cols; j++) {
            mc->data[j + i * mb->sz_cols] = 0;
            for (size_t k = 0; k < ma->sz_cols; k++) {
                mc->data[j + i * mb->sz_cols] +=
                    ma->data[k + i * ma->sz_cols] *
                    mb->data[j + k * mb->sz_cols];
            }
        }
    }

    return 0;
}

int
zsl_mtx_scalar_mult(struct zsl_mtx *m, zsl_real_t s)
{
    for (size_t i = 0; i < m->sz_rows * m->sz_cols; i++) {
        m->data[i] *= s;
    }

    return 0;
}

int
zsl_mtx_trans(struct zsl_mtx *ma, struct zsl_mtx *mb)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
    /* Ensure that ma and mb have the same shape. */
    if ((ma->sz_rows != mb->sz_cols) || (ma->sz_cols != mb->sz_rows)) {
        return -EINVAL;
    }
#endif

    zsl_real_t d[ma->sz_cols];

    for (size_t i = 0; i < ma->sz_rows; i++) {
        zsl_mtx_get_row(ma, i, d);
        zsl_mtx_set_col(mb, i, d);
    }

    return 0;
}

int
zsl_mtx_adjoint_3x3(struct zsl_mtx *m, struct zsl_mtx *ma)
{
    /* Make sure this is a square matrix. */
    if ((m->sz_rows != m->sz_cols) || (ma->sz_rows != ma->sz_cols)) {
        return -EINVAL;
    }

    #if CONFIG_ZSL_BOUNDS_CHECKS
        /* Make sure this is a 3x3 matrix. */
        if ((m->sz_rows != 3) || (ma->sz_rows != 3)) {
            return -EINVAL;
        }
    #endif

    /*
     * 3x3 matrix element to array table:
     *
     * 1,1 = 0  1,2 = 1  1,3 = 2
     * 2,1 = 3  2,2 = 4  2,3 = 5
     * 3,1 = 6  3,2 = 7  3,3 = 8
     */

    ma->data[0] = m->data[4] * m->data[8] - m->data[7] * m->data[5];
    ma->data[1] = m->data[7] * m->data[2] - m->data[1] * m->data[8];
    ma->data[2] = m->data[1] * m->data[5] - m->data[4] * m->data[2];

    ma->data[3] = m->data[6] * m->data[5] - m->data[3] * m->data[8];
    ma->data[4] = m->data[0] * m->data[8] - m->data[6] * m->data[2];
    ma->data[5] = m->data[3] * m->data[2] - m->data[0] * m->data[5];

    ma->data[6] = m->data[3] * m->data[7] - m->data[6] * m->data[4];
    ma->data[7] = m->data[6] * m->data[1] - m->data[0] * m->data[7];
    ma->data[8] = m->data[0] * m->data[4] - m->data[3] * m->data[1];

    return 0;
}

int
zsl_mtx_adjoint(struct zsl_mtx *m, struct zsl_mtx *ma)
{
    /* Currently only 3x3 matrices are supported. */
    if (m->sz_rows == 3) {
        return zsl_mtx_adjoint_3x3(m, ma);
    }

    /* TODO: Recursive calls to reduce matrix to 3x3 chunks. */

    return -ENOSYS; /* Not yet implemented! */
}

int
zsl_mtx_deter_3x3(struct zsl_mtx *m, zsl_real_t *d)
{
    /* Make sure this is a square matrix. */
    if (m->sz_rows != m->sz_cols) {
        return -EINVAL;
    }

#if CONFIG_ZSL_BOUNDS_CHECKS
    /* Make sure this is a 3x3 matrix. */
    if (m->sz_rows != 3) {
        return -EINVAL;
    }
#endif

    /*
     * 3x3 matrix element to array table:
     *
     * 1,1 = 0  1,2 = 1  1,3 = 2
     * 2,1 = 3  2,2 = 4  2,3 = 5
     * 3,1 = 6  3,2 = 7  3,3 = 8
     */

    *d = m->data[0] * (m->data[4] * m->data[8] - m->data[7] * m->data[5]);
    *d -= m->data[3] * (m->data[1] * m->data[8] - m->data[7] * m->data[2]);
    *d += m->data[6] * (m->data[1] * m->data[5] - m->data[4] * m->data[2]);

    return 0;
}

int
zsl_mtx_deter(struct zsl_mtx *m, zsl_real_t *d)
{
    /* Currently only 3x3 matrices are supported. */
    if (m->sz_rows == 3) {
        return zsl_mtx_deter_3x3(m, d);
    }

    /* TODO: Recursive calls to reduce matrix to 3x3 chunks. */

    return -ENOSYS; /* Not yet implemented! */
}

int
zsl_mtx_inv_3x3(struct zsl_mtx *m, struct zsl_mtx *mi)
{
    int rc;
    zsl_real_t d;   /* Determinant. */
    zsl_real_t s;   /* Scale factor. */

    /* Make sure these are square matrices. */
    if ((m->sz_rows != m->sz_cols) || (mi->sz_rows != mi->sz_cols)) {
        return -EINVAL;
    }

#if CONFIG_ZSL_BOUNDS_CHECKS
    /* Make sure 'm' and 'mi' have the same shape. */
    if (m->sz_rows != mi->sz_rows) {
        return -EINVAL;
    }
    if (m->sz_cols != mi->sz_cols) {
        return -EINVAL;
    }
    /* Make sure these are 3x3 matrices. */
    if ((m->sz_cols != 3) || (mi->sz_cols != 3)) {
        return -EINVAL;
    }
#endif

    /* Calculate the determinant. */
    rc = zsl_mtx_deter_3x3(m, &d);
    if (rc) {
        goto err;
    }

    /* Calculate the adjoint matrix. */
    rc = zsl_mtx_adjoint_3x3(m, mi);
    if (rc) {
        goto err;
    }

    /* Scale the output using the determinant. */
    if (d != 0) {
        s = 1.0 / d;
        rc = zsl_mtx_scalar_mult(mi, s);
    } else {
        /* Provide an identity matrix if the determinant is zero. */
        mi->data[0] = 1.0;
        mi->data[4] = 1.0;
        mi->data[8] = 1.0;
    }

    return 0;
err:
    return rc;
}

int
zsl_mtx_inv(struct zsl_mtx *m, struct zsl_mtx *mi)
{
    /* Currently only 3x3 matrices are supported. */
    if (m->sz_rows == 3) {
        return zsl_mtx_inv_3x3(m, mi);
    }

    /* TODO: Recursive calls to reduce matrix to 3x3 chunks. */

    return -ENOSYS; /* Not yet implemented! */
}

int
zsl_mtx_eigen(struct zsl_mtx *m, zsl_real_t *val, struct zsl_mtx *vec)
{
    return -ENOSYS; /* Not yet implemented! */
}

int
zsl_mtx_min(struct zsl_mtx *m, zsl_real_t *x)
{
    zsl_real_t min = m->data[0];

    for (size_t i = 0; i < m->sz_cols * m->sz_rows; i++) {
        if (m->data[i] < min) {
            min = m->data[i];
        }
    }

    *x = min;

    return 0;
}

int
zsl_mtx_max(struct zsl_mtx *m, zsl_real_t *x)
{
    zsl_real_t max = m->data[0];

    for (size_t i = 0; i < m->sz_cols * m->sz_rows; i++) {
        if (m->data[i] > max) {
            max = m->data[i];
        }
    }

    *x = max;

    return 0;
}

int
zsl_mtx_min_idx(struct zsl_mtx *m, size_t *i, size_t *j)
{
    zsl_real_t min = m->data[0];
    *i = 0;
    *j = 0;

    for (size_t _i = 0; _i < m->sz_rows; _i++) {
        for (size_t _j = 0; _j < m->sz_cols; _j++) {
            if (m->data[_i * m->sz_cols + _j] < min) {
                min = m->data[_i * m->sz_cols + _j];
                *i = _i;
                *j = _j;
            }
        }
    }

    return 0;
}

int
zsl_mtx_max_idx(struct zsl_mtx *m, size_t *i, size_t *j)
{
    zsl_real_t max = m->data[0];
    *i = 0;
    *j = 0;

    for (size_t _i = 0; _i < m->sz_rows; _i++) {
        for (size_t _j = 0; _j < m->sz_cols; _j++) {
            if (m->data[_i * m->sz_cols + _j] > max) {
                max = m->data[_i * m->sz_cols + _j];
                *i = _i;
                *j = _j;
            }
        }
    }

    return 0;
}

bool
zsl_mtx_is_equal(struct zsl_mtx *ma, struct zsl_mtx *mb)
{
    int res;

    /* Make sure shape is the same. */
    if ((ma->sz_rows != mb->sz_rows) || (ma->sz_cols != mb->sz_cols)) {
        return false;
    }

    res = memcmp(ma->data, mb->data,
        sizeof(zsl_real_t) * (ma->sz_rows + ma->sz_cols));

    return res == 0 ? true : false;
}

bool
zsl_mtx_is_notneg(struct zsl_mtx *m)
{
    for (size_t i = 0; i < m->sz_rows * m->sz_cols; i++) {
        if (m->data[i] < 0.0) {
            return false;
        }
    }

    return true;
}
