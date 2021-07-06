/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @addtogroup ELECTRICAL_COMPS Electrical components.
 *
 * @brief Electrical component functions.
 *
 * @ingroup PHYSICS
 *  @{ */

/**
 * @file
 * @brief API header file for electrical components in zscilib.
 *
 * This file contains the zscilib electrical component APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_ELEC_COMPS_H_
#define ZEPHYR_INCLUDE_ZSL_ELEC_COMPS_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculates capacitance in farads based on charge (q) and voltage (v).
 *
 * @param q     Charge in coulombs.
 * @param v     Voltage in volts.
 * @param c     Pointer to the output capacitance in farads. Will be set to
 *              NAN if v = 0.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_ecmp_capac_cpv(zsl_real_t q, zsl_real_t v, zsl_real_t *c);

/**
 * @brief Calculates capacitance in farads based on the permitivity
 *        of freespace multiplied by area (a) and divided by distance (d).
 *        i.e.: C = E (A / D) where E = 8.85 x 10^-12 farad per meter (F/m).
 *
 * This function can be used to calculate the value of a parallel plate
 * capacitor, consisting of two metallic plates of area a, separated by a
 * vacuum with a gap of d.
 *
 * @param a     Area in square meters.
 * @param d     Distance in meters.
 * @param c     Pointer to the output capacitance in farads. Will be set to
 *              NAN if d = 0.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_ecmp_capac_ad(zsl_real_t a, zsl_real_t d, zsl_real_t *c);

/**
 * @brief Calculates the energy in joules of a capacitor of capacitance 'c' and
 *        voltage 'v'.
 *
 * @param c     Capacitance in farads.
 * @param v     Voltage in volts.
 * @param u     Pointer to the output energy in joules.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_ecmp_ener_capac(zsl_real_t c, zsl_real_t v, zsl_real_t *u);

/**
 * @brief Calculates the energy in joules of an inductor of inductance 'l' and
 *        electric current 'i'.
 *
 * @param l     Inductance in henrys.
 * @param i     Electric current in amperes.
 * @param u     Pointer to the output energy in joules.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_ecmp_ener_induc(zsl_real_t l, zsl_real_t i, zsl_real_t *u);

/**
 * @brief Calculates the output voltage in volts in a two-coil based
 *        transformer given the number of turns on each coil and the input
 *        voltage.
 *
 * @param t1     Number of turns of the input coil.
 * @param v1     Voltage in the input coil, in volts.
 * @param t2     Number of turns of the output coil.
 * @param v2     Pointer to the output voltage in the output coil in volts. Will
 *               be set to NAN if any of the number of turns is negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_ecmp_trans(uint8_t t1, zsl_real_t v1, uint8_t t2, zsl_real_t *v2);

/**
 * @brief Calculates the total voltage of a RCL circuit based on the voltage in
 *        the resistance (vr), in the inductor (vl) and in the capacitor (vc).
 *
 * @param vr    Voltage in the resistance.
 * @param vl    Voltage in the inductor.
 * @param vc    Voltage in the capacitor.
 * @param v     Pointer to the total voltage in volts.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_ecmp_rlc_volt(zsl_real_t vr, zsl_real_t vl, zsl_real_t vc,
			  zsl_real_t *v);

/**
 * @brief Calculates the electric current of a capacitor in a RC circuit
 *        during its charging phase.
 *
 * @param r     Resistance in ohms.
 * @param c     Capacitance in farads.
 * @param t     Time in seconds.
 * @param i0    Initial current of the capacitor in amperes.
 * @param i     Pointer to the output electric current in amperes. Will be set
 *              to NAN if r * c is less or equal than zero or if the time or the
 *              initial current are negavive.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_ecmp_rc_charg_i(zsl_real_t r, zsl_real_t c, zsl_real_t t,
			    zsl_real_t i0, zsl_real_t *i);

/**
 * @brief Calculates the electric charge in coulombs of a capacitor in a RC
 *        circuit during its charging phase.
 *
 * @param r     Resistance in ohms.
 * @param c     Capacitance in farads.
 * @param t     Time in seconds.
 * @param q0    Initial charge of the capacitor in coulombs.
 * @param q     Pointer to the output electric charge in coulombs. Will be set
 *              to NAN if r * c is less or equal than zero or if the time or the
 *              initial charge are negavive.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_ecmp_rc_charg_q(zsl_real_t r, zsl_real_t c, zsl_real_t t,
			    zsl_real_t q0, zsl_real_t *q);

/**
 * @brief Calculates the electric current of a capacitor in a RC circuit
 *        during its discharging phase.
 *
 * @param r     Resistance in ohms.
 * @param c     Capacitance in farads.
 * @param t     Time in seconds.
 * @param i0    Initial current of the capacitor in amperes.
 * @param i     Pointer to the output electric current in amperes. Will be set
 *              to NAN if r * c is less or equal than zero or if the time or the
 *              initial current are negavive.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_ecmp_rc_discharg_i(zsl_real_t r, zsl_real_t c, zsl_real_t t,
			       zsl_real_t i0, zsl_real_t *i);

/**
 * @brief Calculates the electric charge in coulombs of a capacitor in a RC
 *        circuit during its discharging phase.
 *
 * @param r     Resistance in ohms.
 * @param c     Capacitance in farads.
 * @param t     Time in seconds.
 * @param q0    Initial charge of the capacitor in coulombs.
 * @param q     Pointer to the output electric charge in coulombs. Will be set
 *              to NAN if r * c is less or equal than zero or if the time or the
 *              initial charge are negavive.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_ecmp_rc_discharg_q(zsl_real_t r, zsl_real_t c, zsl_real_t t,
			       zsl_real_t q0, zsl_real_t *q);

/**
 * @brief Calculates the electric current in amperes of a RL circuit in time.
 *
 * @param r     Resistance in ohms.
 * @param l     Inductance in henrys.
 * @param t     Time in seconds.
 * @param i0    Initial current of the capacitor in amperes.
 * @param i     Pointer to the output electric current in amperes. Will be set
 *              to NAN the time, the inductance, the resistance or the initial
 *              current are negavive.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_ecmp_rl_current(zsl_real_t r, zsl_real_t l, zsl_real_t t,
			    zsl_real_t i0, zsl_real_t *i);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_ELEC_COMPS_H_ */

/** @} */ /* End of electrical components group */
