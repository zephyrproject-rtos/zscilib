/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup CHEMISTRY Chemistry
 *
 * @brief Chemistry functions.
 * @{
 */

/**
 * @file
 * @brief API header file for chemistry in zscilib.
 *
 * This file contains the zscilib chemistry APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_CHEMISTRY_H_
#define ZEPHYR_INCLUDE_ZSL_CHEMISTRY_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CHEM_STRUCTS Structs and Enums
 *
 * Common chemistry-related structs and enums.
 *
 * @ingroup CHEMISTRY
 *  @{ */

/**
 * @brief List of all elements in the periodic table, where the value
 *        associated with the enum entry equals the element's atomic number.
 */
enum zsl_chem_elements {
        ZSL_CHEM_ELEM_HYDROGEN      = 1,
        ZSL_CHEM_ELEM_HELIUM        = 2,
        ZSL_CHEM_ELEM_LITHIUM       = 3,
        ZSL_CHEM_ELEM_BERYLLIUM     = 4,
        ZSL_CHEM_ELEM_BORON         = 5,
        ZSL_CHEM_ELEM_CARBON        = 6,
        ZSL_CHEM_ELEM_NITROGEN      = 7,
        ZSL_CHEM_ELEM_OXYGEN        = 8,
        ZSL_CHEM_ELEM_FLUORINE      = 9,
        ZSL_CHEM_ELEM_NEON          = 10,
        ZSL_CHEM_ELEM_SODIUM        = 11,
        ZSL_CHEM_ELEM_MAGNESIUM     = 12,
        ZSL_CHEM_ELEM_ALUMINIUM     = 13,
        ZSL_CHEM_ELEM_SILICON       = 14,
        ZSL_CHEM_ELEM_PHOSPHORUS    = 15,
        ZSL_CHEM_ELEM_SULFUR        = 16,
        ZSL_CHEM_ELEM_CHLORINE      = 17,
        ZSL_CHEM_ELEM_ARGON         = 18,
        ZSL_CHEM_ELEM_POTASSIUM     = 19,
        ZSL_CHEM_ELEM_CALCIUM       = 20,
        ZSL_CHEM_ELEM_SCANDIUM      = 21,
        ZSL_CHEM_ELEM_TITANIUM      = 22,
        ZSL_CHEM_ELEM_VANADIUM      = 23,
        ZSL_CHEM_ELEM_CHROMIUM      = 24,
        ZSL_CHEM_ELEM_MANGANESE     = 25,
        ZSL_CHEM_ELEM_IRON          = 26,
        ZSL_CHEM_ELEM_COBALT        = 27,
        ZSL_CHEM_ELEM_NICKEL        = 28,
        ZSL_CHEM_ELEM_COPPER        = 29,
        ZSL_CHEM_ELEM_ZINC          = 30,
        ZSL_CHEM_ELEM_GALLIUM       = 31,
        ZSL_CHEM_ELEM_GERMANIUM     = 32,
        ZSL_CHEM_ELEM_ARSENIC       = 33,
        ZSL_CHEM_ELEM_SELENIUM      = 34,
        ZSL_CHEM_ELEM_BROMINE       = 35,
        ZSL_CHEM_ELEM_KRYPTON       = 36,
        ZSL_CHEM_ELEM_RUBIDIUM      = 37,
        ZSL_CHEM_ELEM_STRONTIUM     = 38,
        ZSL_CHEM_ELEM_YTTRIUM       = 39,
        ZSL_CHEM_ELEM_ZIRCONIUM     = 40,
        ZSL_CHEM_ELEM_NIOBIUM       = 41,
        ZSL_CHEM_ELEM_MOLYBDENUM    = 42,
        ZSL_CHEM_ELEM_TECHNETIUM    = 43,
        ZSL_CHEM_ELEM_RUTHENIUM     = 44,
        ZSL_CHEM_ELEM_RHODIUM       = 45,
        ZSL_CHEM_ELEM_PALLADIUM     = 46,
        ZSL_CHEM_ELEM_SILVER        = 47,
        ZSL_CHEM_ELEM_CADMIUM       = 48,
        ZSL_CHEM_ELEM_INDIUM        = 49,
        ZSL_CHEM_ELEM_TIN           = 50,
        ZSL_CHEM_ELEM_ANTIMONY      = 51,
        ZSL_CHEM_ELEM_TELLURIUM     = 52,
        ZSL_CHEM_ELEM_IODINE        = 53,
        ZSL_CHEM_ELEM_XENON         = 54,
        ZSL_CHEM_ELEM_CAESIUM       = 55,
        ZSL_CHEM_ELEM_BARIUM        = 56,
        ZSL_CHEM_ELEM_LANTHANUM     = 57,
        ZSL_CHEM_ELEM_CERIUM        = 58,
        ZSL_CHEM_ELEM_PRASEODYMIUM  = 59,
        ZSL_CHEM_ELEM_NEODYMIUM     = 60,
        ZSL_CHEM_ELEM_PROMETHIUM    = 61,
        ZSL_CHEM_ELEM_SAMARIUM      = 62,
        ZSL_CHEM_ELEM_EUROPIUM      = 63,
        ZSL_CHEM_ELEM_GADOLINIUM    = 64,
        ZSL_CHEM_ELEM_TERBIUM       = 65,
        ZSL_CHEM_ELEM_DYSPROSIUM    = 66,
        ZSL_CHEM_ELEM_HOLMIUM       = 67,
        ZSL_CHEM_ELEM_ERBIUM        = 68,
        ZSL_CHEM_ELEM_THULIUM       = 69,
        ZSL_CHEM_ELEM_YTTERBIUM     = 70,
        ZSL_CHEM_ELEM_LUTETIUM      = 71,
        ZSL_CHEM_ELEM_HAFNIUM       = 72,
        ZSL_CHEM_ELEM_TANTALUM      = 73,
        ZSL_CHEM_ELEM_TUNGSTEN      = 74,
        ZSL_CHEM_ELEM_RHENIUM       = 75,
        ZSL_CHEM_ELEM_OSMIUM        = 76,
        ZSL_CHEM_ELEM_IRIDIUM       = 77,
        ZSL_CHEM_ELEM_PLATINUM      = 78,
        ZSL_CHEM_ELEM_GOLD          = 79,
        ZSL_CHEM_ELEM_MERCURY       = 80,
        ZSL_CHEM_ELEM_THALLIUM      = 81,
        ZSL_CHEM_ELEM_LEAD          = 82,
        ZSL_CHEM_ELEM_BISMUTH       = 83,
        ZSL_CHEM_ELEM_POLONIUM      = 84,
        ZSL_CHEM_ELEM_ASTATINE      = 85,
        ZSL_CHEM_ELEM_RADON         = 86,
        ZSL_CHEM_ELEM_FRANCIUM      = 87,
        ZSL_CHEM_ELEM_RADIUM        = 88,
        ZSL_CHEM_ELEM_ACTINIUM      = 89,
        ZSL_CHEM_ELEM_THORIUM       = 90,
        ZSL_CHEM_ELEM_PROTACTINIUM  = 91,
        ZSL_CHEM_ELEM_URANIUM       = 92,
        ZSL_CHEM_ELEM_NEPTUNIUM     = 93,
        ZSL_CHEM_ELEM_PLUTONIUM     = 94,
        ZSL_CHEM_ELEM_AMERICIUM     = 95,
        ZSL_CHEM_ELEM_CURIUM        = 96,
        ZSL_CHEM_ELEM_BERKELIUM     = 97,
        ZSL_CHEM_ELEM_CALIFORNIUM   = 98,
        ZSL_CHEM_ELEM_EINSTEINIUM   = 99,
        ZSL_CHEM_ELEM_FERMIUM       = 100,
        ZSL_CHEM_ELEM_MENDELEVIUM   = 101,
        ZSL_CHEM_ELEM_NOBELIUM      = 102,
        ZSL_CHEM_ELEM_LAWRENCIUM    = 103,
        ZSL_CHEM_ELEM_RUTHERFORDIUM = 104,
        ZSL_CHEM_ELEM_DUBNIUM       = 105,
        ZSL_CHEM_ELEM_SEABORGIUM    = 106,
        ZSL_CHEM_ELEM_BOHRIUM       = 107,
        ZSL_CHEM_ELEM_HASSIUM       = 108,
        ZSL_CHEM_ELEM_MEITNERIUM    = 109,
        ZSL_CHEM_ELEM_DARMSTADTIUM  = 110,
        ZSL_CHEM_ELEM_ROENTGENIUM   = 111,
        ZSL_CHEM_ELEM_COPERNICIUM   = 112,
        ZSL_CHEM_ELEM_NIHONIUM      = 113,
        ZSL_CHEM_ELEM_FLEROVIUM     = 114,
        ZSL_CHEM_ELEM_MOSCOVIUM     = 115,
        ZSL_CHEM_ELEM_LIVERMORIUM   = 116,
        ZSL_CHEM_ELEM_TENNESSINE    = 117,
        ZSL_CHEM_ELEM_OGANESSON     = 118
};

/**
 * @brief An array of standard atomic weights for all elements.
 *
 * This lookup table uses IUPAC-defined values rounded to four decimal places.
 * When no standard atomic weight is available for a chemical element, a weight
 * of 0.0 is indicated.
 *
 * For the thirteen chemical elements with a range of values (hydrogen,
 * nitrogen, etc.), the CIAAW single-number 'conventional atomic weight' is
 * given.
 *
 * Source: https://physics.nist.gov/cgi-bin/Compositions/stand_alone.pl
 */
extern const zsl_real_t zsl_chem_atom_weight[119];

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_CHEMLISTRY_H_ */

/** @} */ /* End of chemistry group */
