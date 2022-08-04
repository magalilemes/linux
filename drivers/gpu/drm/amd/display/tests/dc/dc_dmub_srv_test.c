// SPDX-License-Identifier: MIT
/*
 * KUnit tests for dc_dmub_srv.c
 *
 * Copyright (C) 2022, Maíra Canal <mairacanal@riseup.net>
 */

#include <kunit/test.h>
#include "dc_dmub_srv.h"

/* static function defined at dc_dmub_srv.c */
extern void populate_subvp_cmd_drr_info(struct dc *dc,
		struct pipe_ctx *subvp_pipe,
		struct pipe_ctx *vblank_pipe,
		struct dmub_cmd_fw_assisted_mclk_switch_pipe_data_v2 *pipe_data);

struct populate_subvp_cmd_drr_info_test_case {
	const char *desc;
	struct dc *dc;
	struct pipe_ctx *subvp_pipe;
	struct pipe_ctx *vblank_pipe;
	const u8 drr_in_use;
	const u8 drr_window_size_ms;
	const u16 min_vtotal_supported;
	const u16 max_vtotal_supported;
	const u8 use_ramping;
};

struct populate_subvp_cmd_drr_info_test_case populate_subvp_cmd_drr_info_cases[] = {
	{
		.desc = "Same Clock Frequency",
		.dc = &(struct dc) {
			.caps = {
				.subvp_prefetch_end_to_mall_start_us = 0,
			}
		},
		.subvp_pipe = &(struct pipe_ctx) {
			.stream = &(struct dc_stream_state) {
				.timing = {
					.h_total = 2784,
					.v_addressable = 1080,
					.pix_clk_100hz = 1855800,
				},
				.mall_stream_config = {
					.paired_stream = &(struct dc_stream_state) {
						.timing = {
							.h_total = 3600,
							.v_total = 1111,
							.v_addressable = 1080,
							.v_front_porch = 3,
							.pix_clk_100hz = 1855800,
						},
					},
				},
			},
		},
		.vblank_pipe = &(struct pipe_ctx) {
			.stream = &(struct dc_stream_state) {
				.timing = {
					.h_total = 2784,
					.v_total = 1111,
					.v_addressable = 1080,
					.pix_clk_100hz = 1855800,
				},
			},
		},
		.drr_in_use = true,
		.use_ramping = false,
		.drr_window_size_ms = 4,
		.min_vtotal_supported = 2540,
		.max_vtotal_supported = 2619,
	},
	{
		.desc = "Same Clock Frequency with Prefetch End to Mall Start",
		.dc = &(struct dc) {
			.caps = {
				.subvp_prefetch_end_to_mall_start_us = 500,
			}
		},
		.subvp_pipe = &(struct pipe_ctx) {
			.stream = &(struct dc_stream_state) {
				.timing = {
					.h_total = 2784,
					.v_addressable = 1080,
					.pix_clk_100hz = 1855800,
				},
				.mall_stream_config = {
					.paired_stream = &(struct dc_stream_state) {
						.timing = {
							.h_total = 3600,
							.v_total = 1111,
							.v_addressable = 1080,
							.v_front_porch = 3,
							.pix_clk_100hz = 1855800,
						},
					},
				},
			},
		},
		.vblank_pipe = &(struct pipe_ctx) {
			.stream = &(struct dc_stream_state) {
				.timing = {
					.h_total = 2784,
					.v_total = 1111,
					.v_addressable = 1080,
					.pix_clk_100hz = 1855800,
				},
			},
		},
		.drr_in_use = true,
		.use_ramping = false,
		.drr_window_size_ms = 4,
		.min_vtotal_supported = 2540,
		.max_vtotal_supported = 2619,
	},
	{
		.desc = "Same Clock Frequency Not Multiple of 2",
		.dc = &(struct dc) {
			.caps = {
				.subvp_prefetch_end_to_mall_start_us = 0,
			}
		},
		.subvp_pipe = &(struct pipe_ctx) {
			.stream = &(struct dc_stream_state) {
				.timing = {
					.h_total = 2784,
					.v_addressable = 1080,
					.pix_clk_100hz = 1866743,
				},
				.mall_stream_config = {
					.paired_stream = &(struct dc_stream_state) {
						.timing = {
							.h_total = 3600,
							.v_total = 2400,
							.v_addressable = 2360,
							.v_front_porch = 4,
							.pix_clk_100hz = 1866743,
						},
					},
				},
			},
		},
		.vblank_pipe = &(struct pipe_ctx) {
			.stream = &(struct dc_stream_state) {
				.timing = {
					.h_total = 3600,
					.v_total = 2400,
					.v_addressable = 2360,
					.pix_clk_100hz = 1866743,
				},
			},
		},
		.drr_in_use = true,
		.use_ramping = false,
		.drr_window_size_ms = 4,
		.min_vtotal_supported = 1387,
		.max_vtotal_supported = 2875,
	},
	{
		.desc = "Different Clock Frequency for smaller h_total and v_total",
		.dc = &(struct dc) {
			.caps = {
				.subvp_prefetch_end_to_mall_start_us = 300,
			}
		},
		.subvp_pipe = &(struct pipe_ctx) {
			.stream = &(struct dc_stream_state) {
				.timing = {
					.h_total = 1280,
					.v_addressable = 600,
					.pix_clk_100hz = 1855800,
				},
				.mall_stream_config = {
					.paired_stream = &(struct dc_stream_state) {
						.timing = {
							.h_total = 1280,
							.v_total = 720,
							.v_addressable = 600,
							.v_front_porch = 4,
							.pix_clk_100hz = 1866743,
						},
					},
				},
			},
		},
		.vblank_pipe = &(struct pipe_ctx) {
			.stream = &(struct dc_stream_state) {
				.timing = {
					.h_total = 1280,
					.v_total = 720,
					.v_addressable = 600,
					.pix_clk_100hz = 2100800,
				},
			},
		},
		.drr_in_use = true,
		.use_ramping = false,
		.drr_window_size_ms = 4,
		.min_vtotal_supported = 1477,
		.max_vtotal_supported = 9954,
	},
	{
		.desc = "Different Clock Frequency for approximately 1920x1080",
		.dc = &(struct dc) {
			.caps = {
				.subvp_prefetch_end_to_mall_start_us = 0,
			}
		},
		.subvp_pipe = &(struct pipe_ctx) {
			.stream = &(struct dc_stream_state) {
				.timing = {
					.h_total = 1920,
					.v_addressable = 1000,
					.pix_clk_100hz = 1855800,
				},
				.mall_stream_config = {
					.paired_stream = &(struct dc_stream_state) {
						.timing = {
							.h_total = 1911,
							.v_total = 1080,
							.v_addressable = 1000,
							.v_front_porch = 7,
							.pix_clk_100hz = 1866743,
						},
					},
				},
			},
		},
		.vblank_pipe = &(struct pipe_ctx) {
			.stream = &(struct dc_stream_state) {
				.timing = {
					.h_total = 1280,
					.v_total = 720,
					.v_addressable = 600,
					.pix_clk_100hz = 2100800,
				},
			},
		},
		.drr_in_use = true,
		.use_ramping = false,
		.drr_window_size_ms = 4,
		.min_vtotal_supported = 2482,
		.max_vtotal_supported = 8971,
	},
};

static void populate_subvp_cmd_drr_info_test_to_desc(struct
		populate_subvp_cmd_drr_info_test_case * t, char *desc)
{
	strcpy(desc, t->desc);
}

KUNIT_ARRAY_PARAM(populate_subvp_cmd_drr_info, populate_subvp_cmd_drr_info_cases,
		  populate_subvp_cmd_drr_info_test_to_desc);

static void populate_subvp_cmd_drr_info_test(struct kunit *test)
{
	const struct populate_subvp_cmd_drr_info_test_case *test_param =
		test->param_value;
	struct dmub_cmd_fw_assisted_mclk_switch_pipe_data_v2 *pipe_data;

	pipe_data = kunit_kzalloc(test,
				  sizeof(struct dmub_cmd_fw_assisted_mclk_switch_pipe_data_v2),
				  GFP_KERNEL);

	populate_subvp_cmd_drr_info(test_param->dc, test_param->subvp_pipe,
				    test_param->vblank_pipe, pipe_data);

	KUNIT_EXPECT_EQ(test, test_param->drr_in_use,
			pipe_data->pipe_config.vblank_data.drr_info.drr_in_use);
	KUNIT_EXPECT_EQ(test, test_param->drr_window_size_ms,
			pipe_data->pipe_config.vblank_data.drr_info.drr_window_size_ms);
	KUNIT_EXPECT_EQ(test, test_param->use_ramping,
			pipe_data->pipe_config.vblank_data.drr_info.use_ramping);
	KUNIT_EXPECT_EQ(test, test_param->min_vtotal_supported,
			pipe_data->pipe_config.vblank_data.drr_info.min_vtotal_supported);
	KUNIT_EXPECT_EQ(test, test_param->max_vtotal_supported,
			pipe_data->pipe_config.vblank_data.drr_info.max_vtotal_supported);
}

static struct kunit_case dc_dmub_srv_cases[] = {
	KUNIT_CASE_PARAM(populate_subvp_cmd_drr_info_test, populate_subvp_cmd_drr_info_gen_params),
	{  }
};

static struct kunit_suite dc_dmub_srv_suite = {
	.name = "dc_dmub_srv",
	.test_cases = dc_dmub_srv_cases,
};

kunit_test_suite(dc_dmub_srv_suite);
MODULE_LICENSE("Dual MIT/GPL");
