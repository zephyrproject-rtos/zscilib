#include <stdio.h>
#include <errno.h>
#include "zsl/matrices.h"
#include "zsl/vectors.h"

/* Input 3x3 matrix used for various tests in this module. */
zsl_real_t data[9] = { 1.0, 0.0, 0.0,
                       0.0, 0.5, 0.0,
                       0.0, 0.0, 0.1 };

struct zsl_mtx m = {
    .sz_rows = 3,
    .sz_cols = 3,
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

/**
 * Performs a known-good test just as a demonstration.
 */
int
zsl_mtx_test_inv(void)
{
        int rc;

        /* Declare and init the empty 3x3 inverse matrix placeholder. */
        ZSL_MATRIX_DEF(minv, 3, 3);
        zsl_mtx_init(&minv, NULL);

        /* Display the input matrix. */
        printf("Input:\n");
	zsl_mtx_print(&m);

        /* Calculate the inverse of 'm', placing the values in 'minv'. */
        rc = zsl_mtx_inv(&m, &minv);

        /* Display the inverse. */
        printf("\nInverse:\n");
	zsl_mtx_print(&minv);

        return 0;
}

/**
 * @brief Attempts to calculate the eigenvetor/value of the supplied matrix.
 *
 * This function declares two different 3x3 matrices, and a single real
 * number variable:
 *
 * m            A 3x3 matrix that we will define specific initial values for.
 * eigenmtx     An empty 3x3 matrix that will be populated with values by
 *              'zsl_mtx_eigen'.
 * eigenval     A real number that will be updated by 'zsl_mtx_eigen' with the
 *              eignvalue associated with matrix 'm'.
 *
 * @return 0 if everything executed properly, otherwise an error code.
 */
int
zsl_mtx_test_eigen(void)
{
	int rc;
        zsl_real_t eigenval;

        /* Declare and init the empty 3x3 eigenvector matrix. */
        ZSL_MATRIX_DEF(eigenmtx, 3, 3);
        zsl_mtx_init(&eigenmtx, NULL);

        /* Display the input matrix. */
        printf("Input:\n");
	zsl_mtx_print(&m);

        /* Determine the eigenvectors/value of matrix 'm'. */
        rc = zsl_mtx_eigen(&m, &eigenval, &eigenmtx);
        if (rc) {
                goto err;
        }

        /* Display the results. */
        printf("\nEigenvalue: %f\n", eigenval);
        zsl_mtx_print(&eigenmtx);

        return 0;
err:
        printf("Ooops! An error occurred in '%s'\n", __FUNCTION__);
	return rc;
}

/**
 * Program entry point.
 */
int
main(void)
{
	printf("Hello, zscilib!\n\n");

        /* Display the results of a known-good operation. */
        printf("--- 3x3 Inverse --------------------\n");
        zsl_mtx_test_inv();
        printf("------------------------------------\n\n");

        /* Execute the test function and return the resulting code. */
        printf("--- 3x3 Eigenvector ----------------\n");
	zsl_mtx_test_eigen();
        printf("------------------------------------\n\n");

        return 0;
}
