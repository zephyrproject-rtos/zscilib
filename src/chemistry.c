/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <zephyr/kernel.h>
#include <zsl/zsl.h>
#include <zsl/chemistry.h>

const zsl_real_t zsl_chem_atom_weight[119] = {
        0.0,        /* NULL element so that index matches atomic number. */
        1.0079,     /* Hydrogen, H,         1 */
        4.0026,     /* Helium, He           2 */
        6.941,      /* Lithium, Li          3 */
        9.0122,     /* Beryllium, Be        4 */
        10.811,     /* Boron, B             5 */
        12.0107,    /* Carbon, C            6 */
        14.0067,    /* Nitrogen, N          7 */
        15.9994,    /* Oxygen, O            8 */
        18.9984,    /* Fluorine, F          9 */
        20.1797,    /* Neon, Ne             10 */
        22.9897,    /* Sodium, Na           11 */
        24.305,     /* Magnesium, Mg        12 */
        26.9815,    /* Aluminum, Al         13 */
        28.0855,    /* Silicon, Si          14 */
        30.9738,    /* Phosphorus, P        15 */
        32.065,     /* Sulfur, S            16 */
        35.453,     /* Chlorine, Cl         17 */
        39.948,     /* Argon, Ar            18 */
        39.0983,    /* Potassium, K         19 */
        40.078,     /* Calcium, Ca          20 */
        44.9559,    /* Scandium, Sc         21 */
        47.867,     /* Titanium, Ti         22 */
        50.9415,    /* Vanadium, V          23 */
        51.9961,    /* Chromium, Cr         24 */
        54.938,     /* Manganese, Mn        25 */
        55.845,     /* Iron, Fe             26 */
        58.9332,    /* Cobalt, Co           27 */
        58.6934,    /* Nickel, Ni           28 */
        63.546,     /* Copper, Cu           29 */
        65.39,      /* Zinc, Zn             30 */
        69.723,     /* Gallium, Ga          31 */
        72.64,      /* Germanium, Ge        32 */
        74.9216,    /* Arsenic, As          33 */
        78.96,      /* Selenium, Se         34 */
        79.904,     /* Bromine, Br          35 */
        83.8,       /* Krypton, Kr          36 */
        85.4678,    /* Rubidium, Rb         37 */
        87.62,      /* Strontium, Sr        38 */
        88.9059,    /* Yttrium, Y           39 */
        91.224,     /* Zirconium, Zr        40 */
        92.9064,    /* Niobium, Nb          41 */
        95.94,      /* Molybdenum, Mo       42 */
        98.0,       /* Technetium, Tc       43 */
        101.07,     /* Ruthenium, Ru        44 */
        102.9055,   /* Rhodium, Rh          45 */
        106.42,     /* Palladium, Pd        46 */
        107.8682,   /* Silver, Ag           47 */
        112.411,    /* Cadmium, Cd          48 */
        114.818,    /* Indium, In           49 */
        118.71,     /* Tin, Sn              50 */
        121.76,     /* Antimony, Sb         51 */
        127.6,      /* Tellurium, Te        52 */
        126.9045,   /* Iodine, I            53 */
        131.293,    /* Xenon, Xe            54 */
        132.9055,   /* Cesium, Cs           55 */
        137.327,    /* Barium, Ba           56 */
        138.9055,   /* Lanthanum, La        57 */
        140.116,    /* Cerium, Ce           58 */
        140.9077,   /* Praseodymium, Pr     59 */
        144.24,     /* Neodymium, Nd        60 */
        145.0,      /* Promethium, Pm       61 */
        150.36,     /* Samarium, Sm         62 */
        151.964,    /* Europium, Eu         63 */
        157.25,     /* Gadolinium, Gd       64 */
        158.9253,   /* Terbium, Tb          65 */
        162.5,      /* Dysprosium, Dy       66 */
        164.9303,   /* Holmium, Ho          67 */
        167.259,    /* Erbium, Er           68 */
        168.9342,   /* Thulium, Tm          69 */
        173.04,     /* Ytterbium, Yb        70 */
        174.967,    /* Lutetium, Lu         71 */
        178.49,     /* Hafnium, Hf          72 */
        180.9479,   /* Tantalum, Ta         73 */
        183.84,     /* Tungsten, W          74 */
        186.207,    /* Rhenium, Re          75 */
        190.23,     /* Osmium, Os           76 */
        192.217,    /* Iridium, Ir          77 */
        195.078,    /* Platinum, Pt         78 */
        196.9665,   /* Gold, Au             79 */
        200.59,     /* Mercury, Hg          80 */
        204.3833,   /* Thallium, Tl         81 */
        207.2,      /* Lead, Pb             82 */
        208.9804,   /* Bismuth, Bi          83 */
        209.0,      /* Polonium, Po         84 */
        210.0,      /* Astatine, At         85 */
        222.0,      /* Radon, Rn            86 */
        223.0,      /* Francium, Fr         87 */
        226.0,      /* Radium, Ra           88 */
        227.0,      /* Actinium, Ac         89 */
        232.0381,   /* Thorium, Th          90 */
        231.0359,   /* Protactinium, Pa     91 */
        238.0289,   /* Uranium, U           92 */
        237.0,      /* Neptunium, Np        93 */
        244.0,      /* Plutonium, Pu        94 */
        0.0,        /* Americium, Am        95 */
        0.0,        /* Curium, Cm           96 */
        0.0,        /* Berkelium, Bk        97 */
        0.0,        /* Californium, Cf      98 */
        0.0,        /* Einsteinium, Es      99 */
        0.0,        /* Fermium, Fm          100 */
        0.0,        /* Mendelevium, Md      101 */
        0.0,        /* Nobelium, No         102 */
        0.0,        /* Lawrencium, Lr       103 */
        0.0,        /* Rutherfordium, Rf    104 */
        0.0,        /* Dubnium, Db          105 */
        0.0,        /* Seaborgium, Sg       106 */
        0.0,        /* Bohrium, Bh          107 */
        0.0,        /* Hassium, Hs          108 */
        0.0,        /* Meitnerium, Mt       109 */
        0.0,        /* Darmstadtium, Ds     110 */
        0.0,        /* Roentgenium, Rg      111 */
        0.0,        /* Copernicium, Cn      112 */
        0.0,        /* Nihonium, Nh         113 */
        0.0,        /* Flerovium, Fl        114 */
        0.0,        /* Moscovium, Mc        115 */
        0.0,        /* Livermorium, Lv      116 */
        0.0,        /* Tennessine, Ts       117 */
        0.0         /* Oganesson, Og        118 */
};
