#include <stdio.h>
#include <errno.h>
#include "zsl/matrices.h"
#include "zsl/vectors.h"

/* Input 3x3 matrix used for various tests in this module. */
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

/**
 * @brief Prints an mxn matrix.
 *
 * @param m     The zsl_mtx struct to use.
 *
 * @return 0 if everything executed properly, otherwise an error code.
 */
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

// /**
//  * Performs a known-good test just as a demonstration.
//  */
// int
// zsl_mtx_test_inv(void)
// {
//     int rc;
//
//     /* Declare and init the empty 3x3 inverse matrix placeholder. */
//     ZSL_MATRIX_DEF(minv, 3, 3);
//     zsl_mtx_init(&minv, NULL);
//
//     /* Display the input matrix. */
//     printf("Input:\n");
//     zsl_mtx_print(&m);
//
//     /* Calculate the inverse of 'm', placing the values in 'minv'. */
//     rc = zsl_mtx_inv(&m, &minv);
//
//     /* Display the inverse. */
//     printf("\nInverse:\n");
//     zsl_mtx_print(&minv);
//
//     return 0;
// }
//
// /**
//  * @brief Attempts to calculate the eigenvetor/value of the supplied matrix.
//  *
//  * This function declares two different 3x3 matrices, and a single real
//  * number variable:
//  *
//  * m            A 3x3 matrix that we will define specific initial values for.
//  * eigenmtx     An empty 3x3 matrix that will be populated with values by
//  *              'zsl_mtx_eigen'.
//  * eigenval     A real number that will be updated by 'zsl_mtx_eigen' with the
//  *              eignvalue associated with matrix 'm'.
//  *
//  * @return 0 if everything executed properly, otherwise an error code.
//  */
// int
// zsl_mtx_test_eigen(void)
// {
//     int rc;
//     zsl_real_t eigenval;
//
//     /* Declare and init the empty 3x3 eigenvector matrix. */
//     ZSL_MATRIX_DEF(eigenmtx, 3, 3);
//     zsl_mtx_init(&eigenmtx, NULL);
//
//     /* Display the input matrix. */
//     printf("Input:\n");
//     zsl_mtx_print(&m);
//
//     /* Determine the eigenvectors/value of matrix 'm'. */
//     rc = zsl_mtx_eigen(&m, &eigenval, &eigenmtx);
//     if (rc) {
//             goto err;
//     }
//
//     /* Display the results. */
//     printf("\nEigenvalue: %f\n", eigenval);
//     zsl_mtx_print(&eigenmtx);
//
//     return 0;
// err:
//     printf("Ooops! An error occurred in '%s'\n", __FUNCTION__);
//     return rc;
// }

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

    /* Display the m matrix. It should be the identity. */
    printf("\nDiagonal:\n");
    zsl_mtx_print(&m);

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

    /* Create an (n-1)x(n-1) matrix to place the values of the reduced matrix in. */
    ZSL_MATRIX_DEF(mr,4,4);
    zsl_mtx_init(&mr,NULL);

    /* Calculate the reduced matrix of 'm', placing the values in 'mr'. */
    zsl_mtx_red(&m, &mr, 0, 0);

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

    printf("--- nxn reduce tester --------------------\n");
    zsl_mtx_test_red();
    printf("------------------------------------\n\n");

    printf("--- nxn inverse tester --------------------\n");
    zsl_mtx_test_inv_nxn();
    printf("------------------------------------\n\n");

    return 0;
}
