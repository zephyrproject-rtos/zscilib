/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @addtogroup ELECTRICITY Electricity
 *
 * @brief Electricity functions.
 *
 * \ingroup PHYSICS
 *  @{ */

/**
 * @file
 * @brief API header file for electricity in zscilib.
 *
 * This file contains the zscilib electricity APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_ELECTRICITY_H_
#define ZEPHYR_INCLUDE_ZSL_ELECTRICITY_H_

#include <zsl/zsl.h>
#include <zsl/vectors.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculates the electric current based on the charge (q) that crosses
 * 		  a transversal area during a period of time (t).
 *
 * @param q    Charge in coulombs.
 * @param t    Time in seconds.
 * @param i    Pointer to the calculated electric current in amperes. Will be
 * 			   set to NAN if the time is negative ir zero.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_elcty_current(zsl_real_t q, zsl_real_t t, zsl_real_t *i);

/**
 * @brief Calculates the equivalent resistance of a set of resistors in series.
 *
 * @param v    Vector whose coefficients are the resistances in Ohms of the
 * 			   resistors in series.
 * @param r    Pointer to the equivalent resistance in Ohms. Will be set to NAN
 *             if any of the input resistances is negative.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_elcty_res_series(struct zsl_vec *v, zsl_real_t *r);

/**
 * @brief Calculates the equivalent resistance of a set of resistors
 * 		  in parallel.
 *
 * @param v    Vector whose coefficients are the resistances in Ohms of the
 * 			   resistors in parallel.
 * @param r    Pointer to the equivalent resistance in Ohms. Will be set to NAN
 *             if any of the input resistances is negative or zero.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_elcty_res_parallel(struct zsl_vec *v, zsl_real_t *r);

/**
 * @brief Calculates the equivalent capacitance of a set of capacitors
 * 		  in series.
 *
 * @param v    Vector whose coefficients are the capacitances in farads of the
 * 			   capacitors in series.
 * @param c    Pointer to the equivalent capacitance in farads. Will be set to
 *             NAN if any of the input capacitances is negative or zero.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_elcty_cap_series(struct zsl_vec *v, zsl_real_t *c);

/**
 * @brief Calculates the equivalent capacitance of a set of capacitors
 * 		  in parallels.
 *
 * @param v    Vector whose coefficients are the capacitances in farads of the
 * 			   capacitors in parallel.
 * @param c    Pointer to the equivalent capacitance in farads. Will be set to
 *             NAN if any of the input capacitances is negative.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_elcty_cap_parallel(struct zsl_vec *v, zsl_real_t *c);

/**
 * @brief Calculates the resistivity of a uniform piece of material of area
 * 		  section 'a', length 'l' and resistance 'r'.
 *
 * @param r     Resistance of the material in ohms.
 * @param a     Area section in square meters.
 * @param l     Length in meters.
 * @param rty   Pointer to the resistivity in ohms times meters. Will be set to
 *              NAN if the area section, length or resistance are negative, or
 *              if l = 0.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_elcty_resistivity(zsl_real_t r, zsl_real_t a, zsl_real_t l,
			      zsl_real_t *rty);

/**
 * @brief Calculates the voltage between two points in an electrical circiut
 * 	      based on the electric current (i) and resistance (r) using Ohm's law.
 *
 * @param i    Electric current in amperes.
 * @param r    Resistance in ohms.
 * @param v    Pointer to the calculated voltage in volts. Will be set to NAN
 *             if the resistance is negative.
 *
 * @return 0 on success, and non-zero error code on failure.
 */
int zsl_phy_elcty_ohm_law(zsl_real_t i, zsl_real_t r, zsl_real_t *v);

/**
 * @brief Calculates the electric power based on the voltage (v) and electric
 * 		  current (i).
 *
 * @param v    Voltage in volts.
 * @param i    Electric current in amperes.
 * @param p    Pointer to the output electric power in watts.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_phy_elcty_power_vi(zsl_real_t v, zsl_real_t i, zsl_real_t *p);

/**
 * @brief Calculates the electric power based on the electric current (i) and
 * 		  the resistance (r).
 *
 * @param i    Electric current in amperes.
 * @param r    Resistance in ohms.
 * @param p    Pointer to the output electric power in watts. Will be set to
 * 			   NAN if the resistance is negative.
 *
 * @return  0 on success, and non-zero error code on failure.
 */
int zsl_phy_elcty_power_ir(zsl_real_t i, zsl_real_t r, zsl_real_t *p);

/**
 * @brief Calculates the electric power based on the voltage (v) and the
 * 		  resistance (r).
 *
 * @param v    Voltage in volts.
 * @param r    Resistance in ohms.
 * @param p    Pointer to the output electric power in watts. Will be set to
 * 			   NAN if the resistance is negative or zero.
 *
 * @return  0 on success, and non-zero error code on failure.
 */
int zsl_phy_elcty_power_vr(zsl_real_t v, zsl_real_t r, zsl_real_t *p);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_ELECTRICITY_H_ */

/** @} */ /* End of electricity group */
