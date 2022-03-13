// SPDX-License-Identifier: MIT
/* Unit tests for dml/calcs/bw_fixed.c
 */

#include <kunit/test.h>

static void abs_i64_test(struct kunit *test)
{
	KUNIT_EXPECT_EQ(test, 0, abs_i64(0));
	KUNIT_EXPECT_EQ(test, (uint64_t)LLONG_MAX, abs_i64(LLONG_MAX));
	KUNIT_EXPECT_EQ(test, (uint64_t)LLONG_MAX + 1, abs_i64(LLONG_MIN));
}

static struct kunit_case dml_calcs_bw_fixed_test_cases[] = {
	KUNIT_CASE(abs_i64_test),
	{}
};

static struct kunit_suite dml_calcs_bw_fixed_test_suite = {
	.name = "dml-calcs-bw-fixed",
	.test_cases = dml_calcs_bw_fixed_test_cases,
};

kunit_test_suite(dml_calcs_bw_fixed_test_suite);
