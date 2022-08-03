// SPDX-License-Identifier: MIT
/*
 * KUnit tests for dml/dcn20/display_rq_dlg_calc_20.c
 *
 * Copyright (c) 2022, Isabella Basso <isabbasso@riseup.net>
 */

#include <kunit/test.h>
#include "dml/display_mode_lib.h"

/* static function defined at display_rq_dlg_calc_20.c */
extern unsigned int get_bytes_per_element(enum source_format_class source_format,
					  bool is_chroma);
extern unsigned int get_blk_size_bytes(const enum source_macro_tile_size tile_size);
extern bool is_dual_plane(enum source_format_class source_format);

static void get_bytes_per_element_test(struct kunit *test)
{
	/* last numbers should tell us the horizontal 4-element region binary
	 * size N used for subsampling, thus giving us N/8 bytes per element
	 */
	/* note that 4:4:4 is not subsampled */
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_444_16, false), 2);
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_444_32, false), 4);
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_444_64, false), 8);

	/* dcn20 doesn't support bit depths over 10b */
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_420_12, false), 0);
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_420_12, true), 0);

	/* dm_444_XX are not dual plane */
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_444_16, true), 0);
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_444_32, true), 0);
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_444_64, true), 0);

	/* in the dm_42* values, last numbers specify bit depth, demanding we
	 * treat chroma and luma channels separately
	 */
	/* thus we'll now have ceil(N/8) bytes for luma */
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_420_8, false), 1);
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_420_10, false), 2);
	/* and double the luma value for accommodating blue and red chroma
	 * channels
	 */
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_420_8, true), 2);
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_420_10, true), 4);

	/* monochrome encodings should mirror non-subsampled variants */
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_mono_8, false),
			get_bytes_per_element(dm_444_8, false));
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_mono_16, false),
			get_bytes_per_element(dm_444_16, false));
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_mono_16, true), 0);

	/* dcn20 doesn't support 4:2:2 chroma subsampling */
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_422_8, false), 0);
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_422_8, true), 0);
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_422_10, false), 0);
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_422_10, true), 0);

	/* dcn20 doesn't support RGBE encodings */
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_rgbe, false), 0);
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_rgbe, true), 0);
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_rgbe_alpha, false), 0);
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_rgbe_alpha, true), 0);

	/* as in the first values, _8 here represents horizontal binary length */
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_444_8, false), 1);
	/* in a non-subsampled monochrome encoding chroma and luma should be the
	 * same length
	 */
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_444_8, false),
			get_bytes_per_element(dm_444_8, true));

	/* as dm_mono_8 == dm_444_8, it must behave the same way */
	KUNIT_ASSERT_EQ(test, get_bytes_per_element(dm_mono_8, false),
			get_bytes_per_element(dm_mono_8, true));
}

static void is_dual_plane_test(struct kunit *test)
{
	/* strictly speaking monochrome formats are not dual plane, but they're
	 * included here for completeness
	 */
	int source_format_count = 11;

	for (int i = 0; i < source_format_count; i++) {
		/* dcn20 doesn't support other dual plane formats */
		if (i == 3 || i == 4)
			KUNIT_ASSERT_TRUE(test, is_dual_plane(i));
		else
			KUNIT_ASSERT_FALSE(test, is_dual_plane(i));
	}
}

static void get_blk_size_bytes_test(struct kunit *test)
{
	/* corresponds to 4^4 kB tiles */
	KUNIT_ASSERT_EQ(test, get_blk_size_bytes(dm_256k_tile), 256 * 1024);
	/* corresponds to 4^3 kB tiles */
	KUNIT_ASSERT_EQ(test, get_blk_size_bytes(dm_64k_tile), 64 * 1024);
	/* corresponds to 4^1 kB tiles */
	KUNIT_ASSERT_EQ(test, get_blk_size_bytes(dm_4k_tile), 4 * 1024);

	/* garbage values should give us an error */
	/* KUNIT_ASSERT_EQ(test, get_blk_size_bytes(-1), -1); */
}

static struct kunit_case dcn20_rq_dlg_calc_20_test_cases[] = {
	KUNIT_CASE(get_bytes_per_element_test),
	KUNIT_CASE(is_dual_plane_test),
	KUNIT_CASE(get_blk_size_bytes_test),
	{  }
};

static struct kunit_suite dcn20_rq_dlg_calc_20_test_suite = {
	.name = "display_rq_dlg_calc_20",
	.test_cases = dcn20_rq_dlg_calc_20_test_cases,
};

kunit_test_suites(&dcn20_rq_dlg_calc_20_test_suite);
MODULE_LICENSE("Dual MIT/GPL");
