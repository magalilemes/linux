// SPDX-License-Identifier: MIT
/* Unit tests for dml/calcs/bw_fixed.c
 */

#include <kunit/test.h>
#include "../../dc/inc/bw_fixed.h"

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

static struct kunit_case dml_calcs_bw_fixed_test_cases[] = {
	KUNIT_CASE(bw_frc_to_fixed_test),
	{  }
};

static struct kunit_suite dml_calcs_bw_fixed_test_suite = {
	.name = "dml-calcs-bw-fixed",
	.test_cases = dml_calcs_bw_fixed_test_cases,
};

kunit_test_suite(dml_calcs_bw_fixed_test_suite);
