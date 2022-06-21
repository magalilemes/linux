// SPDX-License-Identifier: MIT
/*
 * KUnit tests for dml/calcs/bw_fixed.h
 *
 * Copyright (C) 2022, Magali Lemes <magalilemes00@gmail.com>
 */

#include <kunit/test.h>
#include "../../../dc/dml/dcn20/dcn20_fpu.h"

/**
 * DOC: Unit tests for AMDGPU DML dcn20/dcn20_fpu.h
 *
 * --TO WRITE
 *
 */

void _vcs_dpi_soc_bounding_box_st_initialize(struct _vcs_dpi_soc_bounding_box_st *loaded_bb) {

	loaded_bb->num_states = 4;
	loaded_bb->clock_limits[0] = (struct _vcs_dpi_voltage_scaling_st){
			.dcfclk_mhz = 506.0,
			.fabricclk_mhz = 506.0,
			.dispclk_mhz = 1284.0,
			.dppclk_mhz = 400.00,
			.phyclk_mhz = 810.0,
			.socclk_mhz = 506.0,
			.dscclk_mhz = 428.0,
			.dram_speed_mts = 1600.0,
		};
	loaded_bb->clock_limits[1] = (struct _vcs_dpi_voltage_scaling_st){
			.dcfclk_mhz = 540.0,
			.fabricclk_mhz = 540.0,
			.dispclk_mhz = 1284.0,
			.dppclk_mhz = 1284.0,
			.phyclk_mhz = 810.0,
			.socclk_mhz = 540.0,
			.dscclk_mhz = 428.0,
			.dram_speed_mts = 8000.0,
		};
	loaded_bb->clock_limits[2] = (struct _vcs_dpi_voltage_scaling_st){
			.dcfclk_mhz = 675.0,
			.fabricclk_mhz = 675.0,
			.dispclk_mhz = 1284.0,
			.dppclk_mhz = 1284.0,
			.phyclk_mhz = 810.0,
			.socclk_mhz = 675.0,
			.dscclk_mhz = 428.0,
			.dram_speed_mts = 10000.0,
		};
	loaded_bb->clock_limits[3] = (struct _vcs_dpi_voltage_scaling_st){
			.dcfclk_mhz = 1265.0,
			.fabricclk_mhz = 1266.0,
			.dispclk_mhz = 1284.0,
			.dppclk_mhz = 1284.0,
			.phyclk_mhz = 810.0,
			.socclk_mhz = 1266.0,
			.dscclk_mhz = 428.0,
			.dram_speed_mts = 15000.0,
		};
}

void pp_smu_nv_clock_table_initialize(struct pp_smu_nv_clock_table *max_clocks) {
	max_clocks->dcfClockInKhz = 1265000;
	max_clocks->uClockInKhz = 875000;
	max_clocks->fabricClockInKhz = 0;
	max_clocks->displayClockInKhz = 1284000;
	max_clocks->dppClockInKhz = 0;
	max_clocks->phyClockInKhz = 810000;
	max_clocks->socClockInKhz = 1266000;
	max_clocks->dscClockInKhz = 0;
}

/*
void vba_vars_st_initialize(vba_vars_st *v) {
	int i, j, k;

	for (i = 0; i < 8; i++)
		for (j = 0; j < 2; j++)
			for (k = 0; k < 9; k++)
				v->RequiredDPPCLK[i][j][k] = 1.0;
}
*/

/* TODO: mock get_wm_writeback_urgent and get_wm_writeback_dram_clock_change
 https://kunit.dev/third_party/stable_kernel/docs/usage.html#mocking-arbitrary-functions*/
// static void dcn20_fpu_set_wb_arb_params_test(struct kunit *test);

static void dcn20_cap_soc_clocks_test(struct kunit *test)
{
	struct _vcs_dpi_soc_bounding_box_st bb;
	struct pp_smu_nv_clock_table max_clocks;
	int i;

	_vcs_dpi_soc_bounding_box_st_initialize(&bb);
	pp_smu_nv_clock_table_initialize(&max_clocks);

	DC_FP_START();
	dcn20_cap_soc_clocks(&bb, max_clocks);
	DC_FP_END();

	/* Check if no clock limit is higher than the specified maximum */
	for (i = 0; i < bb.num_states; i++) {
		KUNIT_EXPECT_LE(test, (int)bb.clock_limits[i].dcfclk_mhz, max_clocks.dcfClockInKhz / 1000);
		KUNIT_EXPECT_LE(test, (int)bb.clock_limits[i].dram_speed_mts, (max_clocks.uClockInKhz / 1000) * 16);
		KUNIT_EXPECT_LE(test, (int)bb.clock_limits[i].dispclk_mhz, max_clocks.displayClockInKhz / 1000);
		KUNIT_EXPECT_LE(test, (int)bb.clock_limits[i].phyclk_mhz, max_clocks.phyClockInKhz / 1000);
		KUNIT_EXPECT_LE(test, (int)bb.clock_limits[i].socclk_mhz, max_clocks.socClockInKhz / 1000);
	}

	/* There is no duplicate clock state */
	KUNIT_EXPECT_EQ(test, 4, bb.num_states);

	/* One duplicate clock state */
	bb.clock_limits[1].dcfclk_mhz = 675.0;
	bb.clock_limits[1].fabricclk_mhz = 675.0;
	bb.clock_limits[1].socclk_mhz = 675.0;
	bb.clock_limits[1].dram_speed_mts = 10000.0;

	DC_FP_START();
	dcn20_cap_soc_clocks(&bb, max_clocks);
	DC_FP_START();

	KUNIT_EXPECT_EQ(test, 3, bb.num_states);
}

/*
static void dcn20_fpu_set_wm_ranges_test(struct kunit *test)
{
	int i;
	struct pp_smu_wm_range_sets ranges = {0};
	struct _vcs_dpi_soc_bounding_box_st *loaded_bb;

	_vcs_dpi_soc_bounding_box_st_initialize(loaded_bb);

	for (i = 0; i < 3 && i; i++)
		dcn20_fpu_set_wm_ranges(i, &ranges, loaded_bb);

	KUNIT_EXPECT_EQ(test, 5.0, ranges->reader_wm_sets[0].min_fill_clk_mhz);
	KUNIT_EXPECT_EQ(test, 5.0, ranges->reader_wm_sets[0].max_fill_clk_mhz);

	KUNIT_EXPECT_EQ(test, 5.0, ranges->reader_wm_sets[1].min_fill_clk_mhz);
	KUNIT_EXPECT_EQ(test, 5.0, ranges->reader_wm_sets[1].max_fill_clk_mhz);

	KUNIT_EXPECT_EQ(test, 5.0, ranges->reader_wm_sets[2].min_fill_clk_mhz);
	KUNIT_EXPECT_EQ(test, 5.0, ranges->reader_wm_sets[2].max_fill_clk_mhz);
}

static void dcn20_fpu_adjust_dppclk_test(struct kunit *test)
{
	vba_vars_st v;
	int vlevel, max_mpc_comb, pipe_idx;
	bool is_validating_bw;

	is_validating_bw = true;
	vlevel = 0;
	max_mpc_comb = 0;
	pipe_idx = 0;
	dcn20_fpu_adjust_dppclk(&v, vlevel, max_mpc_comb, pipe_idx, is_validating_bw);
	KUNIT_EXPECT_EQ(test, 5.0, v[vlevel][max_mpc_comb][pipe_idx]);

	vlevel = 3;
	dcn20_fpu_adjust_dppclk(&v, vlevel, max_mpc_comb, pipe_idx, is_validating_bw);
	KUNIT_EXPECT_EQ(test, 5.0, v[vlevel][max_mpc_comb][pipe_idx]);

	max_mpc_comb = 1;
	dcn20_fpu_adjust_dppclk(&v, vlevel, max_mpc_comb, pipe_idx, is_validating_bw);
	KUNIT_EXPECT_EQ(test, 5.0, v[vlevel][max_mpc_comb][pipe_idx]);

	pipe_idx = 5;
	dcn20_fpu_adjust_dppclk(&v, vlevel, max_mpc_comb, pipe_idx, is_validating_bw);
	KUNIT_EXPECT_EQ(test, 5.0, v[vlevel][max_mpc_comb][pipe_idx]);

	vlevel = 8;
	dcn20_fpu_adjust_dppclk(&v, vlevel, max_mpc_comb, pipe_idx, is_validating_bw);
	KUNIT_EXPECT_EQ(test, 5.0, v[vlevel][max_mpc_comb][pipe_idx]);

	max_mpc_comb = 2;
	dcn20_fpu_adjust_dppclk(&v, vlevel, max_mpc_comb, pipe_idx, is_validating_bw);
	KUNIT_EXPECT_EQ(test, 5.0, v[vlevel][max_mpc_comb][pipe_idx]);

	pipe_idx = 9;
	dcn20_fpu_adjust_dppclk(&v, vlevel, max_mpc_comb, pipe_idx, is_validating_bw);
	KUNIT_EXPECT_EQ(test, 5.0, v[vlevel][max_mpc_comb][pipe_idx]);

	is_validating_bw = false;
	vlevel = 0;
	max_mpc_comb = 0;
	pipe_idx = 0;
	dcn20_fpu_adjust_dppclk(&v, vlevel, max_mpc_comb, pipe_idx, is_validating_bw);
	KUNIT_EXPECT_EQ(test, 5.0, v[vlevel][max_mpc_comb][pipe_idx]);

	vlevel = 3;
	dcn20_fpu_adjust_dppclk(&v, vlevel, max_mpc_comb, pipe_idx, is_validating_bw);
	KUNIT_EXPECT_EQ(test, 5.0, v[vlevel][max_mpc_comb][pipe_idx]);

	max_mpc_comb = 1;
	dcn20_fpu_adjust_dppclk(&v, vlevel, max_mpc_comb, pipe_idx, is_validating_bw);
	KUNIT_EXPECT_EQ(test, 5.0, v[vlevel][max_mpc_comb][pipe_idx]);

	pipe_idx = 5;
	dcn20_fpu_adjust_dppclk(&v, vlevel, max_mpc_comb, pipe_idx, is_validating_bw);
	KUNIT_EXPECT_EQ(test, 5.0, v[vlevel][max_mpc_comb][pipe_idx]);

	vlevel = 8;
	dcn20_fpu_adjust_dppclk(&v, vlevel, max_mpc_comb, pipe_idx, is_validating_bw);
	KUNIT_EXPECT_EQ(test, 5.0, v[vlevel][max_mpc_comb][pipe_idx]);

	max_mpc_comb = 2;
	dcn20_fpu_adjust_dppclk(&v, vlevel, max_mpc_comb, pipe_idx, is_validating_bw);
	KUNIT_EXPECT_EQ(test, 5.0, v[vlevel][max_mpc_comb][pipe_idx]);

	pipe_idx = 9;
	dcn20_fpu_adjust_dppclk(&v, vlevel, max_mpc_comb, pipe_idx, is_validating_bw);
	KUNIT_EXPECT_EQ(test, 5.0, v[vlevel][max_mpc_comb][pipe_idx]);
}*/

static struct kunit_case dcn20_fpu_test_cases[] = {
	// KUNIT_CASE(dcn20_fpu_set_wb_arb_params_test),
	KUNIT_CASE(dcn20_cap_soc_clocks_test),
	// KUNIT_CASE(dcn20_fpu_set_wm_ranges_test),
	// KUNIT_CASE(dcn20_fpu_adjust_dppclk_test),
	{  }
};

static struct kunit_suite dcn20_fpu_test_suite = {
	.name = "dml_dcn20_fpu",
	.test_cases = dcn20_fpu_test_cases,
};

kunit_test_suite(dcn20_fpu_test_suite);
