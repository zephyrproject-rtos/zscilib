#include <stdio.h>
#include <errno.h>
#include "zsl/matrices.h"
#include "zsl/vectors.h"

/* Input matrix used in this module. */
zsl_real_t data[25] = { 1.0, 1.0, 2.0, 2.0, 1.0,
                        0.0, 0.0, 0.0, 1.0, 2.0,
                        0.0, 0.0, 1.0, 2.0, 2.0,
                        0.0, 0.0, 1.0, 1.0, 2.0,
                        0.0, 1.0, 1.0, 2.0, 1.0 };

struct zsl_mtx m = {
        .sz_rows = 5,
        .sz_cols = 5,
        .data = data
};

int
zsl_mtx_test_inv_nxn(void)
{
        /* Display the input matrix. */
        printf("Input:\n");
        zsl_mtx_print(&m);

        /* Create a copy of m in mt, because m will be turned into identity. */
        ZSL_MATRIX_DEF(mt,5,5);
        zsl_mtx_copy(&mt, &m);

        /* Create a nxn null matrix to place the values of the inverse in it. */
        ZSL_MATRIX_DEF(mi,5,5);
        zsl_mtx_init(&mi, NULL);

        /* Calculate the inverse of 'm', placing the values in 'mi'. */
        zsl_mtx_inv(&m, &mi);

        /* Display the inverse (mi). */
        printf("\nInverse:\n");
        zsl_mtx_print(&mi);

        /* Calculate the product of m by its inverse. Should be id. */
        printf("\nm*mi\n");
        ZSL_MATRIX_DEF(mu,5,5);
        zsl_mtx_init(&mu,NULL);
        zsl_mtx_mult(&mt,&mi,&mu);
        zsl_mtx_print(&mu);

        /* Calculate the product of the inverse by m. Should be id. */
        printf("\nmi*m\n");
        ZSL_MATRIX_DEF(ml,5,5);
        zsl_mtx_init(&ml,NULL);
        zsl_mtx_mult(&mi,&mt,&ml);
        zsl_mtx_print(&ml);

        return 0;
}

int
zsl_mtx_test_red(void)
{
        /* Display the input matrix. */
        printf("Input:\n");
        zsl_mtx_print(&m);

        /* Create an (n-1)x(n-1) matrix to store the reduced matrix. */
        ZSL_MATRIX_DEF(mr,4,4);
        zsl_mtx_init(&mr,NULL);

        /* Calculate the reduced matrix of 'm', placing the values in 'mr'. */
        zsl_mtx_reduce(&m, &mr, 0, 0);

        /* Display the reduced matrix (mr). */
        printf("\nOutput:\n");

        zsl_mtx_print(&mr);


        return 0;
}

/**
 * Program entry point.
 */
int
main(void)
{
        printf("Hello, zscilib!\n\n");

        printf("--- nxn inverse tester --------------------\n");
        zsl_mtx_test_inv_nxn();
        printf("-------------------------------------------\n\n");

        return 0;
}
