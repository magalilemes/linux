// SPDX-License-Identifier: MIT
/*
 * KUnit tests for dml/display_mode_vba.h
 *
 * Copyright (C) 2022, Maíra Canal <mairacanal@riseup.net>
 */

#include <kunit/test.h>

#include "display_mode_vba_test.h"

const static struct pixel_clock_adjustment_for_progressive_to_interlace_unit_test_case
pixel_clock_adjustment_for_progressive_to_interlace_unit_cases[] = {
	{
		.desc = "No active planes",
		.number_of_active_planes = 0,
		.interlace = {false, false, false, false, false, false, false, false},
		.progressive_to_interlace_unit_in_OPP = false,
		.pixel_clock = {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00},
		.expected = {
			.pixel_clock = {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00},
			.pixel_clock_backend = {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00},
		},
	},
	{
		.desc = "Two active planes with no interlaced output",
		.number_of_active_planes = 2,
		.interlace = {false, false, false, false, false, false, false, false},
		.progressive_to_interlace_unit_in_OPP = true,
		.pixel_clock = {3200.00, 1360.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00},
		.expected = {
			.pixel_clock = {3200.00, 1360.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00},
			.pixel_clock_backend = {3200.00, 1360.00, 0.00, 0.00, 0.00, 0.00,
				0.00, 0.00},
		},
	},
	{
		.desc = "Three active planes with one interlaced plane",
		.number_of_active_planes = 3,
		.interlace = {false, true, false, false, false, false, false, false},
		.progressive_to_interlace_unit_in_OPP = true,
		.pixel_clock = {3200.00, 1360.00, 400.00, 0.00, 0.00, 0.00, 0.00, 0.00},
		.expected = {
			.pixel_clock = {3200.00, 2720.00, 400.00, 0.00, 0.00, 0.00, 0.00, 0.00},
			.pixel_clock_backend = {3200.00, 1360.00, 400.00, 0.00, 0.00, 0.00,
				0.00, 0.00},
		},
	},
	{
		.desc = "Five active planes with three interlaced planes",
		.number_of_active_planes = 5,
		.interlace = {false, true, false, true, true, false, false, false},
		.progressive_to_interlace_unit_in_OPP = true,
		.pixel_clock = {3200.00, 1360.00, 400.00, 340.00, 680.00, 0.00, 0.00, 0.00},
		.expected = {
			.pixel_clock = {3200.00, 2720.00, 400.00, 680.00, 1360.00, 0.00, 0.00,
				0.00},
			.pixel_clock_backend = {3200.00, 1360.00, 400.00, 340.00, 680.00, 0.00,
				0.00, 0.00},
		},
	},
	{
		.desc = "Eight active planes with five interlaced planes",
		.number_of_active_planes = 8,
		.interlace = {true, true, false, true, true, false, true, false},
		.progressive_to_interlace_unit_in_OPP = true,
		.pixel_clock = {3200.00, 1360.00, 400.00, 340.00, 680.00, 1360.00, 2720.00, 340.00},
		.expected = {
			.pixel_clock = {6400.00, 2720.00, 400.00, 680.00, 1360.00, 1360.00, 5440.00,
				340.00},
			.pixel_clock_backend = {3200.00, 1360.00, 400.00, 340.00, 680.00, 1360.00,
				2720.00, 340.0},
		},
	},
	{
		.desc = "Eight active planes with all planes interlaced",
		.number_of_active_planes = 8,
		.interlace = {true, true, true, true, true, true, true, true},
		.progressive_to_interlace_unit_in_OPP = true,
		.pixel_clock = {3200.00, 1360.00, 400.00, 340.00, 680.00, 1360.00, 2720.00, 340.00},
		.expected = {
			.pixel_clock = {6400.00, 2720.00, 800.0, 680.00, 1360.00, 2720.00, 5440.0,
				680.00},
			.pixel_clock_backend = {3200.00, 1360.00, 400.00, 340.00, 680.00, 1360.00,
				2720.00, 340.00},
		},
	},
	{
		.desc = "Eight active planes with no interlaced plane",
		.number_of_active_planes = 8,
		.interlace = {false, false, false, false, false, false, false, false},
		.progressive_to_interlace_unit_in_OPP = false,
		.pixel_clock = {3200.00, 1360.00, 400.00, 340.00, 680.00, 1360.00, 2720.00, 340.00},
		.expected = {
			.pixel_clock = {3200.00, 1360.00, 400.00, 340.00, 680.00, 1360.00,
				2720.00, 340.00},
			.pixel_clock_backend = {3200.00, 1360.00, 400.00, 340.00, 680.00, 1360.00,
				2720.00, 340.00},
		},
	},
	{
		.desc = "Eight active planes with no progressive_to_interlace_unit_in_OPP",
		.number_of_active_planes = 8,
		.interlace = {true, true, true, true, true, true, true, true},
		.progressive_to_interlace_unit_in_OPP = false,
		.pixel_clock = {3200.00, 1360.00, 400.00, 340.00, 680.00, 1360.00, 2720.00, 340.00},
		.expected = {
			.pixel_clock = {3200.00, 1360.00, 400.00, 340.00, 680.00, 1360.00, 2720.00,
				340.00},
			.pixel_clock_backend = {3200.00, 1360.00, 400.00, 340.00, 680.00, 1360.00,
				2720.00, 340.00},
		},
	},
};

const static struct calculate_256B_block_sizes_test_case
calculate_256B_block_sizes_cases[] = {
	/*
	 * Here 16-bit specifies the number of bits in the horizontal 4-element region
	 * used for subsampling
	 */
	{
		.desc = "4:4:4 16-bit encoding with linear swizzle",
		.source_pixel_format = dm_444_16,
		.surface_tiling = dm_sw_linear,
		.byte_per_pixel_Y = 2,
		.byte_per_pixel_C = 0,
		.block_height_256_bytes_Y = 1,
		.block_width_256_bytes_Y = 128,
		.block_height_256_bytes_C = 0,
		.block_width_256_bytes_C = 0,
	},
	{
		.desc = "4:4:4 16-bit encoding with 256B standard swizzle",
		.source_pixel_format = dm_444_16,
		.surface_tiling = dm_sw_256b_s,
		.byte_per_pixel_Y = 2,
		.byte_per_pixel_C = 0,
		.block_height_256_bytes_Y = 8,
		.block_width_256_bytes_Y = 16,
		.block_height_256_bytes_C = 0,
		.block_width_256_bytes_C = 0,
	},
	/*
	 * Here 32-bit specifies the number of bits in the horizontal 4-element region
	 * used for subsampling
	 */
	{
		.desc = "4:4:4 32-bit encoding with linear swizzle",
		.source_pixel_format = dm_444_32,
		.surface_tiling = dm_sw_linear,
		.byte_per_pixel_Y = 4,
		.byte_per_pixel_C = 0,
		.block_height_256_bytes_Y = 1,
		.block_width_256_bytes_Y = 64,
		.block_height_256_bytes_C = 0,
		.block_width_256_bytes_C = 0,
	},
	{
		.desc = "4:4:4 32-bit encoding with 256B display swizzle",
		.source_pixel_format = dm_444_32,
		.surface_tiling = dm_sw_256b_d,
		.byte_per_pixel_Y = 4,
		.byte_per_pixel_C = 0,
		.block_height_256_bytes_Y = 8,
		.block_width_256_bytes_Y = 8,
		.block_height_256_bytes_C = 0,
		.block_width_256_bytes_C = 0,
	},
	/*
	 * Here 64-bit specifies the number of bits in the horizontal 4-element region
	 * used for subsampling
	 */
	{
		.desc = "4:4:4 64-bit encoding with linear swizzle",
		.source_pixel_format = dm_444_64,
		.surface_tiling = dm_sw_linear,
		.byte_per_pixel_Y = 8,
		.byte_per_pixel_C = 0,
		.block_height_256_bytes_Y = 1,
		.block_width_256_bytes_Y = 32,
		.block_height_256_bytes_C = 0,
		.block_width_256_bytes_C = 0,
	},
	{
		.desc = "4:4:4 64-bit encoding with 4KB standard swizzle",
		.source_pixel_format = dm_444_64,
		.surface_tiling = dm_sw_4kb_s,
		.byte_per_pixel_Y = 8,
		.byte_per_pixel_C = 0,
		.block_height_256_bytes_Y = 4,
		.block_width_256_bytes_Y = 8,
		.block_height_256_bytes_C = 0,
		.block_width_256_bytes_C = 0,
	},
	{
		.desc = "4:4:4 8-bit encoding with linear swizzle",
		.source_pixel_format = dm_444_8,
		.surface_tiling = dm_sw_linear,
		.byte_per_pixel_Y = 1,
		.byte_per_pixel_C = 0,
		.block_height_256_bytes_Y = 1,
		.block_width_256_bytes_Y = 256,
		.block_height_256_bytes_C = 0,
		.block_width_256_bytes_C = 0,
	},
	{
		.desc = "4:4:4 8-bit encoding with 4KB display swizzle",
		.source_pixel_format = dm_444_8,
		.surface_tiling = dm_sw_4kb_d,
		.byte_per_pixel_Y = 1,
		.byte_per_pixel_C = 0,
		.block_height_256_bytes_Y = 16,
		.block_width_256_bytes_Y = 16,
		.block_height_256_bytes_C = 0,
		.block_width_256_bytes_C = 0,
	},
	{
		.desc = "8-bit mono encoding with linear swizzle",
		.source_pixel_format = dm_mono_8,
		.surface_tiling = dm_sw_linear,
		.byte_per_pixel_Y = 1,
		.byte_per_pixel_C = 0,
		.block_height_256_bytes_Y = 1,
		.block_width_256_bytes_Y = 256,
		.block_height_256_bytes_C = 0,
		.block_width_256_bytes_C = 0,
	},
	{
		.desc = "8-bit mono encoding with 64KB standard swizzle",
		.source_pixel_format = dm_mono_8,
		.surface_tiling = dm_sw_64kb_s,
		.byte_per_pixel_Y = 1,
		.byte_per_pixel_C = 0,
		.block_height_256_bytes_Y = 16,
		.block_width_256_bytes_Y = 16,
		.block_height_256_bytes_C = 0,
		.block_width_256_bytes_C = 0,
	},
	{
		.desc = "16-bit mono encoding with linear swizzle",
		.source_pixel_format = dm_mono_16,
		.surface_tiling = dm_sw_linear,
		.byte_per_pixel_Y = 2,
		.byte_per_pixel_C = 0,
		.block_height_256_bytes_Y = 1,
		.block_width_256_bytes_Y = 128,
		.block_height_256_bytes_C = 0,
		.block_width_256_bytes_C = 0,
	},
	{
		.desc = "16-bit mono encoding with 64KB display swizzle",
		.source_pixel_format = dm_mono_16,
		.surface_tiling = dm_sw_64kb_d,
		.byte_per_pixel_Y = 2,
		.byte_per_pixel_C = 0,
		.block_height_256_bytes_Y = 8,
		.block_width_256_bytes_Y = 16,
		.block_height_256_bytes_C = 0,
		.block_width_256_bytes_C = 0,
	},
	{
		.desc = "8-bit 4:2:0 encoding with linear swizzle",
		.source_pixel_format = dm_420_8,
		.surface_tiling = dm_sw_linear,
		.byte_per_pixel_Y = 1,
		.byte_per_pixel_C = 2,
		.block_height_256_bytes_Y = 1,
		.block_width_256_bytes_Y = 256,
		.block_height_256_bytes_C = 1,
		.block_width_256_bytes_C = 128,
	},
	{
		.desc = "8-bit 4:2:0 encoding with VAR standard swizzle",
		.source_pixel_format = dm_420_8,
		.surface_tiling = dm_sw_var_s,
		.byte_per_pixel_Y = 1,
		.byte_per_pixel_C = 2,
		.block_height_256_bytes_Y = 16,
		.block_width_256_bytes_Y = 16,
		.block_height_256_bytes_C = 8,
		.block_width_256_bytes_C = 16,
	},
	{
		.desc = "10-bit 4:2:0 encoding with linear swizzle",
		.source_pixel_format = dm_420_10,
		.surface_tiling = dm_sw_linear,
		.byte_per_pixel_Y = 4.0 / 3.0,
		.byte_per_pixel_C = 8.0 / 3.0,
		.block_height_256_bytes_Y = 1,
		.block_width_256_bytes_Y = 256,
		.block_height_256_bytes_C = 1,
		.block_width_256_bytes_C = 128,
	},
	{
		.desc = "10-bit 4:2:0 encoding with VAR display swizzle",
		.source_pixel_format = dm_420_10,
		.surface_tiling = dm_sw_var_d,
		.byte_per_pixel_Y = 4.0 / 3.0,
		.byte_per_pixel_C = 8.0 / 3.0,
		.block_height_256_bytes_Y = 8,
		.block_width_256_bytes_Y =  32,
		.block_height_256_bytes_C = 8,
		.block_width_256_bytes_C = 16,
	},
};

static void pixel_clock_adjustment_for_progressive_to_interlace_unit_test_to_desc
(const struct pixel_clock_adjustment_for_progressive_to_interlace_unit_test_case *t, char *desc)
{
	strcpy(desc, t->desc);
}

KUNIT_ARRAY_PARAM(pixel_clock_adjustment_for_progressive_to_interlace_unit,
		  pixel_clock_adjustment_for_progressive_to_interlace_unit_cases,
		pixel_clock_adjustment_for_progressive_to_interlace_unit_test_to_desc);

static void calculate_256B_block_sizes_test_to_desc(const struct
		calculate_256B_block_sizes_test_case * t, char *desc)
{
	strcpy(desc, t->desc);
}

KUNIT_ARRAY_PARAM(calculate_256B_block_sizes, calculate_256B_block_sizes_cases,
		  calculate_256B_block_sizes_test_to_desc);

static struct kunit_case display_mode_vba_test_cases[] = {
	KUNIT_CASE_PARAM(pclk_adjustment_for_progressive_to_interlace_unit_test,
			 pixel_clock_adjustment_for_progressive_to_interlace_unit_gen_params),
	KUNIT_CASE_PARAM(calculate_256B_block_sizes_test, calculate_256B_block_sizes_gen_params),
	KUNIT_CASE(calculate_min_and_max_prefetch_mode_test),
	{  }
};

static struct kunit_suite display_mode_vba_test_suite = {
	.name = "dml_display_mode_vba",
	.test_cases = display_mode_vba_test_cases,
};

kunit_test_suite(display_mode_vba_test_suite);
MODULE_LICENSE("Dual MIT/GPL");
