// SPDX-License-Identifier: MIT
/*
 * KUnit tests for dml/dcn20/display_mode_vba_20.c
 *
 * Copyright (C) 2022, Maíra Canal <mairacanal@riseup.net>
 */

#include <kunit/test.h>
#include "dml/display_mode_enums.h"

/* macros defined at display_mode_vba_20.c */
#ifndef BPP_INVALID
#define BPP_INVALID 0
#endif

/* static functions defined at display_mode_vba_20.c */
extern unsigned int dscComputeDelay(enum output_format_class pixelFormat);
extern double CalculateTWait(unsigned int PrefetchMode,
			     double DRAMClockChangeLatency,
			     double UrgentLatencyPixelDataOnly,
			     double SREnterPlusExitTime);
extern unsigned int TruncToValidBPP(double DecimalBPP, bool DSCEnabled,
				    enum output_encoder_class Output,
				    enum output_format_class Format,
				    unsigned int DSCInputBitPerComponent);

static void DSC_compute_delay_test(struct kunit *test)
{
	/* 4:4:4 encoding */
	KUNIT_EXPECT_EQ(test, dscComputeDelay(dm_444), 30);

	/* 4:2:0 encoding */
	KUNIT_EXPECT_EQ(test, dscComputeDelay(dm_420), 48);

	/* 4:2:2 n-encoding */
	KUNIT_EXPECT_EQ(test, dscComputeDelay(dm_n422), 49);

	/* 4:2:2 s-encoding */
	KUNIT_EXPECT_EQ(test, dscComputeDelay(dm_s422), 30);
}

static void calculate_TWait_test(struct kunit *test)
{
	/* Zeroed Prefetch Mode */

	/* DRAMClockChangeLatency > UrgentLatencyPixelDataOnly > SREnterPlusExitTime*/
	KUNIT_EXPECT_EQ(test, CalculateTWait(0, 1300000, 1200000, 1000000), 2500000);

	/* DRAMClockChangeLatency > SREnterPlusExitTime > UrgentLatencyPixelDataOnly */
	KUNIT_EXPECT_EQ(test, CalculateTWait(0, 1300000, 900000, 1200000), 2200000);

	/* UrgentLatencyPixelDataOnly > DRAMClockChangeLatency > SREnterPlusExitTime */
	KUNIT_EXPECT_EQ(test, CalculateTWait(0, 1000000, 2000000, 900000), 3000000);

	/* UrgentLatencyPixelDataOnly > SREnterPlusExitTime > DRAMClockChangeLatency */
	KUNIT_EXPECT_EQ(test, CalculateTWait(0, 1000000, 1200000, 1100000), 2200000);

	/* SREnterPlusExitTime > DRAMClockChangeLatency > UrgentLatencyPixelDataOnly */
	KUNIT_EXPECT_EQ(test, CalculateTWait(0, 1000000, 900000, 2000000), 2000000);

	/* SREnterPlusExitTime > UrgentLatencyPixelDataOnly > DRAMClockChangeLatency */
	KUNIT_EXPECT_EQ(test, CalculateTWait(0, 1000000, 1200000, 1300000), 2200000);

	/* Prefetch Mode equals 1 */
	KUNIT_EXPECT_EQ(test, CalculateTWait(1, 2500000, 2000000, 900000), 2000000);
	KUNIT_EXPECT_EQ(test, CalculateTWait(1, 1300000, 900000, 1200000), 1200000);
	KUNIT_EXPECT_EQ(test, CalculateTWait(1, 1100000, 1200000, 1000000), 1200000);
	KUNIT_EXPECT_EQ(test, CalculateTWait(1, 1000000, 1200000, 1100000), 1200000);
	KUNIT_EXPECT_EQ(test, CalculateTWait(1, 1000000, 900000, 2000000), 2000000);
	KUNIT_EXPECT_EQ(test, CalculateTWait(1, 1000000, 1200000, 1300000), 1300000);

	/* Prefetch Mode greater than 1 */
	KUNIT_EXPECT_EQ(test, CalculateTWait(1, 2500000, 2000000, 900000), 2000000);
	KUNIT_EXPECT_EQ(test, CalculateTWait(2, 2500000, 900000, 2000000), 900000);
	KUNIT_EXPECT_EQ(test, CalculateTWait(5, 1100000, 1200000, 1000000), 1200000);
	KUNIT_EXPECT_EQ(test, CalculateTWait(4, 1000000, 1300000, 1200000), 1300000);
	KUNIT_EXPECT_EQ(test, CalculateTWait(6, 1000000, 900000, 2000000), 900000);
	KUNIT_EXPECT_EQ(test, CalculateTWait(3, 1000000, 1200000, 1300000), 1200000);
}

static void trunc_to_valid_BPP_test(struct kunit *test)
{
	/* HDMI output for 4:2:0 encoding */
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(18.01, true, dm_hdmi, dm_420, 0), 18);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(18, true, dm_hdmi, dm_420, 10), 18);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(15.01, false, dm_hdmi, dm_420, 0), 15);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(15, false, dm_hdmi, dm_420, 8), 15);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(12.01, true, dm_hdmi, dm_420, 0), 12);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(12, true, dm_hdmi, dm_420, 5), 12);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(11.99, false, dm_hdmi, dm_420, 5), BPP_INVALID);

	/* HDMI output for 4:2:0 encoding */
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(36.01, true, dm_hdmi, dm_444, 0), 36);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(36, true, dm_hdmi, dm_444, 10), 36);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(30.01, false, dm_hdmi, dm_444, 0), 30);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(30, false, dm_hdmi, dm_444, 8), 30);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(24.01, true, dm_hdmi, dm_444, 0), 24);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(24, true, dm_hdmi, dm_444, 5), 24);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(18.01, true, dm_hdmi, dm_444, 0), 18);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(18, true, dm_hdmi, dm_444, 5), 18);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(17.99, false, dm_hdmi, dm_444, 5), BPP_INVALID);

	/* HDMI output for different encoding types */
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(36.01, true, dm_hdmi, dm_n422, 0), 24);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(36, true, dm_hdmi, dm_s422, 10), 24);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(30.01, false, dm_hdmi, dm_n422, 0), 20);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(30, false, dm_hdmi, dm_s422, 8), 20);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(24.01, true, dm_hdmi, dm_n422, 0), 16);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(24, true, dm_hdmi, dm_s422, 5), 16);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(15.99, false, dm_hdmi, dm_n422, 5), BPP_INVALID);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(-1, true, dm_hdmi, dm_s422, 0), BPP_INVALID);

	/* Display Port output with DSC enabled and 4:2:0 encoding */
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(5.99, true, dm_dp, dm_420, 0), BPP_INVALID);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(-1, true, dm_dp, dm_420, 0), BPP_INVALID);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(12, true, dm_dp, dm_420, 8), 12);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(1.5 * 8 - 1 / (double) 16, true, dm_dp, dm_420, 8), 11);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(7, true, dm_dp, dm_420, 3), 4);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(1.5 * 9 - 1 / (double) 16, true, dm_dp, dm_420, 9), 13);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(6, true, dm_dp, dm_420, 8), 6);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(6.25, true, dm_dp, dm_420, 8), 6);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(6 + 1 / (double) 3, true, dm_dp, dm_420, 8), 6);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(6.50, true, dm_dp, dm_420, 8), 6);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(6.75, true, dm_dp, dm_420, 8), 6);

	/* Embedded Display Port output with DSC enabled and n-4:2:2 encoding */
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(6.99, true, dm_edp, dm_n422, 0), BPP_INVALID);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(-1, true, dm_edp, dm_n422, 0), BPP_INVALID);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(16, true, dm_edp, dm_n422, 7), 14);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(2 * 7 - 1 / (double) 16, true, dm_edp, dm_n422, 7), 13);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(7, true, dm_edp, dm_n422, 3), 6);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(2 * 9 - 1 / (double) 16, true, dm_edp, dm_n422, 9), 17);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(11, true, dm_edp, dm_n422, 8), 11);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(11 + 1 / (double) 3, true, dm_edp, dm_n422, 8), 11);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(11.40, true, dm_edp, dm_n422, 8), 11);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(11.50, true, dm_edp, dm_n422, 8), 11);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(11.75, true, dm_edp, dm_n422, 8), 11);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(11.95, true, dm_edp, dm_n422, 8), 11);

	/* Display Port 2.0 output with DSC enabled and 4:4:4 encoding */
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(7.99, true, dm_dp2p0, dm_444, 0), BPP_INVALID);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(-1, true, dm_dp2p0, dm_444, 0), BPP_INVALID);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(22, true, dm_dp2p0, dm_444, 11), 22);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(3 * 7 - 1 / (double) 16, true, dm_dp2p0, dm_444, 7), 20);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(14, true, dm_dp2p0, dm_444, 3), 9);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(3 * 9 - 1 / (double) 16, true, dm_dp2p0, dm_444, 9), 26);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(17, true, dm_dp2p0, dm_444, 7), 17);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(17 + 1 / (double) 3, true, dm_dp2p0, dm_444, 7), 17);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(17.40, true, dm_dp2p0, dm_444, 7), 17);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(17.50, true, dm_dp2p0, dm_444, 7), 17);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(17.75, true, dm_dp2p0, dm_444, 7), 17);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(17.95, true, dm_dp2p0, dm_444, 7), 17);

	/* WB output with DSC enabled and 4:2:2 s-encoding */
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(7.99, true, dm_wb, dm_s422, 0), BPP_INVALID);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(-1, true, dm_wb, dm_s422, 0), BPP_INVALID);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(22, true, dm_wb, dm_s422, 11), 22);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(3 * 7 - 1 / (double) 16, true, dm_wb, dm_s422, 7), 20);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(18, true, dm_wb, dm_s422, 3), 9);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(3 * 9 - 1 / (double) 16, true, dm_wb, dm_s422, 9), 26);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(17, true, dm_wb, dm_s422, 7), 17);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(17 + 1 / (double) 3, true, dm_wb, dm_s422, 7), 17);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(17.40, true, dm_wb, dm_s422, 7), 17);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(17.50, true, dm_wb, dm_s422, 7), 17);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(17.75, true, dm_wb, dm_s422, 7), 17);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(17.95, true, dm_wb, dm_s422, 7), 17);

	/* Display Port output with DSC disabled for 4:2:0 encoding */
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(18.01, false, dm_dp, dm_420, 0), 18);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(18, false, dm_dp, dm_420, 10), 18);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(15.01, false, dm_dp, dm_420, 0), 15);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(15, false, dm_dp, dm_420, 8), 15);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(12.01, false, dm_dp, dm_420, 0), 12);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(12, false, dm_dp, dm_420, 5), 12);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(11.99, false, dm_dp, dm_420, 5), BPP_INVALID);

	/* Embedded Display Port output with DSC disabled for 4:2:2 encoding */
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(24.01, false, dm_edp, dm_s422, 0), 24);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(24, false, dm_edp, dm_n422, 10), 24);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(20.01, false, dm_edp, dm_s422, 0), 20);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(20, false, dm_edp, dm_n422, 8), 20);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(16.01, false, dm_edp, dm_s422, 0), 16);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(16, false, dm_edp, dm_n422, 5), 16);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(15.99, false, dm_edp, dm_s422, 5), BPP_INVALID);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(-1, false, dm_edp, dm_n422, 5), BPP_INVALID);

	/* WB output with DSC disabled for 4:4:4 encoding */
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(36.01, false, dm_wb, dm_444, 0), 36);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(36, false, dm_wb, dm_444, 10), 36);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(30.01, false, dm_wb, dm_444, 0), 30);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(30, false, dm_wb, dm_444, 8), 30);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(24.01, false, dm_wb, dm_444, 0), 24);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(24, false, dm_wb, dm_444, 5), 24);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(18.01, false, dm_wb, dm_444, 0), 18);
	KUNIT_EXPECT_EQ(test, TruncToValidBPP(18, false, dm_wb, dm_444, 5), 18);

	KUNIT_EXPECT_EQ(test, TruncToValidBPP(17.99, false, dm_wb, dm_444, 5), BPP_INVALID);
}

static struct kunit_case display_mode_vba_20_cases[] = {
	KUNIT_CASE(DSC_compute_delay_test),
	KUNIT_CASE(calculate_TWait_test),
	KUNIT_CASE(trunc_to_valid_BPP_test),
	{  }
};

static struct kunit_suite display_mode_vba_20_suite = {
	.name = "display_mode_vba_20",
	.test_cases = display_mode_vba_20_cases,
};

kunit_test_suite(display_mode_vba_20_suite);
MODULE_LICENSE("Dual MIT/GPL");
