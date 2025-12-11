/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <string.h>
#include <zsl/colorimetry.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

/**
 * 7x7 matrix used for conversion from (x,y) or (u',v') to CCT, Duv.
 *
 * Source: "Calculation of CCT and Duv and Practical Conversion Formulae",
 * Yoshi Ohno, Presentation CORM 2011.
 */
static const zsl_real_t zsl_clr_conv_xyy_cct_ohno_2011_data[7][7] = {
	/** kiO..ki6[0] */
	{ -1.77348E-01,
	  1.115559E+00,
	  -1.5008606E+00,
	  9.750013E-01,
	  -3.307009E-01,
	  5.6061400E-02,
	  -3.7146000E-03 },

	/** ki0..ki6[1] */
	{  5.308409E-04,
	   2.1595434E-03,
	   -4.3534788E-03,
	   3.6196568E-03,
	   -1.589747E-03,
	   3.5700160E-04,
	   -3.2325500E-05 },

	/** ki0..ki6[2] */
	{ -8.58308927E-01,
	  1.964980251E+00,
	  -1.873907584E+00,
	  9.53570888E-01,
	  -2.73172022E-01,
	  4.17781315E-02,
	  -2.6653835E-03 },

	/** ki0..ki6[3] */
	{ -2.3275027E+02,
	  1.49284136E+03,
	  -2.7966888E+03,
	  2.51170136E+03,
	  -1.1785121E+03,
	  2.7183365E+02,
	  -2.3524950E+01 },

	/** ki0..ki6[4] */
	{ -5.926850606E+08,
	  1.34488160614E+09,
	  -1.27141290956E+09,
	  6.40976356945E+08,
	  -1.81749963507E+08,
	  2.7482732935E+07,
	  -1.731364909E+06 },

	/** ki0..ki6[5] */
	{ -2.3758158E+06,
	  3.89561742E+06,
	  -2.65299138E+06,
	  9.60532935E+05,
	  -1.9500061E+05,
	  2.10468274E+04,
	  -9.4353083E+02 },

	/** ki0..ki6[6] */
	{  2.8151771E+06,
	   -4.11436958E+06,
	   2.48526954E+06,
	   -7.93406005E+05,
	   1.4101538E+05,
	   -1.321007E+04,
	   5.0857956E+02 }
};

/**
 * 7x1 matrix used for conversion from (x,y) or (u',v') to CCT, Duv.
 *
 * Source: Yoshi Ohno (2014)
 *         Practical Use and Calculation of CCT and Duv
 *         LEUKOS, 10:1, 47-55, DOI: 10.1080/15502724.2014.839020
 */
static const zsl_real_t zsl_clr_conv_xyy_cct_ohno_2014_data[7] = {
	/** kO..k6 */
	-0.471106,
	1.925865,
	-2.4243787,
	1.5317403,
	-0.5179722,
	0.0893944,
	-0.00616793,
};

/**
 * Color temperature to (u,v) chromaticity lookup table for OHNO2014.
 *
 * 1000.00 K to 20000.00 K CT to (u,v) in 1.000% steps.
 *
 * NOTE: This table could be removed by calculating these values on
 * demand, though at the expense of extra processing power.
 */
static const zsl_real_t zsl_clr_conv_ct_uv_ohno_2014_data[304][3] = {
	{ 990.000000, 0.45042258, 0.35439942 },
	{ 1000.000000, 0.44806562, 0.35460468 },
	{ 1010.000000, 0.44573364, 0.35480622 },
	{ 1020.100000, 0.44340355, 0.35500602 },
	{ 1030.301000, 0.44107575, 0.35520396 },
	{ 1040.604010, 0.43875059, 0.35539995 },
	{ 1051.010050, 0.43642845, 0.35559388 },
	{ 1061.520151, 0.43410968, 0.35578566 },
	{ 1072.135352, 0.43179464, 0.35597519 },
	{ 1082.856706, 0.42948368, 0.35616235 },
	{ 1093.685273, 0.42717713, 0.35634706 },
	{ 1104.622125, 0.42487534, 0.35652920 },
	{ 1115.668347, 0.42257863, 0.35670867 },
	{ 1126.825030, 0.42028732, 0.35688537 },
	{ 1138.093280, 0.41800174, 0.35705919 },
	{ 1149.474213, 0.41572219, 0.35723003 },
	{ 1160.968955, 0.41344897, 0.35739778 },
	{ 1172.578645, 0.41118239, 0.35756233 },
	{ 1184.304431, 0.40892273, 0.35772358 },
	{ 1196.147476, 0.40667028, 0.35788143 },
	{ 1208.108950, 0.40442532, 0.35803576 },
	{ 1220.190040, 0.40218812, 0.35818647 },
	{ 1232.391940, 0.39995895, 0.35833346 },
	{ 1244.715860, 0.39773806, 0.35847662 },
	{ 1257.163018, 0.39552571, 0.35861583 },
	{ 1269.734649, 0.39332214, 0.35875101 },
	{ 1282.431995, 0.39112759, 0.35888203 },
	{ 1295.256315, 0.38894231, 0.35900881 },
	{ 1308.208878, 0.38676651, 0.35913122 },
	{ 1321.290967, 0.38460041, 0.35924918 },
	{ 1334.503877, 0.38244424, 0.35936257 },
	{ 1347.848915, 0.38029821, 0.35947130 },
	{ 1361.327404, 0.37816251, 0.35957526 },
	{ 1374.940679, 0.37603734, 0.35967435 },
	{ 1388.690085, 0.37392290, 0.35976848 },
	{ 1402.576986, 0.37181937, 0.35985754 },
	{ 1416.602756, 0.36972693, 0.35994144 },
	{ 1430.768784, 0.36764576, 0.36002008 },
	{ 1445.076471, 0.36557603, 0.36009337 },
	{ 1459.527236, 0.36351789, 0.36016121 },
	{ 1474.122509, 0.36147152, 0.36022352 },
	{ 1488.863734, 0.35943705, 0.36028020 },
	{ 1503.752371, 0.35741464, 0.36033117 },
	{ 1518.789895, 0.35540443, 0.36037634 },
	{ 1533.977794, 0.35340656, 0.36041562 },
	{ 1549.317572, 0.35142115, 0.36044894 },
	{ 1564.810747, 0.34944835, 0.36047620 },
	{ 1580.458855, 0.34748826, 0.36049735 },
	{ 1596.263443, 0.34554101, 0.36051229 },
	{ 1612.226078, 0.34360671, 0.36052096 },
	{ 1628.348338, 0.34168547, 0.36052328 },
	{ 1644.631822, 0.33977738, 0.36051918 },
	{ 1661.078140, 0.33788256, 0.36050861 },
	{ 1677.688921, 0.33600109, 0.36049149 },
	{ 1694.465811, 0.33413306, 0.36046777 },
	{ 1711.410469, 0.33227857, 0.36043738 },
	{ 1728.524573, 0.33043768, 0.36040027 },
	{ 1745.809819, 0.32861049, 0.36035639 },
	{ 1763.267917, 0.32679705, 0.36030570 },
	{ 1780.900597, 0.32499745, 0.36024813 },
	{ 1798.709603, 0.32321174, 0.36018365 },
	{ 1816.696699, 0.32143998, 0.36011222 },
	{ 1834.863666, 0.31968225, 0.36003381 },
	{ 1853.212302, 0.31793857, 0.35994837 },
	{ 1871.744425, 0.31620902, 0.35985588 },
	{ 1890.461869, 0.31449363, 0.35975630 },
	{ 1909.366488, 0.31279244, 0.35964963 },
	{ 1928.460153, 0.31110549, 0.35953583 },
	{ 1947.744755, 0.30943283, 0.35941488 },
	{ 1967.222202, 0.30777447, 0.35928678 },
	{ 1986.894424, 0.30613045, 0.35915151 },
	{ 2006.763368, 0.30450080, 0.35900906 },
	{ 2026.831002, 0.30288553, 0.35885943 },
	{ 2047.099312, 0.30128466, 0.35870263 },
	{ 2067.570305, 0.29969821, 0.35853865 },
	{ 2088.246008, 0.29812618, 0.35836750 },
	{ 2109.128468, 0.29656860, 0.35818918 },
	{ 2130.219753, 0.29502545, 0.35800373 },
	{ 2151.521951, 0.29349675, 0.35781114 },
	{ 2173.037170, 0.29198250, 0.35761144 },
	{ 2194.767542, 0.29048268, 0.35740465 },
	{ 2216.715217, 0.28899730, 0.35719081 },
	{ 2238.882369, 0.28752635, 0.35696993 },
	{ 2261.271193, 0.28606980, 0.35674206 },
	{ 2283.883905, 0.28462765, 0.35650723 },
	{ 2306.722744, 0.28319988, 0.35626548 },
	{ 2329.789971, 0.28178646, 0.35601685 },
	{ 2353.087871, 0.28038738, 0.35576139 },
	{ 2376.618750, 0.27900261, 0.35549915 },
	{ 2400.384937, 0.27763212, 0.35523019 },
	{ 2424.388787, 0.27627588, 0.35495455 },
	{ 2448.632675, 0.27493385, 0.35467229 },
	{ 2473.119001, 0.27360600, 0.35438348 },
	{ 2497.850191, 0.27229230, 0.35408818 },
	{ 2522.828693, 0.27099269, 0.35378646 },
	{ 2548.056980, 0.26970714, 0.35347839 },
	{ 2573.537550, 0.26843560, 0.35316403 },
	{ 2599.272926, 0.26717802, 0.35284347 },
	{ 2625.265655, 0.26593436, 0.35251678 },
	{ 2651.518311, 0.26470457, 0.35218403 },
	{ 2678.033494, 0.26348858, 0.35184532 },
	{ 2704.813829, 0.26228634, 0.35150073 },
	{ 2731.861968, 0.26109779, 0.35115034 },
	{ 2759.180587, 0.25992289, 0.35079423 },
	{ 2786.772393, 0.25876155, 0.35043251 },
	{ 2814.640117, 0.25761372, 0.35006527 },
	{ 2842.786518, 0.25647934, 0.34969259 },
	{ 2871.214384, 0.25535833, 0.34931457 },
	{ 2899.926527, 0.25425063, 0.34893132 },
	{ 2928.925793, 0.25315617, 0.34854293 },
	{ 2958.215051, 0.25207487, 0.34814950 },
	{ 2987.797201, 0.25100666, 0.34775114 },
	{ 3017.675173, 0.24995147, 0.34734794 },
	{ 3047.851925, 0.24890922, 0.34694002 },
	{ 3078.330444, 0.24787982, 0.34652748 },
	{ 3109.113749, 0.24686321, 0.34611043 },
	{ 3140.204886, 0.24585929, 0.34568897 },
	{ 3171.606935, 0.24486800, 0.34526322 },
	{ 3203.323004, 0.24388923, 0.34483329 },
	{ 3235.356234, 0.24292292, 0.34439929 },
	{ 3267.709797, 0.24196897, 0.34396133 },
	{ 3300.386895, 0.24102729, 0.34351952 },
	{ 3333.390764, 0.24009781, 0.34307398 },
	{ 3366.724671, 0.23918042, 0.34262482 },
	{ 3400.391918, 0.23827504, 0.34217215 },
	{ 3434.395837, 0.23738158, 0.34171610 },
	{ 3468.739795, 0.23649995, 0.34125677 },
	{ 3503.427193, 0.23563005, 0.34079429 },
	{ 3538.461465, 0.23477179, 0.34032876 },
	{ 3573.846080, 0.23392507, 0.33986030 },
	{ 3609.584541, 0.23308981, 0.33938903 },
	{ 3645.680386, 0.23226590, 0.33891506 },
	{ 3682.137190, 0.23145325, 0.33843851 },
	{ 3718.958562, 0.23065176, 0.33795950 },
	{ 3756.148148, 0.22986134, 0.33747814 },
	{ 3793.709629, 0.22908188, 0.33699454 },
	{ 3831.646725, 0.22831329, 0.33650882 },
	{ 3869.963193, 0.22755546, 0.33602110 },
	{ 3908.662824, 0.22680831, 0.33553148 },
	{ 3947.749453, 0.22607172, 0.33504009 },
	{ 3987.226947, 0.22534561, 0.33454702 },
	{ 4027.099217, 0.22462986, 0.33405241 },
	{ 4067.370209, 0.22392438, 0.33355635 },
	{ 4108.043911, 0.22322907, 0.33305896 },
	{ 4149.124350, 0.22254382, 0.33256035 },
	{ 4190.615594, 0.22186854, 0.33206062 },
	{ 4232.521750, 0.22120312, 0.33155989 },
	{ 4274.846967, 0.22054746, 0.33105826 },
	{ 4317.595437, 0.21990146, 0.33055584 },
	{ 4360.771391, 0.21926502, 0.33005274 },
	{ 4404.379105, 0.21863804, 0.32954906 },
	{ 4448.422896, 0.21802040, 0.32904489 },
	{ 4492.907125, 0.21741202, 0.32854036 },
	{ 4537.836196, 0.21681280, 0.32803555 },
	{ 4583.214558, 0.21622261, 0.32753057 },
	{ 4629.046704, 0.21564138, 0.32702551 },
	{ 4675.337171, 0.21506899, 0.32652048 },
	{ 4722.090543, 0.21450535, 0.32601557 },
	{ 4769.311448, 0.21395035, 0.32551087 },
	{ 4817.004562, 0.21340389, 0.32500648 },
	{ 4865.174608, 0.21286588, 0.32450249 },
	{ 4913.826354, 0.21233620, 0.32399900 },
	{ 4962.964618, 0.21181477, 0.32349609 },
	{ 5012.594264, 0.21130147, 0.32299385 },
	{ 5062.720206, 0.21079622, 0.32249236 },
	{ 5113.347409, 0.21029892, 0.32199172 },
	{ 5164.480883, 0.20980945, 0.32149201 },
	{ 5216.125691, 0.20932773, 0.32099330 },
	{ 5268.286948, 0.20885366, 0.32049569 },
	{ 5320.969818, 0.20838714, 0.31999925 },
	{ 5374.179516, 0.20792807, 0.31950405 },
	{ 5427.921311, 0.20747636, 0.31901018 },
	{ 5482.200524, 0.20703191, 0.31851771 },
	{ 5537.022530, 0.20659462, 0.31802671 },
	{ 5592.392755, 0.20616440, 0.31753726 },
	{ 5648.316682, 0.20574115, 0.31704942 },
	{ 5704.799849, 0.20532479, 0.31656326 },
	{ 5761.847848, 0.20491521, 0.31607885 },
	{ 5819.466326, 0.20451232, 0.31559626 },
	{ 5877.660989, 0.20411603, 0.31511555 },
	{ 5936.437599, 0.20372626, 0.31463678 },
	{ 5995.801975, 0.20334290, 0.31416000 },
	{ 6055.759995, 0.20296587, 0.31368529 },
	{ 6116.317595, 0.20259507, 0.31321269 },
	{ 6177.480771, 0.20223043, 0.31274227 },
	{ 6239.255579, 0.20187184, 0.31227407 },
	{ 6301.648135, 0.20151923, 0.31180815 },
	{ 6364.664616, 0.20117250, 0.31134456 },
	{ 6428.311262, 0.20083156, 0.31088335 },
	{ 6492.594375, 0.20049635, 0.31042456 },
	{ 6557.520318, 0.20016675, 0.30996824 },
	{ 6623.095522, 0.19984270, 0.30951444 },
	{ 6689.326477, 0.19952411, 0.30906320 },
	{ 6756.219742, 0.19921090, 0.30861455 },
	{ 6823.781939, 0.19890298, 0.30816854 },
	{ 6892.019758, 0.19860028, 0.30772521 },
	{ 6960.939956, 0.19830271, 0.30728459 },
	{ 7030.549355, 0.19801019, 0.30684672 },
	{ 7100.854849, 0.19772265, 0.30641163 },
	{ 7171.863398, 0.19744000, 0.30597935 },
	{ 7243.582032, 0.19716217, 0.30554991 },
	{ 7316.017852, 0.19688909, 0.30512334 },
	{ 7389.178030, 0.19662067, 0.30469967 },
	{ 7463.069811, 0.19635685, 0.30427892 },
	{ 7537.700509, 0.19609754, 0.30386113 },
	{ 7613.077514, 0.19584268, 0.30344630 },
	{ 7689.208289, 0.19559220, 0.30303447 },
	{ 7766.100372, 0.19534601, 0.30262565 },
	{ 7843.761376, 0.19510406, 0.30221986 },
	{ 7922.198989, 0.19486627, 0.30181712 },
	{ 8001.420979, 0.19463257, 0.30141745 },
	{ 8081.435189, 0.19440289, 0.30102086 },
	{ 8162.249541, 0.19417718, 0.30062736 },
	{ 8243.872036, 0.19395535, 0.30023697 },
	{ 8326.310757, 0.19373735, 0.29984970 },
	{ 8409.573864, 0.19352312, 0.29946555 },
	{ 8493.669603, 0.19331258, 0.29908454 },
	{ 8578.606299, 0.19310567, 0.29870668 },
	{ 8664.392362, 0.19290234, 0.29833196 },
	{ 8751.036286, 0.19270251, 0.29796040 },
	{ 8838.546648, 0.19250614, 0.29759200 },
	{ 8926.932115, 0.19231316, 0.29722677 },
	{ 9016.201436, 0.19212352, 0.29686470 },
	{ 9106.363450, 0.19193714, 0.29650580 },
	{ 9197.427085, 0.19175399, 0.29615006 },
	{ 9289.401356, 0.19157400, 0.29579749 },
	{ 9382.295369, 0.19139711, 0.29544808 },
	{ 9476.118323, 0.19122327, 0.29510184 },
	{ 9570.879506, 0.19105243, 0.29475875 },
	{ 9666.588301, 0.19088454, 0.29441882 },
	{ 9763.254184, 0.19071954, 0.29408203 },
	{ 9860.886726, 0.19055738, 0.29374839 },
	{ 9959.495593, 0.19039801, 0.29341788 },
	{ 10059.090549, 0.19024139, 0.29309050 },
	{ 10159.681455, 0.19008745, 0.29276624 },
	{ 10261.278269, 0.18993616, 0.29244509 },
	{ 10363.891052, 0.18978747, 0.29212704 },
	{ 10467.529963, 0.18964133, 0.29181207 },
	{ 10572.205262, 0.18949769, 0.29150019 },
	{ 10677.927315, 0.18935650, 0.29119137 },
	{ 10784.706588, 0.18921774, 0.29088560 },
	{ 10892.553654, 0.18908134, 0.29058288 },
	{ 11001.479190, 0.18894728, 0.29028318 },
	{ 11111.493982, 0.18881549, 0.28998650 },
	{ 11222.608922, 0.18868596, 0.28969281 },
	{ 11334.835011, 0.18855862, 0.28940211 },
	{ 11448.183361, 0.18843345, 0.28911437 },
	{ 11562.665195, 0.18831040, 0.28882959 },
	{ 11678.291847, 0.18818944, 0.28854774 },
	{ 11795.074766, 0.18807052, 0.28826881 },
	{ 11913.025513, 0.18795362, 0.28799277 },
	{ 12032.155768, 0.18783868, 0.28771962 },
	{ 12152.477326, 0.18772569, 0.28744934 },
	{ 12274.002099, 0.18761459, 0.28718190 },
	{ 12396.742120, 0.18750536, 0.28691728 },
	{ 12520.709541, 0.18739797, 0.28665548 },
	{ 12645.916637, 0.18729237, 0.28639646 },
	{ 12772.375803, 0.18718854, 0.28614022 },
	{ 12900.099561, 0.18708644, 0.28588672 },
	{ 13029.100557, 0.18698604, 0.28563595 },
	{ 13159.391562, 0.18688731, 0.28538789 },
	{ 13290.985478, 0.18679022, 0.28514251 },
	{ 13423.895333, 0.18669474, 0.28489980 },
	{ 13558.134286, 0.18660084, 0.28465974 },
	{ 13693.715629, 0.18650850, 0.28442231 },
	{ 13830.652785, 0.18641767, 0.28418747 },
	{ 13968.959313, 0.18632834, 0.28395522 },
	{ 14108.648906, 0.18624047, 0.28372553 },
	{ 14249.735395, 0.18615405, 0.28349838 },
	{ 14392.232749, 0.18606904, 0.28327375 },
	{ 14536.155077, 0.18598542, 0.28305161 },
	{ 14681.516628, 0.18590316, 0.28283194 },
	{ 14828.331794, 0.18582223, 0.28261473 },
	{ 14976.615112, 0.18574262, 0.28239994 },
	{ 15126.381263, 0.18566430, 0.28218756 },
	{ 15277.645076, 0.18558725, 0.28197757 },
	{ 15430.421526, 0.18551143, 0.28176994 },
	{ 15584.725742, 0.18543684, 0.28156465 },
	{ 15740.572999, 0.18536344, 0.28136168 },
	{ 15897.978729, 0.18529122, 0.28116100 },
	{ 16056.958516, 0.18522016, 0.28096260 },
	{ 16217.528101, 0.18515022, 0.28076646 },
	{ 16379.703382, 0.18508140, 0.28057254 },
	{ 16543.500416, 0.18501368, 0.28038083 },
	{ 16708.935420, 0.18494702, 0.28019131 },
	{ 16876.024775, 0.18488142, 0.28000395 },
	{ 17044.785022, 0.18481685, 0.27981873 },
	{ 17215.232873, 0.18475330, 0.27963564 },
	{ 17387.385201, 0.18469075, 0.27945464 },
	{ 17561.259053, 0.18462918, 0.27927573 },
	{ 17736.871644, 0.18456856, 0.27909886 },
	{ 17914.240360, 0.18450890, 0.27892404 },
	{ 18093.382764, 0.18445015, 0.27875122 },
	{ 18274.316592, 0.18439233, 0.27858040 },
	{ 18457.059757, 0.18433539, 0.27841154 },
	{ 18641.630355, 0.18427934, 0.27824464 },
	{ 18828.046659, 0.18422414, 0.27807967 },
	{ 19016.327125, 0.18416980, 0.27791660 },
	{ 19206.490396, 0.18411629, 0.27775542 },
	{ 19398.555300, 0.18406359, 0.27759611 },
	{ 19592.540853, 0.18401170, 0.27743864 },
	{ 19788.466262, 0.18396060, 0.27728300 },
	{ 19986.350925, 0.18391028, 0.27712917 },
	{ 20186.214434, 0.18386072, 0.27697712 }
};

#define OHNO2014_LOOKUP_RECS (sizeof(zsl_clr_conv_ct_uv_ohno_2014_data) / \
			      (sizeof(zsl_clr_conv_ct_uv_ohno_2014_data[0])))

int
zsl_clr_conv_spd_xyz(const struct zsl_clr_spd *spd, enum zsl_clr_obs obs,
		     struct zsl_clr_xyz *xyz)
{
	int rc;
	int matches;
	unsigned int nm_idx;
	const struct zsl_clr_obs_data *obs_data;

	/* Clear the output values (used for internal manipulation). */
	memset(xyz, 0, sizeof(*xyz));
	matches = 0;

	/*
	 * Realistically we should have at least 3 vals, but technically the
	 * calculations can be made with only one.
	 */
	if (spd->size < 1) {
		rc = -EINVAL;
		goto err;
	}

	/* Get a reference to the standard observer CMF dataset. */
	zsl_clr_obs_get(obs, &obs_data);

	/* Sum contents of the spd. Only accept values from 360 to 830 nm. */
	for (int i = 0; i < spd->size; i++) {
		if ((spd->comps[i].nm > 359) && (spd->comps[i].nm < 831)) {
			/* Round nm to the nearest 5 nm interval. */
			nm_idx = ((spd->comps[i].nm - 360) / 5);
			/* Accumulate tristimulus values. */
			xyz->xyz_x += spd->comps[i].value *
				      obs_data->data[nm_idx].xyz_x;
			xyz->xyz_y += spd->comps[i].value *
				      obs_data->data[nm_idx].xyz_y;
			xyz->xyz_z += spd->comps[i].value *
				      obs_data->data[nm_idx].xyz_z;
			matches++;
		}
	}

	/* Avoid divide by zero error if no matches found. */
	if (!matches) {
		rc = -EINVAL;
		goto err;
	}

	/* Divide the results by the number of valid components. */
	xyz->xyz_x /= matches;
	xyz->xyz_y /= matches;
	xyz->xyz_z /= matches;

	/* Scale output to Y=1.0 */
	xyz->xyz_x /= xyz->xyz_y;
	xyz->xyz_z /= xyz->xyz_y;
	xyz->xyz_y = 1.0;

	/* Set the observer model. */
	xyz->observer = obs;

	return 0;
err:
	xyz->x_invalid = 1;
	xyz->y_invalid = 1;
	xyz->z_invalid = 1;
	return rc;
}

int
zsl_clr_conv_xyy_xyz(struct zsl_clr_xyy *xyy, struct zsl_clr_xyz *xyz)
{
	/*
	 *    X = xY / y
	 *    Y = Y
	 *    Z = (1 - x - y) * Y
	 *        ---------------
	 *               y
	 */
	xyz->xyz_x = xyy->xyy_x * xyy->xyy_Y / xyy->xyy_y;
	xyz->xyz_y = xyy->xyy_Y;
	xyz->xyz_z = (1.0 - xyy->xyy_x - xyy->xyy_y) * xyy->xyy_Y / xyy->xyy_y;
	xyz->observer = xyy->observer;
	xyz->illuminant = xyy->illuminant;

	return 0;
}

int
zsl_clr_conv_xyz_xyy(struct zsl_clr_xyz *xyz, struct zsl_clr_xyy *xyy)
{
	/*
	 *    x = X/(X+Y+Z)
	 *    y = Y/(X+Y+Z)
	 *    Y = Y
	 */
	xyy->xyy_x = xyz->xyz_x / (xyz->xyz_x + xyz->xyz_y + xyz->xyz_z);
	xyy->xyy_y = xyz->xyz_y / (xyz->xyz_x + xyz->xyz_y + xyz->xyz_z);
	xyy->xyy_Y = xyz->xyz_y;
	xyy->observer = xyz->observer;
	xyy->illuminant = xyz->illuminant;

	return 0;
}

int
zsl_clr_conv_xyy_uv60(struct zsl_clr_xyy *xyy, struct zsl_clr_uv60 *uv)
{
	/*
	 *    u = 4x / (-2x + 12y + 3)
	 *    v = 6y / (-2x + 12y + 3)
	 */
	uv->uv60_u = 4 * xyy->xyy_x / ((2 * xyy->xyy_x * -1.0) +
				       (12 * xyy->xyy_y) + 3);
	uv->uv60_v = 6 * xyy->xyy_y / ((2 * xyy->xyy_x * -1.0) +
				       (12 * xyy->xyy_y) + 3);
	uv->observer = xyy->observer;
	uv->illuminant = xyy->illuminant;

	return 0;
}

int
zsl_clr_conv_xyz_uv60(struct zsl_clr_xyz *xyz, struct zsl_clr_uv60 *uv)
{
	struct zsl_clr_xyy xyy;

	/* First convert XYZ to the equivalent CIE 1931 xyY value. */
	zsl_clr_conv_xyz_xyy(xyz, &xyy);

	/* Now convert xyY to an equivalent CIE 1960 uv chromaticity. */
	zsl_clr_conv_xyy_uv60(&xyy, uv);

	return 0;
}

int
zsl_clr_conv_uv60_xyz(struct zsl_clr_uv60 *uv, struct zsl_clr_xyz *xyz)
{
	struct zsl_clr_xyy xyy;

	/* First convert to xyY. */
	zsl_clr_conv_uv60_xyy(uv, &xyy);

	/* Then convert xyY to XYZ. */
	zsl_clr_conv_xyy_xyz(&xyy, xyz);

	return 0;
}

int
zsl_clr_conv_uv60_xyy(struct zsl_clr_uv60 *uv, struct zsl_clr_xyy *xyy)
{
	memset(xyy, 0, sizeof *xyy);

	xyy->xyy_x = (3.0 * uv->uv60_u) / (2.0 * uv->uv60_u -
					   8.0 * uv->uv60_v + 4.0);
	xyy->xyy_y = (2.0 * uv->uv60_v) / (2.0 * uv->uv60_u -
					   8.0 * uv->uv60_v + 4.0);
	xyy->xyy_Y = 1.0;
	xyy->observer = uv->observer;
	xyy->illuminant = uv->illuminant;

	return 0;
}

int
zsl_clr_conv_uv60_uv76(struct zsl_clr_uv60 *uv60, struct zsl_clr_uv76 *uv76)
{
	uv76->uv76_u = uv60->uv60_u;
	uv76->uv76_v = uv60->uv60_v * 1.5;
	uv76->observer = uv60->observer;
	uv76->illuminant = uv60->illuminant;
	uv76->u_invalid = uv60->u_invalid;
	uv76->v_invalid = uv60->v_invalid;

	return 0;
}

int
zsl_clr_conv_uv76_uv60(struct zsl_clr_uv76 *uv76, struct zsl_clr_uv60 *uv60)
{
	uv60->uv60_u = uv76->uv76_u;
	uv60->uv60_v = uv76->uv76_v / 1.5;
	uv60->observer = uv76->observer;
	uv60->illuminant = uv76->illuminant;
	uv60->u_invalid = uv76->u_invalid;
	uv60->v_invalid = uv76->v_invalid;

	return 0;
}

int
zsl_clr_conv_ct_uv60(zsl_real_t ct, enum zsl_clr_obs obs, struct zsl_clr_uv60 *uv)
{
	int status;
	struct zsl_clr_xyz xyz;

	/* First get the XYZ equivalent for the current color temp. */
	status = zsl_clr_conv_ct_xyz(ct, obs, &xyz);
	if (status) {
		goto err;
	}

	/* Convert XYZ tristimulus to (u,v) chromaticity. */
	status = zsl_clr_conv_xyz_uv60(&xyz, uv);
	if (status) {
		goto err;
	}

	return 0;
err:
	return status;
}

int
zsl_clr_conv_ct_xyz(zsl_real_t ct, enum zsl_clr_obs obs, struct zsl_clr_xyz *xyz)
{
	zsl_real_t c1;
	zsl_real_t c2;
	zsl_real_t d_wl_m = 0.0;
	zsl_real_t d_wl_m5 = 0.0;
	zsl_real_t bbody = 0.0;
	const struct zsl_clr_obs_data *obs_data;

	/* TODO: Validate input range! */

	/*
	 * C1 and C2 constants for Planck's radiation law, where:
	 *
	 *   c1 = 3.74183 x 10^-16W m^2
	 *   c2 = 1.4388 x 10^-2 m K
	 *
	 * Note: Values are scaled further in algorithm below. */
	c1 = 374.183162616761619;
	c2 = 14.387863142323088;

	/* Clear the output placeholder. */
	memset(xyz, 0, sizeof *xyz);

	/* Get a reference to the standard observer CMF dataset. */
	zsl_clr_obs_get(obs, &obs_data);

	/* Calculate emittance at given wavelength using Planck's radiation
	 * law and the specified 5nm standard observer lookup table. */
	for (int nm = 360; nm <= 830; nm += 5) {
		int i = (nm - 360) / 5;
		/* Scale wavelength to micrometers. */
		d_wl_m = nm * 1.0e-3;
		/* d_wl_m^5. */
		d_wl_m5 = d_wl_m * d_wl_m * d_wl_m * d_wl_m * d_wl_m;
		/* Calculate black-body value. Source: Bruce Lindbloom */
		bbody = c1 / (d_wl_m5 * 1.0e-12 *
			      (ZSL_EXPM1(c2 / (ct * d_wl_m * 1.0e-3))));
		/* Calculate XYZ tristimulus using the std observer model. */
		xyz->xyz_x += (bbody * obs_data->data[i].xyz_x);
		xyz->xyz_y += (bbody * obs_data->data[i].xyz_y);
		xyz->xyz_z += (bbody * obs_data->data[i].xyz_z);
	}

	/* Normalise XYZ tristimulus for y = 1.0. */
	xyz->xyz_x /= xyz->xyz_y;
	xyz->xyz_z /= xyz->xyz_y;
	xyz->xyz_y = 1.0;

	/* Reference the standard observer model used. */
	xyz->observer = obs;

	return 0;
}

int
zsl_clr_conv_ct_rgb8(zsl_real_t ct, enum zsl_clr_obs obs, struct zsl_mtx *mtx,
		     struct zsl_clr_rgb8 *rgb)
{
	int rc;
	struct zsl_clr_xyz xyz;

	memset(&xyz, 0, sizeof xyz);

	/* Convert color temperature to an XYZ tristimulus. */
	rc = zsl_clr_conv_ct_xyz(ct, obs, &xyz);
	if (rc) {
		goto err;
	}

	/* Convert XYZ to RGBA using 'zsl_clr_conv_xyz_rgb8'. */
	return zsl_clr_conv_xyz_rgb8(&xyz, mtx, rgb);
err:
	rgb->r_invalid = 1;
	rgb->g_invalid = 1;
	rgb->b_invalid = 1;
	rgb->a_invalid = 1;
	return rc;
}

int
zsl_clr_conv_ct_rgbf(zsl_real_t ct, enum zsl_clr_obs obs, struct zsl_mtx *mtx,
		     struct zsl_clr_rgbf *rgb)
{
	int rc;
	struct zsl_clr_xyz xyz;

	memset(&xyz, 0, sizeof xyz);

	/* Convert color temperature to an XYZ tristimulus. */
	rc = zsl_clr_conv_ct_xyz(ct, obs, &xyz);
	if (rc) {
		goto err;
	}

	/* Convert XYZ to RGBA using 'zsl_clr_conv_xyz_rgbf'. */
	return zsl_clr_conv_xyz_rgbf(&xyz, mtx, rgb);
err:
	rgb->r_invalid = 1;
	rgb->g_invalid = 1;
	rgb->b_invalid = 1;
	rgb->a_invalid = 1;
	return rc;
}

int
zsl_clr_conv_cct_xyy(struct zsl_clr_cct *cct, enum zsl_clr_obs obs, struct zsl_clr_xyy *xyy)
{
	int rc;
	struct zsl_clr_xyz xyz;
	struct zsl_clr_uv60 delta;
	struct zsl_clr_uv60 uv0;
	struct zsl_clr_uv60 uv1;
	struct zsl_clr_uv60 final;

	/* Clear the xyY placeholder */
	memset(xyy, 0, sizeof *xyy);

	/*
	 * Calculate (u0,v0) of the Planckian radiator at T(K).
	 */
	rc = zsl_clr_conv_ct_xyz(cct->cct, obs, &xyz);
	if (rc) {
		goto err;
	}
	rc = zsl_clr_conv_xyz_uv60(&xyz, &uv0);
	if (rc) {
		goto err;
	}

	/*
	 * Calculate (u1,v1) of the Planckian radiator at T+dT(K) where dT=0.01.
	 */
	rc = zsl_clr_conv_ct_xyz(cct->cct + 0.01, obs, &xyz);
	if (rc) {
		goto err;
	}
	rc = zsl_clr_conv_xyz_uv60(&xyz, &uv1);
	if (rc) {
		goto err;
	}

	/*
	 * Calculate the slope angle theta of the Planckian locus at T(K):
	 *
	 *    du = u0 - u1
	 *    dv = v0 - v1
	 *    u = u0 - Duv * sin(theta)
	 *    v = v0 + Duv * cos(theta)
	 *
	 * where:
	 *
	 *    sin(theta) = dv / sqrt(du^2 + dv^2)
	 *    cos(theta) = du / sqrt(du^2 + dv^2)
	 */
	delta.uv60_u = uv0.uv60_u - uv1.uv60_u;
	delta.uv60_v = uv0.uv60_v - uv1.uv60_v;
	final.uv60_u = uv0.uv60_u -
		       cct->duv * (delta.uv60_v /
				   ZSL_SQRT(delta.uv60_u * delta.uv60_u +
					delta.uv60_v * delta.uv60_v));
	final.uv60_v = uv0.uv60_v +
		       cct->duv * (delta.uv60_u /
				   ZSL_SQRT(delta.uv60_u * delta.uv60_u +
					delta.uv60_v * delta.uv60_v));

	/*
	 * Then the CIE1960 (u,v) value (final) can be converted to CIE 1973
	 * (u',v') and a CIE 1931 (x,y) chromaticity via:
	 *
	 *    u' = u
	 *    v' = 1.5v
	 *
	 *    x = 9u' / (6u' - 16v' + 12)
	 *    y = 2v' / (3u' - 8v' + 6)
	 */
	xyy->xyy_x = 9.0 * final.uv60_u / (6.0 * final.uv60_u - 16.0 *
					   final.uv60_v * 1.5 + 12.0);
	xyy->xyy_y = 2.0 * final.uv60_v * 1.5 / (3.0 * final.uv60_u - 8.0 *
						 final.uv60_v * 1.5 + 6.0);
	xyy->xyy_Y = 1.0;
	xyy->observer = obs;

	return 0;
err:
	xyy->x_invalid = 1;
	xyy->y_invalid = 1;
	xyy->Y_invalid = 1;
	return rc;
}

int
zsl_clr_conv_cct_xyz(struct zsl_clr_cct *cct, enum zsl_clr_obs obs, struct zsl_clr_xyz *xyz)
{
	int rc;
	struct zsl_clr_xyy xyy;

	/* Perform the initial CCT+Duv to xyY conversion */
	rc = zsl_clr_conv_cct_xyy(cct, obs, &xyy);
	if (rc) {
		goto err;
	}

	/* Convert the xyY chromaticity to the equivalent XYZ tristimulus */
	rc = zsl_clr_conv_xyy_xyz(&xyy, xyz);
	if (rc) {
		goto err;
	}

	return 0;
err:
	xyz->x_invalid = 1;
	xyz->y_invalid = 1;
	xyz->z_invalid = 1;
	return rc;
}

static int
zsl_clr_conv_uv60_cct_mccamy(struct zsl_clr_uv60 *uv, struct zsl_clr_cct *cct)
{
	int rc;
	zsl_real_t n;
	struct zsl_clr_xyy xyy;

	/*
	 * CCT(x, y) = 449 * n^3 + 3525 * n^2 + 6823.3 * n + 5520.33
	 *
	 * Where:
	 *
	 * n = (x − xe) / (ye - y)
	 * xe = 0.3320
	 * ye = 0.1858
	 */

	/* Basic input validation. */
	if ((uv->u_invalid) || (uv->v_invalid)) {
		rc = -EINVAL;
		goto err;
	}

	/* Duv is not calculated as part of McCamy's aproximation. */
	cct->duv = 0.0;
	cct->duv_invalid = 1;

	/* Calculate cct using McCamy's approximation. */
	rc = zsl_clr_conv_uv60_xyy(uv, &xyy);
	n = (xyy.xyy_x - 0.3320) / (0.1858 - xyy.xyy_y);
	cct->cct = (449.0 * ZSL_POW(n, 3) + 3525.0 * ZSL_POW(n, 2) +
		    6823.3 * n + 5520.33);

	return 0;
err:
	cct->cct_invalid = 1;
	cct->duv_invalid = 1;
	return rc;
}

static int
zsl_clr_conv_uv60_cct_ohno2011(struct zsl_clr_uv60 *uv, struct zsl_clr_cct *cct)
{
	int rc;
	zsl_real_t l_fp; /* L = lightness (aka spectral radiance) */
	zsl_real_t l_bb;
	zsl_real_t l_p;
	zsl_real_t a_1;
	zsl_real_t a;
	zsl_real_t t1;
	zsl_real_t t2;
	zsl_real_t dt_c1;
	zsl_real_t dt_c2;
	zsl_real_t c;

	/* Basic input validation. */
	if ((uv->u_invalid) || (uv->v_invalid)) {
		rc = -EINVAL;
		goto err;
	}

	/* Clear CCT before starting. */
	memset(cct, 0, sizeof *cct);

	/* Calculate L_fp. */
	l_fp = ZSL_SQRT((uv->uv60_u - 0.292) * (uv->uv60_u - 0.292) +
		    (uv->uv60_v - 0.24) * (uv->uv60_v - 0.24));

	/* Calculate the black-body spectral radiance using k[0] constants.
	 * Note: This formula uses an approximation since we don't know
	 * the CCT. */
	a_1 = atan((uv->uv60_v - 0.24) / (uv->uv60_u - 0.292));
	a = a_1 >= 0.0 ?  a_1 : a_1 + M_PI;
	l_bb = zsl_clr_conv_xyy_cct_ohno_2011_data[0][6] * ZSL_POW(a, 6) +
	       zsl_clr_conv_xyy_cct_ohno_2011_data[0][5] * ZSL_POW(a, 5) +
	       zsl_clr_conv_xyy_cct_ohno_2011_data[0][4] * ZSL_POW(a, 4) +
	       zsl_clr_conv_xyy_cct_ohno_2011_data[0][3] * ZSL_POW(a, 3) +
	       zsl_clr_conv_xyy_cct_ohno_2011_data[0][2] * ZSL_POW(a, 2) +
	       zsl_clr_conv_xyy_cct_ohno_2011_data[0][1] * a +
	       zsl_clr_conv_xyy_cct_ohno_2011_data[0][0];

	/* Set the Duv value. */
	cct->duv = l_fp - l_bb;

	/* TODO: Determine proper value of L_p! */
	l_p = l_fp;

	/* Calculate T1 and DT_c1 delta depending on the value of a. */
	if (a < 2.54) {
		t1 = 1 / (zsl_clr_conv_xyy_cct_ohno_2011_data[1][6] * ZSL_POW(a, 6) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[1][5] * ZSL_POW(a, 5) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[1][4] * ZSL_POW(a, 4) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[1][3] * ZSL_POW(a, 3) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[1][2] * ZSL_POW(a, 2) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[1][1] * a +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[1][0]);
		dt_c1 =  (zsl_clr_conv_xyy_cct_ohno_2011_data[3][6] * ZSL_POW(a, 6) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[3][5] * ZSL_POW(a, 5) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[3][4] * ZSL_POW(a, 4) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[3][3] * ZSL_POW(a, 3) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[3][2] * ZSL_POW(a, 2) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[3][1] * a +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[3][0]) *
			(l_bb + 0.01) / l_p * cct->duv / 0.01;
	} else {
		t1 = 1 / (zsl_clr_conv_xyy_cct_ohno_2011_data[2][6] * ZSL_POW(a, 6) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[2][5] * ZSL_POW(a, 5) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[2][4] * ZSL_POW(a, 4) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[2][3] * ZSL_POW(a, 3) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[2][2] * ZSL_POW(a, 2) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[2][1] * a +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[2][0]);
		dt_c1 =  (zsl_clr_conv_xyy_cct_ohno_2011_data[4][6] * ZSL_POW(a, 6) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[4][5] * ZSL_POW(a, 5) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[4][4] * ZSL_POW(a, 4) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[4][3] * ZSL_POW(a, 3) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[4][2] * ZSL_POW(a, 2) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[4][1] * a +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[4][0]) *
			(l_bb + 0.01) / l_p * cct->duv / 0.01;
	}

	/* Calculate T2 and c. */
	t2 = t1 - dt_c1;
	c = log10(t2);

	/* Calculate DT_c2 depending on positive or negative Duv. */
	if (cct->duv >= 0.0) {
		dt_c2 =  (zsl_clr_conv_xyy_cct_ohno_2011_data[5][6] * ZSL_POW(c, 6) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[5][5] * ZSL_POW(c, 5) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[5][4] * ZSL_POW(c, 4) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[5][3] * ZSL_POW(c, 3) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[5][2] * ZSL_POW(c, 2) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[5][1] * c +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[5][0]);
	} else {
		dt_c2 =  (zsl_clr_conv_xyy_cct_ohno_2011_data[6][6] * ZSL_POW(c, 6) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[6][5] * ZSL_POW(c, 5) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[6][4] * ZSL_POW(c, 4) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[6][3] * ZSL_POW(c, 3) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[6][2] * ZSL_POW(c, 2) +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[6][1] * c +
			  zsl_clr_conv_xyy_cct_ohno_2011_data[6][0]) *
			ZSL_POW(cct->duv / 0.03, 2);
	}

	/* Assign the final correlated color temperature. */
	cct->cct = t2 - dt_c2;

	return 0;
err:
	cct->cct_invalid = 1;
	cct->duv_invalid = 1;
	return rc;
}

// /**
//  * @brief Calculates the distance between the supplied (u,v) chromaticity
//  *        (uv) and the equivalent (u,v) chromaticity for the specified color
//  *        temperature. Used to determine how far the supplied uv parameter is
//  *        from the supplied color temperature.
//  *
//  * @param ct    The ref. color temperature to convert to a (u,v) chromaticity.
//  * @param obs   The CIE standard observer model to use for the conversion.
//  * @param uv    Pointer to the (u,v) chromaticity to compare against ct.
//  * @param d     Pointer to the placeholder for the calculated distance.
//  *
//  * @return The distance between uv and ct's (u,v) equivalent.
//  */
// static int
// zsl_clr_conv_calc_dist_uv60(zsl_real_t ct, enum zsl_clr_obs obs,
// 			struct zsl_clr_uv60 *uv, zsl_real_t *d)
// {
// 	int status;
// 	struct zsl_clr_uv60 ct_uv;
//
// 	status = zsl_clr_conv_ct_uv60(ct, obs, &ct_uv);
// 	if (status) {
// 		goto err;
// 	}
//
// 	*d = ((ct_uv.uv60_u - uv->uv60_u) * (ct_uv.uv60_u - uv->uv60_u) +
// 	      (ct_uv.uv60_v - uv->uv60_v) * (ct_uv.uv60_v - uv->uv60_v));
//
// 	return 0;
// err:
// 	return status;
// }

static int
zsl_clr_conv_uv60_cct_ohno2014(struct zsl_clr_uv60 *uv, struct zsl_clr_cct *cct)
{
	int rc;
	zsl_real_t l_fp;
	zsl_real_t l_bb;
	zsl_real_t a;
	zsl_real_t d_cur;
	zsl_real_t d_best;      /* d */
	zsl_real_t d_prev;      /* d m-1 */
	zsl_real_t d_next;      /* d m+1 */
	zsl_real_t x;           /* Distance from d_best to d m+1. */
	zsl_real_t l;           /* Total width of d m-1 to d m+1. */
	int match_idx;          /* Lookup index for d_best values. */

	/* Basic input validation. */
	if ((uv->u_invalid) || (uv->v_invalid)) {
		rc = -EINVAL;
		goto err;
	}

	/* Clear values before starting. */
	memset(cct, 0, sizeof *cct);

	/* Triangular solution for CCT. */
	d_best = 1.0;

	/* Search for closest match for uv from 1% CT to (u,v) lookup.
	 * Note: the static lookup table could be removed at the cost of a
	 * performance hit by using the following function to calculate
	 * distance on demand:
	 *   zsl_clr_conv_calc_dist_uv60(ct_cur, ZSL_CLR_OBS_2_DEG, uv, &d_cur);
	 */
	match_idx = 0;
	for (size_t i = 0; i < OHNO2014_LOOKUP_RECS; i++) {
		/* Calculate distance of CT (u,v) to ref (u,v) chromaticity. */
		d_cur = ((zsl_clr_conv_ct_uv_ohno_2014_data[i][1] - uv->uv60_u) *
			 (zsl_clr_conv_ct_uv_ohno_2014_data[i][1] - uv->uv60_u) +
			 (zsl_clr_conv_ct_uv_ohno_2014_data[i][2] - uv->uv60_v) *
			 (zsl_clr_conv_ct_uv_ohno_2014_data[i][2] - uv->uv60_v));
		/* Track best match. */
		if (d_cur < d_best) {
			d_best = d_cur;
			match_idx = i;
		}
	}

	/* Make sure we're within the 1000 K to 20000 K range. */
	if ((zsl_clr_conv_ct_uv_ohno_2014_data[match_idx][0] <
	     zsl_clr_conv_ct_uv_ohno_2014_data[1][0]) ||
	    (zsl_clr_conv_ct_uv_ohno_2014_data[match_idx][0] >
	     zsl_clr_conv_ct_uv_ohno_2014_data[OHNO2014_LOOKUP_RECS - 2][0])) {
		return -EINVAL;
	}

	/* Calculate prev distance. */
	d_prev = ((zsl_clr_conv_ct_uv_ohno_2014_data[match_idx - 1][1] -
		   uv->uv60_u) *
		  (zsl_clr_conv_ct_uv_ohno_2014_data[match_idx - 1][1] -
		   uv->uv60_u) +
		  (zsl_clr_conv_ct_uv_ohno_2014_data[match_idx - 1][2] -
		   uv->uv60_v) *
		  (zsl_clr_conv_ct_uv_ohno_2014_data[match_idx - 1][2] -
		   uv->uv60_v));

	/* Calculate next distance. */
	d_next = ((zsl_clr_conv_ct_uv_ohno_2014_data[match_idx + 1][1] -
		   uv->uv60_u) *
		  (zsl_clr_conv_ct_uv_ohno_2014_data[match_idx + 1][1] -
		   uv->uv60_u) +
		  (zsl_clr_conv_ct_uv_ohno_2014_data[match_idx + 1][2] -
		   uv->uv60_v) *
		  (zsl_clr_conv_ct_uv_ohno_2014_data[match_idx + 1][2] -
		   uv->uv60_v));

	l = ((zsl_clr_conv_ct_uv_ohno_2014_data[match_idx + 1][1] -
	      zsl_clr_conv_ct_uv_ohno_2014_data[match_idx - 1][1]) *
	     (zsl_clr_conv_ct_uv_ohno_2014_data[match_idx + 1][1] -
	      zsl_clr_conv_ct_uv_ohno_2014_data[match_idx - 1][1])) +
	    ((zsl_clr_conv_ct_uv_ohno_2014_data[match_idx + 1][2] -
	      zsl_clr_conv_ct_uv_ohno_2014_data[match_idx - 1][2]) *
	     (zsl_clr_conv_ct_uv_ohno_2014_data[match_idx + 1][2] -
	      zsl_clr_conv_ct_uv_ohno_2014_data[match_idx - 1][2]));
	l = ZSL_SQRT(l);

	x = ((d_prev * d_prev) - (d_next * d_next) + (l * l)) / (2.0 * l);

	cct->cct = zsl_clr_conv_ct_uv_ohno_2014_data[match_idx - 1][0] +
		   (zsl_clr_conv_ct_uv_ohno_2014_data[match_idx + 1][0] -
		    zsl_clr_conv_ct_uv_ohno_2014_data[match_idx - 1][0]) *
		   (x / l);

	/* Calculate Duv. */
	l_fp = ZSL_SQRT((uv->uv60_u - 0.292) * (uv->uv60_u - 0.292) +
		    (uv->uv60_v - 0.24) * (uv->uv60_v - 0.24));
	a = ZSL_ACOS((uv->uv60_u - 0.292) / l_fp);
	l_bb = zsl_clr_conv_xyy_cct_ohno_2014_data[6] * ZSL_POW(a, 6) +
	       zsl_clr_conv_xyy_cct_ohno_2014_data[5] * ZSL_POW(a, 5) +
	       zsl_clr_conv_xyy_cct_ohno_2014_data[4] * ZSL_POW(a, 4) +
	       zsl_clr_conv_xyy_cct_ohno_2014_data[3] * ZSL_POW(a, 3) +
	       zsl_clr_conv_xyy_cct_ohno_2014_data[2] * ZSL_POW(a, 2) +
	       zsl_clr_conv_xyy_cct_ohno_2014_data[1] * a +
	       zsl_clr_conv_xyy_cct_ohno_2014_data[0];

	/* Set the Duv value. */
	cct->duv = l_fp - l_bb;

	return 0;
err:
	cct->cct_invalid = 1;
	cct->duv_invalid = 1;
	return rc;
}

int
zsl_clr_conv_uv60_cct(enum zsl_clr_uv_cct_method method, struct zsl_clr_uv60 *uv,
		      struct zsl_clr_cct *cct)
{
	/* Use the specified algorithm for the conversion. */
	switch (method) {
	case ZSL_CLR_UV_CCT_METHOD_MCCAMY:
		return zsl_clr_conv_uv60_cct_mccamy(uv, cct);
	case ZSL_CLR_UV_CCT_METHOD_OHNO2011:
		return zsl_clr_conv_uv60_cct_ohno2011(uv, cct);
	case ZSL_CLR_UV_CCT_METHOD_OHNO2014:
		return zsl_clr_conv_uv60_cct_ohno2014(uv, cct);
	default:
		return zsl_clr_conv_uv60_cct_ohno2014(uv, cct);
	}
}

int
zsl_clr_conv_xyz_rgb8(struct zsl_clr_xyz *xyz, struct zsl_mtx *mtx,
		      struct zsl_clr_rgb8 *rgb)
{
	int rc;
	struct zsl_clr_rgbf rgbf;

	/* Use the more precise floating point version, then convert. */
	rc = zsl_clr_conv_xyz_rgbf(xyz, mtx, &rgbf);
	if (rc) {
		goto err;
	}

	/* Flag out of gamut colors. */
	rgb->r_invalid = rgbf.r_invalid;
	rgb->g_invalid = rgbf.g_invalid;
	rgb->b_invalid = rgbf.b_invalid;

	/* We need to cap out of gamut colors before converting to 8-bit. */
	#if !CONFIG_ZSL_CLR_RGBF_BOUND_CAP
	if (rgbf.r > 1.0) {
		rgbf.r = 1.0;
	}
	if (rgbf.r < 0.0) {
		rgbf.r = 0.0;
	}
	if (rgbf.g > 1.0) {
		rgbf.g = 1.0;
	}
	if (rgbf.g < 0.0) {
		rgbf.g = 0.0;
	}
	if (rgbf.b > 1.0) {
		rgbf.b = 1.0;
	}
	if (rgbf.b < 0.0) {
		rgbf.b = 0.0;
	}
	#endif

	rgb->r = (uint8_t)(rgbf.r * 255.0);
	rgb->g = (uint8_t)(rgbf.g * 255.0);
	rgb->b = (uint8_t)(rgbf.b * 255.0);
	rgb->a = 0xFF;

	return 0;
err:
	rgb->r_invalid = 1;
	rgb->g_invalid = 1;
	rgb->b_invalid = 1;
	rgb->a_invalid = 1;
	return rc;
}

int
zsl_clr_conv_xyz_rgbf(struct zsl_clr_xyz *xyz, struct zsl_mtx *mtx,
		      struct zsl_clr_rgbf *rgb)
{
	int rc;

	/* Actually vectors, but declaring as a matrix makes life easier. */
	ZSL_MATRIX_DEF(xyz_mtx, 3, 1);
	ZSL_MATRIX_DEF(rgb_mtx, 3, 1);
	zsl_mtx_init(&xyz_mtx, NULL);
	zsl_mtx_init(&rgb_mtx, NULL);

	/* Clear the RGB placeholder and the interim XYZ tristimulus. */
	memset(rgb, 0, sizeof *rgb);

	/* Assign the XYZ values to the temp matrix. */
	xyz_mtx.data[0] = xyz->xyz_x;
	xyz_mtx.data[1] = xyz->xyz_y;
	xyz_mtx.data[2] = xyz->xyz_z;

	/* Convert XYZ to RGBF using the specified correlation matrix. */
	rc = zsl_mtx_mult(mtx, &xyz_mtx, &rgb_mtx);
	if (rc) {
		goto err;
	}

	/* Correct for out of gamut colors, marking them as invalid. */
	/* Red underflow correction. */
	if (rgb_mtx.data[0] < 0.0) {
		#if CONFIG_ZSL_CLR_RGBF_BOUND_CAP
		rgb_mtx.data[0] = 0.0;
		#endif
		rgb->r_invalid = 1;
	}
	/* Green underflow correction. */
	if (rgb_mtx.data[1] < 0.0) {
		#if CONFIG_ZSL_CLR_RGBF_BOUND_CAP
		rgb_mtx.data[1] = 0.0;
		#endif
		rgb->g_invalid = 1;
	}
	/* Blue underflow correction. */
	if (rgb_mtx.data[2] < 0.0) {
		#if CONFIG_ZSL_CLR_RGBF_BOUND_CAP
		rgb_mtx.data[2] = 0.0;
		#endif
		rgb->b_invalid = 1;
	}

	/* Red overflow correction. */
	if (rgb_mtx.data[0] >= 1.0) {
		#if CONFIG_ZSL_CLR_RGBF_BOUND_CAP
		rgb->r = 1.0;
		#else
		rgb->r = rgb_mtx.data[0];
		#endif
		rgb->r_invalid = 1;
	} else {
		rgb->r = rgb_mtx.data[0];
	}
	/* Green overflow correction. */
	if (rgb_mtx.data[1] >= 1.0) {
		#if CONFIG_ZSL_CLR_RGBF_BOUND_CAP
		rgb->g = 1.0;
		#else
		rgb->g = rgb_mtx.data[1];
		#endif
		rgb->g_invalid = 1;
	} else {
		rgb->g = rgb_mtx.data[1];
	}
	/* Blue overflow correction. */
	if (rgb_mtx.data[2] >= 1.0) {
		#if CONFIG_ZSL_CLR_RGBF_BOUND_CAP
		rgb->b = 1.0;
		#else
		rgb->b = rgb_mtx.data[2];
		#endif
		rgb->b_invalid = 1;
	} else {
		rgb->b = rgb_mtx.data[2];
	}

	/* Alpha channel. */
	rgb->a = 1.0;

	return 0;
err:
	rgb->r_invalid = 1;
	rgb->g_invalid = 1;
	rgb->b_invalid = 1;
	rgb->a_invalid = 1;
	return rc;
}
