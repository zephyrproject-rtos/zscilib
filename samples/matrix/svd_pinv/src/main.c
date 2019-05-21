/*
 * Copyright (c) 2019 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zephyr.h>
#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include "zsl/vectors.h"

#ifndef EPSILON
/* Define EPSILON for floating point comparisons. */
#define EPSILON 1e-6
#endif

/* To avoid declaring multiple matrices on the stack, several chunks of
 * statically declared memory are made available here for reused in interative
 * functions, etc. */
static zsl_real_t scratch_data_1[100] = { 0 };
static zsl_real_t scratch_data_2[100] = { 0 };
#define SCRATCH_1_CLEAR (memset(&scratch_data_1, 0, sizeof scratch_data_1))
#define SCRATCH_2_CLEAR (memset(&scratch_data_1, 0, sizeof scratch_data_1))

/* Input matrix used in this module. */
zsl_real_t data[9] = { 0.0, -6.0,  -4.0,
                       5.0, -11.0, -6.0,
                       -6.0, 9.0,   4.0 };

struct zsl_mtx m = {
        .sz_rows = 3,
        .sz_cols = 3,
        .data = data
};

int
zsl_vec_zte(struct zsl_vec *v)
{
        size_t x=0;
        for(size_t g = 0; g < v->sz; g++) {
                if((v->data[g-x] >= 0.0 && v->data[g-x] < EPSILON) ||
                   (v->data[g-x] <= 0.0 && v->data[g-x] > EPSILON)) {
                        for (size_t p = g - x; p <( v->sz - 1); p++) {
                                v->data[p] = v->data[p + 1];
                        }
                        v->data[v->sz - 1] = 0.0;
                        x++;
                }
        }

        return 0;
}

int
zsl_mtx_householder(struct zsl_mtx *m, struct zsl_mtx *mout)
{
        ZSL_VECTOR_DEF(v, m->sz_rows);
        ZSL_VECTOR_DEF(w, m->sz_rows);
        ZSL_MATRIX_DEF(mp, m->sz_rows, 1);
        ZSL_MATRIX_DEF(mpt, 1, m->sz_rows);
        ZSL_MATRIX_DEF(mid, m->sz_rows, m->sz_rows);
        ZSL_MATRIX_DEF(mq, m->sz_rows, m->sz_rows);
        zsl_vec_init(&w);
        w.data[0] = 1.0;
        zsl_mtx_get_col(m, 0, v.data);
        zsl_vec_scalar_mult(&w, -zsl_vec_norm(&v));
        zsl_vec_add(&v, &w, &v);
        zsl_vec_scalar_div(&v, zsl_vec_norm(&v));
        zsl_mtx_from_arr(&mp, v.data);
        zsl_mtx_trans(&mp, &mpt);
        zsl_mtx_mult(&mp, &mpt, &mq);
        zsl_mtx_init(&mid, zsl_mtx_entry_fn_identity);
        zsl_mtx_scalar_mult_d(&mq, -2);
        zsl_mtx_add(&mid, &mq, mout);
        return 0;
}


int
zsl_mtx_augm_diag(struct zsl_mtx *m, struct zsl_mtx *maug)
{
        zsl_real_t x;
        size_t diff = (maug->sz_rows)-(m->sz_rows);
        zsl_mtx_init(maug, zsl_mtx_entry_fn_identity);
        for (size_t i = 0; i < m->sz_rows; i++) {
                for (size_t j = 0; j <m ->sz_rows; j++) {
                        zsl_mtx_get(m, i, j, &x);
                        zsl_mtx_set(maug, i+diff, j+diff, x);
                }
        }
        return 0;
}

int
zsl_mtx_reduce_iter(struct zsl_mtx *m, struct zsl_mtx *mred)
{
        if (m->sz_rows == mred->sz_rows) {
                zsl_mtx_copy(mred, m);
                return 0;
        }

        ZSL_MATRIX_DEF(my, (m->sz_rows - 1), (m->sz_rows - 1));
        zsl_mtx_reduce(m, &my, 0, 0);
        zsl_mtx_reduce_iter(&my,mred);

        return 0;
}

/**
 * @brief QR decompisition.
 */
int
zsl_mtx_qrd(struct zsl_mtx *m, struct zsl_mtx *q, struct zsl_mtx *r)
{
        ZSL_MATRIX_DEF(ha2, m->sz_rows, m->sz_rows);
        ZSL_MATRIX_DEF(h2, m->sz_rows, m->sz_rows);
        ZSL_MATRIX_DEF(h, m->sz_rows, m->sz_rows);
        ZSL_MATRIX_DEF(qp, m->sz_rows, m->sz_rows);

        zsl_mtx_init(&qp, zsl_mtx_entry_fn_identity);
        zsl_mtx_copy(r, m);
        for (size_t g = 0; g <( m->sz_rows - 1); g++) {
                ZSL_MATRIX_DEF(mred, (m->sz_rows - g), (m->sz_rows - g));
                zsl_mtx_reduce_iter(r, &mred);
                zsl_mtx_householder(&mred, &mred);
                zsl_mtx_augm_diag(&mred, &h2);
                zsl_mtx_mult(&h2, r, &ha2);
                zsl_mtx_mult(&h2, &qp, &h);
                zsl_mtx_copy(&qp, &h);
                zsl_mtx_copy(r, &ha2);
        }
        zsl_mtx_trans(&qp,q);

        return 0;
}

/**
 * @brief Iterative QR decompisition.
 */
int
zsl_mtx_qrd_iter(struct zsl_mtx *m, struct zsl_mtx *mout, size_t iter)
{
        if (iter == 0) {
                return 0;
        }

        /* Use scratch memory to avoid stack overflow when these functions
         * are called iteratively. */
        SCRATCH_1_CLEAR;
        struct zsl_mtx q = {
                .sz_rows      = m->sz_rows,
                .sz_cols      = m->sz_cols,
                .data         = scratch_data_1
        };

        SCRATCH_2_CLEAR;
        struct zsl_mtx r = {
                .sz_rows      = m->sz_rows,
                .sz_cols      = m->sz_cols,
                .data         = scratch_data_2
        };

        zsl_mtx_qrd(m, &q, &r);

        zsl_mtx_mult(&r, &q, mout);

        zsl_mtx_qrd_iter(mout, mout, iter - 1);

        return 0;
}

int
zsl_mtx_eigenvalues(struct zsl_mtx *m, struct zsl_vec *v, size_t iter)
{
        zsl_real_t p;
        zsl_real_t q;
        ZSL_MATRIX_DEF(mout, m->sz_rows, m->sz_rows);

        zsl_mtx_qrd_iter(m, &mout, iter);

        zsl_vec_init(v);

        for (size_t y =0 ; y < (m->sz_rows - 1); y++) {
                zsl_mtx_get(&mout, y+1, y, &q);
                if ((q > EPSILON) || (q < -EPSILON)) {
                        return -101;
                }
        }

        for (size_t g=0; g<m->sz_rows; g++) {
                zsl_mtx_get(&mout, g, g, &p);
                v->data[g]=p;
        }
        zsl_vec_zte(v);

        return 0;
}

int
zsl_vec_project(struct zsl_vec *u, struct zsl_vec *v, struct zsl_vec *w)
{
        zsl_real_t p;
        zsl_real_t t;
        zsl_vec_copy(w,u);
        zsl_vec_dot(v, u, &p);
        zsl_vec_dot(u, u, &t);
        zsl_vec_scalar_mult(w, p/t);

        return 0;
}

int
zsl_mtx_gram_schmidt(struct zsl_mtx *m, struct zsl_mtx *mort)
{
        ZSL_VECTOR_DEF(v, m->sz_rows);
        ZSL_VECTOR_DEF(w, m->sz_rows);
        ZSL_VECTOR_DEF(q, m->sz_rows);

        for(size_t t=0; t<m->sz_cols; t++) {
                zsl_vec_init(&q);
                zsl_mtx_get_col(m, t, v.data);
                for(size_t g = 0; g < t; g++) {
                        zsl_mtx_get_col(mort, g, w.data);
                        zsl_vec_project(&w, &v, &w);
                        zsl_vec_add(&q, &w, &q);
                }
                zsl_vec_sub(&v, &q, &v);
                zsl_mtx_set_col(mort, t, v.data);
        }

        return 0;
}

int
zsl_mtx_cols_norm(struct zsl_mtx *m, struct zsl_mtx *mnorm)
{
        ZSL_VECTOR_DEF(v, m->sz_rows);
        for (size_t g = 0; g <m ->sz_cols; g++) {
                zsl_mtx_get_col(m, g, v.data);
                zsl_vec_to_unit(&v);
                zsl_mtx_set_col(mnorm, g, v.data);
        }
        return 0;
}

int
zsl_mtx_eigenvectors(struct zsl_mtx *m, struct zsl_mtx *mev, size_t iter,
                     bool orthonormal)
{
        size_t b=0;
        size_t s=0;
        size_t ze=0;
        size_t gu=0;
        size_t ga=0;
        zsl_real_t x;
        ZSL_VECTOR_DEF(k, m->sz_rows);
        ZSL_VECTOR_DEF(f, m->sz_rows);
        ZSL_VECTOR_DEF(o, m->sz_rows);
        ZSL_MATRIX_DEF(mp, m->sz_rows, m->sz_rows);
        ZSL_MATRIX_DEF(mi, m->sz_rows, m->sz_rows);
        ZSL_MATRIX_DEF(mid, m->sz_rows, m->sz_rows);

        zsl_mtx_init(mev, NULL);
        zsl_vec_init(&o);
        zsl_mtx_eigenvalues(m, &k, iter);
        for (size_t q = 0; q < m->sz_rows; q++) {
                if((k.data[q] >= 0 && k.data[q] < EPSILON) ||
                   (k.data[q] <= 0 && k.data[q] > -EPSILON)) {
                        if(ze == 0) {
                                o.data[s]=k.data[q];
                                s++;
                                ze++;
                                q++;
                        }
                }
                if(zsl_vec_contains(&o, k.data[q], EPSILON) == 0) {
                        o.data[s]=k.data[q];
                        s++;
                }
        }

        for (size_t g=0; g<s; g++) {
                gu=0;
                ga=0;
                zsl_mtx_init(&mp, zsl_mtx_entry_fn_identity);
                zsl_mtx_scalar_mult_d(&mp, -o.data[g]);
                zsl_mtx_add_d(&mp, m);
                zsl_mtx_gauss_reduc(&mp, &mid, &mi);
                if(orthonormal == true) {
                        for (size_t h = 0; h <m ->sz_rows; h++) {
                                zsl_mtx_get(&mi, h, h, &x);
                                if ((x >= 0 && x < EPSILON) ||
                                    (x <= 0 && x > -EPSILON)) {
                                        gu++;
                                }
                        }
                        ZSL_MATRIX_DEF(mt, m->sz_rows, gu);
                        ZSL_MATRIX_DEF(mt2, m->sz_rows, gu);
                        ZSL_MATRIX_DEF(mt3, m->sz_rows, gu);
                        for (size_t h =0 ; h < m->sz_rows; h++) {
                                zsl_mtx_get(&mi, h, h, &x);
                                if ((x >= 0 && x < EPSILON) ||
                                    (x <= 0 && x > -EPSILON)) {
                                        zsl_mtx_set(&mi, h, h, -1);
                                        zsl_mtx_get_col(&mi, h, f.data);
                                        zsl_vec_neg(&f);
                                        zsl_mtx_set_col(&mt, ga, f.data);
                                        ga++;
                                }
                        }
                        if(gu > 1) {
                                zsl_mtx_gram_schmidt(&mt, &mt3);
                                zsl_mtx_cols_norm(&mt3, &mt2);
                        }
                        else{
                                zsl_mtx_cols_norm(&mt, &mt2);
                        }

                        for(size_t gi=0; gi<gu; gi++) {
                                zsl_mtx_get_col(&mt2, gi, f.data);
                                zsl_mtx_set_col(mev, b, f.data);
                                b++;
                        }
                }
                else{
                        for (size_t h = 0; h < m->sz_rows; h++) {
                                zsl_mtx_get(&mi, h, h, &x);
                                if ((x >= 0.0 && x < EPSILON) ||
                                    (x <= 0.0 && x > -EPSILON)) {
                                        zsl_mtx_set(&mi, h, h, -1);
                                        zsl_mtx_get_col(&mi, h, f.data);
                                        zsl_vec_neg(&f);
                                        zsl_mtx_set_col(mev, b, f.data);
                                        b++;
                                }
                        }
                }
        }

        /*zsl_mtx_get_col(mev, (m->sz_rows-1), k.data);
           if(zsl_vec_contains(&k, 0.0, EPSILON)== m->sz_rows){
            return -101;
           }*/


        return 0;
}

int
zsl_mtx_svd(struct zsl_mtx *m, struct zsl_mtx *u, struct zsl_mtx *e,
            struct zsl_mtx *vt, size_t iter)
{

        ZSL_MATRIX_DEF(aat, m->sz_rows, m->sz_rows);
        ZSL_MATRIX_DEF(ata, m->sz_cols, m->sz_cols);
        ZSL_MATRIX_DEF(at, m->sz_cols, m->sz_rows);

        zsl_mtx_trans(m, &at);
        zsl_mtx_mult(m, &at, &aat);

        zsl_mtx_mult(&at, m, &ata);

        size_t min = m->sz_cols;
        if(m->sz_rows <= m->sz_cols) {
                min = m->sz_rows;
        }

        ZSL_VECTOR_DEF(ev, min);
        if(min < m->sz_cols) {
                zsl_mtx_eigenvalues(&aat, &ev, iter);
        }
        else{
                zsl_mtx_eigenvalues(&ata, &ev, iter);
        }


        zsl_mtx_init(e, NULL);
        for(size_t g = 0; g < min; g++) {
                zsl_mtx_set(e, g, g, sqrt(ev.data[g]));
        }

        ZSL_VECTOR_DEF(ui, m->sz_rows);
        ZSL_MATRIX_DEF(ui2, m->sz_cols, 1);
        ZSL_MATRIX_DEF(ui3, m->sz_rows, 1);

        ZSL_VECTOR_DEF(ui4, m->sz_rows);
        ZSL_MATRIX_DEF(ui5, m->sz_rows, 1);
        ZSL_MATRIX_DEF(ui6, m->sz_cols, 1);

        ZSL_VECTOR_DEF(hu, m->sz_cols);
        ZSL_VECTOR_DEF(hu2, m->sz_rows);

        zsl_real_t d;

        if (min<m->sz_cols) {
                zsl_mtx_eigenvectors(&ata, vt, iter, true);
                for (size_t gu = 0; gu < m->sz_rows; gu++) {
                        zsl_mtx_get_col(vt, gu, ui.data);
                        zsl_mtx_from_arr(&ui2, ui.data);

                        zsl_mtx_get(e, gu, gu, &d);
                        zsl_mtx_mult(m, &ui2, &ui3);

                        if((d >= 0.0 && d < EPSILON) ||
                           (d <= 0.0 && d > -EPSILON)) {
                                zsl_vec_init(&hu2);
                                hu2.data[0]=1.0;
                                zsl_vec_from_arr(&ui, hu2.data);
                        }
                        else{
                                zsl_mtx_scalar_mult_d(&ui3, (1/d));
                                zsl_vec_from_arr(&ui, ui3.data);
                        }
                        zsl_mtx_set_col(u, gu, ui.data);
                }

        }

        else{
                zsl_mtx_eigenvectors(&aat, u, iter, true);
                for (size_t gu=0; gu<m->sz_cols; gu++) {
                        zsl_mtx_get_col(u, gu, ui4.data);
                        zsl_mtx_from_arr(&ui5, ui4.data);

                        zsl_mtx_get(e, gu, gu, &d);
                        zsl_mtx_mult(&at, &ui5, &ui6);

                        if((d >= 0.0 && d < EPSILON) ||
                           (d <= 0.0 && d > -EPSILON)) {
                                zsl_vec_init(&hu);
                                hu.data[0]=1.0;
                                zsl_vec_from_arr(&ui4, hu.data);
                        }
                        else{
                                zsl_mtx_scalar_mult_d(&ui6, (1/d));
                                zsl_vec_from_arr(&ui4, ui6.data);
                        }
                        zsl_mtx_set_col(vt, gu, ui4.data);
                }
        }

        return 0;
}

int
zsl_mtx_pinv(struct zsl_mtx *m, struct zsl_mtx *pinv, size_t iter)
{
        zsl_real_t x;
        ZSL_MATRIX_DEF(u, m->sz_rows, m->sz_rows);
        ZSL_MATRIX_DEF(e, m->sz_rows, m->sz_cols);
        ZSL_MATRIX_DEF(v, m->sz_cols, m->sz_cols);
        ZSL_MATRIX_DEF(et, m->sz_cols, m->sz_rows);
        ZSL_MATRIX_DEF(ut, m->sz_rows, m->sz_rows);
        ZSL_MATRIX_DEF(pas, m->sz_cols, m->sz_rows);

        zsl_mtx_svd(m, &u, &e, &v, iter);
        zsl_mtx_trans(&u, &ut);

        size_t min = m->sz_cols;
        if(m->sz_rows <= m->sz_cols) {
                min = m->sz_rows;
        }

        for (size_t g=0; g<min; g++) {
                zsl_mtx_get(&e, g, g, &x);
                if ((x < EPSILON) || (x > -EPSILON)) {
                        x=1/x;
                        zsl_mtx_set(&e, g, g, x);
                }
        }

        zsl_mtx_trans(&e, &et);

        zsl_mtx_mult(&v, &et, &pas);
        zsl_mtx_mult(&pas, &ut, pinv);

        return 0;
}

int svd_test()
{
        /* Generates random matrix shape (q, p) and random values for vi. */

        size_t x;
        size_t q = sys_rand32_get() % 7 + 2;
        size_t p = sys_rand32_get() % 7 + 2;
        zsl_real_t vi[q * p];

        // srand((unsigned)time(NULL));
        for(x = 0; x < q * p; x++) {
                vi[x]= ((zsl_real_t)sys_rand32_get() / RAND_MAX) * 10.0;
        }

        /* Define the qxp matrix, assigning the random values. */
        ZSL_MATRIX_DEF(mep, q, p);
        zsl_mtx_from_arr(&mep, vi);

        /* Define the three SVD output matrices. */
        ZSL_MATRIX_DEF(u, mep.sz_rows, mep.sz_rows);
        ZSL_MATRIX_DEF(e, mep.sz_rows, mep.sz_cols);
        ZSL_MATRIX_DEF(v, mep.sz_cols, mep.sz_cols);

        /* Define placeholder matrices for SVD and related operations. */
        ZSL_MATRIX_DEF(ue, mep.sz_rows, mep.sz_cols);
        ZSL_MATRIX_DEF(vt, mep.sz_cols, mep.sz_cols);
        ZSL_MATRIX_DEF(uevt, mep.sz_rows, mep.sz_cols);

        /* Display the input matrix for reference. */
        printf("\nInput %dx%d matrix:\n", q, p);
        zsl_mtx_print(&mep);
        printf("\n\n");

        /* Calculate and display the SVD using 150 iterations. */
        zsl_mtx_svd(&mep, &u, &e, &v, 150);
        printf("\nSVD results:\n");
        printf("\nU matrix (%dx%d):\n", q, q);
        zsl_mtx_print(&u);
        printf("\n\n");
        printf("\nSigma matrix (%dx%d):\n", q, p);
        zsl_mtx_print(&e);
        printf("\n\n");
        printf("\nV matrix: (%dx%d)\n", p, p);
        zsl_mtx_print(&v);
        printf("\n\n");

        /* Multiply u, sigma and v(transposed) from the SVD calculations,
         * which shoud give us the original matrix. */
        zsl_mtx_mult(&u, &e, &ue);
        zsl_mtx_trans(&v, &vt);
        zsl_mtx_mult(&ue, &vt, &uevt);
        printf("\nU * Sigma * V^t test results:\n");
        zsl_mtx_print(&uevt);
        printf("\n\n");

        /* Make sure the input matrix and uevt match. */
        zsl_real_t dif;
        for(size_t g = 0; g < p * q; g++) {
                dif = mep.data[g] - uevt.data[g];
                if((dif > EPSILON) || (dif < -EPSILON)) {
                        printf("\nThe SVD process seems to have failed!\n");
                        return -EINVAL;
                }
        }

        return 0;
}

int pinv_test()
{
        return 0;
}

int main(void)
{
        printf("zscilib SVD and pinv demo\n\n");

        printf("SVD tester:\n\n");
        svd_test();

        //printf("Pseudoinverse tester:\n\n");
        //pinv_test();

        while (1) {
                k_sleep(100);
        }

        return 0;
}
