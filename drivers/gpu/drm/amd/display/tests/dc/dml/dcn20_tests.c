// SPDX-License-Identifier: MIT
/* KUnit tests for dml/dcn20/
 */

#include <kunit/test.h>
#include "dcn20/dcn20_fpu_test.h"

static void dcn20_cap_soc_clocks_test_to_desc(struct dcn20_cap_soc_clocks_test_case *t, char *desc)
{
	strcpy(desc, t->desc);
}

static void dcn21_update_bw_bounding_box_test_to_desc(struct dcn21_update_bw_bounding_box_test_case *t, char *desc)
{
	strcpy(desc, t->desc);
}

KUNIT_ARRAY_PARAM(dcn20_cap_soc_clocks,
	dcn20_cap_soc_clocks_test_cases,
	dcn20_cap_soc_clocks_test_to_desc);

static struct kunit_case dcn20_fpu_test_cases[] = {
	KUNIT_CASE_PARAM(dcn20_cap_soc_clocks_test, dcn20_cap_soc_clocks_gen_params),
	{  }
};

static struct kunit_suite dcn20_fpu_test_suite = {
	.name = "dml_dcn20_fpu",
	.test_cases = dcn20_fpu_test_cases,
};

KUNIT_ARRAY_PARAM(dcn21_update_bw_bounding_box,
	dcn21_update_bw_bounding_box_test_data,
	dcn21_update_bw_bounding_box_test_to_desc);

static struct kunit_case dcn20_fpu_dcn21_update_bw_bounding_box_test_cases[] = {
	KUNIT_CASE_PARAM(dcn21_update_bw_bounding_box_test, dcn21_update_bw_bounding_box_gen_params),
	{  }
};

static struct kunit_suite dcn21_update_bw_bounding_box_test_suite = {
	.name = "dml_dcn20_fpu_dcn21_update_bw_bounding_box_test",
	.init = dcn20_fpu_dcn21_update_bw_bounding_box_test_init,
	.exit = dcn20_fpu_dcn21_update_bw_bounding_box_test_exit,
	.test_cases = dcn20_fpu_dcn21_update_bw_bounding_box_test_cases,
};

kunit_test_suites(&dcn20_fpu_test_suite, &dcn21_update_bw_bounding_box_test_suite);

MODULE_LICENSE("Dual MIT/GPL");
