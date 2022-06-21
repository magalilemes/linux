// SPDX-License-Identifier: MIT
/*
 * KUnit tests for dml/dcn20/dcn20_fpu.h
 *
 * Copyright (C) 2022, Magali Lemes <magalilemes00@gmail.com>
 */

#include <kunit/test.h>
#include "../../../../dc/inc/resource.h"
#include "dcn21/dcn21_resource.h"
#include "../../../../dc/dml/dcn20/dcn20_fpu.h"
#include "clk_mgr.h"

/**
 * DOC: Unit tests for AMDGPU DML dcn20/dcn20_fpu.h
 */

struct dcn20_cap_soc_clocks_test_case {
	const char *desc;
	struct _vcs_dpi_soc_bounding_box_st bb;
	struct pp_smu_nv_clock_table max_clocks;
	const int clock_states;
	const struct _vcs_dpi_voltage_scaling_st expected_clock_limits[DC__VOLTAGE_STATES];
};

struct dcn21_update_bw_bounding_box_test_case {
	const char *desc;
	struct dc dc;
	struct clk_bw_params bw_params;
	const int clocks_to_compare;
	const struct _vcs_dpi_voltage_scaling_st expected_clock_limits[DC__VOLTAGE_STATES];
};

struct dcn20_cap_soc_clocks_test_case dcn20_cap_soc_clocks_test_cases[] = {
	{
		.desc = "4-state bounding box clock limits ",
		.bb = {
			.clock_limits = {
				{
					.dcfclk_mhz = 506.0,
					.fabricclk_mhz = 506.0,
					.dispclk_mhz = 1284.0,
					.dppclk_mhz = 400.0,
					.phyclk_mhz = 810.0,
					.socclk_mhz = 506.0,
					.dscclk_mhz = 428.0,
					.dram_speed_mts = 1600.0,
				},
				{
					.dcfclk_mhz = 540.0,
					.fabricclk_mhz = 540.0,
					.dispclk_mhz = 1284.0,
					.dppclk_mhz = 1284.0,
					.phyclk_mhz = 810.0,
					.socclk_mhz = 540.0,
					.dscclk_mhz = 428.0,
					.dram_speed_mts = 8000.0,
				},
				{
					.dcfclk_mhz = 675.0,
					.fabricclk_mhz = 675.0,
					.dispclk_mhz = 1284.0,
					.dppclk_mhz = 1284.0,
					.phyclk_mhz = 810.0,
					.socclk_mhz = 675.0,
					.dscclk_mhz = 428.0,
					.dram_speed_mts = 10000.0,
				},
				{
					.dcfclk_mhz = 1265.0,
					.fabricclk_mhz = 1266.0,
					.dispclk_mhz = 1284.0,
					.dppclk_mhz = 1284.0,
					.phyclk_mhz = 810.0,
					.socclk_mhz = 1266.0,
					.dscclk_mhz = 428.0,
					.dram_speed_mts = 15000.0,
				},
			},
			.num_states = 4,
		},
		.max_clocks = {
			.dcfClockInKhz = 1265000,
			.uClockInKhz = 875000,
			.fabricClockInKhz = 0,
			.displayClockInKhz = 1284000,
			.dppClockInKhz = 0,
			.phyClockInKhz = 810000,
			.socClockInKhz = 1266000,
			.dscClockInKhz = 0,
		},
		.clock_states = 4,
		.expected_clock_limits = {
			{
				.dcfclk_mhz = 506.0,
				.fabricclk_mhz = 506.0,
				.dispclk_mhz = 1284.0,
				.dppclk_mhz = 400.0,
				.phyclk_mhz = 810.0,
				.socclk_mhz = 506.0,
				.dscclk_mhz = 428.0,
				.dram_speed_mts = 1600.0,
			},
			{
				.dcfclk_mhz = 540.0,
				.fabricclk_mhz = 540.0,
				.dispclk_mhz = 1284.0,
				.dppclk_mhz = 1284.0,
				.phyclk_mhz = 810.0,
				.socclk_mhz = 540.0,
				.dscclk_mhz = 428.0,
				.dram_speed_mts = 8000.0,
			},
			{
				.dcfclk_mhz = 675.0,
				.fabricclk_mhz = 675.0,
				.dispclk_mhz = 1284.0,
				.dppclk_mhz = 1284.0,
				.phyclk_mhz = 810.0,
				.socclk_mhz = 675.0,
				.dscclk_mhz = 428.0,
				.dram_speed_mts = 10000.0,
			},
			{
				.dcfclk_mhz = 1265.0,
				.fabricclk_mhz = 1266.0,
				.dispclk_mhz = 1284.0,
				.dppclk_mhz = 1284.0,
				.phyclk_mhz = 810.0,
				.socclk_mhz = 1266.0,
				.dscclk_mhz = 428.0,
				.dram_speed_mts = 14000.0,
			},
		},
	},
	{
		.desc = "One duplicate clock state",
		.bb = {
			.clock_limits = {
				{
					.dcfclk_mhz = 506.0,
					.fabricclk_mhz = 506.0,
					.dispclk_mhz = 1284.0,
					.dppclk_mhz = 400.0,
					.phyclk_mhz = 810.0,
					.socclk_mhz = 506.0,
					.dscclk_mhz = 428.0,
					.dram_speed_mts = 1600.0,
				},
				{
					.dcfclk_mhz = 675.0,
					.fabricclk_mhz = 675.0,
					.dispclk_mhz = 1284.0,
					.dppclk_mhz = 1284.0,
					.phyclk_mhz = 810.0,
					.socclk_mhz = 675.0,
					.dscclk_mhz = 428.0,
					.dram_speed_mts = 10000.0,
				},
				{
					.dcfclk_mhz = 675.0,
					.fabricclk_mhz = 675.0,
					.dispclk_mhz = 1284.0,
					.dppclk_mhz = 1284.0,
					.phyclk_mhz = 810.0,
					.socclk_mhz = 675.0,
					.dscclk_mhz = 428.0,
					.dram_speed_mts = 10000.0,
				},
				{
					.dcfclk_mhz = 1265.0,
					.fabricclk_mhz = 1266.0,
					.dispclk_mhz = 1284.0,
					.dppclk_mhz = 1284.0,
					.phyclk_mhz = 810.0,
					.socclk_mhz = 1266.0,
					.dscclk_mhz = 428.0,
					.dram_speed_mts = 15000.0,
				},
			},
			.num_states = 4,
		},
		.max_clocks = {
			.dcfClockInKhz = 1265000,
			.uClockInKhz = 875000,
			.fabricClockInKhz = 0,
			.displayClockInKhz = 1284000,
			.dppClockInKhz = 0,
			.phyClockInKhz = 810000,
			.socClockInKhz = 1266000,
			.dscClockInKhz = 0,
		},
		.clock_states = 3,
		.expected_clock_limits = {
			{
				.dcfclk_mhz = 506.0,
				.fabricclk_mhz = 506.0,
				.dispclk_mhz = 1284.0,
				.dppclk_mhz = 400.0,
				.phyclk_mhz = 810.0,
				.socclk_mhz = 506.0,
				.dscclk_mhz = 428.0,
				.dram_speed_mts = 1600.0,
			},
			{
				.dcfclk_mhz = 675.0,
				.fabricclk_mhz = 675.0,
				.dispclk_mhz = 1284.0,
				.dppclk_mhz = 1284.0,
				.phyclk_mhz = 810.0,
				.socclk_mhz = 675.0,
				.dscclk_mhz = 428.0,
				.dram_speed_mts = 10000.0,
			},
			{
				.dcfclk_mhz = 675.0,
				.fabricclk_mhz = 675.0,
				.dispclk_mhz = 1284.0,
				.dppclk_mhz = 1284.0,
				.phyclk_mhz = 810.0,
				.socclk_mhz = 675.0,
				.dscclk_mhz = 428.0,
				.dram_speed_mts = 10000.0,
			},
		},
	},
	{
		.desc = "Zeroed max clocks",
		.bb = {
			.clock_limits = {
				{
					.dcfclk_mhz = 506.0,
					.fabricclk_mhz = 506.0,
					.dispclk_mhz = 1284.0,
					.dppclk_mhz = 400.0,
					.phyclk_mhz = 810.0,
					.socclk_mhz = 506.0,
					.dscclk_mhz = 428.0,
					.dram_speed_mts = 1600.0,
				},
			},
			.num_states = 1,
		},
		.max_clocks = {
			.dcfClockInKhz = 0,
			.uClockInKhz = 0,
			.fabricClockInKhz = 0,
			.displayClockInKhz = 0,
			.dppClockInKhz = 0,
			.phyClockInKhz = 0,
			.socClockInKhz = 0,
			.dscClockInKhz = 0,
		},
		.clock_states = 1,
		.expected_clock_limits = {
			{
				.dcfclk_mhz = 506.0,
				.fabricclk_mhz = 506.0,
				.dispclk_mhz = 1284.0,
				.dppclk_mhz = 400.0,
				.phyclk_mhz = 810.0,
				.socclk_mhz = 506.0,
				.dscclk_mhz = 428.0,
				.dram_speed_mts = 1600.0,
			},
		},
	},
};

struct dcn21_update_bw_bounding_box_test_case dcn21_update_bw_bounding_box_test_data[] = {
	{
		.desc = "5-entry bounding box clocks table",
		.dc = {
			.res_pool = &(struct resource_pool) {
				.pipe_count = 2,
				.res_cap = &(struct resource_caps) {
					.num_timing_generator = 3
				},
			},
		},
		.bw_params = {
			.num_channels = 1,
			.clk_table = {
				.entries = {
					{
						.voltage = 0,
						.dcfclk_mhz = 200,
						.fclk_mhz = 400,
						.memclk_mhz = 800,
						.socclk_mhz = 0,
					},
					{
						.voltage = 0,
						.dcfclk_mhz = 201,
						.fclk_mhz = 800,
						.memclk_mhz = 1600,
						.socclk_mhz = 0,
					},
					{
						.voltage = 0,
						.dcfclk_mhz = 553,
						.fclk_mhz = 1067,
						.memclk_mhz = 1067,
						.socclk_mhz = 0,
					},
					{
						.voltage = 0,
						.dcfclk_mhz = 602,
						.fclk_mhz = 1333,
						.memclk_mhz = 1600,
						.socclk_mhz = 0,
					},
					{
						.voltage = 0,
						.dispclk_mhz = 1372,
						.dppclk_mhz = 1372,
						.phyclk_mhz = 810,
						.phyclk_d18_mhz = 667,
						.dtbclk_mhz = 600,
					},
				},

				.num_entries = 5,
			},
		},
		.expected_clock_limits = {
			{
				.state = 0,
				.dcfclk_mhz = 200.0,
				.fabricclk_mhz = 400.0,
				.dispclk_mhz = 600.0,
				.dppclk_mhz = 400.0,
				.phyclk_mhz = 600.0,
				.socclk_mhz = 0.0,
				.dscclk_mhz = 205.67,
				.dram_speed_mts = 1600.0,
			},
			{
				.state = 1,
				.dcfclk_mhz = 200.0,
				.fabricclk_mhz = 400.0,
				.dispclk_mhz = 600.0,
				.dppclk_mhz = 400.0,
				.phyclk_mhz = 600.0,
				.socclk_mhz = 0.0,
				.dscclk_mhz = 205.67,
				.dram_speed_mts = 1600.0,
			},
			{
				.state = 2,
				.dcfclk_mhz = 201,
				.fabricclk_mhz = 800.0,
				.dispclk_mhz = 600.0,
				.dppclk_mhz = 400.0,
				.phyclk_mhz = 600.0,
				.socclk_mhz = 0.0,
				.dscclk_mhz = 205.67,
				.dram_speed_mts = 3200.0,
			},
			{
				.state = 3,
				.dcfclk_mhz = 553.0,
				.fabricclk_mhz = 1067.0,
				.dispclk_mhz = 757.89,
				.dppclk_mhz = 685.71,
				.phyclk_mhz = 600.0,
				.socclk_mhz = 0.0,
				.dscclk_mhz = 287.67,
				.dram_speed_mts = 2134.0,
			},
			{
				.state = 4,
				.dcfclk_mhz = 602.0,
				.fabricclk_mhz = 1333.0,
				.dispclk_mhz = 847.06,
				.dppclk_mhz = 757.89,
				.phyclk_mhz = 600.0,
				.socclk_mhz = 0.0,
				.dscclk_mhz = 318.334,
				.dram_speed_mts = 3200.0,
			},
			{
				.state = 5,
				.dcfclk_mhz = 0.0,
				.fabricclk_mhz = 0.0,
				.dispclk_mhz = 600.0,
				.dppclk_mhz = 400.0,
				.phyclk_mhz = 600.0,
				.socclk_mhz = 0.0,
				.dscclk_mhz = 205.67,
				.dram_speed_mts = 0.0,
			},
			{
				.state = 6,
				.dcfclk_mhz = 0.0,
				.fabricclk_mhz = 0.0,
				.dispclk_mhz = 600.0,
				.dppclk_mhz = 400.0,
				.phyclk_mhz = 600.0,
				.socclk_mhz = 0.0,
				.dscclk_mhz = 205.67,
				.dram_speed_mts = 0.0,
			},
		},
		.clocks_to_compare = 6,
	},
};

/**
 * dcn20_cap_soc_clocks_test - KUnit test for dcn20_cap_soc_clocks
 * @test: represents a running instance of a test.
 */
static void dcn20_cap_soc_clocks_test(struct kunit *test)
{
	struct dcn20_cap_soc_clocks_test_case *test_param =
		(struct dcn20_cap_soc_clocks_test_case *)test->param_value;
	int i;

	DC_FP_START();
	dcn20_cap_soc_clocks(&test_param->bb, test_param->max_clocks);
	DC_FP_END();

	/* Check if no clock limit is higher than the specified maximum */
	for (i = 0; i < test_param->bb.num_states; i++) {
		KUNIT_EXPECT_EQ(test, test_param->bb.clock_limits[i].dcfclk_mhz,
				test_param->expected_clock_limits[i].dcfclk_mhz);
		KUNIT_EXPECT_EQ(test, test_param->bb.clock_limits[i].fabricclk_mhz,
				test_param->expected_clock_limits[i].fabricclk_mhz);
		KUNIT_EXPECT_EQ(test, test_param->bb.clock_limits[i].dispclk_mhz,
				test_param->expected_clock_limits[i].dispclk_mhz);
		KUNIT_EXPECT_EQ(test, test_param->bb.clock_limits[i].dppclk_mhz,
				test_param->expected_clock_limits[i].dppclk_mhz);
		KUNIT_EXPECT_EQ(test, test_param->bb.clock_limits[i].phyclk_mhz,
				test_param->expected_clock_limits[i].phyclk_mhz);
		KUNIT_EXPECT_EQ(test, test_param->bb.clock_limits[i].socclk_mhz,
				test_param->expected_clock_limits[i].socclk_mhz);
		KUNIT_EXPECT_EQ(test, test_param->bb.clock_limits[i].dscclk_mhz,
				test_param->expected_clock_limits[i].dscclk_mhz);
		KUNIT_EXPECT_EQ(test, test_param->bb.clock_limits[i].dram_speed_mts,
				test_param->expected_clock_limits[i].dram_speed_mts);
	}

	KUNIT_EXPECT_EQ(test, test_param->clock_states, test_param->bb.num_states);
}

static struct _vcs_dpi_soc_bounding_box_st original_dcn2_1_soc;
static struct _vcs_dpi_ip_params_st original_dcn2_1_ip;

/**
 * dcn20_fpu_dcn21_update_bw_bounding_box_test_init - Store backup copies of DCN global structures
 * @test: represents a running instance of a test.
 */
int dcn20_fpu_dcn21_update_bw_bounding_box_test_init(struct kunit *test)
{
	memcpy(&original_dcn2_1_soc, &dcn2_1_soc, sizeof(struct _vcs_dpi_soc_bounding_box_st));
	memcpy(&original_dcn2_1_ip, &dcn2_1_ip, sizeof(struct _vcs_dpi_ip_params_st));

	return 0;
}

/**
 * dcn20_fpu_dcn21_update_bw_bounding_box_test_exit - Restore original values
 * of DCN global structures
 * @test: represents a running instance of a test.
 */
static void dcn20_fpu_dcn21_update_bw_bounding_box_test_exit(struct kunit *test)
{
	memcpy(&dcn2_1_soc, &original_dcn2_1_soc, sizeof(struct _vcs_dpi_soc_bounding_box_st));
	memcpy(&dcn2_1_ip, &original_dcn2_1_ip, sizeof(struct _vcs_dpi_ip_params_st));
}

/**
 * dcn21_update_bw_bounding_box_test - KUnit test for dcn21_update_bw_bounding_box
 * @test: represents a running instance of a test.
 */
static void dcn21_update_bw_bounding_box_test(struct kunit *test)
{
	struct dcn21_update_bw_bounding_box_test_case *test_param =
		(struct dcn21_update_bw_bounding_box_test_case *)test->param_value;
	int i;

	DC_FP_START();
	dcn21_update_bw_bounding_box(&test_param->dc, &test_param->bw_params);
	DC_FP_END();

	KUNIT_EXPECT_EQ(test, test_param->dc.res_pool->res_cap->num_timing_generator,
			dcn2_1_ip.max_num_otg);
	KUNIT_EXPECT_EQ(test, test_param->dc.res_pool->pipe_count, dcn2_1_ip.max_num_dpp);
	KUNIT_EXPECT_EQ(test, test_param->bw_params.num_channels, dcn2_1_soc.num_chans);

	for (i = 0; i <= test_param->clocks_to_compare; i++) {
		KUNIT_EXPECT_EQ(test, test_param->expected_clock_limits[i].state,
				dcn2_1_soc.clock_limits[i].state);
		KUNIT_EXPECT_EQ(test, test_param->expected_clock_limits[i].dcfclk_mhz,
				dcn2_1_soc.clock_limits[i].dcfclk_mhz);
		KUNIT_EXPECT_EQ(test, test_param->expected_clock_limits[i].fabricclk_mhz,
				dcn2_1_soc.clock_limits[i].fabricclk_mhz);
		KUNIT_EXPECT_EQ(test, test_param->expected_clock_limits[i].dispclk_mhz,
				dcn2_1_soc.clock_limits[i].dispclk_mhz);
		KUNIT_EXPECT_EQ(test, test_param->expected_clock_limits[i].dppclk_mhz,
				dcn2_1_soc.clock_limits[i].dppclk_mhz);
		KUNIT_EXPECT_EQ(test, test_param->expected_clock_limits[i].phyclk_mhz,
				dcn2_1_soc.clock_limits[i].phyclk_mhz);
		KUNIT_EXPECT_EQ(test, test_param->expected_clock_limits[i].socclk_mhz,
				dcn2_1_soc.clock_limits[i].socclk_mhz);
		KUNIT_EXPECT_EQ(test, test_param->expected_clock_limits[i].dscclk_mhz,
				dcn2_1_soc.clock_limits[i].dscclk_mhz);
		KUNIT_EXPECT_EQ(test, test_param->expected_clock_limits[i].dram_speed_mts,
				dcn2_1_soc.clock_limits[i].dram_speed_mts);
	}

	if (test_param->bw_params.clk_table.num_entries)
		KUNIT_EXPECT_EQ(test, test_param->bw_params.clk_table.num_entries,
				dcn2_1_soc.num_states);
}

static void dcn20_cap_soc_clocks_test_to_desc(struct dcn20_cap_soc_clocks_test_case *t, char *desc)
{
	strcpy(desc, t->desc);
}

static void dcn21_update_bw_bounding_box_test_to_desc(struct dcn21_update_bw_bounding_box_test_case *t, char *desc)
{
	strcpy(desc, t->desc);
}

KUNIT_ARRAY_PARAM(dcn20_cap_soc_clocks,
		  dcn20_cap_soc_clocks_test_cases,
		  dcn20_cap_soc_clocks_test_to_desc);

static struct kunit_case dcn20_fpu_test_cases[] = {
	KUNIT_CASE_PARAM(dcn20_cap_soc_clocks_test, dcn20_cap_soc_clocks_gen_params),
	{  }
};

static struct kunit_suite dcn20_fpu_test_suite = {
	.name = "dml_dcn20_fpu",
	.test_cases = dcn20_fpu_test_cases,
};

KUNIT_ARRAY_PARAM(dcn21_update_bw_bounding_box,
		  dcn21_update_bw_bounding_box_test_data,
		  dcn21_update_bw_bounding_box_test_to_desc);

static struct kunit_case dcn20_fpu_dcn21_update_bw_bounding_box_test_cases[] = {
	KUNIT_CASE_PARAM(dcn21_update_bw_bounding_box_test,
			 dcn21_update_bw_bounding_box_gen_params),
	{  }
};

static struct kunit_suite dcn21_update_bw_bounding_box_test_suite = {
	.name = "dml_dcn20_fpu_dcn21_update_bw_bounding_box_test",
	.init = dcn20_fpu_dcn21_update_bw_bounding_box_test_init,
	.exit = dcn20_fpu_dcn21_update_bw_bounding_box_test_exit,
	.test_cases = dcn20_fpu_dcn21_update_bw_bounding_box_test_cases,
};

kunit_test_suites(&dcn20_fpu_test_suite, &dcn21_update_bw_bounding_box_test_suite);
