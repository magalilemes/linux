# SPDX-License-Identifier: GPL-2.0-only
#
# Makefile for the KUnit Tests for DML
#

dml_tests_ccflags := -mhard-float -msse

CFLAGS_$(AMDDALPATH)/amdgpu_dm/tests/dcn20/dcn20_fpu_test.o := $(dml_tests_ccflags)

DML_TESTS = calcs/bw_fixed_test.o dcn20/dcn20_fpu_test.o display_mode_lib_test.o

AMD_DAL_DML_TESTS = $(addprefix $(AMDDALPATH)/amdgpu_dm/tests/,$(DML_TESTS))

AMD_DISPLAY_FILES += $(AMD_DAL_DML_TESTS)
