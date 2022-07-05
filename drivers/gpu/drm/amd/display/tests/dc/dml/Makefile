# SPDX-License-Identifier: MIT
#
# Makefile for the KUnit Tests for DML
#

DML_TESTS = calcs/bw_fixed_test.o display_mode_vba_test.o
AMD_DAL_DML_TESTS = $(addprefix $(AMDDALPATH)/tests/dc/dml/,$(DML_TESTS))
AMD_DISPLAY_FILES += $(AMD_DAL_DML_TESTS)

DML_TEST_MODULES = dml_calcs_tests.o vba_tests.o
AMD_DAL_DML_TEST_MODULES = $(addprefix $(AMDDALPATH)/tests/dc/dml/,$(DML_TEST_MODULES))
obj-$(CONFIG_DML_KUNIT_TEST) += $(AMD_DAL_DML_TEST_MODULES)
