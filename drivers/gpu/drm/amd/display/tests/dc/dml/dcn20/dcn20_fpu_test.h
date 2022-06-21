/* SPDX-License-Identifier: MIT */
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

extern struct dcn20_cap_soc_clocks_test_case dcn20_cap_soc_clocks_test_cases[3];
extern struct dcn21_update_bw_bounding_box_test_case dcn21_update_bw_bounding_box_test_data[1];

int dcn20_fpu_dcn21_update_bw_bounding_box_test_init(struct kunit *test);
void dcn20_fpu_dcn21_update_bw_bounding_box_test_exit(struct kunit *test);

void dcn20_cap_soc_clocks_test(struct kunit *test);
void dcn21_update_bw_bounding_box_test(struct kunit *test);
