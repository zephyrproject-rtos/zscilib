/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * \defgroup PROBABILITY Probability
 *
 * @brief Probability-related functions.
 *
 * @{
 */

/**
 * @file
 * @brief API header file for probability in zscilib.
 *
 * This file contains the zscilib probability APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_PROBABILITY_H_
#define ZEPHYR_INCLUDE_ZSL_PROBABILITY_H_

#include <zsl/zsl.h>
#include <zsl/vectors.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Computes the image of a value x of the uniform probability
 *        distribution function (PDF), which has zero probability everywhere
 *        except in the interval (a, b).
 *
 * @param a  The lower bound of the interval in which the probability is
 *           not zero.
 * @param b  The higher bound of the interval in which the probability is
 *           not zero.
 * @param x  Value to calculate its image.
 *
 * @return The uniform probility of the value x. If b <= a, then it
 *         returns -EINVAL.
 */
zsl_real_t zsl_prob_uni_pdf(zsl_real_t *a, zsl_real_t *b, zsl_real_t *x);

/**
 * @brief Computes the mean of a uniform probability distribution function of
 *        interval (a, b).
 *
 * @param a  The lower bound of the interval in which the probability is
 *           not zero.
 * @param b  The higher bound of the interval in which the probability is
 *           not zero.
 * @param m  The arithmetic mean of the uniform probability distribution.
 *
 * @return 0 on success, and -EINVAL if b <= a.
 */
int zsl_prob_uni_mean(zsl_real_t *a, zsl_real_t *b, zsl_real_t *m);

/**
 * @brief Computes the variance of a uniform probability distribution function
 *        of interval (a, b).
 *
 * @param a  The lower bound of the interval in which the probability is
 *           not zero.
 * @param b  The higher bound of the interval in which the probability is
 *           not zero.
 * @param v  The variance of the uniform probability distribution.
 *
 * @return 0 on success, and -EINVAL if b <= a.
 */
int zsl_prob_uni_var(zsl_real_t *a, zsl_real_t *b, zsl_real_t *v);

/**
 * @brief Computes the image of a value x of the uniform cumulative
 *        distribution function (CDF) with interval (a, b).
 *
 * @param a  The lower bound of the interval in which the probability is
 *           not zero.
 * @param b  The higher bound of the interval in which the probability is
 *           not zero.
 * @param x  Value to calculate its image.
 *
 * @return The uniform cumulative density image of the value x. If b <= a,
 *         then it returns -EINVAL.
 */
zsl_real_t zsl_prob_uni_cdf(zsl_real_t *a, zsl_real_t *b, zsl_real_t *x);

/**
 * @brief Computes the image of a value x of the normal probability
 *        distribution function (PDF) of mean 'm' and standard deviation 's'.
 *
 * @param m  Mean value of the normal distribution.
 * @param s  Standard deviation of the normal distribution.
 * @param x  Value to calculate its image.
 *
 * @return The normal probility of the value x, otherwise an appropriate
 *         error code.
 */
zsl_real_t zsl_prob_normal_pdf(zsl_real_t *m, zsl_real_t *s, zsl_real_t *x);

/**
 * @brief Computes the image of a value x of the normal cumulative
 *        distribution function (CDF) of mean 'm' and standard deviation 's'.
 *
 * @param m  Mean value of the normal distribution.
 * @param s  Standard deviation of the normal distribution.
 * @param x  Value to calculate its image.
 *
 * @return The normal cumulative density image of the value x, otherwise an
 *         appropriate error code.
 */
zsl_real_t zsl_prob_normal_cdf(zsl_real_t *m, zsl_real_t *s, zsl_real_t *x);

/**
 * @brief Computes the value of the inverse error fuction of a value 'x' using
 *        the Chebyshev interpolation to estimate the value of the function.
 *
 * @param x  The input value, between -1 and 1 (not included).
 *
 * @return The image of the inverse error function of the value x, and -EINVAL
 *         if x is outside the open interval (-1, 1).
 */
zsl_real_t zsl_prob_erf_inv(zsl_real_t *x);

/**
 * @brief Computes the inverse of the normal cumulative distribution function
 *        of a value x.
 *
 * @param m  Mean value of the normal distribution.
 * @param s  Standard deviation of the normal distribution.
 * @param x  Value to calculate its image. Must be in the open interval (0, 1).
 *
 * @return The inverse of the normal cumulative distribution function of 'x'.
 *         If x >= 1 or x is negative of zero, it returns -EINVAL.
 */
zsl_real_t zsl_prob_normal_cdf_inv(zsl_real_t *m, zsl_real_t *s, zsl_real_t *x);


/**
 * @brief Computes the factorial of a natural number, i. e., the finite product
 *        n * (n - 1) * (n - 2) * (n - 3) * ... * 3 * 2 * 1.
 *
 * @param n  Natural number to compute its factorial.
 *
 * @return The factorial of the input number n. If the input number is
 *         negative, it returns -EINVAL.
 */
int zsl_prob_factorial(int *n);

/**
 * @brief Computes binomial coefficient of n and k (commonly writen as 'n over
 *        k').
 *
 * @param n  The first input natural number.
 * @param k  The second input natural number. This number can be negative or
 *           bigger than 'n', but then the function will be returning zero.
 * @param c  The combinatory number of 'n' over 'k'.
 *
 * @return 0 on success, -EINVAL if n is negative.
 */
int zsl_prob_binomial_coef(int *n, int *k, int *c);

/**
 * @brief Computes the image of a value x of the binomial probability
 *        distribution function (PDF).
 *
 * The binomial distribution describes how likely it is to get the same outcome
 * of an experiment exactly 'x' out of the 'n' times the experiment is repeated.
 * In this formula, 'p' is the probability to get such outcome.
 *
 * @param n  The number of times the experiment has been done.
 * @param p  The probability of the outcome.
 * @param x  The number of times we want the outcome.
 *
 * @return 0 on success, -EINVAL if n is negative or the probability is not
 *         between 0 and 1.
 */
zsl_real_t zsl_prob_binomial_pdf(int *n, zsl_real_t *p, int *x);

/**
 * @brief Computes the mean of a binomial probability distribution function.
 *
 * @param n  The number of times the experiment has been done.
 * @param p  The probability of the outcome.
 * @param m  The mean value of the binomial distribution.
 *
 * @return 0 on success, -EINVAL if n is negative or the probability is not
 *         between 0 and 1.
 */
int zsl_prob_binomial_mean(int *n, zsl_real_t *p, zsl_real_t *m);

/**
 * @brief Computes the variance of a binomial probability distribution function.
 *
 * @param n  The number of times the experiment has been performed.
 * @param p  The probability of the outcome.
 * @param v  The variance value of the binomial distribution.
 *
 * @return 0 on success, -EINVAL if n is negative or the probability is not
 *         between 0 and 1.
 */
int zsl_prob_binomial_var(int *n, zsl_real_t *p, zsl_real_t *v);

/**
 * @brief  Computes the image of a value x of the ubinomial cumulative
 *         distribution function (CDF).
 *
 * @param n  The number of times the experiment has been done.
 * @param p  The probability of the outcome.
 * @param x  The number of times we want the outcome.
 *
 * @return 0 on success, -EINVAL if n is negative or the probability is not
 *         between 0 and 1.
 */
zsl_real_t zsl_prob_binomial_cdf(int *n, zsl_real_t *p, int *x);

/**
 * @brief Computes the factorial of a natural number, i. e., the finite product
 *        n * (n - 1) * (n - 2) * (n - 3) * ... * 3 * 2 * 1.
 *
 * @param n  Natural number to compute its factorial.
 *
 * @return The factorial of the input number n. If the input number is
 *         negative, it returns -EINVAL.
 */
int zsl_prob_factorial(int *n);

/**
 * @brief Computes binomial coefficient of n and k (commonly writen as 'n over
 *        k').
 *
 * @param n  The first input natural number.
 * @param k  The second input natural number. This number can be negative or
 *           bigger than 'n', but then the function will be returning zero.
 * @param c  The combinatory number of 'n' over 'k'.
 *
 * @return 0 on success, -EINVAL if n is negative.
 */
int zsl_prob_binomial_coef(int *n, int *k, int *c);

/**
 * @brief Computes the image of a value x of the binomial probability
 *        distribution function (PDF).
 *
 * The binomial distribution describes how likely it is to get the same outcome
 * of an experiment exactly 'x' out of the 'n' times the experiment is repeated.
 * In this formula, 'p' is the probability to get such outcome.
 *
 * @param n  The number of times the experiment has been done.
 * @param p  The probability of the outcome.
 * @param x  The number of times we want the outcome.
 *
 * @return 0 on success, -EINVAL if n is negative or the probability is not
 *         between 0 and 1.
 */
zsl_real_t zsl_prob_binomial_pdf(int *n, zsl_real_t *p, int *x);

/**
 * @brief Computes the mean of a binomial probability distribution function.
 *
 * @param n  The number of times the experiment has been done.
 * @param p  The probability of the outcome.
 * @param m  The mean value of the binomial distribution.
 *
 * @return 0 on success, -EINVAL if n is negative or the probability is not
 *         between 0 and 1.
 */
int zsl_prob_binomial_mean(int *n, zsl_real_t *p, zsl_real_t *m);

/**
 * @brief Computes the variance of a binomial probability distribution function.
 *
 * @param n  The number of times the experiment has been performed.
 * @param p  The probability of the outcome.
 * @param v  The variance value of the binomial distribution.
 *
 * @return 0 on success, -EINVAL if n is negative or the probability is not
 *         between 0 and 1.
 */
int zsl_prob_binomial_var(int *n, zsl_real_t *p, zsl_real_t *v);

/**
 * @brief  Computes the image of a value x of the ubinomial cumulative
 *         distribution function (CDF).
 *
 * @param n  The number of times the experiment has been done.
 * @param p  The probability of the outcome.
 * @param x  The number of times we want the outcome.
 *
 * @return 0 on success, -EINVAL if n is negative or the probability is not
 *         between 0 and 1.
 */
zsl_real_t zsl_prob_binomial_cdf(int *n, zsl_real_t *p, int *x);

/**
 * @brief Computes the Shannon entropy of a set of events with given
 *        probabilities.
 *
 * @param v  The vector with the probabilities of the events.
 * @param h  The Shannon entropy of the probabilities in v.
 *
 * @return 0 on success, -EINVAL if the probabilities in v don't add up to 1 or
 *         any of the probabilities is negative.
 */
int zsl_prob_entropy(struct zsl_vec *v, zsl_real_t *h);

/**
 * @brief Computes the probability of an event A to occur given the event B,
 *        based on the probability of A, B and the probability of B given A,
 *        using the Bayes' Theorem.
 *
 * @param pa    Probability of the event A.
 * @param pb    Probability of the event B.
 * @param pba   Probability of the event B given the event A.
 * @param pab   Probability of the event A given the event B.
 *
 * @return 0 on success, -EINVAL if the probabilities are greater than one or
 *         negative, or if P(B|A) * P(A) > P(B).
 */
int zsl_prob_bayes(zsl_real_t *pa, zsl_real_t *pb, zsl_real_t *pba,
		   zsl_real_t *pab);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_PROBABILITY_H_ */

/** @} */ /* End of probability group */
