/*
 * Copyright (c) 2019 Kevin Townsend (KTOWN)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>

extern void test_conv_spd_xyz(void);
extern void test_conv_ct_xyz(void);
extern void test_conv_ct_rgb8(void);
extern void test_conv_cct_xyy(void);
extern void test_conv_cct_xyz(void);
extern void test_conv_uv60_cct_ohno2011(void);
extern void test_conv_uv60_cct_ohno2014(void);

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
extern void test_matrix_mult_d(void);
extern void test_matrix_scalar_mult_d(void);
extern void test_matrix_scalar_mult_row_d(void);
extern void test_matrix_trans(void);
extern void test_matrix_adjoint_3x3(void);
extern void test_matrix_adjoint(void);
extern void test_matrix_reduce(void);
extern void test_matrix_reduce_iter(void);
extern void test_matrix_augm_diag(void);
extern void test_matrix_deter_3x3(void);
extern void test_matrix_deter(void);
extern void test_matrix_gauss_elim(void);
extern void test_matrix_gauss_elim_d(void);
extern void test_matrix_gauss_reduc(void);
extern void test_matrix_gram_schmidt_sq(void);
extern void test_matrix_gram_schmidt_rect(void);
extern void test_matrix_cols_norm(void);
extern void test_matrix_norm_elem(void);
extern void test_matrix_norm_elem_d(void);
extern void test_matrix_inv_3x3(void);
extern void test_matrix_inv(void);
extern void test_matrix_cholesky(void);
extern void test_matrix_balance(void);
extern void test_matrix_householder_sq(void);
extern void test_matrix_householder_rect(void);
extern void test_matrix_qrd(void);
extern void test_matrix_qrd_hess(void);
extern void test_matrix_min(void);
extern void test_matrix_max(void);
extern void test_matrix_min_idx(void);
extern void test_matrix_max_idx(void);
extern void test_matrix_is_equal(void);
extern void test_matrix_is_notneg(void);
extern void test_matrix_is_sym(void);

/* Test for functions that only work with double-precision floats. */
#ifndef CONFIG_ZSL_SINGLE_PRECISION
extern void test_matrix_vector_wedge(void);
extern void test_matrix_qrd_iter(void);
extern void test_matrix_eigenvalues(void);
extern void test_matrix_eigenvectors(void);
extern void test_matrix_svd(void);
extern void test_matrix_pinv(void);
#endif

extern void test_vector_init(void);
extern void test_vector_from_arr(void);
extern void test_vector_copy(void);
extern void test_vector_get_subset(void);
extern void test_vector_add(void);
extern void test_vector_sub(void);
extern void test_vector_neg(void);
extern void test_vector_sum(void);
extern void test_vector_scalar_add(void);
extern void test_vector_scalar_mult(void);
extern void test_vector_scalar_div(void);
extern void test_vector_dist(void);
extern void test_vector_dot(void);
extern void test_vector_norm(void);
extern void test_vector_project(void);
extern void test_vector_to_unit(void);
extern void test_vector_cross(void);
extern void test_vector_sum_of_sqrs(void);
extern void test_vector_mean(void);
extern void test_vector_ar_mean(void);
extern void test_vector_rev(void);
extern void test_vector_zte(void);
extern void test_vector_is_equal(void);
extern void test_vector_is_nonneg(void);
extern void test_vector_contains(void);
extern void test_vector_sort(void);

extern void test_phy_atom_nucl_radius(void);
extern void test_phy_atom_bohr_orb_radius(void);
extern void test_phy_atom_bohr_orb_vel(void);
extern void test_phy_atom_bohr_orb_ener(void);
extern void test_phy_atom_rad_decay(void);
extern void test_phy_atom_bragg(void);

extern void test_phy_dyn_newton(void);
extern void test_phy_dyn_fric(void);
extern void test_phy_dyn_normal(void);
extern void test_phy_dyn_pendul_period(void);
extern void test_phy_dyn_pendul_max_speed(void);
extern void test_phy_dyn_tension(void);
extern void test_phy_dyn_lever(void);

extern void test_phy_ecmp_capac_cpv(void);
extern void test_phy_ecmp_capac_ad(void);
extern void test_phy_ecmp_ener_capac(void);
extern void test_phy_ecmp_ener_induc(void);
extern void test_phy_ecmp_trans(void);
extern void test_phy_ecmp_rlc_volt(void);
extern void test_phy_ecmp_rc_charg_i(void);
extern void test_phy_ecmp_rc_charg_q(void);
extern void test_phy_ecmp_rc_discharg_i(void);
extern void test_phy_ecmp_rc_discharg_q(void);
extern void test_phy_ecmp_rl_current(void);

extern void test_phy_elcty_current(void);
extern void test_phy_elcty_res_series(void);
extern void test_phy_elcty_res_parallel(void);
extern void test_phy_elcty_cap_series(void);
extern void test_phy_elcty_cap_parallel(void);
extern void test_phy_elcty_resistivity(void);
extern void test_phy_elcty_ohm_law(void);
extern void test_phy_elcty_power_vi(void);
extern void test_phy_elcty_power_ir(void);
extern void test_phy_elcty_power_vr(void);

extern void test_phy_elec_charge_dens(void);
extern void test_phy_elec_force(void);
extern void test_phy_elec_force2(void);
extern void test_phy_elec_field(void);
extern void test_phy_elec_pot_ener(void);
extern void test_phy_elec_potential(void);
extern void test_phy_elec_flux(void);

extern void test_phy_ener_kin(void);
extern void test_phy_ener_rot(void);
extern void test_phy_ener_grav_pot(void);
extern void test_phy_ener_elas_pot(void);
extern void test_phy_ener_power(void);
extern void test_phy_ener_fric(void);
extern void test_phy_ener_mec(void);
extern void test_phy_ener_final(void);
extern void test_phy_ener_photon(void);
extern void test_phy_ener_photon2(void);

extern void test_phy_fluid_dens(void);
extern void test_phy_fluid_simple_press(void);
extern void test_phy_fluid_press(void);
extern void test_phy_fluid_bouyant_force(void);
extern void test_phy_fluid_flow_rate(void);
extern void test_phy_fluid_force_prop(void);
extern void test_phy_fluid_bernoulli(void);
extern void test_phy_fluid_volume_flow_rate(void);

extern void test_phy_gas_av_vel(void);
extern void test_phy_gas_press(void);
extern void test_phy_gas_boyle(void);
extern void test_phy_gas_charles_lussac(void);

extern void test_phy_grav_orb_period(void);
extern void test_phy_grav_esc_vel(void);
extern void test_phy_grav_acc(void);
extern void test_phy_grav_orb_vel(void);
extern void test_phy_grav_force(void);
extern void test_phy_grav_pot_ener(void);

extern void test_phy_kin_dist(void);
extern void test_phy_kin_init_pos(void);
extern void test_phy_kin_init_pos2(void);
extern void test_phy_kin_time(void);
extern void test_phy_kin_vel(void);
extern void test_phy_kin_vel2(void);
extern void test_phy_kin_init_vel(void);
extern void test_phy_kin_init_vel2(void);
extern void test_phy_kin_init_vel3(void);
extern void test_phy_kin_av_vel(void);
extern void test_phy_kin_accel(void);
extern void test_phy_kin_ener(void);

extern void test_phy_magn_force(void);
extern void test_phy_magn_force_wire(void);
extern void test_phy_magn_torque_loop(void);
extern void test_phy_magn_pot_dipole(void);
extern void test_phy_magn_orb_radius(void);
extern void test_phy_magn_flux(void);
extern void test_phy_magn_mom(void);

extern void test_phy_mass_center(void);

extern void test_phy_mom_mom(void);
extern void test_phy_mom_imp(void);
extern void test_phy_mom_change(void);
extern void test_phy_mom_elas(void);
extern void test_phy_mom_inelas(void);

extern void test_phy_opt_refrac_index(void);
extern void test_phy_opt_snell(void);
extern void test_phy_opt_focus(void);
extern void test_phy_opt_critic_angle(void);
extern void test_phy_opt_power(void);
extern void test_phy_opt_magn(void);
extern void test_phy_opt_dif(void);

extern void test_phy_photon_ener(void);
extern void test_phy_photon_mom(void);
extern void test_phy_photon_wavelenght(void);
extern void test_phy_photon_frequency(void);
extern void test_phy_photon_photoelectric(void);

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

extern void test_phy_rot_angle(void);
extern void test_phy_rot_dist(void);
extern void test_phy_rot_turn(void);
extern void test_phy_rot_time(void);
extern void test_phy_rot_omega(void);
extern void test_phy_rot_omega2(void);
extern void test_phy_rot_av_omega(void);
extern void test_phy_rot_vel(void);
extern void test_phy_rot_ang_accel(void);
extern void test_phy_rot_accel(void);
extern void test_phy_rot_ener(void);
extern void test_phy_rot_period(void);
extern void test_phy_rot_frequency(void);
extern void test_phy_rot_cent_accel(void);
extern void test_phy_rot_cent_accel2(void);
extern void test_phy_rot_total_accel(void);
extern void test_phy_rot_power(void);

extern void test_phy_sound_press_amp(void);
extern void test_phy_sound_level(void);
extern void test_phy_sound_intensity(void);
extern void test_phy_sound_shock_wave_angle(void);
extern void test_phy_sound_dop_effect(void);
extern void test_phy_sound_beat(void);

extern void test_phy_thermo_fahren_cels(void);
extern void test_phy_thermo_cels_kel(void);
extern void test_phy_thermo_heat_fusion(void);
extern void test_phy_thermo_heat(void);
extern void test_phy_thermo_expan(void);
extern void test_phy_thermo_mean_free_path(void);
extern void test_phy_thermo_effic_heat_engine(void);
extern void test_phy_thermo_carnot_engine(void);

extern void test_phy_work_module(void);
extern void test_phy_work_x(void);
extern void test_phy_work_y(void);
extern void test_phy_work_kin(void);

extern void test_sta_mean(void);
extern void test_sta_trim_mean(void);
extern void test_sta_weighted_mean(void);
extern void test_sta_time_weighted_mean(void);
extern void test_sta_demean(void);
extern void test_sta_percentile(void);
extern void test_sta_median(void);
extern void test_sta_weighted_median(void);
extern void test_sta_quartiles(void);
extern void test_sta_quart_range(void);
extern void test_sta_mode(void);
extern void test_sta_data_range(void);
extern void test_sta_mean_abs_dev(void);
extern void test_sta_median_abs_dev(void);
extern void test_sta_variance(void);
extern void test_sta_standard_deviation(void);
extern void test_sta_covariance(void);
extern void test_sta_covariance_matrix(void);
extern void test_sta_linear_regression(void);
extern void test_sta_absolute_error(void);
extern void test_sta_relative_error(void);
extern void test_sta_standard_error(void);

/* Test for functions that only work with double-precision floats. */
#ifndef CONFIG_ZSL_SINGLE_PRECISION
extern void test_sta_mult_linear_regression(void);
extern void test_sta_weighted_mult_linear_regression(void);
extern void test_sta_quadric_fitting(void);
#endif

extern void test_prob_uniform_pdf(void);
extern void test_prob_uniform_mean(void);
extern void test_prob_uniform_variance(void);
extern void test_prob_uniform_cdf(void);
extern void test_prob_normal_pdf(void);
extern void test_prob_normal_cdf(void);
extern void test_prob_erf_inverse(void);
extern void test_prob_normal_cdf_inv(void);
extern void test_prob_factorial(void);
extern void test_prob_binomial_coef(void);
extern void test_prob_binomial_pdf(void);
extern void test_prob_binomial_mean(void);
extern void test_prob_binomial_variance(void);
extern void test_prob_binomial_cdf(void);
extern void test_prob_poisson_pdf(void);
extern void test_prob_poisson_cdf(void);
extern void test_prob_entropy(void);
extern void test_prob_bayes(void);

extern void test_att_to_vec(void);
extern void test_att_to_euler(void);
extern void test_att_from_euler(void);
extern void test_att_from_accelmag(void);
extern void test_att_from_accel(void);
extern void test_att_accel_angle(void);

extern void test_comp_dms_to_dd(void);
extern void test_comp_magn_north(void);
extern void test_comp_geo_north(void);

extern void test_eul_to_vec(void);

extern void test_grav_lat_alt(void);

extern void test_quat_init(void);
extern void test_quat_magn(void);
extern void test_quat_to_unit(void);
extern void test_quat_is_unit(void);
extern void test_quat_scale(void);
extern void test_quat_mult(void);
extern void test_quat_exp(void);
extern void test_quat_log(void);
extern void test_quat_pow(void);
extern void test_quat_conj(void);
extern void test_quat_inv(void);
extern void test_quat_diff(void);
extern void test_quat_rot(void);
extern void test_quat_lerp(void);
extern void test_quat_slerp(void);
extern void test_quat_from_ang_vel(void);
extern void test_quat_from_ang_mom(void);
extern void test_quat_to_euler(void);
extern void test_quat_from_euler(void);
extern void test_quat_to_rot_mtx(void);
extern void test_quat_from_rot_mtx(void);
extern void test_quat_to_axis_angle(void);
extern void test_quat_from_axis_angle(void);

extern void test_fus_madgwick(void);
extern void test_fus_mahony(void);
extern void test_fus_saam(void);
extern void test_fus_complementary(void);
extern void test_fus_aqua(void);
extern void test_fus_kalman(void);

extern void test_fus_cal_rot_mtx(void);
extern void test_fus_cal_rot_axis_angle(void);
extern void test_fus_cal_corr_scalar(void);
extern void test_fus_cal_corr_vec(void);
/* extern void test_fus_cal_madg(void); */
/* extern void test_fus_cal_mahn(void); */

/* Test for functions that only work with double-precision floats. */
#ifndef CONFIG_ZSL_SINGLE_PRECISION
extern void test_fus_cal_magn(void);
extern void test_fus_cal_magn_fast(void);
#endif

void test_main(void)
{
	ztest_test_suite(zsl_tests,

			 ztest_unit_test(test_conv_spd_xyz),
			 ztest_unit_test(test_conv_ct_xyz),
			 ztest_unit_test(test_conv_ct_rgb8),
			 ztest_unit_test(test_conv_cct_xyy),
			 ztest_unit_test(test_conv_cct_xyz),
			 ztest_unit_test(test_conv_uv60_cct_ohno2011),
			 ztest_unit_test(test_conv_uv60_cct_ohno2014),

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
			 ztest_unit_test(test_matrix_mult_d),
			 ztest_unit_test(test_matrix_scalar_mult_d),
			 ztest_unit_test(test_matrix_scalar_mult_row_d),
			 ztest_unit_test(test_matrix_trans),
			 ztest_unit_test(test_matrix_adjoint_3x3),
			 ztest_unit_test(test_matrix_adjoint),
			 ztest_unit_test(test_matrix_reduce),
			 ztest_unit_test(test_matrix_reduce_iter),
			 ztest_unit_test(test_matrix_augm_diag),
			 ztest_unit_test(test_matrix_deter_3x3),
			 ztest_unit_test(test_matrix_deter),
			 ztest_unit_test(test_matrix_gauss_elim),
			 ztest_unit_test(test_matrix_gauss_elim_d),
			 ztest_unit_test(test_matrix_gauss_reduc),
			 ztest_unit_test(test_matrix_gram_schmidt_sq),
			 ztest_unit_test(test_matrix_gram_schmidt_rect),
			 ztest_unit_test(test_matrix_cols_norm),
			 ztest_unit_test(test_matrix_norm_elem),
			 ztest_unit_test(test_matrix_norm_elem_d),
			 ztest_unit_test(test_matrix_inv_3x3),
			 ztest_unit_test(test_matrix_inv),
			 ztest_unit_test(test_matrix_cholesky),
			 ztest_unit_test(test_matrix_balance),
			 ztest_unit_test(test_matrix_householder_sq),
			 ztest_unit_test(test_matrix_householder_rect),
			 ztest_unit_test(test_matrix_qrd),
			 ztest_unit_test(test_matrix_qrd_hess),
			 ztest_unit_test(test_matrix_min),
			 ztest_unit_test(test_matrix_max),
			 ztest_unit_test(test_matrix_min_idx),
			 ztest_unit_test(test_matrix_max_idx),
			 ztest_unit_test(test_matrix_is_equal),
			 ztest_unit_test(test_matrix_is_notneg),
			 ztest_unit_test(test_matrix_is_sym),

			 ztest_unit_test(test_vector_init),
			 ztest_unit_test(test_vector_from_arr),
			 ztest_unit_test(test_vector_copy),
			 ztest_unit_test(test_vector_get_subset),
			 ztest_unit_test(test_vector_add),
			 ztest_unit_test(test_vector_sub),
			 ztest_unit_test(test_vector_neg),
			 ztest_unit_test(test_vector_sum),
			 ztest_unit_test(test_vector_scalar_add),
			 ztest_unit_test(test_vector_scalar_mult),
			 ztest_unit_test(test_vector_scalar_div),
			 ztest_unit_test(test_vector_dist),
			 ztest_unit_test(test_vector_dot),
			 ztest_unit_test(test_vector_norm),
			 ztest_unit_test(test_vector_project),
			 ztest_unit_test(test_vector_to_unit),
			 ztest_unit_test(test_vector_cross),
			 ztest_unit_test(test_vector_sum_of_sqrs),
			 ztest_unit_test(test_vector_mean),
			 ztest_unit_test(test_vector_ar_mean),
			 ztest_unit_test(test_vector_rev),
			 ztest_unit_test(test_vector_zte),
			 ztest_unit_test(test_vector_is_equal),
			 ztest_unit_test(test_vector_is_nonneg),
			 ztest_unit_test(test_vector_contains),
			 ztest_unit_test(test_vector_sort),

			 ztest_unit_test(test_phy_atom_nucl_radius),
			 ztest_unit_test(test_phy_atom_bohr_orb_radius),
			 ztest_unit_test(test_phy_atom_bohr_orb_vel),
			 ztest_unit_test(test_phy_atom_bohr_orb_ener),
			 ztest_unit_test(test_phy_atom_rad_decay),
			 ztest_unit_test(test_phy_atom_bragg),

			 ztest_unit_test(test_phy_dyn_newton),
			 ztest_unit_test(test_phy_dyn_fric),
			 ztest_unit_test(test_phy_dyn_normal),
			 ztest_unit_test(test_phy_dyn_pendul_period),
			 ztest_unit_test(test_phy_dyn_pendul_max_speed),
			 ztest_unit_test(test_phy_dyn_tension),
			 ztest_unit_test(test_phy_dyn_lever),

			 ztest_unit_test(test_phy_ecmp_capac_cpv),
			 ztest_unit_test(test_phy_ecmp_capac_ad),
			 ztest_unit_test(test_phy_ecmp_ener_capac),
			 ztest_unit_test(test_phy_ecmp_ener_induc),
			 ztest_unit_test(test_phy_ecmp_trans),
			 ztest_unit_test(test_phy_ecmp_rlc_volt),
			 ztest_unit_test(test_phy_ecmp_rc_charg_i),
			 ztest_unit_test(test_phy_ecmp_rc_charg_q),
			 ztest_unit_test(test_phy_ecmp_rc_discharg_i),
			 ztest_unit_test(test_phy_ecmp_rc_discharg_q),
			 ztest_unit_test(test_phy_ecmp_rl_current),

			 ztest_unit_test(test_phy_elcty_current),
			 ztest_unit_test(test_phy_elcty_res_series),
			 ztest_unit_test(test_phy_elcty_res_parallel),
			 ztest_unit_test(test_phy_elcty_cap_series),
			 ztest_unit_test(test_phy_elcty_cap_parallel),
			 ztest_unit_test(test_phy_elcty_resistivity),
			 ztest_unit_test(test_phy_elcty_ohm_law),
			 ztest_unit_test(test_phy_elcty_power_vi),
			 ztest_unit_test(test_phy_elcty_power_ir),
			 ztest_unit_test(test_phy_elcty_power_vr),

			 ztest_unit_test(test_phy_elec_charge_dens),
			 ztest_unit_test(test_phy_elec_force),
			 ztest_unit_test(test_phy_elec_force2),
			 ztest_unit_test(test_phy_elec_field),
			 ztest_unit_test(test_phy_elec_pot_ener),
			 ztest_unit_test(test_phy_elec_potential),
			 ztest_unit_test(test_phy_elec_flux),

			 ztest_unit_test(test_phy_ener_kin),
			 ztest_unit_test(test_phy_ener_rot),
			 ztest_unit_test(test_phy_ener_grav_pot),
			 ztest_unit_test(test_phy_ener_elas_pot),
			 ztest_unit_test(test_phy_ener_power),
			 ztest_unit_test(test_phy_ener_fric),
			 ztest_unit_test(test_phy_ener_mec),
			 ztest_unit_test(test_phy_ener_final),
			 ztest_unit_test(test_phy_ener_photon),
			 ztest_unit_test(test_phy_ener_photon2),

			 ztest_unit_test(test_phy_fluid_dens),
			 ztest_unit_test(test_phy_fluid_simple_press),
			 ztest_unit_test(test_phy_fluid_press),
			 ztest_unit_test(test_phy_fluid_bouyant_force),
			 ztest_unit_test(test_phy_fluid_flow_rate),
			 ztest_unit_test(test_phy_fluid_force_prop),
			 ztest_unit_test(test_phy_fluid_bernoulli),
			 ztest_unit_test(test_phy_fluid_volume_flow_rate),

			 ztest_unit_test(test_phy_gas_av_vel),
			 ztest_unit_test(test_phy_gas_press),
			 ztest_unit_test(test_phy_gas_boyle),
			 ztest_unit_test(test_phy_gas_charles_lussac),

			 ztest_unit_test(test_phy_grav_orb_period),
			 ztest_unit_test(test_phy_grav_esc_vel),
			 ztest_unit_test(test_phy_grav_acc),
			 ztest_unit_test(test_phy_grav_orb_vel),
			 ztest_unit_test(test_phy_grav_force),
			 ztest_unit_test(test_phy_grav_pot_ener),

			 ztest_unit_test(test_phy_kin_dist),
			 ztest_unit_test(test_phy_kin_init_pos),
			 ztest_unit_test(test_phy_kin_init_pos2),
			 ztest_unit_test(test_phy_kin_time),
			 ztest_unit_test(test_phy_kin_vel),
			 ztest_unit_test(test_phy_kin_vel2),
			 ztest_unit_test(test_phy_kin_init_vel),
			 ztest_unit_test(test_phy_kin_init_vel2),
			 ztest_unit_test(test_phy_kin_init_vel3),
			 ztest_unit_test(test_phy_kin_av_vel),
			 ztest_unit_test(test_phy_kin_accel),
			 ztest_unit_test(test_phy_kin_ener),

			 ztest_unit_test(test_phy_magn_force),
			 ztest_unit_test(test_phy_magn_force_wire),
			 ztest_unit_test(test_phy_magn_torque_loop),
			 ztest_unit_test(test_phy_magn_pot_dipole),
			 ztest_unit_test(test_phy_magn_orb_radius),
			 ztest_unit_test(test_phy_magn_flux),
			 ztest_unit_test(test_phy_magn_mom),

			 ztest_unit_test(test_phy_mass_center),

			 ztest_unit_test(test_phy_mom_mom),
			 ztest_unit_test(test_phy_mom_imp),
			 ztest_unit_test(test_phy_mom_change),
			 ztest_unit_test(test_phy_mom_elas),
			 ztest_unit_test(test_phy_mom_inelas),

			 ztest_unit_test(test_phy_opt_refrac_index),
			 ztest_unit_test(test_phy_opt_snell),
			 ztest_unit_test(test_phy_opt_focus),
			 ztest_unit_test(test_phy_opt_critic_angle),
			 ztest_unit_test(test_phy_opt_power),
			 ztest_unit_test(test_phy_opt_magn),
			 ztest_unit_test(test_phy_opt_dif),

			 ztest_unit_test(test_phy_photon_ener),
			 ztest_unit_test(test_phy_photon_mom),
			 ztest_unit_test(test_phy_photon_wavelenght),
			 ztest_unit_test(test_phy_photon_frequency),
			 ztest_unit_test(test_phy_photon_photoelectric),

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

			 ztest_unit_test(test_phy_rot_angle),
			 ztest_unit_test(test_phy_rot_dist),
			 ztest_unit_test(test_phy_rot_turn),
			 ztest_unit_test(test_phy_rot_time),
			 ztest_unit_test(test_phy_rot_omega),
			 ztest_unit_test(test_phy_rot_omega2),
			 ztest_unit_test(test_phy_rot_av_omega),
			 ztest_unit_test(test_phy_rot_vel),
			 ztest_unit_test(test_phy_rot_ang_accel),
			 ztest_unit_test(test_phy_rot_accel),
			 ztest_unit_test(test_phy_rot_ener),
			 ztest_unit_test(test_phy_rot_period),
			 ztest_unit_test(test_phy_rot_frequency),
			 ztest_unit_test(test_phy_rot_cent_accel),
			 ztest_unit_test(test_phy_rot_cent_accel2),
			 ztest_unit_test(test_phy_rot_total_accel),
			 ztest_unit_test(test_phy_rot_power),

			 ztest_unit_test(test_phy_sound_press_amp),
			 ztest_unit_test(test_phy_sound_level),
			 ztest_unit_test(test_phy_sound_intensity),
			 ztest_unit_test(test_phy_sound_shock_wave_angle),
			 ztest_unit_test(test_phy_sound_dop_effect),
			 ztest_unit_test(test_phy_sound_beat),

			 ztest_unit_test(test_phy_thermo_fahren_cels),
			 ztest_unit_test(test_phy_thermo_cels_kel),
			 ztest_unit_test(test_phy_thermo_heat_fusion),
			 ztest_unit_test(test_phy_thermo_heat),
			 ztest_unit_test(test_phy_thermo_expan),
			 ztest_unit_test(test_phy_thermo_mean_free_path),
			 ztest_unit_test(test_phy_thermo_effic_heat_engine),
			 ztest_unit_test(test_phy_thermo_carnot_engine),

			 ztest_unit_test(test_phy_work_module),
			 ztest_unit_test(test_phy_work_x),
			 ztest_unit_test(test_phy_work_y),
			 ztest_unit_test(test_phy_work_kin),

			 ztest_unit_test(test_sta_mean),
			 ztest_unit_test(test_sta_trim_mean),
			 ztest_unit_test(test_sta_weighted_mean),
			 ztest_unit_test(test_sta_time_weighted_mean),
			 ztest_unit_test(test_sta_demean),
			 ztest_unit_test(test_sta_percentile),
			 ztest_unit_test(test_sta_median),
			 ztest_unit_test(test_sta_weighted_median),
			 ztest_unit_test(test_sta_quartiles),
			 ztest_unit_test(test_sta_quart_range),
			 ztest_unit_test(test_sta_mode),
			 ztest_unit_test(test_sta_data_range),
			 ztest_unit_test(test_sta_mean_abs_dev),
			 ztest_unit_test(test_sta_median_abs_dev),
			 ztest_unit_test(test_sta_variance),
			 ztest_unit_test(test_sta_standard_deviation),
			 ztest_unit_test(test_sta_covariance),
			 ztest_unit_test(test_sta_covariance_matrix),
			 ztest_unit_test(test_sta_linear_regression),
			 ztest_unit_test(test_sta_absolute_error),
			 ztest_unit_test(test_sta_relative_error),
			 ztest_unit_test(test_sta_standard_error),

			 ztest_unit_test(test_prob_uniform_pdf),
			 ztest_unit_test(test_prob_uniform_mean),
			 ztest_unit_test(test_prob_uniform_variance),
			 ztest_unit_test(test_prob_uniform_cdf),
			 ztest_unit_test(test_prob_normal_pdf),
			 ztest_unit_test(test_prob_normal_cdf),
			 ztest_unit_test(test_prob_erf_inverse),
			 ztest_unit_test(test_prob_normal_cdf_inv),
			 ztest_unit_test(test_prob_factorial),
			 ztest_unit_test(test_prob_binomial_coef),
			 ztest_unit_test(test_prob_binomial_pdf),
			 ztest_unit_test(test_prob_binomial_mean),
			 ztest_unit_test(test_prob_binomial_variance),
			 ztest_unit_test(test_prob_binomial_cdf),
			 ztest_unit_test(test_prob_entropy),
			 ztest_unit_test(test_prob_bayes),

			 ztest_unit_test(test_att_to_vec),
			 ztest_unit_test(test_att_to_euler),
			 ztest_unit_test(test_att_from_euler),
			 ztest_unit_test(test_att_from_accelmag),
			 ztest_unit_test(test_att_from_accel),
			 ztest_unit_test(test_att_accel_angle),

			 ztest_unit_test(test_eul_to_vec),

			 ztest_unit_test(test_grav_lat_alt),

			 ztest_unit_test(test_quat_init),
			 ztest_unit_test(test_quat_magn),
			 ztest_unit_test(test_quat_to_unit),
			 ztest_unit_test(test_quat_is_unit),
			 ztest_unit_test(test_quat_scale),
			 ztest_unit_test(test_quat_mult),
			 ztest_unit_test(test_quat_exp),
			 ztest_unit_test(test_quat_log),
			 ztest_unit_test(test_quat_pow),
			 ztest_unit_test(test_quat_conj),
			 ztest_unit_test(test_quat_inv),
			 ztest_unit_test(test_quat_diff),
			 ztest_unit_test(test_quat_rot),
			 ztest_unit_test(test_quat_lerp),
			 ztest_unit_test(test_quat_slerp),
			 ztest_unit_test(test_quat_from_ang_vel),
			 ztest_unit_test(test_quat_from_ang_mom),
			 ztest_unit_test(test_quat_to_euler),
			 ztest_unit_test(test_quat_from_euler),
			 ztest_unit_test(test_quat_to_rot_mtx),
			 ztest_unit_test(test_quat_from_rot_mtx),
			 ztest_unit_test(test_quat_to_axis_angle),
			 ztest_unit_test(test_quat_from_axis_angle),

			 ztest_unit_test(test_fus_madgwick),
			 ztest_unit_test(test_fus_mahony),
			 ztest_unit_test(test_fus_saam),
			 ztest_unit_test(test_fus_complementary),
			 ztest_unit_test(test_fus_aqua),
			 ztest_unit_test(test_fus_kalman),

			 ztest_unit_test(test_fus_cal_rot_mtx),
			 ztest_unit_test(test_fus_cal_rot_axis_angle),
			 ztest_unit_test(test_fus_cal_corr_scalar),
			 ztest_unit_test(test_fus_cal_corr_vec)
			 /* ztest_unit_test(test_fus_cal_madg), */
			 /* ztest_unit_test(test_fus_cal_mahn) */
			 );

	ztest_run_test_suite(zsl_tests);

	/* Run test on functions that only exist w/double-precision floats. */
	#ifndef CONFIG_ZSL_SINGLE_PRECISION
	ztest_test_suite(zsl_tests_double,
			 ztest_unit_test(test_matrix_vector_wedge),
			 ztest_unit_test(test_matrix_qrd_iter),
			 ztest_unit_test(test_matrix_eigenvalues),
			 ztest_unit_test(test_matrix_eigenvectors),
			 ztest_unit_test(test_matrix_svd),
			 ztest_unit_test(test_matrix_pinv),

			 ztest_unit_test(test_sta_mult_linear_regression),
			 ztest_unit_test(test_sta_weighted_mult_linear_regression),
			 ztest_unit_test(test_sta_quadric_fitting),

			 ztest_unit_test(test_fus_cal_magn),
			 ztest_unit_test(test_fus_cal_magn_fast)
			 );

	ztest_run_test_suite(zsl_tests_double);
	#endif
}
