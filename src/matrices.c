/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
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
        return zsl_mtx_set(m, i, j, 0);
}

int
zsl_mtx_entry_fn_identity(struct zsl_mtx *m, size_t i, size_t j)
{
        return zsl_mtx_set(m, i, j, i == j ? 1.0 : 0);
}

int
zsl_mtx_entry_fn_random(struct zsl_mtx *m, size_t i, size_t j)
{
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
        memcpy(m->data, a, (m->sz_rows * m->sz_cols) * sizeof(zsl_real_t));

        return 0;
}

int
zsl_mtx_copy(struct zsl_mtx *mdest, struct zsl_mtx *msrc)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
        /* Ensure that msrc and mdest have the same shape. */
        if ((mdest->sz_rows != msrc->sz_cols) ||
            (mdest->sz_cols != msrc->sz_rows)) {
                return -EINVAL;
        }
#endif

        /* Make a copy of matrix 'msrc'. */
        mdest->sz_rows = msrc->sz_rows;
        mdest->sz_cols = msrc->sz_cols;
        memcpy(mdest->data, msrc->data, sizeof(zsl_real_t) *
               msrc->sz_rows * msrc->sz_cols);

        return 0;
}

int
zsl_mtx_get(struct zsl_mtx *m, size_t i, size_t j, zsl_real_t *x)
{
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
                        m->data[i] = ZSL_ROUND(m->data[i]);
                        break;
                case ZSL_MTX_UNARY_OP_ABS:
                        m->data[i] = ZSL_ABS(m->data[i]);
                        break;
                case ZSL_MTX_UNARY_OP_FLOOR:
                        m->data[i] = ZSL_FLOOR(m->data[i]);
                        break;
                case ZSL_MTX_UNARY_OP_CEIL:
                        m->data[i] = ZSL_CEIL(m->data[i]);
                        break;
                case ZSL_MTX_UNARY_OP_EXP:
                        m->data[i] = ZSL_EXP(m->data[i]);
                        break;
                case ZSL_MTX_UNARY_OP_LOG:
                        m->data[i] = ZSL_LOG(m->data[i]);
                        break;
                case ZSL_MTX_UNARY_OP_LOG10:
                        m->data[i] = ZSL_LOG10(m->data[i]);
                        break;
                case ZSL_MTX_UNARY_OP_SQRT:
                        m->data[i] = ZSL_SQRT(m->data[i]);
                        break;
                case ZSL_MTX_UNARY_OP_SIN:
                        m->data[i] = ZSL_SIN(m->data[i]);
                        break;
                case ZSL_MTX_UNARY_OP_COS:
                        m->data[i] = ZSL_COS(m->data[i]);
                        break;
                case ZSL_MTX_UNARY_OP_TAN:
                        m->data[i] = ZSL_TAN(m->data[i]);
                        break;
                case ZSL_MTX_UNARY_OP_ASIN:
                        m->data[i] = ZSL_ASIN(m->data[i]);
                        break;
                case ZSL_MTX_UNARY_OP_ACOS:
                        m->data[i] = ZSL_ACOS(m->data[i]);
                        break;
                case ZSL_MTX_UNARY_OP_ATAN:
                        m->data[i] = ZSL_ATAN(m->data[i]);
                        break;
                case ZSL_MTX_UNARY_OP_SINH:
                        m->data[i] = ZSL_SINH(m->data[i]);
                        break;
                case ZSL_MTX_UNARY_OP_COSH:
                        m->data[i] = ZSL_COSH(m->data[i]);
                        break;
                case ZSL_MTX_UNARY_OP_TANH:
                        m->data[i] = ZSL_TANH(m->data[i]);
                        break;
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
                case ZSL_MTX_BINARY_OP_MEAN:
                        mc->data[i] = (ma->data[i] + mb->data[i]) / 2.0;
                case ZSL_MTX_BINARY_OP_EXPON:
                        mc->data[i] = ZSL_POW(ma->data[i], mb->data[i]);
                case ZSL_MTX_BINARY_OP_MIN:
                        mc->data[i] = ma->data[i] < mb->data[i] ?
                                      ma->data[i] : mb->data[i];
                case ZSL_MTX_BINARY_OP_MAX:
                        mc->data[i] = ma->data[i] > mb->data[i] ?
                                      ma->data[i] : mb->data[i];
                case ZSL_MTX_BINARY_OP_EQUAL:
                        mc->data[i] = ma->data[i] == mb->data[i] ? 1.0 : 0.0;
                case ZSL_MTX_BINARY_OP_NEQUAL:
                        mc->data[i] = ma->data[i] != mb->data[i] ? 1.0 : 0.0;
                case ZSL_MTX_BINARY_OP_LESS:
                        mc->data[i] = ma->data[i] < mb->data[i] ? 1.0 : 0.0;
                case ZSL_MTX_BINARY_OP_GREAT:
                        mc->data[i] = ma->data[i] > mb->data[i] ? 1.0 : 0.0;
                case ZSL_MTX_BINARY_OP_LEQ:
                        mc->data[i] = ma->data[i] <= mb->data[i] ? 1.0 : 0.0;
                case ZSL_MTX_BINARY_OP_GEQ:
                        mc->data[i] = ma->data[i] >= mb->data[i] ? 1.0 : 0.0;
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
zsl_mtx_sum_rows_d(struct zsl_mtx *m, size_t i, size_t j)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
        if ((i >= m->sz_rows) || (j >= m->sz_rows)) {
                return -EINVAL;
        }
#endif

        /* Add row j to row i, element by element. */
        for (size_t x = 0; x < m->sz_cols; x++) {
                m->data[(i * m->sz_cols) + x] += m->data[(j * m->sz_cols) + x];
        }

        return 0;
}

int zsl_mtx_sum_rows_scaled_d(struct zsl_mtx *m,
                              size_t i, size_t j, zsl_real_t s)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
        if ((i >= m->sz_rows) || (j >= m->sz_cols)) {
                return -EINVAL;
        }
#endif

        /* Set the values in row 'i' to 'i[n] += j[n] * s' . */
        for(size_t x = 0; x < m->sz_cols; x++) {
                m->data[(i * m->sz_cols) + x] +=
                        (m->data[(j * m->sz_cols) + x] * s);
        }

        return 0;
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
zsl_mtx_scalar_mult_d(struct zsl_mtx *m, zsl_real_t s)
{
        for (size_t i = 0; i < m->sz_rows * m->sz_cols; i++) {
                m->data[i] *= s;
        }

        return 0;
}

int
zsl_mtx_scalar_mult_row_d(struct zsl_mtx *m, size_t i, zsl_real_t s)
{
#if CONFIG_ZSL_BOUNDS_CHECKS
        if (i >= m->sz_rows) {
                return -EINVAL;
        }
#endif

        for (size_t k = 0; k < m->sz_cols; k++) {
                m->data[(i * m->sz_cols) + k] *= s;
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
zsl_mtx_reduce(struct zsl_mtx *m, struct zsl_mtx *mr, size_t i, size_t j)
{
        size_t u = 0.0;
        zsl_real_t x;
        zsl_real_t v[mr->sz_rows * mr->sz_rows];

#if CONFIG_ZSL_BOUNDS_CHECKS
        /* Make sure mr is 1 less than m. */
        if (mr->sz_rows != m->sz_rows - 1) {
                return -EINVAL;
        }
        if (mr->sz_cols != m->sz_cols - 1) {
                return -EINVAL;
        }
        if ((i >= m->sz_rows) || (j >= m->sz_cols)) {
                return -EINVAL;
        }
#endif

        for(size_t k = 0; k < m->sz_rows; k++) {
                for(size_t g = 0; g < m->sz_rows; g++) {
                        if(k != i && g != j) {
                                zsl_mtx_get(m, k, g, &x);
                                v[u] = x;
                                u++;
                        }
                }
        }
        zsl_mtx_from_arr(mr, v);

        return 0;
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
        /* Shortcut for 3x3 matrices. */
        if (m->sz_rows == 3) {
                return zsl_mtx_deter_3x3(m, d);
        }

#if CONFIG_ZSL_BOUNDS_CHECKS
        /* Make sure this is a square matrix. */
        if (m->sz_rows != m->sz_cols) {
                return -EINVAL;
        }
#endif

        /* Full calculation required for non 3x3 matrices. */
        int rc;
        zsl_real_t dtmp;
        zsl_real_t cur;
        zsl_real_t sign;
        ZSL_MATRIX_DEF(mr, (m->sz_rows - 1), (m->sz_rows - 1));

        /* Clear determinant output before starting. */
        *d = 0.0;

        /*
         * Iterate across row 0, removing columns one by one.
         * Note that these calls are recursive until we reach a 3x3 matrix,
         * which will be calculated using the shortcut at the top of this
         * function.
         */
        for (size_t g = 0; g < m->sz_cols; g++) {
                zsl_mtx_get(m, 0, g, &cur);   /* Get value at (0, g). */
                zsl_mtx_init(&mr, NULL);      /* Clear mr. */
                zsl_mtx_reduce(m, &mr, 0, g); /* Remove row 0, column g. */
                rc = zsl_mtx_deter(&mr, &dtmp); /* Calc. determinant of mr. */
                sign = 1.0;
                if (rc) {
                        return -EINVAL;
                }

                /* Uneven elements are negative. */
                if (g % 2 != 0) {
                        sign = -1.0;
                }

                /* Add current determinant to final output value. */
                *d += dtmp * cur * sign;
        }

        return 0;
}

int
zsl_mtx_gauss_elim(struct zsl_mtx *m, struct zsl_mtx *mg, struct zsl_mtx *mi,
                   size_t i, size_t j)
{
        int rc;
        zsl_real_t x, y;
        zsl_real_t epsilon = 1E-6;

        /* Make a copy of matrix m. */
        rc = zsl_mtx_copy(mg, m);
        if (rc) {
                return -EINVAL;
        }

        /* Get the value of the element at position (i, j). */
        rc = zsl_mtx_get(mg, i, j, &y);
        if (rc) {
                return rc;
        }

        /* If this is a zero value, don't do anything. */
        if ((y >= 0 && y < epsilon) || (y <= 0 && y > -epsilon)) {
                return 0;
        }

        /* Cycle through the matrix row by row. */
        for (size_t p = 0; p < mg->sz_rows; p++) {
                /* Skip row 'i'. */
                if (p == i) {
                        p++;
                }
				if (p == mg->sz_rows){
					break;
				}
                /* Get the value of (p, j), aborting if value is zero. */
                zsl_mtx_get(mg, p, j, &x);
                if((x >= 0 && x > epsilon) || (x <= 0 && x < -epsilon)) {
                        rc = zsl_mtx_sum_rows_scaled_d(mg, p, i, -(x / y));

                        if (rc) {
                                return -EINVAL;
                        }
                        rc = zsl_mtx_sum_rows_scaled_d(mi, p, i, -(x / y));
                        if (rc) {
                                return -EINVAL;
                        }
                }
        }

        return 0;
}

int
zsl_mtx_gauss_elim_d(struct zsl_mtx *m, struct zsl_mtx *mi, size_t i, size_t j)
{
        return zsl_mtx_gauss_elim(m, m, mi, i, j);
}

int
zsl_mtx_gauss_reduc(struct zsl_mtx *m, struct zsl_mtx *mid,
                    struct zsl_mtx *mi)
{
        zsl_real_t v[m->sz_rows];
        zsl_real_t epsilon = 1E-6;

        zsl_mtx_copy(mi, m);

        /* TODO: Properly comment this code block! */
        for (size_t k = 0; k < m->sz_rows; k++) {
                zsl_real_t x;
                zsl_mtx_get(mi, k, k, &x);
                if ((x >= 0 && x < epsilon) || (x <= 0 && x > -epsilon)) {
                        zsl_mtx_get_col(mi, k, v);
                        for (size_t q = k + 1; q < m->sz_rows; q++) {
                                if ((v[q] >= epsilon) || (v[q] <= -epsilon)) {
                                        zsl_mtx_sum_rows_d(mi, k, q);
                                        zsl_mtx_sum_rows_d(mid, k, q);
                                        break;
                                }
                        }
                }
                zsl_mtx_gauss_elim_d(mi, mid, k, k);
                zsl_mtx_norm_elem_d(mi, mid, k, k);

        }

        return 0;
}

int
zsl_mtx_norm_elem(struct zsl_mtx *m, struct zsl_mtx *mn, struct zsl_mtx *mi,
                  size_t i, size_t j)
{
        int rc;
        zsl_real_t x;
        zsl_real_t epsilon = 1E-6;

        /* Make a copy of matrix m. */
        rc = zsl_mtx_copy(mn, m);
        if (rc) {
                return -EINVAL;
        }

        /* Get the value to normalise. */
        rc = zsl_mtx_get(mn, i, j, &x);
        if (rc) {
                return rc;
        }

        /* If the value is 0.0, abort. */
        if ((x >= 0 && x < epsilon) || (x <= 0 && x > -epsilon)) {
                return 0;
        }

        rc = zsl_mtx_scalar_mult_row_d(mn, i, (1.0/x));
        if (rc) {
                return -EINVAL;
        }

        rc = zsl_mtx_scalar_mult_row_d(mi, i, (1.0/x));
        if (rc) {
                return -EINVAL;
        }

        return 0;
}

int
zsl_mtx_norm_elem_d(struct zsl_mtx *m, struct zsl_mtx *mi, size_t i, size_t j)
{
        return zsl_mtx_norm_elem(m, m, mi, i, j);
}

int
zsl_mtx_inv_3x3(struct zsl_mtx *m, struct zsl_mtx *mi)
{
        int rc;
        zsl_real_t d; /* Determinant. */
        zsl_real_t s; /* Scale factor. */

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
                rc = zsl_mtx_scalar_mult_d(mi, s);
        } else {
                /* Provide an identity matrix if the determinant is zero. */
                rc = zsl_mtx_init(mi, zsl_mtx_entry_fn_identity);
                if (rc) {
                        return -EINVAL;
                }
        }

        return 0;
err:
        return rc;
}

int
zsl_mtx_inv(struct zsl_mtx *m, struct zsl_mtx *mi)
{
        int rc;
        zsl_real_t d = 0.0;

        /* Shortcut for 3x3 matrices. */
        if (m->sz_rows == 3) {
                return zsl_mtx_inv_3x3(m, mi);
        }

        /* Make sure we have square matrices. */
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
#endif

        /* Make a copy of matrix m on the stack to avoid modifying it. */
        ZSL_MATRIX_DEF(m_tmp, mi->sz_rows, mi->sz_cols);
        rc = zsl_mtx_copy(&m_tmp, m);
        if (rc) {
                return -EINVAL;
        }

        /* Initialise 'mi' as an identity matrix. */
        rc = zsl_mtx_init(mi, zsl_mtx_entry_fn_identity);
        if (rc) {
                return -EINVAL;
        }

        /* Make sure the determinant of 'm' is not zero. */
        zsl_mtx_deter(m, &d);

        if (d==0) {
                return 0;
        }

        /* Use Gauss-Jordan elimination for nxn matrices. */
        zsl_mtx_gauss_reduc(m, mi, &m_tmp);

        return 0;
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

int
zsl_mtx_print(struct zsl_mtx *m)
{
        int rc;
        zsl_real_t x;

        for (size_t i = 0; i < m->sz_rows; i++) {
                for (size_t j = 0; j < m->sz_cols; j++ ) {
                        rc = zsl_mtx_get(m, i, j, &x);
                        if (rc) {
                                printf("Error reading (%zu,%zu)!\n", i, j);
                                return -EINVAL;
                        }
                        /* Print the current floating-point value. */
                        printf("%f ", x);
                }
                printf("\n");
        }

        return 0;
}
