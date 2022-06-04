// SPDX-License-Identifier: MIT
/*
 * KUnit tests for dml/display_mode_lib.h
 *
 * Copyright (C) 2022, Maíra Canal <mairacanal@riseup.net>
 */

#include <kunit/test.h>
#include "../../dc/dml/display_mode_lib.h"
#include "../../dc/dml/display_mode_enums.h"

/**
 * DOC: Unit tests for AMDGPU DML display_mode_lib.h
 *
 * The display_mode_lib.h holds the functions responsible for the instantiation
 * and logging of the Display Mode Library (DML).
 *
 * These KUnit tests were implemented with the intention of assuring the proper
 * logging of the DML.
 *
 */

static void dml_get_status_message_test(struct kunit *test)
{
	KUNIT_EXPECT_STREQ(test, dml_get_status_message(DML_VALIDATION_OK), "Validation OK");
	KUNIT_EXPECT_STREQ(test, dml_get_status_message(DML_FAIL_SCALE_RATIO_TAP), "Scale ratio/tap");
	KUNIT_EXPECT_STREQ(test, dml_get_status_message(DML_FAIL_SOURCE_PIXEL_FORMAT), "Source pixel format");
	KUNIT_EXPECT_STREQ(test, dml_get_status_message(DML_FAIL_VIEWPORT_SIZE), "Viewport size");
	KUNIT_EXPECT_STREQ(test, dml_get_status_message(DML_FAIL_TOTAL_V_ACTIVE_BW), "Total vertical active bandwidth");
	KUNIT_EXPECT_STREQ(test, dml_get_status_message(DML_FAIL_DIO_SUPPORT), "DIO support");
	KUNIT_EXPECT_STREQ(test, dml_get_status_message(DML_FAIL_NOT_ENOUGH_DSC), "Not enough DSC Units");
	KUNIT_EXPECT_STREQ(test, dml_get_status_message(DML_FAIL_DSC_CLK_REQUIRED), "DSC clock required");
	KUNIT_EXPECT_STREQ(test, dml_get_status_message(DML_FAIL_URGENT_LATENCY), "Urgent latency");
	KUNIT_EXPECT_STREQ(test, dml_get_status_message(DML_FAIL_REORDERING_BUFFER), "Re-ordering buffer");
	KUNIT_EXPECT_STREQ(test, dml_get_status_message(DML_FAIL_DISPCLK_DPPCLK), "Dispclk and Dppclk");
	KUNIT_EXPECT_STREQ(test, dml_get_status_message(DML_FAIL_TOTAL_AVAILABLE_PIPES), "Total available pipes");
	KUNIT_EXPECT_STREQ(test, dml_get_status_message(DML_FAIL_NUM_OTG), "Number of OTG");
	KUNIT_EXPECT_STREQ(test, dml_get_status_message(DML_FAIL_WRITEBACK_MODE), "Writeback mode");
	KUNIT_EXPECT_STREQ(test, dml_get_status_message(DML_FAIL_WRITEBACK_LATENCY), "Writeback latency");
	KUNIT_EXPECT_STREQ(test, dml_get_status_message(DML_FAIL_WRITEBACK_SCALE_RATIO_TAP), "Writeback scale ratio/tap");
	KUNIT_EXPECT_STREQ(test, dml_get_status_message(DML_FAIL_CURSOR_SUPPORT), "Cursor support");
	KUNIT_EXPECT_STREQ(test, dml_get_status_message(DML_FAIL_PITCH_SUPPORT), "Pitch support");
	KUNIT_EXPECT_STREQ(test, dml_get_status_message(DML_FAIL_PTE_BUFFER_SIZE), "PTE buffer size");
	KUNIT_EXPECT_STREQ(test, dml_get_status_message(DML_FAIL_DSC_INPUT_BPC), "DSC input bpc");
	KUNIT_EXPECT_STREQ(test, dml_get_status_message(DML_FAIL_PREFETCH_SUPPORT), "Prefetch support");
	KUNIT_EXPECT_STREQ(test, dml_get_status_message(DML_FAIL_V_RATIO_PREFETCH), "Vertical ratio prefetch");
	KUNIT_EXPECT_STREQ(test, dml_get_status_message(-1), "Unknown Status");
}

static struct kunit_case display_mode_lib_test_cases[] = {
	KUNIT_CASE(dml_get_status_message_test),
	{  }
};

static struct kunit_suite display_mode_lib_test_suite = {
	.name = "dml_display_mode_lib",
	.test_cases = display_mode_lib_test_cases,
};

kunit_test_suite(display_mode_lib_test_suite);
