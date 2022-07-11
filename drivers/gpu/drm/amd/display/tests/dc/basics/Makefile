# SPDX-License-Identifier: MIT
#
# Makefile for the KUnit Tests for the 'utils' sub-component of DAL
#

# Build the test functions as part of the driver to enable exporting symbols...
BASICS_TESTS = dc_basics_fixpt31_32_test.o
AMD_DAL_BASICS_TESTS = $(addprefix $(AMDDALPATH)/tests/dc/basics/,$(BASICS_TESTS))
AMD_DISPLAY_FILES += $(AMD_DAL_BASICS_TESTS)

# ...to the test suites, which can be compiled as built-in or module.
obj-$(CONFIG_AMD_DAL_BASICS_KUNIT_TEST) += $(AMDDALPATH)/tests/dc/basics/dc_basics_tests.o
