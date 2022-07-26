.. SPDX-License-Identifier: GPL-2.0+

========================
Display Core Unit Tests
========================

Display core provides a set of unit tests, currently focused on the Display Mode
Library. The unit tests use KUnit (Kernel Unit Testing Framework), a common
framework for unit tests within the Linux Kernel.

This section covers the specifics of the tests for the AMDGPU driver. For general
information about KUnit, please refer to Documentation/dev-tools/kunit/start.rst.

How to run the tests?
=====================

In order to facilitate running the test suite, a configuration file is present
in ``drivers/gpu/drm/amd/display/tests/dc/.kunitconfig``. This configuration file
can be used to run the kunit_tool, a Python script (``tools/testing/kunit/kunit.py``)
used to configure, build, exec, parse and run tests.

.. code-block:: bash

	$ ./tools/testing/kunit/kunit.py run --arch=x86_64 \
	    --kunitconfig=drivers/gpu/drm/amd/display/tests

Currently, the Display Core Unit Tests are only supported on x86_64.

Moreover, the tests can also be run on real hardware or in other emulation
environments. To include the Display Core Unit Tests on a deployable kernel,
you might add the following config options to your ``.config``:

.. code-block:: none

	CONFIG_KUNIT=y
	CONFIG_AMDGPU=m
	CONFIG_AMD_DC_BASICS_KUNIT_TEST=y
	CONFIG_DML_KUNIT_TEST=y

Once the kernel is built and installed, you can load the ``amdgpu`` module
to run all tests available.

Also, the tests can be added to the kernel as built-in modules, by adding the
following config options to your ``.config``:

.. code-block:: none

	CONFIG_KUNIT=y
	CONFIG_AMDGPU=y
	CONFIG_AMD_DC_BASICS_KUNIT_TEST=y
	CONFIG_DML_KUNIT_TEST=y

In order to run specific tests, you can check the filter options from KUnit on
Documentation/dev-tools/kunit/kunit-tool.rst.

How to add new tests?
=====================

Tests covering different parts of the Display Core are always welcomed. Adding
a new test is a simple procedure, that consists in creating a unit test file
and adding the following guard to the end of the tested file:

.. code-block:: c

	#ifdef CONFIG_MY_KUNIT_TEST
	#include "my_kunit_test.c"
	#endif

The ``display/tests`` folder replicates the folder hierarchy of the ``display``
folder, so this must be considered while adding new tests.

More information on how to write unit tests with the KUnit API can be provided
on Documentation/dev-tools/kunit/api/test.rst.
