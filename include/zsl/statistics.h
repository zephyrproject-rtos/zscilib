/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * \defgroup STATISTICS Statistics
 *
 * @brief Statistics-related functions.
 *
 * @{
 */

/**
 * @file
 * @brief API header file for statistics in zscilib.
 *
 * This file contains the zscilib statistics APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_STATISTICS_H_
#define ZEPHYR_INCLUDE_ZSL_STATISTICS_H_

#include <zsl/zsl.h>
#include <zsl/vectors.h>
#include <zsl/matrices.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Simple linear regression coefficients. */
struct zsl_sta_linreg {
	/**
	 * @brief The estimated slope.
	 */
	zsl_real_t slope;
	/**
	 * @brief The estimated intercept.
	 */
	zsl_real_t intercept;
	/**
	 * @brief The correlation coefficient, where closer to 1.0 is better.
	 */
	zsl_real_t correlation;
};

/**
 * @brief Computes the arithmetic mean (average) of a vector.
 *
 * @param v  The vector to use.
 * @param m  The arithmetic mean of the components of v.
 *
 * @return 0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_sta_mean(struct zsl_vec *v, zsl_real_t *m);

/**
 * @brief Computes the trimmed arithmetic mean (average) of a vector.
 *
 * The trimmed arithmetic mean of a dataset is described by a number (in this
 * case 'p') from 0 to 50 that describes the percent of the data that will not
 * be taken into account when computing the mean. Thus, a 3% trimmed
 * mean will only use 94% of the data to calculate the arithmetic mean, and
 * will ignore the lowest 3% of data and the highest 3% of data in the sorted
 * data vector.
 *
 * @param v  The vector to use.
 * @param p  The percent of data that will be ignored in the computation of
 *           the mean (0.0 .. 50.0).
 * @param m  The trimmed arithmetic mean of the components of v.
 *
 * @return 0 if everything executed correctly, -EINVAL if the number 'p' is not
 *         between 0.0 and 50.0.
 */
int zsl_sta_trim_mean(struct zsl_vec *v, zsl_real_t p, zsl_real_t *m);

/**
 * @brief Computes the weighted arithmetic mean (average) of a data vector (v)
 *        and a weight vector (w).
 *
 * @param v  The data vector to use.
 * @param w  The vector containing the weights to use.
 * @param m  The weighted arithmetic mean of the components of v taking the
 *           weights in the vector w into account.
 *
 * @return 0 if everything executed correctly, -EINVAL if the dimensions of v
 *         and w don't match, or if any weights are negative or all of them
 *         are zero.
 */
int zsl_sta_weighted_mean(struct zsl_vec *v, struct zsl_vec *w, zsl_real_t *m);

/**
 * @brief Computes the time-weighted arithmetic mean (average) of a positive
 *        data vector (v) and its time vector (w).
 *
 * The time-weighted mean takes into consideration not only the numerical
 * levels of a particular variable, but also the amount of time spent on it.
 *
 * @param v  The data vector to use, with positive coefficients.
 * @param t  The vector containing the time associated to the data vector.
 * @param m  The time-weighted arithmetic mean of the components of v taking
 *           the times in the vector t into account.
 *
 * @return 0 if everything executed correctly, -EINVAL if the dimensions of v
 *         and w don't match, or if any elements in 'v' are negative or if
 *         any time value in the vector 't' is repeated.
 */
int zsl_sta_time_weighted_mean(struct zsl_vec *v, struct zsl_vec *t,
			       zsl_real_t *m);

/**
 * @brief Subtracts the mean of vector v from every component of the vector.
 *        The output vector w then has a zero mean.
 *
 * @param v  The vector to use.
 * @param w  The output vector with zero mean.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_sta_demean(struct zsl_vec *v, struct zsl_vec *w);

/**
 * @brief Computes the given percentile of a vector.
 *
 * @param v    The input vector.
 * @param p    The percentile to be calculated.
 * @param val  The output value.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_sta_percentile(struct zsl_vec *v, zsl_real_t p, zsl_real_t *val);

/**
 * @brief Computes the median of a vector (the value separating the higher half
 *        from the lower half of a data sample).
 *
 * @param v  The vector to use.
 * @param m  The median of the components of v.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_sta_median(struct zsl_vec *v, zsl_real_t *m);

/**
 * @brief Computes the weighted median of a data vector (v) and a weight
 *        vector (w).
 *
 * @param v  The data vector to use.
 * @param w  The vector containing the weights to use.
 * @param m  The weighted median of the components of v taking the weights in
 *           the vector w into account.
 *
 * @return 0 if everything executed correctly, -EINVAL if the dimensions of v
 *         and w don't match, or if any weights are negative or the sum of all
 *         the weights is not 1.
 */
int zsl_sta_weighted_median(struct zsl_vec *v, struct zsl_vec *w, zsl_real_t *m);

/**
 * @brief Calculates the first, second and third quartiles of a vector v.
 *
 * @param v   The vector to use.
 * @param q1  The first quartile of v.
 * @param q2  The second quartile of v, also the median of v.
 * @param q3  The third quartile of v.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_sta_quart(struct zsl_vec *v, zsl_real_t *q1, zsl_real_t *q2,
		  zsl_real_t *q3);

/**
 * @brief Calculates the numeric difference between the third and the first
 *        quartiles of a vector v.
 *
 * @param v  The input vector.
 * @param r  The interquartile range of v.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_sta_quart_range(struct zsl_vec *v, zsl_real_t *r);

/**
 * @brief Computes the mode or modes of a vector v.
 *
 * @param v  The vector to use.
 * @param w  Output vector whose components are the modes. If there is only
 *           one mode, the length of w will be 1.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_sta_mode(struct zsl_vec *v, struct zsl_vec *w);

/**
 * @brief Computes the difference between the greatest value and the lowest in
 *        a vector v.
 *
 * @param v The vector to use.
 * @param r The range of the data in v.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_sta_data_range(struct zsl_vec *v, zsl_real_t *r);

/**
 * @brief Computes the mean absolute deviation of a data vector v.
 *
 * The mean absolute deviation is calculated by computing the mean of the
 * de-meaned data vector, i. e., the arithmetic mean of the absolute value of
 * each value in v minus the mean of the data in 'v'. This number describes the
 * average deviation from the arithmetic mean of the dataset in the vector 'v'.
 *
 * @param v The vector to use.
 * @param m The mean absolute deviation.
 *
 * @return  0 if everything executed correctly. If the dimension of the data
 *          vector v is zero, a negative error is returned.
 */
int zsl_sta_mean_abs_dev(struct zsl_vec *v, zsl_real_t *m);

/**
 * @brief Computes the median absolute deviation of a data vector v.
 *
 * The mean absolute deviation is calculated by computing the median of the
 * absolute value of each value in 'v' minus the median of the data in 'v'.
 * This provides a robust estimate of variability.
 *
 * @param v The vector to use.
 * @param m The median absolute deviation.
 *
 * @return  0 if everything executed correctly. otherwise an appropriate
 *          error code.
 */
int zsl_sta_median_abs_dev(struct zsl_vec *v, zsl_real_t *m);

/**
 * @brief Computes the variance of a vector v (the average of the squared
 *        differences from the mean).
 *
 * @param v     The vector to use.
 * @param var   The variance of v.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_sta_var(struct zsl_vec *v, zsl_real_t *var);

/**
 * @brief Computes the standard deviation of vector v.
 *
 * Standard deviation is an indication of how spread-out numbers in 'v' are,
 * relative to the mean. It helps differentiate what is in the "standard"
 * range (1 standard deviation from mean), and what is outside (above or below)
 * this range, to pick out statistical outliers.
 *
 * @param v  The vector to use.
 * @param s  The output standard deviation of the vector v.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_sta_std_dev(struct zsl_vec *v, zsl_real_t *s);

/**
 * @brief Computes the variance of two sets of data: v and w.
 *
 * @param v  First set of data.
 * @param w  Second set of data.
 * @param c  Covariance of the vectors v and w.
 *
 * @return 0 on success, and -EINVAL if the vectors aren't identically sized.
 */
int zsl_sta_covar(struct zsl_vec *v, struct zsl_vec *w, zsl_real_t *c);

/**
 * @brief Calculates the nxn covariance matrix of a set of n vectors of the
 *        same length.
 *
 * @param m   Input matrix, whose columns are the different data sets.
 * @param mc  Output nxn covariance matrix.
 *
 * @return 0 on success, and -EINVAL if 'mc' is not a square matrix with the
 *         same number of columns as 'm'.
 */
int zsl_sta_covar_mtx(struct zsl_mtx *m, struct zsl_mtx *mc);

/**
 * @brief Calculates the slope, intercept and correlation coefficient of the
 *        linear regression of two vectors, allowing us to make a prediction
 *        of y on the basis of x.
 *
 * Simple linear regression is useful for predicting a quantitative response.
 * It assumes that there is an approximately linear relationship between vector
 * x and vector y, and calculates a series of coefficients to project this
 * relationship in either direction.
 *
 * The output of this function is a slope and intercept value, such
 * that the resulting line closely tracks the linear progression of the input
 * samples. The correlation coefficient  estimates the 'closeness' of the
 * match.
 *
 * Given the equation 'y = slope * x + intercept', where we provide x, we can
 * estimate the y value for a arbitrary value of x, where x is related to the
 * range of values provided in vector 'x' (the x axis), and y is related to the
 * values provided in vector 'y' (the y axis).
 *
 * Simple linear regression is a special case of the multiple linear regression
 * (see below). The correlation coefficient is the square root of the
 * coefficient of determination, a measure useful in multiple linear regression.
 *
 * @param x   The first input vector, corresponding to the x-axis.
 * @param y   The second input vector, corresponding to the y-axis.
 * @param c   Pointer to the calculated linear regression coefficients.
 *
 * @return 0 on success, and -EINVAL if the vectors aren't identically sized.
 */
int zsl_sta_linear_reg(struct zsl_vec *x, struct zsl_vec *y,
		       struct zsl_sta_linreg *c);

#ifndef CONFIG_ZSL_SINGLE_PRECISION
/**
 * @brief Calculates the coefficients (vector 'b') of the multiple linear
 *        regression of the x_i values (columns of the matrix 'x') and the y
 *        values.
 *
 * @param x   Matrix, whose columns are the different x_i datasets.
 * @param y   The second input dataset, corresponding to the y-axis.
 * @param b   Pointer to the calculated multiple linear regression coefficients.
 * @param r   Pointer to the calculated coefficient of determination (also
 *            reffered to as R squared).
 *
 * @return 0 on success, and -EINVAL if dimensions of the input vectors and
 *         matrix don't match.
 */
int zsl_sta_mult_linear_reg(struct zsl_mtx *x, struct zsl_vec *y,
			    struct zsl_vec *b, zsl_real_t *r);
#endif

#ifndef CONFIG_ZSL_SINGLE_PRECISION
/**
 * @brief Calculates the coefficients (vector 'b') of the weighted multiple
 *        linear regression of the x_i values (columns of the matrix 'x'), the y
 *        values and the weights in the vector 'w'.
 *
 * @param x   Matrix, whose columns are the different x_i datasets.
 * @param y   The second input dataset, corresponding to the y-axis.
 * @param w   The weights to use in the weighted least squares.
 * @param b   Pointer to the calculated weighted multiple linear regression
 *            coefficients.
 * @param r   Pointer to the calculated coefficient of determination (also
 *            reffered to as R squared).
 *
 * @return 0 on success, and -EINVAL if dimensions of the input vectors and
 *         matrix don't match.
 */
int zsl_sta_weighted_mult_linear_reg(struct zsl_mtx *x, struct zsl_vec *y,
				     struct zsl_vec *w, struct zsl_vec *b, zsl_real_t *r);
#endif

#ifndef CONFIG_ZSL_SINGLE_PRECISION
/**
 * @brief This function uses the least squares fitting method to compute the
 *        coefficients of a quadric surface given a set of tridimensional
 *        points.
 *
 * A quadric is a 3D surface that is defined by the equation:
 * Ax^2 + By^2 + Cz^2 + 2Dxy + 2Exz + 2Fyz + 2Gx + 2Hy + 2Iz = 1.
 * Spheres and ellipsoids are special cases of quadrics. This function takes a
 * set of points (x,y,z) and returns the coeffitiens (A, B, C, D, E, F, G, H, I)
 * of the quadric surface that best fit the given points.
 *
 * @param m   Matrix, whose rows are the (x, y, z) points.
 * @param b   Pointer to the calculated coefficients of the quadric.
 *
 * @return 0 on success, and -EINVAL if dimension of the input vectors isn't 9
 *         and the input matrix isn't a Nx3 matrix.
 */
int zsl_sta_quad_fit(struct zsl_mtx *m, struct zsl_vec *b);
#endif

/**
 * @brief Calculates the absolute error given a value and its expected value.
 *
 * @param val       Input value.
 * @param exp_val   Input expected value.
 * @param err       Output absolute error.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_sta_abs_err(zsl_real_t *val, zsl_real_t *exp_val, zsl_real_t *err);

/**
 * @brief Calculates the relative error given a value and its expected value.
 *
 * @param val       Input value.
 * @param exp_val   Input expected value.
 * @param err       Output relative error.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_sta_rel_err(zsl_real_t *val, zsl_real_t *exp_val, zsl_real_t *err);

/**
 * @brief Calculates the standard error of the mean of a sample (vector v).
 *
 * The standard error of the mean measures how far the arithmetic mean of the
 * sample in vector 'v' ¡ is likely to be from the true total population mean.
 *
 * @param v       Sample data vector.
 * @param err     Output standard error of the mean.
 *
 * @return  0 if everything executed correctly. If the dimension of the vector
 *          'v' is zero, a negative error is returned.
 */
int zsl_sta_sta_err(struct zsl_vec *v, zsl_real_t *err);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_STATISTICS_H_ */

/** @} */ /* End of statistics group */
