/* SPDX-License-Identifier: MIT */
/* Unit tests for display/include/fixed31_32.h and dc/basics/fixpt31_32.c
 *
 * Copyright (C) 2022, Tales L. Aparecida <tales.aparecida@gmail.com>
 */

#include <kunit/test.h>

void dc_fixpt_from_int_test(struct kunit *test);
void dc_fixpt_from_fraction_test(struct kunit *test);
void dc_fixpt_mul_test(struct kunit *test);
void dc_fixpt_recip_test(struct kunit *test);
void dc_fixpt_sqr_test(struct kunit *test);
