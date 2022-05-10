/*
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zsl/zsl.h>
#include <zsl/statistics.h>

void statistics_demo(void)
{
	zsl_real_t ngp_mean, ngp_trim_mean, ngp_median, ngp_q1, ngp_q2, ngp_q3;
	zsl_real_t ngp_qrng, ngp_drng, ngp_var, ngp_stdev, ngp_sterr;
	zsl_real_t ngp_mean_absdev, ngp_median_absdev;

	ZSL_VECTOR_DEF(ngp_mode, 284);
	zsl_real_t cpi_covar;
	struct zsl_sta_linreg coef;

	ZSL_VECTOR_DEF(ngpvec, 284);            /* Natural gas prices. */
	ZSL_VECTOR_DEF(cpivalvec, 204);         /* CPI values. */
	ZSL_VECTOR_DEF(cpiperiodvec, 204);      /* CPI period. */
	ZSL_VECTOR_DEF(ngpvecsub, 204);
	ZSL_MATRIX_DEF(ngpmtx, 204, 2);
	ZSL_MATRIX_DEF(ngp_covar_mtx, 2, 2);

	printf("Statistics API demo\n");
	printf("-------------------\n\n");

	/* Source: https://datahub.io/core/natural-gas#resource-monthly. */
	zsl_real_t ngp_data[284] = {
		3.45, 2.15, 1.89, 2.03, 2.25, 2.20,             // Year 1997
		2.19, 2.49, 2.88, 3.07, 3.01, 2.35,
		2.09, 2.23, 2.24, 2.43, 2.17, 2.14,             // Year 1998
		2.17, 1.85, 2.02, 1.91, 2.12, 1.72,
		1.85, 1.77, 1.79, 2.15, 2.26, 2.30,             // Year 1999
		2.31, 2.80, 2.55, 2.73, 2.37, 2.36,
		2.42, 2.66, 2.79, 3.04, 3.59, 4.29,             // Year 2000
		3.99, 4.43, 5.06, 5.02, 5.52, 8.90,
		8.17, 5.61, 5.23, 5.19, 4.19, 3.72,             // Year 2001
		3.11, 2.97, 2.19, 2.46, 2.34, 2.30,
		2.32, 2.32, 3.03, 3.43, 3.50, 3.26,             // Year 2002
		2.99, 3.09, 3.55, 4.13, 4.04, 4.74,
		5.43, 7.71, 5.93, 5.26, 5.81, 5.82,             // Year 2003
		5.03, 4.99, 4.62, 4.63, 4.47, 6.13,
		6.14, 5.37, 5.39, 5.71, 6.33, 6.27,             // Year 2004
		5.93, 5.41, 5.15, 6.35, 6.17, 6.58,
		6.15, 6.14, 6.96, 7.16, 6.47, 7.18,             // Year 2005
		7.63, 9.53, 11.75, 13.42, 10.3, 13.05,
		8.69, 7.54, 6.89, 7.16, 6.25, 6.21,             // Year 2006
		6.17, 7.14, 4.90, 5.85, 7.41, 6.73,
		6.55, 8.00, 7.11, 7.60, 7.64, 7.35,             // Year 2007
		6.22, 6.22, 6.08, 6.74, 7.10, 7.11,
		7.99, 8.54, 9.41, 10.18, 11.27, 12.69,          // Year 2008
		11.09, 8.26, 7.67, 6.74, 6.68, 5.82,
		5.24, 4.52, 3.96, 3.50, 3.83, 3.80,             // Year 2009
		3.38, 3.14, 2.99, 4.01, 3.66, 5.35,
		5.83, 5.32, 4.29, 4.03, 4.14, 4.80,             // Year 2010
		4.63, 4.32, 3.89, 3.43, 3.71, 4.25,
		4.49, 4.09, 3.97, 4.24, 4.31, 4.54,             // Year 2011
		4.42, 4.06, 3.90, 3.57, 3.24, 3.17,
		2.67, 2.51, 2.17, 1.95, 2.43, 2.46,             // Year 2012
		2.95, 2.84, 2.85, 3.32, 3.54, 3.34,
		3.33, 3.33, 3.81, 4.17, 4.04, 3.83,             // Year 2013
		3.62, 3.43, 3.62, 3.68, 3.64, 4.24,
		4.71, 6.00, 4.90, 4.66, 4.58, 4.59,             // Year 2014
		4.05, 3.91, 3.92, 3.78, 4.12, 3.48,
		2.99, 2.87, 2.83, 2.61, 2.85, 2.78,             // Year 2015
		2.84, 2.77, 2.66, 2.34, 2.09, 1.93,
		2.28, 1.99, 1.73, 1.92, 1.92, 2.59,             // Year 2016
		2.82, 2.82, 2.99, 2.98, 2.55, 3.59,
		3.30, 2.85, 2.88, 3.10, 3.15, 2.98,             // Year 2017
		2.98, 2.90, 2.98, 2.88, 3.01, 2.82,
		3.87, 2.67, 2.69, 2.80, 2.80, 2.97,             // Year 2018
		2.83, 2.96, 3.00, 3.28, 4.09, 4.04,
		3.11, 2.69, 2.95, 2.65, 2.64, 2.40,             // Year 2019
		2.37, 2.22, 2.56, 2.33, 2.65, 2.22,
		2.02, 1.91, 1.79, 1.74, 1.75, 1.63,             // Year 2020
		1.77, 2.30
	};

	/* Source: https://datahub.io/core/cpi-us */
	zsl_real_t cpival_data[204] = {
		159.1, 159.6, 160.0, 160.2, 160.1, 160.3,               // Year 1997
		160.5, 160.8, 161.2, 161.6, 161.5, 161.3,
		161.6, 161.9, 162.2, 162.5, 162.8, 163.0,               // Year 1998
		163.2, 163.4, 163.6, 164.0, 164.0, 163.9,
		164.3, 164.5, 165.0, 166.2, 166.2, 166.2,               // Year 1999
		166.7, 167.1, 167.9, 168.2, 168.3, 168.3,
		168.8, 169.8, 171.2, 171.3, 171.5, 172.4,               // Year 2000
		172.8, 172.8, 173.7, 174.0, 174.1, 174.0,
		175.1, 175.8, 176.2, 176.9, 177.7, 178.0,               // Year 2001
		177.5, 177.5, 178.3, 177.7, 177.4, 176.7,
		177.1, 177.8, 178.8, 179.8, 179.8, 179.9,               // Year 2002
		180.1, 180.7, 181.0, 181.3, 181.3, 180.9,
		181.7, 183.1, 184.2, 183.8, 183.5, 183.7,               // Year 2003
		183.9, 184.6, 185.2, 185.0, 184.5, 184.3,
		185.2, 186.2, 187.4, 188.0, 189.1, 189.7,               // Year 2004
		189.4, 189.5, 189.9, 190.9, 191.0, 190.3,
		190.7, 191.8, 193.3, 194.6, 194.4, 194.5,               // Year 2005
		195.4, 196.4, 198.8, 199.2, 197.6, 196.8,
		198.3, 198.7, 199.8, 201.5, 202.5, 202.9,               // Year 2006
		203.5, 203.9, 202.9, 201.8, 201.5, 201.8,
		202.416, 203.499, 205.352, 206.686, 207.949, 208.352,   // Year 2007
		208.299, 207.917, 208.490, 208.936, 210.177, 210.036,
		211.080, 211.693, 213.528, 214.823, 216.632, 218.815,   // Year 2008
		219.964, 219.086, 218.783, 216.573, 212.425, 210.228,
		211.143, 212.193, 212.709, 213.240, 213.856, 215.693,   // Year 2009
		215.351, 215.834, 215.969, 216.177, 216.330, 215.949,
		216.687, 216.741, 217.631, 218.009, 218.178, 217.965,   // Year 2010
		218.011, 218.312, 218.439, 218.711, 218.803, 219.179,
		220.223, 221.309, 223.467, 224.906, 225.964, 225.722,   // Year 2011
		225.922, 226.545, 226.889, 226.421, 226.230, 225.672,
		226.665, 227.663, 229.392, 230.085, 229.815, 229.478,   // Year 2012
		229.104, 230.379, 231.407, 231.317, 230.221, 229.601,
		230.280, 232.166, 232.773, 232.531, 232.945, 233.504,   // Year 2013
		233.596, 233.877, 234.149, 233.546, 233.069, 233.049
	};

	zsl_real_t cpiperiod_data[204] = {
		1997.0 + (0. / 12.), 1997.0 + (1. / 12.), 1997.0 + (2. / 12.),
		1997.0 + (3. / 12.), 1997.0 + (4. / 12.), 1997.0 + (5. / 12.),
		1997.0 + (6. / 12.), 1997.0 + (7. / 12.), 1997.0 + (8. / 12.),
		1997.0 + (9. / 12.), 1997.0 + (10. / 12.), 1997.0 + (11. / 12.),
		1998.0 + (0. / 12.), 1998.0 + (1. / 12.), 1998.0 + (2. / 12.),
		1998.0 + (3. / 12.), 1998.0 + (4. / 12.), 1998.0 + (5. / 12.),
		1998.0 + (6. / 12.), 1998.0 + (7. / 12.), 1998.0 + (8. / 12.),
		1998.0 + (9. / 12.), 1998.0 + (10. / 12.), 1998.0 + (11. / 12.),
		1999.0 + (0. / 12.), 1999.0 + (1. / 12.), 1999.0 + (2. / 12.),
		1999.0 + (3. / 12.), 1999.0 + (4. / 12.), 1999.0 + (5. / 12.),
		1999.0 + (6. / 12.), 1999.0 + (7. / 12.), 1999.0 + (8. / 12.),
		1999.0 + (9. / 12.), 1999.0 + (10. / 12.), 1999.0 + (11. / 12.),
		2000.0 + (0. / 12.), 2000.0 + (1. / 12.), 2000.0 + (2. / 12.),
		2000.0 + (3. / 12.), 2000.0 + (4. / 12.), 2000.0 + (5. / 12.),
		2000.0 + (6. / 12.), 2000.0 + (7. / 12.), 2000.0 + (8. / 12.),
		2000.0 + (9. / 12.), 2000.0 + (10. / 12.), 2000.0 + (11. / 12.),
		2001.0 + (0. / 12.), 2001.0 + (1. / 12.), 2001.0 + (2. / 12.),
		2001.0 + (3. / 12.), 2001.0 + (4. / 12.), 2001.0 + (5. / 12.),
		2001.0 + (6. / 12.), 2001.0 + (7. / 12.), 2001.0 + (8. / 12.),
		2001.0 + (9. / 12.), 2001.0 + (10. / 12.), 2001.0 + (11. / 12.),
		2002.0 + (0. / 12.), 2002.0 + (1. / 12.), 2002.0 + (2. / 12.),
		2002.0 + (3. / 12.), 2002.0 + (4. / 12.), 2002.0 + (5. / 12.),
		2002.0 + (6. / 12.), 2002.0 + (7. / 12.), 2002.0 + (8. / 12.),
		2002.0 + (9. / 12.), 2002.0 + (10. / 12.), 2002.0 + (11. / 12.),
		2003.0 + (0. / 12.), 2003.0 + (1. / 12.), 2003.0 + (2. / 12.),
		2003.0 + (3. / 12.), 2003.0 + (4. / 12.), 2003.0 + (5. / 12.),
		2003.0 + (6. / 12.), 2003.0 + (7. / 12.), 2003.0 + (8. / 12.),
		2003.0 + (9. / 12.), 2003.0 + (10. / 12.), 2003.0 + (11. / 12.),
		2004.0 + (0. / 12.), 2004.0 + (1. / 12.), 2004.0 + (2. / 12.),
		2004.0 + (3. / 12.), 2004.0 + (4. / 12.), 2004.0 + (5. / 12.),
		2004.0 + (6. / 12.), 2004.0 + (7. / 12.), 2004.0 + (8. / 12.),
		2004.0 + (9. / 12.), 2004.0 + (10. / 12.), 2004.0 + (11. / 12.),
		2005.0 + (0. / 12.), 2005.0 + (1. / 12.), 2005.0 + (2. / 12.),
		2005.0 + (3. / 12.), 2005.0 + (4. / 12.), 2005.0 + (5. / 12.),
		2005.0 + (6. / 12.), 2005.0 + (7. / 12.), 2005.0 + (8. / 12.),
		2005.0 + (9. / 12.), 2005.0 + (10. / 12.), 2005.0 + (11. / 12.),
		2006.0 + (0. / 12.), 2006.0 + (1. / 12.), 2006.0 + (2. / 12.),
		2006.0 + (3. / 12.), 2006.0 + (4. / 12.), 2006.0 + (5. / 12.),
		2006.0 + (6. / 12.), 2006.0 + (7. / 12.), 2006.0 + (8. / 12.),
		2006.0 + (9. / 12.), 2006.0 + (10. / 12.), 2006.0 + (11. / 12.),
		2007.0 + (0. / 12.), 2007.0 + (1. / 12.), 2007.0 + (2. / 12.),
		2007.0 + (3. / 12.), 2007.0 + (4. / 12.), 2007.0 + (5. / 12.),
		2007.0 + (6. / 12.), 2007.0 + (7. / 12.), 2007.0 + (8. / 12.),
		2007.0 + (9. / 12.), 2007.0 + (10. / 12.), 2007.0 + (11. / 12.),
		2008.0 + (0. / 12.), 2008.0 + (1. / 12.), 2008.0 + (2. / 12.),
		2008.0 + (3. / 12.), 2008.0 + (4. / 12.), 2008.0 + (5. / 12.),
		2008.0 + (6. / 12.), 2008.0 + (7. / 12.), 2008.0 + (8. / 12.),
		2008.0 + (9. / 12.), 2008.0 + (10. / 12.), 2008.0 + (11. / 12.),
		2009.0 + (0. / 12.), 2009.0 + (1. / 12.), 2009.0 + (2. / 12.),
		2009.0 + (3. / 12.), 2009.0 + (4. / 12.), 2009.0 + (5. / 12.),
		2009.0 + (6. / 12.), 2009.0 + (7. / 12.), 2009.0 + (8. / 12.),
		2009.0 + (9. / 12.), 2009.0 + (10. / 12.), 2009.0 + (11. / 12.),
		2010.0 + (0. / 12.), 2010.0 + (1. / 12.), 2010.0 + (2. / 12.),
		2010.0 + (3. / 12.), 2010.0 + (4. / 12.), 2010.0 + (5. / 12.),
		2010.0 + (6. / 12.), 2010.0 + (7. / 12.), 2010.0 + (8. / 12.),
		2010.0 + (9. / 12.), 2010.0 + (10. / 12.), 2010.0 + (11. / 12.),
		2011.0 + (0. / 12.), 2011.0 + (1. / 12.), 2011.0 + (2. / 12.),
		2011.0 + (3. / 12.), 2011.0 + (4. / 12.), 2011.0 + (5. / 12.),
		2011.0 + (6. / 12.), 2011.0 + (7. / 12.), 2011.0 + (8. / 12.),
		2011.0 + (9. / 12.), 2011.0 + (10. / 12.), 2011.0 + (11. / 12.),
		2012.0 + (0. / 12.), 2012.0 + (1. / 12.), 2012.0 + (2. / 12.),
		2012.0 + (3. / 12.), 2012.0 + (4. / 12.), 2012.0 + (5. / 12.),
		2012.0 + (6. / 12.), 2012.0 + (7. / 12.), 2012.0 + (8. / 12.),
		2012.0 + (9. / 12.), 2012.0 + (10. / 12.), 2012.0 + (11. / 12.),
		2013.0 + (0. / 12.), 2013.0 + (1. / 12.), 2013.0 + (2. / 12.),
		2013.0 + (3. / 12.), 2013.0 + (4. / 12.), 2013.0 + (5. / 12.),
		2013.0 + (6. / 12.), 2013.0 + (7. / 12.), 2013.0 + (8. / 12.),
		2013.0 + (9. / 12.), 2013.0 + (10. / 12.), 2013.0 + (11. / 12.)
	};

	zsl_vec_from_arr(&ngpvec, ngp_data);
	zsl_vec_from_arr(&cpivalvec, cpival_data);
	zsl_vec_from_arr(&cpiperiodvec, cpiperiod_data);

	printf("Monthly price of natural gas from the year 1997 to 2020:\n");

	zsl_sta_mean(&ngpvec, &ngp_mean);
	printf("  mean:                       %f\n", ngp_mean);

	zsl_sta_sta_err(&ngpvec, &ngp_sterr);
	printf("  mean standard error:        %f\n", ngp_sterr);

	zsl_sta_trim_mean(&ngpvec, 4, &ngp_trim_mean);
	printf("  4%% trimmed mean:            %f\n", ngp_trim_mean);

	zsl_sta_median(&ngpvec, &ngp_median);
	printf("  median:                     %f\n", ngp_median);

	zsl_sta_mode(&ngpvec, &ngp_mode);
	printf("  mode(s):                    ");
	zsl_vec_print(&ngp_mode);

	zsl_sta_quart(&ngpvec, &ngp_q1, &ngp_q2, &ngp_q3);
	printf("  first quartile:             %f\n", ngp_q1);
	printf("  second quartile:            %f\n", ngp_q3);

	zsl_sta_quart_range(&ngpvec, &ngp_qrng);
	printf("  interquartile range:        %f\n", ngp_qrng);

	zsl_sta_data_range(&ngpvec, &ngp_drng);
	printf("  range:                      %f\n", ngp_drng);

	zsl_sta_mean_abs_dev(&ngpvec, &ngp_mean_absdev);
	printf("  mean absolut deviation:     %f\n", ngp_mean_absdev);

	zsl_sta_median_abs_dev(&ngpvec, &ngp_median_absdev);
	printf("  median absolut deviation:   %f\n", ngp_median_absdev);

	zsl_sta_var(&ngpvec, &ngp_var);
	printf("  variance:                   %f\n", ngp_var);

	zsl_sta_std_dev(&ngpvec, &ngp_stdev);
	printf("  standard deviation:         %f\n", ngp_stdev);

	printf("\n");
	printf("Monthly consumer price index (CPI) in US from 1997 to 2013:\n");

	zsl_sta_linear_reg(&cpiperiodvec, &cpivalvec, &coef);
	printf("  linear regression:\n");
	printf("    slope:                    %f\n", coef.slope);
	printf("    intercept:                %f\n", coef.intercept);
	printf("    regression coeff:         %f\n", coef.correlation);

	printf("\n");
	zsl_vec_get_subset(&ngpvec, 0, 204, &ngpvecsub);
	zsl_mtx_set_col(&ngpmtx, 0, ngpvecsub.data);
	zsl_mtx_set_col(&ngpmtx, 1, cpival_data);

	zsl_sta_covar(&ngpvecsub, &cpivalvec, &cpi_covar);
	printf("  covariance of CPI datasets: %f\n", cpi_covar);

	zsl_sta_covar_mtx(&ngpmtx, &ngp_covar_mtx);
	printf("  covariance matrix:\n\n");
	zsl_mtx_print(&ngp_covar_mtx);

	ZSL_MATRIX_DEF(bodyvec, 22, 4);                 /* Body measurements. */
	ZSL_VECTOR_DEF(massvec, 22);                    /* Weight. */
	ZSL_VECTOR_DEF(mult_reg_coef, 5);

	/* Coefficient of determination. */
	zsl_real_t coef_det;

	/* Source: Larner, M. (1996). 'Mass and its Relationship to Physical
	 * Measurements'. MS305 Data Project, Department of Mathematics,
	 * University of Queensland. */

	zsl_real_t body_data[220] = {
		/*  Forearm   		Waist		 	 Height    		  Thigh
		 *  (cm)			(cm)			 (cm)			  (cm)		*/
		28.5,           85.0,            178.0,           53.0,
		29.5,           90.5,            187.0,           52.0,
		25.0,           80.5,            175.0,           49.0,
		28.5,           91.5,            183.0,           50.0,
		28.5,           92.0,            174.0,           53.0,
		30.5,           101.0,           180.0,           57.5,
		26.5,           76.0,            177.5,           50.0,
		27.0,           84.0,            182.5,           49.0,
		26.5,           74.0,            178.5,           47.0,
		26.5,           76.0,            168.5,           46.0,
		28.5,           80.0,            170.0,           50.0,
		27.5,           86.0,            180.0,           49.0,
		29.5,           82.0,            186.5,           49.0,
		25.0,           82.0,            188.0,           49.5,
		29.5,           95.5,            173.0,           52.5,
		26.5,           81.0,            171.0,           48.0,
		24.0,           76.0,            169.0,           42.0,
		25.5,           84.0,            181.0,           42.0,
		30.0,           88.0,            188.0,           50.5,
		28.0,           82.0,            173.0,           49.0,
		29.0,           96.0,            179.0,           51.0,
		31.0,           99.5,            184.0,           55.0
	};

	/* Weight in kilograms. */
	zsl_real_t mass_data[22] = { 77.0, 85.5, 63.0, 80.5, 79.5, 94.0, 66.0,
				     69.0, 65.0, 58.0, 69.5, 73.0, 74.0, 68.0,
				     80.0, 66.0, 54.5, 64.0, 84.0, 73.0, 89.0,
				     94.0 };

	zsl_mtx_from_arr(&bodyvec, body_data);
	zsl_vec_from_arr(&massvec, mass_data);

	printf("Weight and other physical measurements of men's bodies:\n");

	zsl_sta_mult_linear_reg(&bodyvec, &massvec, &mult_reg_coef, &coef_det);
	/* weight = b0 + b1 * forearm + b2 * waist + b3 * height + b4 * thigh */
	printf("  multiple linear regression:\n");
	printf("    b0:                         %f\n", mult_reg_coef.data[0]);
	printf("    b1:                         %f\n", mult_reg_coef.data[1]);
	printf("    b2:                         %f\n", mult_reg_coef.data[2]);
	printf("    b3:                         %f\n", mult_reg_coef.data[3]);
	printf("    b4:                         %f\n", mult_reg_coef.data[4]);
	printf("    coeff of determination:     %f\n", coef_det);

	ZSL_MATRIX_DEF(invenergyvec, 10, 1);
	ZSL_VECTOR_DEF(crossxvec, 10);
	ZSL_VECTOR_DEF(wls, 2);
	ZSL_VECTOR_DEF(sdvec, 10);
	ZSL_VECTOR_DEF(weights, 10);

	/* Source: Faraway, J. (2002). 'Practical Regression and Anova using R'. */

	/* Inverse of the energy. */
	zsl_real_t invenergy_data[10] = { 0.345, 0.287, 0.251, 0.225, 0.207,
					  0.186, 0.161, 0.132, 0.084, 0.060 };

	/* Cross section. */
	zsl_real_t crossx_data[10] = { 367.0, 311.0, 295.0, 268.0, 253.0,
				       239.0, 220.0, 213.0, 193.0, 192.0 };

	/* Standard deviation of the data. */
	zsl_real_t sd_data[10] = { 17.0, 9.0, 9.0, 7.0, 7.0,
				   6.0, 6.0, 6.0, 5.0, 5.0 };

	zsl_mtx_from_arr(&invenergyvec, invenergy_data);
	zsl_vec_from_arr(&crossxvec, crossx_data);
	zsl_vec_from_arr(&sdvec, sd_data);

	for (size_t i = 0; i < sdvec.sz; i++) {
		weights.data[i] = sdvec.data[i] * sdvec.data[i];
	}

	printf("\n");
	printf("Cross section relatonship to the inverse of the energy:\n");

	zsl_sta_weighted_mult_linear_reg(&invenergyvec, &crossxvec, &weights,
					 &wls, &coef_det);
	printf("  weighted linear regression:\n");
	printf("    intercept:                  %f\n", wls.data[0]);
	printf("    slope:                      %f\n", wls.data[1]);
	printf("    coeff of determination:     %f\n", coef_det);

	printf("\n");

	ZSL_MATRIX_DEF(el, 12, 3);
	ZSL_VECTOR_DEF(el_coef, 9);

	/* Ellipsoid points. */
	zsl_real_t el_data[36] = {
		13.0, 12.0, 41.0,
		2.0,  19.0, 21.0,
		9.0,  23.0, 31.0,
		6.0,  19.0, 27.0,
		11.0, 11.0, 29.0,
		18.0, 20.0, 20.0,
		8.0,  21.0, 32.0,
		8.0,  17.0, 29.0,
		10.0, 22.0, 32.0,
		13.0, 29.0, 38.0,
		12.0, 23.0, 39.0,
		12.0, 12.0, 28.0
	};

	zsl_mtx_from_arr(&el, el_data);

	printf("\n");
	printf("Ellipsoid fitting:\n");

	zsl_sta_quad_fit(&el, &el_coef);

	printf("  least squares method:\n");
	printf("    A:      %f\n", el_coef.data[0]);
	printf("    B:      %f\n", el_coef.data[1]);
	printf("    C:      %f\n", el_coef.data[2]);
	printf("    D:      %f\n", el_coef.data[3]);
	printf("    E:      %f\n", el_coef.data[4]);
	printf("    F:      %f\n", el_coef.data[5]);
	printf("    G:      %f\n", el_coef.data[6]);
	printf("    H:      %f\n", el_coef.data[7]);
	printf("    I:      %f\n", el_coef.data[8]);
}

void main(void)
{
	statistics_demo();
}
