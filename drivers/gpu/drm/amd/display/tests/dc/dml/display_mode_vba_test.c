// SPDX-License-Identifier: MIT
/*
 * KUnit tests for dml/display_mode_vba.h
 *
 * Copyright (C) 2022, Maíra Canal <mairacanal@riseup.net>
 */

#include <drm/drm_util.h>
#include "display_mode_vba_test.h"

/**
 * pclk_adjustment_for_progressive_to_interlace_unit_test - KUnit test
 * for PixelClockAdjustmentForProgressiveToInterlaceUnit
 * @test: represents a running instance of a test.
 */
void pclk_adjustment_for_progressive_to_interlace_unit_test(struct kunit *test)
{
	const struct pixel_clock_adjustment_for_progressive_to_interlace_unit_test_case
		*test_param = test->param_value;
	struct display_mode_lib *mode_lib;
	size_t pixel_clock_size = DC__NUM_DPP__MAX * sizeof(const double);
	size_t interlace_size = DC__NUM_DPP__MAX * sizeof(const bool);

	mode_lib = kunit_kzalloc(test, sizeof(struct display_mode_lib), GFP_KERNEL);
	KUNIT_ASSERT_NOT_ERR_OR_NULL(test, mode_lib);

	mode_lib->vba.NumberOfActivePlanes = test_param->number_of_active_planes;
	memcpy(mode_lib->vba.Interlace, test_param->interlace, interlace_size);
	mode_lib->vba.ProgressiveToInterlaceUnitInOPP = test_param->progressive_to_interlace_unit_in_OPP;
	memcpy(mode_lib->vba.PixelClock, test_param->pixel_clock, pixel_clock_size);

	PixelClockAdjustmentForProgressiveToInterlaceUnit(mode_lib);

	KUNIT_EXPECT_TRUE(test, !memcmp(mode_lib->vba.PixelClock,
					test_param->expected.pixel_clock, pixel_clock_size));
	KUNIT_EXPECT_TRUE(test, !memcmp(mode_lib->vba.PixelClockBackEnd,
					test_param->expected.pixel_clock_backend,
					pixel_clock_size));
}
EXPORT_SYMBOL_FOR_TESTS_ONLY(pclk_adjustment_for_progressive_to_interlace_unit_test);

/**
 * calculate_256B_block_sizes_test - KUnit test for Calculate256BBlockSizes
 * @test: represents a running instance of a test.
 */
void calculate_256B_block_sizes_test(struct kunit *test)
{
	const struct calculate_256B_block_sizes_test_case *test_param = test->param_value;
	unsigned int *block_height_256_bytes_Y, *block_height_256_bytes_C;
	unsigned int *block_width_256_bytes_Y, *block_width_256_bytes_C;

	block_height_256_bytes_Y = kunit_kzalloc(test, sizeof(unsigned int), GFP_KERNEL);
	KUNIT_ASSERT_NOT_ERR_OR_NULL(test, block_height_256_bytes_Y);

	block_height_256_bytes_C = kunit_kzalloc(test, sizeof(unsigned int), GFP_KERNEL);
	KUNIT_ASSERT_NOT_ERR_OR_NULL(test, block_height_256_bytes_C);

	block_width_256_bytes_Y = kunit_kzalloc(test, sizeof(unsigned int), GFP_KERNEL);
	KUNIT_ASSERT_NOT_ERR_OR_NULL(test, block_width_256_bytes_Y);

	block_width_256_bytes_C = kunit_kzalloc(test, sizeof(unsigned int), GFP_KERNEL);
	KUNIT_ASSERT_NOT_ERR_OR_NULL(test, block_width_256_bytes_C);

	Calculate256BBlockSizes(test_param->source_pixel_format,
				test_param->surface_tiling, test_param->byte_per_pixel_Y,
			test_param->byte_per_pixel_C, block_height_256_bytes_Y,
			block_height_256_bytes_C, block_width_256_bytes_Y,
			block_width_256_bytes_C);

	KUNIT_EXPECT_EQ(test, *block_height_256_bytes_Y, test_param->block_height_256_bytes_Y);
	KUNIT_EXPECT_EQ(test, *block_height_256_bytes_C, test_param->block_height_256_bytes_C);
	KUNIT_EXPECT_EQ(test, *block_width_256_bytes_Y, test_param->block_width_256_bytes_Y);
	KUNIT_EXPECT_EQ(test, *block_width_256_bytes_C, test_param->block_width_256_bytes_C);
}
EXPORT_SYMBOL_FOR_TESTS_ONLY(calculate_256B_block_sizes_test);

/**
 * calculate_min_and_max_prefetch_mode_test - KUnit test for CalculateMinAndMaxPrefetchMode
 * @test: represents a running instance of a test.
 */
void calculate_min_and_max_prefetch_mode_test(struct kunit *test)
{
	unsigned int *min_prefetch_mode, *max_prefetch_mode;

	min_prefetch_mode = kunit_kzalloc(test, sizeof(unsigned int), GFP_KERNEL);
	KUNIT_ASSERT_NOT_ERR_OR_NULL(test, min_prefetch_mode);

	max_prefetch_mode = kunit_kzalloc(test, sizeof(unsigned int), GFP_KERNEL);
	KUNIT_ASSERT_NOT_ERR_OR_NULL(test, max_prefetch_mode);

	/* Try to allow DRAM self-refresh and MCLK switch */
	KUNIT_EXPECT_FALSE(test,
			   CalculateMinAndMaxPrefetchMode(dm_try_to_allow_self_refresh_and_mclk_switch,
							  min_prefetch_mode, max_prefetch_mode));
	KUNIT_EXPECT_EQ(test, *min_prefetch_mode, 0);
	KUNIT_EXPECT_EQ(test, *max_prefetch_mode, 2);

	/* Allow DRAM self-refresh and MCLK switch */
	KUNIT_EXPECT_FALSE(test,
			   CalculateMinAndMaxPrefetchMode(dm_allow_self_refresh_and_mclk_switch,
							  min_prefetch_mode, max_prefetch_mode));
	KUNIT_EXPECT_EQ(test, *min_prefetch_mode, 0);
	KUNIT_EXPECT_EQ(test, *max_prefetch_mode, 0);

	/* Allow only DRAM self-refresh  */
	KUNIT_EXPECT_FALSE(test,
			   CalculateMinAndMaxPrefetchMode(dm_allow_self_refresh, min_prefetch_mode,
							  max_prefetch_mode));
	KUNIT_EXPECT_EQ(test, *min_prefetch_mode, 1);
	KUNIT_EXPECT_EQ(test, *max_prefetch_mode, 1);

	/* Allow neither DRAM self-refresh nor MCLK switch */
	KUNIT_EXPECT_FALSE(test,
			   CalculateMinAndMaxPrefetchMode(dm_neither_self_refresh_nor_mclk_switch,
							  min_prefetch_mode, max_prefetch_mode));
	KUNIT_EXPECT_EQ(test, *min_prefetch_mode, 2);
	KUNIT_EXPECT_EQ(test, *max_prefetch_mode, 2);

	/* Invalid self-refresh affinity */
	KUNIT_EXPECT_TRUE(test,
			  CalculateMinAndMaxPrefetchMode(-1,
							 min_prefetch_mode, max_prefetch_mode));
	KUNIT_EXPECT_EQ(test, *min_prefetch_mode, 0);
	KUNIT_EXPECT_EQ(test, *max_prefetch_mode, 2);
}
EXPORT_SYMBOL_FOR_TESTS_ONLY(calculate_min_and_max_prefetch_mode_test);
