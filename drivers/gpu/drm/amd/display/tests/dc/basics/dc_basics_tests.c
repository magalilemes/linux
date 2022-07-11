// SPDX-License-Identifier: MIT
/* Unit tests for files under dc/basics/
 */

#include <kunit/test.h>
#include "dc_basics_fixpt31_32_test.h"

static struct kunit_case dc_basics_fixpt31_32_test_cases[] = {
	KUNIT_CASE(dc_fixpt_from_int_test),
	KUNIT_CASE(dc_fixpt_from_fraction_test),
	KUNIT_CASE(dc_fixpt_mul_test),
	KUNIT_CASE(dc_fixpt_sqr_test),
	KUNIT_CASE(dc_fixpt_recip_test),
	{  }
};

static struct kunit_suite dc_basics_fixpt31_32_test_suite = {
	.name = "dc_basics_fixpt31_32",
	.test_cases = dc_basics_fixpt31_32_test_cases,
};

kunit_test_suites(&dc_basics_fixpt31_32_test_suite);

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Tales L. Aparecida");
