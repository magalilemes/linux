/* SPDX-License-Identifier: MIT */
/*
 * KUnit tests for dml/display_mode_vba.h
 *
 * Copyright (C) 2022, Maíra Canal <mairacanal@riseup.net>
 */

#include <kunit/test.h>

#include "../../dml/display_mode_structs.h"
#include "../../dml/display_mode_lib.h"
#include "../../dml/display_mode_vba.h"

struct pixel_clock_adjustment_for_progressive_to_interlace_unit_expected {
	const double pixel_clock[DC__NUM_DPP__MAX];
	const double pixel_clock_backend[DC__NUM_DPP__MAX];
};

struct pixel_clock_adjustment_for_progressive_to_interlace_unit_test_case {
	const char *desc;
	const unsigned int number_of_active_planes;
	const bool interlace[DC__NUM_DPP__MAX];
	const bool progressive_to_interlace_unit_in_OPP;
	const double pixel_clock[DC__NUM_DPP__MAX];
	const struct pixel_clock_adjustment_for_progressive_to_interlace_unit_expected expected;
};

struct calculate_256B_block_sizes_test_case {
	const char *desc;
	const enum source_format_class source_pixel_format;
	const enum dm_swizzle_mode surface_tiling;
	const unsigned int byte_per_pixel_Y;
	const unsigned int byte_per_pixel_C;
	const unsigned int block_height_256_bytes_Y;
	const unsigned int block_height_256_bytes_C;
	const unsigned int block_width_256_bytes_Y;
	const unsigned int block_width_256_bytes_C;
};

void pclk_adjustment_for_progressive_to_interlace_unit_test(struct kunit *test);
void calculate_256B_block_sizes_test(struct kunit *test);
void calculate_min_and_max_prefetch_mode_test(struct kunit *test);
