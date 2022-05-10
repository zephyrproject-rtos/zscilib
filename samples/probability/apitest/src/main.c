/*
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zsl/zsl.h>
#include <zsl/probability.h>

void probability_demo(void)
{
	printf("Probability API demo\n");
	printf("--------------------\n\n");

	printf("A particle is located in an unidimensional box that stretches\n");
	printf("from x = -5.0 to x = 5.0. The particle has the same probability\n");
	printf("of being at any point inside the box. Calculate the mean, the\n");
	printf("median, the variance and the probability of the particle being\n");
	printf("between x = -1.5 and x = 3.0.\n\n\n");

	zsl_real_t a = -5.0, b = 5.0;
	zsl_real_t x, mean, var, prob1, prob2;

	/* Calculate the mean. */
	zsl_prob_uni_mean(&a, &b, &mean);
	printf("The mean value is %f.\n\n", mean);

	/* Calculate the median, which is the height of the pdf. */
	x = 0.0;
	printf("The median value is %f.\n\n", zsl_prob_uni_pdf(&a, &b, &x));

	/* Calculate the variance. */
	zsl_prob_uni_var(&a, &b, &var);
	printf("The variance is %f.\n\n", var);

	/* Calculate the probability of the particle being in (-INFINITY, -1.5)
	 * and (-INFINITY, 3.0) and subtract them to get the probability of it
	 * being in (-1.5, 3.0). */
	x = -1.5;
	prob1 = zsl_prob_uni_cdf(&a, &b, &x);
	x = 3.0;
	prob2 = zsl_prob_uni_cdf(&a, &b, &x);
	printf("The probability of the particle being between x = -1.5 and\n");
	printf("x = 3.0 is %f.\n\n\n", prob2 - prob1);

	printf("The particle has now a 50%% chance of being in the interval\n");
	printf("(-5.0, -3.5), a 10%% chance of being in (-3.5, 0.5), a 15%%\n");
	printf("chance of being in (0.5, 4.0) and a 25%% chance of being\n");
	printf("between x = 4.0 and x = 5.0. Calculate the Shannon entropy of\n");
	printf("the system.\n\n\n");

	/* Create the probability vector from the data given. */
	ZSL_VECTOR_DEF(v, 4);
	zsl_real_t p[4] = { 0.5, 0.1, 0.15, 0.25 };

	zsl_vec_from_arr(&v, p);

	/* Calculate the Shannon entropy. */
	zsl_real_t h;

	zsl_prob_entropy(&v, &h);
	printf("The Shannon entropy of the system is %f.\n\n\n", h);

	printf("Finally, the particle leaves the box and moves freely. After a\n");
	printf("while, we know that the function that describes the probability\n");
	printf("density of the particle in the unidimensional space is a\n");
	printf("gaussian function with mean 3.0 and standard deviation 0.5.\n");
	printf("Determine the highest point of this function, the probability\n");
	printf("of the particle being between x = 2.75 and x = 4.0 and the\n");
	printf("value of x such as the probability of the particle being in the\n");
	printf("interval (-INFINITY, x) is 0.3.\n\n\n");

	zsl_real_t m = 3.0, s = 0.5, prob;

	/* Calculate the highest point, which is at x = 3.0 (the mean). */
	x = 3.0;
	h = zsl_prob_normal_pdf(&m, &s, &x);
	printf("The highest value of the normal distribution is %f.\n\n", h);

	/* Calculate the probability of the particle being in (-INFINITY, 2.75)
	 * and (-INFINITY, 4.0) and subtract them to get the probability of it
	 * being in (2.75, 4.0). */
	x = 2.75;
	prob1 = zsl_prob_normal_cdf(&m, &s, &x);
	x = 4.0;
	prob2 = zsl_prob_normal_cdf(&m, &s, &x);
	prob = prob2 - prob1;
	printf("The probability of the particle being between x = 2.75 and\n");
	printf("x = 4.0 is %f.\n\n", prob);

	/* Calculate the value of x that makes the probability of the particle
	 * being in (-INFINITY, x) equal 0.3. */
	zsl_real_t pr = 0.3;

	printf("The chance of the particle being between x = -INFINITY and\n");
	printf("x = %f is 30%%.\n\n\n", zsl_prob_normal_cdf_inv(&m, &s, &pr));

	printf("We want to determine the position of ten particles with the\n");
	printf("same probability distribution (gaussian, with mean 3.0 and\n");
	printf("standard deviation 0.5). What is the probability of finding\n");
	printf("exactly six of the ten particles between x = 2.75 and x = 4.0?\n");
	printf("\n\n");

	int n = 10, x_bd = 6;

	/* Use the binomial distribution pdf function to calculate the probability
	 * of finding six particles between x = 2.75 and x = 4.0. */
	h = zsl_prob_binomial_pdf(&n, &prob, &x_bd);
	printf("The probability is %f.\n\n", h);
}

void main(void)
{
	printf("\n\nzscilib probability demo\n\n");

	probability_demo();
}
