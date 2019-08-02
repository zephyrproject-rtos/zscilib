/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ztest.h>

extern void test_complex_add(void);

extern void test_interp_lerp(void);
extern void test_interp_find_x_asc(void);
extern void test_interp_find_x_desc(void);
extern void test_interp_nn(void);
extern void test_interp_nn_arr(void);
extern void test_interp_lin_y(void);
extern void test_interp_lin_y_arr(void);
extern void test_interp_lin_x(void);
extern void test_interp_cubic_arr(void);

extern void test_matrix_init(void);
extern void test_matrix_from_arr(void);
extern void test_matrix_copy(void);
extern void test_matrix_get(void);
extern void test_matrix_set(void);
extern void test_matrix_get_set_row(void);
extern void test_matrix_get_set_col(void);
extern void test_matrix_row_from_vec(void);
extern void test_matrix_unary_op(void);
extern void test_matrix_unary_func(void);
extern void test_matrix_binary_op(void);
extern void test_matrix_binary_func(void);
extern void test_matrix_add(void);
extern void test_matrix_add_d(void);
extern void test_matrix_sum_rows_d(void);
extern void test_matrix_sum_rows_scaled_d(void);
extern void test_matrix_sub(void);
extern void test_matrix_sub_d(void);
extern void test_matrix_mult_sq(void);
extern void test_matrix_mult_rect(void);
extern void test_matrix_scalar_mult_d(void);
extern void test_matrix_scalar_mult_row_d(void);
extern void test_matrix_trans(void);
extern void test_matrix_adj(void);
extern void test_matrix_deter(void);
extern void test_matrix_gauss_elim(void);
extern void test_matrix_gauss_elim_d(void);
extern void test_matrix_gauss_reduc(void);
extern void test_matrix_norm_elem(void);
extern void test_matrix_norm_elem_d(void);
extern void test_matrix_inv(void);
extern void test_matrix_inv_nxn(void);
extern void test_matrix_min(void);
extern void test_matrix_max(void);
extern void test_matrix_min_idx(void);
extern void test_matrix_max_idx(void);
extern void test_matrix_is_equal(void);
extern void test_matrix_is_notneg(void);

extern void test_vector_from_arr(void);
extern void test_vector_copy(void);
extern void test_vector_get_subset(void);
extern void test_vector_add(void);
extern void test_vector_sub(void);
extern void test_vector_neg(void);
extern void test_vector_sum(void);
extern void test_vector_magn(void);
extern void test_vector_scalar_add(void);
extern void test_vector_scalar_mult(void);
extern void test_vector_scalar_div(void);
extern void test_vector_dist(void);
extern void test_vector_dot(void);
extern void test_vector_norm(void);
extern void test_vector_to_unit(void);
extern void test_vector_cross(void);
extern void test_vector_sum_of_sqrs(void);
extern void test_vector_mean(void);
extern void test_vector_ar_mean(void);
extern void test_vector_rev(void);
extern void test_vector_is_equal(void);
extern void test_vector_is_notneg(void);
extern void test_vector_contains(void);

extern void test_phy_grav_orb_period(void);
extern void test_phy_grav_esc_vel(void);
extern void test_phy_grav_acc(void);
extern void test_phy_grav_orb_vel(void);
extern void test_phy_grav_force(void);

extern void test_phy_proj_init_vel(void);
extern void test_phy_proj_time(void);
extern void test_phy_proj_time_first(void);
extern void test_phy_proj_time2(void);
extern void test_phy_proj_ver_motion(void);
extern void test_phy_proj_ver_vel(void);
extern void test_phy_proj_hor_motion(void);
extern void test_phy_proj_trajectory(void);
extern void test_phy_proj_vel(void);
extern void test_phy_proj_angle(void);
extern void test_phy_proj_range(void);

extern void test_phy_kin_dist(void);
extern void test_phy_kin_time(void);
extern void test_phy_kin_vel(void);
extern void test_phy_kin_vel2(void);
extern void test_phy_kin_av_vel(void);
extern void test_phy_kin_accel(void);
extern void test_phy_kin_ener(void);

extern void test_phy_mass_center(void);

extern void test_phy_atom_nucl_radius(void);
extern void test_phy_atom_bohr_orb_radius(void);
extern void test_phy_atom_bohr_orb_vel(void);
extern void test_phy_atom_bohr_orb_ener(void);
extern void test_phy_atom_rad_decay(void);
extern void test_phy_atom_bragg(void);

extern void test_phy_dyn_newton(void);
extern void test_phy_dyn_fric(void);
extern void test_phy_dyn_normal(void);

extern void test_phy_rot_angle(void);

extern void test_phy_ecmp_capac_cpv(void);
extern void test_phy_ecmp_capac_ad(void);

void test_main(void)
{
	ztest_test_suite(zsl_tests,

        ztest_unit_test(test_complex_add),

        ztest_unit_test(test_interp_lerp),
        ztest_unit_test(test_interp_find_x_asc),
        ztest_unit_test(test_interp_find_x_desc),
        ztest_unit_test(test_interp_nn),
        ztest_unit_test(test_interp_nn_arr),
        ztest_unit_test(test_interp_lin_y),
        ztest_unit_test(test_interp_lin_y_arr),
        ztest_unit_test(test_interp_lin_x),
        ztest_unit_test(test_interp_cubic_arr),

        ztest_unit_test(test_matrix_init),
        ztest_unit_test(test_matrix_from_arr),
        ztest_unit_test(test_matrix_copy),
        ztest_unit_test(test_matrix_get),
        ztest_unit_test(test_matrix_set),
        ztest_unit_test(test_matrix_get_set_row),
        ztest_unit_test(test_matrix_get_set_col),
        ztest_unit_test(test_matrix_row_from_vec),
        ztest_unit_test(test_matrix_unary_op),
        ztest_unit_test(test_matrix_unary_func),
        ztest_unit_test(test_matrix_binary_op),
        ztest_unit_test(test_matrix_binary_func),
        ztest_unit_test(test_matrix_add),
        ztest_unit_test(test_matrix_add_d),
        ztest_unit_test(test_matrix_sum_rows_d),
        ztest_unit_test(test_matrix_sum_rows_scaled_d),
        ztest_unit_test(test_matrix_sub),
        ztest_unit_test(test_matrix_sub_d),
        ztest_unit_test(test_matrix_mult_sq),
        ztest_unit_test(test_matrix_mult_rect),
        ztest_unit_test(test_matrix_scalar_mult_d),
        ztest_unit_test(test_matrix_scalar_mult_row_d),
        ztest_unit_test(test_matrix_trans),
        ztest_unit_test(test_matrix_adj),
        ztest_unit_test(test_matrix_deter),
        ztest_unit_test(test_matrix_gauss_elim),
        ztest_unit_test(test_matrix_gauss_elim_d),
        ztest_unit_test(test_matrix_gauss_reduc),
        ztest_unit_test(test_matrix_norm_elem),
        ztest_unit_test(test_matrix_norm_elem_d),
        ztest_unit_test(test_matrix_inv),
        ztest_unit_test(test_matrix_inv_nxn),
        ztest_unit_test(test_matrix_min),
        ztest_unit_test(test_matrix_max),
        ztest_unit_test(test_matrix_min_idx),
        ztest_unit_test(test_matrix_max_idx),
        ztest_unit_test(test_matrix_is_equal),
        ztest_unit_test(test_matrix_is_notneg),

        ztest_unit_test(test_vector_from_arr),
        ztest_unit_test(test_vector_copy),
        ztest_unit_test(test_vector_get_subset),
        ztest_unit_test(test_vector_add),
        ztest_unit_test(test_vector_sub),
        ztest_unit_test(test_vector_neg),
        ztest_unit_test(test_vector_sum),
        ztest_unit_test(test_vector_magn),
        ztest_unit_test(test_vector_scalar_add),
        ztest_unit_test(test_vector_scalar_mult),
        ztest_unit_test(test_vector_scalar_div),
        ztest_unit_test(test_vector_dist),
        ztest_unit_test(test_vector_dot),
        ztest_unit_test(test_vector_norm),
        ztest_unit_test(test_vector_to_unit),
        ztest_unit_test(test_vector_cross),
        ztest_unit_test(test_vector_sum_of_sqrs),
        ztest_unit_test(test_vector_mean),
        ztest_unit_test(test_vector_ar_mean),
        ztest_unit_test(test_vector_rev),
        ztest_unit_test(test_vector_is_equal),
        ztest_unit_test(test_vector_is_notneg),
        ztest_unit_test(test_vector_contains),

        ztest_unit_test(test_phy_grav_orb_period),
        ztest_unit_test(test_phy_grav_esc_vel),
        ztest_unit_test(test_phy_grav_acc),
        ztest_unit_test(test_phy_grav_orb_vel),
        ztest_unit_test(test_phy_grav_force),

        ztest_unit_test(test_phy_proj_init_vel),
        ztest_unit_test(test_phy_proj_time),
	ztest_unit_test(test_phy_proj_time_first),
        ztest_unit_test(test_phy_proj_time2),
	ztest_unit_test(test_phy_proj_ver_motion),
	ztest_unit_test(test_phy_proj_ver_vel),
	ztest_unit_test(test_phy_proj_hor_motion),
	ztest_unit_test(test_phy_proj_trajectory),
	ztest_unit_test(test_phy_proj_vel),
	ztest_unit_test(test_phy_proj_angle),
	ztest_unit_test(test_phy_proj_range),

	ztest_unit_test(test_phy_kin_dist),
	ztest_unit_test(test_phy_kin_time),
	ztest_unit_test(test_phy_kin_vel),
	ztest_unit_test(test_phy_kin_vel2),
	ztest_unit_test(test_phy_kin_av_vel),
	ztest_unit_test(test_phy_kin_accel),
	ztest_unit_test(test_phy_kin_ener),

	ztest_unit_test(test_phy_mass_center),
			
	ztest_unit_test(test_phy_atom_nucl_radius),
	ztest_unit_test(test_phy_atom_bohr_orb_radius),
	ztest_unit_test(test_phy_atom_bohr_orb_vel),
	ztest_unit_test(test_phy_atom_bohr_orb_ener),
	ztest_unit_test(test_phy_atom_rad_decay),
	ztest_unit_test(test_phy_atom_bragg),
			 
	ztest_unit_test(test_phy_dyn_newton),
	ztest_unit_test(test_phy_dyn_fric),
	ztest_unit_test(test_phy_dyn_normal),
	
	ztest_unit_test(test_phy_rot_angle),

        ztest_unit_test(test_phy_ecmp_capac_cpv),
        ztest_unit_test(test_phy_ecmp_capac_ad)

	);

	ztest_run_test_suite(zsl_tests);
}
