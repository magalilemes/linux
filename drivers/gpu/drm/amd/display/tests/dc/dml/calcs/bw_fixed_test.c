// SPDX-License-Identifier: MIT
/*
 * KUnit tests for dml/calcs/bw_fixed.h
 *
 * Copyright (C) 2022, Magali Lemes <magalilemes00@gmail.com>
 * Copyright (C) 2022, Maíra Canal <mairacanal@riseup.net>
 * Copyright (C) 2022, Tales Aparecida <tales.aparecida@gmail.com>
 */

#include <kunit/test.h>
#include <drm/drm_util.h>
#include "bw_fixed.h"
#include "bw_fixed_test.h"

/**
 * DOC: Unit tests for AMDGPU DML calcs/bw_fixed.h
 *
 * bw_fixed.h performs a lot of the mathematical operations involving
 * fixed-point arithmetic and the conversion of integers to fixed-point
 * representation.
 *
 * As fixed-point representation is the base foundation of the DML calcs
 * operations, these tests intend to assure the proper functioning of the
 * basic mathematical operations of fixed-point arithmetic, such as
 * multiplication, conversion from fractional to fixed-point number, and more.
 *
 */

/**
 * abs_i64_test - KUnit test for abs_i64
 * @test: represents a running instance of a test.
 */
void abs_i64_test(struct kunit *test)
{
	KUNIT_EXPECT_EQ(test, 0ULL, abs_i64(0LL));

	/* Argument type limits */
	KUNIT_EXPECT_EQ(test, (uint64_t)MAX_I64, abs_i64(MAX_I64));
	KUNIT_EXPECT_EQ(test, (uint64_t)MAX_I64 + 1, abs_i64(MIN_I64));
}
EXPORT_SYMBOL_FOR_TESTS_ONLY(abs_i64_test);

/**
 * bw_int_to_fixed_nonconst_test - KUnit test for bw_int_to_fixed_nonconst
 * @test: represents a running instance of a test.
 */
void bw_int_to_fixed_nonconst_test(struct kunit *test)
{
	struct bw_fixed res;

	/* Add BW_FIXED_BITS_PER_FRACTIONAL_PART trailing 0s to binary number */
	res = bw_int_to_fixed_nonconst(1000);          /* 0x3E8 */
	KUNIT_EXPECT_EQ(test, 16777216000, res.value); /* 0x3E8000000 */

	res = bw_int_to_fixed_nonconst(-1000);          /* -0x3E8 */
	KUNIT_EXPECT_EQ(test, -16777216000, res.value); /* -0x3E8000000 */

	res = bw_int_to_fixed_nonconst(0LL);
	KUNIT_EXPECT_EQ(test, 0, res.value);

	/**
	 * Test corner cases, as the function's argument has to be an int64_t
	 * between BW_FIXED_MIN_I32 and BW_FIXED_MAX_I32.
	 */
	res = bw_int_to_fixed_nonconst(BW_FIXED_MAX_I32 - 1);  /* 0x7FFFFFFFFE */
	KUNIT_EXPECT_EQ(test, 9223372036821221376, res.value); /* 0x7FFFFFFFFE000000 */

	res = bw_int_to_fixed_nonconst(BW_FIXED_MIN_I32 + 1);   /* -0x7FFFFFFFFF */
	KUNIT_EXPECT_EQ(test, -9223372036837998592, res.value); /* -0x7FFFFFFFFF000000 */
}
EXPORT_SYMBOL_FOR_TESTS_ONLY(bw_int_to_fixed_nonconst_test);

/**
 * bw_frc_to_fixed_test - KUnit test for bw_frc_to_fixed
 * @test: represents a running instance of a test.
 */
void bw_frc_to_fixed_test(struct kunit *test)
{
	struct bw_fixed res;

	/* Extreme scenarios */

	/* A fraction of N/N should result in "1.0" */
	res = bw_frc_to_fixed(MAX_I64, MAX_I64);
	KUNIT_EXPECT_EQ(test, 1LL << BW_FIXED_BITS_PER_FRACTIONAL_PART, res.value);

	res = bw_frc_to_fixed(1, MAX_I64);
	KUNIT_EXPECT_EQ(test, 0LL, res.value);

	res = bw_frc_to_fixed(0, MAX_I64);
	KUNIT_EXPECT_EQ(test, 0LL, res.value);

	/* Turn a repeating decimal to the fixed-point representation */

	/* A repeating decimal that doesn't round up the LSB */
	res = bw_frc_to_fixed(4, 3);
	KUNIT_EXPECT_EQ(test, 22369621LL, res.value);     /* 0x1555555 */

	res = bw_frc_to_fixed(-4, 3);
	KUNIT_EXPECT_EQ(test, -22369621LL, res.value);    /* -0x1555555 */

	res = bw_frc_to_fixed(99999997, 100000000);
	KUNIT_EXPECT_EQ(test, 16777215LL, res.value);     /* 0x0FFFFFF */

	/* A repeating decimal that rounds up the MSB */
	res = bw_frc_to_fixed(5, 3);
	KUNIT_EXPECT_EQ(test, 27962027LL, res.value);     /* 0x1AAAAAB */

	res = bw_frc_to_fixed(-5, 3);
	KUNIT_EXPECT_EQ(test, -27962027LL, res.value);    /* -0x1AAAAAB */

	res = bw_frc_to_fixed(99999998, 100000000);
	KUNIT_EXPECT_EQ(test, 1LL << BW_FIXED_BITS_PER_FRACTIONAL_PART, res.value);

	/* Turn a terminating decimal to the fixed-point representation */
	res = bw_frc_to_fixed(62609, 100);
	KUNIT_EXPECT_EQ(test, 10504047165LL, res.value);  /* 0X272170A3D */

	res = bw_frc_to_fixed(-62609, 100);
	KUNIT_EXPECT_EQ(test, -10504047165LL, res.value); /* -0X272170A3D */
}
EXPORT_SYMBOL_FOR_TESTS_ONLY(bw_frc_to_fixed_test);

/**
 * bw_floor2_test - KUnit test for bw_floor2
 * @test: represents a running instance of a test.
 */
void bw_floor2_test(struct kunit *test)
{
	struct bw_fixed arg;
	struct bw_fixed significance;
	struct bw_fixed res;

	/* Round 10 down to the nearest multiple of 3 */
	arg.value = 10;
	significance.value = 3;
	res = bw_floor2(arg, significance);
	KUNIT_EXPECT_EQ(test, 9, res.value);

	/* Round 10 down to the nearest multiple of 5 */
	arg.value = 10;
	significance.value = 5;
	res = bw_floor2(arg, significance);
	KUNIT_EXPECT_EQ(test, 10, res.value);

	/* Round 100 down to the nearest multiple of 7 */
	arg.value = 100;
	significance.value = 7;
	res = bw_floor2(arg, significance);
	KUNIT_EXPECT_EQ(test, 98, res.value);

	/* Round an integer down to its nearest multiple should return itself */
	arg.value = MAX_I64;
	significance.value = MAX_I64;
	res = bw_floor2(arg, significance);
	KUNIT_EXPECT_EQ(test, MAX_I64, res.value);

	arg.value = MIN_I64;
	significance.value = MIN_I64;
	res = bw_floor2(arg, significance);
	KUNIT_EXPECT_EQ(test, MIN_I64, res.value);

	/* Value is a multiple of significance, result should be value */
	arg.value = MAX_I64;
	significance.value = MIN_I64 + 1;
	res = bw_floor2(arg, significance);
	KUNIT_EXPECT_EQ(test, MAX_I64, res.value);

	/* Round 0 down to the nearest multiple of any number should return 0 */
	arg.value = 0;
	significance.value = MAX_I64;
	res = bw_floor2(arg, significance);
	KUNIT_EXPECT_EQ(test, 0, res.value);

	arg.value = 0;
	significance.value = MIN_I64;
	res = bw_floor2(arg, significance);
	KUNIT_EXPECT_EQ(test, 0, res.value);
}
EXPORT_SYMBOL_FOR_TESTS_ONLY(bw_floor2_test);

/**
 * bw_ceil2_test - KUnit test for bw_ceil2
 * @test: represents a running instance of a test.
 */
void bw_ceil2_test(struct kunit *test)
{
	struct bw_fixed arg;
	struct bw_fixed significance;
	struct bw_fixed res;

	/* Round 10 up to the nearest multiple of 3 */
	arg.value = 10;
	significance.value = 3;
	res = bw_ceil2(arg, significance);
	KUNIT_EXPECT_EQ(test, 12, res.value);

	/* Round 10 up to the nearest multiple of 5 */
	arg.value = 10;
	significance.value = 5;
	res = bw_ceil2(arg, significance);
	KUNIT_EXPECT_EQ(test, 10, res.value);

	/* Round 100 up to the nearest multiple of 7 */
	arg.value = 100;
	significance.value = 7;
	res = bw_ceil2(arg, significance);
	KUNIT_EXPECT_EQ(test, 105, res.value);

	/* Round an integer up to its nearest multiple should return itself */
	arg.value = MAX_I64;
	significance.value = MAX_I64;
	res = bw_ceil2(arg, significance);
	KUNIT_EXPECT_EQ(test, MAX_I64, res.value);

	arg.value = MIN_I64 + 1;
	significance.value = MIN_I64 + 1;
	res = bw_ceil2(arg, significance);
	KUNIT_EXPECT_EQ(test, MIN_I64 + 1, res.value);

	/* Value is a multiple of significance, result should be value */
	arg.value = MAX_I64;
	significance.value = MIN_I64 + 1;
	res = bw_ceil2(arg, significance);
	KUNIT_EXPECT_EQ(test, MAX_I64, res.value);

	/* Round 0 up to the nearest multiple of any number should return 0 */
	arg.value = 0;
	significance.value = MAX_I64;
	res = bw_ceil2(arg, significance);
	KUNIT_EXPECT_EQ(test, 0, res.value);

	arg.value = 0;
	significance.value = MIN_I64;
	res = bw_ceil2(arg, significance);
	KUNIT_EXPECT_EQ(test, 0, res.value);
}
EXPORT_SYMBOL_FOR_TESTS_ONLY(bw_ceil2_test);

/**
 * bw_mul_test - KUnit test for bw_mul
 * @test: represents a running instance of a test.
 */
void bw_mul_test(struct kunit *test)
{
	struct bw_fixed arg1;
	struct bw_fixed arg2;
	struct bw_fixed res;
	struct bw_fixed ans;

	/* Extreme scenario */
	arg1.value = MAX_I64;
	arg2.value = MIN_I64;
	res = bw_mul(arg1, arg2);
	KUNIT_EXPECT_EQ(test, BW_FIXED_MAX_I32 + 1, res.value);

	/* Testing multiplication property: x * 1 = x */
	arg1.value = 1;
	arg2.value = MAX_I64;
	res = bw_mul(arg1, arg2);
	KUNIT_EXPECT_EQ(test, BW_FIXED_MAX_I32 + 1, res.value);

	arg1.value = 1;
	arg2.value = MIN_I64;
	res = bw_mul(arg1, arg2);
	KUNIT_EXPECT_EQ(test, BW_FIXED_MIN_I32, res.value);

	/* Testing multiplication property: x * 0 = 0 */
	arg1.value = 0;
	arg2.value = 0;
	res = bw_mul(arg1, arg2);
	KUNIT_EXPECT_EQ(test, 0, res.value);

	arg1.value = 0;
	arg2.value = MAX_I64;
	res = bw_mul(arg1, arg2);
	KUNIT_EXPECT_EQ(test, 0, res.value);

	arg1.value = 0;
	arg2.value = MIN_I64;
	res = bw_mul(arg1, arg2);
	KUNIT_EXPECT_EQ(test, 0, res.value);

	/* Testing multiplication between integers */
	res = bw_mul(bw_int_to_fixed(8), bw_int_to_fixed(10));
	KUNIT_EXPECT_EQ(test, 1342177280LL, res.value); /* 0x50000000 */

	res = bw_mul(bw_int_to_fixed(10), bw_int_to_fixed(5));
	KUNIT_EXPECT_EQ(test, 838860800LL, res.value); /* 0x32000000 */

	res = bw_mul(bw_int_to_fixed(-10), bw_int_to_fixed(7));
	KUNIT_EXPECT_EQ(test, -1174405120LL, res.value); /* -0x46000000 */

	/* Testing multiplication between fractions and integers */
	res = bw_mul(bw_frc_to_fixed(4, 3), bw_int_to_fixed(3));
	ans = bw_int_to_fixed(4);

	/*
	 * As bw_frc_to_fixed(4, 3) didn't round up the fixed-point representation,
	 * the ans must be subtracted by 1.
	 */
	KUNIT_EXPECT_EQ(test, ans.value - 1, res.value);

	res = bw_mul(bw_frc_to_fixed(5, 3), bw_int_to_fixed(3));
	ans = bw_int_to_fixed(5);

	/*
	 * As bw_frc_to_fixed(5, 3) rounds up the fixed-point representation,
	 * the ans must be added by 1.
	 */
	KUNIT_EXPECT_EQ(test, ans.value + 1, res.value);
}
EXPORT_SYMBOL_FOR_TESTS_ONLY(bw_mul_test);
