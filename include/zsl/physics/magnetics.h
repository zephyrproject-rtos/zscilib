/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @addtogroup MAGNETICS Magnetics
 *
 * @brief Magnetics functions.
 *
 * @ingroup PHYSICS
 *  @{ */

/**
 * @file
 * @brief API header file for magnetics in zscilib.
 *
 * This file contains the zscilib magnetics APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_MAGNETICS_H_
#define ZEPHYR_INCLUDE_ZSL_MAGNETICS_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculates the module of the magnetic force in newtons that
 *        experiences a charged body moving in a magnetic field.
 *
 * @param q       Charge of the body in coulombs.
 * @param v       Module of the velocity vector in meters per second.
 * @param b       Module of the magnetic field vector in teslas.
 * @param theta   Angle between the velocity and magnetic field vectors in
 *                radians.
 * @param f       Pointer to the output magnetic force in newtons. Will be set
 *                to NAN if either the velocity or the magnetic field are
 *                negative values.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_magn_force(zsl_real_t q, zsl_real_t v, zsl_real_t b,
		       zsl_real_t theta, zsl_real_t *f);

/**
 * @brief Calculates the module of the magnetic force in newtons that
 *        experiences an electric wire.
 *
 * @param i       Intensity running through the wire in amperes.
 * @param l       Lenght of the wire in meters.
 * @param b       Module of the magnetic field vector in teslas.
 * @param theta   Angle between the vector parallel to the wire and the
 *                magnetic field vector in radians.
 * @param f       Pointer to the output magnetic force in newtons. Will be set
 *                to NAN if either the length or the magnetic field are
 *                negative values.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_magn_force_wire(zsl_real_t i, zsl_real_t l, zsl_real_t b,
			    zsl_real_t theta, zsl_real_t *f);

/**
 * @brief Calculates the torque in newtons and meters that experiences a wire
 *        loop with intensity 'i' and area 'a' under a magnetic field 'b'.
 *
 * @param i       Intensity running through the wire loop in amperes.
 * @param a       Surface of the loop in meters squared.
 * @param b       Module of the magnetic field vector in teslas.
 * @param theta   Angle between the vector normal to the surface of the loop
 *                and the magnetic field vector in radians.
 * @param tor     Pointer to the output torque in newtons and meters. Will be
 *                set to NAN if either the area or the magnetic field are
 *                negative values.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_magn_torque_loop(zsl_real_t i, zsl_real_t a, zsl_real_t b,
			     zsl_real_t theta, zsl_real_t *tor);

/**
 * @brief Calculates the potential energy in joules of a magnetic dipole with
 *        magnetic momentum 'mom' under a magnetic field 'b' that has been
 *        rotated an angle 'theta' from its resting position.
 *
 * @param mom     Module of the magnetic momentum of the dipole in amperes and
 *                meters squared.
 * @param b       Module of the magnetic field vector in teslas.
 * @param theta   Angle between the magnetic momentum and the magnetic field
 *                vectors in radians.
 * @param u       Pointer to the output potential energy in joules. Will be
 *                set to NAN if either the magnetic momentum or the magnetic
 *                field are negative values.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_magn_pot_dipole(zsl_real_t mom, zsl_real_t b, zsl_real_t theta,
			    zsl_real_t *u);

/**
 * @brief Calculates the radius of the circular trajectory a charged object
 *        descrives when entering a magnetic field.
 *
 * @param q     Charge of the object in coulombs.
 * @param v     Module of the velocity vector in meters per second.
 * @param b     Module of the magnetic field vector in teslas.
 * @param m     Mas of the object in kilograms.
 * @param r     Pointer to the output radius in meters. Will be set to NAN if
 *              either the mass or the radius itself are negative values, or if
 *              the charge or the magnetic field are zero.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_magn_orb_radius(zsl_real_t q, zsl_real_t v, zsl_real_t b,
			    zsl_real_t m, zsl_real_t *r);

/**
 * @brief Calculates the magnetic flux in webers that flows across a surface
 *        based on the area (a), the magnetic field (b) and the angle they form.
 *
 * @param a       Surface in meters squared.
 * @param b       Module of the magnetic field vector in teslas.
 * @param theta   Angle between the vector normal to the surface and the
 *                magnetic field vector in radians.
 * @param fl      Pointer to the output magnetic flux in webers. Will be set to
 *                NAN if either the area or the magnetic field are negative
 *                values.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_magn_flux(zsl_real_t a, zsl_real_t b, zsl_real_t theta,
		      zsl_real_t *fl);

/**
 * @brief Calculates the magnetic momentum of a wire loop in amperes and meters
 *        squared based on its intensity (i) and surface (a).
 *
 * @param i       Intensity running through the wire loop in amperes.
 * @param a       Surface of the loop in meters squared.
 * @param mom     Pointer to the output magnetic momentum in amperes and meters
 *                squared. Will be set to NAN if the area is negative.
 *
 * @return 0 if everything executed properly, error code on failure.
 */
int zsl_phy_magn_mom(zsl_real_t i, zsl_real_t a, zsl_real_t *mom);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_MAGNETICS_H_ */

/** @} */ /* End of magnetics group */
