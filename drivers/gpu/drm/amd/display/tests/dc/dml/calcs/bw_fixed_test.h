/* SPDX-License-Identifier: MIT */
/* KUnit tests for dml/calcs/bw_fixed.h
 *
 * Copyright (C) 2022, Magali Lemes <magalilemes00@gmail.com>
 * Copyright (C) 2022, Maíra Canal <mairacanal@riseup.net>
 * Copyright (C) 2022, Tales Aparecida <tales.aparecida@gmail.com>
 */

#include <kunit/test.h>

void abs_i64_test(struct kunit *test);
void bw_int_to_fixed_nonconst_test(struct kunit *test);
void bw_frc_to_fixed_test(struct kunit *test);
void bw_floor2_test(struct kunit *test);
void bw_ceil2_test(struct kunit *test);
void bw_mul_test(struct kunit *test);
