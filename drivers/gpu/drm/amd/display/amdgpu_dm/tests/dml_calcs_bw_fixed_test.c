// SPDX-License-Identifier: MIT
/* Unit tests for dml/calcs/bw_fixed.c
 */

#include <kunit/test.h>
#include "../../dc/inc/bw_fixed.h"

static void abs_i64_test(struct kunit *test)
{
	KUNIT_EXPECT_EQ(test, 0ULL, abs_i64(0LL));
	KUNIT_EXPECT_EQ(test, (uint64_t)MAX_I64, abs_i64(MAX_I64));
	KUNIT_EXPECT_EQ(test, (uint64_t)MAX_I64 + 1, abs_i64(MIN_I64));
}

static void bw_int_to_fixed_nonconst_test(struct kunit *test)
{
	struct bw_fixed res;

	res = bw_int_to_fixed_nonconst(0LL);
	KUNIT_EXPECT_EQ(test, 0, res.value);

	res = bw_int_to_fixed_nonconst(1000);
	KUNIT_EXPECT_EQ(test, 16777216000, res.value);

	res = bw_int_to_fixed_nonconst(-1000);
	KUNIT_EXPECT_EQ(test, -16777216000, res.value);

	res = bw_int_to_fixed_nonconst(BW_FIXED_MAX_I32 - 1);
	KUNIT_EXPECT_EQ(test, 9223372036821221376, res.value);

	res = bw_int_to_fixed_nonconst(BW_FIXED_MIN_I32 + 1);
	KUNIT_EXPECT_EQ(test, -9223372036837998592, res.value);
}

static void bw_frc_to_fixed_test(struct kunit *test)
{
	struct bw_fixed res;

	res = bw_frc_to_fixed(MAX_I64, MAX_I64);
	KUNIT_EXPECT_EQ(test, 1LL << 24, res.value);

	res = bw_frc_to_fixed(1, MAX_I64);
	KUNIT_EXPECT_EQ(test, 0LL, res.value);

	res = bw_frc_to_fixed(0, MAX_I64);
	KUNIT_EXPECT_EQ(test, 0LL, res.value);

	res = bw_frc_to_fixed(MIN_I64, 1);
	KUNIT_EXPECT_EQ(test, 0LL, res.value);

	res = bw_frc_to_fixed(4, 3);
	KUNIT_EXPECT_EQ(test, 22369621LL, res.value);

	res = bw_frc_to_fixed(62609, 100);
	KUNIT_EXPECT_EQ(test, 10504047165LL, res.value);

	res = bw_frc_to_fixed(-62609, 100);
	KUNIT_EXPECT_EQ(test, -10504047165LL, res.value);
}

static void bw_floor2_test(struct kunit *test)
{
	struct bw_fixed arg;
	struct bw_fixed significance;
	struct bw_fixed res;

	arg.value = 10;
	significance.value = 3;
	res = bw_floor2(arg, significance);
	KUNIT_EXPECT_EQ(test, 9, res.value);

	arg.value = 10;
	significance.value = 5;
	res = bw_floor2(arg, significance);
	KUNIT_EXPECT_EQ(test, 10, res.value);

	arg.value = 10;
	significance.value = 7;
	res = bw_floor2(arg, significance);
	KUNIT_EXPECT_EQ(test, 7, res.value);

	arg.value = MAX_I64;
	significance.value = MAX_I64;
	res = bw_floor2(arg, significance);
	KUNIT_EXPECT_EQ(test, MAX_I64, res.value);

	arg.value = MIN_I64;
	significance.value = MAX_I64;
	res = bw_floor2(arg, significance);
	KUNIT_EXPECT_EQ(test, MIN_I64 + 1, res.value);

	arg.value = MIN_I64;
	significance.value = MIN_I64;
	res = bw_floor2(arg, significance);
	KUNIT_EXPECT_EQ(test, MIN_I64, res.value);

	arg.value = 0;
	significance.value = MAX_I64;
	res = bw_floor2(arg, significance);
	KUNIT_EXPECT_EQ(test, 0, res.value);

	arg.value = 0;
	significance.value = MIN_I64;
	res = bw_floor2(arg, significance);
	KUNIT_EXPECT_EQ(test, 0, res.value);
}

static void bw_ceil2_test(struct kunit *test)
{
	struct bw_fixed arg;
	struct bw_fixed significance;
	struct bw_fixed res;

	arg.value = 10;
	significance.value = 3;
	res = bw_ceil2(arg, significance);
	KUNIT_EXPECT_EQ(test, 12, res.value);

	arg.value = 10;
	significance.value = 5;
	res = bw_ceil2(arg, significance);
	KUNIT_EXPECT_EQ(test, 10, res.value);

	arg.value = 10;
	significance.value = 7;
	res = bw_ceil2(arg, significance);
	KUNIT_EXPECT_EQ(test, 14, res.value);

	arg.value = MAX_I64;
	significance.value = MAX_I64;
	res = bw_ceil2(arg, significance);
	KUNIT_EXPECT_EQ(test, MAX_I64, res.value);

	arg.value = MAX_I64;
	significance.value = MIN_I64 + 1;
	res = bw_ceil2(arg, significance);
	KUNIT_EXPECT_EQ(test, MAX_I64, res.value);

	arg.value = MIN_I64 + 1;
	significance.value = MIN_I64 + 1;
	res = bw_ceil2(arg, significance);
	KUNIT_EXPECT_EQ(test, MIN_I64 + 1, res.value);

	arg.value = 0;
	significance.value = MAX_I64;
	res = bw_ceil2(arg, significance);
	KUNIT_EXPECT_EQ(test, 0, res.value);

	arg.value = 0;
	significance.value = MIN_I64;
	res = bw_ceil2(arg, significance);
	KUNIT_EXPECT_EQ(test, 0, res.value);
}

static struct kunit_case dml_calcs_bw_fixed_test_cases[] = {
	KUNIT_CASE(abs_i64_test),
	KUNIT_CASE(bw_int_to_fixed_nonconst_test),
	KUNIT_CASE(bw_floor2_test),
	KUNIT_CASE(bw_ceil2_test),
	KUNIT_CASE(bw_frc_to_fixed_test),
	{  }
};

static struct kunit_suite dml_calcs_bw_fixed_test_suite = {
	.name = "dml-calcs-bw-fixed",
	.test_cases = dml_calcs_bw_fixed_test_cases,
};

kunit_test_suite(dml_calcs_bw_fixed_test_suite);
