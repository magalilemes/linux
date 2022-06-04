// SPDX-License-Identifier: MIT
/* KUnit tests for dml/calcs/
 */

#include <kunit/test.h>
#include "calcs/bw_fixed_test.h"

static struct kunit_case bw_fixed_test_cases[] = {
	KUNIT_CASE(abs_i64_test),
	KUNIT_CASE(bw_int_to_fixed_nonconst_test),
	KUNIT_CASE(bw_frc_to_fixed_test),
	KUNIT_CASE(bw_floor2_test),
	KUNIT_CASE(bw_ceil2_test),
	KUNIT_CASE(bw_mul_test),
	{  }
};

static struct kunit_suite bw_fixed_test_suite = {
	.name = "dml_calcs_bw_fixed",
	.test_cases = bw_fixed_test_cases,
};

kunit_test_suites(&bw_fixed_test_suite);

MODULE_LICENSE("Dual MIT/GPL");
